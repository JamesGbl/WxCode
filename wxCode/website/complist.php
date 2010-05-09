<?php $logsys_disabled=TRUE; require("globals.inc.php"); ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 30/6/2005
// RCS-ID: $Id: complist.php,v 1.87 2009/02/02 17:43:06 frm Exp $
// Notes: shows the list of component stored in the wxCode DB;
//        unlike the search page, this page use the cached page links
//        instead of simple "Page 1 | Page 2 | Page ..." links
////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="complist";
        $pagetitle="Components list";
        $menuentry="Components";
        require("header.inc.php");

        // our additionally required helper files
        require_once("viewopt.inc.php");
        require_once("dbquery.inc.php");
        require_once("dboutput.inc.php");

        $total_components = DbQuery::numcomp();
    ?>

    <?php write_h1("Components", "complist", FALSE); ?>
    <p>Here you can browse the wxCode component database.
       It contains <strong><?php echo $total_components; ?> components</strong>
       (note that each component may appear in many categories in the list below).
    </p>
    <p>Choose your view options:</p>

    <form method="post" action="complist.php">
    <?php
        // load from POST vars...
        $view = new ViewOptAndPos();
        $view->get_from_post();

        $showviewform = TRUE;
        $autosubmit = FALSE;
        require("viewmode.inc.php");
    ?>
    <p>For a description of the categories which are used to classify wxCode components <a href="categories.php">click here</a>.</p>
    <p>NOTE: You can also browse all the component list in a single <strong>big</strong> XHTML page <a href="singlepagelist.php">here</a>.</p>
    </form>


    <!-- TODO: insert here the link to the JavaScript to expand/collapse components
               shown in FULL mode to something like COMPACT mode was -->
    <hr style="width: 90%"/>

    <?php
        // do the query to the wxCode dabatase
        $result = DbQuery::complist($view);

        // include the right file with the links for the current viewmode
        $view->write_cached_page_links(TRUE);
        assert(isset($numcomps) and $numcomps > 0);
            // see write_cached_page_links for more info; $numcomps is extracted
            // from the cached links file's $numcomps_serialized variable and
            // represents the number of components for the current category only!

        // write additional links
        echo "<div class='pagelink'><p>";
        $view->write_current_pos();
        $view->write_page_links_for_section($filename, $numcomps);
        echo "</p></div><br/>\n";

        // place here query results
        DbOutput::queryres($result, $view);

        // end with another line of page links (in reverse order)
        echo "<br/><div class='pagelink'><p>";
        $view->write_page_links_for_section($filename, $numcomps);
        $view->write_current_pos();
        echo "</p></div>\n";

        $view->write_cached_page_links(TRUE);
    ?>

    <br/><?php require("footer.inc.php"); ?>
</html>

