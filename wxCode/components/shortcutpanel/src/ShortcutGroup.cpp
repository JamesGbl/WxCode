/***************************************************************************
									    
  ShortcutGroup.cpp
  
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
#include "ShortcutGroup.h"

/*************************************************************************/
/*  ShortcutGroup                                                        */
/*************************************************************************/
BEGIN_EVENT_TABLE(ShortcutGroup, wxWindow)
    EVT_MOUSE_EVENTS(ShortcutGroup::OnMouseEvents)
    EVT_BUTTON(-1, ShortcutGroup::OnButton)
END_EVENT_TABLE()
ShortcutGroup::ShortcutGroup (wxWindow *parent, int id, wxString name)
    : wxWindow (parent, id)
{
    SetBackgroundColour (wxColour (120, 120, 120));
    m_sc_count = 0;
    m_name = name;
}

ShortcutGroup::~ShortcutGroup ()
{
    for (int i = 0; i < m_sc_count; i++) {
        delete m_shortcuts[i];
        delete m_sc_button[i];
        delete m_sc_text[i];
    }
}

void ShortcutGroup::OnButton (wxCommandEvent &e)
{
    wxCommandEvent ev;
    for (int i = 0; i < m_sc_count; i++) {
        if (e.GetEventObject () == m_sc_button[i]) {
            ev.SetEventType (wxEVT_COMMAND_MENU_SELECTED);
            ev.SetClientData (m_shortcuts[i]);
            ev.SetId (m_shortcuts[i]->m_id);
            wxPostEvent (GetParent (), ev);
            return;
        }
    }
}

void ShortcutGroup::OnMouseEvents (wxMouseEvent &e)
{
    ClientToScreen (&e.m_x, &e.m_y);
    GetParent()->ScreenToClient(&e.m_x, &e.m_y);
    GetParent()->ProcessEvent(e);
}

Shortcut *ShortcutGroup::HitTestShortcut (wxPoint p)
{
    int y = 2, h, i;
    wxSize bs;
    for (i = 0; y < p.y && i < m_sc_count; i++) {
        bs = wxSize (m_shortcuts[i]->m_bmp.GetWidth() + 8,
                     m_shortcuts[i]->m_bmp.GetHeight() + 8);
        h = bs.GetHeight() + 28;
        if (p.y >= y && p.y < y + h) return m_shortcuts[i];
        y += h;
    }
    return NULL;
}

void ShortcutGroup::Organize ()
{
    int w = GetSize().GetWidth();
    int y = 2;
    int i;
    wxSize bs;
    for (i = 0; i < m_sc_count; i++) {
        bs = wxSize(m_shortcuts[i]->m_bmp.GetWidth() + 8, m_shortcuts[i]->m_bmp.GetHeight()+8);
        m_sc_button[i]->Move((w-bs.GetWidth())/2, y);
        m_sc_button[i]->SetSize(bs);
        y += bs.GetHeight() + 2;
        m_sc_text[i]->Centre(wxHORIZONTAL);
        m_sc_text[i]->Move(-1, y);
        y += 26;
    }
}

void ShortcutGroup::AddShortcut (Shortcut *sc)
{
    m_shortcuts[m_sc_count] = sc;
    m_sc_button[m_sc_count] = new wxBitmapButton (this,
                                                  -1,
                                                  sc->m_bmp,
                                                  wxDefaultPosition,
                                                  wxDefaultSize);
    m_sc_button[m_sc_count]->SetForegroundColour (wxColour (255, 0, 0));
    m_sc_button[m_sc_count]->Connect (-1,
                                      wxEVT_RIGHT_UP,
                                      (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) &ShortcutGroup::OnMouseEvents);
    m_sc_text[m_sc_count] = new wxStaticText (this,
                                              -1,
                                              sc->m_name,
                                              wxDefaultPosition,
                                              wxDefaultSize,
                                              wxALIGN_CENTRE);
    m_sc_text[m_sc_count]->SetForegroundColour (wxColour (255, 255, 255));
    m_sc_text[m_sc_count]->Connect (-1,
                                    wxEVT_RIGHT_UP,
                                    (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) &ShortcutGroup::OnMouseEvents);
    
    m_sc_count++;
    Organize ();
}

bool ShortcutGroup::RemoveShortcut (Shortcut *sc)
{
    int i, j;
    for (i = 0; i < m_sc_count; i++) {
        if (m_shortcuts[i] == sc) {
            delete m_sc_button[i];
            delete m_sc_text[i];
            m_sc_count--;
            for (j = i; j < m_sc_count; j++) {
                m_shortcuts[j] = m_shortcuts[j+1];
                m_sc_button[j] = m_sc_button[j+1];
                m_sc_text[j] = m_sc_text[j+1];
            }
            Organize();
            return true;
        }
    }
    return false;
}

/**
 *  Anropas av ShortcutPanel::UpdateShortcut.
 *  Returnerar sant om genvägen hittades, annars falskt.
 */
bool ShortcutGroup::UpdateShortcut (Shortcut *sc)
{
    for (int i = 0; i < m_sc_count; i++) {
        if (m_shortcuts[i] == sc) {
            m_sc_button[i]->SetBitmapLabel (sc->m_bmp);
            m_sc_button[i]->SetBitmapFocus (sc->m_bmp);
            m_sc_button[i]->SetBitmapSelected (sc->m_bmp);
            m_sc_text[i]->SetLabel (sc->m_name);
            Organize ();
            return true;
        }
    }
    return false;
}

Shortcut *ShortcutGroup::GetFirstShortcut(long &cookie)
{
    cookie = 0;
    return GetNextShortcut (cookie);
}

Shortcut *ShortcutGroup::GetNextShortcut(long &cookie)
{
    if (cookie < m_sc_count) {
        return m_shortcuts[cookie++];
    } else {
        return NULL;
    }
}
