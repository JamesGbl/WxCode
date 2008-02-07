////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        MainFrame.hpp                                                 //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007                                                          //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP



#include "wx/ODBC/ODBCTable.hpp"
#include "MainFrame.h"



class  MainFrame : public  wxMDIParentFrame
{
   private   :
      typedef wxMDIParentFrame
                           super;
      
   public    :
      static const long    DEFAULTSTYLE;
      
   private   :
      DECLARE_EVENT_TABLE  ()
      
   private   :
      wxODBCEnv            env;
      wxODBCDbc *          dbc;
      
   private   :
      void                 OnMenuOpen  ( wxCommandEvent & );
      
   public    :
      MainFrame   ();
      ~MainFrame  ();
};



#endif
