/***************************************************************
 * Name:      stringformatterframe.h
 * Purpose:   Code for Application Class
 * Author:    Nathan Shaffer
 * Created:   2013-01-27
 * Copyright: Nathan Shaffer
 * License:   wxWindows licence
 **************************************************************/
#ifndef __StringFormatterFrame__
#define __StringFormatterFrame__

#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/frame.h>
#include "wx/stringformatter.h"
#include <wx/string.h>
#include <wx/tokenzr.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class StringFormatterFrame
///////////////////////////////////////////////////////////////////////////////
class StringFormatterFrame : public wxFrame
{
	private:
		wxStringFormatter m_sf;

	protected:
		wxTextCtrl* m_textCtrl3;
		wxStaticText* m_staticText6;
		wxTextCtrl* m_textCtrl1;
		wxButton* m_formatBtn;
		wxStaticText* m_staticText1;

		void OnFormatClick( wxCommandEvent& event );


	public:

		StringFormatterFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~StringFormatterFrame();

};


class capitalizeFunc : public wxFormatFunction
{
	public:
		capitalizeFunc() {}
		~capitalizeFunc() {}
		wxString Parse(wxString input)
		{
			return input.MakeUpper();
		}
};


#endif // __StringFormatterFrame__
