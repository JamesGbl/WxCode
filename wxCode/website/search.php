<?php
    // we don't want the login system
    $logsys_disabled=TRUE;

    // but we still need sessions to pass all the search options to the searchshow.php file
    session_start();
    require("globals.inc.php");

    /* don't leave spaces after ?> keyword below and <!DOCTYPE */
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 7/7/2005
// RCS-ID: $Id: search.php,v 1.11 2006/05/14 09:53:46 frm Exp $
// Notes: lets the user search the component table for a specific feature
////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="search";
        $pagetitle="Search components";
        $menuentry="Search";
        require("header.inc.php");
        require_once("dbquery.inc.php");
        require_once("comp.inc.php");
    ?>

    <?php write_h1("Advanced component search", "compsearch", FALSE); ?>
    <p>NOTE: here you can search the wxCode component database only. If you want to do
    a site-search then use the &quot;Search this site&quot; button in the top-right
    corner of this page.</p>
    <p>Below you can use the 'search data' form to input the search values. Remember
    that <strong>the '%' symbol means 'anything'</strong> (and can be used like the '*' wildcard)
    and the 'Any' option in the comboboxes below still means that any value will be matched.</p>

    <!-- here starts our big form -->
    <br/><form action="searchshow.php" method="post">
    <?php
          $comp = new Component();

          // set the default seach options
          $cdatedisabled = TRUE;
          $locationdisabled = TRUE;
          $wikidisabled = TRUE;
          $samplesdisabled = TRUE;
          $labels = FALSE;
          $searchmode = TRUE;
          $includesendbtn = FALSE;
          $autosubmit = FALSE;

          $comp->init_all_with("%");

          write_h1("Search data");
          echo "<p><br/></p>";             // inside FORM elements <br/> tags are not allowed so we enclose it in <p> tags
          require("form.inc.php");
          echo "<p><br/></p>";             // inside FORM elements <br/> tags are not allowed so we enclose it in <p> tags
    ?>
    <p><br/></p>
    <?php
          write_h1("Result view options");

          // load view opt
          $view = new ViewOptAndPos();
          $view->get_from_post();

          $showviewform = TRUE;
          $includeviewbtn = FALSE;
          require("viewmode.inc.php");
    ?>
    <div style="text-align: center; margin-left: auto; margin-right: auto;">
    <input style="width: 30%; height: 2em;" type="submit" name="searchBtn" value="Search"/>
    </div>
  </form>

    <br/><?php require("footer.inc.php"); ?>
</html>
