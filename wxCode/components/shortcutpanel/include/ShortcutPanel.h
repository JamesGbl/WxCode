#ifndef __SHORTCUT_BAR_SHORTCUT_PANEL_H__
#define __SHORTCUT_BAR_SHORTCUT_PANEL_H__

/***************************************************************************
									    
  shortcut.h
  
  Shortcut panel
  
  (C) 2001 Phoenix Data
  
  $Author: jblough $
  $Revision: 1.1 $
  $Date: 2004-09-04 01:27:58 $
  
**************************************************************************/

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

//#ifndef WX_PRECOMP
    #include "wx/wx.h"
//#endif

#include "ShortcutGroup.h"
#include "Shortcut.h"

class ShortcutPanel : public wxPanel
{
  DECLARE_DYNAMIC_CLASS( ShortcutPanel )
  DECLARE_EVENT_TABLE()
public:
  ShortcutPanel( );
  ShortcutPanel (wxWindow *parent, wxWindowID id,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize);
  void           OnSize(wxSizeEvent& e);
  void           OnButton(wxCommandEvent& e);
  void           OnRightDown(wxMouseEvent& e);
  void           OnRightUp(wxMouseEvent& e);
  bool           SelectGroup(int groupid);
  int            AddGroup(wxString name);
  bool           AddShortcut(int groupid, Shortcut *sc);
  Shortcut      *HitTestShortcut(const wxPoint point);
  int            HitTestGroup(const wxPoint& point);
  int            FindGroup(wxString name);
  wxString       GetGroupName(int id);
  void           SetGroupName(int id, wxString name);
  void           UpdateShortcut(Shortcut *sc);
  int            GetSelectedGroup() { return m_selected_group; }
  wxString       GetSelectedGroupName() { return GetGroupName(m_selected_group); }
  int            GetFirstGroupId(long &cookie);
  int            GetNextGroupId(long &cookie);
  Shortcut      *GetFirstShortcut(int groupid, long &cookie);
  Shortcut      *GetNextShortcut(int groupid, long &cookie);
  bool           RemoveShortcut(Shortcut *sc);
  void           RemoveGroup(int groupid);
private:
  int            m_group_id;
  wxColour       m_default_background;
  void           Organize();
  int            m_group_count;
  int            m_selected_group;
  ShortcutGroup *m_groups[MAX_SHORTCUT_GROUPS];
  wxButton      *m_group_button[MAX_SHORTCUT_GROUPS];
};


#endif // __SHORTCUT_BAR_SHORTCUT_PANEL_H__
