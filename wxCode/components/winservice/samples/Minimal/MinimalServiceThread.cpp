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
   // Only create a <name>.log file when running as a service!

   FILE *         fp    = ( log == Log_FILE ) ? fopen ( name + ".log", "a" ) : 0;
   
// if ( fp == 0 )
//    return (  0 );
      
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

   if ( fp != 0 )   
      fclose ( fp );

   return (  0 );
}



MinimalServiceThread :: MinimalServiceThread ( const wxString &  _name, const Log &  _log )
   : wxWinServiceThread (),
   name  ( _name ),
   log   ( _log  )
{
   wxLogMessage ( "Thread %s", name.c_str () );
}


   
