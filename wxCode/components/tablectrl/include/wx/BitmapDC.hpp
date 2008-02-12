////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        BitmapDC.hpp                                                  //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     1998                                                          //
// Copyright:   (c) 1998-2008 Jan Knepper                                     //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef BITMAPDC_HPP
#define BITMAPDC_HPP



#include <wx/dcmemory.h>

#include "BitmapDC.h"



class  wxBitmapDC : public  wxMemoryDC
{
   private   :
      typedef wxMemoryDC   super;

   private   :
      wxBitmap             bitmap;
      wxDC *               dc;
      wxRect               rect;

   public    :
      wxBitmapDC  ( wxDC *  dc, const wxRect &  rect );
      wxBitmapDC  ( wxDC *  dc, const wxSize &  size );
      wxBitmapDC  ( wxDC *  dc, int  w, int  h );

      void                 Update   ( int  rop = wxCOPY );
      void                 Update   ( const wxPoint &, int  rop = wxCOPY );
};



#endif
