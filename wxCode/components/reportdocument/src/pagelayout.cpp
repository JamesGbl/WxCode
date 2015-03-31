#include "wx/report/pagelayout.h"
#include "wx/arrimpl.cpp"

WX_DEFINE_USER_EXPORTED_OBJARRAY(PagesArray);

wxReportPage::wxReportPage()
{
	this->m_fPageNumberActive = false;
	this->m_fDelete = false;
}

wxReportPage::wxReportPage(const wxReportPage& sourcePage)
{
	this->DoCopy(sourcePage);
}

wxReportPage::~wxReportPage()
{
	int nItems = this->m_arItems.GetCount();
	int ftItems = this->m_arFooter.GetCount();
	int hdItems = this->m_arHeader.GetCount();
	
	for(int i=0; i<ftItems; ++i)
		delete this->m_arFooter.Item(i);
		
	for(int i=0; i<hdItems; ++i)
		delete this->m_arHeader.Item(i);
	
	for(int i=0; i<nItems; ++i)
		delete this->m_arItems.Item(i);
		
	this->m_arItems.Clear();
	this->m_arFooter.Clear();
	this->m_arHeader.Clear();
	this->m_mapVariables.clear();
}

wxReportPage& wxReportPage::operator= (const wxReportPage& sourcePage)
{
	if(this == &sourcePage)
		return *this;
		
	this->DoCopy(sourcePage);
	return *this;
}

void wxReportPage::DoCopy(const wxReportPage& sourcePage)
{
	this->m_fPageNumberActive = sourcePage.m_fPageNumberActive;
	this->m_fDelete = sourcePage.m_fDelete;
	this->m_pageNumber = sourcePage.m_pageNumber;
	this->m_style = sourcePage.m_style;
	int itemsCount = sourcePage.m_arItems.GetCount();
	
	for(int i=0; i<itemsCount; ++i)
	{
		wxReportPageItem *pItem = sourcePage.m_arItems.Item(i);
		
		switch(pItem->m_iType)
		{
			case wxRP_TEXT:
				this->AddItem(*((wxReportTextItem*)pItem));
				break;
				
			case wxRP_TABLE:
				this->AddItem(*((wxReportTableItem*)pItem));
				break;
				
			case wxRP_IMAGE:
				this->AddItem(*((wxReportImageItem*)pItem));
				break;
				
			case wxRP_LINESHAPE:
				this->AddItem(*((wxReportLineShape*)pItem));
				break;
				
			case wxRP_RECTANGLESHAPE:
				this->AddItem(*((wxReportRectangleShape*)pItem));
				break;
				
			case wxRP_CIRCLESHAPE:
				this->AddItem(*((wxReportCircleShape*)pItem));
				break;
		}
	}
}

int wxReportPage::FindIndex(const wxString& name) const
{
	int itemsCount = this->m_arItems.GetCount();
	
	for(int i=0; i<itemsCount; ++i)
	{
		if(this->m_arItems.Item(i)->GetName() == name)
			return i;
	}
	
	return -1;
}

void wxReportPage::AddRemoveVariables(wxReportTextItem* pItem, bool add)
{
	int parCount = pItem->m_arParagraphs.GetCount();
	
	for(int p=0; p<parCount; ++p)
	{
		wxReportTextParagraph *pPar = pItem->m_arParagraphs.Item(p);
		int textCount = pPar->m_arTextValues.GetCount();
		
		for(int t=0; t<textCount; ++t)
		{
			wxReportTextValue *pText = pPar->m_arTextValues.Item(t);
			if(pText->m_fVariable && pText->m_sName != wxT(""))
			{
				if(add)
					this->m_mapVariables[pText->m_sName] = pText;
				else
					this->m_mapVariables.erase(pText->m_sName);
			}
		}
	}
}

void wxReportPage::AddRemoveVariables(wxReportTableItem* pItem, bool add)
{
	int rowsCount = pItem->m_nRows;
	
	for(int r=0; r<rowsCount; ++r)
	{
		CellsArray *pRow = pItem->m_cells.Item(r);
		
		if(pRow)
		{
			for(int c=0; c<pItem->m_nColumns; ++c)
			{
				wxReportTableCell *pCell = pRow->Item(c);
				if(pCell)
				{
					if(pCell->IsVariable())
					{
						if(pCell->GetName() != wxT(""))
						{
							if(add)
								this->m_mapVariables[pCell->GetName()] = pCell;
							else
								this->m_mapVariables.erase(pCell->GetName());
						}
					}
				}
			}
		}
	}
}

void wxReportPage::SetStyle(const wxReportPageStyle& style)
{
	this->m_style = style;
}

void wxReportPage::AddItem(const wxString& name)
{
	wxReportPageItem *pItem = new wxReportTextItem();
	pItem->SetName(name);
	this->m_arItems.Add(pItem);
}

void wxReportPage::AddItem(const wxReportTextItem& item)
{
	wxReportTextItem *pItem = new wxReportTextItem(item);
	this->AddRemoveVariables(pItem);
	this->m_arItems.Add(pItem);
}

void wxReportPage::AddItem(const wxReportImageItem& item)
{
	wxReportPageItem *pItem = new wxReportImageItem(item);
	
	if(pItem->m_fVariable && pItem->m_sName != wxT(""))
		this->m_mapVariables[pItem->m_sName] = pItem;
		
	this->m_arItems.Add(pItem);
}

void wxReportPage::AddItem(const wxReportTableItem& item)
{
	wxReportTableItem *pItem = new wxReportTableItem(item);
	
	if(pItem->m_sName != wxT("") && !this->m_mapVariables.count(pItem->m_sName)/* && pItem->IsVariable()*/)
		this->m_mapVariables[pItem->m_sName] = pItem;
	
	this->m_arItems.Add(pItem);
}

void wxReportPage::AddItem(const wxReportLineShape& lineShape)
{
	wxReportLineShape *pItem = new wxReportLineShape(lineShape);
	this->m_arItems.Add(pItem);
}

void wxReportPage::AddItem(const wxReportRectangleShape& rectShape)
{
	wxReportRectangleShape *pItem = new wxReportRectangleShape(rectShape);
	this->m_arItems.Add(pItem);
}

void wxReportPage::AddItem(const wxReportCircleShape& circleShape)
{
	wxReportCircleShape *pItem = new wxReportCircleShape(circleShape);
	this->m_arItems.Add(pItem);
}

void wxReportPage::AddItem(const wxReportPageItem* item, int place)
{
	wxReportPageItem *pItem = NULL;
		
	switch(item->m_iType)
	{
		case wxRP_TEXT:
			pItem = new wxReportTextItem(*((wxReportTextItem*)item));
			this->AddRemoveVariables((wxReportTextItem*)pItem);
			break;
			
		case wxRP_TABLE:
			pItem = new wxReportTableItem(*((wxReportTableItem*)item));
			break;
				
		case wxRP_IMAGE:
			pItem = new wxReportImageItem(*((wxReportImageItem*)item));
			break;
				
		case wxRP_LINESHAPE:
			pItem = new wxReportLineShape(*((wxReportLineShape*)item));
			break;
				
		case wxRP_RECTANGLESHAPE:
			pItem = new wxReportRectangleShape(*((wxReportRectangleShape*)item));
			break;
				
		case wxRP_CIRCLESHAPE:
			pItem = new wxReportCircleShape(*((wxReportCircleShape*)item));
			break;
	}
	
	if(pItem)
	{
		if(pItem->m_sName == wxT("PageNumber"))
		{
			this->m_arPageNumberItems.Add(pItem);
			this->RefreshPageNumbers();
		}
			
		switch(place)
		{
			case wxRP_HEADER:
				this->m_arHeader.Add(pItem);
				break;
				
			case wxRP_FOOTER:
				this->m_arFooter.Add(pItem);
				break;
				
			case wxRP_BODY:
				this->m_arItems.Add(pItem);
				break;
		}
	}
}

void wxReportPage::AddItemAt(const wxReportImageItem& item, int index)
{
	if( index >= 0 && index <= (int)(this->m_arItems.GetCount()) )
	{
		wxReportImageItem *pItem = new wxReportImageItem(item);
		if(pItem->m_fVariable && pItem->m_sName != wxT(""))
		{
			this->m_mapVariables[pItem->m_sName] = pItem;
		}
		this->m_arItems.Insert(pItem, index);
	}
}

void wxReportPage::AddItemAt(const wxReportTableItem& item, int index)
{
	if( index >= 0 && index <= (int)(this->m_arItems.GetCount()) )
	{
		wxReportTableItem *pItem = new wxReportTableItem(item);
		if(pItem->m_sName != wxT("") && this->m_mapVariables.count(pItem->m_sName))
			this->m_mapVariables[pItem->m_sName] = pItem;
		this->m_arItems.Insert(pItem, index);
	}
}

void wxReportPage::AddItemAt(const wxReportTextItem& item, int index)
{
	if( index >= 0 && index <= (int)(this->m_arItems.GetCount()) )
	{
		wxReportTextItem *pItem = new wxReportTextItem(item);
		this->AddRemoveVariables(pItem);
		this->m_arItems.Insert(pItem, index);
	}
}

void wxReportPage::DeleteItem(const wxString& name)
{
	int i = this->FindIndex(name);	
	this->DeleteItemAt(i);
}

void wxReportPage::DeleteItemAt(int index)
{
	if(index >= 0 && index < (int)(this->m_arItems.GetCount()))
	{
		wxReportPageItem *pItem = this->m_arItems.Item(index);
		
		if(pItem->m_fVariable || pItem->m_iType == wxRP_TABLE)
		{
			switch(pItem->m_iType)
			{
				case wxRP_TEXT:
					this->AddRemoveVariables((wxReportTextItem*)pItem, false);
					break;
					
				case wxRP_TABLE:
					this->m_mapVariables.erase(pItem->m_sName);
					break;
					
				case wxRP_IMAGE:
					this->m_mapVariables.erase(pItem->m_sName);
			}
		}
		
		delete this->m_arItems.Item(index);
		this->m_arItems.RemoveAt(index);
	}
}

int wxReportPage::GetCount() const
{
	return this->m_arItems.GetCount();
}

const wxReportPageItem* wxReportPage::GetItem(const wxString& name) const
{
	int i = this->FindIndex(name);
	
	if(i >= 0)
		return this->m_arItems.Item(i);
		
	return NULL;
}

const wxReportPageItem* wxReportPage::GetItem(int index) const
{
	if( index >= 0 && index < (int)(this->m_arItems.GetCount()) )
		return this->m_arItems.Item(index);
		
	return NULL;
}

wxString wxReportPage::GetPageNumberString() const
{
	if(this->m_fPageNumberActive)
		return this->m_pageNumber.GetTextValue(0)->GetValue();
	else
		return wxT("");
}

wxReportPageStyle& wxReportPage::GetStyle()
{
	return this->m_style;
}

void wxReportPage::RefreshVariables()
{
	for(ItemsMap::iterator it = this->m_mapVariables.begin(); it != this->m_mapVariables.end(); ++it)
	{
		wxReportPageItem *pItem = it->second;
		
		if(pItem->GetType() == wxRP_TABLE)
		{
			wxReportTableItem *pTable = (wxReportTableItem*)pItem;
			bool removeTab = true;
			int nSrc = pTable->m_columnsSources.GetCount();
			
			for(int s=0; s<nSrc && !(pTable->m_fDelete); ++s) // check if the table should be deleted - columns
			{
				wxReportTableCellsSource *src = &(pTable->m_columnsSources.Item(s));
				if(src->m_iStartPos <= src->GetActualArrayCount() /*&&*/|| src->m_pArrSource /*!=*/== NULL)
				{
					removeTab = false;
					break;
				}
			}
			
			nSrc = pTable->m_rowsSources.GetCount();
			if(removeTab && !(pTable->m_fDelete)) // check if the table should be deleted - rows
			{
				for(int s=0; s<nSrc; ++s)
				{
					wxReportTableCellsSource *src = &(pTable->m_rowsSources.Item(s));
					if(src->m_pArrSource == NULL && removeTab)
						continue;
					
					if(src->m_iStartPos <= src->GetActualArrayCount()/* || (src->m_pArrSource !=== NULL && !removeTab)*/)
					{
						removeTab = false;
						break;
					}
				}
			}
			
			if(removeTab || pTable->m_fDelete)
			{
				this->DeleteItem(pTable->m_sName);
				if((int)(this->m_arItems.GetCount()) == 0) // if the page is empty than set delete flag to true
				{
					this->m_fDelete = true;
					break;
				}
				continue;
			}
		}
		
		pItem->RefreshVariable();
	}
	
	this->RefreshPageNumbers();
}

void wxReportPage::RefreshPageNumbers()
{
	int nItems = this->m_arPageNumberItems.GetCount();
	
	for(int i=0; i<nItems; ++i)
	{
		wxReportTextItem *pItem = (wxReportTextItem*)(this->m_arPageNumberItems[i]);
		wxReportTextValue *pText = (wxReportTextValue*)(pItem->m_arParagraphs[0]->m_arTextValues[0]);
		int startPos = pText->m_sValue.Find(wxT('\t')) + 1;
		int endPos = pText->m_sValue.find(wxT('\t'), startPos);
		pText->m_sValue.Remove(startPos, endPos-startPos);
		pText->m_sValue.insert(startPos, wxString::Format(wxT("%d"), this->m_iPageNumber+1));
		
		wxFont font(pText->m_style.GetFont());
		wxScreenDC dc;
		int w, h;
		dc.GetTextExtent(pText->m_sValue, &w, &h, NULL, NULL, &font);
		pItem->m_dWidth = 25.4 * (w / (double)(dc.GetPPI().x)) + 5;
	}
}

wxXmlNode* wxReportPage::CreateXmlNode()
{
	wxXmlNode *pageNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Page"));
	pageNode->AddProperty(this->m_style.CreateXmlNode()->GetAttributes());
		
	int count = this->m_arItems.GetCount();
		
	for(int i=0; i<count; ++i)
	{
		pageNode->AddChild(this->m_arItems.Item(i)->CreateXmlNode());
	}
		
	return pageNode;
}

bool wxReportPage::RetrieveFromXmlNode(const wxXmlNode* node)
{
	if(node->GetName() != wxT("Page")) return false;
	
	if(!this->m_style.RetrieveFromXmlNode(node)) return false;	
	wxXmlNode *itemNode = node->GetChildren();
	
	while(itemNode)
	{
		long itemType = -1;
		wxReportPageItem *item;
		
		itemNode->GetAttribute(wxT("Type"), wxT("")).ToLong(&itemType);
		
		switch(itemType)
		{
			case wxRP_TEXT:
				item = new wxReportTextItem();
				break;
				
			case wxRP_IMAGE:
				item = new wxReportImageItem();
				break;
				
			case wxRP_TABLE:
				item = new wxReportTableItem();
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
				
			default:
				return false;
		}
		
		if(item->RetrieveFromXmlNode(itemNode))
		{
			switch(item->m_iType)
			{
				case wxRP_TEXT:
					this->AddRemoveVariables((wxReportTextItem*)item);
					break;
					
				case wxRP_TABLE:
					if(item->m_sName != wxT(""))
						this->m_mapVariables[item->m_sName] = item;
					break;
					
				case wxRP_IMAGE:
					if(item->m_fVariable)
						this->m_mapVariables[item->m_sName] = item;
			}
						
			this->m_arItems.Add(item);
		}
		else
			return false;
		
		itemNode = itemNode->GetNext();
	}
	
	return true;
}

void wxReportPage::DrawToDC(wxDC* dc, bool toScreen)
{
	/*wxSize pageSize = dc->GetSizeMM();
	wxSize pxSize = dc->GetSize();*/
	
	wxColour bgColor = this->m_style.GetBackgroundColor();
		
	if(this->m_style.GetBorder() > 0)
	{
		int lm = MM2PX(this->m_style.GetLeftMargin(), dc, toScreen);
		int rm = MM2PX(this->m_style.GetRightMargin(), dc, toScreen);
		int tm = MM2PX(this->m_style.GetTopMargin(), dc, toScreen);
		int bm = MM2PX(this->m_style.GetBottomMargin(), dc, toScreen);
		int w = MM2PX(this->m_style.GetSize().x, dc, toScreen);
		int h = MM2PX(this->m_style.GetSize().y, dc, toScreen);
		
		int bx = lm;
		int by = tm;
		int bw = w - lm - rm;
		int bh = h - tm - bm;
		
		int borderWidth = MM2PX(this->m_style.GetBorderThickness(), dc, toScreen);
		dc->SetPen(wxPen(this->m_style.GetBorderColor(), borderWidth));
		if( bgColor != wxNullColour )
			dc->SetBrush(wxBrush(this->m_style.GetBackgroundColor()));
		else
			dc->SetBrush(*wxTRANSPARENT_BRUSH);
		dc->DrawRectangle(bx, by, bw, bh);
	}
	
	if(bgColor != wxNullColour)
		dc->SetBackground(wxBrush(bgColor));
		
	if(this->m_fPageNumberActive)
		this->m_pageNumber.DrawToDC(dc, toScreen, this->m_style);
		
	wxReportPageStyle pgStyleNoMargins = this->m_style;
	pgStyleNoMargins.SetMargins(0, 0, 0, 0);
	
	int hdItems = this->m_arHeader.GetCount();
	for(int h=0; h<hdItems; ++h)
		this->m_arHeader.Item(h)->DrawToDC(dc, toScreen, pgStyleNoMargins);
		
	int ftItems = this->m_arFooter.GetCount();
	for(int f=0; f<ftItems; ++f)
		this->m_arFooter.Item(f)->DrawToDC(dc, toScreen, pgStyleNoMargins);
		
	int nItems = this->m_arItems.GetCount();
	for(int i=0; i<nItems; ++i)
		this->m_arItems.Item(i)->DrawToDC(dc, toScreen, this->m_style);
}