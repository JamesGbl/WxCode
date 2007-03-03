/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/palettefrm.h
// Purpose:     wxExtMiniFrame, wxMainFrame, wxMiniButton
//              wxCloseBox, wxMaximizeBox, wxMinimizeBox, wxCollapseBox
// Author:      Francesco Montorsi
// Created:     2004/03/03
// RCS-ID:      $Id$
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PALETTEFRM_H_
#define _WX_PALETTEFRM_H_


// The main frame which owns wxExtMiniFrames.
// Applications using wxExtMiniFrame should derive their own
// frames from this class instead of wxFrame.
//
// A extminiframe window
class wxExtMiniFrame : public wxExtMiniFrameBase
{
protected:      // internal utilities

public:

    // Default constructor.
    wxExtMiniFrame() {}
    wxExtMiniFrame(
        wxMainFrameBase *parent,
        wxWindowID id,
        const wxString &title = wxT("Palette"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxEXTMINIFRM_DEFAULT_STYLE,
        const wxString& name = wxT("wxExtMiniFrame"));

    bool Create(wxMainFrameBase *parent,
            wxWindowID id,
            const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxEXTMINIFRM_DEFAULT_STYLE,
            const wxString& name = wxFrameNameStr);

    // Destructor.
    virtual ~wxExtMiniFrame();



public:     // these must be public since they must be accessible by C functions

    bool m_isDragging;
    int m_diffX, m_diffY;
    int m_oldX, m_oldY;

    // This is used by the C callbacks
    GdkWindow *m_gdkLabelWnd;

private:
    DECLARE_DYNAMIC_CLASS( wxExtMiniFrame )
    DECLARE_EVENT_TABLE()
};


#ifdef wxEXTMINIFRM_USE_MINIBTN

// A GTK mini button: a GDK pixmap attached to a gtkWidget
class WXDLLEXPORT wxMiniButton : public wxMiniButtonBase
{
protected:

    GtkWidget *m_button;

    // Creates this minibutton with the given pixmap.
    virtual void CreatePixmap(char *pixmap[]);

    // Creates the m_button widget.
    virtual void CreateButtonWidget(char **xpm);

public:

    wxMiniButton() {}
    wxMiniButton(wxExtMiniFrameBase *parent, int bSmall = -1)
        : wxMiniButtonBase(parent, bSmall) {}
    virtual ~wxMiniButton() {}


public:     // these must be public since they must be accessible by C functions

    void OnExec() { Exec(); }	
    virtual void Move(int x, int y);

private:
    DECLARE_ABSTRACT_CLASS( wxMiniButton )
};

#endif      // wxEXTMINIFRM_USE_MINIBTN

#endif      // _WX_PALETTEFRM_BASE_H_
