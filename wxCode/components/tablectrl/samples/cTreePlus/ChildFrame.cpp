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
#include <wx/textctrl.h>

#include <wx/TableCtrl.hpp>
#include <wx/cTreePlus/cTreeTable.hpp>

#include "TableMenu.h"
#include "MainFrame.hpp"
#include "ChildFrame.hpp"



const long              ChildFrame :: DEFAULTSTYLE = wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION;



BEGIN_EVENT_TABLE ( ChildFrame, wxMDIChildFrame )
   EVT_MENU_RANGE ( IDM_BEGIN , 
                    IDM_END   , ChildFrame :: OnMenu  )
                    
   EVT_TABLE_COLUMN_BEGIN_MOVE   ( wxID_ANY  , ChildFrame :: OnTableColumn )
   EVT_TABLE_COLUMN_MOVING       ( wxID_ANY  , ChildFrame :: OnTableColumn )
   EVT_TABLE_COLUMN_MOVED        ( wxID_ANY  , ChildFrame :: OnTableColumn )

   EVT_TABLE_COLUMN_BEGIN_SIZE   ( wxID_ANY  , ChildFrame :: OnTableColumn )
   EVT_TABLE_COLUMN_SIZING       ( wxID_ANY  , ChildFrame :: OnTableColumn )
   EVT_TABLE_COLUMN_SIZED        ( wxID_ANY  , ChildFrame :: OnTableColumn )
   
   EVT_TABLE_COLUMN_LEFT_CLICK   ( wxID_ANY  , ChildFrame :: OnTableColumn )
   EVT_TABLE_COLUMN_RIGHT_CLICK  ( wxID_ANY  , ChildFrame :: OnTableColumn )
   EVT_TABLE_COLUMN_MIDDLE_CLICK ( wxID_ANY  , ChildFrame :: OnTableColumn )
END_EVENT_TABLE   ()



void  ChildFrame :: OnMenu ( wxCommandEvent &  ce )
{
   switch ( ce.GetId () )
   {
      case IDM_MULTISELECT       :
         tc -> MultiSelect      ( ce.IsChecked () );
         tc -> RefreshLayout    ();
         
         break;

      case IDM_DRAGANDDROP       :
         tc -> DragAndDrop      ( ce.IsChecked () );
         tc -> RefreshLayout    ();
         
         break;

      case IDM_GRIDHORIZONTAL    :
         tc -> GridHorizontal   ( ce.IsChecked () );
         tc -> RefreshLayout    ();
         
         break;

      case IDM_GRIDVERTICAL      :
         tc -> GridVertical     ( ce.IsChecked () );
         tc -> RefreshLayout    ();
         
         break;

      case IDM_CHECKBOXES        :
         tc -> CheckBoxes       ( ce.IsChecked () );
         tc -> RefreshLayout    ();
         
         break;

      case IDM_FOCUSRECT         :
         tc -> FocusRect        ( ce.IsChecked () );
         tc -> RefreshLayout    ();
         
         break;

      case IDM_TOOLTIPS          :
         tc -> SetToolTip       ( ce.IsChecked () );
         tc -> RefreshLayout    ();
         
         break;

      case IDM_NATIVEHEADER      :
         tc -> SetNativeHeader  ( ce.IsChecked () );
         tc -> RefreshLayout    ();
         
         break;

      case IDM_COLUMNRESIZING    :
         columnresizing = ! columnresizing;

         if ( tc != 0 )
         {
            wxTableCtrl :: Header &    header   = tc -> GetHeader ();
            size_t                     COLS     = header.size ();
            
            for ( size_t  i = 0 ; i < COLS ; ++i )
            {
               wxTableCtrl :: Column *    column   = header.Find ( i );
               
               if ( column != 0 )
                  column -> Resize ( columnresizing );
            }
         }

         break;
         
      case IDM_SWITCH_FAST       :
      case IDM_SWITCH_MERGE      :
      case IDM_SWITCH_ROLL       :
         tc -> SetMode          ( static_cast < wxTableCtrl :: Body :: Mode > ( ce.GetId () - IDM_SWITCH ) );
         
         break;
   }
}



void  ChildFrame :: OnTableColumn ( wxTableEvent &  te )
{
   const wxRect &    rect     = te.GetRect ();
   const wxTableCtrl :: Column *
                     column   = te.GetColumn ();

   wxLogMessage ( "wxTableEvent %d (%d,%d-%d,%d) %s", te.GetEventType (), rect.GetX (), rect.GetY (), rect.GetWidth  (), rect.GetHeight (), ( column != 0 ) ? column -> Name () : "" );
   
   te.Skip  ();
}



ChildFrame :: ChildFrame ( MainFrame *  _window, wxWindowID  _id, const wxString &  _title, const wxPoint &  _point, const wxSize &  _size, long  _style )
   : wxMDIChildFrame ( _window, _id, _title, _point, _size, _style )
{
   session        = 0;
   table          = 0;
   t              = 0;
   columnresizing = true;
   
   wxMenuBar *    menubar     = new  wxMenuBar  ();
   wxMenu *       filemenu    = new  wxMenu     ();
   wxMenu *       tablemenu   = new  wxMenu     ();
   wxMenu *       switchmenu  = new  wxMenu     ();
   
   filemenu    -> Append            ( wxID_OPEN    , "&Open..."   , wxEmptyString );
   filemenu    -> AppendSeparator   ();
   filemenu    -> Append            ( wxID_EXIT    , "E&xit"      , wxEmptyString );
   
   switchmenu  -> Append            ( IDM_SWITCH_FAST    , "&Fast"            , "Switch columns in \"FAST\" mode"                         , wxITEM_RADIO );
   switchmenu  -> Append            ( IDM_SWITCH_MERGE   , "&Merge"           , "Switch columns in \"MERGE\" mode"                        , wxITEM_RADIO );
   switchmenu  -> Append            ( IDM_SWITCH_ROLL    , "&Roll"            , "Switch columns in \"ROLL\" mode"                         , wxITEM_RADIO );
   
   tablemenu   -> Append            ( IDM_MULTISELECT    , "&Multi Select"    , "Allow Multi Select (selection of more than one record)"  , wxITEM_CHECK );
   tablemenu   -> Append            ( IDM_DRAGANDDROP    , "&Drag && Drop"    , "Allow Drag & Drop of the selected record(s)"             , wxITEM_CHECK );
   tablemenu   -> Append            ( IDM_GRIDHORIZONTAL , "Grid &Horizontal" , "Show Horizontal Grid Lines"                              , wxITEM_CHECK );
   tablemenu   -> Append            ( IDM_GRIDVERTICAL   , "Grid &Vertical"   , "Show Vertical Grid Lines"                                , wxITEM_CHECK );
   tablemenu   -> Append            ( IDM_CHECKBOXES     , "&CheckBoxes"      , "Show Check Boxes"                                        , wxITEM_CHECK );
   tablemenu   -> Append            ( IDM_FOCUSRECT      , "&FocusRect"       , "Show a focus rectangle around the wxTableCtrl"           , wxITEM_CHECK );
   tablemenu   -> AppendSubMenu     ( switchmenu         , "&Switch"          );
   tablemenu   -> Append            ( IDM_TOOLTIPS       , "&ToolTips"        , "Show Tooltips"                                           , wxITEM_CHECK );
   tablemenu   -> AppendSeparator   ();
   tablemenu   -> Append            ( IDM_NATIVEHEADER   , "&Native Header"   , "Native Header"                                           , wxITEM_CHECK );
   tablemenu   -> Append            ( IDM_COLUMNRESIZING , "Column &Resizing" , "Allow Column Resizing"                                   , wxITEM_CHECK );
   
   menubar     -> Append            ( filemenu           , "&File"            );
   menubar     -> Append            ( tablemenu          , "&Table"           );
   
   menubar     -> Check             ( IDM_COLUMNRESIZING , true );
   
   SetMenuBar  ( menubar );

   sizer    = new  wxBoxSizer ( wxVERTICAL );
   
   sizer    -> Add   ( tc  = new  wxTableCtrl   ( this )                                                                            , 1, wxALL | wxEXPAND, 0 );
   sizer    -> Add   ( log = new  wxTextCtrl    ( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE ) , 0, wxALL | wxEXPAND, 0 );
   
   tc    -> SetToolTip  ( true );
   tc    -> DragAndDrop ( true );
   
   log   -> SetMinSize  ( wxSize ( wxDefaultCoord, 64 ) );
   
   old   = wxLog :: SetActiveTarget ( new  wxLogTextCtrl ( log ) );
   
   SetSizer ( sizer );
}



ChildFrame :: ~ChildFrame ()
{
   delete  wxLog :: SetActiveTarget ( old );
   
   tc       -> Table          ( 0 );
   
   delete  t;
   
   table    -> Close          ();
   
   delete  record;
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
