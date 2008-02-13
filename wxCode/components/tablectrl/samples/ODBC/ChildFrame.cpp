////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        ChildFrame.cpp                                                //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007                                                          //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#include <wx/wxprec.h>

#include <wx/TableCtrl.hpp>

#include "wx/ODBC/ODBCTable.hpp"

#include "MainFrame.hpp"
#include "ChildFrame.hpp"



const long              ChildFrame :: DEFAULTSTYLE = wxDEFAULT_FRAME_STYLE;



ChildFrame :: ChildFrame ( MainFrame *  _window, wxWindowID  _id, const wxString &  _title, const wxPoint &  _point, const wxSize &  _size, long  _style )
   : wxMDIChildFrame ( _window, _id, _title, _point, _size, _style )
{
#if 0
   session  = 0;
   table    = 0;
   t        = 0;
#endif
   
   sizer    = new  wxBoxSizer ( wxVERTICAL );
   
   sizer    -> Add   ( tc  = new  wxTableCtrl   ( this ), 1, wxALL | wxEXPAND, 0 );
   
   tc -> SetToolTip  ( true );
   tc -> DragAndDrop ( true );
   
   SetSizer ( sizer );
}



ChildFrame :: ~ChildFrame ()
{
   tc       -> Table          ( 0 );
   
   delete  table;
}



bool  ChildFrame :: Open ( wxODBCDbc *  _dbc, ODBCTable *  _table )
{  
   dbc   = _dbc;
   table = _table;
   
   tc    -> Table ( table );
   
   return ( false );
}
