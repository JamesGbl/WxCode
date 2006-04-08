/////////////////////////////////////////////////////////////////////////////
// Name:        MyFrame.h
// Purpose:     
// Author:      Ernesto Rangel Dallet
// Modified by: 
// Created:     12/04/05 19:31:56
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _MYFRAME_H_
#define _MYFRAME_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "MyFrame.cpp"
#endif

#include "wx/frame.h"
#include "wx/notebook.h"

class MyPicsApp;

#define ID_FRAME 10000
#define SYMBOL_MYFRAME_STYLE wxDEFAULT_FRAME_STYLE|wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_MYFRAME_TITLE _("MyPics sample App")
#define SYMBOL_MYFRAME_IDNAME ID_FRAME
#define SYMBOL_MYFRAME_SIZE wxSize(400, 300)
#define SYMBOL_MYFRAME_POSITION wxDefaultPosition
#define ID_QUIT 10002
#define ID_ABOUT 10007
#define ID_NOTEBOOK 10003
#define ID_PNL_PICS 10005
#define ID_CBO_PICS 10004
#define ID_PNL_SRCH 10008
#define ID_TEXTCTRL 10010
#define ID_PNL_SRCCPP 10009
#define ID_TEXTCTRL1 10011

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif
#ifndef wxFIXED_MINSIZE
#define wxFIXED_MINSIZE 0
#endif

DECLARE_APP(MyPicsApp)

class MyFrame: public wxFrame
{    
    DECLARE_CLASS( MyFrame )
    DECLARE_EVENT_TABLE()

public:
    MyFrame( );
    MyFrame( wxWindow* parent, wxWindowID id = SYMBOL_MYFRAME_IDNAME, const wxString& caption = SYMBOL_MYFRAME_TITLE, const wxPoint& pos = SYMBOL_MYFRAME_POSITION, const wxSize& size = SYMBOL_MYFRAME_SIZE, long style = SYMBOL_MYFRAME_STYLE );
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_MYFRAME_IDNAME, const wxString& caption = SYMBOL_MYFRAME_TITLE, const wxPoint& pos = SYMBOL_MYFRAME_POSITION, const wxSize& size = SYMBOL_MYFRAME_SIZE, long style = SYMBOL_MYFRAME_STYLE );
    void CreateControls();

    void OnQuitClick( wxCommandEvent& event );
    void OnAboutClick( wxCommandEvent& event );
    void OnCboPicsSelected( wxCommandEvent& event );


    void ShowPic(const wxString& PicName);
    static bool ShowToolTips();

    wxComboBox* m_pCboPicList;
    wxStaticText* m_pLblDescription;
    wxStaticBitmap* m_pBmpThePic;
    wxTextCtrl* m_pTxtHFile;
    wxTextCtrl* m_pTxtCppFile;
};

#endif // _MYFRAME_H_
