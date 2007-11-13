// Soli Deo Gloria!



#ifndef ODBC_HPP
#define ODBC_HPP



#include "MainFrame.h"
#include "ODBC.h"



class  ODBC : public  wxApp
{
   private   :
      typedef wxApp     super;
   
   private   :
      MainFrame *       mainframe;
         
   public    :
      bool              OnInit   ();
};



DECLARE_APP ( ODBC )



#endif
