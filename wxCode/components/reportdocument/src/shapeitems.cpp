#include "wx/report/shapeitems.h"
#include "wx/tokenzr.h"

//////////////////////////////// wxReportShapeItem

void wxReportShapeItem::SetLineThickness(double thickness)
{
	this->m_dLineThickness = thickness;
}

void wxReportShapeItem::SetLineColor(const wxColour& color)
{
	this->m_lineColor = color;
}

void wxReportShapeItem::SetLineStyle(int lineStyle)
{
	this->m_iLineStyle = lineStyle;
}

void wxReportShapeItem::SetFillColor(const wxColour& fillColor)
{
	this->m_fillColor = fillColor;
}

const double& wxReportShapeItem::GetLineThickness()
{
	return this->m_dLineThickness;
}

const wxColour& wxReportShapeItem::GetLineColor()
{
	return this->m_lineColor;
}

const int& wxReportShapeItem::GetLineStyle()
{
	return this->m_iLineStyle;
}

const wxColour& wxReportShapeItem::GetFillColor()
{
	return this->m_fillColor;
}

//////////////////////////////// wxReportLineShape

wxReportLineShape::wxReportLineShape()
{
	this->m_iType = wxRP_LINESHAPE;
	this->m_point1 = wxRealPoint(0, 0);
	this->m_point2 = wxRealPoint(0, 0);
	this->m_dLineThickness = 0.5;
	this->m_lineColor = *wxBLACK;
	this->m_iLineStyle = wxSOLID;
	this->m_fillColor = wxNullColour;
}

wxReportLineShape::wxReportLineShape(const wxRealPoint& point1, const wxRealPoint& point2, double lineThickness, const wxColour& lineColor, int lineStyle)
{
	this->m_iType = wxRP_LINESHAPE;
	this->m_point1 = point1;
	this->m_point2 = point2;
	this->m_dLineThickness = lineThickness;
	this->m_lineColor = lineColor;
	this->m_iLineStyle = lineStyle;
	this->m_fillColor = wxNullColour;
}

wxReportLineShape::wxReportLineShape(double x1, double y1, double x2, double y2, double lineThickness, const wxColour& lineColor, int lineStyle)
{
	this->m_iType = wxRP_LINESHAPE;
	this->m_point1 = wxRealPoint(x1, y1);
	this->m_point2 = wxRealPoint(x2, y2);
	this->m_dLineThickness = lineThickness;
	this->m_lineColor = lineColor;
	this->m_iLineStyle = lineStyle;
	this->m_fillColor = wxNullColour;
}

void wxReportLineShape::SetPoints(double x1, double y1, double x2, double y2)
{
	wxRealPoint p1(x1, y1);
	wxRealPoint p2(x2, y2);
	this->m_point1 = p1;
	this->m_point2 = p2;
}

void wxReportLineShape::SetPoints(const wxRealPoint& point1, const wxRealPoint& point2)
{
	this->m_point1 = point1;
	this->m_point2 = point2;
}

const wxRealPoint& wxReportLineShape::GetPoint1()
{
	return this->m_point1;
}

const wxRealPoint& wxReportLineShape::GetPoint2()
{
	return this->m_point2;
}

wxXmlNode* wxReportLineShape::CreateXmlNode()
{
	wxXmlNode *node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Item"));
	node->AddAttribute(wxT("Type"), wxString::Format(wxT("%d"), this->m_iType));
	node->AddAttribute(wxT("Name"), this->m_sName);
	
	node->AddAttribute(wxT("Coordinates"), wxString::Format(wxT("%G,%G,%G,%G"),
										   this->m_point1.x, this->m_point1.y, this->m_point2.x, this->m_point2.y));
										   
	node->AddAttribute(wxT("LineThickness"), wxString::Format(wxT("%G"), this->m_dLineThickness));
	
	node->AddAttribute( wxT("LineColor"), wxString::Format(wxT("%d,%d,%d"),
											this->m_lineColor.Red(),
											this->m_lineColor.Blue(),
											this->m_lineColor.Green()) );
	return node;
}

bool wxReportLineShape::RetrieveFromXmlNode(const wxXmlNode* node)
{
	if(node->GetName() != wxT("Item")) return false;
	
	int type = -1;
	wxSscanf(node->GetAttribute(wxT("Type"), wxT("")), wxT("%d"), &type);
	if(type != wxRP_LINESHAPE) return false;
	
	double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	wxString coord = node->GetAttribute(wxT("Coordinates"), wxT(""));
	wxStringTokenizer tokenizer(coord, wxT(","), wxTOKEN_STRTOK);
	tokenizer.GetNextToken().ToDouble(&x1);
	tokenizer.GetNextToken().ToDouble(&y1);
	tokenizer.GetNextToken().ToDouble(&x2);
	tokenizer.GetNextToken().ToDouble(&y2);
		
	this->m_point1 = wxRealPoint(x1, y1);
	this->m_point2 = wxRealPoint(x2, y2);
	
	double thickness = -1;
	node->GetAttribute(wxT("LineThickness"), wxT("")).ToDouble(&thickness);
	if(thickness < 0) return false;
	this->m_dLineThickness = thickness;
	
	int r = 0, g = 0, b = 0;
	wxSscanf(node->GetAttribute(wxT("LineColor"), wxT("")), wxT("%d,%d,%d"), &r, &g, &b);
	wxColour color((unsigned char)r, (unsigned char)g, (unsigned char)b);
	this->m_lineColor = color;
	
	return true;
}

void wxReportLineShape::DrawToDC(wxDC* dc, bool toScreen, const wxReportPageStyle& pageStyle)
{
	int width = MM2PX(this->m_dLineThickness, dc, toScreen);
	dc->SetPen(wxPen(this->m_lineColor, width, this->m_iLineStyle));
	
	int x1 = MM2PX(this->m_point1.x, dc, toScreen);
	int y1 = MM2PX(this->m_point1.y, dc, toScreen);
	int x2 = MM2PX(this->m_point2.x, dc, toScreen);
	int y2 = MM2PX(this->m_point2.y, dc, toScreen);
	
	dc->DrawLine(x1, y1, x2, y2);
}

//////////////////////////////// wxReportRectangleShape

wxReportRectangleShape::wxReportRectangleShape()
{
	this->m_iType = wxRP_RECTANGLESHAPE;
	this->m_topLeftPoint = wxRealPoint(0, 0);
	this->m_dWidth = 0;
	this->m_dHeight = 0;
	this->m_dLineThickness = 0.5;
	this->m_lineColor = *wxBLACK;
	this->m_iLineStyle = wxSOLID;
	this->m_fillColor = wxNullColour;
}

wxReportRectangleShape::wxReportRectangleShape(double x, double y, double width, double height, double lineThickness, const wxColour &lineColor, int lineStyle, const wxColour &fillColor)
{
	this->m_iType = wxRP_RECTANGLESHAPE;
	this->m_topLeftPoint = wxRealPoint(x, y);
	this->m_dWidth = width;
	this->m_dHeight = height;
	this->m_dLineThickness = lineThickness;
	this->m_lineColor = lineColor;
	this->m_iLineStyle = lineStyle;
	this->m_fillColor = fillColor;
}

wxReportRectangleShape::wxReportRectangleShape(const wxRealPoint& topLeftCorner, double width, double height, double lineThickness, const wxColour &lineColor, int lineStyle, const wxColour &fillColor)
{
	this->m_iType = wxRP_RECTANGLESHAPE;
	this->m_topLeftPoint = topLeftCorner;
	this->m_dWidth = width;
	this->m_dHeight = height;
	this->m_dLineThickness = lineThickness;
	this->m_lineColor = lineColor;
	this->m_iLineStyle = lineStyle;
	this->m_fillColor = fillColor;
}

void wxReportRectangleShape::SetTopLeftCorner(double x, double y)
{
	this->m_topLeftPoint = wxRealPoint(x, y);
}

void wxReportRectangleShape::SetTopLeftCorner(const wxRealPoint& point)
{
	this->m_topLeftPoint = point;
}

void wxReportRectangleShape::SetWidth(double width)
{
	this->m_dWidth = width;
}

void wxReportRectangleShape::SetHeight(double height)
{
	this->m_dHeight = height;
}

const wxRealPoint& wxReportRectangleShape::GetTopLeftCorner()
{
	return this->m_topLeftPoint;
}

const double& wxReportRectangleShape::GetWidth()
{
	return this->m_dWidth;
}

const double& wxReportRectangleShape::GetHeight()
{
	return this->m_dHeight;
}

wxXmlNode* wxReportRectangleShape::CreateXmlNode()
{
	wxXmlNode *node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Item"));
	node->AddAttribute(wxT("Type"), wxString::Format(wxT("%d"), this->m_iType));
	node->AddAttribute(wxT("Name"), this->m_sName);
	
	node->AddAttribute(wxT("Coordinates"), wxString::Format(wxT("%G,%G"),
										   this->m_topLeftPoint.x, this->m_topLeftPoint.y));
											   
	node->AddAttribute(wxT("Width"), wxString::Format(wxT("%G"), this->m_dWidth));
	node->AddAttribute(wxT("Height"), wxString::Format(wxT("%G"), this->m_dHeight));										   
	node->AddAttribute(wxT("LineThickness"), wxString::Format(wxT("%G"), this->m_dLineThickness));
	
	node->AddAttribute( wxT("LineColor"), wxString::Format(wxT("%d,%d,%d"),
											this->m_lineColor.Red(),
											this->m_lineColor.Blue(),
											this->m_lineColor.Green()) );
											
	if(this->m_fillColor == wxNullColour)
	{
		node->AddAttribute(wxT("FillColor"), wxT(""));
	}
	else
	{
		node->AddAttribute( wxT("FillColor"), wxString::Format(wxT("%d,%d,%d"),
											  this->m_fillColor.Red(),
											  this->m_fillColor.Blue(),
											  this->m_fillColor.Green()) );
	}
	
	return node;
}

bool wxReportRectangleShape::RetrieveFromXmlNode(const wxXmlNode* node)
{
	if(node->GetName() != wxT("Item")) return false;
	
	int type = -1;
	wxSscanf(node->GetAttribute(wxT("Type"), wxT("")), wxT("%d"), &type);
	if(type != wxRP_RECTANGLESHAPE) return false;
	
	double x = 0, y = 0;
	wxString coord = node->GetAttribute(wxT("Coordinates"), wxT(""));
	wxStringTokenizer tokenizer(coord, wxT(","), wxTOKEN_STRTOK);
	tokenizer.GetNextToken().ToDouble(&x);
	tokenizer.GetNextToken().ToDouble(&y);
	this->m_topLeftPoint = wxRealPoint(x, y);
	
	double width = -1;
	node->GetAttribute(wxT("Width"), wxT("")).ToDouble(&width);
	if(width < 0) return false;
	this->m_dWidth = width;
	
	double height = -1;
	node->GetAttribute(wxT("Height"), wxT("")).ToDouble(&height);
	if(height < 0) return false;
	this->m_dHeight = height;
	
	double thickness = -1;
	node->GetAttribute(wxT("LineThickness"), wxT("")).ToDouble(&thickness);
	if(thickness < 0) return false;
	this->m_dLineThickness = thickness;
	
	unsigned char r = 0, g = 0, b = 0;
	wxSscanf(node->GetAttribute(wxT("LineColor"), wxT("")), wxT("%d,%d,%d"), &r, &g, &b);
	wxColour color(r, g, b);
	this->m_lineColor = color;
	
	r = g = b = 0;
	if(wxSscanf(node->GetAttribute(wxT("FillColor"), wxT("")), wxT("%d,%d,%d"), &r, &g, &b) < 3)
		this->m_fillColor = wxNullColour;
	else
	{
		color.Set(r, g, b);
	}
	
	return true;
}

void wxReportRectangleShape::DrawToDC(wxDC* dc, bool toScreen, const wxReportPageStyle& pageStyle)
{
	int lineWidth = MM2PX(this->m_dLineThickness, dc, toScreen);
	dc->SetPen(wxPen(this->m_lineColor, lineWidth, this->m_iLineStyle));
	
	if(this->m_fillColor != wxNullColour)
		dc->SetBrush(wxBrush(this->m_fillColor));
	else
		dc->SetBrush(*wxTRANSPARENT_BRUSH);
	
	int x = MM2PX(this->m_topLeftPoint.x, dc, toScreen);
	int y = MM2PX(this->m_topLeftPoint.y, dc, toScreen);
	int w = MM2PX(this->m_dWidth, dc, toScreen);
	int h = MM2PX(this->m_dHeight, dc, toScreen);
	
	dc->DrawRectangle(x, y, w, h);
}

//////////////////////////////// wxReportRectangleShape

wxReportCircleShape::wxReportCircleShape()
{
	this->m_iType = wxRP_CIRCLESHAPE;
	this->m_centreCoord = wxRealPoint(0, 0);
	this->m_dRadius = 0;
	this->m_dLineThickness = 0.5;
	this->m_lineColor = *wxBLACK;
	this->m_iLineStyle = wxSOLID;
	this->m_fillColor = wxNullColour;
}

wxReportCircleShape::wxReportCircleShape(double centreX, double centreY, double radius, double lineThickness, const wxColour &lineColor, int lineStyle, const wxColour &fillColor)
{
	this->m_iType = wxRP_CIRCLESHAPE;
	this->m_centreCoord = wxRealPoint(centreX, centreY);
	this->m_dRadius = radius;
	this->m_dLineThickness = lineThickness;
	this->m_lineColor = lineColor;
	this->m_iLineStyle = lineStyle;
	this->m_fillColor = fillColor;
}

wxReportCircleShape::wxReportCircleShape(const wxRealPoint& centreCoord, double radius, double lineThickness, const wxColour &lineColor, int lineStyle, const wxColour &fillColor)
{
	this->m_iType = wxRP_CIRCLESHAPE;
	this->m_centreCoord = centreCoord;
	this->m_dRadius = radius;
	this->m_dLineThickness = lineThickness;
	this->m_lineColor = lineColor;
	this->m_iLineStyle = lineStyle;
	this->m_fillColor = fillColor;
}

void wxReportCircleShape::SetCentreCoord(double coordX, double coordY)
{
	this->m_centreCoord = wxRealPoint(coordX, coordY);
}

void wxReportCircleShape::SetRadius(double r)
{
	this->m_dRadius = r;
}

const wxRealPoint& wxReportCircleShape::GetCentreCoord()
{
	return this->m_centreCoord;
}

const double& wxReportCircleShape::GetRadius()
{
	return this->m_dRadius;
}

wxXmlNode* wxReportCircleShape::CreateXmlNode()
{
	wxXmlNode *node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Item"));
	node->AddAttribute(wxT("Type"), wxString::Format(wxT("%d"), this->m_iType));
	node->AddAttribute(wxT("Name"), this->m_sName);
	
	node->AddAttribute(wxT("CentreCoord"), wxString::Format(wxT("%G,%G"),
										   this->m_centreCoord.x, this->m_centreCoord.y));
										   
	node->AddAttribute(wxT("Radius"), wxString::Format(wxT("%G"), this->m_dRadius));
	node->AddAttribute(wxT("LineThickness"), wxString::Format(wxT("%G"), this->m_dLineThickness));
	
	node->AddAttribute( wxT("LineColor"), wxString::Format(wxT("%d,%d,%d"),
											this->m_lineColor.Red(),
											this->m_lineColor.Blue(),
											this->m_lineColor.Green()) );
											
	if(this->m_fillColor == wxNullColour)
	{
		node->AddAttribute(wxT("FillColor"), wxT(""));
	}
	else
	{
		node->AddAttribute( wxT("FillColor"), wxString::Format(wxT("%d,%d,%d"),
											  this->m_fillColor.Red(),
											  this->m_fillColor.Blue(),
											  this->m_fillColor.Green()) );
	}
	
	return node;
}

bool wxReportCircleShape::RetrieveFromXmlNode(const wxXmlNode* node)
{
	if(node->GetName() != wxT("Item")) return false;
	
	int type = -1;
	wxSscanf(node->GetAttribute(wxT("Type"), wxT("")), wxT("%d"), &type);
	if(type != wxRP_CIRCLESHAPE) return false;
	
	double x = 0, y = 0;
	wxString coord = node->GetAttribute(wxT("CentreCoord"), wxT(""));
	wxStringTokenizer tokenizer(coord, wxT(","), wxTOKEN_STRTOK);
	tokenizer.GetNextToken().ToDouble(&x);
	tokenizer.GetNextToken().ToDouble(&y);
	this->m_centreCoord = wxRealPoint(x, y);
	
	double width = 0;
	node->GetAttribute(wxT("Radius"), wxT("")).ToDouble(&width);
	if(width <= 0) return false;
	this->m_dRadius = width;
		
	double thickness = -1;
	node->GetAttribute(wxT("LineThickness"), wxT("")).ToDouble(&thickness);
	if(thickness < 0) return false;
	this->m_dLineThickness = thickness;
	
	unsigned char r = 0, g = 0, b = 0;
	wxSscanf(node->GetAttribute(wxT("LineColor"), wxT("")), wxT("%d,%d,%d"), &r, &g, &b);
	wxColour color(r, g, b);
	this->m_lineColor = color;
	
	r = g = b = 0;
	if(wxSscanf(node->GetAttribute(wxT("FillColor"), wxT("")), wxT("%d,%d,%d"), &r, &g, &b) < 3)
		this->m_fillColor = wxNullColour;
	else
	{
		color.Set(r, g, b);
	}
	
	return true;
}

void wxReportCircleShape::DrawToDC(wxDC* dc, bool toScreen, const wxReportPageStyle& pageStyle)
{
	int r = MM2PX(this->m_dRadius, dc, toScreen);
	int x = MM2PX(this->m_centreCoord.x, dc, toScreen);
	int y = MM2PX(this->m_centreCoord.y, dc, toScreen);
	int lineWidth = MM2PX(this->m_dLineThickness, dc, toScreen);
	dc->SetPen(wxPen(this->m_lineColor, lineWidth, this->m_iLineStyle));
	
	if(this->m_fillColor != wxNullColour)
		dc->SetBrush(wxBrush(this->m_fillColor));
	else
		dc->SetBrush(*wxTRANSPARENT_BRUSH);
	
	dc->DrawCircle(x, y, r);
}