/////////////////////////////////////////////////////////////////////////////
// Name:        contrib/samples/ogl/studio/mainfrm.h
// Purpose:     Studio main window class
// Author:      Julian Smart
// Modified by:
// Created:     27/7/98
// RCS-ID:      $Id: mainfrm.h,v 1.1 2007/03/28 15:15:50 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _STUDIO_MAINFRM_H_
#define _STUDIO_MAINFRM_H_

class WXDLLIMPEXP_FWD_ADV wxSashLayoutWindow;
class WXDLLIMPEXP_FWD_ADV wxSashEvent;

class csFrame : public wxDocMDIParentFrame
{
   typedef wxDocMDIParentFrame base;
public:
    csFrame(wxDocManager*, wxFrame* parent, wxWindowID, const wxString& title, const wxPoint&, const wxSize&, long style);

protected:
    void OnCloseWindow(wxCloseEvent&);
    void OnSize(wxSizeEvent&);
    void OnAbout(wxCommandEvent&);
    void OnNewWindow(wxCommandEvent&);
    void OnQuit(wxCommandEvent&);
    void OnSashDragPaletteWindow(wxSashEvent&);
    void OnSashDragProjectWindow(wxSashEvent&);
    void OnIdle(wxIdleEvent&);
    void OnHelp(wxCommandEvent&);
    void OnSettings(wxCommandEvent&);

    // General handler for disabling items
    void OnUpdateDisable(wxUpdateUIEvent&);
    void OnSaveUpdate(wxUpdateUIEvent&);

    DECLARE_EVENT_TABLE()
};

class csMDIChildFrame : public wxDocMDIChildFrame
{
    typedef wxDocMDIChildFrame base;
public:
    csMDIChildFrame(wxDocument*, wxMDIParentFrame* parent, wxWindowID, const wxString& title, const wxPoint&, const wxSize&);

protected:
    void OnActivate(wxActivateEvent&);
    DECLARE_EVENT_TABLE()
};

#endif
  // _STUDIO_MAINFRM_H_
