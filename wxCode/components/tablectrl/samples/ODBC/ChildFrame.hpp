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

#include "ODBCTable.h"
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
      
#if 0
      CTSession *          session;
      CTTable *            table;
      CTRecord *           record;
      
      NADCTreeTable *      t;
#endif      
      wxODBCDbc *          dbc;
      ODBCTable *          table;
      
      wxSizer *            sizer;
      
      wxTableCtrl *        tc;
      
   public    :
      ChildFrame  ( MainFrame *, wxWindowID = wxID_ANY, const wxString & = wxEmptyString, const wxPoint & = wxDefaultPosition, const wxSize & = wxDefaultSize, long = DEFAULTSTYLE );
      ~ChildFrame ();

#if 0      
      bool                 Open  ( CTSession *, const wxString & );
#endif
      bool                 Open  ( wxODBCDbc *, ODBCTable * );    
};



#endif
