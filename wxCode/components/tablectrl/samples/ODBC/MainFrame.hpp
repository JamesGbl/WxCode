// Soli Deo Gloria!



#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP



#include "ODBCTable.hpp"
#include "MainFrame.h"



class  MainFrame : public  wxMDIParentFrame
{
   private   :
      typedef wxMDIParentFrame
                           super;
      
   public    :
      static const long    DEFAULTSTYLE;
      
   private   :
      DECLARE_EVENT_TABLE  ()
      
   private   :
      wxODBCEnv            env;
      wxODBCDbc *          dbc;
      
   private   :
      void                 OnMenuOpen  ( wxCommandEvent & );
      
   public    :
      MainFrame   ();
      ~MainFrame  ();
};



#endif
