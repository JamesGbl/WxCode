/////////////////////////////////////////////////////////////////////////////
// Name:        menu.cpp
// Purpose:     awxtest
// Author:      Joachim Buermann
// Id:          $Id$
// Copyright:   (c) 2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#include "menu.h"

// create a menu bar
wxMenuBar* CreateMenuBar()
{
    wxMenu *menuFile = new wxMenu(wxT(""));
    menuFile->Append(ID_QUIT, _("E&xit\tCtrl-X"));

    wxMenu *menuView = new wxMenu;
    menuView->AppendRadioItem(ID_VIEWICON,_("&Icon view\tCtrl-I"));
    menuView->AppendRadioItem(ID_VIEWTREE,_("&Tree view\tCtrl-T"));
    
    wxMenu *menuExtras = new wxMenu;
    menuExtras->AppendCheckItem(ID_TB_TEXT,_("Show toolbar text"));
    menuExtras->Check(ID_TB_TEXT,true);
    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, _("&File"));
    menuBar->Append(menuView, _("&View"));
    menuBar->Append(menuExtras, _("&Extras"));

    return menuBar;
};
