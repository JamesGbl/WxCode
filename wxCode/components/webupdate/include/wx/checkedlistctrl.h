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

//! This is the class which performs all transactions with the server.
//! It uses the wxSocket facilities.
class WXDLLIMPEXP_WEBUPDATE wxCheckedListCtrl : public wxListCtrl
{
protected:

	wxImageList m_imageList;

public:
	wxCheckedListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pt,
						const wxSize& sz, long style);
	virtual ~wxCheckedListCtrl() {}

protected:

	void OnMouseEvent(wxMouseEvent& event);
	bool IsChecked(long item) const;
	void SetChecked(long item, bool checked);

private:
	DECLARE_CLASS(wxCheckedListCtrl)
	DECLARE_EVENT_TABLE()
};

#endif	// wxWU_USE_CHECKEDLISTCTRL

#endif	// _WX_CHECKEDLISTCTRL_H_

