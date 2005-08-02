////////////////////////////////////////////////////////////////////////////
// Name:        app.cpp
// Purpose:     The WebUpdater application
// Author:      Francesco Montorsi
// Modified by: 
// Created:     29/07/05
// RCS-ID:      $Id$
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------
// defines
// ----------------------------------------------------------------------------

//! The local XML m_script which is loaded by default by the WebUpdater application.
#define wxWU_LOCAL_XMLSCRIPT		wxT("local.xml")

//! The local XRC file which is loaded.
#define wxWU_LOCAL_XRC				wxT("webupdatedlg.xrc")

//! The name of our default log file.
#define wxWU_LOGFILENAME			wxT("webupdatelog.txt")


#if defined( __WXMSW__ ) && defined( __VISUALC__ )
#include <crtdbg.h>
#define mcDUMP_ON_EXIT		\
	{ _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF); }
#else
#define mcDUMP_ON_EXIT		/* expand to nothing */
#endif


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

#include <wx/xrc/xmlres.h>
#include <wx/fs_mem.h>
#include <wx/image.h>
#include <wx/wfstream.h>
#include <wx/cmdline.h>
#include <wx/textfile.h>

#include "wx/webupdatedlg.h"


// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

#include "www.xpm"

// the application icon (under Windows and OS/2 it is in resources and even
// though we could still include the XPM here it would be unused)
#if !defined(__WXMSW__) && !defined(__WXPM__)
    #include "app.xpm"
#endif


// ----------------------------------------------------------------------------
// usage instructions for this app
// ----------------------------------------------------------------------------

#define OPTION_XMLSCRIPT		wxT("cfg")
#define SWITCH_RESTART			wxT("r")
#define SWITCH_SAVELOG			wxT("s")

static const wxCmdLineEntryDesc g_cmdLineDesc[] =
{
	// options
	{ wxCMD_LINE_OPTION, OPTION_XMLSCRIPT, wxT("config"), 
		wxT("Use the given local XML file"),
		wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
		
	// switches
	{ wxCMD_LINE_SWITCH, SWITCH_RESTART, wxT("restart"), 
		wxT("Restart the updated application when WebUpdater is closed"),
		wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
	{ wxCMD_LINE_SWITCH, SWITCH_SAVELOG, wxT("savelog"), 
		wxT("Saves the log messages to 'log.txt'") },

	{ wxCMD_LINE_NONE }
};


// -----------------------------------------------------------------------------
// a little log class which saves log messages to a file (see /savelog switch)
// by Francesco Montorsi <frm@users.sourceforge.net>; wxWidgets-licensed
// -----------------------------------------------------------------------------

class wxFileLog : public wxLogPassThrough
{
	wxTextFile out;

public:
	wxFileLog(const wxString &filename, const wxString &header = wxEmptyString) { 
		wxRemoveFile(filename);
		out.Create(filename);

		// create a little header to make our log nicer
		if (!header.IsEmpty()) {
			out.AddLine(wxEmptyString);
			out.AddLine(header);
			out.AddLine(wxT(" ") + wxString(wxT('-'), 70));
			out.AddLine(wxEmptyString);
		}

		PassMessages(TRUE);
	}

	virtual ~wxFileLog() 
		{ out.Write(); out.Close(); }

	// writes to the file the log message
	virtual void DoLog(wxLogLevel level, const wxChar *msg, time_t timestamp)
		{ wxLogPassThrough::DoLog(level, msg, timestamp);
		  wxString str; TimeStamp(&str);
		  out.AddLine(wxT(" ") + str + msg); }
};


// ----------------------------------------------------------------------------
// the application
// ----------------------------------------------------------------------------

class WebUpdaterApp : public wxApp
{
public:

	// our main dialog
	wxDialog *m_dlg;

	// our local script
	wxWebUpdateLocalXMLScript m_script;

	// TRUE if the updated app will be restarted
	bool m_bRestart;

	// our log window
	wxLogWindow *m_log;

public:
	WebUpdaterApp() : m_dlg(NULL), m_bRestart(FALSE), m_log(NULL) {}
	virtual ~WebUpdaterApp() {}

	// Initializes the WebUpdaterApp.
    virtual bool OnInit();
    
    // Closes the WebUpdaterApp.
	virtual int OnExit();
};


// ----------------------------------------------------------------------------
// the dialog of the application
// ----------------------------------------------------------------------------

class WebUpdaterDlg : public wxWebUpdateDlg
{
public:
	WebUpdaterDlg(const wxWebUpdateLocalXMLScript &script)
		: wxWebUpdateDlg(NULL, script) {}
	virtual ~WebUpdaterDlg() {}


public:			// we need to intercept some events in order to make
				// this dialog work as 'frame' of our application

    void OnQuit(wxCloseEvent &event);

	virtual void EndModal(int retCode);

private:	
    DECLARE_EVENT_TABLE()
};


// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

IMPLEMENT_APP(WebUpdaterApp)
BEGIN_EVENT_TABLE(WebUpdaterDlg, wxWebUpdateDlg)
	EVT_CLOSE(WebUpdaterDlg::OnQuit)
END_EVENT_TABLE()


// ----------------------------------------------------------------------------
// WEBUPDATERAPP
// ----------------------------------------------------------------------------

bool WebUpdaterApp::OnInit()
{
	mcDUMP_ON_EXIT;			// for debugging only	
	//_CrtSetBreakAlloc(31100);

	// parse the command line
    wxCmdLineParser parser(g_cmdLineDesc, argc, argv);
    if (parser.Parse() != 0)
		return 0;		// help was shown / an error occurred

#ifdef __WXDEBUG__XXXXX				// the logframe causes problems in the exit stage
	// create an useful log window
	m_log = new wxLogWindow(NULL, wxT("WebUpdater log"));
	m_log->GetFrame()->Move(50, 50+350);
	m_log->GetFrame()->SetSize(800, 300);
#endif
	
	// check for other options & switches (ORDER IS IMPORTANT !)
	wxString toload;
	m_bRestart = parser.Found(SWITCH_RESTART);
	if (parser.Found(SWITCH_SAVELOG))
		new wxFileLog(wxWU_LOGFILENAME, wxT(" LOG OF WEBUPDATER SESSION BEGAN AT ") + 
						wxDateTime::Now().Format(wxT("%x %X")));		// it automatically installs itself as the new logger
	if (!parser.Found(OPTION_XMLSCRIPT, &toload))
		toload = wxWU_LOCAL_XMLSCRIPT;

	// this is for using wxDownloadThread
	wxSocketBase::Initialize();

	// this little snippet allows us to embed our WWW bitmap directly in the
	// webupdater app's executable so that the users of WebUpdate are not required
	// to ship together with the EXE also the www image...
	wxInitAllImageHandlers();
	wxXmlResource::Get()->InitAllHandlers();
	wxFileSystem::AddHandler(new wxMemoryFSHandler);
    wxMemoryFSHandler::AddFile(wxT("www.xpm"), wxBitmap(www_xpm), wxBITMAP_TYPE_XPM);	

	// load the local XML webupdate script
	if (!m_script.Load(toload)) {
		wxMessageBox(wxT("The installation of the WebUpdater component of this application\n")
					wxT("is corrupted (missing ") +  toload + wxT("); please reinstall the program."), wxT("Fatal error"),
					wxICON_ERROR | wxOK);	
		return FALSE;
	}
	
    // load our XRC file
    wxXmlResource::Get()->Load(wxWU_LOCAL_XRC);

	// create our main dialog
#if 1
	m_dlg = new WebUpdaterDlg(m_script);
	SetTopWindow(m_dlg);
	SetExitOnFrameDelete(TRUE);
#else

	// to test wxWebUpdateAdvPanel alone
	m_dlg = new wxDialog(NULL, -1, wxT(""), wxDefaultPosition, wxDefaultSize, 
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
	new wxWebUpdateAdvPanel(m_dlg);
	SetTopWindow(m_dlg);		
#endif

	// show the dialog...
	m_dlg->CenterOnScreen();	
	m_dlg->Show(TRUE);	
	
	// success
	return TRUE;
}

int WebUpdaterApp::OnExit()
{
	// remove the singleton objects
	delete wxWebUpdateInstaller::Set(NULL);
	
	// before exiting this app, rerun the program we've just updated
	if (m_bRestart) {
		wxExecute(
#ifdef __WXMSW__
			m_script.GetAppFile() + wxT(".exe")
#else
			wxT("./") + m_script.GetAppFile()
#endif 
		);
	}

	return wxApp::OnExit();
}



// ----------------------------------------------------------------------------
// WEBUPDATERDLG
// ----------------------------------------------------------------------------

void WebUpdaterDlg::OnQuit(wxCloseEvent &)
{
	// NOTE Since our main window is a dialog and not
	// ---- a frame we have to close it using Destroy
	//      rather than Close. In fact Close doesn't
	//      actually close a dialog but just hides it
	//      so that the application will hang there
	//      with his only window hidden and thus unable
	//      to get any user input.
    
	// --> Don't use Close with a wxDialog,
	//     use Destroy instead.
#ifdef __WXDEBUG__
	wxFrame *f = NULL;
	if (wxGetApp().m_log) f = wxGetApp().m_log->GetFrame();
	if (f) f->Close(true);
#endif
    Destroy();
	//wxExit();
}

void WebUpdaterDlg::EndModal(int)
{
	// we are not showing the dlg as modal, thus just close it
	Close(true);
}
