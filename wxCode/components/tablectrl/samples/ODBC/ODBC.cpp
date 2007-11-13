#include <wx/wxprec.h>

#include <wx/TableCtrl.hpp>

#include "MainFrame.hpp"
#include "ODBC.hpp"



IMPLEMENT_APP  ( ODBC )



bool  ODBC :: OnInit ()
{
   mainframe   = new  MainFrame ();
   
   mainframe   -> Show  ();
   
   return ( true );
}
