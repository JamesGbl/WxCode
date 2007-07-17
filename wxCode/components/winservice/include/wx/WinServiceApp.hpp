////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        WinServiceApp.hpp                                             //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007/06/10                                                    //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef WINSERVICEAPP_HPP
#define WINSERVICEAPP_HPP



#include <wx/app.h>

#include "WinServiceDef.h"
#include "WinServiceEvent.h"
#include "WinServiceApp.h"



class WXDLLIMPEXP_WINSERVICE  wxWinServiceStatus : protected  SERVICE_STATUS
{
   private   :
      typedef SERVICE_STATUS  super;
      
   public    :
      enum  Type
      {
         Type_KERNEL_DRIVER            = SERVICE_KERNEL_DRIVER       ,  
         Type_FILE_SYSTEM_DRIVER       = SERVICE_FILE_SYSTEM_DRIVER  ,  
         Type_ADAPTER                  = SERVICE_ADAPTER             ,  
         Type_RECOGNIZER_DRIVER        = SERVICE_RECOGNIZER_DRIVER   ,  
         Type_DRIVER                   = SERVICE_DRIVER              ,  
         Type_WIN32_OWN_PROCESS        = SERVICE_WIN32_OWN_PROCESS   ,  
         Type_WIN32_SHARE_PROCESS      = SERVICE_WIN32_SHARE_PROCESS ,  
         Type_WIN32                    = SERVICE_WIN32               ,  
//       Type_INTERACTIVE_PROCESS      = SERVICE_INTERACTIVE_PROCESS ,
         Type_TYPE_ALL                 = SERVICE_TYPE_ALL            ,  
      };
      
      

      enum  State
      {
         State_STOPPED                 = SERVICE_STOPPED                      ,
         State_START_PENDING           = SERVICE_START_PENDING                ,
         State_STOP_PENDING            = SERVICE_STOP_PENDING                 ,
         State_RUNNING                 = SERVICE_RUNNING                      ,
         State_CONTINUE_PENDING        = SERVICE_CONTINUE_PENDING             ,
         State_PAUSE_PENDING           = SERVICE_PAUSE_PENDING                ,
         State_PAUSED                  = SERVICE_PAUSED                       ,
      };
      
      
            
      enum  Accept
      {
         Accept_NULL                   = 0                                    ,
         
         Accept_STOP                   = SERVICE_ACCEPT_STOP                  ,
         Accept_PAUSE_CONTINUE         = SERVICE_ACCEPT_PAUSE_CONTINUE        ,
         Accept_SHUTDOWN               = SERVICE_ACCEPT_SHUTDOWN              ,
         Accept_PARAMCHANGE            = SERVICE_ACCEPT_PARAMCHANGE           ,
         Accept_NETBINDCHANGE          = SERVICE_ACCEPT_NETBINDCHANGE         ,
         Accept_HARDWAREPROFILECHANGE  = SERVICE_ACCEPT_HARDWAREPROFILECHANGE ,
         Accept_POWEREVENT             = SERVICE_ACCEPT_POWEREVENT            ,
         Accept_SESSIONCHANGE          = SERVICE_ACCEPT_SESSIONCHANGE         ,
      };
      
      
      
   public    :
      static const Type       SERVICETYPE;
      static const Accept     ACCEPTED;

   public    :
      wxWinServiceStatus   ();
      wxWinServiceStatus   ( const State &, Accept = ACCEPTED );
   
      operator                SERVICE_STATUS *  () { return ( this ); }   
      
      const State             GetState          () const                   { return ( static_cast < State > ( dwCurrentState ) ); }
      
      const wxChar *          FormatState       () const;
      
      static const wxChar *   FormatState       ( const State & );
};



class WXDLLIMPEXP_WINSERVICE  wxWinServiceThread : public  wxThread
{
   private   :
      typedef wxThread        super;
      
   protected :
      wxWinServiceThread   ();
};



class WXDLLIMPEXP_WINSERVICE  wxWinServiceApp : public  wxAppConsole
{
   private   :
      typedef wxAppConsole    super;
      
   private   :
      DECLARE_EVENT_TABLE  ()
      
   public    :
      int                     OnRun             ();
      
   private   :
      static void WINAPI      Main              ( DWORD, LPTSTR * );
      static DWORD WINAPI     Handler           ( DWORD, DWORD, LPVOID, LPVOID );
      static VOID WINAPI      Exit              ( PVOID, BOOLEAN );
      
   private   :
      const wxString          servicename;
      const wxString          displayname;

      wxMutex                 mutex;            // Purely for RegisterWaitForSingleObject bugs/stupidity.
            
      SERVICE_STATUS_HANDLE   ssh;
      HANDLE                  wait;
      HANDLE                  event;
      
      wxWinServiceStatus      wss;              // Last set status.

   protected :      
      wxWinServiceThread *    thread;
      
   private   :
      void                    OnWinService      ( wxWinServiceEvent & );
      
   protected :
      void                    Install           ( bool );
      bool                    SetStatus         ( const wxWinServiceStatus & );
   
      void                    Stop              ();
      
   protected :
      virtual wxWinServiceThread *      
                              StartThread       ()       = 0;
      
   private   :
      bool                    Start             ();
      void                    Exit              ( bool );
   
   protected :
      int                     Debug             ( int, wxChar ** );
         
   protected :
      wxWinServiceApp   ( const wxString &, const wxString & );
      
   public    :
      ~wxWinServiceApp  ();
};



#endif
