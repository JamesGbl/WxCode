////////////////////////////////////////////////////////////////////////////////
// Soli Deo Gloria!                                                           //
//                                                                            //
// Name:        ChildFrame.cpp                                                //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007                                                          //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#include <wx/wxprec.h>

#include <wx/filename.h>

#include <wx/TableCtrl.hpp>
#include <cTreeTable.hpp>

#include "MainFrame.hpp"
#include "ChildFrame.hpp"



const long              ChildFrame :: DEFAULTSTYLE = wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION;



ChildFrame :: ChildFrame ( MainFrame *  _window, wxWindowID  _id, const wxString &  _title, const wxPoint &  _point, const wxSize &  _size, long  _style )
   : wxMDIChildFrame ( _window, _id, _title, _point, _size, _style )
{
   session  = 0;
   table    = 0;
   t        = 0;
   
   sizer    = new  wxBoxSizer ( wxVERTICAL );
   
   sizer    -> Add   ( tc  = new  wxTableCtrl   ( this ), 1, wxALL | wxEXPAND, 0 );
   
   tc -> SetToolTip  ( true );
   tc -> DragAndDrop ( true );
   
   SetSizer ( sizer );
}



ChildFrame :: ~ChildFrame ()
{
   tc       -> Table          ( 0 );
   
   delete  t;
   
   table    -> Close          ();
   
   delete  table;
}



bool  ChildFrame :: Open ( CTSession *  session, const wxString &  file )
{
   const wxFileName     fn    ( file );
   const wxString       PATH  ( fn.GetPath () );
   const wxString       NAME  ( fn.GetName () );
   
   try
   {
      table    = new  CTTable    ( session );

      table    -> SetPath        ( PATH.c_str () );
      table    -> Open           ( NAME.c_str (), CTOPEN_NORMAL );
      
      record   = new  CTRecord   ( table );
      
      t        = new  cTreeTable ( *table, *record );
   
      tc       -> Table ( t );
   }
   catch ( CTException &  e )
   {
      wxLogMessage ( "%d: %s %s:%d", e.GetErrorCode (), e.GetErrorMsg (), e.GetSourceFile (), e.GetLineNbr () );
   }
      
   return ( true );
}
