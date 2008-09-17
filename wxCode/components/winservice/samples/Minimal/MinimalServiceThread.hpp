////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        MinimalServiceThread.hpp                                      //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007/06/10                                                    //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef MINIMALSERVICETHREAD_HPP
#define MINIMALSERVICETHREAD_HPP



#include <wx/WinServiceApp.hpp>

#include "CmdLineParser.hpp"
#include "MinimalServiceThread.h"



class  MinimalServiceThread : public  wxWinServiceThread
{
   private   :
      typedef wxWinServiceThread 
                           super;
   
   public    :
      enum  Log
      {
         Log_FILE          = 0,
         Log_STDERR        = 1,
      };



   protected :
      ExitCode             Entry          ();
      
   private   :
      const wxString       name;
      const Log            log;
      
   public    :
      MinimalServiceThread ( const wxString &, const Log & );
};



#endif
