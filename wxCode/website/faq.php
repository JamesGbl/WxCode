<?php $logsys_disabled=TRUE; require("globals.inc.php"); ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<?php
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Author: Francesco Montorsi
  // Creation date: 30/6/2005
  // RCS-ID: $Id: faq.php,v 1.22 2007/04/05 09:51:24 frm Exp $
  // Notes: the Frequently Asked Questions of wxCode
  ////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="faq";
        $menuentry="FAQ";
        $pagetitle="Frequently Asked Questions";
        require("header.inc.php");
    ?>

    <!-- FAQ SECTION -->
    <?php write_h1("About components", "faq", FALSE); ?>
    <p id="1" class="question">1) Can I access the components from Subversion ?</p>
    <p class="answer">Sure! You just need a configured Subversion client, see <a href="http://sourceforge.net/svn/?group_id=51305">this Sourceforge SVN doc</a>.
    When reading the document, refer to the &quot;About&quot; and &quot;Anonymous access&quot; sections. Our suggested Subversion client is <a href="http://tortoisesvn.tigris.org/">TortoiseSVN</a>.</p>

    <p class="answer">Please note that you can also browse the Subversion repository of a component online just following the links in the <a href="complist.php">component list page</a>.</p>

    <p id="2" class="question">2) What is the best way to get or use the wxCode components ?</p>
    <p class="answer">It is recommended that you use SVN to get a copy of wxCode or just the individual components you use. Doing it this way places all the components in the same place, making it easier for you to use them in your own project.</p>
    <p class="answer">If you find some issues with a component or just want to enhance it, you'll be able to provide diffs for sending <strong>patches</strong> (see question below) to the maintainer and then retrieve the updated version. We do however recommend that maintainers regularly create releases which can be downloaded from SourceForge and this is also a reasonable way to obtain the components you want. These <strong>releases should be considered stable versions</strong>. </p>
    <p class="answer">As a last resort, you could copy the files into your own project, but you'll have to keep track of what you've got and manage your own build system. We strive to have each project have their own build files that will create a library to link to and using these is obviously recommended.</p>


    <p id="3" class="question">3) How can I submit my modifications back to the maintainer ?</p>
    <p class="answer">In order to ensure a fast and easy processing of your modifications by the wxCode maintainers, you should strive to submit your modifications in the form of <strong>patches</strong> (a patch is something that we could apply with one command, not arbitrary text that we'd spend hours trying to understand):

    <ol>
    <li>If you haven't already done so, get the <strong>SVN</strong> repository of the component. Look at FAQ #1 for more details.</li>

    <li>Under <strong>Windows</strong> you can tipically use your Subversion's &quot;Patch&quot; function (e.g. for TortoiseSVN you can right-click the component's folder from Explorer and choose &quot;Make patch&quot; or similar); save the patch somewhere (e.g. on the desktop) with a sensible name (possibly with extension .patch).<br/><br/>

    Under <strong>Unix</strong>-like operating systems, open a &quot;terminal&quot; and type:
        <pre class="sourcecode">svn diff --diff-cmd="diff" --extensions="-buw" >mypatch.patch</pre>
    </li>

    <li>Open the just-created file with any plain text <strong>editor</strong> and see if it <em>looks</em> correct (i.e. contains all modifications you want to sent to the maintainer).
    </li>

    <li><strong>Submit your patch</strong> <a href="http://sourceforge.net/tracker/?atid=462818&group_id=51305&func=browse">here</a> (after logging in - i.e. you'll need a SourceForge user account - that's free and quick to create), <strong><em>attaching</em></strong> it and not copying-and-pasting.<br/><br/>
    Also please describe up to a reasonable level of detail what the patch does and why and name the patch item as &quot;<em>[Component Name] subject of the patch</em>&quot; so that the relative maintainer can easily spot patches affecting his component.
    </li>
    </ol>

    <p id="4" class="question">4) How to submit a bug/feature-request for a specific component ?</p>
    <p class="answer">See the <a href="support.php">support page</a>.</p>



    <br/><?php write_h1("About wxCode"); ?>

    <p id="5" class="question">5) Why should I host my component at wxCode when I can create my own Sourceforge project ?</p>
    <p class="answer">You should create a separate SourceForge project if your component is very large and can sustain it's own community of users. If your component is small to medium sized, there are various advantages for hosting it at wxCode:</p>
    <ul>
    <li>wxCode is a <strong>well-known place</strong> for developers using wxWidgets to look for code. It's more likely that you find someone who will appreciate (and find) your work here.</li>
    <li>Since wxCode contains a number of components, people can pick and choose among them. If they use SVN, they won't have to download each package individually and deal with the hassle of managing them separately.</li>
    <li>You can use the <strong>wxCode bakefiles</strong> to easily write the build systems for your components.  The wxCode bakefiles provide all the basic stuff for a typical component.</li>
    <li>You already have a website <strong>skeleton</strong> for your component and <strong>documentation</strong> specific for setting up a wxWidgets-based cross-platform component.</li>
    </ul>
    <p>In conclusion, hosting your component at wxCode requires much less work and thus is a lot <strong>faster for you</strong> and easier for others to find (and use it).</p>

    <p id="6" class="question">6) The component list or something else in the site is not working as expected. What can I do?</p>
    <p class="answer">Report the errors/warnings you get in the component list page as
    a wxCode bug... see the <a href="support.php">support page</a>...</p>



    <br/><?php write_h1("About component administration"); ?>

    <p id="7" class="question">7) How can I access the SVN repository for my component? How can I make a release for my component? How can I make the website for my component? How can I add a Bakefile-based build system to my component?</p>
    <p class="answer">See the <a href="maintguide.php">wxCode maintainer guide</a> and eventually ask for help on the <a href="http://lists.sourceforge.net/lists/listinfo/wxcode-users">wxCode-users mailing list</a>...</p>

<!--    
    <p id="8" class="question">How can I create a wiki-based website for my component ?</p>
    <p class="answer">Just go in the <a href="http://wxcode.sourceforge.net/wiki">wxCode wiki</a> and create a new link to your component page in the sidebar menu. Then edit your component page as you would do in any other wiki. To make it easier (for you) to update your component info you should not writ ein the wiki (which is not connected to the wxCode database) some things like the most updated version of your component; instead you should give to the user a link to your component database info:</p>
    <p class="code">http://wxcode.sourceforge.net/showcomp.php?name=MYCOMP</p>
    <p class="answer">Common things to put in the wiki: TODO lists, feature request lists (alternative to the SF feature request tracker), a space for user's comments...</p>
-->

    <p id="9" class="question">8) How should I ship resources embedded in my components: as XRC files or as hardcoded build functions ?</p>
    <p class="answer">XRC resources allow you to keep separed the GUI and the code which handle it and, most important, they also make it possible for the end user to edit the resources of your component without modifying the code and possibly using a graphical XRC editor (wxGlade, wxDesigner... etc). However they force your component's user to link its program also with wxXML and wxXRC libraries. Hardcoded build functions instead has some speed advantage over XRC and do not require any additional linkage but they are much more difficult to modify and customize for users' needs. You are free to choice the resource build system you prefer but take in count that XRC system is widely used and in the long run is much more flexible...</p>

    <br/><?php require("footer.inc.php"); ?>
</html>
