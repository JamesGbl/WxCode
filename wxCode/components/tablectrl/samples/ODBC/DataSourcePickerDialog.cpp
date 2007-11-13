// Soli Deo Gloria!



#include <wx/wxprec.h>

#include <sql.h>
#include <sqlext.h>
#include <odbcinst.h>

#include "DataSourcePickerDialog.hpp"



BEGIN_EVENT_TABLE ( DataSourcePickerDialog, wxDialog  )
   EVT_BUTTON  ( wxID_OK   , DataSourcePickerDialog :: OnOK  )
END_EVENT_TABLE   ()



void  DataSourcePickerDialog :: OnOK ( wxCommandEvent &  ce )
{
   if ( sources -> GetSelection () != wxNOT_FOUND )
   {
#if 0   
      connect.SetDsn          ( sources   -> GetString ( sources -> GetSelection () ) );
      connect.SetUid          ( userid    -> GetValue  () );
      connect.SetPassword     ( passwd    -> GetValue  () );
      connect.SetDefaultDir   ( directory -> GetValue  () );
#endif      
   }
      
   ce.Skip  ();
}



void  DataSourcePickerDialog :: LoadDataSources ()
{
   const size_t   DESC  = 255;
   
   SQLCHAR     dsn   [ SQL_MAX_DSN_LENGTH + 1 ];
   SQLCHAR     desc  [ DESC + 1 ];
   wxInt16     ldsn;
   wxInt16     ldesc;
   SQLHENV     env;
   SQLRETURN   rc;
   
   if ( SQLAllocHandle ( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env ) != SQL_SUCCESS )
      return;
      
   while ( ( rc = SQLDataSources ( env, SQL_FETCH_FIRST, dsn, SQL_MAX_DSN_LENGTH, &ldsn, desc, DESC, &ldesc ) ) == SQL_SUCCESS )
      sources -> Append ( dsn );
      
   SQLFreeHandle ( SQL_HANDLE_ENV, env );
}



DataSourcePickerDialog :: DataSourcePickerDialog ( wxWindow *  _window )
   : wxDialog ( _window, wxID_ANY, "Select ODBC Data Source" )
{
   sizer    = new  wxBoxSizer       ( wxVERTICAL );
   
   sizer    -> Add   ( input     = new  wxFlexGridSizer  ( 1, 2, 0, 0 )                         , 0, wxEXPAND | wxALL, 0 );
   
   input    -> Add   (             new  wxStaticText     ( this, wxID_ANY     , "Data Sources" ), 0, wxEXPAND | wxALL, 4 );
   input    -> Add   ( sources   = new  wxListBox        ( this, wxID_ANY )                     , 0, wxEXPAND | wxALL, 4 );
   input    -> Add   (             new  wxStaticText     ( this, wxID_ANY     , "Userid"       ), 0, wxEXPAND | wxALL, 4 );
   input    -> Add   ( userid    = new  wxTextCtrl       ( this, wxID_ANY )                     , 0, wxEXPAND | wxALL, 4 );
   input    -> Add   (             new  wxStaticText     ( this, wxID_ANY     , "Passwd"       ), 0, wxEXPAND | wxALL, 4 );
   input    -> Add   ( passwd    = new  wxTextCtrl       ( this, wxID_ANY )                     , 0, wxEXPAND | wxALL, 4 );
   input    -> Add   (             new  wxStaticText     ( this, wxID_ANY     , "Directory"    ), 0, wxEXPAND | wxALL, 4 );
   input    -> Add   ( directory = new  wxTextCtrl       ( this, wxID_ANY )                     , 0, wxEXPAND | wxALL, 4 );
   
   sizer    -> Add   ( button    = new  wxBoxSizer       ( wxHORIZONTAL )                       , 0, wxCENTER | wxALL, 0 );
   
   button   -> Add   (             new  wxButton         ( this, wxID_OK      , "&OK"          ), 0, wxEXPAND | wxALL, 4 );
   button   -> Add   (             new  wxButton         ( this, wxID_CANCEL  , "&Cancel"      ), 0, wxEXPAND | wxALL, 4 );
   
   sources  -> SetMinSize  ( wxSize ( 320, 240 ) );
   
   LoadDataSources   ();
   
   SetSizer ( sizer );
   Fit      ();
}   
