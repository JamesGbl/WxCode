/////////////////////////////////////////////////////////////////////////////
// Name:        bsizer.cpp
// Purpose:     provide new wxBorderSizer class for layout
// Author:      Alex Andruschak
// Modified by:
// Created:
// RCS-ID:      $Id:bsizer.cpp,v 1.14 2000/20/04 
// Copyright:   (c) DekaSoft
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "bsizer.h"
#endif

#ifdef __BORLANDC__
    #pragma hdrstop
#endif
// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/bsizer.h"

//---------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxBorderSizer, wxSizer);
IMPLEMENT_ABSTRACT_CLASS(wxBorderItemHandle, wxObject);

//---------------------------------------------------------------------------
// wxBorderItemHandle
//---------------------------------------------------------------------------

wxBorderItemHandle::wxBorderItemHandle( wxString ln )
{
    if (ln.CmpNoCase("North") == 0) {
        m_location = wxString("North");
        m_code = 0;
    } else if (ln.CmpNoCase("South") == 0) {
        m_location = wxString("South");
        m_code = 1;
    } else if (ln.CmpNoCase("West") == 0) {
        m_location = wxString("West");
        m_code = 2;
    } else if (ln.CmpNoCase("East") == 0) {
        m_location = wxString("East");
        m_code = 3;
    } else {
        m_location = wxString("Center");
        m_code = 4;
    }
}
//---------------------------------------------------------------------------
wxString wxBorderItemHandle::GetLocationName()
{
    return m_location;
} 
//---------------------------------------------------------------------------
unsigned long wxBorderItemHandle::GetLocationCode()
{
    return m_code;
} 
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// wxBorderSizer
//---------------------------------------------------------------------------
wxBorderSizer::wxBorderSizer()
{
}
//---------------------------------------------------------------------------
wxBorderSizer::~wxBorderSizer()
{
    m_children.DeleteContents(TRUE);
}
//---------------------------------------------------------------------------
void wxBorderSizer::RecalcSizes()
{
    wxPoint  c_point;
    wxSize   c_size;
    long     north_offset = 0;
    long     south_offset = 0;
    long     west_offset = 0;
    long     east_offset = 0;
    wxNode*  current;
    wxSize   size = GetSize();
    wxPoint  pos = GetPosition();

    if (m_children.GetCount() == 0)
        return;

    // m_children list must be sorted before layouting
    m_children.Sort(SortFunction);

    current = m_children.GetFirst();
    while (current != NULL) {
        wxSizerItem *item = (wxSizerItem*) current->Data();
        wxBorderItemHandle* location;
        if (item != NULL && (location = (wxBorderItemHandle *)item->GetUserData()) != NULL) {
            wxSize old_size = item->CalcMin();

            if (item->IsWindow() && item->GetWindow()->GetSizer() != NULL) {
                wxSize min_size = item->GetWindow()->GetSizer()->CalcMin();
                if (old_size.GetWidth() < min_size.GetWidth()
                    || old_size.GetHeight() < min_size.GetHeight()) {
                    old_size = min_size;
                }
            }

            // North
            if (location->GetLocationCode() == 0) {
                c_point.x = pos.x;
                c_point.y = pos.y;
                c_size.SetWidth(size.GetWidth());
                c_size.SetHeight(old_size.GetHeight());
                north_offset = old_size.GetHeight();
            }

            // South
            if (location->GetLocationCode() == 1) {
                c_point.x = pos.x;
                c_point.y = pos.y + size.GetHeight() - old_size.GetHeight(); 
                c_size.SetWidth(size.GetWidth());
                c_size.SetHeight(old_size.GetHeight());
                south_offset = old_size.GetHeight();
            }

            // West
            if (location->GetLocationCode() == 2) {
                c_point.x = pos.x;
                c_point.y = pos.y + north_offset; 
                c_size.SetWidth(old_size.GetWidth());
                c_size.SetHeight(size.GetHeight() - (north_offset + south_offset));
                west_offset = old_size.GetWidth();
            }

            // East
            if (location->GetLocationCode() == 3) {
                c_point.x = pos.x + size.GetWidth() - old_size.GetWidth();
                c_point.y = pos.y + north_offset; 
                c_size.SetWidth(old_size.GetWidth());
                c_size.SetHeight(size.GetHeight() - (north_offset + south_offset));
                east_offset = old_size.GetWidth();
            }

            // Center
            if (location->GetLocationCode() == 4) {
                c_point.x = pos.x + west_offset;
                c_point.y = pos.y + north_offset; 
                c_size.SetWidth(size.GetWidth() - (west_offset + east_offset));
                c_size.SetHeight(size.GetHeight() - (north_offset + south_offset));
            }

            item->SetDimension(c_point, c_size);
        }
        current = current->Next();
    }

}
//---------------------------------------------------------------------------
wxSize wxBorderSizer::CalcMin()
{
    unsigned long width1 = 0;
    unsigned long width2 = 0;
    unsigned long min_width = 0;

    unsigned long height1 = 0;
    unsigned long height2 = 0;
    unsigned long min_height = 0;

    wxNode        *current;

    if (m_children.GetCount() == 0)
        return wxSize(2,2);

    // m_children list must be sorted before calculating
    m_children.Sort(SortFunction);

    current = m_children.GetFirst();
    while (current != NULL) {
        wxSizerItem *item = (wxSizerItem*) current->Data();
        wxBorderItemHandle* location;
        if (item != NULL && (location = (wxBorderItemHandle *)item->GetUserData()) != NULL) {
            wxSize old_size = item->CalcMin();

            if (item->IsWindow() && item->GetWindow()->GetSizer() != NULL) {
                wxSize min_size = item->GetWindow()->GetSizer()->CalcMin();
                if (old_size.GetWidth() < min_size.GetWidth()
                    || old_size.GetHeight() < min_size.GetHeight()) {
                    old_size = min_size;
                }
            }

            // North 
            if (location->GetLocationCode() == 0) {
                min_height += old_size.GetHeight();
                width1 = old_size.GetWidth();
            }

            // South
            if (location->GetLocationCode() == 1) {
                min_height += old_size.GetHeight();
                width2 = old_size.GetWidth();
            }

            // West
            if (location->GetLocationCode() == 2) {
                height1 = old_size.GetHeight();
                min_width += old_size.GetWidth();
            }

            // East
            if (location->GetLocationCode() == 3) {
                height2 = old_size.GetHeight();
                min_width += old_size.GetWidth();
            }

        }
        current = current->Next();
    }

    if (width1 > width2 ) {
        if (width1 > min_width) {
            min_width = width1;
        }
    } else {
        if (width2 > min_width) {
            min_width = width2;
        }
    }
    if (height1 > height2) {
        min_height += height1;
    } else {
        min_height += height2;
    }

    return wxSize( min_width, min_height );
}
//---------------------------------------------------------------------------
int wxBorderSizer::SortFunction(const void* arg1, const void* arg2) {
    wxSizerItem** item1 = (wxSizerItem**)arg1;
    wxSizerItem** item2 = (wxSizerItem**)arg2;

    wxBorderItemHandle* ih1 = (wxBorderItemHandle*)(*item1)->GetUserData();
    wxBorderItemHandle* ih2 = (wxBorderItemHandle*)(*item2)->GetUserData();

    if (ih1 == NULL || ih2 == NULL) {
        return 0;
    }

    if (ih1->GetLocationCode() == ih2->GetLocationCode()) {
        return 0;
    }

    if (ih1->GetLocationCode() > ih2->GetLocationCode()) {
        return 1;
    } else {
        return -1;
    }
}
//---------------------------------------------------------------------------

