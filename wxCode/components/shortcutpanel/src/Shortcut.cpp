/***************************************************************************
									    
  Shortcut.cpp
  
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
#include "Shortcut.h"
#include "ShortcutPanel.h"

/*************************************************************************/
/*  Shortcut                                                             */
/*************************************************************************/
Shortcut::Shortcut ()
{
    Shortcut (-1, wxEmptyString, wxNullBitmap);
}

Shortcut::Shortcut (int id, wxString name, const wxBitmap& bmp)
{
    m_id = id;
    m_name = name;
    m_bmp = bmp;
    m_parent = NULL;
}

void Shortcut::SetParent (ShortcutPanel *parent)
{
    m_parent = parent;
}

void Shortcut::SetName (wxString name)
{
    m_name = name;
    if (m_parent != NULL) m_parent->UpdateShortcut(this);
}

void Shortcut::SetBitmap (wxBitmap &bmp)
{
    m_bmp = bmp;
    if (m_parent != NULL) m_parent->UpdateShortcut(this);
}
