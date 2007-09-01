/////////////////////////////////////////////////////////////////////////////
// Name:        prefs.cpp
// Purpose:     awxtest
// Author:      Joachim Buermann
// Id:          $Id$
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#include "prefs.h"
#include "prefs_wdr.h"
#include <wx/awx-0.3/obdlg.h>

#include "icons/48x48/group.xpm"
#include "icons/48x48/user.xpm"

class FolderUser : public wxOutbarPanel
{
public:
    FolderUser(wxWindow* parent,
			wxWindowID id,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize) :
	   wxOutbarPanel(parent,id,pos,size)
	   {
 		  User(this);
	   };
    bool Get(prefs_t& prefs);
    bool Set(prefs_t& prefs);
};

bool FolderUser::Get(prefs_t& prefs)
{
    prefs.m_name = ((wxTextCtrl*)FindWindow(ID_NAME))->GetValue();
    prefs.m_adr = ((wxTextCtrl*)FindWindow(ID_ADDRESS))->GetValue();
    prefs.m_phone = ((wxTextCtrl*)FindWindow(ID_PHONE))->GetValue();
    return true;
};

bool FolderUser::Set(prefs_t& prefs)
{
    ((wxTextCtrl*)FindWindow(ID_NAME))->SetValue(prefs.m_name);
    ((wxTextCtrl*)FindWindow(ID_ADDRESS))->SetValue(prefs.m_adr);
    ((wxTextCtrl*)FindWindow(ID_PHONE))->SetValue(prefs.m_phone);
    return true;
};

class FolderGroup : public wxOutbarPanel
{
public:
    FolderGroup(wxWindow* parent,
			 wxWindowID id,
			 const wxPoint& pos = wxDefaultPosition,
			 const wxSize& size = wxDefaultSize) :
	   wxOutbarPanel(parent,id,pos,size)
	   {
		  Group(this);
	   };
    bool Get(prefs_t& prefs);
    bool Set(prefs_t& prefs);
};

bool FolderGroup::Get(prefs_t& prefs)
{
    prefs.m_group = ((wxRadioBox*)FindWindow(ID_GROUP))->GetSelection();
    return true;
};

bool FolderGroup::Set(prefs_t& prefs)
{
    ((wxRadioBox*)FindWindow(ID_GROUP))->SetSelection(prefs.m_group);
    return true;
};

bool Preferences(wxWindow* parent, struct prefs_t& prefs)
{
    wxOutbarDialog d(parent,-1,_("Preferences"));
    d.AddPage(user_xpm,
		    _("User data"),
		    new FolderUser(d.GetEmptyPage(),-1,
					    wxDefaultPosition,wxDefaultSize),
		    _("Information about the user")
	   );
    d.AddPage(group_xpm,
		    _("Group data"),
		    new FolderGroup(d.GetEmptyPage(),-1,
						wxDefaultPosition,wxDefaultSize),
		    _("The user is member of this groups")
	   );
    d.SetSelection(_("User data"));

    ((FolderUser*)d.GetPage(0))->Set(prefs);
    ((FolderGroup*)d.GetPage(1))->Set(prefs);

    if(d.ShowModal() == wxID_OK) {
	   ((FolderUser*)d.GetPage(0))->Get(prefs);
	   ((FolderGroup*)d.GetPage(1))->Get(prefs);
	   // something was changed
	   return true;
    }
    // no changes
    return false;
};

