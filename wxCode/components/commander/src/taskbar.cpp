#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "taskbar.h"

enum {
    PU_RESTORE,
    PU_HIDE,
    PU_EXIT,
};

BEGIN_EVENT_TABLE(wxCommanderTaskBar, wxTaskBarIcon)
    EVT_MENU(PU_RESTORE, wxCommanderTaskBar::OnMenuRestore)
    EVT_MENU(PU_HIDE, wxCommanderTaskBar::OnMenuHide)
    EVT_MENU(PU_EXIT, wxCommanderTaskBar::OnMenuExit)
    EVT_TASKBAR_LEFT_DCLICK(wxCommanderTaskBar::OnLeftButtonDClick)
END_EVENT_TABLE()

void wxCommanderTaskBar::OnMenuRestore(wxCommandEvent& )
{
    dialog->Show(true);
    isVisible = true;
}

void wxCommanderTaskBar::OnMenuHide(wxCommandEvent& )
{
    dialog->Show(false);
    isVisible = false;
}

void wxCommanderTaskBar::OnMenuExit(wxCommandEvent& )
{
    dialog->Close(true);
}

wxMenu *wxCommanderTaskBar::CreatePopupMenu()
{
    wxMenu* menu = new wxMenu;
#ifndef __WXMAC_OSX__ /*Mac has built-in quit menu*/
    menu->Append(PU_RESTORE, _T("&Restore wxCommander"));
    menu->Append(PU_HIDE, _T("&Hide wxCommander"));
    menu->AppendSeparator();
    menu->Append(PU_EXIT,    _T("E&xit"));
#endif
    return menu;
}

void wxCommanderTaskBar::OnLeftButtonDClick(wxTaskBarIconEvent&)
{
    dialog->Show(!isVisible);
    isVisible = !isVisible;
}
