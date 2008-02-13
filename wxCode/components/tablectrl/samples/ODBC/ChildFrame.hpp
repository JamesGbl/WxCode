////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        ChildFrame.hpp                                                //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007                                                          //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef CHILDFRAME_HPP
#define CHILDFRAME_HPP



#include <wx/TableCtrl.h>

#include "wx/ODBC/ODBCTable.h"

#include "MainFrame.h"
#include "ChildFrame.h"



class  ChildFrame : public  wxMDIChildFrame
{
   private   :
      typedef wxMDIChildFrame
                           super;
                           
   public    :
      static const long    DEFAULTSTYLE;
                                 
   private   :
      wxODBCDbc *          dbc;
      ODBCTable *          table;
      
      wxSizer *            sizer;
      
      wxTableCtrl *        tc;
      
   public    :
      ChildFrame  ( MainFrame *, wxWindowID = wxID_ANY, const wxString & = wxEmptyString, const wxPoint & = wxDefaultPosition, const wxSize & = wxDefaultSize, long = DEFAULTSTYLE );
      ~ChildFrame ();

      bool                 Open  ( wxODBCDbc *, const wxString & );    
};



#endif
