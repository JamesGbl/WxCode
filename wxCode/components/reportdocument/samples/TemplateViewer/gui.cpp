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
	wxMenuItem* menuFileOpen;
	menuFileOpen = new wxMenuItem( m_menuFile, wxID_OPEN, wxString( _("Open layout...") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( menuFileOpen );
	
	wxMenuItem* m_separator1;
	m_separator1 = m_menuFile->AppendSeparator();
	
	wxMenuItem* menuFilePrint;
	menuFilePrint = new wxMenuItem( m_menuFile, wxID_PRINT, wxString( _("Print...") ) + wxT('\t') + wxT("Ctrl+P"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( menuFilePrint );
	
	wxMenuItem* menuFilePreview;
	menuFilePreview = new wxMenuItem( m_menuFile, wxID_PREVIEW, wxString( _("Show print preview") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( menuFilePreview );
	
	wxMenuItem* m_separator2;
	m_separator2 = m_menuFile->AppendSeparator();
	
	wxMenuItem* menuFileExit;
	menuFileExit = new wxMenuItem( m_menuFile, wxID_EXIT, wxString( _("E&xit") ) + wxT('\t') + wxT("Alt+X"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( menuFileExit );
	
	m_menuBar->Append( m_menuFile, _("&File") ); 
	
	this->SetMenuBar( m_menuBar );
	
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxVERTICAL );
	
	m_canvas = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 420,597 ), wxDOUBLE_BORDER|wxTAB_TRAVERSAL );
	m_canvas->SetMinSize( wxSize( 420,597 ) );
	m_canvas->SetMaxSize( wxSize( 420,597 ) );
	
	bSizer->Add( m_canvas, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxTOP, 5 );
	
	mainSizer->Add( bSizer, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_spinPages = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	bSizer4->Add( m_spinPages, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mainSizer->Add( bSizer4, 0, wxEXPAND, 5 );
	
	this->SetSizer( mainSizer );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Connect( menuFileOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnOpen ) );
	this->Connect( menuFilePrint->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnPrint ) );
	this->Connect( menuFilePreview->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnPreview ) );
	this->Connect( menuFileExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnExitClick ) );
	m_canvas->Connect( wxEVT_PAINT, wxPaintEventHandler( MainFrameBase::OnPaint ), NULL, this );
	m_spinPages->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MainFrameBase::OnPageNumber ), NULL, this );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Disconnect( wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnOpen ) );
	this->Disconnect( wxID_PRINT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnPrint ) );
	this->Disconnect( wxID_PREVIEW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnPreview ) );
	this->Disconnect( wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnExitClick ) );
	m_canvas->Disconnect( wxEVT_PAINT, wxPaintEventHandler( MainFrameBase::OnPaint ), NULL, this );
	m_spinPages->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MainFrameBase::OnPageNumber ), NULL, this );
	
}
