/////////////////////////////////////////////////////////////////////////////
// Name:        httpenginedlg.cpp
// Purpose:     HTTPE Engine Dialog
// Author:      Angelo Mandato
// Created:     2005/08/11
// RCS-ID:      $Id$
// Copyright:   (c) 2002-2005 Angelo Mandato
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "httpenginedlg.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#ifndef WX_PRECOMP
	// here goes the #include <wx/abc.h> directives for those
	// files which are not included by wxprec.h
	#include <wx/wx.h>
	#include <wx/filename.h>
	#include <wx/msgdlg.h>
#endif

#include <wx/filename.h>

#include "httpengineapp.h"
#include "httpenginedlg.h"

#include <wx/httpbuilder.h>
#include <wx/httpbuilderthread.h>
#include <wx/authdlg.h>
#include <wx/proxysettingsdlg.h>


// For demonstration purposes:
#define USETHREAD
#define USEPLEASEWAITDLG
#define TESTRESUMECANCEL
#define TESTPROXYSETTINGSCLASS
#define TESTHTTPAUTHSETTINGSCLASS

#define DEFAULT_URL wxT("http://localhost/phpinfo.php")

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------
// the application icon
#if defined(__WXGTK__) || defined(__WXMOTIF__)
    #include "httpengine.xpm"
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Minimal_Quit = 1,
    Minimal_About,
    Minimal_Go,
		Minimal_HeadGo,
    Minimal_AddField,

    Minimal_ProxySettings,
    Minimal_Authentication,
		Minimal_SaveResults,

    Minimal_UseProxySettings,
    Minimal_UseAuthentication,
    Minimal_Stop,

    Minimal_Thread,
		Minimal_Thread2,

    Minimal_Timer,

};



wxGetInputFromUser::wxGetInputFromUser(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxDialog( parent, -1, title, pos, size)
{
    // Create listbox
    m_ctrl = new wxTextCtrl(this, -1, wxT(""), wxPoint(5, 35), wxSize( 384, 202 ), wxTE_MULTILINE );

    new wxButton(this, wxID_OK, _T("OK"), wxPoint(5, 245), wxSize(70, 25));
    new wxButton(this, wxID_CANCEL, _T("Cancel"), wxPoint(242, 245), wxSize(70, 25));
}

// Dialog destructor
wxGetInputFromUser::~wxGetInputFromUser()
{

}


// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
//IMPLEMENT_CLASS(wxHTTPEngineDialog, wxDialog)
//BEGIN_EVENT_TABLE(wxHTTPEngineDialog, wxDialog)

IMPLEMENT_CLASS(wxHTTPEngineDialog, wxFrame)
BEGIN_EVENT_TABLE(wxHTTPEngineDialog, wxFrame)

    EVT_MENU(Minimal_ProxySettings,  wxHTTPEngineDialog::OnProxySettings)
    EVT_MENU(Minimal_Authentication,  wxHTTPEngineDialog::OnAuthentication)

    EVT_BUTTON(Minimal_Quit,  wxHTTPEngineDialog::OnQuit)
    EVT_MENU(Minimal_Quit,  wxHTTPEngineDialog::OnQuit)

    EVT_BUTTON(Minimal_About, wxHTTPEngineDialog::OnAbout)
    EVT_MENU(Minimal_About, wxHTTPEngineDialog::OnAbout)

    EVT_BUTTON(Minimal_Go, wxHTTPEngineDialog::OnGo)
		EVT_MENU(Minimal_Go, wxHTTPEngineDialog::OnGo)
		EVT_MENU(Minimal_HeadGo, wxHTTPEngineDialog::OnHeadGo)
    EVT_BUTTON(Minimal_AddField, wxHTTPEngineDialog::OnAddField)
    EVT_CLOSE(wxHTTPEngineDialog::OnClose)

    EVT_MENU(Minimal_UseProxySettings, wxHTTPEngineDialog::OnUseProxy)
    EVT_MENU(Minimal_UseAuthentication, wxHTTPEngineDialog::OnUseAuth)
		EVT_MENU(Minimal_SaveResults, wxHTTPEngineDialog::OnSaveResults)
		

    
    EVT_UPDATE_UI(Minimal_Go, wxHTTPEngineDialog::OnUpdate)
		EVT_UPDATE_UI(Minimal_HeadGo, wxHTTPEngineDialog::OnUpdate)
    EVT_UPDATE_UI(Minimal_AddField, wxHTTPEngineDialog::OnUpdate)
    EVT_UPDATE_UI(Minimal_ProxySettings, wxHTTPEngineDialog::OnUpdate)
    EVT_UPDATE_UI(Minimal_Authentication, wxHTTPEngineDialog::OnUpdate)
    EVT_UPDATE_UI(Minimal_UseProxySettings, wxHTTPEngineDialog::OnUpdate)
		EVT_UPDATE_UI(Minimal_SaveResults, wxHTTPEngineDialog::OnUpdate)
		
    EVT_UPDATE_UI(Minimal_UseAuthentication, wxHTTPEngineDialog::OnUpdate)
    EVT_UPDATE_UI(Minimal_Stop, wxHTTPEngineDialog::OnUpdate)

    EVT_HTTPBUILDER_FINISHED(Minimal_Thread, wxHTTPEngineDialog::OnDownloadComplete)
		EVT_HTTPBUILDER_FINISHED(Minimal_Thread2, wxHTTPEngineDialog::OnDownloadComplete2)

    EVT_TIMER(Minimal_Timer, wxHTTPEngineDialog::OnTimer)

END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// main Dialog
// ----------------------------------------------------------------------------

// Dialog constructor
wxHTTPEngineDialog::wxHTTPEngineDialog(const wxString& title, const wxPoint& pos, const wxSize& size)
      // : wxDialog((wxDialog *) NULL, -1, title, pos, size)
       : wxFrame((wxFrame *) NULL, -1, title, pos, size, wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
  // Load previously saved settings
  wxConfigBase *pConfig = wxConfigBase::Get();
  m_authSettings.AuthSettingsLoad(*pConfig);
  m_proxySettings.ProxySettingsLoad(*pConfig);

  m_thread = NULL;
  m_http = NULL;
  m_timer = NULL;
	m_pleaseWaitDlg = NULL;
	
  // Menus
    wxMenu *fileMenu = new wxMenu;
    wxMenu *optionsMenu = new wxMenu;
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, _T("&About...\tF1"), _T("Show about dialog"));

    // Options:
    optionsMenu->AppendCheckItem(Minimal_UseProxySettings, _T("Use Proxy"), _T("Use HTTP Proxy Server"));
    optionsMenu->AppendCheckItem(Minimal_UseAuthentication, _T("Authenticate"), _T("Use Basic Authentication"));
		wxMenuItem *tmp = optionsMenu->AppendCheckItem(Minimal_SaveResults, _T("Save Results"), _T("Save results as file, otherwise view in textbox"));
		tmp->Check(true);
    optionsMenu->AppendSeparator();
		optionsMenu->Append(Minimal_Go, _T("Go"), _T("Send built information and receive results."));
		optionsMenu->Append(Minimal_HeadGo, _T("Go (HEAD)"), _T("Send built information and receive headers only."));
    optionsMenu->Append(Minimal_Stop, _T("Stop"), _T("Stop downloading"));

    fileMenu->Append(Minimal_ProxySettings, _T("Proxy Settings"), _T("Set proxy settings"));
    fileMenu->Append(Minimal_Authentication, _T("Authenticate"), _T("Set HTTP Authentication settings"));
    fileMenu->AppendSeparator();
    fileMenu->Append(Minimal_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, _T("&File"));
    menuBar->Append(optionsMenu, _T("&Options"));
    menuBar->Append(helpMenu, _T("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);


    wxPanel *panel = new wxPanel(this);
    // set the Dialog icon
    SetIcon(wxICON(wxHTTPEngine));

    //m_sock = new wxSocketClient();
    
    new wxStaticText(panel, -1, _T("URL:"), wxPoint( 5, 5), wxSize(30, 22) );
    m_txtUrl = new wxTextCtrl(panel,-1,_T("http://localhost/phpinfo.php"), wxPoint(60,5), wxSize(250, -1) );
    wxButton* downloadButton = new wxButton(panel, Minimal_Go, _T("Go"), wxPoint(318, 5), wxSize(70, 23) );
    downloadButton = NULL;
  
    // Create listbox
    m_lcData = new wxListCtrl(panel, -1, wxPoint(5, 35), wxSize( 384, 202 ), wxLC_REPORT|wxSUNKEN_BORDER|wxLC_EDIT_LABELS );

    new wxButton(panel, Minimal_AddField, _T("Add Field"), wxPoint(5, 245), wxSize(70, 25));
    new wxButton(panel, Minimal_About, _T("About"), wxPoint(242, 245), wxSize(70, 25));
    new wxButton(panel, Minimal_Quit, _T("Quit"), wxPoint(318, 245), wxSize(70, 25));

    // Initialize m_lcData with 3 columsn: 
    m_lcData->InsertColumn(0, wxT("Name"));
    m_lcData->InsertColumn(1, wxT("Value"));
    m_lcData->InsertColumn(2, wxT("Type"));

    m_lcData->SetColumnWidth( 0, 120 );
    m_lcData->SetColumnWidth( 1, 200 );
    m_lcData->SetColumnWidth( 2, 45 );


    m_bUseProxy = false;        
    m_szProxyHost = wxT("proxy");
    m_nProxyPort = 80;
    m_bProxyAuth = false;
    m_szProxyUsername = wxT("");
    m_szProxyPassword = wxT("");

    m_bUseAuth = false;
    m_szAuthUsername = wxT("");
    m_szAuthPassword = wxT("");
    
		m_bSaveResults = true;

    CreateStatusBar(2);
}

// Dialog destructor
wxHTTPEngineDialog::~wxHTTPEngineDialog()
{
  //delete m_sock;

  // Save previously set settings
  wxConfigBase *pConfig = wxConfigBase::Get();
  m_authSettings.AuthSettingsSave(*pConfig);
  m_proxySettings.ProxySettingsSave(*pConfig);
}

// event handlers

void wxHTTPEngineDialog::OnQuit(wxCommandEvent &)
{
  Destroy();
}

void wxHTTPEngineDialog::OnAbout(wxCommandEvent &)
{
  AboutDialog dlgAbout( this,
                 -1,
                 wxT("About wxHTTPEngine"), wxDefaultPosition, wxSize(350, 200) );
  dlgAbout.Center();
	dlgAbout.ShowModal();

  //wxMessageBox( "wxHTTPEngine, a wxHTTPBuilder example application.\n(c) 2005 Angelo Mandato (http://www.spaceblue.com/)");
}

void wxHTTPEngineDialog::OnProxySettings(wxCommandEvent &event)
{
  wxUnusedVar(event);
  wxProxySettingsDlg dlg(this, -1, wxT("Proxy Settings"), wxT(""), true );

#ifdef TESTPROXYSETTINGSCLASS
  dlg.SetProxySettings(m_proxySettings);
#else
  dlg.SetIcon( GetIcon() );
  dlg.SetHost(m_szProxyHost);
  dlg.SetPortNumber(m_nProxyPort);
  dlg.SetUsername(m_szProxyUsername);
  dlg.SetPassword(m_szProxyPassword);
  dlg.SetAuthProxy(m_bProxyAuth);
  dlg.SetHostExceptions(m_szProxyExceptions);
#endif
  // Your custom exception descriptiono and note
  //dlg.SetHostExceptionsDesc( _T("") );
  //dlg.SetHostExceptionsNote( _T("") );

  if( dlg.ShowModal() == wxID_OK )
  {
#ifdef TESTPROXYSETTINGSCLASS
    m_proxySettings = dlg.GetProxySettings();
#else
    m_szProxyHost = dlg.GetHost();
    m_nProxyPort = dlg.GetPortNumber();
    m_szProxyUsername = dlg.GetUsername( );
    m_szProxyPassword = dlg.GetPassword( );
    m_bProxyAuth = dlg.IsAuthProxy();
    m_szProxyExceptions = dlg.GetHostExceptions();
#endif
  }
}


void wxHTTPEngineDialog::OnAuthentication(wxCommandEvent &event)
{
  wxUnusedVar(event);

  // Demonstrates the 4 different types of authentication dialogs you may want to make:
  wxAuthenticateDlg dlg(this, -1, wxT("Authenticate"), wxT("Enter Username and Password for \"Test\""), true );
  //wxAuthenticateDlg dlg(this, -1, wxT("Authenticate"), wxT("Enter Username and Password for \")Test\"", false );
  //wxAuthenticateDlg dlg(this, -1, wxT("Authenticate"), "", false );
  //wxAuthenticateDlg dlg(this, -1, wxT("Authenticate"), "", true );
#ifdef TESTHTTPAUTHSETTINGSCLASS
  dlg.SetAuthSettings(m_authSettings);
#else
  dlg.SetUsername(m_szAuthUsername);
  dlg.SetPassword(m_szAuthPassword);
  dlg.SetRememberPassword(m_bRememberPasswd);
#endif
  if( dlg.ShowModal() == wxID_OK )
  {
#ifdef TESTHTTPAUTHSETTINGSCLASS
    m_authSettings = dlg.GetAuthSettings();
#else
    m_szAuthUsername = dlg.GetUsername();
    m_szAuthPassword = dlg.GetPassword();
    m_bRememberPasswd = dlg.GetRememberPassword();
#endif
  }
}


void wxHTTPEngineDialog::OnClose(wxCloseEvent &event)
{
  if( (m_thread||m_http) && event.CanVeto() )
  {
    wxMutexLocker lock( m_mutexHTTPObj);

    event.Veto();
    if( m_thread )
    {
      m_thread->Kill();
      m_thread = NULL;
    }
    if( m_http )
    {
      m_http->Stop(true);

      delete m_http;
      m_http = NULL;
    }
  }
  else
  {
    Destroy();
  }
}

void wxHTTPEngineDialog::OnGo(wxCommandEvent &)
{
  if( m_http )
    return;

  wxString szURL = m_txtUrl->GetValue();
  if( szURL.IsEmpty() )
  {
    wxMessageBox( _T("You did not enter a URL.") );
    return;
  }

	wxString szSaveAs = wxT("");
	if( m_bSaveResults )
	{
		szSaveAs = wxFileSelector( wxT("Save downloaded content as"), wxT(""), wxT(""), wxT(""), wxT("*.*"), wxSAVE|wxOVERWRITE_PROMPT, this );
		if( szSaveAs.IsEmpty() )
			return;
	}
	else
	{
		szSaveAs = wxFileName::CreateTempFileName( wxT("test") );
		m_szTempFileName = szSaveAs;
		wxRemove(szSaveAs );
	}
	/*
	wxGetInputFromUser dlg(this, wxT("Enter Multipart Data"), wxPoint(-1, -1), wxSize(400, 300) );
		dlg.set
    if( dlg.ShowModal() == wxID_OK )
      szValue = dlg.GetValue();
	*/
		


  // wxHTTP class file download example:
  //wxHTTPBuilder *http = new wxHTTPBuilder();
  
  m_http = new wxHTTPBuilder();
  m_http->InitContentTypes(); // Initialise the content types on the page


  if( m_bUseProxy )
  {
#ifdef TESTPROXYSETTINGSCLASS
    wxProxySettings set;
    m_proxySettings.SetUseProxy(true);
    m_http->SetProxySettings(m_proxySettings);
#else
    m_http->HttpProxy(m_szProxyHost, m_nProxyPort );
    if( m_bProxyAuth )
      m_http->HttpProxyAuth( m_szProxyUsername, m_szProxyPassword);
#endif
  }
  
  if( m_bUseAuth )
  {
#ifdef TESTHTTPAUTHSETTINGSCLASS
    m_authSettings.SetBasicAuth(); // Set the class to use the authentication settings
    m_http->SetAuthentication( m_authSettings );
#else
    m_http->Authenticate(m_szAuthUsername, m_szAuthPassword);
#endif
  }

  // Add some values:
  for( unsigned long x = 0; x < (unsigned long)m_lcData->GetItemCount(); x++ )
  {
    wxString szName = m_lcData->GetItemText(x);
    wxString szValue = wxT("");
    wxString szType = wxT("");
    wxListItem item;
    item.m_itemId = x;
    item.m_col = 1;
    item.m_mask = wxLIST_MASK_TEXT;
    if( m_lcData->GetItem( item ) )
      szValue = item.m_text;
    item.m_itemId = x;
    item.m_col = 2;
    item.m_mask = wxLIST_MASK_TEXT;
    if( m_lcData->GetItem( item ) )
    {
      szType = item.m_text;

      wxHTTPBuilder::wxHTTP_Type nType = wxHTTPBuilder::wxHTTP_TYPE_ANY;
      if( szType == wxT("GET") )
        nType = wxHTTPBuilder::wxHTTP_TYPE_GET;
      if( szType == wxT("POST") )
        nType = wxHTTPBuilder::wxHTTP_TYPE_POST;
      if( szType == wxT("FILE") )
        nType = wxHTTPBuilder::wxHTTP_TYPE_FILE;
      if( szType == wxT("MULTIPARTDATA") )
        nType = wxHTTPBuilder::wxHTTP_TYPE_MULTIPARTDATA;
      if( szType == wxT("COOKIE") )
        nType = wxHTTPBuilder::wxHTTP_TYPE_COOKIE;
      if( szType == wxT("COOKIERAW") )
        nType = wxHTTPBuilder::wxHTTP_TYPE_COOKIERAW;

      m_http->SetValue( szName, szValue, nType );
    }
  }



  //http->Authenticate(wxT("username"), "password"); // Auth Type basic

#ifdef USETHREAD

  m_thread = new wxHTTPBuilderThread(this, Minimal_Thread, m_http, szURL);
	m_thread->SaveToFile(true, szSaveAs);

  if( m_thread->Create() != wxTHREAD_NO_ERROR )
  {
		m_thread = NULL;
    delete m_http;
    m_http = NULL;
  }
  else
  {
    m_thread->Run();
  }
  SetStatusText(wxT("Downloading, please wait..."), 1);

	if( m_timer )
		delete m_timer;
  m_timer = new wxTimer(this, Minimal_Timer);
  m_timer->Start( 500, FALSE ); // Fire every 1/2 second
	
#ifdef USEPLEASEWAITDLG
	if(m_pleaseWaitDlg)
		delete m_pleaseWaitDlg; // Close the last dialog if it wasnt destoryed previously
	
	m_pleaseWaitDlg = new wxPleaseWaitDlg( _T("wxHTTPEngine"),
                            _T("Downloading, please wait..."),
                            this,   // parent
                            wxPW_APP_MODAL|
                            wxPW_CAN_CANCEL|
                            wxPW_SHOW_NOW);
#endif

#else
	// No threads, we need to tell the socket to not wait, but to block the GUI when working
	// we add the BLOCK because this event we perform the operation with the server will not 
	// not end until the thread is done.
	m_http->SetFlags(wxSOCKET_NONE|wxSOCKET_BLOCK);

  wxString szTemp = wxFileName::CreateTempFileName(wxT("wxHTTPEngine"));

  // Download results as a file:
  if( m_http->SaveFile( szTemp, szURL) )
  {
    if( wxFileExists(szTemp) )
    {
      if( wxFileExists(szSaveAs) )
        wxRemoveFile(szSaveAs);

      wxRenameFile(szTemp, szSaveAs);

			int read = m_http->GetBytesRead();
			int sent = m_http->GetBytesSent();
			
			// We are done!
			SetStatusText(wxT("Download complete"), 1);

			wxString msg;
			if( sent > 0 )
				msg = wxString::Format(wxT("Sent: %dK, Read: %dK"), sent/1024, read/1024);
			else
				msg = wxString::Format(wxT("Read: %dK"), read/1024);
			SetStatusText(msg, 1);
			SetStatusText(wxT("Request completed."), 0);
			//wxMessageBox( wxString::Format(wxT("Headers follow...\n\n%s"), m_http->GetRawHeaders().c_str() ) );

			if( !m_bSaveResults )
			{
				wxFile file;
				file.Open( m_szTempFileName, wxFile::read );
				int nBytesRead = 0;

				wxFileOffset len = file.Length();
				wxString szContents;
        file.Read(wxStringBuffer(szContents, len), len);
				file.Close();
				
				wxRemoveFile(m_szTempFileName);
				m_szTempFileName = "";
				
				wxMessageBox(szContents);
				wxRemoveFile(szSaveAs);
			}
    }
    else
    {
      wxMessageBox( wxString::Format(wxT("An error occurred saving file:\n%s"), m_http->GetLastError().c_str() ) );
    }
  }
  else
  {
    wxMessageBox( wxString::Format(wxT("An error occurred saving file:\n%s"), m_http->GetLastError().c_str() ) );
  }
#endif

  /*
  // Get Imput Stream:  Example not using a thread:
  in_stream = http->GetInputStream(wxT("/phpinfo.php?FileName=productsServices.pdf"));

  if( in_stream )
  {
    wxFile file;
    file.Open( szSaveAs, wxFile::write );

    int nBytesRead = 0;

    while( in_stream->Eof() == false )
    {
      wxChar buf[8192];
      in_stream->Read( buf, WXSIZEOF(buf) );

      file.Write( buf, in_stream->LastRead() );

      if( in_stream->LastRead() == 0 )
        break;

      nBytesRead += in_stream->LastRead();
    }
    file.Close();

    delete in_stream;
  }
  */
#ifndef USETHREAD
  delete m_http;
  m_http = NULL;
#endif
}

void wxHTTPEngineDialog::OnHeadGo( wxCommandEvent &event)
{
	if( m_http )
    return;

  wxString szURL = m_txtUrl->GetValue();
  if( szURL.IsEmpty() )
  {
    wxMessageBox( _T("You did not enter a URL.") );
    return;
  }

	m_http = new wxHTTPBuilder();
  m_http->InitContentTypes(); // Initialise the content types on the page

  if( m_bUseProxy )
  {
#ifdef TESTPROXYSETTINGSCLASS
    wxProxySettings set;
    m_proxySettings.SetUseProxy(true);
    m_http->SetProxySettings(m_proxySettings);
#else
    m_http->HttpProxy(m_szProxyHost, m_nProxyPort );
    if( m_bProxyAuth )
      m_http->HttpProxyAuth( m_szProxyUsername, m_szProxyPassword);
#endif
  }
  
  if( m_bUseAuth )
  {
#ifdef TESTHTTPAUTHSETTINGSCLASS
    m_authSettings.SetBasicAuth(); // Set the class to use the authentication settings
    m_http->SetAuthentication( m_authSettings );
#else
    m_http->Authenticate(m_szAuthUsername, m_szAuthPassword);
#endif
  }

  // Add some values:
  for( unsigned long x = 0; x < (unsigned long)m_lcData->GetItemCount(); x++ )
  {
    wxString szName = m_lcData->GetItemText(x);
    wxString szValue = wxT("");
    wxString szType = wxT("");
    wxListItem item;
    item.m_itemId = x;
    item.m_col = 1;
    item.m_mask = wxLIST_MASK_TEXT;
    if( m_lcData->GetItem( item ) )
      szValue = item.m_text;
    item.m_itemId = x;
    item.m_col = 2;
    item.m_mask = wxLIST_MASK_TEXT;
    if( m_lcData->GetItem( item ) )
    {
      szType = item.m_text;

      wxHTTPBuilder::wxHTTP_Type nType = wxHTTPBuilder::wxHTTP_TYPE_ANY;
			// Only Query string and cookie values can be sent in a HEAD
      if( szType == wxT("GET") )
        nType = wxHTTPBuilder::wxHTTP_TYPE_GET;
      if( szType == wxT("COOKIE") )
        nType = wxHTTPBuilder::wxHTTP_TYPE_COOKIE;
      if( szType == wxT("COOKIERAW") )
        nType = wxHTTPBuilder::wxHTTP_TYPE_COOKIERAW;

      m_http->SetValue( szName, szValue, nType );
    }
  }



  //http->Authenticate(wxT("username"), "password"); // Auth Type basic

#ifdef USETHREAD

  m_thread = new wxHTTPBuilderThread(this, Minimal_Thread2, m_http, szURL);
	m_thread->GetHeadRequest(true);

  if( m_thread->Create() != wxTHREAD_NO_ERROR )
  {
		m_thread = NULL;
    delete m_http;
    m_http = NULL;
  }
  else
  {
    m_thread->Run();
  }
  SetStatusText(wxT("Sending HEAD request, please wait..."), 1);

	if( m_timer )
		delete m_timer;
  m_timer = new wxTimer(this, Minimal_Timer);
  m_timer->Start( 500, FALSE ); // Fire every 1/2 second
	
	#ifdef USEPLEASEWAITDLG
		if(m_pleaseWaitDlg)
			delete m_pleaseWaitDlg; // Close the last dialog if it wasnt destoryed previously
		
		m_pleaseWaitDlg = new wxPleaseWaitDlg( _T("wxHTTPEngine"),
                            _T("Sending HEAD request, please wait..."),
                            this,   // parent
                            wxPW_APP_MODAL|
                            wxPW_CAN_CANCEL|
                            wxPW_SHOW_NOW);
	#endif

#else
	// No threads, we need to tell the socket to not wait
	m_http->SetFlags(wxSOCKET_NONE|wxSOCKET_BLOCK);

	int retCode = m_http->GetHeadResponse(szURL);
	if( retCode < 0 )
	{
		wxMessageBox( wxString::Format(wxT("Head request error: %s\nProtocol error code: %d"), m_http->GetLastError().c_str(), m_http->GetError() ) );
	}
	else
	{
		//int read = m_http->GetBytesRead();
		int sent = m_http->GetBytesSent();
		int contentLength = m_http->GetContentLength();
		// We are done!
		SetStatusText(wxT("Download complete"), 1);

		wxString msg;
		if( sent > 0 )
			msg = wxString::Format(wxT("Sent: %dK, Returned Content-Length: %dK"), sent/1024, contentLength/1024);
		else
			msg = wxString::Format(wxT("Returned Content-Length: %dK"), contentLength/1024);
		SetStatusText(msg, 1);
		SetStatusText(wxT("Request completed."), 0);

		wxMessageBox( wxString::Format(wxT("Head request response code: %d\nHeaders follow...\n\n%s"), retCode, m_http->GetRawHeaders().c_str() ) );
	}

#endif

  /*
  // Get Imput Stream:  Example not using a thread:
  in_stream = http->GetInputStream(wxT("/phpinfo.php?FileName=productsServices.pdf"));

  if( in_stream )
  {
    wxFile file;
    file.Open( szSaveAs, wxFile::write );

    int nBytesRead = 0;

    while( in_stream->Eof() == false )
    {
      wxChar buf[8192];
      in_stream->Read( buf, WXSIZEOF(buf) );

      file.Write( buf, in_stream->LastRead() );

      if( in_stream->LastRead() == 0 )
        break;

      nBytesRead += in_stream->LastRead();
    }
    file.Close();

    delete in_stream;
  }
  */
#ifndef USETHREAD
  delete m_http;
  m_http = NULL;
#endif
}

void wxHTTPEngineDialog::OnAddField(wxCommandEvent &)
{
  wxArrayString szaOptions;
  szaOptions.Add(wxT("Default"));
  szaOptions.Add(wxT("GET"));
  szaOptions.Add(wxT("POST"));
  szaOptions.Add(wxT("FILE"));
  szaOptions.Add(wxT("MULTIPARTDATA"));
  szaOptions.Add(wxT("COOKIE"));
  szaOptions.Add(wxT("COOKIERAW"));
  wxString szType = wxGetSingleChoice( _T("Select field type:"),_T("wxHTTP Engine"),szaOptions, this );
  if( szType.IsNull() )
    return;

  wxHTTPBuilder::wxHTTP_Type type = wxHTTPBuilder::wxHTTP_TYPE_ANY;
  if( szType == wxT("GET") )
    type = wxHTTPBuilder::wxHTTP_TYPE_GET;
  if( szType == wxT("POST") )
    type = wxHTTPBuilder::wxHTTP_TYPE_POST;
  if( szType == wxT("FILE") )
    type = wxHTTPBuilder::wxHTTP_TYPE_FILE;
  if( szType == wxT("MULTIPARTDATA") )
    type = wxHTTPBuilder::wxHTTP_TYPE_MULTIPARTDATA;
  if( szType == wxT("COOKIE") )
    type = wxHTTPBuilder::wxHTTP_TYPE_COOKIE;
  if( szType == wxT("COOKIERAW") )
    type = wxHTTPBuilder::wxHTTP_TYPE_COOKIERAW;

  wxString szName = wxGetTextFromUser(_T("Enter name of field:"), _T("wxHTTP Engine"), _T(""), this );
  if( szName.IsNull() )
    return;

  wxString szValue = wxT("");

  // Get text from user (check):

  if( type == wxHTTPBuilder::wxHTTP_TYPE_FILE )
  {
    // Get file path from user (check):
    szValue = wxFileSelector( _T("Select file:"), _T(""), _T(""), _T(""), wxT("*.*"), wxOPEN|wxFILE_MUST_EXIST, this);

    if( szValue.IsNull() )
      return;
  }
  else if( type == wxHTTPBuilder::wxHTTP_TYPE_MULTIPARTDATA )
  {
		
    wxGetInputFromUser dlg(this, wxT("Enter Multipart Data"), wxPoint(-1, -1), wxSize(400, 300) );
    if( dlg.ShowModal() == wxID_OK )
      szValue = dlg.GetValue();
    else
      return;
    //szValue = wxGetTextFromUser(_T("Enter value of field:"), _T("wxHTTP Engine"), _T(""), this, -1, -1, true );
  }
  else
  {
    // Get text from user:
    szValue = wxGetTextFromUser(_T("Enter value of field:"), _T("wxHTTP Engine"), _T(""), this );
  }

  //if( m_http == NULL )
  //  m_http = new wxHTTPBuilder();

  //m_http->SetValue( szName, szValue, type );

  // If we get this far, add the sucker to the socket object...

  // Add this name, value pair to to the listctrl

  int i = 0;
  long tmp = m_lcData->InsertItem(i, szName, 0);
  m_lcData->SetItemData(tmp, i);
  m_lcData->SetItem(i, 1, szValue);
  m_lcData->SetItem(i, 2, szType);

}

void wxHTTPEngineDialog::OnUpdate(wxUpdateUIEvent &event)
{
  switch( event.GetId() )
  {
    case  Minimal_Stop:   
      event.Enable( (m_thread != NULL) );
      break;
    case  Minimal_Go:   
      event.Enable( (m_thread == NULL) );
      break;
    case  Minimal_AddField:   
      event.Enable( (m_thread == NULL) );
      break;
    case  Minimal_ProxySettings:   
      event.Enable( (m_thread == NULL) );
      break;
    case  Minimal_Authentication:   
      event.Enable( (m_thread == NULL) );
      break;
    case  Minimal_UseProxySettings:   
      event.Enable( (m_thread == NULL) );
      break;
    case  Minimal_UseAuthentication:   
      event.Enable( (m_thread == NULL) );
      break;
		case Minimal_SaveResults:
			event.Enable( (m_thread == NULL) );
      break;
  }
}

void wxHTTPEngineDialog::OnDownloadComplete(wxHTTPBuilderEvent &)
{
  m_thread = NULL;

  wxMutexLocker lock( m_mutexHTTPObj);

  int read = m_http->GetBytesRead();
  int sent = m_http->GetBytesSent();
	int response = m_http->GetResponse();
	wxString szResponseStr = m_http->GetResponseString();
	
	if( m_timer )
  {
    m_timer->Stop();
    delete m_timer;
    m_timer = NULL;
  }
	
	if( m_pleaseWaitDlg )
	{
		delete m_pleaseWaitDlg;
		m_pleaseWaitDlg = NULL;
	}

  if( m_http )
    delete m_http;
  m_http = NULL;

  // We are done!
  SetStatusText(wxT("Download complete"), 1);

  wxString msg;
  
  if( sent > 0 )
    msg = wxString::Format(wxT("Sent: %dK, Read: %dK"), sent/1024, read/1024);
  else
    msg = wxString::Format(wxT("Read: %dK"), read/1024);
  SetStatusText(msg, 1);
  SetStatusText(_T("Request completed."), 0);

	if( !m_bSaveResults && wxFileName::FileExists(m_szTempFileName) )
	{
		// Read the file, delete the file and then display results in dlg
		wxFile file;
    file.Open( m_szTempFileName, wxFile::read );
		
		wxFileOffset len = file.Length();
		wxString szContents;
    file.Read(wxStringBuffer(szContents, len), len);
		file.Close();

		wxRemoveFile(m_szTempFileName);
		m_szTempFileName = wxT("");

		// Display dialog here:
		//wxGetInputFromUser dlg(this, wxT("Results"), wxPoint(-1, -1), wxSize(400, 300) );
		//dlg.SetValue(szContents);
		//dlg.ShowModal();
		wxMessageBox(szContents);
	}
	else if( response != 200 ) // something happeend
	{
		wxMessageBox(szResponseStr);
	}
}

void wxHTTPEngineDialog::OnDownloadComplete2(wxHTTPBuilderEvent &)
{
  m_thread = NULL;

	wxString szHeaders = m_http->GetRawHeaders();
	

  wxMutexLocker lock( m_mutexHTTPObj);

  int read = m_http->GetBytesRead();
  int sent = m_http->GetBytesSent();
	
	if( m_timer )
  {
    m_timer->Stop();
    delete m_timer;
    m_timer = NULL;
  }
	
	if( m_pleaseWaitDlg )
	{
		delete m_pleaseWaitDlg;
		m_pleaseWaitDlg = NULL;
	}

	

  if( m_http )
    delete m_http;
  m_http = NULL;

  // We are done!
  SetStatusText(wxT("Download complete"), 1);

  wxString msg;
  
  if( sent > 0 )
    msg = wxString::Format(wxT("Sent: %dK, Read: %dK"), sent/1024, read/1024);
  else
    msg = wxString::Format(wxT("Read: %dK"), read/1024);
  SetStatusText(msg, 1);
  SetStatusText(_T("Request completed."), 0);

	wxMessageBox( wxString::Format(wxT("Headers follow...\n\n%s"), szHeaders.c_str() ) );
	// Display dialog here:
		//wxGetInputFromUser dlg(this, wxT("Results"), wxPoint(-1, -1), wxSize(400, 300) );
		//dlg.SetValue(szContents);
		//dlg.ShowModal();
		//wxMessageBox(szContents);
}

void wxHTTPEngineDialog::OnUseProxy(wxCommandEvent &event)
{
  m_bUseProxy = event.IsChecked();
}

void wxHTTPEngineDialog::OnUseAuth(wxCommandEvent &event)
{
  m_bUseAuth = event.IsChecked();
}

void wxHTTPEngineDialog::OnSaveResults(wxCommandEvent &event)
{
	m_bSaveResults = event.IsChecked();
}

void wxHTTPEngineDialog::OnTimer(wxTimerEvent &)
{
  //bool bStopThread = false;

  // Update the bytes sent and received in the status bar...
  if( m_http )
  {
    wxMutexLocker lock(m_mutexHTTPObj);

    int read = m_http->GetBytesRead();
    int sent = m_http->GetBytesSent();
    wxString msg = wxString::Format(wxT("Sent: %dK, Read: %dK"), sent/1024, read/1024);
    SetStatusText(msg, 1);
		
		if( m_pleaseWaitDlg )
		{
			if( !m_pleaseWaitDlg->Update() ) // They clicked cancel, lets stop what we are doing...
			{
        int ret = wxYES;
#ifdef TESTRESUMECANCEL
        m_timer->Stop();
        ret = wxMessageBox(_T("Do you wish to cancel the download?"), _T("wxHTTPEngine"), wxYES_NO|wxICON_QUESTION, this );
#endif

        if( ret == wxNO )
        {
          m_timer->Start( 500, FALSE );
          m_pleaseWaitDlg->Resume();
        }
        else
        {
				  delete m_pleaseWaitDlg;
				  m_pleaseWaitDlg = NULL;

          // Tell the downloader thread to stop:
          m_http->Stop(true); // Lets the thread know in a graceful way to stop
          if( m_timer )
          {
            if( m_timer->IsRunning() )
              m_timer->Stop();
            delete m_timer;
            m_timer = NULL;
          }
        }
			}
		}
  }
  else if( m_timer )
  {
    // end this timer
    m_timer->Stop();
    delete m_timer;
    m_timer = NULL;
  }
}

enum {
    About_Link1 = 1,
    About_Link2,
    About_Link3,
    About_Link4,

    About_Close,
};


////////////////////////////////////////////
// About Dialog
////////////////////////////////////////////
BEGIN_EVENT_TABLE(AboutDialog, wxDialog)
#ifndef HYPERLINKCTRL_COMMANDEVENT
    EVT_LINK_MCLICKED( About_Link2, AboutDialog::OnHyperlink )
    EVT_LINK_RCLICKED( About_Link3, AboutDialog::OnHyperlink )
#endif
    EVT_LINK( About_Link4, AboutDialog::OnHyperlink )

    EVT_MENU(About_Close, AboutDialog::OnMenu)
END_EVENT_TABLE()



AboutDialog::AboutDialog
 ( wxWindow *parent, wxWindowID id,
   const wxString &title,
   const wxPoint &position, const wxSize& size,
   long style
 ) :
 wxDialog( parent, id, title, position, size, style)
{
	wxButton *b = new wxButton( this, wxID_OK, wxT("OK"), wxPoint(258,8), wxDefaultSize );
	b->SetFocus();

	wxStaticText *txtTitle = new wxStaticText( this, -1, wxT("wxHTTPEngine Examples"), wxPoint(12,10), wxSize( 170, 24));

	wxFont fntTemp = txtTitle->GetFont();
	fntTemp.SetWeight( wxBOLD );
	fntTemp.SetPointSize( fntTemp.GetPointSize() + 2);
	txtTitle->SetFont( fntTemp );

  new wxStaticText( this, -1, wxT("wxHTTPBuilder and wxHyperlinkCtrl examples"), wxPoint(50,40) );
	new wxStaticText( this, -1, wxT("Copyright (C) 2002-2005 Angelo Mandato"), wxPoint(50,60) );

  // Default hyperlink
	new wxHyperlinkCtrl( this, About_Link1, wxT("wxHTTPEngine at wxCode.sf.net"),
		 	wxPoint(50,80), wxDefaultSize, 0, wxT("statictextlink2"), wxT("http://wxcode.sourceforge.net/components/wxhttpengine/") );

  // Web link with underline rollovers, opens in new window
	//wxHyperlinkCtrl *hyper2 = new wxHyperlinkCtrl( this, About_Link2, wxT("www.spaceblue.com"),
	//	 	wxPoint(50,100), wxDefaultSize, wxTAB_TRAVERSAL , wxT("statictextlink2"), wxT("http://www.spaceblue.com") );

  wxHyperlinkCtrl *hyper2 = new wxHyperlinkCtrl( this, About_Link2, wxT("www.leakybagel.com"), 
      wxPoint(50,100), wxDefaultSize, wxTAB_TRAVERSAL , wxT("statictextlink1"), wxT("http://www.leakybagel.com") );

	hyper2->SetColours( wxColour(wxT("BLUE")), wxColour(wxT("BLUE")), wxColour(wxT("BLUE")) );
	hyper2->EnableRollover(true);
	hyper2->SetUnderlines(false, false, true);
	hyper2->SetBold(true);
  hyper2->OpenInSameWindow( true ); // middle click to open in new window
  hyper2->SetToolTip( wxT("Middle click to open in new browser window"));
	hyper2->UpdateLink();
  //hyper2->SetWindowStyle( hyper2->GetWindowStyle() | wxWANTS_CHARS );

	// Intense link examples..
	wxHyperlinkCtrl *hyper3 = new wxHyperlinkCtrl( this,About_Link3, wxT("wxWindows online search"),
	  wxPoint(50,120), wxDefaultSize, 0, wxT("statictextlink2"), wxT("http://www.wxwindows.org/search.htm") );

	hyper3->SetToolTip( wxT("Right click for custom pop up menu") );
	hyper3->SetLinkCursor( wxCURSOR_QUESTION_ARROW );
	hyper3->SetColours( wxColour(wxT("GREEN")), wxColour(wxT("RED")), wxColour(wxT("YELLOW")) );
	hyper3->SetUnderlines(false, false, false);
	//hyper3->SetBrowserPath("C:\\Program Files\\Internet Explorer\\IEXPLORE.EXE" );
  //hyper3->SetBrowserPath("C:\\Program Files\\Netscape\\Netscape 6\\netscp6.exe" );
	hyper3->SetBrowserPath(wxT("c:\\program files\\mozilla firefox\\firefox.exe") );
	hyper3->EnableRollover(true);
	hyper3->SetBold(true);
	hyper3->DoPopup(false);

	hyper3->UpdateLink();


	wxHyperlinkCtrl *hyper4 = new wxHyperlinkCtrl( this, About_Link4, wxT("Open Google in current browser window?"),
    wxPoint(50,140), wxDefaultSize, 0, wxT("statictextlink2"), wxT("") );

	hyper4->SetToolTip( wxT("Click link for Yes, No, Cancel Dialog") );
	hyper4->AutoBrowse(false);
	hyper4->UpdateLink();
  // Test some other links...
}
/*
 wxDialog( parent, id, title, position, size, style)
{
	wxButton *b = new wxButton( this, wxID_OK, wxT("OK"), wxPoint(258,8), wxDefaultSize );
	b->SetFocus();

	wxStaticText *txtTitle = new wxStaticText( this, -1, wxT("wxHTTPEngine Examples"), wxPoint(12,10), wxSize( 170, 24)); 

	wxFont fntTemp = txtTitle->GetFont();
	fntTemp.SetWeight( wxBOLD );
	fntTemp.SetPointSize( fntTemp.GetPointSize() + 2);
	txtTitle->SetFont( fntTemp );

  new wxStaticText( this, -1, wxT("wxHTTPBuilder and wxHyperlinkCtrl examples"), wxPoint(50,40) ); 
	new wxStaticText( this, -1, wxT("Copyright (C) 2002-2005 Angelo Mandato"), wxPoint(50,60) ); 
  
  // Default hyperlink
	new wxHyperlinkCtrl( this, About_Link1, wxT("http://www.spaceblue.com/code.php"),
		 	wxPoint(50,80), wxDefaultSize, 0, wxT("statictextlink2"), wxT("http://www.spaceblue.com/code.php") );

  // Web link with underline rollovers, opens in new window
	wxHyperlinkCtrl *hyper2 = new wxHyperlinkCtrl( this, About_Link2, wxT("www.wxwindows.org"),
		 	wxPoint(50,100), wxDefaultSize, wxTAB_TRAVERSAL , wxT("statictextlink2"), wxT("http://www.wxwindows.org") );

	hyper2->SetColours( wxColour(wxT("BLUE")), wxColour(wxT("BLUE")), wxColour(wxT("BLUE")) );
	hyper2->EnableRollover(true);
	hyper2->SetUnderlines(false, false, true);
	hyper2->SetBold(true);
  hyper2->OpenInSameWindow( true ); // middle click to open in new window
  hyper2->SetToolTip( wxT("Middle click to open in new browser window"));
	hyper2->UpdateLink();
  //hyper2->SetWindowStyle( hyper2->GetWindowStyle() | wxWANTS_CHARS );

	// Intense link examples..
	wxHyperlinkCtrl *hyper3 = new wxHyperlinkCtrl( this,About_Link3, wxT("wxWindows online search"),
	  wxPoint(50,120), wxDefaultSize, 0, wxT("statictextlink2"), wxT("http://www.wxwindows.org/search.htm") );

	hyper3->SetLinkCursor( wxCURSOR_QUESTION_ARROW );
	hyper3->SetColours( wxColour(wxT("GREEN")), wxColour(wxT("RED")), wxColour(wxT("YELLOW")) );
	hyper3->SetUnderlines(false, false, false);
	//hyper3->SetBrowserPath("C:\\Program Files\\Internet Explorer\\IEXPLORE.EXE" );
  //hyper3->SetBrowserPath("C:\\Program Files\\Netscape\\Netscape 6\\netscp6.exe" );
	hyper3->SetBrowserPath(wxT("c:\\program files\\mozilla firefox\\firefox.exe") );
	hyper3->EnableRollover(true);
	hyper3->SetBold(true);
	hyper3->DoPopup(false);

	hyper3->UpdateLink();


	wxHyperlinkCtrl *hyper4 = new wxHyperlinkCtrl( this, About_Link4, wxT("Open Google in current browser window?"),
    wxPoint(50,140), wxDefaultSize, 0, wxT("statictextlink2"), wxT("") );

	hyper4->SetToolTip( wxT("Click link for Yes, No, Cancel Dialog") );	
	hyper4->AutoBrowse(false);
	hyper4->UpdateLink();
  // Test some other links...
}
*/
#ifdef HYPERLINKCTRL_COMMANDEVENT

void AboutDialog::OnHyperlink(wxCommandEvent &event)
{
  switch( event.GetId() )
  {
  case About_Link4: {
      // Custom link for your own use:
      int nResult = wxMessageBox( _T("Open Google in current browser window?"), _T("wxHyperlinksCtrl"), wxYES_NO|wxCANCEL|wxICON_QUESTION, this );

      if( nResult == wxYES )
        wxHyperlinkCtrl::GotoURL(wxT("http://www.google.com"), "", false, true );
      else if( nResult == wxNO )
        wxHyperlinkCtrl::GotoURL(wxT("http://www.google.com"), "", false, false );
    }; break;
  }
}

#else

void AboutDialog::OnHyperlink(wxHyperlinkEvent &event)
{
  switch( event.GetId() )
  {
  case About_Link2: {
      // Your own custom middle click event: (open link in new window like firefox middle click)
      wxHyperlinkCtrl::GotoURL(wxT("http://www.wxwindows.org"), wxT(""), false, false );
    }; break;
  case About_Link3: {
      // Custom right click menu (or what ever you want)
      wxPoint pos = event.GetPosition();
      wxMenu *menuPopUp = new wxMenu(wxT(""), wxMENU_TEAROFF);
	    menuPopUp->Append(About_Close, wxT("Close Dialog"));
	    PopupMenu( menuPopUp, pos );
	    delete menuPopUp; 
    }; break;
  case About_Link4: {
      // Custom link for your own use:
      int nResult = wxMessageBox( _T("Open Google in current browser window?"), _T("wxHyperlinksCtrl"), wxYES_NO|wxCANCEL|wxICON_QUESTION, this );

      if( nResult == wxYES )
        wxHyperlinkCtrl::GotoURL(wxT("http://www.google.com"), wxT(""), false, true );
      else if( nResult == wxNO )
        wxHyperlinkCtrl::GotoURL(wxT("http://www.google.com"), wxT(""), false, false );
    }; break;
  }
}
#endif

void AboutDialog::OnMenu(wxCommandEvent &event)
{
  switch( event.GetId() )
  {
  case About_Close: {
    Destroy();
    }; break;

  }
}

// eof




