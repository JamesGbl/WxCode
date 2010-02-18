<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Author: Moskvichev Andrey V.
  // Creation date: 11/11/2008
  // RCS-ID: $Id: index.php,v 1.7 2005/09/01 22:26:16 frm Exp $
  // Notes: the index page of the website of this component
  ////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
	<?php
      $compname = "wxAdvTable";
      $subdir = "advtable";
      require("compwebsite.inc.php");
  ?>

	<br/><?php write_h1("Screenshots"); ?>
	<p>Description</p>
  <div class="center">
    <?php writeScreenshotLink("wxadvtable1.png", "Demo application screenshot"); ?>
  </div>
  
  <br/><?php write_h1("Features"); ?>
  <ol>
  	<li>Composite rows/columns.<br><img src="comprows.png"></li>
  	<li>Highlight mode.<br><img src="highlight.png"></li>
  	<li>Built-in sorting support for different data types (integer, string, date, choice, etc).</li>
  	<li>Flexible rendering and editing support.</li>
  	<li>Flexible data access model.</li>
  	<li>Full documented source code.</li>
  	<li>Dynamic row/columns add/removal.</li>
  	<li>Various visual features.</li>
  	<li>Various built-in data types, and renderers and editors for them.</li>
  	<li>Everything is easily configurable: you can change row/column label font, color, alignment, size, etc.<br>You can set font, color, etc attributes to any cell.</li>
  	<li>Clean MCV (Model-Controller-View) design.</li>
  </ol>
  
  <br/><?php write_h1("Advantages from wxGrid (standard wxWidgets table control)"); ?>
  <ol>
  	<li>Support of composite rows/columns.</li>
  	<li>Built-in sorting support.</li>
  	<li>Highlight mode.<br><img src="highlight.png"></li>
  	<li>Clean MCV (Model-Controller-View) design.</li>
  	<li>Flexible data access model.</li>
  </ol>
  
  <!-- NOTE TO THE DOCUMENTATION: for uploading you component docs in the website,
       see the ReadMe.txt file at wxCode\template... -->
  <br/><?php write_h1("Documentation"); ?>
  <p>The documentation for this component is available online
  <a href="../../docs/advtable/">here</a>.</p>

	<br/><?php write_h1("Support project"); ?>
	<p>Donate to support project development</p><br/>
	<form action="https://www.paypal.com/cgi-bin/webscr" method="post">
<input type="hidden" name="cmd" value="_s-xclick">
<input type="hidden" name="hosted_button_id" value="10094632">
<input type="image" src="https://www.paypal.com/en_US/i/btn/btn_donateCC_LG.gif" border="0" name="submit" alt="PayPal - The safer, easier way to pay online!">
<img alt="" border="0" src="https://www.paypal.com/ru_RU/i/scr/pixel.gif" width="1" height="1">
</form>
	
	<br/><?php write_h1("Known bugs"); ?>
  <p>None. To submit a bug report please look at <a href="../../support.php">wxCode support page</a>.</p>

    
	<br/><?php write_h1("Future enhancements"); ?>
	<ol>
		<li>Perl and Python bindings.</li>
		<li>Rows/columns move by drag.</li>
		<li>Add filtering support.</li>
		<li>Resize code optimization.</li>
		<li>Add bitmap data type.</li>
	</ol>
  <p>None. To submit a feature request please look at <a href="../../support.php">wxCode support page</a>.</p>
    
  <br/><?php require("footer.inc.php"); ?>
</html>