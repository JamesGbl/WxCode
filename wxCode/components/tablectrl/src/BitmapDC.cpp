////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        BitmapDC.cpp                                                  //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     1998                                                          //
// Copyright:   (c) 1998-2008 Jan Knepper                                     //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#include <wx/wxprec.h>

#include "wx/BitmapDC.hpp"



wxBitmapDC :: wxBitmapDC ( wxDC *  _dc, const wxRect &  _rect )
   : wxMemoryDC ( _dc ),
   bitmap   ( _rect.GetWidth (), _rect.GetHeight (), *_dc )
{
   dc    = _dc;
   rect  = _rect;
   
// SetLogicalOrigin  ( rect.GetX (), rect.GetY () );
   
   SelectObject   ( bitmap );
}



wxBitmapDC :: wxBitmapDC ( wxDC *  _dc, const wxSize &  _size )
   : wxMemoryDC ( _dc ),
   bitmap   ( _size.GetWidth (), _size.GetHeight (), *_dc )
{
   dc    = _dc;
   rect  = _size;

   SelectObject   ( bitmap );
}



wxBitmapDC :: wxBitmapDC ( wxDC *  _dc, int  w, int  h )
   : wxMemoryDC ( _dc ),
   bitmap   ( w, h, *_dc )
{
   dc    = _dc;
   rect  = wxRect ( 0, 0, w, h );

   SelectObject   ( bitmap );
}



void  wxBitmapDC :: Update ( int  rop )
{
   dc -> Blit ( rect.GetPosition (), rect.GetSize (), this, wxPoint ( 0, 0 ), rop );
}



void  wxBitmapDC :: Update ( const wxPoint &  point, int  rop )
{
   dc -> Blit ( rect.GetPosition (), rect.GetSize (), this, point, rop );
}
