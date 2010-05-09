<?php require("globals.inc.php"); ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 30/6/2005
// RCS-ID: $Id: edit.php,v 1.18 2008/06/18 18:14:53 frm Exp $
// Notes: allows the user to edit the entry in the wxCode database about its own component
//        this file queries both the "components" & the "maintainers" tables...
////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="edit";
        $menuentry="Edit your component";
        $pagetitle="Edit your component's entry in the wxCode database";
        require("header.inc.php");
        require("dbquery.inc.php");

        if (!$logged_in) {

            // the user needs to login before accessing this page !
            displayLogin();

        } else if (isset($_POST['compFormBtn'])) {   // compFormBtn is the name of the button of form.inc.php
            write_h1("Component update results", "", FALSE);

            $comp = new Component();
            $comp->get_from_post();
            if ($comp->name == "")
                $comp->name = $_POST['currcomp'];
            assert($comp->name != "");

            $q = "";
            $res = $comp->updatedb($q);

            if ($res == FALSE) {
                echo "<p class='error'>" . mysql_error() . "</p>";
                echo "<br/>";
                require("footer.inc.php");
                echo "</html>";
                exit;
            } else {

                echo "<p><strong>The component info have been successfully updated</strong> with the query: &quot;<em>$q</em>&quot;";
                echo "<br/><br/>Below is the edit form with updated values.</p>";
            }

        }

        // in this way after a successfully update, we also show the EDIT form again...
        if  ($logged_in) {

            write_h1("Choose your component to edit", "choose", FALSE);
    ?>

        <!-- Show the CHOOSE-YOUR-COMPONENT form -->
        <form action="edit.php" method="post">
            <p><strong> &gt; Your wxCode maintainer ID is: </strong>
            <?php

                // get the ID for this maintainer
                $id = DbQuery::maintainer_id($_SESSION['username']);
                assert(isset($id) and !empty($id));
                echo "$id<br/>";
            ?>

            <p><strong> &gt; Quick links to your components: <br/></strong>
            <?php
                // ask to the DB how many components are associated to this maintainer
                db_connect();   // be sure to be connected
                $result = mysql_query("SELECT name FROM components WHERE maintainerid='$id'", $dbconn);
                $numcomps = mysql_num_rows($result);

                $i = 0;
                echo "<div class='pagelink'><p>";
                while ($row = mysql_fetch_assoc($result)) {
                    if ($i > 0) echo " | ";
                    echo "<a href='" . SITEBASEURL .
                        "showcomp.php?name=$row[name]'>$row[name]</a>";
                    $i++;
                }
                echo "</p></div>";
            ?><br/>

            <strong> &gt; Choose the component you want to edit: </strong>
            <select name="currcomp" onchange="this.form.submit();">
            <?php
                assert(isset($id) and !empty($id));

                // NOTE: for better performance we re-use the query results stored in $result
                //       of the quick-links section above
                mysql_data_seek($result, 0);
                $row = mysql_fetch_assoc($result);      // start again from the first result row

                // are we already editing a component ?
                if (isset($_REQUEST['currcomp']))
                    $currcomp = $_REQUEST['currcomp'];
                else
                    $currcomp = $row['name'];       // curr component will be set as the
                                                    // first component owned by this maintainer

                // write the option items
                $validcomp = false;
                for ($i=0; $i < $numcomps; $i++) {
                    $entry = $row['name'];

                    if (strcasecmp($currcomp, $entry) == 0)
                        $currcomp = $entry;     // the name of current component had a wrong case somewhere
                    if ($currcomp == $entry)
                        $validcomp = true;      // the current component exists and is owned by this maintainer !

                    writeOptionItem($entry, $entry, $currcomp);
                    $row = mysql_fetch_assoc($result);  // get next
                }
             ?>
            </select></p>
        </form>


        <!-- Show the EDIT form -->
        <?php

            // in case the $currcomp variable was taken from the $_REQUEST array and
            // this maintainer doesn't have any component with that name, give an error...
            if ($validcomp == false) {

                echo "<br/><p class='error'>Invalid component: <em>" . $currcomp . "</em>.\n";
                echo "That component does not exist or you are not its maintainer.</p><br/>";

            } else {
                echo "<br/>\n";
                write_h1("Edit your component's info", "submit");
        ?>
        <p>Here you can edit all the values which belong to this component except for
        the component name (that field will be ignored).</p>

        <!-- here starts the form -->
        <form action="edit.php" method="post">
            <?php

                // load all the data of this component from the DB
                $comp = new Component();
                $comp->get_from_db($currcomp);

                // set our show options for the EDIT form
                $includesendbtn = TRUE;

                $showmaintainersfuser = FALSE;
                $submitlabel = "Update";
                $labels = "edit";
                $namedisabled = TRUE;
                require("form.inc.php");
            ?>
            <input name="currcomp" type="hidden" value="<?php echo $currcomp; ?>"/>
        </form>

    <?php
            }
        }
    ?>

    <br/><?php require("footer.inc.php"); ?>
</html>
