/////////////////////////////////////////////////////////////////////////////
// Name:        wxAdvTable.h
// Purpose:     Definitions of wxAdvTable classes
// Author:      Moskvichev Andrey V.
// Created:     2008/10/09
// RCS-ID:      $Id: wxAdvTable.h,v 1.3 2008/10/09 16:42:58 frm Exp $
// Copyright:   (c) 2008 Moskvichev A.V.
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WXADVTABLE_H_
#define _WX_WXADVTABLE_H_

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/dynarray.h>
#include "observable.h"
#include "cutils.h"

#define FOREACH_HDRCELL(n, hdrCells) for (size_t n = 0; n < hdrCells.GetCount(); n++)

//
// Table cell formats
//
enum {
	wxStringFormat = 1,
	wxIntFormat,
	wxDoubleFormat,
	wxBoolFormat,
};

// Forward declarations
class wxAdvTable;
class wxAdvHdrCell;
class wxAdvRange;
class wxAdvTableCellRenderer;

WX_DECLARE_OBJARRAY(wxArrayString, wxArrayArrayString);
WX_DECLARE_OBJARRAY(wxAdvHdrCell, wxAdvHdrCellArray);
WX_DECLARE_OBJARRAY(wxAdvHdrCell *, wxAdvHdrCellPtrArray);
WX_DECLARE_OBJARRAY(wxAdvRange, wxAdvRangeArray);
WX_DECLARE_OBJARRAY(wxCoord, wxCoordArray);
WX_DECLARE_OBJARRAY(size_t, wxIndexArray);
WX_DECLARE_HASH_MAP(int, wxAdvTableCellRenderer *, wxIntegerHash, wxIntegerEqual, wxAdvTableCellRendererMap);

/**
 * Base class for receiving wxAdvTableDataModel events.
 */
class wxAdvTableDataModelObserver
{
public:
	wxAdvTableDataModelObserver();
	virtual ~wxAdvTableDataModelObserver();

	/**
	 * Called when single cell value has been changed.
	 * @param row row index
	 * @param col column index
	 */
	virtual void CellChanged(size_t row, size_t col) = 0;

	/**
	 * Called when all table data has been changed.
	 */
	virtual void TableChanged() = 0;
};

/**
 * Model for wxAdvTable data, such as cell values, formats.
 */
class wxAdvTableDataModel : public Observable<wxAdvTableDataModelObserver>
{
public:
	wxAdvTableDataModel();
	virtual ~wxAdvTableDataModel();

	virtual wxString GetCellValue(size_t row, size_t col) = 0;

	virtual int GetCellFormat(size_t row, size_t col) = 0;

	virtual bool IsCellEditable(size_t row, size_t col) = 0;

protected:
	FIRE_WITH_VALUE2(CellChanged, size_t, row, size_t, col);
	FIRE_VOID(TableChanged);
};

/**
 * Simpliest table model for wxAdvTable.
 * Stores data as strings, all cells have string format.
 */
class wxAdvStringTableDataModel : public wxAdvTableDataModel
{
public:
	wxAdvStringTableDataModel(size_t _numRows, size_t _numCols, bool _readOnly);
	virtual ~wxAdvStringTableDataModel();

	virtual wxString GetCellValue(size_t row, size_t col);

	virtual int GetCellFormat(size_t row, size_t col);

	virtual bool IsCellEditable(size_t row, size_t col);

private:
	bool readOnly;

	wxArrayArrayString data;
};

/**
 * Stores data in double array of strings, and updates it when underlaying model
 * data has been changed.
 */
class wxAdvFilterTableDataModel : public wxAdvTableDataModel, public wxAdvTableDataModelObserver
{
public:
	/**
	 * Constructs new object.
	 * @param _model underlaying model, wxAdvFilterTableDataModel takes ownership for _model
	 * @param _numRows number of rows
	 * @param _numCol number of columns
	 */
	wxAdvFilterTableDataModel(wxAdvTableDataModel *_model, size_t _numRows, size_t _numCols);
	virtual ~wxAdvFilterTableDataModel();

	virtual wxString GetCellValue(size_t row, size_t col);

	virtual int GetCellFormat(size_t row, size_t col);

	virtual bool IsCellEditable(size_t row, size_t col);

	//
	// wxAdvTableDataModelObserver
	//
	virtual void CellChanged(size_t row, size_t col);

	virtual void TableChanged();

private:
	void UpdateValues();

	wxAdvTableDataModel *model;

	wxArrayArrayString data;
	bool needUpdate;
};

//
// Renderers
//

/**
 * Draws cell content.
 */
class wxAdvTableCellRenderer
{
public:
	wxAdvTableCellRenderer();
	virtual ~wxAdvTableCellRenderer();

	virtual void Draw(wxDC &dc, wxRect rc, wxString value, bool selected, bool focused) = 0;
};

/**
 * Draws cell content as text.
 */
class wxAdvStringCellRenderer : public wxAdvTableCellRenderer
{
public:
	wxAdvStringCellRenderer();
	virtual ~wxAdvStringCellRenderer();

	virtual void Draw(wxDC &dc, wxRect rc, wxString value, bool selected, bool focused);
};

/**
 * Renderer to draw boolean cell values (draws checkmarks).
 */
class wxAdvBoolTableCellRenderer : public wxAdvTableCellRenderer
{
public:
	wxAdvBoolTableCellRenderer();
	virtual ~wxAdvBoolTableCellRenderer();

	virtual void Draw(wxDC &dc, wxRect rc, wxString value, bool selected, bool focused);
};


//
// Editors
//

/**
 */
class wxAdvTableCellEditor
{
public:
	wxAdvTableCellEditor();
	virtual ~wxAdvTableCellEditor();

	/**
	 * Return true, if editor activated with one click (such as boolean cells editor).
	 */
	virtual bool OneClickActivate();
};


//
// Sorters
//

/**
 * Performs table data sorting
 */
class wxAdvTableSorter
{
public:
	wxAdvTableSorter();
	virtual ~wxAdvTableSorter();

	virtual int Compare(wxAdvTable *table, size_t row1, size_t col1, size_t row2, size_t col2) = 0;
};

/**
 * Sorts data by
 */
class wxAdvTableStringSorter : public wxAdvTableSorter
{
public:
	wxAdvTableStringSorter();
	virtual ~wxAdvTableStringSorter();

	virtual int Compare(wxAdvTable *table, size_t row1, size_t col1, size_t row2, size_t col2);

private:
	int Compare(wxString value1, wxString value2);
};


const int undefinedSize = 0;

/**
 * Row/column description.
 *
 */
class wxAdvHdrCell
{
	friend class wxAdvTable;
public:

	wxAdvHdrCell(const wxAdvHdrCell &o)
	{
		// copy attributes
		subCells = o.subCells;
		label = o.label;
		size  = o.size;
		alignVertical = o.alignVertical;
		alignHorizontal = o.alignHorizontal;
		spacing = o.spacing;
		verticalText = o.verticalText;
		sortable = o.sortable;

		// copy internals
		rc = o.rc;
		index = o.index;
		isRow = o.isRow;
		isReal = o.isReal;
	}

	wxAdvHdrCell(const wxChar *_label = wxT(""))
	{
		label = _label;
		size = undefinedSize;
		alignVertical = wxALIGN_TOP;
		alignHorizontal = wxALIGN_CENTER;
		spacing = 5;
		verticalText = false;
		sortable = false;
		isReal = false;
	}

	wxAdvHdrCell(wxString &_label)
	{
		label = _label;
		size = undefinedSize;
		alignVertical = wxALIGN_TOP;
		alignHorizontal = wxALIGN_CENTER;
		spacing = 5;
		verticalText = false;
		sortable = false;
		isReal = false;
	}

	virtual ~wxAdvHdrCell()
	{
	}

	/**
	 * Adds sub row/column.
	 */
	wxAdvHdrCell &Sub(wxAdvHdrCell subRowCol)
	{
		subCells.Add(subRowCol);
		return *this;
	}

	wxAdvHdrCell &Sub(const wxChar *_label)
	{
		subCells.Add(wxAdvHdrCell(_label));
		return *this;
	}

	bool IsComposite()
	{
		return subCells.Count() != 0;
	}

	/**
	 * Sets vertical text alignment for header cell (not table values).
	 * @param _alignVertical vertical alignment
	 */
	wxAdvHdrCell &AlignVertical(int _alignVertical)
	{
		alignVertical = _alignVertical;
		return *this;
	}

	int AlignVertical()
	{
		return alignVertical;
	}

	/**
	 * Sets horizontal text alignment for header cell (not table values).
	 * @param _alignHorizontal horizontal alignment
	 */
	wxAdvHdrCell &AlignHorizontal(int _alignHorizontal)
	{
		alignHorizontal = _alignHorizontal;
		return *this;
	}

	int AlignHorizontal()
	{
		return alignHorizontal;
	}

	wxAdvHdrCell &VerticalText()
	{
		verticalText = true;
		return *this;
	}

	wxAdvHdrCell &Sortable()
	{
		sortable = true;
		return *this;
	}

	wxAdvHdrCell &Spacing(wxCoord _spacing)
	{
		spacing = _spacing;
		return *this;
	}

	wxSize CalcExtent(wxDC &dc)
	{
		// TODO add support for multiline text
		wxSize extent = dc.GetTextExtent(label);

		extent.IncBy(2 * spacing);

		if (verticalText) {
			// rotate by 90 degrees
			wxCoord tmp = extent.x;
			extent.x = extent.y;
			extent.y = tmp;
		}

		if (size != undefinedSize) {
			if (isRow) {
				extent.SetHeight(size);
			}
			else {
				extent.SetWidth(size);
			}
		}
		return extent;
	}

	/**
	 * Returns maximal sub layers count.
	 * 1 - for not composite header cell.
	 */
	size_t Layers()
	{
		size_t layers = 1;
		size_t maxSubLayers = IsComposite() ? 1 : 0;

		FOREACH_HDRCELL(nsub, subCells) {
			if (subCells[nsub].IsComposite()) {
				size_t subLayers = subCells[nsub].Layers();
				if (subLayers > maxSubLayers)
					maxSubLayers = subLayers;
			}
		}
		return layers + maxSubLayers;
	}

	/**
	 * Set header cell label.
	 * @param _label label
	 */
	wxAdvHdrCell &Label(wxString _label)
	{
		label = _label;
		return *this;
	}

	/**
	 * Returns header cell label.
	 */
	wxString Label()
	{
		return label;
	}

	/**
	 * Sets size (width for column, height for row) for header cell.
	 */
	wxAdvHdrCell &Size(wxCoord _size)
	{
		size = _size;
		return *this;
	}

	wxCoord Size()
	{
		return size;
	}

	/**
	 * Returns hdrCell layer header cell count.
	 * e.g. real row/column count.
	 */
	size_t GetRealCellCount()
	{
		size_t count = 0;

		if (!IsComposite()) {
			return 1;
		}

		FOREACH_HDRCELL(n, subCells) {
			count += subCells[n].GetRealCellCount();
		}
		return count;
	}

	/**
	 * Returns summary hdrCell layer cell count.
	 */
	static size_t GetRealCellCount(wxAdvHdrCell *cells, size_t numCells);

private:
	//
	// Internal routines
	//
	void MarkAsRow()
	{
		SetIsRowRecursive(true);
	}

	void MarkAsCol()
	{
		SetIsRowRecursive(false);
	}

	void SetIsRowRecursive(bool _isRow)
	{
		isRow = _isRow;

		FOREACH_HDRCELL(n, subCells) {
			subCells[n].SetIsRowRecursive(_isRow);
		}
	}

	wxAdvHdrCellArray subCells;
	wxString label;
	wxCoord size;

	int alignVertical;
	int alignHorizontal;

	wxCoord spacing;

	bool verticalText;

	bool sortable;

	//
	// used internally by wxAdvTable
	//
	// for wxAdvTable to store row/column position
	wxRect rc;
	size_t index;
	bool isRow;
	bool isReal;
};

class wxAdvRange
{
	friend class wxAdvTable;

public:
	wxAdvRange()
	{
		Set(-1, -1, -1, -1);
	}

	wxAdvRange(size_t _row1, size_t _col1, size_t _row2, size_t _col2)
	{
		Set(_row1, _col1, _row2, _col2);
	}

	virtual ~wxAdvRange()
	{
	}

	void Set(size_t _row, size_t _col)
	{
		row1 = _row;
		col1 = _col;
		row2 = _row;
		col2 = _col;
	}

	void Set(size_t _row1, size_t _col1, size_t _row2, size_t _col2)
	{
		row1 = min(_row1, _row2);
		col1 = min(_col1, _col2);
		row2 = max(_row1, _row2);
		col2 = max(_col1, _col2);
	}

	bool Contains(size_t _row, size_t _col)
	{
		return (_row >= row1 && _row <= row2) &&
				(_col >= col1 && _col <= col2);
	}

	void operator = (const wxAdvRange &o)
	{
		row1 = o.row1;
		col1 = o.col1;
		row2 = o.row2;
		col2 = o.col2;
	}

private:
	size_t min(size_t a, size_t b)
	{
		size_t m = MIN(a, b);
		if (m == (size_t) -1) {
			if (a != (size_t) -1) {
				m = a;
			}
			else if (b != (size_t) -1) {
				m = b;
			}
		}
		return m;
	}

	size_t max(size_t a, size_t b)
	{
		size_t m = MAX(a, b);
		if (m == (size_t) -1) {
			if (a != (size_t) -1) {
				m = a;
			}
			else if (b != (size_t) -1) {
				m = b;
			}
		}
		return m;
	}

	size_t row1, col1;
	size_t row2, col2;
};

/**
 * Flexible and simple table control implementation.
 *
 * Features:
 * - composite rows/columns.
 * - cell concatenations.
 * - sorting and filtering.
 * - MCV design (designed after Java(tm) JTable classes).
 *   Data accesed through wxAdvTableModel, renderered by wxAdvTableCellRenderer, edited by wxAdvTableCellEditor
 * - rows/columns dynamic add/remove support
 * - TODO: implement editors and various renderers (for bool, color, etc data types)
 * - TODO: make drawing code optimizations (it is too slow now)
 * - TODO: add printing support
 * - TODO: add filters support
 */
class wxAdvTable : public wxScrolledWindow, public wxAdvTableDataModelObserver
{
public:
	enum SortMode {
		Rows, Cols,
	};

	enum SelectMode {
		SelectCell, SelectRows, SelectCols, SelectBlock,
	};

	enum HighlightMode {
		HighlightNone, HighlightRows, HighlightCols, HighlightBoth,
	};

	enum SortDirection {
		Ascending, Descending,
	};

	wxAdvTable(wxWindow *parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);
	virtual ~wxAdvTable();

	/**
	 * Create table and arrange rows and columns.
	 * Call this after you create wxAdvTable before using it.
	 * wxAdvTable takes ownership for table model
	 * @param _rows rows array
	 * @param numRows number of elements in rows array
	 * @param _cols columns arrray
	 * @param numCols number of elements in columns array
	 * @param _corner string to use as corner
	 * @param model data model
	 */
	void Create(wxAdvHdrCell *_rows, size_t numRows, wxAdvHdrCell *_cols, size_t numCols, wxString _corner, wxAdvTableDataModel *_model);

	/**
	 * Create table and arrange rows and columns,
	 * with specified row count.
	 * Call this after you create wxAdvTable before using it.
	 * wxAdvTable takes ownership for table model
	 * @param numRows number of elements in rows array
	 * @param _cols columns array
	 * @param numCols number of elements in columns array
	 * @param _model data model
	 */
	void Create(size_t numRows, wxAdvHdrCell *_cols, size_t numCols, wxAdvTableDataModel *_model);

	/**
	 * Create table and arrange rows and columns,
	 * with specified column count.
	 * Call this after you create wxAdvTable before using it.
	 * wxAdvTable takes ownership for table model
	 * @param numRows number of elements in rows array
	 * @param _cols columns array
	 * @param numCols number of elements in columns array
	 * @param _model data model
	 */
	void Create(wxAdvHdrCell *_rows, size_t numRows, size_t numCols, wxAdvTableDataModel *_model);

	/**
	 * Create table and arrange rows and columns, with specified
	 * row and column count.
	 * Call this after you create wxAdvTable before using it.
	 * wxAdvTable takes ownership for table model
	 * @param numRows number of elements in rows array
	 * @param numCols number of elements in columns array
	 * @param _model data model
	 */
	void Create(size_t numRows, size_t numCols, wxAdvTableDataModel *_model);

	void DestroyTable();

	//
	// Dynamic row/column add/remove functions.
	//
	void AddRows(wxAdvHdrCell *_rows, size_t numRows);

	void AddCols(wxAdvHdrCell *_cols, size_t numCols);

	/**
	 * Remove rows from
	 */
/*	void RemoveRows(size_t from, size_t to);

	void RemoveCols(size_t from, size_t to);
*/

	void SetShowHeaders(bool _showRows, bool _showCols);

	void SetShowRows(bool _showRows)
	{
		SetShowHeaders(_showRows, showCols);
	}

	bool GetShowRows()
	{
		return showRows;
	}

	void SetShowCols(bool _showCols)
	{
		SetShowHeaders(showRows, _showCols);
	}

	bool GetShowCols()
	{
		return showCols;
	}

	/**
	 * Sets sorter for table data.
	 * wxAdvTable takes ownership for sorter object.
	 * @param _sorter new sorter
	 */
	void SetSorter(wxAdvTableSorter *_sorter);

	void SetSortMode(SortMode _sortMode);

	void SetSortingIndex(size_t _sortingIndex);

	//void SetFilter(wxAdvTableFilter *_filter);

	void SetHighlighMode(HighlightMode _highlightMode);

	HighlightMode GetHighlightMode()
	{
		return highlightMode;
	}

	//
	// Row column access functions.
	//
    size_t GetRowCount()
	{
		return rows.GetCount();
	}

    /**
     * Returns real row count (after composite rows decomposition).
     */
	size_t GetRealRowCount()
	{
		return realRows.Count();
	}

	size_t GetColCount()
	{
		return cols.GetCount();
	}

    /**
     * Returns real column count (after composite columns decomposition).
     */
	size_t GetRealColCount()
	{
		return realCols.Count();
	}

	wxAdvHdrCell &GetRow(size_t index)
	{
		return rows[index];
	}

	wxAdvHdrCell *GetRealRow(size_t index)
	{
		return realRows[index];
	}

	wxAdvHdrCell *GetRealCol(size_t index)
	{
		return realCols[index];
	}

	wxAdvTableCellRenderer *GetRendererForCell(size_t nRow, size_t nCol);

	/**
	 * Sets table cell renderer for cell format.
	 * @param format cell format
	 * @param renderer renderer for cell format
	 */
	void SetRendererForFormat(int format, wxAdvTableCellRenderer *renderer);

	void SetDefaultRenderer(wxAdvTableCellRenderer *renderer);

	/**
	 * Returns table cell value.
	 * @param nRow row number
	 * @param nCol column number
	 */
	wxString GetCellValue(size_t nRow, size_t nCol);

	void GetRealCellIndex(size_t row, size_t col, size_t &realRow, size_t &realCol);

	/**
	 * Returns table cell rectangle.
	 * @param nRow row number
	 * @param nCol column number
	 */
	wxRect GetCellRect(size_t nRow, size_t nCol);

	/**
	 * Returns cell at point.
	 * @param pt point
	 * @param row output row index
	 * @param col output column index
	 * @return true if point is in cell, false if point don't intersect any cell
	 */
	bool GetCellAt(wxPoint pt, size_t &row, size_t &col);

	/**
	 * Returns header cell (row or column) at given point.
	 * @return header cell or NULL if not found
	 */
	wxAdvHdrCell *GetHdrCellAt(wxPoint pt);

	/**
	 * Checks if specified cell is selected.
	 * @param row row index of cell
	 * @param col column index of cell
	 */
	bool IsCellSelected(size_t row, size_t col)
	{
		return selected.Contains(row, col);
	}

	/**
	 * Checks if specified cell is highlighted.
	 * @param row row index of cell
	 * @param col column index of cell
	 */
	bool IsCellHighlighted(size_t row, size_t col);

	//
	// Selection functions
	//
	void SetSelection(wxAdvRange &range);

	void SetSelection(size_t row, size_t col);

	void SetSelection(size_t row1, size_t col1, size_t row2, size_t col2);

	/**
	 * Returns selection range.
	 */
	const wxAdvRange &GetSelection();

	/**
	 * Sets select mode.
	 * @param _selectMode new select mode, possible values: SelectCell, SelectRows, SelectCols, SelectBlock.
	 */
	void SetSelectMode(SelectMode _selectMode);

	int GetSelectMode();

	/**
	 * Checks if specified cell is in focus.
	 * @param row row index of cell
	 * @param col column index of cell
	 */
	bool IsCellFocused(size_t row, size_t col)
	{
		return (focusedRow == row) && (focusedCol == col);
	}

	void SetFocusedCell(size_t row, size_t col);

	wxCoord GetTotalColsWidth()
	{
		return SumDimensions(realCols, true);
	}

	wxCoord GetTotalRowsHeight()
	{
		return SumDimensions(realRows, false);
	}

	wxCoord GetTotalRowLayersWidth()
	{
		return SumLayerSizes(rowLayerWidths);
	}

	wxCoord GetTotalColLayersHeight()
	{
		return SumLayerSizes(colLayerHeights);
	}

	/**
	 * Returns table area rectangle.
	 */
	wxRect GetTableRect()
	{
		return wxRect(GetTotalRowLayersWidth(), GetTotalColLayersHeight(),
				GetTotalColsWidth(), GetTotalRowsHeight());
	}

	/**
	 * Returns rows area rectangle.
	 */
	wxRect GetRowsRect()
	{
		if (showRows) {
			wxCoord y = (showCols) ? GetTotalColLayersHeight() : 0;
			return wxRect(0, y,
					GetTotalRowLayersWidth(), GetTotalRowsHeight());
		}
		else {
			return wxRect(0, 0, 0, 0);
		}
	}

	/**
	 * Returns columns area rectangle.
	 */
	wxRect GetColsRect()
	{
		if (showCols) {
			wxCoord x = (showRows) ? SumLayerSizes(rowLayerWidths) : 0;
			return wxRect(x, 0,
					GetTotalColsWidth(), SumLayerSizes(colLayerHeights));
		}
		else {
			return wxRect(0, 0, 0, 0);
		}
	}

	void ClearSelection();

	/**
	 * wxWindow override.
	 */
	virtual void Update();

	//
	// wxAdvTableDataModelObserver
	//
	virtual void CellChanged(size_t row, size_t col);

	virtual void TableChanged();

private:
	//
	// event handlers
	//
	void OnPaint(wxPaintEvent &ev);
	void OnLeftDown(wxMouseEvent &ev);
	void OnLeftUp(wxMouseEvent &ev);
	void OnMotion(wxMouseEvent &ev);
	void OnKeyDown(wxKeyEvent &ev);

	//
	// drawing functions
	//
	void DrawTable(wxDC &dc);
	void DrawTableCell(wxDC &dc, size_t nRow, size_t nCol);

	void DrawHdrCells(wxDC &dc, wxAdvHdrCellArray &hdrCells);
	void DrawHdrCell(wxDC &dc, wxAdvHdrCell &hdrCell);
	void DrawHeaderCell(wxDC &dc, wxAdvHdrCell &hdrCell);

    void FindRealSubCells(wxAdvHdrCellArray &hdrCells, wxAdvHdrCellPtrArray &flattenRowCols);
	void FindRealSubCell(wxAdvHdrCell &hdrCell, wxAdvHdrCellPtrArray &flattenRowCols);

	//
	// table geometry functions
	//

	/**
	 * Recalculate table layers and geometry.
	 */
	void CalcTableGeometry();

	/**
	 * Recalculate header geometry.
	 * @param dc device context to determine text sizes, etc.
	 */
	void CalcHeaderGeometry(wxDC &dc);

	void CalcLayerSizes(wxAdvHdrCell &hdrCell, wxCoordArray &layerSizes, bool isRows, size_t nLayer);

	wxSize FitForSubCells(wxDC &dc, wxAdvHdrCell &hdrCell, bool isRows);

	void UpdateHeaderSizes(wxAdvHdrCell &hdrCell, wxCoordArray &layerSizes, bool isRows, size_t nLayer);

	void UpdateHeaderPositions(wxAdvHdrCell &hdrCell, wxCoord x0, wxCoord y0, wxCoordArray &layerSizes, size_t nLayer, bool isRows);

	size_t LastLayer(wxAdvHdrCell &hdrCell, wxCoordArray &layerSizes)
	{
		return layerSizes.Count() - hdrCell.Layers();
	}

	void UpdateSortOrder();

	//
	// Helper functions.
	//
	wxCoord SumDimensions(wxAdvHdrCellPtrArray &hdrCells, bool widths);

	wxCoord SumLayerSizes(wxCoordArray &layerSizes, size_t first, size_t last);

	wxCoord SumLayerSizes(wxCoordArray &layerSizes)
	{
		return SumLayerSizes(layerSizes, 0, layerSizes.Count() - 1);
	}

	wxAdvHdrCell *GetHdrCellAtRecursive(wxAdvHdrCell *cell, wxPoint &pt);

	static size_t GetLayerCount(wxAdvHdrCellArray &hdrCells);

	static void AddHdrCells(wxAdvHdrCellArray &arr, wxAdvHdrCell *hdrCells, size_t numCells, bool isRows);

	/**
	 * Translate point from window to viewport coordinates.
	 * @param pt point in window coordinates
	 */
	wxPoint ToViewportPosition(wxPoint &pt);


	bool created;

	bool showRows;
	bool showCols;

	wxAdvHdrCellArray rows;
	wxAdvHdrCellArray cols;
	wxAdvHdrCell cornerCell;
	wxAdvTableSorter *sorter;
	SortMode sortMode;

	wxIndexArray rowsOrder;
	wxIndexArray colsOrder;

	wxAdvHdrCellPtrArray realRows;
	wxAdvHdrCellPtrArray realCols;

	wxCoordArray rowLayerWidths;
	wxCoordArray colLayerHeights;

	HighlightMode highlightMode;

	wxAdvRangeArray cellConcats;

	// graphic objects
	wxPen gridPen;
	wxPen focusedPen;
	wxBrush bgBrush;
	wxBrush selectedBgBrush;
	wxBrush focusedBgBrush;
	wxBrush highlightedBgBrush;

	wxAdvTableDataModel *model;
	wxAdvTableCellRenderer *defaultRenderer;

	wxAdvTableCellRendererMap renderers;

	wxAdvHdrCell *clickedCell;

	SelectMode selectMode;
	wxAdvRange selected;
	size_t selRow, selCol;

	size_t sortingIndex;
	int sortDirection;

	size_t focusedRow, focusedCol;

	DECLARE_EVENT_TABLE()
};

#endif /*_WX_WXADVTABLE_H_*/
