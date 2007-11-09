////////////////////////////////////////////////////////////////////////////////
// Soli Deo Gloria!                                                           //
//                                                                            //
// Name:        cTreePlusView.cpp                                             //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007                                                          //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#include <wx/wxprec.h>

#include "MainFrame.hpp"
#include "cTreePlusView.hpp"



IMPLEMENT_APP  ( cTreePlusView )



bool  cTreePlusView :: OnInit ()
{
   // Create cTree-Plus Session.

   ctThrdAttach   ();
   
   // Create the main application window

   mainframe   = new  MainFrame  ();

   mainframe   -> Show  ();

   SetTopWindow   ( mainframe );

   return ( true );
}



int  cTreePlusView :: OnExit ()
{
   ctThrdDetach   ();

   return ( super :: OnExit () );
}



cTreePlusView :: cTreePlusView ()
   : wxApp ()
{
   mainframe   = 0;
}



cTreePlusView :: ~cTreePlusView ()
{
}
