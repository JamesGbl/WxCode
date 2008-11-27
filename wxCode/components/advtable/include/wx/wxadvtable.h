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

// include grid.h for events definitions
#include <wx/grid.h>

#include <wx/dynarray.h>
#include "observable.h"
#include "wxadvtabledefs.h"

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

// boolean true - string representation
#define boolTrue wxT("1")
#define boolFalse wxT("0")

// Forward declarations
class wxAdvTable;
class wxAdvHdrCell;
class wxAdvCoord;
class wxAdvRange;
class wxAdvCellAttribute;
class wxAdvTableCellRenderer;
class wxAdvTableCellEditor;

// arrays/maps declarations
WX_DECLARE_EXPORTED_OBJARRAY(wxArrayString, wxArrayArrayString);
WX_DECLARE_EXPORTED_OBJARRAY(wxAdvHdrCell, wxAdvHdrCellArray);
WX_DECLARE_EXPORTED_OBJARRAY(wxAdvHdrCell *, wxAdvHdrCellPtrArray);
WX_DECLARE_EXPORTED_OBJARRAY(wxAdvRange, wxAdvRangeArray);
WX_DECLARE_EXPORTED_OBJARRAY(wxCoord, wxCoordArray);
WX_DECLARE_EXPORTED_OBJARRAY(size_t, wxIndexArray);
WX_DECLARE_HASH_MAP(int, wxAdvTableCellRenderer *, wxIntegerHash, wxIntegerEqual, wxAdvTableCellRendererMap);
WX_DECLARE_HASH_MAP(int, wxAdvTableCellEditor *, wxIntegerHash, wxIntegerEqual, wxAdvTableCellEditorMap);


/**
 * Table cell coordinate.
 */
class WXDLLEXPORT wxAdvCoord
{
public:
	wxAdvCoord()
	{
		Unset();
	}

	virtual ~wxAdvCoord()
	{
	}

	void Set(size_t row, size_t col)
	{
		m_row = row;
		m_col = col;
	}

	bool IsSet()
	{
		return (m_row != (size_t) -1) && (m_col != (size_t) -1);
	}

	void Unset()
	{
		m_row = (size_t) -1;
		m_col = (size_t) -1;
	}

	bool Equal(size_t row, size_t col)
	{
		return (m_row == row) && (m_col == col);
	}

	size_t Row()
	{
		return m_row;
	}

	size_t Col()
	{
		return m_col;
	}

private:
	size_t m_row, m_col;
};

/**
 * Range
 */
class WXDLLEXPORT wxAdvRange
{
	friend class wxAdvTable;

public:
	wxAdvRange()
	{
		Set((size_t) -1, (size_t) -1, (size_t) -1, (size_t) -1);
	}

	wxAdvRange(size_t row1, size_t col1, size_t row2, size_t col2)
	{
		Set(row1, col1, row2, col2);
	}

	virtual ~wxAdvRange()
	{
	}

	/**
	 * Setup range to single cell.
	 * @param row row index
	 * @param col row index
	 */
	void Set(size_t row, size_t col)
	{
		m_row1 = row;
		m_col1 = col;
		m_row2 = row;
		m_col2 = col;
	}

	void Set(size_t row1, size_t col1, size_t row2, size_t col2)
	{
		m_row1 = min(row1, row2);
		m_col1 = min(col1, col2);
		m_row2 = max(row1, row2);
		m_col2 = max(col1, col2);
	}

	/**
	 * Determines if specified cell is in range.
	 * @param row row index
	 * @param col column index
	 * @return true if cell in range
	 */
	bool Contains(size_t row, size_t col)
	{
		return (row >= m_row1 && row <= m_row2) &&
				(col >= m_col1 && col <= m_col2);
	}

	void operator = (const wxAdvRange &o)
	{
		m_row1 = o.m_row1;
		m_col1 = o.m_col1;
		m_row2 = o.m_row2;
		m_col2 = o.m_col2;
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

	size_t m_row1, m_col1;
	size_t m_row2, m_col2;
};

class WXDLLEXPORT wxAdvCellAttribute
{
public:
	wxAdvCellAttribute();
	virtual ~wxAdvCellAttribute();

	void Alignment(int alignment)
	{
		m_alignment = alignment;
	}

	int Alignment()
	{
		return m_alignment;
	}

	const wxFont &Font()
	{
		return m_font;
	}

	const wxBrush &Brush()
	{
		return m_brush;
	}

	void Brush(wxBrush brush)
	{
		m_brush = brush;
	}

	wxColour TextColour()
	{
		return m_textColour;
	}

	void TextColour(wxColour textColour)
	{
		m_textColour = textColour;
	}

private:
	wxBrush m_brush;
	wxFont m_font;

	int m_alignment;
	wxColour m_textColour;
};

/**
 * Base class for receiving wxAdvTableDataModel events.
 */
class WXDLLEXPORT wxAdvTableDataModelObserver
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
class WXDLLEXPORT wxAdvTableDataModel : public Observable<wxAdvTableDataModelObserver>
{
public:
	wxAdvTableDataModel();
	virtual ~wxAdvTableDataModel();

	virtual wxString GetCellValue(size_t row, size_t col) = 0;

	virtual bool SetCellValue(size_t row, size_t col, wxString value) = 0;

	virtual int GetCellFormat(size_t row, size_t col) = 0;

	virtual bool IsCellEditable(size_t row, size_t col) = 0;

	virtual wxAdvCellAttribute *GetCellAttribute(size_t row, size_t col) = 0;

	virtual double GetCellValueDouble(size_t row, size_t col);

protected:
	FIRE_WITH_VALUE2(CellChanged, size_t, row, size_t, col);
	FIRE_VOID(TableChanged);
};

/**
 * Simpliest table model for wxAdvTable.
 * Stores data as strings, all cells have string format.
 */
class WXDLLEXPORT wxAdvStringTableDataModel : public wxAdvTableDataModel
{
public:
	wxAdvStringTableDataModel(size_t _numRows, size_t _numCols, bool _readOnly);
	virtual ~wxAdvStringTableDataModel();

	virtual wxString GetCellValue(size_t row, size_t col);

	virtual bool SetCellValue(size_t row, size_t col, wxString value);

	virtual int GetCellFormat(size_t row, size_t col);

	virtual bool IsCellEditable(size_t row, size_t col);

	virtual wxAdvCellAttribute *GetCellAttribute(size_t row, size_t col);

	virtual double GetCellValueDouble(size_t row, size_t col);

private:
	bool m_readOnly;

	wxArrayArrayString m_data;
	wxAdvCellAttribute m_defaultCellAttribute;
};

/**
 * Stores data in double array of strings, and updates it when underlaying model
 * data has been changed.
 */
class WXDLLEXPORT wxAdvFilterTableDataModel : public wxAdvTableDataModel, public wxAdvTableDataModelObserver
{
public:
	/**
	 * Constructs new object.
	 * @param model underlaying model, wxAdvFilterTableDataModel takes ownership for _model
	 * @param numRows number of rows
	 * @param numCols number of columns
	 */
	wxAdvFilterTableDataModel(wxAdvTableDataModel *model, size_t numRows, size_t numCols);
	virtual ~wxAdvFilterTableDataModel();

	virtual wxString GetCellValue(size_t row, size_t col);

	virtual bool SetCellValue(size_t row, size_t col, wxString value);

	virtual int GetCellFormat(size_t row, size_t col);

	virtual bool IsCellEditable(size_t row, size_t col);

	virtual wxAdvCellAttribute *GetCellAttribute(size_t row, size_t col);

	virtual double GetCellValueDouble(size_t row, size_t col);

	//
	// wxAdvTableDataModelObserver
	//
	virtual void CellChanged(size_t row, size_t col);

	virtual void TableChanged();

private:
	void UpdateValues();

	wxAdvTableDataModel *m_undelayingModel;

	wxArrayArrayString m_data;
	bool m_needUpdate;
};

//
// Renderers
//

/**
 * Draws cell content.
 */
class WXDLLEXPORT wxAdvTableCellRenderer
{
public:
	wxAdvTableCellRenderer();
	virtual ~wxAdvTableCellRenderer();

	virtual void Draw(wxAdvTable *table, wxDC &dc, wxRect rc, wxString value, bool selected, bool focused, wxAdvCellAttribute *attr) = 0;
};

/**
 * Draws cell content as text.
 */
class WXDLLEXPORT wxAdvStringCellRenderer : public wxAdvTableCellRenderer
{
public:
	wxAdvStringCellRenderer();
	virtual ~wxAdvStringCellRenderer();

	virtual void Draw(wxAdvTable *table, wxDC &dc, wxRect rc, wxString value, bool selected, bool focused, wxAdvCellAttribute *attr);
};

/**
 * Renderer to draw boolean cell values (draws checkmarks).
 */
class WXDLLEXPORT wxAdvBoolTableCellRenderer : public wxAdvTableCellRenderer
{
public:
	wxAdvBoolTableCellRenderer();
	virtual ~wxAdvBoolTableCellRenderer();

	virtual void Draw(wxAdvTable *table, wxDC &dc, wxRect rc, wxString value, bool selected, bool focused, wxAdvCellAttribute *attr);
};


//
// Editors
//

/**
 * Table cell editors base class.
 */
class WXDLLEXPORT wxAdvTableCellEditor
{
public:
	wxAdvTableCellEditor();
	virtual ~wxAdvTableCellEditor();

	/**
	 * Return true, if editor activated with one click (such as boolean cells editor).
	 */
	virtual bool OneClickActivate();

	void Activate(wxAdvTable *table, size_t row, size_t col);

	virtual void Deactivate(wxAdvTable *table) = 0;

protected:
	virtual void DoActivate(wxAdvTable *table, size_t row, size_t col) = 0;

	void SetNewValue(wxString newValue);

	/**
	 * Call this when editor initiate self deactivation.
	 */
	void EndEditing();

	wxAdvTable *m_table;
	wxAdvCoord m_cell;
};

/**
 * Editor for string cell values. Uses wxTextCtrl for text input.
 */
class WXDLLEXPORT wxAdvStringTableCellEditor : public wxEvtHandler, public wxAdvTableCellEditor
{
public:
	wxAdvStringTableCellEditor(wxAdvTable *table);
	virtual ~wxAdvStringTableCellEditor();

	virtual bool OneClickActivate();

	virtual void Deactivate(wxAdvTable *table);

protected:
	virtual void DoActivate(wxAdvTable *table, size_t row, size_t col);

private:
	void OnTextEnter(wxCommandEvent &ev);
	void OnKillFocus(wxFocusEvent &ev);
	void OnTextKeydown(wxKeyEvent &ev);

	wxTextCtrl *m_textCtrl;

	DECLARE_EVENT_TABLE()
};

/**
 * Editor for boolean cell values. Uses wxCheckBox for editing.
 */
class WXDLLEXPORT wxAdvBoolTableCellEditor : public wxEvtHandler, public wxAdvTableCellEditor
{
public:
	wxAdvBoolTableCellEditor(wxAdvTable *table);
	virtual ~wxAdvBoolTableCellEditor();

	virtual bool OneClickActivate();

	virtual void Deactivate(wxAdvTable *table);

protected:
	virtual void DoActivate(wxAdvTable *table, size_t row, size_t col);

private:
	//void SetNewValue(bool newValue);

	void OnKillFocus(wxFocusEvent &ev);
	void OnCheckbox(wxCommandEvent &ev);

	wxCheckBox *m_checkBox;

	DECLARE_EVENT_TABLE()
};


//
// Sorters
//

/**
 * Performs table data sorting
 */
class WXDLLEXPORT wxAdvTableSorter
{
public:
	wxAdvTableSorter();
	virtual ~wxAdvTableSorter();

	virtual int Compare(wxAdvTable *table, size_t row1, size_t col1, size_t row2, size_t col2) = 0;
};

/**
 * Sorts data by comparing string values.
 */
class WXDLLEXPORT wxAdvTableStringSorter : public wxAdvTableSorter
{
public:
	wxAdvTableStringSorter();
	virtual ~wxAdvTableStringSorter();

	virtual int Compare(wxAdvTable *table, size_t row1, size_t col1, size_t row2, size_t col2);

private:
	int Compare(wxString value1, wxString value2);
};

/**
 * Sorts data by comparing double values.
 */
class WXDLLEXPORT wxAdvTableDoubleSorter : public wxAdvTableSorter
{
public:
	wxAdvTableDoubleSorter();
	virtual ~wxAdvTableDoubleSorter();

	virtual int Compare(wxAdvTable *table, size_t row1, size_t col1, size_t row2, size_t col2);

private:
	int Compare(double value1, double value2);
};

const int undefinedSize = 0;

/**
 * Row/column description.
 *
 */
class WXDLLEXPORT wxAdvHdrCell
{
	friend class wxAdvTable;
public:

	wxAdvHdrCell(const wxAdvHdrCell &o)
	{
		// copy attributes
		m_subCells = o.m_subCells;
		m_label = o.m_label;
		m_size  = o.m_size;
		m_alignVertical = o.m_alignVertical;
		m_alignHorizontal = o.m_alignHorizontal;
		m_spacing = o.m_spacing;
		m_verticalText = o.m_verticalText;
		m_sortable = o.m_sortable;

		// copy internals
		m_rc = o.m_rc;
		m_index = o.m_index;
		m_isRow = o.m_isRow;
		m_isDecomp = o.m_isDecomp;
	}

	wxAdvHdrCell(const wxChar *label = wxT(""))
	{
		m_label = label;
		m_size = undefinedSize;
		m_alignVertical = wxALIGN_TOP;
		m_alignHorizontal = wxALIGN_CENTER;
		m_spacing = 5;
		m_verticalText = false;
		m_sortable = false;
		m_isDecomp = false;
	}

	wxAdvHdrCell(wxString &label)
	{
		m_label = label;
		m_size = undefinedSize;
		m_alignVertical = wxALIGN_TOP;
		m_alignHorizontal = wxALIGN_CENTER;
		m_spacing = 5;
		m_verticalText = false;
		m_sortable = false;
		m_isDecomp = false;
	}

	virtual ~wxAdvHdrCell()
	{
	}

	/**
	 * Adds sub row/column.
	 * @param subHdrCell sub row/column
	 */
	wxAdvHdrCell &Sub(wxAdvHdrCell subHdrCell)
	{
		m_subCells.Add(subHdrCell);
		return *this;
	}

	/**
	 * Adds sub row/column.
	 * @param label label for sub row/column
	 */
	wxAdvHdrCell &Sub(const wxChar *label)
	{
		m_subCells.Add(wxAdvHdrCell(label));
		return *this;
	}

	bool IsComposite()
	{
		return m_subCells.Count() != 0;
	}

	/**
	 * Sets vertical text alignment for header cell (not table values).
	 * @param alignVertical vertical alignment
	 */
	wxAdvHdrCell &AlignVertical(int alignVertical)
	{
		m_alignVertical = alignVertical;
		return *this;
	}

	int AlignVertical()
	{
		return m_alignVertical;
	}

	/**
	 * Sets horizontal text alignment for header cell (not table values).
	 * @param alignHorizontal horizontal alignment
	 */
	wxAdvHdrCell &AlignHorizontal(int alignHorizontal)
	{
		m_alignHorizontal = alignHorizontal;
		return *this;
	}

	int AlignHorizontal()
	{
		return m_alignHorizontal;
	}

	wxAdvHdrCell &VerticalText()
	{
		m_verticalText = true;
		return *this;
	}

	wxAdvHdrCell &Sortable()
	{
		m_sortable = true;
		return *this;
	}

	wxAdvHdrCell &Spacing(wxCoord spacing)
	{
		m_spacing = spacing;
		return *this;
	}

	wxSize CalcExtent(wxDC &dc)
	{
		// TODO add support for multiline text
		wxSize extent = dc.GetTextExtent(m_label);

		extent.IncBy(2 * m_spacing);

		if (m_verticalText) {
			// rotate by 90 degrees
			wxCoord tmp = extent.x;
			extent.x = extent.y;
			extent.y = tmp;
		}

		if (m_size != undefinedSize) {
			if (m_isRow) {
				extent.SetHeight(m_size);
			}
			else {
				extent.SetWidth(m_size);
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

		FOREACH_HDRCELL(nsub, m_subCells) {
			if (m_subCells[nsub].IsComposite()) {
				size_t subLayers = m_subCells[nsub].Layers();
				if (subLayers > maxSubLayers)
					maxSubLayers = subLayers;
			}
		}
		return layers + maxSubLayers;
	}

	/**
	 * Set header cell label.
	 * @param label label
	 */
	wxAdvHdrCell &Label(const wxString &label)
	{
		m_label = label;
		return *this;
	}

	/**
	 * Returns header cell label.
	 */
	wxString Label()
	{
		return m_label;
	}

	/**
	 * Sets size (width for column, height for row) for header cell.
	 */
	wxAdvHdrCell &Size(wxCoord size)
	{
		m_size = size;
		return *this;
	}

	wxCoord Size()
	{
		return m_size;
	}

	/**
	 * Returns hdrCell layer header cell count.
	 * e.g. real row/column count.
	 */
	size_t GetDecompCellCount()
	{
		size_t count = 0;

		if (!IsComposite()) {
			return 1;
		}

		FOREACH_HDRCELL(n, m_subCells) {
			count += m_subCells[n].GetDecompCellCount();
		}
		return count;
	}

	/**
	 * Returns summary hdrCell layer cell count.
	 */
	static size_t GetDecompCellCount(wxAdvHdrCell *cells, size_t numCells);

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

	void SetIsRowRecursive(bool isRow)
	{
		m_isRow = isRow;

		FOREACH_HDRCELL(n, m_subCells) {
			m_subCells[n].SetIsRowRecursive(isRow);
		}
	}

	wxAdvHdrCellArray m_subCells;
	wxString m_label;
	wxCoord m_size;

	int m_alignVertical;
	int m_alignHorizontal;

	wxCoord m_spacing;

	bool m_verticalText;

	bool m_sortable;

	//
	// used internally by wxAdvTable
	//
	// for wxAdvTable to store row/column position
	wxRect m_rc;
	size_t m_index;
	bool m_isRow;
	bool m_isDecomp;
};

/**
 * Main component class.
 * Table control implementation.
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
 *
 *   NOTES:
 *	There are two types of cell coordinates: table and model.
 *	Table coordinate used everywhere in table, when you click on cell,
 *	when you access data through wxAdvTable::GetCellValue.
 *	Model coordinates used by model.
 */
class WXDLLEXPORT wxAdvTable : public wxScrolledWindow, public wxAdvTableDataModelObserver
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
	 * @param rows rows array
	 * @param numRows number of elements in rows array
	 * @param cols columns arrray
	 * @param numCols number of elements in columns array
	 * @param cornerLabel string to use as corner
	 * @param model data model
	 */
	void Create(wxAdvHdrCell *rows, size_t numRows, wxAdvHdrCell *cols, size_t numCols, const wxString &cornerLabel, wxAdvTableDataModel *model);

	/**
	 * Create table and arrange rows and columns,
	 * with specified row count.
	 * Call this after you create wxAdvTable before using it.
	 * wxAdvTable takes ownership for table model
	 * @param numRows number of elements in rows array
	 * @param cols columns array
	 * @param numCols number of elements in columns array
	 * @param cornerLabel string to use as corner
	 * @param model data model
	 */
	void Create(size_t numRows, wxAdvHdrCell *cols, size_t numCols, const wxString &cornerLabel, wxAdvTableDataModel *model);

	/**
	 * Create table and arrange rows and columns,
	 * with specified column count.
	 * Call this after you create wxAdvTable before using it.
	 * wxAdvTable takes ownership for table model
	 * @param rows rows array
	 * @param numRows number of elements in rows array
	 * @param numCols number of elements in columns array
	 * @param cornerLabel string to use as corner
	 * @param model data model
	 */
	void Create(wxAdvHdrCell *rows, size_t numRows, size_t numCols, const wxString &cornerLabel, wxAdvTableDataModel *model);

	/**
	 * Create table and arrange rows and columns, with specified
	 * row and column count.
	 * Call this after you create wxAdvTable before using it.
	 * wxAdvTable takes ownership for table model
	 * @param numRows number of elements in rows array
	 * @param numCols number of elements in columns array
	 * @param cornerLabel string to use as corner
	 * @param model data model
	 */
	void Create(size_t numRows, size_t numCols, const wxString &cornerLabel, wxAdvTableDataModel *model);

	void DestroyTable();

	//
	// Dynamic row/column add/remove functions.
	//
	void AddRows(wxAdvHdrCell *rows, size_t numRows);

	void AddCols(wxAdvHdrCell *cols, size_t numCols);

	/**
	 * Remove rows from
	 */
/*	void RemoveRows(size_t from, size_t to);

	void RemoveCols(size_t from, size_t to);
*/

	void SetShowHeaders(bool showRows, bool showCols, bool showCorner);

	void SetShowRows(bool showRows)
	{
		SetShowHeaders(showRows, m_showCols, m_showCorner);
	}

	bool GetShowRows()
	{
		return m_showRows;
	}

	void SetShowCols(bool showCols)
	{
		SetShowHeaders(m_showRows, showCols, m_showCorner);
	}

	bool GetShowCols()
	{
		return m_showCols;
	}

	void SetShowCorner(bool showCorner)
	{
		SetShowHeaders(m_showRows, m_showCols, showCorner);
	}

	bool GetShowCorner()
	{
		return m_showCorner;
	}

	//
	// Sorting functions.
	//

	/**
	 * Sets sorter for table data.
	 * wxAdvTable takes ownership for sorter object.
	 * @param sorter new sorter
	 */
	void SetSorter(wxAdvTableSorter *sorter);

	/**
	 * Sets sorting mode (eg Rows to sort rows - vertical sort, or Cols to
	 * sort columns - horizontal sort).
	 * @param sortMode new sorting mode
	 */
	void SetSortMode(SortMode sortMode);

	void SetSortingIndex(size_t sortingIndex);

	//void SetFilter(wxAdvTableFilter *_filter);

	void SetHighlightMode(HighlightMode highlightMode);

	HighlightMode GetHighlightMode()
	{
		return m_highlightMode;
	}

	//
	// Row/column access functions.
	//

    size_t GetRowCount()
	{
		return m_rows.GetCount();
	}

    /**
     * Returns decomposed row count.
     */
	size_t GetDecompRowCount()
	{
		return m_decompRows.Count();
	}

	size_t GetColCount()
	{
		return m_cols.GetCount();
	}

    /**
     * Returns decomposed column count.
     */
	size_t GetDecompColCount()
	{
		return m_decompCols.Count();
	}

	wxAdvHdrCell &GetRow(size_t index)
	{
		return m_rows[index];
	}

	wxAdvHdrCell *GetDecompRow(size_t index)
	{
		return m_decompRows[index];
	}

	wxAdvHdrCell *GetDecompCol(size_t index)
	{
		return m_decompCols[index];
	}

	wxAdvTableDataModel *GetModel()
	{
		return m_model;
	}

	wxAdvTableCellEditor *GetEditorForCell(size_t row, size_t col);

	/**
	 * Set cell editor for specified cell format.
	 * wxAdvTable takes ownership over editor.
	 * @param format cell format
	 * @param editor cell editor for format
	 */
	void SetEditorForFormat(int format, wxAdvTableCellEditor *editor);

	/**
	 * Deactivate editor if active, and commit changes.
	 */
	void StopEditing();

	wxAdvTableCellRenderer *GetRendererForCell(size_t row, size_t col);

	/**
	 * Sets table cell renderer for specified cell format.
	 * wxAdvTable takes ownership over renderer.
	 * @param format cell format
	 * @param renderer renderer for cell format
	 */
	void SetRendererForFormat(int format, wxAdvTableCellRenderer *renderer);

	void SetDefaultRenderer(wxAdvTableCellRenderer *renderer);

	/**
	 * Returns table cell value.
	 * @param row row number
	 * @param col column number
	 */
	wxString GetCellValue(size_t row, size_t col);

	bool IsCellEditable(size_t row, size_t col);

	/**
	 * Transforms cell coordinate from table to model space.
	 * @param row sorted row index
	 * @param col sorted column index
	 * @param modelRow unsorted row index
	 * @param modelCol unsorted column index
	 */
	void ToModelCellCoord(size_t row, size_t col, size_t &modelRow, size_t &modelCol);

	/**
	 * Transforms cell coordinate from model to table space.
	 * @param row sorted row index
	 * @param col sorted column index
	 * @param tableRow unsorted row index
	 * @param tableCol unsorted column index
	 */
	void ToTableCellCoord(size_t row, size_t col, size_t &tableRow, size_t &tableCol);

	/**
	 * Returns table cell rectangle.
	 * @param row row number
	 * @param col column number
	 */
	wxRect GetCellRect(size_t row, size_t col);

	bool GetCellRect(wxRect &rc, size_t row, size_t col);

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
		return m_selected.Contains(row, col);
	}

	/**
	 * Checks if specified cell is highlighted.
	 * @param row row index of cell
	 * @param col column index of cell
	 */
	bool IsCellHighlighted(size_t row, size_t col);

	//
	// Selection functions
	// They are don't trigger selection events
	//
	void SetSelection(wxAdvRange &range);

	void SetSelection(size_t row, size_t col);

	void SetSelection(size_t row1, size_t col1, size_t row2, size_t col2);

	/**
	 * Returns selection range.
	 */
	const wxAdvRange &GetSelection();

	/**
	 * Clear selection.
	 */
	void ClearSelection();

	/**
	 * Sets select mode.
	 * @param selectMode new select mode, possible values: SelectCell, SelectRows, SelectCols, SelectBlock.
	 */
	void SetSelectMode(SelectMode selectMode);

	/**
	 * Returns select mode.
	 */
	int GetSelectMode();

	/**
	 * Checks if specified cell is in focus.
	 * @param row row index of cell
	 * @param col column index of cell
	 */
	bool IsCellFocused(size_t row, size_t col)
	{
		return m_focused.Equal(row, col);
	}

	/**
	 * Sets focused cell.
	 * @param row row index of cell
	 * @param col column index of cell
	 */
	void SetFocusedCell(size_t row, size_t col);

	wxCoord CalcTotalColsWidth()
	{
		return SumDimensions(m_decompCols, true);
	}

	wxCoord CalcTotalRowsHeight()
	{
		return SumDimensions(m_decompRows, false);
	}

	wxCoord CalcTotalRowLayersWidth()
	{
		return SumLayerSizes(m_rowLayerWidths);
	}

	wxCoord CalcTotalColLayersHeight()
	{
		return SumLayerSizes(m_colLayerHeights);
	}

	/**
	 * Returns table area rectangle.
	 */
	wxRect CalcTableRect()
	{
		return wxRect(CalcTotalRowLayersWidth(), CalcTotalColLayersHeight(),
				CalcTotalColsWidth(), CalcTotalRowsHeight());
	}

	/**
	 * Returns rows area rectangle.
	 */
	wxRect CalcRowsRect()
	{
		if (m_showRows) {
			wxCoord y = (m_showCols || m_showCorner) ? CalcTotalColLayersHeight() : 0;
			return wxRect(0, y,
					CalcTotalRowLayersWidth(), CalcTotalRowsHeight());
		}
		else {
			return wxRect(0, 0, 0, 0);
		}
	}

	/**
	 * Returns columns area rectangle.
	 */
	wxRect CalcColsRect()
	{
		if (m_showCols) {
			wxCoord x = (m_showRows || m_showCorner) ? SumLayerSizes(m_rowLayerWidths) : 0;
			return wxRect(x, 0,
					CalcTotalColsWidth(), SumLayerSizes(m_colLayerHeights));
		}
		else {
			return wxRect(0, 0, 0, 0);
		}
	}

	wxRect CalcEntireRowRect(size_t row, size_t col);
	wxRect CalcEntireColRect(size_t row, size_t col);

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
	void OnMouseEvents(wxMouseEvent &ev);
	void OnKeyDown(wxKeyEvent &ev);
	void OnKillFocus(wxFocusEvent &ev);

	void HandleHdrCellMouseEvent(wxMouseEvent &ev, wxAdvHdrCell *cell);
	void HandleCellMouseEvent(wxMouseEvent &ev, size_t row, size_t col);

	/**
	 * Update selection, called from mouse and key event handlers.
	 */
	void SelectCells(wxMouseEvent &ev, size_t row, size_t col);

	//
	// Editing internal functions.
	//
	void EditCell(wxAdvTableCellEditor *editor, size_t row, size_t col);

	void SetPressedHdrCell(wxAdvHdrCell *cell);
	void SetCurrentHdrCell(wxAdvHdrCell *cell);

	//
	// drawing functions
	//
	void DrawTable(wxDC &dc, wxRegion reg);
	void DrawTableCell(wxDC &dc, size_t row, size_t col);

	void DrawHdrCells(wxDC &dc, wxAdvHdrCellArray &hdrCells);
	void DrawHdrCell(wxDC &dc, wxAdvHdrCell &hdrCell);
	void DrawHeaderCell(wxDC &dc, wxAdvHdrCell &hdrCell);

	void RedrawHdrCell(wxAdvHdrCell *cell);
	void RedrawViewportRect(wxRect rc);
	void RedrawRangeRect(wxAdvRange *range);
	void RedrawHighlightRect();

    void DecomposeHdrCells(wxAdvHdrCellArray &hdrCells, wxAdvHdrCellPtrArray &decompHdrCells);
	void DecomposeHdrCell(wxAdvHdrCell &hdrCell, wxAdvHdrCellPtrArray &decompHdrCells);

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

	void AddHdrCells(wxAdvHdrCellArray &arr, wxAdvHdrCell *hdrCells, size_t numCells, bool isRows);

	/**
	 * Translate point from window to viewport coordinates.
	 * @param pt point in window coordinates
	 */
	wxPoint ToViewportPosition(wxPoint &pt, bool transVert = true, bool transHoriz = true);

	void SendEvent(const wxEventType type, size_t row, size_t col, wxMouseEvent &ev);
	void SendRangeEvent(const wxEventType type, size_t row1, size_t col1, size_t row2, size_t col2, wxMouseEvent &ev);

	bool m_tableCreated;

	bool m_showRows;
	bool m_showCols;
	bool m_showCorner;

	wxAdvCellAttribute m_defaultCellAttribute;
	wxAdvHdrCell m_defaultRow, m_defaultCol;

	wxAdvHdrCellArray m_rows;
	wxAdvHdrCellArray m_cols;
	wxAdvHdrCell m_cornerCell;
	wxAdvTableSorter *m_sorter;
	SortMode m_sortMode;

	wxIndexArray m_sortOrder;

	wxAdvHdrCellPtrArray m_decompRows;
	wxAdvHdrCellPtrArray m_decompCols;

	wxCoordArray m_rowLayerWidths;
	wxCoordArray m_colLayerHeights;

	HighlightMode m_highlightMode;

	wxAdvRangeArray m_cellConcats;

	// graphic objects
	wxPen m_gridPen;
	wxPen m_focusedPen;
	wxBrush m_bgBrush;
	wxBrush m_selectedBgBrush;
	wxBrush m_focusedBgBrush;
	wxBrush m_highlightedBgBrush;

	wxAdvTableDataModel *m_model;

	wxAdvTableCellRenderer *m_defaultRenderer;
	wxAdvTableCellRendererMap m_renderers;
	wxAdvTableCellEditorMap m_editors;

	// selection variables
	SelectMode m_selectMode;
	wxAdvRange m_selected;
	wxAdvCoord m_pressedCell;

	// sorting variables
	size_t m_sortingIndex;
	int m_sortDirection;

	wxAdvCoord m_focused;

	wxAdvTableCellEditor *m_currentEditor;

	wxAdvHdrCell *m_pressedHdrCell;
	wxAdvHdrCell *m_currentHdrCell;

	DECLARE_EVENT_TABLE()
	DECLARE_CLASS(wxAdvTable)
	DECLARE_NO_COPY_CLASS(wxAdvTable)
};

#endif /*_WX_WXADVTABLE_H_*/
