/////////////////////////////////////////////////////////////////////////////
// Name:        checkedlistctrl.h
// Purpose:     wxCheckedListCtrl
// Author:      Uknown ? (find at http://wiki.wxwidgets.org/wiki.pl?WxListCtrl)
// Created:     2005/06/29
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_CHECKEDLISTCTRL_H_
#define _WX_CHECKEDLISTCTRL_H_

// optimization for GCC
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "webupdate.h"
#endif

// wxWidgets headers
#include "wx/webupdate.h"		// for the WXDLLIMPEXP_WEBUPDATE macro
#include <wx/listctrl.h>
#include <wx/imaglist.h>

#if wxWU_USE_CHECKEDLISTCTRL

#define wxCLC_UNCHECKED_IMGIDX				0
#define wxCLC_CHECKED_IMGIDX				1
#define wxCLC_DISABLED_UNCHECKED_IMGIDX		2
#define wxCLC_DISABLED_CHECKED_IMGIDX		3

// additional state mask flags (wx's defines should end at 0x0040; see listbase.h)
//#define wxLIST_MASK_STATE_CHECKED			0x0100
//#define wxLIST_MASK_STATE_ENABLED			0x1000

// additional state flags (wx's defines should end at 0x0100; see listbase.h)
#define wxLIST_STATE_CHECKED				0x010000
#define wxLIST_STATE_ENABLED				0x100000


//! This is the class which performs all transactions with the server.
//! It uses the wxSocket facilities.
class WXDLLIMPEXP_WEBUPDATE wxCheckedListCtrl : public wxListCtrl
{
protected:
	
	// our set of checkbox images...
	wxImageList m_imageList;

public:
	wxCheckedListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pt,
						const wxSize& sz, long style);
	virtual ~wxCheckedListCtrl() {}


public:			// utilities
	
	// shortcuts to the SetItemState function
	void Check(long item, bool checked)
		{ SetItemState(item, (long)checked, wxLIST_STATE_CHECKED); }
	void Enable(long item, bool enable)
		{ SetItemState(item, (long)enable, wxLIST_STATE_ENABLED); }

	// shortcuts to the GetItemState function
	bool IsChecked(long item) const
		{ return GetItemState(item, wxLIST_STATE_CHECKED) != 0; }
	bool IsEnabled(long item) const
		{ return GetItemState(item, wxLIST_STATE_ENABLED) != 0; }

	// we overload these so we are sure they will use our
	// #GetItem and #SetItem functions...
	bool SetItemState(long item, long state, long stateMask);
	int GetItemState(long item, long stateMask) const;

	bool GetItem(wxListItem& info) const;
	bool SetItem(wxListItem& info);


	long SetItem(long index, int col, const wxString& label, int imageId = -1)
		{ return wxListCtrl::SetItem(index, col, label); /* remove imageId ! */ }

	// the image associated with an element is already in used by wxCheckedListCtrl
	// itself to show the checkbox and it cannot be handled by the user !
	bool SetItemImage(long item, int image)
		{ wxASSERT_MSG(0, wxT("This function cannot be used with wxCheckedListCtrl !")); }

protected:		// event handlers

	void OnMouseEvent(wxMouseEvent& event);

private:
	DECLARE_CLASS(wxCheckedListCtrl)
	DECLARE_EVENT_TABLE()
};

#endif	// wxWU_USE_CHECKEDLISTCTRL

#endif	// _WX_CHECKEDLISTCTRL_H_

