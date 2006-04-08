/////////////////////////////////////////////////////////////////////////////
// Name:        DlgLang.h
// Purpose:     
// Author:      Ernesto Rangel Dallet
// Modified by: 
// Created:     12/04/05 20:11:36
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _DLGLANG_H_
#define _DLGLANG_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "DlgLang.cpp"
#endif

#define ID_DIALOG 10004
#define SYMBOL_DLGLANG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_DLGLANG_TITLE _("Choose your Language")
#define SYMBOL_DLGLANG_IDNAME ID_DIALOG
#define SYMBOL_DLGLANG_SIZE wxSize(400, 300)
#define SYMBOL_DLGLANG_POSITION wxDefaultPosition
#define ID_COMBOBOX 10005

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif
#ifndef wxFIXED_MINSIZE
#define wxFIXED_MINSIZE 0
#endif

class DlgLang: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( DlgLang )
    DECLARE_EVENT_TABLE()

public:
    DlgLang( );
    DlgLang( wxWindow* parent, wxWindowID id = SYMBOL_DLGLANG_IDNAME, const wxString& caption = SYMBOL_DLGLANG_TITLE, const wxPoint& pos = SYMBOL_DLGLANG_POSITION, const wxSize& size = SYMBOL_DLGLANG_SIZE, long style = SYMBOL_DLGLANG_STYLE );
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_DLGLANG_IDNAME, const wxString& caption = SYMBOL_DLGLANG_TITLE, const wxPoint& pos = SYMBOL_DLGLANG_POSITION, const wxSize& size = SYMBOL_DLGLANG_SIZE, long style = SYMBOL_DLGLANG_STYLE );
    void CreateControls();
    void OnComboboxSelected( wxCommandEvent& event );
    static bool ShowToolTips();
    wxString GetSelLanguage(void);

    wxComboBox* m_pCboLangs;
    wxString m_SelLang;
};

#endif
    // _DLGLANG_H_
