<?php $logsys_disabled=TRUE; require("globals.inc.php"); ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<?php
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Author: Francesco Montorsi
  // Creation date: 30/6/2005
  // RCS-ID: $Id: showcomp.php,v 1.5 2005/09/04 11:08:47 frm Exp $
  // Notes: shows the info about a specific component 
  ////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="showcomp";
        
        if (!isset($_REQUEST['name']))
            $pagetitle="Error!";
        else {
            $compname = $_REQUEST['name'];
            $pagetitle=$compname;
        }
        $menuentry=writeMenuEntry("complist", "Components");        
        require("header.inc.php");
    ?>
    
    
    
    <?php   
        if (!isset($compname)) {
            echo "<p class=\"error\">Missing the name of the component to show !<br/>";
            echo "Call this page with the format: http://wxcode.sourceforge.net/showcomp.php?name=compname,<br/>";
            echo "where <em>compname</em> is the name of the component details you want to show.</p>";
        } else {
            write_h1($compname, "", FALSE);
            require("dbquery.inc.php");
            require("dboutput.inc.php");
            DbOutput::comp($compname);
            echo "<br/>\n";
        }
    ?>

    <br/><?php require("footer.inc.php"); ?>
</html>
