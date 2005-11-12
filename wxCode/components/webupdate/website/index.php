<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Author: Francesco Montorsi
  // Creation date: 13/7/2005
  // RCS-ID: $Id$
  // Notes: the index page of the website of this component
  ////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
        <?php
      $compname = "WebUpdate";
      $subdir = "webupdate";
      require("compwebsite.inc.php");
  ?>


  <br/><?php write_h1("Screenshots"); ?>
  <p>WebUpdate can be used with several GUI masks (all customizable):</p>
  <div class="center">
    <?php writeScreenshotLink("kde-simple.png", "WebUpdate on wxGTK 2.7.0 with a simple GUI mask"); ?>
    <?php writeScreenshotLink("msw-simple.png", "WebUpdate on wxMSW 2.6.2 with a simple GUI mask"); ?>
    <br/>
    <?php writeScreenshotLink("kde-adv.png", "WebUpdate on wxGTK 2.7.0 with the advanced GUI mask"); ?>
    <?php writeScreenshotLink("msw-adv.png", "WebUpdate on wxMSW 2.6.2 with the advanced GUI mask"); ?>
    <br/>
    <?php writeScreenshotLink("kde-logwnd.png", "WebUpdate on wxGTK 2.7.0 with the simple GUI mask + log window"); ?>
    <?php writeScreenshotLink("msw-logwnd.png", "WebUpdate on wxMSW 2.6.2 with the simple GUI mask + log window"); ?>
  </div>



  <!-- NOTE TO THE DOCUMENTATION: for uploading you component docs in the website,
       see the ReadMe.txt file at wxCode\template... -->
  <br/><?php write_h1("Documentation"); ?>
  <p>The documentation for this component is available online
  <a href="../../docs/webupdate/">here</a>.</p>


        <br/><?php write_h1("Known bugs"); ?>
  <p>None. To submit a bug report please look at <a href="../../support.php">wxCode support page</a>.</p>


        <br/><?php write_h1("Future enhancements"); ?>
    <p>- Allow in the REMOTE XML script to specify a "category" for the packages and use
a wxWebUpdateTreeCtrl instead of wxWebUpdateListCtrl to show the packages (and make
WebUpdater similar to Cygwin setup program). </p>

    <p>To submit a feature request please look at <a href="../../support.php">wxCode support page</a>.</p>


        <br/><?php require("footer.inc.php"); ?>
</html>