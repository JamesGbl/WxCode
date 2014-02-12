/***************************************************************
 * Name:      stringformatterframe.cpp
 * Purpose:   Code for Application Class
 * Author:    Nathan Shaffer
 * Created:   2013-01-27
 * Copyright: Nathan Shaffer
 * License:   wxWindows licence
 **************************************************************/
#include "stringformatterframe.h"

StringFormatterFrame::StringFormatterFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{

    wxLogWindow* logwin;
    logwin = new wxLogWindow(this, "log", true, true);
    logwin->Show();

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );

	m_textCtrl3 = new wxTextCtrl( this, wxID_ANY, wxT("a=apple\nb=banana"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	bSizer2->Add( m_textCtrl3, 0, wxALL|wxEXPAND, 5 );

	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Enter symbols to the right ex: a=apple    >>> $a will expand to apple.\nuse the function CAPITALIZE() to capitalize strings.\nenter format strings in the text box below."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer2->Add( m_staticText6, 0, wxALL, 5 );

	bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );

	m_textCtrl1 = new wxTextCtrl( this, wxID_ANY, wxT("CAPITALIZE($a)"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_textCtrl1, 0, wxALL|wxEXPAND, 5 );

	m_formatBtn = new wxButton( this, wxID_ANY, wxT("format"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_formatBtn, 0, wxALL, 5 );

	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer1->Add( m_staticText1, 1, wxALL|wxEXPAND, 5 );

	this->SetSizer( bSizer1 );
	this->Layout();

	// Connect Events
	m_formatBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StringFormatterFrame::OnFormatClick ), NULL, this );
	m_sf.AddFormatFunction("CAPITALIZE", new capitalizeFunc());
}

StringFormatterFrame::~StringFormatterFrame()
{
	// Disconnect Events
	m_formatBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StringFormatterFrame::OnFormatClick ), NULL, this );
}

void StringFormatterFrame::OnFormatClick( wxCommandEvent& event )
{
	wxArrayString symbols = wxStringTokenize(m_textCtrl3->GetValue(), wxDEFAULT_DELIMITERS + wxString("="));
	int index = 0;
	while(index * 2 < symbols.GetCount())
	{

		m_sf.AddString(symbols.Item(index * 2), symbols.Item((index *2) +1));
		index++;
	}
	m_staticText1->SetLabel(m_sf.Parse(m_textCtrl1->GetValue()));
}
