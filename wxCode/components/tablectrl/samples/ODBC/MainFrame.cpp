////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        MainFrame.cpp                                                 //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007                                                          //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#include <wx/wxprec.h>

#include <wx/filedlg.h>
#include <wx/dbtable.h>

#include "wx/ODBC/ODBCTable.hpp"

#include "DataSourcePickerDialog.hpp"
#include "TablePickerDialog.hpp"
#include "ChildFrame.hpp"
#include "MainFrame.hpp"



const long              MainFrame :: DEFAULTSTYLE  = wxDEFAULT_FRAME_STYLE;



BEGIN_EVENT_TABLE ( MainFrame , wxMDIParentFrame   )
   EVT_MENU ( wxID_OPEN , MainFrame :: OnMenuOpen  )
END_EVENT_TABLE   ()



void  MainFrame :: OnMenuOpen ( wxCommandEvent &  ce )
{
   DataSourcePickerDialog  dlg   ( this, env );
   
   if ( dlg.ShowModal () != wxID_OK )
      return;
      
   if ( ! dbc -> Connect ( dlg.GetDSN (), dlg.GetUserId (), dlg.GetPasswd () ) )
   {
      wxLogMessage ( "Could not open %s (%s/%s) %s", dlg.GetDSN ().c_str (), dlg.GetUserId ().c_str (), dlg.GetPasswd ().c_str () );
      
      return;
   }

   wxODBCTableArray        ta;
   
   if ( ! dbc -> GetTables ( ta ) )
   {
      wxLogMessage ( "Could not get tables" );
      
      return;
   }
   
   TablePickerDialog       tlg   ( this, *dbc, ta );
   
   if ( tlg.ShowModal () != wxID_OK )
      return;
      
   const wxString          table = tlg.GetTable ();
// const wxString          table = "bestuurders";   
   wxODBCColumnArray       ca;
   
   if ( ! dbc -> GetColumns ( ca, table ) )
   {
      wxLogMessage ( "Could not get columns" );
      
      return;
   }
   
   if ( ca.Count () == 0 )
   {
      wxLogMessage ( "Could not determine columns in table" );
      
      return;
   }
   
   ChildFrame *            child = new  ChildFrame ( this, wxID_ANY, wxString ( dlg.GetDSN () ) + "/" + table );
   
   child -> Open  ( dbc, table ); 
}



MainFrame :: MainFrame ()
   : wxMDIParentFrame ( 0, wxID_ANY, "ODBC (iTableCtrl)", wxDefaultPosition, wxSize ( 600, 400 ) ),
   env   ()
{
   SetIcon ( wxIcon ( "ICON" ) );
   
   wxMenuBar *    menubar  = new  wxMenuBar  ();
   wxMenu *       filemenu = new  wxMenu     ();
   
   filemenu -> Append            ( wxID_OPEN    , "&Open..."   , wxEmptyString );
   filemenu -> AppendSeparator   ();
   filemenu -> Append            ( wxID_EXIT    , "E&xit"      , wxEmptyString );
   
   menubar  -> Append            ( filemenu     , "&File"      );
   
   SetMenuBar  ( menubar );

   dbc   = new  wxODBCDbc  ( env );
}



MainFrame :: ~MainFrame ()
{
   DestroyChildren   ();
   
   delete  dbc;
}
