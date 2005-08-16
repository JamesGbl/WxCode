//---------------------------------------------------------------------------
// $RCSfile: MultiCtrlTestXML.cpp,v $
// $Source: /home/ulrich/cvs-wxcode/wxCode/components/treemultictrl/contrib/samples/xmlmapper/MultiCtrlTestXML.cpp,v $
// $Revision: 1.1 $
// $Date: 2005-08-16 19:33:33 $
//---------------------------------------------------------------------------
// Author:      Jorgen Bodde
// Copyright:   (c) Jorgen Bodde
// License:     wxWidgets License
//---------------------------------------------------------------------------

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wxTreeMultiCtrl.h"
#include "TreeMultiXmlMapper.h"

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources)
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #include "mondrian.xpm"
#endif

#define XMLMAPPER_TESTFILE "xmlmapper.xml"

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

// Define a new frame type: this is going to be our main frame
class MyAppFrame : public wxFrame
{
public:
    // ctor(s)
    MyAppFrame(const wxString& title, const wxPoint& pos, const wxSize& size,
            long style = wxDEFAULT_FRAME_STYLE);

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);

private:
    wxTreeMultiCtrl *_tmc;
	// any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    MultiCtrlTest_Quit = 1,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    MultiCtrlTest_About = wxID_ABOUT
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyAppFrame, wxFrame)
    EVT_MENU(MultiCtrlTest_Quit,  MyAppFrame::OnQuit)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

const char *tmcxml =
		"<?xml version=\"1.0\"  standalone='no' >\n"
		"<!-- Our XML for wxTreeMultiCtrl -->"
		"<wizard>\n"
        "  <category caption=\"Please enter your personal experience\" >"
        "    <check id=\"LIKEITORNOT\" caption=\"Do you like this?\" /> "
        "  </category>"
        "  <category caption=\"And now for some other fun\" >"
        "    <category caption=\"Project settings\">"
        "      <check id=\"INCLUDE_SETTINGS\" caption=\"Include a wxConfig code fragment\" /> "
        "      <check id=\"INCLUDE_ABOUT\" caption=\"Include an about dialog\" default=\"yes\"/> "
        "      <check id=\"USE_XRC\" caption=\"Setup an XRC fragment for XRC dialogs\" /> "
        "    </category>"
        "    <check id=\"CB_ONLY\" caption=\"Only checkboxes right now ;-)\" /> "
        "    <row>"
        "       <check id=\"CB_A\" caption=\"One on a row  \" />"
        "       <check id=\"CB_B\" caption=\"And another one  \" />"
        "       <button id=\"GO_OK\" caption=\"Verify!\" />"
        "    </row>"
        "    <button id=\"GO_OK2\" caption=\"Cool!\" />"
        "  </category>"
		"</wizard>";

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
	wxString str;

	str = _T("MultiCtrlTest wxWindows App - ");
	str.Append(wxVERSION_STRING);
    
	// create the main application window
    MyAppFrame *frame = new MyAppFrame(str, wxPoint(50, 50), wxSize(500, 500));

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(TRUE);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned FALSE here, the
    // application would exit immediately.
    return TRUE;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyAppFrame::MyAppFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
       : wxFrame(NULL, -1, title, pos, size, style)
{    
	// set the frame icon
    SetIcon(wxICON(mondrian));
	
	_tmc = new wxTreeMultiCtrl(this, -1);

	_tmc->SetBackgroundColour(*wxWHITE);

#ifndef LINUX
	wxTreeMultiWindowInfo wndinfo(wxTMC_BG_ADJUST_CNT, 8, 0);
#else
	wxTreeMultiWindowInfo wndinfo(wxTMC_BG_ADJUST_ALL, 8, 0);
#endif
	
    // make it a bit bigger
	wxFont somefont;
	somefont = _tmc->GetCaptionFont();
	somefont.SetFamily(wxMODERN);
	somefont.SetWeight(wxBOLD);
	somefont.SetPointSize(somefont.GetPointSize()+2);
	_tmc->SetCaptionFont(somefont);
    
    // Do the XML magic
    wxString xml(tmcxml);
    wxTreeMultiXmlMapper tmcmapper(_tmc);
    if(!tmcmapper.InitWizard(tmcxml, _("wizard")))
        wxLogError(_("Cannot parse the XML file specified!"));

    // create a menu bar
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(MultiCtrlTest_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, _T("&File"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText(_T("Welcome to the TreeMultiXmlMapper !"));
}


// event handlers

void MyAppFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

