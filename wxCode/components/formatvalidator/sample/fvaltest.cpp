/////////////////////////////////////////////////////////////////////////////
// Name:        fvaltest.cpp
// Purpose:     Format and validate number sample
// Author:      Manuel Martin
// Modified by: MM Apr 2012
// Created:     Mar 2003
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#ifdef __GNUG__
    #pragma implementation "fvaltest.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------
// the application icon
#ifndef __WXMSW__
    #include "sample.xpm"
#endif

//other application headers
#include "wx/statline.h"
#include "wx/grid.h"
#include "wx/colordlg.h"
#include "forstrnu.h"
#include "fvalnum.h"
#include "fvalgred.h"

//this file header
#include "fvaltest.h"

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(FSN_QUIT,  MyFrame::OnQuit)
    EVT_MENU(FSN_DLG,   MyFrame::OnDialog)
    EVT_MENU(FSN_GRID,  MyFrame::OnGrid)
    EVT_MENU(FSN_BELL,  MyFrame::OnToggleBell)
    EVT_MENU(FSN_ABOUT, MyFrame::OnAbout)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(myPanel, wxPanel)
    EVT_BUTTON(FSVN_BUTF, myPanel::OnButtonFPressed)
    EVT_BUTTON(FSVN_BUTU, myPanel::OnButtonUPressed)
    EVT_BUTTON(FSVN_VRESET, myPanel::OnButtonVPressed)
    EVT_BUTTON(FSVN_VTOWIN, myPanel::OnButtonTTWPressed)
    EVT_BUTTON(FSVN_VFRWIN, myPanel::OnButtonTFWPressed)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyTextCtrl, wxTextCtrl)
    EVT_CHAR(MyTextCtrl::OnChar)
    EVT_IDLE(MyTextCtrl::OnIdle)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyTextColorSel, wxTextCtrl)
    EVT_LEFT_DOWN(MyTextColorSel::OnClick)
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


//global variables for validators
wxString stval(wxT(""));
wxString gvar1(wxT(""));
wxString gvar2(wxT(""));
wxString gvar3(wxT(""));
wxString gvar4(wxT(""));
//Choices for combo in dialog
wxArrayString cChoi;

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // create the main application window
    MyFrame *frame = new MyFrame(_("Format And Validate String As Number Sample"));

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title)
       : wxFrame((wxFrame *)NULL, wxID_ANY, title)
{
#ifdef __WXMAC__
    // we need this in order to allow the about menu relocation, since ABOUT is
    // not the default id of the about menu
    wxApp::s_macAboutMenuItemId = FSN_ABOUT;
#endif

    // set the frame icon
    SetIcon(wxICON(sample));

    // create a menu bar
    wxMenuBar *menuBar = new wxMenuBar();

    //create the menus in the menu bar
    wxMenu *menuFile = new wxMenu();
    menuFile->Append(FSN_QUIT, _("E&xit\tAlt-X"), _("Quit this program"));
    wxMenu *menuOpt = new wxMenu();
    menuOpt->AppendCheckItem(FSN_BELL, _("&Bell on error"), _("Toggle bell on error"));
    menuOpt->Check(FSN_BELL, true);
    wxMenu *menuDiag = new wxMenu;
    menuDiag->Append(FSN_DLG, _("Dialog with validation"), _("Create a wxDialog"));
    wxMenu *menuGrid = new wxMenu;
    menuGrid->Append(FSN_GRID, _("Validating in a grid"), _("Create a wxGrid"));
    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(FSN_ABOUT, _("&About...\tCtrl-A"), _("Show about dialog"));

    // now append the freshly created menu to the menu bar...
    menuBar->Append(menuFile, _("&File"));
    menuBar->Append(menuOpt , _("&Options"));
    menuBar->Append(menuDiag, _("&Dialog"));
    menuBar->Append(menuGrid, _("&Grid"));
    menuBar->Append(helpMenu, _("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

#if wxUSE_STATUSBAR
    // create a status bar (by default with 1 pane only)
    CreateStatusBar(1);
#endif // wxUSE_STATUSBAR

    //Add a panel to place things
    mPanel = new myPanel(this);
    //The sizer is set for the panel. Because the panel is the only child
    //of the frame, it will fill all frame's client area.
    //We also want to set the minimum size of this frame.
    mPanel->GetSizer()->SetSizeHints(this);

    //Initialize the GridFrame pointer
    gf = (GridFrame*) NULL;

    //Init the choices for the dialog
    cChoi.Add(wxT("10%"));
    cChoi.Add(wxT("50%"));
    cChoi.Add(wxT("80%"));
    cChoi.Add(wxT("max."));
}

/////////////////////////////////////////////////////////////////////////////
MyFrame::~MyFrame()
{
}

/////////////////////////////////////////////////////////////////////////////
void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

/////////////////////////////////////////////////////////////////////////////
void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf( wxT("Format And Validate String As Number Sample\n")
                wxT("version 2.0\n")
                wxT("Manuel Martin"));

    wxMessageBox(msg, wxT("About Format and Validate"), wxOK | wxICON_INFORMATION, this);
}

/////////////////////////////////////////////////////////////////////////////
void MyFrame::OnToggleBell(wxCommandEvent& WXUNUSED(event))
{
    bool opt = GetMenuBar()->IsChecked(FSN_BELL);
    wxValidator::SuppressBellOnError( !opt ); //for new validator
    mPanel->OnMenuBell( opt ); //for current
}

/////////////////////////////////////////////////////////////////////////////
void MyFrame::OnDialog(wxCommandEvent& WXUNUSED(event))
{
    MyDialog dlg(this);
    dlg.ShowModal();
}

/////////////////////////////////////////////////////////////////////////////
void MyFrame::OnGrid(wxCommandEvent& WXUNUSED(event))
{
    gf = new GridFrame(this);
    gf->Show();
}

// ----------------------------------------------------------------------------
// the panel inside the frame
// ----------------------------------------------------------------------------

myPanel::myPanel(wxWindow* parent)
             : wxPanel(parent, wxID_ANY,
                        wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
    SetBackgroundColour(wxColour(190,230,255));
    //Layout is done using several sizers.
    //The main, other sizers manager, vertical-expanding sizer
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    //A grid sizer to layout formatting samples
    wxGridSizer *gridSizer = new wxGridSizer(4, 5, 15); //4 cols, vgap=5, hgap=15

    wxStaticText* stAnyText = new wxStaticText(this, wxID_ANY, _("With this style"));
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, _("this number"));
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, _("gets formatted into"));
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, _("unformatted is"));
    gridSizer->Add(stAnyText, 0);
    mainSizer->Add(gridSizer, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 10);
    wxStaticLine* seline = new wxStaticLine(this);
    mainSizer->Add(seline, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

    //Let's make some samples
    gridSizer = new wxGridSizer(4, 5, 15); //4 cols, vgap=5, hgap=15
    wxString stastyle, stanum;
    wxString strFormatted, strUnFormatted;

    stastyle = wxT("[,###].'.'*");
    wxFormatStringAsNumber fsnsample(stastyle);

    stanum = wxT("1234567890.4321");
    fsnsample.FormatStr(stanum, strFormatted);
    fsnsample.UnFormatStr(strFormatted, strUnFormatted, wxUF_BESTRICT);
    stAnyText = new wxStaticText(this, wxID_ANY, stastyle);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, stanum);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, strFormatted);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, strUnFormatted);
    gridSizer->Add(stAnyText, 0);

    stastyle = wxT("[mM000].s's000bhbk#");
    stanum = wxT("12345678.4321");
    fsnsample.SetStyle(stastyle);
    fsnsample.FormatStr(stanum, strFormatted);
    fsnsample.UnFormatStr(strFormatted, strUnFormatted, wxUF_BESTRICT);
    stAnyText = new wxStaticText(this, wxID_ANY, stastyle);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, stanum);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, strFormatted);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, strUnFormatted);
    gridSizer->Add(stAnyText, 0);

    stastyle = wxT("-##0.','00E-* tru@ncat@ed");
    stanum = wxT("-1234.567e98");
    fsnsample.SetStyle(stastyle);
    fsnsample.FormatStr(stanum, strFormatted);
    fsnsample.UnFormatStr(strFormatted, strUnFormatted, wxUF_BESTRICT);
    stAnyText = new wxStaticText(this, wxID_ANY, stastyle);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, stanum);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, strFormatted);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, strUnFormatted);
    gridSizer->Add(stAnyText, 0);

    stastyle = wxT("@num i@s= [#].' and '0d0c+");
    double d = -1234.5598;
    stanum = wxString::FromCDouble(d);
    fsnsample.SetStyle(stastyle);
    fsnsample.FormatStr(stanum, strFormatted);
    fsnsample.UnFormatStr(strFormatted, strUnFormatted, wxUF_BESTRICT);
    stAnyText = new wxStaticText(this, wxID_ANY, stastyle);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, stanum);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, strFormatted);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, strUnFormatted);
    gridSizer->Add(stAnyText, 0);

    stastyle = wxT("@#[bB0 ]@-000e+bxbybz0+@#");
    stanum = wxT("12345678E+98");
    fsnsample.SetStyle(stastyle);
    fsnsample.FormatStr(stanum, strFormatted);
    fsnsample.UnFormatStr(strFormatted, strUnFormatted, wxUF_BESTRICT);
    stAnyText = new wxStaticText(this, wxID_ANY, stastyle);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, stanum);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, strFormatted);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, strUnFormatted);
    gridSizer->Add(stAnyText, 0);

    stastyle = wxT("+[#].'.'#;(000.','00)");
    stanum = wxT("12.3");
    fsnsample.SetStyle(stastyle);
    fsnsample.FormatStr(stanum, strFormatted);
    fsnsample.UnFormatStr(strFormatted, strUnFormatted, wxUF_BESTRICT);
    stAnyText = new wxStaticText(this, wxID_ANY, stastyle);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, stanum);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, strFormatted);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, strUnFormatted);
    gridSizer->Add(stAnyText, 0);

    stanum = wxT("-12.3");
    fsnsample.SetStyle(stastyle);
    fsnsample.FormatStr(stanum, strFormatted);
    fsnsample.UnFormatStr(strFormatted, strUnFormatted, wxUF_BESTRICT);
    stAnyText = new wxStaticText(this, wxID_ANY, stastyle);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, stanum);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, strFormatted);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, strUnFormatted);
    gridSizer->Add(stAnyText, 0);

    stastyle = wxT("-[#].'.'00N+## rou@nd@ed");
    stanum = wxT("-12375.444");
    fsnsample.SetStyle(stastyle);
    fsnsample.FormatStr(stanum, strFormatted, wxUF_BESTRICT, true);
    fsnsample.UnFormatStr(strFormatted, strUnFormatted, wxUF_BESTRICT);
    stAnyText = new wxStaticText(this, wxID_ANY, stastyle);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, stanum);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, strFormatted);
    gridSizer->Add(stAnyText, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, strUnFormatted);
    gridSizer->Add(stAnyText, 0);

    mainSizer->Add(gridSizer, 0, wxEXPAND | wxALL, 10);

    //A sizer, horizontal
    wxBoxSizer *sizerHor = new wxBoxSizer(wxHORIZONTAL);

    //Try your own style, number, result and unformatted
    wxButton* butA = new wxButton(this, FSVN_BUTF,
                                   _("Press here to test your own format"));
    sizerHor->Add(butA, 0, wxLEFT, 10);
    butA = new wxButton(this, FSVN_BUTU, _("Press here to test your own unformat"));
    sizerHor->Add(butA, 0, wxLEFT, 10);
    sizerHor->AddStretchSpacer();
    //A sizer, vertical
    wxBoxSizer *sizerVer = new wxBoxSizer(wxVERTICAL);
    beStrict = new wxCheckBox(this, wxID_ANY, _("Strict Mode"));
    sizerVer->Add(beStrict, 0);
    doRound = new wxCheckBox(this, wxID_ANY, _("Round off"));
    sizerVer->Add(doRound, 0, wxTOP, 5);
    sizerHor->Add(sizerVer, 0, wxRIGHT, 20);
    sizerHor->AddSpacer(20);
    //Another sizer, vertical
    sizerVer = new wxBoxSizer(wxVERTICAL);
    leftrim0 = new wxCheckBox(this, wxID_ANY, _("Trim zeros at left"));
    sizerVer->Add(leftrim0, 0);
    rigtrim0 = new wxCheckBox(this, wxID_ANY, _("Trim zeros at right"));
    sizerVer->Add(rigtrim0, 0, wxTOP, 5);
    sizerHor->Add(sizerVer, 0, wxRIGHT, 20);

    mainSizer->Add(sizerHor, 0, wxEXPAND);

    sizerHor = new wxBoxSizer(wxHORIZONTAL);

    yourstyle = new wxTextCtrl(this, wxID_ANY);
    sizerHor->Add(yourstyle, 1);
    sizerHor->AddSpacer(10);
    yournumb  = new wxTextCtrl(this, wxID_ANY);
    sizerHor->Add(yournumb, 1);
    sizerHor->AddSpacer(10);
    yourresul = new wxTextCtrl(this, wxID_ANY);
    sizerHor->Add(yourresul, 1);
    sizerHor->AddSpacer(10);
    yourunfor = new wxTextCtrl(this, wxID_ANY);
    sizerHor->Add(yourunfor, 1);

    mainSizer->Add(sizerHor, 0, wxEXPAND | wxALL, 10);
    mainSizer->AddSpacer(10);

    //Validate test
    sizerHor = new wxBoxSizer(wxHORIZONTAL);
    sizerVer = new wxBoxSizer(wxVERTICAL);
    stAnyText = new wxStaticText(this, wxID_ANY, _("VALIDATE"));
    sizerVer->Add(stAnyText, 0);
    butA = new wxButton(this, FSVN_VRESET, _("Use style(s), flags and others "));
    sizerVer->Add(butA, 0);
    sizerVer->AddSpacer(10);
    stAnyText = new wxStaticText(this, wxID_ANY, _("Normal style:"));
    sizerVer->Add(stAnyText, 0);
    //Here we can type the style used for validation
    stastyle = wxT("-[ ##0].'.'0000E+##");
    yourvalsty = new wxTextCtrl(this, wxID_ANY, stastyle);
    sizerVer->Add(yourvalsty, 0, wxEXPAND);
    stAnyText = new wxStaticText(this, wxID_ANY, _("Typing style:"));
    sizerVer->Add(stAnyText, 0, wxTOP, 5);
    stastyle = wxT("-[,###].'w'*e-##");
    yourvalstC = new wxTextCtrl(this, wxID_ANY, stastyle);
    sizerVer->Add(yourvalstC, 0, wxEXPAND);

    sizerHor->Add(sizerVer, 1, wxLEFT | wxRIGHT, 10);

    sizerVer = new wxBoxSizer(wxVERTICAL);
    butA = new wxButton(this, FSVN_VTOWIN, _("Transfer to validator window"));
    sizerVer->Add(butA, 0, wxALIGN_CENTER | wxBOTTOM, 5);
    butA = new wxButton(this, FSVN_VFRWIN, _("Transfer to var"));
    butA->SetToolTip(_("Attention: with no previous Validate() in this sample"));
    sizerVer->Add(butA, 0, wxALIGN_CENTER | wxBOTTOM, 5);
    stAnyText = new wxStaticText(this, wxID_ANY, _("String in var:"));
    sizerVer->Add(stAnyText, 0);
    // A control to show our global var 'stval'
    yourvaldat = new wxTextCtrl(this, wxID_ANY, stval);
    yourvaldat->SetToolTip(
            _("This string will only link with the validator using Transfer buttons"));
    sizerVer->Add(yourvaldat, 0, wxEXPAND);
    sizerVer->AddSpacer(5);
    stAnyText = new wxStaticText(this, wxID_ANY, _("Validator window:"));
    sizerVer->Add(stAnyText, 0);
    //The control we associate a wxNumberValidator with.
    //We use a wxTextCtrl derived because we want a default navigation for 'TAB'
    yourtest   = new MyTextCtrl(this, FSVN_VALTXT, wxT(""),
                      wxDefaultPosition, wxDefaultSize,
                      wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB,
                      wxNumberValidator(stastyle, wxVAL_DEFAULT, &stval) );

    //Just to see a msg when 'Esc' is used twice on 'yourtest'
    yourtest->Bind(wxEVT_KEY_DOWN, &myPanel::OnEscKey, this);

    sizerVer->Add(yourtest, 0, wxEXPAND);
    sizerHor->Add(sizerVer, 1, wxRIGHT, 10);

    //More flags
    sizerVer = new wxBoxSizer(wxVERTICAL);
    vOnChar  = new wxCheckBox(this, wxID_ANY, _("Validate On Char"));
    vOnChar->SetValue(true);
    vOnEnTab = new wxCheckBox(this, wxID_ANY, _("Validate On Enter/Tab"));
    vOnEnTab->SetValue(true);
    vOnKillF = new wxCheckBox(this, wxID_ANY, _("Validate On Kill Focus"));
    vOnKillF->SetValue(true);
    vOnRetaF = new wxCheckBox(this, wxID_ANY, _("Retain focus"));
    vOnRetaF->SetValue(true);
    vTransUn = new wxCheckBox(this, wxID_ANY, _("Transfer unformatted"));
    vTransUn->SetValue(true);
    vEscAct = new wxCheckBox(this, wxID_ANY, _("'Esc' active"));
    vEscAct->SetValue(true);
    vNoMsg = new wxCheckBox(this, wxID_ANY, _("Don't show error messages"));
    vNoMsg->SetValue(false);
    sizerVer->Add(vOnChar, 0, wxBOTTOM, 5);
    sizerVer->Add(vOnEnTab, 0, wxBOTTOM, 5);
    sizerVer->Add(vOnKillF, 0, wxBOTTOM, 5);
    sizerVer->Add(vOnRetaF, 0, wxBOTTOM, 5);
    sizerVer->Add(vTransUn, 0, wxBOTTOM, 5);
    sizerVer->Add(vEscAct, 0, wxBOTTOM, 5);
    sizerVer->Add(vNoMsg, 0, wxBOTTOM, 5);
    sizerVer->AddSpacer(7);
    //color selectors
    yourgoodcolor = new MyTextColorSel(this, FSVN_OKCOL, _("Colors when validation is OK"));
    yourgoodcolor->SetDefColors(yourtest->GetForegroundColour(),
                                 yourtest->GetBackgroundColour());
    yourgoodcolor->SetToolTip(_("Click here to change colors"));
    sizerVer->Add(yourgoodcolor, 0, wxEXPAND);
    yourfailcolor = new MyTextColorSel(this, FSVN_FACOL, _("Colors when validation failed"));
    yourfailcolor->SetDefColors(yourtest->GetForegroundColour(),
                                 yourtest->GetBackgroundColour());
    yourfailcolor->SetToolTip(_("Click here to change colors"));
    sizerVer->Add(yourfailcolor, 0, wxEXPAND | wxTOP, 5);

    sizerHor->Add(sizerVer, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);

    //Other options
    sizerVer = new wxBoxSizer(wxVERTICAL);
    vAllowEmpty = new wxCheckBox(this, wxID_ANY, _("Replace empty value with"));
    vAllowEmpty->SetValue(true);
    sizerVer->Add(vAllowEmpty, 0);
    stAnyText = new wxStaticText(this, wxID_ANY, _("(Unchecked means empty is invalid)"));
    sizerVer->Add(stAnyText, 0);
    yourvalRep = new wxTextCtrl(this, wxID_ANY);
    sizerVer->Add(yourvalRep, 0, wxEXPAND);
    sizerVer->AddSpacer(10);
    wxBoxSizer *sizerH2 = new wxBoxSizer(wxHORIZONTAL);
    stAnyText = new wxStaticText(this, wxID_ANY, _("Min value:"));
    sizerH2->Add(stAnyText, 1, wxALIGN_LEFT);
    vMinLimit = new wxCheckBox(this, wxID_ANY, _("Min limited"));
    sizerH2->Add(vMinLimit, 1, wxALIGN_RIGHT);
    sizerVer->Add(sizerH2, 0, wxEXPAND);
    yourvalmin = new wxTextCtrl(this, wxID_ANY);
    yourvalmin->SetToolTip(_("An unformatted value"));
    sizerVer->Add(yourvalmin, 0, wxEXPAND);
    sizerH2 = new wxBoxSizer(wxHORIZONTAL);
    stAnyText = new wxStaticText(this, wxID_ANY, _("Max value:"));
    sizerH2->Add(stAnyText, 1, wxALIGN_LEFT);
    vMaxLimit = new wxCheckBox(this, wxID_ANY, _("Max limited"));
    sizerH2->Add(vMaxLimit, 1, wxALIGN_RIGHT);
    sizerVer->Add(sizerH2, 0, wxEXPAND | wxTOP, 5);
    yourvalmax = new wxTextCtrl(this, wxID_ANY);
    yourvalmax->SetToolTip(_("An unformatted value"));
    sizerVer->Add(yourvalmax, 0, wxEXPAND);
    sizerVer->AddSpacer(7);
    stAnyText = new wxStaticText(this, wxID_ANY, _("Also allowed strings:"));
    sizerVer->Add(stAnyText, 0);
    yourvalAls = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
                                wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    sizerVer->Add(yourvalAls, 1, wxEXPAND);

    sizerHor->Add(sizerVer, 1, wxRIGHT, 10);
    mainSizer->Add(sizerHor, 0, wxEXPAND | wxBOTTOM, 10);

    //The layout of this panel is managed by this sizer
    SetSizer(mainSizer);
}

/////////////////////////////////////////////////////////////////////////////
myPanel::~myPanel()
{
}

/////////////////////////////////////////////////////////////////////////////
//The "format" button pressed
void myPanel::OnButtonFPressed(wxCommandEvent& WXUNUSED(event))
{
    int res;
    wxString testFormatted;
    wxFormatStringAsNumber usrtest;

    //Get style
    res = usrtest.SetStyle(yourstyle->GetValue());
    if (res != -1)
    {   wxMessageBox(usrtest.GetLastError(), _("Error in style"));
        yourstyle->SetInsertionPoint((long)res);
        yourstyle->SetFocus();
        return;
    }
    //Flags
    int bStric = beStrict->GetValue() ? wxUF_BESTRICT : wxUF_NOSTRICT;
    bool dRoun = doRound->GetValue() ? true : false;
    //Format with that style
    res = usrtest.FormatStr(yournumb->GetValue(), testFormatted, bStric, dRoun);
    if (res != -1)
    {   wxMessageBox(usrtest.GetLastError(), _("Error in number"));
        yournumb->SetInsertionPoint((long)res);
        yournumb->SetFocus();
        return;
    }
    yourresul->SetValue(testFormatted);

}

/////////////////////////////////////////////////////////////////////////////
//The "unformat" button pressed
void myPanel::OnButtonUPressed(wxCommandEvent& WXUNUSED(event))
{
    int res, bstric= wxUF_NOSTRICT, tz= 0;
    wxString testUnFormatted;
    wxFormatStringAsNumber usrtest;

    //Get style
    res = usrtest.SetStyle(yourstyle->GetValue());
    if (res != -1)
    {   wxMessageBox(usrtest.GetLastError(), _("Error in style"));
        yourstyle->SetInsertionPoint((long)res);
        yourstyle->SetFocus();
        return;
    }
    if (beStrict->GetValue())
        bstric = wxUF_BESTRICT;
    //Unformat with that style and behavior
    res = usrtest.UnFormatStr(yourresul->GetValue(), testUnFormatted, bstric);
    if (res != -1)
    {   //Let's show the unformatted string, even erroneous
        yourunfor->SetValue(testUnFormatted);
        wxMessageBox(usrtest.GetLastError(), _("Error in formatted number"));
        yourresul->SetInsertionPoint((long)res);
        yourresul->SetFocus();
        return;
    }
    //Ask the "formatter object" to trim zeros as indicated
    if (leftrim0->GetValue())
        tz = tz | wxTZ_LEFT;
    if (rigtrim0->GetValue())
        tz = tz | wxTZ_RIGHT;
    usrtest.TrimZeros(testUnFormatted, tz);

    yourunfor->SetValue(testUnFormatted);
}

/////////////////////////////////////////////////////////////////////////////
//Used for changing validator's style and behavior
void myPanel::OnButtonVPressed(wxCommandEvent& WXUNUSED(event))
{
    int res, vflags=0;

    wxNumberValidator *nva = (wxNumberValidator*) yourtest->GetValidator() ;
    res = nva->SetStyle(yourvalsty->GetValue());
    if (res != -1)
    {   wxMessageBox(nva->GetLastError(), _("Error in validate style"));
        yourvalsty->SetInsertionPoint((long)res);
        yourvalsty->SetFocus();
        return;
    }

    //Setting a special style for validating while typing
    res = nva->SetStyleForTyping(yourvalstC->GetValue());
    if (res != -1)
    {   wxMessageBox(nva->GetLastError(), _("Error in typing validate style"));
        yourvalstC->SetInsertionPoint((long)res);
        yourvalstC->SetFocus();
    }

    //Use all checkboxes values
    if (beStrict->GetValue())
        vflags |= wxUF_BESTRICT;
    else
        vflags |= wxUF_NOSTRICT;
    if (doRound->GetValue())
        vflags |= wxUF_ROUND;
    if (leftrim0->GetValue())
        vflags |= wxTZ_LEFT;
    if (rigtrim0->GetValue())
        vflags |= wxTZ_RIGHT;
    if (vOnChar->GetValue())
        vflags |= wxVAL_ON_CHAR;
    if (vOnEnTab->GetValue())
        vflags |= wxVAL_ON_ENTER_TAB;
    if (vOnKillF->GetValue())
        vflags |= wxVAL_ON_KILL_FOCUS;
    if (vOnRetaF->GetValue())
        vflags |= wxVAL_RETAIN_FOCUS;
    if (vTransUn->GetValue())
        vflags |= wxTRANSFER_UNFORMATTED;
    if (vNoMsg->GetValue())
        vflags |= wxVAL_NO_MSG_SHOWN;

    //Get lines from the multi-line wxTextCtrl
    wxArrayString arstr;
    wxString strAlso;
    for ( long nu = 0; nu <= yourvalAls->GetNumberOfLines(); nu++ )
    {
        strAlso = yourvalAls->GetLineText(nu);
        if ( !strAlso.IsEmpty() )
            arstr.Add(strAlso);
    }
    strAlso.Shrink();
    nva->SetAlsoValid(arstr);

    //More options
    nva->SetBehavior(vflags);
    //For 'Esc' key
    nva->RestoreWithEsc(vEscAct->GetValue());
    //For empty values
    nva->SetEmptyAs( vAllowEmpty->GetValue(), yourvalRep->GetValue() );
    //Min and max values
    if ( ! nva->SetMin(yourvalmin->GetValue(), vMinLimit->GetValue()) )
        wxMessageBox(_("Min value is not a number. Ignored"), _("Setting error"));
    if ( ! nva->SetMax(yourvalmax->GetValue(), vMaxLimit->GetValue()) )
        wxMessageBox(_("Max value is not a number. Ignored"), _("Setting error"));

    //The colors
    wxFValNumColors cols;
    cols.colOKForegn = yourgoodcolor->GetForegroundColour();
    cols.colOKBackgn = yourgoodcolor->GetBackgroundColour();
    cols.colFaForegn = yourfailcolor->GetForegroundColour();
    cols.colFaBackgn = yourfailcolor->GetBackgroundColour();
    nva->SetColors(cols);

    return;
}


/////////////////////////////////////////////////////////////////////////////
//Transfer from our global var to the control
void myPanel::OnButtonTTWPressed(wxCommandEvent& WXUNUSED(event))
{
    wxNumberValidator *nva = (wxNumberValidator*) yourtest->GetValidator() ;
    stval = yourvaldat->GetValue(); //from the control used to show "raw" var
    nva->TransferToWindow(); //to the control with validator associated
    return;
}

/////////////////////////////////////////////////////////////////////////////
//Validate and transfer from validator's control to window we use to show var
void myPanel::OnButtonTFWPressed(wxCommandEvent& WXUNUSED(event))
{
    wxNumberValidator *nva = (wxNumberValidator*) yourtest->GetValidator() ;
    //We don't call Validate(). This is not a good idea, but the reason for this
    // is just you show how TransferFromWindow() works without validation.
    nva->TransferFromWindow(); //global 'stval' is updated
    yourvaldat->SetValue(stval); //show it in its window
    return;
}

void myPanel::OnMenuBell(bool opt)
{
    //Update this flag in the current validator
    yourtest->GetValidator()->SuppressBellOnError( !opt );
}

void myPanel::OnEscKey(wxKeyEvent& event)
{
    if ( event.GetId() == FSVN_VALTXT
        && (event.GetKeyCode() == WXK_ESCAPE || event.GetKeyCode() == WXK_CANCEL)
       )
    {
        wxMessageBox(
            _("Default 'Esc' processing for\ntwo consecutive times cancellation.\n"),
                       _("Sample information") );
    }

    event.Skip();
}

//---------------------------------------------------------------------------
//    A specialized wxTextCtrl
//If we want our wxNumberValidator to handle TAB or ENTER events, the control
// must have wxTE_PROCESS_TAB and wxTE_PROCESS_ENTER flags. Otherwise, the
// 'KeyDown' and 'Char' events for these keys are not sent to the validator.
//But if our control has these flags, its behavior changes. So, TAB will be
// considered as a character and not as a navigation key.
//
//We change these flags, and restore them at OnIdle
//---------------------------------------------------------------------------
void MyTextCtrl::OnChar(wxKeyEvent& event)
{
    event.Skip();

    switch ( event.GetKeyCode() )
    {
        case WXK_RETURN:
        {
            //Stop processing. We avoid the possible 'beep'
            event.Skip(false);
            break;
        }

        case WXK_TAB:
        {
            if ( HasFlag(wxTE_PROCESS_TAB) )
                SetWindowStyleFlag( m_origFlags ^ wxTE_PROCESS_TAB );
        }
    }
}

void MyTextCtrl::OnIdle(wxIdleEvent& WXUNUSED(event))
{   //Restore original flags
     SetWindowStyleFlag( m_origFlags );
}

//---------------------------------------------------------------------------
// a dialog to select colors
//---------------------------------------------------------------------------
MyColorsDialog::MyColorsDialog(wxWindow* parent, const wxString& label)
                   : wxDialog(parent, wxID_ANY, label)
{
    wxBoxSizer *sizerVer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sizerHor = new wxBoxSizer(wxHORIZONTAL);
    butB = new wxButton(this, wxID_ANY,_("Select background color"));
    butF = new wxButton(this, wxID_ANY,_("Select foreground color"));
    sizerHor->Add(butB, 0, wxALL, 10);
    sizerHor->Add(butF, 0, wxALL, 10);
    sizerVer->Add(sizerHor, 0);

    sizerHor = new wxBoxSizer(wxHORIZONTAL);
    txct = new wxTextCtrl(this, wxID_ANY, _("Selected colors"));
    butRestore = new wxButton(this, wxID_ANY,_("Restore default colors"));
    sizerHor->Add(txct, 0, wxALIGN_CENTRE | wxALL, 10);
    sizerHor->Add(butRestore, 0, wxALIGN_CENTRE | wxALL, 10);
    sizerVer->Add(sizerHor, 0, wxALIGN_CENTRE);

    //Let's make buttons do something
    butB->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyColorsDialog::SelectAndSetColor, this);
    butF->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyColorsDialog::SelectAndSetColor, this);
    butRestore->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyColorsDialog::SelectAndSetColor, this);

    //OK and CANCEL buttons
    wxStdDialogButtonSizer *btsi = new wxStdDialogButtonSizer();
    btsi->AddButton(new wxButton(this, wxID_OK));
    btsi->AddButton(new wxButton(this, wxID_CANCEL));
    btsi->Realize();

    sizerVer->Add(btsi, 0, wxGROW | wxALL, 10);
    SetSizer(sizerVer);
    sizerVer->SetSizeHints(this);
}

//This dialog buttons handler
void MyColorsDialog::SelectAndSetColor(wxCommandEvent& event)
{
    if ( event.GetEventObject() == butRestore )
    {   //restore default colors
        defFcolor = wxNullColour;
        defBcolor = wxNullColour;
        txct->SetForegroundColour(wxNullColour);
        txct->SetBackgroundColour(wxNullColour);
        txct->Refresh();
        return;
    }
    //set colors
    wxColourDialog cdiag(this, NULL);
    if ( cdiag.ShowModal() == wxID_OK )
    {   //colors for this dialog token
        wxColourData cdat = cdiag.GetColourData();
        if ( event.GetEventObject() == butB )
        {   //store and set it
            defBcolor = cdat.GetColour();
            txct->SetBackgroundColour(defBcolor);
        }
        else
        {   //store and set it
            defFcolor = cdat.GetColour();
            txct->SetForegroundColour(defFcolor);
        }
        txct->Refresh();
    }
}


//---------------------------------------------------------------------------
//    A specialized wxTextCtrl to select colors
//---------------------------------------------------------------------------
//Because this text box has 'ReadOnly' style, the OS may assign different
//color as for a normal wxTextCtrl.
//But we want here to see the same colors, if they are the default.
void MyTextColorSel::SetFGColors(const wxColor& fcolor, const wxColor& bcolor)
{
    //Choose the default given colors (not the OS colors)
    wxColor useForeColor;
    useForeColor =  fcolor == wxNullColour ? m_myForeColor : fcolor;
    wxColor useBackColor;
    useBackColor =  bcolor == wxNullColour ? m_myBackColor : bcolor;
    //Set them
    SetForegroundColour(useForeColor);
    SetBackgroundColour(useBackColor);
    Refresh();
}

//store as the default colors, and set them
void MyTextColorSel::SetDefColors(const wxColor& fcolor, const wxColor& bcolor)
{
    m_myForeColor = fcolor;
    m_myBackColor = bcolor;
    SetForegroundColour(m_myForeColor);
    SetBackgroundColour(m_myBackColor);
}

void MyTextColorSel::OnClick(wxMouseEvent& event)
{
    wxString st;
    if (event.GetId() == FSVN_OKCOL)
        st = _("Good validation colors");
    else
        st = _("Failed validation colors");
    MyColorsDialog colorDiag(GetParent(), st);
    if ( colorDiag.ShowModal() == wxID_OK )
    {   //Set the colors to the text control that was clicked
        SetFGColors(colorDiag.defFcolor, colorDiag.defBcolor);
    }
}

//---------------------------------------------------------------------------
//    Dialog with validation
//
// Some examples with different behaviors
//---------------------------------------------------------------------------

MyDialog::MyDialog(MyFrame* parent)
                   : wxDialog(parent, FSVN_DIALOG, _("Validation dialog"),
                       wxDefaultPosition, wxDefaultSize,
                       wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    //Layout using 3 sizers
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *upSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);

    //For an integer like "-12.345.678"
    //Its validation is done only when OK dialog's button is pressed
    wxTextCtrl *txct = new MyTextCtrl(this, wxID_ANY, wxEmptyString,
                            wxDefaultPosition, wxDefaultSize, 0,
            // In this case Enter/Tab are not handled, so no need of
            // the flags wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB
                            wxNumberValidator(wxT("-[@.###]"),
                            wxUF_BESTRICT, &gvar1));

    txct->SetToolTip(_("Any formatted integer. Use '.' as thousands delimiter"));
    leftSizer->Add(txct, 0, wxEXPAND | wxALL, 10);

    //A reusable pointer to validator
    wxNumberValidator *nuvt = new wxNumberValidator(wxT("-[,###].d.d00E-*"),
                                    wxVAL_DEFAULT | wxVAL_ON_KILL_FOCUS, &gvar2);
    //colors
    wxFValNumColors stcolors;
    stcolors.colOKForegn = wxColour(0, 0, 255); //Blue
    stcolors.colOKBackgn = wxColour(160, 255, 160); //Light green
    stcolors.colFaForegn = wxColour(255, 0, 0); //Red
    stcolors.colFaBackgn = wxColour(255, 160, 255); //Light pink
    nuvt->SetColors(stcolors);
    //For any floating number
    txct = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
                            wxDefaultPosition, wxDefaultSize,
                            wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB,
                            *nuvt);

    delete nuvt; //Validator has been cloned, so ours can be deleted
    txct->SetToolTip(_("Any formatted, two decimals, floating, like -1,234.56E98"));
    leftSizer->Add(txct, 0, wxEXPAND | wxALL, 10);

    //Real number, "INF" "NAN" "PI" included
    wxArrayString sInc;
    sInc.Add(wxT("PI"));
    sInc.Add(wxT("INF"));
    sInc.Add(wxT("NAN"));
    nuvt =  new wxNumberValidator(wxT("+[ ###].d,d00*E-*"), //at least 2 dec.
                                    wxVAL_ON_KILL_FOCUS, &gvar3);
    nuvt->SetStyleForTyping(wxT("-[,###].d.d##E-*")); //no forced decimals
    nuvt->SetAlsoValid(sInc);
    txct = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
                            wxDefaultPosition, wxDefaultSize,
                            wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB,
                            *nuvt);
    delete nuvt;
    txct->SetToolTip(_("Any number. The words 'INF' 'NAN' 'PI' are valid too"));
    leftSizer->Add(txct, 0, wxEXPAND | wxALL, 10);

    upSizer->Add(leftSizer, 1, wxEXPAND);

    //A colored combobox with validation. It also retains focus on error.
    sInc.Clear();
    sInc.Add(cChoi.Last()); //The string "max."
    nuvt =  new wxNumberValidator(wxT("##0.'.'*%"),
                                    wxVAL_RETAIN_FOCUS | wxVAL_ON_EDIT, &gvar4);
    nuvt->SetColors(stcolors);
    nuvt->SetMin(wxT("0"));
    nuvt->SetMax(wxT("100"));
    nuvt->SetAlsoValid(sInc);
    nuvt->SetEmptyAs(false);
    wxComboBox *coBx = new wxComboBox(this, wxID_ANY, wxEmptyString,
                            wxDefaultPosition, wxDefaultSize, cChoi,
                            wxCB_DROPDOWN | wxTE_PROCESS_ENTER,
                            *nuvt);
    delete nuvt;
    coBx->SetToolTip(_("Floating between 0 and 100"));

    upSizer->Add(coBx, 1, wxALL, 10);

    mainSizer->Add(upSizer, 1, wxEXPAND);

    //OK and CANCEL buttons
    wxStdDialogButtonSizer *btsi = new wxStdDialogButtonSizer();
    btsi->AddButton(new wxButton(this, wxID_OK));
    btsi->AddButton(new wxButton(this, wxID_CANCEL));
    btsi->Realize();

    mainSizer->Add(btsi, 0, wxGROW | wxALL, 10);

    SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);
}

//---------------------------------------------------------------------------
//    Validating in a wxGrid
//---------------------------------------------------------------------------

GridFrame::GridFrame(MyFrame* mainWin)
                    : wxFrame(mainWin, FSVN_GRID, _("Validating grid"))
{
    m_grid = new wxGrid(this, FSVN_GRID);
    m_grid->SetDefaultColSize(100);
    m_grid->CreateGrid(3, 4);
    m_grid->SetColLabelValue(0, _("Only integers\ne.g. 1 234"));
    m_grid->SetColLabelValue(1, _("Max = 19,999.99"));
    m_grid->SetColLabelValue(2, _("Two substyles\n(try +/- values)"));
    m_grid->SetColLabelValue(3, _("Choose your\ninteger < 1000"));
    m_grid->SetDefaultCellAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);

    /*NOTE:
    *  In this grid example, we set wxVAL_ON_KILL_FOCUS to all validators,
    *  directly or resembling it with a "style for typing". So, you can
    *  quickly see the results of your actions.
    *  Not doing this way, would require some job on your own to get
    *  valid values from those retrieved from the grid table.
    */

    //Once the validator is passed to the CellEditor, it is copied and then
    //we can delete it. So, we can use a pointer.
    wxNumberValidator *vali = new wxNumberValidator();

    //Create a validator for integers
    vali->SetBehavior(wxVAL_DEFAULT | wxVAL_ON_KILL_FOCUS);
    vali->SetStyle(wxT("-[ ###]"));
    //Create an editor with this validator. We pass "false" to indicate its
    //behavior is the wxGrid typical, i.e. not allowing an invalid value.
    wxGridCellFormattedNumEditor *cEdt1 =
            new wxGridCellFormattedNumEditor(*vali, false);
    //Create an attributes object
    wxGridCellAttr *cellAtt1 = new wxGridCellAttr();
    //Set the editor for this object
    cellAtt1->SetEditor(cEdt1);
    //Use it in all the first column
    m_grid->SetColAttr(0, cellAtt1);

    //Adapt the validator for another use
    vali->SetBehavior(wxVAL_ON_EDIT | wxVAL_ON_KILL_FOCUS);
    vali->SetStyle(wxT("-[,###].'.'*E-##"));
    wxFValNumColors stcolors;
    stcolors.colOKForegn = wxColour(0, 0, 255); //Blue
    stcolors.colOKBackgn = wxColour(170, 255, 170); //Light green
    stcolors.colFaForegn = wxColour(255, 0, 0); //Red
    stcolors.colFaBackgn = wxColour(255, 170, 255); //Light pink
    vali->SetColors(stcolors);
    vali->SetMax(wxT("19999.99"));
    //Create an new editor with this validator. We pass "true" to indicate its
    //behavior is not the wxGrid typical, because an invalid value is allowed.
    cEdt1 =  new wxGridCellFormattedNumEditor(*vali, true);
    //Create a new attributes object
    cellAtt1 = new wxGridCellAttr();
    //Set the editor for this object
    cellAtt1->SetEditor(cEdt1);
    //Use it in all the second column
    m_grid->SetColAttr(1, cellAtt1);

    //Using two styles. We want also two sub-styles for "normal" style; so
    //wxUF_BESTRICT is needed. wxVAL_DEFAULT uses it.
    delete vali;
    vali = new wxNumberValidator(wxT("+[,b b 0].'.'00;([,000].'.'00)"));
    vali->SetStyleForTyping(wxT("-[,###].'.'##"));
    cEdt1 =  new wxGridCellFormattedNumEditor(*vali, true);
    cellAtt1 = new wxGridCellAttr();
    cellAtt1->SetEditor(cEdt1);
    m_grid->SetColAttr(2, cellAtt1);

    //With wxComboBox
    delete vali;
    vali = new wxNumberValidator(wxT("###"), wxVAL_DEFAULT | wxVAL_ON_KILL_FOCUS);
    wxArrayString sChoices;
    sChoices.Add(wxT("NULL"));
    sChoices.Add(wxT("1"));
    sChoices.Add(wxT("2"));
    sChoices.Add(wxT("4"));
    sChoices.Add(wxT("8"));
    wxArrayString sAValid;
    sAValid.Add( sChoices.Item(0) ); //allow "NULL" entry
    vali->SetAlsoValid(sAValid);
    vali->SetEmptyAs(true, sChoices.Item(0));
    wxGridCellFormattedChoiceEditor
     *cEdt2 =  new wxGridCellFormattedChoiceEditor(*vali, false, sChoices);
    cellAtt1 = new wxGridCellAttr();
    cellAtt1->SetEditor(cEdt2);
    m_grid->SetColAttr(3, cellAtt1);

    //delete our reused validator
    delete vali;

    m_grid->ForceRefresh();
    m_grid->Fit();
    SetClientSize(m_grid->GetSize());
}

GridFrame::~GridFrame()
{
}

