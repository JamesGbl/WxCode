/////////////////////////////////////////////////////////////////////////////
// Name:        simple.h
// Purpose:     wxTTS sample for wxWindows
// Author:      Julian Smart
// Modified by:
// Created:     12/5/2000
// RCS-ID:      $Id: simple.h,v 1.1.1.1 2003-12-28 21:10:20 wyo Exp $
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma interface "simple.h"
#endif

#ifndef _SIMPLE_H_
#define _SIMPLE_H_

#include "tts/wxmbrola.h"
#include "tts/freephone.h"

// Define a new application
class MyApp: public wxApp
{
public:
    MyApp(void);
    bool OnInit(void);

};

DECLARE_APP(MyApp)

class SimpleDialog: public wxDialog
{
public:
    SimpleDialog(wxWindow *parent, wxWindowID id, const wxString& title,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, const wxString& name = "dialogBox");

    void OnCloseWindow(wxCloseEvent& event);
    void OnSpeak(wxCommandEvent& event);
    void OnStop(wxCommandEvent& event);
    void OnSpeakFromFile(wxCommandEvent& event);
    void OnSpeakToFile(wxCommandEvent& event);
    void OnOK(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()

protected:
    wxMbrola        m_mbrola;
    wxFreePhone     m_freephone;
    wxTextCtrl*     m_textCtrl;
};

#define SIMPLE_DIALOG                   500
#define SIMPLE_SPEAK                    501
#define SIMPLE_SPEAK_FROM_FILE          502
#define SIMPLE_SPEAK_TO_FILE            503
#define SIMPLE_STOP                     504

#endif

