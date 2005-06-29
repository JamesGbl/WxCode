/////////////////////////////////////////////////////////////////////////////
// Name:        checkedlistctrl.cpp
// Purpose:     wxCheckedListCtrl
// Author:      Uknown ? (find at http://wiki.wxwidgets.org/wiki.pl?WxListCtrl)
// Created:     2005/06/29
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
	#pragma implementation "checkedlistctrl.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#include "wx/checkedlistctrl.h"


#if wxWU_USE_CHECKEDLISTCTRL

// resources
#include "checked.xpm"
#include "unchecked.xpm"

IMPLEMENT_CLASS(wxCheckedListCtrl, wxListCtrl)
BEGIN_EVENT_TABLE(wxCheckedListCtrl, wxListCtrl)
    EVT_LEFT_DOWN(wxCheckedListCtrl::OnMouseEvent)
END_EVENT_TABLE()



// ------------------
// wxCHECKEDLISTCTRL
// ------------------

wxCheckedListCtrl::wxCheckedListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pt,
        const wxSize& sz, long style) :
        wxListCtrl(parent, id, pt, sz, style), m_imageList(16, 16, TRUE)

{
    SetImageList(&m_imageList, wxIMAGE_LIST_SMALL);

    m_imageList.Add(wxICON(unchecked_xpm));
    m_imageList.Add(wxICON(checked_xpm));
}

void wxCheckedListCtrl::OnMouseEvent(wxMouseEvent& event)
{
    if (event.LeftDown())
    {
        int flags;
        long item = HitTest(event.GetPosition(), flags);
        if (item > -1 && (flags & wxLIST_HITTEST_ONITEMICON))
        {
            SetChecked(item, !IsChecked(item));
        }
        else
            event.Skip();
    }
    else
    {
        event.Skip();
    }
}

bool wxCheckedListCtrl::IsChecked(long item) const
{
    wxListItem info;
    info.m_mask = wxLIST_MASK_IMAGE;
    info.m_itemId = item;

    if (GetItem(info))
    {
        return (info.m_image == 1);
    }
    else
        return FALSE;
}

void wxCheckedListCtrl::SetChecked(long item, bool checked)
{
    SetItemImage(item, (checked ? 1 : 0), -1);
}

#endif		// wxWU_USE_CHECKEDLISTCTRL
