/////////////////////////////////////////////////////////////////////////////
// Name:        layout_test.cpp
// Purpose:     
// Author:      Aleksandras Gluchovas
// Modified by:
// Created:     2000/1/15
// RCS-ID:      
// Copyright:   (c) Aleksandras Gluchovas
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#ifdef __GNUG__
    #pragma implementation "minimal.cpp"
    #pragma interface "minimal.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

// ----------------------------------------------------------------------------
// ressources
// ----------------------------------------------------------------------------
// the application icon
#if defined(__WXGTK__) || defined(__WXMOTIF__)
    #include "mondrian.xpm"
#endif

#include "layout_test.h"

/***** Implementation for class TestConfig:: *****/

TestConfig::TestConfig()

	: mHgap( 10 ),
	  mVgap( 10 ),
	  mInsets( 5,5, 5,5 ),
	  mRowCount( 3 ),
	  mColCount( 2 ),
	  mUseLightWeight( FALSE ),
	  mUseDoubleBuf( TRUE ),
	  mShowSrcCode( FALSE ),
	  mAutoPack( FALSE ),
	  mUseTwoPhase( TRUE ),
	  mpFont( NULL )
{}

/***** Implementation for class SourceWindow *****/

BEGIN_EVENT_TABLE( SourceWindow, wxFrame )

	EVT_BUTTON( wxID_CLOSE, SourceWindow::OnClose )

END_EVENT_TABLE()

SourceWindow::SourceWindow( MyFrame* pFrm )
	: wxFrame( NULL, -1, "Source code window", 
			   wxDefaultPosition, wxSize( 350, 460 ), wxDEFAULT_FRAME_STYLE ),
	  mpBox( NULL ),
	  mpParent( NULL ),
	  mpFrm( pFrm )
{
	Recreate();

}

SourceWindow::~SourceWindow()
{
	if ( mpFrm->IsDestroying() ) return;

	mpFrm->GetConfig().mShowSrcCode = FALSE;
	mpFrm->SetSourceWindow( NULL );
	mpFrm->SetMenuChecks();
}

void SourceWindow::OnClose( wxCommandEvent& event )
{
	this->Destroy();
}

void SourceWindow::Recreate()
{
	if ( mpParent ) 
	{
		mpParent->removeAll();
		mpParent->dispose();
		delete mpParent;
	}

	Container* parent = new Container( this );

	SetBackgroundColour(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DFACE));

	parent->setLayout( new GridBagLayout() );

	GridBagConstraints c;
	c.fill   = GridBagConstraints::BOTH;
	c.weightx = 1.0;
	c.weighty = 1.0;

#ifdef __WINDOWS__ || 1
	mpBox = new wxStaticBox( this, -1, mSrcName );
#else
	// under my wxGtk, wxStaticBox'es crash...
	mpBox = new wxPanel( this, -1 );
#endif

	Container* boxPanel = new Container( mpBox );

	boxPanel->setInsets( Insets( 20,10, 10,10 ) );

	parent->add( boxPanel, &c );

	mpSrc = new wxTextCtrl( mpBox, -1, "", wxDefaultPosition, 
							wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL );

	boxPanel->setLayout( new GridBagLayout() );

	c.fill   = GridBagConstraints::BOTH;
	c.weightx = 1.0;
	c.weighty = 1.0;
	c.gridwidth = GridBagConstraints::REMAINDER;

	boxPanel->add( new Component( mpSrc ), &c );

	c.gridwidth = GridBagConstraints::REMAINDER;
	c.fill   = GridBagConstraints::NONE;
	c.weighty = 0.0;
	c.insets.top = 10;
	c.ipady = 3; // button seems to small in height without this

	boxPanel->add( new wxButton( mpBox, wxID_CLOSE, "&Close", 
								 wxDefaultPosition, wxSize( 70, 30 ) ), &c );

	parent->doLayout();

	mpParent = parent;

	wxFont font;

#if defined(__WXMSW__) || defined(__WINDOWS__) 
    font.SetFaceName("Courier");
    font.SetPointSize( 9 );
    font.RealizeResource();
#else
    font = wxSystemSettings::GetSystemFont(wxSYS_OEM_FIXED_FONT);
    font.SetStyle(wxNORMAL);
    font.SetWeight(wxNORMAL);
    font.SetPointSize( 9);
#endif

	this->SetFont( font );

}

void SourceWindow::SetSource( const wxString& src, const wxString& srcName )
{
	// FOR NOW:: need to recreate things since it's not possible
	//           to change the text of wxStaticBox (really?)

	mSrcName =  "Code for \"" + srcName + "\" sample";

	Recreate();

	mpSrc->SetValue( src );
}

//@@ConfigDialog

/***** Implementation for class ConfigDialog *****/

static wxPoint _gCfgDlgPos( -1, -1 );
static wxSize  _gCfgDlgSize( 320, 200 ); // for memorizing ajusted size
static bool    _gCfgDlgSizeAdjusted = FALSE;

BEGIN_EVENT_TABLE( ConfigDialog, wxDialog )

	EVT_BUTTON( ID_SELECT_FONT,     ConfigDialog::OnSelectFont )
	EVT_BUTTON( ID_SHOW_DLG_SOURCE, ConfigDialog::OnShowDlgSource )
	EVT_BUTTON( wxID_APPLY,         ConfigDialog::OnApply )
	EVT_BUTTON( ID_APPLY_AND_EXIT,  ConfigDialog::OnApplyAndExit )
	EVT_SIZE  ( ConfigDialog::OnSize )
	EVT_MOVE  ( ConfigDialog::OnMove )

END_EVENT_TABLE()

ConfigDialog::ConfigDialog( wxWindow* pParent, TestConfig* pCfg )

	: wxDialog( pParent, -1, "Active Layout Settings...",
				wxDefaultPosition,
				wxSize( _gCfgDlgSize.x ,_gCfgDlgSize.y),
				wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ),

	  mpCfg( pCfg ),
	  mpFontObj( NULL )
{
	AddControls();

	ReadConfig();

	if ( _gCfgDlgPos.x == -1 ) 

		Center( wxBOTH );
	else
		Move( _gCfgDlgPos ); // retain last position
}

void ConfigDialog::AddControls()
{
	Container* parent = new Container( this );
	parent->setLayout( new GridBagLayout() );

	Container* fieldsPanel = new Container();
	fieldsPanel->setLayout( new GridBagLayout() );

	GridBagConstraints c; 
	c.gridwidth = 1;  c.insets = Insets( 5,5, 5,5 );
	c.fill = GridBagConstraints::HORIZONTAL;
	int rmd = GridBagConstraints::REMAINDER;
	int y = 0;

	c.gridy = y++; 
	fieldsPanel->add( new wxStaticText( this,-1,"hgap:" ), &c ); c.gridwidth = rmd;
	fieldsPanel->add( MakeField( mpHgap ),  &c ); c.gridwidth = 1;  

	c.gridy = y++; 
	fieldsPanel->add( new wxStaticText( this,-1, "vgap:" ), &c ); c.gridwidth = rmd;
	fieldsPanel->add( MakeField( mpVgap ),  &c ); c.gridwidth = 1;  

	c.gridy = y++; 
	fieldsPanel->add( new wxStaticText( this,-1, "inset.left:" ), &c ); c.gridwidth = rmd;
	fieldsPanel->add( MakeField( mpLeft ),        &c ); c.gridwidth = 1;        

	c.gridy = y++; 
	fieldsPanel->add( new wxStaticText( this,-1, "insets.right:" ), &c ); c.gridwidth = rmd;
	fieldsPanel->add( MakeField( mpRight ),         &c ); c.gridwidth = 1;         

	c.gridy = y++; 
	fieldsPanel->add( new wxStaticText( this,-1, "insets.top:" ), &c ); c.gridwidth = rmd;
	fieldsPanel->add( MakeField( mpTop ),         &c ); c.gridwidth = 1;         

	c.gridy = y++; 
	fieldsPanel->add( new wxStaticText( this,-1, "insets.bottom:" ), &c ); c.gridwidth = rmd;
	fieldsPanel->add( MakeField( mpBottom ),         &c  ); c.gridwidth = 1;         

	c.gridy = y++; 
	fieldsPanel->add( new wxStaticText( this,-1, "Grid-rows:" ), &c ); c.gridwidth = rmd;
	fieldsPanel->add( MakeField( mpRowCount ),   &c  ); c.gridwidth = 1;   

	c.gridy = y++; 
	fieldsPanel->add( new wxStaticText( this,-1, "Grid-cols:" ), &c ); c.gridwidth = rmd;
	fieldsPanel->add( MakeField( mpColCount ),   &c  ); c.gridwidth = 1;   

	c.gridy = y++; 
	fieldsPanel->add( new wxStaticText( this,-1, "Panel-font:" ), &c ); c.gridwidth = 1;   c.weightx = 1.0;
	fieldsPanel->add( MakeField( mpFont ),        &c ); c.gridwidth = rmd; c.weightx = 0.0;

	fieldsPanel->add( MakeButton( "Select &font...", ID_SELECT_FONT ), &c  ); c.gridwidth = 1;

	Container* checkPanel1 = new Container();
	Container* checkPanel2 = new Container();
	Container* checkPanel3 = new Container();
	Container* btnPanel    = new Container();

	c.weightx = 1.0;

	c.gridx = 0; c.gridy = y++; 
	parent->add( fieldsPanel, &c ); c.gridwidth = rmd;

	c.insets.left = 10;	c.gridx = 0; 
	
	c.gridy = y++; parent->add( checkPanel1, &c ); c.gridwidth = rmd;

	c.gridy = y++; parent->add( checkPanel2, &c );

	c.gridy = y++; parent->add( checkPanel3, &c );

	c.gridy = y++; parent->add( new wxStaticText( this,-1, 
								"(Note: this dialog is also resizable)" ), &c );

	c.gridy = y++; 
	parent->add( btnPanel, &c );

	checkPanel1->setLayout( new BoxLayout( BoxLayout::X_AXIS ) );
	checkPanel2->setLayout( new BoxLayout( BoxLayout::X_AXIS ) );
	checkPanel3->setLayout( new BoxLayout( BoxLayout::X_AXIS ) );

	c.insets.left = 10;
	checkPanel1->add( MakeCheckBox( "Use light-wieght comp. ",   mpUseLightWeight ) );
	checkPanel1->add( MakeCheckBox( "Use double-buffering.",    mpUseDoubleBuf ) );
	checkPanel2->add( MakeCheckBox( "Show source for layout.",  mpShowSrcCode ) );
	checkPanel2->add( MakeCheckBox( "Auto-pack layout.",        mpAutoPack ) );
	checkPanel3->add( MakeCheckBox( "Enable \"2-Phase layouting\" feature.", mpUseTwoPhase ) );

	btnPanel->setLayout( new FlowLayout() );

	btnPanel->add( MakeButton( "&Apply", wxID_APPLY ) );
	btnPanel->add( MakeButton( "Apply && &return", ID_APPLY_AND_EXIT ) );
	btnPanel->add( MakeButton( "&Cancel", wxID_CANCEL ) );
	btnPanel->add( MakeButton( "&View dlg-source", ID_SHOW_DLG_SOURCE ) );

	mpFont->Enable( FALSE ); 
									 
	if ( !_gCfgDlgSizeAdjusted ) 
	{
		// for packing dialog initially use only 1-phase layouting
		// to let buttons in FlowLayout'ed btnPanel strech within\ in a single row

		parent->setTwoPhaseLayoutingGlobally(FALSE);

		parent->pack();

		// for the other resizing do it the "featured" way

		parent->setTwoPhaseLayoutingGlobally(TRUE);
	}
}

void ConfigDialog::OnSize( wxSizeEvent& event )
{
	int w = 0; int h = 0;
	GetSize( &w, &h );
	_gCfgDlgSize.x = w;
	_gCfgDlgSize.y = h;
	_gCfgDlgSizeAdjusted = TRUE;
}

void ConfigDialog::OnMove( wxMoveEvent& event )
{
	_gCfgDlgPos = event.GetPosition();

#ifdef __WINDOWS__
	// HACK::......
	_gCfgDlgPos.y -= 23; // title-bar?
	_gCfgDlgPos.x -= 4;
#endif
}

void ConfigDialog::ExchgCheck( wxCheckBox* pChk, bool& value )
{
	if ( mToDlg ) pChk->SetValue( value );

			 else value = pChk->GetValue();
}

void ConfigDialog::ExchgIntField( wxTextCtrl* pFld, int& value )
{
	if ( mToDlg )
	{
		char buf[64];

		sprintf( buf, "%d", value );
		pFld->SetValue( buf );
	}
	else
	{
		wxString txt = pFld->GetLineText( 0 );
		value = atoi( txt );
	}
}

void ConfigDialog::ReadConfig()
{
	mToDlg = TRUE;
	DoExchangeFields();

	if ( mpCfg->mpFont ) 
		
		mpFont->SetValue( mpCfg->mpFont->GetFaceName() );
}

void ConfigDialog::ApplyConfig()
{
	mToDlg = FALSE;
	DoExchangeFields();
}

void ConfigDialog::DoExchangeFields()
{
	ExchgIntField( mpVgap,      mpCfg->mVgap );
	ExchgIntField( mpHgap,      mpCfg->mHgap );

	ExchgIntField( mpLeft,     mpCfg->mInsets.left );
	ExchgIntField( mpRight,    mpCfg->mInsets.right );
	ExchgIntField( mpTop,      mpCfg->mInsets.top );
	ExchgIntField( mpBottom,   mpCfg->mInsets.bottom );

	ExchgIntField( mpRowCount, mpCfg->mRowCount );
	ExchgIntField( mpColCount, mpCfg->mColCount );

	ExchgCheck( mpUseLightWeight, mpCfg->mUseLightWeight );
	ExchgCheck( mpUseDoubleBuf,   mpCfg->mUseDoubleBuf );
	ExchgCheck( mpShowSrcCode,    mpCfg->mShowSrcCode );
	ExchgCheck( mpAutoPack,       mpCfg->mAutoPack );
	ExchgCheck( mpUseTwoPhase,    mpCfg->mUseTwoPhase );

	if ( mToDlg && mpCfg->mpFont ) mpFontObj = new wxFont( *mpCfg->mpFont );

	if ( !mToDlg && mpFontObj ) 
	{
		if ( mpCfg->mpFont ) delete mpCfg->mpFont;

		mpCfg->mpFont = new wxFont( *mpFontObj );
	}
}

void ConfigDialog::OnSelectFont( wxCommandEvent& event )
{
	wxFontData data;
	data.EnableEffects( TRUE );

	if ( mpFontObj == NULL )

		data.SetInitialFont( this->GetFont() );
	else
		data.SetInitialFont( *mpFontObj );

	wxFontDialog* dialog = new wxFontDialog(this, &data);

	if (dialog->ShowModal() == wxID_OK)
	{
		if ( mpFontObj ) delete mpFontObj;

		wxFontData retData = dialog->GetFontData();
		mpFontObj = new wxFont( retData.GetChosenFont() );
		mpFont->SetValue( mpFontObj->GetFaceName() );
	}

	dialog->Destroy();
}

void ConfigDialog::OnApply( wxCommandEvent& event )
{
	ApplyConfig();
	((MyFrame*)GetParent())->ReapplyConfig();
}

void ConfigDialog::OnApplyAndExit( wxCommandEvent& event )
{
	ApplyConfig();
	EndModal( wxID_APPLY );
}

void ConfigDialog::OnShowDlgSource( wxCommandEvent& event )
{
	MyFrame* mpFrm = (MyFrame*)GetParent();

	mpCfg->mShowSrcCode = TRUE;
	mpFrm->ShowSource();
	mpFrm->SetMenuChecks();

	mpFrm->GetSourceWindow()->
		SetSource( mpFrm->FindSourceFor("ConfigDialog"), 
				   "Source for Active Layout Settings dialog" );

	EndModal( wxID_CANCEL );

	mpFrm->GetSourceWindow()->Raise();
}

wxButton* ConfigDialog::MakeButton( const wxString& name, int id )
{
	return new wxButton( this, id, name, wxDefaultPosition );
}

wxTextCtrl* ConfigDialog::MakeField( wxTextCtrl*& pCtl )
{
	pCtl = new wxTextCtrl( this, -1, "" );

	return pCtl;
}

wxCheckBox* ConfigDialog::MakeCheckBox( const wxString& name, wxCheckBox*& pCheck )
{
	pCheck = new wxCheckBox( this, -1, name );

	return pCheck;
}

//$$ConfigDialog

/***** Implementation for class MyFrame:: *****/

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
    EVT_MENU(Minimal_About, MyFrame::OnAbout)

	EVT_MENU(Test_absolute,  MyFrame::Test_AbsoluteLayout )
	EVT_MENU(Test_card,      MyFrame::Test_CardLayout )
	EVT_MENU(Test_card_flip, MyFrame::Test_CardLayout_flip )
	EVT_MENU(Test_border,    MyFrame::Test_BorderLayout )
	EVT_MENU(Test_box_vert,  MyFrame::Test_BoxLayout_vert )
	EVT_MENU(Test_box_horiz, MyFrame::Test_BoxLayout_horiz )
	EVT_MENU(Test_flow,      MyFrame::Test_FlowLayout )
	EVT_MENU(Test_gridbag,   MyFrame::Test_GridBagLayout )
	EVT_MENU(Test_gridbag_1,   MyFrame::Test_GridBagLayout_1 )
	EVT_MENU(Test_gridbag_2,   MyFrame::Test_GridBagLayout_2 )
	EVT_MENU(Test_grid,      MyFrame::Test_GridLayout )
	EVT_MENU(Test_all_1,     MyFrame::Test_All_1 )
	EVT_MENU(Test_all_2,     MyFrame::Test_All_2 )
	EVT_MENU(Test_packing,   MyFrame::Test_Packing )

	EVT_MENU( Settings_setup,       MyFrame::OnMiscSettings )
	EVT_MENU( Settings_lightweight, MyFrame::OnLightWeight )
	EVT_MENU( Settings_doublebuf,   MyFrame::OnDoubleBuf )
	EVT_MENU( Settings_autopack,    MyFrame::OnAutoPack )
	EVT_MENU( Settings_showsrc,     MyFrame::OnShowSource )
	EVT_MENU( Settings_setfont,     MyFrame::OnSetFont )
	
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

#define FRAME_TITLE "AWT-Layouts contrib. demo"

// `Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // Create the main application window
    MyFrame *frame = new MyFrame("",
                                 wxPoint(50, 50), wxSize(670, 460));

    // Show it and tell the application that it's our main window
    // @@@ what does it do exactly, in fact? is it necessary here?
    frame->Show(TRUE);
    SetTopWindow(frame);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned FALSE here, the
    // application would exit immediately.
    return TRUE;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame((wxFrame *)NULL, -1, title, pos, size),
	     parent( NULL ),
		 mpCurCont( NULL ),
		 mpSrcWnd( NULL )
{
    // set the frame icon
    SetIcon(wxICON(mondrian));

    // create a menu bar
    wxMenu *menuFile = new wxMenu("");

    menuFile->Append(Minimal_About, "&About AWT-Layouts...", "Show about dialog");
    menuFile->AppendSeparator();
    menuFile->Append(Minimal_Quit, "E&xi", "Quit this program");

	wxMenu *menuTest = new wxMenu("");

	menuTest->Append( Test_absolute,  "&AbsoluteLayout" );
	menuTest->Append( Test_border,    "&BorderLayout" );
	menuTest->Append( Test_box_vert,  "Bo&xLayout (vert)" );
	menuTest->Append( Test_box_horiz, "Bo&xLayout (horiz)" );
	menuTest->Append( Test_card,      "&CardLayout" );
	menuTest->Append( Test_card_flip, "&CardLayout (fl&ip)" );
	menuTest->Append( Test_flow,      "&FlowLayout" );
	menuTest->Append( Test_grid,      "&GridLayout" );
	menuTest->Append( Test_gridbag,     "GridB&agLayout" );
	menuTest->Append( Test_gridbag_1,   "GridBagLayout-1" );
	menuTest->Append( Test_gridbag_2,   "GridBagLayout-2" );
	menuTest->AppendSeparator();
	menuTest->Append( Test_all_1,     "Test_All_&1" );
	menuTest->Append( Test_all_2,     "Test_All_&2" );
	menuTest->AppendSeparator();
	menuTest->Append( Test_packing,  "&Pack" );

	wxMenu* menuSettings = new wxMenu("");

	menuSettings->Append( Settings_setup,       "&Misc. layout parameters..." );
	menuSettings->AppendSeparator();
	menuSettings->Append( Settings_lightweight, "Use &light-weight (\"Swing\") components", "", TRUE );
	menuSettings->Append( Settings_doublebuf,   "Use double-buffering for l-w comps.", "", TRUE );
	menuSettings->Append( Settings_autopack,    "&Auto-pack frame around layout", "", TRUE );
	menuSettings->Append( Settings_showsrc,     "Show source code for current. layout", "", TRUE );
	menuSettings->AppendSeparator();
	menuSettings->Append( Settings_setfont, "Set &font..." );

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&File");
	menuBar->Append(menuTest, "&Test");
	menuBar->Append(menuSettings, "&Settings");

	this->SetBackgroundColour( *wxWHITE );

	//new wxButton( this, -1, "Hi", wxDefaultPosition );

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

	SetMenuChecks();

	Show( TRUE );

	/***************************************************/

	wxCommandEvent evt;

	//Test_AbsoluteLayout(evt);
	Test_BorderLayout(evt);
	//Test_BoxLayout_vert(evt);
	//Test_BoxLayout_horiz(evt);
	//Test_CardLayout(evt);
	//Test_CardLayout_flip(evt);
	//Test_FlowLayout(evt);
	//Test_GridLayout(evt);
	//Test_GridBagLayout(evt);
	//Test_GridBagLayout_1(evt);
	//Test_GridBagLayout_2(evt);
	//Test_All_1(evt);
	//Test_All_2(evt);

	//ConfigDialog dlg( this, &mCfg );
	//dlg.ShowModal();

	/***************************************************/
}

MyFrame::~MyFrame()
{
	mIsDestroying = TRUE;

	if ( mpSrcWnd ) 
		
		mpSrcWnd->Destroy();
}

void MyFrame::SetTestName( const wxString& name )
{
	Component::setDoubleBufferingGlobally( mCfg.mUseDoubleBuf );

	SetTitle( wxString( FRAME_TITLE ) + " - [" + name + "]" );

	if ( !mCfg.mUseLightWeight )

		SetBackgroundColour(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DFACE));
	else
		SetBackgroundColour( *wxWHITE );

	mTestName = name;

	ShowSource();
}

// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf( "This is the about dialog for AWT-Layouts demo.\n\n"
				"Copyright (C) 1999 Aleksandras Gluchovas (mailto:alex@soften.ktu.lt)\n\n"
                "Welcome to %s"
#ifdef wxBETA_NUMBER
               " (beta %d)!"
#endif // wxBETA_NUMBER
               , wxVERSION_STRING
#ifdef wxBETA_NUMBER
               , wxBETA_NUMBER
#endif // wxBETA_NUMBER
              );

    wxMessageBox(msg, "About AWT-Layouts", wxOK | wxICON_INFORMATION, this);
}


#ifdef __HACK_MY_MSDEV40__

char wxDummyChar = 0;

#include <fstream.h>
// stream-stuff linking magic

filebuf fb1( 1 );
ostream_withassign cout(&fb1);

filebuf fb2( 2 );
ostream_withassign cerr(&fb2);

#endif

void MyFrame::RemoveAll()
{
	if ( parent ) 
	{
		parent->removeAll();

		parent->dispose();

		delete parent;

		parent = NULL;

		Refresh();
	}
}

Component* MyFrame::CreateComp( String name = "", int width, int height )
{
	if ( !mCfg.mUseLightWeight )
	{
		return MakeButton( name );
	}

	Component* pComp = new Component();

	pComp->setName( name );

	if ( width != -1 )

		pComp->setMinimumSize( Dimension( width, height ) );

	return pComp;
}

//@@AbsoluteLayout

void MyFrame::Test_AbsoluteLayout( wxCommandEvent& event )
{
	RemoveAll(); SetTestName( "AbsoluteLayout" );

	parent = new Container( this );
	SetCurrentContainer( parent );

	parent->setLayout( new AbsoluteLayout() );

	Component* c1 = CreateComp( "Hello1" );

	c1->setLocation( 100, 30 );

	Component* c2 = CreateComp( "Hello2" );

	c2->setLocation( 30, 120 );

	Component* c3 = CreateComp( "Hello3" );

	c3->setLocation( 230, 200 );

	parent->add( c1 );
	parent->add( c2 );
	parent->add( c3 );

	if ( mCfg.mAutoPack ) parent->pack();
				     else parent->doLayout();
}

//$$AbsoluteLayout

//@@BorderLayout

void MyFrame::Test_BorderLayout( wxCommandEvent& event )
{
	RemoveAll(); SetTestName( "BorderLayout" );

	parent = new Container( this );
	SetCurrentContainer( parent );

	parent->setLayout( new BorderLayout( mCfg.mHgap, mCfg.mVgap ) );

	Component* north  = CreateComp( "North"  );
	Component* south  = CreateComp( "South"  );
	Component* west   = CreateComp( "West"   );
	Component* east   = CreateComp( "East"   );
	Component* center = CreateComp( "Center" );

	parent->add( north,  BorderLayout::NORTH  );
	parent->add( south,  BorderLayout::SOUTH  );
	parent->add( west,   BorderLayout::WEST   );
	parent->add( east,   BorderLayout::EAST   );
	parent->add( center, BorderLayout::CENTER );

	if ( mCfg.mAutoPack ) parent->pack();
				     else parent->doLayout();
}

//$$BorderLayout

//@@BoxLayout

void MyFrame::Test_BoxLayout_vert( wxCommandEvent& event )
{
	RemoveAll(); SetTestName( "BoxLayout" );

	parent = new Container( this );
	SetCurrentContainer( parent );

	parent->setLayout( new BoxLayout( parent, BoxLayout::Y_AXIS ) );

	for( int i = 0; i != 5; ++i )

		parent->add( CreateComp( "Item" + IntToStr(i) ) );

	if ( mCfg.mAutoPack ) parent->pack();
				     else parent->doLayout();
}

//$$BoxLayout

void MyFrame::Test_BoxLayout_horiz( wxCommandEvent& event )
{
	RemoveAll(); SetTestName( "BoxLayout" );

	parent = new Container( this );
	SetCurrentContainer( parent );

	parent->setLayout( new BoxLayout( parent, BoxLayout::X_AXIS ) );

	for( int i = 0; i != 5; ++i )

		parent->add( CreateComp( "Item" + IntToStr(i) ) );

	DoPack( parent );
}

//@@CardLayout

void MyFrame::Test_CardLayout( wxCommandEvent& event )
{
	RemoveAll(); SetTestName( "CardLayout" );

	parent = new Container( this );
	SetCurrentContainer( parent );

	parent->setLayout( new CardLayout() );

	parent->add( CreateComp( "Card1" ), (Object*)new String("1") );
	parent->add( CreateComp( "Card2" ), (Object*)new String("2") );
	parent->add( CreateComp( "Card3" ), (Object*)new String("3") );
	
	if ( mCfg.mAutoPack ) parent->pack();
				     else parent->doLayout();
}

//$$CardLayout

void MyFrame::Test_CardLayout_flip( wxCommandEvent& event )
{
	if ( mTestName != "CardLayout" ) Test_CardLayout( event );

	CardLayout* pCardLayout = (CardLayout*)parent->getLayout();

	pCardLayout->next( parent );
}

//@@FlowLayout

void MyFrame::Test_FlowLayout( wxCommandEvent& event )
{
	RemoveAll(); SetTestName( "FlowLayout" );

	parent = new Container( this );
	SetCurrentContainer( parent );

	parent->setLayout( new FlowLayout( FlowLayout::CENTER, mCfg.mHgap, mCfg.mVgap ) );

	for( int i = 0; i != 5; ++i )

		parent->add( CreateComp( "Item" + IntToStr(i) ) );

	if ( mCfg.mAutoPack ) parent->pack();
				     else parent->doLayout();
}

//$$FlowLayout

//@@GridBagLayout

void MyFrame::Test_GridBagLayout( wxCommandEvent& event )
{
	RemoveAll(); SetTestName( "GridBagLayout" );

	parent = new Container( this );
	SetCurrentContainer( parent );

	parent->setLayout( new GridBagLayout() );

	GridBagConstraints c;

	/*
	parent->add( CreateComp( "Item1" ), &c );
	parent->add( CreateComp( "Item1-1" ), &c );
	parent->add( CreateComp( "Item1-2" ), &c );

	c.gridwidth = GridBagConstraints::REMAINDER;

	parent->add( CreateComp( "Item2" ), &c );

	c.gridwidth = 4;
	c.fill      = GridBagConstraints::BOTH;

	parent->add( CreateComp( "Item3" ), &c );
	*/

	//c.gridwidth = GridBagConstraints::REMAINDER;

	parent->add( CreateComp( "Item1" ), &c );
	parent->add( CreateComp( "Item1-2" ), &c );

	c.gridwidth = GridBagConstraints::REMAINDER;

	parent->add( CreateComp( "Item2" ), &c );

	c.gridwidth = 1;

	parent->add( CreateComp( "Item3" ), &c );

	c.gridx = 0;
	c.gridy = 2;
	c.gridwidth = 4;
	c.fill      = GridBagConstraints::NONE;
	parent->add( CreateComp( "Item4" ), &c );
	c.fill      = GridBagConstraints::NONE;

	c.gridx = 1;
	c.gridy = 1;
	c.gridwidth = 1;
	parent->add( CreateComp( "Item5" ), &c );
	c.gridwidth = 0;

	c.gridx = 0;
	c.gridy = 3;
	c.gridwidth = GridBagConstraints::REMAINDER;
	c.fill      = GridBagConstraints::BOTH;
	parent->add( CreateComp( "Item6" ), &c );

	c.gridx = 3;
	c.gridy = 1;
	c.gridwidth = GridBagConstraints::REMAINDER;
	c.fill      = GridBagConstraints::BOTH;
	parent->add( CreateComp( "Item7" ), &c );

	c.gridx = 5;
	c.gridy = 4;
	c.gridwidth = GridBagConstraints::REMAINDER;
	c.fill      = GridBagConstraints::BOTH;
	parent->add( CreateComp( "Item8" ), &c );

	if ( mCfg.mAutoPack ) parent->pack();
				     else parent->doLayout();
}

//$$GridBagLayout

//@@GridBagLayout-1

void MyFrame::Test_GridBagLayout_1( wxCommandEvent& event )
{
	RemoveAll(); SetTestName( "GridBagLayout-1" );

	parent = new Container( this );
	SetCurrentContainer( parent );

	parent->setLayout( new GridBagLayout() );

	GridBagConstraints c;

	///////////////////////////////////////////////////////////////////
	// this code is taken from java.awt.GridBagLayout test-sample    //
	// found in JDK1.1 docs, and adapted for C++ version Serves as a //
	// test for complience with behaviour of actual java classes     //
	///////////////////////////////////////////////////////////////////

	c.fill = GridBagConstraints::BOTH;
	c.weightx = 1.0;
	parent->add( CreateComp( "Button1" ), &c );
	parent->add( CreateComp( "Button2" ), &c );
	parent->add( CreateComp( "Button3" ), &c );

	c.gridwidth = GridBagConstraints::REMAINDER; //end row
	parent->add( CreateComp( "Button4" ), &c );

	c.weightx = 0.0;                  //reset to the default
	parent->add( CreateComp( "Button5" ), &c ); //another row

	// Slight-INCOMPATIBILITY:: actually gridwidth of "Button6" should be 
	// defined as "c.gridwidth = GridBagConstraints::REMAINDER;"
	// THE HACK:: we set gridwidth to 3 instead of REMAINDER
	// FIXME:: the behaviour of AWT is not simulated completely at this point...

	c.gridwidth = 3; //next-to-last in row
	parent->add( CreateComp( "Button6" ), &c );

	c.gridwidth = GridBagConstraints::REMAINDER; //end row
	parent->add( CreateComp( "Button7" ), &c );

	c.gridwidth = 1;                //reset to the default
	c.gridheight = 2;
	c.weighty = 1.0;
	parent->add( CreateComp( "Button8" ), &c );

	c.weighty = 0.0;                  //reset to the default
	c.gridwidth = GridBagConstraints::REMAINDER; //end row
	c.gridheight = 1;               //reset to the default
	parent->add( CreateComp( "Button9" ), &c );
	parent->add( CreateComp( "Button10" ), &c );

	if ( mCfg.mAutoPack ) parent->pack();
				     else parent->doLayout();
}

//$$GridBagLayout-1

//@@GridBagLayout-2

void MyFrame::Test_GridBagLayout_2( wxCommandEvent& event )
{
	RemoveAll(); SetTestName( "GridBagLayout-2" );

	parent = new Container( this );
	SetCurrentContainer( parent );

	parent->setLayout( new GridBagLayout() );

	GridBagConstraints c;
	c.fill = GridBagConstraints::BOTH;
	c.weightx = 1.0;
	c.weighty = 1.0;
	c.gridwidth = GridBagConstraints::REMAINDER;

	parent->add( MakeButton( "Hi" ), &c );

	Container* panel = new Container();

	c.weighty = 0.0;
	parent->add( panel, &c );

	panel->setLayout( new FlowLayout() );

	for( int i = 0;i != 5; ++i )

		panel->add( MakeButton( "Button" + IntToStr(i) ) );

	if ( mCfg.mAutoPack ) parent->pack();
				     else parent->doLayout();
}

//$$GridBagLayout-2

//@@GridLayout

void MyFrame::Test_GridLayout( wxCommandEvent& event )
{
	RemoveAll(); SetTestName( "GridLayout" );

	parent = new Container( this );
	SetCurrentContainer( parent );

	parent->setLayout( new GridLayout( mCfg.mRowCount, mCfg.mColCount,
									   mCfg.mHgap, mCfg.mVgap ) );

	for( int i = 0; i != mCfg.mRowCount * mCfg.mColCount - 1; ++i )

		parent->add( CreateComp( "Item" + IntToStr(i) ) );

	if ( mCfg.mAutoPack ) parent->pack();
				     else parent->doLayout();
}

//$$GridLayout

//@@Test_All_1

void MyFrame::Test_All_1( wxCommandEvent& event )
{
	RemoveAll(); SetTestName( "Test_All_1" );

	parent = new Container( this );
	SetCurrentContainer( parent );

	parent->setLayout( new GridBagLayout() );

	GridBagConstraints c;
	c.weightx   = 1.0;
	c.weighty   = 1.0;
	c.fill      = GridBagConstraints::BOTH;
	c.gridwidth = GridBagConstraints::REMAINDER;

	Container* sortPanel = MakeStaticBox( "Filtering options: (in GridBagLayout)" );

	Container* btnPanel = new Container();

	parent->add( sortPanel, &c );

	c.weighty   = 0.0;
	parent->add( btnPanel,  &c );

	btnPanel->setLayout( new FlowLayout() );
	btnPanel->setInsets( Insets( 10,10, 10,10 ) );

	btnPanel->add( MakeLabel( "Buttons:" ) );
	btnPanel->add( MakeButton( "&Apply" ) );
	btnPanel->add( MakeButton( "&Cancel" ) );
	btnPanel->add( MakeButton( "&Help" ) );
	btnPanel->add( MakeLabel( "in a FlowLayout" ) );

	SetCurrentContainer( sortPanel );

	sortPanel->setInsets( GetInsetsForStaticBox( sortPanel ) );

	// creating "Resolution" panel

	sortPanel->setLayout( new GridBagLayout() );

	char* resValues[] = { "FIXED", "INVALID", "WONTFIX", "LATER", "REMIND", 
					      	"DUPLICATE", "WORKSFORME" };
	char* pltfValues[] = { "All", "DEC", "HP", "Macintosh", "PC", "SGI", "Sun", "Other" };

	char* opSysValues[] = { "All", "Windows 3.1", "Windows 95", "Windows 98", "Windows NT", 
						    "Mac Sys7", "Mac Sys7.5", "Mac Sys7.6.1", "Mac Sys8.0",
						    "Mac Sys7", "Mac Sys8.5", "Mac Sys8.6",
						    "Aix", "BSD", "IRIX", "Linux", "FreeBSD", "NetBSD", "OSF/1", "SOLARIS",
						    "SunOS", "Neutrino", "OS/2", "OpenVMS", "other" };

	char* compValues[] = { "Not applied", "wxGrid", "wxHTML", "wxMedia",
						   "wxLayout", "wxDialupManager", "wxFrameLayout" };

	char* severValues[] = { "Blocker", "Critical", "Major", "Minor",
						    "Trivial", "Enhancement", "Showstopper" };

	char* priValues[]   = { "P1", "P2", "P3", "P4", "P5" };

	int resCnt   = sizeof( resValues )   / sizeof(char*);
	int pltfCnt  = sizeof( pltfValues )  / sizeof(char*);
	int opSysCnt = sizeof( opSysValues ) / sizeof(char*);
	int compCnt  = sizeof( compValues  ) / sizeof(char*);
	int svrCnt   = sizeof( severValues ) / sizeof(char*);
	int priCnt   = sizeof( priValues   ) / sizeof(char*);

	c.weightx   = 1.0;
	c.fill      = GridBagConstraints::BOTH;

	c = GridBagConstraints(); // reset defauls
	c.fill      = GridBagConstraints::BOTH;
	int rel = GridBagConstraints::RELATIVE;
	c.weightx = 1.0;
	c.insets.left = 2; c.insets.right = 2;

	c.gridx = 1; c.gridy = 1;
	sortPanel->add( MakeListBox ( resValues, resCnt ),   &c ); c.gridx = 2; c.gridwidth = 2;
	sortPanel->add( MakeListBox ( severValues, svrCnt ), &c ); c.gridx = 4; c.gridwidth = 1;
	sortPanel->add( MakeListBox ( priValues, priCnt ),   &c );

	c.gridy = 3; c.gridx = 1; c.weighty = 1.0; c.gridwidth = 2;
	sortPanel->add( MakeListBox ( compValues, compCnt ), &c ); c.gridx = 3;
	sortPanel->add( MakeListBox ( pltfValues, pltfCnt ), &c ); c.gridwidth = 1;

	c.gridx = 0; c.gridy = 1; c.weighty = 1.0; c.gridheight = GridBagConstraints::REMAINDER;
	sortPanel->add( MakeListBox ( opSysValues, opSysCnt ), &c );

	c.gridx = 0; c.gridy = 0; c.gridheight = 1; c.weighty = 0.0;
	sortPanel->add( MakeCheckBox( "OpSys" ),      &c ); c.gridx = 1;
	sortPanel->add( MakeCheckBox( "Resolution" ), &c ); c.gridx = 2; c.gridwidth = 2;
	sortPanel->add( MakeCheckBox( "Severity" ),   &c ); c.gridx = 4; c.gridwidth = 0;
	sortPanel->add( MakeCheckBox( "Priority" ),   &c );

	c.gridx = 1; c.gridy = 2; c.gridwidth = 2;
	sortPanel->add( MakeCheckBox( "Component" ), &c ); c.gridx = 3;
	sortPanel->add( MakeCheckBox( "Platform" ),  &c ); c.gridwidth = 0;

	Container* keywordsPanel = MakeStaticBox( "Contain Keywords: (in another grid-bag)" );
	keywordsPanel->setInsets( GetInsetsForStaticBox( keywordsPanel ) );

	c.gridx = 1; c.gridy = 4; c.insets.top = 5;
	c.gridwidth = GridBagConstraints::REMAINDER; c.weighty = 0.0;
	sortPanel->add( keywordsPanel, &c );

	keywordsPanel->setLayout( new GridBagLayout() );

	SetCurrentContainer( keywordsPanel );

	c = GridBagConstraints(); // reset defauls
	c.anchor = GridBagConstraints::NORTHWEST;

	c.gridx = 0; c.gridy = 0;
	keywordsPanel->add( MakeRadioButton( "Up", TRUE ), &c );
	
	c.gridx = 0; c.gridy = 1;
	keywordsPanel->add( MakeRadioButton( "Down" ), &c );

	c.gridx = 0; c.gridy = 2;
	keywordsPanel->add( MakeRadioButton( "Start" ), &c );

	c.fill   = GridBagConstraints::BOTH;
	c.gridx = 1; c.gridy = 0; c.weightx = 1.0; c.weighty = 1.0; c.gridheight = 3; 
	keywordsPanel->add( MakeTextField( "p1, fixed, bcc" ), &c );

	if ( mCfg.mAutoPack ) parent->pack();
				     else parent->doLayout();
}

//$$Test_All_1

//@@Test_All_2

void MyFrame::Test_All_2( wxCommandEvent& event )
{
	RemoveAll(); SetTestName( "Test_All_2" );

	parent = new Container( this );
	SetCurrentContainer( parent );

	parent->setLayout( new GridBagLayout() );

	GridBagConstraints c;
	c.weightx   = 1.0;
	c.weighty   = 1.0;
	c.fill      = GridBagConstraints::BOTH;
	c.gridwidth = GridBagConstraints::REMAINDER;
	c.insets    = Insets( 5,10,2,10 );

	Container* imagePanel = new Container();

	parent->add( imagePanel, &c );

	imagePanel->setLayout( new FlowLayout() );

	for( int i = 0; i != 15; ++i )
	{
		Component* pComp = MakeButton( "< " + IntToStr( i + 1 ) + " >" );
		pComp->setMinimumSize( Dimension( 60,60 ) );

		imagePanel->add( pComp );
	}

	c.weighty   = 0.0;
	c.fill = GridBagConstraints::VERTICAL; c.insets.top = 2; 
	parent->add( MakeLabel( "---- Purchase A Fine Image Today -----" ), &c );

	c.insets.top = 5;
	c.fill = GridBagConstraints::BOTH;

	Container* infoPanel = MakeStaticBox( "" );
	Container* btnPanel  = new Container();

	parent->add( infoPanel, &c );
	parent->add( btnPanel,  &c );

	btnPanel->setLayout( new FlowLayout() );

	btnPanel->add( MakeButton( "&Purchase" ) );
	btnPanel->add( MakeButton( "&Cancel" ) );

	infoPanel->setLayout( new GridBagLayout() );

	infoPanel->setInsets( GetInsetsForStaticBox( infoPanel ) );

	SetCurrentContainer( infoPanel );

	int rmd = GridBagConstraints::REMAINDER;

	c.insets = Insets( 5,5, 0,0 );
	c.fill = GridBagConstraints::HORIZONTAL;

	c.weightx = 0.0; c.gridx = 0; c.gridy = 0;
	infoPanel->add( MakeLabel( "Name:" ), &c ); c.gridwidth = rmd; c.weightx = 1.0; c.gridwidth = 3;
	infoPanel->add( MakeTextField( "" ), &c );  c.gridwidth = 0;

	c.weightx = 0.0; c.gridx = 0; c.gridy = 1;
	infoPanel->add( MakeLabel( "Address:" ), &c ); c.gridwidth = rmd; c.weightx = 1.0; c.gridwidth = 3;
	infoPanel->add( MakeTextField( "" ), &c );  c.gridwidth = 0;

	c.weightx = 0.0; c.gridx = 0; c.gridy = 2;
	infoPanel->add( MakeLabel( "City:" ), &c ); c.weightx = 1.0;
	infoPanel->add( MakeTextField( "" ), &c );  c.weightx = 0.0;
	infoPanel->add( MakeLabel( "State:" ), &c ); 
	infoPanel->add( MakeTextField( "" ), &c );

	char* methods[] = { "Visa", "MasterCard", "Check" };

	c.weightx = 0.0; c.gridx = 0; c.gridy = 3;
	infoPanel->add( MakeLabel( "Payment:" ), &c ); c.weightx = 0.0;

	c.fill   = GridBagConstraints::NONE;
	c.anchor = GridBagConstraints::WEST;
	infoPanel->add( MakeComboBox( methods, sizeof(methods)/sizeof(char*) ), &c );
	
	// no auto-packing for this panel... 
	// (so many images in flow-layout may cause default width to be too big)

	parent->doLayout();
}

//$$Test_All_2

void MyFrame::Test_Packing( wxCommandEvent& event )
{
	if ( parent ) parent->pack();
}

Component* MyFrame::MakeListBox( char** values, int nValues )
{
	if ( !mCfg.mUseLightWeight )
	{
		wxString choices[50];

		for( int i = 0; i != nValues; ++i )

			choices[i] = values[i];

		wxListBox* widget = new wxListBox( mpCurCont->getPeer(), -1,
										   wxDefaultPosition, wxDefaultSize, 
										   nValues, choices, wxLB_MULTIPLE );

		if ( nValues > 10 )

			widget->SetSelection( nValues / 2, TRUE );

		return new Component( widget );
	}
	else
		return CreateComp( "[ListBox]" );
}

Component* MyFrame::MakeComboBox( char** values, int nValues )
{
	if ( !mCfg.mUseLightWeight )
	{
		wxString choices[50];

		for( int i = 0; i != nValues; ++i )

			choices[i] = values[i];

		wxChoice* widget = new wxChoice( mpCurCont->getPeer(), -1, 
											 wxDefaultPosition, wxDefaultSize, 
											 nValues, choices );

		widget->SetSelection( 0 );

		return new Component( widget );
	}
	else
	{
		if ( nValues != 0 )

			return CreateComp( "[ComboBox] " + wxString(values[0]) );
		else
			return CreateComp( "[ComboBox]" );
	}

	return NULL; // never reached
}

Container* MyFrame::MakeStaticBox( const wxString& name)
{
	if ( !mCfg.mUseLightWeight )
	{
#ifdef __WINDOWS__  
		wxStaticBox* widget = new wxStaticBox( mpCurCont->getPeer(), -1, name );
#else
		// under my wxGtk, wxStaticBox'es crash...
		wxPanel* widget = new wxPanel( mpCurCont->getPeer(), -1 );
		widget->SetFont( mpCurCont->getPeer()->GetFont() );
		//wxStaticBox* widget = new wxStaticBox( mpCurCont->getPeer(), -1, name, wxPoint(10,10), wxSize(120,120) );

#endif

		return new Container( widget );
	}
	else
	{
		Container* pCont = new Container();
		pCont->setName( name );
		return pCont;
	}
}

Component* MyFrame::MakeButton( const wxString& name )
{
	if ( !mCfg.mUseLightWeight )
	{
		wxButton* widget = new wxButton( mpCurCont->getPeer(), -1, name, wxPoint(0,0) );

		return new Component( widget );
	}
	else
		return CreateComp( "[Button] " + name );
}

Component* MyFrame::MakeTextField( const wxString& value )
{
	if ( !mCfg.mUseLightWeight )
	{
		wxTextCtrl* widget = new wxTextCtrl( mpCurCont->getPeer(), -1, value );

		return new Component( widget );
	}
	else
		return CreateComp( "[TextCtrl] " + value );
}

Component* MyFrame::MakeLabel( const wxString& value )
{
	if ( !mCfg.mUseLightWeight )
	{
		wxStaticText* widget = new wxStaticText( mpCurCont->getPeer(), -1, value );

		return new Component( widget );
	}
	else
		return CreateComp( "[Label] " + value );
}

Component* MyFrame::MakeRadioButton( const wxString& name, bool selected )
{
	if ( !mCfg.mUseLightWeight )
	{
		wxRadioButton* widget = new wxRadioButton( mpCurCont->getPeer(), -1, name );
		widget->SetValue( selected );

		return new Component( widget );
	}
	else
		return CreateComp( "[RadioButton] " + name );
}

Component* MyFrame::MakeCheckBox( const wxString& value, bool checked )
{
	if ( !mCfg.mUseLightWeight )
	{
		wxCheckBox* widget = new wxCheckBox( mpCurCont->getPeer(), -1, value );
		widget->SetValue( checked );

		return new Component( widget );
	}
	else
		return CreateComp( "[CheckBox] " + value );
}

Component* MyFrame::SetCurrentContainer( Container* pCont )
{
	mpCurCont = pCont;
	return pCont;
}

/*** menu and config. handling ***/

void MyFrame::OnMiscSettings( wxCommandEvent& event )
{
	ConfigDialog dlg( this, &mCfg );

	if ( dlg.ShowModal() == wxID_APPLY )

		ReapplyConfig();
}

void MyFrame::OnLightWeight( wxCommandEvent& event )
{
	mCfg.mUseLightWeight = GetMenuBar()->IsChecked( Settings_lightweight );

	ReapplyConfig();
}

void MyFrame::OnDoubleBuf( wxCommandEvent& event )
{
	mCfg.mUseDoubleBuf = GetMenuBar()->IsChecked( Settings_doublebuf );

	ReapplyConfig();
}

void MyFrame::OnAutoPack( wxCommandEvent& event )
{
	mCfg.mAutoPack = GetMenuBar()->IsChecked( Settings_autopack );

	ReapplyConfig();
}

void MyFrame::OnShowSource( wxCommandEvent& event )
{
	mCfg.mShowSrcCode = GetMenuBar()->IsChecked( Settings_showsrc );

	ReapplyConfig();
}

void MyFrame::OnSetFont( wxCommandEvent& event )
{
	wxFontData data;
	data.EnableEffects( TRUE );

	if ( mCfg.mpFont == NULL )

		data.SetInitialFont( this->GetFont() );
	else
		data.SetInitialFont( *mCfg.mpFont );

	wxFontDialog* dialog = new wxFontDialog(this, &data);

	if (dialog->ShowModal() == wxID_OK)
	{
		if ( mCfg.mpFont ) delete mCfg.mpFont;

		wxFontData retData = dialog->GetFontData();
		mCfg.mpFont = new wxFont( retData.GetChosenFont() );

		ReapplyConfig();
	}

	dialog->Destroy();

}

void MyFrame::SetMenuChecks()
{
	if ( mIsDestroying ) return;

	wxMenuBar& bar = *GetMenuBar();

	bar.Check( Settings_lightweight, mCfg.mUseLightWeight );
	bar.Check( Settings_doublebuf,   mCfg.mUseDoubleBuf );
	bar.Check( Settings_showsrc,     mCfg.mShowSrcCode );
	bar.Check( Settings_autopack,    mCfg.mAutoPack );
}

void MyFrame::ShowSource()
{
	if ( mCfg.mShowSrcCode ) DoShowSource();
	else
	{
		if (mpSrcWnd) mpSrcWnd->Destroy();

		mpSrcWnd = NULL;
	}
}

void MyFrame::RecreateLayout()
{
	wxCommandEvent evt;

	if ( mTestName == "AbsoluteLayout" ) Test_AbsoluteLayout(evt);
	else
	if ( mTestName == "BorderLayout" ) Test_BorderLayout(evt);
	else
	if ( mTestName == "BoxLayout" ) Test_BoxLayout_horiz(evt);
	else
	if ( mTestName == "CardLayout" ) Test_CardLayout(evt);
	else
	if ( mTestName == "FlowLayout" ) Test_FlowLayout(evt);
	else
	if ( mTestName == "GridBagLayout" ) Test_GridBagLayout(evt);
	else
	if ( mTestName == "GridBagLayout-1" ) Test_GridBagLayout_1(evt);
	else
	if ( mTestName == "GridBagLayout-2" ) Test_GridBagLayout_2(evt);
	else
	if ( mTestName == "GridLayout" ) Test_GridLayout(evt);
	else
	if ( mTestName == "Test_All_1" ) Test_All_1(evt);
	else
	if ( mTestName == "Test_All_2" ) Test_All_2(evt);
}

void MyFrame::DoShowSource()
{
	if ( mpSrcWnd == NULL )
	{
		mpSrcWnd = new SourceWindow( this );

		mpSrcWnd->Show( TRUE );
	}

	mpSrcWnd->SetSource( GetSourceForLayout(), mTestName );
}

void MyFrame::ReapplyConfig()
{
	if ( mCfg.mpFont ) this->SetFont( *mCfg.mpFont );

	Component::setTwoPhaseLayoutingGlobally( mCfg.mUseTwoPhase );

	RecreateLayout();

	SetMenuChecks();
}

void MyFrame::DoPack( Container* pCont )
{
	if ( mCfg.mAutoPack ) pCont->pack();
				     else pCont->doLayout();
}

Insets MyFrame::GetInsetsForStaticBox( Container* pStaticBox )
{
	Dimension dim = pStaticBox->getMinimumSize();

	// FOR NOW::...
	return Insets( dim.height,10, 10,10 );
}

wxString MyFrame::GetSourceForLayout()
{
	return FindSourceFor( mTestName );
}

wxString MyFrame::FindSourceFor( const wxString& tag )
{
	if ( mCfg.mSrcFileContent.length() == 0 )
	{
		wxString fname = SRC_FILE_NAME;
		
		if ( !wxFileExists( SRC_FILE_NAME ) )
		{
			wxMessageBox( "File " + wxString( SRC_FILE_NAME ) + 
				          "not in the current directory, source code cannot be displayed!" );

			return "source file not found";
		}
		else
		{
			wxFile fl( SRC_FILE_NAME );

			if ( fl.IsOpened() )
			{
				char* buf = new char[ fl.Length() ];
				fl.Read( buf, fl.Length() );

				mCfg.mSrcFileContent = wxString( buf, fl.Length() );

				delete [] buf;
			}
			else
				return "cannot open file " + wxString(SRC_FILE_NAME) + " for reading";
		}
	}

	int i = mCfg.mSrcFileContent.First( "@@" + tag );

	if ( i == -1 ) return "source fragment not found!";

	i += tag.length() + 2;

	int k = mCfg.mSrcFileContent.First( "$$" + tag );

	if ( i == -1 ) return "end of source fragment not found!";

	return wxString( mCfg.mSrcFileContent, i, k - i - 2 );
}
