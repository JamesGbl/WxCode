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


  <br/><?php write_h1("Download"); ?>
  <p>There are three downloads available for this component: the <a href="http://prdownloads.sourceforge.net/wxcode/webupdate-1.0.tar.gz?download">source package</a>, a <a href="http://prdownloads.sourceforge.net/wxcode/webupdate-msw-1.0.zip?download">precompiled sample for Win32</a> and a <a href="http://prdownloads.sourceforge.net/wxcode/webupdate-gtk2-1.0.tar.gz?download">precompiled sample for GTK2</a>.<br/>
    To tryout the component, you can download one of the sample packages but to work with it you'll need to download the source package (which also contains the full documentation which is available also <a href="../../docs/webupdate/">here</a>).</p>


  <br/><?php write_h1("Documentation"); ?>
  <p>The documentation for this component is available online <a href="../../docs/webupdate/">here</a>.</p>


  <br/><?php write_h1("Known bugs"); ?>
  <p>None. To submit a bug report please look at <a href="../../support.php">wxCode support page</a>.</p>


  <br/><?php write_h1("Future enhancements"); ?>
  <p>- Allow in the REMOTE XML script to specify a "category" for the packages and use
a wxWebUpdateTreeCtrl instead of wxWebUpdateListCtrl to show the packages (and make
WebUpdater similar to Cygwin setup program). </p>

  <p>To submit a feature request please look at <a href="../../support.php">wxCode support page</a>.</p>


  <br/><?php require("footer.inc.php"); ?>
</html>