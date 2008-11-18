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

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(wxAdvHdrCellArray);
WX_DEFINE_OBJARRAY(wxAdvHdrCellPtrArray);
WX_DEFINE_OBJARRAY(wxAdvRangeArray);
WX_DEFINE_OBJARRAY(wxCoordArray);
WX_DEFINE_OBJARRAY(wxIndexArray);
WX_DEFINE_OBJARRAY(wxArrayArrayString);


static wxAdvHdrCell stdRow(wxEmptyString);
static wxAdvHdrCell stdCol(wxEmptyString);

size_t wxAdvHdrCell::GetRealCellCount(wxAdvHdrCell *cells, size_t numCells)
{
	size_t count = 0;

	for (size_t n = 0; n < numCells; n++) {
		count += cells[n].GetRealCellCount();
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

bool wxAdvStringTableDataModel::IsCellEditable(size_t row, size_t col)
{
	return !m_readOnly;
}

int wxAdvStringTableDataModel::GetCellFormat(size_t row, size_t col)
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

wxAdvCellAttribute &wxAdvStringTableDataModel::GetCellAttribute(size_t row, size_t col)
{
	return m_defaultCellAttribute;
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

wxAdvCellAttribute &wxAdvFilterTableDataModel::GetCellAttribute(size_t row, size_t col)
{
	return m_undelayingModel->GetCellAttribute(row, col);
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

void wxAdvBoolTableCellRenderer::Draw(wxAdvTable *table, wxDC &dc, wxRect rc, wxString value, bool selected, bool focused, wxAdvCellAttribute &attr)
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

void wxAdvStringCellRenderer::Draw(wxAdvTable *table, wxDC &dc, wxRect rc, wxString value, bool selected, bool focused, wxAdvCellAttribute &attr)
{
	// TODO implement cell vertical/horizontal alignments, colored text, and fonts
	wxDCClipper clip(dc, rc);

	dc.SetBrush(attr.Brush());
	dc.DrawRectangle(rc);
	dc.SetFont(attr.Font());
	//dc.SetTextColor(
	dc.DrawLabel(value, rc, attr.Alignment());
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

void wxAdvTableCellEditor::SetNewValue(wxAdvTable *table, wxString newValue)
{
	wxCHECK_RET(m_cell.IsSet(), "wxAdvTableCellEditor::SetNewValue: m_cell.IsSet()");
	table->GetModel()->SetCellValue(m_cell.Row(), m_cell.Col(), newValue);
}

void wxAdvTableCellEditor::EndEditing()
{
	wxCHECK_RET(m_table != NULL, "wxAdvTableCellEditor::EndEditing");
	m_table->StopEditing();
}

//
// wxAdvStringTableCellEditor
//
BEGIN_EVENT_TABLE(wxAdvStringTableCellEditor, wxEvtHandler)
	EVT_TEXT_ENTER(wxID_ANY, wxAdvStringTableCellEditor::OnTextEnter)
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

	m_textCtrl->ChangeValue(table->GetModel()->GetCellValue(row, col));

	m_textCtrl->SetSize(rc.x, rc.y, rc.width, rc.height);
	m_textCtrl->Show();

	m_textCtrl->SetFocus();
}

void wxAdvStringTableCellEditor::Deactivate(wxAdvTable *table)
{
	m_textCtrl->Show(false);

	SetNewValue(table, m_textCtrl->GetValue());
}

void wxAdvStringTableCellEditor::OnTextEnter(wxCommandEvent &ev)
{
	EndEditing();
}

//
// wxAdvBoolTableCellEditor
//
wxAdvBoolTableCellEditor::wxAdvBoolTableCellEditor(wxAdvTable *table)
{
	m_checkBox = new wxCheckBox(table, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	m_checkBox->Show(false);
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

	bool checked = table->GetModel()->GetCellValue(row, col).Cmp(boolTrue) == 0;
	m_checkBox->SetValue(!checked);

	m_checkBox->SetSize(rc.x, rc.y, rc.width, rc.height);
	m_checkBox->Show();

	m_checkBox->SetFocus();
}

void wxAdvBoolTableCellEditor::Deactivate(wxAdvTable *table)
{
	m_checkBox->Show(false);

	bool checked = m_checkBox->IsChecked();
	SetNewValue(table, (checked) ? boolTrue : boolFalse);
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
		m_sortDirection = o.m_sortDirection;
	}

	SortHelper(wxAdvTable *table, wxAdvTableSorter *sorter, size_t row, size_t col, int sortDirection)
	{
		m_table = table;
		m_sorter = sorter;
		m_row = row;
		m_col = col;
		m_sortDirection = sortDirection;
	}

	virtual ~SortHelper()
	{
	}

	wxAdvTableSorter *m_sorter;
	wxAdvTable *m_table;
	size_t m_row;
	size_t m_col;
	int m_sortDirection;
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
	wxString value1 = table->GetCellValue(row1, col1);
	wxString value2 = table->GetCellValue(row2, col2);

	return Compare(value1, value2);
}

int wxAdvTableStringSorter::Compare(wxString value1, wxString value2)
{
	return value1.Cmp(value2);
}

//
// wxAdvTable
//
BEGIN_EVENT_TABLE(wxAdvTable, wxScrolledWindow)
	EVT_PAINT(wxAdvTable::OnPaint)
	EVT_MOUSE_EVENTS(wxAdvTable::OnMouseEvents)
	EVT_KEY_DOWN(wxAdvTable::OnKeyDown)
	EVT_KILL_FOCUS(wxAdvTable::OnKillFocus)
END_EVENT_TABLE()

wxAdvTable::wxAdvTable(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &m_size)
: wxScrolledWindow(parent, id, pos, m_size, wxHSCROLL | wxVSCROLL | wxFULL_REPAINT_ON_RESIZE),
m_cornerCell(wxEmptyString)
{
	m_tableCreated = false;
	m_model = NULL;
	m_defaultRenderer = new wxAdvStringCellRenderer();
	m_sorter = NULL;
	m_sortMode = Rows;
	m_highlightMode = HighlightNone;
	m_clickedCell = NULL;
	m_currentHdrCell = NULL;
	m_sortDirection = Ascending;

	m_currentEditor = NULL;

	m_selectMode = SelectBlock;

	m_showRows = true;
	m_showCols = true;
	m_showCorner = true;

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

	m_realRows.Clear();
	m_realCols.Clear();

	m_rowLayerWidths.Clear();
	m_colLayerHeights.Clear();

	m_cellConcats.Clear();

	SAFE_DELETE(m_model);

	ClearSelection();
}

void wxAdvTable::AddRows(wxAdvHdrCell *_rows, size_t numRows)
{
	wxCHECK_RET(m_tableCreated, wxT("wxAdvTable::AddRows: Table is not created"));

	AddHdrCells(m_rows, _rows, numRows, true);
	CalcTableGeometry();
}

void wxAdvTable::AddCols(wxAdvHdrCell *_cols, size_t numCols)
{
	wxCHECK_RET(m_tableCreated, wxT("wxAdvTable::AddCols: Table is not created"));

	AddHdrCells(m_cols, _cols, numCols, false);
	CalcTableGeometry();
}

//
// Table geometry routines.
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
	m_realRows.Clear();
	m_realCols.Clear();

	FindRealSubCells(m_rows, m_realRows);
	FindRealSubCells(m_cols, m_realCols);

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

	SetVirtualSize(virtWidth, virtHeight);
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
		wxAdvHdrCell &subRowCol = hdrCell.m_subCells[n];

		wxSize subSize;
		if (subRowCol.IsComposite()) {
			subSize = FitForSubCells(dc, subRowCol, isRows);
		}
		else {
			subSize = subRowCol.CalcExtent(dc);
		}

		if (isRows) {
			size.x = MAX(size.x, subSize.x);
			size.y += subSize.y;
		}
		else {
			size.x += subSize.x;
			size.y = MAX(size.y, subSize.y);
		}

		subRowCol.m_rc.SetPosition(wxPoint(0, 0));
		subRowCol.m_rc.SetSize(subSize);
	}

	hdrCell.m_rc.SetPosition(wxPoint(0, 0));
	hdrCell.m_rc.SetSize(size);
	return size;
}

void wxAdvTable::CalcLayerSizes(wxAdvHdrCell &hdrCell, wxCoordArray &layerSizes, bool isRows, size_t nLayer)
{
	FOREACH_HDRCELL(n, hdrCell.m_subCells) {
		wxAdvHdrCell &subRowCol = hdrCell.m_subCells[n];
		CalcLayerSizes(subRowCol, layerSizes, isRows, nLayer + 1);
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
		wxAdvHdrCell &subRowCol = hdrCell.m_subCells[n];

		wxCoord x, y;
		if (isRows) {
			x = x0 + layerSizes[nLayer];
			y = y0;
		}
		else {
			x = x0;
			y = y0 + layerSizes[nLayer];
		}

		UpdateHeaderPositions(subRowCol, x, y, layerSizes, nLayer + 1, isRows);

		if (isRows) {
			y0 += subRowCol.m_rc.height;
		}
		else {
			x0 += subRowCol.m_rc.width;
		}
	}
}

void wxAdvTable::UpdateHeaderSizes(wxAdvHdrCell &hdrCell, wxCoordArray &layerSizes, bool isRows, size_t nLayer)
{
	FOREACH_HDRCELL(n, hdrCell.m_subCells) {
		wxAdvHdrCell &subRowCol = hdrCell.m_subCells[n];
		UpdateHeaderSizes(subRowCol, layerSizes, isRows, nLayer + 1);
	}

	wxCoord size = SumLayerSizes(layerSizes, nLayer, LastLayer(hdrCell, layerSizes));
	if (isRows) {
		hdrCell.m_rc.width = size;
	}
	else {
		hdrCell.m_rc.height = size;
	}
}

void wxAdvTable::FindRealSubCells(wxAdvHdrCellArray &hdrCells, wxAdvHdrCellPtrArray &flattenRowCols)
{
	FOREACH_HDRCELL(n, hdrCells) {
		wxAdvHdrCell &hdrCell = hdrCells[n];
		FindRealSubCell(hdrCell, flattenRowCols);
	}
}

void wxAdvTable::FindRealSubCell(wxAdvHdrCell &hdrCell, wxAdvHdrCellPtrArray &flattenRowCols)
{
	if (!hdrCell.IsComposite()) {
		hdrCell.m_isReal = true;
		hdrCell.m_index = flattenRowCols.Count();
		flattenRowCols.Add(&hdrCell);
	}
	else {
		FOREACH_HDRCELL(n, hdrCell.m_subCells) {
			FindRealSubCell(hdrCell.m_subCells[n], flattenRowCols);
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
			cell = (isRows) ? &stdRow : &stdCol;
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
// Sorting, selection, highlight, etc routines.
//
void wxAdvTable::SetSorter(wxAdvTableSorter *_sorter)
{
	SAFE_REPLACE(m_sorter, _sorter);
	UpdateSortOrder();
}

void wxAdvTable::SetSortMode(SortMode _sortMode)
{
	m_sortMode = _sortMode;
	UpdateSortOrder();
}

void wxAdvTable::UpdateSortOrder()
{
	if (m_sorter == NULL)
		return ;

	if (m_sortMode == Rows) {
		if (m_sortingIndex >= m_realCols.Count())
			return ;

		size_t rowCount = m_realRows.Count();

		wxSortHelperArray helpers(SorterFunc);
		helpers.Alloc(rowCount);

		for (size_t row = 0; row < rowCount; row++) {
			helpers.Add(new SortHelper(this, m_sorter, row, m_sortingIndex, m_sortDirection));
		}

		m_rowsOrder.Empty();
		m_rowsOrder.Alloc(rowCount);

		for (size_t row = 0; row < rowCount; row++) {
			m_rowsOrder.Add(helpers[row]->m_row);
		}
	}
	else if (m_sortMode == Cols) {
		if (m_sortingIndex >= m_realRows.Count())
			return ;

		size_t colCount = m_realCols.Count();

		wxSortHelperArray helpers(SorterFunc);
		helpers.Alloc(colCount);

		for (size_t col = 0; col < colCount; col++) {
			helpers.Add(new SortHelper(this, m_sorter, m_sortingIndex, col, m_sortDirection));
		}

		m_colsOrder.Empty();
		m_colsOrder.Alloc(colCount);

		for (size_t col = 0; col < colCount; col++) {
			m_colsOrder.Add(helpers[col]->m_col);
		}
	}
	else {
		return ;
	}

	Refresh();
}

void wxAdvTable::SetSortingIndex(size_t _sortingIndex)
{
	if (m_sortingIndex == _sortingIndex) {
		if (m_sortDirection == Ascending) {
			m_sortDirection = Descending;
		}
		else {
			m_sortDirection = Ascending;
		}
	}
	else {
		m_sortDirection = Ascending;
	}

	m_sortingIndex = _sortingIndex;
	UpdateSortOrder();
}

void wxAdvTable::SetSelectMode(SelectMode _selectMode)
{
	if (m_selectMode != _selectMode) {
		m_selectMode = _selectMode;
		Refresh();
	}
}

int wxAdvTable::GetSelectMode()
{
	return m_selectMode;
}

void wxAdvTable::SetHighlightMode(HighlightMode _highlightMode)
{
	if (m_highlightMode != _highlightMode) {
		m_highlightMode = _highlightMode;
		Refresh();
	}
}

void wxAdvTable::ClearSelection()
{
	m_focused.Unset();
	m_sel.Unset();

	Refresh();
}

void wxAdvTable::SetShowHeaders(bool showRows, bool showCols, bool showCorner)
{
	m_showRows = showRows;
	m_showCols = showCols;
	m_showCorner = showCorner;

	wxClientDC dc(this);
	CalcHeaderGeometry(dc);

	Refresh();
}

//
// Drawing routines
//
void wxAdvTable::DrawHdrCells(wxDC &dc, wxAdvHdrCellArray &hdrCells)
{
	FOREACH_HDRCELL(n, hdrCells) {
		DrawHdrCell(dc, hdrCells[n]);
	}
}

void wxAdvTable::DrawHdrCell(wxDC &dc, wxAdvHdrCell &hdrCell)
{
	DrawHeaderCell(dc, hdrCell);

	FOREACH_HDRCELL(n, hdrCell.m_subCells) {
		DrawHdrCell(dc, hdrCell.m_subCells[n]);
	}
}

void wxAdvTable::DrawHeaderCell(wxDC &dc, wxAdvHdrCell &hdrCell)
{
	wxDCClipper clip(dc, hdrCell.m_rc);
	wxRect rc = hdrCell.m_rc;

	wxHeaderSortIconType sortIcon = wxHDR_SORT_ICON_NONE;
	if (m_sortingIndex == hdrCell.m_index) {
		sortIcon = (m_sortDirection == Ascending) ? wxHDR_SORT_ICON_DOWN : wxHDR_SORT_ICON_UP;
	}

	int flags = 0;
	if (&hdrCell == m_clickedCell) {
		flags |= wxCONTROL_PRESSED;
	}
	if (&hdrCell == m_currentHdrCell) {
		flags |= wxCONTROL_SELECTED;
	}

	wxRendererNative::Get().DrawHeaderButton(this, dc, rc, flags, sortIcon);
	// TODO draw vertical text
	//dc.DrawText(hdrCell.label, hdrCell.rc.x, hdrCell.rc.y);
	dc.SetFont(*wxNORMAL_FONT);
	dc.DrawLabel(hdrCell.m_label, hdrCell.m_rc, hdrCell.m_alignHorizontal | hdrCell.m_alignVertical);
}

void wxAdvTable::DrawTable(wxDC &dc, wxRegion reg)
{
	FOREACH_HDRCELL(row, m_realRows) {
		FOREACH_HDRCELL(col, m_realCols) {
			if (reg.Intersect(GetCellRect(row, col))) {
				DrawTableCell(dc, row, col);
			}
		}
	}
}

void wxAdvTable::DrawTableCell(wxDC &dc, size_t row, size_t col)
{
	wxRect rc = GetCellRect(row, col);

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

	dc.DrawRectangle(rc);

	GetUnsortedCellIndex(row, col, row, col);

	wxString value = GetCellValue(row, col);
	wxAdvCellAttribute &attr = m_model->GetCellAttribute(row, col);

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

bool wxAdvTable::GetCellAt(wxPoint pt, size_t &row, size_t &col)
{
	pt = ToViewportPosition(pt);

	FOREACH_HDRCELL(nRow, m_realRows) {
		wxAdvHdrCell *rowCell = m_realRows[nRow];

		if (pt.y > rowCell->m_rc.y && pt.y < (rowCell->m_rc.y + rowCell->m_rc.height)) {

			FOREACH_HDRCELL(nCol, m_realCols) {
				wxAdvHdrCell *colCell = m_realCols[nCol];

				if (pt.x > colCell->m_rc.x && pt.x < (colCell->m_rc.x + colCell->m_rc.width)) {
					row = nRow;
					col = nCol;
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

		FOREACH_HDRCELL(nRow, m_rows) {
			if ((cell = GetHdrCellAtRecursive(&m_rows[nRow], ptRow)) != NULL) {
				return cell;
			}
		}
	}

	if (m_showCols) {
		wxPoint ptCol = ToViewportPosition(pt, false, true);

		FOREACH_HDRCELL(nCol, m_cols) {
			if ((cell = GetHdrCellAtRecursive(&m_cols[nCol], ptCol)) != NULL) {
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
	return m_model->GetCellValue(row, col);
}

wxAdvTableCellEditor *wxAdvTable::GetEditorForCell(size_t nRow, size_t nCol)
{
	int format = m_model->GetCellFormat(nRow, nCol);

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

wxAdvTableCellRenderer *wxAdvTable::GetRendererForCell(size_t nRow, size_t nCol)
{
	int format = m_model->GetCellFormat(nRow, nCol);

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
	Refresh();
}

void wxAdvTable::SetDefaultRenderer(wxAdvTableCellRenderer *renderer)
{
	SAFE_REPLACE(m_defaultRenderer, renderer);
	Refresh();
}

void wxAdvTable::GetUnsortedCellIndex(size_t row, size_t col, size_t &unsortedRow, size_t &unsortedCol)
{
	if (m_sortMode == Rows && m_sortingIndex < m_rowsOrder.Count()) {
		unsortedRow = m_rowsOrder[row];
		unsortedCol = col;
	}
	else if (m_sortMode == Cols && m_sortingIndex < m_rowsOrder.Count()) {
		unsortedRow = row;
		unsortedCol = m_colsOrder[col];
	}
	else {
		unsortedRow = row;
		unsortedCol = col;
	}
}

wxRect wxAdvTable::GetCellRect(size_t nRow, size_t nCol)
{
	wxRect rc;

	if (nRow < m_realRows.Count() && nCol < m_realCols.Count()) {
		rc.x = m_realCols[nCol]->m_rc.x;
		rc.y = m_realRows[nRow]->m_rc.y;
		rc.width = m_realCols[nCol]->m_rc.width;
		rc.height = m_realRows[nRow]->m_rc.height;
	}
	return rc;
}

//
// Selection functions
//
void wxAdvTable::SetSelection(wxAdvRange &range)
{
	m_selected = range;
	Refresh();
}

void wxAdvTable::SetSelection(size_t row, size_t col)
{
	SetSelection(row, col, row, col);
}

void wxAdvTable::SetSelection(size_t row1, size_t col1, size_t row2, size_t col2)
{
	m_selected.Set(row1, col1, row2, col2);
	Refresh();
}

void wxAdvTable::SelectCells(wxMouseEvent &ev, size_t row, size_t col)
{
	switch (m_selectMode) {
	case SelectCell:
		m_selected.Set(row, col);
		break;
	case SelectRows:
		m_selected.Set(m_sel.Row(), 0, row, GetRealColCount());
		break;
	case SelectCols:
		m_selected.Set(0, m_sel.Col(), GetRealRowCount(), col);
		break;
	case SelectBlock:
		m_selected.Set(m_sel.Row(), m_sel.Col(), row, col);
		break;
	}

	SendRangeEvent(wxEVT_GRID_RANGE_SELECT,
			m_selected.m_row1, m_selected.m_col1,
			m_selected.m_row2, m_selected.m_col2,
			ev);
	Refresh();
}

void wxAdvTable::SetFocusedCell(size_t row, size_t col)
{
	if (row >= GetRealRowCount() || col >= GetRealColCount())
		return ;

	//RefreshHighlightRect();

	m_focused.Set(row, col);
	// TODO scroll to new focused cell

	StopEditing();

	//RefreshHighlightRect();
	Refresh();
}

void wxAdvTable::StopEditing()
{
	if (m_currentEditor != NULL) {
		m_currentEditor->Deactivate(this);
		m_currentEditor = NULL;
	}
}

/*
void wxAdvTable::RefreshHighlightRect()
{
	if (m_focusedRow == -1 || m_focusedCol == -1) {
		return ;
	}

	wxRect rc;

	switch (m_highlightMode) {
		case HighlightNone:
			rc = GetCellRect(m_focusedRow, m_focusedCol);
			break;
		case HighlightRows:
			rc = GetCellRect(m_focusedRow, m_focusedCol);
			rc.
			break;
		case HighlightCols:
			return m_focusedCol == col;
		case HighlightBoth:
			return (m_focusedRow == row) || (m_focusedCol == col);
	}

}
*/

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

void wxAdvTable::RedrawViewportRect(wxRect rc)
{
	CalcScrolledPosition(rc.x, rc.y, &rc.x, &rc.y);
	RefreshRect(rc);
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
void wxAdvTable::OnPaint(wxPaintEvent &ev)
{
	wxAutoBufferedPaintDC dc(this);

	int dx, dy;
	int xunit, yunit;

	GetViewStart(&dx, &dy);
	GetScrollPixelsPerUnit(&xunit, &yunit);

	// draw window background
	dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(GetBackgroundColour()));
	dc.SetPen(*wxThePenList->FindOrCreatePen(GetBackgroundColour(), 1, wxSOLID));
	dc.DrawRectangle(GetClientRect());
	if (!m_tableCreated) {
		return ;
	}

	// draw table
	dc.SetDeviceOrigin(-dx * xunit, -dy * yunit);
	DrawTable(dc, GetUpdateRegion());

	// draw rows
	if (m_showRows) {
		dc.SetDeviceOrigin(0, -dy * yunit);
		DrawHdrCells(dc, m_rows);
	}

	// draw columns
	if (m_showCols) {
		dc.SetDeviceOrigin(-dx * xunit, 0);
		DrawHdrCells(dc, m_cols);
	}

	// draw corner
	if (m_showCorner) {
		dc.SetDeviceOrigin(0, 0);
		DrawHeaderCell(dc, m_cornerCell);

		dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(GetBackgroundColour()));
		dc.SetPen(*wxThePenList->FindOrCreatePen(GetBackgroundColour(), 1, wxSOLID));
		if (!m_showRows) {
			wxRect rc(0, CalcTotalColLayersHeight(),
					CalcTotalRowLayersWidth(), CalcTotalRowsHeight());

			dc.DrawRectangle(rc);
		}

		if (!m_showCols) {
			wxRect rc(SumLayerSizes(m_rowLayerWidths), 0,
					CalcTotalColsWidth(), SumLayerSizes(m_colLayerHeights));
			dc.DrawRectangle(rc);
		}
	}
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
			RedrawViewportRect(m_currentHdrCell->m_rc);
			m_currentHdrCell = NULL;
		}
	}

	ev.Skip();
}

void wxAdvTable::HandleHdrCellMouseEvent(wxMouseEvent &ev, wxAdvHdrCell *cell)
{
	if (ev.LeftDown()) {
		m_clickedCell = cell;
	}
	else if (ev.LeftUp()) {
		if (cell == m_clickedCell && m_clickedCell != NULL) {
			if ((m_sorter != NULL && m_clickedCell->m_sortable && m_clickedCell->m_isReal) &&
				((!m_clickedCell->m_isRow && m_sortMode == Rows) ||
				(m_clickedCell->m_isRow && m_sortMode == Cols))) {
				SetSortingIndex(m_clickedCell->m_index);
			}
		}

		m_clickedCell = NULL;
	}
	else if (ev.LeftDClick()) {
	}

	if (ev.ButtonUp()) {
		m_clickedCell = NULL;
	}

	if (m_currentHdrCell != cell) {
		RedrawViewportRect(cell->m_rc);
		if (m_currentHdrCell != NULL) {
			RedrawViewportRect(m_currentHdrCell->m_rc);
		}

		m_currentHdrCell = cell;
	}
}

void wxAdvTable::HandleCellMouseEvent(wxMouseEvent &ev, size_t row, size_t col)
{

	if (ev.LeftDown()) {
		m_sel.Set(row, col);

		SetFocusedCell(row, col);
		SelectCells(ev, row, col);

		Refresh();
	}
	else if (ev.Dragging() && ev.m_leftDown) {
		SetFocusedCell(row, col);
		SelectCells(ev, row, col);

		Refresh();
	}
	else if (ev.LeftUp()) {
		if (m_sel.Equal(row, col)) {
			if (m_model->IsCellEditable(row, col)) {
				wxAdvTableCellEditor *editor = GetEditorForCell(row, col);

				if (editor != NULL && editor->OneClickActivate()) {
					EditCell(editor, row, col);
				}
			}

			SendEvent(wxEVT_GRID_CELL_LEFT_CLICK, row, col, ev);
		}
	}
	else if (ev.LeftDClick()) {
		if (m_model->IsCellEditable(row, col)) {
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

void wxAdvTable::OnKeyDown(wxKeyEvent &ev)
{
	// TODO handle alphanumber keycodes to activate editor
#if 0
	int keyCode = ev.GetKeyCode();

	int colShift = 0;
	int rowShift = 0;

	switch (keyCode) {
		case WXK_LEFT:
			colShift = -1;
			break;
		case WXK_RIGHT:
			colShift = 1;
			break;
		case WXK_UP:
			rowShift = -1;
			break;
		case WXK_DOWN:
			rowShift = 1;
			break;
		default:
			return ;
	}

	int newFocusedCol = ((int) m_focusedCol) + colShift;
	int newFocusedRow = ((int) m_focusedRow) + rowShift;

	if (newFocusedCol < 0) {
		newFocusedCol = 0;
	}
	else if (newFocusedCol >= (int) GetRealColCount()) {
		newFocusedCol = (int) GetRealColCount();
	}

	if (newFocusedRow < 0) {
		newFocusedRow = 0;
	}
	else if (newFocusedRow >= (int) GetRealRowCount()) {
		newFocusedRow = (int) GetRealRowCount();
	}

	SetFocusedCell((size_t) newFocusedRow, (size_t) newFocusedCol);

	if (ev.ShiftDown()) {
		size_t row1 = m_selected.m_row1;
		size_t col1 = m_selected.m_col1;
		size_t row2 = m_selected.m_row2;
		size_t col2 = m_selected.m_col2;

		// TODO broken, this code must be rewritten!
		if (rowShift > 0) {
			row2 = (size_t) newFocusedRow;
		}
		else if (rowShift < 0) {
			if (row2 > row1) {
				row2 = (size_t) newFocusedRow;
			}
			else {
				row2 = row1;
				row1 = (size_t) newFocusedRow;
			}
		}

		if (colShift > 0) {
			col2 = (size_t) newFocusedCol;
		}
		else if (colShift < 0) {
			if (col1 < (size_t) newFocusedCol) {
				col1 = (size_t) newFocusedCol;
			}
			else {
				col2 = (size_t) newFocusedCol;
			}
		}

		SetSelection(row1, col1, row2, col2);
	}
	else {
		SetSelection((size_t) newFocusedRow, (size_t) newFocusedCol);
	}
#endif
}

void wxAdvTable::OnKillFocus(wxFocusEvent &ev)
{
	StopEditing();
}

void wxAdvTable::Update()
{
	Refresh();
}

void wxAdvTable::CellChanged(size_t row, size_t col)
{
	Refresh();
}

void wxAdvTable::TableChanged()
{
	Refresh();
}

