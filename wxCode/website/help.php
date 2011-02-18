<?php $logsys_disabled=TRUE; require("globals.inc.php"); ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 30/6/2005
// RCS-ID: $Id: help.php,v 1.6 2009/01/30 12:08:04 frm Exp $
// Notes: some hints for those people who want to be the maintainers of an
//         abandoned component...
////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="help";
        $pagetitle="Help wanted";
        $menuentry=writeMenuEntry("about", "About wxCode");
        require("header.inc.php");
    ?>


    <!-- HELP SECTION -->
    <?php write_h1("Help wanted", "help", FALSE); ?>
    <p>Do you need to use a component marked as UNMAINTAINED?
    Then why don't you ask to be registered as the maintainer of that component?</p>
    <p>As explained in <a href="rules.php">wxCode rules</a> all unmaintained components can be 'resurrected' if someone opts in as its maintainer. Unmaintained components can still be greatly useful since most of them still work. They are also a good occasion to learn from code written by other experienced users of wxWidgets...</p>


    <br/><?php write_h1("4 steps to take over the maintainance of a component"); ?>
    <ol>
    <li>Take a look at the <a href="http://wxcode.svn.sourceforge.net/viewvc/wxcode/trunk/wxCode/components/">Subversion</a> repository of the component you want to maintain: be sure to be able to understand the work of the original author of the code so that you can make bugfixes and improvements...</li>
    <li>Send a message to <a href="mailto:wxcode-users@lists.sourceforge.net">wxCode-users mailing list</a> saying that you would be interested to take over the maintenance of component X. Eventually provide a short list of the modifications you think that the component needs (in order to update it to the latest wxWidgets version, etc...).</li>
    <li>After the wxCode administrators reply, you should <a href="register.php">register as wxCode maintainer</a>. Given your wxCode maintainer ID (you can see that in the <a href="http://wxcode.sourceforge.net/edit.php">edit page</a>) to wxCode administrators you will be set as the maintainer for component X.</li>
    <li>Read the <a href="maintguide.php">wxCode maintainer guide</a> to know how to use all the services provided by wxCode and SourceForge...</li>
    </ol>


    <br/><?php require("footer.inc.php"); ?>
</html>
