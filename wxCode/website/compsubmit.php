<?php require("globals.inc.php"); ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 30/6/2005
// RCS-ID: $Id: compsubmit.php,v 1.10 2008/11/06 14:41:30 frm Exp $
// Notes: allows the user to ask for submission of its own component and thus add
//        one entry in the "components" table.
////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="compsubmit";
        $pagetitle="Register your component";
        $menuentry=writeMenuEntry("submit", "Submit your component");
        require("header.inc.php");
        require("comp.inc.php");

        if (isset($_POST['compFormBtn'])) {   // compFormBtn is the name of the button of form.inc.php
            write_h1("Component submission results", "", FALSE);

            // load from post
            $comp = new Component();
            $comp->get_from_post();

            if ($comp->check_for_duplicates()) {
    ?>
            <p class="error">A component with this name or this subdir already exist;
                            these two values must be unique.</p>
            <br/><?php write_h1("The component form", "submitform"); ?>
            <form action="compsubmit.php" method="post">
                <?php
                    $includesendbtn = TRUE;
                    $var = $_POST;
                    $loadfromvar = TRUE;
                    $showmaintainersfuser = TRUE;
                    $maint->password = "";    // clean password field (it was encoded with MD5 and thus is not recognizable by the user)
                    require("form.inc.php");
                ?>
            </form>
            <br/><?php require("footer.inc.php"); ?>
            </html>
    <?php
                exit;
            }

            // now we have to search the ID of the maintainer using its SF account name
            $maint = new Maintainer();
            if (!$maint->get_from_db($_POST['maintainersfuser'])) {
    ?>
            <p class="error">Cannot find a wxCode maintainer with the &quot;<?php echo $_POST['maintainersfuser']; ?>&quot; SF account name !</p>
            <br/><?php write_h1("The component form", "submitform"); ?>
            <form action="compsubmit.php" method="post">
                <?php
                    $includesendbtn = TRUE;
                    $var = $_POST;
                    $loadfromvar = TRUE;
                    $showmaintainersfuser = TRUE;
                    $maint->password = "";    // clean password field (it was encoded with MD5 and thus is not recognizable by the user)
                    require("form.inc.php");
                ?>
            </form>
            <br/><?php require("footer.inc.php"); ?>
            </html>
    <?php
                exit;
            }

            // check maintainer password
            if ($maint->password != md5($_POST['maintainerpassword'])) {
    ?>
            <p class="error">The password &quot;<?php echo $_POST['maintainerpassword']; ?>&quot; is not valid !</p>
            <br/><?php write_h1("The component form", "submitform"); ?>
            <form action="compsubmit.php" method="post">
                <?php
                    $includesendbtn = TRUE;
                    $var = $_POST;
                    $loadfromvar = TRUE;
                    $showmaintainersfuser = TRUE;
                    $maint->password = "";    // clean password field (it was encoded with MD5 and thus is not recognizable by the user)
                    require("form.inc.php");
                ?>
            </form>
            <br/><?php require("footer.inc.php"); ?>
            </html>
    <?php
                exit;
            }

            // then check the comp values submitted
            if (!$comp->are_required_fields_valid()) {
    ?>
                <p class="error">The fields &quot;Component Name&quot;, &quot;Subdir&quot;,
                &quot;Description&quot;, &quot;Author&quot; cannot be empty. The fields
                &quot;Component Name&quot;, &quot;Subdir&quot; cannot contains spaces and the
                last one must be entirely in lowercase. Be sure to fill them with the right values !</p>
                <br/><?php write_h1("The component form", "submitform"); ?>
                <form action="compsubmit.php" method="post">
                    <?php
                        $includesendbtn = TRUE;
                        $var = $_POST;
                        $loadfromvar = TRUE;
                        $showmaintainersfuser = TRUE;
                        $maint->password = "";    // clean password field (it was encoded with MD5 and thus is not recognizable by the user)
                        require("form.inc.php");
                    ?>
                </form>
                <br/><?php require("footer.inc.php"); ?>
                </html>
    <?php
                exit;
            }

            // all checks passed; link the maintainer with the component
            $comp->maintainerid = $maint->id;
            assert($comp->maintainerid != 0);       // we should have a valid maintainer ID !

            // we can now insert this new field (as NOT approved)
            if ($comp->insert_in_db() == FALSE) {
                echo "<p class='error'>" . mysql_error() . "</p>";
                echo "<br/>";
                require("footer.inc.php");
                echo "</html>";
                exit;
            }

            echo "<p>Thanks for your submission ! You have successfully submitted your ";
            echo "&quot;$comp->name&quot; component to wxCode administrators approval !</p>";
            echo "<p>As soon as possible you should receive a notification at the ";
            echo "<strong>$maint->mail</strong> address (which is the email address you gave when ";
            echo "you registered as wxCode maintainer)...";
            echo "<p>Don't forget to subscribe to the ";
            echo "<a href='http://lists.sourceforge.net/lists/listinfo/wxcode-users'>wxcode-users</a>";
            echo " mailing list. It's not a requirement but you get informed about any changes/news affecting your ";
            echo "component. And more important you might ask others for help/suggestions if you need!</p>";

            // NOTE
            // TEMPORARY WARNING ABOUT THE FACT THAT SF HAS DISABLED CRON SCRIPT
            echo "<br/><p><strong>IMPORTANT</strong>: since some time SourceForge has disabled some settings on his servers which automated the checks about the queue of component submissions. This means that your submission must be manually checked by wxCode administrators; because of this your submission may take quite some time to be processed; it's strongly recommended that you send a mail to the wxcode-users mailing list informing the wxCode administrators that you just submitted your component.</p>";

        } else {
    ?>

        <!-- ###################################################### -->
        <!-- #############  THE SUBMISSION FORM ################### -->
        <!-- ###################################################### -->
        <br/><?php write_h1("The component form", "submitform"); ?>
        <form action="compsubmit.php" method="post">
            <?php
                $includesendbtn = TRUE;
                $loadfrompost = FALSE;
                $showmaintainersfuser = TRUE;
                require("form.inc.php");
            ?>
        </form>

    <?php
        }
    ?>

    <br/><?php require("footer.inc.php"); ?>
</html>
