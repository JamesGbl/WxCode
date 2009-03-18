/***************************************************************
 * Name:      sampel.cpp
 * Purpose:   Sampel-code for the use of wxLEDPanel
 * Author:    Christian Gräfe (info@mcs-soft.de)
 * Created:   2007-03-16
 * Copyright: Christian Gräfe (www.mcs-soft.de)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP //
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "sample.h"

IMPLEMENT_APP(ledpanelApp);

#include "smile.xpm"

bool ledpanelApp::OnInit()
{
	ledpanelFrame* frame = new ledpanelFrame(NULL, wxT("wxLEDPanel Sampel"));
	frame->Show();

	return true;
}

ledpanelFrame::ledpanelFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title)
{
	// The sizer
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	// The first LEDPanel -> Static text
	wxLEDPanel* lp1=new wxLEDPanel(this,wxID_ANY,wxSize(4,4),wxSize(63,9),0);
	lp1->SetText(wxT("wxLEDPanel"),wxALIGN_CENTER);	// Set the LED-Text
	sizer->Add(lp1,0,wxALIGN_CENTER|wxALL,5);

	// The 2nd LEDPanel
	wxLEDPanel* lp2=new wxLEDPanel(this,wxID_ANY,wxSize(4,4),wxSize(63,9),0);
	lp2->SetContentAlign(wxALIGN_CENTER_VERTICAL);	// Center the text vertical
	lp2->SetLEDColour(wxLED_COLOUR_GREEN);          // Set the LEDColour (default colour is red)
	lp2->SetScrollDirection(wxLEFT);	            // Set the Scrolldirection
	lp2->SetScrollSpeed(80);                        // Set the Scrollspeed
	lp2->SetText(wxT("wxLEDPanel"));
	sizer->Add(lp2,0,wxALIGN_CENTER|wxALL,5);

	// The 3rd LEDPanel
	wxLEDPanel* lp3=new wxLEDPanel(this,wxID_ANY,wxSize(4,4),wxSize(63,9),0);
	lp3->SetContentAlign(wxALIGN_CENTER_HORIZONTAL);    // Center the text horizontal
	lp3->SetLEDColour(wxLED_COLOUR_BLUE);               // Set the LEDColour (default colour is red)
	lp3->SetScrollDirection(wxUP);          	        // Set the Scrolldirection
	lp3->SetScrollSpeed(120);                           // Set the Scrollspeed
	lp3->SetText(wxT("wxLEDPanel\n2nd Line\n3rd Line\n4th Line"),wxALIGN_RIGHT);  // multi-line text
	sizer->Add(lp3,0,wxALIGN_CENTER|wxALL,5);

	// The 4th LEDPanel
	// Bigger LEDs and space (1px) between the leds
	wxLEDPanel* lp4=new wxLEDPanel(this,wxID_ANY,wxSize(5,5),wxSize(63,9),1);
	lp4->SetText(wxT("wxLEDPanel"),wxALIGN_CENTER);
	lp4->SetLEDColour(wxLED_COLOUR_CYAN);           // Set the LEDColour (default colour is red)
	sizer->Add(lp4,0,wxALIGN_CENTER|wxALL,5);

	// The 5th LEDPanel -> Static text, invertet
	wxLEDPanel* lp5=new wxLEDPanel(this,wxID_ANY,wxSize(4,4),wxSize(76,9),0);
	lp5->SetLetterSpace(2);                         // more space between the letters
	lp5->ShowInvertet(true);                        // Draw the LEDs invertet
	lp5->SetText(wxT("wxLEDPanel"),wxALIGN_CENTER);	// Set the LED-Text
	sizer->Add(lp5,0,wxALIGN_CENTER|wxALL,5);

	// The 6th LEDPanel
	wxLEDPanel* lp6=new wxLEDPanel(this,wxID_ANY,wxSize(4,4),wxSize(76,9),0);
	lp6->SetLEDColour(wxLED_COLOUR_GREEN);
	lp6->SetScrollDirection(wxRIGHT);	            // Set the Scrolldirection
	lp6->SetScrollSpeed(80);                        // Set the Scrollspeed
	lp6->ShowInactivLEDs(false);                    // Don't draw inactiv LEDs
	lp6->SetFontType(wxLEDFont7x7);                	// use a bigger font with 7x7 Letters
	lp6->SetText(wxT("wxLEDPanel"),wxALIGN_CENTER);	// Set the LED-Text
	sizer->Add(lp6,0,wxALIGN_CENTER|wxALL,5);

	// The 7th LEDPanel
	wxLEDPanel* lp7=new wxLEDPanel(this,wxID_ANY,wxSize(4,4),wxSize(76,9),0);
	lp7->SetLEDColour(wxLED_COLOUR_BLUE);
	lp7->SetScrollDirection(wxDOWN);        	    // Set the Scrolldirection
	lp7->SetScrollSpeed(120);                       // Set the Scrollspeed
	lp7->ShowInactivLEDs(false);                    // Don't draw inactiv LEDs
	lp7->ShowInvertet(true);                        // Draw the LEDs invertet
	lp7->SetFontType(wxLEDFont7x7);                	// use a bigger font with 7x7 Letters
	lp7->SetText(wxT("wxLEDPanel\n2nd Line\n3rd Line\n4th Line"),wxALIGN_RIGHT);	// Set the LED-Text
	sizer->Add(lp7,0,wxALIGN_CENTER|wxALL,5);

	// The 8th LEDPanel
	wxLEDPanel* lp8=new wxLEDPanel(this,wxID_ANY,wxSize(4,4),wxSize(58,58),0);
	lp8->SetLEDColour(wxLED_COLOUR_YELLOW);
	lp8->SetContentPaddingLeft(0);
	lp8->ShowInactivLEDs(false);                    // Don't draw inactiv LEDs
	lp8->SetImage(wxImage(smile_xpm));
	lp8->SetScrollSpeed(80);                       // Set the Scrollspeed
	lp8->SetScrollDirection(wxUP);        	    // Set the Scrolldirection
	sizer->Add(lp8,0,wxALIGN_CENTER|wxALL,5);

	// The 9th LEDPanel
	wxLEDPanel* lp9=new wxLEDPanel(this,wxID_ANY,wxSize(5,5),wxSize(17,17),0);
	lp9->SetContentAlign(wxALIGN_CENTER);    // Center
	wxAnimation ani;
	ani.LoadFile(wxT("../samples/animation.gif"),wxANIMATION_TYPE_GIF);
	lp9->SetAnimation(ani);
	sizer->Add(lp9,0,wxALIGN_CENTER|wxALL,5);

	// Sizer for the Panel
	this->SetSizerAndFit(sizer);
}


ledpanelFrame::~ledpanelFrame()
{
}



