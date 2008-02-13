////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        ODBC.cpp                                                      //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007                                                          //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#include <wx/wxprec.h>

#include <wx/TableCtrl.hpp>

#include "MainFrame.hpp"
#include "ODBC.hpp"



IMPLEMENT_APP  ( ODBC )



bool  ODBC :: OnInit ()
{
   mainframe   = new  MainFrame ();
   
   mainframe   -> Show  ();
   
   SetTopWindow   ( mainframe );
   
   return ( true );
}



int  ODBC :: OnExit ()
{
   return ( super :: OnExit () );
}



ODBC :: ODBC ()
   : wxApp ()
{
   mainframe   = 0;
}



ODBC :: ~ODBC ()
{
}
