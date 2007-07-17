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

#include "MinimalServiceThread.h"



class  MinimalServiceThread : public  wxWinServiceThread
{
   private   :
      typedef wxWinServiceThread 
                           super;
   
   protected :
      ExitCode             Entry          ();
      
   private   :
      const wxString       name;
      
   public    :
      MinimalServiceThread ( const wxString & );
};



#endif
