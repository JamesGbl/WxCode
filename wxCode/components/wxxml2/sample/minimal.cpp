////////////////////////////////////////////////////////////////////////////
// Name:        minimal.cpp
// Purpose:     Minimal wxWindows sample
// Author:      Julian Smart
// Modified by:
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
#include "wx/xml2.h"				// include libxml2 wrapper definitions
#include "wx/dtd.h"				// include libxml2 wrapper definitions

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

	Minimal_LoadXML,			// these were added by me
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
#if defined(__VISUALC__)
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
	#define new                 new(_NORMAL_BLOCK, THIS_FILE, __LINE__)
	#define mcDUMP_ON_EXIT		{ _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF); }

	
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;


	// this little class is used to access Stackwalker functions
	// without changing a line of code...
	class mcLeakDetector {

	public:
		mcLeakDetector() { /*mcDUMP_ON_EXIT;*/ }
		~mcLeakDetector() {}
	};

	// ...infact, instancing a STATIC mcLeakDetector class, we
	// can start memory-leak detection at the very beginning of
	// the program (when the main() or winmain() has not been
	// called yet, that is, when the framework is creating the
	// static variables of the program) and end it at the very
	// end of the program (when, after the main() or winmain(),
	// the framework removes the static variables).
	//static mcLeakDetector detector;

#endif



// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // create the main application window
    MyFrame *frame = new MyFrame(_T("Minimal wxWindows App"));

#if 1
	// create an useful log window
	wxLogWindow *pwindow = new wxLogWindow(frame, "log");
	pwindow->GetFrame()->Move(50, 50+350);
	pwindow->GetFrame()->SetSize(800, 300);

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);
#endif


	wxXml2::Init();


    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}


int MyApp::OnExit()
{
	wxXml2::Cleanup();
	return 0;
}


// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxPoint(50, 50), wxSize(400, 300))
{
    // set the frame icon
    SetIcon(wxICON(mondrian));

#if 1
	// create the wxTextCtrl where the file structure is shown
	m_text = new wxTextCtrl(this, -1, 
		"This program will provide some examples about the three main operations "
		"it provides on XML files:\n\n"
		"\t- Loading: wxXml2 allows you to load any XML file including DTDs.\n"
		"\t- Editing: wxXml2 allows you to create, edit, manipulate any XML file.\n"
		"\t- Saving: wxXml2 can saves XML trees or DTDs to memory buffers or to files.", 
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
    menuFile->Append(Minimal_SaveSimple, _T("Save a simple HTML file"), _T("Creates a sample HTML file and saves it in the given location."));	
	menuFile->Append(Minimal_SaveAdv, _T("Save an advanced XHTML file"), _T("Creates a sample XHTML file and saves it in the given location."));	
	menuFile->Append(Minimal_SaveDTD, _T("Save a DTD file"), _T("Creates a sample DTD file and saves it in the given location."));	
	menuFile->AppendSeparator();
	menuFile->Append(Minimal_Valid, _T("Validate against DTD"), _T("Validates an XML file against a DTD."));	
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
    msg.Printf( _T("This is a little demonstration of wxXml2 wrappers for libxml2.\n\n"
					"They are based on original Vaclav Slavik's design but they\n"
					"use libxml2 (which is a widely-used, powerful cross-platform\n"
					"library) instead of EXPAT.\n\n"
					"The classes were rewritten by Francesco Montorsi (frm@users.sourceforge.net)\n"
					"and this sample is a modified version of the minimal sample of wxWidgets."));

    wxMessageBox(msg, _T("About Minimal"), wxOK | wxICON_INFORMATION, this);
}



#define STEP			4


// helper recursive function: THIS IS ONLY A "DIDACTIVE" EXAMPLE OF A
// RECURSIVE FUNCTION WHICH CAN BE USED TO WALK AN XML TREE... 
// IF YOU JUST NEED TO OUTPUT THE CONTENTS OF AN XML TREE YOU CAN USE
// THE wxXml2Document::Save FUNCTION ON A wxMemoryOutputStream AS
// MyFrame::LoadDTD DOES TO SHOW TO THE USER THE STRUCTURE OF AN XML DTD.
void ParseNode(const wxXml2Node &node, wxString &str, int n)
{
	wxLogDebug("ParseNode - parsing [%s]", node.GetName().c_str());
	wxString toadd, spaces(' ', n);

	// concatenate the name of this node
	toadd = node.GetName();
		
	// if this is a text node, then add also the contents...
	if (node.GetType() == wxXML_TEXT_NODE ||
		node.GetType() == wxXML_COMMENT_NODE || 
		node.GetType() == wxXML_CDATA_SECTION_NODE) {

		wxString content = node.GetContent();
		if (content.Last() == '\n') content.RemoveLast();
		if (content.GetChar(0) == '\n') content.Remove(0, 1);

		// a little exception: libxml2 when loading a document creates a
		// lot of text nodes containing just a simple \n;
		// in this cases, just show "[null]"
		wxString tmp = content.Trim();
		if (tmp.IsEmpty())
			toadd += "node: [null]";
		else 
			toadd += "node: " + content;


	} else {		// if it's not a text node, then add the properties...

		wxXml2Property prop(node.GetProperties());
		while (prop != wxXml2EmptyProperty) {

			toadd += " " + prop.GetName() + "=";
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
	if (toadd != "textnode: [null]") str += toadd;
#endif

	// go one line down
	str += "\n";

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

	if (bClose) str += wxString(' ', n) + "/" + node.GetName() + "\n";
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

	// parse the file
	if (!doc.Load(filename, &err)) {
		wxMessageBox(wxString::Format("Couldn't parse the file you chose:\n%s", err.c_str()), "Error");
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
		tree += "DTD name=" + dtd.GetName();

		if (dtd.IsPublicSubset()) 
			tree +=	" PUBLIC externalID=" + dtd.GetExternalID() +
				" externalURI=" + dtd.GetExternalURI() + "\n\n";

		if (dtd.IsSystemSubset()) 
			tree +=	" SYSTEM systemID=" + dtd.GetSystemID() + "\n\n";
	}

	// get a string with the tree structure...
	ParseNodeAndSiblings(root, tree, indent);

	// show it to the user	
	m_text->SetValue(tree);

	// cleanup
	dtd.DestroyIfUnlinked();
	root.DestroyIfUnlinked();
	doc.DestroyIfUnlinked();
}

void MyFrame::LoadDTD(const wxString &filename)
{
    wxXml2DTD doc;
	wxString err;

	// parse the file
	if (!doc.Load(filename, &err)) {
		wxMessageBox(wxString::Format("Couldn't parse the file you chose:\n%s", err.c_str()), "Error");
		return;
	}

	// show the wxXml2DTD tree in a simple format...
#if 0
	wxMemoryOutputStream stream;
	wxString str;

	// we won't use the ParseNodeAndSiblings because it requires
	// wxXml2Nodes to parse: here we have instead wxXml2ElemDecl,
	// wxXml2AttrDecl... node types.
	str.Alloc(1024);
	int size = doc.Save(stream);
	
	// copy the stream into a wxString and change the text shown to the user...
	stream.CopyTo(str.GetWriteBuf(size), size);
	str.SetChar(size, '\0');
	m_text->SetValue(str);
#endif

	doc.DestroyIfUnlinked();
}

void MyFrame::OnLoadXML(wxCommandEvent& WXUNUSED(event))
{
	// ask the user which file we must load...
	wxFileDialog fd(this, "Choose the XML file to load", "", "", 
		"XML and HTML files|*.xml;*.html;*.xhtml|All files|*.*", wxOPEN);
	if (fd.ShowModal() == wxID_CANCEL)
		return;

	LoadXML(fd.GetPath());
}

void MyFrame::OnLoadDTD(wxCommandEvent& WXUNUSED(event))
{
	// ask the user which file we must load...
	wxFileDialog fd(this, "Choose the DTD to load", "", "", 
		"DTD files|*.dtd|All files|*.*", wxOPEN);
	if (fd.ShowModal() == wxID_CANCEL)
		return;

	LoadDTD(fd.GetPath());
}

void MyFrame::OnSaveSimple(wxCommandEvent& WXUNUSED(event))
{
    wxXml2Document doc;
	wxString err;

	// ask the user where we must save
	wxFileDialog fd(this, "Save simple HTML example file as...", "", "", 
		"XML and HTML files|*.xml;*.html;*.xhtml|All files|*.*",
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
	html.Get(wxT("head")).AddTextChild(wxT("title"), "Hi! I'm a sample HTML file");
	wxXml2Node body = html.AddContainerChild(wxT("body"));

	// and a comment:
	//    <!-- [sample comment] -->.
	//
	body.AddCommentChild(
		wxT(" This is a dummy comment "));

	// also add a simple paragraph... <P>text</P>
	body.AddTextChild("p",
		wxT(" HTML exported by wxXml2 wrappers for libxml2 - ") \
		wxT(" write to frm@users.sourceforge.net if you encounter any ") \
		wxT(" problem or if you want to give suggestions or advices. "));

	// now, save the file where the user choose
	if (1) {//doc.Save(fd.GetPath())) {
		
		int ret = wxMessageBox("File correctly saved. Do you want to load it ?", 
			"Question", wxYES_NO | wxICON_QUESTION);
		
		if (ret == wxYES) LoadXML(fd.GetPath());

	} else {

		wxMessageBox("File could not be correctly saved !!!", 
					"Error", wxOK | wxICON_ERROR);
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
	wxFileDialog fd(this, "Save advanced XHTML example file as...", "", "", 
		"XML and HTML files|*.xml;*.html;*.xhtml|All files|*.*",
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
	root.AddCDATAChild("my cdata block: this text is not parsed and it can "
		"contain any special character like <>'\",! except for the characters "
		"] ] > in sequence without spaces since they mark the end of this node");
	
	// create a PI node: even if we create it here, it will be automatically
	// linked at the beginning of the document...
	root.AddPIChild("xml-stylesheet", 
		"type=\"text/xsl\" href=\"http://www.w3.org/Math/XSL/mathml.xsl\"");

	// create a node containing a reference to an entity:
	// if everything works good, it will be recognized and embedded into
	// a wxXML_REFERENCE_NODE by wxXml2
	root.AddTextChild("refnode", "&copy;");

#define CHECK_UNICODE
#ifdef CHECK_UNICODE
	// then, do a little check for non-UTF8 characters
	root.AddCommentChild(
		wxT(" In the followig text child, some non-UTF8 characters are used "));
	root.AddTextChild("textchild",	wxT("/u20ac"));
#endif

	// now, save the file where the user choose
	if (doc.Save(fd.GetPath())) {
		
		int ret = wxMessageBox("File correctly saved. Do you want to load it ?", 
			"Question", wxYES_NO | wxICON_QUESTION);
		
		if (ret == wxYES) LoadXML(fd.GetPath());

	} else {

		wxMessageBox("File could not be correctly saved !!!", 
					"Error", wxOK | wxICON_ERROR);
	}

	root.DestroyIfUnlinked();
	doc.DestroyIfUnlinked();
}

void MyFrame::OnSaveDTD(wxCommandEvent &)
{
    wxXml2DTD doc;
	wxString err;

	// ask the user where we must save
	wxFileDialog fd(this, "Save DTD example file as...", "", "", 
		"DTD files|*.dtd|All files|*.*",
		wxSAVE | wxOVERWRITE_PROMPT );
	if (fd.ShowModal() == wxID_CANCEL)
		return;

	// create the DTD document
	doc.Create(wxXml2EmptyDoc, "mydtd", "none", "none");

	// create an element declaration and set the root
	doc.AddElemDecl("myelement", wxXML_ELEMENT_TYPE_ELEMENT, 
					wxXml2ElemContent("myelement", wxXML_ELEMENT_CONTENT_PCDATA));	

	doc.AddAttrDecl("mydata", "type", wxXml2EmptyNamespace,
					wxXML_ATTRIBUTE_ENUMERATION, wxXML_ATTRIBUTE_REQUIRED,
					wxEmptyString, wxXml2Enumeration("text|link"));

	wxXml2EntityDecl ent(doc, "myentity", wxXML_INTERNAL_GENERAL_ENTITY, "", "", "mycontent");
	

	// now, save the file where the user choose
	if (doc.Save(fd.GetPath())) {
		
		int ret = wxMessageBox("File correctly saved. Do you want to load it ?", 
			"Question", wxYES_NO | wxICON_QUESTION);
		
		if (ret == wxYES) LoadDTD(fd.GetPath());

	} else {

		wxMessageBox("File could not be correctly saved !!!", 
					"Error", wxOK | wxICON_ERROR);
	}
	
	doc.DestroyIfUnlinked();
}

void MyFrame::OnValid(wxCommandEvent &)
{
	// ask the user which file we must load...
	wxFileDialog fd(this, "Choose the XML file to validate", "", "", 
		"XML and HTML files|*.xml;*.html;*.xhtml|All files|*.*", wxOPEN);
	if (fd.ShowModal() == wxID_CANCEL)
		return;
	
	// try to do everything
    wxXml2Document doc;
	wxString err;

	// parse the file
	if (!doc.Load(fd.GetPath(), &err)) {
		wxMessageBox(wxString::Format("Couldn't parse the file you chose:\n%s", err.c_str()), "Error");
		return;
	}

	wxXml2DTD dtd(doc.GetDTD());
	if (dtd == wxXml2EmptyDTD) {	// if it misses a DTD...
		
		// ...ask the user which DTD we must load...
		wxFileDialog dlg(this, "Choose the DTD to use", "", "", 
			"DTD files|*.dtd|All files|*.*", wxOPEN);
		if (dlg.ShowModal() == wxID_CANCEL)
			return;
		
		// load the DTD
		wxXml2DTD dtd;
		if (!dtd.Load(dlg.GetPath(), &err)) {

			wxMessageBox("Couldn't load that DTD:\n" + err, 
					"Load failed", wxOK | wxICON_ERROR);
			return;
		}

		// and set it for our doc
		doc.SetDTD(dtd);
	
	} else if (dtd.IsExternalReference()) {	// if it's external

		// load it
		if (!dtd.LoadFullDTD(&err)) {

			wxMessageBox("Couldn't load that DTD:\n" + err, 
					"Load failed", wxOK | wxICON_ERROR);
			return;
		}

	} else {

		wxASSERT(dtd.IsOk());
	}
	
	// validate the document using the DTD it now has embedded
	if (!doc.IsDTDValid(&err, TRUE)) {
		wxMessageBox("The document is not valid for the given DTD:\n\n" + err, 
			"Validation failed", wxOK | wxICON_ERROR);
		return;
	}

	wxMessageBox("The document is valid for the given DTD !!", 
		"Validation successful", wxOK | wxICON_EXCLAMATION);
	doc.DestroyIfUnlinked();
}


