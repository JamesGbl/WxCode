/////////////////////////////////////////////////////////////////////////////
// Name:        gbsizer.h
// Purpose:     provide wxGridBagSizer class for layouting
// Author:      Alex Andruschak
// Modified by:
// Created:
// RCS-ID:      $Id: gbsizer.h,v 1.10 2000/04/26 
// Copyright:   (c) DekaSoft
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_GRID_BAG_SIZER_H__
#define __WX_GRID_BAG_SIZER_H__

#ifdef __GNUG__
#pragma interface "gbsizer.h"
#endif

#include "wx/sizer.h"

//---------------------------------------------------------------------------
// classes
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// wxGridBagItemHandle
//---------------------------------------------------------------------------

class WXDLLEXPORT wxGridBagItemHandle: public wxObject  
{
    DECLARE_CLASS(wxGridBagItemHandle);
protected:
    double m_x;
    double m_y;
    double m_width;
    double m_height;

public:
    wxGridBagItemHandle( double x, double y, double width, double height);
    double GetX();
    double GetY();
    double GetWidth();
    double GetHeight();
};

//---------------------------------------------------------------------------
// wxGridBagSizer
//---------------------------------------------------------------------------

class WXDLLEXPORT wxGridBagSizer: virtual public wxSizer
{
    DECLARE_CLASS(wxGridBagSizer);

protected:
    wxSize m_cell_count;

public:
   wxGridBagSizer(wxSize & size);
   ~wxGridBagSizer();

   virtual void RecalcSizes();
   virtual wxSize CalcMin();

};

#endif
  // __WX_GRID_BAG_SIZER_H__
