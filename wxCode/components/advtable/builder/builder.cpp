/////////////////////////////////////////////////////////////////////////////
// Name:	builder.cpp
// Purpose: wxAdvTable builder application implementation
// Author:	Moskvichev Andrey V.
// Created:	20.09.2009
// Copyright:	(c) 2009-2010 Moskvichev Andrey V.
// Licence:	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "builder.h"
#include <wx/aui/aui.h>
#include <wx/aboutdlg.h>

#include <wx/tokenzr.h>

#if wxUSE_STD_IOSTREAM
#error "wxUSE_STD_IOSTREAM defined"
#endif

const wxString appName = wxT("wxAdvTable Builder 1.0");

static MainFrame *g_mainFrame;

/**
 * Builder application.
 */
class BuilderApp : public wxApp
{
public:
	bool OnInit()
	{
		m_docManager = new wxDocManager();

		m_docManager->SetMaxDocsOpen(1);

		new wxDocTemplate(m_docManager, wxT("wxAdvTable builder document"),
		wxT("*.txt"), wxEmptyString, wxT("txt"),
		wxT("wxAdvTable builder document"), wxT("wxAdvTable builder document view"),
		CLASSINFO(BuilderDocument), CLASSINFO(BuilderView));

		MainFrame *mainFrame = new MainFrame(m_docManager);

		mainFrame->Show(true);
		SetTopWindow(mainFrame);
		return true;
	}

	int OnExit()
	{
		wxDELETE(m_docManager);
		return 0;
	}

private:
	wxDocManager *m_docManager;
};

IMPLEMENT_APP(BuilderApp);


class EmptyTableDataModel : public wxAdvTableDataModel
{
public:
	EmptyTableDataModel()
	{
	}

	virtual ~EmptyTableDataModel()
	{
	}

	virtual wxString GetCellValue(size_t row, size_t col)
	{
		return wxEmptyString;
	}

	virtual bool SetCellValue(size_t row, size_t col, wxString value)
	{
		return true;
	}

	virtual int GetCellFormat(size_t row, size_t col)
	{
		return wxStringFormat;
	}

	virtual bool IsCellEditable(size_t row, size_t col)
	{
		return false;
	}

	virtual wxAdvCellAttribute *GetCellAttribute(size_t row, size_t col)
	{
		return NULL;
	}
};

//
// wxAdvTableSerializer
//

wxString wxAdvTableSerializer::SaveHdrCell(wxAdvHdrCell *hdrCell)
{
	wxString out;

	// save label
	out << wxT("wxAdvHdrCell(wxT(\"") << SaveText(hdrCell->Label()) << wxT("\"))");

	// save properties
	if (hdrCell->AlignVertical() != wxAdvHdrCell::defaultAlignVertical) {
		out << wxT(".AlignVertical(") << AlignVertical(hdrCell) << wxT(")");
	}
	if (hdrCell->AlignHorizontal() != wxAdvHdrCell::defaultAlignHorizontal) {
		out << wxT(".AlignHorizontal(") << AlignHorizontal(hdrCell) << wxT(")");
	}
	if (hdrCell->Size() != 0) {
		out << wxT(".Size(") << hdrCell->Size() << wxT(")");
	}
	if (hdrCell->Spacing() != wxAdvHdrCell::defaultSpacing) {
		out << wxT(".Spacing(") << hdrCell->Spacing() << wxT(")");
	}
	if (hdrCell->MinSize() != 0) {
		out << wxT(".MinSize(") << hdrCell->MinSize() << wxT(")");
	}
	if (hdrCell->MaxSize() != (wxCoord) UNDEF_SIZE) {
		out << wxT(".MaxSize(") << hdrCell->MaxSize() << wxT(")");
	}
	if (hdrCell->IsVerticalText()) {
		out << wxT(".VerticalText()");
	}
	if (hdrCell->IsSortable()) {
		out << wxT(".Sortable()");
	}

	// save subcells
	if (hdrCell->IsComposite()) {
		for (size_t n = 0; n < hdrCell->GetSubCellCount(); n++) {
			out << wxT(".Sub(") << SaveHdrCell(hdrCell->GetSubCell(n)) << wxT(")");
		}
	}

	return out;
}

wxString wxAdvTableSerializer::AlignVertical(wxAdvHdrCell *hdrCell)
{
	switch (hdrCell->AlignVertical()) {
	case wxALIGN_TOP:
		return wxT("wxALIGN_TOP");
	case wxALIGN_CENTER_VERTICAL:
		return wxT("wxALIGN_CENTER_VERTICAL");
	case wxALIGN_BOTTOM:
		return wxT("wxALIGN_BOTTOM");
	default:
		return wxT("wxALIGN_CENTER_VERTICAL"); // XXX fallback to center!
	}
}

wxString wxAdvTableSerializer::AlignHorizontal(wxAdvHdrCell *hdrCell)
{
	switch (hdrCell->AlignHorizontal()) {
	case wxALIGN_LEFT:
		return wxT("wxALIGN_LEFT");
	case wxALIGN_CENTER_HORIZONTAL:
		return wxT("wxALIGN_CENTER_HORIZONTAL");
	case wxALIGN_RIGHT:
		return wxT("wxALIGN_RIGHT");
	default:
		return wxT("wxALIGN_CENTER_HORIZONTAL"); // XXX fallback to center!
	}
}

wxString wxAdvTableSerializer::SaveText(const wxString &text)
{
	wxString out = text;
	out.Replace(wxT("\\"), wxT("\\\\"));
	out.Replace(wxT("\n"), wxT("\\n"));
	out.Replace(wxT("\t"), wxT("\\t"));
	out.Replace(wxT("\r"), wxT("\\r"));

	return out;
}

void wxAdvTableSerializer::LoadHdrCells(const wxString &text, wxAdvHdrCellArray &out)
{
	wxStringTokenizer tok(text, wxT(","));

	while (tok.HasMoreTokens()) {
		wxString token = tok.GetNextToken();

		wxAdvHdrCell cell;
		if (LoadHdrCell(cell, token)) {
			out.Add(cell);
		}
	}
}

bool wxAdvTableSerializer::LoadHdrCell(wxAdvHdrCell &hdrCell, const wxString &text)
{
	wxStringTokenizer tok(text, wxT("."));

	int nToken = 0;
	while (tok.HasMoreTokens()) {
		wxString token = tok.GetNextToken();

		if ((token.StartsWith(wxT("wxAdvHdrCell")) && nToken == 0) || token.StartsWith(wxT("Label"))) {
			wxString label = GetArg(token);

			label = LoadText(label);
			hdrCell.Label(label);
		}
		else if (token.StartsWith(wxT("Sub"))) {
			wxString strSubCell = GetArg(token);

			wxAdvHdrCell subCell;
			if (LoadHdrCell(subCell, strSubCell)) {
				hdrCell.Sub(subCell);
			}
		}
		else if (token.StartsWith(wxT("AlignVertical"))) {
			wxString strAlign = GetArg(token);
			int align;

			if (strAlign.Cmp(wxT("wxALIGN_TOP")) == 0) {
				align = wxALIGN_TOP;
			}
			else if (strAlign.Cmp(wxT("wxALIGN_CENTER_VERTICAL")) == 0) {
				align = wxALIGN_CENTER_VERTICAL;
			}
			else if (strAlign.Cmp(wxT("wxALIGN_BOTTOM")) == 0) {
				align = wxALIGN_BOTTOM;
			}
			else {
				wxLogError(wxT("Invalid vertical align value %s"), strAlign.c_str());
				continue;
			}

			hdrCell.AlignVertical(align);
		}
		else if (token.StartsWith(wxT("AlignHorizontal"))) {
			wxString strAlign = GetArg(token);
			int align;

			if (strAlign.Cmp(wxT("wxALIGN_LEFT")) == 0) {
				align = wxALIGN_LEFT;
			}
			else if (strAlign.Cmp(wxT("wxALIGN_CENTER_HORIZONTAL")) == 0) {
				align = wxALIGN_CENTER_HORIZONTAL;
			}
			else if (strAlign.Cmp(wxT("wxALIGN_RIGHT")) == 0) {
				align = wxALIGN_RIGHT;
			}
			else {
				wxLogError(wxT("Invalid horizontal align value %s"), strAlign.c_str());
				continue;
			}

			hdrCell.AlignHorizontal(align);
		}
		else if (token.StartsWith(wxT("Size"))) {
			wxString strSize = GetArg(token);

			long size;
			if (strSize.ToLong(&size)) {
				hdrCell.Size((wxCoord) size);
			}
			else {
				wxLogError(wxT("Invalid size %s"), strSize.c_str());
			}
		}
		else if (token.StartsWith(wxT("Spacing"))) {
			wxString strSpacing = GetArg(token);

			long spacing;
			if (strSpacing.ToLong(&spacing)) {
				hdrCell.Spacing((wxCoord) spacing);
			}
			else {
				wxLogError(wxT("Invalid spacing %s"), strSpacing.c_str());
			}
		}
		else if (token.StartsWith(wxT("MinSize"))) {
			wxString strMinSize = GetArg(token);

			long minSize;
			if (strMinSize.ToLong(&minSize)) {
				hdrCell.MinSize((wxCoord) minSize);
			}
			else {
				wxLogError(wxT("Invalid size %s"), strMinSize.c_str());
			}
		}
		else if (token.StartsWith(wxT("MaxSize"))) {
			wxString strMaxSize = GetArg(token);

			long maxSize;
			if (strMaxSize.ToLong(&maxSize)) {
				hdrCell.MaxSize((wxCoord) maxSize);
			}
			else {
				wxLogError(wxT("Invalid size %s"), strMaxSize.c_str());
			}
		}
		else if (token.StartsWith(wxT("VerticalText"))) {
			hdrCell.VerticalText();
		}
		else if (token.StartsWith(wxT("Sortable"))) {
			hdrCell.Sortable();
		}

		nToken++;
	}

	return true;
}

wxString wxAdvTableSerializer::GetArg(const wxString &text)
{
	int openCount = 0;
	int argPos;

	for (size_t n = 0; n < text.Length(); n++) {
		wxChar ch = text[n];

		if (ch == wxT('(')) {
			openCount++;
			if (openCount == 1) {
				argPos = n + 1;
			}
		}
		else if (ch == wxT(')')) {
			openCount--;
			if (openCount == 0) {
				return text.SubString(argPos, n - 1);
			}
		}
	}
	return wxEmptyString;
}

wxString wxAdvTableSerializer::LoadText(const wxString &text)
{
	int quoteCount = 0;
	int textPos;

	wxChar prevCh = wxT('\0');

	for (size_t n = 0; n < text.Length(); n++) {
		wxChar ch = text[n];

		if (ch == wxT('"') && prevCh != wxT('\\')) {
			quoteCount++;
			if (quoteCount == 1) {
				textPos = n + 1;
			}
			else if (quoteCount == 2) {
				wxString out = text.SubString(textPos, n - 1);

				out.Replace(wxT("\\n"), wxT("\n"));
				out.Replace(wxT("\\t"), wxT("\t"));
				out.Replace(wxT("\\r"), wxT("\r"));
				out.Replace(wxT("\\\\"), wxT("\\"));
				return out;
			}
		}

		prevCh = ch;
	}
	return wxEmptyString;
}

//
// BuilderDocument
//
IMPLEMENT_DYNAMIC_CLASS(BuilderDocument, wxDocument);

const wxString g_docHeader = wxT("/* created by wxAdvTable Builder */");

BuilderDocument::BuilderDocument()
{
	m_table = NULL;
}

BuilderDocument::~BuilderDocument()
{
}

void BuilderDocument::SetTable(wxAdvTable *table)
{
	m_table = table;
}

void BuilderDocument::Commit(wxAdvTable *table, wxAdvHdrCellArray &rows, wxAdvHdrCellArray &cols)
{
	table->Create(rows, cols, wxT("Corner"), new EmptyTableDataModel());
}

wxOutputStream& BuilderDocument::SaveObject(wxOutputStream& stream)
{
	wxCHECK(m_table != NULL, stream);

	wxTextOutputStream out(stream);

	out << g_docHeader << endl;

	// save rows
	out << wxT("wxAdvHdrCell rows[] = {") << endl;
	for (size_t n = 0; n < m_table->GetRowCount(); n++) {
		wxAdvHdrCell *hdrCell = m_table->GetRow(n);

		out << wxT("\t");
		out << wxAdvTableSerializer::SaveHdrCell(hdrCell).c_str();
		out << wxT(",\n");
	}
	out << wxT("};") << endl;

	// save columns
	out << wxT("wxAdvHdrCell cols[] = {") << endl;
	for (size_t n = 0; n < m_table->GetColCount(); n++) {
		wxAdvHdrCell *hdrCell = m_table->GetCol(n);

		out << wxT("\t");
		out << wxAdvTableSerializer::SaveHdrCell(hdrCell).c_str();
		out << wxT(",\n");
	}
	out << wxT("};") << endl;

    return stream;
}

wxInputStream& BuilderDocument::LoadObject(wxInputStream& stream)
{
	wxTextInputStream in(stream);

	wxString sign = in.ReadLine();
	if (sign != g_docHeader) {
		wxLogError(wxT("File is not a Builder document."));
		return stream;
	}

	// document parsing
	bool inRows = false;
	bool inCols = false;

	wxString rowsText;
	wxString colsText;

	while (!stream.Eof()) {
		wxString line = in.ReadLine();
		if (!stream.IsOk()) {
			break;
		}

		line.Trim();
		line.Trim(false);

		if (inRows) {
			if (line == wxT("};")) {
				inRows = false;
				continue;
			}
			rowsText += line;
		}
		else if (inCols) {
			if (line == wxT("};")) {
				inCols = false;
				continue;
			}
			colsText += line;
		}
		else {
			if (line == wxT("wxAdvHdrCell rows[] = {")) {
				inRows = true;
			}
			else if (line == wxT("wxAdvHdrCell cols[] = {")) {
				inCols = true;
			}
		}
	}

	wxAdvHdrCellArray rows;
	wxAdvHdrCellArray cols;

	wxAdvTableSerializer::LoadHdrCells(rowsText, rows);
	wxAdvTableSerializer::LoadHdrCells(colsText, cols);

	Commit(m_table, rows, cols);
	return stream;
}

void BuilderDocument::BeginUpdate()
{
	m_updateCount++;
}

void BuilderDocument::EndUpdate()
{
	if (--m_updateCount <= 0) {
		m_updateCount = 0;

		Modify(true);
		UpdateAllViews();
	}
}

//
// wxAdvHdrCell properties
//
enum {
	HDRCELL_LABEL = 1,
	HDRCELL_VERTICAL_ALIGN,
	HDRCELL_HORIZONTAL_ALIGN,
	HDRCELL_SIZE,
	HDRCELL_SPACING,
	HDRCELL_VERTICAL_TEXT,
	HDRCELL_SORTABLE,
//	HDRCELL_,
};

static const wxChar *g_vertAlignKeys[] = {
		wxT("Top"),
		wxT("Center"),
		wxT("Bottom"),
};

static int g_vertAlignValues[] = {
		wxALIGN_TOP,
		wxALIGN_CENTER_VERTICAL,
		wxALIGN_BOTTOM
};

static const wxChar *g_horizAlignKeys[] = {
		wxT("Left"),
		wxT("Center"),
		wxT("Right"),
};

static int g_horizAlignValues[] = {
		wxALIGN_LEFT,
		wxALIGN_CENTER_HORIZONTAL,
		wxALIGN_RIGHT
};

static Property *g_hdrCellProps[] = {
		new PropertyString(wxT("Label"), HDRCELL_LABEL),
		new PropertySelect(wxT("Vertical align"), HDRCELL_VERTICAL_ALIGN,
				g_vertAlignKeys, g_vertAlignValues, WXSIZEOF(g_vertAlignKeys)),
		new PropertySelect(wxT("Horizontal align"), HDRCELL_HORIZONTAL_ALIGN,
				g_horizAlignKeys, g_horizAlignValues, WXSIZEOF(g_horizAlignKeys)),
		new PropertyInt(wxT("Size"), HDRCELL_SIZE),
		new PropertyInt(wxT("Spacing"), HDRCELL_SPACING),
		new PropertyBool(wxT("Vertical text"), HDRCELL_VERTICAL_TEXT),
		new PropertyBool(wxT("Sortable"), HDRCELL_SORTABLE),
//		new Property
};

class HdrCellPropertyObject : public PropertyObject
{
public:
	HdrCellPropertyObject(wxAdvTable *table, wxAdvHdrCell *hdrCell);
	virtual ~HdrCellPropertyObject();

	virtual wxString GetProperty(Property *property);

	virtual PropertyCollection *GetProperties();

protected:
	virtual void DoSetProperty(Property *property, const wxString &value);

private:
	PropertyCollection *m_propColl;

	wxAdvTable *m_table;
	wxAdvHdrCell *m_hdrCell;

	wxCoord m_size;
};

HdrCellPropertyObject::HdrCellPropertyObject(wxAdvTable *table, wxAdvHdrCell *hdrCell)
{
	m_propColl = new PropertyCollection(g_hdrCellProps, WXSIZEOF(g_hdrCellProps));
	m_table = table;
	m_hdrCell = hdrCell;
}

HdrCellPropertyObject::~HdrCellPropertyObject()
{
	wxDELETE(m_propColl);
}

wxString HdrCellPropertyObject::GetProperty(Property *property)
{
	switch (property->GetUID()) {
	case HDRCELL_LABEL:
		return m_hdrCell->Label();
	case HDRCELL_VERTICAL_ALIGN:
		return property->Format((const void *) m_hdrCell->AlignVertical());
	case HDRCELL_HORIZONTAL_ALIGN:
		return property->Format((const void *) m_hdrCell->AlignHorizontal());
	case HDRCELL_SIZE:
		return property->Format((const void *) m_hdrCell->Size());
	case HDRCELL_SPACING:
		return property->Format((const void *) m_hdrCell->Spacing());
	case HDRCELL_VERTICAL_TEXT:
		return property->Format((const void *) m_hdrCell->IsVerticalText());
	case HDRCELL_SORTABLE:
		return property->Format((const void *) m_hdrCell->IsSortable());
	default:
		return wxEmptyString;
	}
}

PropertyCollection *HdrCellPropertyObject::GetProperties()
{
	return m_propColl;
}

void HdrCellPropertyObject::DoSetProperty(Property *property, const wxString &value)
{
	switch (property->GetUID()) {
	case HDRCELL_LABEL:
		m_hdrCell->Label(value);
		break;
	case HDRCELL_VERTICAL_ALIGN:
		m_hdrCell->AlignVertical(((PropertySelect *) property)->GetValue(value));
		break;
	case HDRCELL_HORIZONTAL_ALIGN:
		m_hdrCell->AlignHorizontal(((PropertySelect *) property)->GetValue(value));
		break;
	case HDRCELL_SIZE: {
		wxCoord newSize = ((PropertyInt *) property)->Decode(value);
		m_hdrCell->Size(newSize);
		break;
	}
	case HDRCELL_SPACING:
		m_hdrCell->Spacing(((PropertyInt *) property)->Decode(value));
		break;
	case HDRCELL_VERTICAL_TEXT:
		m_hdrCell->SetVerticalText(((PropertyBool *) property)->Decode(value));
		break;
	case HDRCELL_SORTABLE:
		m_hdrCell->SetSortable(((PropertyBool *) property)->Decode(value));
		break;
	default:
		return ;
	}

	m_table->UpdateGeometry();
}

//
// TablePanel
//

enum {
	HDRCELL_ADDSUB = 101,
	HDRCELL_DELETE,
};

BEGIN_EVENT_TABLE(TablePanel, wxPanel)
	EVT_ADVTABLE_HDRCELL_RIGHT_CLICK(TablePanel::OnAdvTableHdrCellRightClick)
	EVT_ADVTABLE_HDRCELL_LEFT_CLICK(TablePanel::OnAdvTableHdrCellLeftClick)

	EVT_MENU(HDRCELL_ADDSUB, TablePanel::OnHdrCellAddSub)
	EVT_MENU(HDRCELL_DELETE, TablePanel::OnHdrCellDelete)

END_EVENT_TABLE()

TablePanel::TablePanel(MainFrame *parent)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(500, 400))
{
	wxAuiManager *auiManager = new wxAuiManager(this);

	m_table = new wxAdvTable(this, wxID_ANY, wxDefaultPosition, wxSize(500, 400));
	auiManager->AddPane(m_table, wxCENTER, wxT("Table"));

	EmptyTable();
	m_table->SetResizeAllRowsAllowed(true);
	m_table->SetResizeAllColsAllowed(true);

	CreatePopupMenus();

	auiManager->Update();
}

TablePanel::~TablePanel()
{
}

wxAdvTable *TablePanel::GetTable()
{
	return m_table;
}

void TablePanel::EmptyTable()
{
	m_table->Create(0, 0, wxT("Corner"), new EmptyTableDataModel());
}

void TablePanel::OnAdvTableHdrCellRightClick(wxAdvHdrCellEvent &ev)
{
	wxAdvHdrCell *hdrCell = ev.GetHdrCell();

	m_hdrCell = hdrCell;
	PopupMenu(m_popupMenuHdrCell, ev.GetPosition());
}

void TablePanel::OnAdvTableHdrCellLeftClick(wxAdvHdrCellEvent &ev)
{
	wxAdvHdrCell *hdrCell = ev.GetHdrCell();

	HdrCellPropertyObject *propObj = new HdrCellPropertyObject(m_table, hdrCell);
	MainFrame::Get()->GetPropertyPage()->SetObject(propObj, true);
}

void TablePanel::OnHdrCellAddSub(wxCommandEvent &ev)
{
	wxCHECK_RET(m_hdrCell != NULL, wxT("TablePanel::OnHdrCellAddSub: m_hdrCell == NULL"));

	m_hdrCell->Sub(wxT("Sub"));
	m_table->UpdateGeometry();
}

void TablePanel::OnHdrCellDelete(wxCommandEvent &ev)
{
	wxCHECK_RET(m_hdrCell != NULL, wxT("TablePanel::OnHdrCellDelete: m_hdrCell == NULL"));

	m_table->RemoveHdrCell(m_hdrCell);
}

void TablePanel::CreatePopupMenus()
{
	m_popupMenuHdrCell = new wxMenu(wxT("Header cell"));
	m_popupMenuHdrCell->Append(HDRCELL_ADDSUB, wxT("Add sub row/column"));
	m_popupMenuHdrCell->Append(HDRCELL_DELETE, wxT("Delete"));
}

void TablePanel::InsertRows()
{
	wxAdvHdrCell rows[] = {
		wxAdvHdrCell(wxT("Row")),
	};

	m_table->AddRows(rows, WXSIZEOF(rows));
}

void TablePanel::InsertCols()
{
	wxAdvHdrCell cols[] = {
		wxAdvHdrCell(wxT("Col")),
	};

	m_table->AddCols(cols, WXSIZEOF(cols));
}

//
// MainFrame
//
enum {
	MENU_INSERT_ROWS = 101,
	MENU_INSERT_COLS,
};

IMPLEMENT_CLASS(MainFrame, wxDocParentFrame);

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_MENU(MENU_INSERT_ROWS, MainFrame::OnMenuInsertRows)
	EVT_MENU(MENU_INSERT_COLS, MainFrame::OnMenuInsertCols)

	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
END_EVENT_TABLE()

MainFrame::MainFrame(wxDocManager *docManager)
: wxDocParentFrame(docManager, NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(900, 600))
{
	g_mainFrame = this;

	wxAuiManager *auiManager = new wxAuiManager(this);

	m_propPage = new PropertyPage(this);
	m_tablePanel = new TablePanel(this);

	auiManager->AddPane(m_propPage, wxAuiPaneInfo().Caption(wxT("Properties")).Left().MinSize(200, 600));
	auiManager->AddPane(m_tablePanel, wxCENTER, wxT("wxAdvTable"));

	CreateMenuBar();

	auiManager->Update();

	docManager->CreateDocument(wxEmptyString, wxDOC_NEW);

	Center();
}

MainFrame::~MainFrame()
{
	g_mainFrame = NULL;
}

MainFrame *MainFrame::Get()
{
	return g_mainFrame;
}

void MainFrame::CreateMenuBar()
{
	wxMenuBar *menuBar = new wxMenuBar();

	wxMenu *menuFile = new wxMenu();
	menuFile->Append(wxID_NEW, wxT("&New"));
	menuFile->Append(wxID_OPEN, wxT("&Open"));
	menuFile->Append(wxID_SAVE, wxT("&Save"));
	menuFile->Append(wxID_SAVEAS, wxT("S&ave as"));
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT, wxT("&Exit"));

	wxMenu *menuHelp = new wxMenu();
	menuHelp->Append(wxID_ABOUT, wxT("&About"));

	wxMenu *menuInsert = new wxMenu();
	menuInsert->Append(MENU_INSERT_ROWS, wxT("Insert rows"));
	menuInsert->Append(MENU_INSERT_COLS, wxT("Insert columns"));

	menuBar->Append(menuFile, wxT("&File"));
	menuBar->Append(menuInsert, wxT("&Insert"));
	menuBar->Append(menuHelp, wxT("&Help"));

	SetMenuBar(menuBar);
}

void MainFrame::OnExit(wxCommandEvent &ev)
{
	Close();
}

void MainFrame::OnAbout(wxCommandEvent &ev)
{
	wxAboutDialogInfo about;
	about.SetName(appName);
	about.SetVersion(wxT("1.0"));
	about.SetDescription(wxT("Application for visual wxAdvTable construction"));
	about.SetCopyright(wxT("Copyright (C) 2009 Moskvichev Andrey V."));

	wxAboutBox(about);
}

void MainFrame::OnMenuInsertRows(wxCommandEvent &ev)
{
	m_tablePanel->InsertRows();
}

void MainFrame::OnMenuInsertCols(wxCommandEvent &ev)
{
	m_tablePanel->InsertCols();
}

PropertyPage *MainFrame::GetPropertyPage()
{
	return m_propPage;
}

void MainFrame::UpdateGUI()
{
	// TODO not implemented!
}

void MainFrame::OnCloseDocument()
{
	m_tablePanel->EmptyTable();
	m_propPage->SetObject(NULL);
}

void MainFrame::SetDocument(BuilderDocument *doc)
{
	m_tablePanel->EmptyTable();
	m_propPage->SetObject(NULL);

	doc->SetTable(m_tablePanel->GetTable());
}

//
// BuilderView
//

IMPLEMENT_DYNAMIC_CLASS(BuilderView, wxView);

BuilderView::BuilderView()
{
}

BuilderView::~BuilderView()
{
}

bool BuilderView::OnCreate(wxDocument *doc, long flags)
{
	MainFrame *frame = MainFrame::Get();

	frame->SetDocument((BuilderDocument *) doc);
	return true;
}

void BuilderView::OnDraw(wxDC *dc)
{
}

void BuilderView::OnUpdate(wxView *sender, wxObject *hint)
{
	MainFrame *frame = MainFrame::Get();
	frame->UpdateGUI();
}

bool BuilderView::OnClose(bool deleteWindow)
{
	MainFrame *frame = MainFrame::Get();
	if (frame != NULL) {
		frame->OnCloseDocument();
	}
	return true;
}
