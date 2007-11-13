// Soli Deo Gloria!



#include <wx/wxprec.h>

#include <wx/filedlg.h>
#include <wx/dbtable.h>

#include "DataSourcePickerDialog.hpp"
#include "TablePickerDialog.hpp"
#include "ODBCTable.hpp"
#include "ChildFrame.hpp"
#include "MainFrame.hpp"


#if 0
MainFrame :: Session :: Session ()
   : CTSession ( CTSESSION_CTREE )
{
// SetPath        ( "C:\" );
   SetParam       ( CT_USERPROF, USERPRF_NTKEY | USERPRF_CLRCHK );
   SetDefDateType ( CTDATE_MDY );
   SetDefTimeType ( CTTIME_HMS );
   Logon          ();
}


   
MainFrame :: Session :: ~Session ()
{
   Logout         ();
}
#endif


const long              MainFrame :: DEFAULTSTYLE  = wxDEFAULT_FRAME_STYLE;



BEGIN_EVENT_TABLE ( MainFrame , wxMDIParentFrame   )
   EVT_MENU ( wxID_OPEN , MainFrame :: OnMenuOpen  )
END_EVENT_TABLE   ()



void  MainFrame :: OnMenuOpen ( wxCommandEvent &  ce )
{
   DataSourcePickerDialog  dlg   ( this, env );
   
   if ( dlg.ShowModal () != wxID_OK )
      return;
      
   dbc   = new  wxODBCDbc  ( env );
   
   if ( ! dbc -> Connect ( dlg.GetDSN (), dlg.GetUserId (), dlg.GetPasswd () ) )
   {
      delete  dbc;
      
      dbc   = 0;
      
      wxLogMessage ( "Could not open %s (%s/%s) %s", dlg.GetDSN ().c_str (), dlg.GetUserId ().c_str (), dlg.GetPasswd ().c_str () );
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
   
   ODBCTable *             t     = new  ODBCTable  ( dbc, table );
   ChildFrame *            child = new  ChildFrame ( this, wxID_ANY, wxString ( dlg.GetDSN () ) + "/" + table );
   
   child -> Open  ( dbc, t ); 
   
#if 0
   wxDb *   db = new  wxDb ( connect.GetHenv (), false );
   
   if ( ! db -> Open ( &connect ) )
   {
      delete  db;
      
      return;
   }

   wxDbInf *               dbinfo      = db -> GetCatalog ();
   
   if ( dbinfo == 0 )
      return;
      
   if ( dbinfo -> numTables <= 0 )
      return;
      
   wxDbTableInf &          tableinfo   = dbinfo -> pTableInf [ 4 ];
   ODBCTable *             table       = new  ODBCTable  ( db, tableinfo.tableName, tableinfo.numCols, connect.GetDefaultDir () );
   ChildFrame *            child       = new  ChildFrame ( this, wxID_ANY, wxString ( connect.GetDsn () ) + "/" + tableinfo.tableName );
   
   child -> Open  ( db, table );
#endif      
#if 0
   wxFileDialog      dlg   ( this, "Open ODBC File", wxEmptyString, wxEmptyString, "All Files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST ); 
   
   if ( dlg.ShowModal () != wxID_OK )
      return;
      
   ChildFrame *            child = new  ChildFrame ( this, wxID_ANY, dlg.GetPath () );
   
// child -> Open  ( session, dlg.GetPath () );   
   child -> Open  ( 0, dlg.GetPath () );
   child -> Show  ();
#endif   
}



MainFrame :: MainFrame ()
   : wxMDIParentFrame ( 0, wxID_ANY, "ODBC (iTableCtrl)", wxDefaultPosition, wxSize ( 600, 400 ) ),
   env   ()
{
   dbc   = 0;
   
   SetIcon ( wxIcon ( "ICON" ) );
   
   wxMenuBar *    menubar  = new  wxMenuBar  ();
   wxMenu *       filemenu = new  wxMenu     ();
   
   filemenu -> Append            ( wxID_OPEN    , "&Open..."   , wxEmptyString );
   filemenu -> AppendSeparator   ();
   filemenu -> Append            ( wxID_EXIT    , "E&xit"      , wxEmptyString );
   
   menubar  -> Append            ( filemenu     , "&File"      );
   
   SetMenuBar  ( menubar );
   
#if 0   
   session  = new  Session ();
#endif   
}



MainFrame :: ~MainFrame ()
{
#if 0
   delete  session;
#endif   
}
