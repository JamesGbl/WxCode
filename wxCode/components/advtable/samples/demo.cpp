/////////////////////////////////////////////////////////////////////////////
// Name:        demo.cpp
// Purpose:     demo application implementation
// Author:      Moskvichev Andrey V.
// Created:     2008/10/09
// RCS-ID:      $Id: wxAdvTable.h,v 1.3 2008/10/09 16:42:58 frm Exp $
// Copyright:   (c) 2008-2009 Moskvichev A.V.
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "demo.h"
#include <wx/aui/aui.h>
#include <wx/aboutdlg.h>

const wxString appVersion = wxT("1.1");
const wxString appName = wxT("wxAdvTable demo 1.1");

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
// GUI to set various properties to wxAdvTable
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
	CHOICE_SORT_MODE,
	CHECK_SHOW_ROWS,
	CHECK_SHOW_COLS,
	CHECK_SHOW_CORNER,
	CHECK_SORT_BY_ANY_ROW,
	CHECK_SORT_BY_ANY_COL,
	SPINCTRL_GRID_WIDTH,
	SPINCTRL_FOCUSED_WIDTH,
};

BEGIN_EVENT_TABLE(ControlPanel, wxPanel)
	EVT_CHOICE(CHOICE_SELECT_MODE, ControlPanel::OnChoiceSelectMode)
	EVT_CHOICE(CHOICE_HIGHLIGHT_MODE, ControlPanel::OnChoiceHighlightMode)
	EVT_CHOICE(CHOICE_SORT_MODE, ControlPanel::OnChoiceSortMode)
	EVT_CHECKBOX(CHECK_SHOW_ROWS, ControlPanel::OnCheckShowRows)
	EVT_CHECKBOX(CHECK_SHOW_COLS, ControlPanel::OnCheckShowCols)
	EVT_CHECKBOX(CHECK_SHOW_CORNER, ControlPanel::OnCheckShowCorner)
	EVT_CHECKBOX(CHECK_SORT_BY_ANY_ROW, ControlPanel::OnCheckSortByAnyRow)
	EVT_CHECKBOX(CHECK_SORT_BY_ANY_COL, ControlPanel::OnCheckSortByAnyCol)
	EVT_SPINCTRL(SPINCTRL_GRID_WIDTH, ControlPanel::OnSpinCtrlGridWidth)
	EVT_SPINCTRL(SPINCTRL_FOCUSED_WIDTH, ControlPanel::OnSpinCtrlFocusedWidth)
END_EVENT_TABLE()

ControlPanel::ControlPanel(wxWindow *parent, wxAdvTable *advTable)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(100, 200))
{
	m_advTable = advTable;

	wxSizer *sizer = new wxFlexGridSizer(0, 1, 5, 5);
	wxSizer *choiceSizer = new wxFlexGridSizer(0, 2, 5, 5);

	// select mode choices
	wxArrayString choicesSelectMode;
	choicesSelectMode.Add(wxT("SelectCell"));
	choicesSelectMode.Add(wxT("SelectRows"));
	choicesSelectMode.Add(wxT("SelectCols"));
	choicesSelectMode.Add(wxT("SelectBlock"));

	wxChoice *choiceSelectMode = new wxChoice(this, CHOICE_SELECT_MODE, wxDefaultPosition, wxDefaultSize,
			choicesSelectMode);
	ADD_FIELD(this, choiceSizer, wxT("Select mode"), choiceSelectMode);

	advTable->SetSelectMode(wxAdvTable::SelectCell);
	choiceSelectMode->SetSelection(0);

	// highlight mode choices
	wxArrayString choicesHighlightMode;
	choicesHighlightMode.Add(wxT("HighlightNone"));
	choicesHighlightMode.Add(wxT("HighlightRows"));
	choicesHighlightMode.Add(wxT("HighlightCols"));
	choicesHighlightMode.Add(wxT("HighlightBoth"));

	wxChoice *choiceHighlightMode = new wxChoice(this, CHOICE_HIGHLIGHT_MODE, wxDefaultPosition, wxDefaultSize,
			choicesHighlightMode);
	ADD_FIELD(this, choiceSizer, wxT("Highlight mode"), choiceHighlightMode);

	advTable->SetHighlightMode(wxAdvTable::HighlightNone);
	choiceHighlightMode->SetSelection(0);

	// sort mode choices
	wxArrayString choicesSortMode;
	choicesSortMode.Add(wxT("SortRows"));
	choicesSortMode.Add(wxT("SortCols"));
	choicesSortMode.Add(wxT("SortDisabled"));

	wxChoice *choiceSortMode = new wxChoice(this, CHOICE_SORT_MODE, wxDefaultPosition, wxDefaultSize,
			choicesSortMode);
	ADD_FIELD(this, choiceSizer, wxT("Sort mode"), choiceSortMode);

	switch (advTable->GetSortMode()) {
	case wxAdvTable::SortRows:
		choiceSortMode->SetSelection(0);
		break;
	case wxAdvTable::SortCols:
		choiceSortMode->SetSelection(1);
		break;
	case wxAdvTable::SortDisabled:
		choiceSortMode->SetSelection(2);
		break;
	}

	sizer->Add(choiceSizer,
			0, wxEXPAND);

	// create "show rows" checkbox
	wxCheckBox *checkBox;
	checkBox = new wxCheckBox(this, CHECK_SHOW_ROWS, wxT("Show rows"), wxDefaultPosition, wxDefaultSize);
	checkBox->SetValue(m_advTable->GetShowRows());
	sizer->Add(checkBox,
			0, wxEXPAND);

	// create "show columns" checkbox
	checkBox = new wxCheckBox(this, CHECK_SHOW_COLS, wxT("Show columns"), wxDefaultPosition, wxDefaultSize);
	checkBox->SetValue(m_advTable->GetShowCols());
	sizer->Add(checkBox,
			0, wxEXPAND);

	// create "show corner" checkbox
	checkBox = new wxCheckBox(this, CHECK_SHOW_CORNER, wxT("Show corner"), wxDefaultPosition, wxDefaultSize);
	checkBox->SetValue(m_advTable->GetShowCorner());
	sizer->Add(checkBox,
			0, wxEXPAND);

	// create "sort by any row" checkbox
	checkBox = new wxCheckBox(this, CHECK_SORT_BY_ANY_ROW, wxT("Allow sort by any row"), wxDefaultPosition, wxDefaultSize);
	checkBox->SetValue(m_advTable->IsAllowSortByAnyRow());
	sizer->Add(checkBox,
			0, wxEXPAND);

	// create "sort by any column" checkbox
	checkBox = new wxCheckBox(this, CHECK_SORT_BY_ANY_COL, wxT("Allow sort by any column"), wxDefaultPosition, wxDefaultSize);
	checkBox->SetValue(m_advTable->IsAllowSortByAnyCol());
	sizer->Add(checkBox,
			0, wxEXPAND);

	// create graphics object, grid pen, focused pen, etc.
	wxSizer *graphicsSizer = new wxFlexGridSizer(0, 2, 5, 5);

	ADD_FIELD(this, graphicsSizer, wxT("Grid lines width"),
			new wxSpinCtrl(this, SPINCTRL_GRID_WIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize,
					wxSP_ARROW_KEYS, 1, 100, 1));

	ADD_FIELD(this, graphicsSizer, wxT("Focused lines width"),
			new wxSpinCtrl(this, SPINCTRL_FOCUSED_WIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize,
					wxSP_ARROW_KEYS, 1, 100, 1));

	sizer->Add(graphicsSizer,
			0, wxEXPAND);

	sizer->SetSizeHints(this);
	SetSizer(sizer);
}

ControlPanel::~ControlPanel()
{
}

//
// ControlPanel event handlers.
//
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

void ControlPanel::OnChoiceSortMode(wxCommandEvent &ev)
{
	int mode;

	switch (ev.GetSelection()) {
	case 0:
		mode = wxAdvTable::SortRows;
		break;
	case 1:
		mode = wxAdvTable::SortCols;
		break;
	case 2:
		mode = wxAdvTable::SortDisabled;
		break;
	default:
		return ;
	}

	m_advTable->SetSortMode((wxAdvTable::SortMode) mode);
	wxLogMessage(wxT("Sort mode %s"), ev.GetString().c_str());
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

void ControlPanel::OnCheckSortByAnyRow(wxCommandEvent &ev)
{
	m_advTable->SetAllowSortByAnyRow(ev.IsChecked());
}

void ControlPanel::OnCheckSortByAnyCol(wxCommandEvent &ev)
{
	m_advTable->SetAllowSortByAnyCol(ev.IsChecked());
}

void ControlPanel::OnSpinCtrlGridWidth(wxSpinEvent &ev)
{
	wxPen pen = m_advTable->GetGridPen();
	pen.SetWidth(ev.GetPosition());

	m_advTable->SetGridPen(pen);
}

void ControlPanel::OnSpinCtrlFocusedWidth(wxSpinEvent &ev)
{
	wxPen pen = m_advTable->GetFocusedPen();
	pen.SetWidth(ev.GetPosition());

	m_advTable->SetFocusedPen(pen);
}


//
// MainFrame
//

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
	EVT_GRID_CELL_LEFT_CLICK(MainFrame::OnGridLeftClick)
	EVT_GRID_CELL_RIGHT_CLICK(MainFrame::OnGridRightClick)
	EVT_GRID_CELL_LEFT_DCLICK(MainFrame::OnGridLeftDClick)
	EVT_GRID_CELL_RIGHT_DCLICK(MainFrame::OnGridRightDClick)
	EVT_GRID_RANGE_SELECT(MainFrame::OnGridRangeSelect)
	EVT_GRID_CELL_CHANGE(MainFrame::OnGridCellChange)
	EVT_GRID_SELECT_CELL(MainFrame::OnGridSelectCell)
	EVT_GRID_EDITOR_SHOWN(MainFrame::OnGridEditorShown)
	EVT_GRID_EDITOR_HIDDEN(MainFrame::OnGridEditorHidden)

	EVT_ADVTABLE_HDRCELL_LEFT_CLICK(MainFrame::OnAdvTableHdrCellLeftClick)
	EVT_ADVTABLE_HDRCELL_RIGHT_CLICK(MainFrame::OnAdvTableHdrCellRightClick)
	EVT_ADVTABLE_HDRCELL_LEFT_DCLICK(MainFrame::OnAdvTableHdrCellLeftDClick)
	EVT_ADVTABLE_HDRCELL_RIGHT_DCLICK(MainFrame::OnAdvTableHdrCellRightDClick)
END_EVENT_TABLE()

MainFrame::MainFrame()
: wxFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(900, 600))
{
	wxAuiManager *auiManager = new wxAuiManager(this);

	// create wxAdvTable
	m_advTable = new wxAdvTable(this, wxID_ANY);
	auiManager->AddPane(m_advTable, wxAuiPaneInfo().Center().Caption(wxT("wxAdvTable")).CloseButton(false));

	// create control panel, used to set table properties
	m_controlPanel = new ControlPanel(this, m_advTable);
	auiManager->AddPane(m_controlPanel, wxAuiPaneInfo().Left().Caption(wxT("Control")).CloseButton(false));

	// create log window
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

	wxMenu *menuHelp = new wxMenu();
	menuHelp->Append(wxID_ABOUT, wxT("&About"));

	menuBar->Append(menuFile, wxT("&File"));
	menuBar->Append(menuHelp, wxT("&Help"));

	SetMenuBar(menuBar);

	// create wxAdvTable structure
	CreateTableStructure();
}

MainFrame::~MainFrame()
{
}

void MainFrame::OnAbout(wxCommandEvent &ev)
{
	wxAboutDialogInfo about;
	about.SetName(wxT("wxAdvTable demo"));
	about.SetVersion(appVersion);
	about.SetDescription(wxT("This demo shows wxAdvTable features"));
	about.SetCopyright(wxT("Copyright (C) 2008-2009 Moskvichev Andrey V."));

	wxAboutBox(about);
}

void MainFrame::OnExit(wxCommandEvent &ev)
{
	Close();
}

//
// wxAdvTable event handlers
//
void MainFrame::OnGridLeftClick(wxGridEvent &ev)
{
	wxLogMessage(wxT("wxEVT_GRID_LEFT_CLICK row=%i col=%i"), ev.GetRow(), ev.GetCol());
}

void MainFrame::OnGridRightClick(wxGridEvent &ev)
{
	wxLogMessage(wxT("wxEVT_GRID_RIGHT_CLICK row=%i col=%i"), ev.GetRow(), ev.GetCol());
}

void MainFrame::OnGridLeftDClick(wxGridEvent &ev)
{
	wxLogMessage(wxT("wxEVT_GRID_LEFT_DCLICK row=%i col=%i"), ev.GetRow(), ev.GetCol());
}

void MainFrame::OnGridRightDClick(wxGridEvent &ev)
{
	wxLogMessage(wxT("wxEVT_GRID_RIGHT_DCLICK row=%i col=%i"), ev.GetRow(), ev.GetCol());
}

void MainFrame::OnGridRangeSelect(wxGridRangeSelectEvent &ev)
{
	wxLogMessage(wxT("wxEVT_GRID_RANGE_SELECT row1=%i col1=%i, row2=%i col2=%i"),
			ev.GetTopRow(), ev.GetLeftCol(),
			ev.GetBottomRow(), ev.GetRightCol());
}

void MainFrame::OnGridCellChange(wxGridEvent &ev)
{
	wxLogMessage(wxT("wxEVT_GRID_CELL_CHANGE row=%i col=%i"),
			ev.GetRow(), ev.GetCol());

	if (ev.GetRow() == 3 && ev.GetCol() == 0) {
		wxMessageDialog dlg(this, wxT("Allow cell change?"), wxT("Question"), wxYES_NO);
		if (dlg.ShowModal() == wxID_NO) {
			ev.Veto();
		}
	}
}

void MainFrame::OnGridSelectCell(wxGridEvent &ev)
{
	wxLogMessage(wxT("wxEVT_GRID_SELECT_CELL row=%i col=%i"),
			ev.GetRow(), ev.GetCol());
}

void MainFrame::OnGridEditorShown(wxGridEvent &ev)
{
	wxLogMessage(wxT("wxEVT_GRID_EDITOR_SHOWN"));
}

void MainFrame::OnGridEditorHidden(wxGridEvent &ev)
{
	wxLogMessage(wxT("wxEVT_GRID_EDITOR_HIDDEN"));
}

void MainFrame::OnAdvTableHdrCellLeftClick(wxAdvHdrCellEvent &ev)
{
	wxAdvHdrCell *hdrCell = ev.GetHdrCell();

	wxLogMessage(wxT("wxEVT_ADVTABLE_HDRCELL_LEFT_CLICK: %s index=%i"),
			(hdrCell->IsRow() ? wxT("row") : wxT("column")), hdrCell->Index());
}

void MainFrame::OnAdvTableHdrCellRightClick(wxAdvHdrCellEvent &ev)
{
	wxAdvHdrCell *hdrCell = ev.GetHdrCell();

	wxLogMessage(wxT("wxEVT_ADVTABLE_HDRCELL_RIGHT_CLICK: %s index=%i"),
			(hdrCell->IsRow() ? wxT("row") : wxT("column")), hdrCell->Index());
}

void MainFrame::OnAdvTableHdrCellLeftDClick(wxAdvHdrCellEvent &ev)
{
	wxAdvHdrCell *hdrCell = ev.GetHdrCell();

	wxLogMessage(wxT("wxEVT_ADVTABLE_HDRCELL_LEFT_DCLICK: %s index=%i"),
			(hdrCell->IsRow() ? wxT("row") : wxT("column")), hdrCell->Index());
}

void MainFrame::OnAdvTableHdrCellRightDClick(wxAdvHdrCellEvent &ev)
{
	wxAdvHdrCell *hdrCell = ev.GetHdrCell();

	wxLogMessage(wxT("wxEVT_ADVTABLE_HDRCELL_RIGHT_DCLICK: %s index=%i"),
			(hdrCell->IsRow() ? wxT("row") : wxT("column")), hdrCell->Index());
}

//
// Most interesting function - this creates table structure.
//
void MainFrame::CreateTableStructure()
{
	// row definitions
	wxAdvHdrCell rows[] = {
		wxAdvHdrCell(wxT("Row\nI")).Sortable(), // make first row sortable
		wxAdvHdrCell(wxT("Row\nII")).VerticalText() // we need to draw row name vertical
			.Sub(wxAdvHdrCell(wxT("Row II-I")) // first subrow, it also contains two subrows
					.Sub(wxT("Row II-I-I"))
					.Sub(wxT("Row II-I-II")))
			.Sub(wxT("Row II-II")) // second subrow
			.Sub(wxT("Row II-III")), // third subrow
	};

	// column definitions
	wxAdvHdrCell cols[] = {
		wxAdvHdrCell(wxT("Col I")).Size(150), // we can explicitly define size of column
		wxAdvHdrCell(wxT("Col II")) // second column containing three subcolumns
			.Sub(wxT("Col II-I"))
			.Sub(wxT("Col II-II"))
			.Sub(wxT("Col\nII-III")),
		wxAdvHdrCell(wxT("Col III")),
		wxAdvHdrCell(wxT("Col IV")).Sortable().Size(100), // make last column sortable
	};

	// we also need table corner
	wxString cornerLabel = wxT("Table corner");

	// create table model
	// take attention on wxAdvHdrCell::GetDecompCellCount call
	// table row count can be NOT equal to row count in definitions array.
	wxAdvDefaultTableDataModel *model = new wxAdvDefaultTableDataModel(
			wxAdvHdrCell::GetDecompCellCount(rows, N(rows)),
			wxAdvHdrCell::GetDecompCellCount(cols, N(cols)),
			false);

	// and set some values to it
	model->SetCellValue(0, 0, wxT("value"));
	model->SetCellValue(2, 0, wxT("another value"));
	model->SetCellValue(3, 0, wxT("vetoable edit value"));

	// set formats for entire columns
	model->SetColFormat(3, wxColourFormat);
	model->SetColFormat(4, wxBoolFormat);

	// set some colour values
	model->SetCellValue(0, 3, wxGREEN->GetAsString());
	model->SetCellValue(1, 3, wxRED->GetAsString());
	model->SetCellValue(2, 3, wxBLUE->GetAsString());
	model->SetCellValue(3, 3, wxCYAN->GetAsString());
	model->SetCellValue(4, 3, wxLIGHT_GREY->GetAsString());

	// set some boolean values
	model->SetCellValue(0, 4, boolFalse);
	model->SetCellValue(1, 4, boolTrue);
	model->SetCellValue(2, 4, boolTrue);

	// set values to last column, to sort table by them
	model->SetCellValue(0, 5, wxT("d"));
	model->SetCellValue(1, 5, wxT("b"));
	model->SetCellValue(2, 5, wxT("e"));
	model->SetCellValue(3, 5, wxT("c"));
	model->SetCellValue(4, 5, wxT("a"));

	// create table
	// we pass our row and column definitions and table data model to it
	m_advTable->Create(rows, N(rows), cols, N(cols), cornerLabel, model);

	m_advTable->SetSorter(new wxAdvTableStringSorter());
	m_advTable->SetSortingIndex(5); // sort table by 5-th column
	m_advTable->SetSortDirection(wxAdvTable::SortDirAscending);
	m_advTable->SetSortMode(wxAdvTable::SortRows); // we will sort rows
}
