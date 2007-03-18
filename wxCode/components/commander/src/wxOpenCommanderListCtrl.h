//---------------------------------------------------------------------------
//
// Name:        wxOpenCommanderListCtrl.h
// Author:      aurdiales
// Created:     27/2/2007 23:16:26
// Description: wxOpenCommanderListCtrl class declaration
//
//---------------------------------------------------------------------------

#ifndef __wxOpenCommanderLISTCTRL_h__
#define __wxOpenCommanderLISTCTRL_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#include <wx/listctrl.h>

#include "cCommander.h"
#include "multiLang.h"
#include "wxOpenCommanderUtils.h"

#include "Images/icon1.xpm"
//#include "Images/icon2.xpm"
#include "Images/icon3.xpm"
//#include "Images/icon4.xpm"
#include "Images/icon5.xpm"
#include "Images/hardDisk.xpm"
#include "Images/dvd.xpm"
#include "Images/floppy.xpm"

class wxOpenCommanderListCtrl : public wxListCtrl

{
   public:
   wxOpenCommanderListCtrl(wxWindow *parent,
               const wxWindowID id,
               const wxPoint& pos,
               const wxSize& size,
               long style,
               cCommander* theCCommander,
               multiLang& langMap);

   void addColumns();
   wxString wxOpenCommanderListCtrl::OnGetItemText(long item, long column) const;
   int wxOpenCommanderListCtrl::OnGetItemColumnImage(long item, long column) const;
   wxListItemAttr *wxOpenCommanderListCtrl::OnGetItemAttr(long item) const;
   int getIcon(long itemPos, long itemCol) const;
   int getIconDevice(long itemPos, long itemCol) const;

   private:
   wxListItemAttr m_attr;
   cCommander* m_cCommander;
   multiLang& lang;
   wxImageList* imageList;
};
#endif
