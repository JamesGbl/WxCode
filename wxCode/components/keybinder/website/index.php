<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Author: Francesco Montorsi
  // Creation date: 7/7/2005
  // RCS-ID: $Id$
  // Notes: the index page of the website of this component
  ////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
	<?php       
      $compname = "KeyBinder";
      $subdir = "keybinder";
      require("compwebsite.inc.php");
  ?>

	<!--
       Note on the write_h1() php function:
         - first arg: the label of the h1 tag
         - second arg: the ID of the h1 tag (defaults to "")
         - third arg: if TRUE the TOP link is added; if FALSE it is omitted (defaults to TRUE)
         use FALSE as third arg for the first h1 tag of the page since it must
         not have the TOP link...
  -->

	<!-- HERE STARTS THE CONTENT OF THIS PAGE THAT YOU (COMPONENT ADMIN) SHOULD EDIT -->
	<br/><?php write_h1("Screenshots"); ?>
	<p>Obviously, since keybinder is not an application, the following screenshots
	refer to the sample application included with this module which shows the usage
	of the keybinder classes.<br/>The following screenshots have been taken on a
	Windows XP system using different build flags:</p><br>
	<?php writeScreenshotLink("win1.png", "imgcenter", "The keybinder sample running with wxMSW 2.5.3"); ?>
	&nbsp;&nbsp;&nbsp;&nbsp;
	<img class="imgcenter" alt="The keybinder sample running with wxMSW 2.5.3" src="../../screenshots/keybinder/win2.png"/>	
	
	<p>The following ones instead have been taken on Red Hat Linux 9.0:</p>
	<img class="imgcenter" alt="The keybinder sample running with wxGTK 2.5.3" src="../../screenshots/keybinder/gtk3.png"/>
	&nbsp;&nbsp;&nbsp;&nbsp;
	<img class="imgcenter" alt="The keybinder sample running with wxGTK 2.5.3" src="../../screenshots/keybinder/gtk4.png"/>


    <br/><?php write_h1("Documentation"); ?>
    <p>The doxygen-generated documentation for this component is available online
    <a href="../../docs/keybinder/">here</a>. Please let me know if you think something is missing.</p>
    

	<br/><?php write_h1("Known bugs"); ?>
    <p>None. To submit a bug report please look at <a href="../../faq.php">wxCode FAQ</a>.</p>

    
	<br/><?php write_h1("Future enhancements"); ?>
    <p>None. To submit a feature request please look at <a href="../../faq.php">wxCode FAQ</a>.</p>
    

    

	<br/><?php require("footer.inc.php"); ?>
</html>
