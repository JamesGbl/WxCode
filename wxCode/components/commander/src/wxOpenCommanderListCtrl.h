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

#define ICON_NORMAL_FILE 0
#define ICON_DIRECTORY 1
#define ICON_EXEC_FILE 2
#define ICON_HARD_DISK 3
#define ICON_DVD 4
#define ICON_FLOPPY 5
#define ICON_REMOVABLE 6

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

     wxString wxOpenCommanderListCtrl::OnGetItemText(long item, long column) const;
     int wxOpenCommanderListCtrl::OnGetItemColumnImage(long item, long column) const;
     wxListItemAttr *wxOpenCommanderListCtrl::OnGetItemAttr(long item) const;
     int getIcon(long itemPos, long itemCol) const;
     int getIconDevice(long itemPos, long itemCol) const;
     void OnSort(wxListEvent& event);


   private:
     wxListItemAttr m_attr;
     cCommander* m_cCommander;
     multiLang& lang;
   public:
     bool showPathAndFile;
};
#endif
