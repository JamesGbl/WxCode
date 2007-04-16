/////////////////////////////////////////////////////////////////////////////
// Name:        menu.h
// Purpose:     awxtest
// Author:      Joachim Buermann
// Id:          $Id$
// Copyright:   (c) 2001-2003 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#ifndef __MENU_H
#define __MENU_H

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

enum {
    ID_QUIT = wxID_HIGHEST + 1,
    ID_TOGGLE,
    ID_FILEOPEN,
    ID_FILESAVE,
    ID_CHOICE,
    ID_SETTINGS,
    ID_VIEW,
    ID_VIEWICON,
    ID_VIEWTREE,
    ID_TB_TEXT
};

wxMenuBar* CreateMenuBar();

#endif
