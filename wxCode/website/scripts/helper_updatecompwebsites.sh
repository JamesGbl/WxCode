#!/usr/bin/php -q
<?php

    // Author: Francesco Montorsi
    // Purpose: connects the wxCode DB and updates the component websites
    //          NOTE: this script is written in PHP so that we can use
    //          1) the better PHP support for MySQL connections
    //          2) the dbconnection.inc.php file which already contains the info required
    //             for DB connection (like user & password) so that we do not have to duplicate
    //             them here...

    require("../dbconnection.inc.php");

    // constants:
    define("CVSROOT", ':pserver:anonymous@wxcode.cvs:/cvsroot/wxcode');
    define("SVNROOT", 'https://wxcode.svn.sourceforge.net/svnroot/wxcode/trunk/wxCode');
    define("COMP_WEBSITE", '/home/project-web/wxcode/htdocs/components');

    echo "Updating the component websites...";

    // connect to the DB
    global $dbconn;
    db_connect();

    // query all approved components
    $res = mysql_query("select * from components where approved='1'");
    if ($res == FALSE) {
        echo "Error while querying the database !\n";
        exit;
    }

    $n = mysql_num_rows($res);
    echo " there are $n components to update...\n";

    // do we have maintained components ?
    if ($n == 0) {
        // no, we don't
        exit;
    }

    $count=1;
    while ($row = mysql_fetch_assoc($res)) {

        echo "#$count: Updating $row[name] (from ";
        if ($row['inCVS'])
            echo "CVS";
        else
            echo "Subversion";
        echo ")...\n";

        chdir(COMP_WEBSITE);
        $weblocation = COMP_WEBSITE . "/$row[location]";

        if (!is_dir($weblocation)) {
            echo "   ...ops: $weblocation does not exist - checking it out...\n";

            // checkout this component website into the relative folder
            exec("svn -q checkout -q --non-interactive " . SVNROOT .
                 "/components/$row[location]/website $row[location]");

        } else {

            echo "   ...the component's website folder already exists - updating it...\n";
            chdir($weblocation);

            // checkout this component website into the relative folder
            exec("svn -q update -q");
        }

        $count++;
    }
?>