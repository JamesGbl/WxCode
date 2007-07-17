////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        Minimal.hpp                                                   //
// Purpose:     Minimal Windows Service example.                              //
// Author:      Jan Knepper                                                   //
// Created:     2007/06/10                                                    //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef MINIMAL_HPP
#define MINIMAL_HPP



#include <wx/WinServiceEvent.hpp>
#include <wx/WinServiceApp.hpp>

#include "MinimalServiceThread.h"
#include "Minimal.h"


    
class  Minimal : public  wxWinServiceApp
{
   private   :
      typedef wxWinServiceApp super;
      
   private   :
      DECLARE_EVENT_TABLE  ()
       
   public    :
      bool                    OnInit            ();
      int                     OnExit            ();
      
   protected :
      wxWinServiceThread *    StartThread       ();
      
   private   :
      void                    OnWinService      ( wxWinServiceEvent & );
      
   public    :
      Minimal  ();
      ~Minimal ();
};



#endif
