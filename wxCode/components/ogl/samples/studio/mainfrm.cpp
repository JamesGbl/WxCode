/////////////////////////////////////////////////////////////////////////////
// Name:        ogl/stufio/mainfrm.cpp
// Purpose:     Studio main frame
// Author:      Julian Smart
// Modified by:
// Created:     27/7/98
// RCS-ID:      $Id: mainfrm.cpp,v 1.1 2007/03/28 15:15:50 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:
/////////////////////////////////////////////////////////////////////////////

#include "precomp.h"
#include <wx/aboutdlg.h>

#include "app.h"
#include "view.h"
#include "doc.h"
#include "cspalette.h"
#include "mainfrm.h"
#include "dialogs.h"

BEGIN_EVENT_TABLE(csFrame, wxDocMDIParentFrame)
    EVT_MENU(wxID_ABOUT, csFrame::OnAbout)
    EVT_MENU(wxID_EXIT, csFrame::OnQuit)
    EVT_MENU(wxID_HELP, csFrame::OnHelp)
    EVT_MENU(ID_CS_SETTINGS, csFrame::OnSettings)
    EVT_SIZE(csFrame::OnSize)
    EVT_SASH_DRAGGED(ID_LAYOUT_WINDOW_PALETTE, csFrame::OnSashDragPaletteWindow)
    EVT_SASH_DRAGGED(ID_LAYOUT_WINDOW_PROJECT, csFrame::OnSashDragProjectWindow)
    EVT_IDLE(csFrame::OnIdle)
    EVT_UPDATE_UI(wxID_PRINT, csFrame::OnUpdateDisable)
    EVT_UPDATE_UI(wxID_PREVIEW, csFrame::OnUpdateDisable)
    EVT_UPDATE_UI(wxID_SAVE, csFrame::OnSaveUpdate)
    EVT_UPDATE_UI(wxID_SAVEAS, csFrame::OnSaveUpdate)
    EVT_UPDATE_UI(wxID_UNDO, csFrame::OnUpdateDisable)
    EVT_UPDATE_UI(wxID_REDO, csFrame::OnUpdateDisable)
    EVT_UPDATE_UI(wxID_CUT, csFrame::OnUpdateDisable)
    EVT_UPDATE_UI(wxID_COPY, csFrame::OnUpdateDisable)
    EVT_UPDATE_UI(wxID_PASTE, csFrame::OnUpdateDisable)
    EVT_CLOSE(csFrame::OnCloseWindow)
END_EVENT_TABLE()

// Define my frame constructor
csFrame::csFrame(wxDocManager* manager, wxFrame *parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
  wxDocMDIParentFrame(manager, parent, id, title, pos, size, style)
{
    CreateToolBar(wxNO_BORDER|wxTB_FLAT|wxTB_HORIZONTAL);
    wxGetApp().InitToolBar(GetToolBar());

    // Accelerators
    const wxAcceleratorEntry accel[] =
    {
        wxAcceleratorEntry(wxACCEL_NORMAL, WXK_HELP, wxID_HELP),
        wxAcceleratorEntry(wxACCEL_CMD,    'O',      wxID_OPEN),
        wxAcceleratorEntry(wxACCEL_CMD,    'N',      wxID_NEW),
        wxAcceleratorEntry(wxACCEL_CMD,    'P',      wxID_PRINT)
    };
    SetAcceleratorTable(wxAcceleratorTable(WXSIZEOF(accel), accel));
}

void csFrame::OnHelp(wxCommandEvent& WXUNUSED(event))
{
    wxHelpControllerBase* help;
    help = wxGetApp().GetHelpController();
    if (help)
        help->DisplayContents();
}

void csFrame::OnSettings(wxCommandEvent& WXUNUSED(event))
{
    csSettingsDialog* dialog = new csSettingsDialog(this);
    /* int ret = */ dialog->ShowModal();
    dialog->Destroy();
}

void csFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
      Close(true);
}

void csFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxAboutDialogInfo info;
#if (wxVERSION_NUMBER < 2900)
    info.SetName(wxGetApp().GetAppDisplayName());
#endif
    info.SetCopyright(wxT("Copyright (c) 1999 Julian Smart"));
    info.AddDeveloper(wxT("Julian Smart"));
    info.SetDescription(wxVERSION_STRING);
    info.SetWebSite(wxT("http://wxcode.sf.net/showcomp.php?name=ogl"));
    info.SetLicense(wxT("wxWindows"));
    ::wxAboutBox(info, this);
}

void csFrame::OnSashDragPaletteWindow(wxSashEvent& event)
{
    if (event.GetDragStatus() == wxSASH_STATUS_OUT_OF_RANGE)
        return;

    switch (event.GetId())
    {
        case ID_LAYOUT_WINDOW_PALETTE:
        {
            wxGetApp().GetDiagramPaletteSashWindow()->SetDefaultSize(wxSize(10000, event.GetDragRect().height));
            break;
        }
    }
    wxLayoutAlgorithm layout;
    layout.LayoutMDIFrame(this);
}

void csFrame::OnSashDragProjectWindow(wxSashEvent& event)
{
    if (event.GetDragStatus() == wxSASH_STATUS_OUT_OF_RANGE)
        return;

    switch (event.GetId())
    {
        case ID_LAYOUT_WINDOW_PROJECT:
        {
            wxGetApp().GetProjectSashWindow()->SetDefaultSize(wxSize(event.GetDragRect().width, 10000));
            break;
        }
    }
    wxLayoutAlgorithm layout;
    layout.LayoutMDIFrame(this);
}

// Define the behaviour for the frame closing
// - must delete all frames except for the main one.
void csFrame::OnCloseWindow(wxCloseEvent& event)
{
    int x, y;
    GetPosition(& x, & y);
    wxGetApp().m_mainFramePos = wxPoint(x, y);

    GetSize(& x, & y);
    wxGetApp().m_mainFrameSize = wxSize(x, y);

    event.Skip();
}

void csFrame::OnSize(wxSizeEvent& event)
{
    wxLayoutAlgorithm layout;
    layout.LayoutMDIFrame(this);
    event.Skip();
}

// Make sure the correct toolbars are showing for the active view
void csFrame::OnIdle(wxIdleEvent& event)
{
    wxSashLayoutWindow* paletteWin = wxGetApp().GetDiagramPaletteSashWindow();
    wxSashLayoutWindow* diagramToolBarWin = wxGetApp().GetDiagramToolBarSashWindow();
    if (!paletteWin || !diagramToolBarWin)
        return;
    bool doLayout = false;
    if (GetActiveChild())
    {
        if (!paletteWin->IsShown() || !diagramToolBarWin->IsShown())
        {
            paletteWin->Show();
            diagramToolBarWin->Show();

            doLayout = true;
        }
    }
    else
    {
        if (paletteWin->IsShown() || diagramToolBarWin->IsShown())
        {
            paletteWin->Show(false);
            diagramToolBarWin->Show(false);
            doLayout = true;
        }
    }
    if (doLayout)
    {
        wxLayoutAlgorithm layout;
        layout.LayoutMDIFrame(this);

#if defined(__WXMSW__)
        // Need to do something else to get it to refresh properly
        // when a client frame is first displayed; moving the client
        // window doesn't cause the proper refresh. Just refreshing the
        // client doesn't work (presumably because it's clipping the
        // children).
        // FIXED in wxWidgets, by intercepting wxMDIClientWindow::DoSetSize
        // and checking if the position has changed, before redrawing the
        // child windows.
#if 0
        wxMDIChildFrame* childFrame = GetActiveChild();
        if (childFrame)
        {
            HWND hWnd = (HWND) childFrame->GetHWND();
            ::RedrawWindow(hWnd, NULL, NULL, RDW_FRAME|RDW_ALLCHILDREN|RDW_INVALIDATE );

        }
#endif
#endif // __WXMSW__
    }
    event.Skip();
}

// General handler for disabling items
void csFrame::OnUpdateDisable(wxUpdateUIEvent& event)
{
    event.Enable(false);
}

void csFrame::OnSaveUpdate(wxUpdateUIEvent& event)
{
    event.Enable( (GetActiveChild() != NULL) );
}

/*
 * Child frame
 */

BEGIN_EVENT_TABLE(csMDIChildFrame, wxDocMDIChildFrame)
  EVT_ACTIVATE(csMDIChildFrame::OnActivate)
END_EVENT_TABLE()

csMDIChildFrame::csMDIChildFrame(wxDocument* doc, wxMDIParentFrame *parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size):
wxDocMDIChildFrame(doc, doc->GetFirstView(), parent, id, title, pos, size, 
     wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE | wxMAXIMIZE)
{
    // Accelerators
    const wxAcceleratorEntry accel[] =
    {
        // Usual editing functions
        wxAcceleratorEntry(wxACCEL_NORMAL,  WXK_DELETE,    wxID_CLEAR),
        wxAcceleratorEntry(wxACCEL_CMD,    'X',            wxID_CUT),
        wxAcceleratorEntry(wxACCEL_CMD,    'C',            wxID_COPY),
        wxAcceleratorEntry(wxACCEL_SHIFT,   WXK_INSERT,    wxID_PASTE),
        wxAcceleratorEntry(wxACCEL_CMD,    'V',            wxID_PASTE),
        wxAcceleratorEntry(wxACCEL_CMD,    'A',            wxID_SELECTALL),

        // Undo/redo
        wxAcceleratorEntry(wxACCEL_CMD,    'Z',            wxID_UNDO),
        wxAcceleratorEntry(wxACCEL_CMD,    'Y',            wxID_REDO),

        // Other
        wxAcceleratorEntry(wxACCEL_NORMAL,  WXK_RETURN,     wxID_PROPERTIES),
        wxAcceleratorEntry(wxACCEL_ALT,     WXK_RETURN,     wxID_PROPERTIES),
        wxAcceleratorEntry(wxACCEL_CMD,    'D',             wxID_DUPLICATE),
        wxAcceleratorEntry(wxACCEL_NORMAL,  WXK_HELP,       wxID_HELP),

        // File handling
        wxAcceleratorEntry(wxACCEL_CMD,   'S',            wxID_SAVE),
        wxAcceleratorEntry(wxACCEL_CMD | wxACCEL_SHIFT, 'S', wxID_SAVEAS),
        wxAcceleratorEntry(wxACCEL_CMD,   'O',            wxID_OPEN),
        wxAcceleratorEntry(wxACCEL_CMD,   'N',            wxID_NEW),
        wxAcceleratorEntry(wxACCEL_CMD,   'P',            wxID_PRINT),
        wxAcceleratorEntry(wxACCEL_CMD,   'W',            wxID_CLOSE)
    };
    SetAcceleratorTable(wxAcceleratorTable(WXSIZEOF(accel), accel));
}

void csMDIChildFrame::OnActivate(wxActivateEvent& event)
{
    event.Skip();
/*
    wxSashLayoutWindow* win = wxGetApp().GetDiagramPaletteSashWindow();
    if (!win)
        return;

    win->Show(event.GetActive());

    wxLayoutAlgorithm layout;
    layout.LayoutMDIFrame((wxMDIParentFrame*) GetParent());
*/
}
