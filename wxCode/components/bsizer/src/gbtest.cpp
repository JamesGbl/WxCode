/*
 * File:	gbtest.cpp
 * Purpose:	wxGridBagSizer test
 * Author:	Alex Andruschak
 * Created:	2000
 * Updated:	
 * Copyright:   (c) 2000, Deka-Soft
 */

static const char sccsid[] = "%W% %G%";

#ifdef __GNUG__
#pragma implementation
#pragma interface
#endif

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/gbsizer.h"

class MyApp: public wxApp
{ public:
    bool OnInit(void);
};


class MyFrame: public wxFrame
{ 

public:
    MyFrame(wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnCloseWindow(wxCloseEvent& event);

DECLARE_EVENT_TABLE()
};

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit(void)
{
  MyFrame *frame = new MyFrame((wxFrame *) NULL, (char *) "wxGridBagSizer Sample", wxPoint(50, 50), wxSize(600, 500));

  frame->Show(TRUE);

  SetTopWindow(frame);
  return TRUE;
}

MyFrame::MyFrame(wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size):
  wxFrame(frame, -1, title, pos, size, wxDEFAULT_FRAME_STYLE | wxVSCROLL | wxHSCROLL)
{
  // create sizer 4 columns 5 rows
  wxGridBagSizer *sizer = new wxGridBagSizer(wxSize(4,5));

  // add first row
  sizer->Add( 
    new wxButton( this, -1, "Button 1" ), 
    0, 0, 0, new wxGridBagItemHandle(0,0,1,1));   
  sizer->Add( 
    new wxButton( this, -1, "Button 2" ), 
    0, 0, 0, new wxGridBagItemHandle(1,0,1,1));   
  sizer->Add( 
    new wxButton( this, -1, "Button 3" ), 
    0, 0, 0, new wxGridBagItemHandle(2,0,1,1));   
  sizer->Add( 
    new wxButton( this, -1, "Button 4" ), 
    0, 0, 0, new wxGridBagItemHandle(3,0,1,1));   

  // add button for secord row
  sizer->Add( 
    new wxButton( this, -1, "Button 5" ), 
    0, 0, 0, new wxGridBagItemHandle(0,1,4,1));   

  // add buttons for next row
  sizer->Add( 
    new wxButton( this, -1, "Button 6" ), 
    0, 0, 0, new wxGridBagItemHandle(0,2,3,1));   
  sizer->Add( 
    new wxButton( this, -1, "Button 7" ), 
    0, 0, 0, new wxGridBagItemHandle(3,2,1,1));   

  // and last additions
  sizer->Add( 
    new wxButton( this, -1, "Button 8" ), 
    0, 0, 0, new wxGridBagItemHandle(0,3,1,2));   
  sizer->Add( 
    new wxButton( this, -1, "Button 9" ), 
    0, 0, 0, new wxGridBagItemHandle(1,3,3,1));   
  sizer->Add( 
    new wxButton( this, -1, "Button 10" ), 
    0, 0, 0, new wxGridBagItemHandle(1,4,3,1));   

  SetAutoLayout( TRUE );  
  SetSizer( sizer );    
}

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
  EVT_CLOSE(MyFrame::OnCloseWindow)
END_EVENT_TABLE()

void MyFrame::OnCloseWindow(wxCloseEvent& event)
{
  Destroy();
}



