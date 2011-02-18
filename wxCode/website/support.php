<?php $logsys_disabled=TRUE; require("globals.inc.php"); ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<?php
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Author: Francesco Montorsi
  // Creation date: 30/6/2005
  // RCS-ID: $Id: support.php,v 1.11 2006/03/23 15:56:18 frm Exp $
  // Notes: some notes about the SF tracker usages and other support-related info....
  ////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="support";
        $menuentry="Support";
        $pagetitle="Support";
        require("header.inc.php");
    ?>


    <!-- SUPPORT SECTION -->
    <?php write_h1("Bugs and feature requests", "support", FALSE); ?>
    <p class="question">How can I submit a bug report for the component X ?</p>
    <p class="answer">Go to the <a href="http://sourceforge.net/tracker/?func=add&amp;group_id=51305&amp;atid=462816">wxCode bug tracker</a> and create a new tracker item named &quot;[X] bug description&quot;. Your submission should be handled by the relative wxCode maintainer as soon as possible.</p>
    <p class="question">How can I submit a feature request for the component X ?</p>
    <p class="answer">Go to the <a href="http://sourceforge.net/tracker/?func=add&amp;group_id=51305&amp;atid=462819">wxCode feature-request tracker</a> and create a new tracker item named &quot;[X] feature-request description&quot;. Your submission should be handled by the relative wxCode maintainer as soon as possible.</p>

    <br/><?php write_h1("Help on a specific component"); ?>
    <p>To get help about a specific component hosted at wxCode you should contact the maintainer using the email address
    linked in the <a href="complist.php">component list</a> or in the component website.</p>
    <p>Please keep in mind that wxCode hosts a lot of components and wxCode administrators cannot give help about a specific component...</p>

    <br/><?php write_h1("Mailing list"); ?>
    <p>The <strong>wxCode-users mailing list</strong> is sure the most active place where wxCode developers discuss about their software; you can <a href="http://sourceforge.net/search/index.php?type_of_search=mlists&amp;group_id=51305&amp;forum_id=39460">search the archives</a> or just <a href="http://lists.sourceforge.net/lists/listinfo/wxcode-users">subscribe it</a> and ask for help. Remember that <strong>the mailing list allows posts only by subscribed members</strong>!</p>

    <p>You can also browse it through <a href="http://news.gmane.org/gmane.comp.lib.wxwidgets.wxcode.user">GMane web interface</a> or <a href="http://search.gmane.org/index.php?group=gmane.comp.lib.wxwidgets.wxcode.user">search it</a>. More info about GMane services for wxCode mailing list can be found at <a href="http://dir.gmane.org/gmane.comp.lib.wxwidgets.wxcode.user">http://dir.gmane.org/gmane.comp.lib.wxwidgets.wxcode.user</a>. It's strongly suggested for <strong>occasional users</strong> to use <a href="http://news.gmane.org/gmane.comp.lib.wxwidgets.wxcode.user">GMane web interface</a> to post messages there as GMane allows you to subscribe the mailing list in a very easy way (after your first post to the mailing list you will receive a confirmation mail which just needs to be replied with empty body and you will then be subscribed).</p>

    <br/><?php write_h1("Forum"); ?>
    <p>The <strong><a href="http://wxforum.shadonet.com/viewforum.php?f=30">wxCode forum</a></strong> is a good place where you can find a lot of other wx developers and perhaps someone who had your same problem and can help you. Definitively don't miss it.</p>

    <br/><?php write_h1("Last resource: wxCode administrators"); ?>
    <p>As a last resort you can send a mail to one of wxCode administrators: <a href="mailto:utelle@nospam@users.sourceforge.net">Ulrich Telle</a> or <a href="mailto:frm@nospam@users.sourceforge.net">Francesco Montorsi</a>...</p>

    <p><b>Acknowlegdements</b></p>
    <p>Many thanks go to the past wxCode administrators for their great work:<br/>
    <ul>
    <li>Otto Wyss (2002 - 2006)</li>
    <li>John Labenski (2006 - 2007)</li>
    </ul>
    </p>

    <br/><?php require("footer.inc.php"); ?>
</html>
