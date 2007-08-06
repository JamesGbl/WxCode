//
// Copyright (C) 2007 Arne Steinarson <arst at users dot sourceforge dot net>
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any
// damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute
// it freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must
//    not claim that you wrote the original software. If you use this
//    software in a product, an acknowledgment in the product
//    documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must
//    not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source
//    distribution.
//


#ifndef LOCALMENU_H
#define LOCALMENU_H

#include "wx/menu.h"

class wxMenuLocal : public wxMenu {
public:
	wxMenuLocal( const wxChar *title=_T(""), int style=0 ) : wxMenu(title,style), m_id(wxID_NONE) { }
	
	// Returns the ID of the selected item or wxID_NONE if error/no selection
	int PopupMenu( wxWindow *wnd, wxPoint pt=wxDefaultPosition );
	
protected:
	void OnCommand( wxCommandEvent& evt );
	int m_id;
	
	DECLARE_EVENT_TABLE();
};

#endif // LOCALMENU_H
