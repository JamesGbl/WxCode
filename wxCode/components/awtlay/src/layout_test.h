/////////////////////////////////////////////////////////////////////////////
// Name:        layout_test.h
// Purpose:     
// Author:      Aleksandras Gluchovas
// Modified by:
// Created:     2000/1/15
// RCS-ID:      
// Copyright:   (c) Aleksandras Gluchovas
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __LAYOUT_TEST_G__
#define __LAYOUT_TEST_G__

#include "wx/font.h"
#include "wx/dialog.h"
#include "wx/frame.h"
#include "wx/fontdlg.h"
#include "wx/file.h"

#include "awt_emul.h"

#include "absolutelayout.h"
#include "gridlayout.h"
#include "borderlayout.h"
#include "boxlayout.h"
#include "flowlayout.h"
#include "gridbaglayout.h"
#include "cardlayout.h"

#include <stdlib.h> // atoi(..)
#include <stdio.h>  // sprintf(..)

class MyFrame;

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

struct TestConfig
{
public:
	TestConfig();

	int mVgap;
	int mHgap;
	Insets mInsets;
	int mRowCount;
	int mColCount;

	bool   mUseLightWeight;
	bool   mUseDoubleBuf;
	bool   mShowSrcCode;
	bool   mAutoPack;
	bool   mUseTwoPhase;
	wxFont *mpFont;
	wxString mSrcFileContent;
};

#define SRC_FILE_NAME "layout_test.cpp"

class SourceWindow : public wxFrame
{
public:
	SourceWindow( MyFrame* pFrm );
	~SourceWindow();

	void SetSource( const wxString& src, const wxString& srcName );
	void Recreate();
	void OnClose( wxCommandEvent& event );

	DECLARE_EVENT_TABLE();

protected:

	wxString mSrcName;
	Container* mpParent;
	wxTextCtrl* mpSrc;
	wxWindow* mpBox;
	MyFrame* mpFrm;
};

class ConfigDialog : public wxDialog
{
public:
	ConfigDialog( wxWindow* pParent, TestConfig* pCfg );

	void AddControls();
	void ExchgCheck( wxCheckBox* pChk, bool& value );
	void ExchgIntField( wxTextCtrl* pFld, int& value );

	void ReadConfig();
	void ApplyConfig();
	void DoExchangeFields();

	void OnSelectFont( wxCommandEvent& event );
	void OnShowDlgSource( wxCommandEvent& event );
	void OnApply( wxCommandEvent& event );
	void OnApplyAndExit( wxCommandEvent& event );
	void OnSize( wxSizeEvent& event );
	void OnMove( wxMoveEvent& event );

	wxButton* MakeButton( const wxString& name, int id );
	wxTextCtrl* MakeField( wxTextCtrl*& pCtl );
	wxCheckBox* MakeCheckBox( const wxString& name, wxCheckBox*& pCheck );


protected:
	TestConfig* mpCfg;
	wxTextCtrl* mpHgap;
	wxTextCtrl* mpVgap;
	wxTextCtrl* mpLeft;
	wxTextCtrl* mpRight;
	wxTextCtrl* mpTop;
	wxTextCtrl* mpBottom;
	wxTextCtrl* mpRowCount;
	wxTextCtrl* mpColCount;
	wxTextCtrl* mpFont;
	wxCheckBox* mpUseLightWeight;
	wxCheckBox* mpUseDoubleBuf;
	wxCheckBox* mpShowSrcCode;
	wxCheckBox* mpAutoPack;
	wxCheckBox* mpUseTwoPhase;

	wxButton* mpSelectFont;

	wxFont*   mpFontObj;

	SourceWindow* mpSrcWnd;

	bool mToDlg;

	enum { ID_SELECT_FONT = 250,
		   ID_SHOW_DLG_SOURCE,
		   ID_APPLY_AND_EXIT,
	     };

	DECLARE_EVENT_TABLE();
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	~MyFrame();

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

	void Test_AbsoluteLayout( wxCommandEvent& event );
	void Test_BorderLayout( wxCommandEvent& event );
	void Test_BoxLayout_horiz( wxCommandEvent& event );
	void Test_BoxLayout_vert( wxCommandEvent& event );
	void Test_CardLayout( wxCommandEvent& event );
	void Test_CardLayout_flip( wxCommandEvent& event );
	void Test_FlowLayout( wxCommandEvent& event );
	void Test_GridBagLayout( wxCommandEvent& event );
	void Test_GridBagLayout_1( wxCommandEvent& event );
	void Test_GridBagLayout_2( wxCommandEvent& event );
	void Test_GridLayout( wxCommandEvent& event );

	void Test_All_1( wxCommandEvent& event );
	void Test_All_2( wxCommandEvent& event );

	void Test_Packing( wxCommandEvent& event );
	void SetTestName( const wxString& name );

public:

	void RemoveAll();
	Component* CreateComp( String name, int width = -1, int height = -1 );

    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()

public:
	Container* parent;

	wxString IntToStr( int i ) { char buf[32]; sprintf(buf,"%d",i); return buf; }

	Component* MakeListBox( char** values, int nValues );
	Component* MakeComboBox( char** values, int nValues );
	Container* MakeStaticBox( const wxString& name );
	Component* MakeButton( const wxString& name );
	Component* MakeTextField( const wxString& value );
	Component* MakeLabel( const wxString& value );
	Component* MakeRadioButton( const wxString& name, bool selected = FALSE );
	Component* MakeCheckBox( const wxString& value, bool checked = TRUE );

	Component* SetCurrentContainer( Container* pCont );

	void OnMiscSettings( wxCommandEvent& event );
	void OnLightWeight( wxCommandEvent& event );
	void OnDoubleBuf( wxCommandEvent& event );
	void OnAutoPack( wxCommandEvent& event );
	void OnShowSource( wxCommandEvent& event );
	void OnSetFont( wxCommandEvent& event );

	void SetMenuChecks();
	void ShowSource();
	void RecreateLayout();
	void DoShowSource();
	wxString GetSourceForLayout();
	void DoPack( Container* pCont );

	Insets GetInsetsForStaticBox( Container* pStaticBox );

	TestConfig& GetConfig() { return mCfg; }
	SourceWindow* GetSourceWindow() { return mpSrcWnd; }
	void SetSourceWindow( SourceWindow* pWnd ) { mpSrcWnd = pWnd; }
	bool IsDestroying() { return mIsDestroying; }

	wxString FindSourceFor( const wxString& tag );

	void ReapplyConfig();

	bool          mIsDestroying;
	Container*    mpCurCont;
	SourceWindow* mpSrcWnd;

	wxString mTestName;

	TestConfig mCfg;
};

// IDs for the controls and the menu commands
enum
{
    // menu items
    Minimal_Quit = 1,
    Minimal_About,

	Test_absolute = 200,
	Test_card,
	Test_card_flip,
	Test_border,
	Test_box_vert,
	Test_box_horiz,
	Test_flow,
	Test_gridbag,
	Test_gridbag_1,
	Test_gridbag_2,
	Test_grid,
	Test_all_1,
	Test_all_2,
	Test_packing,

	Settings_setup,
	Settings_lightweight,
	Settings_doublebuf,
	Settings_autopack,
	Settings_showsrc,
	Settings_setfont,
};

#endif // __LAYOUT_TEST_G__