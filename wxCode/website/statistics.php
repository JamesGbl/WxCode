<?php $logsys_disabled=TRUE; require("globals.inc.php"); ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 30/6/2005
// RCS-ID: $Id: statistics.php,v 1.2 2005/09/04 11:08:47 frm Exp $
// Notes: shows some statistics about this website
////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="statistics";
        $pagetitle="wxCode statistics";
        $menuentry="Statistics";
        require("header.inc.php");
        
        // our additionally required helper files
        require_once("viewopt.inc.php");
        require_once("dbquery.inc.php");
        require_once("dboutput.inc.php");
    ?>
    
    <?php 
        write_h1("wxCode statistics", "stats", FALSE); 

        // do the queries to the wxCode dabatase
        $numcomp = DbQuery::numcomp();
        $unmaint = DbQuery::numcomp_unmaint();
        $nummaint = DbQuery::nummaint();
    ?>

    <p>There are <strong><?php echo $numcomp; ?> components registered</strong> in the wxCode website;
    <strong><?php echo $unmaint; ?> of them</strong> are UNMAINTAINED.</p>

    <p>Currently there are <strong><?php echo $nummaint; ?> maintainers registered.</strong></p>
    
    <p>TODO: add more statistics here</p>
    
    <p>For statistics about the wxCode project, you can also look at the
    <a href="http://sourceforge.net/project/stats/index.php?group_id=51305&ugn=wxcode&type=&mode=alltime">SourceForge statistics</a>...</p>

    <br/><?php require("footer.inc.php"); ?>
</html>

