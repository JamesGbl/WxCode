<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<!--
  Author: Francesco Montorsi
  Creation date: 7/7/2005
  RCS-ID: $Id$
  Notes: the index page of the website of this component
-->

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
	<?php
       require("../../globals.inc.php");
       $filename="index";
       $filelocation="components/keybinder";
       $menuentry=writeMenuEntry("complist", "Components");
       $pagetitle="KeyBinder";
       require("../../header.inc.php");
       require("../../dbconnection.inc.php");
       require("../../dbquery.inc.php");
    ?>

	<!-- Note on the write_h1() php function:
         - first arg: the label of the h1 tag
         - second arg: the ID of the h1 tag (defaults to "")
         - third arg: if TRUE the TOP link is added; if FALSE it is omitted (defaults to TRUE)
         use FALSE as third arg for the first h1 tag of the page since it must
         not have the TOP link...
    -->
	<?php
        $compname = "KeyBinder";
        write_h1($compname, "", FALSE);
        db_output_comp($compname);
    ?>

	<!-- HERE STARTS THE CONTENT OF THIS PAGE THAT YOU (COMPONENT ADMIN) SHOULD EDIT -->
	<br/><?php write_h1("Screenshots"); ?>
	<p>Screenshot description:
    <img src="../../screenshots/keybinder/myshot1.png" alt="My Screenshot"/></p>

    <br/><?php write_h1("Documentation"); ?>
    <p>The doxygen-generated documentation for this component is available online
    <a href="../../docs/keybinder/">here</a>...</p>
    

	<br/><?php write_h1("Known bugs"); ?>
    <p>None. To submit a bug report...</p>

    
	<br/><?php write_h1("Future enhancements"); ?>
    <p>None. To submit a feature request...</p>
    

	<br/><?php require("../../footer.inc.php"); ?>
</html>
