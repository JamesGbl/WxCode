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
      $compname = "wxExtMiniFrame";
      $subdir = "extminiframe";
      require("compwebsite.inc.php");
  ?>

  <br/><?php write_h1("Screenshots"); ?>
  <p>These screenshots have been taken from the sample program included with this component.</p>
  <p>This is a wxMainFrame (no aesthetic differences with a standard wxFrame) with two wxExtMiniFrames with
  some different wxMiniButtons in their titlebars, running on WinXP:</p>
  <div align="center"><?php writeScreenshotLink("winxp.png", "The wxExtMiniFrame on wxMSW"); ?></div>
  <p>Please note that, even if the cursor and the focus is given to the wxExtMiniFrame #2, also the wxMainFrame
  is drawn with an active titlebar: this is a trick which was hard to implement but it is quite common in the
  Win32 toolbars.<br/>
  This is the same sample running on Ubuntu under KDE/wxX11:</p>
  <div align="center"><?php writeScreenshotLink("x11.png", "The wxExtMiniFrame on wxX11"); ?></div>

  This is the same sample running on Ubuntu under KDE/wxGTK:</p>
  <div align="center"><?php writeScreenshotLink("gtk.png", "The wxExtMiniFrame on wxGTK"); ?></div>

  <br/><?php write_h1("Future enhancements"); ?>
  <p>The wxExtMiniFrame component is complete under wxMSW, wxGTK and wxX11. The only feature missing in the
  last two is the miniframe resizing system.</p>


  <?php write_h1("Documentation"); ?>
  <p>Sorry; I've written no docs for this component because its use should be quite intuitive. Also,
  the sample is quite heavily commented and thus you should be able to learn the additional features
  of wxExtMiniFrame (respect wxMiniFrame) looking at the sample source file.</p>


  <br/><?php write_h1("Known bugs"); ?>
  <p>Look at the Readme. To submit a bug report please look at <a href="../../faq.php">wxCode FAQ</a>.</p>


  <br/><?php write_h1("Future enhancements"); ?>
  <p>Miniframe resizing system for wxGTK and wxX11. wxMac support (I don't have a mac so for this I require the help of someone who has it). To submit a feature request please look at <a href="../../faq.php">wxCode FAQ</a>.</p>


  <br/><?php require("footer.inc.php"); ?>
</html>
