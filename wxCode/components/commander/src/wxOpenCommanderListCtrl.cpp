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
   imageList=NULL;
}

void wxOpenCommanderListCtrl::addColumns()
{
   while (GetColumnCount()>0) DeleteColumn(0);

   InsertColumn(0, lang["Files"], wxLIST_FORMAT_LEFT, 212);
   InsertColumn(1, lang["Size"], wxLIST_FORMAT_RIGHT, 65);
   InsertColumn(2, lang["Date"], wxLIST_FORMAT_RIGHT, 100);

   int size = 20;
   //if (imageList != NULL) delete(imageList); 
   imageList = new wxImageList(size, size, true);

   wxIcon icons[6];
   icons[0] = wxIcon(icon1_xpm);
   icons[1] = wxIcon(icon3_xpm);
   icons[2] = wxIcon(icon5_xpm);
   icons[3] = wxIcon(hardDisk_xpm);
   icons[4] = wxIcon(dvd_xpm);
   icons[5] = wxIcon(floppy_xpm);
   icons[6] = wxIcon(usb_xpm);

   int sizeOrig = icons[0].GetWidth();
   for ( size_t i = 0; i < WXSIZEOF(icons); i++ )
   {
       if ( size == sizeOrig )
           imageList->Add(icons[i]);
       else
           imageList->Add(wxBitmap(wxBitmap(icons[i]).ConvertToImage().Rescale(size, size)));
   }

   AssignImageList(imageList, wxIMAGE_LIST_SMALL);
}

wxString wxOpenCommanderListCtrl::OnGetItemText(long item, long column) const
{
    wxString itemText = m_cCommander->getFileDirActualPath(item, column);
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
   if (m_cCommander->getListDevices()) return getIconDevice(itemPos, itemCol);
   if (itemPos == 0) return 1;
   if (wxDir::Exists(m_cCommander->getActualPath() + "//" + m_cCommander->getFileDirActualPath(itemPos, itemCol)))
      return 1;
   else
      return 0;
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
                 if (iter->first == "A:\\" || iter->first == "B:\\")
                    return 5;
                 else
                    return 3;
              }
     	        case DRIVE_CDROM:
                 return 4;
              default:
                 return 3;
           }
        }
        i++;
     }
   #endif
}
