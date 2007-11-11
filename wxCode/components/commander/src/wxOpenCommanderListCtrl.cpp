//---------------------------------------------------------------------------
//
// Name:        wxOpenCommanderListCtrl.cpp
// Author:      aurdiales
// Created:     27/2/2007 23:16:26
// Description: wxOpenCommanderListCtrl class implementation
//
//---------------------------------------------------------------------------

#include "wxOpenCommanderListCtrl.h"

wxOpenCommanderListCtrl::wxOpenCommanderListCtrl(wxWindow *parent, const wxWindowID id, const wxPoint& pos, const wxSize& size, long style, cCommander* theCCommander, multiLang& langMap)
: lang(langMap) , wxListCtrl(parent, id, pos, size, style)
{
   m_cCommander = theCCommander;
   showPathAndFile = false;
}

wxString wxOpenCommanderListCtrl::OnGetItemText(long item, long column) const
{
    wxString itemText = m_cCommander->getFileDirActualPath(item, column);
    if (column == 0 && !showPathAndFile) itemText = getLastDir(itemText);
    if (m_cCommander->getListDevices())
       return lang[itemText];
    else
       return itemText;
} 

int wxOpenCommanderListCtrl::OnGetItemColumnImage(long item, long column) const
{
   if (column) return -1;
   return getIcon(item, column);
}

wxListItemAttr *wxOpenCommanderListCtrl::OnGetItemAttr(long item) const
{
    return item % 2 ? NULL : (wxListItemAttr *)&m_attr;
}

int wxOpenCommanderListCtrl::getIcon(long itemPos, long itemCol) const
{
   wxString actualFileDir = m_cCommander->getFileDirActualPath(itemPos, itemCol);
   if (m_cCommander->getListDevices()) return getIconDevice(itemPos, itemCol);
   if (wxDir::Exists(actualFileDir))
      return ICON_DIRECTORY;
   else
   {
      wxString ext = getFileExtension(actualFileDir);
      ext.UpperCase();
      if (ext != "EXE" && ext != "BAT" && ext != "COM")
         return ICON_NORMAL_FILE;
      else
         return ICON_EXEC_FILE;
   }
}

int wxOpenCommanderListCtrl::getIconDevice(long itemPos, long itemCol) const
{
    #ifdef __WXMSW__
     driversMap drives = wxGetDrives();
     driversMap::iterator iter;
     long i=0;
     for (iter = drives.begin(); iter != drives.end(); iter++)
     {
        if (i == itemPos)
        {
           switch (iter->second)
           {
              case DRIVE_REMOVABLE:
              {
                 if (iter->first == "A:" || iter->first == "B:")
                    return ICON_FLOPPY;
                 else
                    return ICON_REMOVABLE;
              }
     	        case DRIVE_CDROM:
                 return ICON_DVD;
              default:
                 return ICON_HARD_DISK;
           }
        }
        i++;
     }
   #endif
}
