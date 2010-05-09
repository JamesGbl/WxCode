<?php $logsys_disabled=TRUE; require("globals.inc.php"); ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 30/6/2005
// RCS-ID: $Id: toapprove.php,v 1.4 2009/10/16 11:58:43 frm Exp $
// Notes: shows the list of components to approve
////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="toapprove";
        $pagetitle="Components to approve";
        $menuentry="Components";
        require("header.inc.php");

        // our additionally required helper files
        require_once("viewopt.inc.php");
        require_once("dbquery.inc.php");
        require_once("dboutput.inc.php");
    ?>

    <?php write_h1("Components to approve", "complist_to_approve", FALSE); ?>
    <p>List of the components waiting approval:</p>

    <?php
        // do the query to the wxCode dabatase
        $result = DbQuery::comp_to_approve();

        // load from POST vars...
        $view = new ViewOptAndPos();
        $view->get_from_post();

        if (mysql_num_rows($result) < 1)
            echo "<p>No components waiting for approval.</p>";
        else
            DbOutput::queryres($result, $view);        // place here query results
    ?>

    <br/><?php require("footer.inc.php"); ?>
</html>

