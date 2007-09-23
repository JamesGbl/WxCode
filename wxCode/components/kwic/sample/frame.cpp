/////////////////////////////////////////////////////////////////////////////
// Name:        frame.cpp
// Purpose:     wxIndustrialControls test
// Author:      Ettl Martin - ettl@fs.wettzell.de
// Copyright:   (C)2007 Copyright Ettl Martin
// Licence:     KWIC License http://www.koansoftware.com/kwic/kwic-license.htm
/////////////////////////////////////////////////////////////////////////////

#include "frame.h"

#include <wx/textdlg.h>

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

// wxEvent Table 
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_QUIT			, MyFrame::OnQuit)
    EVT_MENU(ID_ABOUT			, MyFrame::OnAbout)
    EVT_BUTTON(ID_SWITCH_BUTTON , MyFrame::vOnButtonPressed)
	EVT_KWIC_ANG_REG(ID_ANGULAR_REGULATOR, MyFrame::vOnAngularRegulator)
	EVT_KWIC_LIN_REG(ID_SLIDER,MyFrame::vOnSlider)
END_EVENT_TABLE()


MyFrame::MyFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title)
{

	// set up the menu bar
	vSetUpMenuBar();

	// set up the status bar
	vSetUpStatusBar();
	
	// create a panel to place all the controls
	m_LCDPanel  	= new wxPanel(this
								, wxID_ANY
								, wxPoint(0,0)
								, wxSize(600,600)); 
	// set up the LCD Display
	vSetUpLCDDisplay();
	
	// set up a kwxslider
	vSetUpSlider();
	
	// set up angular meter									
	vSetUpAngularMeter();

	// set up the angular regulator
	vSetUpAngularRegulator();
	
	// set up the bitmap switcher
	vSetUpBmpSwitcher();


	this->SetSize(900,400);
							
}

void MyFrame::vSetUpBmpSwitcher(void)
{

	(void) new wxStaticBox(m_LCDPanel,wxID_ANY,_T("BmpSwitcher"),wxPoint(10,200),wxSize(230,100),wxSUNKEN_BORDER);
	m_BmpSwitcher = new kwxBmpSwitcher(m_LCDPanel
										, ID_BITMAP_SWITCHER
										, _T("Bitmap Switcher")
										, wxPoint(25,220)
										, wxSize(60,60)
										, wxSUNKEN_BORDER);

	m_SwitchButton = new wxButton(m_LCDPanel
										, ID_SWITCH_BUTTON
										, wxT("Change State")
										, wxPoint(110,230)
										, wxSize(120,40)
										, wxSUNKEN_BORDER);
	// create bitmaps
	m_GreenLight = new wxBitmap();
	m_RedLight	 = new wxBitmap();

#ifdef __VISUALC__
    // when running samples from MSVC, the working directory is not
    // the sample directory:
    const wxString prefix = wxT("../sample/");
#else
    const wxString prefix = wxEmptyString;
#endif
	m_GreenLight->LoadFile(prefix + "res/true.xpm",wxBITMAP_TYPE_XPM);
	m_RedLight->LoadFile(prefix + "res/false.xpm",wxBITMAP_TYPE_XPM);

	// add the bitmaps
	m_BmpSwitcher->AddBitmap(m_GreenLight);
	m_BmpSwitcher->AddBitmap(m_RedLight);

	return;
}


void MyFrame::vSetUpLCDDisplay(void)
{

	(void) new wxStaticBox(m_LCDPanel,wxID_ANY,wxT("LCD Display"),wxPoint(10,90),wxSize(140,100));

	//Topsizer->Add(m_LCDPanel);
	m_LCDDisplay = new kwxLCDDisplay(m_LCDPanel
									,wxPoint(25,120)
									,wxSize(100,50));

	// set the number of digits
	m_LCDDisplay->SetNumberDigits(4);
	
	// set a value
	m_LCDDisplay->SetValue(_T("1978"));
	
	
	// set the light colour
	m_LCDDisplay->SetLightColour(wxColor(*wxBLUE));

	// set the greyed colour
	m_LCDDisplay->SetGrayColour(wxColor(*wxBLACK));
}


void MyFrame::vSetUpSlider(void)
{

	// creat a static box, surrounding the the slider
	(void)new wxStaticBox(m_LCDPanel,wxID_ANY,wxT("Slider"),wxPoint(10,10),wxSize(230,70));


	// create the slider object
	m_Slider = new kwxLinearReg( m_LCDPanel
							, ID_SLIDER
							, 0 	// min
							, 100 	// max
							, wxPoint(25,30)
							, wxSize(200,40)
							,	 kwxRegulatorBase::STYLE_DRAW_CURRENT  
							   | kwxRegulatorBase::STYLE_SNAP_DOUBLE_CLK 
							   | kwxRegulatorBase::STYLE_EDITABLE		
							   | kwxRegulatorBase::STYLE_RECT_ON_EDIT);	
							
	// set the range value						
	m_Slider->SetRangeVal(0,100);
	
	// set the active bar colour
	m_ActiveBarColor = new wxColour(*wxBLUE);
	m_Slider->SetActiveBarColour(m_ActiveBarColor);
	
	// set needle colour
	m_NeedleColor = new wxColour(*wxBLACK);
	m_Slider->SetNeedleColour(m_NeedleColor);
	
	// set the passive bar color
	m_PassiveBarColor = new wxColour(*wxLIGHT_GREY);
	m_Slider->SetPassiveBarColour(m_PassiveBarColor);
	
	// set the txt limit colour
	m_TxtLimitColor = new wxColour(*wxRED);
	m_Slider->SetTxtLimitColour(m_TxtLimitColor);
	
	// set the border color
	m_BorderColor = new wxColour(*wxWHITE);
	m_Slider->SetBorderColour(m_BorderColor);
	
	// set the value colour
	m_TxtValueColor =  new wxColour(*wxWHITE);
	m_Slider->SetTxtValueColour(m_TxtValueColor);
	
	// set the tags colour
	m_TagsColor = new wxColour(*wxBLACK);
	m_Slider->SetTagsColour(m_TagsColor);
	
	//set the arrow
	m_Slider->vSetArrow(10,10);
	
	// set value
	m_Slider->SetValue(12);

	// set small steps
	m_Slider->vSetSmallStep(5);	
	
	// set tics
	m_Slider->vSetTagsCount(5);

	// set postfix string
	m_Slider->vSetPostfix(wxT("%"));
	
	// set point position
	m_Slider->vSetPointPosition(30);
	
	return;
}

void MyFrame::vSetUpAngularRegulator(void)
{



	// creat a static box, surrounding the the slider
	(void)new wxStaticBox(m_LCDPanel,wxID_ANY,wxT("Angular Regulator"),wxPoint(580,10),wxSize(240,240));

	m_AngularRegulator = new kwxAngularReg	(m_LCDPanel
												, ID_ANGULAR_REGULATOR
												,0,100
												, wxPoint(600,30)
												, wxSize(200,200)
												, kwxRegulatorBase::STYLE_DRAW_CURRENT
 												| kwxRegulatorBase::STYLE_SNAP_DOUBLE_CLK
 												| kwxRegulatorBase::STYLE_EDITABLE
 												| kwxRegulatorBase::STYLE_RECT_ON_EDIT 
 												);  
												

	// set the range
	m_AngularRegulator->vSetRange(0,200);
	
	// set the angle
	//m_AngularRegulator->SetAngle(0,360);
	
	// set a value
	m_AngularRegulator->SetValue(99);

	// set the outer circle colour
	m_ExtCircleColor = new wxColour(*wxLIGHT_GREY);
	m_AngularRegulator->SetExtCircleColour(m_ExtCircleColor);

	// set the inner circle colour
	m_InnerCircleColor= new wxColour(*wxBLUE);
	m_AngularRegulator->SetIntCircleColour(m_InnerCircleColor);
	
	// set the knob border colour
	m_KnobBorderColor = new wxColour(*wxWHITE);
	m_AngularRegulator->SetKnobBorderColour(m_KnobBorderColor);


	// set the knob border colour
	m_KnobColor = new wxColour(*wxBLACK);
	m_AngularRegulator->SetKnobColour(m_KnobColor);
	

	// set the limits colour
	m_LimitsColor = new wxColor(*wxRED);
	m_AngularRegulator->SetLimitsColour(m_LimitsColor);

	// set the tag colour
	m_TagColor = new wxColor(*wxWHITE);
	m_AngularRegulator->SetTagsColour(*m_TagColor);

	// set the number of tags
	m_AngularRegulator->vSetTagsCount(5);
	
	// set the posix element
	m_AngularRegulator->vSetPostfix("%");


	return;
				
}


void MyFrame::vSetUpAngularMeter(void)
{

	// create a static box, surrounding the the slider
	(void)new wxStaticBox(m_LCDPanel,wxID_ANY,wxT("Angular Meter"),wxPoint(280,10),wxSize(250,240));
	
	// create the angular regulator object	
	m_AngularMeter = new kwxAngularMeter(m_LCDPanel
									, ID_ANGULAR_METER
									, _T("Angular Meter")
									, wxPoint(305,30)
									, wxSize(200,200));
	

	// set the number of sectors
	m_AngularMeter->SetNumSectors(3);
	
	// set the number of tics
	m_AngularMeter->SetNumTick(3);
	
	// set the foreground colour
	m_AngularMeter->SetSectorColor(0,*wxBLUE);
	m_AngularMeter->SetSectorColor(1,*wxRED);	
	m_AngularMeter->SetSectorColor(2,*wxGREEN);	
	
	// set range
	m_AngularMeter->SetRange(0,100);
	
	// set angle
	m_AngularMeter->SetAngle(-20,200);
	
	// set the value
	m_AngularMeter->SetValue(80);
	
	// set the needle colour
	m_AngularMeter->SetNeedleColour(*wxWHITE);
	
	// set the background colour
	m_AngularMeter->SetBackColour(*wxLIGHT_GREY);
	
	// set the border colour
	m_AngularMeter->SetBorderColour(*wxWHITE);

}

MyFrame::~MyFrame()
{
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close();
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{

 
 	wxAboutDialogInfo AboutDialogInfo;
	AboutDialogInfo.AddDeveloper(wxT("Ettl Martin ettl@fs.wettzell.de\nLeidig Andreas leidig@fs.wettzell.de\nThis demo is based\non KWIC library project\n(http://www.koansoftware.com/kwic/index.htm)."));
	AboutDialogInfo.SetDescription(wxT("KWIC wxIndustrialControl Demo\nby\nEttl Martin\nLeidig Andreas"));
	AboutDialogInfo.SetName(wxT("KWIC | wxIndustrialControl"));
	AboutDialogInfo.SetVersion(wxT("v22.06.2007"));
 
 	wxAboutBox(AboutDialogInfo);

}




void MyFrame::vOnButtonPressed(wxCommandEvent &event)
{
	
	switch(event.GetId())
	{
		case ID_SWITCH_BUTTON:
		{

			if(m_BmpSwitcher->GetState() == 1)
				m_BmpSwitcher->IncState();
			else
				m_BmpSwitcher->SetState(1);
			break;
		}
		
	
	}
	
	return;
}


void MyFrame::vSetUpMenuBar(void)
{
#if wxUSE_MENUS
	
    m_menubar = new wxMenuBar();

	// set up the file menu
    wxMenu* m_fileMenu = new wxMenu(_T(""));
    m_fileMenu->Append(ID_QUIT, _("&Quit\tAlt-F4"), _("Quit the application"));
    m_menubar->Append(m_fileMenu, _("&File"));

	// set up the help menu
    wxMenu* m_helpMenu = new wxMenu(_T(""));
    m_helpMenu->Append(ID_ABOUT, _("&About\tF2"), _("Show info about this application"));
    m_menubar->Append(m_helpMenu, _("&Help"));

	// realize the menu( send it to the window)
    SetMenuBar(m_menubar);
	
#endif // wxUSE_MENUS


}

void MyFrame::vSetUpStatusBar(void)
{

	#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(_("Hello wxIndustrialControl user !"),0);
    SetStatusText(wxbuildinfo(short_f),1);
	#endif // wxUSE_STATUSBAR	

	return;		
}


void MyFrame::vOnAngularRegulator(wxCommandEvent &event)
{		
		
	switch(event.GetId())
	{
		case ID_ANGULAR_REGULATOR:
		{
			
			wxString value; 
			value.Printf(_T("%i"),m_AngularRegulator->iGetValue());
			m_LCDDisplay->SetValue(value);
			
			break;
		}
	
	}

	return;
}


void MyFrame::vOnSlider(wxCommandEvent &event)
{
	switch(event.GetId())
	{
		case ID_SLIDER:
		{
			//std::cout<< "\nSlider was active\n";
			m_AngularMeter->SetValue(m_Slider->iGetValue());
			break;
		}
	
	}
	
	return;

}

// *************************************************************************
// * END OF FILE (CVS Concurrent Version Control)
// * -----------------------------------------------------------------------
// * $RCSfile: $
// * $Revision: $
// *************************************************************************


