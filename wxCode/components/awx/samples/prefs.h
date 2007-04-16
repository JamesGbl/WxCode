/////////////////////////////////////////////////////////////////////////////
// Name:        prefs.h
// Purpose:     awxtest
// Author:      Joachim Buermann
// Id:          $Id$
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#ifndef __PREFS_H
#define __PREFS_H

#include <wx/window.h>

struct prefs_t {
    wxString m_name;
    wxString m_adr;
    wxString m_phone;
    int m_group;
    prefs_t() {
	   m_group = 0;
    };
};

bool Preferences(wxWindow* parent,struct prefs_t& prefs);

#endif
