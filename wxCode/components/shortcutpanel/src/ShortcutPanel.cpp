/***************************************************************************
									    
  ShortcutPanel.cpp
  
  Shortcut panel
  
  (C) 2002 Phoenix Data
  
  $Author: jblough $
  $Revision: 1.1 $
  $Date: 2004-09-04 01:28:32 $
  
 **************************************************************************/
/* For compilers that support precompilation, includes "wx.h". */
#include <wx/wxprec.h>
#ifdef __BORLANDC__
#pragma hdrstop
#endif
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "ShortcutPanel.h"

IMPLEMENT_DYNAMIC_CLASS( ShortcutPanel, wxPanel )

/*************************************************************************/
/*  ShortcutPanel                                                        */
/*************************************************************************/
BEGIN_EVENT_TABLE(ShortcutPanel, wxPanel)
    EVT_SIZE(ShortcutPanel::OnSize)
    EVT_BUTTON(-1, ShortcutPanel::OnButton)
END_EVENT_TABLE()

ShortcutPanel::ShortcutPanel( )
{
}

ShortcutPanel::ShortcutPanel (wxWindow *parent, wxWindowID id,
                              const wxPoint &pos, const wxSize &size)
    : wxPanel (parent, -1, pos, size, wxSUNKEN_BORDER | wxTAB_TRAVERSAL)
{
    m_group_id = 0;
    m_group_count = 0;
    m_selected_group = 0;
    m_default_background = GetBackgroundColour ();
    SetBackgroundColour (wxColour (120, 120, 120));
    SetForegroundColour (wxColour (255, 255, 255));
}

Shortcut *ShortcutPanel::HitTestShortcut (const wxPoint p)
{
    for (int i = 0; i < m_group_count; i++) {
        if (m_selected_group == m_groups[i]->GetId()) {
            return m_groups[i]->HitTestShortcut(p - m_groups[i]->GetPosition());
        }
    }
    return NULL;
}

int ShortcutPanel::HitTestGroup (const wxPoint& p)
{
    int i, y, h;
    for (i = 0, y = 0; i < m_group_count; i++) {
        h = GROUP_BUTTON_HEIGHT;
        if (m_groups[i]->GetId() == m_selected_group) {
            m_groups[i]->GetSize(NULL, &h);
            h += GROUP_BUTTON_HEIGHT;
        }
        if (p.y >= y && p.y < y + h) {
            return m_groups[i]->GetId();
        }
        y += h;
    }
    return 0;
}

void ShortcutPanel::OnRightDown (wxMouseEvent& e)
{
    ;
}

/**
 *  OnButton
 */
void ShortcutPanel::OnButton (wxCommandEvent& e)
{
    int id = e.GetId ();
    for (int i = 0; i < m_group_count; i++) {
        if (m_groups[i]->GetId () == id) {
            m_selected_group = id;
            Organize ();
        }
    }
}

/**
 *  Förflyttar alla underliggande fönster (knappar osv) så att de ligger rätt
 */
void ShortcutPanel::Organize ()
{
    wxSize s = GetClientSize ();
    wxSize bb;
    int i, y;
    for (i = 0, y = 0; i < m_group_count; i++) {
        if (m_groups[i]->GetId () == m_selected_group) { // Group with focus
            // Place group button
            m_group_button[i]->SetSize (0, y, s.GetWidth (), GROUP_BUTTON_HEIGHT);
            
            // Place and rescale group window
            bb = wxSize (s.GetWidth (),
                         s.GetHeight () - m_group_count * GROUP_BUTTON_HEIGHT);
            m_groups[i]->Move (0, y + GROUP_BUTTON_HEIGHT);
            m_groups[i]->SetSize (bb);
            m_groups[i]->Organize ();
            m_groups[i]->Show (true);
            
            y += bb.GetHeight () + GROUP_BUTTON_HEIGHT;
            
        } else {			// Group without focus
            m_group_button[i]->SetSize (0, y, s.GetWidth (), GROUP_BUTTON_HEIGHT);
            m_groups[i]->Show (false);
            
            y += GROUP_BUTTON_HEIGHT;
        }
    }
}

void ShortcutPanel::OnSize (wxSizeEvent& e)
{
    Organize ();
}

bool ShortcutPanel::SelectGroup (int id)
{
    for (int i = 0; i < m_group_count; i++) {
        if (m_groups[i]->GetId () == id) {
            m_selected_group = id;
            Organize ();
            return true;
        }
    }
    return false;
}

int ShortcutPanel::AddGroup (wxString name)
{
    m_group_id++;
    if (m_group_count == MAX_SHORTCUT_GROUPS) return 0;
    m_groups[m_group_count] = new ShortcutGroup (this, m_group_id, name);
    m_group_button[m_group_count] = new wxButton (this, m_group_id, name);
    m_group_button[m_group_count]->SetBackgroundColour (m_default_background);
    m_group_button[m_group_count]->SetForegroundColour (wxColour(0, 0, 0));
    m_group_count++;
    m_selected_group = m_group_id;
    Organize ();
    return m_group_id;
}

bool ShortcutPanel::AddShortcut (int groupid, Shortcut *sc)
{
    for (int i = 0; i < m_group_count; i++) {
        if (m_groups[i]->GetId () == groupid) {
            m_groups[i]->AddShortcut (sc);
            sc->SetParent (this);
            return true;
        }
    }
    return false;
}

wxString ShortcutPanel::GetGroupName (int id)
{
    for (int i = 0; i < m_group_count; i++)
        if (m_groups[i]->GetId () == id) return m_groups[i]->m_name;
    return wxEmptyString;
}

void ShortcutPanel::SetGroupName (int id, wxString name)
{
    for (int i = 0; i < m_group_count; i++) {
        if (m_groups[i]->GetId () == id) { 
            m_groups[i]->m_name = name;
            m_group_button[i]->SetLabel (name);
            return;
        }
    }
}

/* Return id of group */
int ShortcutPanel::FindGroup (wxString name)
{
  for (int i = 0; i < m_group_count; i++) {
    if (m_groups[i]->m_name == name) {
      return m_groups[i]->GetId ();
    }
  }
  return -1;
}

void ShortcutPanel::UpdateShortcut (Shortcut *sc)
{
    for (int i = 0; i < m_group_count; i++) {
        if (m_groups[i]->UpdateShortcut (sc)) break;
    }
}

int ShortcutPanel::GetFirstGroupId (long &cookie)
{
    cookie = 0;
    return GetNextGroupId (cookie);
}

int ShortcutPanel::GetNextGroupId (long &cookie)
{
    if (cookie < m_group_count) {
        return m_groups[cookie++]->GetId ();
    } else {
        return -1;
    }
}

Shortcut *ShortcutPanel::GetFirstShortcut (int groupid, long &cookie)
{
    for (int i = 0; i < m_group_count; i++) {
        if (m_groups[i]->GetId () == groupid) {
            return m_groups[i]->GetFirstShortcut (cookie);
        }
    }
    return NULL;
}

Shortcut *ShortcutPanel::GetNextShortcut (int groupid, long &cookie)
{
    for (int i = 0; i < m_group_count; i++) {
        if (m_groups[i]->GetId () == groupid) {
            return m_groups[i]->GetNextShortcut (cookie);
        }
    }
    return NULL;
}

bool ShortcutPanel::RemoveShortcut (Shortcut *sc)
{
    bool ret = false;
    for (int i = 0; i < m_group_count; i++) {
        ret |= m_groups[i]->RemoveShortcut (sc);
    }
    if (ret) Organize ();
    return ret;
}

void ShortcutPanel::RemoveGroup (int groupid)
{
    for (int i = 0; i < m_group_count; i++) {
        if (m_groups[i]->GetId () == groupid) {
            delete m_groups[i];
            delete m_group_button[i];
            m_group_count--;
            for (int j = i; j < m_group_count; j++) {
                m_groups[j] = m_groups[j+1];
                m_group_button[j] = m_group_button[j+1];
            }
            if (m_selected_group == groupid) {
                if (i > 0)
                    m_selected_group = m_groups[i-1]->GetId ();
                else if (m_group_count != 0)
                    m_selected_group = m_groups[0]->GetId ();
                else
                    m_selected_group = 0;
            }
            Organize();
        }
    }
}
