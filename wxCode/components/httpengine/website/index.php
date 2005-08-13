<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Author: Angelo Mandato
  // Creation date: 7/7/2005
  // RCS-ID: $Id$
  // Notes: the index page of the website of this component
  ////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
	<?php 
      $compname = "wxHTTPEngine";
      $subdir = "wxhttpengine";
      require("compwebsite.inc.php");
  ?>

	<br/><?php write_h1("Screenshots"); ?>
	<p>Obviously, wxHTTPEngine is not an application, the following screenshots
	refer to the sample application included with this library.<br/>The following screenshots have been taken on a
	Windows XP:</p>
	
	<div class="center">
	<?php writeScreenshotLink("win1.png", "The wxHTTPBuilder sample running with wxMSW 2.6.1"); ?>
	&nbsp;&nbsp;&nbsp;&nbsp;
	<?php writeScreenshotLink("win2.png", "The wxHTTPBuilder sample running with wxMSW 2.6.1"); ?>
	</div>
	
  <br/><?php write_h1("Documentation"); ?>
  <p>The doxygen-generated documentation for this component is available online
  <a href="../../docs/wxhttpengine/">here</a>.</p>
  

	<br/><?php write_h1("Known bugs"); ?>
  <p>None. To submit a bug report please look at <a href="../../faq.php">wxCode FAQ</a>.</p>

    
	<br/><?php write_h1("Future enhancements"); ?>
  <p>None. To submit a feature request please look at <a href="../../faq.php">wxCode FAQ</a>.</p>
	
	<br/><?php write_h1("Many Thanks"); ?>
	<p>Many thanks to Francesco Montorsi for assisting me with improvements and getting the project on the wxCode site.</p>
	<p>Thank you Mark McManus for contributing code to the wxHyperlinkCtrl.</p>
    
	<br/><?php require("footer.inc.php"); ?>
</html>
