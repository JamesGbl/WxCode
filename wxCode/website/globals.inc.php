<?php
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Author: Francesco Montorsi
  // Creation date: 5/7/2005
  // RCS-ID: $Id: globals.inc.php,v 1.17 2009/01/24 22:04:26 frm Exp $
  // Notes: defines some global utilities:
  //       - writeOptionItem(): writes an <option> tag for a <select> item
  ////////////////////////////////////////////////////////////////////////////////////////////

// we absolutely need to avoid that this file outputs some HTML
// code since some files which include this one need to send
// cookies or header informations *before* anything else has been
// sent (like spaces, newlines, ... everything !).
// See the PHP manual for the setcookie() or header() functions.

define("SITEBASEURL", "http://wxcode.sourceforge.net/");
define("SERVERBASEURL", "/home/groups/w/wx/wxcode/htdocs/");
define("FILEBASEURL", SITEBASEURL);
define("WIKIBASEURL", "http://wiki.wxwidgets.org/");

// this are the only characters accepted in isStrValid.
define("VALID_CHARS", "abcdefghijklmnopqrstuvwxyz-_ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
define("LOWERCASE_CHARS", "abcdefghijklmnopqrstuvwxyz-_0123456789");

function writeOptionItem($sn, $ln, $var)
{
  /*echo "var is $var; sn is $sn";*/
  if (strpos($var, $sn) === FALSE)      // the three = are very important !
    echo "<option value='$sn'>$ln</option>";
  else
    echo "<option value='$sn' selected='selected'>$ln</option>";
}

function writeMenuEntry($parentfilename, $parentlabel)
{
  global $filename, $pagetitle;     // these should have already been defined
  assert(isset($filename) and isset($pagetitle));
  return "<a href='$parentfilename.php'>$parentlabel</a> &raquo; <a href='$filename.php'>$pagetitle</a>";
}

/* here we provide a general PHP function to write H1 tags with 'go to TOP' embedded links */
function write_h1($text, $id = "", $withtoplink = TRUE)
{
    if ($withtoplink == FALSE)    // this means this is the first H1 section of the page...
        echo "<br/>";             // ... leave some empty space then...

    /* to be sure that the content of H1 tag and the TOP link are shown
       in the same line we'll use a little table */
    $id = trim($id);
    if (empty($id))
        $h1id = "";
    else
        $h1id = " id='$id'";

    echo "<table class='h1like' cellspacing='0' cellpadding='0'>";
    echo "<tr><td><h1 $h1id>$text</h1></td>";

    if ($withtoplink)
        echo "<td><a class='topOfPage' href='#top' title='Go to the top of this page'>^ TOP</a></td>";
    echo "</tr></table>";
}

function writeAutoSubmit() {
    global $autosubmit;
    if ($autosubmit)
      echo "onchange='this.form.submit();'";
}

function convertArrayToStr($arr)
{
    $str = "";
    for ($i=0; $i <= count($arr); $i++) {
        if ($arr[$i] == "") continue;   // skip this token
        if ($str != "") $str .= ",";
        $str .= $arr[$i];
    }
    return $str;
}

function convertStrToArray($str)
{
    $arr = array();
    $arr = explode(",", $str);
    return $arr;
}

// Returns TRUE if the given string contains only valid characters
function isStrValid($str)
{
   $len = strlen($str);   // compute only once
   if ($len < 1 or $len > 60)
      return FALSE;   // invalid size !

   for ($i=0; $i < $len; $i++)
     if (strpos(VALID_CHARS, $str{$i}) === FALSE)
        return FALSE;     // we've found a non-valid char
   return TRUE;
}

// Returns TRUE if the given string contains only lowercase characters
function isStrLowercase($str)
{
   $len = strlen($str);   // compute only once
   if ($len < 1 or $len > 60)
      return FALSE;   // invalid size !

   for ($i=0; $i < $len; $i++)
     if (strpos(LOWERCASE_CHARS, $str{$i}) === FALSE)
        return FALSE;     // we've found a non-valid char
   return TRUE;
}





// GLOBAL CODE
// -----------------------------------


if (!isset($logsys_disabled))
    $logsys_disabled = FALSE;     // default value

if ($logsys_disabled) {

    // we don't care about login system in this page...
    require_once("dbconnection.inc.php");

} else {

    // log in stuff
    //session_cache_limiter('nocache');       sometimes this gives problems about "Failed to initialize storage module"...
    session_start();

    require_once("dbconnection.inc.php");
    require_once("login.inc.php");
}

?>
