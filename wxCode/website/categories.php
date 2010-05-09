<?php $logsys_disabled=TRUE; require("globals.inc.php"); ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
////////////////////////////////////////////////////////////////////////////////////////////
// Author: Francesco Montorsi
// Creation date: 5/7/2005
// RCS-ID: $Id: categories.php,v 1.7 2005/09/05 23:27:38 jrl1 Exp $
// Notes: the categories description; when to use them and when not to use them.
////////////////////////////////////////////////////////////////////////////////////////////
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-AU">
    <?php
        $filename="categories";
        $menuentry="wxCode categories description";
        $pagetitle="wxCode categories description";
        require("header.inc.php");
    ?>
    
    
    <!-- CATEGORIES SECTION -->
    <?php write_h1("About wxCode categories", "cat", FALSE); ?>
    <p>The following is a list of the currently available categories
       for wxCode and their descriptions. Each component may be in multiple 
       categories to better describe what it provides. The examples are 
       common wxWidgets classes that you might find in each category if they 
       were components at wxCode.</p>
    <p>Developers, please note that carefully categorizing your component is 
       very important since it will make it easier for people to browse and
       find what they're looking for. 
       Try to use as few categories as possible to describe it. 
       For example, if your component merely provides a dialog for setting 
       preferences, you should probably not use the dialog category since it's 
       only a secondary feature and cannot be used by itself.
       If a category seems to be missing please post a message to the 
       wxcode-users mailing list and we can add it.
       As a very last resort you may use the miscellaneous category.</p>

    <!-- NOTE FOR wxCode ADMINISTRATOR:
         To add/remove a category to wxCode website you need to:
         1) modify the "category" field of the "components" table structure
         2) update this list
         3) update the function writeCategories() of "form.inc.php"
    -->
    <dl>
        <dt>window container</dt>
        <dd>A wxWindow derived class that manages the placement, size, and/or visibility of child windows.
        <br/><i>Examples: wxSplitterWindow, wxNotebook</i></dd>

        <dt>dialog</dt>
        <dd>A wxDialog derived class (or any top level window) that can be used like a dialog for displaying or getting values from the user.
        <br/><i>Examples: wxColourDialog, wxFileDialog, wxPasswordEntryDialog, wxWizard</i></dd>

        <dt>control</dt>
        <dd>A wxWindow derived class that can be used as a control for displaying or getting values from the user.
        <br/><i>Examples: wxButton, wxListCtrl, wxScrollBar, wxStaticBitmap</i></dd>

        <dt>window layout</dt>
        <dd>A wxWindow layout mechanism that is not a window itself (see also window container).
        <br/><i>Examples: wxSizer, wxFlexGridSizer</i></dd>

        <dt>networking</dt>
        <dd>A networking library for interprocess or internet communication or protocol handling.
        <br/><i>Examples: wxSocketClient, wxFTP, wxClient, wxIPaddress</i></dd>

        <dt>stream</dt>
        <dd>A wxStreamBase derived class to read/write data from/to.
        <br/><i>Examples: wxInputStream, wxBufferedInputStream, wxTextInputStream, wxZlibInputStream</i></dd>

        <dt>database</dt>
        <dd>A database library to interact with or is a database.
        <br/><i>Examples: wxDb, wxDbTable, wxDbConnectInf</i></dd>

        <dt>data container</dt>
        <dd>A library that provides data container structures.
        <br/><i>Examples: wxObject, wxArray, wxHashMap, wxRect, wxVaraint</i></dd>

        <dt>graphics<dt>
        <dd>A graphics library that provides drawing, image processing, device contexts, video.
        <br/><i>Examples: wxImage, wxBitmap, wxQuantize, wxDC, wxGLCanvas</i></dd>

        <dt>text processing<dt>
        <dd>A library that displays, formats, or processes text.
        <br/><i>Examples: wxTextCtrl, wxStringTokenizer, wxRegEx</i></dd>

        <dt>printing<dt>
        <dd>A printing library that can be used to generate formatted output.
        <br/><i>Examples: wxPrinter, wxPrinterDC, wxPrintPreview, wxHTMLEasyPrinting</i></dd>

        <dt>import/export<dt>
        <dd>A library that can be used to import/export data to/from a program (see also streams).
        <br/><i>Examples: wxHTML, wxXml, wxImage (loading/saving)</i></dd>

        <dt>system</dt>
        <dd>System libraries to access the hardware, file system, operating system, or provide other low level functionality.
        <br/><i>Examples: wxJoystick, wxTimer, wxProcess, wxDllLoader, wxTempFile</i></dd>

        <dt>tutorials/documentation<dt>
        <dd>Documentation or tutorials of any kind.
        <br/><i>Examples: none</i></dd>

        <dt>application<dt>
        <dd>Standalone applications that are useful by themselves. Components that provide full featured samples may also use this category.
        <br/><i>Examples: HelpView, Tex2RTF</i></dd>

        <dt>wrapper<dt>
        <dd>A library that wraps an existing library making it easier to use with wxWidgets (see other categories for purpose).
        <br/><i>Examples: wxStyledTextCtrl, wxZlib, wxXml</i></dd>

        <dt>miscellaneous</dt>
        <dd>Uncategoried components or ones that do not fit into any existing categories.
        <br/><i>Examples: there should be none</i></dd>
    </dl>
    
    <?php require("footer.inc.php"); ?>
</html>
