/////////////////////////////////////////////////////////////////////////////
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

	Minimal_Load,			// these were added by me
	Minimal_Save
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

	wxStaticText *m_text;
	void Load(const wxString &filename);
	void OnLoad(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);

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

    EVT_MENU(Minimal_Load, MyFrame::OnLoad)
    EVT_MENU(Minimal_Save, MyFrame::OnSave)
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


    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
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
	// create the wxStaticText where the file structure is shown
	m_text = new wxStaticText(this, -1, 
		"Just choose 'File->Load & parse' and you will see in this wxStaticText item\n"
		"the XML structure of the file chosen, as it is parsed by libxml2.", 
		wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE | wxALIGN_LEFT);
	m_text->SetBackgroundColour(*wxWHITE);
#endif

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *menuFile = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, _T("&About...\tF1"), _T("Show about dialog"));

    menuFile->Append(Minimal_Load, _T("Load && parse"), _T("Loads the given file and tries to parse it..."));	
    menuFile->Append(Minimal_Save, _T("Save a sample file"), _T("Creates a sample HTML file and saves it in the given location."));	
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





// helper recursive function
#define STEP			4

void ParseNode(const wxXml2Node &node, wxString &str, int n)
{
	wxString toadd, spaces(' ', n);

	// concatenate the name of this node
	toadd = node.GetName();
		
	// if this is a text node, then add also the contents...
	if (node.GetType() == wxXML_TEXT_NODE) {

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

		wxXml2Property prop = node.GetProperties();
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
	wxXml2Node child = node.GetFirstChild();
	while (child != wxXml2EmptyNode) {

		ParseNode(child, str, n+STEP);
		child = child.GetNext();

		// add a close tag because at least one child is present...
		bClose = TRUE;
	}

	if (bClose) str += wxString(' ', n) + "/" + node.GetName() + "\n";
}

void MyFrame::Load(const wxString &filename)
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
	tree.Alloc(1024);

	// get a string with the tree structure...
	ParseNode(root, tree, 3);

	// show it to the user	
	m_text->SetLabel(tree);
}


void MyFrame::OnLoad(wxCommandEvent& WXUNUSED(event))
{
	// ask the user which file we must load...
	wxFileDialog fd(this, "Choose the file to load", "", "", 
		"XML and HTML files|*.xml;*.html;*.xhtml|All files|*.*", wxOPEN);
	if (fd.ShowModal() == wxID_CANCEL)
		return;

	Load(fd.GetPath());
}

void MyFrame::OnSave(wxCommandEvent& WXUNUSED(event))
{
    wxXml2Document doc;
	wxString err;

	// ask the user where we must save
	wxFileDialog fd(this, "Save HML example file as...", "", "", 
		"XML and HTML files|*.xml;*.html;*.xhtml|All files|*.*", wxSAVE | wxOVERWRITE_PROMPT );
	if (fd.ShowModal() == wxID_CANCEL)
		return;

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
		wxT(" HTML exported by wxXml2 wrappers for libxml2 - ") \
		wxT(" write to frm@users.sourceforge.net if you encounter any ") \
		wxT(" problem or if you want to give suggestions or advices. "));

	// also add a simple paragraph... <P>text</P>
	body.AddTextChild("P",
		wxT(" HTML exported by wxXml2 wrappers for libxml2 - ") \
		wxT(" write to frm@users.sourceforge.net if you encounter any ") \
		wxT(" problem or if you want to give suggestions or advices. "));


	// now, save the file where the user chose
	if (doc.Save(fd.GetPath())) {
		
		int ret = wxMessageBox("File correctly saved. Do you want to load it ?", 
			"Question", wxYES_NO | wxICON_QUESTION);
		
		if (ret == wxYES) Load(fd.GetPath());

	} else {

		wxMessageBox("File could not be correctly saved !!!", 
					"Error", wxOK | wxICON_ERROR);
	}
}


