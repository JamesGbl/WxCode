/////////////////////////////////////////////////////////////////////////////
// Name:        gbsizer.cpp
// Purpose:     provide new wxGridBagSizer class for layout
// Author:      Alex Andruschak
// Modified by:
// Created:
// RCS-ID:      $Id:gbsizer.cpp,v 1.14 2000/01/10 
// Copyright:   (c) DekaSoft
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "gbsizer.h"
#endif

#ifdef __BORLANDC__
    #pragma hdrstop
#endif
// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/gbsizer.h"

//---------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxGridBagSizer, wxSizer);
IMPLEMENT_ABSTRACT_CLASS(wxGridBagItemHandle, wxRect);

//---------------------------------------------------------------------------
// wxGridBagItemHandle
//---------------------------------------------------------------------------

wxGridBagItemHandle::wxGridBagItemHandle( double x, double y, double width, double height)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
}
//---------------------------------------------------------------------------
double wxGridBagItemHandle::GetX()
{
    return m_x;
} 
//---------------------------------------------------------------------------
double wxGridBagItemHandle::GetY()
{
    return m_y;
} 
//---------------------------------------------------------------------------
double wxGridBagItemHandle::GetWidth()
{
    return m_width;
} 
//---------------------------------------------------------------------------
double wxGridBagItemHandle::GetHeight()
{
    return m_height;
} 
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// wxGridBagSizer
//---------------------------------------------------------------------------

wxGridBagSizer::wxGridBagSizer( wxSize & size )
{
    m_cell_count = size;
}
//---------------------------------------------------------------------------
wxGridBagSizer::~wxGridBagSizer()
{
    m_children.DeleteContents(TRUE);
}
//---------------------------------------------------------------------------
void wxGridBagSizer::RecalcSizes()
{
    if (m_children.GetCount() == 0)
        return;
    wxSize size = GetSize();
    wxPoint pos = GetPosition();

    double cell_width = (double)size.GetWidth() / (double)m_cell_count.GetWidth(); 
    double cell_height = (double)size.GetHeight() / (double)m_cell_count.GetHeight();
    wxPoint c_point;
    wxSize  c_size;

    wxNode *current;
    current = m_children.GetFirst();
    while (current != NULL) {
        wxSizerItem *item = (wxSizerItem*) current->Data();
        wxGridBagItemHandle *rect;
        if (item != NULL &&
            (rect = (wxGridBagItemHandle *)item->GetUserData()) != NULL) {
            c_point.x = pos.x + (rect->GetX() * cell_width);
            c_point.y = pos.y + (rect->GetY() * cell_height);
            c_size.SetWidth((rect->GetWidth() * cell_width));
            c_size.SetHeight((rect->GetHeight() * cell_height));

            item->SetDimension(c_point, c_size);
        }
        current = current->Next();
    }
}
//---------------------------------------------------------------------------
wxSize wxGridBagSizer::CalcMin()
{
    if (m_children.GetCount() == 0)
        return wxSize(2,2);

    return wxSize( 100, 100 );
}
//---------------------------------------------------------------------------

