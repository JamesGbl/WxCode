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

#include "FakeTable.hpp"



////////////////////////////////////////////////////////////////////////////////
// MainFrame                                                                  //
////////////////////////////////////////////////////////////////////////////////



class  MainFrame : public  wxFrame
{
   private   :
      typedef wxFrame   super;
      
   private   :
      wxSizer *         sizer;
      wxTable *         table;
      wxTableCtrl *     tablectrl;
      
   public    :
      MainFrame   ();
};



MainFrame :: MainFrame ()
   : wxFrame ( 0, wxID_ANY, "Simple" )
{
   sizer = new  wxBoxSizer ( wxVERTICAL );
   
   sizer -> Add   ( tablectrl = new  wxTableCtrl   ( this ), 1, wxEXPAND | wxALL, 0 );
   
   table = new  FakeTable  ( 1000, 10 );
   
   tablectrl   -> Table       ( table );
   tablectrl   -> SetToolTip  ( true );
   
   SetSizer ( sizer );
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
