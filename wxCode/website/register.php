<?php require("globals.inc.php"); ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 30/6/2005
// RCS-ID: $Id: register.php,v 1.4 2005/09/03 23:14:52 frm Exp $
// Notes: allows the user to register himself in the "users" table
////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="register";
        $pagetitle="Register as wxCode maintainer";
        $menuentry=writeMenuEntry("submit", "Submit your component");
        require("header.inc.php");

        // contains Maintainer class
        require("comp.inc.php");

        if (isset($_POST['maintFormBtn'])) {   // maintFormBtn is the name of the button of form.inc.php

            write_h1("wxCode registration results", "", FALSE);

            // load from post the maintainer data
            $maint = new Maintainer();
            $maint->get_from_post();

            if ($maint->check_for_duplicates()) {
    ?>
                <p class="error">A maintainer with this name or with the same SF id already exist; 
                                 these two values must be unique. Change them and retry.</p>
                <br/><?php write_h1("The wxCode maintainer registration form", "register"); ?>
                <form action="register.php" method="post">
                    <?php $includesendbtn = TRUE; $loadfrompost = TRUE; require("maintainer.inc.php"); ?>
                </form>
                <br/><?php require("footer.inc.php"); ?>
                </html>
    <?php
                exit;
            }

            // then check all the values submitted
            if ($maint->name == "" or $maint->sfuser == "" or $maint->password == "" or $maint->mail == "") {
    ?>
                <p class="error">All the fields must be non-empty. Be sure to fill them with the right values !</p>
                
                <br/><?php write_h1("The wxCode maintainer registration form", "register"); ?>
                <form action="register.php" method="post">
                    <?php $includesendbtn = TRUE; $loadfrompost = TRUE; require("maintainer.inc.php"); ?>
                </form>
                <br/><?php require("footer.inc.php"); ?>
                </html>
    <?php
                exit;
            }

            if ($maint->insert_in_db() == FALSE) {
                echo "<p class=\"error\">" . mysql_error() . "</p>";
                echo "<br/>";
                require("footer.inc.php");
                echo "</html>";
                exit;
            }
    ?>

        <p>You are now successfully registered as wxCode maintainer !<br/>
           You can now be set as a component maintainer and submit your own components.</p>
        <p>For more info about component submission return to the <a href="submit.php">this page</a>...</p>

    <?php
        } else {
    ?>

        <!-- ###################################################### -->
        <!-- #############  THE SUBMISSION FORM ################### -->
        <!-- ###################################################### -->
        <?php write_h1("The wxCode maintainer registration form", "register", FALSE); ?>
        <form action="register.php" method="post">
            <?php $includesendbtn = TRUE; $loadfrompost = FALSE; require("maintainer.inc.php"); ?>
        </form>
        
    <?php
        }
    ?>

    <br/><?php require("footer.inc.php"); ?>
</html>
