#ifndef __SHORTCUT_BAR_SHORTCUT_GROUP_H__
#define __SHORTCUT_BAR_SHORTCUT_GROUP_H__

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

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#define MAX_SHORTCUT_GROUPS 16
#define GROUP_BUTTON_HEIGHT 22

#include "Shortcut.h"
#include "ShortcutPanel.h"

class ShortcutGroup : public wxWindow
{
    friend class ShortcutPanel;
 public:
    ShortcutGroup (wxWindow *parent, int id, wxString name);
    ~ShortcutGroup ();
    
    void            Expand           (bool b);
    int             GetMinimalHeight ();
    void            OnSize           (wxSizeEvent& e);
    void            OnButton         (wxCommandEvent &e);
    void            OnMouseEvents    (wxMouseEvent &e);
    void            SetWidth         (int width);
    void            AddShortcut      (Shortcut *sc);
    Shortcut       *HitTestShortcut  (wxPoint p);
    bool            UpdateShortcut   (Shortcut *sc);
    Shortcut       *GetFirstShortcut (long &cookie);
    Shortcut       *GetNextShortcut  (long &cookie);
    bool            RemoveShortcut   (Shortcut *sc);
 private:
    wxWindow       *m_parent;
    void            Organize();
    int             m_sc_count;
    Shortcut       *m_shortcuts[MAX_SHORTCUT_GROUPS];
    wxBitmapButton *m_sc_button[MAX_SHORTCUT_GROUPS];
    wxStaticText   *m_sc_text[MAX_SHORTCUT_GROUPS];
    wxString        m_name;
    DECLARE_EVENT_TABLE();
};

#endif // __SHORTCUT_BAR_SHORTCUT_GROUP_H__
