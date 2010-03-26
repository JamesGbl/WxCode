////////////////////////////////////////////////////////////////////////////
// Name:        minimal.cpp
// Purpose:     Minimal wxXml2 sample
// Author:      Julian Smart
// Modified by: Francesco Montorsi (contributions by Cecilio)
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

// to test if xml2.h header conflicts with wxXML library, include xml.h header
//#include <wx/xml/xml.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/filename.h>
#include <wx/mstream.h>
#include <wx/sstream.h>

#include "wx/xml2.h"                // include libxml2 wrapper definitions
#include "wx/dtd.h"                 // include libxml2 wrapper definitions


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

    void OnLoadXML(wxCommandEvent& event);
    void OnLoadDTD(wxCommandEvent& event);
    void OnValid(wxCommandEvent& event);

    void OnSaveSimple(wxCommandEvent& event);
    void OnSaveAdv(wxCommandEvent& event);
    void OnSaveDTD(wxCommandEvent& event);


    // helpers
    void LoadXML(const wxString &filename);
    void LoadDTD(const wxString &filename);
    void ParseNodeAndSiblings(const wxXml2Node &node, wxString &str, int n, const wxString &filename);
    void ParseNode(const wxXml2Node &node, wxString &str, int n, const wxString &filename);

private:
    wxTextCtrl *m_text;
    
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

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // create the main application window
    MyFrame *frame = new MyFrame(wxS("Minimal wxXml2 App"));

#if 1
    // create an useful log window
    wxLogWindow *pwindow = new wxLogWindow(frame, wxS("log"));
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
      wxLog::AddTraceMask( "module" );
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
        wxS("This program will provide some examples about the three main operations ")
        wxS("it provides on XML files:\n\n")
        wxS("\t- Loading: wxXml2 allows you to load any XML file including DTDs.\n")
        wxS("\t- Editing: wxXml2 allows you to create, edit, manipulate any XML file.\n")
        wxS("\t- Saving: wxXml2 can saves XML trees or DTDs to memory buffers or to files."), 
        wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);
    m_text->SetBackgroundColour(*wxWHITE);
#endif

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *menuFile = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, wxS("&About...\tF1"), wxS("Show about dialog"));

    menuFile->Append(Minimal_LoadXML, wxS("Load XML..."), 
                     wxS("Loads the given file and tries to parse it..."));
    menuFile->Append(Minimal_LoadDTD, wxS("Load DTD..."), 
                     wxS("Loads the given file and tries to parse it..."));
    menuFile->AppendSeparator();
    
    menuFile->Append(Minimal_SaveSimple, wxS("Save a simple HTML file"), 
                     wxS("Creates a sample HTML file and saves it in the given location."));  
    menuFile->Append(Minimal_SaveAdv, wxS("Save an advanced XHTML file"), 
                     wxS("Creates a sample XHTML file and saves it in the given location."));    
    menuFile->Append(Minimal_SaveDTD, wxS("Save a DTD file"), 
                     wxS("Creates a sample DTD file and saves it in the given location."));  
    menuFile->AppendSeparator();
    
    menuFile->Append(Minimal_Valid, wxS("Validate against DTD"), 
                     wxS("Validates an XML file against a DTD."));    
    menuFile->AppendSeparator();
    
    menuFile->Append(Minimal_Quit, wxS("E&xit\tAlt-X"), wxS("Quit this program"));    

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, wxS("&File"));
    menuBar->Append(helpMenu, wxS("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText(wxS("Welcome to wxWindows!"));
#endif // wxUSE_STATUSBAR
}

MyFrame::~MyFrame()
{
}



// ----------------------------------------------------------------------------
// XML helpers
// ----------------------------------------------------------------------------

#define STEP            4

// helper recursive function: THIS IS ONLY A "DIDACTIVE" EXAMPLE OF A
// RECURSIVE FUNCTION WHICH CAN BE USED TO WALK AN XML TREE... 
// IF YOU JUST NEED TO OUTPUT THE CONTENTS OF AN XML TREE YOU CAN USE
// THE wxXml2Document::Save FUNCTION ON A wxMemoryOutputStream AS
// MyFrame::LoadDTD DOES TO SHOW TO THE USER THE STRUCTURE OF AN XML DTD.
void MyFrame::ParseNode(const wxXml2Node &node, wxString &str, int n, const wxString &filename)
{
    static bool m_fExtEntity=false;             //processing an external entity
    static wxString m_sExtEntityName;           // the anem of the external entity
    
    if (node == wxXml2EmptyNode) return;
    wxLogDebug(wxS("ParseNode - parsing [%s]"), node.GetName().c_str());
    wxString toadd, spaces(wxS(' '), n);

    // concatenate the name of this node
    toadd = node.GetName();
        
    // if this is a text node, then add also the contents...
    if (node.GetType() == wxXML2_TEXT_NODE ||
        node.GetType() == wxXML2_COMMENT_NODE || 
        node.GetType() == wxXML2_CDATA_SECTION_NODE) 
    {
        wxString content = node.GetContent();
        if (content.Last() == wxS('\n')) content.RemoveLast();
        if (content.GetChar(0) == wxS('\n')) content.Remove(0, 1);

        // a little exception: libxml2 when loading a document creates a
        // lot of text nodes containing just a simple \n;
        // in this cases, just show "[null]"
        wxString tmp = content.Trim();
        if (tmp.IsEmpty())
            toadd += wxS("node: [null]");
        else 
            toadd += wxS("node: ") + content;

    }
    else if (node.GetType() == wxXML2_ENTITY_REF_NODE)
    {
        // A reference to an entity. It is like a text node, but this node contains 
        // only an "entity". Entities are strings like: &amp;  &quot;  &lt;  etc
        // A wxXML2_ENTITY_REF_NODE always has a wxXML2_ENTITY_DECL child
        toadd += wxString::Format(wxS(", Reference to external entity &%s;"), node.GetName().c_str());

        // Save the name of the node
        m_fExtEntity = true;
        m_sExtEntityName = node.GetName();
    }
    else if (node.GetType() == wxXML2_ENTITY_DECL)
    {
        // As children of an external reference node we will find the list of all declared
        // external entities, a node per entity. The first one will be the one corresponding
        // to the previously found wxXML2_ENTITY_REF_NODE. 
        wxXml2EntityDecl* pNode = (wxXml2EntityDecl*)&node;
        toadd += wxString::Format(wxS(", Entity declaration, name='%s', SystemID='%s'\n"),
                                  node.GetName().c_str(), pNode->GetSystemID().c_str() );

        // if this decalarion is the taht corresponds to the refeneced external entity
        // insert here the referenced xml tree
        if (m_fExtEntity && m_sExtEntityName == node.GetName())
        {
            // load the referenced XML file as tree of nodes
            wxXml2Document oDoc;
            wxString sError;

            // TODO:
            // In this sample I am assuming that the external entity URL is just
            // "filename.ext", that is, it is located at the same folder than
            // main XML file.
            // In general, this is a non-valid assuption and so, more code must be
            // inserted to compose the right URL.
            //----------------------------------------------
            wxFileName oFN(filename);
            oFN.SetFullName(pNode->GetSystemID());
            wxString sFilename = oFN.GetFullPath();

            if (!oDoc.Load(sFilename, &sError)) 
            {
                wxLogError(wxS("Error parsing file %s\nError:%s\n"),
                           sFilename.c_str(), sError.c_str());
                return;
            }

            // Process it recursively
            m_fExtEntity = false;
            m_sExtEntityName = wxEmptyString;

            wxXml2Node oRoot = oDoc.GetRoot();
            toadd += spaces;
            toadd += wxString::Format(wxS("Inserting and processing file %s\n"), sFilename.c_str());
            toadd += spaces + wxS("------------------------------------\n");
            
            wxString sChildTree;
            ParseNodeAndSiblings(oRoot, sChildTree, n, sFilename);
            toadd += sChildTree;
            
            toadd += spaces + wxS("------------------------------------\n");
            toadd += spaces + wxS("Returning to previous file\n");
            toadd += spaces + wxS("------------------------------------\n");
        }
    }
    else    // if it's not a text node, then add the properties...
    {      
        wxXml2Property prop(node.GetProperties());
        while (prop != wxXml2EmptyProperty) {

            toadd += wxS(" ") + prop.GetName() + wxS("=");
            toadd += prop.GetValue();

            prop = prop.GetNext();
        }
    }
        
    str += spaces;

#define SHOW_ANNOYING_NEWLINES      0
#if SHOW_ANNOYING_NEWLINES   

    // text nodes with newlines and/or spaces will be shown as [null]
    str += toadd;
#else

    // text nodes with newlines won't be shown at all
    if (toadd != wxS("textnode: [null]")) str += toadd;
#endif

    // go one line down
    str += wxS("\n");

    // do we must add the close tag ?
    bool bClose = FALSE;

    // and then, a step more indented, its children
    wxXml2Node child(node.GetFirstChild());
    while (child != wxXml2EmptyNode) {

        ParseNode(child, str, n+STEP, filename);
        child = child.GetNext();

        // add a close tag because at least one child is present...
        bClose = TRUE;
    }

    if (bClose) str += wxString(wxS(' '), n) + wxS("/") + node.GetName() + wxS("\n");
}

void MyFrame::ParseNodeAndSiblings(const wxXml2Node &node, wxString &str, int n, 
                                    const wxString &filename)
{
    wxXml2Node curr(node);

    do {
        ParseNode(curr, str, n, filename);
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
            wxS("Couldn't parse the file you chose:\n%s"), err.c_str()), wxS("Error"));
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
        tree += wxS("DTD name=") + dtd.GetName();

        if (dtd.IsPublicSubset()) 
            tree += wxS(" PUBLIC externalID=") + dtd.GetExternalID() +
                wxS(" externalURI=") + dtd.GetExternalURI() + wxS("\n\n");

        if (dtd.IsSystemSubset()) 
            tree += wxS(" SYSTEM systemID=") + dtd.GetSystemID() + wxS("\n\n");
    }

    // get a string with the tree structure...
    ParseNodeAndSiblings(root, tree, indent, filename);

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
            wxS("Couldn't parse the file you chose:\n%s"), err.c_str()), wxS("Error"));
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



// ----------------------------------------------------------------------------
// event handlers
// ----------------------------------------------------------------------------

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf( wxS("This is a little demonstration of wxXml2 wrappers for libxml2.\n\n")
                wxS("They are based on original Vaclav Slavik's design but they\n")
                wxS("use libxml2 (which is a widely-used, powerful cross-platform\n")
                wxS("library) instead of EXPAT.\n\n")
                wxS("The classes were rewritten by Francesco Montorsi (frm@users.sourceforge.net)\n")
                wxS("and this sample is a modified version of the minimal sample of wxWidgets.\n\n")
                wxS("This sample program was compiled with libxml2 version %s."),
                wxXml2::GetLibxml2Version().c_str());

    wxMessageBox(msg, wxS("About Minimal"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnLoadXML(wxCommandEvent& WXUNUSED(event))
{
    // ask the user which file we must load...
    wxFileDialog fd(this, wxS("Choose the XML file to load"), wxS(""), wxS(""), 
                    wxS("XML and HTML files|*.xml;*.html;*.xhtml|All files|*.*"), wxFD_OPEN);
    if (fd.ShowModal() == wxID_CANCEL)
        return;

    LoadXML(fd.GetPath());

#define DO_TEST_SAVE 0
#if DO_TEST_SAVE
    // ask the user where to save it now
    wxFileDialog fd2(this, wxS("Choose the XML file to save on"), wxS(""), wxS(""),
                     wxS("XML and HTML files|*.xml;*.html;*.xhtml|All files|*.*"), wxFD_SAVE);
    if (fd2.ShowModal() == wxID_CANCEL)
        return;
#endif

    wxXml2Document tmpDoc;
    tmpDoc.Load(fd.GetPath());
#if DO_TEST_SAVE
    tmpDoc.Save(fd2.GetPath());//, wxS("UTF-8"), 0);
#endif
}

void MyFrame::OnLoadDTD(wxCommandEvent& WXUNUSED(event))
{
    // ask the user which file we must load...
    wxFileDialog fd(this, wxS("Choose the DTD to load"), wxS(""), wxS(""), 
                    wxS("DTD files|*.dtd|All files|*.*"), wxFD_OPEN);
    if (fd.ShowModal() == wxID_CANCEL)
        return;

    LoadDTD(fd.GetPath());
}

void MyFrame::OnSaveSimple(wxCommandEvent& WXUNUSED(event))
{
    wxXml2Document doc;
    wxString err;

    // ask the user where we must save
    wxFileDialog fd(this, wxS("Save simple HTML example file as..."), wxS(""), wxS(""), 
                    wxS("XML and HTML files|*.xml;*.html;*.xhtml|All files|*.*"),
                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT );
    if (fd.ShowModal() == wxID_CANCEL)
        return;

    // create an XML version 1.0
    doc.Create();

    // create the HTML header if required
    wxXml2Node html;
    html.CreateRoot(doc, wxS("html"));

    // set HTML header:
    //    <html lang="en">
    //
    html.AddProperty(wxS("lang"), wxS("en"));

    // add the head, title & body tags:
    //   <HEAD><TITLE> [title] </TITLE></HEAD>
    //   <BODY></BODY>
    //
    html.AddContainerChild(wxS("head"));
    html.Get(wxS("head")).AddTextChild(wxS("title"), wxS("Hi! I'm a sample HTML file"));
    wxXml2Node body = html.AddContainerChild(wxS("body"));

    // and a comment:
    //    <!-- [sample comment] -->.
    //
    body.AddCommentChild(
        wxS(" This is a dummy comment "));

    // also add a simple paragraph... <P>text</P>
    body.AddTextChild(wxS("p"),
        wxS(" HTML exported by wxXml2 wrappers for libxml2 - ") \
        wxS(" write to frm@users.sourceforge.net if you encounter any ") \
        wxS(" problem or if you want to give suggestions or advices. "));

    // now, save the file where the user choose
    if (doc.Save(fd.GetPath())) {
        
        int ret = wxMessageBox(wxS("File correctly saved. Do you want to load it ?"), 
                               wxS("Question"), wxYES_NO | wxICON_QUESTION);
        
        if (ret == wxYES) LoadXML(fd.GetPath());

    } else {

        wxMessageBox(wxS("File could not be correctly saved !!!"), 
                    wxS("Error"), wxOK | wxICON_ERROR);
    }

    html.DestroyIfUnlinked();
    body.DestroyIfUnlinked();
    doc.DestroyIfUnlinked();
}

void MyFrame::OnSaveAdv(wxCommandEvent& WXUNUSED(event))
{
    wxXml2Document doc;
    wxString err;

    // ask the user where we must save
    wxFileDialog fd(this, wxS("Save advanced XHTML example file as..."), wxS(""), wxS(""), 
                    wxS("XML and HTML files|*.xml;*.html;*.xhtml|All files|*.*"),
                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT );
    if (fd.ShowModal() == wxID_CANCEL)
        return;

    // create an XML version 1.0
    doc.Create();

    // create the HTML header if required
    wxXml2Node root;
    root.CreateRoot(doc, wxS("advanced_sample"));
    
    // create a MathML DTD and set it for this doc
    doc.SetMathMLDTD();

    // create a CDATA node  
    root.AddCDATAChild(wxS("my cdata block: this text is not parsed and it can ")
        wxS("contain any special character like <>'\",! except for the characters ")
        wxS("] ] > in sequence without spaces since they mark the end of this node"));
    
    // create a PI node: even if we create it here, it will be automatically
    // linked at the beginning of the document...
    root.AddPIChild(wxS("xml-stylesheet"), 
        wxS("type=\"text/xsl\" href=\"http://www.w3.org/Math/XSL/mathml.xsl\""));

    // create a node containing a reference to an entity:
    // if everything works good, it will be recognized and embedded into
    // a wxXML2_REFERENCE_NODE by wxXml2
    root.AddTextChild(wxS("refnode"), wxS("&copy;"));

#if wxUSE_UNICODE
    // then, do a little check for Unicode
    root.AddCommentChild(
        wxS("In the followig text child, some non-ANSI (i.e. Unicode) characters are used; ")
        wxS("If you see a greek ALPHA, BETA and GAMMA characters, it means that you are ")
        wxS("using a viewer which supports the Unicode standard."));

    wxString greekstring = wxS("\u03B1\u03B2\u03B3");
    root.AddTextChild(wxS("textchild"), greekstring);
#endif

    // create some simple nodes just to highlight the indentation feature used below...
    root.AddContainerChild(wxS("mydata"));
    root.Get(wxS("mydata")).AddContainerChild(wxS("mysubdata"));
    root.Get(wxS("mydata")).Get(wxS("mysubdata")).AddTextChild(wxS("subdata1"), wxS("mytext"));
    root.Get(wxS("mydata")).Get(wxS("mysubdata")).AddTextChild(wxS("subdata2"), wxS("mytext"));

    // let's test the Encapsulate function
    root.Get(wxS("mydata")).AddContainerChild(wxS("mysubdata3"));
    root.Get(wxS("mydata")).Get(wxS("mysubdata3")).Encapsulate(wxS("mysubdata2"));
    root.Get(wxS("mydata")).Get(wxS("mysubdata2")).Get(wxS("mysubdata3")).AddTextChild(wxS("subdata2"), wxS("mytext"));

    // now, save the file where the user choose
    if (doc.Save(fd.GetPath(), wxS("utf8"), 
        wxXML2DOC_USE_NATIVE_NEWLINES | wxXML2DOC_USE_INDENTATION)) {
        
        int ret = wxMessageBox(wxS("File correctly saved. Do you want to load it ?"), 
            wxS("Question"), wxYES_NO | wxICON_QUESTION);
        
        if (ret == wxYES) LoadXML(fd.GetPath());

    } else {

        wxMessageBox(wxS("File could not be correctly saved !!!"), 
                    wxS("Error"), wxOK | wxICON_ERROR);
    }

    root.DestroyIfUnlinked();
    doc.DestroyIfUnlinked();
}

void MyFrame::OnSaveDTD(wxCommandEvent& WXUNUSED(event))
{
    wxXml2DTD doc;
    wxString err;

    // ask the user where we must save
    wxFileDialog fd(this, wxS("Save DTD example file as..."), wxS(""), wxS(""), 
                    wxS("DTD files|*.dtd|All files|*.*"),
                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT );
    if (fd.ShowModal() == wxID_CANCEL)
        return;

    // create the DTD document:
    // NOTE: THE WXXML2 APIs FOR DTD CREATION ARE NOT VERY EXTENDED:
    //       THIS IS BECAUSE USUALLY DTD ARE NOT CREATED FROM A PROGRAM:
    //       USUALLY THEY ARE WRITTEN BY HAND AND THE PROGRAMS USE THEM
    //       TO VALIDATE XML DOCUMENTS.
    //       IF YOU NEED AN EXTENSION OF THESE FUNCTIONS, PLEASE CONTACT ME...
    doc.Create(wxXml2EmptyDoc, wxS("mydtd"), wxS("none"), wxS("none"));

    // create an element declaration and set the root
    // some errors have been experienced with some libxml2 versions previous to 2.6.16
    // (which is the one I use): unfortunately I haven't time to dig into these ones...
    wxXml2ElemContent content(wxS("myelement"), wxXML2_ELEMENT_CONTENT_PCDATA);
    doc.AddElemDecl(wxS("myelement"), wxXML2_ELEMENT_TYPE_ELEMENT, content); 

    wxXml2ElemContent content2(wxS("myelement2"), wxXML2_ELEMENT_CONTENT_ELEMENT, wxXML2_ELEMENT_CONTENT_MULT);
    doc.AddElemDecl(wxS("myelement2"), wxXML2_ELEMENT_TYPE_ELEMENT, content2);

    wxXml2Enumeration values(wxS("text|link"));
    doc.AddAttrDecl(wxS("mydata"), wxS("type"), wxXml2EmptyNamespace,
                    wxXML2_ATTRIBUTE_ENUMERATION, wxXML2_ATTRIBUTE_REQUIRED,
                    wxS("default"), values);

    wxXml2Enumeration values2(values);
    doc.AddAttrDecl(wxS("mydata"), wxS("data"), wxXml2EmptyNamespace,
                    wxXML2_ATTRIBUTE_ENUMERATION, wxXML2_ATTRIBUTE_REQUIRED,
                    wxS("default"), values2);
    
    doc.AddEntityDecl(wxS("myentity"), wxXML2_INTERNAL_GENERAL_ENTITY, 
                        wxS(""), wxS(""), wxS("mycontent"));


    // now, save the file where the user choose
    if (doc.Save(fd.GetPath())) {
        
        int ret = wxMessageBox(wxS("File correctly saved. Do you want to load it ?"), 
            wxS("Question"), wxYES_NO | wxICON_QUESTION);
        
        if (ret == wxYES) LoadDTD(fd.GetPath());

    } else {

        wxMessageBox(wxS("File could not be correctly saved !!!"), 
                    wxS("Error"), wxOK | wxICON_ERROR);
    }
    
    doc.DestroyIfUnlinked();
}

void MyFrame::OnValid(wxCommandEvent& WXUNUSED(event))
{
    // ask the user which file we must load...
    wxFileDialog fd(this, wxS("Choose the XML file to validate"), wxS(""), wxS(""), 
                    wxS("XML and HTML files|*.xml;*.html;*.xhtml|All files|*.*"), wxFD_OPEN);
    if (fd.ShowModal() == wxID_CANCEL)
        return;
    
    // try to do everything
    wxXml2Document doc;
    wxString err;

    // parse the file
    if (!doc.Load(fd.GetPath(), &err)) {
        wxMessageBox(wxString::Format(
            wxS("Couldn't parse the file you chose:\n%s"), err.c_str()), wxS("Error"));
        return;
    }

    wxXml2DTD dtd(doc.GetDTD());
    if (dtd == wxXml2EmptyDTD) {    // if it misses a DTD...
        
        // ...ask the user which DTD we must load...
        wxFileDialog dlg(this, wxS("Choose the DTD to use"), wxS(""), wxS(""), 
                         wxS("DTD files|*.dtd|All files|*.*"), wxFD_OPEN);
        if (dlg.ShowModal() == wxID_CANCEL)
            return;
        
        // load the DTD
        wxXml2DTD dtd;
        if (!dtd.Load(dlg.GetPath(), &err)) {

            wxMessageBox(wxS("Couldn't load that DTD:\n") + err, 
                         wxS("Load failed"), wxOK | wxICON_ERROR);
            return;
        }

        // and set it for our doc
        doc.SetDTD(dtd);
    
    } else if (dtd.IsExternalReference()) { // if it's external

        // load it
        if (!dtd.LoadFullDTD(&err)) {

            wxMessageBox(wxS("Couldn't load that DTD:\n") + err, 
                         wxS("Load failed"), wxOK | wxICON_ERROR);
            return;
        }

    } else {

        wxASSERT(dtd.IsOk());
    }
    
    // validate the document using the DTD it now has embedded
    if (!doc.IsDTDValid(&err, TRUE)) {
        wxMessageBox(wxS("The document is not valid for the given DTD:\n\n") + err, 
                     wxS("Validation failed"), wxOK | wxICON_ERROR);
        return;
    }

    wxMessageBox(wxS("The document is valid for the given DTD !!"), 
                 wxS("Validation successful"), wxOK | wxICON_EXCLAMATION);
    doc.DestroyIfUnlinked();
}


