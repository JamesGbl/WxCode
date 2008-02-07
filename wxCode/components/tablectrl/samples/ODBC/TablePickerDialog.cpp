////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        TablePickerDialog.cpp                                         //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007                                                          //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#include <wx/wxprec.h>

#include <sql.h>
#include <sqlext.h>
#include <odbcinst.h>

#include "wx/ODBC/ODBCTable.hpp"
#include "TablePickerDialog.hpp"



BEGIN_EVENT_TABLE ( TablePickerDialog, wxDialog  )
   EVT_BUTTON  ( wxID_OK   , TablePickerDialog :: OnOK  )
END_EVENT_TABLE   ()



void  TablePickerDialog :: OnOK ( wxCommandEvent &  ce )
{
   ce.Skip  ();
}



void  TablePickerDialog :: LoadTables ()
{
   const size_t   SIZE  = ta.GetCount ();
   
   for ( size_t  i = 0 ; i < SIZE ; ++i )
   {
      const wxODBCTable &  t  = ta [ i ];
      const long           l  = tables   -> InsertItem ( i, t.GetName () );
      
      tables   -> SetItem  ( l, 1, t.GetType    () );
      tables   -> SetItem  ( l, 2, t.GetRemarks () );
   }
}



TablePickerDialog :: TablePickerDialog ( wxWindow *  _window, wxODBCDbc &  _dbc, wxODBCTableArray &  _ta )
   : wxDialog ( _window, wxID_ANY, "Select ODBC Table" ),
   dbc   ( _dbc ),
   ta    ( _ta  )
{
   sizer    = new  wxBoxSizer       ( wxVERTICAL );
   
   sizer    -> Add   ( input     = new  wxFlexGridSizer  ( 1, 2, 0, 0 )                                                                            , 0, wxEXPAND | wxALL, 0 );
   
   input    -> Add   (             new  wxStaticText     ( this, wxID_ANY     , "Tables"       )                                                   , 0, wxEXPAND | wxALL, 4 );
   input    -> Add   ( tables    = new  wxListCtrl       ( this, wxID_ANY     , wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL   ), 0, wxEXPAND | wxALL, 4 );
   
   sizer    -> Add   ( button    = new  wxBoxSizer       ( wxHORIZONTAL )                                                                          , 0, wxCENTER | wxALL, 0 );
   
   button   -> Add   (             new  wxButton         ( this, wxID_OK      , "&OK"          )                                                   , 0, wxEXPAND | wxALL, 4 );
   button   -> Add   (             new  wxButton         ( this, wxID_CANCEL  , "&Cancel"      )                                                   , 0, wxEXPAND | wxALL, 4 );
   
   tables   -> SetMinSize     ( wxSize ( 320, 240 ) );
   tables   -> InsertColumn   ( 0, "Name"    );
   tables   -> InsertColumn   ( 1, "Type"    );
   tables   -> InsertColumn   ( 2, "Remark"  );
   
   LoadTables   ();
   
   SetSizer ( sizer );
   Fit      ();
}   



const wxString  TablePickerDialog :: GetTable () const
{
   return ( tables -> GetItemText ( tables -> GetNextItem ( wxNOT_FOUND, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED ) ) );
}
