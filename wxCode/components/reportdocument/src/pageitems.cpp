#include "wx/report/pageitems.h"
#include "wx/arrimpl.cpp"

WX_DEFINE_USER_EXPORTED_OBJARRAY(ItemsArray);

wxPoint CalcNegPos(const wxRealPoint& pos, const wxPoint& pxPos, int w, int h, int lm, int tm, int rm, int bm, wxDC* dc)
{
	double origX = pos.x;
	double origY = pos.y;
	int px_x = pxPos.x;
	int px_y = pxPos.y;
	wxSize pageSize = dc->GetSize();
	
	double sx, sy;
	dc->GetUserScale(&sx, &sy);
	int pagex = wxRound((double)(pageSize.x)/sx - lm - rm);
	int pagey = wxRound((double)(pageSize.y)/sy - tm - bm);
	
	int x = px_x + lm;
	int y = px_y + tm;
	
	if(origX < 0)
		x = pagex + px_x - w + lm / sx;
	else if(origX == wxRP_LEFT)
		x = lm;
	else if(origX == wxRP_CENTER)
		x = (pagex - w) / 2. + lm;
	else if(origX == wxRP_RIGHT)
		x = pagex - w + lm;
	
	if(origY < 0)
		y = pagey + px_x - h + tm / sy;
	else if(origY == wxRP_TOP)
		y = tm;
	else if(origY == wxRP_CENTER)
		y = (pagey - h) / 2. + tm;
	else if(origY == wxRP_BOTTOM)
		y = pagey - h + tm;
		
	return wxPoint(x, y);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// wxReportPageItem Class Implementation
/////////////////////////////////////////////////////////////////////////////////////////////

wxReportPageItem::wxReportPageItem()
{
	this->m_fVariable = false;
	this->m_pVariable = NULL;
}

wxReportPageItem::wxReportPageItem(const wxString& name, bool isVariable)
{
	this->m_sName = name;
	this->m_fVariable = isVariable;
	this->m_pVariable = NULL;
}

void wxReportPageItem::SetName(const wxString& name)
{
	this->m_sName = name;
}

void wxReportPageItem::SetIfVariable(bool isVariable)
{
	this->m_fVariable = isVariable;
}

const wxString& wxReportPageItem::GetName()
{
	return this->m_sName;
}

int wxReportPageItem::GetType()
{
	return this->m_iType;
}

bool wxReportPageItem::IsVariable()
{
	return this->m_fVariable;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// wxReportPositionedItem Class Implementation
/////////////////////////////////////////////////////////////////////////////////////////////

wxReportPositionedItem::wxReportPositionedItem()
{
	this->m_position = wxRealPoint(0, 0);
}

wxReportPositionedItem::wxReportPositionedItem(const wxString& name, double x, double y, bool isVariable)
{
	this->m_sName = name;
	this->m_fVariable = isVariable;
	this->m_pVariable = NULL;
	this->m_position = wxRealPoint(x, y);
}

void wxReportPositionedItem::SetPosition(double x, double y)
{
	this->m_position = wxRealPoint(x, y);
}

void wxReportPositionedItem::SetPosition(const wxRealPoint& position)
{
	this->m_position = position;
}

const wxRealPoint& wxReportPositionedItem::GetPosition()
{
	return this->m_position;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// wxReportImageItem Class Implementation
/////////////////////////////////////////////////////////////////////////////////////////////

wxReportImageItem::wxReportImageItem()
{
	this->m_iType = wxRP_IMAGE;
	this->m_iHeight = 0;
	this->m_iWidth = 0;
	this->m_iPPI = 72;
	this->m_pVariable = NULL;
}

wxReportImageItem::wxReportImageItem(const wxString& name, const wxString& path, double x, double y, int PPI, bool isVariable)
{
	this->m_sName = name;
	this->m_iType = wxRP_IMAGE;
	this->m_fVariable = isVariable;
	this->m_pVariable = NULL;
	
	if(x >= 0 && y >= 0)
		this->m_position = wxRealPoint(x, y);
	else
		this->m_position = wxRealPoint(0, 0);
	
	/*this->m_sValue = path;
	
	if(PPI > 0)
		this->m_iPPI = PPI;
	
	wxInitAllImageHandlers();
	wxImage image(path);
	this->m_iHeight = image.GetHeight();
	this->m_iWidth = image.GetWidth();*/
	
	SetPath( path );
	SetPPI( PPI );
}

void wxReportImageItem::SetPath(const wxString& path)
{
	this->m_sValue = path;
	wxImage image(path);
	this->m_iHeight = image.GetHeight();
	this->m_iWidth = image.GetWidth();
	if(this->m_fVariable)
		this->m_pVariable = (const void*)&path;
}

void wxReportImageItem::SetPPI(int ppi)
{
	if(ppi > 0)
		this->m_iPPI = ppi;
}

void wxReportImageItem::SetStyle(const wxReportImageStyle& style)
{
	this->m_style = style;
}

const wxString& wxReportImageItem::GetPath()
{
	return this->m_sValue;
}

int wxReportImageItem::GetPPI()
{
	return this->m_iPPI;
}

wxReportImageStyle& wxReportImageItem::GetStyle()
{
	return this->m_style;
}

int wxReportImageItem::GetHeight()
{
	return this->m_iHeight;
}

int wxReportImageItem::GetWidth()
{
	return this->m_iWidth;
}

void wxReportImageItem::RefreshVariable()
{
	if(this->m_fVariable && this->m_pVariable)
	{
		this->m_sValue = *((const wxString*)this->m_pVariable);		
	}
}

void wxReportImageItem::Clear()
{
	this->m_iHeight = 0;
	this->m_iWidth = 0;
	this->m_sValue = wxT("");
	this->m_pVariable = NULL;
}

wxXmlNode* wxReportImageItem::CreateXmlNode()
{
	wxXmlNode *itemNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Item"));
	
	itemNode->AddAttribute( wxT("Type"), wxString::Format(wxT("%d"), this->m_iType) );
	itemNode->AddAttribute( wxT("Name"), this->m_sName);
	itemNode->AddAttribute( wxT("Path"), this->m_sValue);
	itemNode->AddAttribute( wxT("X"), wxString::Format(wxT("%G"), this->m_position.x) );
	itemNode->AddAttribute( wxT("Y"), wxString::Format(wxT("%G"), this->m_position.y) );
	itemNode->AddAttribute( wxT("Width"), wxString::Format(wxT("%d"), this->m_iWidth) );
	itemNode->AddAttribute( wxT("Height"), wxString::Format(wxT("%d"), this->m_iHeight) );
	itemNode->AddAttribute( wxT("PPI"), wxString::Format(wxT("%d"), this->m_iPPI) );
	itemNode->AddProperty(this->m_style.CreateXmlNode()->GetProperties());
		
	return itemNode;
}

bool wxReportImageItem::RetrieveFromXmlNode(const wxXmlNode* node)
{
	long type = -1, ppi = 72;
	double x = -1, y = -1;
	
	node->GetAttribute(wxT("Type"), wxT("")).ToLong(&type);	
	if(type != wxRP_IMAGE) return false;
	if(!this->m_style.RetrieveFromXmlNode(node));	
	this->m_sName = node->GetAttribute(wxT("Name"), wxT(""));	
	wxString path = node->GetAttribute(wxT("Path"), wxT(""));
	wxImage image;
	
	if(image.LoadFile(path))
	{
		this->m_sValue = path;
	}
	else return false;
	
	wxSscanf(node->GetAttribute(wxT("X"), wxT("")), wxT("%lf"), &x);
	wxSscanf(node->GetAttribute(wxT("Y"), wxT("")), wxT("%lf"), &y);
	
	if(x >= 0 && y >= 0)
		this->m_position = wxRealPoint(x, y);
	else return false;
	
	wxSscanf(node->GetAttribute(wxT("PPI"), wxT("")), wxT("%d"), &ppi);
	this->m_iPPI = ppi;
	
	this->m_iWidth = image.GetWidth();
	this->m_iHeight = image.GetHeight();
	
	return true;
}

void wxReportImageItem::DrawToDC(wxDC* dc, bool toScreen, const wxReportPageStyle& pageStyle)
{
	wxImage image;
	if(image.LoadFile(this->m_sValue))
	{
		double scale = dc->GetPPI().x / (double)this->m_iPPI; //(25.4 * dc->GetSize().x) / (this->m_iPPI * dc->GetSizeMM().x); // calculate scale factor
		double updscale = scale;
		double prevScaleX = 1, prevScaleY = 1;
		dc->GetUserScale(&prevScaleX, &prevScaleY);
		if(toScreen)
			updscale *= prevScaleX;
			
		dc->SetUserScale(updscale, updscale);
		
		int x = (double)(MM2PX(this->m_position.x + pageStyle.GetLeftMargin(), dc, true)) / scale;// / (scale*prevScaleX);
		int y = (double)(MM2PX(this->m_position.y + pageStyle.GetTopMargin(), dc, true)) / scale;// / (scale*prevScaleX);
				
		dc->DrawBitmap(wxBitmap(image), x, y);
		
		if(this->m_style.GetBorder() > 0)
		{
			int borderWidth = (double)(MM2PX(this->m_style.GetBorderThickness(), dc, true)) / scale;
			/*int bx = x - borderWidth;
			int by = y - borderWidth;
			int bw = this->m_iWidth + 2*borderWidth;
			int bh = this->m_iHeight + 2*borderWidth;*/
			dc->SetPen(wxPen(this->m_style.GetBorderColor(), borderWidth));
			/*dc->DrawRectangle(bx, by, bw, bh);*/
			dc->DrawRectangle(x, y, this->m_iWidth, this->m_iHeight);
		}
		
		dc->SetUserScale(prevScaleX, prevScaleY);
	}
}