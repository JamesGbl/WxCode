#!/usr/bin/php -q
<?php

    // Author: Francesco Montorsi
    // Purpose: connects the wxCode DB and sends a message to wxcode-users@lists.sourceforge.net
    //          about all those components which have been submitted for wxCode admin approval.
    //          NOTE: this script is written in PHP so that we can use
    //          1) the better PHP support for MySQL connections
    //          2) the dbconnection.inc.php file which already contains the info required
    //             for DB connection (like user & password) so that we do not have to duplicate
    //             them here...

    require("../dbconnection.inc.php");


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


    // connect and ask the DB for not-approved components
    db_connect_fromscript();
    $res = mysql_query("select * from components where approved='0'");
    if ($res == FALSE) {
        echo "Error while querying the database !\n";
        exit;
    }

    $tosend = mysql_num_rows($res);
    echo "I've found that there are $tosend component pending approvation...\n";

    // do we have non-approved components ?
    if ($tosend == 0) {

        exit;

    } else {

        // send a mail for each non-approved component
        while ($row = mysql_fetch_assoc($res)) {

            // be sure we can find a maintainer for such component
            $res2 = mysql_query("select * from maintainers where id='$row[maintainerid]'");
            if ($res2 == FALSE) {
                echo "Could not find a maintainer for a just-submitted component ($row[name]) !\n";
                exit;
            }
            if (mysql_num_rows($res2) != 1) {
                echo "Duplicated IDs ($row[maintainerid]) in the 'maintainers' table ?? !\n";
                exit;
            }
            $maintainer = mysql_fetch_assoc($res2);

            $body = "";
            foreach ($row as $key => $value)
                $body .= "$key: $value\n";

            // don't dump all the field of this maintainer (like password) !
            $body .= "\nMaintainer SF username: $maintainer[sfuser]\n";
            $body .= "Maintainer name: $maintainer[name]\n";
            $body .= "Maintainer mail address: $maintainer[mail]\n";

            // FIXME: since currently mails to wxcode-users mailing list are not dispatched,
            //        we send this mail also to the wxCode admins directly
            $dest = "wxcode-users@lists.sourceforge.net, frm@users.sourceforge.net";
            sendMail("$dest", "Cron job: $row[name] is pending approval", $body);

            $maintainerres = mysql_query("select * from maintainers where id='$row[maintainerid]'");
            if ($maintainerres == FALSE or mysql_num_rows($maintainerres) != 1) {
                echo "Error while notifying the maintainer !\n";
                exit;
            }

            $maint = mysql_fetch_assoc($maintainerres);
            $dest = "$maint[mail]";

            echo "Writing a mail to $dest confirming that the component submission has been processed\n";
            sendMail("$dest", "Component submission has been sent successfully",
                     "Hi,\nif you are reading this automated message it means that your '$row[name]' component " .
                     "submission has been successfully inserted in the wxCode database. Also, a mail has been " .
                     "already sent to the wxcode-users mailing list for approval... you should receive feedback" .
                     " in few days.\n\nDon't forget to subscribe that mailing list ! Just go to\n\n         " .
                     "http://lists.sourceforge.net/lists/listinfo/wxcode-users\n\n" .
                     "and enter your mail address to subscribe it.\n" .
                     "The following is the list of the data you have inserted when you did the submission:\n\n" .
                     $body .
                     "\n\nFor any problems send a mail to one of wxCode administrator,\n the wxCode team");
        }
    }
?>