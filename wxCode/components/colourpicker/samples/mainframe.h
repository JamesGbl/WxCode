/////////////////////////////////////////////////////////////////////////////
// Name:        mainframe.h
// Purpose:     wxColourPicker sample frame header
// Author:      Juan Antonio Ortega
// Created:     2006/06/17
// RCS-ID:      $Id: mainframe.h,v 1.1 2006-06-27 18:10:54 ja_ortega Exp $
// Copyright:   (c) 2006 Juan Antonio Ortega
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wxprec.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/colourpicker.h>

class MainFrame : public wxFrame {
    wxColourPicker *m_cp;
    wxPanel *m_colour;
    enum{
        PICKER = 1000,
        CHECKBOX
    };
public:
    MainFrame(wxWindow *parent);

    //Events
    void OnCheckBoxChanged(wxCommandEvent& e);
    void OnColourChanged(wxColourPickerEvent& e);

    DECLARE_EVENT_TABLE()
};

#endif

