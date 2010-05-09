<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: JP, Francesco Montorsi
// Creation date: 8/7/2005
// RCS-ID: $Id: login.inc.php,v 1.8 2005/09/04 11:49:49 frm Exp $
// Notes: the log in stuff.
//        Taken from http://www.evolt.org/article/
//          PHP_Login_Script_with_Remember_Me_Feature/17/60265/index.html
////////////////////////////////////////////////////////////////////////////////////////////


/**
 * Checks whether or not the given username is in the
 * database, if so it checks if the given password is
 * the same password in the database for that user.
 * If the user doesn't exist or if the passwords don't
 * match up, it returns an error code (1 or 2).
 * On success it returns 0.
 */
function confirmUser($username, $password) {
   //echo "################################################# confirming user...";

   /* be sure that password & username are valid */
   if (!isStrValid($username) or !isStrValid($password))
      die("Invalid username &quot;$username&quot; and/or password &quot;$password&quot; ! Contact webmaster...");

   /* Verify that user is in database */
   global $dbconn;
   db_connect();    // be sure to be connected
   $q = "SELECT password FROM maintainers WHERE sfuser='$username'";
   $result = mysql_query($q, $dbconn);
   if($result == FALSE || mysql_numrows($result) < 1)
      return 1;         // Indicates username failure

   /* Validate that password is correct */
   $dbarray = mysql_fetch_array($result);
   if ($password == $dbarray['password'])
      return 0;       // Success! Username and password confirmed
   else
      return 2;       // Indicates password failure
}

/**
 * checkLogin - Checks if the user has already previously
 * logged in, and a session with the user has already been
 * established. Also checks to see if user has been remembered.
 * If so, the database is queried to make sure of the user's
 * authenticity. Returns true if the user has logged in.
 */
function checkLogin(){
//    echo "################################################# checkLogin...";

   /* Check if user has been remembered */
   if (isset($_COOKIE['cookname']) && isset($_COOKIE['cookpass'])) {

      // load cookies data
      $_SESSION['username'] = $_COOKIE['cookname'];
      $_SESSION['password'] = $_COOKIE['cookpass'];
   }

   /* Username and password have been set ? */
   if (isset($_SESSION['username']) && isset($_SESSION['password'])) {

      /* Confirm that username and password are valid */
      if(confirmUser($_SESSION['username'], $_SESSION['password']) != 0){

         /* Variables are incorrect, log out the user */
         unset($_SESSION['username']);
         unset($_SESSION['password']);
         return FALSE;
         
      } else {
      
         /* the _SESSION var already contains the password & username and they proved to be valid */
         return TRUE;
      }
   }

   /* User not logged in */
   return FALSE;
}

/**
 * Determines whether or not to display the login
 * form or to show the user that he is logged in
 * based on if the session variables are set.
 */
function displayLogin(){
   // echo "################################################# displayLogin...";
   global $logged_in, $login_errmsg;
   if ($logged_in) {

      assert(isset($_SESSION['username']) and isset($_SESSION['password']));
      write_h1("Logged in", "", FALSE);
      echo "<p>Welcome <strong>$_SESSION[username]</strong>, you are logged in.<br/>";
      echo "In case you want to logout, click <a href=\"logout.php\">here</a>.</p>";
      
   } else {

      assert(!isset($_SESSION['username']) and !isset($_SESSION['password']));
      write_h1("Authenticate as wxCode component maintainer", "auth", FALSE);
      
      // did the user already tried to authenticate ?
      if (!empty($login_errmsg))
          echo "<p class=\"error\">$login_errmsg</p>";
          
      // show the log in form which will redirect the user to this same PHP file
?>

  <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
    <?php require("auth.inc.php"); ?>
  </form>
  
<?php
   }
}

/**
 * Checks to see if the user has submitted his
 * username and password through the login form,
 * if so, checks authenticity in database and
 * creates session.
 */
function checkLoginFormSubmission()
{
    //echo "################################################# login.inc.php...";
 
    if (!isset($_POST['sublogin']))
      return FALSE;
      
    // the user has just sent the login form to this file... check it
    global $login_errmsg;

    /* Check that all fields were typed in */
    if (empty($_POST['user']) || empty($_POST['pass'])) {
        $login_errmsg = 'You didn\'t fill in a required field.';
        return FALSE;
    }
    
    /* Check username */
    if (!isStrValid($_POST['user'])) {
        $login_errmsg = "Sorry, the username &quot;$_POST[user]&quot; is not valid; it must be shorter than 30 characters and it must " .
                    "contain only the following characters: &quot;" . VALID_CHARS . "&quot;";
        return;
    }
    
    /* Check password */
    if (!isStrValid($_POST['pass'])) {
        $login_errmsg = "Sorry, the password is not valid; it must be shorter than 30 characters and it must " .
                    "contain only the following characters: &quot;" . VALID_CHARS . "&quot;";
        return;
    }
    
    /* Checks that username is in database and password is correct */
    $md5pass = md5($_POST['pass']);
    $result = confirmUser($_POST['user'], $md5pass);

    /* Check error codes */
    if($result == 1) {
      $login_errmsg = 'That username doesn\'t exist in our database.';
      return;
    } else if ($result == 2) {
      $login_errmsg = 'Incorrect password, please try again.';
      return;
    }

    /* Username and password correct, register session variables */
    $_SESSION['username'] = $_POST['user'];
    $_SESSION['password'] = $md5pass;

    if (isset($_POST['remember'])) {

        /**
        * This is the cool part: the user has requested that we remember that
        * he's logged in, so we set two cookies. One to hold his username,
        * and one to hold his md5 encrypted password. We set them both to
        * expire in 100 days. Now, next time he comes to our site, we will
        * log him in automatically.
        */
        setcookie("cookname", $_SESSION['username'], time()+60*60*24*100);
        setcookie("cookpass", $_SESSION['password'], time()+60*60*24*100);
    }

    return TRUE;      // login form contained valid data; session has been saved
}


/* these are the global action which are executed by all the files which include
   this login.inc.php file */
checkLoginFormSubmission();

/* Sets the value of the logged_in variable, which can be used in your code */
$logged_in = checkLogin();

?>
