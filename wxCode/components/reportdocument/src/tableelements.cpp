#include "wx/report/tableelements.h"
#include "wx/arrimpl.cpp"

WX_DEFINE_USER_EXPORTED_OBJARRAY(wxArrayPoints);
WX_DEFINE_USER_EXPORTED_OBJARRAY(CellsArray);
WX_DEFINE_USER_EXPORTED_OBJARRAY(ArrayOfCellsArrays);
WX_DEFINE_USER_EXPORTED_OBJARRAY(wxReportTableCellsSourcesArray);

int wxReportTableCellsSource::GetActualArrayCount()
{
	int count = 0;
	switch(this->m_iArrSourceType)
	{
		case wxRP_WXARRAYSHORT:
			count = ((wxArrayShort*)(this->m_pArrSource))->GetCount();
			break;
		
		case wxRP_WXARRAYINT:
			count = ((wxArrayInt*)(this->m_pArrSource))->GetCount();
			break;
		
		case wxRP_WXARRAYLONG:
			count = ((wxArrayLong*)(this->m_pArrSource))->GetCount();
			break;
			
		case wxRP_WXARRAYDOUBLE:
			count = ((wxArrayDouble*)(this->m_pArrSource))->GetCount();
			break;
			
		case wxRP_WXARRAYSTRING:
			count = ((wxArrayString*)(this->m_pArrSource))->GetCount();
			break;
			
		default:
			count = this->m_count;
	}
	return count;
}

void wxReportTableCellsSource::Refresh()
{
	this->m_count = this->GetActualArrayCount();
}

//////////////// wxReportTableCell class

wxReportTableCell::wxReportTableCell()
{
	this->m_dHeight = 0;
	this->m_dWidth = 0;
	this->m_textAlign = wxRP_LEFTALIGN;
	this->m_iType = wxRP_TABLECELL;
	this->m_sValue = wxT("");
}

void wxReportTableCell::SetWidth(double cellWidth)
{
	this->m_dWidth = cellWidth;
}

void wxReportTableCell::SetHeight(double cellHeight)
{
	this->m_dHeight = cellHeight;
}

void wxReportTableCell::SetTextAlign(int alignType)
{
	this->m_textAlign = alignType;
}

const double& wxReportTableCell::GetWidth()
{
	return this->m_dWidth;
}

const double& wxReportTableCell::GetHeight()
{
	return this->m_dHeight;
}

int wxReportTableCell::GetTextAlign()
{
	return this->m_textAlign;
}

wxXmlNode* wxReportTableCell::CreateXmlNode(const wxReportTextStyle& cellsStyle)
{
	wxXmlNode *mainNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("TableCell"));//this->m_Style.CreateXmlNode();
	wxXmlNode *valueNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Value"));
	wxXmlNode *valueNodeContent = new wxXmlNode();
	valueNodeContent->SetType(wxXML_TEXT_NODE);
	wxString content = wxT("");
	
	//if(!this->m_fVariable)
		content = this->m_sValue;	
	
	valueNodeContent->SetContent(content);
	valueNode->AddChild(valueNodeContent);
	mainNode->AddAttribute(wxT("Type"), wxString::Format(wxT("%d"), this->m_iType));
	mainNode->AddAttribute(wxT("Name"), this->m_sName);
	mainNode->AddAttribute(wxT("Variable"), wxString::Format(wxT("%d"), this->m_fVariable));
	mainNode->AddAttribute(wxT("Width"), wxString::Format(wxT("%G"), this->m_dWidth));
	mainNode->AddAttribute(wxT("Height"), wxString::Format(wxT("%G"), this->m_dHeight));
	mainNode->AddAttribute(wxT("Align"), wxString::Format(wxT("%d"), this->m_textAlign));
	
	if(!(cellsStyle == this->m_style))
		mainNode->AddProperty(this->m_style.CreateXmlNode()->GetProperties());
	
	mainNode->AddChild(valueNode);	
	
	return mainNode;
}

bool wxReportTableCell::RetrieveFromXmlNode(const wxXmlNode* node, const wxReportTextStyle& cellsStyle)
{
	try
	{
		int type = -1, variable = 0, align = wxRP_LEFTALIGN;
		double w = -1, h = -1;
		
		wxSscanf(node->GetAttribute(wxT("Type"), wxT("")), wxT("%d"), &type);
		if(type == wxRP_TABLECELL)
			this->m_iType = type;
		else
			return false;
		
		this->m_sName = node->GetAttribute(wxT("Name"), wxT(""));
		
		wxSscanf(node->GetAttribute(wxT("Variable"), wxT("")), wxT("%d"), &variable);
		this->m_fVariable = variable;
		
		wxSscanf(node->GetAttribute(wxT("Width"), wxT("")), wxT("%lf"), &w);
		wxSscanf(node->GetAttribute(wxT("Height"), wxT("")), wxT("%lf"), &h);
		if(w >= 0 && h >= 0)
		{
			this->m_dHeight = h;
			this->m_dWidth = w;
		}
		else
			return false;
			
		wxSscanf(node->GetAttribute(wxT("Align"), wxT("")), wxT("%d"), &align);
		this->m_textAlign = align;
		
		if(!this->m_style.RetrieveFromXmlNode(node))
			this->m_style = cellsStyle;
		
		/*if(this->m_fVariable)
			this->m_sValue = wxT("");
		else
		{*/
			wxXmlNode *valNode = node->GetChildren()->GetChildren();
			if(valNode)
				this->m_sValue = valNode->GetContent();
			else
				this->m_sValue = wxT("");
		//}
			
	}
	catch(...)
	{
		return false;
	}
	
	return true;
}

//////////////// wxReportTableItem class

wxReportTableItem::wxReportTableItem()
{
	// set default name here because noname tables cannot be spread over multiple pages
	// if needed
	static int counter = 0;
	this->m_sName = wxString::Format( wxT("TableItem_%d"), ++counter );
	
	this->m_iType = wxRP_TABLE;
	this->m_textAlign = wxRP_LEFTALIGN;
	this->m_nColumns = 0;
	this->m_nRows = 0;
	this->m_fContinue = false;
	this->m_fHasNext = false;
	this->m_fDelete = false;
}

wxReportTableItem::wxReportTableItem(const wxString& name)
{
	this->m_iType = wxRP_TABLE;
	this->m_textAlign = wxRP_LEFTALIGN;
	this->m_nColumns = 0;
	this->m_nRows = 0;
	this->m_sName = name;
	this->m_fContinue = false;
	this->m_fHasNext = false;
	this->m_fDelete = false;
}

wxReportTableItem& wxReportTableItem::operator= (const wxReportTableItem& sourceTable)
{
	if(this == &sourceTable)
		return *this;
		
	this->DoCopy(sourceTable);
	return *this;
}

void wxReportTableItem::DoCopy(const wxReportTableItem& table)
{
	int nRows = table.m_cells.GetCount();
	for(int r=0; r<nRows; ++r)
	{
		CellsArray *pRowSrc = table.m_cells.Item(r);
		CellsArray *pRow = new CellsArray();
		int nCols = pRowSrc->GetCount();
		for(int c=0; c<nCols; ++c)
		{
			wxReportTableCell *pCell = new wxReportTableCell(*(pRowSrc->Item(c)));
			pRow->Add(pCell);
		}
		this->m_cells.Add(pRow);
	}

	this->m_cellsStyle = table.m_cellsStyle;
	this->m_columnsSources = table.m_columnsSources;
	this->m_columnsWidths = table.m_columnsWidths;
	this->m_fContinue = table.m_fContinue;
	this->m_fDelete = table.m_fDelete;
	this->m_fHasNext = table.m_fHasNext;
	this->m_fVariable = true;
	
	int nHeaders = table.m_headersColumn.GetCount();
	for(int h=0; h<nHeaders; ++h)
	{
		wxReportTableCell *pHeaderSrc = table.m_headersColumn.Item(h);
		wxReportTableCell *pHeader = NULL;
		if(pHeaderSrc)
			pHeader = new wxReportTableCell(*pHeaderSrc);
			
		this->m_headersColumn.Add(pHeader);			
	}
	
	nHeaders = table.m_headersRow.GetCount();
	for(int h=0; h<nHeaders; ++h)
	{
		wxReportTableCell *pHeaderSrc = table.m_headersRow.Item(h);
		wxReportTableCell *pHeader = NULL;
		if(pHeaderSrc)
			pHeader = new wxReportTableCell(*pHeaderSrc);
			
		this->m_headersRow.Add(pHeader);			
	}
	
	this->m_iActualColumn = table.m_iActualColumn;
	this->m_iActualRow = table.m_iActualRow;
	this->m_iType = wxRP_TABLE;
	this->m_nColumns = table.m_nColumns;
	this->m_nRows = table.m_nRows;
	this->m_pVariable = NULL;
	this->m_position = table.m_position;
	this->m_rowsHeights = table.m_rowsHeights;
	this->m_rowsSources = table.m_rowsSources;
	this->m_sName = table.m_sName;
	this->m_sValue = wxT("");
	this->m_style = table.m_style;
	this->m_textAlign = table.m_textAlign;
}

void wxReportTableItem::AddColumn(int position)
{
	this->m_nColumns++;
	this->m_columnsWidths.Add(0);
	
	if(this->m_nRows == 0)
		this->m_nRows++;
	
	if(this->m_cells.GetCount() == 0)
	{
		this->m_cells.Add(new CellsArray());
		this->m_nRows = 1;
	}
	
	this->m_iActualRow = 0;
	
	if(position < 0)
		this->m_iActualColumn = this->m_nColumns - 1; //sets actual position at the inserted column
	else
		this->m_iActualColumn = position;
}

void wxReportTableItem::AddColumn(const wxArrayShort& array, const wxString& header, int position, bool isVariable)
{
	if(isVariable)
		this->DoSetSourceRecord((const void*)&array, wxRP_WXARRAYSHORT, array.GetCount(), false, position);
	else
		this->DoSetSourceRecord(NULL, -1, 0, false, position);
	
	this->DoAddColumn(array, header, position, isVariable);
	this->SynchronizeCellsSizes();
}

void wxReportTableItem::AddColumn(const wxArrayInt& array, const wxString& header, int position, bool isVariable)
{
	if(isVariable)
		this->DoSetSourceRecord((const void*)&array, wxRP_WXARRAYINT, array.GetCount(), false, position);
	else
		this->DoSetSourceRecord(NULL, -1, 0, false, position);
	
	this->DoAddColumn(array, header, position, isVariable);
	this->SynchronizeCellsSizes();
}

void wxReportTableItem::AddColumn(const wxArrayLong& array, const wxString& header, int position, bool isVariable)
{
	if(isVariable)
		this->DoSetSourceRecord((const void*)&array, wxRP_WXARRAYLONG, array.GetCount(), false, position);
	else
		this->DoSetSourceRecord(NULL, -1, 0, false, position);
	
	this->DoAddColumn(array, header, position, isVariable);
	this->SynchronizeCellsSizes();
}

void wxReportTableItem::AddColumn(const wxArrayDouble& array, const wxString& header, int position, bool isVariable)
{
	if(isVariable)
		this->DoSetSourceRecord((const void*)&array, wxRP_WXARRAYDOUBLE, array.GetCount(), false, position);
	else
		this->DoSetSourceRecord(NULL, -1, 0, false, position);
	
	this->DoAddColumn(array, header, position, isVariable);
	this->SynchronizeCellsSizes();
}

void wxReportTableItem::AddColumn(const wxArrayString& array, const wxString& header, int position, bool isVariable)
{
	if(isVariable)
		this->DoSetSourceRecord((const void*)&array, wxRP_WXARRAYSTRING, array.GetCount(), false, position);
	else
		this->DoSetSourceRecord(NULL, -1, 0, false, position);
	
	this->DoAddColumn(array, header, position, isVariable);
	this->SynchronizeCellsSizes();
}

template<class T> void wxReportTableItem::DoAddColumn(const T& array, const wxString& header, int position, bool isVariable)
{
	if(position < this->m_nColumns)
	{
		if(position < 0)
			position = this->m_nColumns;
		
		this->m_nColumns++;
		int cellsCount = array.GetCount();
		int nNewRows = cellsCount - this->m_cells.GetCount();
		
		for(int i=0; i<nNewRows; ++i) //adds new rows, if needed
		{
			this->m_cells.Add(new CellsArray());
			wxReportTableCellsSource src;
			this->m_rowsSources.Add(src);
			this->m_headersRow.Add((wxReportTableCell*)NULL);
		}
		
		this->m_nRows = this->m_cells.GetCount();
		
		wxReportTableCell *pHeader = new wxReportTableCell();
		pHeader->m_style = this->m_cellsStyle;
		pHeader->m_textAlign = this->m_textAlign;
		pHeader->m_sValue = header;
		
		CalculateHeight(pHeader, 0, true);
		CalculateWidth(pHeader, position+1, false);
				
		if(position == this->m_nColumns - 1)
			this->m_headersColumn.Add(pHeader);
		else
			this->m_headersColumn.Insert(pHeader, position);
		
		for(int r=0; r<cellsCount; ++r)
		{
			CellsArray *pRow = this->m_cells.Item(r);
			
			for(int i=pRow->GetCount(); i<position; ++i) //fills cells in row before inserted column
			{
				wxReportTableCell *emptyCell = new wxReportTableCell();
				emptyCell->SetValue(wxT("")); //added for testing purposes
				pRow->Add(emptyCell);
			}
			
			wxReportTableCell *pCell = new wxReportTableCell();
			pCell->m_fVariable = isVariable;
			pCell->SetValue(array.Item(r));
			pCell->m_style = this->m_cellsStyle;
			pCell->m_textAlign = this->m_textAlign;
			CalculateHeight(pCell, r+1, false);
			CalculateWidth(pCell, position+1, false);
			
			if(position == this->m_nColumns - 1)
				pRow->Add(pCell);
			else
				pRow->Insert(pCell, position);
				
			for(int i=pRow->GetCount(); i<this->m_nColumns; ++i)
			{
				wxReportTableCell *emptyCell = new wxReportTableCell();
				emptyCell->SetValue(wxT("")); //added for testing purposes
				pRow->Add(emptyCell);
			}
		}
		
		for(int r=cellsCount; r<this->m_nRows; ++r) //fills remaining cells in the column
		{
			wxReportTableCell *emptyCell = new wxReportTableCell();
			emptyCell->SetValue(wxT("")); //added for testing purposes
			
			if(position >= (int)(this->m_cells.Item(r)->GetCount()))
				this->m_cells.Item(r)->Add(emptyCell);
			else
				this->m_cells.Item(r)->Insert(emptyCell, position);
		}
	}
}

void wxReportTableItem::AddRow()
{
	CellsArray *newRow = new CellsArray();
	this->m_cells.Add(newRow);
	wxReportTableCellsSource src;
	this->m_rowsSources.Add(src);
	this->m_headersRow.Add((wxReportTableCell*)NULL);
	this->m_rowsHeights.Add(0);
	this->m_nRows++;
	this->m_iActualRow = this->m_nRows - 1;
}

void wxReportTableItem::AddRow(const wxArrayShort& array, const wxString& header, int position, bool isVariable)
{
	if(isVariable)
		this->DoSetSourceRecord((const void*)&array, wxRP_WXARRAYSHORT, array.GetCount(), true, position);
	else
		this->DoSetSourceRecord(NULL, -1, 0, true, position);
	
	this->DoAddRow(array, header, position, isVariable);
}

void wxReportTableItem::AddRow(const wxArrayInt& array, const wxString& header, int position, bool isVariable)
{
	if(isVariable)
		this->DoSetSourceRecord((const void*)&array, wxRP_WXARRAYINT, array.GetCount(), true, position);
	else
		this->DoSetSourceRecord(NULL, -1, 0, true, position);
	
	this->DoAddRow(array, header, position, isVariable);
}

void wxReportTableItem::AddRow(const wxArrayLong& array, const wxString& header, int position, bool isVariable)
{
	if(isVariable)
		this->DoSetSourceRecord((const void*)&array, wxRP_WXARRAYLONG, array.GetCount(), true, position);
	else
		this->DoSetSourceRecord(NULL, -1, 0, true, position);
	
	this->DoAddRow(array, header, position, isVariable);
}

void wxReportTableItem::AddRow(const wxArrayDouble& array, const wxString& header, int position, bool isVariable)
{
	if(isVariable)
		this->DoSetSourceRecord((const void*)&array, wxRP_WXARRAYDOUBLE, array.GetCount(), true, position);
	else
		this->DoSetSourceRecord(NULL, -1, 0, true, position);
	
	this->DoAddRow(array, header, position, isVariable);
}

void wxReportTableItem::AddRow(const wxArrayString& array, const wxString& header, int position, bool isVariable)
{
	if(isVariable)
		this->DoSetSourceRecord((const void*)&array, wxRP_WXARRAYSTRING, array.GetCount(), true, position);
	else
		this->DoSetSourceRecord(NULL, -1, 0, true, position);
	
	this->DoAddRow(array, header, position, isVariable);
}

template<class T> void wxReportTableItem::DoAddRow(const T& array, const wxString& header, int position, bool isVariable)
{
	if(position <= this->m_nRows)
	{
		if(position < 0)
			position = this->m_nRows;
		
		CellsArray *pRow = new CellsArray();
		int columnsCount = array.GetCount();
		
		wxReportTableCell *pHeader = new wxReportTableCell();
		pHeader->m_style = this->m_cellsStyle;
		pHeader->m_sValue = header;
		CalculateWidth(pHeader, 0, true);
		CalculateHeight(pHeader, position+1, false);
				
		for(int i=0; i<columnsCount; ++i)
		{
			wxReportTableCell *pCell = new wxReportTableCell();
			pCell->m_fVariable = isVariable;
			pCell->SetValue(array.Item(i));
			pCell->m_style = this->m_cellsStyle;
			//pCell->m_dHeight = 1.2 * pCell->m_style.GetFontSize() * (25.4/72.);
			pCell->m_textAlign = this->m_textAlign;
			CalculateWidth(pCell, i+1, false);
			CalculateHeight(pCell, position+1, false);
			pRow->Add(pCell);
		}
		
		int newCols = columnsCount - this->m_nColumns;
		
		if(newCols > 0)
			this->m_nColumns = columnsCount;
			
		for(int i=pRow->GetCount(); i<this->m_nColumns; ++i)
		{
			wxReportTableCell *emptyCell = new wxReportTableCell();
			emptyCell->SetValue(wxT("")); // '*' for testing
			pRow->Add(emptyCell);
		}
		
		if(position == this->m_nRows)
		{
			this->m_cells.Add(pRow);
			wxReportTableCellsSource src;
			this->m_rowsSources.Add(src);
			this->m_headersRow.Add(pHeader);
		}
		else
		{
			this->m_cells.Insert(pRow, position);
			this->m_headersRow.Insert(pHeader, position);
		}
		
		this->m_nRows = m_cells.GetCount();
		
		if(newCols > 0)
		{
			for(int c=0; c<newCols; ++c) // insert sources for new columns
			{
				wxReportTableCellsSource src;
				this->m_columnsSources.Add(src);
				this->m_headersColumn.Add((wxReportTableCell*)NULL);
			}
			
			for(int r=0; r<this->m_nRows; ++r) // added empty cells above and under the added row, if needed 
			{
				if(r != position)
				{
					CellsArray *pR = this->m_cells.Item(r);
					
					for(int c=0; c<newCols; ++c)
					{	
						wxReportTableCell *emptyCell = new wxReportTableCell();
						emptyCell->SetValue(wxT("")); // '*' for testing
						pR->Add(emptyCell);
					}
				}
			}
		}
	}
}

void wxReportTableItem::AddCell()
{
}

void wxReportTableItem::AddCellToColumn(const short& value, int position, bool isVariable)
{
	this->DoAddCellToColumn(value, position, isVariable);	
}

void wxReportTableItem::AddCellToColumn(const int& value, int position, bool isVariable)
{
	this->DoAddCellToColumn(value, position, isVariable);
}

void wxReportTableItem::AddCellToColumn(const long& value, int position, bool isVariable)
{
	this->DoAddCellToColumn(value, position, isVariable);
}

void wxReportTableItem::AddCellToColumn(const float& value, int position, bool isVariable)
{
	this->DoAddCellToColumn(value, position, isVariable);
}

void wxReportTableItem::AddCellToColumn(const double& value, int position, bool isVariable)
{
	this->DoAddCellToColumn(value, position, isVariable);
}

void wxReportTableItem::AddCellToColumn(const wxString& value, int position, bool isVariable)
{
	this->DoAddCellToColumn(value, position, isVariable);
}

template<class T> bool wxReportTableItem::DoAddCellToColumn(const T& value, int position, bool isVariable)
{
	if(position < this->m_nColumns)
	{
		wxReportTableCell *lastCell = NULL;
		wxReportTextStyle emptyCellStyle;
		int startRow = 0;
		
		if(position < 0)
			position = this->m_nColumns - 1;
		
		for(int r=this->m_cells.GetCount()-1; r>=0; --r) // find the start row (followed by last filled cell in the column)
		{
			if((int)(this->m_cells.Item(r)->GetCount()) > position)
				if(this->m_cells.Item(r)->Item(position) != NULL)
				{
					startRow = r + 1;
					lastCell = m_cells.Item(r)->Item(position);
					break;
				}
		}
		
		wxReportTableCell *pCell = new wxReportTableCell();
		pCell->m_fVariable = isVariable;
		pCell->SetValue(value);
		pCell->m_style = this->m_cellsStyle;
		pCell->m_textAlign = this->m_textAlign;
		CalculateWidth(pCell, position+1, false);
		CalculateHeight(pCell, startRow+1, false);
		
		if(lastCell)
			emptyCellStyle = lastCell->m_style;
		else
			emptyCellStyle = this->m_cellsStyle;
		
		bool atNewRow = false;
		if(startRow >= (int)(this->m_cells.GetCount()) ) // insert new row
		{
			CellsArray *pRow = new CellsArray();
			
			if(this->m_nColumns > 1)
			{
				atNewRow = true;
				
				for(int c=0; c<position; ++c) // fill row with empty cells, if needed
				{
					wxReportTableCell *emptyCell = new wxReportTableCell();
					emptyCell->SetStyle(emptyCellStyle);
					pRow->Add(emptyCell);
				}
			}
	
			this->m_cells.Add(pRow);
			wxReportTableCellsSource src;
			this->m_rowsSources.Add(src);
			this->m_headersRow.Add((wxReportTableCell*)NULL);
			this->m_nRows++;
		}
			
		if(position == this->m_nColumns - 1 || atNewRow) // add cell, if is added after the last column then Add, if not then AddAt
			this->m_cells.Item(startRow)->Add(pCell);
		else
			this->m_cells.Item(startRow)->Insert(pCell, position);
				
		this->m_iActualRow++;
		this->m_iActualColumn = position;
		return true;
	}
	
	return false;
}

void wxReportTableItem::AddCellToRow(const short& value, int position, bool isVariable)
{	
	this->DoAddCellToRow(value, position, isVariable);
}

void wxReportTableItem::AddCellToRow(const int& value, int position, bool isVariable)
{
	this->DoAddCellToRow(value, position, isVariable);
}

void wxReportTableItem::AddCellToRow(const long& value, int position, bool isVariable)
{
	this->DoAddCellToRow(value, position, isVariable);
}

void wxReportTableItem::AddCellToRow(const float& value, int position, bool isVariable)
{
	this->DoAddCellToRow(value, position, isVariable);
}

void wxReportTableItem::AddCellToRow(const double& value, int position, bool isVariable)
{
	this->DoAddCellToRow(value, position, isVariable);
}

void wxReportTableItem::AddCellToRow(const wxString& value, int position, bool isVariable)
{
	this->DoAddCellToRow(value, position, isVariable);
}

template<class T> void wxReportTableItem::DoAddCellToRow(const T& value, int position, bool isVariable)
{
	if(position < this->m_nRows)
	{
		if(position < 0)
			position = this->m_cells.GetCount() - 1;
			
		CellsArray *pRow = this->m_cells.Item(position);
		
		if(this->m_nColumns == (int)(pRow->GetCount())) // if adding cell to new column then increment count
		{
			wxReportTableCellsSource src;
			this->m_columnsSources.Add(src);
			this->m_headersColumn.Add((wxReportTableCell*)NULL);
			this->m_nColumns++;
		}
				
		wxReportTableCell *pCell = new wxReportTableCell();
		pCell->m_fVariable = isVariable;
		pCell->SetValue(value);
		pCell->m_style = this->m_cellsStyle;
		//pCell->m_dHeight = 1.2 * pCell->m_style.GetFontSize() * (25.4/72);
		pCell->m_textAlign = this->m_textAlign;
		CalculateWidth(pCell, pRow->GetCount()+1, false);
		CalculateHeight(pCell, position+1, false);
		pRow->Add(pCell);
		
		if((int)(pRow->GetCount()) >= this->m_nColumns)
			this->m_nColumns = pRow->GetCount();
	}
}

void wxReportTableItem::SetCellValue(const short& value, int row, int column, bool isVariable)
{
	this->DoSetCellValue(value, row, column, isVariable);
}

void wxReportTableItem::SetCellValue(const int& value, int row, int column, bool isVariable)
{
	this->DoSetCellValue(value, row, column, isVariable);
}

void wxReportTableItem::SetCellValue(const long& value, int row, int column, bool isVariable)
{
	this->DoSetCellValue(value, row, column, isVariable);
}

void wxReportTableItem::SetCellValue(const float& value, int row, int column, bool isVariable)
{
	this->DoSetCellValue(value, row, column, isVariable);
}

void wxReportTableItem::SetCellValue(const double& value, int row, int column, bool isVariable)
{
	this->DoSetCellValue(value, row, column, isVariable);
}

void wxReportTableItem::SetCellValue(const wxString& value, int row, int column, bool isVariable)
{
	this->DoSetCellValue(value, row, column, isVariable);
}

template<class T> void wxReportTableItem::DoSetCellValue(const T& value, int row, int column, bool isVariable)
{
	if(row >= 0 && row < this->m_nRows && column >= 0 && column < this->m_nColumns)
	{
		if(row < (int)(this->m_cells.GetCount()))
		{
			CellsArray *pRow = this->m_cells.Item(row);
			
			if(column < (int)(pRow->GetCount()))
			{
				wxReportTableCell *pCell = pRow->Item(column);
				pCell->m_fVariable = isVariable;				
				pCell->SetValue(value);
			}
		}
	}
}

bool wxReportTableItem::SetVariableForColumn(int columnIndex, const wxArrayShort& array)
{
	this->DoSetSourceRecord((const void*)&array, wxRP_ARRAYSHORT, array.GetCount(), false, columnIndex);	
	return this->DoSetVariableForColumn(columnIndex, array, 0);
}

bool wxReportTableItem::SetVariableForColumn(int columnIndex, const wxArrayInt& array)
{
	this->DoSetSourceRecord((const void*)&array, wxRP_ARRAYINT, array.GetCount(), false, columnIndex);
	return this->DoSetVariableForColumn(columnIndex, array, 0);
}

bool wxReportTableItem::SetVariableForColumn(int columnIndex, const wxArrayLong& array)
{
	this->DoSetSourceRecord((const void*)&array, wxRP_ARRAYLONG, array.GetCount(), false, columnIndex);
	return this->DoSetVariableForColumn(columnIndex, array, 0);
}

bool wxReportTableItem::SetVariableForColumn(int columnIndex, const wxArrayDouble& array)
{
	this->DoSetSourceRecord((const void*)&array, wxRP_ARRAYDOUBLE, array.GetCount(), false, columnIndex);
	return this->DoSetVariableForColumn(columnIndex, array, 0);
}

bool wxReportTableItem::SetVariableForColumn(int columnIndex, const wxArrayString& array)
{
	this->DoSetSourceRecord((const void*)&array, wxRP_WXARRAYSTRING, array.GetCount(), false, columnIndex);	
	return this->DoSetVariableForColumn(columnIndex, array, 0);
}

bool wxReportTableItem::SetVariableForRow(int rowIndex, const wxArrayShort& array)
{
	this->DoSetSourceRecord((const void*)&array, wxRP_WXARRAYSHORT, array.GetCount(), true, rowIndex);	
	return this->DoSetVariableForRow(rowIndex, array);
}

bool wxReportTableItem::SetVariableForRow(int rowIndex, const wxArrayInt& array)
{
	this->DoSetSourceRecord((const void*)&array, wxRP_WXARRAYINT, array.GetCount(), true, rowIndex);	
	return this->DoSetVariableForRow(rowIndex, array);
}

bool wxReportTableItem::SetVariableForRow(int rowIndex, const wxArrayLong& array)
{
	this->DoSetSourceRecord((const void*)&array, wxRP_WXARRAYLONG, array.GetCount(), true, rowIndex);	
	return this->DoSetVariableForRow(rowIndex, array);
}

bool wxReportTableItem::SetVariableForRow(int rowIndex, const wxArrayDouble& array)
{
	this->DoSetSourceRecord((const void*)&array, wxRP_WXARRAYDOUBLE, array.GetCount(), true, rowIndex);	
	return this->DoSetVariableForRow(rowIndex, array);
}

bool wxReportTableItem::SetVariableForRow(int rowIndex, const wxArrayString& array)
{
	this->DoSetSourceRecord((const void*)&array, wxRP_WXARRAYSTRING, array.GetCount(), true, rowIndex);	
	return this->DoSetVariableForRow(rowIndex, array);
}

void wxReportTableItem::DoSetSourceRecord(const void* array, int arrayId, int count, bool toRow, int index)
{
	wxReportTableCellsSource *src;
	
	if(toRow)
	{
		if(index >= 0 && index < this->m_nRows)
			src = &(this->m_rowsSources.Item(index));
		else
			src = NULL;
	}
	else
	{
		if(index >= 0 && index < this->m_nColumns)
			src = &(this->m_columnsSources.Item(index));
		else
			src = NULL;
	}
	
	if(src)
	{
		src->m_pArrSource = array;
		src->m_iStartPos = 0;
		src->m_iArrSourceType = arrayId;
		src->m_count = count;
	}
	else
	{
		wxReportTableCellsSource newSrc(array, arrayId, 0, count);
		
		if(toRow)
		{
			if(index < 0)
				index = this->m_nRows;
				
			if(index <= this->m_nRows)
			{
				if(index == this->m_nRows)
					this->m_rowsSources.Add(newSrc);
				else
					this->m_rowsSources.Insert(newSrc, index);
			}
		}
		else
		{
			if(index < 0)
				index = this->m_nColumns;
				
			if(index <= this->m_nColumns)
			{
				if(index == this->m_nColumns)
					this->m_columnsSources.Add(newSrc);
				else
					this->m_columnsSources.Insert(newSrc, index);
			}
		}			
	}
}

template<class T> bool wxReportTableItem::DoSetVariableForColumn(int columnIndex, const T& array, int start)
{
	if(columnIndex >= 0 && columnIndex < this->m_nColumns)
	{
		int arrCount = array.GetCount();
		int r, r1;
				
		for(r=start; r<arrCount; ++r)
		{
			r1 = r - start; 
			if(r1 < this->m_nRows) // set sources for existing cells
			{
				wxReportTableCell *pCell = this->GetCell(r1, columnIndex);
				pCell->m_fVariable = true;
				pCell->SetValue(array.Item(r));				
				
				if(pCell->m_dHeight == 0)
				{
					/*pCell->m_dHeight = 1.2 * pCell->m_style.GetFontSize() * (25.4/72.);
					pCell->m_dWidth = 30;*/
					pCell->m_textAlign = this->m_textAlign;
					pCell->m_style = this->m_cellsStyle;
					CalculateWidth(pCell, columnIndex+1, false);
					CalculateHeight(pCell, r1+1, false);
				}					
			}
			else // source array has more cells than now - add new cells
			{
				if(!m_fHasNext) // filled all existing cells and the table continues on the next page - stop adding new cells to column
				{
					this->DoAddCellToColumn(array.Item(r), columnIndex, true);
					for(int c=columnIndex+1; c<this->m_nColumns; ++c)
					{
						wxReportTableCell *eCell = new wxReportTableCell();
						/*double wi = this->GetCell(r1, c-1)->m_dWidth;
						double he = this->GetCell(r1-1, c)->m_dWidth;*/
						eCell->SetStyle(this->m_cellsStyle);
						eCell->m_dWidth = this->m_columnsWidths[c+1];
						eCell->m_dHeight = this->m_rowsHeights[r1+1];
						this->m_cells.Item(r1)->Add(eCell);
					}
				}
				else
					return true;
			}
		}
				
		int nRedundantCells = arrCount - this->m_nRows - start; 
		
		if(nRedundantCells < 0) // check if there are some redundant cells to fill (complete uncompleted column); also clear remaining cell if the whole array was processed
		{
			if(arrCount < start)
				arrCount = start;
			for(int r=arrCount - start; r<this->m_nRows; ++r)
			{
				wxReportTableCell *pCell = this->GetCell(r, columnIndex);
				pCell->m_sValue = wxT("");
				pCell->m_fVariable = false;
				pCell->m_pVariable = NULL;
			}
		}
		
		return true;
	}
	else
		return false;
	
}

template<class T> bool wxReportTableItem::DoSetVariableForRow(int rowIndex, const T& array)
{
	if(rowIndex >= 0 && rowIndex < this->m_nRows)
	{
		int arrCount = array.GetCount();
		CellsArray *pRow = this->m_cells.Item(rowIndex);
		int nColsInRow = pRow->GetCount();
		
		for(int c=0; c<arrCount; ++c)
		{
			if(c < nColsInRow)
			{
				wxReportTableCell *pCell = pRow->Item(c);
				pCell->m_fVariable = true;
				pCell->SetValue(array.Item(c));
				if(pCell->m_dHeight == 0)
				{
					/*pCell->m_dHeight = 1.2 * pCell->m_style.GetFontSize() * (25.4/72.);
					pCell->m_dWidth = 30;*/
					pCell->m_textAlign = this->m_textAlign;
					pCell->m_style = this->m_cellsStyle;
					CalculateHeight(pCell, rowIndex+1, false);
					CalculateWidth(pCell, c+1, false);
				}
			}
			else
			{
				this->DoAddCellToRow(array.Item(c), rowIndex, true);
				for(int r=0; r<rowIndex; ++r)
				{
					if((int)(this->m_cells.Item(r)->GetCount()) < this->m_nColumns)
					{
						wxReportTableCell *newCell = new wxReportTableCell();
						newCell->SetStyle(this->m_cellsStyle);
						this->m_cells.Item(r)->Add(newCell);						
					}
				}					
			}
		}
		
		int nRedundantCells = arrCount - this->m_nColumns;
		
		if(nRedundantCells < 0) // check if there are some redundant cells to fill
		{
			for(int r=arrCount; r<this->m_nColumns; ++r)
			{
				wxReportTableCell *pCell = pRow->Item(r);
				pCell->m_sValue = wxT(""); //for testing purposes
				pCell->m_fVariable = false;
				pCell->m_pVariable = NULL;
			}
		}
		
		return true;
	}
	else
		return false;
}

template<class T> bool wxReportTableItem::DoSetVariableForCell(int rowIndex, const T& var)
{
	return false;
}

void wxReportTableItem::ClearColumn(int position, bool withHeader)
{
	if(position >= 0 && position < this->m_nColumns)
	{
		int startPos = 1;
		
		if(withHeader)
			startPos = 0;
			
		for(int r=startPos; r<this->m_nRows; ++r)
		{
			delete this->m_cells.Item(r)->Item(position);
			this->m_cells.Item(r)->Item(position) = new wxReportTableCell();
		}
	}
}

void wxReportTableItem::ClearRow(int position, bool withHeader)
{
	if(position >= 0 && position < this->m_nRows)
	{
		int startPos = 1;
		
		if(withHeader)
			startPos = 0;
			
		for(int c=startPos; c<this->m_nColumns; ++c)
		{
			delete this->m_cells.Item(position)->Item(c);
			this->m_cells.Item(position)->Item(c) = new wxReportTableCell();
		}
	}
}

void wxReportTableItem::ClearCell(int row, int column)
{
	if(row >= 0 && row < this->m_nRows && column >= 0 && column < this->m_nColumns)
	{
		if(row < (int)(this->m_cells.GetCount()))
		{
			CellsArray *pRow = this->m_cells.Item(row);
			
			if(column < (int)(pRow->GetCount()))
				pRow->Item(column) = new wxReportTableCell();
		}
	}
}

void wxReportTableItem::RemoveColumn(int position)
{
	if(position >= 0 && position < this->m_nColumns)
	{
		int rowsCount = this->m_cells.GetCount();
		
		for(int r=0; r<rowsCount; ++r)
		{
			CellsArray *pRow = this->m_cells.Item(r);
			
			if((int)(pRow->GetCount()) > position)
			{
				delete pRow->Item(position);
				pRow->RemoveAt(position);
			}			
		}
		
		this->m_columnsSources.RemoveAt(position);
		this->m_headersColumn.RemoveAt(position);
		this->m_columnsWidths.RemoveAt(position);
		this->m_nColumns--;
	}
}

void wxReportTableItem::RemoveRow(int position)
{
	if(position >= 0 && position < this->m_nRows)
	{
		int columnsCount = this->m_cells.Item(position)->GetCount();
		
		for(int c=0; c<columnsCount; ++c)
			delete this->m_cells.Item(position)->Item(c);
			
		this->m_cells.RemoveAt(position);
		this->m_rowsSources.RemoveAt(position);
		this->m_headersRow.RemoveAt(position);
		this->m_rowsHeights.RemoveAt(position);
		this->m_nRows--;
	}
}

void wxReportTableItem::RemoveEmptyRows()
{
	for(int r=this->m_nRows-1; r>=0; --r)
	{
		CellsArray *pRow = this->m_cells.Item(r);
		int c;
		
		for(c=0; c<this->m_nColumns; ++c)
		{
			wxReportTableCell *pCell = pRow->Item(c);
			
			if(pCell->m_sValue != wxT(""))
				break;
		}
		
		if(c == this->m_nColumns)
			this->RemoveRow(this->m_cells.GetCount() - 1);
	}
	
	this->m_nRows = this->m_cells.GetCount();
}

void wxReportTableItem::RemoveEmptyColumns()
{
	for(int c=this->m_nColumns-1; c>=0; --c)
	{
		int r;
		for(r=0; r<this->m_nRows; ++r)
			if(this->m_cells.Item(r)->Item(c)->GetValue() != wxT(""))
				break;
		
		if(r == this->m_nRows)
			this->RemoveColumn(c);
	}
}

void wxReportTableItem::SetColumnHeader(const wxReportTableCell& header, int columnIndex)
{
	if(columnIndex >=0 && columnIndex < this->m_nColumns)
	{
		if(this->m_headersColumn[columnIndex])
			*(this->m_headersColumn[columnIndex]) = header;
		else
			this->m_headersColumn[columnIndex] = new wxReportTableCell(header);
		
		CalculateWidth(this->m_headersColumn[columnIndex], columnIndex+1, false);
		CalculateHeight(this->m_headersColumn[columnIndex], 0, true);
		this->SynchronizeCellsSizes();
	}
}

void wxReportTableItem::SetRowHeader(const wxReportTableCell& header, int rowIndex)
{
	if(rowIndex >=0 && rowIndex < this->m_nRows)
	{
		if(this->m_headersRow[rowIndex])
			*(this->m_headersRow[rowIndex]) = header;
		else
			this->m_headersRow[rowIndex] = new wxReportTableCell(header);
			
		CalculateWidth(this->m_headersRow[rowIndex], 0, true);
		CalculateHeight(this->m_headersRow[rowIndex], rowIndex+1, false);
		this->SynchronizeCellsSizes();
	}
}

void wxReportTableItem::SetRowHeight(int rowNumber, double height)
{
	if(rowNumber < this->m_nRows && height >= 0)
	{
		this->m_rowsHeights[rowNumber+1] = height;
		CellsArray *pRow = this->m_cells.Item(rowNumber);
		
		if(pRow)
		{
			int colsCount = pRow->GetCount();
			
			for(int c=0; c<colsCount; ++c)
			{
				wxReportTableCell *pCell = pRow->Item(c);
				
				if(pCell)
					pCell->m_dHeight = height;
			}
		}
	}
}

void wxReportTableItem::SetRowHeight(double height)
{
	for( int r = 0; r < this->m_nRows; ++r )
		SetRowHeight( r, height );
}

double wxReportTableItem::GetRowHeight(int rowIndex) const
{
	if(rowIndex < 0)
		return this->m_rowsHeights[0];
	
	if(rowIndex >=0 && rowIndex < this->m_nRows)
		return this->m_rowsHeights[rowIndex+1];
	else
		return 0.;
}

void wxReportTableItem::SetColumnWidth(int columnNumber, double width)
{
	if(columnNumber >= 0 && columnNumber < this->m_nColumns && width >= 0)
	{
		this->m_columnsWidths[columnNumber+1] = width;
		int rowsCount = this->m_cells.GetCount();
		
		for(int r=0; r<rowsCount; ++r)
		{
			CellsArray *pRow = this->m_cells.Item(r);
			
			if(pRow)
			{
				wxReportTableCell *pCell = pRow->Item(columnNumber);
				
				if(pCell)
					pCell->m_dWidth = width;
			}
		}
	}
	else if(columnNumber < 0)
	{
		this->m_columnsWidths[0] = width;
		for(int c=0; c<(int)(this->m_headersRow.GetCount()); ++c)
		{
			wxReportTableCell *pCell = this->m_headersRow.Item(c);
			if(pCell)
				pCell->m_dWidth = width;
		}
	}
}

void wxReportTableItem::SetColumnWidth(double width)
{
	for(int c=0; c<this->m_nColumns; ++c)
		this->SetColumnWidth(c, width);
		
	this->SetColumnWidth(-1, width);
}

double wxReportTableItem::GetColumnWidth(int columnIndex) const
{
	if(columnIndex < 0)
	{
		double width = 0;
		for(int r=0; r<(int)(this->m_headersRow.GetCount()); ++r)
		{
			wxReportTableCell *pCell = this->m_headersRow.Item(r);
			if(pCell)
				if(pCell->m_dWidth > width)
					width = pCell->m_dWidth;
		}
		return width;
	}
		
		
	if(columnIndex >=0 && columnIndex < this->m_nColumns && this->m_nRows >= 0)
		return this->m_cells.Item(0)->Item(columnIndex)->m_dWidth;
	else
		return 0.;
}

void wxReportTableItem::SynchronizeCellsSizes()
{
	for(int r=0; r<this->m_nRows; ++r)
	{
		CellsArray *pRow = this->m_cells[r];
		double h = this->m_rowsHeights[r+1];
		for(int c=0; c<this->m_nColumns; ++c)
		{
			wxReportTableCell *pCell = pRow->Item(c);
			pCell->m_dWidth = this->m_columnsWidths[c+1];
			pCell->m_dHeight = h;
		}
	}
	
	int nRowHeaders = this->m_headersRow.GetCount();
	double w = this->m_columnsWidths[0];
	for(int r=0; r<nRowHeaders; ++r)
	{
		wxReportTableCell *pCell = this->m_headersRow[r];
		if(pCell)
		{
			pCell->m_dWidth = w;
			pCell->m_dHeight = this->m_rowsHeights[r+1];
		}
	}
	
	int nColsHeaders = this->m_headersColumn.GetCount();
	double h = this->m_rowsHeights[0];
	for(int c=0; c<nColsHeaders; ++c)
	{
		wxReportTableCell *pCell = this->m_headersColumn[c];
		if(pCell)
		{
			pCell->m_dWidth = this->m_columnsWidths[c+1];
			pCell->m_dHeight = h;
		}
	}			
}

void wxReportTableItem::SetStyle(const wxReportTableStyle& style)
{
	this->m_style = style;
}

void wxReportTableItem::SetCellsStyle(const wxReportTextStyle& style)
{
	this->m_cellsStyle = style;
}

void wxReportTableItem::SetCellsStyleForColumn(const wxReportTextStyle& style, int columnIndex)
{
	if(columnIndex >= 0 && columnIndex < this->m_nColumns)
	{
		for(int r=0; r<this->m_nRows; ++r)
		{
			CellsArray *pRow = this->m_cells.Item(r);
			
			if((int)(pRow->GetCount()) > columnIndex)
				pRow->Item(columnIndex)->SetStyle(style);
		}
	}
}

void wxReportTableItem::SetCellsStyleForRow(const wxReportTextStyle& style, int rowIndex)
{
	if(rowIndex >= 0 && rowIndex < this->m_nRows)
	{
		CellsArray *pRow = this->m_cells.Item(rowIndex);
		int nCols = pRow->GetCount();
		
		for(int c=0; c<nCols; ++c)
			pRow->Item(c)->SetStyle(style);
	}
}

void wxReportTableItem::SetCellStyle(int row, int column, const wxReportTextStyle& style)
{
	if(row >= 0 && column >= 0)
	{
		if(row < (int)(this->m_cells.GetCount()))
		{
			CellsArray *pRow = this->m_cells.Item(row);
			
			if(pRow)
			{
				if(column < (int)(pRow->GetCount()))
				{
					wxReportTableCell *pCell = pRow->Item(column);
					
					if(pCell)
						pCell->m_style = style;
				}				
			}
		}
	}
}

void wxReportTableItem::SetRowsHeadersStyle(const wxReportTextStyle& style, int rowIndex)
{
	if(rowIndex < 0)
	{
		for(int r=0; r<this->m_nRows; ++r)
		{
			wxReportTableCell *pHeader = this->m_headersRow.Item(r);
			if(pHeader)
				pHeader->m_style = style;
			
		}
	}
	else if(rowIndex < this->m_nRows)
	{
		wxReportTableCell *pHeader = this->m_headersRow.Item(rowIndex);
		if(pHeader)
			pHeader->m_style = style;
	}
}

void wxReportTableItem::SetColumnsHeadersStyle(const wxReportTextStyle& style, int columnIndex)
{
	if(columnIndex < 0)
	{
		for(int c=0; c<this->m_nColumns; ++c)
		{
			wxReportTableCell *pHeader = this->m_headersColumn.Item(c);
			if(pHeader)
				pHeader->m_style = style;			
		}
	}
	else if(columnIndex < this->m_nColumns)
	{
		wxReportTableCell *pHeader = this->m_headersColumn.Item(columnIndex);
		if(pHeader)
			pHeader->m_style = style;
	}
}

void wxReportTableItem::SetTextAlign(int alignType)
{
	this->m_textAlign = alignType;
}

wxReportTableStyle& wxReportTableItem::Style()
{
	return this->m_style;
}

wxReportTextStyle& wxReportTableItem::CellsStyle()
{
	return this->m_cellsStyle;
}

const wxReportTextStyle& wxReportTableItem::GetCellsStyle() const
{
	return this->m_cellsStyle;
}

wxReportTableCell* wxReportTableItem::GetCell(int row, int column) const
{
	wxReportTableCell *pCell = NULL;
	
	if(row >= 0 && row < this->m_nRows && column >= 0 && column < this->m_nColumns)
	{
		if(row < (int)(this->m_cells.GetCount()))
		{
			CellsArray *pRow = this->m_cells.Item(row);
			
			if(column < (int)(pRow->GetCount()))
				pCell = pRow->Item(column);
		}
	}
	
	return pCell;
}

wxString wxReportTableItem::GetCellValue(int row, int column)
{
	if(row >= 0 && row < this->m_nRows && column >= 0 && column < this->m_nColumns)
	{
		if(row < (int)(this->m_cells.GetCount()))
		{
			CellsArray *pRow = this->m_cells.Item(row);
			
			if(column < (int)(pRow->GetCount()))
				return pRow->Item(column)->m_sValue;
			else return wxT("");
		}
		else return wxT("");
	}	
	else return wxT("");
}

int wxReportTableItem::GetColumnsCount()  const
{
	return this->m_nColumns;
}

int wxReportTableItem::GetRowsCount()  const
{
	return this->m_nRows;
}

bool wxReportTableItem::HasRowsHeaders() const
{
	wxReportTableCell *pHeader = NULL;
	for(int r=0; r<this->m_nRows; ++r)
	{
		pHeader = this->m_headersRow.Item(r);
		if(pHeader != NULL)
			break;
	}
	
	return (pHeader != NULL);
}

bool wxReportTableItem::HasColumnsHeaders() const
{
	wxReportTableCell *pHeader = NULL;
	for(int c=0; c<this->m_nColumns; ++c)
	{
		pHeader = this->m_headersColumn.Item(c);
		if(pHeader != NULL)
			break;
	}
	
	return (pHeader != NULL);
}

double wxReportTableItem::GetTableHeight() const
{
	double th = 0;
	int nRows = this->m_rowsHeights.GetCount();
	for(int r=0; r<nRows; ++r)
		th += this->m_rowsHeights[r];
	
	return th;
}

double wxReportTableItem::GetTableWidth() const
{
	//CellsArray *pRow = this->m_cells.Item(0);
	double tw = 0.;
	
	int nCols = this->m_columnsWidths.GetCount();
	for(int c=0; c<nCols; ++c)
	{
		if(c==0 && !(this->HasRowsHeaders()))
			continue;
		
		tw += this->m_columnsWidths[c];
	}
	
	return tw;
}

void wxReportTableItem::RefreshVariable()
{
	for(int c=0; c<this->m_nColumns; ++c)
	{
		wxReportTableCellsSource *src = &(this->m_columnsSources.Item(c));
		
		if(src->m_pArrSource) // column is filled from array
		{
			src->Refresh();
			switch(src->m_iArrSourceType)
			{
				case wxRP_WXARRAYSHORT:
					this->DoSetVariableForColumn(c, *((const wxArrayShort*)(src->m_pArrSource)), src->m_iStartPos);
					break;
					
				case wxRP_WXARRAYINT:
					this->DoSetVariableForColumn(c, *((const wxArrayInt*)(src->m_pArrSource)), src->m_iStartPos);
					break;
					
				case wxRP_WXARRAYLONG:
					this->DoSetVariableForColumn(c, *((const wxArrayLong*)(src->m_pArrSource)), src->m_iStartPos);
					break;
					
				case wxRP_WXARRAYDOUBLE:
					this->DoSetVariableForColumn(c, *((const wxArrayDouble*)(src->m_pArrSource)), src->m_iStartPos);
					break;
					
				case wxRP_WXARRAYSTRING:
					this->DoSetVariableForColumn(c, *((const wxArrayString*)(src->m_pArrSource)), src->m_iStartPos);
					break;
			}
		}		
	}
	
	this->RemoveEmptyRows();
	
	bool next = false;
	for(int c=0; c<this->m_nColumns; ++c) // check if the table will continue on next page
	{
		wxReportTableCellsSource *src = &(this->m_columnsSources.Item(c));
		if(src->m_iStartPos + this->m_nRows < src->m_count && src->m_pArrSource != NULL)
		{
			next = true;
			break;
		}
	}
	this->m_fHasNext = next;
	
	for(int r=0; r<this->m_nRows; ++r)
	{
		wxReportTableCellsSource *src = &(this->m_rowsSources.Item(r));
		
		if(src->m_pArrSource) // row is filled from array
		{
			switch(src->m_iArrSourceType)
			{
				case wxRP_WXARRAYSHORT:
					this->DoSetVariableForRow(r, *((const wxArrayShort*)(src->m_pArrSource)));
					break;
					
				case wxRP_WXARRAYINT:
					this->DoSetVariableForRow(r, *((const wxArrayInt*)(src->m_pArrSource)));
					break;
					
				case wxRP_WXARRAYLONG:
					this->DoSetVariableForRow(r, *((const wxArrayLong*)(src->m_pArrSource)));
					break;
					
				case wxRP_WXARRAYDOUBLE:
					this->DoSetVariableForRow(r, *((const wxArrayDouble*)(src->m_pArrSource)));
					break;
					
				case wxRP_WXARRAYSTRING:
					this->DoSetVariableForRow(r, *((const wxArrayString*)(src->m_pArrSource)));
					break;
			}
		}
		else // row isnt filled from array, fill cells from individual sources
		{
			CellsArray *pRow = this->m_cells.Item(r);
			int nCols = pRow->GetCount();
			
			for(int c=0; c<nCols; ++c)
			{
				wxReportTableCellsSource *srcCol = &(this->m_columnsSources.Item(c));
				wxReportTableCell *pCell = pRow->Item(c);
				
				if(pCell && srcCol->m_pArrSource == NULL) // check if the cell isnt filled from column array
				{
					if(pCell->IsVariable())
						pCell->RefreshVariable();
				}
			}				
		}
	}
	
	this->SynchronizeCellsSizes();
	//this->RemoveEmptyColumns();
}

wxXmlNode* wxReportTableItem::CreateXmlNode()
{
	wxXmlNode *itemNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Item"));
	itemNode->AddAttribute(wxT("Type"), wxString::Format(wxT("%d"), this->m_iType));
	itemNode->AddAttribute(wxT("Name"), this->m_sName);
	itemNode->AddAttribute(wxT("X"), wxString::Format(wxT("%G"), this->m_position.x));
	itemNode->AddAttribute(wxT("Y"), wxString::Format(wxT("%G"), this->m_position.y));
	itemNode->AddAttribute(wxT("Variable"), wxString::Format(wxT("%d"), this->m_fVariable));
	itemNode->AddProperty(this->m_style.CreateXmlNode()->GetAttributes());
	
	wxXmlNode *cellsStyleNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("DefaultCellsStyle"));
	cellsStyleNode->AddChild(this->m_cellsStyle.CreateXmlNode());
	itemNode->AddChild(cellsStyleNode);
	
	wxXmlNode *rowHeadersNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("RowHeaders"));
	for(int r=0; r<this->m_nRows; ++r) // save row headers
	{
		wxReportTableCell *pHeader = this->m_headersRow[r];
		wxXmlNode *headerNode;
		
		if(pHeader)
			headerNode = pHeader->CreateXmlNode(this->m_cellsStyle);
		else
			headerNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("TableCell"));
		
		rowHeadersNode->AddChild(headerNode);
	}
	itemNode->AddChild(rowHeadersNode);
	
	wxXmlNode *columnHeadersNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("ColumnHeaders"));
	for(int c=0; c<this->m_nColumns; ++c) // save row headers
	{
		wxReportTableCell *pHeader = this->m_headersColumn[c];
		wxXmlNode *headerNode;
		
		if(pHeader)
			headerNode = pHeader->CreateXmlNode(this->m_cellsStyle);
		else
			headerNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("TableCell"));
		
		columnHeadersNode->AddChild(headerNode);
	}
	itemNode->AddChild(columnHeadersNode);
	
	for(int r=0; r<this->m_nRows; ++r)
	{
		CellsArray *pRow = this->m_cells.Item(r);
		wxXmlNode *rowNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Row"));
		
		for(int c=0; c<this->m_nColumns; ++c)
		{
			wxReportTableCell *pCell = pRow->Item(c);
			wxXmlNode *cellNode;
			
			if(pCell)
				cellNode = pCell->CreateXmlNode(this->m_cellsStyle);
			else
				cellNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("TableCell"));
			
			rowNode->AddChild(cellNode);
		}

		itemNode->AddChild(rowNode);
	}
	
	return itemNode;
}

bool wxReportTableItem::RetrieveFromXmlNode(const wxXmlNode* node)
{
	int type = -1;
	double x = 0, y = 0;
	
	wxSscanf(node->GetAttribute(wxT("Type"), wxT("")), wxT("%d"), &type);
	if(type != wxRP_TABLE)
		return false;
		
	this->m_sName = node->GetAttribute(wxT("Name"), wxT(""));
	
	wxSscanf(node->GetAttribute(wxT("X"), wxT("")), wxT("%lf"), &x);
	wxSscanf(node->GetAttribute(wxT("Y"), wxT("")), wxT("%lf"), &y);
	this->m_position = wxRealPoint(x, y);
	
	bool isVariable = false;
	wxSscanf(node->GetAttribute(wxT("Variable"), wxT("")), wxT("%d"), &isVariable);
	this->m_fVariable = isVariable;
	
	if(!this->m_style.RetrieveFromXmlNode(node))
		return false;
		
	wxXmlNode *cellsStyleNode = node->GetChildren();
	this->m_cellsStyle.RetrieveFromXmlNode(cellsStyleNode->GetChildren());
		
	wxXmlNode *rowHeadersNode = cellsStyleNode->GetNext();
	wxXmlNode *headerNode = rowHeadersNode->GetChildren();
	while(headerNode)
	{
		wxReportTableCell *pHeader = new wxReportTableCell();
		if(!pHeader->RetrieveFromXmlNode(headerNode, this->m_cellsStyle))
		{
			delete pHeader;
			pHeader = NULL;
		}
		
		this->m_headersRow.Add(pHeader);
		headerNode = headerNode->GetNext();
	}
	
	wxXmlNode *columnHeadersNode = rowHeadersNode->GetNext();
	headerNode = columnHeadersNode->GetChildren();
	while(headerNode)
	{
		wxReportTableCell *pHeader = new wxReportTableCell();
		if(!pHeader->RetrieveFromXmlNode(headerNode, this->m_cellsStyle))
		{
			delete pHeader;
			pHeader = NULL;
		}
		
		if(pHeader)
		{
			this->CalculateHeight(pHeader, this->m_headersColumn.GetCount()+1, false, pHeader->m_dHeight);
			this->CalculateWidth(pHeader, 0, true, pHeader->m_dWidth);
		}
		this->m_headersColumn.Add(pHeader);
		headerNode = headerNode->GetNext();
	}
		
	wxXmlNode *rowNode = columnHeadersNode->GetNext();
	
	while(rowNode)
	{
		this->m_rowsSources.Add(wxReportTableCellsSource()); // define new sources record for this row
		
		CellsArray *pRow = new CellsArray();
		wxXmlNode *cellNode = rowNode->GetChildren();

		while(cellNode)
		{
			wxReportTableCell *pCell = new wxReportTableCell();
			if(!pCell->RetrieveFromXmlNode(cellNode, this->m_cellsStyle))
			{
				delete pCell;
				pCell = new wxReportTableCell();
			}
			
			this->CalculateHeight(pCell, this->m_cells.GetCount()+1, false, pCell->m_dHeight);
			this->CalculateWidth(pCell, pRow->GetCount()+1, false, pCell->m_dWidth);
			pRow->Add(pCell);
			cellNode = cellNode->GetNext();
		}
		
		this->m_cells.Add(pRow);
		rowNode = rowNode->GetNext();
	}
	
	this->m_nRows = this->m_cells.GetCount();
	
	if(this->m_nRows > 0)
		this->m_nColumns = this->m_cells.Item(0)->GetCount();
	else
		this->m_nColumns = 0;
	
	for(int c=0; c<this->m_nColumns; ++c)
		this->m_columnsSources.Add(wxReportTableCellsSource()); // define new sources record for this column
	
	return true;
}

void wxReportTableItem::DrawToDC(wxDC* dc, bool toScreen, const wxReportPageStyle& pageStyle)
{
	double lm = pageStyle.GetLeftMargin(); // apge margins
	double rm = pageStyle.GetRightMargin();
	double tm = pageStyle.GetTopMargin();
	double bm = pageStyle.GetBottomMargin();
	double tw=this->GetTableWidth();
	double th=this->GetTableHeight();
	
	wxPoint posPX(MM2PX(m_position.x, dc, toScreen), MM2PX(m_position.y, dc, toScreen));
	
	if(this->m_position.x >= 10000 || this->m_position.y >= 10000)
		posPX = CalcNegPos(m_position, posPX, MM2PX(tw, dc, toScreen), MM2PX(th, dc, toScreen), dc);
		
	if(this->m_position.x < wxRP_CENTER)
		posPX.x += MM2PX(lm, dc, toScreen);
	if(this->m_position.x == wxRP_RIGHT)	
		posPX.x -= MM2PX(rm, dc, toScreen);
	if(this->m_position.y < wxRP_CENTER)
		posPX.y += MM2PX(tm, dc, toScreen);
	if(this->m_position.x == wxRP_BOTTOM)	
		posPX.y -= MM2PX(bm, dc, toScreen);
		 
	int cy = posPX.y;
		
	for(int r=-1; r<this->m_nRows; ++r)
	{
		if(r < 0)
			if(! this->HasColumnsHeaders() )
				continue;
		
		CellsArray *pRow = NULL;
		if(r < 0)
			pRow = &(this->m_headersColumn);
		else
			pRow = this->m_cells.Item(r);		
		
		int cx = posPX.x; //this->m_position.x;//MM2PX(this->m_position.x);
		
		for(int c=-1; c<this->m_nColumns; ++c)
		{
			if(c < 0)
				if(! this->HasRowsHeaders() )
					continue;
				
			wxReportTableCell *pCell = NULL;
			wxReportTextStyle *pStyle;
			double cw, ch;
			
			if(c >= 0 && r >= 0)
				pCell = pRow->Item(c);
				
			if(c < 0 && r >= 0)
				pCell = this->m_headersRow.Item(r);
			
			if(r < 0 && c >=0)
				if(this->m_headersColumn.Item(c))
					pCell = this->m_headersColumn.Item(c);
			
			if(c < 0 && r < 0)
			{
				pCell = new wxReportTableCell();
				pCell->m_style = this->m_cellsStyle;
				pCell->m_dWidth = this->GetColumnWidth(-1);
				pCell->m_dHeight = this->GetRowHeight(-1);
			}
			pStyle = &(pCell->m_style);
			cw = MM2PX(pCell->m_dWidth, dc, toScreen);
			ch = MM2PX(pCell->m_dHeight, dc, toScreen);
			
			int borderWidth = 0;
			
			if(pStyle->GetBorder() > 0 || pStyle->GetBackgroundColor() != wxNullColour)
			{
				wxColour borderColor = pStyle->GetBorderColor();
				borderWidth = MM2PX(pStyle->GetBorderThickness(), dc, toScreen);
				wxColour bgColor = pStyle->GetBackgroundColor();
				wxPen pen(borderColor, borderWidth);
				pen.SetCap(wxCAP_BUTT);
				dc->SetPen(pen);
				
				if(bgColor == wxNullColour)
					dc->SetBrush(*wxTRANSPARENT_BRUSH);
				else
				{
					dc->SetBrush(wxBrush(bgColor));
					if(pStyle->GetBorder() != wxRP_ALLBORDER)
					{
						pen.SetColour(bgColor);
						dc->SetPen(pen);
						dc->DrawRectangle(cx, cy, cw, ch);
						pen.SetColour(borderColor);
						dc->SetPen(pen);
					}
				}
				
				DrawBorder(dc, pStyle->GetBorder(), cx, cy, cw, ch);
				//dc->DrawRectangle(cx, cy, cw, ch);
			}
			
			if(pCell)
			{
				//dc->SetMapMode(wxMM_POINTS);
				dc->SetFont(pCell->GetStyle().GetFont());
				int textW, textH;
				dc->GetTextExtent(pCell->GetValue(), &textW, &textH);
				
				double cos = 0; // cell off-set
				
				switch(pCell->GetTextAlign()) // set x position or word spacing for specified align
				{
					case wxRP_CENTERALIGN:
						cos += ((double)cw - (double)textW) / 2;
						break;
						
					case wxRP_RIGHTALIGN:
						cos += cw - textW - 3;
						break;
						
					default:
						cos = 3;
				}
				
				dc->SetTextForeground(pStyle->GetTextColor());
				dc->DrawText(pCell->m_sValue, cx + cos, cy);
				//dc->SetMapMode(wxMM_TEXT);
			}
			
			cx += cw; //MM2PX(pStyle->GetBorderThickness(), dc, toScreen);
			
			if(c == this->m_nColumns - 1)
			{
				cy += ch; //MM2PX(pStyle->GetBorderThickness(), dc, toScreen);
			}
		}
	}
}
