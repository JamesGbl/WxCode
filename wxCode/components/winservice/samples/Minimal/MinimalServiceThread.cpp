////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        MinimalServiceThread.cpp                                      //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007/06/10                                                    //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#include <wx/wxprec.h>

#include "MinimalServiceThread.hpp"



static const wxUint32      CYCLE_SLEEP = 15000;



MinimalServiceThread :: ExitCode  MinimalServiceThread :: Entry ()
{  
   // This will create a <name>.log file in the Windows SYSTEM32 directory!

   FILE *         fp    = fopen ( name + ".log", "a" );
   
   if ( fp == 0 )
      return (  0 );
      
   wxLogStderr *  log   = new  wxLogStderr   ( fp );
   wxLog *        prev  = wxLog :: SetActiveTarget ( log );
   
   wxLogMessage ( "Minimal Started" );
   
   wxUint32    loop  = 0;
   
   while ( ! TestDestroy () )
   {
      wxLogMessage ( "Minimal Thread Loop %lu", ++loop );
      
      Sleep ( CYCLE_SLEEP );
   }
   
   wxLogMessage ( "Minimal Stopped" );

   wxLog :: SetActiveTarget ( prev );
   
   delete  log;
   
   fclose ( fp );

   return (  0 );
}



MinimalServiceThread :: MinimalServiceThread ( const wxString &  _name )
   : wxWinServiceThread (),
   name  ( _name )
{
   wxLogMessage ( "Thread %s", name.c_str () );
}


   
