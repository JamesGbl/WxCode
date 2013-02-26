///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 30 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUI_H__
#define __GUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameBase
///////////////////////////////////////////////////////////////////////////////
class MainFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menuBar;
		wxMenu* m_menuFile;
		wxStaticText* m_staticText2;
		wxStaticText* m_staticText4;
		wxSpinCtrl* m_spinCtrl1;
		wxTextCtrl* m_textCtrl2;
		wxStaticText* m_staticText21;
		wxStaticText* m_staticText41;
		wxSpinCtrl* m_spinCtrl11;
		wxTextCtrl* m_textCtrl21;
		wxStaticText* m_staticText22;
		wxStaticText* m_staticText42;
		wxSpinCtrl* m_spinCtrl12;
		wxTextCtrl* m_textCtrl22;
		wxStaticText* m_staticText23;
		wxStaticText* m_staticText43;
		wxSpinCtrl* m_spinCtrl13;
		wxTextCtrl* m_textCtrl23;
		wxButton* m_button2;
		wxButton* m_button1;
		wxStatusBar* m_statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseFrame( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnExitClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void DoChangeCount( wxCommandEvent& event ) { event.Skip(); }
		virtual void Print( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Tables Demo"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 373,418 ), long style = wxCLOSE_BOX|wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrameBase();
	
};

#endif //__GUI_H__
