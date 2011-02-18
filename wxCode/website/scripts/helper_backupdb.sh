#!/usr/bin/php -q
<?php

    // Author: Francesco Montorsi
    // Purpose: connects the wxCode DB and dumps it in a TAR.GZ file as backup
    //          NOTE: this script is written in PHP so that we can use
    //          1) the better PHP support for MySQL connections
    //          2) the dbconnection.inc.php file which already contains the info required
    //             for DB connection (like user & password) so that we do not have to duplicate
    //             them here...

    require("../dbconnection.inc.php");

    // be sure that the backup folder exists
    define("BACKUP_FOLDER", "/home/project-web/wxcode/htdocs/backup");
    if (!file_exists(BACKUP_FOLDER))
        mkdir(BACKUP_FOLDER);

    // call the db_connect function so it define the constants we need
    db_connect_fromscript();
    $today = date("Y_m_d");

    // use the MYSQLDUMP utility as described at:
    // https://sourceforge.net/docman/display_doc.php?docid=4297&group_id=1#security
    shell_exec("mysqldump --skip-lock-tables --user=" . DB_USERNAME . " --password=" . DB_PASSWORD .
            " --host=" . DB_HOST . " " . DB_NAME . " | gzip > " . BACKUP_FOLDER . "/wxcodedb.$today.sql.gz");
?>