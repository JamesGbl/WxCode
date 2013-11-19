#include "wx/report/reportdocument.h"
#include "wx/arrimpl.cpp"
#include <wx/wfstream.h>

#define BAD_VALUE(item) {delete item; return false;}

WX_DEFINE_USER_EXPORTED_OBJARRAY(HandlersArray);

wxReportDocument::wxReportDocument()
{
	this->m_fAutoRefresh = true;
	this->m_iActivePageIndex = -1;
	this->AddPage();
	this->m_pActivePage = this->m_arrPages.Last();
	//this->m_iUnitType = wxRP_MM;
	this->m_printData.SetPaperId(wxPAPER_A4);
	this->m_iDPI = 600;
	this->m_sVersion = wxT("1.2.1 Beta");
	
	/*wxLocale locale;
	locale.Init();*/
}

wxReportDocument::~wxReportDocument()
{
	this->DoDestroy();	
}

void wxReportDocument::DoDestroy()
{
	int nHandlers = this->m_arrHandlers.GetCount();
	int nPages = this->m_arrPages.GetCount();
	int ftItems = this->m_arFooter.GetCount();
	int hdItems = this->m_arHeader.GetCount();
	
	for(int i=0; i<nHandlers; ++i)
		delete this->m_arrHandlers.Item(i);
		
	for(int i=0; i<nPages; ++i)
		delete this->m_arrPages.Item(i);
		
	for(int i=0; i<ftItems; ++i)
		delete this->m_arFooter.Item(i);
		
	for(int i=0; i<hdItems; ++i)
		delete this->m_arHeader.Item(i);
		
	this->m_arrHandlers.Clear();
	this->m_arrPages.Clear();
	this->m_arFooter.Clear();
	this->m_arHeader.Clear();
}

wxReportDocument::wxReportDocument(const wxReportDocument& report)
{
	this->DoDestroy();
	this->DoCopy(report);
}

wxReportDocument& wxReportDocument::operator= (const wxReportDocument& report)
{
	if(this == &report)
		return *this;
	
	this->DoDestroy();
	this->DoCopy(report);
	return *this;
}

void wxReportDocument::DoCopy(const wxReportDocument& report)
{
	int ftItems = report.m_arFooter.GetCount();
	int hdItems = report.m_arHeader.GetCount();
		
	for(int f=0; f<ftItems; ++f)
	{
		wxReportPageItem *newItem = this->CreateItemCopy(report.m_arFooter.Item(f));
		if(newItem)
			this->m_arFooter.Add(newItem);
	}
		
	for(int h=0; h<hdItems; ++h)
	{
		wxReportPageItem *newItem = this->CreateItemCopy(report.m_arHeader.Item(h));
		if(newItem)
			this->m_arHeader.Add(newItem);
	}
	
	int nPages = report.m_arrPages.GetCount();
	for(int p=0; p<nPages; ++p)
	{
		wxReportPage *pP = new wxReportPage(*(report.m_arrPages.Item(p)));
		this->m_arrPages.Add(pP);
	}
	
	this->m_fAutoRefresh = report.m_fAutoRefresh;
	this->m_iActivePageIndex = report.m_iActivePageIndex;
	this->m_iDPI = report.m_iDPI;
	this->m_iUnitType = report.m_iUnitType;
	this->m_pActivePage = this->m_arrPages.Item(this->m_iActivePageIndex);
	this->m_pageNumberFormat = report.m_pageNumberFormat;
	this->m_pageStyle = report.m_pageStyle;
	this->m_printData = report.m_printData;
}

void wxReportDocument::SetPageStyle(const wxReportPageStyle& pageStyle)
{
	this->m_pageStyle = pageStyle;
	this->m_arrPages.Last()->m_style = this->m_pageStyle;
}

void wxReportDocument::SetActivePage(int pageIndex)
{
	if(pageIndex >= 0 && pageIndex < (int)(this->m_arrPages.GetCount()))
	{
		this->m_pActivePage = this->m_arrPages.Item(pageIndex);
		this->m_iActivePageIndex = pageIndex;
	}
}

bool wxReportDocument::SetVariable(const short& var, const wxString& name, int pageIndex)
{
	return this->DoSetVariable(name, pageIndex, var, wxRP_SHORT);
}

bool wxReportDocument::SetVariable(const int& var, const wxString& name, int pageIndex)
{
	return this->DoSetVariable(name, pageIndex, var, wxRP_INT);
}

bool wxReportDocument::SetVariable(const long& var, const wxString& name, int pageIndex)
{
	return this->DoSetVariable(name, pageIndex, var, wxRP_LONG);
}

bool wxReportDocument::SetVariable(const float& var, const wxString& name, int pageIndex)
{
	return this->DoSetVariable(name, pageIndex, var, wxRP_FLOAT);
}

bool wxReportDocument::SetVariable(const double& var, const wxString& name, int pageIndex)
{
	return this->DoSetVariable(name, pageIndex, var, wxRP_DOUBLE);
}

bool wxReportDocument::SetVariable(const wxString& var, const wxString& name, int pageIndex)
{
	if(pageIndex >= (int)(this->m_arrPages.GetCount()))
		return false;
		
	bool retVal = false;
	int iPage = pageIndex, iEnd;
	
	if(pageIndex < 0)
		iEnd = m_arrPages.GetCount() - 1;
	else
	{
		iEnd = pageIndex;
		--iPage;
	}

	while(++iPage <= iEnd)
	{
		wxReportPage *page = this->m_arrPages.Item(iPage);
		ItemsMap::iterator it = page->m_mapVariables.find(name);

		if(it != page->m_mapVariables.end())
		{
			wxReportPageItem *pItem = it->second;
			pItem->m_pVariable = (const void*)&var;
			pItem->m_sValue = var;
			pItem->m_iValueType = wxRP_WXSTRING;
			retVal = true;
		}
		else
			retVal = false;
	}
	
	return retVal;
}

template<class T> bool wxReportDocument::DoSetVariable(const wxString& name, int pageIndex, const T& var, int varType)
{
	if(pageIndex >= (int)(this->m_arrPages.GetCount()))
		return false;
		
	bool retVal = false;
	int iPage = pageIndex, iEnd;
	
	if(pageIndex < 0)
		iEnd = m_arrPages.GetCount() - 1;
	else
	{
		iEnd = pageIndex;
		--iPage;
	}

	while(++iPage <= iEnd)
	{
		wxReportPage *page = this->m_arrPages.Item(iPage);
		ItemsMap::iterator it = page->m_mapVariables.find(name);

		if(it != page->m_mapVariables.end())
		{
			wxReportPageItem *pItem = it->second;
			pItem->m_pVariable = (const void*)&var;
			pItem->m_iValueType = varType;
			wxString valFormat;
			
			if(pItem->m_iValueType == wxRP_FLOAT || pItem->m_iValueType == wxRP_DOUBLE)
				valFormat = wxT("%G");
			else
				valFormat = wxT("%d");		
			
			pItem->m_sValue = wxString::Format(valFormat, var);
			retVal = true;
		}
		else
			retVal = false;
	}
	
	return retVal;
}

bool wxReportDocument::SetVariableForTableColumn(const wxString& name, int columnIndex, const wxArrayShort& array)
{
	return this->DoSetVariableForTable(name, false, columnIndex, array);
}

bool wxReportDocument::SetVariableForTableColumn(const wxString& name, int columnIndex, const wxArrayInt& array)
{
	return this->DoSetVariableForTable(name, false, columnIndex, array);
}

bool wxReportDocument::SetVariableForTableColumn(const wxString& name, int columnIndex, const wxArrayLong& array)
{
	return this->DoSetVariableForTable(name, false, columnIndex, array);
}

bool wxReportDocument::SetVariableForTableColumn(const wxString& name, int columnIndex, const wxArrayDouble& array)
{
	return this->DoSetVariableForTable(name, false, columnIndex, array);
}

bool wxReportDocument::SetVariableForTableColumn(const wxString& name, int columnIndex, const wxArrayString& array)
{
	return this->DoSetVariableForTable(name, false, columnIndex, array);
}

bool wxReportDocument::SetVariableForTableRow(const wxString& name, int rowIndex, const wxArrayShort& array)
{
	return this->DoSetVariableForTable(name, true, rowIndex, array);
}

bool wxReportDocument::SetVariableForTableRow(const wxString& name, int rowIndex, const wxArrayInt& array)
{
	return this->DoSetVariableForTable(name, true, rowIndex, array);
}

bool wxReportDocument::SetVariableForTableRow(const wxString& name, int rowIndex, const wxArrayLong& array)
{
	return this->DoSetVariableForTable(name, true, rowIndex, array);
}

bool wxReportDocument::SetVariableForTableRow(const wxString& name, int rowIndex, const wxArrayDouble& array)
{
	return this->DoSetVariableForTable(name, true, rowIndex, array);
}

bool wxReportDocument::SetVariableForTableRow(const wxString& name, int rowIndex, const wxArrayString& array)
{
	return this->DoSetVariableForTable(name, true, rowIndex, array);
}

template<class T> bool wxReportDocument::DoSetVariableForTable(const wxString& name, bool toRow, int index, const T& array)
{
	int nPages = this->m_arrPages.GetCount();
	
	for(int p=0; p<nPages; ++p)
	{
		wxReportPage *pPage = this->m_arrPages.Item(p);
		ItemsMap::iterator it = pPage->m_mapVariables.find(name);
		
		if(it != pPage->m_mapVariables.end())
		{
			if(it->second->GetType() == wxRP_TABLE)
			{
				wxReportTableItem *pTable = (wxReportTableItem*)(it->second);
				
				if(toRow)
					return pTable->SetVariableForRow(index, array);
				else
					return pTable->SetVariableForColumn(index, array);
			}			
		}
	}
	
	return false;
}

void wxReportDocument::DoDivideTable(const wxReportTableItem& tableItem, int pageIndex, int itemIndex)
{
	if( tableItem.m_sName != wxT("") )
	{
		// do not add table if its position exceeds page height...
		if( tableItem.m_position.y > this->m_pActivePage->m_style.GetSize().y - 
									( this->m_pActivePage->m_style.GetBottomMargin() + this->m_pActivePage->m_style.GetTopMargin() ) ) return;

		wxReportPage *pOldActPage = this->m_pActivePage; // store current active page
		int iOldActPage = this->m_iActivePageIndex;
		this->m_pActivePage = this->m_arrPages.Item(pageIndex); // set active page to page with the processed table
		this->m_iActivePageIndex = pageIndex;
		double endPosY = tableItem.m_position.y + this->m_pActivePage->m_style.GetTopMargin() + tableItem.GetRowHeight(-1); // bottom position of current table
		int nRows = tableItem.m_nRows;
		int nCols = tableItem.m_nColumns;
		wxReportTableItem *pTable = new wxReportTableItem();
		pTable->m_position.y = tableItem.m_position.y;
		pTable->m_columnsWidths.Add(tableItem.m_columnsWidths[0]);
		pTable->m_rowsHeights.Add(tableItem.m_rowsHeights[0]);
		bool firstTable = true; // tells if first table
		int nR = 0; // rows numbering in new table
		int startRow = tableItem.m_columnsSources.Item(0).m_iStartPos; // start row for each new table
		
		for(int r=0; r<nRows; ++r)
		{
			if(endPosY + tableItem.GetRowHeight(r) /*+ tableItem.GetCellsStyle().GetBorderThickness()*/ < this->m_pActivePage->m_style.GetSize().y - this->m_pActivePage->m_style.GetBottomMargin()) // check if the bottom table position fit in the actual page
			{
				pTable->AddRow();
				pTable->m_rowsHeights[pTable->m_rowsHeights.GetCount()-1] = tableItem.m_rowsHeights[r+1];//.Add(tableItem.m_rowsHeights[r+1]);
				pTable->m_rowsSources.Add(tableItem.m_rowsSources.Item(r));
				for(int c=0; c<nCols; ++c) // add columns to the row in new table
				{
					wxReportTableCell *pCell = new wxReportTableCell(*(tableItem.GetCell(r, c)));
					if(nR == 0)
						pTable->m_columnsWidths.Add(tableItem.m_columnsWidths[c+1]);
					pTable->m_cells.Item(nR)->Add(pCell);
				}
				endPosY += tableItem.GetRowHeight(r) /*+ tableItem.GetCellsStyle().GetBorderThickness()*/;
				++nR;
			}
			else // insert this table to the actual page
			{
				pTable->m_sName = tableItem.m_sName;
				//pTable->m_fVariable = tableItem.m_fVariable;
				pTable->m_cellsStyle = tableItem.m_cellsStyle;
				pTable->m_style = tableItem.m_style;
				pTable->m_nRows = nR;
				pTable->m_nColumns = tableItem.m_nColumns;
				pTable->m_fHasNext = (r < nRows);
				pTable->m_position.x = tableItem.m_position.x;
				pTable->m_position.y = tableItem.m_position.y;
				
				for(int rh=0; rh<nRows; ++rh) // inserts rows headers
				{
					wxReportTableCell *pHeaderOrig = tableItem.m_headersRow.Item(rh);
					wxReportTableCell *pHeader = NULL;
					
					if(pHeaderOrig)
						pHeader = new wxReportTableCell(*pHeaderOrig);
						
					pTable->m_headersRow.Add(pHeader);
				}
				
				for(int ch=0; ch<nCols; ++ch) // inserts columns headers
				{
					wxReportTableCell *pHeaderOrig = tableItem.m_headersColumn.Item(ch);
					wxReportTableCell *pHeader = NULL;
					
					if(pHeaderOrig)
						pHeader = new wxReportTableCell(*pHeaderOrig);
						
					pTable->m_headersColumn.Add(pHeader);
				}
				
				for(int c=0; c<nCols; ++c)
				{
					wxReportTableCellsSource src = tableItem.m_columnsSources.Item(c);
					src.m_iStartPos = startRow;
					pTable->m_columnsSources.Add(src);
				}
				
				if(!firstTable)
				{
					//pTable->m_position.y = this->m_pActivePage->m_style.GetTopMargin();
					pTable->m_fContinue = false;
				}
				
				if(firstTable && itemIndex != wxRP_AT_LAST)
					this->m_pActivePage->m_arItems.Insert(pTable, itemIndex);
				else
					this->m_pActivePage->m_arItems.Add(pTable);
				
				//if(pTable->IsVariable())
					this->m_pActivePage->m_mapVariables[pTable->m_sName] = pTable;
				
				this->AddPage();
				endPosY = tableItem.m_position.y + this->m_pActivePage->m_style.GetTopMargin() + tableItem.GetRowHeight(-1);
				
				if(r < nRows)
				{
					pTable->m_fHasNext = true;
					pTable = new wxReportTableItem(); // create new table for rest of the rows
					pTable->m_columnsWidths.Add(tableItem.m_columnsWidths[0]);
					pTable->m_rowsHeights.Add(tableItem.m_rowsHeights[0]);
				}
				
				firstTable = false;
				startRow += nR;
				nR = 0;
				--r;
			}
		}
		
		if(pTable->m_nRows > 0) // insert last table
		{
			pTable->m_sName = tableItem.m_sName;
			//pTable->m_fVariable = tableItem.m_fVariable;
			pTable->m_cellsStyle = tableItem.m_cellsStyle;
			pTable->m_style = tableItem.m_style;
			pTable->m_fContinue = !firstTable;
			pTable->m_fHasNext = false;
			
			for(int rh=0; rh<nRows; ++rh) // inserts rows headers
			{
				wxReportTableCell *pHeaderOrig = tableItem.m_headersRow.Item(rh);
				wxReportTableCell *pHeader = NULL;
				
				if(pHeaderOrig)
					pHeader = new wxReportTableCell(*pHeaderOrig);
						
				pTable->m_headersRow.Add(pHeader);
			}
				
			for(int ch=0; ch<nCols; ++ch) // inserts columns headers
			{
				wxReportTableCell *pHeaderOrig = tableItem.m_headersColumn.Item(ch);
				wxReportTableCell *pHeader = NULL;
					
				if(pHeaderOrig)
					pHeader = new wxReportTableCell(*pHeaderOrig);
						
				pTable->m_headersColumn.Add(pHeader);
			}
			
			for(int c=0; c<nCols; ++c)
			{
				wxReportTableCellsSource src(tableItem.m_columnsSources.Item(c));
				src.m_iStartPos = startRow;
				pTable->m_columnsSources.Add(src);
			}
			
			pTable->m_nRows = nR;
			pTable->m_nColumns = tableItem.m_nColumns;
			pTable->m_position.x = tableItem.m_position.x;
			pTable->m_position.y = tableItem.m_position.y;
			this->m_pActivePage->m_arItems.Add(pTable);
			
			//if(pTable->IsVariable())
				this->m_pActivePage->m_mapVariables[pTable->m_sName] = pTable;
		}
		
		this->m_pActivePage = pOldActPage;
		this->m_iActivePageIndex = iOldActPage;
	}
}

void wxReportDocument::AddPage(int count)
{
	if(count < 1)
		count = 1;

	for(int i=0; i<count; ++i)
	{
		wxReportPage *layout = new wxReportPage();
		layout->SetStyle(this->m_pageStyle);
		int ftItems = this->m_arFooter.GetCount();
		int hdItems = this->m_arHeader.GetCount();
		
		if(this->m_iActivePageIndex == (int)(this->m_arrPages.GetCount()) - 1 || (int)(this->m_arrPages.GetCount()) == 0)
		{
			layout->m_iPageNumber = ++m_iActivePageIndex;
			this->m_arrPages.Add(layout);
			this->m_pActivePage = this->m_arrPages.Last();
		}
		else
		{
			layout->m_iPageNumber = this->m_iActivePageIndex;
			this->m_arrPages.Insert(layout, this->m_iActivePageIndex + 1);
		}
		
		for(int f=0; f<ftItems; ++f)
		{
			wxReportPageItem *pItem = (wxReportPageItem*)(this->m_arFooter.Item(f));
			layout->AddItem(pItem, wxRP_FOOTER);
		}
		
		for(int h=0; h<hdItems; ++h)
		{
			wxReportPageItem *pItem = (wxReportPageItem*)(this->m_arHeader.Item(h));
			layout->AddItem(pItem, wxRP_HEADER);
		}
	}
}

void wxReportDocument::AddPage(const wxReportPage& layout)
{
	wxReportPage *pLay;
	
	if(this->m_iActivePageIndex == (int)(this->m_arrPages.GetCount()) - 1)
	{
		pLay = new wxReportPage(layout);
		this->m_arrPages.Add(pLay);
		this->m_pActivePage = this->m_arrPages.Last();
		this->m_iActivePageIndex++;
	}
	else
		this->m_arrPages.Insert(pLay, this->m_iActivePageIndex + 1);
}

void wxReportDocument::AddPageAt(int index, int count)
{
	if( index >= 0 || index <= (int)(this->m_arrPages.GetCount()) )
		for(int i=0; i<count; ++i)
			this->m_arrPages.Insert(new wxReportPage(), index);
}

void wxReportDocument::AddPageAt(const wxReportPage& layout, int index)
{
	if( index >= 0 || index <= (int)(this->m_arrPages.GetCount()) )
		
		this->m_arrPages.Insert(new wxReportPage(layout), index);
}

void wxReportDocument::ReplacePage(int index)
{
	this->m_arrPages.Item(index) = new wxReportPage();
}

void wxReportDocument::ReplacePage(const wxReportPage& layout, int index)
{
	this->m_arrPages.Item(index) = new wxReportPage(layout);
}

void wxReportDocument::DeleteAllPages()
{
	int nPages = this->m_arrPages.GetCount();
	int ftItems = this->m_arFooter.GetCount();
	int hdItems = this->m_arHeader.GetCount();
		
	for(int i=0; i<nPages; ++i)
		delete this->m_arrPages.Item(i);
		
	for(int i=0; i<ftItems; ++i)
		delete this->m_arFooter.Item(i);
		
	for(int i=0; i<hdItems; ++i)
		delete this->m_arHeader.Item(i);
		
	this->m_arrPages.Clear();
	this->m_arFooter.Clear();
	this->m_arHeader.Clear();
	
	this->m_pActivePage = NULL;
	this->m_iActivePageIndex = -1;
}

void wxReportDocument::DeletePage(int index)
{
	if( index >= 0 && index < (int)(this->m_arrPages.GetCount()) )
	{
		wxReportPage *page = this->m_arrPages.Item(index);
		if( page == m_pActivePage ) {
			m_pActivePage = NULL;
			m_iActivePageIndex = -1;
		}
		delete page;
		this->m_arrPages.RemoveAt(index);
	}
}

void wxReportDocument::DeletePages(int indexFrom, int indexTo)
{
	if( indexFrom >= 0 && indexTo >= 0 && indexFrom < indexTo )
	{
		for(int i=indexFrom; i<indexTo; ++i) {
			wxReportPage *page = this->m_arrPages.Item(i);
			if( page == m_pActivePage ) {
				m_pActivePage = NULL;
				m_iActivePageIndex = -1;
			}
			delete page;
		}
		this->m_arrPages.RemoveAt(indexFrom, indexTo - indexFrom);
	}
}

wxReportPage* wxReportDocument::GetPage(int pageNumber)
{
	if(pageNumber >= 0 && pageNumber < (int)(this->m_arrPages.Count()) )
	{
		return this->m_arrPages.Item(pageNumber);
	}
	else
		return NULL;
	
}

int wxReportDocument::GetPagesCount()
{
	return this->m_arrPages.GetCount();
}

void wxReportDocument::AddItem(const wxReportTextItem& textItem)
{
	this->m_pActivePage->AddItem(textItem);
}

void wxReportDocument::AddItem(const wxReportImageItem& imageItem)
{
	this->m_pActivePage->AddItem(imageItem);
}

void wxReportDocument::AddItem(const wxReportTableItem& tableItem) // divide passed table to the several pages
{
	// do not add table if its position exceeds page height...
	if( tableItem.m_position.y > this->m_pActivePage->m_style.GetSize().y - 
								( this->m_pActivePage->m_style.GetBottomMargin() + this->m_pActivePage->m_style.GetTopMargin() ) ) return;
	
	// ... otherwise determine whether the table should be spread over several pages
	double btmPos = tableItem.m_position.y + tableItem.GetTableHeight() + this->m_pActivePage->m_style.GetTopMargin();
	if(btmPos <= this->m_pActivePage->m_style.GetSize().y - this->m_pActivePage->m_style.GetBottomMargin())
		this->m_pActivePage->AddItem(tableItem);
	else
		this->DoDivideTable(tableItem, this->m_iActivePageIndex, wxRP_AT_LAST);
}

void wxReportDocument::AddItem(const wxReportLineShape& lineShape)
{
	this->m_pActivePage->AddItem(lineShape);
}

void wxReportDocument::AddItem(const wxReportRectangleShape& rectShape)
{
	this->m_pActivePage->AddItem(rectShape);
}

void wxReportDocument::AddItem(const wxReportCircleShape& circleShape)
{
	this->m_pActivePage->AddItem(circleShape);
}

void wxReportDocument::AddItemToHeader(const wxReportTextItem& textItem)
{
	wxReportTextItem *pItem = new wxReportTextItem(textItem);
	this->m_arHeader.Add(pItem);
	this->DoAddItemToHeader(pItem);
}

void wxReportDocument::AddItemToHeader(const wxReportTableItem& tableItem)
{
	wxReportTableItem *pItem = new wxReportTableItem(tableItem);
	this->m_arHeader.Add(pItem);
	this->DoAddItemToHeader(pItem);
}

void wxReportDocument::AddItemToHeader(const wxReportImageItem& imageItem)
{
	wxReportImageItem *pItem = new wxReportImageItem(imageItem);
	this->m_arHeader.Add(pItem);
	this->DoAddItemToHeader(pItem);
}

void wxReportDocument::AddItemToHeader(const wxReportLineShape& lineShape)
{
	wxReportLineShape *pItem = new wxReportLineShape(lineShape);
	this->m_arHeader.Add(pItem);
	this->DoAddItemToHeader(pItem);
}

void wxReportDocument::AddItemToHeader(const wxReportRectangleShape& rectShape)
{
	wxReportRectangleShape *pItem = new wxReportRectangleShape(rectShape);
	this->m_arHeader.Add(pItem);
	this->DoAddItemToHeader(pItem);
}

void wxReportDocument::AddItemToHeader(const wxReportCircleShape& circleShape)
{
	wxReportCircleShape *pItem = new wxReportCircleShape(circleShape);
	this->m_arHeader.Add(pItem);
	this->DoAddItemToHeader(pItem);
}

void wxReportDocument::DoAddItemToHeader(wxReportPageItem* pItem)
{
	int nPages = this->m_arrPages.GetCount();
	for(int p=0; p<nPages; ++p)
	{
		wxReportPage *pPage = this->m_arrPages.Item(p);
		pPage->AddItem(pItem, wxRP_HEADER);
	}
}

void wxReportDocument::AddItemToFooter(const wxReportTextItem& textItem)
{
	wxReportTextItem *pItem = new wxReportTextItem(textItem);
	this->m_arFooter.Add(pItem);
	this->DoAddItemToFooter(pItem);
}

void wxReportDocument::AddItemToFooter(const wxReportTableItem& tableItem)
{
	wxReportTableItem *pItem = new wxReportTableItem(tableItem);
	this->m_arFooter.Add(pItem);
	this->DoAddItemToFooter(pItem);
}

void wxReportDocument::AddItemToFooter(const wxReportImageItem& imageItem)
{
	wxReportImageItem *pItem = new wxReportImageItem(imageItem);
	this->m_arFooter.Add(pItem);
	this->DoAddItemToFooter(pItem);
}

void wxReportDocument::AddItemToFooter(const wxReportLineShape& lineShape)
{
	wxReportLineShape *pItem = new wxReportLineShape(lineShape);
	this->m_arFooter.Add(pItem);
	this->DoAddItemToFooter(pItem);
}

void wxReportDocument::AddItemToFooter(const wxReportRectangleShape& rectShape)
{
	wxReportRectangleShape *pItem = new wxReportRectangleShape(rectShape);
	this->m_arFooter.Add(pItem);
	this->DoAddItemToFooter(pItem);
}

void wxReportDocument::AddItemToFooter(const wxReportCircleShape& circleShape)
{
	wxReportCircleShape *pItem = new wxReportCircleShape(circleShape);
	this->m_arFooter.Add(pItem);
	this->DoAddItemToFooter(pItem);
}

void wxReportDocument::DoAddItemToFooter(wxReportPageItem* pItem)
{
	int nPages = this->m_arrPages.GetCount();
	for(int p=0; p<nPages; ++p)
	{
		wxReportPage *pPage = this->m_arrPages.Item(p);
		pPage->AddItem(pItem, wxRP_FOOTER);
	}
}

void wxReportDocument::AddItemToAllPages(wxReportPageItem *item)
{
	int cnt = this->m_arrPages.GetCount() - 1;
	this->DoChangeItem(0, cnt, item);
}

void wxReportDocument::AddItemToPage(wxReportPageItem *item, int pageNumber)
{
	this->DoChangeItem(pageNumber, pageNumber, item);
}

void wxReportDocument::AddItemToPages(wxReportPageItem *item, int indexFrom, int indexTo)
{
	this->DoChangeItem(indexFrom, indexTo, item);
}

void wxReportDocument::ReplaceItemByName(const wxString& replacedItemName, wxReportPageItem *newItem)
{
	int cnt = this->m_arrPages.GetCount() - 1;
	this->DoChangeItem(0, cnt, newItem, replacedItemName);
}

void wxReportDocument::ReplaceItemByName(const wxString& replacedItemName, wxReportPageItem *newItem, int pageNumber)
{
	this->DoChangeItem(pageNumber, pageNumber, newItem, replacedItemName);
}

void wxReportDocument::ReplaceItemByName(const wxString& replacedItemName, wxReportPageItem *newItem, int indexFrom, int indexTo)
{
	this->DoChangeItem(indexFrom, indexTo, newItem, replacedItemName);
}

void wxReportDocument::DeleteItemByName(const wxString& itemName)
{
	int cnt = this->m_arrPages.GetCount() - 1;
	this->DoChangeItem(0, cnt, NULL, itemName);
}

void wxReportDocument::DeleteItemByName(const wxString& itemName, int pageNumber)
{
	//int cnt = this->m_arrPages.GetCount() - 1;
	this->DoChangeItem(pageNumber, pageNumber, NULL, itemName);
}

void wxReportDocument::DeleteItemByName(const wxString& itemName, int indexFrom, int indexTo)
{
	//int cnt = this->m_arrPages.GetCount() - 1;
	this->DoChangeItem(indexFrom, indexTo, NULL, itemName);
}

void wxReportDocument::DoChangeItem(int from, int to, wxReportPageItem* item, const wxString& itemName)
{
	if(itemName == wxEmptyString) //vlozi novy
	{
		for(int i=from; i<=to; ++i)
		{
//			this->m_arrPages.Item(i).AddItem(item);
		}

		return;
	}

	if(item == NULL) //smaze item
	{
		for(int i=from; i<=to; ++i)
		{
			this->m_arrPages.Item(i)->DeleteItem(itemName);
		}

		return;
	}

	if(item != NULL && itemName != wxEmptyString) // nahradi item
	{
		for(int p=from; p<=to; ++p)
		{
			wxReportPage *pPage = this->m_arrPages.Item(p);
			int itemIndex = pPage->FindIndex(itemName);
			delete pPage->m_arItems[itemIndex];
			pPage->m_arItems[itemIndex] = item;
			
			if(item->IsVariable())
				pPage->m_mapVariables[itemName] = item;
		}
	}
}

void wxReportDocument::InsertPageNumbering(const wxString& format, const wxReportTextStyle& textStyle, double posX, double posY, int atPlace, int fromPage)
{
	wxString strFormat = format;
	int numPos = strFormat.Find(wxT("%d"));
	strFormat.insert(numPos, 1, wxT('\t'));
	strFormat.insert(numPos+3, 1, wxT('\t'));
	wxReportTextItem pageNum(wxT("PageNumber"), posX, posY);
	pageNum.SetActiveTextStyle(textStyle);
	pageNum.SetTextAlign(wxRP_CENTERALIGN);
	pageNum.AddText(strFormat);
	
	switch(atPlace)
	{
		case wxRP_HEADER:
			this->AddItemToHeader(pageNum);
			break;
			
		case wxRP_BODY:
			this->AddItem(pageNum);
			break;
			
		case wxRP_FOOTER:
			this->AddItemToFooter(pageNum);
	}
}

/*void wxReportDocument::RemovePageNumbering()
{
	this->DoRemovePageNumbering(0, this->m_arrPages.GetCount());
}

void wxReportDocument::RemovePageNumbering(int pageFrom, int pageTo)
{
	if(pageFrom >= 0 && pageTo < (int)(this->m_arrPages.GetCount()))
		this->DoRemovePageNumbering(pageFrom, pageTo);
}

void wxReportDocument::DoRemovePageNumbering(int from, int to)
{
	for(int i=from; i<=to; ++i)
	{
		wxReportPage *page = this->m_arrPages.Item(from);
		page->m_fPageNumberActive = false;
		page->m_pageNumber.ClearAllText();
	}
}*/

/*void wxReportDocument::SetUnits(int unitType)
{
	wxReportUnit::SetUnitsRatio(unitType);
	m_iUnitType = unitType;
}*/

void wxReportDocument::AddOutputHandler(wxReportFileHandler* handler)
{
	this->m_arrHandlers.Add(handler);
	handler = NULL;
}

wxReportFileHandler* wxReportDocument::GetHandlerBySuffix(const wxString& suffix)
{
	wxReportFileHandler *pHandler = NULL;
	int handlersCount = this->m_arrHandlers.GetCount();
	
	for(int i=0; i<handlersCount; ++i)
	{
		pHandler = this->m_arrHandlers.Item(i);
		
		if(pHandler->GetFormatSuffix() == suffix)
			break;
		else
			pHandler = NULL;
	}
	
	return pHandler;
}

void wxReportDocument::SetPrintData(const wxPrintData& printData)
{
	this->m_printData = printData;
}

const wxPrintData& wxReportDocument::GetPrintData()
{
	return this->m_printData;
}

void wxReportDocument::SetDPI(int dpi)
{
	if(dpi > 0)
		this->m_iDPI = dpi;
}

const int& wxReportDocument::GetDPI()
{
	return this->m_iDPI;
}

void wxReportDocument::Print(wxWindow *parent)
{
	if(this->m_fAutoRefresh)
		this->RefreshVariables();
	
	//this->m_printData.SetPaperId(wxPAPER_LETTER);
	this->m_printData.SetPrintMode(wxPRINT_MODE_PRINTER);
	
	wxPrintDialogData printDlgData(this->m_printData);
	printDlgData.SetMinPage(1);
	printDlgData.SetMaxPage(this->m_arrPages.GetCount());
	
	wxReportPrintout printout(wxT(""));
	printout.SetPages(&(this->m_arrPages));
	
	wxPrinter printer;
	printer.Print(parent, &printout, true);
}

void wxReportDocument::ShowPrintPreview(wxWindow *parent, const wxSize& size, int zoom, const wxString& title, bool maximized)
{
	if(this->m_fAutoRefresh)
		this->RefreshVariables();
	
	wxReportPrintout *printout1 = new wxReportPrintout(wxT(""));
	wxReportPrintout *printout2 = new wxReportPrintout(wxT(""));
	printout1->SetPages(&m_arrPages);
	printout2->SetPages(&m_arrPages);
	
	wxPrintDialogData data(m_printData);
	data.SetMinPage(1);
	data.SetMaxPage(this->m_arrPages.GetCount());
	data.SetFromPage(1);
	data.SetToPage(this->m_arrPages.GetCount());
	
	wxPrintPreview *preview = new wxPrintPreview(printout1, printout2, &data);
	if (!preview->IsOk())
	{
		delete preview;
		wxMessageBox(wxT("There was a problem with previewing.\nPerhaps your current printer is not set correctly?"), wxT("Previewing"), wxOK);
		return;
	} 
	preview->SetZoom( zoom );
	
	wxPreviewFrame *frame = new wxPreviewFrame( preview, parent, title );
	if( size.IsFullySpecified() ) frame->SetSize( size );
	frame->Centre( wxBOTH );
	frame->Maximize( maximized );
	frame->Initialize();
	frame->Show();
}

bool wxReportDocument::SaveReport(const wxString& fileName, int formatId)
{
	wxReportFileHandler *handler;
	wxString suffix;
	
	switch(formatId)
	{
		default:
			suffix = fileName.AfterLast('.');
			if(suffix != fileName)
			{
				handler = this->GetHandlerBySuffix(suffix);
				if(handler == NULL)
					return false;
			}
			else
				return false;
	}
	
	handler->SaveToFile(&(this->m_arrPages), fileName, this->m_iDPI, this->m_printData);
	return true;
}

void wxReportDocument::RefreshVariables()
{
	for(int p=0; p<(int)(m_arrPages.GetCount()); ++p)
	{	
		wxReportPage *pPage = this->m_arrPages.Item(p);
		
		this->m_pActivePage = pPage;
		pPage->RefreshVariables();
		
		if(pPage->m_fDelete) // delete page if it's marked to delete
		{
			delete pPage;
			this->m_arrPages.RemoveAt(p--);
			this->m_pActivePage = this->m_arrPages.Last();
			this->m_iActivePageIndex = this->m_arrPages.GetCount() - 1;
			continue;
		}
		
		ItemsArray *pItems = &(pPage->m_arItems);
		int nItems = pItems->GetCount();
		
		for(int i=0; i<nItems; ++i)
		{
			if(pItems->Item(i)->m_iType == wxRP_TABLE)
			{
				wxReportTableItem *pTable = (wxReportTableItem*)(pItems->Item(i));
				double btmPos = pTable->m_position.y + pTable->GetTableHeight() + this->m_pActivePage->m_style.GetTopMargin();
								
				if(btmPos > this->m_pActivePage->m_style.GetSize().y /*+ this->m_pActivePage->m_style.GetTopMargin()*/ - this->m_pActivePage->m_style.GetBottomMargin() || pTable->m_nRows >= (int)(this->m_pActivePage->m_style.GetSize().y))
				{
					if(pTable->m_fHasNext) // if the table has continue and doesn't fit to this page, fill it with all values from the array and destroy the rest of tables on the next pages.
					{
						pTable->m_fHasNext = false;
						pTable->RefreshVariable();
						wxString name = pTable->m_sName;
						for(int p1=p+1; p1<(int)(m_arrPages.GetCount()); ++p1)
							((wxReportTableItem*)(this->m_arrPages.Item(p1)->m_mapVariables[name]))->m_fDelete = true;
					}
					
					int toEndPos = m_arrPages.GetCount() - p;
					pPage->m_arItems.RemoveAt(i);
					this->DoDivideTable(*pTable, p, i);	
					delete pTable;
					p = m_arrPages.GetCount() - toEndPos;
				}
			}
		}		
	}
}

void wxReportDocument::SetAutoRefresh(bool refresh)
{
	this->m_fAutoRefresh = refresh;
}

bool wxReportDocument::SaveLayoutToXML(wxOutputStream& out)
{
	if(this->m_fAutoRefresh)
		this->RefreshVariables();
	
	wxXmlDocument xmlDoc;
	wxXmlNode *rootNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("wxReport"));
	rootNode->AddAttribute(wxT("UnitsType"), wxString::Format(wxT("%d"), this->m_iUnitType));
	
	wxXmlNode *headerNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("HeaderItems"));
	int hdItems = this->m_arHeader.GetCount();
	for(int h=0; h<hdItems; ++h)
	{
		wxXmlNode *hdItemNode = this->m_arHeader.Item(h)->CreateXmlNode();
		if(hdItemNode)
			headerNode->AddChild(hdItemNode);
	}
	rootNode->AddChild(headerNode);
	
	wxXmlNode *footerNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("FooterItems"));
	int ftItems = this->m_arFooter.GetCount();
	for(int f=0; f<ftItems; ++f)
	{
		wxXmlNode *ftItemNode = this->m_arFooter.Item(f)->CreateXmlNode();
		if(ftItemNode)
			footerNode->AddChild(ftItemNode);
	}
	rootNode->AddChild(footerNode);
	
	wxXmlNode *pagesNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Pages"));
	int pagesCount = this->m_arrPages.GetCount();

	for(int i=0; i<pagesCount; ++i)
	{
		wxXmlNode *pageNode = this->m_arrPages.Item(i)->CreateXmlNode();

		if(pageNode)
			pagesNode->AddChild(pageNode);
	}
	
	rootNode->AddChild(pagesNode);
	xmlDoc.SetRoot(rootNode);
	return xmlDoc.Save(out);
}

bool wxReportDocument::LoadLayoutFromXML(wxInputStream& in)
{
	this->DoDestroy();
	
	wxXmlDocument xmlDoc;
	if( !xmlDoc.Load(in) ) return false;

	wxXmlNode *rootNode = xmlDoc.GetRoot();
	
	int unitType = 0;
	wxSscanf(rootNode->GetAttribute(wxT("UnitsType"), wxT("")), wxT("%d"), &unitType);
	if(unitType >= 0 && unitType <= 1)
		this->m_iUnitType = unitType;
		
	wxXmlNode *headerNode = rootNode->GetChildren();
	wxXmlNode *headerItemNode = headerNode->GetChildren();	
	while(headerItemNode)
	{
		long itemType = -1;
		wxReportPageItem *item = NULL;
		
		headerItemNode->GetAttribute(wxT("Type"), wxT("")).ToLong(&itemType);
		
		switch(itemType)
		{
			case wxRP_TEXT:
				item = new wxReportTextItem();
				break;
				
			case wxRP_TABLE:
				item = new wxReportTableItem();
				break;
				
			case wxRP_IMAGE:
				item = new wxReportImageItem();
				break;
				
			case wxRP_LINESHAPE:
				item = new wxReportLineShape();
				break;
				
			case wxRP_RECTANGLESHAPE:
				item = new wxReportRectangleShape();
				break;
				
			case wxRP_CIRCLESHAPE:
				item = new wxReportCircleShape();
				break;
		}
		
		if(item)
			if(item->RetrieveFromXmlNode(headerItemNode))
				this->m_arHeader.Add(item);
		
		headerItemNode = headerItemNode->GetNext();
	}
	
	wxXmlNode *footerNode = headerNode->GetNext();
	wxXmlNode *footerItemNode = footerNode->GetChildren();	
	while(footerItemNode)
	{
		long itemType = -1;
		wxReportPageItem *item = NULL;
		
		footerItemNode->GetAttribute(wxT("Type"), wxT("")).ToLong(&itemType);
		
		switch(itemType)
		{
			case wxRP_TEXT:
				item = new wxReportTextItem();
				break;
				
			case wxRP_TABLE:
				item = new wxReportTableItem();
				break;
				
			case wxRP_IMAGE:
				item = new wxReportImageItem();
				break;
				
			case wxRP_LINESHAPE:
				item = new wxReportLineShape();
				break;
				
			case wxRP_RECTANGLESHAPE:
				item = new wxReportRectangleShape();
				break;
				
			case wxRP_CIRCLESHAPE:
				item = new wxReportCircleShape();
				break;
		}
		
		if(item)
			if(item->RetrieveFromXmlNode(footerItemNode))
				this->m_arFooter.Add(item);
		
		footerItemNode = footerItemNode->GetNext();
	}
	
	wxXmlNode *pageNode = footerNode->GetNext()->GetChildren();
	int p = 0;
	int hdItems = this->m_arHeader.GetCount();
	int ftItems = this->m_arFooter.GetCount();

	while(pageNode)
	{
		wxReportPage *page = new wxReportPage();

		if(page->RetrieveFromXmlNode(pageNode))
		{
			page->m_iPageNumber = p;
			for(int f=0; f<ftItems; ++f) // add header and footer items to the new page
				page->AddItem(this->m_arFooter.Item(f), wxRP_FOOTER);
			
			for(int h=0; h<hdItems; ++h)
				page->AddItem(this->m_arHeader.Item(h), wxRP_HEADER);
			
			this->m_arrPages.Add(page);
		}
		
		++p;
		pageNode = pageNode->GetNext();
	}

	return true;
}

bool wxReportDocument::SaveLayoutToXML(const wxString& path)
{
	wxFileOutputStream fout( path );
	if( fout.IsOk() ) {
		return SaveLayoutToXML( fout );
	} else 
		return false;
}

bool wxReportDocument::LoadLayoutFromXML(const wxString& path)
{
	wxFileInputStream fin( path );
	if( fin.IsOk() ) {
		return LoadLayoutFromXML( fin );
	} else
		return false;
}

const wxString& wxReportDocument::GetVersion()
{
	return m_sVersion;
}
