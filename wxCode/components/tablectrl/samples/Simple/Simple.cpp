////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        Simple.cpp                                                    //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007/10/07                                                    //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#include <wx/wxprec.h>

#include <wx/TableCtrl.hpp>

#include "TableMenu.h"
#include "FakeTable.hpp"



////////////////////////////////////////////////////////////////////////////////
// MainFrame                                                                  //
////////////////////////////////////////////////////////////////////////////////



class  MainFrame : public  wxFrame
{
   private   :
      typedef wxFrame   super;
      
   private   :
      DECLARE_EVENT_TABLE  ()
      
   private   :
      wxSizer *         sizer;
      wxTable *         table;
      wxTableCtrl *     tablectrl;
      wxTable :: DefaultSet
                        select;
      wxTable :: DefaultSet
                        check;
      
   private   :
      void              OnMenu      ( wxCommandEvent & );
      
   public    :
      MainFrame   ();
      ~MainFrame  ();
};


#if 0
static const wxWindowID    IDM_BEGIN            = 1000;

static const wxWindowID    IDM_MULTISELECT      = 1000;
static const wxWindowID    IDM_DRAGANDDROP      = 1001;
static const wxWindowID    IDM_GRIDHORIZONTAL   = 1002;
static const wxWindowID    IDM_GRIDVERTICAL     = 1003;
static const wxWindowID    IDM_CHECKBOXES       = 1004;
static const wxWindowID    IDM_FOCUSRECT        = 1005;
static const wxWindowID    IDM_TOOLTIPS         = 1006;

static const wxWindowID    IDM_NATIVEHEADER     = 1007;

static const wxWindowID    IDM_SWITCH           = 1011;
static const wxWindowID    IDM_SWITCH_FAST      = IDM_SWITCH + wxTableCtrl :: Body :: Mode_FAST;
static const wxWindowID    IDM_SWITCH_MERGE     = IDM_SWITCH + wxTableCtrl :: Body :: Mode_MERGE;
static const wxWindowID    IDM_SWITCH_ROLL      = IDM_SWITCH + wxTableCtrl :: Body :: Mode_ROLL;

static const wxWindowID    IDM_END              = 2000;
#endif


BEGIN_EVENT_TABLE ( MainFrame , wxFrame   )
   EVT_MENU_RANGE ( IDM_BEGIN , 
                    IDM_END   , MainFrame :: OnMenu   )
END_EVENT_TABLE   ()



void  MainFrame :: OnMenu ( wxCommandEvent &  ce )
{
   switch ( ce.GetId () )
   {
      case IDM_MULTISELECT       :
         tablectrl -> MultiSelect      ( ce.IsChecked () );
         tablectrl -> RefreshLayout    ();
         
         break;

      case IDM_DRAGANDDROP       :
         tablectrl -> DragAndDrop      ( ce.IsChecked () );
         tablectrl -> RefreshLayout    ();
         
         break;

      case IDM_GRIDHORIZONTAL    :
         tablectrl -> GridHorizontal   ( ce.IsChecked () );
         tablectrl -> RefreshLayout    ();
         
         break;

      case IDM_GRIDVERTICAL      :
         tablectrl -> GridVertical     ( ce.IsChecked () );
         tablectrl -> RefreshLayout    ();
         
         break;

      case IDM_CHECKBOXES        :
         tablectrl -> CheckBoxes       ( ce.IsChecked () );
         tablectrl -> RefreshLayout    ();
         
         break;

      case IDM_FOCUSRECT         :
         tablectrl -> FocusRect        ( ce.IsChecked () );
         tablectrl -> RefreshLayout    ();
         
         break;

      case IDM_TOOLTIPS          :
         tablectrl -> SetToolTip       ( ce.IsChecked () );
         tablectrl -> RefreshLayout    ();
         
         break;

      case IDM_NATIVEHEADER      :
         tablectrl -> SetNativeHeader  ( ce.IsChecked () );
         tablectrl -> RefreshLayout    ();
         
         break;

      case IDM_SWITCH_FAST       :
      case IDM_SWITCH_MERGE      :
      case IDM_SWITCH_ROLL       :
         tablectrl -> SetMode          ( static_cast < wxTableCtrl :: Body :: Mode > ( ce.GetId () - IDM_SWITCH ) );
         
         break;
   }
}



MainFrame :: MainFrame ()
   : wxFrame ( 0, wxID_ANY, "Simple" )
{
   SetIcon ( wxIcon ( "ICON" ) );
   
   wxMenuBar *    menubar     = new  wxMenuBar  ();
   wxMenu *       filemenu    = new  wxMenu     ();
   wxMenu *       tablemenu   = new  wxMenu     ();
   wxMenu *       switchmenu  = new  wxMenu     ();
   
   filemenu    -> Append            ( wxID_EXIT          , "E&xit"            , wxEmptyString );

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
   
   menubar     -> Append            ( filemenu           , "&File"            );
   menubar     -> Append            ( tablemenu          , "&Table"           );
   
   SetMenuBar     ( menubar );
   SetStatusBar   ( new  wxStatusBar   ( this ) );
   
   sizer = new  wxBoxSizer ( wxVERTICAL );
   
   sizer -> Add   ( tablectrl = new  wxTableCtrl   ( this ), 1, wxEXPAND | wxALL, 0 );
   
   table = new  FakeTable  ( 1000, 10 );
   
   tablectrl   -> Table       ( table );
   tablectrl   -> SetToolTip  ( true );
   
   menubar     -> Check       ( IDM_SWITCH + tablectrl -> GetMode (), true );
   
   table -> Select ().Use  ( &select );
   table -> Check  ().Use  ( &check  );
   
   SetSizer ( sizer );
}



MainFrame :: ~MainFrame ()
{
   tablectrl -> Table   ( 0 );
   
   delete  table;
}



////////////////////////////////////////////////////////////////////////////////
// MainFrame                                                                  //
////////////////////////////////////////////////////////////////////////////////



class  Simple : public  wxApp
{
   private   :
      typedef wxApp     super;
   
   private   :
      MainFrame *       mainframe;
         
   public    :
      bool              OnInit   ();
};



DECLARE_APP    ( Simple )



IMPLEMENT_APP  ( Simple )



bool  Simple :: OnInit ()
{
   mainframe   = new  MainFrame ();
   
   mainframe   -> Show  ();
   
   return ( true );
}
