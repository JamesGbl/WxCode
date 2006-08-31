/////////////////////////////////////////////////////////////////////////////
// Name:        appbar.h
// Purpose:     Contains prototypes and constants for appbar.c
// Author:      bart rolet
// Created:     2006/08/01
// Copyright:   (c) 2006 SESS Canada Inc.
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#ifndef _APPBAR_H
#define _APPBAR_H


//#include "windows.h"
#include "wx/wx.h"

//////////////////////////////////////////////////////////////////////////////
// Constants and Enumerations

#define CX_DEFWIDTH     80
#define CY_DEFHEIGHT    60

#define IDT_AUTOHIDE    1
#define IDT_AUTOUNHIDE  2

#define APPBAR_CALLBACK     WM_USER + 1010



//////////////////////////////////////////////////////////////////////////////
// Types

typedef struct tagOPTIONS
{
    BOOL fAutoHide;
    BOOL fOnTop;
    BOOL fHiding;
    UINT uSide;
    DWORD cxWidth;
    DWORD cyHeight;
    RECT rcEdges[4];
} OPTIONS, *POPTIONS;



//////////////////////////////////////////////////////////////////////////////
// Prototypes
class CAppBar 
{
public:

    CAppBar(wxFrame* mainFrame);
    ~CAppBar();
    void AppBar_Size();
    void AppBar_QueryPos( LPRECT lprc);
    void AppBar_QuerySetPos(UINT, LPRECT, PAPPBARDATA, BOOL);
    void AppBar_Callback(HWND, UINT, WPARAM, LPARAM);
    void AppBar_PosChanged(PAPPBARDATA);
    BOOL AppBar_SetAutoHide(BOOL fHide);
    BOOL AppBar_UnRegister();
    BOOL AppBar_Register();
    BOOL AppBar_SetSide(UINT uSide);
    void AppBar_SetAlwaysOnTop(BOOL fOnTop);
    void AppBar_Hide();
    void AppBar_UnHide();
    void AppBar_SetAutoHideTimer();
    void AppBar_SetAutoUnhideTimer();
    POPTIONS GetAppbarData();   
    
    void SlideWindow(LPRECT prc);
    
    BOOL AppBar_AutoHide();
    BOOL AppBar_NoAutoHide();  
    BOOL IsDocked();
    
private:

    wxFrame* m_pMainFrame;
    HWND pHandler;
     
    BOOL g_fAppRegistered;      // TRUE if the appbar is registered
    RECT g_rcAppBar;                    // Current rect of the appbar

    DWORD g_cxWidth, g_cyHeight;


    int g_dtSlideHide;
    int g_dtSlideShow;
    
    POPTIONS m_pOptions;
    
    APPBARDATA abd;
     RECT       m_rc;
};

#endif              
