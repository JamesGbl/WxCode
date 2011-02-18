<?php $logsys_disabled=TRUE; require("globals.inc.php"); ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 29/10/2005
// RCS-ID: $Id: template.php,v 1.14 2009/05/16 09:51:03 frm Exp $
// Notes: the wxCode template structure for new components
////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="template";
        $pagetitle="Template guide";
        $menuentry=writeMenuEntry("maintguide", "Maintainer guide");
        require("header.inc.php");
    ?>


    <!-- WXCODE TEMPLATE SECTION -->
    <?php write_h1("Guidelines for new components", "template", FALSE); ?>
    <p>Index of this document:</p>
    <ul>
    <li><a href="#files">Source &amp; header files</a></li>
    <li><a href="#docs">Documentation</a><br/>
        &rsaquo; <a href="#readme">The ReadMe</a>
    </li>
    <li><a href="#build">The build system</a></li>
    <li><a href="#samples">The samples</a></li>
    <li><a href="#website">The website</a><br/>
        &rsaquo; <a href="#autosync">About automatic CVS-website synchronization</a>
    </li>
    <li><a href="#other">Other docs for wxCode</a></li>
    </ul>

    <p>This documentation contains guidelines for the organization of your
       wxCode component. While reading it you can see the structure of the
       'template' folder described in the text <a href="http://wxcode.svn.sourceforge.net/viewvc/wxcode/trunk/wxCode/template/">here</a>.</p>

    <p>Remember that the format &amp; the structure explained in this file will make your
        component not only standardized and thus easier to use in bigger projects but
        will also make possible to integrate it into the official wxWidgets
        distribution.</p>

    <p>If you need further info/assistance with the structure of your component,
    please ask in the wxcode-users mailing list at <a href="mailto:wxcode-users@lists.sourceforge.net">wxcode-users@lists.sourceforge.net</a>.</p>


    <br/><?php write_h1("Source &amp; header files", "files"); ?>
    <p>You should keep your sources in a "src" folder; they should all have a .cpp or .c
    extension (avoid .cxx or .c++ extensions); your include files should instead go into
    a "include/wx" folder so that they can be included as any other standard wxWidgets
    include file:</p>

    <div style="text-align:center"><tt>#include &lt;wx/comp.h&gt;</tt></div>

    <p>they should have a .h extension (avoid .hpp extensions).</p>

    <p>To be able to support precompiled headers and also to be sure that your license applies
    to every piece of your code, you must always start your header and source files with
    some standard contents shown in <tt>wxCode/template/include/wx/mycomp.h</tt> and <tt>wxCode/template/src/mycomp.cpp</tt>.</p>

    <p>Also, to support shared builds you should always include in your component the
    WXDLLIMPEXP_MYCOMP symbols in the declaration of each class, function, event...
    you should keep these defines in a separate include file (since they must be
    included in all the files of your project), look at <tt>wxCode/template/include/wx/mycompdef.h</tt>
    for a more detailed explanation about the things you need to support shared builds
    and also for an example of the declaration of WXDLLIMPEXP_MYCOMP macros.</p>

    <p>Last, to support Unicode builds you should always enclose all the strings in your
    source code with the <tt>wxT()</tt> macro:</p>

    <div style="text-align:center"><tt>wxString str = wxT("hello world!");</tt></div>

    <p>for more info search in wxWidgets documentation.</p>


    <br/><?php write_h1("Documentation", "docs"); ?>
    <p>In order to make your component easily integrable in other applications, you need to provide
    a good documentation for it. Since you probably need to create a simple documentation and you want to do it quickly, 
    I suggest to use doxygen (http://www.doxygen.org); it's a free program which creates the
    documentation of a project extracting the comments you put in your source and header files.</p>

    <p>In this template component, you'll find a simple "Doxyfile" in docs/ folder.
    If you have already used the doxygen comment syntax in your component source files, then you
    probably need only to change the PROJECT_NAME field in the Doxyfile and then you can run
    doxygen to get your fresh docs!</p>

    <p id="readme"><strong>&rsaquo; The ReadMe</strong></p>
    <div style="margin-left: 1em">
    <p>Each component should provide at least a "ReadMe.txt" file in its root folder.
    Since all major info about the component are kept in the wxCode database, and you probably
    want to update your component's info only once when making a new release, such readme file
    should contain only the most important info; all other info should be updated in the wxCode
    website using the EDIT form.</p>

    <p>For a simple template of the Readme of a component, see Readme.txt.template.</p>

    <p>It's important to keep a "version" field in the readme so that the user who is already using
    your component can quickly find if he is using a version older than the version currently
    published in the wxCode website.</p>
    </div>


    <br/><?php write_h1("The build system", "build"); ?>
    <p>Even if it's not required that you use Bakefile as build system for your component you are
    strongly encouraged to do so because it's almost impossible to handle all build settings for
    all possible builds writing manually the makefiles or, even worse, editing the IDE project
    files: if you support ANSI/Unicode + static/shared + debug/release builds for your component's
    library and your component's sample you'll have up to 16 different configurations.</p>

    <p>Also, consider that if users cannot build your component they will probably simply discard
    it and choose something else or they will just write their own versions... to make Open
    Source great you should make your code easy to compile. This also means that you should
    support many compilers (differently from Unixes, which usually have a standard GCC compiler,
    there are various win32 compilers: borland, watcom, digitalmars, msvc, mingw...).</p>

    <p>Bakefile is very extensible and you can handle any required build option with it.<br/>
    So, look at <tt>wxCode/build/bakefiles/ReadMe.txt</tt> for a quick-start guide on the generation 
    (a matter of few minutes!) of the bakefile for your component.</p>


    <br/><?php write_h1("The samples", "samples"); ?>
    <p>Samples are:</p>
    <ul>
    <li>a good way for users to understand how to use your component</li>
    <li>an easy way for users to try out your component and test your component's features</li>
    <li>an easy way for the maintainer to test the improvements/changes on the component</li>
    </ul>

    <p>In conclusion, you should always provide at least one sample per component.
    The easiest way to create a sample is to modify the "minimal" sample; look at 
    <tt>wxCode/template/samples</tt> folder for the required file for a minimal sample.</p>


    <br/><?php write_h1("The website", "website"); ?>
    <p>Your component's website should be stored in wxCode/components/YOURCOMP/website folder.
    This is necessary since the scripts of wxCode automatically search that folder
    (see section 5.2).</p>

    <p>In order to create your component website you should:</p>
    <ol>
    <li>put in your component's website folder all the files you need to make the main page.
    You should *exclude* documentation and screenshots from that folder since they make
    CVS/SVN slower and wxCode provides other standard placed for these things (see below).</li>

    <li>edit your <tt>website/index.php</tt> file (open it in a simple editor and change it to suit
    your needs...).</li>

    <li>edit your <tt>website/image.png</tt> file: it should be a simple logo sized 160x120 px which
    gives an idea of the function of your component.</li>

    <li>upload your DOCUMENTATION to the wxCode website <tt>docs</tt> folder.
    You have to do this using some SFTP/SCP client (I use WinSCP on Windows - see <a href="www.winscp.net">www.winscp.net</a>).
    Here is some detailed information to configure your client: 
    <a href="http://sourceforge.net/apps/trac/sourceforge/wiki/SFTP">SF SFTP service documentation page</a>.

    Note that if the folder <tt>/home/project-web/wxcode/htdocs/docs/YOURCOMPNAME</tt> does not exists,
    then you should create it.<br/>
    Also note that you must use your private SSH key to log in to the webserver.</li>

    <li>using the same settings used for step #4, upload your SCREENSHOTS to the 
    <tt>/home/project-web/wxcode/htdocs/screenshots/YOURCOMPNAME</tt> directory.<br/>
    Note that if the folder <tt>/home/projcet-web/wxcode/htdocs/screenshots/YOURCOMPNAME</tt> 
    does not exists, then you should create it.</li>
    </ol>

    <p><strong>Note:</strong> Website write access is currently not automatically granted. Please ask a wxCode administrator to grant write access rights to you.</p>

    <p id="autosync"><strong>&rsaquo; About SVN-website synchronization</strong></p>
    <div style="margin-left: 1em">
    <p>Automatically synchronizing the wxCode website is not supported by SourceForge anymore. wxCode has to be synchronized manually with the SVN version. That will be done by one of the wxCode administrators in irregular intervals.
    Also all component's "website" folders are updated on the server.
    So, all the files you put in YOURCOMP/website folder should be under SVN control
    and will be used to keep your component website up to date.</p>
    </div>


    <br/><?php write_h1("Other docs for wxCode", "other"); ?>
    <p>This ReadMe focuses on the *structure* that your component should follow.
    For hints about the design of your wxWidgets-based code at wxCode there are other
    documents at:</p>

    <div style="text-align:center"><a href="http://wxcode.sourceforge.net/faq.php">http://wxcode.sourceforge.net/faq.php</a></div>

    <p>For help on SVN operations and SourceForge file release system, look at:</p>

    <div style="text-align:center"><a href="http://wxcode.sourceforge.net/maintguide.php">http://wxcode.sourceforge.net/maintguide.php</a></div>

    <p>If you still need help then just ask on the wxcode-users mailing list.</p>


    <br/><?php require("footer.inc.php"); ?>
</html>
