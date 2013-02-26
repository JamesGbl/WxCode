///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 30 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui.h"

///////////////////////////////////////////////////////////////////////////

MainFrameBase::MainFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_menuBar = new wxMenuBar( 0 );
	m_menuFile = new wxMenu();
	wxMenuItem* menuFileExit;
	menuFileExit = new wxMenuItem( m_menuFile, wxID_EXIT, wxString( _("E&xit") ) + wxT('\t') + wxT("Alt+X"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( menuFileExit );
	
	m_menuBar->Append( m_menuFile, _("&File") ); 
	
	this->SetMenuBar( m_menuBar );
	
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Column 1") ), wxHORIZONTAL );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 2, 2, 0, 0 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, _("Size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	gSizer1->Add( m_staticText2, 0, wxALL|wxALIGN_BOTTOM, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, _("Cells text"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	gSizer1->Add( m_staticText4, 0, wxALL|wxALIGN_BOTTOM, 5 );
	
	m_spinCtrl1 = new wxSpinCtrl( this, wxID_ANY, wxT("100"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 100 );
	gSizer1->Add( m_spinCtrl1, 1, wxALL|wxEXPAND, 5 );
	
	m_textCtrl2 = new wxTextCtrl( this, wxID_ANY, _("Cell1"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_textCtrl2, 1, wxALL|wxEXPAND, 5 );
	
	sbSizer1->Add( gSizer1, 1, wxEXPAND, 5 );
	
	mainSizer->Add( sbSizer1, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Column 2") ), wxVERTICAL );
	
	wxGridSizer* gSizer11;
	gSizer11 = new wxGridSizer( 2, 2, 0, 0 );
	
	m_staticText21 = new wxStaticText( this, wxID_ANY, _("Size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	gSizer11->Add( m_staticText21, 0, wxALL|wxALIGN_BOTTOM, 5 );
	
	m_staticText41 = new wxStaticText( this, wxID_ANY, _("Cells text"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41->Wrap( -1 );
	gSizer11->Add( m_staticText41, 0, wxALL|wxALIGN_BOTTOM, 5 );
	
	m_spinCtrl11 = new wxSpinCtrl( this, wxID_ANY, wxT("100"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 100 );
	gSizer11->Add( m_spinCtrl11, 1, wxALL|wxEXPAND, 5 );
	
	m_textCtrl21 = new wxTextCtrl( this, wxID_ANY, _("Cell2"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer11->Add( m_textCtrl21, 1, wxALL|wxEXPAND, 5 );
	
	sbSizer3->Add( gSizer11, 1, wxEXPAND, 5 );
	
	mainSizer->Add( sbSizer3, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Column 3") ), wxVERTICAL );
	
	wxGridSizer* gSizer12;
	gSizer12 = new wxGridSizer( 2, 2, 0, 0 );
	
	m_staticText22 = new wxStaticText( this, wxID_ANY, _("Size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText22->Wrap( -1 );
	gSizer12->Add( m_staticText22, 0, wxALL|wxALIGN_BOTTOM, 5 );
	
	m_staticText42 = new wxStaticText( this, wxID_ANY, _("Cells text"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText42->Wrap( -1 );
	gSizer12->Add( m_staticText42, 0, wxALL|wxALIGN_BOTTOM, 5 );
	
	m_spinCtrl12 = new wxSpinCtrl( this, wxID_ANY, wxT("100"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 100 );
	gSizer12->Add( m_spinCtrl12, 1, wxALL|wxEXPAND, 5 );
	
	m_textCtrl22 = new wxTextCtrl( this, wxID_ANY, _("Cell3"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer12->Add( m_textCtrl22, 1, wxALL|wxEXPAND, 5 );
	
	sbSizer4->Add( gSizer12, 1, wxEXPAND, 5 );
	
	mainSizer->Add( sbSizer4, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Column 4") ), wxVERTICAL );
	
	wxGridSizer* gSizer13;
	gSizer13 = new wxGridSizer( 2, 2, 0, 0 );
	
	m_staticText23 = new wxStaticText( this, wxID_ANY, _("Size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	gSizer13->Add( m_staticText23, 0, wxALL|wxALIGN_BOTTOM, 5 );
	
	m_staticText43 = new wxStaticText( this, wxID_ANY, _("Cells text"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText43->Wrap( -1 );
	gSizer13->Add( m_staticText43, 0, wxALL|wxALIGN_BOTTOM, 5 );
	
	m_spinCtrl13 = new wxSpinCtrl( this, wxID_ANY, wxT("100"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 100 );
	gSizer13->Add( m_spinCtrl13, 1, wxALL|wxEXPAND, 5 );
	
	m_textCtrl23 = new wxTextCtrl( this, wxID_ANY, _("Cell4"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer13->Add( m_textCtrl23, 1, wxALL|wxEXPAND, 5 );
	
	sbSizer5->Add( gSizer13, 1, wxEXPAND, 5 );
	
	mainSizer->Add( sbSizer5, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button2 = new wxButton( this, wxID_ANY, _("Change Count"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_button2, 1, wxALL|wxEXPAND, 5 );
	
	m_button1 = new wxButton( this, wxID_ANY, _("Print"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_button1, 1, wxALL|wxEXPAND, 5 );
	
	mainSizer->Add( bSizer2, 0, wxEXPAND, 5 );
	
	this->SetSizer( mainSizer );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Connect( menuFileExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnExitClick ) );
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::DoChangeCount ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::Print ), NULL, this );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Disconnect( wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnExitClick ) );
	m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::DoChangeCount ), NULL, this );
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::Print ), NULL, this );
	
}
