<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Author: Gary Harris
  // Creation date: 11/5/2010
  // RCS-ID: $Id: index.php 505 2007-03-31 10:31:46Z frm $
  // Notes: the index page of the website of this component
  ////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
	<?php
      $compname = "mmwx";
      $subdir = "mmwx";
      require("compwebsite.inc.php");
  ?>

	<!-- NOTE on the write_h1() php function:
         - first arg: the label of the h1 tag
         - second arg: the ID of the h1 tag (defaults to "")
         - third arg: if TRUE the TOP link is added; if FALSE it is omitted (defaults to TRUE)
         use FALSE as third arg for the first h1 tag of the page since it must
         not have the TOP link...
  -->


	<!-- NOTE on SCREENSHOTS:
       to write screenshots link with automatic thumbnail-generation, you can write something like:

       <?php writeScreenshotLink("myshot.png", "MYCOMP running with wxMSW 2.5.3"); ?>

	     where the 'myshot.png' file is a file that you have uploaded in
          /home/groups/w/wx/wxcode/htdocs/screenshots/MYCOMPSUBDIR
       you can do the upload with scp from unix or (I suggest), WinSCP (http://winscp.net)
       from win32... more info in the wxCode\template\readme.txt file.

       To put something centered in this page, put it inside a

          <div class="center">...</div>

       block...
  -->
	<br /><?php write_h1("Screenshots"); ?>
  <div class="center">
    <?php writeScreenshotLink("mmwx1.1.jpg", "mmwx test application running under wxMSW 2.8.10"); ?>
  </div>



  <!-- NOTE ON THE SECTIONS TO INSERT HERE: you are free to add anything you think
       that will make your component more appealing: some sample snippet of code,
       docs, bugs & feature requests... -->
<!--  <br /><?php write_h1("Usage sample"); ?> -->

  <?php 
  //writeSnippet('
// sample CPP snippet which shows how to use this component:
// myComp *newcomp = new myComp();
// newcomp->SetAAAA("aaa");
// newcomp->SetBBBB("bbb");

// inside this section we can use any character we want except
// for the single quote char: if you need to use it you should
// escape it in this way: \'
// newcomp->Show();
//'); 
?>
  <br /><?php write_h1("Notes"); ?>
  <p>
    Since wxODBC has been removed from wxWidgets 2.9, mmDbase is deprecated and I have not tested it. The suggested alternative is 
    <a href="http://debea.net/trac/wiki/WxDba">WxDba</a>.
  </p>
  <p>
    I have not been able to test mmMAPIMail as I do not have access to a MAPI server.
  </p>

  <!-- NOTE TO THE DOCUMENTATION: for uploading you component docs in the website,
       see the ReadMe.txt file at wxCode\template... -->
  <br /><?php write_h1("Documentation"); ?>
  <p>The documentation for this component is available online <a href="http://wxcode.sourceforge.net/docs/mmwx/">here</a>.</p>


	<br /><?php write_h1("Known bugs"); ?>
  <p>None. To submit a bug report please look at <a href="http://wxcode.sourceforge.net/support.php">wxCode support page</a>.</p>
  
   
	<br /><?php write_h1("Future enhancements"); ?>
<!--  <p>None. To submit a feature request please look at <a href="../../support.php">wxCode support page</a>.</p> -->
  <ul>
    <li>Check and compile on Linux and other OSs.</li>
  </ul>

	<br /><?php require("footer.inc.php"); ?>
</html>