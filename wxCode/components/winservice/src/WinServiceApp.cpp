////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        WinServiceApp.cpp                                             //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007/06/10                                                    //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#include <wx/wxprec.h>
//#include <winbase.h>
//#include <WinSvc.h>

#include "wx/WinServiceEvent.hpp"
#include "wx/WinServiceApp.hpp"



//#define USE_THREAD_AS_EVENT



static const DWORD                        THREAD_ALIVE   = 15000UL;



////////////////////////////////////////////////////////////////////////////////
// wxWinServiceStatus                                                         //
////////////////////////////////////////////////////////////////////////////////



const wxWinServiceStatus :: Type      wxWinServiceStatus :: SERVICETYPE  = Type_WIN32_OWN_PROCESS;  // | Type_INTERACTIVE_PROCESS;
const wxWinServiceStatus :: Accept    wxWinServiceStatus :: ACCEPTED     = static_cast < wxWinServiceStatus :: Accept > ( Accept_STOP | Accept_PAUSE_CONTINUE | Accept_SHUTDOWN | Accept_PARAMCHANGE );



wxWinServiceStatus :: wxWinServiceStatus ()
   : SERVICE_STATUS ()
{
   dwServiceType              = SERVICETYPE;
   
   dwCurrentState             = State_STOPPED;
   dwControlsAccepted         = Accept_NULL;
   
   dwWin32ExitCode            = NO_ERROR;
   dwServiceSpecificExitCode  =   0;
   dwCheckPoint               =   0;
   dwWaitHint                 = 100;
}



wxWinServiceStatus :: wxWinServiceStatus ( const State &  _dwCurrentState, Accept  _dwControlsAccepted )
   : SERVICE_STATUS ()
{
   dwServiceType              = SERVICETYPE;
   
   dwCurrentState             = _dwCurrentState;
   dwControlsAccepted         = _dwControlsAccepted;
   
   dwWin32ExitCode            = NO_ERROR;
   dwServiceSpecificExitCode  =   0;
   dwCheckPoint               =   0;
   dwWaitHint                 = 100;
}

   

const wxChar *  wxWinServiceStatus :: FormatState () const
{
   return ( FormatState ( static_cast < State > ( dwCurrentState ) ) );
}



const wxChar *  wxWinServiceStatus :: FormatState ( const State &  state )
{
   static const wxChar *   STATES   [] =
   {
      wxT ( "Unknown"           ),
      wxT ( "Stopped"           ),
      wxT ( "Start Pending"     ),
      wxT ( "Stop Pending"      ),
      wxT ( "Running"           ),
      wxT ( "Continue Pending"  ),
      wxT ( "Pause Pending"     ),
      wxT ( "Paused"            ),
   };
   
   if ( ( state >= State_STOPPED ) ||
        ( state <= State_PAUSED  )    )
      return ( STATES [ state ] );
      
   return ( STATES [ 0 ] );
}



////////////////////////////////////////////////////////////////////////////////
// wxServiceThread                                                            //
////////////////////////////////////////////////////////////////////////////////



wxWinServiceThread :: wxWinServiceThread ()
   : wxThread ( wxTHREAD_JOINABLE )
{
}


   
////////////////////////////////////////////////////////////////////////////////
// Static functions to map Win API to wxWidgets objects                       //
////////////////////////////////////////////////////////////////////////////////



void WINAPI  wxWinServiceApp :: Main ( DWORD  /*argc*/, LPTSTR *  /*argv*/ )
{
   wxWinServiceApp *   wsa   = wxDynamicCast ( wxApp :: GetInstance (), wxWinServiceApp );

   if ( wsa != 0 )
      wsa -> Start ();
}



DWORD WINAPI  wxWinServiceApp :: Handler ( DWORD  control, DWORD  eventtype, LPVOID  eventdata, LPVOID  _wsa /*context*/ )
{
   wxASSERT ( _wsa != 0 );
   
   wxWinServiceApp *   wsa   = wxDynamicCast ( _wsa, wxWinServiceApp );
   
   if ( wsa != 0 )
   {
      wxWinServiceEvent   wse   ( static_cast < wxWinServiceEvent :: Control > ( control ), eventtype, eventdata );
      
      if ( wsa -> ProcessEvent ( wse ) )
         return ( wse.GetError () );
   }         
      
   return ( ERROR_CALL_NOT_IMPLEMENTED );
}



VOID WINAPI  wxWinServiceApp :: Exit ( PVOID  _wsa /*data*/, BOOLEAN  flag )
{
   wxASSERT ( _wsa != 0 );
   
   wxWinServiceApp *   wsa   = wxDynamicCast ( _wsa, wxWinServiceApp );
   
   if ( wsa != 0 )
      wsa -> Exit ( flag != FALSE );
}



////////////////////////////////////////////////////////////////////////////////
// wxWinServiceApp                                                            //
////////////////////////////////////////////////////////////////////////////////



BEGIN_EVENT_TABLE ( wxWinServiceApp, wxAppConsole )
   EVT_WINSERVICE ( wxWinServiceApp :: OnWinService   )
END_EVENT_TABLE   ()



int  wxWinServiceApp :: OnRun ()
{
   const static SERVICE_TABLE_ENTRY    ste   [] =
   {
      { wxT ( ""    ), Main  },
      { 0            , 0     },
   };
      
   if ( StartServiceCtrlDispatcher ( ste ) != FALSE )
      return (  0 );
      
   return ( -1 );
}



void  wxWinServiceApp :: OnWinService ( wxWinServiceEvent &  wse )
{
   switch ( wse.GetControl () )
   {
      case wxWinServiceEvent :: Control_CONTINUE     :
         if ( ( thread != 0 ) && ( thread -> IsPaused () ) )
            thread -> Resume ();
         
         wxLogMessage ( "wxWinServiceEvent :: Control_CONTINUE" );
                  
         SetStatus  ( wxWinServiceStatus ( wxWinServiceStatus :: State_RUNNING ) );
         
         break;               
         
      case wxWinServiceEvent :: Control_INTERROGATE  :
         wxLogMessage ( "wxWinServiceEvent :: Control_INTEROGATE" );
                  
         break;               
         
      case wxWinServiceEvent :: Control_PARAMCHANGE  :
         wxLogMessage ( "wxWinServiceEvent :: Control_PARAMCHANGE" );
                  
         break;                
         
      case wxWinServiceEvent :: Control_PAUSE        :
         if ( ( thread != 0 ) && ( thread -> IsRunning () ) )
            thread -> Pause ();
         
         wxLogMessage ( "wxWinServiceEvent :: Control_PAUSE" );
                  
         SetStatus ( wxWinServiceStatus ( wxWinServiceStatus :: State_PAUSED ) );
         
         break;              
         
      case wxWinServiceEvent :: Control_SHUTDOWN     :  // System Shutdown.
         wxLogMessage ( "wxWinServiceEvent :: Control_SHUTDOWN" );
                  
//       SetEvent ( event );
         
      case wxWinServiceEvent :: Control_STOP         :
         wxLogMessage ( "wxWinServiceEvent :: Control_STOP" );
                  
         SetStatus   ( wxWinServiceStatus ( wxWinServiceStatus :: State_STOP_PENDING, wxWinServiceStatus :: Accept_SHUTDOWN ) );
         Stop        ();
                  
         break;                
         
      default                                            :
         wxLogMessage ( "%s UNKNOWN: %lX %lX %LX", __FUNCTION__, wse.GetControl (), wse.GetType (), wse.GetData () );
         
         wse.SetError ( wxWinServiceEvent :: Error_CALL_NOT_IMPLEMENTED );
         
         wse.Skip     ();                 // Act as it was not handled!
         
         break;
   }
}



void  wxWinServiceApp :: Install ( bool  create )
{
   TCHAR          name     [ FILENAME_MAX ];
   const DWORD    NAME     = :: GetModuleFileName ( 0, name, FILENAME_MAX );
   
   if ( NAME == 0 )
      return;

   name [ NAME ] = _T ( '\0' );
   
   wxLogMessage ( "%s %s", create ? "Installing" : "Uninstalling", name );
         
   SC_HANDLE      scm      = :: OpenSCManager ( 0, 0, SC_MANAGER_ALL_ACCESS );

   if ( scm == 0 )
   {
      wxLogError ( wxT ( "Could not open Service Control Manager" ) );
      
      return;
   }
      
   SC_HANDLE      handle   = 0;
      
   if ( create )
   {
      if ( ( handle = :: CreateService ( scm, servicename, displayname, SC_MANAGER_CREATE_SERVICE, wxWinServiceStatus :: SERVICETYPE, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, name, 0, 0, 0, 0, 0 ) ) != 0 )
      {
         wxLogMessage ( "Installed '%s' (%s)", servicename, name );
         
         :: CloseServiceHandle ( handle );
      }
      else
         wxLogError ( wxT ( "Could not install %s (%s)" ), servicename, name );
   }
   else
   {
      handle = :: OpenService ( scm, servicename, SC_MANAGER_ALL_ACCESS );
      
      if ( handle != 0 )
      {
         wxLogMessage ( "Found Service %s", servicename );
         
         if ( :: DeleteService ( handle ) )
            wxLogMessage ( "Deleted '%s'", servicename );
         else
            wxLogError   ( wxT ( "Could not delete '%s'" ), servicename );
            
         :: CloseServiceHandle ( handle );
      }
      else
         wxLogError ( wxT ( "Could not open Service '%s'" ), servicename );
   }

   :: CloseServiceHandle   ( scm );
}



bool  wxWinServiceApp :: SetStatus ( const wxWinServiceStatus &  ss )
{
   wxLogMessage ( "SetStatus" );
   
   wss   = ss;
   
   if ( ssh != 0 )
      return ( SetServiceStatus ( ssh, wss ) != FALSE );
      
   // Presume debugging/development mode, i.e. running as a console application!
   
   wxLogMessage   ( wss.FormatState () );
   
   return ( true );
}



void  wxWinServiceApp :: Stop ()
{
// wxLogMessage ( "Stop" );
   
   // Signal 'event' so the wait-thread releases!
   
#ifdef USE_THREAD_AS_EVENT
#else   
   SetEvent ( event );              // Release the wait-thread.
#endif   
}



bool  wxWinServiceApp :: Start ()
{
// wxLogMessage ( "StartService" );
   
   if ( ( ssh = RegisterServiceCtrlHandlerEx ( servicename, Handler, this ) ) == 0 )
      return ( false );
      
// wxLogMessage ( "Register" );
   
   if ( ! SetStatus ( wxWinServiceStatus ( wxWinServiceStatus :: State_RUNNING ) ) )
      return ( false );

// wxLogMessage ( "Run" );
   
   thread   = StartThread  ();

// wxLogMessage ( "Wait" );

#ifdef USE_THREAD_AS_EVENT   
   event    = OpenThread   ( SYNCHRONIZE, FALSE, thread -> GetId () );
#else   
   event    = CreateEvent  ( 0, TRUE, FALSE, 0 /*servicename*/ );
#endif   
   
// RegisterWaitForSingleObject   ( &wait, event, wxWinServiceApp :: Exit, this, INFINITE, WT_EXECUTEONLYONCE );
   RegisterWaitForSingleObject   ( &wait, event, wxWinServiceApp :: Exit, this, THREAD_ALIVE, WT_EXECUTEDEFAULT );
   
// wxLogMessage ( "Wait [return]" );
   
   return ( true );
}



// Exit is used for 2 purposes:
// 1. To normally stop (Exit) the service and terminate (Delete) the 
//    wxWinServiceThread. This is done by calling Stop from the Handler/
//    OnWinService event handler which signals 'event'.
// 2. To stop (Exit) the service and terminate (Delete) the wxWinServiceThread
//    in case it has died. This is done every THREAD_ALIVE milliseconds.
//    if 'flag' is true this THREAD_ALIVE timeout is running.



void  wxWinServiceApp :: Exit ( bool  flag )
{
// wxLogMessage ( "(%p) Exit: thread: %p, flag: %d", this, thread, flag );

   if ( thread == 0 )
      return;
      
   if ( flag )                            // Time out has occurred.
   {                                      // Check if the thread is still alive.
      if ( ( thread != 0 ) && ( thread -> IsAlive () ) )
         return;                          // If it is, return.
   }   

   // For RegisterWaitForSingleObject bug/stupidity.
   // It calls the WAITORTIMERCALLBACK (this function) several times when 'event'
   // is being signaled.
   
   if ( mutex.TryLock () == wxMUTEX_BUSY )
      return;
   
// wxLogMessage ( "Exit: CloseHandle ( event )" );
   
   CloseHandle    ( event );
   
// wxLogMessage ( "Exit: UnregisterWait ( wait )" );
   
   UnregisterWait ( wait );

   if ( thread != 0 )
   {
      thread   -> Delete ();
//    thread   -> Wait     ();
      
      delete  thread;
      
      thread   = 0;
   }
   
// wxLogMessage ( "Exit: SetStatus" );
   
   SetStatus   ( wxWinServiceStatus ( wxWinServiceStatus :: State_STOPPED, wxWinServiceStatus :: Accept_SHUTDOWN ) );
   
// mutex.Unlock   ();                     // Not really!   
}



int  wxWinServiceApp :: Debug ( int  /*argc*/, wxChar **  /*argv*/ )
{
   thread   = StartThread  ();

#ifdef USE_THREAD_AS_EVENT
   event    = OpenThread   ( SYNCHRONIZE, FALSE, thread -> GetId () );
#else
   event    = CreateEvent  ( 0, TRUE, FALSE, 0 /*servicename*/ );
#endif   
   
// RegisterWaitForSingleObject   ( &wait, event, wxWinServiceApp :: Exit, this, INFINITE, WT_EXECUTEONLYONCE );
   RegisterWaitForSingleObject   ( &wait, event, wxWinServiceApp :: Exit, this, THREAD_ALIVE, WT_EXECUTEDEFAULT );
   
   while ( true )
   {
      wxLogMessage   ( "[S]top"    );
      wxLogMessage   ( "[P]ause"   );
      wxLogMessage   ( "[R]esume"  );
      wxLogMessage   ( ""          );
      wxLogMessage   ( "Command: " );

      switch ( getchar () )
      {
         case 'S' :
         case 's' :
            Handler ( SERVICE_CONTROL_STOP      , 0, 0, this );
            
            WaitForSingleObject  ( wait, INFINITE );
            
            while ( wss.GetState () != wxWinServiceStatus :: State_STOPPED )
            {
               wxLogMessage ( "Waiting for SetServiceStatus ( SERVICE_STOPPED )" );
               wxSleep (  5 );
            }
            
            return (  0 );
            
         case 'P' :
         case 'p' :
            Handler ( SERVICE_CONTROL_PAUSE     , 0, 0, this );
            
            break;
            
         case 'R' :
         case 'r' :
            Handler ( SERVICE_CONTROL_CONTINUE  , 0, 0, this );
            
            break;
      }
   }
      
   return (  0 );
}



wxWinServiceApp :: wxWinServiceApp ( const wxString &  _servicename, const wxString &  _displayname )
   : wxAppConsole (),
   servicename ( _servicename ),
   displayname ( _displayname )
{
// wxLogMessage ( __FUNCTION__ );
   
// ste      = 0;
   
   ssh      = 0;
   wait     = INVALID_HANDLE_VALUE;
   event    = INVALID_HANDLE_VALUE;
   
   thread   = 0;
}



wxWinServiceApp :: ~wxWinServiceApp ()
{
// wxLogMessage ( __FUNCTION__ );
}



