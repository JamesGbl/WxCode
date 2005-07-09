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
      $compname = "wxXml2";
      $subdir = "wxxml2";
      require("compwebsite.inc.php");
  ?>



  <br/><?php write_h1("Screenshots"); ?>
  <p>Obviously, since wxXml2 is not an application but just a wrapper for the libxml2 library,
  the following screenshots refers to the sample app included with this module which shows
  how much easy is to get the tree structure of an XML doc through wxXml2 classes:</p><br>
  <div align="center">
  <?php writeScreenshotLink("screenshot.png", "The wxXml2 sample running on Windows XP"); ?>
  <?php writeScreenshotLink("screenshot2.png", "The wxXml2 sample running on Red Hat 9"); ?>
  </div>


  <br/><?php write_h1("Documentation"); ?>
  <p>The doxygen-generated documentation for this component is available online
  <a href="../../docs/wxxml2/">here</a>. Please let me know if you think something is missing.</p>


	<br/><?php write_h1("Known bugs"); ?>
  <p>None. To submit a bug report please look at <a href="../../faq.php">wxCode FAQ</a>.</p>


	<br/><?php write_h1("Future enhancements"); ?>
  <p>Support for libxsl.<br/>To submit a feature request please look at <a href="../../faq.php">wxCode FAQ</a>.</p>


	<br/><?php require("footer.inc.php"); ?>
</html>
