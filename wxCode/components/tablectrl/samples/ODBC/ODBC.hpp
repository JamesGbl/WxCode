////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        ODBC.hpp                                                      //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007                                                          //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef ODBC_HPP
#define ODBC_HPP



#include "MainFrame.h"
#include "ODBC.h"



class  ODBC : public  wxApp
{
   private   :
      typedef wxApp     super;
   
   private   :
      MainFrame *       mainframe;
         
   public    :
      bool              OnInit   ();
};



DECLARE_APP ( ODBC )



#endif
