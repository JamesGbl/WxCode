<?php require("globals.inc.php"); ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: JP, Francesco Montorsi
// Creation date: 8/7/2005
// RCS-ID: $Id: login.php,v 1.7 2005/09/03 23:14:52 frm Exp $
// Notes: the log in stuff.
//        Taken from http://www.evolt.org/article/
//          PHP_Login_Script_with_Remember_Me_Feature/17/60265/index.html
////////////////////////////////////////////////////////////////////////////////////////////
?>
  
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="login";
        $menuentry="Log in";
        $pagetitle="Log in";
        require("header.inc.php");

        displayLogin();
  ?>

    <br/><?php require("footer.inc.php"); ?>
</html>
