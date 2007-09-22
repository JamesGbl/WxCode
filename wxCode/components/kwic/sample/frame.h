/////////////////////////////////////////////////////////////////////////////
// Name:        frame.h
// Purpose:     wxIndustrialControls test
// Author:      Ettl Martin - ettl@fs.wettzell.de
// Copyright:   (C)2007 Copyright Ettl Martin
// Licence:     KWIC License http://www.koansoftware.com/kwic/kwic-license.htm
/////////////////////////////////////////////////////////////////////////////

/*! \file
*   \brief This is the defintion of Class Frame. This class
*			is the 'main' frame of the wxIndustrialControl demo application.\n
*
******************************************************************/

#ifndef MAIN_H
#define MAIN_H

#include "app.h"

#include "kwic/slider.h"
#include "kwic/bmpswitcher.h"
#include "kwic/lcdwindow.h"
#include "kwic/angularmeter.h"
#include "kwic/angularregulator.h"

// wx Includes
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/notebook.h>
#include <wx/aboutdlg.h>

class MyFrame: public wxFrame
{
	public:

		MyFrame(wxFrame *frame, const wxString& title);
		~MyFrame();
	private:
		// plus einige EventHandler
		void OnQuit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		void vOnButtonPressed(wxCommandEvent &event);
		void vOnAngularRegulator(wxCommandEvent &event);
		void vOnSlider(wxCommandEvent &event);
		// Dieses Makro definiert den EventTable f�r MyFrame.

		void vSetUpMenuBar(void);
		void vSetUpStatusBar(void);
		void vSetUpAngularMeter(void);
		void vSetUpAngularRegulator(void);
		void vSetUpSlider(void);
		void vSetUpLCDDisplay(void);
		void vSetUpBmpSwitcher(void);
				
		wxBoxSizer  			*Topsizer;
		kwxLCDDisplay 			*m_LCDDisplay;
		kwxAngularReg 			*m_AngularRegulator;		
		kwxAngularMeter 		*m_AngularMeter;
	//	kwxSlider 				*m_Slider;
		kwxLinearReg			*m_Slider;
		kwxBmpSwitcher			*m_BmpSwitcher;
		
		wxNotebook  *m_Notebook;
		wxPanel 	*m_SliderPanel;
		wxPanel 	*m_LCDPanel;
		wxPanel 	*m_AngularRegulatorPanel;
		wxPanel 	*m_AngularMeterPanel;
		
		// define color settings for the angular regulator
		wxColour 	*m_ExtCircleColor;
		wxColour 	*m_InnerCircleColor;
		wxColour 	*m_KnobBorderColor;
		wxColour	*m_KnobColor;
		wxColour 	*m_LimitsColor;
		wxColour	*m_TagColor;
		
		/// colour settings for the slider
		wxColour *m_ActiveBarColor;
		wxColour *m_NeedleColor;	
		wxColour *m_PassiveBarColor;
		wxColour *m_BorderColor;
		wxColour *m_TxtLimitColor;
		wxColour *m_TxtValueColor;
		wxColour *m_TagsColor;
		
		// BmpSwitcher stuff 
		wxBitmap *m_GreenLight;
		wxBitmap *m_RedLight;
		wxButton *m_SwitchButton;
		
		DECLARE_EVENT_TABLE()


		// menu bar stuff
		wxMenuBar	*m_menubar;
		wxMenu	 	*m_fileMenu;
		wxMenu		*m_helpMenu;		
		wxColor m_Color;
};
enum
{
	 ID_ANGULAR_METER = wxID_HIGHEST
	,ID_ANGULAR_REGULATOR
	,ID_SLIDER
	,ID_LCDDISPLAY
	,ID_BITMAP_SWITCHER
	,ID_SWITCH_BUTTON
	,ID_ABOUT
	,ID_QUIT
};

#endif // MAIN_H
