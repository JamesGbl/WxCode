#ifndef __SHORTCUT_BAR_SHORTCUT_H__
#define __SHORTCUT_BAR_SHORTCUT_H__

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

class ShortcutPanel;
class ShortcutGroup;

class Shortcut : public wxObject
{
    friend class ShortcutGroup;
 public:
    Shortcut (int id, wxString name, const wxBitmap& bmp);
    Shortcut ();
    wxString       GetName   () { return m_name; }
    void           SetName   (wxString name);
    void           SetBitmap (wxBitmap &bmp);
    void           SetParent (ShortcutPanel *pnl);
 private:
    wxString       m_name;    // Text to display for this shortcut
    wxBitmap       m_bmp;     // Bitmap to display for this shortcut
    ShortcutPanel *m_parent;  // Owner
    int            m_id;      // ID to be used in a post event when the user clicks on this shortcut
};

#endif // __SHORTCUT_BAR_SHORTCUT_H__
