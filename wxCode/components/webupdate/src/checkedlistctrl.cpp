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
#include "wx/checked.xpm"
#include "wx/unchecked.xpm"
#include "wx/checked_dis.xpm"
#include "wx/unchecked_dis.xpm"

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

	// the add order must respect the wxCLC_XXX_IMGIDX defines in the headers !
    m_imageList.Add(wxIcon(unchecked_xpm));
    m_imageList.Add(wxIcon(checked_xpm));
    m_imageList.Add(wxIcon(unchecked_dis_xpm));
    m_imageList.Add(wxIcon(checked_dis_xpm));
}
/*
bool wxCheckedListCtrl::IsChecked(long item) const
{
    wxListItem info;
    info.m_mask = wxLIST_MASK_IMAGE;
    info.m_itemId = item;

    if (GetItem(info))
        return (info.m_image == wxCLC_CHECKED_IMGIDX ||
				info.m_image == wxCLC_DISABLED_CHECKED_IMGIDX);
    else
        return FALSE;
}*/

bool wxCheckedListCtrl::GetItem(wxListItem& info) const
{
    SetItemImage(item, (checked ? 1 : 0), -1);
}

bool wxCheckedListCtrl::SetItem(wxListItem& info)
{
	if (IsChecked(item))
		SetItemImage(item, (enable ? 1 : 0), -1);
	else
		SetItemImage(item, (checked ? 1 : 0), -1);
}

bool wxCheckedListCtrl::SetItemState(long item, long state, long stateMask)
{ 
	wxListItem li; 
	li.SetId(item); 
	li.SetState(state); 
	li.SetStateMask(stateMask); 

	// so we are sure to use wxCheckedListCtrl::SetItem
	// (and not wxListCtrl::SetItem)
	SetItem(li); 
}

int wxCheckedListCtrl::GetItemState(long item, long stateMask) const
{ 
	wxListItem li; 
	li.SetId(item); 
	li.SetStateMask(stateMask); 
	
	if (!GetItem(li)) 
		return -1;
	return li.GetState();
}



// event handlers

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


#endif		// wxWU_USE_CHECKEDLISTCTRL
