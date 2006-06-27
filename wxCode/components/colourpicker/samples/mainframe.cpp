/////////////////////////////////////////////////////////////////////////////
// Name:        mainframe.cpp
// Purpose:     wxColourPicker sample frame
// Author:      Juan Antonio Ortega
// Created:     2006/06/17
// RCS-ID:      $Id: mainframe.cpp,v 1.1 2006-06-27 18:10:54 ja_ortega Exp $
// Copyright:   (c) 2006 Juan Antonio Ortega
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "mainframe.h"
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/config.h>
#include <wx/colourpicker.h>

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_COLOUR_CHANGED(PICKER, MainFrame::OnColourChanged)
    EVT_CHECKBOX(CHECKBOX, MainFrame::OnCheckBoxChanged)
END_EVENT_TABLE()

MainFrame::MainFrame(wxWindow *parent) : wxFrame(parent, -1, _("ColourPicker Demo"), wxDefaultPosition, wxSize(195, 185), wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER & ~wxMAXIMIZE_BOX)
{
    wxPanel *panel = new wxPanel(this);
    m_cp = new wxColourPicker(panel, PICKER);
    m_cp->SetSize(20, 20, 70, 24);

    m_colour = new wxPanel(panel, -1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);
    m_colour->SetSize(20, 60, 145, 70);
    m_colour->SetBackgroundColour(m_cp->GetColour());

    wxCheckBox *cb = new wxCheckBox(panel, CHECKBOX, _("Enabled"));
    cb->SetValue(true);
    cb->SetSize(110, 20, 70, 23);
}

void MainFrame::OnCheckBoxChanged(wxCommandEvent& e){
    m_cp->Enable(e.IsChecked());
}

void MainFrame::OnColourChanged(wxColourPickerEvent& e){
    wxColour colour = e.GetColour();
    m_colour->SetBackgroundColour(colour);
    m_colour->Refresh();
}
