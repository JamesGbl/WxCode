/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     awxtest
// Author:      Joachim Buermann
// Id:          $Id$
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#include <wx/cmdline.h>

#include <wx/awx-0.3/button.h>
#include <wx/awx-0.3/choice.h>
#include "main.h"
#include "menu.h"
// program icon
#include "icons/32x32/awxtest.xpm"
// toolbar icons
#include "icons/32x32/connect.xpm"
#include "icons/32x32/disconnect.xpm"
#include "icons/32x32/fileopen.xpm"
#include "icons/32x32/fileopen_dis.xpm"
#include "icons/32x32/filesaveas.xpm"
#include "icons/32x32/filesaveas_dis.xpm"
#include "icons/32x32/quit.xpm"
#include "icons/32x32/settings.xpm"
#include "icons/32x32/settings_dis.xpm"
#include "icons/32x32/view_icon.xpm"
#include "icons/32x32/view_tree.xpm"

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_QUIT, MyFrame::OnQuit)
    EVT_MENU(ID_TOGGLE, MyFrame::OnToggle)
    EVT_MENU(ID_VIEWICON, MyFrame::OnViewIcon)
    EVT_MENU(ID_VIEWTREE, MyFrame::OnViewTree)
    EVT_MENU(ID_TB_TEXT, MyFrame::OnToolbarText)
    EVT_MENU(ID_SETTINGS, MyFrame::OnSettings)
    EVT_CHOICE(ID_CHOICE, MyFrame::OnChoice)
END_EVENT_TABLE()
    
IMPLEMENT_APP(MyApp)

// -------------------------------------------------------------------
// globals
// -------------------------------------------------------------------

bool g_theme = true;

// -------------------------------------------------------------------
// MyApp
// -------------------------------------------------------------------

static const wxCmdLineEntryDesc cmdLineDesc[] =
{
    { wxCMD_LINE_SWITCH,wxT("c"),wxT("classic"),wxT("classic toolbar design")},
    { wxCMD_LINE_SWITCH,wxT("d"),wxT("debug"),wxT("print some debug informations")},
    { wxCMD_LINE_NONE }
};


bool MyApp::OnInit()
{
    wxCmdLineParser parser(argc,argv);
    parser.SetDesc(cmdLineDesc);
    parser.Parse();
    if(parser.Found(wxT("c"))) g_theme = false;
    
    MyFrame *frame = new MyFrame(wxT("awx example"),
						   wxDefaultPosition,
						   wxDefaultSize);

    wxLogWindow* log = NULL;
    if(parser.Found(wxT("d"))) {
	   log = new wxLogWindow(frame,wxT("Debug output"));
    }

    frame->Show(true);
    SetTopWindow(frame);

    return true;
};


// -------------------------------------------------------------------
// MyFrame
// -------------------------------------------------------------------

MyFrame::MyFrame(const wxString& title,
			  const wxPoint& pos, 
			  const wxSize& size) :
    wxFrame((wxFrame*)NULL,-1,title,pos,size,
		  wxDEFAULT_FRAME_STYLE)
{
    SetIcon(wxICON(awxtest));

    SetBackgroundColour(wxSystemSettings::GetColour(
					   wxSYS_COLOUR_BTNFACE));

    // create a menu bar
    SetMenuBar(CreateMenuBar());

    // topsizer
    wxBoxSizer* top = new wxBoxSizer(wxVERTICAL);
    // create a toolbar
    m_toolbar = new awxToolbar(this,-1);
    CreateToolbar(m_toolbar);
    top->Add(m_toolbar,0,wxEXPAND|wxALL,0);
    // Textcontrol for log messages
    m_log = new wxTextCtrl(this,-1,wxT(""),wxDefaultPosition,wxDefaultSize,
					  wxTE_MULTILINE | wxTE_READONLY);
    top->Add(m_log,1,wxEXPAND|wxALL,0);
    // tell dialog to use sizer
    SetAutoLayout(true);
    // actually set the sizer
    SetSizer(top);
    top->Fit(this);

    m_log->AppendText(wxT("AWX (Addition wxWidgets Library) Test program\n"
					 "Copyright (c) Joachim Bürmann\n"
					 "For more information, see http://www.iftools.com\n\n"
					 "You can start awxtest with -c parameter for "
					 "classic toolbar look\n"));
};

MyFrame::~MyFrame()
{
};

int MyFrame::CreateToolbar(awxToolbar* mtb,bool showText)
{
    awxButton* mb;
    mtb->EnableTheme(g_theme);
    if(!showText) mtb->SetExtraSpace(2);
    // Quit button
    mb = new awxButton(mtb,ID_QUIT,
				   wxDefaultPosition,wxDefaultSize,
				   quit_xpm,
				   NULL,
				   NULL,
				   quit_xpm);
    mb->SetToolTip(_("Quit"));
    if(showText) mb->SetText(_("Quit"));
    mtb->Add(mb);
    // ---------------------------------
    mtb->Add(new awxSeparator(mtb));
    // Connect
    mb = new awxCheckButton(mtb,ID_TOGGLE,
					   wxDefaultPosition,wxDefaultSize,
					   disconnect_xpm,
					   NULL,
					   connect_xpm,
					   disconnect_xpm);
    mb->SetToolTip(_("(Dis)Connect"));
    mtb->Add(mb);
    // File open button
    mb = new awxButton(mtb,ID_FILEOPEN,
				   wxDefaultPosition,wxDefaultSize,
				   fileopen_xpm,
				   NULL,
				   NULL,
				   fileopen_dis_xpm);
    mb->SetToolTip(_("File open"));
    mtb->Add(mb);
    // File save button
    mb = new awxButton(mtb,ID_FILESAVE,
				   wxDefaultPosition,wxDefaultSize,
				   filesaveas_xpm,
				   NULL,
				   NULL,
				   filesaveas_dis_xpm);
    mb->SetToolTip(_("File save as..."));
    if(showText) mb->SetText(_("Save as"));
    mb->Enable(false);
    mtb->Add(mb);
    
    wxString selection[3] = {
	   wxT("icon"),
	   wxT("tree"),
	   wxT("disabled")
    };
    awxChoice *choice = new awxChoice(mtb,
							   ID_CHOICE,
							   wxDefaultPosition,
							   wxDefaultSize,
							   3,selection);
    mtb->Add(choice);
    choice->EnableItem(2,false);

    // Switch (radio) buttons
    awxRadioButtonBox* m_box = new awxRadioButtonBox(mtb,ID_VIEW);
    // View as icon
    mb = new awxRadioButton(mtb,ID_VIEWICON,
					   wxDefaultPosition,wxDefaultSize,
					   view_icon_xpm,
					   NULL,
					   NULL,
					   view_icon_xpm,
					   m_box
					   );
    mb->SetToolTip(_("View as icons"));
    if(showText) mb->SetText(_("Icon view"));
    mtb->Add(mb);
    // View as tree
    mb = new awxRadioButton(mtb,ID_VIEWTREE,
					   wxDefaultPosition,wxDefaultSize,
					   view_tree_xpm,
					   NULL,
					   NULL,
					   view_tree_xpm,
					   m_box
					   );
    mb->SetToolTip(_("View as tree"));
    if(showText) mb->SetText(_("Tree view"));
    mtb->Add(mb);
    // Settings button
    mb = new awxButton(mtb,ID_SETTINGS,
				   wxDefaultPosition,wxDefaultSize,
				   settings_xpm,
				   NULL,
				   NULL,
				   settings_dis_xpm);
    mb->SetToolTip(_("Setup"));
    if(showText) mb->SetText(_("Settings"));
    mtb->Add(mb);

    m_box->SetValue(ID_VIEWICON);

    mtb->Redraw();
    return 0;
};

void MyFrame::OnChoice(wxCommandEvent& event)
{
    awxRadioButtonBox* box = (awxRadioButtonBox*)FindWindow(ID_VIEW);
    if(box) {
	   if(event.GetSelection() == 0) {
		  box->SetValue(ID_VIEWICON);
	   }
	   else {
		  box->SetValue(ID_VIEWTREE);
	   }
    }
};

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
};

void MyFrame::OnSettings(wxCommandEvent& WXUNUSED(event))
{
    prefs_t prefs = m_prefs;
    if(Preferences(this,prefs)) {
	   // ok, take over the settings
	   m_prefs = prefs;
	   m_log->AppendText(wxT("Save dialog settings\n"));
    }
    else {
	   m_log->AppendText(wxT("Discard dialog settings\n"));
    }
};

void MyFrame::OnToolbarText(wxCommandEvent& event)
{
    delete m_toolbar;
    m_toolbar = new awxToolbar(this,-1);
    CreateToolbar(m_toolbar,event.GetInt() > 0);
    GetSizer()->Prepend(m_toolbar,0,wxEXPAND|wxALL,0);
    Layout();
    m_toolbar->Redraw();
};

void MyFrame::OnToggle(wxCommandEvent& event)
{
    // state represent the current state of the toggle button
    FindWindow(ID_FILESAVE)->Enable(event.GetInt() > 0);
    m_log->AppendText(wxT("A checkbutton with two states, every state has it's "
				  "own bitmap\n"));
};

void MyFrame::OnViewIcon(wxCommandEvent& WXUNUSED(event))
{
    m_log->AppendText(wxT("Radio button icon view pressed\n"));
};

void MyFrame::OnViewTree(wxCommandEvent& WXUNUSED(event))
{
    m_log->AppendText(wxT("Radio button tree view pressed\n"));
};
