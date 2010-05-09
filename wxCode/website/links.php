<?php $logsys_disabled=TRUE; require("globals.inc.php"); ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 30/6/2005
// RCS-ID: $Id: links.php,v 1.18 2008/11/13 16:28:03 frm Exp $
// Notes: the Links page of wxCode
////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="links";
        $menuentry="Links";
        $pagetitle="Useful links";
        require("header.inc.php");
    ?>


    <!-- LINKS SECTION -->
    <?php write_h1("Links to wxCode resources", "links", FALSE); ?>
    <p>Please note that <strong>all components and their screenshots, documentations, source repositories and downloads are linked in the <a href="complist.php">component list</a> page</strong>...</p>
    <ul>
    <li><a href="http://sourceforge.net/projects/wxcode">wxCode project page</a>: the project page at Sourceforge.net</li>
    <li><a href="http://sourceforge.net/project/stats/index.php?group_id=51305&amp;ugn=wxcode&amp;type=&amp;mode=alltime">wxCode statistics</a>: see the statistics for wxCode since it was created !</li>
    <!--<li><a href="http://wxcode.sourceforge.net/wiki/pmwiki.php">wxCode wiki</a>: the wiki of wxCode; leave your opinion on the components you like !</li>-->
    <li><a href="http://lists.sourceforge.net/lists/listinfo/wxcode-users">wxCode-users mailing list</a>: the mailing list for wxCode maintainers and component users</li>
    <li><a href="http://sourceforge.net/search/index.php?type_of_search=mlists&amp;group_id=51305&amp;forum_id=39460">wxCode-users search form</a>: quick link to search among wxCode-users archives</li>
    <li><a href="http://sourceforge.net/mailarchive/forum.php?forum=wxcode-users">wxCode-users archives</a>: the archives browsable online</li>
    <li><a href="http://sourceforge.net/tracker/?atid=462816&amp;group_id=51305&amp;func=browse">wxCode bug tracker</a></li>
    <li><a href="http://sourceforge.net/tracker/?atid=462818&amp;group_id=51305&amp;func=browse">wxCode patch tracker</a></li>
    <li><a href="http://sourceforge.net/tracker/?atid=462819&amp;group_id=51305&amp;func=browse">wxCode feature requests tracker</a></li>
    </ul>

    <!-- Site map -->
    <?php write_h1("wxCode site map", "links", FALSE); ?>
    <ul>
    <h2>General pages</h2>
    <li><a href="index.html">Main page</a>: The wxCode main page; info about this project</li>
    <li><a href="faq.php">FAQ</a>: Frequently asked questions about wxCode</li>
    <li><a href="help.php">Help wanted</a>: Do you want to help wxCode or maintain a component?</li>
    <li><a href="support.php">Support</a>: Learn where to get support for wxCode and the components</li>
    <li><a href="statistics.php">Statistics</a>: Information about the wxCode site and components</li>

    <h2>Component pages</h2>
    <li><a href="complist.php">Component browser</a>: Browse through the wxCode components</li>
    <li><a href="singlepagelist.php">Component list</a>: A single page list of all the wxCode components</li>
    <li><a href="search.php">Component search</a>: Search for a wxCode component</li>
    <li><a href="categories.php">Component categories</a>: Learn what is meant by the component categories</li>

    <h2>Developer pages</h2>
    <li><a href="login.php">Login</a>: Developer login for the wxCode website</li>
    <li><a href="logout.php">Logout</a>: Developer logout for the wxCode website</li>
    <li><a href="register.php">Developer registration</a>: Create a developer account on wxCode</li>
    <br>
    <li><a href="edit.php">Edit your component</a>: Edit the website info about your component (must login)</li>
    <li><a href="submit.php">Submission guide</a>: Learn about submitting a new component</li>
    <li><a href="compsubmit.php">Submit a new component</a>: A form to submit a new wxCode component</li>
    <br>
    <li><a href="rules.php">Maintainer rules</a>: Rules to follow as a developer on wxCode</li>
    <li><a href="maintguide.php">Maintainer guide</a>: Guide for maintaining your wxCode project</li>
    <li><a href="template.php">Project template</a>: Guide to setup your component to work well with wxCode</li>
    <li><a href="bakefile.php">Bakefile guide</a>: How to use wxCode's bakefiles to generate your build files</li>
    <li><a href="autoconf.php">Autoconf guide</a>: How to use wxCode's autoconf to generate a configure script</li>
    </ul>

    <?php write_h1("Links to wxWidgets-related site", "links", FALSE); ?>
    <ul>
    <li><a href="http://www.wxwidgets.org">http://www.wxwidgets.org</a>: the official wxWidgets site</li>
    <li><a href="http://wiki.wxwidgets.org">http://wiki.wxwidgets.org</a>: the official wxWidgets wiki</li>
    <li><a href="http://wxforum.shadonet.com">http://wxforum.shadonet.com</a>: the wxWidgets forum (you can also jump to the <a href="http://wxforum.shadonet.com/viewforum.php?f=30">wxCode section</a> it contains directly)</li>
    <li><a href="http://groups-beta.google.com/group/comp.soft-sys.wxwindows">http://groups-beta.google.com/group/comp.soft-sys.wxwindows</a>: the Google search for the wxWidgets newsgroup (which is synchronized with the wx-users mailing list)</li>
    <li><a href="mailto:wx-users@lists.wxwidgets.org">wx-users@lists.wxwidgets.org</a>:
    the wx-users mailing list</li>
    </ul>

    <br/><?php write_h1("wxCode resources for administrators"); ?>
    <ul>
    <li><a href="https://phpmyadmin-w.sourceforge.net/">phpMyAdmin for wxCode</a>: to manage wxCode database</li>
    <li><a href="http://wxcode.sourceforge.net/toapprove.php">Components to approve</a>: the list of components pending approval</li>
    <li><a href="http://wxcode.sourceforge.net/backup/">Backups</a>: automatic backups</li>
    </ul>

    <br/><?php require("footer.inc.php"); ?>
</html>
