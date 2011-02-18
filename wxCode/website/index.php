<?php $logsys_disabled=TRUE; require("globals.inc.php"); ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 30/6/2005
// RCS-ID: $Id: index.php,v 1.23 2008/11/19 17:16:47 frm Exp $
// Notes: the index page which is used as "news page".
//        Embeds the Sourceforge news exported (in news.inc.php) through
//        the HTML DATA export facility as described at:
//        http://sourceforge.net/docman/display_doc.php?docid=1502&group_id=1
////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="index";
        $menuentry="News";
        $pagetitle="Homepage";
        require("header.inc.php");
    ?>


    <!-- ABOUT SECTION -->
    <?php write_h1("About wxCode", "introduction", FALSE); ?>
    <p>This site and the wxCode project are entirely dedicated to the support of the <a href="http://www.wxwidgets.org">wxWidgets</a> community; <strong>this site is dedicated to (wxWidgets) programmers only</strong>. You won't find programs to download here, just source code. If you are not a programmer and you are looking for Open Source software you can try to search in the <a href="links.php">links page</a>.</p>

    <p>wxCode hosts code snippets and libraries related to <a href="http://www.wxwidgets.org">wxWidgets</a>.
    The management of the various components is decided by their maintainers or by the wxCode administrators.
    A maintainer may resign anytime so any component can be marked as maintained or not; unmaintained components can be taken over by other developers.</p>

    <p><strong>wxCode tries to offer high-quality libraries and add-on components for wxWidgets</strong>, so that anytime a component is added, the author should commit himself in the process of maintenance of his code in respect to new versions of wxWidgets, bugs and problems reported by the users, etc.

    <p>To know more about wxCode management rules, look at the <a href="rules.php">rules</a> page; if you'd like to help, then you can look at the <a href="help.php">help</a> page.</p>

    <p>wxCode is a project hosted by <a href="http://www.sourceforge.net">SourceForge.net</a>; you can visit the <a href="http://sourceforge.net/projects/wxcode">wxCode project page</a> for additional info on this project.</p>

    <p>Currently the wxCode project has two alias domain names:</p>
    <ul>
    <li>The <b><a href="http://wxcode.com">wxCode.com</a></b> domain name is <strong>sponsored by <a href="http://angelo.mandato.com/">Angelo Mandato</a></strong>.</li>
    <li><p>The <b><a href="http://wxcode.org">wxCode.org</a></b> domain name is <strong>sponsored by Bryan Petty</strong>.</li>
    </ul>
    <p>Thanks !</p>


    <!-- INTRODUCTION SECTION -->
    <?php write_h1("About this website", "website", FALSE); ?>
    <p>This is a frame-free website; it uses XHTML 1.0 strict and CSS 2.0; if you have problems viewing this site with your browser you can change the style used to render this page clicking on the &quot;Frame-like&quot; link placed in the top-right corner of each page. A cookie will be saved to remember the CSS style you prefer so that you don't have to set it for each page or in each browser session.</p>
    <p>To wxCode maintainers: please note that your browser must have cookie support enabled for the login system to work.</p>

    <p><strong>Don't miss the <a href="complist.php">component list page</a> to view all of the
    component's descriptions, websites, and download pages.</strong>.</p>


    <!-- NEWS SECTION -->
    <br/><?php write_h1("News", "news"); ?>

    <div style="width: 80%; margin-left: 3em; margin-right: 3em;">
        <?php require("news.inc.html"); ?>
    </div>

    <div style="text-align: right">
    <p>&raquo; <a href="https://sourceforge.net/news/submit.php?group_id=51305">Submit a news item</a> &laquo;</p>
    </div>

    <br/><?php require("footer.inc.php"); ?>
</html>
