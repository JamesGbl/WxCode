#ifndef TABLEELEMENTS_H
#define TABLEELEMENTS_H

#include "wx/report/textelements.h"

WX_DECLARE_OBJARRAY(wxPoint, wxArrayPoints);

/**
 * \brief Encapsulates the pointer to sources array for columns or rows. 
 */
class wxReportTableCellsSource
{
public:
	const void* m_pArrSource;
	int m_iArrSourceType;
	int m_iStartPos;
	int m_count;
	
	/**
	 * \brief Default constructor - creates empty table cells source.
	 * \return 
	 */
	wxReportTableCellsSource() {m_pArrSource = NULL; m_iArrSourceType = -1; m_iStartPos = 0; m_count = 0;}
	/**
	 * \brief Constructor - creates source with specified array pointer.
	 * \param pArraySource pointer to source array
	 * \param ArraySourceType type of the source array. Possible value:
	 * \li wxRP_WXARRAYSHORT: wxArrayShort array
	 * \li wxRP_WXARRAYINT: wxArrayInt array
	 * \li wxRP_WXARRAYLONG: wxArrayLong array
	 * \li wxRP_WXARRAYDOUBLE: wxArrayDouble array
	 * \li wxRP_WXARRAYSTRING: wxArrayString array
	 * \li wxRP_ARRAYSHORT: short array
	 * \li wxRP_ARRAYINT: int array
	 * \li wxRP_ARRAYLONG: long array
	 * \li wxRP_ARRAYDOUBLE: double array
	 * \param startPos
	 * \param count
	 * \return 
	 */
	wxReportTableCellsSource(const void* pArraySource, int ArraySourceType, int startPos, int count)
	{m_pArrSource = pArraySource; m_iArrSourceType = ArraySourceType; m_iStartPos = startPos; m_count = count;}
	/**
	 * \brief Returns the actual count of values in the array.
	 * \return count of values in the array
	 */
	int GetActualArrayCount();
	/**
	 * \brief Refresh the source.
	 */
	void Refresh();
};

WX_DECLARE_USER_EXPORTED_OBJARRAY(wxReportTableCellsSource, wxReportTableCellsSourcesArray, WXDLLIMPEXP_RP);

//////////////// wxReportTableCell class

/**
 * \brief Represents the single cell in the table.
 */
class WXDLLIMPEXP_RP wxReportTableCell : public wxReportTextValue
{
protected:
	double m_dWidth;
	double m_dHeight;
	int m_textAlign;
	
public:
	/**
	 * \brief Default constructor.
	 */
	wxReportTableCell();
	/**
	 * \brief Default destructor
	 */
	~wxReportTableCell() {;}
	/**
	 * \brief Sets the width of the cell.
	 * \param cellWidth width to set
	 */
	void SetWidth(double cellWidth);
	/**
	 * \brief Sets the height of the cell.
	 * \param cellHeight height to set
	 */
	void SetHeight(double cellHeight);
	/**
	 * \brief Sets align of the text in the cell.
	 * \param alignType Possible values:
	 * \li wxRP_LEFT: left align
	 * \li wxRP_RIGHT: right align
	 * \li ID_CENTER: center
	 * \li wxRP_JUSTIFY: justify do whole width
	 */
	void SetTextAlign(int alignType = wxRP_LEFTALIGN);
	/**
	 * \brief Returns the width of the cell.
	 * \param width of the cell
	 */
	const double& GetWidth();
	/**
	 * \brief Returns the height of the cell.
	 * \param height of the cell
	 */
	const double& GetHeight();
	/**
	 * \brief Returns align value of the text in the cell.
	 * \return Possible returned values:
	 * \li wxRP_LEFT: left align
	 * \li wxRP_RIGHT: right align
	 * \li wxRP_CENTER: center
	 * \li wxRP_JUSTIFY: justify do whole width
	 */
	int GetTextAlign();
	/**
	 * \brief Create XML node from the cell.
	 * \return XML node with cell
	 */
	wxXmlNode* CreateXmlNode(const wxReportTextStyle& cellsStyle);
	/**
	 * \brief Retrieve the cell content from the specified XML node.
	 * \param node XML node to retrieve
	 * \return TRUE on succes, FALSE if false
	 */
	bool RetrieveFromXmlNode(const wxXmlNode* node, const wxReportTextStyle& cellsStyle);
	
	friend class wxReportTableItem;
};

WX_DECLARE_USER_EXPORTED_OBJARRAY(wxReportTableCell*, CellsArray, WXDLLIMPEXP_RP);
WX_DECLARE_USER_EXPORTED_OBJARRAY(CellsArray*, ArrayOfCellsArrays, WXDLLIMPEXP_RP);

//////////////// wxReportTableItem class

/**
 * \brief Represents the table page item. Encapsulates the signle cell objects.
 */
class WXDLLIMPEXP_RP wxReportTableItem : public wxReportPositionedItem
{
protected:
	int m_iColumnsWidth, m_iRowsHeight;
	int m_nRows;
	int m_nColumns;
	int m_textAlign;
	double m_dPageHeight;
	wxArrayPoints m_arIndividualRowsHeights;
	wxReportTableStyle m_style;
	wxReportTextStyle m_cellsStyle;
	ArrayOfCellsArrays m_cells;
	CellsArray m_headersRow;
	CellsArray m_headersColumn;
	wxReportTableCellsSourcesArray m_columnsSources;
	wxReportTableCellsSourcesArray m_rowsSources;
	wxArrayDouble m_rowsHeights;
	wxArrayDouble m_columnsWidths;
	int m_iActualRow;
	int m_iActualColumn;
	bool m_fContinue; // if the table is follow-to to table on previous page
	bool m_fHasNext; // if the table continues on the table on next page
	bool m_fDelete;
	//double m_dEndPosY;
	
	/**
	 * \brief Add new column and fill them with values from array T.
	 * \param array array with values to fill
	 * \param header header of the column
	 * \param position position to insert
	 * \param isVariable if TRUE than values in the table will be dynamically changed according to their changes in the source array
	 */
	template<class T> void DoAddColumn(const T& array, const wxString& header, int position, bool isVariable);
	/**
	 * \brief Add new row and fill them with values from array T.
	 * \param array pointer array with values to fill
	 * \param count count of values in the array
	 * \param header header of the row
	 * \param position position to insert
	 * \param isVariable if TRUE than values in the table will be dynamically changed according to their changes in the source array
	 */
	template<class T> void DoAddRow(const T& array, const wxString& header, int position, bool isVariable);
	/**
	 * \brief Add new cell to the active column.
	 * \param value source object to insert
	 * \param position position to insert
	 * \param isVariable if TRUE than the value in the cell will be dynamically changed according to changes in the source object
	 */
	template<class T> bool DoAddCellToColumn(const T& value, int position, bool isVariable);
	/**
	 * \brief Add new cell to the active row.
	 * \param value source object to insert
	 * \param position position to insert
	 * \param isVariable if TRUE than the value in the cell will be dynamically changed according to changes in the source object
	 */
	template<class T> void DoAddCellToRow(const T& value, int position, bool isVariable);
	/**
	 * \brief Set the value for the cell.
	 * \param value source object to insert
	 * \param row index of the row
	 * \param column index of the column
	 * \param isVariable if TRUE than the value in the cell will be dynamically changed according to changes in the source object
	 */
	template<class T> void DoSetCellValue(const T& value, int row, int column, bool isVariable);
	/**
	 * \brief Sets the source record for specified row or column.
	 * \param array source array
	 * \param arrayId type of the array
	 * \param count count of values in the array
	 * \param toRow TRUE for set record for row, FALSE to set record for column
	 * \param index index of row or column to set
	 */
	void DoSetSourceRecord(const void* array, int arrayId, int count, bool toRow, int index);
	/**
	 * \brief Connect specified column with source array.
	 * \param columnIndex index of the column
	 * \param array pointer to the source array
	 * \param start starting index in the array, tells from which position in the source will this table be filled
	 * \return 
	 */
	template<class T> bool DoSetVariableForColumn(int columnIndex, const T& array, int start);
	/**
	 * \brief Connect specified row with source array.
	 * \param rowIndex index of the row
	 * \param array pointer to the source array
	 * \param start starting index in the array, tells from which position in the source array will this table be filled
	 * \param cellsCount total number of values in the source array
	 * \return 
	 */
	template<class T> bool DoSetVariableForRow(int rowIndex, const T& array);
	/**
	 * \brief Connect specified cell with source object.
	 * \param rowIndex index of the row
	 * \param var source object
	 * \return 
	 */
	template<class T> bool DoSetVariableForCell(int rowIndex, const T& var);
	
	void CalculateHeight(wxReportTableCell *cell, int index, bool isHeader, double height = -1)
	{
		if(height < 0)
			cell->m_dHeight = 1.2 * cell->m_style.GetFontSize() * (25.4/72.); // calculate cell's height
		
		if((int)(this->m_rowsHeights.GetCount()) == 0 && !isHeader) // if no header in the empty row
			this->m_rowsHeights.Add(0);
		
		if((int)(this->m_rowsHeights.GetCount()) <= index)
			this->m_rowsHeights.Add(cell->m_dHeight); // inser at the end
		else
		{
			if(cell->m_dHeight > this->m_rowsHeights[index])
				this->m_rowsHeights[index] = cell->m_dHeight;
			else
				cell->m_dHeight = this->m_rowsHeights[index];
		}
	}
	
	inline void CalculateWidth(wxReportTableCell *cell, int index, bool isHeader, double width = -1)
	{
		wxScreenDC dc;
		int iw, ih;
		wxFont font(this->m_cellsStyle.GetFont());
		
		dc.GetTextExtent(cell->m_sValue, &iw, &ih, NULL, NULL, &font);
		double wi = 0;
		if(width < 0)
			wi = (25.4 + 6) * (iw / (double)(dc.GetPPI().x));
		else
			wi = width;
		
		if(this->m_columnsWidths.GetCount() == 0 && !isHeader)
			this->m_columnsWidths.Add(0);
		
		if((int)(this->m_columnsWidths.GetCount()) <= index)
		{
			this->m_columnsWidths.Add(wi);
			cell->m_dWidth = wi;
		}
		else
		{
			if(wi > this->m_columnsWidths[index])
			{
				cell->m_dWidth = wi;
				this->m_columnsWidths[index] = wi;
			}
			else
				cell->m_dWidth = this->m_columnsWidths[index];
		}
	}
	
	void DoCopy(const wxReportTableItem& table);	
	
public:
	/**
	 * \brief Default constructor.
	 */
	wxReportTableItem();
	/**
	 * \brief Constructor.
	 * \param name identification name of the item (must be set otherwise the table will not
	 * be able to spread over several pages if needed).
	 */
	wxReportTableItem(const wxString& name);
	/**
	 * \brief Default destructor.
	 */
	~wxReportTableItem() {;}
	wxReportTableItem& operator= (const wxReportTableItem& sourceTable);
	/**
	 * \brief Add new empty column.
	 * \param position index of added column, values < 0 -> column will be added at last place
	 */
	void AddColumn(int position = -1);
	/**
	 * \brief Add new column and fill them with values from specified array.
	 * \param array array with values to fill
	 * \param header header of the column
	 * \param position position to insert, values < 0 -> column will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddColumn(const wxArrayShort& array, const wxString& header = wxT(""), int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new column and fill them with values from specified array.
	 * \param array array with values to fill
	 * \param header header of the column
	 * \param position position to insert, values < 0 -> column will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddColumn(const wxArrayInt& array, const wxString& header = wxT(""), int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new column and fill them with values from specified array.
	 * \param array array with values to fill
	 * \param header header of the column
	 * \param position position to insert, values < 0 -> column will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddColumn(const wxArrayLong& array, const wxString& header = wxT(""), int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new column and fill them with values from specified array.
	 * \param array array with values to fill
	 * \param header header of the column
	 * \param position position to insert, values < 0 -> column will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddColumn(const wxArrayDouble& array, const wxString& header = wxT(""), int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new column and fill them with values from specified array.
	 * \param array array with values to fill
	 * \param header header of the column
	 * \param position position to insert, values < 0 -> column will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddColumn(const wxArrayString& array, const wxString& header = wxT(""), int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new empty row.
	 */
	void AddRow();
	/**
	 * \brief Add new row and fill them with values from the specified array.
	 * \param array array with values to fill
	 * \param header header of the row
	 * \param position position to insert, values < 0 -> row will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddRow(const wxArrayShort& array, const wxString& header = wxT(""), int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new row and fill them with values from the specified array.
	 * \param array array with values to fill
	 * \param header header of the row
	 * \param position position to insert, values < 0 -> row will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddRow(const wxArrayInt& array, const wxString& header = wxT(""), int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new row and fill them with values from the specified array.
	 * \param array array with values to fill
	 * \param header header of the row
	 * \param position position to insert, values < 0 -> row will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddRow(const wxArrayLong& array, const wxString& header = wxT(""), int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new row and fill them with values from the specified array.
	 * \param array array with values to fill
	 * \param header header of the row
	 * \param position position to insert, values < 0 -> row will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddRow(const wxArrayDouble& array, const wxString& header = wxT(""), int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new row and fill them with values from the specified array.
	 * \param array array with values to fill
	 * \param header header of the row
	 * \param position position to insert, values < 0 -> row will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddRow(const wxArrayString& array, const wxString& header = wxT(""), int position = wxRP_AT_LAST, bool isVariable = false);
	
	/*!
	 * \brief Add new empty cell.
	 */
	void AddCell();
	/**
	 * \brief Add new cell to the column and fill them with values from the source object.
	 * \param value source object or variable
	 * \param count count of the values in the array
	 * \param position position to insert, values < 0 -> cell will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddCellToColumn(const short& value, int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new cell to the column and fill them with values from the source object.
	 * \param value source object or variable
	 * \param count count of the values in the array
	 * \param position position to insert, values < 0 -> cell will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddCellToColumn(const int& value, int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new cell to the column and fill them with values from the source object.
	 * \param value source object or variable
	 * \param count count of the values in the array
	 * \param position position to insert, values < 0 -> cell will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddCellToColumn(const long& value, int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new cell to the column and fill them with values from the source object.
	 * \param value source object or variable
	 * \param count count of the values in the array
	 * \param position position to insert, values < 0 -> cell will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddCellToColumn(const float& value, int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new cell to the column and fill them with values from the source object.
	 * \param value source object or variable
	 * \param count count of the values in the array
	 * \param position position to insert, values < 0 -> cell will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddCellToColumn(const double& value, int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new cell to the column and fill them with values from the source object.
	 * \param value source object or variable
	 * \param count count of the values in the array
	 * \param position position to insert, values < 0 -> cell will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddCellToColumn(const wxString& value, int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new cell to the row and fill them with values from the source object.
	 * \param value source object or variable
	 * \param count count of the values in the array
	 * \param position position to insert, values < 0 -> cell will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddCellToRow(const short& value, int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new cell to the row and fill them with values from the source object.
	 * \param value source object or variable
	 * \param count count of the values in the array
	 * \param position position to insert, values < 0 -> cell will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddCellToRow(const int& value, int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new cell to the row and fill them with values from the source object.
	 * \param value source object or variable
	 * \param count count of the values in the array
	 * \param position position to insert, values < 0 -> cell will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddCellToRow(const long& value, int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new cell to the row and fill them with values from the source object.
	 * \param value source object or variable
	 * \param count count of the values in the array
	 * \param position position to insert, values < 0 -> cell will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddCellToRow(const float& value, int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new cell to the row and fill them with values from the source object.
	 * \param value source object or variable
	 * \param count count of the values in the array
	 * \param position position to insert, values < 0 -> cell will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddCellToRow(const double& value, int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Add new cell to the row and fill them with values from the source object.
	 * \param value source object or variable
	 * \param count count of the values in the array
	 * \param position position to insert, values < 0 -> cell will be added at last place
	 * \param isVariable if TRUE than the values in the table will be dynamically changed according to their changes in the source array
	 */
	void AddCellToRow(const wxString& value, int position = wxRP_AT_LAST, bool isVariable = false);
	/**
	 * \brief Set value for the specified cell.
	 * \param value source object to set
	 * \param row index of the row
	 * \param column index of the column
	 * \param isVariable if TRUE than the value in the cell will be dynamically changed according to changes in the source object
	 */
	void SetCellValue(const short& value, int row, int column, bool isVariable = false);
	/**
	 * \brief Set value for the specified cell.
	 * \param value source object to set
	 * \param row index of the row
	 * \param column index of the column
	 * \param isVariable if TRUE than the value in the cell will be dynamically changed according to changes in the source object
	 */
	void SetCellValue(const int& value, int row, int column, bool isVariable = false);
	/**
	 * \brief Set value for the specified cell.
	 * \param value source object to set
	 * \param row index of the row
	 * \param column index of the column
	 * \param isVariable if TRUE than the value in the cell will be dynamically changed according to changes in the source object
	 */
	void SetCellValue(const long& value, int row, int column, bool isVariable = false);
	/**
	 * \brief Set value for the specified cell.
	 * \param value source object to set
	 * \param row index of the row
	 * \param column index of the column
	 * \param isVariable if TRUE than the value in the cell will be dynamically changed according to changes in the source object
	 */
	void SetCellValue(const float& value, int row, int column, bool isVariable = false);
	/**
	 * \brief Set value for the specified cell.
	 * \param value source object to set
	 * \param row index of the row
	 * \param column index of the column
	 * \param isVariable if TRUE than the value in the cell will be dynamically changed according to changes in the source object
	 */
	void SetCellValue(const double& value, int row, int column, bool isVariable = false);
	/**
	 * \brief Set value for the specified cell.
	 * \param value source object to set
	 * \param row index of the row
	 * \param column index of the column
	 * \param isVariable if TRUE than the value in the cell will be dynamically changed according to changes in the source object
	 */
	void SetCellValue(const wxString& value, int row, int column, bool isVariable = false);
	/**
	 * \brief Connect specified column with the source array.
	 * \param columnIndex index of the column
	 * \param array source array
	 * \return TRUE on succes
	 */
	bool SetVariableForColumn(int columnIndex, const wxArrayShort& array);
	/**
	 * \brief Connect specified column with the source array.
	 * \param columnIndex index of the column
	 * \param array source array
	 * \return TRUE on succes
	 */
	bool SetVariableForColumn(int columnIndex, const wxArrayInt& array);
	/**
	 * \brief Connect specified column with the source array.
	 * \param columnIndex index of the column
	 * \param array source array
	 * \return TRUE on succes
	 */
	bool SetVariableForColumn(int columnIndex, const wxArrayLong& array);
	/**
	 * \brief Connect specified column with the source array.
	 * \param columnIndex index of the column
	 * \param array source array
	 * \return TRUE on succes
	 */
	bool SetVariableForColumn(int columnIndex, const wxArrayDouble& array);
	/**
	 * \brief Connect specified column with the source array.
	 * \param columnIndex index of the column
	 * \param array source array
	 * \return TRUE on succes
	 */
	bool SetVariableForColumn(int columnIndex, const wxArrayString& array);
	/**
	 * \brief Connect specified row with the source array.
	 * \param columnIndex index of the column
	 * \param array source array
	 * \return TRUE on succes
	 */
	bool SetVariableForRow(int rowIndex, const wxArrayShort& array);
	/**
	 * \brief Connect specified row with the source array.
	 * \param rowIndex index of the row
	 * \param array source array
	 * \return TRUE on succes
	 */
	bool SetVariableForRow(int rowIndex, const wxArrayInt& array);
	/**
	 * \brief Connect specified row with the source array.
	 * \param rowIndex index of the row
	 * \param array source array
	 * \return TRUE on succes
	 */
	bool SetVariableForRow(int rowIndex, const wxArrayLong& array);
	/**
	 * \brief Connect specified row with the source array.
	 * \param rowIndex index of the row
	 * \param array source array
	 * \return TRUE on succes
	 */
	bool SetVariableForRow(int rowIndex, const wxArrayDouble& array);
	/**
	 * \brief Connect specified row with the source array.
	 * \param rowIndex index of the row
	 * \param array source array
	 * \return TRUE on succes
	 */
	bool SetVariableForRow(int rowIndex, const wxArrayString& array);
	/**
	 * \brief Clear all cells in the specified column.
	 * \param position index of the column to clear
	 * \param withHeader if true than the header will also be cleaned
	 */
	void ClearColumn(int position, bool withHeader = false);
	/**
	 * \brief Clear all cells in the specified row.
	 * \param position index of the row to clear
	 * \param withHeader if true than the header will also be cleaned
	 */
	void ClearRow(int position, bool withHeader = false);
	/**
	 * \brief Clear the specified cell.
	 * \param row row index
	 * \param column column index
	 */
	void ClearCell(int row, int column);
	/**
	 * \brief Remove specified column from the table.
	 * \param position index of the column to remove
	 */
	void RemoveColumn(int position);
	/**
	 * \brief Remove specified row from the table.
	 * \param position index of the row to remove
	 */
	void RemoveRow(int position);
	/**
	 * \brief Remove all empty columns from the table.
	 */
	void RemoveEmptyColumns();
	/**
	 * \brief Remove all empty rows from the table.
	 */
	void RemoveEmptyRows();
	/**
	 * \brief Sets the header cell for the specified column.
	 * \param header text for the header
	 */
	void SetColumnHeader(const wxReportTableCell& header, int columnIndex);
	/**
	 * \brief Sets the header cell for the specified row.
	 * \param header text for the header
	 */
	void SetRowHeader(const wxReportTableCell& header, int rowIndex);
	/**
	 * \brief Sets the height of the specified row.
	 * \param height row height to set
	 */
	void SetRowHeight(int rowNumber, double height);
	/**
	 * \brief Returns the height of the specified row
	 * \param rowIndex index of the row
	 * \return height vaule of the row
	 */
	double GetRowHeight(int rowIndex) const;
	/**
	 * \brief Sets the width for the specified column.
	 * \param columnNumber index of the specified column
	 * \param width column width to set
	 */
	void SetColumnWidth(int columnNumber, double width);
	/**
	 * \brief Sets the width for all columns.
	 * \param width column width to set
	 */
	void SetColumnWidth(double width);
	/**
	 * \brief Returns the width of the specified column
	 * \param columnIndex index of the row
	 * \return height vaule of the row
	 */
	double GetColumnWidth(int columnIndex) const;
	/**
	 * \brief Synchronizes cells sizes.
	 */
	void SynchronizeCellsSizes();
	/**
	 * \brief Sets the style for the table.
	 * \param style table style to set
	 */
	void SetStyle(const wxReportTableStyle &style);
	/**
	 * \brief Set style for the cells in the table.
	 * \param style cell style to set
	 */
	void SetCellsStyle(const wxReportTextStyle &style);
	/**
	 * \brief Set style for the cells in the specified column.
	 * \param style cell style to set
	 * \param columnIndex index of the column
	 */
	void SetCellsStyleForColumn(const wxReportTextStyle &style, int columnIndex);
	/**
	 * \brief Set style for the cells in the specified row.
	 * \param style cell style to set
	 * \param rowIndex index of the row
	 */
	void SetCellsStyleForRow(const wxReportTextStyle &style, int rowIndex);
	/**
	 * \brief Set style for the specified cell.
	 * \param row index
	 * \param column index
	 * \param style cell style to set
	 */ 
	void SetCellStyle(int row, int column, const wxReportTextStyle &style);
	/**
	 * \brief Sets the style for the header of the row.
	 * \param style style to set
	 * \param rowIndex row index of row to set, values lesser than 0 means all rows
	 */
	void SetRowsHeadersStyle(const wxReportTextStyle& style, int rowIndex = -1);
	/**
	 * \brief Sets the style for the header of the column.
	 * \param style style to set
	 * \param columnIndex index of column to set, values lesser than 0 means all columns
	 */
	void SetColumnsHeadersStyle(const wxReportTextStyle& style, int columnIndex = -1);
	/**
	 * \brief Sets text align for the cells
	 * \param alignType type of the align
	 */
	void SetTextAlign(int alignType = wxRP_LEFTALIGN);
	/**
	 * \brief Returns reference to style object of the table
	 * \return reference to style object
	 */
	wxReportTableStyle& Style();
	/**
	 * \brief Returns reference to cells style object of the table
	 * \return reference to cells style object
	 */
	const wxReportTextStyle& GetCellsStyle() const;
	/**
	 * \brief Returns the constant reference to cells style object of the table
	 * \return reference to cells style object
	 */
	wxReportTextStyle& CellsStyle();
	/**
	 * \brief Returns the text string with the value form the specified cell.
	 * \param row row index
	 * \param column column index
	 * \return wxString with the value of the cell
	 */
	wxString GetCellValue(int row, int column);
	/**
	 * \brief Returns the constant pointer to the specified cell.
	 * \param row row index
	 * \param column column index
	 * \return constant pointer to the cell
	 */
	wxReportTableCell* GetCell(int row, int column) const;
	/**
	 * \brief Returns the count of columns in the table.
	 * \return number of columns
	 */
	int GetColumnsCount() const;
	/**
	 * \brief Returns the count of rows in the table.
	 * \return number of rows
	 */
	int GetRowsCount() const;
	/**
	 * \brief Tells if there is any header in the rows.
	 * \return TRUE if yes, FALSE if no headers
	 */
	bool HasRowsHeaders() const;
	/**
	 * \brief Tells if there is any header in the columns.
	 * \return TRUE if yes, FALSE if none headers
	 */
	bool HasColumnsHeaders() const;
	/**
	 * \brief Returns the total table height.
	 * \return height of the whole table
	 */
	double GetTableHeight() const;
	/**
	 * \brief Returns the total table width.
	 * \return width of the whole table
	 */
	double GetTableWidth() const;
	/**
	 * \brief Refresh cells values which are connected with source objects.
	 */
	void RefreshVariable();
	/**
	 * \brief Creates the XML node for the table.
	 * \return XML node with the table
	 */
	wxXmlNode* CreateXmlNode();
	/**
	 * \brief Retrieves the table form the given XML node
	 * \param node XML node to retrieve
	 * \return TRUE on succes, FALSE if fails
	 */
	bool RetrieveFromXmlNode(const wxXmlNode* node);
	/**
	 * \brief Draws the table to the given device context.
	 * \param dc device context to draw
	 * \param toScreen tells if the page will be drawn to the screen or to the printer
	 * \param pageStyle style of the table where the shape will be drawned
	 */
	void DrawToDC(wxDC *dc, bool toScreen, const wxReportPageStyle& pageStyle);
	
	FRIENDS
	friend class wxReportDocument;
};

#endif // TABLEELEMENTS_H
