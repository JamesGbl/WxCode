#!/usr/bin/php -q
<?php

    // Author: Francesco Montorsi
    // RCS-ID: $Id: maint_approvecomp.sh,v 1.2 2009/01/05 15:13:28 frm Exp $
    // Purpose: set the given component as "approved"
    //
    //          NOTE: this script is written in PHP so that we can use
    //          1) the better PHP support for MySQL connections
    //          2) the dbconnection.inc.php file which already contains the info required
    //             for DB connection (like user & password) so that we do not have to duplicate
    //             them here...

    require("../dbconnection.inc.php");

    if (!isset($argv[1])) {

        echo "Usage: ./maint_approvecomp.sh component_name_in_lowercase\n";
        exit;
    }

    // connect and ask the DB for not-approved components
    global $dbconn;
    db_connect_fromscript();
    $res = mysql_query("SELECT * FROM components WHERE approved='0' AND name='" . $argv[1] . "'");
    if ($res == FALSE) {
        echo "Error while querying the database !\n";
        exit;
    }

    $num = mysql_num_rows($res);
    if ($num != 1) {
        echo "There are $num components with the '$argv[1]' name which are pending approval !\n";
        exit;
    }

    // update this component
    $res2 = mysql_query("UPDATE components SET approved='1' WHERE name='" . $argv[1] . "' LIMIT 1");
    if ($res2 == FALSE) {
        echo "Error while querying the database !\n";
        exit;
    }

    echo "Now the $argv[1] component is set as APPROVED.\n";

    // create a new folder for this component in the website so that our "updatesite.sh" script will be able
    // to keep that folder synchronized with the CVS
    exec("mkdir -p /home/project-web/wxcode/htdocs/components/" . strtolower($argv[1]));

    // call the update scripts
    echo "Calling ./maint_updatesite.sh\n";
    exec("./maint_updatesite.sh >/dev/null 2>&1");

    echo "Calling ./cron_updatecomplistlinks.sh\n";
    exec("./cron_updatecomplistlinks.sh >/dev/null");

    // a little msg for wxCode maintainers
    echo "\nDon't forget to:\n\n";
    echo " -> for SVN components:\n";
    echo "    1) add the new component folder to the SVN (eventually copying the 'template' one)\n";
    echo "    2) add the maintainer to the wxCode project as developer\n";
    echo "    3) call the maint_approvecomp.sh script in the wxCode folder in the SourceForge\n";
    echo "       shell server (this STEP!)\n\n";
    echo " -> for CVS components:\n";
    echo "    1) add the new component folder to the CVS (copying the 'template' one)\n";
    echo "    2) add the maintainer to the wxCode project as developer\n";
    echo "    3) call the maint_approvecomp.sh script in the wxCode folder in the SourceForge\n";
    echo "       shell server (this STEP!)\n";
    echo "    4) update the AVAIL file of the CVSROOT for wxCode module\n";
?>
