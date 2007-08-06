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



#include "wx/LocalMenu.h"

BEGIN_EVENT_TABLE(wxMenuLocal, wxMenu)
    EVT_MENU(wxID_ANY, wxMenuLocal::OnCommand)
END_EVENT_TABLE()

int wxMenuLocal::PopupMenu( wxWindow *wnd, wxPoint pt ){
	if( !wnd ) return wxID_NONE;
	if( !wnd->PopupMenu(this,pt) ) return wxID_NONE;
	// Event handler will catch ID for us to return
	return m_id;
}

void wxMenuLocal::OnCommand(wxCommandEvent &evt){
	m_id = evt.GetId();
}

