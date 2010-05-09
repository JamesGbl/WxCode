<?php $logsys_disabled=TRUE; require("globals.inc.php"); ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 1/7/2005
// RCS-ID: $Id: rules.php,v 1.15 2008/06/14 12:27:22 frm Exp $
// Notes: the rules for component administration
////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
       $filename="rules";
       $pagetitle="Component hosting rules";
       $menuentry=writeMenuEntry("submit", "Submit your component");
       require("header.inc.php");
    ?>


    <!-- RULES SECTION -->
    <?php write_h1("Rules about component hosting at wxCode", "rules", FALSE); ?>
    <p>These are the basic rules you must agree to if you want to host your component at wxCode.<br/>
    Please keep in mind that these rules were created to <strong>avoid that wxCode becomes a &quot;dump&quot; of unmaintained and thus unusable code</strong>... We hope you'll find them to be both reasonable and acceptable.</p>
    <ol>
    <li>First and foremost; <strong>thank you</strong> for making your component publicly available! This means that other people can learn and benefit from your hard work and assist you in making it better.</li>

    <li>Each code snippet submitted to wxCode should follow the outlined <strong>directory structure of the <em>template</em> sample</strong> (you can see this by <a href="http://wxcode.cvs.sourceforge.net/wxcode/wxCode/template">browsing the wxCode CVS</a> or reading the <a href="template.php">template guide</a>) to allow for easy integration with the wxWidgets library, people's projects, and the other components. This means that you should create something with the usual "build", "docs", "include", "samples", "src", etc. directories. Please don't call your source folder &quot;mySoUrCeS&quot; or put your public header files in your source directory since people may have header name conflicts when they try to include them.</li>

    <li>Inside the <a href="http://wxcode.cvs.sourceforge.net/wxcode/wxCode/components">components</a> directory there is a subdirectory for each component. <strong>The name can be freely chosen but it has to consist of only lowercase letters and has to be unique within wxCode.</strong> But as long as nobody objects you're free to choose what ever you like.</li>

    <li>Each component gets the following services: <strong>webspace</strong> dedicated to the component presentation, to its screenshots, and to its documentation; an optional <strong>wiki</strong> which can also be used for the component presentation; a <strong>CVS</strong> folder where the component sources must be stored; <strong>space in the download server</strong> for component releases. Last, the component will be indexed in wxCode project database which is used by this website to list &amp; search the components.</li>

    <li>The maintainer must have a <strong>SourceForge account</strong> (this is required for committing the code in the CVS repository and for the handling of the web services) and should provide a valid email address to allow users of his component to eventually contact you for help.</li>

    <li>The maintainer should send a <strong><em>component status report</em></strong> to the <a href="http://lists.sourceforge.net/lists/listinfo/wxcode-users">wxcode-users</a> mailing list when they provide a new release of their component. This will help to generate interest and show users that your project is active. As SourceForge suggests, &quot;Release early; Release often&quot;.</li>

    <li>The maintainer should <strong><a href="http://wxcode.sourceforge.net/edit.php">regularly update the component info</a></strong> (wxWidgets supported versions, ports, etc.) and keep their component alive (fixing bugs, making new releases).<br/>A maintainer may resign at anytime; please have the courtesy to announce your intentions on the <a href="http://lists.sourceforge.net/lists/listinfo/wxcode-users">wxcode-users</a> mailing list instead of just &quot;disappearing&quot;. In this case, your code will be marked as unmaintained and others will be able to take over this maintainance task.</li>

    <li>Each component must be made available under the <strong><a href="http://www.opensource.org/licenses/wxwindows.php">wxWindows license</a></strong>. This is to ensure that there will be no misunderstandings by users of your code or clashes between components that may rely on each other. Remember; this code repository is designed to facilitate code reuse. Small third party libraries that you may include or link with your component will (of course) maintain their original license and you should note that in your documentation.</li>

    <li>The basic concepts you should keep in mind <strong>when writing your code</strong> are:
        <ul>
        <li>Make it easy for people to understand your code; add comments, documentation, and a sample program.</li>
        <li>Follow the <a href="http://www.wxwidgets.org/develop/standard.htm">wxWidgets coding standards</a>.</li>
        <li>Make your code flexible and extensible.</li>
        <li>Make integrating your component in other people's projects painless and therefore sucessful.</li>
        </ul>
    </li>
    </ol>

    <p>All these rules have been adopted to maintain <strong>high quality code</strong> at wxCode. If you think something is missing or that some rule doesn't make sense for your component or it's too strict, please discuss it in the <a href="http://lists.sourceforge.net/lists/listinfo/wxcode-users">wxCode-users mailing list</a>. We look forward helping you help the wxWidgets community and your advice is appreciated...</p>

    <br/><?php require("footer.inc.php"); ?>
</html>
