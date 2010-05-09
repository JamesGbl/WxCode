<?php require("globals.inc.php"); ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi and TOWRITE
// Creation date: 18/7/2005
// RCS-ID: $Id: maintguide.php,v 1.24 2008/06/18 18:45:02 frm Exp $
// Notes: the wxCode Maintainer Guide
////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="maintguide";
        $menuentry="Maintainer guide";
        $pagetitle="Maintainer guide";
        require("header.inc.php");
    ?>


    <!-- THE MAINTAINER GUIDE -->
    <?php write_h1("wxCode Maintainer Guide", "maintguide", FALSE); ?>
    <p>NOTE: this document applies only to the wxCode maintainers.</p>

    <p><strong>Index of the page:</strong></p>
    <ul>
      <li><a href="#cvs">STEP #1: set up the CVS for your component</a><br/>
         &rsaquo; <a href="#cvs_linux">For Unix-based OS</a><br/>
         &rsaquo; <a href="#cvs_win">For Windows-based OS</a><br/>
         &rsaquo; <a href="#cvs_alt">For both Linux-based and Windows-based OS</a>
      </li>

      <li><a href="#svn">STEP #2: set up the SVN for your component</a><br/>
         &rsaquo; <a href="#svn_linux">For Unix-based OS</a><br/>
         &rsaquo; <a href="#svn_win">For Windows-based OS</a><br/>
         &rsaquo; <a href="#merge_cvssvn">How to merge wxCode CVS with wxCode SVN</a><br/>
      </li>


      <li><a href="#customize">STEP #3: upload your sources</a><br/>
         &rsaquo; <a href="template.php">Template guide</a><br/>
         &rsaquo; <a href="bakefile.php">Bakefile guide</a><br/>
         &rsaquo; <a href="autoconf.php">Autoconf guide</a>
      </li>

      <li><a href="#release">STEP #4: make a release for your component</a><br/>
         &rsaquo; <a href="#release_first">First ever file release of your project</a><br/>
         &rsaquo; <a href="#release_sub">Subsequent file releases</a><br/>
      </li>
    </ul>





    <!-- =================================================================================== -->
    <!--                                 STEP #1                                             -->
    <!-- =================================================================================== -->

    <br/><?php write_h1("STEP #1: set up the CVS for your component", "cvs"); ?>

    <p><strong>Note:</strong>
        since some developers prefer to use <a href="http://subversion.tigris.org/">Subversion</a>
        instead of <a href="http://www.nongnu.org/cvs/">CVS</a>, some wxCode components are
        hosted in the CVS repository, other in the
        Subversion repository.<br/>
        So, you need to setup Subversion only if your component is there or if
        it uses components placed in the Subversion repository or if you want to get all wxCode
        components on your hard disk.
    </p>

    <p id="cvs_linux"><strong>&rsaquo; For Unix-based OS:</strong></p>
    <div style="margin-left: 1em">
    <p>You will need to have cvs installed and set up on your system. You will
    also need a <strong>SSH client</strong> such as <tt>OpenSSH</tt> installed and set up.
    A modern linux distribution will already have both. Finally you will need a
    sourceforge username (here referenced as <tt>mysfname</tt>) and a password.</p>

    <p>Here is a small tutorial which will guide you in the process. Open a shell (e.g. <tt>Konsole</tt> or <tt>gnome-terminal</tt>) and copy and paste (comments won't harm) the following commands:</p>
    <pre class="sourcecode">
        cd                   # this brings to the home
        export CVS_RSH=ssh   # this tells cvs to communicate using ssh
        cvs -d:ext:mysfname@wxcode.cvs.sourceforge.net:/cvsroot/wxcode co wxCode

        # now you will be asked for your password: give the password of your SourceForge account.
        # Then all of the wxCode modules will be downloaded, 58 MB at May 2006
        # (aren't you <i>glad</i> you have broadband?).
    </pre>

    <p>Now you have the <tt>~/wxCode/components/YOURCOMPONENT</tt> folder; it should contain a standard tree like the one of the &quot;wxCode\template&quot; directory. Copy into it the files that comprise your module and change/remove existing files to suit your needs. Tell cvs about your project files with a command like:</p>

    <pre class="sourcecode">
        cvs add myfile.1 myfile.2          # tell CVS about new files
        cvs commit -m "Initial commit"     # commit them to the repository
    </pre>

    <p>The bit in quotes is used by cvs as a label for your future reference.
    You can substitute a message of your choice, or "" for none.  If you
    omit the -m "something" then your default editor will be summoned so
    that you can compose the message there.  After a delay while the
    sourceforge server gets updated, all should be well.</p>

    <p>Notice that the initial checkout required the long form of command with
    <tt>-d:ext:mysfname@wxcode.cvs.sourceforge.net:/cvsroot/wxcode</tt>.  Afterwards this is
    unnecessary as it is stored in your local cvs, so as long as you are in
    a directory below <tt>/home/me/wxCode</tt>, you won't need to use that option. Your password
    is <strong>not</strong> remembered though, so you will be asked for it with each command.</p>
    </div>



    <br/><p id="cvs_win"><strong>&rsaquo; For Windows-based OS:</strong></p>
    <div style="margin-left: 1em">
    <p>Upgrade your system to linux, then proceed as above ;-)<br/>
    If you're really decided to use Windows, then you need to download a CVS client like <a href="http://tortoisecvs.sourceforge.net/">TortoiseCVS</a> or <a href="http://wincvs.sourceforge.net">WinCVS</a>... then set the following data in your client:</p>

    <table style="margin-left: auto; margin-right: auto; width:50%; background-color: rgb(240,240,240);" cellspacing="10" cellpadding="0">
    <tr>
    <td style="width:50%">Protocol:</td>
    <td style="width:50%"><strong>Extended shell (:ext) *</strong></td>
    </tr>
    <tr>
    <td>Server:</td>
    <td><strong>wxcode.cvs.sourceforge.net</strong></td>
    </tr>
    <tr>
    <td>Repository folder:</td>
    <td><strong>/cvsroot/wxcode</strong></td>
    </tr>
    <tr>
    <td>User name:</td>
    <td><strong>yourSFusername</strong></td>
    </tr>
    <tr>
    <td>Module:</td>
    <td><strong>wxCode</strong></td>
    </tr>
    </table>

    <div align="center">
    <p><strong>*</strong>&nbsp;if you have problems with <tt>ext</tt> try to use &quot;Secure shell SSH (:ssh:)&quot;</p>
    </div>

    <p>Please note that all CVS commands are case-sensitive and thus you should check carefully the strings
    typed in order to avoid problems. The <strong>CVSROOT</strong> shown by your client at the end should be:</p>
    <pre class="sourcecode">:ssh:yourSFusername@wxcode.cvs.sourceforge.net:/cvsroot/wxcode</pre>
    <p>Then, just choose in your CVS client the &quot;checkout&quot; command and wait...</p>
    </div>



    <br/><p id="cvs_alt"><strong>&rsaquo; For both Linux-based and Windows-based OS:</strong></p>
    <div style="margin-left: 1em">
    <p>Note that the entire wxCode CVS repository is quite large (about 40 MB
    at July 2005) and that you can also download only what you need. This is done by
    checking-out only the modules:</p>

    <pre class="sourcecode">
        wxCode/build
        wxCode/template
        wxCode/components/YOURCOMPONENT</pre>

    <p>Otherwise, you can also <a href="http://sourceforge.net/project/showfiles.php?group_id=51305&amp;package_id=114273&amp;release_id=347076">download the wxCode CVS repository</a> using SourceForge.net File Release System which is much much more fast than checking-out using CVS but also has some disadvantages:</p>
    <ul>
    <li>It's not up-to-date as the CVS is, since wxCode CVS is continuosly modified and you should then do in any case a CVS update (which is however, quicker than a CVS checkout)</li>
    <li>It requires to use the &quot;changeroot.sh&quot; script on the wxCode CVS repository downloaded to use it in write mode.</li>
    </ul>
    <p>If you find problems with this download, please ask in the wxCode-users mailing list.</p>

    <p>Both for linux and windows users, <strong>there is much more detail available on sourceforge at
    <a href="http://sourceforge.net/docman/display_doc.php?docid=25888&amp;group_id=1#developer_config">
    SourceForge CVS documentation page</strong></a>,
    and in the <a href="http://cvsbook.red-bean.com/cvsbook.html">online CVS book</a>.</p>
    <p>After the complete download of the wxCode repository, you're ready for step #2...</p>
    </div>



    <!-- =================================================================================== -->
    <!--                                 STEP #2                                             -->
    <!-- =================================================================================== -->


    <br/><?php write_h1("STEP #2: set up the SVN for your component", "svn"); ?>

    <p><strong>
        Note: you need to setup Subversion only if your component is there or if
              it uses components placed in the Subversion repository.
    </strong></p>

    <p id="svn_linux"><strong>&rsaquo; For Unix-based OS:</strong></p>
    <div style="margin-left: 1em">
    <p>You will need to have subversion installed and set up on your system.
       You will need a sourceforge username (here referenced as <tt>mysfname</tt>) and a password
       (these will be asked only for write operations).</p>

    <p>Open a shell (e.g. <tt>Konsole</tt> or <tt>gnome-terminal</tt>) and copy and paste (comments won't harm) the following commands:</p>
    <pre class="sourcecode">
        cd                   # this brings to the home

        # this command will now checkout all wxCode components of the SVN repository
        # in a directory called 'wxCode'
        svn co https://wxcode.svn.sourceforge.net/svnroot/wxcode/trunk/wxCode
    </pre>

    <p>Now you have the <tt>~/wxCode/components/YOURCOMPONENT</tt> folder; it should contain a standard tree like the one of the &quot;wxCode\template&quot; directory. Copy into it the files that comprise your module and change/remove existing files to suit your needs. Tell subversion about your project files with a command like:</p>

    <pre class="sourcecode">
        svn add myfile.1 myfile.2          # tell SVN about new files
        svn commit -m "Initial commit"     # commit them to the repository
    </pre>

    <p>The bit in quotes is used by subversion as a label for your future reference.
    You can substitute a message of your choice, or "" for none.  If you
    omit the -m "something" then your default editor will be summoned so
    that you can compose the message there.  After a delay while the
    sourceforge server gets updated, all should be well.</p>
    </div>


    <br/><p id="svn_win"><strong>&rsaquo; For Windows-based OS:</strong></p>
    <div style="margin-left: 1em">
    <p>You need to download a Subversion client like <a href="http://tortoisesvn.tigris.org/">TortoiseSVN</a> and just use as URL:</p>
    <pre class="sourcecode">https://wxcode.svn.sourceforge.net/svnroot/wxcode/trunk/wxCode</pre>
    <p>Adding files and committing are all operations extremely easy with the GUI of TortoiseSVN; refer to its manual for more info.</p>
    </div>


    <br/><p id="merge_cvssvn"><strong>&rsaquo; How to merge wxCode CVS with wxCode SVN:</strong></p>
    <div style="margin-left: 1em">
    <p>If you want to keep both wxCode CVS and wxCode SVN in a single wxCode folder on your
       hard disk, you just have to checkout the CVS and SVN repositories of wxCode (see above)
       in two different folders.<br/>
       Then, move the components of the Subversion repository in the
       <tt>wxCode/components</tt> folder created by the CVS checkout (don't forget to move also
       the hidden <tt>.svn</tt> folders!).</p>

    <p>Now you can use both CVS and SVN commands (or graphical CVS and SVN clients) in that folder.</p>
    </div>





    <!-- =================================================================================== -->
    <!--                                 STEP #3                                             -->
    <!-- =================================================================================== -->

    <br/><?php write_h1("STEP #3: upload your sources", "customize"); ?>
    <p>This is the longest part, since it probably requires you to do some changes to your component and test them:</p>
    <ol>
    <li>look at the <a href="template.php">template page</a>: it explains some basic rules about naming of the source and header files for your component, the website of your component, etc</li>
    <li>learn how to create a bakefile-based build system for your component; this enables you to support multiple compilers, multiple platforms and multiple build settings (ANSI/Unicode, DLL/static, debug/release, etc) with little effort; use the <a href="bakefile.php">wxCode bakefile page</a> and <a href="autoconf.php">wxCode autoconf page</a> as guide.</li>
    <li>add all the stuff regarding your component to the CVS/Subversion (except, as said in the readme, for the screenshots, the documentation and the release packages) and edit the &quot;website&quot; folder for your component...</li>
    </ol>




    <!-- =================================================================================== -->
    <!--                                 STEP #4                                             -->
    <!-- =================================================================================== -->

    <br/><?php write_h1("STEP #4: make a release for your component", "release"); ?>
    <p>CVS is for wxCode maintainers; users just want to easily download an archive of the project. For this to be possible you need to make a file release through Sourceforge. Also remember that Open Source philosophy is &quot;<strong>Release often, release early</strong>&quot; !</p>

    <p id="release_first"><strong>&rsaquo; First ever file release of your project:</strong></p>
    <ol>
    <li>Compress your files into one or more archives (tar.gz, tar.bz2 or zip); it's a good idea to name this something like <tt>myproject-1.0.ext</tt> (i.e. lowercase name, hyphen, version), with subsequent releases called 1.1 etc. You aren't allowed to use spaces in the name. <br/>

    Then, refer to the Sourceforge's <a href="https://sourceforge.net/apps/trac/sourceforge/wiki/Release%20files%20for%20download">FRS system</a> help page for uploading 
it to Sourceforge servers (you can use various methods, choose your favourite one).

<!--
    Using your favourite ftp client (for win32, we suggest looking at <a href="http://sourceforge.net/projects/filezilla">FileZilla</a> or <a href="http://www.smartftp.com/">SmartFTP</a>; for Gnome, I use <a href="http://gftp.seul.org/">gFTP</a>; for KDE have a look at <a href="http://kbear.sf.net">KBear</a>), log on by anonymous ftp to &quot;<strong>upload.sourceforge.net</strong>&quot; and upload your archive(s) to the &quot;<strong>incoming</strong>&quot; directory.<br/><br/>
    For <strong>Unix users only</strong>, the following will probably Just Work from usual console:
    <pre class="sourcecode">
ftp -v upload.sourceforge.net
# use "anonymous" for username and "" (empty string) for password
cd incoming
put myproject-1.0.ext  # upload the file (warning: no progress bar!)</pre>
    </li>-->

    <li>Go to the Sourceforge page <a href="https://sourceforge.net/project/admin/explorer.php?group_id=51305">the wxCode file release page</a> (Sourceforge will first get you to log on).
    Click &quot;Edit releases&quot; and look at the list of components available to be edited, you will (currently) see several entries called <strong>&quot;USE THIS FOR NEW COMPONENTS&quot;</strong>, with a status of &quot;Hidden&quot;. These are orphaned releases, unwanted and unloved. To adopt one, just click its &quot;Edit This Release&quot; link, delete any old files that were left behind, change the name and date to that of your project and set the status to &quot;Active&quot;.
    </li>

    <li>Add or change whatever you wish in the first section (for wxCode projects you don't really need to fill in the Release Notes or Change Log).  Now the important bit: you should see your uploaded archive(s) in section 2 (together with all the rest of the day's Sourceforge uploads). Tick the checkbox for your file(s), then click the "Add Files" button. Finally fill in the boxes in the newly-revealed Step 3, click Update/Refresh and you've made a File Release. To prove that it worked, go back to the wxCode webpage and check that you can download your archive successfully.</li>
    </ol>


    <p id="release_sub"><strong>&rsaquo; Subsequent file releases:</strong></p>
    <div style="margin-left: 1em">
    <p>The next time you want to make a file release for this project, the procedure is much the same. However when you go to <a href="https://sourceforge.net/project/admin/explorer.php?group_id=51305">the wxCode file release 
page</a>, you click &quot;Edit releases&quot;<!-- instead of &quot;Add release&quot; -->. On the next page, find your component and click its &quot;Edit This Release&quot; link. Then you upload the new archive as before.<br/>
    It may be a good idea this time to write in the Release Notes box about what is new in this Release.  It probably now makes sense to <strong>delete the old release</strong>, and <strong>change the release date</strong>.</p>

    <p>More detailed information is available on <a href="https://sourceforge.net/apps/trac/sourceforge/wiki/Release%20files%20for%20download">this Sourceforge page</a>.
    </div>
    </p>

    <br/><?php require("footer.inc.php"); ?>
</html>
