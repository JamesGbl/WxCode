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
// wxAdvTableDataModelObserver::
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
// wxAdvTableCellRenderer
//
wxAdvTableCellRenderer::wxAdvTableCellRenderer()
{
}

wxAdvTableCellRenderer::~wxAdvTableCellRenderer()
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
// wxAdvBoolTableCellRenderer
//
wxAdvBoolTableCellRenderer::wxAdvBoolTableCellRenderer()
{
}

wxAdvBoolTableCellRenderer::~wxAdvBoolTableCellRenderer()
{
}

void wxAdvBoolTableCellRenderer::Draw(wxDC &dc, wxRect rc, wxString value, bool selected, bool focused)
{
	// TODO not implemented
}

wxAdvStringCellRenderer::wxAdvStringCellRenderer()
{
}

wxAdvStringCellRenderer::~wxAdvStringCellRenderer()
{
}

void wxAdvStringCellRenderer::Draw(wxDC &dc, wxRect rc, wxString value, bool selected, bool focused)
{
	// TODO implement cell vertical/horizontal alignments, colored text, and fonts
	dc.SetFont(*wxNORMAL_FONT);
	dc.DrawText(value, rc.x, rc.y);
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
	EVT_LEFT_DOWN(wxAdvTable::OnLeftDown)
	EVT_LEFT_UP(wxAdvTable::OnLeftUp)
	EVT_MOTION(wxAdvTable::OnMotion)
	EVT_KEY_DOWN(wxAdvTable::OnKeyDown)
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
	m_sortDirection = Ascending;

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

	wxCoord virtWidth = GetTotalColsWidth();
	wxCoord virtHeight = GetTotalRowsHeight();

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
	m_selectMode = _selectMode;
	Refresh();
}

int wxAdvTable::GetSelectMode()
{
	return m_selectMode;
}

void wxAdvTable::SetHighlightMode(HighlightMode _highlightMode)
{
	m_highlightMode = _highlightMode;
	Refresh();
}

void wxAdvTable::ClearSelection()
{
	m_focusedRow = -1;
	m_focusedCol = -1;

	m_selRow = -1;
	m_selCol = -1;

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
	//rc.Inflate(1, 1);

	// TODO make own drawing routine - don't use native renderer
	wxHeaderSortIconType sortIcon = wxHDR_SORT_ICON_NONE;
	if (m_sortingIndex == hdrCell.m_index) {
		sortIcon = (m_sortDirection == Ascending) ? wxHDR_SORT_ICON_DOWN : wxHDR_SORT_ICON_UP;
	}

	wxRendererNative::Get().DrawHeaderButton(this, dc, rc, 0, sortIcon);
	// TODO draw vertical text
	//dc.DrawText(hdrCell.label, hdrCell.rc.x, hdrCell.rc.y);
	dc.SetFont(*wxNORMAL_FONT);
	dc.DrawLabel(hdrCell.m_label, hdrCell.m_rc, hdrCell.m_alignHorizontal | hdrCell.m_alignVertical);
}

void wxAdvTable::DrawTable(wxDC &dc)
{
	FOREACH_HDRCELL(row, m_realRows) {
		FOREACH_HDRCELL(col, m_realCols) {
			DrawTableCell(dc, row, col);
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
	GetRendererForCell(row, col)->Draw(dc, rc, value, selected, focused);
}

wxPoint wxAdvTable::ToViewportPosition(wxPoint &pt)
{
	int dx, dy;
	int xunit, yunit;

	GetViewStart(&dx, &dy);
	GetScrollPixelsPerUnit(&xunit, &yunit);

	return wxPoint(pt.x + dx * xunit, pt.y + dy * yunit);
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
	pt = ToViewportPosition(pt);

	wxAdvHdrCell *cell;
	if (m_showRows) {
		FOREACH_HDRCELL(nRow, m_rows) {
			if ((cell = GetHdrCellAtRecursive(&m_rows[nRow], pt)) != NULL) {
				return cell;
			}
		}
	}

	if (m_showCols) {
		FOREACH_HDRCELL(nCol, m_cols) {
			if ((cell = GetHdrCellAtRecursive(&m_cols[nCol], pt)) != NULL) {
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

void wxAdvTable::GetUnsortedCellIndex(size_t row, size_t col, size_t &realRow, size_t &realCol)
{
	if (m_sorter == NULL || m_sortingIndex >= m_realCols.Count()) {
		realRow = row;
		realCol = col;
		return ;
	}

	if (m_sortMode == Rows) {
		realRow = m_rowsOrder[row];
		realCol = col;
	}
	else if (m_sortMode == Cols) {
		realRow = row;
		realCol = m_colsOrder[col];
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

void wxAdvTable::SetFocusedCell(size_t row, size_t col)
{
	if (row >= GetRealRowCount() || col >= GetRealColCount())
		return ;

	m_focusedRow = row;
	m_focusedCol = col;
	// TODO scroll to new focused cell

	Refresh();
}

bool wxAdvTable::IsCellHighlighted(size_t row, size_t col)
{
	switch (m_highlightMode) {
		case HighlightNone:
			return false;
		case HighlightRows:
			return m_focusedRow == row;
		case HighlightCols:
			return m_focusedCol == col;
		case HighlightBoth:
			return (m_focusedRow == row) || (m_focusedCol == col);
		default:
			return false;
	}
}

void wxAdvTable::SendEvent(const wxEventType, size_t row, size_t col)
{
	// TODO
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

	dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(GetBackgroundColour()));
	dc.SetPen(*wxThePenList->FindOrCreatePen(GetBackgroundColour(), 1, wxSOLID));
	dc.DrawRectangle(GetClientRect());
	if (!m_tableCreated) {
		return ;
	}

	// draw table
	dc.SetDeviceOrigin(-dx * xunit, -dy * yunit);
	DrawTable(dc);

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
	}
}

void wxAdvTable::OnLeftDown(wxMouseEvent &ev)
{
	size_t row, col;

	wxPoint pt = ev.GetPosition();

	wxAdvHdrCell *cell;

	if ((cell = GetHdrCellAt(pt)) != NULL) {
		m_clickedCell = cell;
	}
	else if (GetCellAt(pt, row, col)) {
		m_selRow = row;
		m_selCol = col;

		switch (m_selectMode) {
		case SelectRows:
			m_selected.Set(row, 0, row, GetRealColCount());
			break;
		case SelectCols:
			m_selected.Set(0, col, GetRealRowCount(), col);
			break;
		default:
			m_selected.Set(row, col);
		}

		SetFocusedCell(row, col);

		Refresh();
	}
}

void wxAdvTable::OnLeftUp(wxMouseEvent &ev)
{
	size_t row, col;

	wxPoint pt = ev.GetPosition();

	wxAdvHdrCell *cell;

	if ((cell = GetHdrCellAt(pt)) == m_clickedCell && m_clickedCell != NULL) {
		if (m_sorter != NULL && m_clickedCell->m_sortable && m_clickedCell->m_isReal) {
			if ((!m_clickedCell->m_isRow && m_sortMode == Rows)
				|| (m_clickedCell->m_isRow && m_sortMode == Cols)) {
				SetSortingIndex(m_clickedCell->m_index);
			}
		}
	}
	else if (GetCellAt(pt, row, col)) {
		if (row == m_selRow && col == m_selCol) {
			if (m_model->IsCellEditable(row, col)) {
				//wxAdvTableCellEditor *editor = GetEditorForCell(row, col);

				// TODO
			}
		}
	}
}

void wxAdvTable::OnMotion(wxMouseEvent &ev)
{
	if (ev.Dragging() && ev.m_leftDown) {
		size_t row, col;

		wxPoint pt = ev.GetPosition();

		if (GetCellAt(pt, row, col)) {
			m_focusedRow = row;
			m_focusedCol = col;

			switch (m_selectMode) {
			case SelectCell:
				m_selected.Set(row, col);
				break;
			case SelectRows:
				m_selected.Set(m_selRow, 0, row, GetRealColCount());
				break;
			case SelectCols:
				m_selected.Set(0, m_selCol, GetRealRowCount(), col);
				break;
			case SelectBlock:
				m_selected.Set(m_selRow, m_selCol, row, col);
				break;
			}

			Refresh();
		}
	}
}

void wxAdvTable::OnKeyDown(wxKeyEvent &ev)
{
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

