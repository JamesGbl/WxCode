////////////////////////////////////////////////////////////////////////////////
// Soli Deo Gloria!                                                           //
//                                                                            //
// Name:        MainFrame.cpp                                                 //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007                                                          //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#include <wx/wxprec.h>

#include "ChildFrame.hpp"
#include "MainFrame.hpp"



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



const long              MainFrame :: DEFAULTSTYLE  = wxDEFAULT_FRAME_STYLE;



BEGIN_EVENT_TABLE ( MainFrame , wxMDIParentFrame   )
   EVT_MENU ( wxID_OPEN , MainFrame :: OnMenuOpen  )
END_EVENT_TABLE   ()



void  MainFrame :: OnMenuOpen ( wxCommandEvent &  ce )
{
   wxFileDialog     dlg   ( this, "Open cTreePlus Database File", wxEmptyString, wxEmptyString, "cTreePlus File (*.dat)|*.dat", wxFD_OPEN | wxFD_FILE_MUST_EXIST ); 
   
   if ( dlg.ShowModal () != wxID_OK )
      return;
      
   ChildFrame *            child = new  ChildFrame ( this, wxID_ANY, dlg.GetPath () );
   
   child -> Open  ( session, dlg.GetPath () );   
   child -> Show  ();
}



MainFrame :: MainFrame ()
   : wxMDIParentFrame ( 0, wxID_ANY, "cTreePlusView", wxDefaultPosition, wxSize ( 600, 400 ) )
{
   SetIcon ( wxIcon ( "ICON" ) );
   
   wxMenuBar *    menubar     = new  wxMenuBar  ();
   wxMenu *       filemenu    = new  wxMenu     ();
   
   filemenu    -> Append            ( wxID_OPEN          , "&Open..."   , wxEmptyString );
   filemenu    -> AppendSeparator   ();
   filemenu    -> Append            ( wxID_EXIT          , "E&xit"      , wxEmptyString );
   
   menubar     -> Append            ( filemenu           , "&File"      );
   
   SetMenuBar  ( menubar );
   
   session  = new  Session ();
}



MainFrame :: ~MainFrame ()
{
   DestroyChildren   ();
   
   delete  session;
}
