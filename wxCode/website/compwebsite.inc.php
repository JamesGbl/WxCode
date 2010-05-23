<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 8/7/2005
// RCS-ID: $Id: compwebsite.inc.php,v 1.14 2005/09/04 19:54:46 frm Exp $
// Notes: includes everything required for a component website...
////////////////////////////////////////////////////////////////////////////////////////////

// this file must be included *after* defining these two vars;
// see wxCode\template\website\index.php for more info...
assert(isset($compname) and isset($subdir));

$filename="index";
$filelocation="components/$subdir";
$pagetitle=$compname;
$logsys_disabled=TRUE;   // don't care about log status in component pages

require("globals.inc.php");
$menuentry=writeMenuEntry("../../complist", "Components");
require("header.inc.php");
require_once("dboutput.inc.php");

if (!isset($writesynopsis) or $writesynopsis == TRUE) {
    write_h1($compname, "", FALSE);
    require("dbquery.inc.php");
    DbOutput::comp($compname);
    echo "<br/>\n";
}

// this is used by component websites to create links to their screenshots:
// < ? php writeScreenshotLink("myimage.png", "description", 200); ? >
//
// NOTE: remove the spaces between the <, ? symbols in the example line above
/*function writeScreenshotLink($img, $desc, $width = 150, $class = "")
{
    global $compname, $subdir;
    echo "<a href=\"../../screenshots/$subdir/$img\">";
    echo "<img class='$class' alt='$desc' " .
          "src='../../phpTumb/phpThumb.php?src=../screenshots/$subdir/$img&amp;w=$width'/>";
    echo "</a>";
}*/
function writeScreenshotLink($img, $desc, $width = 150, $class = "")
{
    global $compname, $subdir;

    echo "<a href=\"../../screenshots/$subdir/$img\" rel=\"lightbox\" title=\"$img\">\n";
    echo "  <img class='$class' border=\"0\" alt='$desc' src='../../phpTumb/phpThumb.php?src=../screenshots/$subdir/$img&amp;w=$width'/>\n";
    echo "</a>\n";
}


// this is used by component websites to create syntax highlighted snippets of code:
//
function writeSnippet($str)
{
    // some beautifier config stuff
    global $BEAUT_PATH;
    $BEAUT_PATH = "/home/groups/w/wx/wxcode/htdocs/beautifier/php";
    require_once("$BEAUT_PATH/Beautifier/Core.php");
    require_once("$BEAUT_PATH/HFile/HFile_cpp.php");
    require_once("$BEAUT_PATH/Output/Output_css.php");

    // allocate the highlighter class
    $highlighter = new Core(new HFile_cpp(), new Output_css());
    echo "<pre class='sourcecode'>\n";
    print $highlighter->highlight_text($str);
    echo "</pre>\n\n";
}
?>

