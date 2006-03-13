////////////////////////////////////////////////////////////////////////////
// Name:        minimal.cpp
// Purpose:     Minimal wxWindows sample
// Author:      Julian Smart
// Modified by: Francesco Montorsi
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include "wx/xml2.h"                // include libxml2 wrapper definitions
#include "wx/dtd.h"             // include libxml2 wrapper definitions
#include <wx/sstream.h>
#include <wx/mstream.h>


#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources and even
// though we could still include the XPM here it would be unused)
#if !defined(__WXMSW__) && !defined(__WXPM__)
    #include "mondrian.xpm"
#endif

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();

    int OnExit();
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Minimal_Quit = wxID_EXIT,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Minimal_About = wxID_ABOUT,

    Minimal_LoadXML,            // these were added by me
    Minimal_LoadDTD,
    Minimal_SaveSimple,
    Minimal_SaveAdv,
    Minimal_SaveDTD,
    Minimal_Valid
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title);
    ~MyFrame();

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    wxTextCtrl *m_text;

    void LoadXML(const wxString &filename);
    void LoadDTD(const wxString &filename);

    void OnLoadXML(wxCommandEvent& event);
    void OnLoadDTD(wxCommandEvent& event);
    void OnValid(wxCommandEvent& event);

    void OnSaveSimple(wxCommandEvent& event);
    void OnSaveAdv(wxCommandEvent& event);
    void OnSaveDTD(wxCommandEvent& event);

private:
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
    EVT_MENU(Minimal_About, MyFrame::OnAbout)

    EVT_MENU(Minimal_LoadXML, MyFrame::OnLoadXML)
    EVT_MENU(Minimal_LoadDTD, MyFrame::OnLoadDTD)
    EVT_MENU(Minimal_SaveSimple, MyFrame::OnSaveSimple)
    EVT_MENU(Minimal_SaveAdv, MyFrame::OnSaveAdv)
    EVT_MENU(Minimal_SaveDTD, MyFrame::OnSaveDTD)
    EVT_MENU(Minimal_Valid, MyFrame::OnValid)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// first of all, decide if we can use the system...
#if defined(__VISUALC__) && defined(__WXDEBUG__)
    #define mcDETECT_MEMORY_LEAKS
#endif


#ifdef mcDETECT_MEMORY_LEAKS

    // "crtdbg.h" is included only with MSVC++ and Borland, I think...
    // "stackwalker.h" instead, contains a set of stack walker functions
    // created by Jochen Kalmbach (thanks !!!) which allow to read the
    // intercept unhandled exceptions and memory-leaks.
    // To be used, the file must be part of the project; this is why
    // it's contained (with its CPP counterpart) in the folder of this
    // test program. Anyway,  you can find it also online at:
    //     http://www.codeproject.com/tools/leakfinder.asp
    #include <crtdbg.h>

    // define some useful macros
    #define mcDUMP_ON_EXIT      { _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF); }


    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;


    // this little class is used to access Stackwalker functions
    // without changing a line of code...
    class mcLeakDetector {

    public:
        mcLeakDetector() { mcDUMP_ON_EXIT; }
        ~mcLeakDetector() {}
    };

    // ...infact, instancing a STATIC mcLeakDetector class, we
    // can start memory-leak detection at the very beginning of
    // the program (when the main() or winmain() has not been
    // called yet, that is, when the framework is creating the
    // static variables of the program) and end it at the very
    // end of the program (when, after the main() or winmain(),
    // the framework removes the static variables).
    static mcLeakDetector detector;

#endif




// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// wxT('Main program') equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // create the main application window
    MyFrame *frame = new MyFrame(_T("Minimal wxWindows App"));

#if 1
    // create an useful log window
    wxLogWindow *pwindow = new wxLogWindow(frame, wxT("log"));
    pwindow->GetFrame()->Move(50, 50+350);
    pwindow->GetFrame()->SetSize(800, 300);

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);
#endif

    // init libxml2 library
    wxXml2::Init();


    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}


int MyApp::OnExit()
{
    // cleanup libxml2 statics
    wxXml2::Cleanup();

    return 0;
}


// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxPoint(50, 50), wxSize(500, 300))
{
    // set the frame icon
    SetIcon(wxICON(mondrian));

#if 1
    // create the wxTextCtrl where the file structure is shown
    m_text = new wxTextCtrl(this, -1,
        wxT("This program will provide some examples about the three main operations ")
        wxT("it provides on XML files:\n\n")
        wxT("\t- Loading: wxXml2 allows you to load any XML file including DTDs.\n")
        wxT("\t- Editing: wxXml2 allows you to create, edit, manipulate any XML file.\n")
        wxT("\t- Saving: wxXml2 can saves XML trees or DTDs to memory buffers or to files."),
        wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);
    m_text->SetBackgroundColour(*wxWHITE);
#endif

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *menuFile = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, _T("&About...\tF1"), _T("Show about dialog"));

    menuFile->Append(Minimal_LoadXML, _T("Load XML..."), _T("Loads the given file and tries to parse it..."));
    menuFile->Append(Minimal_LoadDTD, _T("Load DTD..."), _T("Loads the given file and tries to parse it..."));
    menuFile->AppendSeparator();
    menuFile->Append(Minimal_SaveSimple, _T("Save a simple HTML file..."), _T("Creates a sample HTML file and saves it in the given location."));
    menuFile->Append(Minimal_SaveAdv, _T("Save an advanced XHTML file..."), _T("Creates a sample XHTML file and saves it in the given location."));
    menuFile->Append(Minimal_SaveDTD, _T("Save a DTD file..."), _T("Creates a sample DTD file and saves it in the given location."));
    menuFile->AppendSeparator();
    menuFile->Append(Minimal_Valid, _T("Validate against DTD..."), _T("Validates an XML file against a DTD."));
    menuFile->AppendSeparator();
    menuFile->Append(Minimal_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, _T("&File"));
    menuBar->Append(helpMenu, _T("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText(_T("Welcome to wxWindows!"));
#endif // wxUSE_STATUSBAR
}

MyFrame::~MyFrame()
{
}



// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf( wxT("This is a little demonstration of wxXml2 wrappers for libxml2.\n\n")
                wxT("They are based on original Vaclav Slavik's design but they\n")
                wxT("use libxml2 (which is a widely-used, powerful cross-platform\n")
                wxT("library) instead of EXPAT.\n\n")
                wxT("The classes were rewritten by Francesco Montorsi (frm@users.sourceforge.net)\n")
                wxT("and this sample is a modified version of the minimal sample of wxWidgets.\n\n")
                wxT("This sample program was compiled with libxml2 version %s."),
                    wxXml2::GetLibxml2Version().c_str());

    wxMessageBox(msg, _T("About Minimal"), wxOK | wxICON_INFORMATION, this);
}



#define STEP            4


// helper recursive function: THIS IS ONLY A "DIDACTIVE" EXAMPLE OF A
// RECURSIVE FUNCTION WHICH CAN BE USED TO WALK AN XML TREE...
// IF YOU JUST NEED TO OUTPUT THE CONTENTS OF AN XML TREE YOU CAN USE
// THE wxXml2Document::Save FUNCTION ON A wxMemoryOutputStream AS
// MyFrame::LoadDTD DOES TO SHOW TO THE USER THE STRUCTURE OF AN XML DTD.
void ParseNode(const wxXml2Node &node, wxString &str, int n)
{
    if (node == wxXml2EmptyNode) return;
    wxLogDebug(wxT("ParseNode - parsing [%s]"), node.GetName().c_str());
    wxString toadd, spaces(wxT(' '), n);

    // concatenate the name of this node
    toadd = node.GetName();

    // if this is a text node, then add also the contents...
    if (node.GetType() == wxXML_TEXT_NODE ||
        node.GetType() == wxXML_COMMENT_NODE ||
        node.GetType() == wxXML_CDATA_SECTION_NODE) {

        wxString content = node.GetContent();
        if (content.Last() == wxT('\n')) content.RemoveLast();
        if (content.GetChar(0) == wxT('\n')) content.Remove(0, 1);

        // a little exception: libxml2 when loading a document creates a
        // lot of text nodes containing just a simple \n;
        // in this cases, just show "[null]"
        wxString tmp = content.Trim();
        if (tmp.IsEmpty())
            toadd += wxT("node: [null]");
        else
            toadd += wxT("node: ") + content;


    } else {        // if it's not a text node, then add the properties...

        wxXml2Property prop(node.GetProperties());
        while (prop != wxXml2EmptyProperty) {

            toadd += wxT(" ") + prop.GetName() + wxT("=");
            toadd += prop.GetValue();

            prop = prop.GetNext();
        }
    }

    str += spaces;

//#define SHOW_ANNOYING_NEWLINES
#ifdef SHOW_ANNOYING_NEWLINES

    // text nodes with newlines and/or spaces will be shown as [null]
    str += toadd;
#else

    // text nodes with newlines won't be shown at all
    if (toadd != wxT("textnode: [null]")) str += toadd;
#endif

    // go one line down
    str += wxT("\n");

    // do we must add the close tag ?
    bool bClose = FALSE;

    // and then, a step more indented, its children
    wxXml2Node child(node.GetFirstChild());
    while (child != wxXml2EmptyNode) {

        ParseNode(child, str, n+STEP);
        child = child.GetNext();

        // add a close tag because at least one child is present...
        bClose = TRUE;
    }

    if (bClose) str += wxString(wxT(' '), n) + wxT("/") + node.GetName() + wxT("\n");
}

// another helper function
void ParseNodeAndSiblings(const wxXml2Node &node, wxString &str, int n)
{
    wxXml2Node curr(node);

    do {
        ParseNode(curr, str, n);
        curr = curr.GetNext();
    } while (curr != wxXml2EmptyNode);
}




void MyFrame::LoadXML(const wxString &filename)
{
    wxXml2Document doc;
    wxString err;


    // if the file is big our raugh ::ParseNodeAndSiblings function could
    // take a while... in this case disable the log system for better performance
    wxFileInputStream tmp(filename);
    if (tmp.IsOk() && tmp.GetSize() > 2048)
        wxLog::EnableLogging(false);

    // parse the file
    if (!doc.Load(filename, &err)) {
        wxMessageBox(wxString::Format(
            wxT("Couldn't parse the file you chose:\n%s"), err.c_str()), wxT("Error"));
        return;
    }

    // show the wxXml2Node tree in a simple format...
    wxString tree;
    wxXml2Node root = doc.GetRoot();
    int indent = 3;
    tree.Alloc(1024);

    // show the DTD declaration, if present
    wxXml2DTD dtd(doc.GetDTD());
    if (dtd != wxXml2EmptyDTD) {
        tree += wxT("DTD name=") + dtd.GetName();

        if (dtd.IsPublicSubset())
            tree += wxT(" PUBLIC externalID=") + dtd.GetExternalID() +
                wxT(" externalURI=") + dtd.GetExternalURI() + wxT("\n\n");

        if (dtd.IsSystemSubset())
            tree += wxT(" SYSTEM systemID=") + dtd.GetSystemID() + wxT("\n\n");
    }

    // get a string with the tree structure...
    ParseNodeAndSiblings(root, tree, indent);

    // show it to the user
    m_text->SetValue(tree);

    // cleanup
    dtd.DestroyIfUnlinked();
    root.DestroyIfUnlinked();
    doc.DestroyIfUnlinked();
    wxLog::EnableLogging(true);
}

void MyFrame::LoadDTD(const wxString &filename)
{
    wxXml2DTD doc;
    wxString err;

    // parse the file
    if (!doc.Load(filename, &err)) {
        wxMessageBox(wxString::Format(
            wxT("Couldn't parse the file you chose:\n%s"), err.c_str()), wxT("Error"));
        return;
    }

    // show the wxXml2DTD tree in a simple format...
    wxStringOutputStream stream;

    // we won't use the ParseNodeAndSiblings because it requires
    // wxXml2Nodes to parse: here we have instead wxXml2ElemDecl,
    // wxXml2AttrDecl... node types.
    doc.Save(stream);
    m_text->SetValue(stream.GetString());

    doc.DestroyIfUnlinked();
}

void MyFrame::OnLoadXML(wxCommandEvent& WXUNUSED(event))
{
    // ask the user which file we must load...
    wxFileDialog fd(this, wxT("Choose the XML file to load"), wxT(""), wxT(""),
        wxT("XML and HTML files|*.xml;*.html;*.xhtml|All files|*.*"), wxOPEN);
    if (fd.ShowModal() == wxID_CANCEL)
        return;

    LoadXML(fd.GetPath());

    // ask the user which file we must load...
    wxFileDialog fd2(this, wxT("Choose the XML file to save on"), wxT(""), wxT(""),
        wxT("XML and HTML files|*.xml;*.html;*.xhtml|All files|*.*"), wxSAVE);
    if (fd2.ShowModal() == wxID_CANCEL)
        return;

    wxXml2Document tmpDoc;
    tmpDoc.Load(fd.GetPath());
    tmpDoc.Save(fd2.GetPath());//, wxT("UTF-8"), 0);
}

void MyFrame::OnLoadDTD(wxCommandEvent& WXUNUSED(event))
{
    // ask the user which file we must load...
    wxFileDialog fd(this, wxT("Choose the DTD to load"), wxT(""), wxT(""),
        wxT("DTD files|*.dtd|All files|*.*"), wxOPEN);
    if (fd.ShowModal() == wxID_CANCEL)
        return;

    LoadDTD(fd.GetPath());
}

void MyFrame::OnSaveSimple(wxCommandEvent& WXUNUSED(event))
{
    wxXml2Document doc;
    wxString err;

    // ask the user where we must save
    wxFileDialog fd(this, wxT("Save simple HTML example file as..."), wxT(""), wxT(""),
        wxT("XML and HTML files|*.xml;*.html;*.xhtml|All files|*.*"),
        wxSAVE | wxOVERWRITE_PROMPT );
    if (fd.ShowModal() == wxID_CANCEL)
        return;

    // create an XML version 1.0
    doc.Create();

    // create the HTML header if required
    wxXml2Node html;
    html.CreateRoot(doc, wxT("html"));

    // set HTML header:
    //    <html lang="en">
    //
    html.AddProperty(wxT("lang"), wxT("en"));

    // add the head, title & body tags:
    //   <HEAD><TITLE> [title] </TITLE></HEAD>
    //   <BODY></BODY>
    //
    html.AddContainerChild(wxT("head"));
    html.Get(wxT("head")).AddTextChild(wxT("title"), wxT("Hi! I'm a sample HTML file"));
    wxXml2Node body = html.AddContainerChild(wxT("body"));

    // and a comment:
    //    <!-- [sample comment] -->.
    //
    body.AddCommentChild(
        wxT(" This is a dummy comment "));

    // also add a simple paragraph... <P>text</P>
    body.AddTextChild(wxT("p"),
        wxT(" HTML exported by wxXml2 wrappers for libxml2 - ") \
        wxT(" write to frm@users.sourceforge.net if you encounter any ") \
        wxT(" problem or if you want to give suggestions or advices. "));

    // now, save the file where the user choose
    if (doc.Save(fd.GetPath())) {

        int ret = wxMessageBox(wxT("File correctly saved. Do you want to load it ?"),
            wxT("Question"), wxYES_NO | wxICON_QUESTION);

        if (ret == wxYES) LoadXML(fd.GetPath());

    } else {

        wxMessageBox(wxT("File could not be correctly saved !!!"),
                    wxT("Error"), wxOK | wxICON_ERROR);
    }

    html.DestroyIfUnlinked();
    body.DestroyIfUnlinked();
    doc.DestroyIfUnlinked();
}

void MyFrame::OnSaveAdv(wxCommandEvent &)
{
    wxXml2Document doc;
    wxString err;

    // ask the user where we must save
    wxFileDialog fd(this, wxT("Save advanced XHTML example file as..."), wxT(""), wxT(""),
        wxT("XML and HTML files|*.xml;*.html;*.xhtml|All files|*.*"),
        wxSAVE | wxOVERWRITE_PROMPT );
    if (fd.ShowModal() == wxID_CANCEL)
        return;

    // create an XML version 1.0
    doc.Create();

    // create the HTML header if required
    wxXml2Node root;
    root.CreateRoot(doc, wxT("advanced_sample"));

    // create a MathML DTD and set it for this doc
    doc.SetMathMLDTD();

    // create a CDATA node
    root.AddCDATAChild(wxT("my cdata block: this text is not parsed and it can ")
        wxT("contain any special character like <>'\",! except for the characters ")
        wxT("] ] > in sequence without spaces since they mark the end of this node"));

    // create a PI node: even if we create it here, it will be automatically
    // linked at the beginning of the document...
    root.AddPIChild(wxT("xml-stylesheet"),
        wxT("type=\"text/xsl\" href=\"http://www.w3.org/Math/XSL/mathml.xsl\""));

    // create a node containing a reference to an entity:
    // if everything works good, it will be recognized and embedded into
    // a wxXML_REFERENCE_NODE by wxXml2
    root.AddTextChild(wxT("refnode"), wxT("&copy;"));

#if wxUSE_UNICODE
    // then, do a little check for Unicode
    root.AddCommentChild(
        wxT("In the followig text child, some non-ANSI (i.e. Unicode) characters are used; ")
        wxT("If you see a greek ALPHA, BETA and GAMMA characters, it means that you are ")
        wxT("using a viewer which supports the Unicode standard."));

    wxString greekstring = wxT("\x3B1\x3B2\x3B3");
    root.AddTextChild(wxT("textchild"), greekstring);
#endif

    // create some simple nodes just to highlight the indentation feature used below...
    root.AddContainerChild(wxT("mydata"));
    root.Get(wxT("mydata")).AddContainerChild(wxT("mysubdata"));
    root.Get(wxT("mydata")).Get(wxT("mysubdata")).AddTextChild(wxT("subdata1"), wxT("mytext"));
    root.Get(wxT("mydata")).Get(wxT("mysubdata")).AddTextChild(wxT("subdata2"), wxT("mytext"));

    // let's test the Encapsulate function
    root.Get(wxT("mydata")).AddContainerChild(wxT("mysubdata3"));
    root.Get(wxT("mydata")).Get(wxT("mysubdata3")).Encapsulate(wxT("mysubdata2"));
    root.Get(wxT("mydata")).Get(wxT("mysubdata2")).Get(wxT("mysubdata3")).AddTextChild(wxT("subdata2"), wxT("mytext"));

    // now, save the file where the user choose
    if (doc.Save(fd.GetPath(), wxT("utf8"),
        wxXML2DOC_USE_NATIVE_NEWLINES | wxXML2DOC_USE_INDENTATION)) {

        int ret = wxMessageBox(wxT("File correctly saved. Do you want to load it ?"),
            wxT("Question"), wxYES_NO | wxICON_QUESTION);

        if (ret == wxYES) LoadXML(fd.GetPath());

    } else {

        wxMessageBox(wxT("File could not be correctly saved !!!"),
                    wxT("Error"), wxOK | wxICON_ERROR);
    }

    root.DestroyIfUnlinked();
    doc.DestroyIfUnlinked();
}

void MyFrame::OnSaveDTD(wxCommandEvent &)
{
    wxXml2DTD doc;
    wxString err;

    // ask the user where we must save
    wxFileDialog fd(this, wxT("Save DTD example file as..."), wxT(""), wxT(""),
        wxT("DTD files|*.dtd|All files|*.*"),
        wxSAVE | wxOVERWRITE_PROMPT );
    if (fd.ShowModal() == wxID_CANCEL)
        return;

    // create the DTD document:
    // NOTE: THE WXXML2 APIs FOR DTD CREATION ARE NOT VERY EXTENDED:
    //       THIS IS BECAUSE USUALLY DTD ARE NOT CREATED FROM A PROGRAM:
    //       USUALLY THEY ARE WRITTEN BY HAND AND THE PROGRAMS USE THEM
    //       TO VALIDATE XML DOCUMENTS.
    //       IF YOU NEED AN EXTENSION OF THESE FUNCTIONS, PLEASE CONTACT ME...
    doc.Create(wxXml2EmptyDoc, wxT("mydtd"), wxT("none"), wxT("none"));

    // create an element declaration and set the root
    // some errors have been experienced with some libxml2 versions previous to 2.6.16
    // (which is the one I use): unfortunately I haven't time to dig into these ones...
    wxXml2ElemContent content(wxT("myelement"), wxXML_ELEMENT_CONTENT_PCDATA);
    doc.AddElemDecl(wxT("myelement"), wxXML_ELEMENT_TYPE_ELEMENT, content);

    wxXml2ElemContent content2(wxT("myelement2"), wxXML_ELEMENT_CONTENT_ELEMENT, wxXML_ELEMENT_CONTENT_MULT);
    doc.AddElemDecl(wxT("myelement2"), wxXML_ELEMENT_TYPE_ELEMENT, content2);

    wxXml2Enumeration values(wxT("text|link"));
    doc.AddAttrDecl(wxT("mydata"), wxT("type"), wxXml2EmptyNamespace,
                    wxXML_ATTRIBUTE_ENUMERATION, wxXML_ATTRIBUTE_REQUIRED,
                    wxT("default"), values);

    wxXml2Enumeration values2(values);
    doc.AddAttrDecl(wxT("mydata"), wxT("data"), wxXml2EmptyNamespace,
                    wxXML_ATTRIBUTE_ENUMERATION, wxXML_ATTRIBUTE_REQUIRED,
                    wxT("default"), values2);

    doc.AddEntityDecl(wxT("myentity"), wxXML_INTERNAL_GENERAL_ENTITY,
                        wxT(""), wxT(""), wxT("mycontent"));


    // now, save the file where the user choose
    if (doc.Save(fd.GetPath())) {

        int ret = wxMessageBox(wxT("File correctly saved. Do you want to load it ?"),
            wxT("Question"), wxYES_NO | wxICON_QUESTION);

        if (ret == wxYES) LoadDTD(fd.GetPath());

    } else {

        wxMessageBox(wxT("File could not be correctly saved !!!"),
                    wxT("Error"), wxOK | wxICON_ERROR);
    }

    doc.DestroyIfUnlinked();
}

void MyFrame::OnValid(wxCommandEvent &)
{
    // ask the user which file we must load...
    wxFileDialog fd(this, wxT("Choose the XML file to validate"), wxT(""), wxT(""),
        wxT("XML and HTML files|*.xml;*.html;*.xhtml|All files|*.*"), wxOPEN);
    if (fd.ShowModal() == wxID_CANCEL)
        return;

    // try to do everything
    wxXml2Document doc;
    wxString err;

    // parse the file
    if (!doc.Load(fd.GetPath(), &err)) {
        wxMessageBox(wxString::Format(
            wxT("Couldn't parse the file you chose:\n%s"), err.c_str()), wxT("Error"));
        return;
    }

    wxXml2DTD dtd(doc.GetDTD());
    if (dtd == wxXml2EmptyDTD) {    // if it misses a DTD...

        // ...ask the user which DTD we must load...
        wxFileDialog dlg(this, wxT("Choose the DTD to use"), wxT(""), wxT(""),
            wxT("DTD files|*.dtd|All files|*.*"), wxOPEN);
        if (dlg.ShowModal() == wxID_CANCEL)
            return;

        // load the DTD
        wxXml2DTD dtd;
        if (!dtd.Load(dlg.GetPath(), &err)) {

            wxMessageBox(wxT("Couldn't load that DTD:\n") + err,
                    wxT("Load failed"), wxOK | wxICON_ERROR);
            return;
        }

        // and set it for our doc
        doc.SetDTD(dtd);

    } else if (dtd.IsExternalReference()) { // if it's external

        // load it
        if (!dtd.LoadFullDTD(&err)) {

            wxMessageBox(wxT("Couldn't load that DTD:\n") + err,
                    wxT("Load failed"), wxOK | wxICON_ERROR);
            return;
        }

    } else {

        wxASSERT(dtd.IsOk());
    }

    // validate the document using the DTD it now has embedded
    if (!doc.IsDTDValid(&err, TRUE)) {
        wxMessageBox(wxT("The document is not valid for the given DTD:\n\n") + err,
            wxT("Validation failed"), wxOK | wxICON_ERROR);
        return;
    }

    wxMessageBox(wxT("The document is valid for the given DTD !!"),
        wxT("Validation successful"), wxOK | wxICON_EXCLAMATION);
    doc.DestroyIfUnlinked();
}


