/////////////////////////////////////////////////////////////////////////////
// Name:        simple.cpp
// Purpose:     wxTTS sample for wxWindows
// Author:      Julian Smart
// Modified by:
// Created:     12/5/2000
// RCS-ID:      $Id: simple.cpp,v 1.1.1.1 2003-12-28 21:10:20 wyo Exp $
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "simple.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/statline.h"

#include "simple.h"

IMPLEMENT_APP(MyApp)

MyApp::MyApp(void)
{
}

bool MyApp::OnInit(void)
{
    SimpleDialog dialog(NULL, SIMPLE_DIALOG, wxT("wxTTS Demo"), wxDefaultPosition,
        wxSize(400, 400), wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxDIALOG_MODAL);

    dialog.ShowModal();

    return FALSE;
}

BEGIN_EVENT_TABLE(SimpleDialog, wxDialog)
    EVT_CLOSE(SimpleDialog::OnCloseWindow)
    EVT_BUTTON(SIMPLE_SPEAK, SimpleDialog::OnSpeak)
    EVT_BUTTON(wxID_OK, SimpleDialog::OnOK)
    EVT_BUTTON(SIMPLE_SPEAK_TO_FILE, SimpleDialog::OnSpeakToFile)
    EVT_BUTTON(SIMPLE_SPEAK_FROM_FILE, SimpleDialog::OnSpeakFromFile)
    EVT_BUTTON(SIMPLE_STOP, SimpleDialog::OnStop)
END_EVENT_TABLE()

SimpleDialog::SimpleDialog(wxWindow *parent, wxWindowID id, const wxString& title,
    const wxPoint& pos, const wxSize& size,
    long style, const wxString& name):
      wxDialog(parent, id, title, pos, size, style, name),
      m_freephone("..\\..\\..\\dictionary\\english")
{
	if (!m_mbrola.Init())
		wxMessageBox("There was a problem initialising Mbrola.", "wxTTS Sample", wxOK|wxICON_EXCLAMATION);
    else
	    m_mbrola.SetCurrentDatabase("en1");

	m_freephone.Init();

    m_textCtrl = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(300, 200), wxTE_MULTILINE);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *buttonSizer1 = new wxBoxSizer( wxHORIZONTAL );
    wxBoxSizer *buttonSizer2 = new wxBoxSizer( wxHORIZONTAL );

    wxButton* speakButton;

    buttonSizer1->Add(
        (speakButton = new wxButton( this, SIMPLE_SPEAK, "Speak" )),
        0,           // make horizontally unstretchable
        wxALL,       // make border all around (implicit top alignment)
        8 );        // set border width
    buttonSizer1->Add(
        new wxButton( this, SIMPLE_SPEAK_FROM_FILE, "Speak from file" ),
        0,           // make horizontally unstretchable
        wxALL,       // make border all around (implicit top alignment)
        8 );        // set border width
    buttonSizer1->Add(
        new wxButton( this, SIMPLE_SPEAK_TO_FILE, "Speak to file" ),
        0,           // make horizontally unstretchable
        wxALL,       // make border all around (implicit top alignment)
        8 );        // set border width

    buttonSizer2->Add(
        new wxButton( this, SIMPLE_STOP, "Stop" ),
        0,           // make horizontally unstretchable
        wxALL,       // make border all around (implicit top alignment)
        8 );        // set border width
    buttonSizer2->Add(
        new wxButton( this, wxID_OK, "Exit" ),
        0,           // make horizontally unstretchable
        wxALL,       // make border all around (implicit top alignment)
        8 );        // set border width

    topSizer->Add(
        m_textCtrl,
        1,            // make vertically stretchable
        wxEXPAND |    // make horizontally stretchable
        wxALL,        //   and make border all around
        8 );         // set border width

    topSizer->Add(
            new wxStaticLine(this, -1),
            0, wxGROW | wxLEFT | wxRIGHT, 10);
    
    topSizer->Add(
        buttonSizer1,
        0,                // make vertically unstretchable
        wxALIGN_CENTER ); // no border and centre horizontally

    topSizer->Add(
        buttonSizer2,
        0,                // make vertically unstretchable
        wxALIGN_CENTER ); // no border and centre horizontally

    speakButton->SetDefault();
    m_textCtrl->SetFocus();
    
    SetAutoLayout( TRUE );     // tell dialog to use sizer
    SetSizer( topSizer );      // actually set the sizer

    topSizer->Fit( this );            // set size to minimum size as calculated by the sizer
    Centre(wxCENTRE_ON_SCREEN|wxBOTH);
}

void SimpleDialog::OnCloseWindow(wxCloseEvent& event)
{
    EndModal(wxID_OK);
}

void SimpleDialog::OnSpeak(wxCommandEvent& event)
{
    wxString val = m_textCtrl->GetValue();

    wxString phonemes;
    if (m_freephone.TextToPhoneme(val, phonemes))
    {
        int errVal = m_mbrola.Play(phonemes, TRUE);
        if (errVal != wxMBRERR_NOERROR && errVal != wxMBRERR_CANCELLEDBYUSER)
        {
            wxMessageBox("There was a problem in wxMbrola::Play.", "wxTTS Sample", wxOK|wxICON_EXCLAMATION);
        }
    }
}

void SimpleDialog::OnStop(wxCommandEvent& event)
{
    m_mbrola.Stop();
}

void SimpleDialog::OnSpeakFromFile(wxCommandEvent& event)
{
    wxFileDialog dialog(this, wxT("Choose a phoneme file"), wxGetCwd(), wxEmptyString,
        wxT("Phoneme files (*.pho)|*.pho|All files (*.*)|*.*"), wxOPEN);

    if (dialog.ShowModal() == wxID_OK)
    {
        wxString filename(dialog.GetPath());

        int errVal = m_mbrola.PlayFromFile(filename, TRUE);

        if (errVal != wxMBRERR_NOERROR && errVal != wxMBRERR_CANCELLEDBYUSER)
        {
            wxMessageBox("There was a problem in wxMbrola::PlayFromFile.", "wxTTS Sample", wxOK|wxICON_EXCLAMATION);
        }
    }
}

void SimpleDialog::OnSpeakToFile(wxCommandEvent& event)
{
    wxString val = m_textCtrl->GetValue();

    if (val.IsEmpty())
        return;

    wxFileDialog dialog(this, wxT("Choose a phoneme file"), wxGetCwd(), wxEmptyString,
        wxT("Phoneme files (*.pho)|*.pho|All files (*.*)|*.*"), wxSAVE);

    if (dialog.ShowModal() == wxID_OK)
    {
        wxString filename(dialog.GetPath());

        if (!m_freephone.TextToPhonemeFile(val, filename))
        {
            wxMessageBox("There was a problem in wxFreePhone::TextToPhonemeFile.", "wxTTS Sample", wxOK|wxICON_EXCLAMATION);
            return;
        }
    }
}

void SimpleDialog::OnOK(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}

