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

	<br/><?php write_h1("Introduction"); ?>
	<p>wxHTTPEngine is a library of classes to easily allow your wxWidgets software to work with web sites.</p>
	<p><strong>wxHyperlinkCtrl</strong> is a hyperlink control for wxWidgets.  Features include underline and
	color rollovers, the ability to perform your own Left, Middle, and Right click events to perform your own custom 
	event handling and ability to open a link in a new Internet Explorer browser window.  Static GotoURL() function
	may be used outside of the wxHyperlinkCtrl.</p>
	<p><strong>wxHTTPBuilder</strong> is a complete HTTP protocol builder extended from the wxHTTP class.
	Features include the ability to build headers such as basic authentication and cookies, ability to populate and url
	encode the GET and POST query strings, and ability to handle multipart data (files). May connect through a HTTP
	proxy server with or without user authentication.</p>
	<p><strong>wxHTTPBuilderThread</strong> is a special thread that may be used in your wxWidgets
	application to allow the user to cancel a wxHTTPBuilder connection.  Keep in mind, this may not be the best way
	to implement threads in your application.  If anything, use the source as a guide.</p>
	<p><strong>wxAuthenticateDlg</strong> is a simple dialog to to get a username and password from the user
	for Basic HTTP Authentication.</p>
	<p><strong>wxPleaseWaitDlg</strong> is a simple dialog similar to the wxProgressDlg.</p>
	<p><strong>wxProxySettingsDlg</strong> is a simple proxy settings dialog with optional exceptions textbox
	and requires authentication checkbox with username and password fields.</p>
	
	<br/><?php write_h1("Version"); ?>
	<p>2.0 - First release of wxHTTPEngine on wxCode.sf.net.</p>
	
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
	<p>AutoBrowse() and DoPopup() in the wxHyperlinkCtrl complements of Mark McManus.</p>
    
	<br/><?php require("footer.inc.php"); ?>
</html>
