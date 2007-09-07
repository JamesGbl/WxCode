<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Author: YOUR_NAME
  // Creation date: someday/somemonth/someyear
  // RCS-ID: $Id: index.php,v 1.7 2005/09/01 22:26:16 frm Exp $
  // Notes: the index page of the website of this component
  ////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
	<?php
      $compname = "wxAvahi";
      $subdir = "avahi";
      require("compwebsite.inc.php");
  ?>

	<br/><?php write_h1("Introduction", "", FALSE); ?>
	<p><var>wxAvahi</var> is a wrapper for the <a href="http://avahi.org/">Avahi</a> library,
	using wxWidgets paradigm.
	Its goal is to provide some facilities to publish and discover service providers on the local area network.</p>

	<br/><?php write_h1("Sample"); ?>
	<p>In order to illustrate wxAvahi capabilities, you can look at the sample which enumerates
	all service types available on the local network and, for each service type, enumerates all service providers.</p>
	<div align="center"><?php writeScreenshotLink("wxavahi.png", "The sample in a little LAN"); ?></div>

	<br/><?php write_h1("Version"); ?>
	<p>0.1.0 - First release of wxAvahi on wxCode.sf.net.</p>

	<br/><?php write_h1("Known bugs"); ?>
  <p>None. To submit a bug report please look at <a href="../../support.php">wxCode support page</a>.</p>

	<br/><?php write_h1("Future enhancements"); ?>
	<p>GUI components like tree, lists ...</p>
  <p>To submit a feature request please look at <a href="../../support.php">wxCode support page</a>.</p>
    

	<br/><?php require("footer.inc.php"); ?>
</html>