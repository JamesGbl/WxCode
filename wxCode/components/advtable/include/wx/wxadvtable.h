/////////////////////////////////////////////////////////////////////////////
// Name:        wxAdvTable.h
// Purpose:     Definitions of wxAdvTable classes
// Author:      Moskvichev Andrey V.
// Created:     2008/10/09
// RCS-ID:      $Id: wxAdvTable.h,v 1.3 2008/10/09 16:42:58 frm Exp $
// Copyright:   (c) 2008-2010 Moskvichev A.V.
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
#include <wx/spinctrl.h>

#include <wx/dynarray.h>
#include "observable.h"

#if wxUSE_DATEPICKCTRL
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#endif /* wxUSE_DATEPICKCTRL */

// dll export macros
#ifdef WXMAKINGDLL_ADVTABLE
#define WXDLLIMPEXP_ADVTABLE WXEXPORT
#define WXDLLIMPEXP_DATA_ADVTABLE(type) WXEXPORT type
#elif defined(WXUSINGDLL)
#define WXDLLIMPEXP_ADVTABLE WXIMPORT
#define WXDLLIMPEXP_DATA_ADVTABLE(type) WXIMPORT type
#else // not making nor using DLL
#define WXDLLIMPEXP_ADVTABLE
#define WXDLLIMPEXP_DATA_ADVTABLE(type) type
#endif

// iterate through all hdrCell in array
#define FOREACH_HDRCELL(n, hdrCells) for (size_t n = 0; n < hdrCells.GetCount(); n++)


#ifndef NULL
#define NULL 0
#endif /* NULL */

#define wxREPLACE(dst, src) do {					\
	wxDELETE(dst);									\
	dst = src;										\
} while (0)

//
// Table cell value formats
//
enum {
	wxStringFormat = 1,
	wxIntFormat,
	wxDoubleFormat,
	wxBoolFormat,
	wxColourFormat,
	wxDateTimeFormat,
	wxChoicesFormat,
	wxBitmapFormat,
	wxLastFormat
};

// boolean true - string representation
#define boolTrue wxT("1")
#define boolFalse wxT("0")

#define UNDEF_SIZE (size_t) -1

//
// Forward declarations
//
class wxAdvTable;
class wxAdvHdrCell;
class wxAdvCoord;
class wxAdvRange;
class wxAdvCellAttribute;
class wxAdvCellRenderer;
class wxAdvCellEditor;

//
// arrays/maps declarations
//
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxArrayString, wxArrayArrayString, WXDLLIMPEXP_ADVTABLE);
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxAdvHdrCell, wxAdvHdrCellArray, WXDLLIMPEXP_ADVTABLE);
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxAdvHdrCell *, wxAdvHdrCellPtrArray, WXDLLIMPEXP_ADVTABLE);
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxAdvRange, wxAdvRangeArray, WXDLLIMPEXP_ADVTABLE);
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxCoord, wxCoordArray, WXDLLIMPEXP_ADVTABLE);
WX_DECLARE_USER_EXPORTED_OBJARRAY(size_t, wxIndexArray, WXDLLIMPEXP_ADVTABLE);
WX_DECLARE_HASH_MAP(int, wxAdvCellRenderer *, wxIntegerHash, wxIntegerEqual, wxAdvCellRendererMap);
WX_DECLARE_HASH_MAP(int, wxAdvCellEditor *, wxIntegerHash, wxIntegerEqual, wxAdvCellEditorMap);
WX_DECLARE_USER_EXPORTED_OBJARRAY(int, wxIntArray, WXDLLIMPEXP_ADVTABLE);
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxIntArray, wxIntIntArray, WXDLLIMPEXP_ADVTABLE);

//
// Events declarations
//

BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_ADVTABLE, wxEVT_ADVTABLE_HDRCELL_LEFT_CLICK, 5000)
	DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_ADVTABLE, wxEVT_ADVTABLE_HDRCELL_RIGHT_CLICK, 5001)
	DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_ADVTABLE, wxEVT_ADVTABLE_HDRCELL_LEFT_DCLICK, 5002)
	DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_ADVTABLE, wxEVT_ADVTABLE_HDRCELL_RIGHT_DCLICK, 5003)
	DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_ADVTABLE, wxEVT_ADVTABLE_HDRCELL_MOVE, 5004)
	DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_ADVTABLE, wxEVT_ADVTABLE_HDRCELL_SIZE, 5005)
	DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_ADVTABLE, wxEVT_ADVTABLE_HDRCELL_SORT, 5006)
END_DECLARE_EVENT_TYPES()

/**
 * Header cell event, such as:
 * <ol>
 * 	<li>wxEVT_ADVTABLE_HDRCELL_LEFT_CLICK - left button click on header cell.</li>
 * 	<li>wxEVT_ADVTABLE_HDRCELL_RIGHT_CLICK - right button click on header cell.</li>
 * 	<li>wxEVT_ADVTABLE_HDRCELL_LEFT_DCLICK - left button double click on header cell.</li>
 * 	<li>wxEVT_ADVTABLE_HDRCELL_RIGHT_DCLICK - right button double click on header cell.</li>
 * 	<li>wxEVT_ADVTABLE_HDRCELL_MOVE - header cell move.</li>
 * 	<li>wxEVT_ADVTABLE_HDRCELL_SIZE - header cell size changed.</li>
 * 	<li>wxEVT_ADVTABLE_HDRCELL_SORT - header cell sorting enabled.</li>
 * </ol>
 */
class WXDLLIMPEXP_ADVTABLE wxAdvHdrCellEvent : public wxNotifyEvent
{
public:
	wxAdvHdrCellEvent()
	: wxNotifyEvent()
	{
		m_hdrCell = NULL;
		m_x = -1;
		m_y = -1;
		m_control = false;
		m_shift = false;
		m_alt = false;
		m_meta = false;
	}

	wxAdvHdrCellEvent(int id, wxEventType type, wxObject* obj,
			wxAdvHdrCell *hdrCell, int x = -1, int y = -1,
			bool control = false, bool shift = false, bool alt = false, bool meta = false);


	wxAdvHdrCell *GetHdrCell() { return m_hdrCell; }
	wxPoint     GetPosition() { return wxPoint( m_x, m_y ); }
	bool        ControlDown() { return m_control; }
	bool        MetaDown() { return m_meta; }
	bool        ShiftDown() { return m_shift; }
	bool        AltDown() { return m_alt; }
	bool        CmdDown()
	{
#if defined(__WXMAC__) || defined(__WXCOCOA__)
		return MetaDown();
#else
		return ControlDown();
#endif
	}

	virtual wxEvent *Clone() const { return new wxAdvHdrCellEvent(*this); }

protected:
	wxAdvHdrCell *m_hdrCell;
	int         m_x;
	int         m_y;
	bool        m_control;
	bool        m_meta;
	bool        m_shift;
	bool        m_alt;

	DECLARE_DYNAMIC_CLASS_NO_ASSIGN(wxAdvHdrCellEvent)
};

typedef void (wxEvtHandler::*wxAdvHdrCellEventFunction)(wxAdvHdrCellEvent &);

#define wxAdvHdrCellEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxAdvHdrCellEventFunction, &func)

#define wx__DECLARE_ADVHDRCELLEVT(evt, id, fn) \
    wx__DECLARE_EVT1(wxEVT_ADVTABLE_ ## evt, id, wxAdvHdrCellEventHandler(fn))

#define EVT_ADVTABLE_CMD_HDRCELL_LEFT_CLICK(id, fn) wx__DECLARE_ADVHDRCELLEVT(HDRCELL_LEFT_CLICK, id, fn)
#define EVT_ADVTABLE_CMD_HDRCELL_RIGHT_CLICK(id, fn) wx__DECLARE_ADVHDRCELLEVT(HDRCELL_RIGHT_CLICK, id, fn)
#define EVT_ADVTABLE_CMD_HDRCELL_LEFT_DCLICK(id, fn) wx__DECLARE_ADVHDRCELLEVT(HDRCELL_LEFT_DCLICK, id, fn)
#define EVT_ADVTABLE_CMD_HDRCELL_RIGHT_DCLICK(id, fn) wx__DECLARE_ADVHDRCELLEVT(HDRCELL_RIGHT_DCLICK, id, fn)
#define EVT_ADVTABLE_CMD_HDRCELL_MOVE(id, fn) wx__DECLARE_ADVHDRCELLEVT(HDRCELL_MOVE, id, fn)
#define EVT_ADVTABLE_CMD_HDRCELL_SIZE(id, fn) wx__DECLARE_ADVHDRCELLEVT(HDRCELL_SIZE, id, fn)
#define EVT_ADVTABLE_CMD_HDRCELL_SORT(id, fn) wx__DECLARE_ADVHDRCELLEVT(HDRCELL_SORT, id, fn)

#define EVT_ADVTABLE_HDRCELL_LEFT_CLICK(fn) EVT_ADVTABLE_CMD_HDRCELL_LEFT_CLICK(wxID_ANY, fn)
#define EVT_ADVTABLE_HDRCELL_RIGHT_CLICK(fn) EVT_ADVTABLE_CMD_HDRCELL_RIGHT_CLICK(wxID_ANY, fn)
#define EVT_ADVTABLE_HDRCELL_LEFT_DCLICK(fn) EVT_ADVTABLE_CMD_HDRCELL_LEFT_DCLICK(wxID_ANY, fn)
#define EVT_ADVTABLE_HDRCELL_RIGHT_DCLICK(fn) EVT_ADVTABLE_CMD_HDRCELL_RIGHT_DCLICK(wxID_ANY, fn)
#define EVT_ADVTABLE_HDRCELL_MOVE(fn) EVT_ADVTABLE_CMD_HDRCELL_MOVE(wxID_ANY, fn)
#define EVT_ADVTABLE_HDRCELL_SIZE(fn) EVT_ADVTABLE_CMD_HDRCELL_SIZE(wxID_ANY, fn)
#define EVT_ADVTABLE_HDRCELL_SORT(fn) EVT_ADVTABLE_CMD_HDRCELL_SORT(wxID_ANY, fn)

/**
 * Table cell coordinate.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvCoord
{
public:
	wxAdvCoord()
	{
		Unset();
	}

	virtual ~wxAdvCoord()
	{
	}

	/**
	 * Sets coordinate.
	 * @param row row index
	 * @param col column index
	 */
	void Set(size_t row, size_t col)
	{
		m_row = row;
		m_col = col;
	}

	/**
	 * Sets this coordinate equal to other coordinate.
	 * @param o other coordinate
	 */
	void operator = (const wxAdvCoord &o)
	{
		m_row = o.m_row;
		m_col = o.m_col;
	}

	/**
	 * Checks whether coordinate is set.
	 * @return true if coordinate is set
	 */
	bool IsSet()
	{
		return (m_row != UNDEF_SIZE) && (m_col != UNDEF_SIZE);
	}

	/**
	 * Unset coordinate.
	 */
	void Unset()
	{
		m_row = UNDEF_SIZE;
		m_col = UNDEF_SIZE;
	}

	/**
	 * Checks coordinate equality.
	 * @param row row index
	 * @param col column index
	 * @return true if coordinate equal to row and col parameters
	 */
	bool Equal(size_t row, size_t col)
	{
		return (m_row == row) && (m_col == col);
	}

	/**
	 * Returns row index.
	 * @return row index
	 */
	size_t Row()
	{
		return m_row;
	}

	/**
	 * Returns column index.
	 * @return column index
	 */
	size_t Col()
	{
		return m_col;
	}

private:
	size_t m_row, m_col;
};

/**
 * Represents cell range, from [row1;col1] to [row2;col2].
 */
class WXDLLIMPEXP_ADVTABLE wxAdvRange
{
public:
	wxAdvRange()
	{
		Unset();
	}

	/**
	 * Constructs new range.
	 * @param row1 first row index
	 * @param col1 first column index
	 * @param row2 second row index
	 * @param col2 second column index
	 */
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

	/**
	 * Sets range.
	 * @param row1 first row index
	 * @param col1 first column index
	 * @param row2 second row index
	 * @param col2 second column index
	 */
	void Set(size_t row1, size_t col1, size_t row2, size_t col2)
	{
		m_row1 = row1;
		m_col1 = col1;
		m_row2 = row2;
		m_col2 = col2;
	}

	/**
	 * Unset range coordinates.
	 */
	void Unset()
	{
		Set(UNDEF_SIZE, UNDEF_SIZE, UNDEF_SIZE, UNDEF_SIZE);
	}

	/**
	 * Checks whether range coordinates is set
	 * @return true if range coordinates is set
	 */
	bool IsSet()
	{
		return (m_row1 != UNDEF_SIZE) && (m_col1 != UNDEF_SIZE) &&
				(m_row2 != UNDEF_SIZE) && (m_col2 != UNDEF_SIZE);
	}

	/**
	 * Determines if specified cell is in range.
	 * @param row row index
	 * @param col column index
	 * @return true if cell in range
	 */
	bool Contains(size_t row, size_t col)
	{
		return (row >= TopRow() && row <= BottomRow()) &&
				(col >= LeftCol() && col <= RightCol());
	}

	/**
	 * Determines if specified row is in range.
	 * @param row row index
	 * @return true if row in range
	 */
	bool ContainsRow(size_t row)
	{
		return (row >= TopRow() && row <= BottomRow());
	}

	/**
	 * Determines if specified column is in range.
	 * @param col column index
	 * @return true if column in range
	 */
	bool ContainsCol(size_t col)
	{
		return (col >= LeftCol() && col <= RightCol());
	}

	/**
	 * Checks whether range contains only one cell.
	 * @return true if range contains only one cell
	 */
	bool IsSingleCell()
	{
		return ((TopRow() == BottomRow()) &&
				(LeftCol() == RightCol()));
	}

	/**
	 * Checks whether this range is equals to other range.
	 * @param row1 first row index
	 * @param col1 first column index
	 * @param row2 second row index
	 * @param col2 second column index
	 * @return true if this range is equals to other range
	 */
	bool Equals(size_t row1, size_t col1, size_t row2, size_t col2)
	{
		return (m_row1 == row1) && (m_col1 == col1)
				&& (m_row2 == row2) && (m_col2 == col2);
	}

	/**
	 * Sets this range equal to other range.
	 * @param o other range
	 */
	void operator = (const wxAdvRange &o)
	{
		m_row1 = o.m_row1;
		m_col1 = o.m_col1;
		m_row2 = o.m_row2;
		m_col2 = o.m_col2;
	}

	size_t Row1()
	{
		return m_row1;
	}

	size_t Row2()
	{
		return m_row2;
	}

	size_t Col1()
	{
		return m_col1;
	}

	size_t Col2()
	{
		return m_col2;
	}

	/**
	 * Returns top row.
	 * @return top row
	 */
	size_t TopRow()
	{
		return min(m_row1, m_row2);
	}

	/**
	 * Returns bottom row.
	 * @return bottom row
	 */
	size_t BottomRow()
	{
		return max(m_row1, m_row2);
	}

	/**
	 * Returns left column.
	 * @return left column
	 */
	size_t LeftCol()
	{
		return min(m_col1, m_col2);
	}

	/**
	 * Returns right column.
	 * @return right column
	 */
	size_t RightCol()
	{
		return max(m_col1, m_col2);
	}

private:
	size_t min(size_t a, size_t b)
	{
		size_t m = wxMin(a, b);
		if (m == UNDEF_SIZE) {
			if (a != UNDEF_SIZE) {
				m = a;
			}
			else if (b != UNDEF_SIZE) {
				m = b;
			}
		}
		return m;
	}

	size_t max(size_t a, size_t b)
	{
		size_t m = wxMax(a, b);
		if (m == UNDEF_SIZE) {
			if (a != UNDEF_SIZE) {
				m = a;
			}
			else if (b != UNDEF_SIZE) {
				m = b;
			}
		}
		return m;
	}

	size_t m_row1, m_col1;
	size_t m_row2, m_col2;
};

/**
 * Cell attribute defines text alignment, font, color, etc.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvCellAttribute
{
public:
	wxAdvCellAttribute();
	virtual ~wxAdvCellAttribute();

	/**
	 * Sets text alignment.
	 * @param alignment text alignment
	 */
	void Alignment(int alignment)
	{
		m_alignment = alignment;
	}

	/**
	 * Returns text alignment.
	 * @return text alignment
	 */
	int Alignment()
	{
		return m_alignment;
	}

	/**
	 * Sets text font.
	 * @param font text font
	 */
	void Font(wxFont &font)
	{
		m_font = font;
	}

	/**
	 * Returns text font.
	 * @return text font
	 */
	const wxFont &Font()
	{
		return m_font;
	}

	/**
	 * Sets background brush.
	 * @param bgBrush background brush
	 */
	void BgBrush(wxBrush bgBrush)
	{
		m_bgBrush = bgBrush;
	}

	/**
	 * Returns background brush.
	 * @return background brush
	 */
	const wxBrush &BgBrush()
	{
		return m_bgBrush;
	}

	/**
	 * Returns text colour.
	 * @return text colour
	 */
	wxColour TextColour()
	{
		return m_textColour;
	}

	/**
	 * Sets text colour.
	 * @param textColour text colour
	 */
	void TextColour(wxColour textColour)
	{
		m_textColour = textColour;
	}

private:
	wxBrush m_bgBrush;
	wxFont m_font;

	int m_alignment;
	wxColour m_textColour;
};

/**
 * Base class for receiving wxAdvTableDataModel events.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvTableDataModelObserver
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

//
// Table data models.
//

/**
 * Model for wxAdvTable data, such as cell values, formats.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvTableDataModel : public Observable<wxAdvTableDataModelObserver>
{
public:
	wxAdvTableDataModel();
	virtual ~wxAdvTableDataModel();

	/**
	 * Returns cell value in string representation.
	 * @param row row index
	 * @param col column index
	 * @return cell value
	 */
	virtual wxString GetCellValue(size_t row, size_t col) = 0;

	/**
	 * Sets new cell value.
	 * @param row row index
	 * @param col column index
	 * @param cell value in string representation
	 * @return true if cell value is set
	 */
	virtual bool SetCellValue(size_t row, size_t col, wxString value) = 0;

	/**
	 * Returns cell value format.
	 * @param row row index
	 * @param col column index
	 * @return cell value format
	 */
	virtual int GetCellFormat(size_t row, size_t col) = 0;

	/**
	 * Check whether cell is editable.
	 * @param row row index
	 * @param col column index
	 * @return true if cell is editable
	 */
	virtual bool IsCellEditable(size_t row, size_t col) = 0;

	/**
	 * Returns cell attribute.
	 * Cell attribute is graphical properties such as
	 * font, text color, alignment, etc.
	 * @param row row index
	 * @param col column index
	 * @return true if cell is editable
	 */
	virtual wxAdvCellAttribute *GetCellAttribute(size_t row, size_t col) = 0;

	/**
	 * Returns cell value as double.
	 * NOTE: not all values can be accessed this way.
	 * @param row row index
	 * @param col column index
	 * @return cell value
	 */
	virtual double GetCellValueAsDouble(size_t row, size_t col);

	/**
	 * Sets cell value as date/time value.
	 * @param row row index
	 * @param col column index
	 * @param dt date cell value
	 */
	void SetCellValueAsDateTime(size_t row, size_t col, wxDateTime &dt);

	/**
	 * Returns cell value as wxDateTime.
	 * NOTE: not all values can be accessed this way.
	 * @param row row index
	 * @param col column index
	 * @return cell value
	 */
	virtual wxDateTime GetCellValueAsDateTime(size_t row, size_t col);

	/**
	 * Parse choices string representation, and split it to choices elements.
	 * @param value choices string representation
	 * @param selection output for current selected choice
	 * @param choices output for choices elements
	 */
	static void StringToChoices(const wxString &value, wxString &selection, wxArrayString &choices);

	/**
	 * Encode choices to string representation.
	 * @param selection current selected choice
	 * @param choices choices elements
	 * @return choices string representation
	 */
	static wxString ChoicesToString(const wxString &selection, wxArrayString &choices);

	/**
	 * Returns current selected choice.
	 * @param value choices string representation
	 * @return current selected choice
	 */
	static wxString ChoicesSelection(const wxString &value);

protected:
	FIRE_WITH_VALUE2(CellChanged, size_t, row, size_t, col);
	FIRE_VOID(TableChanged);
};

/**
 * Default table model for wxAdvTable.
 * Stores data as strings, all cells have string format by default,
 * and can be set by calling SetCellFormat.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvDefaultTableDataModel : public wxAdvTableDataModel
{
public:
	/**
	 * Constructs new wxAdvDefaultTableDataModel.
	 * @param numRows number of rows
	 * @param numCols number of column
	 * @param readOnly if true - all cell will read-only,
	 * 				   false - all cells editable
	 */
	wxAdvDefaultTableDataModel(size_t numRows, size_t numCols, bool readOnly);
	virtual ~wxAdvDefaultTableDataModel();

	//
	// wxAdvTableDataModel
	//
	virtual wxString GetCellValue(size_t row, size_t col);

	virtual bool SetCellValue(size_t row, size_t col, wxString value);

	virtual int GetCellFormat(size_t row, size_t col);

	virtual void SetCellFormat(size_t row, size_t col, int format);

	virtual bool IsCellEditable(size_t row, size_t col);

	virtual wxAdvCellAttribute *GetCellAttribute(size_t row, size_t col);

	virtual double GetCellValueAsDouble(size_t row, size_t col);

	virtual wxDateTime GetCellValueAsDateTime(size_t row, size_t col);

	/**
	 * Set cell format for entire row.
	 * @param row row index
	 * @param format format for row
	 */
	void SetRowFormat(size_t row, int format);

	/**
	 * Set cell format for entire column.
	 * @param col column index
	 * @param format format for column
	 */
	void SetColFormat(size_t col, int format);

	/**
	 * Insert new rows to specified position.
	 * @param before row index before which to insert new rows
	 * @param count row count to insert
	 * @param format format for new rows
	 */
	void InsertRows(size_t before, size_t count, int format = wxStringFormat);

	/**
	 * Insert new columns to specified position.
	 * @param before column index before which to insert new columns
	 * @param count column count to insert
	 * @param format format for new columns
	 */
	void InsertCols(size_t before, size_t count, int format = wxStringFormat);

	/**
	 * Remove specified rows.
	 * @param first first row index to remove
	 * @param count row count to remove
	 */
	void RemoveRows(size_t first, size_t count);

	/**
	 * Remove specified columns.
	 * @param first first column index to remove
	 * @param count column count to remove
	 */
	void RemoveCols(size_t first, size_t count);

private:
	bool m_readOnly;

	wxArrayArrayString m_data;

	wxIntIntArray m_formats;
};

/**
 * Filter table model.
 * Stores data in double array of strings, and updates it when underlaying model
 * data has been changed.
 * This type of model can be used as cache, for example to cache data accessed from
 * database, or another storage with long access time.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvFilterTableDataModel : public wxAdvTableDataModel, public wxAdvTableDataModelObserver
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

	//
	// wxAdvTableDataModel
	//
	virtual wxString GetCellValue(size_t row, size_t col);

	virtual bool SetCellValue(size_t row, size_t col, wxString value);

	virtual int GetCellFormat(size_t row, size_t col);

	virtual bool IsCellEditable(size_t row, size_t col);

	virtual wxAdvCellAttribute *GetCellAttribute(size_t row, size_t col);

	virtual double GetCellValueAsDouble(size_t row, size_t col);

	virtual wxDateTime GetCellValueAsDateTime(size_t row, size_t col);

	//
	// wxAdvTableDataModelObserver
	//
	virtual void CellChanged(size_t row, size_t col);

	virtual void TableChanged();

	/**
	 * Returns underlaying table model.
	 * @return underlaying table model
	 */
	wxAdvTableDataModel *GetUnderlayingModel();

private:
	void UpdateValues();

	wxAdvTableDataModel *m_underlayingModel;

	wxArrayArrayString m_data;
	bool m_needUpdate;
};


//
// Table data renderers.
//

/**
 * Draws header cell content.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvHdrCellRenderer
{
public:
	wxAdvHdrCellRenderer();
	virtual ~wxAdvHdrCellRenderer();

	/**
	 * Called to draw header cell.
	 * @param table wxAdvTable
	 * @param dc device context
	 * @param hdrCell header cell to draw
	 * @param selected true if header cell is selected
	 * @param pressed true if header cell is pressed
	 * @param sortDirection sort direction
	 */
	virtual void Draw(wxAdvTable *table, wxDC &dc, wxAdvHdrCell *hdrCell, bool selected, bool pressed, int sortDirection) = 0;

	/**
	 * Returns copy of this renderer.
	 */
	virtual wxAdvHdrCellRenderer *Clone() = 0;
};

/**
 * Standard header cell renderer.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvDefaultHdrCellRenderer : public wxAdvHdrCellRenderer
{
public:
	wxAdvDefaultHdrCellRenderer();
	virtual ~wxAdvDefaultHdrCellRenderer();

	virtual void Draw(wxAdvTable *table, wxDC &dc, wxAdvHdrCell *hdrCell, bool selected, bool pressed, int sortDirection);

	virtual wxAdvHdrCellRenderer *Clone();

	/**
	 * Sets whether to draw gradient header cells.
	 * @param drawGradient true to draw gradient header cells
	 */
	void SetDrawGradient(bool drawGradient)
	{
		m_drawGradient = drawGradient;
	}

protected:
	virtual void DrawBackground(wxDC &dc, wxAdvHdrCell *hdrCell, bool selected, bool pressed);

	int m_edgeWidth;
	bool m_drawGradient;
};

/**
 * Draws cell content.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvCellRenderer
{
public:
	wxAdvCellRenderer();
	virtual ~wxAdvCellRenderer();

	/**
	 * Called to draw header cell.
	 * @param table wxAdvTable
	 * @param dc device context
	 * @param rc cell rectangle
	 * @param value cell value
	 * @param selected true if cell is selected
	 * @param pressed true if cell is pressed
	 * @param attr cell attribute
	 */
	virtual void Draw(wxAdvTable *table, wxDC &dc, wxRect rc, wxString value, bool selected, bool focused, wxAdvCellAttribute *attr) = 0;

	//virtual wxSize GetExtent(wxAdvTable *table, wxDC &dc, wxRect rc, wxString value, bool selected, bool focused, wxAdvCellAttribute *attr) = 0;
};

/**
 * Draws cell content as text.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvStringCellRenderer : public wxAdvCellRenderer
{
public:
	wxAdvStringCellRenderer();
	virtual ~wxAdvStringCellRenderer();

	virtual void Draw(wxAdvTable *table, wxDC &dc, wxRect rc, wxString value, bool selected, bool focused, wxAdvCellAttribute *attr);
};

/**
 * Renderer to draw boolean cell values (draws checkmarks).
 */
class WXDLLIMPEXP_ADVTABLE wxAdvBoolCellRenderer : public wxAdvCellRenderer
{
public:
	wxAdvBoolCellRenderer();
	virtual ~wxAdvBoolCellRenderer();

	virtual void Draw(wxAdvTable *table, wxDC &dc, wxRect rc, wxString value, bool selected, bool focused, wxAdvCellAttribute *attr);
};

/**
 * Renderer to draw color cell values (draws colored rectangles).
 */
class WXDLLIMPEXP_ADVTABLE wxAdvColourCellRenderer : public wxAdvCellRenderer
{
public:
	wxAdvColourCellRenderer();
	virtual ~wxAdvColourCellRenderer();

	virtual void Draw(wxAdvTable *table, wxDC &dc, wxRect rc, wxString value, bool selected, bool focused, wxAdvCellAttribute *attr);
};

/**
 * Draws datetime cell content.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvDateTimeCellRenderer : public wxAdvStringCellRenderer
{
public:
	wxAdvDateTimeCellRenderer(wxChar *format = wxT("%x"));
	virtual ~wxAdvDateTimeCellRenderer();

	virtual void Draw(wxAdvTable *table, wxDC &dc, wxRect rc, wxString value, bool selected, bool focused, wxAdvCellAttribute *attr);

	void SetFormat(const wxString &format);

private:
	wxString m_format;
};

/**
 * Renderer for choices cell format.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvChoicesCellRenderer : public wxAdvStringCellRenderer
{
public:
	wxAdvChoicesCellRenderer();
	virtual ~wxAdvChoicesCellRenderer();

	virtual void Draw(wxAdvTable *table, wxDC &dc, wxRect rc, wxString value, bool selected, bool focused, wxAdvCellAttribute *attr);
};

//
// Table data editors.
//

/**
 * Table cell editors base class.
 * Editor is an object, that perform cell values editing,
 * it activated from wxAdvTable on mouse click or keypress,
 * and deactivated when user wants to edit another cell or
 * when wxAdvTable losses keyboard focus.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvCellEditor
{
public:
	wxAdvCellEditor(wxAdvTable *table);
	virtual ~wxAdvCellEditor();

	/**
	 * Return true, if editor activated with one click (such as boolean cells editor).
	 */
	virtual bool OneClickActivate();

	/**
	 * Called to activate cell editor.
	 * @param row row index
	 * @param col column index
	 */
	void Activate(size_t row, size_t col);

	/**
	 * Called to deactivate cell editor.
	 * @param table wxAdvTable
	 */
	void Deactivate();

	/**
	 * Checks whether editor is active.
	 * @return true if editor is active
	 */
	bool IsActive();

	/**
	 * Called to handle key event, that triggers editor activation.
	 * Called by wxAdvTable if cell editing activated by key press.
	 */
	virtual void HandleKeyEvent(wxKeyEvent &ev);

protected:

	virtual void DoActivate() = 0;
	virtual void DoDeactivate() = 0;

	wxAdvTable *GetTable();

	size_t GetRow();
	size_t GetCol();

	wxRect GetCellRect();

	wxString GetValue();
	void SetNewValue(wxString newValue);

	/**
	 * Call this when editor initiate self deactivation.
	 */
	void EndEditing();

private:
	wxAdvTable *m_table;
	wxAdvCoord m_cell;

	bool m_active;
	bool m_changed;
};

/**
 * Editor for string cell values. Uses wxTextCtrl for text input.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvStringCellEditor : public wxEvtHandler, public wxAdvCellEditor
{
public:
	wxAdvStringCellEditor(wxAdvTable *table);
	virtual ~wxAdvStringCellEditor();

	virtual bool OneClickActivate();

	virtual void HandleKeyEvent(wxKeyEvent &ev);

protected:
	virtual void DoActivate();
	virtual void DoDeactivate();

private:
	void OnTextEnter(wxCommandEvent &ev);
	void OnKillFocus(wxFocusEvent &ev);
	void OnTextKeydown(wxKeyEvent &ev);

	wxTextCtrl *m_textCtrl;
	bool m_escPressed;

	DECLARE_EVENT_TABLE()
};

/**
 * Editor for boolean cell values.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvBoolCellEditor : public wxAdvCellEditor
{
public:
	wxAdvBoolCellEditor(wxAdvTable *table);
	virtual ~wxAdvBoolCellEditor();

	virtual bool OneClickActivate();

protected:
	virtual void DoActivate();
	virtual void DoDeactivate();
};

/**
 * Editor for color cell values. Uses wxColourDialog for editing.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvColourCellEditor : public wxAdvCellEditor
{
public:
	wxAdvColourCellEditor(wxAdvTable *table);
	virtual ~wxAdvColourCellEditor();

	virtual bool OneClickActivate();

protected:
	virtual void DoActivate();
	virtual void DoDeactivate();
};

/**
 * Editor for interval numeric data. Uses wxSpinCtrl for editing.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvIntervalCellEditor : public wxAdvCellEditor
{
public:
	wxAdvIntervalCellEditor(wxAdvTable *table);
	virtual ~wxAdvIntervalCellEditor();

	virtual bool OneClickActivate();

	void SetRange(int minValue, int maxValue);

protected:
	virtual void DoActivate();
	virtual void DoDeactivate();

private:
	wxSpinCtrl *m_spinCtrl;
};

#if wxUSE_DATEPICKCTRL
/**
 * Editor for datetime data. Uses wxDateTimePicker for editing.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvDateTimeCellEditor : public wxAdvCellEditor, public wxEvtHandler
{
public:
	/**
	 * Constructs new datetime editor.
	 * @param table wxAdvTable for which editor to be created
	 * @param dropDown true if you want dropdown calendar, false - for spin
	 */
	wxAdvDateTimeCellEditor(wxAdvTable *table, bool dropDown = true);
	virtual ~wxAdvDateTimeCellEditor();

	virtual bool OneClickActivate();

protected:
	virtual void DoActivate();
	virtual void DoDeactivate();

private:
	void OnDateChanged(wxDateEvent &ev);

	wxDatePickerCtrl *m_datePicker;

	DECLARE_EVENT_TABLE()
};
#endif /* wxUSE_DATEPICKCTRL */

/**
 * Editor for choices data. Uses wxComboBox for editing.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvChoicesCellEditor : public wxAdvCellEditor, public wxEvtHandler
{
public:
	/**
	 * Constructs new choices editor.
	 * @param table wxAdvTable for which editor to be created
	 * @param editable true if you want to allow user to edit choice text
	 */
	wxAdvChoicesCellEditor(wxAdvTable *table, bool editable = false);
	virtual ~wxAdvChoicesCellEditor();

	virtual bool OneClickActivate();

protected:
	virtual void DoActivate();
	virtual void DoDeactivate();

private:
	void UpdateValue();

	void OnCombobox(wxCommandEvent &ev);
	void OnText(wxCommandEvent &ev);

	wxComboBox *m_comboBox;

	DECLARE_EVENT_TABLE()
};

//
// Sorters.
//

/**
 * Performs table data sorting
 */
class WXDLLIMPEXP_ADVTABLE wxAdvTableSorter
{
public:
	wxAdvTableSorter();
	virtual ~wxAdvTableSorter();

	virtual int Compare(wxAdvTable *table, size_t row1, size_t col1, size_t row2, size_t col2) = 0;
};

/**
 * Sorts data by comparing string values.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvTableStringSorter : public wxAdvTableSorter
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
class WXDLLIMPEXP_ADVTABLE wxAdvTableDoubleSorter : public wxAdvTableStringSorter
{
public:
	wxAdvTableDoubleSorter();
	virtual ~wxAdvTableDoubleSorter();

	virtual int Compare(wxAdvTable *table, size_t row1, size_t col1, size_t row2, size_t col2);

private:
	int Compare(double value1, double value2);
};

/**
 * Sorts data by comparing datetime values.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvTableDateTimeSorter : public wxAdvTableStringSorter
{
public:
	wxAdvTableDateTimeSorter();
	virtual ~wxAdvTableDateTimeSorter();

	virtual int Compare(wxAdvTable *table, size_t row1, size_t col1, size_t row2, size_t col2);

private:
	int Compare(const wxDateTime &value1, const wxDateTime &value2);
};


/**
 * Row/column description.
 * <br>
 * Contains attributes to draw and arrange header cell, such as:
 * <ol>
 * 	<li>cell label</li>
 *  <li>size (if not set it will be calculated automatically)</li>
 *  <li>label horizontal/vertical alignment</li>
 *  <li>spacing - distance between label and cell edges</li>
 *  <li>sortable or not</li>
 *  <li>horizontal or vertical label text</li>
 * </ol>
 * <br>
 * Header cell can be simple or composite.
 * Composite cells contains one or more subcells.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvHdrCell
{
	friend class wxAdvTable;
	friend class wxAdvDefaultHdrCellRenderer;

public:
	static const int defaultSpacing;
	static const int defaultAlignVertical;
	static const int defaultAlignHorizontal;

	/**
	 * Copy constructor.
	 */
	wxAdvHdrCell(const wxAdvHdrCell &o)
	{
		// copy attributes
		m_label = o.m_label;
		m_size  = o.m_size;
		m_alignVertical = o.m_alignVertical;
		m_alignHorizontal = o.m_alignHorizontal;
		m_spacing = o.m_spacing;
		m_verticalText = o.m_verticalText;
		m_sortable = o.m_sortable;
		m_resizeAllowed = o.m_resizeAllowed;

		m_minSize = o.m_minSize;
		m_maxSize = o.m_maxSize;

		if (o.m_renderer != NULL) {
			m_renderer = o.m_renderer->Clone();
		}
		else {
			m_renderer = NULL;
		}

		// copy internals
		m_rc = o.m_rc;
		m_decompIndex = o.m_decompIndex;
		m_isRow = o.m_isRow;
		m_parent = o.m_parent;
		m_subIndex = o.m_subIndex;

		m_table = o.m_table;

		// copy subcells
		m_subCells = o.m_subCells;
		ReparentSubCells();
	}

	/**
	 * Construct new header cell.
	 * @param label header cell label
	 */
	wxAdvHdrCell(const wxChar *label = wxT(""))
	{
		m_label = label;
		SetDefaults();
	}

	/**
	 * Construct new header cell.
	 * @param label header cell label
	 */
	wxAdvHdrCell(const wxString &label)
	{
		m_label = label;
		SetDefaults();
	}

	virtual ~wxAdvHdrCell()
	{
		wxDELETE(m_renderer);
	}

	/**
	 * Adds sub row/column.
	 * @param subHdrCell sub row/column
	 * @return reference to itself
	 */
	wxAdvHdrCell &Sub(wxAdvHdrCell subHdrCell)
	{
		subHdrCell.m_parent = this;
		subHdrCell.m_isRow = m_isRow;
		subHdrCell.m_subIndex = m_subCells.Count();
		m_subCells.Add(subHdrCell);
		return *this;
	}

	/**
	 * Adds sub row/column.
	 * @param label label for sub row/column
	 * @return reference to itself
	 */
	wxAdvHdrCell &Sub(const wxChar *label)
	{
		return Sub(wxAdvHdrCell(label));
	}

	/**
	 * Check if cell is composite.
	 * Composite cell contains one or more subcells.
	 * @return true if cell is composite
	 */
	bool IsComposite()
	{
		return m_subCells.Count() != 0;
	}

	/**
	 * Sets vertical text alignment for header cell.
	 * @param alignVertical vertical alignment, possible values are:
	 * 			wxALIGN_TOP, wxALIGN_CENTER_VERTICAL, wxALIGN_BOTTOM
	 * @return reference to itself
	 */
	wxAdvHdrCell &AlignVertical(int alignVertical)
	{
		m_alignVertical = alignVertical;
		return *this;
	}

	/**
	 * Returns vertical text alignment.
	 * @return vertical text alignment
	 */
	int AlignVertical()
	{
		return m_alignVertical;
	}

	/**
	 * Sets horizontal text alignment for header cell.
	 * @param alignHorizontal horizontal alignment, possible values are:
	 * 			wxALIGN_LEFT, wxALIGN_CENTER_HORIZONTAL, wxALIGN_RIGHT
	 * @return reference to itself
	 */
	wxAdvHdrCell &AlignHorizontal(int alignHorizontal)
	{
		m_alignHorizontal = alignHorizontal;
		return *this;
	}

	/**
	 * Returns horizontal text alignment.
	 * @return horizontal text alignment
	 */
	int AlignHorizontal()
	{
		return m_alignHorizontal;
	}

	/**
	 * Set whether to vertical text label.
	 * @param verticalText true if you want to label be vertical
	 * @return reference to itself
	 */
	wxAdvHdrCell &SetVerticalText(bool verticalText)
	{
		m_verticalText = verticalText;
		return *this;
	}

	/**
	 * Marks cell to draw label vertical.
	 * @return reference to itself
	 */
	wxAdvHdrCell &VerticalText()
	{
		return SetVerticalText(true);
	}

	/**
	 * Checks whether label text is vertical.
	 * @return true if label text is vertical
	 */
	bool IsVerticalText()
	{
		return m_verticalText;
	}

	/**
	 * Marks cell as sortable or not.
	 * @param sortable true if you want to cell be sortable, false - overwise
	 * @return reference to itself
	 */
	wxAdvHdrCell &SetSortable(bool sortable)
	{
		m_sortable = sortable;
		return *this;
	}

	/**
	 * Marks cell as sortable.
	 * @return reference to itself
	 */
	wxAdvHdrCell &Sortable()
	{
		return SetSortable(true);
	}

	/**
	 * Checks whether cell is sortable.
	 * @return true cell is sortable
	 */
	bool IsSortable()
	{
		return m_sortable;
	}

	/**
	 * Allow row/column resize by mouse.
	 * @return reference to itself
	 */
	wxAdvHdrCell &ResizeAllowed()
	{
		m_resizeAllowed = true;
		return *this;
	}

	/**
	 * Sets renderer to draw cell content.
	 * @param renderer new renderer
	 * @return reference to itself
	 */
	wxAdvHdrCell &Renderer(wxAdvHdrCellRenderer *renderer)
	{
		wxREPLACE(m_renderer, renderer);
		return *this;
	}

	/**
	 * Returns renderer to draw cell content.
	 * @return renderer
	 */
	wxAdvHdrCellRenderer *Renderer()
	{
		return m_renderer;
	}

	/**
	 * Sets distance between edges and cell content.
	 * @param spacing distance between edges and cell content.
	 * @return reference to itself
	 */
	wxAdvHdrCell &Spacing(wxCoord spacing)
	{
		m_spacing = spacing;
		return *this;
	}

	/**
	 * Returns spacing - distance between edges and cell content.
	 * @return spacing
	 */
	wxCoord Spacing()
	{
		return m_spacing;
	}

	/**
	 * Set header cell label.
	 * @param label label
	 * @return reference to itself
	 */
	wxAdvHdrCell &Label(const wxString &label)
	{
		m_label = label;
		return *this;
	}

	/**
	 * Returns header cell label.
	 * @return label
	 */
	const wxString &Label()
	{
		return m_label;
	}

	/**
	 * Sets size (width for column, height for row) for header cell.
	 * If size is not set, it will be calculated from label width/height
	 * @param size new size
	 * @return reference to itself
	 */
	wxAdvHdrCell &Size(wxCoord size)
	{
		m_size = size;
		return *this;
	}

	/**
	 * Returns size.
	 * @return size
	 */
	wxCoord Size()
	{
		return m_size;
	}

	/**
	 * Sets minimal size (width for column, height for row) for header cell.
	 * @param minSize new minimal size
	 * @return reference to itself
	 */
	wxAdvHdrCell &MinSize(wxCoord minSize)
	{
		m_minSize = minSize;
		return *this;
	}

	/**
	 * Returns minimal size.
	 * @return minimal size
	 */
	wxCoord MinSize()
	{
		return m_minSize;
	}

	/**
	 * Sets maximal size (width for column, height for row) for header cell.
	 * @param maxSize new maximal size
	 * @return reference to itself
	 */
	wxAdvHdrCell &MaxSize(wxCoord maxSize)
	{
		m_maxSize = maxSize;
		return *this;
	}

	/**
	 * Returns maximal size.
	 * @return maximal size
	 */
	wxCoord MaxSize()
	{
		return m_maxSize;
	}

	/**
	 * Checks whether cell is sub cell of another cell.
	 * @return true if this cell is sub cell of another cell
	 */
	bool IsSubcell()
	{
		return m_parent != NULL;
	}

	/**
	 * Returns subcell count.
	 * @return subcell count
	 */
	size_t GetSubCellCount()
	{
		return m_subCells.Count();
	}

	/**
	 * Returns subcell at index.
	 * @param index subcell index
	 * @return subcell
	 */
	wxAdvHdrCell *GetSubCell(size_t index)
	{
		return &m_subCells[index];
	}

	/**
	 * Returns size, needed for cell.
	 * @param dc device context
	 */
	wxSize CalcExtent(wxDC &dc)
	{
		wxSize extent = dc.GetMultiLineTextExtent(m_label);

		extent.IncBy(2 * m_spacing);
		//extent.IncBy(2 * m_spacing + sortArrowSize);

		if (m_verticalText) {
			// rotate by 90 degrees
			wxCoord tmp = extent.x;
			extent.x = extent.y;
			extent.y = tmp;
		}

		if (m_size != 0) {
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
	 * Returns maximal sublayer count.
	 * 1 - for not composite header cell.
	 * @return sublayer count
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
	 * Returns hdrCell layer header cell count.
	 * e.g. real row/column count.
	 * @return
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
	 * Returns true, if hdrCell is row, false - if column.
	 * @return true if this hdrCell is row
	 */
	bool IsRow()
	{
		return m_isRow;
	}

	/**
	 * Returns decomposed index. If hdrCell is composite,
	 * it will return UNDEF_SIZE.
	 * @return decomposed index
	 */
	size_t Index()
	{
		return m_decompIndex;
	}

	bool IsRightEdge()
	{
		return (RightSibling() == NULL); // if we have right sibling - cell is right edge
	}

	wxAdvHdrCell *RightSibling();

	/**
	 * Returns summary hdrCell layer cell count.
	 * @param cells hdrCell array
	 * @param numCells number of elements in cells array
	 * @return summary hdrCell layer cell count
	 */
	static size_t GetDecompCellCount(wxAdvHdrCell *cells, size_t numCells);

private:
	//
	// Internal functions.
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

	void SetDefaults()
	{
		m_size = 0;
		m_alignVertical = defaultAlignVertical;
		m_alignHorizontal = defaultAlignHorizontal;
		m_spacing = defaultSpacing;
		m_verticalText = false;
		m_sortable = false;
		m_resizeAllowed = false;

		m_renderer = NULL;

		m_minSize = 0;
		m_maxSize = (wxCoord) UNDEF_SIZE;

		m_decompIndex = UNDEF_SIZE;

		m_parent = NULL;
		m_subIndex = UNDEF_SIZE;

		m_table = NULL;
	}

	void ReparentSubCells()
	{
		FOREACH_HDRCELL(n, m_subCells) {
			m_subCells[n].m_parent = this;

			m_subCells[n].ReparentSubCells();
		}
	}

	/**
	 * Remove sub cell at index.
	 * @param index sub cell index
	 */
	void RemoveSubCell(size_t index)
	{
		m_subCells.RemoveAt(index);
	}


	//
	// Variables.
	//
	wxAdvHdrCellArray m_subCells;

	wxString m_label;
	wxCoord m_size;
	wxCoord m_minSize;
	wxCoord m_maxSize;

	int m_alignVertical;
	int m_alignHorizontal;

	wxCoord m_spacing;

	bool m_verticalText;

	bool m_sortable;
	bool m_resizeAllowed;

	wxAdvHdrCellRenderer *m_renderer;

	wxAdvHdrCell *m_parent;
	size_t m_subIndex;

	//
	// used internally by wxAdvTable
	//
	// for wxAdvTable to store row/column position
	wxRect m_rc;
	size_t m_decompIndex;
	bool m_isRow;

	wxAdvTable *m_table;
};

/**
 * Main component class.
 * Advanced table control implementation.
 * <br>
 * Features:
 * <ol>
 *  <li>composite rows/columns</li>
 *  <li>sorting and filtering</li>
 *  <li>Clean MCV design.
 *   Data accessed through wxAdvTableModel, rendered by wxAdvCellRenderer, edited by wxAdvCellEditor
 *  <li>rows/columns dynamic add/remove support</li>
 *  <li>TODO: add printing support</li>
 *  <li>TODO: add filters support</li>
 * </ol>
 *
 *   NOTES:
 *	There are two types of cell coordinates: table and model.
 *	Table coordinate used everywhere in table, when you click on cell,
 *	when you access data through wxAdvTable::GetCellValue.
 *	Model coordinates used by model.
 */
class WXDLLIMPEXP_ADVTABLE wxAdvTable : public wxScrolledWindow, public wxAdvTableDataModelObserver
{
public:
	enum SortMode {
		SortRows, SortCols, SortDisabled,
	};

	enum SelectMode {
		SelectCell, SelectRows, SelectCols, SelectBlock,
	};

	enum HighlightMode {
		HighlightNone, HighlightRows, HighlightCols, HighlightBoth,
	};

	enum SortDirection {
		SortDirAscending, SortDirDescending, SortDirNoSorting
	};

	wxAdvTable(wxWindow *parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);
	virtual ~wxAdvTable();

	//
	// Table creation functions.
	//

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
	 * Create table and arrange rows and columns.
	 * Call this after you create wxAdvTable before using it.
	 * wxAdvTable takes ownership for table model
	 * @param rows rows array
	 * @param cols columns arrray
	 * @param cornerLabel string to use as corner
	 * @param model data model
	 */
	void Create(wxAdvHdrCellArray &rows, wxAdvHdrCellArray &cols, const wxString &cornerLabel, wxAdvTableDataModel *model);

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

	/**
	 * Destroy table, free data model.
	 */
	void DestroyTable();

	/**
	 * Recalculate entire table geometry and repaint it.
	 * Need to be called by user after change in hdrCell attributes.
	 */
	void UpdateGeometry();

	//
	// Dynamic row/column add/remove functions.
	//

	/**
	 * Add rows to table.
	 * @param rows rows array
	 * @param numRows number of rows in rows array
	 * @param before index to row, before which to insert row, UNDEF_SIZE to insert at the end
	 */
	void AddRows(wxAdvHdrCell *rows, size_t numRows, size_t before = UNDEF_SIZE);

	/**
	 * Add columns to table.
	 * @param cols columns array
	 * @param numCols number of columns in cols array
	 * @param before index to column, before which to insert column, UNDEF_SIZE to insert at the end
	 */
	void AddCols(wxAdvHdrCell *cols, size_t numCols, size_t before = UNDEF_SIZE);

	/**
	 * Remove rows from table.
	 * @param from first index to remove
	 * @param count rows count to remove
	 */
	void RemoveRows(size_t from, size_t count);

	/**
	 * Remove columns from table.
	 * @param from first index to remove
	 * @param count columns count to remove
	 */
	void RemoveCols(size_t from, size_t count);

	/**
	 * Remove header cells from table.
	 * @param from first index to remove
	 * @param count cells count to remove
	 * @param isRows true if you want to remove rows, false - columns
	 */
	void RemoveHdrCells(size_t from, size_t count, bool isRows);

	/**
	 * Remove header cell from table.
	 * @param hdrCell header cell to remove
	 */
	void RemoveHdrCell(wxAdvHdrCell *hdrCell);

	//
	// Show headers functions.
	//

	/**
	 * Sets headers (row, column, corner header) show params.
	 * @param showRows true if you wish to rows header to be shown
	 * @param showCols true if you wish to cols header to be shown
	 * @param showCorner true if you wish to corner header to be shown
	 */
	void SetShowHeaders(bool showRows, bool showCols, bool showCorner);

	/**
	 * Sets whether to show rows header.
	 * @param showRows true if you wish to rows header to be shown
	 */
	void SetShowRows(bool showRows)
	{
		SetShowHeaders(showRows, m_showCols, m_showCorner);
	}

	/**
	 * Returns true, if rows header is shown.
	 * @return true, if rows header is shown.
	 */
	bool GetShowRows()
	{
		return m_showRows;
	}

	/**
	 * Sets whether to show columns header.
	 * @param showCols true if you wish to columns header to be shown
	 */
	void SetShowCols(bool showCols)
	{
		SetShowHeaders(m_showRows, showCols, m_showCorner);
	}

	/**
	 * Returns true, if columns header is shown.
	 * @return true, if columns header is shown.
	 */
	bool GetShowCols()
	{
		return m_showCols;
	}

	/**
	 * Sets whether to show table corner.
	 * @param showCorner true if you wish to corner to be shown
	 */
	void SetShowCorner(bool showCorner)
	{
		SetShowHeaders(m_showRows, m_showCols, showCorner);
	}

	/**
	 * Returns true, if table corner is shown.
	 * @return true, if table corner is shown.
	 */
	bool GetShowCorner()
	{
		return m_showCorner;
	}

	//
	// Sorting functions.
	//

	/**
	 * Sets sorter to sort table data.
	 * wxAdvTable takes ownership for sorter object.
	 * @param sorter new sorter
	 */
	void SetSorter(wxAdvTableSorter *sorter);

	/**
	 * Returns sorter to sort table data.
	 * @return sorter to sort table data
	 */
	wxAdvTableSorter *GetSorter();

	/**
	 * Sets sorting mode (eg Rows to sort rows - vertical sort, or Cols to
	 * sort columns - horizontal sort).
	 * @param sortMode new sorting mode
	 */
	void SetSortMode(SortMode sortMode);

	/**
	 * Returns current sort mode.
	 * @return current sort mode
	 */
	SortMode GetSortMode();

	/**
	 * Sets row/column index for sorting.
	 * @param sortingIndex index for sorting
	 * @param invert if true and sortingIndex is same as before call,
	 * sorting direction will be inverted
	 */
	void SetSortingIndex(size_t sortingIndex, bool invert = true);

	/**
	 * Returns current row/column index for sorting.
	 * @return current row/column index for sorting
	 */
	size_t GetSortingIndex();

	/**
	 * Sets sort direction.
	 * @param sortDirection sort direction;
	 */
	void SetSortDirection(SortDirection sortDirection);

	/**
	 * Returns current sort direction.
	 * @return current sort direction
	 */
	SortDirection GetSortDirection();

	/**
	 * Checks whether table is sorted.
	 * @return true if table is sorted
	 */
	bool IsSorting();

	/**
	 * Sets whether to allow sorting by any row, not only
	 * marked as sortable.
	 * @param sortByAnyRow true if you want to sort by any row
	 */
	void SetAllowSortByAnyRow(bool sortByAnyRow);

	bool IsAllowSortByAnyRow();

	/**
	 * Sets whether to allow sorting by any column, not only
	 * marked as sortable.
	 * @param sortByAnyCol true if you want to sort by any column
	 */
	void SetAllowSortByAnyCol(bool sortByAnyCol);

	bool IsAllowSortByAnyCol();

	/**
	 * Checks whether table can be sorted by specified header cell.
	 * @param hdrCell header cell
	 * @return true if table can be sorted by specified header cell
	 */
	bool CanSortByHdrCell(wxAdvHdrCell *hdrCell);

	//
	// Highlight functions.
	//

	/**
	 * Sets new highlight mode.
	 * @param highlightMode new highlight mode
	 */
	void SetHighlightMode(HighlightMode highlightMode);

	/**
	 * Returns current highlight mode
	 * @return current highlight mode
	 */
	HighlightMode GetHighlightMode()
	{
		return m_highlightMode;
	}

	//
	// Row/column access functions.
	//

    /**
     * Returns row count.
     * @return row count
     */
	size_t GetRowCount()
	{
		return m_rows.GetCount();
	}

    /**
     * Returns decomposed row count.
     * @return decomposed row count
     */
	size_t GetDecompRowCount()
	{
		return m_decompRows.Count();
	}

    /**
     * Returns column count.
     * @return column count
     */
	size_t GetColCount()
	{
		return m_cols.GetCount();
	}

    /**
     * Returns decomposed column count.
     * @return decomposed column count
     */
	size_t GetDecompColCount()
	{
		return m_decompCols.Count();
	}

	/**
	 * Returns row at specified index.
	 * @param index row index
	 * @return row at specified index
	 */
	wxAdvHdrCell *GetRow(size_t index)
	{
		wxCHECK(index < m_rows.Count(), NULL);
		return &m_rows[index];
	}

	/**
	 * Returns decomposed row at specified index.
	 * @param index decomposed row index
	 * @return decomposed row at specified index
	 */
	wxAdvHdrCell *GetDecompRow(size_t index)
	{
		return m_decompRows[index];
	}

	/**
	 * Returns column at specified index.
	 * @param index column index
	 * @return column at specified index
	 */
	wxAdvHdrCell *GetCol(size_t index)
	{
		wxCHECK(index < m_cols.Count(), NULL);
		return &m_cols[index];
	}

	/**
	 * Returns decomposed column at specified index.
	 * @param index decomposed column index
	 * @return decomposed column at specified index
	 */
	wxAdvHdrCell *GetDecompCol(size_t index)
	{
		return m_decompCols[index];
	}

	/**
	 * Returns maximal row index.
	 * @return maximal row index
	 */
	size_t MaxRow()
	{
		size_t maxRow = GetDecompRowCount();
		if (maxRow == 0) {
			return 0;
		}
		return maxRow - 1;
	}

	/**
	 * Returns maximal column index.
	 * @return maximal column index
	 */
	size_t MaxCol()
	{
		size_t maxCol = GetDecompColCount();
		if (maxCol == 0) {
			return 0;
		}
		return maxCol - 1;
	}

	/**
	 * Returns table data model.
	 * @return table data model
	 */
	wxAdvTableDataModel *GetModel()
	{
		return m_model;
	}

	/**
	 * Returns editor to edit cell value.
	 * @param row cell row index
	 * @param col cell column index
	 * @return editor for cell
	 */
	wxAdvCellEditor *GetEditorForCell(size_t row, size_t col);

	/**
	 * Set cell editor for specified cell format.
	 * wxAdvTable takes ownership over editor.
	 * @param format cell format
	 * @param editor cell editor for format
	 */
	void SetEditorForFormat(int format, wxAdvCellEditor *editor);

	/**
	 * Stop editing: deactivate editor if active, and commit changes.
	 */
	void StopEditing();

	/**
	 * Returns renderer for cell content.
	 * @param row cell row index
	 * @param col cell column index
	 * @return renderer for cell content
	 */
	wxAdvCellRenderer *GetRendererForCell(size_t row, size_t col);

	/**
	 * Sets table cell renderer for specified cell format.
	 * wxAdvTable takes ownership over renderer.
	 * @param format cell format
	 * @param renderer renderer for cell format
	 */
	void SetRendererForFormat(int format, wxAdvCellRenderer *renderer);

	/**
	 * Sets default renderer to draw cell values.
	 * wxAdvTable takes ownership over renderer.
	 * @param renderer new default renderer
	 */
	void SetDefaultRenderer(wxAdvCellRenderer *renderer);

	/**
	 * Sets default renderer to draw cell values.
	 * wxAdvTable takes ownership over renderer.
	 * @param renderer new default renderer
	 */
	void SetDefaultHdrRenderer(wxAdvHdrCellRenderer *renderer);

	/**
	 * Returns table cell value.
	 * @param row cell row index
	 * @param col cell column index
	 */
	wxString GetCellValue(size_t row, size_t col);

	/**
	 * Called by editor to change cell value.
	 * row and column indices are in model coordinates.
	 * @param row cell row index
	 * @param col cell column index
	 * @param value new cell value
	 */
	void ChangeCellValue(size_t row, size_t col, wxString value);

	/**
	 * Checks whether cell is editable.
	 * @param row cell row index
	 * @param col cell column index
	 * @return true if cell is editable, false - overwise
	 */
	bool IsCellEditable(size_t row, size_t col);

	/**
	 * Transforms cell coordinate from table to model space.
	 * @param row table row index
	 * @param col table column index
	 * @param modelRow output model row index
	 * @param modelCol output model column index
	 */
	void ToModelCellCoord(size_t row, size_t col, size_t &modelRow, size_t &modelCol);

	/**
	 * Transforms cell coordinate from model to table space.
	 * @param row model row index
	 * @param col model column index
	 * @param tableRow output table row index
	 * @param tableCol output table column index
	 */
	void ToCellCoord(size_t row, size_t col, size_t &tableRow, size_t &tableCol);

	/**
	 * Returns table cell rectangle.
	 * @param row cell row index
	 * @param col cell column index
	 */
	wxRect GetCellRect(size_t row, size_t col);

	/**
	 * Returns table cell rectangle.
	 * @param rc reference to rectangle
	 * @param row cell row index
	 * @param col cell column index
	 * @return true if rc is set to table cell rectangle
	 */
	bool GetCellRect(wxRect &rc, size_t row, size_t col);

	/**
	 * Returns cell coordinates at point.
	 * @param pt point
	 * @param row output row index
	 * @param col output column index
	 * @return true if point is in cell, false if point don't intersect any cell
	 */
	bool GetCellAt(const wxPoint &pt, size_t &row, size_t &col);

	/**
	 * Returns header cell (row or column) at given point.
	 * @return header cell or NULL if not found
	 */
	wxAdvHdrCell *GetHdrCellAt(const wxPoint &pt);

	/**
	 * Checks if specified cell is selected.
	 * @param row cell row index
	 * @param col cell column index
	 */
	bool IsCellSelected(size_t row, size_t col)
	{
		return m_selected.Contains(row, col);
	}

	/**
	 * Checks if specified cell is highlighted.
	 * @param row cell row index
	 * @param col cell column index
	 */
	bool IsCellHighlighted(size_t row, size_t col);

	/**
	 * Sets whether is allowed to resize all rows,
	 * not only marked by wxAdvHdrCell::Resizable.
	 * @param resizeAllRowsAllowed true to make all rows resizable
	 */
	void SetResizeAllRowsAllowed(bool resizeAllRowsAllowed)
	{
		m_resizeAllRowsAllowed = resizeAllRowsAllowed;
	}

	/**
	 * Checks whether is allowed to resize all rows,
	 * not only marked by wxAdvHdrCell::Resizable.
	 * @return true if all rows are resizable
	 */
	bool IsResizeAllRowsAllowed()
	{
		return m_resizeAllRowsAllowed;
	}

	/**
	 * Sets whether is allowed to resize all columns,
	 * not only marked by wxAdvHdrCell::Resizable.
	 * @param resizeAllColsAllowed true to make all columns resizable
	 */
	void SetResizeAllColsAllowed(bool resizeAllColsAllowed)
	{
		m_resizeAllColsAllowed = resizeAllColsAllowed;
	}

	/**
	 * Checks whether is allowed to resize all columns,
	 * not only marked by wxAdvHdrCell::Resizable.
	 * @return true if all columns are resizable
	 */
	bool IsResizeAllColsAllowed()
	{
		return m_resizeAllColsAllowed;
	}

	//
	// Selection functions
	// They are don't trigger selection events
	//

	/**
	 * Sets selection range.
	 * @param range new selected range
	 */
	void SetSelection(wxAdvRange &range);

	/**
	 * Sets selection to single cell in SelectCell mode,
	 * row - in SelectRows mode, column - in SelectCols mode.
	 * @param row cell row index
	 * @param col cell column index
	 */
	void SetSelection(size_t row, size_t col);

	/**
	 * Sets selection range.
	 * @param row1 first row index
	 * @param col1 first column index
	 * @param row2 second row index
	 * @param col2 second column index
	 */
	void SetSelection(size_t row1, size_t col1, size_t row2, size_t col2);

	/**
	 * Returns cells range currently visible.
	 * @param range output visible range
	 * @return true if range filled is visible range, false - if nothing is visible
	 */
	bool GetVisibleRange(wxAdvRange &range);

	/**
	 * Returns selection range.
	 * @return selection range
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
	 * Returns current select mode.
	 * @return select mode
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
	 * @param row cell row index
	 * @param col cell column index
	 */
	void SetFocusedCell(size_t row, size_t col);

	/**
	 * Returns sum width of all columns.
	 * @return sum width of all columns
	 */
	wxCoord CalcTotalColsWidth()
	{
		return SumDimensions(m_decompCols, true);
	}

	/**
	 * Returns sum height of all rows.
	 * @return sum height of all rows
	 */
	wxCoord CalcTotalRowsHeight()
	{
		return SumDimensions(m_decompRows, false);
	}

	/**
	 * Returns sum width of all rows layers.
	 * @return sum width of all rows layers
	 */
	wxCoord CalcTotalRowLayersWidth()
	{
		return SumLayerSizes(m_rowLayerWidths);
	}

	/**
	 * Returns sum height of all column layers.
	 * @return sum height of all column layers
	 */
	wxCoord CalcTotalColLayersHeight()
	{
		return SumLayerSizes(m_colLayerHeights);
	}

	/**
	 * Scrolls table to make specified cell visible.
	 * @param row row index
	 * @param col column index
	 */
	void ScrollToCell(size_t row, size_t col);

	//
	// Areas calculation functions.
	//

	/**
	 * Returns table area rectangle.
	 * @return table area rectangle
	 */
	wxRect CalcTableRect();

	/**
	 * Returns rows area rectangle.
	 * @returns rows area rectangle
	 */
	wxRect CalcRowsAreaRect();

	/**
	 * Returns columns area rectangle.
	 * @returns columns area rectangle
	 */
	wxRect CalcColsAreaRect();

	/**
	 * Calculate row rectangle.
	 * @param row row index
	 * @param col column index
	 * @return row rectangle
	 */
	wxRect CalcRowRect(size_t row, size_t col);

	/**
	 * Calculate column rectangle.
	 * @param row row index
	 * @param col column index
	 * @return column rectangle
	 */
	wxRect CalcColRect(size_t row, size_t col);

	/**
	 * Returns rectangle of cells range.
	 * @param row1 first row index
	 * @param col1 first column index
	 * @param row2 second row index
	 * @param col2 second column index
	 * @return rectangle of cells range
	 */
	wxRect GetRangeRect(size_t row1, size_t col1, size_t row2, size_t col2);

	/**
	 * Returns rectangle of cells range.
	 * @param range cells range
	 * @return rectangle of cells range
	 */
	wxRect GetRangeRect(wxAdvRange &range);

	//
	// Graphical objects functions.
	//

	/**
	 * Sets brush to draw cells background.
	 * @param bgBrush new brush
	 */
	void SetBgBrush(const wxBrush &bgBrush);

	/**
	 * Returns cells background brush.
	 * @return cells background brush
	 */
	const wxBrush &GetBgBrush();

	/**
	 * Sets brush to draw selected cells background.
	 * @param selectedBgBrush new brush
	 */
	void SetSelectedBgBrush(const wxBrush &selectedBgBrush);

	/**
	 * Returns selected cells background brush.
	 * @return selected cells background brush
	 */
	const wxBrush &GetSelectedBgBrush();

	/**
	 * Sets pen to draw grid lines.
	 * @param gridPen new grid pen
	 */
	void SetGridPen(const wxPen &gridPen);

	/**
	 * Returns grid lines pen.
	 * @return grid lines pen
	 */
	const wxPen &GetGridPen();

	/**
	 * Sets pen to draw focused cell edges.
	 * @param focusedPen new grid pen
	 */
	void SetFocusedPen(const wxPen &focusedPen);

	/**
	 * Returns focused cell edges pen.
	 * @return focused cell edges pen
	 */
	const wxPen &GetFocusedPen();

	/**
	 * Sets brush to draw focused cells background.
	 * @param focusedBgBrush new brush
	 */
	void SetFocusedBgBrush(const wxBrush &focusedBgBrush);

	/**
	 * Returns focused cells background brush.
	 * @return focused cells background brush
	 */
	const wxBrush &GetFocusedBgBrush();

	/**
	 * Sets brush to draw highlighted cells background.
	 * @param highlightedBgBrush new brush
	 */
	void SetHighlightedBgBrush(const wxBrush &highlightedBgBrush);

	/**
	 * Returns highlighted cells background brush.
	 * @return highlighted cells background brush
	 */
	const wxBrush &GetHighlightedBgBrush();

	/**
	 * wxWindow override
	 * To turn off physical scrolling, for smooth scroll.
	 */
	virtual void ScrollWindow(int dx, int dy, const wxRect* rect);

	/**
	 * wxPanel override.
	 * This is focus change bugfix.
	 */
	virtual void SetFocus();


	//
	// wxAdvTableDataModelObserver
	//
	virtual void CellChanged(size_t row, size_t col);

	virtual void TableChanged();

private:
	//
	// Event handlers.
	//
	void OnPaint(wxPaintEvent &ev);
	void OnMouseEvents(wxMouseEvent &ev);
	void OnKeyDown(wxKeyEvent &ev);
	void OnChar(wxKeyEvent &ev);
	void OnKillFocus(wxFocusEvent &ev);
	void OnSize(wxSizeEvent &ev);
	void OnScrollWin(wxScrollWinEvent &ev);

	void HandleHdrCellMouseEvent(const wxMouseEvent &ev, wxAdvHdrCell *cell);
	void HandleCellMouseEvent(const wxMouseEvent &ev, size_t row, size_t col);

	bool IsHdrCellResizeEvent(const wxMouseEvent &ev, wxAdvHdrCell *cell);

	bool ResizeHdrCellSelf(wxAdvHdrCell *hdrCell, int d, int shift);

	void ResizeHdrCell(const wxMouseEvent &ev, wxAdvHdrCell *cell);

	//
	// Update selection, called from mouse and key event handlers.
	//
	void SelectCells(const wxMouseEvent &ev, size_t row1, size_t col1, size_t row2, size_t col2);
	void SelectCells(const wxKeyEvent &ev, size_t row1, size_t col1, size_t row2, size_t col2);
	void SelectCells(size_t row1, size_t col1, size_t row2, size_t col2, bool controlDown, bool shiftDown, bool altDown, bool metaDown);

	//
	// Editing internal functions.
	//
	void EditCell(wxAdvCellEditor *editor, size_t row, size_t col);

	void SetPressedHdrCell(wxAdvHdrCell *cell);
	void SetCurrentHdrCell(wxAdvHdrCell *cell);

	//
	// Drawing functions.
	//
	void DrawTable(wxDC &dc);
	void DrawCell(wxDC &dc, size_t row, size_t col);

	void DrawHdrCells(wxDC &dc, wxAdvHdrCellArray &hdrCells);
	void DrawHdrCell(wxDC &dc, wxAdvHdrCell *hdrCell);
	void DrawHdrCellSelf(wxDC &dc, wxAdvHdrCell *hdrCell);

	void DrawBackground(wxDC &dc, int width, int height);

	//
	// Redraw functions.
	//
	void ResizeBackBitmaps();

	void RedrawHdrCell(wxAdvHdrCell *cell);
	void RedrawRange(wxAdvRange *range);
	void RedrawRange(size_t row1, size_t col1, size_t row2, size_t col2);
	void RedrawHighlighted(wxAdvCoord *coord);
	void RedrawAll();

    void DecomposeHdrCells(wxAdvHdrCellArray &hdrCells, wxAdvHdrCellPtrArray &decompHdrCells);
	void DecomposeHdrCell(wxAdvHdrCell &hdrCell, wxAdvHdrCellPtrArray &decompHdrCells);

	//
	// Table geometry internal functions.
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

	void UpdateVirtualSize();

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

	void AddHdrCells(wxAdvHdrCellArray &arr, wxAdvHdrCell *hdrCells, size_t numCells, size_t before, bool isRows);

	size_t IncrRow(size_t row, int incr)
	{
		return (size_t) wxClip((int) row + incr, 0, (int) MaxRow());
	}

	size_t IncrCol(size_t col, int incr)
	{
		return (size_t) wxClip((int) col + incr, 0, (int) MaxCol());
	}

	/**
	 * Translate point from window to viewport coordinates.
	 * @param pt point in window coordinates
	 */
	wxPoint ToViewportPosition(const wxPoint &pt, bool transVert = true, bool transHoriz = true);

	//
	// Event functions.
	//

	bool SendEvent(const wxEventType type, size_t row, size_t col);
	bool SendEvent(const wxEventType type, size_t row, size_t col, const wxMouseEvent &ev);
	bool SendRangeEvent(const wxEventType type, size_t row1, size_t col1, size_t row2, size_t col2, const wxMouseEvent &ev);
	bool SendRangeEvent(const wxEventType type, size_t row1, size_t col1, size_t row2, size_t col2,
				bool controlDown, bool shiftDown, bool altDown,	bool metaDown);
	bool SendHdrCellEvent(const wxEventType type, wxAdvHdrCell *hdrCell, const wxMouseEvent &ev);
	bool SendHdrCellEvent(const wxEventType type, wxAdvHdrCell *hdrCell, wxCoord x, wxCoord y,
			bool controlDown, bool shiftDown, bool altDown,	bool metaDown);

	//
	// Variables.
	//

	bool m_tableCreated;

	bool m_showRows;
	bool m_showCols;
	bool m_showCorner;

	wxAdvCellAttribute m_defaultCellAttribute;
	wxAdvHdrCell m_defaultRow, m_defaultCol;

	// rows/columns definitions
	wxAdvHdrCellArray m_rows;
	wxAdvHdrCellArray m_cols;
	wxAdvHdrCell m_cornerCell;

	// decomposed rows/columns
	wxAdvHdrCellPtrArray m_decompRows;
	wxAdvHdrCellPtrArray m_decompCols;

	wxCoordArray m_rowLayerWidths;
	wxCoordArray m_colLayerHeights;

	// resize allow variables
	bool m_resizeAllRowsAllowed;
	bool m_resizeAllColsAllowed;

	// sorting variables
	wxAdvTableSorter *m_sorter;
	SortMode m_sortMode;
	wxIndexArray m_sortOrder;

	HighlightMode m_highlightMode;

	// graphic objects
	wxPen m_gridPen;
	wxPen m_focusedPen;
	wxBrush m_bgBrush;
	wxBrush m_selectedBgBrush;
	wxBrush m_focusedBgBrush;
	wxBrush m_highlightedBgBrush;

	// table data model
	wxAdvTableDataModel *m_model;

	// rendering/editing variables
	wxAdvHdrCellRenderer *m_defaultHdrRenderer;
	wxAdvCellRenderer *m_defaultRenderer;
	wxAdvCellRendererMap m_renderers;
	wxAdvCellEditorMap m_editors;

	// selection variables
	SelectMode m_selectMode;
	wxAdvRange m_selected;
	wxAdvCoord m_pressedCell;

	// sorting state variables
	size_t m_sortingIndex;
	SortDirection m_sortDirection;

	bool m_sortByAnyRow;
	bool m_sortByAnyCol;

	// focused cell coordinate
	wxAdvCoord m_focused;

	// active cell editor
	wxAdvCellEditor *m_activeEditor;

	wxAdvHdrCell *m_pressedHdrCell;
	wxAdvHdrCell *m_currentHdrCell;
	wxAdvHdrCell *m_resizingHdrCell; // hdrCell resizing this time

	// backbuffers for rows, columns header parts, and for table content
	wxBitmap m_backBitmap;
	wxBitmap m_backBitmapRows;
	wxBitmap m_backBitmapCols;

	DECLARE_EVENT_TABLE()
	DECLARE_CLASS(wxAdvTable)
	DECLARE_NO_COPY_CLASS(wxAdvTable)
};

#endif /*_WX_WXADVTABLE_H_*/
