<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Author: Luciano Cattani
  // Creation date: 2007/11/02
  // RCS-ID: $Id: index.php,v 1.2 2007/11/10 16:36:35 luccat Exp $
  // Notes: the index page of the website of this component
  ////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
	<?php
      $compname = "wxJSON";
      $subdir = "wxjson";
      require("compwebsite.inc.php");
  ?>

	<!-- NOTE on the write_h1() php function:
         - first arg: the label of the h1 tag
         - second arg: the ID of the h1 tag (defaults to "")
         - third arg: if TRUE the TOP link is added; if FALSE it is omitted (defaults to TRUE)
         use FALSE as third arg for the first h1 tag of the page since it must
         not have the TOP link...
  -->

  <br/><?php write_h1("Introduction"); ?>

<p>JSON (JavaScript Object Notation) is a lightweight data-interchange format.
It is easy for humans to read and write. It is easy for machines to parse
and generate. It is based on a subset of the JavaScript Programming Language,
Standard ECMA-262 3rd Edition - December 1999. JSON is a text format that is
completely language independent but uses conventions that are familiar to
programmers of the C-family of languages, including C, C++, C#, Java,
JavaScript, Perl, Python, and many others. These properties make JSON an
ideal data-interchange language.<br>
To know more on JSON read the following pages:

<ul>
 <li><a href="http://www.json.org" target=blank>Introducing JSON.</a>
 <li><a href="http://www.json.org/fatfree.html" target=blank>
	The Fat-free alternative to XML.</a>
</ul>
</p>



  <!-- NOTE TO THE DOCUMENTATION: for uploading you component docs in the website,
       see the ReadMe.txt file at wxCode\template... -->
  <br/><?php write_h1("Documentation"); ?>
  <p>The documentation for this component is available online
  <a href="../../docs/wxjson/">here</a>.</p>


	<br/><?php write_h1("Known bugs"); ?>
  <p>To submit a bug report please look at <a href="../../support.php">wxCode support page</a>.</p>

    
	<br/><?php write_h1("Future enhancements"); ?>
  <p>To submit a feature request please look at <a href="../../support.php">wxCode support page</a>.</p>
    

	<br/><?php require("footer.inc.php"); ?>
</html>