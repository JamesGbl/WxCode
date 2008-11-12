/////////////////////////////////////////////////////////////////////////////
// Name:        demo.cpp
// Purpose:
// Author:      Moskvichev Andrey V.
// Created:     2008/10/09
// RCS-ID:      $Id: wxAdvTable.h,v 1.3 2008/10/09 16:42:58 frm Exp $
// Copyright:   (c) 2008 Moskvichev A.V.
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "demo.h"
#include <wx/aui/aui.h>

/**
 * Demo application.
 */
class DemoApp : public wxApp
{
public:
	bool OnInit()
	{
		MainFrame *mainFrame = new MainFrame();

		mainFrame->Show(true);
		SetTopWindow(mainFrame);
		return true;
	}
};

IMPLEMENT_APP(DemoApp);

//
// ControlPanel
//

#define ADD_FIELD(parent, sizer, label, ctrl) do {			\
	sizer->Add(new wxStaticText(parent, wxID_ANY, label),	\
		0, wxEXPAND);										\
	sizer->Add(ctrl,										\
		0, wxEXPAND);										\
} while (0)

enum CONTROL_IDS {
	CHOICE_SELECT_MODE = 101,
	CHOICE_HIGHLIGHT_MODE,
	CHECK_SHOW_ROWS,
	CHECK_SHOW_COLS,
	CHECK_SHOW_CORNER,
};

BEGIN_EVENT_TABLE(ControlPanel, wxPanel)
	EVT_CHOICE(CHOICE_SELECT_MODE, ControlPanel::OnChoiceSelectMode)
	EVT_CHOICE(CHOICE_HIGHLIGHT_MODE, ControlPanel::OnChoiceHighlightMode)
	EVT_CHECKBOX(CHECK_SHOW_ROWS, ControlPanel::OnCheckShowRows)
	EVT_CHECKBOX(CHECK_SHOW_COLS, ControlPanel::OnCheckShowCols)
	EVT_CHECKBOX(CHECK_SHOW_CORNER, ControlPanel::OnCheckShowCorner)
END_EVENT_TABLE()

ControlPanel::ControlPanel(wxWindow *parent, wxAdvTable *advTable)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(100, 200))
{
	m_advTable = advTable;

	wxSizer *sizer = new wxFlexGridSizer(0, 1, 5, 5);
	wxSizer *choiceSizer = new wxFlexGridSizer(0, 2, 5, 5);

	wxArrayString choicesSelectMode;
	choicesSelectMode.Add(wxT("SelectCell"));
	choicesSelectMode.Add(wxT("SelectRows"));
	choicesSelectMode.Add(wxT("SelectCols"));
	choicesSelectMode.Add(wxT("SelectBlock"));
	ADD_FIELD(this, choiceSizer, wxT("Select mode"),
			new wxChoice(this, CHOICE_SELECT_MODE, wxDefaultPosition, wxDefaultSize,
					choicesSelectMode));

	wxArrayString choicesHighlightMode;
	choicesHighlightMode.Add(wxT("HighlightNone"));
	choicesHighlightMode.Add(wxT("HighlightRows"));
	choicesHighlightMode.Add(wxT("HighlightCols"));
	choicesHighlightMode.Add(wxT("HighlightBoth"));
	ADD_FIELD(this, choiceSizer, wxT("Highlight mode"),
			new wxChoice(this, CHOICE_HIGHLIGHT_MODE, wxDefaultPosition, wxDefaultSize,
					choicesHighlightMode));

	sizer->Add(choiceSizer,
			0, wxEXPAND);

	wxCheckBox *checkBox;
	checkBox = new wxCheckBox(this, CHECK_SHOW_ROWS, wxT("Show rows"), wxDefaultPosition, wxDefaultSize);
	checkBox->SetValue(m_advTable->GetShowRows());
	sizer->Add(checkBox,
			0, wxEXPAND);

	checkBox = new wxCheckBox(this, CHECK_SHOW_COLS, wxT("Show columns"), wxDefaultPosition, wxDefaultSize);
	checkBox->SetValue(m_advTable->GetShowCols());
	sizer->Add(checkBox,
			0, wxEXPAND);

	checkBox = new wxCheckBox(this, CHECK_SHOW_CORNER, wxT("Show corner"), wxDefaultPosition, wxDefaultSize);
	checkBox->SetValue(m_advTable->GetShowCorner());
	sizer->Add(checkBox,
			0, wxEXPAND);

	sizer->SetSizeHints(this);
	SetSizer(sizer);
}

ControlPanel::~ControlPanel()
{
}

void ControlPanel::OnChoiceSelectMode(wxCommandEvent &ev)
{
	int mode;

	switch (ev.GetSelection()) {
	case 0:
		mode = wxAdvTable::SelectCell;
		break;
	case 1:
		mode = wxAdvTable::SelectRows;
		break;
	case 2:
		mode = wxAdvTable::SelectCols;
		break;
	case 3:
		mode = wxAdvTable::SelectBlock;
		break;
	default:
		return ;
	}

	m_advTable->SetSelectMode((wxAdvTable::SelectMode) mode);
	wxLogMessage(wxT("Select mode %s"), ev.GetString().c_str());
}

void ControlPanel::OnChoiceHighlightMode(wxCommandEvent &ev)
{
	int mode;

	switch (ev.GetSelection()) {
	case 0:
		mode = wxAdvTable::HighlightNone;
		break;
	case 1:
		mode = wxAdvTable::HighlightRows;
		break;
	case 2:
		mode = wxAdvTable::HighlightCols;
		break;
	case 3:
		mode = wxAdvTable::HighlightBoth;
		break;
	default:
		return ;
	}

	m_advTable->SetHighlightMode((wxAdvTable::HighlightMode) mode);
	wxLogMessage(wxT("Highlight mode %s"), ev.GetString().c_str());
}

void ControlPanel::OnCheckShowRows(wxCommandEvent &ev)
{
	m_advTable->SetShowRows(ev.IsChecked());
}

void ControlPanel::OnCheckShowCols(wxCommandEvent &ev)
{
	m_advTable->SetShowCols(ev.IsChecked());
}

void ControlPanel::OnCheckShowCorner(wxCommandEvent &ev)
{
	m_advTable->SetShowCorner(ev.IsChecked());
}

//
// MainFrame
//

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)

END_EVENT_TABLE()

MainFrame::MainFrame()
: wxFrame(NULL, wxID_ANY, wxT("wxAdvTable demo"), wxDefaultPosition, wxSize(900, 600))
{
	wxAuiManager *auiManager = new wxAuiManager(this);

	m_advTable = new wxAdvTable(this, wxID_ANY);
	auiManager->AddPane(m_advTable, wxAuiPaneInfo().Center().Caption(wxT("wxAdvTable")).CloseButton(false));

	m_controlPanel = new ControlPanel(this, m_advTable);
	auiManager->AddPane(m_controlPanel, wxAuiPaneInfo().Left().Caption(wxT("Control")).CloseButton(false));

	wxTextCtrl *logCtrl = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize,
			wxTE_MULTILINE | wxTE_READONLY);
	auiManager->AddPane(logCtrl, wxAuiPaneInfo().Bottom().Caption(wxT("Messages")).CloseButton(false).MinSize(500, 200));
	wxLog::SetActiveTarget(new wxLogTextCtrl(logCtrl));

	auiManager->Update();

	Centre();

	// Create main menu
	wxMenuBar *menuBar = new wxMenuBar();

	wxMenu *menuFile = new wxMenu();
	menuFile->Append(wxID_EXIT, wxT("E&xit"));

	menuBar->Append(menuFile, wxT("&File"));

	SetMenuBar(menuBar);

	CreateTableStructure();
}

MainFrame::~MainFrame()
{
}

void MainFrame::OnExit(wxCommandEvent &ev)
{
	Close();
}

void MainFrame::CreateTableStructure()
{
	wxAdvHdrCell rows[] = {
		wxAdvHdrCell(wxT("Row I")),
		wxAdvHdrCell(wxT("Row II")).Sub(wxAdvHdrCell(wxT("Row II-I")).Sub(wxT("Row II-I-I")).Sub(wxT("Row II-I-II"))).Sub(wxT("Row II-II")).Sub(wxT("Row II-III")),
	};

	wxAdvHdrCell cols[] = {
		wxAdvHdrCell(wxT("Col I")).Size(100),
		wxAdvHdrCell(wxT("Col II")),
		wxAdvHdrCell(wxT("Col III")),
		wxAdvHdrCell(wxT("Col IV")).Sub(wxT("Col IV-I")).Sub(wxT("Col IV-II")).Sub(wxT("Col\nIV-III")),
		wxAdvHdrCell(wxT("Col V")),
	};

	wxString cornerLabel = wxT("Table corner");

	wxAdvStringTableDataModel *model = new wxAdvStringTableDataModel(
			wxAdvHdrCell::GetRealCellCount(rows, N(rows)),
			wxAdvHdrCell::GetRealCellCount(cols, N(cols)),
			true);

	m_advTable->Create(rows, N(rows), cols, N(cols), cornerLabel, model);

	m_advTable->SetSelectMode(wxAdvTable::SelectCell);
	m_advTable->SetHighlightMode(wxAdvTable::HighlightNone);
}
