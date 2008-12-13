/////////////////////////////////////////////////////////////////////////////
// Name:        wxAdvTable.cpp
// Purpose:     wxAdvTable classes implementation
// Author:      Moskvichev Andrey V.
// Created:     29/10/2008
// RCS-ID:      $Id: wxAdvTable.cpp,v 1.4 2008/10/29 16:42:58 frm Exp $
// Copyright:   (c) 2008 Moskvichev A.V.
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxadvtable.h"

#include <wx/renderer.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <wx/colordlg.h>

#include <wx/arrimpl.cpp>
WX_DEFINE_EXPORTED_OBJARRAY(wxAdvHdrCellArray);
WX_DEFINE_EXPORTED_OBJARRAY(wxAdvHdrCellPtrArray);
WX_DEFINE_EXPORTED_OBJARRAY(wxAdvRangeArray);
WX_DEFINE_EXPORTED_OBJARRAY(wxCoordArray);
WX_DEFINE_EXPORTED_OBJARRAY(wxIndexArray);
WX_DEFINE_EXPORTED_OBJARRAY(wxArrayArrayString);

size_t wxAdvHdrCell::GetDecompCellCount(wxAdvHdrCell *cells, size_t numCells)
{
	size_t count = 0;

	for (size_t n = 0; n < numCells; n++) {
		count += cells[n].GetDecompCellCount();
	}
	return count;
}

//
// wxAdvTableDataModelObserver
//
wxAdvTableDataModelObserver::wxAdvTableDataModelObserver()
{
}

wxAdvTableDataModelObserver::~wxAdvTableDataModelObserver()
{
}

//
// wxAdvTableDataModel
//
wxAdvTableDataModel::wxAdvTableDataModel()
{
}

wxAdvTableDataModel::~wxAdvTableDataModel()
{
}

double wxAdvTableDataModel::GetCellValueDouble(size_t WXUNUSED(row), size_t WXUNUSED(col))
{
	return 0;
}

//
// wxAdvStringTableDataModel
//
wxAdvStringTableDataModel::wxAdvStringTableDataModel(size_t numRows, size_t numCols, bool readOnly)
{
	m_readOnly = readOnly;

	wxArrayString row;
	row.Add(wxEmptyString, numCols);
	m_data.Add(row, numRows);
}

wxAdvStringTableDataModel::~wxAdvStringTableDataModel()
{
}

wxString wxAdvStringTableDataModel::GetCellValue(size_t row, size_t col)
{
	wxCHECK(row < m_data.Count(), wxEmptyString);
	wxCHECK(col < m_data[row].Count(), wxEmptyString);
	return m_data[row][col];
}

bool wxAdvStringTableDataModel::IsCellEditable(size_t WXUNUSED(row), size_t WXUNUSED(col))
{
	return !m_readOnly;
}

int wxAdvStringTableDataModel::GetCellFormat(size_t WXUNUSED(row), size_t WXUNUSED(col))
{
	return wxStringFormat;
}

bool wxAdvStringTableDataModel::SetCellValue(size_t row, size_t col, wxString value)
{
	if (m_readOnly) {
		return false;
	}

	wxCHECK(row < m_data.Count(), false);
	wxCHECK(col < m_data[row].Count(), false);

	m_data[row][col] = value;
	FireCellChanged(row, col);
	return true;
}

wxAdvCellAttribute *wxAdvStringTableDataModel::GetCellAttribute(size_t WXUNUSED(row), size_t WXUNUSED(col))
{
	return &m_defaultCellAttribute;
}

double wxAdvStringTableDataModel::GetCellValueDouble(size_t WXUNUSED(row), size_t WXUNUSED(col))
{
	return 0;
}

//
// wxAdvFilterTableDataModel
//
wxAdvFilterTableDataModel::wxAdvFilterTableDataModel(wxAdvTableDataModel *model, size_t numRows, size_t numCols)
{
	m_undelayingModel = model;
	m_undelayingModel->AddObserver(this);

	wxArrayString row;
	row.Add(wxEmptyString, numCols);
	m_data.Add(row, numRows);

	m_needUpdate = true;
	//UpdateValues();
}

wxAdvFilterTableDataModel::~wxAdvFilterTableDataModel()
{
	delete m_undelayingModel;
}

wxString wxAdvFilterTableDataModel::GetCellValue(size_t row, size_t col)
{
	if (m_needUpdate) {
		UpdateValues();
	}

	wxCHECK(row < m_data.Count(), wxEmptyString);
	wxCHECK(col < m_data[row].Count(), wxEmptyString);
	return m_data[row][col];
}

int wxAdvFilterTableDataModel::GetCellFormat(size_t row, size_t col)
{
	return m_undelayingModel->GetCellFormat(row, col);
}

bool wxAdvFilterTableDataModel::IsCellEditable(size_t row, size_t col)
{
	return m_undelayingModel->IsCellEditable(row, col);
}

void wxAdvFilterTableDataModel::CellChanged(size_t row, size_t col)
{
	m_data[row][col] = m_undelayingModel->GetCellValue(row, col);
	FireCellChanged(row, col);
}

bool wxAdvFilterTableDataModel::SetCellValue(size_t row, size_t col, wxString value)
{
	return m_undelayingModel->SetCellValue(row, col, value);
}

wxAdvCellAttribute *wxAdvFilterTableDataModel::GetCellAttribute(size_t row, size_t col)
{
	return m_undelayingModel->GetCellAttribute(row, col);
}

double wxAdvFilterTableDataModel::GetCellValueDouble(size_t row, size_t col)
{
	return m_undelayingModel->GetCellValueDouble(row, col);
}

void wxAdvFilterTableDataModel::TableChanged()
{
	UpdateValues();
	FireTableChanged();
}

void wxAdvFilterTableDataModel::UpdateValues()
{
	for (size_t row = 0; row < m_data.Count(); row++) {
		for (size_t col = 0; col < m_data[row].Count(); col++) {
			m_data[row][col] = m_undelayingModel->GetCellValue(row, col);
		}
	}
	m_needUpdate = false;
}

//
// wxAdvTableCellRenderer
//
wxAdvTableCellRenderer::wxAdvTableCellRenderer()
{
}

wxAdvTableCellRenderer::~wxAdvTableCellRenderer()
{
}

//
// wxAdvBoolTableCellRenderer
//
wxAdvBoolTableCellRenderer::wxAdvBoolTableCellRenderer()
{
}

wxAdvBoolTableCellRenderer::~wxAdvBoolTableCellRenderer()
{
}

void wxAdvBoolTableCellRenderer::Draw(wxAdvTable *table, wxDC &dc, wxRect rc, wxString value, bool WXUNUSED(selected), bool WXUNUSED(focused), wxAdvCellAttribute *WXUNUSED(attr))
{
	int flags = 0;
	if (value.Cmp(boolTrue) == 0) {
		flags |= wxCONTROL_CHECKED;
	}

	wxRendererNative::Get().DrawCheckBox(table, dc, rc, flags);
}

wxAdvStringCellRenderer::wxAdvStringCellRenderer()
{
}

wxAdvStringCellRenderer::~wxAdvStringCellRenderer()
{
}

void wxAdvStringCellRenderer::Draw(wxAdvTable *WXUNUSED(table), wxDC &dc, wxRect rc, wxString value, bool WXUNUSED(selected), bool WXUNUSED(focused), wxAdvCellAttribute *attr)
{
	wxDCClipper clip(dc, rc);

	if (attr->Brush().GetStyle() !=	wxTRANSPARENT) {
		// blend brush colors.
		wxColour clr1 = dc.GetBrush().GetColour();
		wxColour clr2 = attr->Brush().GetColour();

		wxColour blendedClr((clr1.Red() + clr2.Red()) / 2,
			(clr1.Green() + clr2.Green()) / 2,
			(clr1.Blue() + clr2.Blue()) / 2);

		wxBrush brush = *wxTheBrushList->FindOrCreateBrush(blendedClr, attr->Brush().GetStyle());
		dc.SetBrush(brush);
	}

	dc.DrawRectangle(rc);
	dc.SetFont(attr->Font());
	dc.SetTextForeground(attr->TextColour());
	dc.DrawLabel(value, rc, attr->Alignment());
}

wxAdvColourTableCellRenderer::wxAdvColourTableCellRenderer()
{
}

wxAdvColourTableCellRenderer::~wxAdvColourTableCellRenderer()
{
}

void wxAdvColourTableCellRenderer::Draw(wxAdvTable *table, wxDC &dc, wxRect rc, wxString value, bool WXUNUSED(selected), bool WXUNUSED(focused), wxAdvCellAttribute *WXUNUSED(attr))
{
	wxColour colour(value);

	dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(colour));
	dc.DrawRectangle(rc);
}


//
// editors
//
wxAdvTableCellEditor::wxAdvTableCellEditor()
{
}

wxAdvTableCellEditor::~wxAdvTableCellEditor()
{
}

bool wxAdvTableCellEditor::OneClickActivate()
{
	return false;
}

void wxAdvTableCellEditor::Activate(wxAdvTable *table, size_t row, size_t col)
{
	m_cell.Set(row, col);
	m_table = table;

	DoActivate(table, row, col);
}

void wxAdvTableCellEditor::SetNewValue(wxString newValue)
{
	wxCHECK_RET(m_cell.IsSet(), wxT("wxAdvTableCellEditor::SetNewValue: m_cell.IsSet()"));
	wxCHECK_RET(m_table != NULL, wxT("wxAdvTableCellEditor::SetNewValue: m_table != NULL"));

	m_table->GetModel()->SetCellValue(m_cell.Row(), m_cell.Col(), newValue);
}

void wxAdvTableCellEditor::EndEditing()
{
	wxCHECK_RET(m_table != NULL, wxT("wxAdvTableCellEditor::EndEditing"));
	m_table->StopEditing();
}

//
// wxAdvStringTableCellEditor
//
BEGIN_EVENT_TABLE(wxAdvStringTableCellEditor, wxEvtHandler)
	EVT_TEXT_ENTER(wxID_ANY, wxAdvStringTableCellEditor::OnTextEnter)
	EVT_KILL_FOCUS(wxAdvStringTableCellEditor::OnKillFocus)
	EVT_KEY_DOWN(wxAdvStringTableCellEditor::OnTextKeydown)
END_EVENT_TABLE()

wxAdvStringTableCellEditor::wxAdvStringTableCellEditor(wxAdvTable *table)
{
	m_textCtrl = new wxTextCtrl(table, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
			wxTE_PROCESS_ENTER | wxBORDER_NONE);
	m_textCtrl->Show(false);
	m_textCtrl->SetNextHandler(this);
}

wxAdvStringTableCellEditor::~wxAdvStringTableCellEditor()
{
	m_textCtrl->Close();
}

bool wxAdvStringTableCellEditor::OneClickActivate()
{
	return false;
}

void wxAdvStringTableCellEditor::DoActivate(wxAdvTable *table, size_t row, size_t col)
{
	wxRect rc = table->GetCellRect(row, col);
	rc.Deflate(1, 1);
	table->CalcScrolledPosition(rc.x, rc.y, &rc.x, &rc.y);

	m_textCtrl->ChangeValue(table->GetCellValue(row, col));

	m_textCtrl->SetSize(rc.x, rc.y, rc.width, rc.height);
	m_textCtrl->Show();

	m_textCtrl->SetFocus();
}

void wxAdvStringTableCellEditor::Deactivate(wxAdvTable *table)
{
	m_textCtrl->Show(false);

	SetNewValue(m_textCtrl->GetValue());
}

void wxAdvStringTableCellEditor::OnTextEnter(wxCommandEvent &ev)
{
	EndEditing();
}

void wxAdvStringTableCellEditor::OnKillFocus(wxFocusEvent &WXUNUSED(ev))
{
	EndEditing();
}

void wxAdvStringTableCellEditor::OnTextKeydown(wxKeyEvent &ev)
{
	if (ev.GetKeyCode() == WXK_ESCAPE) {
		EndEditing();
		return ;
	}
	ev.Skip();
}


//
// wxAdvBoolTableCellEditor
//
BEGIN_EVENT_TABLE(wxAdvBoolTableCellEditor, wxEvtHandler)
	EVT_CHECKBOX(wxID_ANY, wxAdvBoolTableCellEditor::OnCheckbox)
	//	EVT_KILL_FOCUS(wxAdvBoolTableCellEditor::OnKillFocus)
END_EVENT_TABLE()

wxAdvBoolTableCellEditor::wxAdvBoolTableCellEditor(wxAdvTable *table)
{
	m_checkBox = new wxCheckBox(table, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	m_checkBox->Show(false);
	m_checkBox->SetNextHandler(this);
}

wxAdvBoolTableCellEditor::~wxAdvBoolTableCellEditor()
{
	m_checkBox->Close();
}

bool wxAdvBoolTableCellEditor::OneClickActivate()
{
	return true;
}

void wxAdvBoolTableCellEditor::DoActivate(wxAdvTable *table, size_t row, size_t col)
{
	wxRect rc = table->GetCellRect(row, col);
	//rc.Deflate(1, 1);
	table->CalcScrolledPosition(rc.x, rc.y, &rc.x, &rc.y);

	bool checked = ! (table->GetCellValue(row, col).Cmp(boolTrue) == 0);
	m_checkBox->SetValue(checked);
	SetNewValue((checked) ? boolTrue : boolFalse);

	m_checkBox->SetSize(rc.x, rc.y, rc.width, rc.height);
	m_checkBox->Show();

	m_checkBox->SetFocus();
}

void wxAdvBoolTableCellEditor::Deactivate(wxAdvTable *table)
{
	m_checkBox->Show(false);

	bool checked = m_checkBox->IsChecked();
	SetNewValue((checked) ? boolTrue : boolFalse);
}

void wxAdvBoolTableCellEditor::OnKillFocus(wxFocusEvent &WXUNUSED(ev))
{
	EndEditing();
}

void wxAdvBoolTableCellEditor::OnCheckbox(wxCommandEvent &WXUNUSED(ev))
{
	bool checked = m_checkBox->IsChecked();
	SetNewValue((checked) ? boolTrue : boolFalse);
}

wxAdvColourTableCellEditor::wxAdvColourTableCellEditor()
{
}

wxAdvColourTableCellEditor::~wxAdvColourTableCellEditor()
{
}

bool wxAdvColourTableCellEditor::OneClickActivate()
{
	return true;
}

void wxAdvColourTableCellEditor::DoActivate(wxAdvTable *table, size_t row, size_t col)
{
	wxColourData clrData;
	clrData.SetColour(wxColour(table->GetCellValue(row, col)));

	wxColourDialog dlg(table, &clrData);
	if (dlg.ShowModal() == wxID_OK) {
		wxString newColourValue = dlg.GetColourData().GetColour().GetAsString();
		SetNewValue(newColourValue);
	}
}

void wxAdvColourTableCellEditor::Deactivate(wxAdvTable *table)
{
}

//
// wxAdvIntervalTableCellEditor
//
wxAdvIntervalTableCellEditor::wxAdvIntervalTableCellEditor(wxAdvTable *table)
{
	m_spinCtrl = new wxSpinCtrl(table, wxID_ANY);
	m_spinCtrl->Show(false);
}

wxAdvIntervalTableCellEditor::~wxAdvIntervalTableCellEditor()
{
	m_spinCtrl->Close();
}

bool wxAdvIntervalTableCellEditor::OneClickActivate()
{
	return false;
}

void wxAdvIntervalTableCellEditor::SetRange(int minValue, int maxValue)
{
	m_spinCtrl->SetRange(minValue, maxValue);
}

void wxAdvIntervalTableCellEditor::Deactivate(wxAdvTable *table)
{
	m_spinCtrl->Show(false);

	SetNewValue(wxString::Format(wxT("%i"), m_spinCtrl->GetValue()));
}

void wxAdvIntervalTableCellEditor::DoActivate(wxAdvTable *table, size_t row, size_t col)
{
	wxRect rc = table->GetCellRect(row, col);
	table->CalcScrolledPosition(rc.x, rc.y, &rc.x, &rc.y);

	wxString value =  table->GetCellValue(row, col);

	m_spinCtrl->SetValue(value);

	m_spinCtrl->SetSize(rc.x, rc.y, rc.width, rc.height);
	m_spinCtrl->Show();
}

//
// wxAdvCellAttribute
//
wxAdvCellAttribute::wxAdvCellAttribute()
{
	m_font = *wxNORMAL_FONT;
	m_alignment = wxALIGN_CENTER | wxALIGN_RIGHT;
	m_brush = *wxTheBrushList->FindOrCreateBrush(*wxBLACK, wxTRANSPARENT);
}

wxAdvCellAttribute::~wxAdvCellAttribute()
{
}


//
// Sorters
//
class SortHelper // XXX dirty hack! used to implement sorters
{
public:
	SortHelper(const SortHelper &o)
	{
		m_sorter = o.m_sorter;
		m_table = o.m_table;
		m_row = o.m_row;
		m_col = o.m_col;
		m_sortRows = o.m_sortRows;
		m_sortDirection = o.m_sortDirection;
	}

	SortHelper(wxAdvTable *table, wxAdvTableSorter *sorter, size_t row, size_t col, bool sortRows, int sortDirection)
	{
		m_table = table;
		m_sorter = sorter;
		m_row = row;
		m_col = col;
		m_sortRows = sortRows;
		m_sortDirection = sortDirection;
	}

	virtual ~SortHelper()
	{
		int i = 0;
		i++;
	}

	size_t Index()
	{
		return (m_sortRows) ? m_row : m_col;
	}

	wxAdvTableSorter *m_sorter;
	wxAdvTable *m_table;
	size_t m_row;
	size_t m_col;
	int m_sortDirection;

	bool m_sortRows;
};

WX_DEFINE_SORTED_ARRAY(SortHelper *, wxSortHelperArray);

int SorterFunc(SortHelper *first, SortHelper *second)
{
	wxAdvTableSorter *sorter = first->m_sorter;
	int res = sorter->Compare(first->m_table, first->m_row, first->m_col, second->m_row, second->m_col);
	if (first->m_sortDirection == wxAdvTable::Descending) {
		return -1 * res;
	}
	return res;
}

wxAdvTableSorter::wxAdvTableSorter()
{
}

wxAdvTableSorter::~wxAdvTableSorter()
{
}

//
// wxAdvTableStringSorter
//
wxAdvTableStringSorter::wxAdvTableStringSorter()
{
}

wxAdvTableStringSorter::~wxAdvTableStringSorter()
{
}

int wxAdvTableStringSorter::Compare(wxAdvTable *table, size_t row1, size_t col1, size_t row2, size_t col2)
{
	wxString value1 = table->GetModel()->GetCellValue(row1, col1);
	wxString value2 = table->GetModel()->GetCellValue(row2, col2);

	return Compare(value1, value2);
}

int wxAdvTableStringSorter::Compare(wxString value1, wxString value2)
{
	return value1.Cmp(value2);
}

//
// wxAdvTableDoubleSorter
//
wxAdvTableDoubleSorter::wxAdvTableDoubleSorter()
{
}

wxAdvTableDoubleSorter::~wxAdvTableDoubleSorter()
{
}

int wxAdvTableDoubleSorter::Compare(wxAdvTable *table, size_t row1, size_t col1, size_t row2, size_t col2)
{
	//double value1 = table->GetCellValueDouble(row1, col1);
	//double value2 = table->GetCellValueDouble(row2, col2);

	//return Compare(value1, value2);
	return 0;
}

int wxAdvTableDoubleSorter::Compare(double value1, double value2)
{
	if (value1 == value2) {
		return 0;
	}
	else if (value1 < value2) {
		return -1;
	}
	else {
		return 1;
	}
}

//
// wxAdvTable
//
BEGIN_EVENT_TABLE(wxAdvTable, wxScrolledWindow)
	EVT_PAINT(wxAdvTable::OnPaint)
	EVT_MOUSE_EVENTS(wxAdvTable::OnMouseEvents)
	EVT_KEY_DOWN(wxAdvTable::OnKeyDown)
	EVT_KILL_FOCUS(wxAdvTable::OnKillFocus)
	EVT_SIZE(wxAdvTable::OnSize)
END_EVENT_TABLE()

IMPLEMENT_CLASS(wxAdvTable, wxScrolledWindow)

wxAdvTable::wxAdvTable(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &m_size)
: wxScrolledWindow(parent, id, pos, m_size, wxHSCROLL | wxVSCROLL | wxFULL_REPAINT_ON_RESIZE)
{
	m_tableCreated = false;
	m_model = NULL;
	m_defaultRenderer = new wxAdvStringCellRenderer();
	m_sorter = NULL;
	m_sortMode = Rows;
	m_highlightMode = HighlightNone;
	m_pressedHdrCell = NULL;
	m_currentHdrCell = NULL;
	m_sortDirection = Ascending;

	m_currentEditor = NULL;

	m_selectMode = SelectBlock;

	m_showRows = true;
	m_showCols = true;
	m_showCorner = true;

	m_needRedraw = false;

	// Graphics initialization
	m_bgBrush = *wxWHITE_BRUSH;
	m_selectedBgBrush = *wxTheBrushList->FindOrCreateBrush(wxColour(200, 200, 250));
	m_gridPen = *wxThePenList->FindOrCreatePen(wxColour(200, 200, 200), 1, wxSOLID);
	m_focusedPen = *wxThePenList->FindOrCreatePen(wxColour(80, 80, 250), 1, wxSOLID);
	m_focusedBgBrush = *wxTheBrushList->FindOrCreateBrush(wxColour(180, 180, 250));
	m_highlightedBgBrush = *wxTheBrushList->FindOrCreateBrush(wxColour(225, 225, 250));
	ClearSelection();

	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	SetScrollRate(10, 10);
}

wxAdvTable::~wxAdvTable()
{
	wxAdvTableCellRendererMap::iterator it;

	for (it = m_renderers.begin(); it != m_renderers.end(); it++) {
		delete it->second;
	}

	SAFE_DELETE(m_model);
	SAFE_DELETE(m_defaultRenderer);
}

//
// Table creation functions.
//

void wxAdvTable::Create(wxAdvHdrCell *rows, size_t numRows, wxAdvHdrCell *cols, size_t numCols, const wxString &cornerLabel, wxAdvTableDataModel *_model)
{
	if (m_tableCreated) {
		DestroyTable();
	}

	m_cornerCell.Label(cornerLabel);

	AddHdrCells(m_rows, rows, numRows, true);
	AddHdrCells(m_cols, cols, numCols, false);

	// recalculate entire geometry
	CalcTableGeometry();

	// assign new model for table data
	SAFE_REPLACE(m_model, _model);
	m_model->AddObserver(this);

	// reset sorting and selection
	m_sortingIndex = (size_t) -1;
	ClearSelection();

	m_tableCreated = true;
	ResizeBackBitmaps();
}

void wxAdvTable::Create(size_t numRows, wxAdvHdrCell *cols, size_t numCols, const wxString &cornerLabel, wxAdvTableDataModel *model)
{
	Create(NULL, numRows, cols, numCols, cornerLabel, model);
}

void wxAdvTable::Create(size_t numRows, size_t numCols, const wxString &cornerLabel, wxAdvTableDataModel *model)
{
	Create(NULL, numRows, NULL, numCols, cornerLabel, model);
}

void wxAdvTable::Create(wxAdvHdrCell *rows, size_t numRows, size_t numCols, const wxString &cornerLabel, wxAdvTableDataModel *model)
{
	Create(rows, numRows, NULL, numCols, cornerLabel, model);
}

void wxAdvTable::DestroyTable()
{
	m_rows.Clear();
	m_cols.Clear();
	m_cornerCell.Label(wxEmptyString);
	m_cornerCell.m_rc = wxRect(0, 0, 0, 0);

	m_decompRows.Clear();
	m_decompCols.Clear();

	m_rowLayerWidths.Clear();
	m_colLayerHeights.Clear();

	SAFE_DELETE(m_model);

	ClearSelection();
}

void wxAdvTable::AddRows(wxAdvHdrCell *rows, size_t numRows)
{
	wxCHECK_RET(m_tableCreated, wxT("wxAdvTable::AddRows: Table is not created"));

	AddHdrCells(m_rows, rows, numRows, true);
	CalcTableGeometry();
}

void wxAdvTable::AddCols(wxAdvHdrCell *cols, size_t numCols)
{
	wxCHECK_RET(m_tableCreated, wxT("wxAdvTable::AddCols: Table is not created"));

	AddHdrCells(m_cols, cols, numCols, false);
	CalcTableGeometry();
}

//
// Table geometry functions.
//
void wxAdvTable::CalcTableGeometry()
{
	size_t numRowLayers = GetLayerCount(m_rows);
	size_t numColLayers = GetLayerCount(m_cols);

	// Layer sizes
	m_rowLayerWidths.Clear();
	m_rowLayerWidths.Alloc(numRowLayers);
	m_rowLayerWidths.Add(0, numRowLayers);

	m_colLayerHeights.Clear();
	m_colLayerHeights.Alloc(numColLayers);
	m_colLayerHeights.Add(0, numColLayers);

	// real rows/columns
	m_decompRows.Clear();
	m_decompCols.Clear();

	DecomposeHdrCells(m_rows, m_decompRows);
	DecomposeHdrCells(m_cols, m_decompCols);

	// update header geometry
	wxClientDC dc(this);
	CalcHeaderGeometry(dc);
}

void wxAdvTable::CalcHeaderGeometry(wxDC &dc)
{
	// setup sizes for all rows and columns
	FOREACH_HDRCELL(n, m_rows) {
		FitForSubCells(dc, m_rows[n], true);
	}
	FOREACH_HDRCELL(n, m_cols) {
		FitForSubCells(dc, m_cols[n], false);
	}

	// calculate layer sizes from row/column maximum heights/widths
	FOREACH_HDRCELL(n, m_rows) {
		CalcLayerSizes(m_rows[n], m_rowLayerWidths, true, 0);
	}
	FOREACH_HDRCELL(n, m_cols) {
		CalcLayerSizes(m_cols[n], m_colLayerHeights, false, 0);
	}

	// set row/columns heights/widths from layer sizes
	FOREACH_HDRCELL(n, m_rows) {
		UpdateHeaderSizes(m_rows[n], m_rowLayerWidths, true, 0);
	}
	FOREACH_HDRCELL(n, m_cols) {
		UpdateHeaderSizes(m_cols[n], m_colLayerHeights, false, 0);
	}

	// setup positions for all rows and columns
	wxCoord totalColHeight = SumLayerSizes(m_colLayerHeights);
	wxCoord totalRowWidth = SumLayerSizes(m_rowLayerWidths);

	wxCoord x0 = 0;
	wxCoord y0 = 0;

	x0 = 0;
	y0 = (m_showCols || m_showCorner) ? totalColHeight : 0;
	FOREACH_HDRCELL(n, m_rows) {
		UpdateHeaderPositions(m_rows[n], x0, y0, m_rowLayerWidths, 0, true);
		y0 += m_rows[n].m_rc.height;
	}

	x0 = (m_showRows || m_showCorner) ? totalRowWidth : 0;
	y0 = 0;
	FOREACH_HDRCELL(n, m_cols) {
		UpdateHeaderPositions(m_cols[n], x0, y0, m_colLayerHeights, 0, false);
		x0 += m_cols[n].m_rc.width;
	}

	m_cornerCell.m_rc = wxRect(0, 0, totalRowWidth, totalColHeight);

	wxCoord virtWidth = CalcTotalColsWidth();
	wxCoord virtHeight = CalcTotalRowsHeight();

	if (m_showRows || m_showCorner) {
		virtWidth += totalRowWidth;
	}
	if (m_showCols || m_showCorner) {
		virtHeight += totalColHeight;
	}

	SetScrollbars(1, 1, virtWidth, virtHeight);
	//SetVirtualSize(virtWidth, virtHeight);
}

wxSize wxAdvTable::FitForSubCells(wxDC &dc, wxAdvHdrCell &hdrCell, bool isRows)
{
	wxSize size;

	if (!hdrCell.IsComposite()) {
		size = hdrCell.CalcExtent(dc);
		hdrCell.m_rc.SetPosition(wxPoint(0, 0));
		hdrCell.m_rc.SetSize(size);
		return size;
	}

	FOREACH_HDRCELL(n, hdrCell.m_subCells) {
		wxAdvHdrCell &subHdrCell = hdrCell.m_subCells[n];

		wxSize subSize;
		if (subHdrCell.IsComposite()) {
			subSize = FitForSubCells(dc, subHdrCell, isRows);
		}
		else {
			subSize = subHdrCell.CalcExtent(dc);
		}

		if (isRows) {
			size.x = MAX(size.x, subSize.x);
			size.y += subSize.y;
		}
		else {
			size.x += subSize.x;
			size.y = MAX(size.y, subSize.y);
		}

		subHdrCell.m_rc.SetPosition(wxPoint(0, 0));
		subHdrCell.m_rc.SetSize(subSize);
	}

	hdrCell.m_rc.SetPosition(wxPoint(0, 0));
	hdrCell.m_rc.SetSize(size);
	return size;
}

void wxAdvTable::CalcLayerSizes(wxAdvHdrCell &hdrCell, wxCoordArray &layerSizes, bool isRows, size_t nLayer)
{
	FOREACH_HDRCELL(n, hdrCell.m_subCells) {
		wxAdvHdrCell &subHdrCell = hdrCell.m_subCells[n];
		CalcLayerSizes(subHdrCell, layerSizes, isRows, nLayer + 1);
	}

	wxCoord size;
	if (isRows) {
		size = hdrCell.m_rc.width;
	}
	else {
		size = hdrCell.m_rc.height;
	}

	if (nLayer == LastLayer(hdrCell, layerSizes)) {
		// update layer size if row/column occupies only one layer.
		layerSizes[nLayer] = MAX(layerSizes[nLayer], size);
	}
}

void wxAdvTable::UpdateHeaderPositions(wxAdvHdrCell &hdrCell, wxCoord x0, wxCoord y0, wxCoordArray &layerSizes, size_t nLayer, bool isRows)
{
	hdrCell.m_rc.x = x0;
	hdrCell.m_rc.y = y0;

	FOREACH_HDRCELL(n, hdrCell.m_subCells) {
		wxAdvHdrCell &subHdrCell = hdrCell.m_subCells[n];

		wxCoord x, y;
		if (isRows) {
			x = x0 + layerSizes[nLayer];
			y = y0;
		}
		else {
			x = x0;
			y = y0 + layerSizes[nLayer];
		}

		UpdateHeaderPositions(subHdrCell, x, y, layerSizes, nLayer + 1, isRows);

		if (isRows) {
			y0 += subHdrCell.m_rc.height;
		}
		else {
			x0 += subHdrCell.m_rc.width;
		}
	}
}

void wxAdvTable::UpdateHeaderSizes(wxAdvHdrCell &hdrCell, wxCoordArray &layerSizes, bool isRows, size_t nLayer)
{
	FOREACH_HDRCELL(n, hdrCell.m_subCells) {
		wxAdvHdrCell &subHdrCell = hdrCell.m_subCells[n];
		UpdateHeaderSizes(subHdrCell, layerSizes, isRows, nLayer + 1);
	}

	wxCoord size = SumLayerSizes(layerSizes, nLayer, LastLayer(hdrCell, layerSizes));
	if (isRows) {
		hdrCell.m_rc.width = size;
	}
	else {
		hdrCell.m_rc.height = size;
	}
}

void wxAdvTable::DecomposeHdrCells(wxAdvHdrCellArray &hdrCells, wxAdvHdrCellPtrArray &decompHdrCells)
{
	FOREACH_HDRCELL(n, hdrCells) {
		wxAdvHdrCell &hdrCell = hdrCells[n];
		DecomposeHdrCell(hdrCell, decompHdrCells);
	}
}

void wxAdvTable::DecomposeHdrCell(wxAdvHdrCell &hdrCell, wxAdvHdrCellPtrArray &decompHdrCells)
{
	if (!hdrCell.IsComposite()) {
		hdrCell.m_isDecomp = true;
		hdrCell.m_index = decompHdrCells.Count();
		decompHdrCells.Add(&hdrCell);
	}
	else {
		FOREACH_HDRCELL(n, hdrCell.m_subCells) {
			DecomposeHdrCell(hdrCell.m_subCells[n], decompHdrCells);
		}
	}
}

wxCoord wxAdvTable::SumDimensions(wxAdvHdrCellPtrArray &hdrCells, bool widths)
{
	wxCoord sum = 0;
	if (widths) {
		FOREACH_HDRCELL(n, hdrCells) {
			sum += hdrCells[n]->m_rc.width;
		}
	}
	else {
		FOREACH_HDRCELL(n, hdrCells) {
			sum += hdrCells[n]->m_rc.height;
		}
	}
	return sum;
}

wxCoord wxAdvTable::SumLayerSizes(wxCoordArray &layerSizes, size_t first, size_t last)
{
	wxCoord size = 0;
	for (size_t n = first; n <= last && n < layerSizes.Count(); n++) {
		size += layerSizes[n];
	}
	return size;
}

size_t wxAdvTable::GetLayerCount(wxAdvHdrCellArray &hdrCells)
{
	size_t numLayers = 0;
	FOREACH_HDRCELL(n, hdrCells) {
		if (hdrCells[n].Layers() > numLayers)
			numLayers = hdrCells[n].Layers();
	}
	return numLayers;
}

void wxAdvTable::AddHdrCells(wxAdvHdrCellArray &arr, wxAdvHdrCell *hdrCells, size_t numCells, bool isRows)
{
	arr.Alloc(arr.Count() + numCells);

	size_t number = arr.Count() + 1;
	for (size_t n = 0; n < numCells; n++) {
		wxAdvHdrCell *cell;

		if (hdrCells != NULL) {
			cell = &hdrCells[n];
		}
		else {
			cell = (isRows) ? &m_defaultRow : &m_defaultCol;
			cell->Label(wxString::Format(wxT("%i"), number++));
		}

		if (isRows) {
			cell->MarkAsRow();
		}
		else {
			cell->MarkAsCol();
		}

		arr.Add(*cell);
	}
}

//
// Sorting, selection, highlight, etc functions.
//
void wxAdvTable::SetSorter(wxAdvTableSorter *sorter)
{
	SAFE_REPLACE(m_sorter, sorter);
	UpdateSortOrder();
}

void wxAdvTable::SetSortMode(SortMode sortMode)
{
	if (m_sortMode != sortMode) {
		m_sortMode = sortMode;
		UpdateSortOrder();
	}
}

void wxAdvTable::UpdateSortOrder()
{
	if (m_sorter == NULL) {
		return ;
	}

	wxSortHelperArray helpers(SorterFunc);

	// XXX need normal sorting method, this code is very bad.
	if (m_sortMode == Rows) {
		if (m_sortingIndex >= m_decompCols.Count()) {
			return ;
		}

		size_t rowCount = m_decompRows.Count();
		helpers.Alloc(rowCount);

		for (size_t row = 0; row < rowCount; row++) {
			helpers.Add(new SortHelper(this, m_sorter, row, m_sortingIndex, true, m_sortDirection));
		}
	}
	else if (m_sortMode == Cols) {
		if (m_sortingIndex >= m_decompRows.Count()) {
			return ;
		}

		size_t colCount = m_decompCols.Count();
		helpers.Alloc(colCount);

		for (size_t col = 0; col < colCount; col++) {
			helpers.Add(new SortHelper(this, m_sorter, m_sortingIndex, col, false, m_sortDirection));
		}
	}
	else {
		return ;
	}

	m_sortOrder.Empty();
	m_sortOrder.Alloc(helpers.Count());

	for (size_t n = 0; n < helpers.Count(); n++) {
		m_sortOrder.Add(helpers[n]->Index());
	}

	// destroy helpers
	for (size_t n = 0; n < helpers.Count(); n++) {
		delete helpers[n];
	}

	RedrawAll();
}

void wxAdvTable::SetSortingIndex(size_t sortingIndex)
{
	if (m_sortingIndex == sortingIndex) {
		m_sortDirection = (m_sortDirection == Ascending) ? Descending : Ascending;
	}
	else {
		m_sortDirection = Ascending;
	}

	m_sortingIndex = sortingIndex;
	UpdateSortOrder();
}

void wxAdvTable::SetSelectMode(SelectMode selectMode)
{
	if (m_selectMode != selectMode) {
		m_selectMode = selectMode;
		ClearSelection();
	}
}

int wxAdvTable::GetSelectMode()
{
	return m_selectMode;
}

void wxAdvTable::SetHighlightMode(HighlightMode highlightMode)
{
	if (m_highlightMode != highlightMode) {
		m_highlightMode = highlightMode;

		RedrawAll();
	}
}

void wxAdvTable::ClearSelection()
{
	m_focused.Unset();
	m_pressedCell.Unset();
	m_selected.Unset();

	RedrawAll();
}

void wxAdvTable::SetShowHeaders(bool showRows, bool showCols, bool showCorner)
{
	m_showRows = showRows;
	m_showCols = showCols;
	m_showCorner = showCorner;

	wxClientDC dc(this);
	CalcHeaderGeometry(dc);

	RedrawAll();
}

//
// Drawing functions.
//
void wxAdvTable::DrawHdrCells(wxDC &dc, wxAdvHdrCellArray &hdrCells)
{
	FOREACH_HDRCELL(n, hdrCells) {
		DrawHdrCell(dc, &hdrCells[n]);
	}
}

void wxAdvTable::DrawHdrCell(wxDC &dc, wxAdvHdrCell *hdrCell)
{
	DrawHdrCellSelf(dc, hdrCell);

	FOREACH_HDRCELL(n, hdrCell->m_subCells) {
		DrawHdrCell(dc, &hdrCell->m_subCells[n]);
	}
}

void wxAdvTable::DrawHdrCellSelf(wxDC &dc, wxAdvHdrCell *hdrCell)
{
	wxDCClipper clip(dc, hdrCell->m_rc);
	wxRect rc = hdrCell->m_rc;

	wxHeaderSortIconType sortIcon = wxHDR_SORT_ICON_NONE;
	if (m_sortingIndex == hdrCell->m_index) {
		if ((hdrCell->m_isRow && m_selectMode == SelectCols)
			|| (!hdrCell->m_isRow && m_selectMode == SelectRows)) {
			sortIcon = (m_sortDirection == Ascending) ? wxHDR_SORT_ICON_DOWN : wxHDR_SORT_ICON_UP;
		}
	}

	int flags = 0;
	if (hdrCell == m_pressedHdrCell) {
		flags |= wxCONTROL_PRESSED;
	}
	if (hdrCell == m_currentHdrCell) {
		flags |= wxCONTROL_SELECTED;
	}

#ifdef wxGTK
	if (hdrCell->m_isRow && hdrCell != m_currentHdrCell) {
		// wxGtk rows drawing bugfix
		int x,y;
		dc.GetDeviceOrigin(&x, &y);

		rc.x += x;
		rc.y += y;
	}
#endif

	wxRendererNative::Get().DrawHeaderButton(this, dc, rc, flags, sortIcon);
	// TODO draw vertical text
	dc.SetFont(*wxNORMAL_FONT);
	dc.SetTextForeground(*wxBLACK);
	dc.DrawLabel(hdrCell->m_label, hdrCell->m_rc, hdrCell->m_alignHorizontal | hdrCell->m_alignVertical);
}

void wxAdvTable::DrawTable(wxDC &dc)
{
	FOREACH_HDRCELL(row, m_decompRows) {
		FOREACH_HDRCELL(col, m_decompCols) {
			DrawTableCell(dc, row, col);
		}
	}
}

void wxAdvTable::DrawTableCell(wxDC &dc, size_t row, size_t col)
{
	bool highlighed = IsCellHighlighted(row, col);
	bool selected = IsCellSelected(row, col);
	bool focused = IsCellFocused(row, col);

	if (selected) {
		dc.SetBrush(m_selectedBgBrush);
	}
	else {
		dc.SetBrush(m_bgBrush);
	}

	if (highlighed) {
		dc.SetBrush(m_highlightedBgBrush);
	}

	if (focused) {
		dc.SetBrush(m_focusedBgBrush);
		dc.SetPen(m_focusedPen);
	}
	else {
		dc.SetPen(m_gridPen);
	}

	size_t modelRow, modelCol;
	ToModelCellCoord(row, col, modelRow, modelCol);

	wxRect rc = GetCellRect(row, col);
	wxString value = m_model->GetCellValue(row, col);
	wxAdvCellAttribute *attr = m_model->GetCellAttribute(row, col);
	if (attr == NULL) {
		attr = &m_defaultCellAttribute;
	}

	dc.DrawRectangle(rc);
	GetRendererForCell(row, col)->Draw(this, dc, rc, value, selected, focused, attr);
}

wxPoint wxAdvTable::ToViewportPosition(wxPoint &pt, bool transVert, bool transHoriz)
{
	int dx, dy;
	int xunit, yunit;

	GetViewStart(&dx, &dy);
	GetScrollPixelsPerUnit(&xunit, &yunit);

	int tx = (transHoriz) ? (dx * xunit) : 0;
	int ty = (transVert) ? (dy * yunit) : 0;

	return wxPoint(pt.x + tx, pt.y + ty);
}

bool wxAdvTable::GetCellAt(wxPoint pt, size_t &cellRow, size_t &cellCol)
{
	pt = ToViewportPosition(pt);

	FOREACH_HDRCELL(row, m_decompRows) {
		wxAdvHdrCell *rowCell = m_decompRows[row];

		if (pt.y > rowCell->m_rc.y && pt.y < (rowCell->m_rc.y + rowCell->m_rc.height)) {

			FOREACH_HDRCELL(col, m_decompCols) {
				wxAdvHdrCell *colCell = m_decompCols[col];

				if (pt.x > colCell->m_rc.x && pt.x < (colCell->m_rc.x + colCell->m_rc.width)) {
					cellRow = row;
					cellCol = col;
					return true;
				}
			}
		}
	}
	return false;
}

wxAdvHdrCell *wxAdvTable::GetHdrCellAt(wxPoint pt)
{
	wxAdvHdrCell *cell;
	if (m_showRows) {
		wxPoint ptRow = ToViewportPosition(pt, true, false);

		FOREACH_HDRCELL(row, m_rows) {
			if ((cell = GetHdrCellAtRecursive(&m_rows[row], ptRow)) != NULL) {
				return cell;
			}
		}
	}

	if (m_showCols) {
		wxPoint ptCol = ToViewportPosition(pt, false, true);

		FOREACH_HDRCELL(col, m_cols) {
			if ((cell = GetHdrCellAtRecursive(&m_cols[col], ptCol)) != NULL) {
				return cell;
			}
		}
	}
	return NULL;
}

wxAdvHdrCell *wxAdvTable::GetHdrCellAtRecursive(wxAdvHdrCell *cell, wxPoint &pt)
{
	if (cell->m_rc.Contains(pt)) {
		return cell;
	}
	else if (cell->IsComposite()) {
		FOREACH_HDRCELL(n, cell->m_subCells) {
			wxAdvHdrCell *subCell = GetHdrCellAtRecursive(&cell->m_subCells[n], pt);
			if (subCell != NULL) {
				return subCell;
			}
		}
	}
	return NULL;
}

wxString wxAdvTable::GetCellValue(size_t row, size_t col)
{
	ToModelCellCoord(row, col, row, col);
	return m_model->GetCellValue(row, col);
}

bool wxAdvTable::IsCellEditable(size_t row, size_t col)
{
	ToModelCellCoord(row, col, row, col);
	return m_model->IsCellEditable(row, col);
}

wxAdvTableCellEditor *wxAdvTable::GetEditorForCell(size_t row, size_t col)
{
	ToModelCellCoord(row, col, row, col);
	int format = m_model->GetCellFormat(row, col);

	if (m_editors.find(format) == m_editors.end()) {
		return NULL;
	}
	return m_editors[format];
}

void wxAdvTable::SetEditorForFormat(int format, wxAdvTableCellEditor *editor)
{
	if (m_editors.find(format) != m_editors.end()) {
		delete m_editors[format];
	}
	m_editors[format] = editor;
}

wxAdvTableCellRenderer *wxAdvTable::GetRendererForCell(size_t row, size_t col)
{
	ToModelCellCoord(row, col, row, col);
	int format = m_model->GetCellFormat(row, col);

	if (m_renderers.find(format) == m_renderers.end()) {
		return m_defaultRenderer;
	}
	return m_renderers[format];
}

void wxAdvTable::SetRendererForFormat(int format, wxAdvTableCellRenderer *renderer)
{
	if (m_renderers.find(format) != m_renderers.end()) {
		delete m_renderers[format];
	}
	m_renderers[format] = renderer;
	RedrawAll();
}

void wxAdvTable::SetDefaultRenderer(wxAdvTableCellRenderer *renderer)
{
	SAFE_REPLACE(m_defaultRenderer, renderer);
	RedrawAll();
}

void wxAdvTable::ToModelCellCoord(size_t row, size_t col, size_t &modelRow, size_t &modelCol)
{
	if (m_sorter == NULL) {
		modelRow = row;
		modelCol = col;
	}
	else if (m_sortMode == Rows) {
		modelRow = m_sortOrder[row];
		modelCol = col;
	}
	else if (m_sortMode == Cols) {
		modelRow = row;
		modelCol = m_sortOrder[col];
	}
	else {
		modelRow = row;
		modelCol = col;
	}
}

void wxAdvTable::ToTableCellCoord(size_t row, size_t col, size_t &tableRow, size_t &tableCol)
{
	if (m_sorter == NULL) {
		tableRow = row;
		tableCol = col;
	}
	else if (m_sortMode == Rows && m_sortingIndex < m_sortOrder.Count()) {
		for (size_t n = 0; n < m_sortOrder.Count(); n++) {
			if (m_sortOrder[n] == row) {
				tableRow = n;
				break;
			}
		}

		tableCol = col;
	}
	else if (m_sortMode == Cols && m_sortingIndex < m_sortOrder.Count()) {
		tableRow = row;

		for (size_t n = 0; n < m_sortOrder.Count(); n++) {
			if (m_sortOrder[n] == row) {
				tableCol = n;
				break;
			}
		}
	}
	else {
		tableRow = row;
		tableCol = col;
	}
}

bool wxAdvTable::GetCellRect(wxRect &rc, size_t row, size_t col)
{
	if (row < m_decompRows.Count() && col < m_decompCols.Count()) {
		rc.x = m_decompCols[col]->m_rc.x;
		rc.y = m_decompRows[row]->m_rc.y;
		rc.width = m_decompCols[col]->m_rc.width;
		rc.height = m_decompRows[row]->m_rc.height;
		return true;
	}
	return false;
}

wxRect wxAdvTable::GetCellRect(size_t row, size_t col)
{
	wxRect rc;
	GetCellRect(rc, row, col);
	return rc;
}

void wxAdvTable::GetRangeRect(wxRect &rc, size_t row1, size_t col1, size_t row2, size_t col2)
{
	wxRect rcCell1;
	wxRect rcCell2;

	GetCellRect(rcCell1, row1, col1);
	GetCellRect(rcCell2, row2, col2);

	rc.x = rcCell1.x;
	rc.y = rcCell1.y;

	rc.width = rcCell2.x - rcCell1.x + rcCell2.width;
	rc.height = rcCell2.y - rcCell1.y + rcCell2.height;
}

//
// Selection functions
//
void wxAdvTable::SetSelection(wxAdvRange &range)
{
	SetSelection(range.m_row1, range.m_col1, range.m_row2, range.m_col2);
}

void wxAdvTable::SetSelection(size_t row, size_t col)
{
	SetSelection(row, col, row, col);
}

void wxAdvTable::SetSelection(size_t row1, size_t col1, size_t row2, size_t col2)
{
	wxAdvRange oldSelected = m_selected;

	m_selected.Set(row1, col1, row2, col2);

	RedrawRange(&oldSelected);
	RedrawRange(&m_selected);
}

void wxAdvTable::SelectCells(wxMouseEvent &ev, size_t row, size_t col)
{
	wxAdvRange oldSelected = m_selected; // save old selection to redraw later

	switch (m_selectMode) {
	case SelectCell:
		m_selected.Set(row, col);
		break;
	case SelectRows:
		m_selected.Set(m_pressedCell.Row(), 0, row, GetDecompColCount() - 1);
		break;
	case SelectCols:
		m_selected.Set(0, m_pressedCell.Col(), GetDecompRowCount() - 1, col);
		break;
	case SelectBlock:
		m_selected.Set(m_pressedCell.Row(), m_pressedCell.Col(), row, col);
		break;
	}

	SendRangeEvent(wxEVT_GRID_RANGE_SELECT,
			m_selected.m_row1, m_selected.m_col1,
			m_selected.m_row2, m_selected.m_col2,
			ev);

	RedrawRange(&oldSelected);
	// redraw new selected cells
	RedrawRange(&m_selected);
}

void wxAdvTable::SetFocusedCell(size_t row, size_t col)
{
	if (row >= GetDecompRowCount() || col >= GetDecompColCount()) {
		return ;
	}

	if (m_focused.Equal(row, col)) {
		return ;
	}

	wxAdvCoord oldFocused = m_focused;

	m_focused.Set(row, col);

	RedrawHighlighted(&oldFocused);

	StopEditing();

	RedrawHighlighted(&m_focused);
}

void wxAdvTable::SetPressedHdrCell(wxAdvHdrCell *cell)
{
	if (m_pressedHdrCell == cell) {
		return ;
	}

	if (m_pressedHdrCell != NULL) {
		wxAdvHdrCell *oldPressedHdrCell = m_pressedHdrCell;
		m_pressedHdrCell = NULL;
		RedrawHdrCell(oldPressedHdrCell);
	}

	m_pressedHdrCell = cell;
	if (m_pressedHdrCell != NULL) {
		RedrawHdrCell(m_pressedHdrCell);
	}
}

void wxAdvTable::SetCurrentHdrCell(wxAdvHdrCell *cell)
{
	if (m_currentHdrCell == cell) {
		return ;
	}

	if (m_currentHdrCell != NULL) {
		wxAdvHdrCell *oldCurrentHdrCell = m_currentHdrCell;
		m_currentHdrCell = NULL;
		RedrawHdrCell(oldCurrentHdrCell);
	}

	m_currentHdrCell = cell;
	if (m_currentHdrCell != NULL) {
		RedrawHdrCell(m_currentHdrCell);
	}
}

void wxAdvTable::StopEditing()
{
	if (m_currentEditor != NULL) {
		m_currentEditor->Deactivate(this);
		m_currentEditor = NULL;
	}
}

//
// Redraw functions
//

void wxAdvTable::ResizeBackBitmaps()
{
	if (!m_tableCreated) {
		return ;
	}

	wxSize size = GetClientSize();

	wxSize dataSize = size;

	wxCoord rowsHeight = CalcTotalRowsHeight();
	wxCoord colsWidth = CalcTotalColsWidth();

	if (m_showRows) {
		wxCoord rowsWidth = CalcTotalRowLayersWidth();

		dataSize.x -= rowsWidth;
		m_backBitmapRows.Create(rowsWidth, MIN(rowsHeight, size.GetHeight()));
	}
	else {
		m_backBitmapRows.Create(0, 0);
	}

	if (m_showCols) {
		wxCoord colsHeight = CalcTotalColLayersHeight();

		dataSize.y -= colsHeight;
		m_backBitmapCols.Create(MIN(colsWidth, size.GetWidth()), colsHeight);
	}
	else {
		m_backBitmapCols.Create(0, 0);
	}

	if ((!m_showCols && !m_showRows) && m_showCorner) {
		dataSize.x -= CalcTotalRowLayersWidth();
		dataSize.y -= CalcTotalColLayersHeight();
	}

	m_backBitmap.Create(MIN(colsWidth, dataSize.GetWidth()), MIN(rowsHeight, dataSize.GetHeight()));

	RedrawAll();
}

void wxAdvTable::RedrawRange(wxAdvRange *range)
{
	RedrawRange(range->m_row1, range->m_col1, range->m_row2, range->m_col2);
}

void wxAdvTable::RedrawRange(size_t row1, size_t col1, size_t row2, size_t col2)
{
	wxAdvRange visibleRange;
	if (!GetVisibleRange(visibleRange)) {
		return ;
	}

	int xViewStart, yViewStart;
	CalcUnscrolledPosition(0, 0, &xViewStart, &yViewStart);

	row1 = MAX(row1, visibleRange.m_row1);
	col1 = MAX(col1, visibleRange.m_col1);
	row2 = MIN(row2, visibleRange.m_row2);
	col2 = MIN(col2, visibleRange.m_col2);

	wxMemoryDC dc(m_backBitmap);

	wxRect rcData = CalcTableRect();
	dc.SetDeviceOrigin(-xViewStart -rcData.x, -yViewStart -rcData.y);

	for (size_t row = row1; row <= row2; row++) {
		for (size_t col = col1; col <= col2; col++) {
			DrawTableCell(dc, row, col);
		}
	}

	wxRect rc;
	GetRangeRect(rc, row1, col1, row2, col2);
	rc.x -= xViewStart;
	rc.y -= yViewStart;

	RefreshRect(rc);
}

void wxAdvTable::RedrawAll()
{
	if (!m_tableCreated) {
		return ;
	}

	int xViewStart, yViewStart;
	CalcUnscrolledPosition(0, 0, &xViewStart, &yViewStart);

	wxRect rcData = CalcTableRect();

	wxMemoryDC dataDc(m_backBitmap);
	dataDc.SetDeviceOrigin(-xViewStart -rcData.x, -yViewStart -rcData.y);
	DrawTable(dataDc);

	// draw rows
	if (m_showRows) {
		wxMemoryDC rowsDc(m_backBitmapRows);
		rowsDc.SetDeviceOrigin(0, -yViewStart -rcData.y);
		DrawHdrCells(rowsDc, m_rows);
	}

	// draw columns
	if (m_showCols) {
		wxMemoryDC colsDc(m_backBitmapCols);
		colsDc.SetDeviceOrigin(-xViewStart -rcData.x, 0);
		DrawHdrCells(colsDc, m_cols);
	}

	Refresh();
}

void wxAdvTable::RedrawHdrCell(wxAdvHdrCell *cell)
{
	wxRect rc = cell->m_rc;

	wxRect rcData = CalcTableRect();

	int xViewStart, yViewStart;
	CalcUnscrolledPosition(0, 0, &xViewStart, &yViewStart);

	if (cell->m_isRow) {
		wxMemoryDC dc(m_backBitmapRows);
		dc.SetDeviceOrigin(0, -yViewStart -rcData.y);

		DrawHdrCell(dc, cell);

		rc.y -= yViewStart;
	}
	else {
		wxMemoryDC dc(m_backBitmapCols);
		dc.SetDeviceOrigin(-xViewStart -rcData.x, 0);

		DrawHdrCell(dc, cell);

		rc.x -= xViewStart;
	}

	RefreshRect(rc);
}

void wxAdvTable::RedrawHighlighted(wxAdvCoord *coord)
{
	if (!coord->IsSet()) {
		return ;
	}

	switch (m_highlightMode) {
		case HighlightNone:
			RedrawRange(coord->Row(), coord->Col(), coord->Row(), coord->Col());
			break;
		case HighlightRows:
			RedrawRange(coord->Row(), 0, coord->Row(), GetDecompColCount() - 1);
			break;
		case HighlightCols:
			RedrawRange(0, coord->Col(), GetDecompRowCount() - 1, coord->Col());
			break;
		case HighlightBoth:
			RedrawRange(coord->Row(), 0, coord->Row(), GetDecompColCount() - 1);
			RedrawRange(0, coord->Col(), GetDecompRowCount() - 1, coord->Col());
			break;
	}
}

bool wxAdvTable::GetVisibleRange(wxAdvRange &range)
{
	size_t row1 = (size_t) -1;
	size_t col1 = (size_t) -1;
	size_t row2 = (size_t) -1;
	size_t col2 = (size_t) -1;

	int xViewStart, yViewStart;
	CalcUnscrolledPosition(0, 0, &xViewStart, &yViewStart);

	wxRect tableRc = CalcTableRect();
	wxCoord x0 = tableRc.x + xViewStart;
	wxCoord y0 = tableRc.y + yViewStart;
	wxCoord x1 = tableRc.x + m_backBitmap.GetWidth() + xViewStart;
	wxCoord y1 = tableRc.y + m_backBitmap.GetHeight() + yViewStart;

	for (size_t row = 0; row < m_decompRows.Count(); row++) {
		wxAdvHdrCell *rowCell = m_decompRows[row];

		if (y0 >= rowCell->m_rc.y &&
			y0 <= (rowCell->m_rc.y + rowCell->m_rc.height)) {
				row1 = row;
				break;
		}
	}
	for (size_t col = 0; col < m_decompCols.Count(); col++) {
		wxAdvHdrCell *colCell = m_decompCols[col];

		if (x0 >= colCell->m_rc.x &&
			x0 <= (colCell->m_rc.x + colCell->m_rc.width)) {
				col1 = col;
				break;
		}
	}

	if (row1 == (size_t) -1 || col1 == (size_t) -1) {
		return false;
	}

	for (size_t row = row1; row < m_decompRows.Count(); row++) {
		wxAdvHdrCell *rowCell = m_decompRows[row];

		row2 = row;

		if (y1 > rowCell->m_rc.y &&
			y1 <= (rowCell->m_rc.y + rowCell->m_rc.height)) {
				break;
		}
	}
	for (size_t col = col1; col < m_decompCols.Count(); col++) {
		wxAdvHdrCell *colCell = m_decompCols[col];

		col2 = col;
		if (x1 > colCell->m_rc.x &&
			x1 <= (colCell->m_rc.x + colCell->m_rc.width)) {
				break;
		}
	}

	range.Set(row1, col1, row2, col2);
	return true;
}

wxRect wxAdvTable::CalcEntireRowRect(size_t row, size_t col)
{
	wxRect rcTable = CalcTableRect();
	wxRect rc;

	rc = GetCellRect(row, col);
	rc.x = rcTable.x;
	rc.width = rcTable.width;
	return rc;
}

wxRect wxAdvTable::CalcEntireColRect(size_t row, size_t col)
{
	wxRect rcTable = CalcTableRect();
	wxRect rc;

	rc = GetCellRect(row, col);
	rc.y = rcTable.y;
	rc.height = rcTable.height;
	return rc;
}

bool wxAdvTable::IsCellHighlighted(size_t row, size_t col)
{
	switch (m_highlightMode) {
		case HighlightNone:
			return false;
		case HighlightRows:
			return m_focused.Row() == row;
		case HighlightCols:
			return m_focused.Col() == col;
		case HighlightBoth:
			return (m_focused.Row() == row) || (m_focused.Col() == col);
		default:
			return false;
	}
}

//
// Event send functions.
//

void wxAdvTable::SendEvent(const wxEventType type, size_t row, size_t col, wxMouseEvent &mouseEv)
{
	wxGridEvent ev(GetId(), type, this,
               (int) row, (int) col,
               mouseEv.GetX(),
               mouseEv.GetY(),
               false/*XXX FIXME*/,
               mouseEv.ControlDown(),
               mouseEv.ShiftDown(),
               mouseEv.AltDown(),
               mouseEv.MetaDown());

	GetEventHandler()->ProcessEvent(ev);
}

void wxAdvTable::SendRangeEvent(const wxEventType type, size_t row1, size_t col1, size_t row2, size_t col2, wxMouseEvent &mouseEv)
{
	wxGridRangeSelectEvent ev(GetId(), type, this,
               wxGridCellCoords((int) row1, (int) col1),
               wxGridCellCoords((int) row2, (int) col2),
               false/*XXX FIXME*/,
               mouseEv.ControlDown(),
               mouseEv.ShiftDown(),
               mouseEv.AltDown(),
               mouseEv.MetaDown());

	GetEventHandler()->ProcessEvent(ev);
}

void wxAdvTable::EditCell(wxAdvTableCellEditor *editor, size_t row, size_t col)
{
	if (m_currentEditor != NULL) {
		m_currentEditor->Deactivate(this);
	}

	editor->Activate(this, row, col);
	m_currentEditor = editor;
}

//
// Event handlers
//
void wxAdvTable::OnPaint(wxPaintEvent &WXUNUSED(ev))
{
	if (m_needRedraw) {
		RedrawAll();
		m_needRedraw = false;
	}

	//wxAutoBufferedPaintDC dc(this);
	wxPaintDC dc(this);

	/*
	dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(GetBackgroundColour()));
	dc.SetPen(*wxThePenList->FindOrCreatePen(GetBackgroundColour(), 1, wxSOLID));
	dc.DrawRectangle(GetClientRect());
	*/

	if (!m_tableCreated) {
		return ;
	}

	if (m_showRows) {
		wxRect rcRows = CalcRowsRect();
		dc.DrawBitmap(m_backBitmapRows, rcRows.x, rcRows.y);
	}

	if (m_showCols) {
		wxRect rcCols = CalcColsRect();
		dc.DrawBitmap(m_backBitmapCols, rcCols.x, rcCols.y);
	}

	if (m_showCorner) {
		DrawHdrCell(dc, &m_cornerCell);
	}

	// draw data
	wxRect rcData = CalcTableRect();
	dc.DrawBitmap(m_backBitmap, rcData.x, rcData.y);
}

void wxAdvTable::OnMouseEvents(wxMouseEvent &ev)
{
	wxPoint pt = ev.GetPosition();

	size_t row, col;
	wxAdvHdrCell *cell = GetHdrCellAt(pt);

	if (cell != NULL) {
		HandleHdrCellMouseEvent(ev, cell);
	}
	else if (GetCellAt(pt, row, col)) {
		HandleCellMouseEvent(ev, row, col);
	}

	if (ev.Leaving() || cell == NULL) {
		if (m_currentHdrCell != NULL) {
			SetCurrentHdrCell(NULL);
			SetPressedHdrCell(NULL);
		}
	}

	if (ev.ButtonUp() || ev.Leaving()) {
		SetPressedHdrCell(NULL);
	}

	ev.Skip();
}

void wxAdvTable::HandleHdrCellMouseEvent(wxMouseEvent &ev, wxAdvHdrCell *cell)
{
	if (ev.LeftDown()) {
		SetPressedHdrCell(cell);
	}
	else if (ev.LeftUp()) {
		if (cell == m_pressedHdrCell && m_pressedHdrCell != NULL) {
			if ((m_sorter != NULL && m_pressedHdrCell->m_sortable && m_pressedHdrCell->m_isDecomp) &&
				((!m_pressedHdrCell->m_isRow && m_sortMode == Rows) ||
				(m_pressedHdrCell->m_isRow && m_sortMode == Cols))) {
				SetSortingIndex(m_pressedHdrCell->m_index);
			}
		}

		SetPressedHdrCell(NULL);
	}
	else if (ev.LeftDClick()) {
	}

	SetCurrentHdrCell(cell);
}

void wxAdvTable::HandleCellMouseEvent(wxMouseEvent &ev, size_t row, size_t col)
{

	if (ev.LeftDown()) {
		m_pressedCell.Set(row, col);

		SetFocusedCell(row, col);
		SelectCells(ev, row, col);

		//Refresh();
	}
	else if (ev.Dragging() && ev.m_leftDown) {
		SetFocusedCell(row, col);
		SelectCells(ev, row, col);

		//Refresh();
	}
	else if (ev.LeftUp()) {
		if (m_pressedCell.Equal(row, col)) {
			if (IsCellEditable(row, col)) {
				wxAdvTableCellEditor *editor = GetEditorForCell(row, col);

				if (editor != NULL && editor->OneClickActivate()) {
					EditCell(editor, row, col);
				}
			}

			SendEvent(wxEVT_GRID_CELL_LEFT_CLICK, row, col, ev);
		}
	}
	else if (ev.LeftDClick()) {
		if (IsCellEditable(row, col)) {
			wxAdvTableCellEditor *editor = GetEditorForCell(row, col);
			if (editor != NULL) {
				EditCell(editor, row, col);
			}
		}

		SendEvent(wxEVT_GRID_CELL_LEFT_DCLICK, row, col, ev);
	}
	else if (ev.RightDClick()) {
		SendEvent(wxEVT_GRID_CELL_RIGHT_DCLICK, row, col, ev);
	}
}

void wxAdvTable::OnKeyDown(wxKeyEvent &WXUNUSED(ev))
{
	// TODO handle alphanumber keycodes to activate editor
	// TODO add selection by arrow keys
}

void wxAdvTable::OnKillFocus(wxFocusEvent &WXUNUSED(ev))
{
	StopEditing();
}

void wxAdvTable::OnSize(wxSizeEvent &WXUNUSED(ev))
{
	ResizeBackBitmaps();
}

void wxAdvTable::Update()
{
	m_needRedraw = true;
	Refresh();
}

void wxAdvTable::CellChanged(size_t row, size_t col)
{
	wxRect rc = GetCellRect(row, col);
	RedrawRange(row, col, row, col);
}

void wxAdvTable::TableChanged()
{
	RedrawAll();
}

