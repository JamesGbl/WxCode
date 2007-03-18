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

BEGIN_EVENT_TABLE(wxOpenCommanderTaskBar, wxTaskBarIcon)
    EVT_MENU(PU_RESTORE, wxOpenCommanderTaskBar::OnMenuRestore)
    EVT_MENU(PU_HIDE, wxOpenCommanderTaskBar::OnMenuHide)
    EVT_MENU(PU_EXIT, wxOpenCommanderTaskBar::OnMenuExit)
    EVT_TASKBAR_LEFT_DCLICK(wxOpenCommanderTaskBar::OnLeftButtonDClick)
END_EVENT_TABLE()

void wxOpenCommanderTaskBar::OnMenuRestore(wxCommandEvent& )
{
    dialog->Show(true);
    isVisible = true;
}

void wxOpenCommanderTaskBar::OnMenuHide(wxCommandEvent& )
{
    dialog->Show(false);
    isVisible = false;
}

void wxOpenCommanderTaskBar::OnMenuExit(wxCommandEvent& )
{
    dialog->Close(true);
}

wxMenu *wxOpenCommanderTaskBar::CreatePopupMenu()
{
    wxMenu* menu = new wxMenu;
#ifndef __WXMAC_OSX__ /*Mac has built-in quit menu*/
    menu->Append(PU_RESTORE, _T(lang["&Restore wxOpenCommander"]));
    menu->Append(PU_HIDE, _T(lang["&Hide wxOpenCommander"]));
    menu->AppendSeparator();
    menu->Append(PU_EXIT,    _T(lang["&Exit"]));
#endif
    return menu;
}

void wxOpenCommanderTaskBar::OnLeftButtonDClick(wxTaskBarIconEvent&)
{
    dialog->Show(!isVisible);
    isVisible = !isVisible;
}
