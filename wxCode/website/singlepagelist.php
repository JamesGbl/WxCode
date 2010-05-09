<?php $logsys_disabled=TRUE; require("globals.inc.php"); ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 30/6/2005
// RCS-ID: $Id: singlepagelist.php,v 1.4 2005/09/04 11:49:49 frm Exp $
// Notes: shows the list of component stored in the wxCode DB;
//        unlike the COMPLIST.php page, this page DOES NOT use the cached page links
////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="singlepagelist";
        $pagetitle="Components list in SINGLE page mode";
        $menuentry="Components";
        require("header.inc.php");
        
        // our additionally required helper files
        require_once("viewopt.inc.php");
        require_once("dbquery.inc.php");
        require_once("dboutput.inc.php");
    ?>
    
    <?php write_h1("Components", "complist", FALSE); ?>
    <p>Here you can browse the wxCode component database as a single XHTML page. Choose your view options:</p>
    <form method="post" action="singlepagelist.php">
    <?php
        // load from POST vars...
        $view = new ViewOptAndPos();
        $view->get_from_post();

        // a little exception to provide a "clone" of the old website's list
        if ($view->opt->viewmode == VIEWOPT_VIEWMODE_SINGLETABLE)
            $view->opt->compperpage = VIEWOPT_COMPPERPAGE_ALL;
        
        $showviewform = TRUE;
        $autosubmit = FALSE;
        $showsinglepagelist = TRUE;
        require("viewmode.inc.php");
    ?>
    <p>NOTE: You can also browse all the component in a categorized mode in the <a href="complist.php">component list page</a>.</p>
    </form>
    <hr style="width: 90%"/>
    <?php
        // do the query to the wxCode dabatase
        $result = DbQuery::complist($view, TRUE, TRUE);

        // place here query results
        $n = DbQuery::numcomp();
        $view->write_all_page_links($filename, $n, TRUE);
        DbOutput::queryres($result, $view);
        
        // end with another line of page links
        echo "<br/>";       
        $view->write_all_page_links($filename, $n, TRUE);
    ?>

    <br/><?php require("footer.inc.php"); ?>
</html>

