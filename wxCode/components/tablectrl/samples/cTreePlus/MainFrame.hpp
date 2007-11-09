////////////////////////////////////////////////////////////////////////////////
// Soli Deo Gloria!                                                           //
//                                                                            //
// Name:        MainFrame.hpp                                                 //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007                                                          //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP



#include <ctdbsdk.hpp>                    // cTreePlus DB SDK.

#include "MainFrame.h"



class  MainFrame : public  wxMDIParentFrame
{
   private   :
      typedef wxMDIParentFrame
                           super;
      
   private   :
      class  Session : public  CTSession
      {
         private   :
            typedef CTSession    super;
            
         public    :
            Session  ();
            ~Session ();
      };
      
      
      
   public    :
      static const long    DEFAULTSTYLE;
      
   private   :
      DECLARE_EVENT_TABLE  ()
      
   private   :
      Session *            session;
      
   private   :
      void                 OnMenuOpen  ( wxCommandEvent & );
      
   public    :
      MainFrame   ();
      ~MainFrame  ();
};



#endif
