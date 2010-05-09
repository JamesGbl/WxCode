#!/usr/bin/php -q
<?php

    // Author: Francesco Montorsi
    // Purpose: connects the wxCode DB and sends a message to each wxCode
    //          maintainer about its component & their status.
    //          NOTE: this script is written in PHP so that we can use
    //          1) the better PHP support for MySQL connections
    //          2) the dbconnection.inc.php file which already contains the info required
    //             for DB connection (like user & password) so that we do not have to duplicate
    //             them here...

    require("../dbconnection.inc.php");

    // constants:
    define("CVSROOT", ':pserver:anonymous@wxcode.cvs:/cvsroot/wxcode');
    define("SVNROOT", 'https://wxcode.svn.sourceforge.net/svnroot/wxcode/trunk/wxCode');
    define("TEMP_DIR", '/tmp/wxCode');

    /**
    * Recursively removes a directory
    */
    function rmdirRecursive($path, $followLinks=false)
    {
        if (!is_dir($path))
            return false;

        $dir = opendir($path);

        while ( $entry = readdir($dir) ) {
            if ( is_file( "$path/$entry" ) || ((!$followLinks) && is_link("$path/$entry")) )
                unlink( "$path/$entry" );
            elseif ( is_dir( "$path/$entry" ) && $entry!='.' && $entry!='..' )
                rmdirRecursive( "$path/$entry" );
        }
        closedir($dir);

        return rmdir($path);
    }

    /**
    * Recursive version of glob
    *
    * @return array containing all pattern-matched files.
    *
    * @param string $sDir      Directory to start with.
    * @param string $sPattern  Pattern to glob for.
    * @param int $nFlags      Flags sent to glob.
    */
    function rglob($sDir, $sPattern, $nFlags = NULL)
    {
        $sDir = escapeshellcmd($sDir);

        // Get the list of all matching files currently in the
        // directory.

        $aFiles = glob("$sDir/$sPattern", $nFlags);

        // Then get a list of all directories in this directory, and
        // run ourselves on the resulting array.  This is the
        // recursion step, which will not execute if there are no
        // directories.

        foreach (glob("$sDir/*", GLOB_ONLYDIR) as $sSubDir)
        {
            $aSubFiles = rglob($sSubDir, $sPattern, $nFlags);
            $aFiles = array_merge($aFiles, $aSubFiles);
        }

        // The array we return contains the files we found, and the
        // files all of our children found.

        return $aFiles;
    }

    /**
    * sends a mail unless --dry-run is given to the command line
    */
    function sendMail($dest, $subject, $body)
    {
        global $argv;

        if (in_array("--dry-run", $argv)) {
            echo " => I'd write a mail to $dest if --dry-run was not given.\n";
            return;
        }

        echo " => writing a mail to $dest with the following body:\n";
        echo "----------------------------------------------------------------\n";
        echo "$body";
        echo "----------------------------------------------------------------\n";

        mail($dest, $subject, $body);
    }

    // be sure to cd into a clean temp dir
    rmdirRecursive(TEMP_DIR);
    mkdir(TEMP_DIR);
    chdir(TEMP_DIR);

    // connect to the DB
    global $dbconn;
    db_connect();

    // query all approved and maintained components
    $res = mysql_query("select * from components where maintainerid!='0' and approved='1'");
    if ($res == FALSE) {
        echo "Error while querying the database !\n";
        exit;
    }

    $n = mysql_num_rows($res);
    echo "I've found that there are $n maintained components...\n";

    // do we have maintained components ?
    if ($n == 0) {
        // no, we don't
        exit;
    }

    $emptycomplist="";
    $count=0;

    // send a mail to the maintainer
    while ($row = mysql_fetch_assoc($res)) {

        echo "Looking at the repo of component $row[location]... ";

        // be sure we can find a maintainer for such component
        $res2 = mysql_query("select * from maintainers where id='$row[maintainerid]'");
        if ($res2 == FALSE || mysql_num_rows($res2) != 1) {
            echo "Could not find a maintainer for component $row[name] !\n";
            exit;
        }
        $maintainer = mysql_fetch_assoc($res2);

        // now, checkout this component to a temporary dir called "TEMP_DIR/comp":
        if ($row['inCVS'] == '1')
            exec("cvs -q -d" . CVSROOT . " checkout -d comp wxCode/components/" .
                "$row[location]" . " 2>/dev/null");
        else
            exec("svn -q checkout -q --non-interactive " . SVNROOT . "/components/" .
                 "$row[location]" . " comp 2>/dev/null");

        // then, see if the "comp" dir has been correctly created:
        if (!is_dir("comp")) {
            echo " it's not in CVS nor in SVN!!\n";
            continue;  // skip it
        }

        // does this component contains any source file?
        $output = rglob("comp", "*.cpp");
        if (count($output) == 0) {

            echo "it does NOT contain source files!\n";

            // no source files!!
            // send a reminder
            $dest = $maintainer['mail'];
            $body = "Hi $maintainer[name],\n";
            $body .= "This is an automated message sent by wxCode project (http://wxcode.sourceforge.net) ";
            $body .= "because you are registered as the wxCode maintainer for the $row[name] component.\n\n";
            $body .= "It looks like you haven't uploaded yet any source file to the repository:\n";

            if ($row['inCVS'])
                $body .= "  http://wxcode.cvs.sourceforge.net/wxcode/wxCode/components/$row[location]";
            else
                $body .= "  http://wxcode.svn.sourceforge.net/viewvc/wxcode/trunk/wxCode/components/$row[location]";

            $body .= "\n\nPlease upload sources as soon as possible.\nFor help on how to setup the ";
            $body .= "CVS software and upload the sources, please refer to:\n";
            $body .= "   http://wxcode.sourceforge.net/maintguide.php\n\n";
            $body .= "Thanks!\nThe wxCode project administrators\n\n";

            sendMail($dest, "$row[name] maintainance", $body);

            $emptycomplist .= "$row[name], maintainer: $maintainer[name]\n";
            $count++;

        } else
            echo "it contains source files (OK)\n";

        // remove the "comp" folder to prepare it for next checkout
        rmdirRecursive("comp");

        // don't load too much the server with lots of continuos checkouts:
        //sleep(1);
    }

    // last, send a mail to wxcode-users
    if ($count > 0)
        sendMail("wxcode-users@lists.sourceforge.net",
                 "Cron job: there are $count empty components",
                 "[automated message for informative purposes].\n\n" .
                 "From an heuristic check it results that $count components have no sources in the CVS/SVN; " .
                 "here follows the list of these components and the relative maintainers:\n" .
                 "$emptycomplist" .
                 "\nThe relative maintainers have already been notified by mail of this issue.\n\n" .
                 "Your friendly wxCode script.\n");

    // remove wxCode temp dir
    chdir("/");
    rmdirRecursive(TEMP_DIR);
?>