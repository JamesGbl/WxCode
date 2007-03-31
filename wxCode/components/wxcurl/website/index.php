<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Author: Francesco Montorsi
  // Creation date: 31/3/2007
  // RCS-ID: $Id: index.php,v 1.7 2005/09/01 22:26:16 frm Exp $
  // Notes: the index page of the website of this component
  ////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
  <?php
      $compname = "wxCURL";
      $subdir = "wxcurl";
      require("compwebsite.inc.php");
  ?>

    <!--
  <br/><?php write_h1("Screenshots"); ?>
  <p>Description</p>
  <div class="center">
    <?php writeScreenshotLink("myshot.png", "description goes here"); ?>
  </div>
  -->


  <!-- NOTE TO THE DOCUMENTATION: for uploading you component docs in the website,
       see the ReadMe.txt file at wxCode\template... -->
  <br/><?php write_h1("Documentation"); ?>
  <p>The documentation for this component is available online
  <a href="../../docs/wxcurl/">here</a>.</p>


  <br/><?php write_h1("Known bugs"); ?>
  <p>None. To submit a bug report please look at <a href="../../support.php">wxCode support page</a>.</p>

  <br/><?php write_h1("Future enhancements"); ?>
  <p>None. To submit a feature request please look at <a href="../../support.php">wxCode support page</a>.</p>


  <br/><?php require("footer.inc.php"); ?>
</html>