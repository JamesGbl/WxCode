//---------------------------------------------------------------------------
//
// Name:        wxOpenCommanderListCtrl.cpp
// Author:      aurdiales
// Created:     27/2/2007 23:16:26
// Description: wxOpenCommanderListCtrl class implementation
//
//---------------------------------------------------------------------------

#include "wxOpenCommanderListCtrl.h"

wxString wxOpenCommanderListCtrl::OnGetItemText(long item, long column) const
{
    wxString itemText = m_cCommander->getFileDirActualPath(item, column);
    if (m_cCommander->getListDevices())
       return (*lang)[itemText];
    else
       return itemText;
} 

int wxOpenCommanderListCtrl::OnGetItemColumnImage(long item, long column) const
{
   if (column) return -1;
   return m_cCommander->getIcon(item, column);
}

wxListItemAttr *wxOpenCommanderListCtrl::OnGetItemAttr(long item) const
{
    return item % 2 ? NULL : (wxListItemAttr *)&m_attr;
}
