////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        WinServiceEvent.hpp                                           //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007/06/10                                                    //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef WINSERVICEEVENT_HPP
#define WINSERVICEEVENT_HPP



#include <wx/event.h>

#include "WinServiceDef.h"
#include "WinServiceApp.h"
#include "WinServiceEvent.h"



DECLARE_EVENT_TYPE   ( wxEVT_WINSERVICE, 0 )



typedef void         ( wxEvtHandler :: *wxWinServiceEventFunction )  ( wxWinServiceEvent & );



#define wxWinServiceEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxWinServiceEventFunction, &func)



#define EVT_WINSERVICE(func) \
   wx__DECLARE_EVT0(wxEVT_WINSERVICE,wxWinServiceEventHandler(func))



class WXDLLIMPEXP_WINSERVICE  wxWinServiceEvent : public  wxEvent
{
   private   :
      typedef wxEvent         super;
      
   public    :
      enum  Control
      {
         Control_STOP                  = SERVICE_CONTROL_STOP                 ,                
         Control_PAUSE                 = SERVICE_CONTROL_PAUSE                ,
         Control_CONTINUE              = SERVICE_CONTROL_CONTINUE             ,
         Control_INTERROGATE           = SERVICE_CONTROL_INTERROGATE          ,
         Control_SHUTDOWN              = SERVICE_CONTROL_SHUTDOWN             ,
         Control_PARAMCHANGE           = SERVICE_CONTROL_PARAMCHANGE          ,
         Control_NETBINDADD            = SERVICE_CONTROL_NETBINDADD           ,
         Control_NETBINDREMOVE         = SERVICE_CONTROL_NETBINDREMOVE        ,
         Control_NETBINDENABLE         = SERVICE_CONTROL_NETBINDENABLE        ,
         Control_NETBINDDISABLE        = SERVICE_CONTROL_NETBINDDISABLE       ,
         
         Control_DEVICEEVENT           = SERVICE_CONTROL_DEVICEEVENT          ,
         Control_HARDWAREPROFILECHANGE = SERVICE_CONTROL_HARDWAREPROFILECHANGE,
         Control_POWEREVENT            = SERVICE_CONTROL_POWEREVENT           ,
         Control_SESSIONCHANGE         = SERVICE_CONTROL_SESSIONCHANGE        , 
      };
      
      
      
      enum  Error
      {
         Error_NONE                    = NO_ERROR                             ,
         Error_CALL_NOT_IMPLEMENTED    = ERROR_CALL_NOT_IMPLEMENTED           ,
      };
      
      
#if 0      
      enum  Type
      {
         DBT_DEVICEARRIVAL 
      };
#endif      
      
   public    :
      wxEvent *               Clone () const;

   private   :
      Control                 control;
      wxUint32                type;
      const void *            data;
      
      Error                   error;
      
   protected :
      wxWinServiceEvent ( const wxWinServiceEvent & );
   
   public    :
//    wxWinServiceEvent ();
      wxWinServiceEvent ( const Control &, wxUint32, const void * );
      wxWinServiceEvent ( wxWinServiceApp *, const Control &, wxUint32, const void * );
      
      const Control &         GetControl  () const          { return ( control ); }
      const wxUint32          GetType     () const          { return ( type ); }
      const void *            GetData     () const          { return ( data ); }
      
      const Error &           GetError    () const          { return ( error ); }
      void                    SetError    ( const Error & );
};



#endif
