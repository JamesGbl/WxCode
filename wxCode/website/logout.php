<?php require("globals.inc.php");

if (isset($_COOKIE['cookname']) && isset($_COOKIE['cookpass'])) {

    /**
    * Delete cookies - the time must be in the past,
    * so just negate what you added when creating the
    * cookie.
    */
    setcookie("cookname", "", time()-60*60*24*100);
    setcookie("cookpass", "", time()-60*60*24*100);
   
} else {

    /* the cookies are not set: we are already logged-out or the user didn't choose the "remember me" feature */
}

// this is to avoid that header.inc.php tells us that we are logged in when we are not
$old_logged_in = $logged_in;
$logged_in = FALSE;

?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<?php
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Author: JP, Francesco Montorsi
  // Creation date: 8/7/2005
  // RCS-ID: $Id: logout.php,v 1.4 2005/09/03 23:14:52 frm Exp $
  // Notes: the log out stuff.
  //        Taken from http://www.evolt.org/article/
  //          PHP_Login_Script_with_Remember_Me_Feature/17/60265/index.html
  ////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="logout";
        $menuentry="Log out";
        $pagetitle="Log out";
        require("header.inc.php");

        if ($old_logged_in == FALSE) {

           write_h1("Log out", "", FALSE);
           echo "<p class=\"error\">Error: you are not currently logged in; logout failed.</p>";
           
        } else {

           /* Kill session variables */
           //unset($_SESSION['username']);
           //unset($_SESSION['password']);
           $_SESSION = array(); // reset session array
           session_destroy();   // destroy session.

           write_h1("Log out", "", FALSE);
           echo "<p>You have successfully <strong>logged out</strong>.</p>";
        }
  ?>

    <br/><?php require("footer.inc.php"); ?>
</html>
