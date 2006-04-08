/////////////////////////////////////////////////////////////////////////////
// Name:        DlgAbout.h
// Purpose:     
// Author:      Ernesto Rangel Dallet
// Modified by: 
// Created:     12/04/05 20:16:05
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _DLGABOUT_H_
#define _DLGABOUT_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "DlgAbout.cpp"
#endif

#define ID_DIALOG 10006
#define SYMBOL_DLGABOUT_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_DLGABOUT_TITLE _("About MyPics")
#define SYMBOL_DLGABOUT_IDNAME ID_DIALOG
#define SYMBOL_DLGABOUT_SIZE wxSize(400, 300)
#define SYMBOL_DLGABOUT_POSITION wxDefaultPosition

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif
#ifndef wxFIXED_MINSIZE
#define wxFIXED_MINSIZE 0
#endif

class MyPicsApp;

DECLARE_APP(MyPicsApp)

class DlgAbout: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( DlgAbout )
    DECLARE_EVENT_TABLE()

public:
    DlgAbout( );
    DlgAbout( wxWindow* parent, wxWindowID id = SYMBOL_DLGABOUT_IDNAME, const wxString& caption = SYMBOL_DLGABOUT_TITLE, const wxPoint& pos = SYMBOL_DLGABOUT_POSITION, const wxSize& size = SYMBOL_DLGABOUT_SIZE, long style = SYMBOL_DLGABOUT_STYLE );
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_DLGABOUT_IDNAME, const wxString& caption = SYMBOL_DLGABOUT_TITLE, const wxPoint& pos = SYMBOL_DLGABOUT_POSITION, const wxSize& size = SYMBOL_DLGABOUT_SIZE, long style = SYMBOL_DLGABOUT_STYLE );
    void CreateControls();
    static bool ShowToolTips();
};

#endif  // _DLGABOUT_H_
