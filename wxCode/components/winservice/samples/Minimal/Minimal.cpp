////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        Minimal.cpp                                                   //
// Purpose:     Minimal wxWidgets Windows Service example.                    //
// Author:      Jan Knepper                                                   //
// Created:     2007/06/10                                                    //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#include <wx/wxprec.h>

#include "CmdLineParser.hpp"
#include "MinimalServiceThread.hpp"
#include "Minimal.hpp"



IMPLEMENT_APP_CONSOLE ( Minimal )



static wxChar *               SERVICENAME = wxT ( "MinimalWinService"                  );
static wxChar *               DISPLAYNAME = wxT ( "Minimal wxWidgets Windows Service"  );



////////////////////////////////////////////////////////////////////////////////
// Minimal                                                                    //
////////////////////////////////////////////////////////////////////////////////



BEGIN_EVENT_TABLE ( Minimal   , wxWinServiceApp )
   EVT_WINSERVICE ( Minimal :: OnWinService )
END_EVENT_TABLE   ()



bool  Minimal :: OnInit ()
{
   cmdlineparser.SetCmdLine ( argc, argv );
   cmdlineparser.Parse      ();
   
   switch ( cmdlineparser.GetAction () )
   {
      case CmdLineParser :: Action_RUN       :
         break;
         
      case CmdLineParser :: Action_INSTALL   :
         Install ( true );
         
         return ( false );
         
      case CmdLineParser :: Action_UNINSTALL :
         Install ( false );
         
         return ( false );
         
      case CmdLineParser :: Action_DEBUG     :
         Debug ( argc, argv );
   }

   return ( true );
}



int  Minimal :: OnExit ()
{
   return ( super :: OnExit () );
}



wxWinServiceThread *  Minimal :: StartThread ()
{
   MinimalServiceThread *    t  = new  MinimalServiceThread  ( SERVICENAME, ( cmdlineparser.GetAction () != CmdLineParser :: Action_DEBUG ) ? MinimalServiceThread :: Log_FILE : MinimalServiceThread :: Log_STDERR );
   
   const wxThreadError  c  = t -> Create   ();
   const wxThreadError  r  = t -> Run      ();
   
   return ( t );
}



void  Minimal :: OnWinService ( wxWinServiceEvent &  wse )
{
   wxLogMessage ( __FUNCTION__ );
   
   wse.Skip ();
}



Minimal :: Minimal ()
   : wxWinServiceApp ( SERVICENAME, DISPLAYNAME )
{
   wxLog :: SetTimestamp ( wxT ( "%c" ) );
   
// wxLogMessage ( __FUNCTION__ );
}



Minimal :: ~Minimal ()
{
// wxLogMessage ( __FUNCTION__ );
}
