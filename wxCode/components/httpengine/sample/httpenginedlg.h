/////////////////////////////////////////////////////////////////////////////
// Name:        httpenginedlg.h
// Purpose:     wxHTTPEngine example
// Author:      Angelo Mandato
// Created:     2005/08/10
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Angelo Mandato (http://www.spaceblue.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// optimization for GCC
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "httpenginedlg.h"
#endif

#ifndef wxHTTPEngineDialog_H
#define wxHTTPEngineDialog_H

#include <wx/socket.h>
#include <wx/url.h>
#include <wx/wfstream.h>
#include <wx/textctrl.h>
#include <wx/listctrl.h>

#include <wx/httpbuilder.h>
#include <wx/httpbuilderthread.h>
#include <wx/hyperlinkctrl.h>
#include <wx/pleasewaitdlg.h>

// Define a new Dialog type: this is going to be our main Dialog, well Frame!
class wxHTTPEngineDialog : public wxFrame
{
public:
	
	void OnTimer( wxTimerEvent &event );
	void OnUseAuth(wxCommandEvent &event );
	void OnUseProxy(wxCommandEvent &event);
	void OnDownloadComplete( wxHTTPBuilderEvent &event );
	void OnUpdate( wxUpdateUIEvent &event);
	void OnAddField( wxCommandEvent &event );
	void OnGo( wxCommandEvent &event );
	void OnClose( wxCloseEvent &event );
	void OnAbout( wxCommandEvent &event );
	void OnQuit( wxCommandEvent &event );
  void OnProxySettings( wxCommandEvent &event );
  void OnAuthentication( wxCommandEvent &event );
  
    // ctor(s)
    wxHTTPEngineDialog(const wxString& title, const wxPoint& pos, const wxSize& size);
	virtual ~wxHTTPEngineDialog();

    // event handlers (these functions should _not_ be virtual)

private:
    wxTextCtrl      *m_txtUrl;
    wxListCtrl      *m_lcData;

    wxHTTPBuilder   *m_http;
    wxHTTPBuilderThread *m_thread;

    // Proxy settings:
    bool      m_bUseProxy;
    wxString  m_szProxyHost;
    long      m_nProxyPort;
    bool      m_bProxyAuth;
    wxString  m_szProxyUsername;
    wxString  m_szProxyPassword;
    wxString  m_szProxyExceptions;

    wxProxySettings m_proxySettings;
      

    bool      m_bUseAuth;
    wxString  m_szAuthUsername;
    wxString  m_szAuthPassword;
    bool      m_bRememberPasswd;

    wxHTTPAuthSettings m_authSettings;

    wxTimer   *m_timer;

    wxMutex   m_mutexHTTPObj;
		
		wxPleaseWaitDlg		*m_pleaseWaitDlg;

    DECLARE_CLASS(wxHTTPEngineDialog)
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};


class wxGetInputFromUser : public wxDialog
{
public:
  wxGetInputFromUser(wxWindow *parent, const wxString& title = wxT("Title here"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	virtual ~wxGetInputFromUser();

  wxString GetValue(void) { return m_ctrl->GetValue(); };
    

private:
  wxTextCtrl  *m_ctrl;
  wxString    m_szText;

};


class AboutDialog : public wxDialog  
{
	public:
		void OnMenu( wxCommandEvent &event );
		AboutDialog
		( wxWindow *parent,
			wxWindowID id,
			const wxString &title,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxDEFAULT_DIALOG_STYLE
			);
#ifdef HYPERLINKCTRL_COMMANDEVENT
    void OnHyperlink( wxCommandEvent &event );
#else
    void OnHyperlink( wxHyperlinkEvent &event );
#endif

  DECLARE_EVENT_TABLE()
};

#endif // wxHTTPEngineDialog_H
