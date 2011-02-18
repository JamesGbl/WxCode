<?php
    // we don't want the login system
    $logsys_disabled=TRUE;

    // but we still need sessions to pass all the search options to the searchshow.php file
    session_start();
    require("globals.inc.php");

    /* NOTE: don't leave spaces after ?> keyword below and < ! DOCTYPE
       NOTE2: this comment *must* be in C style and not C++ style - maybe a PHP bug... */

?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 30/6/2005
// RCS-ID: $Id: searchshow.php,v 1.14 2006/01/13 15:03:25 frm Exp $
// Notes: shows the results of search query
////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="searchshow";
        $pagetitle="Search";
        $menuentry="Search";
        require("header.inc.php");
    ?>

    <?php write_h1("Search results", "reslist", FALSE); ?>
    <p>The following are the results of your query. To submit a new query return
       to the <a href="search.php">search page</a>...</p>
    <?php
        // ensure this page has been called in the proper way
        if (!isset($_REQUEST['searchBtn'])     // this is set by search.php
            and !isset($_SESSION['searchOpt'])) {   // page is set when clicking in one of the page links
    ?>

        <br/>
        <p class="error">Sorry; you cannot call this page yourself.
        Use the <a href="search.php">search page</a> instead.</p>
        <br/>

    <?php
        } else {

            require_once("comp.inc.php");
            require_once("dbquery.inc.php");
            require_once("dboutput.inc.php");

            // load view opt
            $view = new ViewOptAndPos();
            $view->get_from_post();

            // a little exception to provide a "clone" of the old website's list
            if ($view->opt->viewmode == VIEWOPT_VIEWMODE_SINGLETABLE)
              $view->opt->compperpage = VIEWOPT_COMPPERPAGE_ALL;

            // we want to hide the viewmode form since the user already chosen its viewmode in the search.php page
            $showviewform = FALSE;
            require("viewmode.inc.php");

            // get the right array with search opt
            $arr = array();
            if (isset($_POST['searchBtn'])) {

              // we'll use _POST array since this page has been called from search.php
              echo "<!-- using the _POST search data -->";
              $arr = $_POST;

            } else {

              // we'll use _SESSION since this page has been called from a previous searchshow.php page
              // (which should have saved all its search data in _SESSION)
              echo "<!-- using the _SESSION search data -->";
              $arr = $_SESSION['searchOpt'];
            }

            assert(isset($arr) and is_array($arr));


            // do the query to the wxCode dabatase
            $result = DbQuery::searchlist($arr, $query, $numrows, $view, $numpages);
            if ($result != FALSE) {

              // tell the user how many results we have found
              echo "<p><strong>$numrows components</strong> found matching your query...</p>";

              // place here query results
              $view->write_all_page_links($filename, $numrows, TRUE);
              DbOutput::queryres($result, $view);

              // end with another line of page links
              echo "<br/>";
              $view->write_all_page_links($filename, $numrows, TRUE);
            }

            // save in the session array the current search options
            //
            // VERY IMPORTANT NOTE: I lost *ton* of hours trying to make PHP sessions work on Sourceforge sites.
            //                                         In the end I find the solution:
            //                         http://sourceforge.net/docman/display_doc.php?docid=14267&group_id=1#webphpsession
            //                                        and modified the .htaccess file of the wxCode website with the right session save path.
            //                                        Don't modify that setting !!!!!!!
            $_SESSION['searchOpt'] = $arr;       // in case we originally get the array from the _POST variable
            assert(isset($_SESSION['searchOpt']));
        }
    ?>

    <br/><?php require("footer.inc.php"); ?>
</html>
