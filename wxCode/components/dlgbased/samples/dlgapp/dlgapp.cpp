/////////////////////////////////////////////////////////////////////////////
// Name:        dlgapp.cpp
// Purpose:     Dialog based wxWindows sample
// Author:      Marco Ghislanzoni
// Modified by:
// Created:     10/31/1999
// RCS-ID:
// Copyright:   (c) Marco Ghislanzoni
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "dlgapp.cpp"
    #pragma interface "dlgapp.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// For all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

// ----------------------------------------------------------------------------
// ressources
// ----------------------------------------------------------------------------
// The application icon
// Note: if __WIN95__ is defined the application icon appears in the
//       dialog title bar and on the taskbar (loaded from resources)
#if defined(__WXGTK__) || defined(__WXMOTIF__)
    #include "mondrian.xpm"
#endif

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define the application class
class MyApp : public wxApp
{
public:
    // Init method
    virtual bool OnInit();
};

// Define a new dialog type: this is going to be our main window
class MyDialog : public wxDialog
{
public:
    // Constructor(s)
    MyDialog(const wxString& title, const wxPoint& pos, const wxSize& size);

    // Event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnCelToFahr(wxCommandEvent& event);
	void OnFahrToCel(wxCommandEvent& event);

	// Set icon (from wxFrame source code)
    virtual void SetIcon(const wxIcon& icon);

private:
	// Celsius and ferhenheit text controls. Needed for data retrivial
	// inside member functions
	wxTextCtrl *celDegree;
	wxTextCtrl *fahrDegree;

	// Dialog icon
	wxIcon m_icon;

    // Any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls in the dialog
enum
{
    // command buttons
    Dlgapp_CelToFahr = 1,
	Dlgapp_FahrToCel,
    Dlgapp_About,
	Dlgapp_Close,
    
    // other controls
    Dlgapp_CelDegree = 1000,
	Dlgapp_FahrDegree,
	Dlgapp_CelText,
	Dlgapp_FahrText,
	Dlgapp_StaticBox
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyDialog, wxDialog)
	EVT_BUTTON(Dlgapp_CelToFahr, MyDialog::OnCelToFahr)
	EVT_BUTTON(Dlgapp_FahrToCel, MyDialog::OnFahrToCel)
	EVT_BUTTON(Dlgapp_About, MyDialog::OnAbout)
	EVT_BUTTON(Dlgapp_Close, MyDialog::OnQuit)
	// We have to implement this to force closing
	// the dialog when the 'x' button is pressed
	EVT_CLOSE(              MyDialog::OnQuit)
END_EVENT_TABLE()

IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

bool MyApp::OnInit()
{
    // Create the main application window (a dialog in this case)
	// NOTE: Vertical dimension comprises the caption bar.
	//       Horizontal dimension has to take into account the thin
	//       hilighting border around the dialog (2 points in
	//       Win 95).
    MyDialog *dialog = new MyDialog(wxT("Temp Converter"),
                                 wxPoint(0, 0), wxSize(147, 162));
	
	// Center the dialog when first shown
	dialog->Centre();

    // Show it and tell the application that it's our main window
    dialog->Show(TRUE);
    SetTopWindow(dialog);

    return TRUE;
}

// ----------------------------------------------------------------------------
// main dialog
// ----------------------------------------------------------------------------

// Dialog constructor
MyDialog::MyDialog(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxDialog((wxDialog *)NULL, -1, title, pos, size)
{
    // Set the dialog icon
	SetIcon(wxICON(mondrian));

	// Create and position controls in the dialog

	// Create static box to enclose text and conversion buttons
	wxStaticBox *statBox = new wxStaticBox(this, Dlgapp_StaticBox,
		                                   wxString(wxT("Convert")),
										   wxPoint(5,0), wxSize(130, 100));

	// Static text celsius
	wxStaticText *celText = new wxStaticText(this, Dlgapp_CelText, wxString(wxT("Celsius:")), wxPoint(10,15));

	// Text control
	celDegree = new wxTextCtrl(this, Dlgapp_CelDegree, wxString(wxT("")), wxPoint(10,30), wxSize(50,20));

	// Static text fahrenheit
	wxStaticText *fahrText = new wxStaticText(this, Dlgapp_FahrText, wxString(wxT("Fahrenheit:")), wxPoint(10,55));

	// Another text control
	fahrDegree = new wxTextCtrl(this, Dlgapp_FahrDegree, wxString(wxT("")), wxPoint(10,70), wxSize(50,20));
	
	// Four command buttons.
	wxButton *btnCelToFahr = new wxButton( this, Dlgapp_CelToFahr, wxT("C -> F"), wxPoint(80,30), wxSize(50,20));
	wxButton *btnFahrToCel = new wxButton( this, Dlgapp_FahrToCel, wxT("F -> C"), wxPoint(80,70), wxSize(50,20));
	wxButton *btnAbout     = new wxButton( this, Dlgapp_About, wxT("About"), wxPoint(10,110), wxSize(50,20));
	wxButton *btnClose     = new wxButton( this, Dlgapp_Close, wxT("Close"), wxPoint(80,110), wxSize(50,20));

	// no default button
}

// Originally a wxDialog doesn't have any method to set the
// window associated icon since this has been implemented in 
// wxFrame only.
// But in a dialog based app we want to associate an icon
// to the main window (i.e. the dialog)
// This code is the same of wxFrame::SetIcon.
void MyDialog::SetIcon(const wxIcon& icon)
{
  m_icon = icon;
#ifdef __WIN95__
  if ( m_icon.Ok() )
    SendMessage((HWND) GetHWND(), WM_SETICON,
                (WPARAM)TRUE, (LPARAM)(HICON) m_icon.GetHICON());
#endif
}


// event handlers

void MyDialog::OnCelToFahr(wxCommandEvent& WXUNUSED(event))
{
	// Calc celsius to fahrenheit conversion

	// Retrieve control content
	wxString celStr = celDegree->GetValue();
	
	// Check the control is not empty
	// Note: we cannot use wxString.IsNumber() to check the
	//       validity of the entry because this function
	//       just checks for int numbers
	if (celStr.IsEmpty())
	{
		wxMessageBox(wxT("I don't see any valid\n")
			         wxT("Celsius entry!"), wxT("Error!"),
                      wxOK | wxICON_EXCLAMATION | wxCENTRE);
		return;
	}

	// Convert string into number
	float celValue = (float) strtod(celStr.c_str(),NULL);
	// F = ( C * 180/100 ) + 32
	float fahrValue = (celValue * 9.0f / 5.0f ) + 32.0f ;
	// Convert temperature to a string
	wxString fahrStr;
	fahrStr.Printf(wxT("%5.2f"),fahrValue);
	// Update text control
	fahrDegree->SetValue(fahrStr);
	return;
}

void MyDialog::OnFahrToCel(wxCommandEvent& WXUNUSED(event))
{
	// Calc fahrenheit to celsius conversion

	// Retrieve control content
	wxString fahrStr = fahrDegree->GetValue();
	
	// Check the control is not empty
	if (fahrStr.IsEmpty())
	{
		wxMessageBox(wxT("I don't see any valid\n")
			         wxT("Fahrenheit entry!"), wxT("Error!"),
                      wxOK | wxICON_EXCLAMATION | wxCENTRE);
		return;
	}

	// Convert string into number
	float fahrValue = (float) strtod(fahrStr.c_str(),NULL);
	// C = ( F - 32 ) * 100/180
	float celValue = (fahrValue - 32.0f) * 5.0f / 9.0f ;
	// Convert temperature to a string
	wxString celStr;
	celStr.Printf(wxT("%5.2f"),celValue);
	// Update text control
	celDegree->SetValue(celStr);
	return;
}


void MyDialog::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	// NOTE Since our main window is a dialog and not
	// ---- a frame we have to close it using Destroy
	//      rather than Close. In fact Close doesn't
	//      actually close a dialog but just hides it
	//      so that the application will hang there
	//      with his only window hidden and thus unable
	//      to get any user input.
    
	// --> Don't use Close with a wxDialog,
	//     use Destry instead.
    Destroy();
}

void MyDialog::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf(wxT("Thanks for using Temp Converter!\n")
		       wxT("(C) 1999 by Marco Ghislanzoni\n")
               wxT("Written using %s")
#ifdef wxBETA_NUMBER
               wxT(" (beta %d)")
#endif // wxBETA_NUMBER
               , wxVERSION_STRING
#ifdef wxBETA_NUMBER
               , wxBETA_NUMBER
#endif // wxBETA_NUMBER
              );

    wxMessageBox(msg, wxT("Dialog App"), wxOK | wxICON_INFORMATION, this);
}
