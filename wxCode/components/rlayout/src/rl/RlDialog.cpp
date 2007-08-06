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



#ifndef RL_NO_STATE

#include "wx/RlDialog.h"
//#include "utils/utf8String.h"

template<>
wxSize wxRlBaseWindow<wxDialog>::ms_nc_sz(RLDLG_DEF_DIALOG_NC_SZ_X,RLDLG_DEF_DIALOG_NC_SZ_Y);

template<>
wxSize wxRlBaseWindow<wxPanel>::ms_nc_sz(RLDLG_DEF_PANEL_NC_SZ_X,RLDLG_DEF_PANEL_NC_SZ_Y);


BEGIN_EVENT_TABLE(wxRlDialog, wxRlBaseWindow<wxDialog>)
	EVT_BUTTON(wxID_ANY, wxRlDialog::OnButtonClicked)
	//EVT_RADIOBUTTON(wxID_ANY, wxRlDialog::OnRadioSelected)
END_EVENT_TABLE()

wxRlDialog::wxRlDialog( wxWindow* parent, wxWindowID id, const wxChar *title, 
				const wxChar *file_or_data, bool is_file, bool show_edges, double space_ratio,   
				const wxPoint& pos, const wxSize& size, 
				long style, const wxChar *name)
 :  wxRlBaseWindow<wxDialog> (parent, id, title, 
		 					  file_or_data, is_file, show_edges, space_ratio,   
							  pos, size, style, name) {
	 
	// We don't want wxID_OK when user presses escape key
	SetEscapeId( wxID_CANCEL );
}


void wxRlDialog::OnButtonClicked( wxCommandEvent& evt ){
	// First find wxRlItem of this radio button
	wxRlXmlTags *prxt = FindTagsOf(evt);
	
	// If it is a quit button, then end dialog
	if( !prxt || !prxt->m_quits )
		evt.Skip();
	else
		if( prxt->m_quits ) EndModal( evt.GetId() );
}


BEGIN_EVENT_TABLE(wxRlPanel, wxRlBaseWindow<wxPanel>)
	//EVT_BUTTON(wxID_ANY, wxRlPanel::OnButtonClicked)
	//EVT_RADIOBUTTON(wxID_ANY, wxRlDialog::OnRadioSelected)
END_EVENT_TABLE()


wxRlPanel::wxRlPanel( wxWindow* parent, wxWindowID id,  
        const wxChar *file_or_data, bool is_file, bool show_edges, double space_ratio,  
        const wxPoint& pos, const wxSize& size, 
        long style, const wxChar *name ) 
: wxRlBaseWindow<wxPanel> (parent, id,  
                           file_or_data, is_file, show_edges, space_ratio,
                           pos, size, style, name)
{
}

#endif // #ifndef RL_NO_STATE


