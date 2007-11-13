// Soli Deo Gloria!



#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP



#include "MainFrame.h"



class  MainFrame : public  wxMDIParentFrame
{
   private   :
      typedef wxMDIParentFrame
                           super;
      
   private   :
#if 0   
      class  Session : public  CTSession
      {
         private   :
            typedef CTSession    super;
            
         public    :
            Session  ();
            ~Session ();
      };
#endif      
      
      
   public    :
      static const long    DEFAULTSTYLE;
      
   private   :
      DECLARE_EVENT_TABLE  ()
      
   private   :
#if 0   
      Session *            session;
#endif      
      
   private   :
      void                 OnMenuOpen  ( wxCommandEvent & );
      
   public    :
      MainFrame   ();
      ~MainFrame  ();
};



#endif
