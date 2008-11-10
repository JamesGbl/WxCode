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
wxAdvStringTableDataModel::wxAdvStringTableDataModel(size_t _numRows, size_t _numCols, bool _readOnly)
{
	readOnly = _readOnly;

	wxArrayString row;
	row.Add(wxEmptyString, _numCols);
	data.Add(row, _numRows);
}

wxAdvStringTableDataModel::~wxAdvStringTableDataModel()
{
}

wxString wxAdvStringTableDataModel::GetCellValue(size_t row, size_t col)
{
	wxCHECK(row < data.Count(), wxEmptyString);
	wxCHECK(col < data[row].Count(), wxEmptyString);
	return data[row][col];
}

bool wxAdvStringTableDataModel::IsCellEditable(size_t row, size_t col)
{
	return !readOnly;
}

int wxAdvStringTableDataModel::GetCellFormat(size_t row, size_t col)
{
	return wxStringFormat;
}

//
// wxAdvFilterTableDataModel
//

wxAdvFilterTableDataModel::wxAdvFilterTableDataModel(wxAdvTableDataModel *_model, size_t _numRows, size_t _numCols)
{
	model = _model;
	model->AddObserver(this);

	wxArrayString row;
	row.Add(wxEmptyString, _numCols);
	data.Add(row, _numRows);

	needUpdate = true;
	//UpdateValues();
}

wxAdvFilterTableDataModel::~wxAdvFilterTableDataModel()
{
	delete model;
}

wxString wxAdvFilterTableDataModel::GetCellValue(size_t row, size_t col)
{
	if (needUpdate) {
		UpdateValues();
	}

	wxCHECK(row < data.Count(), wxEmptyString);
	wxCHECK(col < data[row].Count(), wxEmptyString);
	return data[row][col];
}

int wxAdvFilterTableDataModel::GetCellFormat(size_t row, size_t col)
{
	return model->GetCellFormat(row, col);
}

bool wxAdvFilterTableDataModel::IsCellEditable(size_t row, size_t col)
{
	return model->IsCellEditable(row, col);
}

void wxAdvFilterTableDataModel::CellChanged(size_t row, size_t col)
{
	data[row][col] = model->GetCellValue(row, col);
	FireCellChanged(row, col);
}

void wxAdvFilterTableDataModel::TableChanged()
{
	UpdateValues();
	FireTableChanged();
}

void wxAdvFilterTableDataModel::UpdateValues()
{
	for (size_t row = 0; row < data.Count(); row++) {
		for (size_t col = 0; col < data[row].Count(); col++) {
			data[row][col] = model->GetCellValue(row, col);
		}
	}
	needUpdate = false;
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
	// TODO
	//int flags = wxCONTROL_CHECKED;

	//wxRendererNative::Get()->DrawCheckBox(wnd, dc, rc, flags);
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
		sorter = o.sorter;
		table = o.table;
		row = o.row;
		col = o.col;
		sortDirection = o.sortDirection;
	}

	SortHelper(wxAdvTable *_table, wxAdvTableSorter *_sorter, size_t _row, size_t _col, int _sortDirection)
	{
		table = _table;
		sorter = _sorter;
		row = _row;
		col = _col;
		sortDirection = _sortDirection;
	}

	virtual ~SortHelper()
	{
		int i = 0;
		i++;
	}

	wxAdvTableSorter *sorter;
	wxAdvTable *table;
	size_t row;
	size_t col;
	int sortDirection;
};

WX_DEFINE_SORTED_ARRAY(SortHelper *, wxSortHelperArray);

int SorterFunc(SortHelper *first, SortHelper *second)
{
	wxAdvTableSorter *sorter = first->sorter;
	int res = sorter->Compare(first->table, first->row, first->col, second->row, second->col);
	if (first->sortDirection == wxAdvTable::Descending) {
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

wxAdvTable::wxAdvTable(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
: wxScrolledWindow(parent, id, pos, size, wxHSCROLL | wxVSCROLL | wxFULL_REPAINT_ON_RESIZE),
cornerCell(wxEmptyString)
{
	created = false;
	model = NULL;
	defaultRenderer = new wxAdvStringCellRenderer();
	sorter = NULL;
	sortMode = Rows;
	highlightMode = HighlightNone;
	clickedCell = NULL;
	sortDirection = Ascending;

	selectMode = SelectBlock;

	showRows = true;
	showCols = true;

	// Graphics initialization
	bgBrush = *wxWHITE_BRUSH;
	selectedBgBrush = *wxTheBrushList->FindOrCreateBrush(wxColour(200, 200, 250));
	gridPen = *wxThePenList->FindOrCreatePen(wxColour(200, 200, 200), 1, wxSOLID);
	focusedPen = *wxThePenList->FindOrCreatePen(wxColour(80, 80, 250), 1, wxSOLID);
	focusedBgBrush = *wxTheBrushList->FindOrCreateBrush(wxColour(180, 180, 250));
	highlightedBgBrush = *wxTheBrushList->FindOrCreateBrush(wxColour(225, 225, 250));
	ClearSelection();

	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	SetScrollRate(10, 10);
}

wxAdvTable::~wxAdvTable()
{
	wxAdvTableCellRendererMap::iterator it;

	for (it = renderers.begin(); it != renderers.end(); it++) {
		delete it->second;
	}

	SAFE_DELETE(model);
	SAFE_DELETE(defaultRenderer);
}

void wxAdvTable::Create(wxAdvHdrCell *_rows, size_t numRows, wxAdvHdrCell *_cols, size_t numCols, wxString _corner, wxAdvTableDataModel *_model)
{
	if (created) {
		DestroyTable();
	}

	cornerCell.Label(_corner);

	AddHdrCells(rows, _rows, numRows, true);
	AddHdrCells(cols, _cols, numCols, false);

	// recalculate entire geometry
	CalcTableGeometry();

	// assign new model for table data
	SAFE_REPLACE(model, _model);
	model->AddObserver(this);

	// reset sorting and selection
	sortingIndex = (size_t) -1;
	ClearSelection();

	created = true;
}

void wxAdvTable::Create(size_t numRows, wxAdvHdrCell *_cols, size_t numCols, wxAdvTableDataModel *_model)
{
	Create(NULL, numRows, _cols, numCols, wxEmptyString, _model);
}

void wxAdvTable::Create(size_t numRows, size_t numCols, wxAdvTableDataModel *_model)
{
	Create(NULL, numRows, NULL, numCols, wxEmptyString, _model);
}

void wxAdvTable::Create(wxAdvHdrCell *_rows, size_t numRows, size_t numCols, wxAdvTableDataModel *_model)
{
	Create(_rows, numRows, NULL, numCols, wxEmptyString, _model);
}

void wxAdvTable::DestroyTable()
{
	rows.Clear();
	cols.Clear();
	cornerCell.Label(wxEmptyString);
	cornerCell.rc = wxRect(0, 0, 0, 0);

	realRows.Clear();
	realCols.Clear();

	rowLayerWidths.Clear();
	colLayerHeights.Clear();

	cellConcats.Clear();

	SAFE_DELETE(model);

	ClearSelection();
}

void wxAdvTable::AddRows(wxAdvHdrCell *_rows, size_t numRows)
{
	wxCHECK_RET(created, wxT("wxAdvTable::AddRows: Table is not created"));

	AddHdrCells(rows, _rows, numRows, true);
	CalcTableGeometry();
}

void wxAdvTable::AddCols(wxAdvHdrCell *_cols, size_t numCols)
{
	wxCHECK_RET(created, wxT("wxAdvTable::AddCols: Table is not created"));

	AddHdrCells(cols, _cols, numCols, false);
	CalcTableGeometry();
}

//
// Table geometry routines.
//
void wxAdvTable::CalcTableGeometry()
{
	size_t numRowLayers = GetLayerCount(rows);
	size_t numColLayers = GetLayerCount(cols);

	// Layer sizes
	rowLayerWidths.Clear();
	rowLayerWidths.Alloc(numRowLayers);
	rowLayerWidths.Add(0, numRowLayers);

	colLayerHeights.Clear();
	colLayerHeights.Alloc(numColLayers);
	colLayerHeights.Add(0, numColLayers);

	// real rows/columns
	realRows.Clear();
	realCols.Clear();

	FindRealSubCells(rows, realRows);
	FindRealSubCells(cols, realCols);

	// update header geometry
	wxClientDC dc(this);
	CalcHeaderGeometry(dc);
}

void wxAdvTable::CalcHeaderGeometry(wxDC &dc)
{
	// setup sizes for all rows and columns
	FOREACH_HDRCELL(n, rows) {
		FitForSubCells(dc, rows[n], true);
	}
	FOREACH_HDRCELL(n, cols) {
		FitForSubCells(dc, cols[n], false);
	}

	// calculate layer sizes from row/column maximum heights/widths
	FOREACH_HDRCELL(n, rows) {
		CalcLayerSizes(rows[n], rowLayerWidths, true, 0);
	}
	FOREACH_HDRCELL(n, cols) {
		CalcLayerSizes(cols[n], colLayerHeights, false, 0);
	}

	// set row/columns heights/widths from layer sizes
	FOREACH_HDRCELL(n, rows) {
		UpdateHeaderSizes(rows[n], rowLayerWidths, true, 0);
	}
	FOREACH_HDRCELL(n, cols) {
		UpdateHeaderSizes(cols[n], colLayerHeights, false, 0);
	}

	// setup positions for all rows and columns
	wxCoord totalColHeight = SumLayerSizes(colLayerHeights);
	wxCoord totalRowWidth = SumLayerSizes(rowLayerWidths);

	wxCoord x0 = 0;
	wxCoord y0 = 0;

	x0 = 0;
	y0 = (showCols) ? totalColHeight : 0;
	FOREACH_HDRCELL(n, rows) {
		UpdateHeaderPositions(rows[n], x0, y0, rowLayerWidths, 0, true);
		y0 += rows[n].rc.height;
	}

	x0 = (showRows) ? totalRowWidth : 0;
	y0 = 0;
	FOREACH_HDRCELL(n, cols) {
		UpdateHeaderPositions(cols[n], x0, y0, colLayerHeights, 0, false);
		x0 += cols[n].rc.width;
	}

	cornerCell.rc = wxRect(0, 0, totalRowWidth, totalColHeight); // XXX

	wxCoord virtWidth = GetTotalColsWidth();
	wxCoord virtHeight = GetTotalRowsHeight();

	if (showRows) {
		virtWidth += totalRowWidth;
	}
	if (showCols) {
		virtHeight += totalColHeight;
	}

	SetVirtualSize(virtWidth, virtHeight);
}

wxSize wxAdvTable::FitForSubCells(wxDC &dc, wxAdvHdrCell &hdrCell, bool isRows)
{
	wxSize size;

	if (!hdrCell.IsComposite()) {
		size = hdrCell.CalcExtent(dc);
		hdrCell.rc.SetPosition(wxPoint(0, 0));
		hdrCell.rc.SetSize(size);
		return size;
	}

	FOREACH_HDRCELL(n, hdrCell.subCells) {
		wxAdvHdrCell &subRowCol = hdrCell.subCells[n];

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

		subRowCol.rc.SetPosition(wxPoint(0, 0));
		subRowCol.rc.SetSize(subSize);
	}

	hdrCell.rc.SetPosition(wxPoint(0, 0));
	hdrCell.rc.SetSize(size);
	return size;
}

void wxAdvTable::CalcLayerSizes(wxAdvHdrCell &hdrCell, wxCoordArray &layerSizes, bool isRows, size_t nLayer)
{
	FOREACH_HDRCELL(n, hdrCell.subCells) {
		wxAdvHdrCell &subRowCol = hdrCell.subCells[n];
		CalcLayerSizes(subRowCol, layerSizes, isRows, nLayer + 1);
	}

	wxCoord size;
	if (isRows) {
		size = hdrCell.rc.width;
	}
	else {
		size = hdrCell.rc.height;
	}

	if (nLayer == LastLayer(hdrCell, layerSizes)) {
		// update layer size if row/column occupies only one layer.
		layerSizes[nLayer] = MAX(layerSizes[nLayer], size);
	}
}

void wxAdvTable::UpdateHeaderPositions(wxAdvHdrCell &hdrCell, wxCoord x0, wxCoord y0, wxCoordArray &layerSizes, size_t nLayer, bool isRows)
{
	hdrCell.rc.x = x0;
	hdrCell.rc.y = y0;

	FOREACH_HDRCELL(n, hdrCell.subCells) {
		wxAdvHdrCell &subRowCol = hdrCell.subCells[n];

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
			y0 += subRowCol.rc.height;
		}
		else {
			x0 += subRowCol.rc.width;
		}
	}
}

void wxAdvTable::UpdateHeaderSizes(wxAdvHdrCell &hdrCell, wxCoordArray &layerSizes, bool isRows, size_t nLayer)
{
	FOREACH_HDRCELL(n, hdrCell.subCells) {
		wxAdvHdrCell &subRowCol = hdrCell.subCells[n];
		UpdateHeaderSizes(subRowCol, layerSizes, isRows, nLayer + 1);
	}

	wxCoord size = SumLayerSizes(layerSizes, nLayer, LastLayer(hdrCell, layerSizes));
	if (isRows) {
		hdrCell.rc.width = size;
	}
	else {
		hdrCell.rc.height = size;
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
		hdrCell.isReal = true;
		hdrCell.index = flattenRowCols.Count();
		flattenRowCols.Add(&hdrCell);
	}
	else {
		FOREACH_HDRCELL(n, hdrCell.subCells) {
			FindRealSubCell(hdrCell.subCells[n], flattenRowCols);
		}
	}
}

wxCoord wxAdvTable::SumDimensions(wxAdvHdrCellPtrArray &hdrCells, bool widths)
{
	wxCoord sum = 0;
	if (widths) {
		FOREACH_HDRCELL(n, hdrCells) {
			sum += hdrCells[n]->rc.width;
		}
	}
	else {
		FOREACH_HDRCELL(n, hdrCells) {
			sum += hdrCells[n]->rc.height;
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
// Sorting, selection, highligh, etc routines.
//
void wxAdvTable::SetSorter(wxAdvTableSorter *_sorter)
{
	SAFE_REPLACE(sorter, _sorter);
	UpdateSortOrder();
}

void wxAdvTable::SetSortMode(SortMode _sortMode)
{
	sortMode = _sortMode;
	UpdateSortOrder();
}

void wxAdvTable::UpdateSortOrder()
{
	if (sorter == NULL)
		return ;

	if (sortMode == Rows) {
		if (sortingIndex >= realCols.Count())
			return ;

		size_t rowCount = realRows.Count();

		wxSortHelperArray helpers(SorterFunc);
		helpers.Alloc(rowCount);

		for (size_t row = 0; row < rowCount; row++) {
			helpers.Add(new SortHelper(this, sorter, row, sortingIndex, sortDirection));
		}

		rowsOrder.Empty();
		rowsOrder.Alloc(rowCount);

		for (size_t row = 0; row < rowCount; row++) {
			rowsOrder.Add(helpers[row]->row);
		}
	}
	else if (sortMode == Cols) {
		if (sortingIndex >= realRows.Count())
			return ;

		size_t colCount = realCols.Count();

		wxSortHelperArray helpers(SorterFunc);
		helpers.Alloc(colCount);

		for (size_t col = 0; col < colCount; col++) {
			helpers.Add(new SortHelper(this, sorter, sortingIndex, col, sortDirection));
		}

		colsOrder.Empty();
		colsOrder.Alloc(colCount);

		for (size_t col = 0; col < colCount; col++) {
			colsOrder.Add(helpers[col]->col);
		}
	}
	else {
		return ;
	}

	Refresh();
}

void wxAdvTable::SetSortingIndex(size_t _sortingIndex)
{
	if (sortingIndex == _sortingIndex) {
		if (sortDirection == Ascending) {
			sortDirection = Descending;
		}
		else {
			sortDirection = Ascending;
		}
	}
	else {
		sortDirection = Ascending;
	}

	sortingIndex = _sortingIndex;
	UpdateSortOrder();
}

void wxAdvTable::SetSelectMode(SelectMode _selectMode)
{
	selectMode = _selectMode;
	Refresh();
}

int wxAdvTable::GetSelectMode()
{
	return selectMode;
}

void wxAdvTable::SetHighlighMode(HighlightMode _highlightMode)
{
	highlightMode = _highlightMode;
	Refresh();
}

void wxAdvTable::ClearSelection()
{
	focusedRow = -1;
	focusedCol = -1;

	selRow = -1;
	selCol = -1;

	Refresh();
}

void wxAdvTable::SetShowHeaders(bool _showRows, bool _showCols)
{
	showRows = _showRows;
	showCols = _showCols;

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

	FOREACH_HDRCELL(n, hdrCell.subCells) {
		DrawHdrCell(dc, hdrCell.subCells[n]);
	}
}

void wxAdvTable::DrawHeaderCell(wxDC &dc, wxAdvHdrCell &hdrCell)
{
	wxDCClipper clip(dc, hdrCell.rc);
	wxRect rc = hdrCell.rc;
	//rc.Inflate(1, 1);

	// TODO make own drawing routine - don't use native renderer
	wxHeaderSortIconType sortIcon = wxHDR_SORT_ICON_NONE;
	if (sortingIndex == hdrCell.index) {
		sortIcon = (sortDirection == Ascending) ? wxHDR_SORT_ICON_DOWN : wxHDR_SORT_ICON_UP;
	}

	wxRendererNative::Get().DrawHeaderButton(this, dc, rc, 0, sortIcon);
	// TODO draw vertical text
	//dc.DrawText(hdrCell.label, hdrCell.rc.x, hdrCell.rc.y);
	dc.SetFont(*wxNORMAL_FONT);
	dc.DrawLabel(hdrCell.label, hdrCell.rc, hdrCell.alignHorizontal | hdrCell.alignVertical);
}

void wxAdvTable::DrawTable(wxDC &dc)
{
	FOREACH_HDRCELL(row, realRows) {
		FOREACH_HDRCELL(col, realCols) {
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
		dc.SetBrush(selectedBgBrush);
	}
	else {
		dc.SetBrush(bgBrush);
	}

	if (highlighed) {
		dc.SetBrush(highlightedBgBrush);
	}

	if (focused) {
		dc.SetBrush(focusedBgBrush);
		dc.SetPen(focusedPen);
	}
	else {
		dc.SetPen(gridPen);
	}

	dc.DrawRectangle(rc);

	GetRealCellIndex(row, col, row, col);

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

	FOREACH_HDRCELL(nRow, realRows) {
		wxAdvHdrCell *rowCell = realRows[nRow];

		if (pt.y > rowCell->rc.y && pt.y < (rowCell->rc.y + rowCell->rc.height)) {

			FOREACH_HDRCELL(nCol, realCols) {
				wxAdvHdrCell *colCell = realCols[nCol];

				if (pt.x > colCell->rc.x && pt.x < (colCell->rc.x + colCell->rc.width)) {
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
	FOREACH_HDRCELL(nRow, rows) {
		if ((cell = GetHdrCellAtRecursive(&rows[nRow], pt)) != NULL) {
			return cell;
		}
	}

	FOREACH_HDRCELL(nCol, cols) {
		if ((cell = GetHdrCellAtRecursive(&cols[nCol], pt)) != NULL) {
			return cell;
		}
	}
	return NULL;
}

wxAdvHdrCell *wxAdvTable::GetHdrCellAtRecursive(wxAdvHdrCell *cell, wxPoint &pt)
{
	if (cell->rc.Contains(pt)) {
		return cell;
	}
	else if (cell->IsComposite()) {
		FOREACH_HDRCELL(n, cell->subCells) {
			wxAdvHdrCell *subCell = GetHdrCellAtRecursive(&cell->subCells[n], pt);
			if (subCell != NULL) {
				return subCell;
			}
		}
	}
	return NULL;
}

wxString wxAdvTable::GetCellValue(size_t row, size_t col)
{
	return model->GetCellValue(row, col);
}

wxAdvTableCellRenderer *wxAdvTable::GetRendererForCell(size_t nRow, size_t nCol)
{
	int format = model->GetCellFormat(nRow, nCol);

	if (renderers.find(format) == renderers.end()) {
		return defaultRenderer;
	}
	return renderers[format];
}

void wxAdvTable::SetRendererForFormat(int format, wxAdvTableCellRenderer *renderer)
{
	if (renderers.find(format) != renderers.end()) {
		delete renderers[format];
	}
	renderers[format] = renderer;
	Refresh();
}

void wxAdvTable::SetDefaultRenderer(wxAdvTableCellRenderer *renderer)
{
	SAFE_REPLACE(defaultRenderer, renderer);
	Refresh();
}

void wxAdvTable::GetRealCellIndex(size_t row, size_t col, size_t &realRow, size_t &realCol)
{
	if (sorter == NULL || sortingIndex >= realCols.Count()) {
		realRow = row;
		realCol = col;
		return ;
	}

	if (sortMode == Rows) {
		realRow = rowsOrder[row];
		realCol = col;
	}
	else if (sortMode == Cols) {
		realRow = row;
		realCol = colsOrder[col];
	}
}

wxRect wxAdvTable::GetCellRect(size_t nRow, size_t nCol)
{
	wxRect rc;

	if (nRow < realRows.Count() && nCol < realCols.Count()) {
		rc.x = realCols[nCol]->rc.x;
		rc.y = realRows[nRow]->rc.y;
		rc.width = realCols[nCol]->rc.width;
		rc.height = realRows[nRow]->rc.height;
	}
	return rc;
}

//
// Selection functions
//
void wxAdvTable::SetSelection(wxAdvRange &range)
{
	selected = range;
	Refresh();
}

void wxAdvTable::SetSelection(size_t row, size_t col)
{
	SetSelection(row, col, row, col);
}

void wxAdvTable::SetSelection(size_t row1, size_t col1, size_t row2, size_t col2)
{
	selected.Set(row1, col1, row2, col2);
	Refresh();
}

void wxAdvTable::SetFocusedCell(size_t row, size_t col)
{
	if (row >= GetRealRowCount() || col >= GetRealColCount())
		return ;

	focusedRow = row;
	focusedCol = col;
	// TODO scroll to new focused cell

	Refresh();
}

bool wxAdvTable::IsCellHighlighted(size_t row, size_t col)
{
	switch (highlightMode) {
		case HighlightNone:
			return false;
		case HighlightRows:
			return focusedRow == row;
		case HighlightCols:
			return focusedCol == col;
		case HighlightBoth:
			return (focusedRow == row) || (focusedCol == col);
		default:
			return false;
	}
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
	if (!created) {
		return ;
	}

	// draw table
	dc.SetDeviceOrigin(-dx * xunit, -dy * yunit);
	DrawTable(dc);

	// draw rows
	if (showRows) {
		dc.SetDeviceOrigin(0, -dy * yunit);
		DrawHdrCells(dc, rows);
	}

	// draw columns
	if (showCols) {
		dc.SetDeviceOrigin(-dx * xunit, 0);
		DrawHdrCells(dc, cols);
	}

	// draw corner
	if (showRows && showCols) {
		dc.SetDeviceOrigin(0, 0);
		DrawHeaderCell(dc, cornerCell);
	}
}

void wxAdvTable::OnLeftDown(wxMouseEvent &ev)
{
	size_t row, col;

	wxPoint pt = ev.GetPosition();

	wxAdvHdrCell *cell;

	if ((cell = GetHdrCellAt(pt)) != NULL) {
		clickedCell = cell;
	}
	else if (GetCellAt(pt, row, col)) {
		selRow = row;
		selCol = col;

		switch (selectMode) {
		case SelectRows:
			selected.Set(row, 0, row, GetRealColCount());
			break;
		case SelectCols:
			selected.Set(0, col, GetRealRowCount(), col);
			break;
		default:
			selected.Set(row, col);
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

	if ((cell = GetHdrCellAt(pt)) == clickedCell && clickedCell != NULL) {
		if (sorter != NULL && clickedCell->sortable && clickedCell->isReal) {
			if ((!clickedCell->isRow && sortMode == Rows)
				|| (clickedCell->isRow && sortMode == Cols)) {
				SetSortingIndex(clickedCell->index);
			}
		}
	}
	else if (GetCellAt(pt, row, col)) {
		if (row == selRow && col == selCol) {
			if (model->IsCellEditable(row, col)) {
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
			focusedRow = row;
			focusedCol = col;

			switch (selectMode) {
			case SelectCell:
				selected.Set(row, col);
				break;
			case SelectRows:
				selected.Set(selRow, 0, row, GetRealColCount());
				break;
			case SelectCols:
				selected.Set(0, selCol, GetRealRowCount(), col);
				break;
			case SelectBlock:
				selected.Set(selRow, selCol, row, col);
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

	int newFocusedCol = ((int) focusedCol) + colShift;
	int newFocusedRow = ((int) focusedRow) + rowShift;

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
		size_t row1 = selected.row1;
		size_t col1 = selected.col1;
		size_t row2 = selected.row2;
		size_t col2 = selected.col2;

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

