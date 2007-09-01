/////////////////////////////////////////////////////////////////////////////
// Name:        main.h
// Purpose:     awxtest
// Author:      Joachim Buermann
// Id:          $Id$
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#if !defined ( __MAIN_H )
#define __MAIN_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/awx-0.3/toolbar.h>
#include "prefs.h"

////////////////////////////////////////////////////////////
// Application
class MyApp : public wxApp
{
protected:
public:
    virtual bool OnInit();
};

////////////////////////////////////////////////////////////
// Application Frame
class MyFrame : public wxFrame
{
protected:
    awxToolbar* m_toolbar;
    wxTextCtrl* m_log;
    prefs_t m_prefs;
    int CreateToolbar(awxToolbar* mtb,bool showText = true);
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    ~MyFrame();
    void OnChoice(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnSettings(wxCommandEvent& event);    
    void OnToolbarText(wxCommandEvent& event);
    void OnToggle(wxCommandEvent& event);
    void OnViewIcon(wxCommandEvent& event);
    void OnViewTree(wxCommandEvent& event);
protected:
    DECLARE_EVENT_TABLE()
};

#endif
