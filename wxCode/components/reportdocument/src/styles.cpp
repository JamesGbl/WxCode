#include "wx/report/styles.h"
#include "wx/tokenzr.h"

//double wxReportUnit::m_unitTypeRatio = 25.4;

/////////// wxReportStyle class

wxReportStyle::wxReportStyle()
{
	this->m_BgColor = wxNullColour;
	this->m_iBorder = wxRP_NOBORDER;
	this->m_BorderColor = *wxBLACK;
	this->m_dBorderThickness = 0.5;
}

wxReportStyle::wxReportStyle(const wxString& name)
{
	this->m_BgColor = wxNullColour;
	this->m_iBorder = wxRP_NOBORDER;
	this->m_BorderColor = *wxBLACK;
	this->m_dBorderThickness = 0.5;
	this->m_sName = name;
}

void wxReportStyle::SetBorder(int borderValue, wxColour color, double thickness)
{
	if(borderValue <= 16) this->m_iBorder = borderValue;
	this->m_BorderColor = color;
	if(thickness > 0) this->m_dBorderThickness = thickness;
}

void wxReportStyle::SetBorderColor(const wxColour& color)
{
	this->m_BorderColor = color;
}

void wxReportStyle::SetBorderThickness(double thickness)
{
	this->m_dBorderThickness = thickness;
}

void wxReportStyle::SetBackgroundColor(const wxColour& color)
{
	this->m_BgColor = color;
}

void wxReportStyle::SetName(const wxString& name)
{
	this->m_sName = name;
}

const wxString& wxReportStyle::GetName() const
{
	return this->m_sName;
}

const int& wxReportStyle::GetBorder() const
{
	return this->m_iBorder;
}

const wxColor& wxReportStyle::GetBorderColor() const
{
	return this->m_BorderColor;
}

const double& wxReportStyle::GetBorderThickness() const
{
	return this->m_dBorderThickness;
}

const wxColour& wxReportStyle::GetBackgroundColor() const
{
	return this->m_BgColor;
}

//////////// wxReportPageStyle class

wxReportPageStyle::wxReportPageStyle()
{
	this->m_dSizeX = 210.0; 
	this->m_dSizeY = 297.0;
	this->m_dLeftMargin = 0;
	this->m_dBottomMargin = 0;
	this->m_dRightMargin = 0;
	this->m_dTopMargin = 0;
}

wxReportPageStyle::wxReportPageStyle(const wxString& name)
{
	this->m_sName = name;
	this->m_dSizeX = 210.0; 
	this->m_dSizeY = 297.0;
}

void wxReportPageStyle::SetWidth(double width)
{
	this->m_dSizeX = width;
}

void wxReportPageStyle::SetHeight(double height)
{
	this->m_dSizeY = height;
}

void wxReportPageStyle::SetMargins(double left, double right, double top, double bottom)
{
	this->m_dLeftMargin = left;
	this->m_dRightMargin = right;
	this->m_dTopMargin = top;
	this->m_dBottomMargin = bottom;
}

wxRealPoint wxReportPageStyle::GetSize() const
{
	return wxRealPoint(this->m_dSizeX, this->m_dSizeY);
}

double wxReportPageStyle::GetLeftMargin() const
{
	return this->m_dLeftMargin;
}

double wxReportPageStyle::GetRightMargin() const
{
	return this->m_dRightMargin;
}

double wxReportPageStyle::GetTopMargin() const
{
	return this->m_dTopMargin;
}

double wxReportPageStyle::GetBottomMargin() const
{
	return this->m_dBottomMargin;
}

wxXmlNode* wxReportPageStyle::CreateXmlNode()
{
	wxXmlNode *node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Style"));
	
	node->AddAttribute( wxT("StyleType"), wxT("Page") );
	node->AddAttribute( wxT("StyleName"), this->m_sName );
	node->AddAttribute( wxT("Width"), wxString::Format(wxT("%d"), this->m_dSizeX) );
	node->AddAttribute( wxT("Height"), wxString::Format(wxT("%d"), this->m_dSizeY) );
	node->AddAttribute( wxT("Margins"), wxString::Format(wxT("%G,%G,%G,%G"), this->m_dLeftMargin, this->m_dRightMargin, this->m_dTopMargin, this->m_dBottomMargin ));
	node->AddAttribute( wxT("Borders"), wxString::Format(wxT("%d"), this->m_iBorder) );
	node->AddAttribute( wxT("BorderThickness"), wxString::Format(wxT("%G"), this->m_dBorderThickness) );
	
	node->AddAttribute( wxT("BorderColor"), wxString::Format(wxT("%d,%d,%d"),
											this->m_BorderColor.Red(),
											this->m_BorderColor.Blue(),
											this->m_BorderColor.Green()) );
											
	wxString bgColorFormat;
	
	if(this->m_BgColor == wxNullColour)
		bgColorFormat = wxT("");
	else
		bgColorFormat = wxString::Format(wxT("%d,%d,%d"),	this->m_BgColor.Red(), this->m_BgColor.Blue(), this->m_BgColor.Green());
	
	node->AddAttribute( wxT("BackgroundColor"), bgColorFormat);
												
	return node;
}

bool wxReportPageStyle::RetrieveFromXmlNode(const wxXmlNode* node)
{
	double width = -1, height = -1, lm = 0, rm = 0, tm = 0, bm = 0, thickness = -1;
	long borders = -1, borderRed = -1, borderGreen = -1, borderBlue = -1, bgRed = -1, bgGreen = -1, bgBlue = -1;
 
	if(node->GetAttribute(wxT("StyleType"), wxT("")) != wxT("Page")) return false;
	wxString name = node->GetAttribute(wxT("StyleName"), wxT(""));
	node->GetAttribute(wxT("Width"), wxT("")).ToDouble(&width);
	node->GetAttribute(wxT("Height"), wxT("")).ToDouble(&height);
	wxString margins=node->GetAttribute(wxT("Margins"), wxT(""));
	wxStringTokenizer tokenizer(margins, wxT(","), wxTOKEN_STRTOK);
	tokenizer.GetNextToken().ToDouble(&lm);
	tokenizer.GetNextToken().ToDouble(&rm);
	tokenizer.GetNextToken().ToDouble(&tm);
	tokenizer.GetNextToken().ToDouble(&bm);
	//wxSscanf(margins, wxT("%lf,%lf,%lf,%lf"), &lm, &rm, &tm, &bm);
	//wxSscanf(node->GetAttribute(wxT("Margins"), wxT("")), wxT("%f,%f,%f,%f"), &lm, &rm, &tm, &bm); 
	node->GetAttribute(wxT("Borders"), wxT("")).ToLong(&borders);
	node->GetAttribute(wxT("BorderThickness"), wxT("")).ToDouble(&thickness);
	
	wxString colorStr = node->GetAttribute(wxT("BorderColor"), wxT(""));
	wxSscanf(colorStr, wxT("%d,%d,%d"), &borderRed, &borderGreen, &borderBlue); 
		
	colorStr = node->GetAttribute(wxT("BackgroundColor"), wxT(""));
	wxSscanf(colorStr, wxT("%d,%d,%d"), &bgRed, &bgGreen, &bgBlue);
	
	this->m_sName = name;
	
	if(width > 0)
		this->m_dSizeX = width;
	else
		this->m_dSizeX = 210.0;
		
	if(height > 0)
		this->m_dSizeY = height;
	else
		this->m_dSizeY = 297.0;
		
	if(lm < 0)
		lm = 0;
		
	if(rm < 0)
		rm = 0;
		
	if(tm < 0)
		tm = 0;
		
	if(bm < 0)
		bm = 0;
		
	this->m_dLeftMargin = lm;
	this->m_dRightMargin = rm;
	this->m_dTopMargin = tm;
	this->m_dBottomMargin = bm;
		
	if(borders >= 0 && borders < 16)
		this->m_iBorder = (int)borders;
	else
		this->m_iBorder = wxRP_NOBORDER;
		
	if(thickness >= 0)
		this->m_dBorderThickness = thickness;
	else
		this->m_dBorderThickness = 0.5;
		
	if(borderRed >= 0 && borderRed <= 255 && borderBlue >= 0 && borderBlue <= 255 && borderGreen >= 0 && borderGreen <= 255)
	{
		wxColour color((unsigned char)borderRed, (unsigned char)borderBlue, (unsigned char)borderGreen);
		this->m_BorderColor = color;
	}
	else
		this->m_BorderColor = *wxBLACK;
	
	if(bgRed >= 0 && bgRed <= 255 && bgBlue >= 0 && bgBlue <= 255 && bgGreen >= 0 && bgGreen <= 255)
	{
		wxColour color((unsigned char)bgRed, (unsigned char)bgBlue, (unsigned char)bgGreen);
		this->m_BgColor = color;
	}
	else
		this->m_BgColor = wxNullColour;
		
	return true;
}

//////////// wxReportParagraphStyle class

wxReportParagraphStyle::wxReportParagraphStyle()
{
	this->m_dParagraphsSpace = 0;
	this->m_dLeftIndent = 0;
	this->m_dRightIndent = 0;
	this->m_iTextAlign = wxRP_LEFTALIGN;
	this->m_dLineHeight = -1;
	this->m_fLineHeightFromFont = true;
}

wxReportParagraphStyle::wxReportParagraphStyle(const wxString& name)
{
	this->m_sName = name;
	this->m_dParagraphsSpace = 0;
	this->m_dLeftIndent = 0;
	this->m_dRightIndent = 0;
	this->m_iTextAlign = wxRP_LEFTALIGN;
	this->m_dLineHeight = -1;
	this->m_fLineHeightFromFont = true;
}

void wxReportParagraphStyle::SetTextAlign(int textAlign)
{
	if(textAlign >= 0)
		this->m_iTextAlign = textAlign;
}

void wxReportParagraphStyle::SetIndent(double leftIndent, double rightIndent)
{
	if(leftIndent >= 0)
		this->m_dLeftIndent = leftIndent;
	
	if(rightIndent >= 0)
		this->m_dRightIndent = rightIndent;
}

void wxReportParagraphStyle::SetLineHeight(double lineHeight)
{
	if(lineHeight >= 0)
	{
		this->m_dLineHeight = lineHeight;
		this->m_fLineHeightFromFont = false;
	}
}

/*void wxReportParagraphStyle::SetLineHeight(bool fromFont)
{
	this->m_fLineHeightFromFont = fromFont;
}*/

void wxReportParagraphStyle::SetParagraphsSpace(double space)
{
	this->m_dParagraphsSpace = space;
}

int wxReportParagraphStyle::GetTextAlign()
{
	return this->m_iTextAlign;
}

double wxReportParagraphStyle::GetLeftIndent()
{
	return this->m_dLeftIndent;
}

double wxReportParagraphStyle::GetRightIndent()
{
	return this->m_dRightIndent;
}

double wxReportParagraphStyle::GetLineHeight()
{
	return this->m_dLineHeight;
}

double wxReportParagraphStyle::GetParagraphsSpace()
{
	return this->m_dParagraphsSpace;
}

wxXmlNode* wxReportParagraphStyle::CreateXmlNode()
{
	wxXmlNode *node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Style"));
	
	node->AddAttribute( wxT("StyleType"), wxT("Paragraph") );
	node->AddAttribute( wxT("StyleName"), this->m_sName );
	node->AddAttribute( wxT("Align"), wxString::Format(wxT("%d"), this->m_iTextAlign) );
	node->AddAttribute( wxT("LineHeight"), wxString::Format(wxT("%G"), this->m_dLineHeight) );
	
	node->AddAttribute( wxT("Indent"), wxString::Format(wxT("%G,%G"),
										this->m_dLeftIndent, this->m_dRightIndent) );
											
	node->AddAttribute( wxT("Borders"), wxString::Format(wxT("%d"), this->m_iBorder) );
	node->AddAttribute( wxT("BorderThickness"), wxString::Format(wxT("%G"), this->m_dBorderThickness) );
	
	node->AddAttribute( wxT("BorderColor"), wxString::Format(wxT("%d,%d,%d"),
											this->m_BorderColor.Red(),
											this->m_BorderColor.Blue(),
											this->m_BorderColor.Green()) );
											
	wxString bgColorFormat;
	
	if(this->m_BgColor == wxNullColour)
		bgColorFormat = wxT("");
	else
		bgColorFormat = wxString::Format(wxT("%d,%d,%d"),	this->m_BgColor.Red(), this->m_BgColor.Blue(), this->m_BgColor.Green());
	
	node->AddAttribute( wxT("BackgroundColor"), bgColorFormat);
	
	return node;
}

bool wxReportParagraphStyle::RetrieveFromXmlNode(const wxXmlNode* node)
{
	long borders = -1, borderRed = -1, borderGreen = -1, borderBlue = -1, bgRed = -1, bgGreen = -1, bgBlue = -1,
		 align = -1;
	double lineHeight = -1, leftIndent = -1, rightIndent = -1, thickness = -1;
	
	wxString type = node->GetAttribute(wxT("StyleType"), wxT(""));
	wxString name = node->GetAttribute(wxT("StyleName"), wxT(""));
	node->GetAttribute(wxT("Align"), wxT("")).ToLong(&align);
	node->GetAttribute(wxT("LineHeight"), wxT("")).ToDouble(&lineHeight);
	
	wxString indent = node->GetAttribute(wxT("Indent"), wxT(""));
	wxStringTokenizer tokenizer(indent, wxT(","), wxTOKEN_STRTOK);
	tokenizer.GetNextToken().ToDouble(&leftIndent);
	tokenizer.GetNextToken().ToDouble(&rightIndent);
	//wxSscanf(indent, wxT("%d,%d"), &leftIndent, &rightIndent);
	
	node->GetAttribute(wxT("Borders"), wxT("")).ToLong(&borders);
	node->GetAttribute(wxT("BorderThickness"), wxT("")).ToDouble(&thickness);
	
	wxString colorStr = node->GetAttribute(wxT("BorderColor"), wxT(""));
	wxSscanf(colorStr, wxT("%d,%d,%d"), &borderRed, &borderGreen, &borderBlue);
	
	colorStr = node->GetAttribute(wxT("BackgroundColor"), wxT(""));
	wxSscanf(colorStr, wxT("%d,%d,%d"), &bgRed, &bgGreen, &bgBlue);
		
	if(type != wxT("Paragraph")) return false;
	
	this->m_sName = name;
		
	if(align >= 0 && align <= 3)
		this->m_iTextAlign = (int)align;
	else
		this->m_iTextAlign = 0;
		
	if(lineHeight >= 1)
		this->m_dLineHeight = lineHeight;
	else
		this->m_dLineHeight = 1;
		
	if(leftIndent >= 0 && rightIndent >= 0)
	{
		this->m_dLeftIndent = leftIndent;
		this->m_dRightIndent = rightIndent;
	}
	else
	{
		this->m_dLeftIndent = 0;
		this->m_dRightIndent = 0;
	}
		
	if(borders >= 0 && borders < 16)
		this->m_iBorder = (int)borders;
	else
		this->m_iBorder = wxRP_NOBORDER;
		
	if(thickness >= 0)
		this->m_dBorderThickness = thickness;
	else
		this->m_dBorderThickness = 0.5;
		
	if(borderRed >= 0 && borderRed <= 255 && borderBlue >= 0 && borderBlue <= 255 && borderGreen >= 0 && borderGreen <= 255)
	{
		wxColour color((unsigned char)borderRed, (unsigned char)borderBlue, (unsigned char)borderGreen);
		this->m_BorderColor = color;
	}
	else
		this->m_BorderColor = *wxBLACK;
	
	if(bgRed >= 0 && bgRed <= 255 && bgBlue >= 0 && bgBlue <= 255 && bgGreen >= 0 && bgGreen <= 255)
	{
		wxColour color((unsigned char)bgRed, (unsigned char)bgBlue, (unsigned char)bgGreen);
		this->m_BgColor = color;
	}
	else
		this->m_BgColor = wxNullColour;
	
	return true;		
}

//////////// TextwxReportStyles

wxReportTextStyle::wxReportTextStyle()
{
	this->m_font = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	this->m_FontColor = *wxBLACK;
}

wxReportTextStyle::wxReportTextStyle(const wxString& name, const wxFont& font, const wxColour& textColor, const wxColour& backgroundColor, int borderType, const wxColour& borderColor )
{
	this->m_BgColor = backgroundColor;
	this->m_iBorder = borderType;
	this->m_BorderColor = borderColor;
	this->m_dBorderThickness = 0.5;
	this->m_sName = name;
	this->m_font = font;
	this->m_FontColor = textColor;
	this->m_BgColor = backgroundColor;
	this->m_iBorder = borderType;
	this->m_BorderColor = borderColor;
}

bool wxReportTextStyle::operator== (const wxReportTextStyle& textStyle) const
{
	if(this->m_BgColor != textStyle.m_BgColor)
		return false;
	if(this->m_BorderColor != textStyle.m_BorderColor)
		return false;
	if(this->m_FontColor != textStyle.m_FontColor)
		return false;
	if(this->m_dBorderThickness != textStyle.m_dBorderThickness)
		return false;
	if(this->m_font != textStyle.m_font)
		return false;
	if(this->m_font.GetPointSize() != textStyle.m_font.GetPointSize())
		return false;
	if(this->m_iBorder != textStyle.m_iBorder)
		return false;
		
	return true;
}

void wxReportTextStyle::SetFont(const wxFont& font)
{
	this->m_font = font;
}

void wxReportTextStyle::SetTextColor(const wxColour& color)
{
	this->m_FontColor = color;
}

const wxFont& wxReportTextStyle::GetFont() const
{
	return this->m_font;
}

int wxReportTextStyle::GetFontSize() const
{
	return this->m_font.GetPointSize();
}

const wxColor& wxReportTextStyle::GetTextColor() const
{
	return this->m_FontColor;
}

wxXmlNode* wxReportTextStyle::CreateXmlNode()
{
	wxXmlNode *node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Style"));
	
	node->AddAttribute( wxT("StyleType"), wxT("Text") );
	node->AddAttribute( wxT("StyleName"), this->m_sName );
	node->AddAttribute( wxT("Borders"), wxString::Format(wxT("%d"), this->m_iBorder) );
	node->AddAttribute( wxT("BorderThickness"), wxString::Format(wxT("%G"), this->m_dBorderThickness) );
	
	node->AddAttribute( wxT("BorderColor"), wxString::Format(wxT("%d,%d,%d"),
											this->m_BorderColor.Red(),
											this->m_BorderColor.Blue(),
											this->m_BorderColor.Green()) );
											
	wxString bgColorFormat;
	
	if(this->m_BgColor == wxNullColour)
		bgColorFormat = wxT("");
	else
		bgColorFormat = wxString::Format(wxT("%d,%d,%d"),	this->m_BgColor.Red(), this->m_BgColor.Blue(), this->m_BgColor.Green());
	
	node->AddAttribute( wxT("BackgroundColor"), bgColorFormat);
											
	wxString underlined;
	if(this->m_font.GetUnderlined())
		underlined = wxT("1");
	else
		underlined = wxT("0");
	
	node->AddAttribute( wxT("Font"), wxString::Format(wxT("%d,%d,%d,%d,") +
									underlined + wxT(",") + this->m_font.GetFaceName(),
									this->m_font.GetPointSize(), this->m_font.GetFamily(),
									this->m_font.GetStyle(), this->m_font.GetWeight()) );
	
	node->AddAttribute( wxT("FontColor"), wxString::Format(wxT("%d,%d,%d"),
												this->m_FontColor.Red(),
												this->m_FontColor.Blue(),
												this->m_FontColor.Green()) );									 
	return node;
}

bool wxReportTextStyle::RetrieveFromXmlNode(const wxXmlNode* node)
{
	long borders = -1, borderRed = -1, borderGreen = -1, borderBlue = -1, fontRed = -1, fontGreen = -1,
		 fontBlue = -1, fontPointSize = -1, fontFamily = -1, fontStyle = -1, fontWeight = -1, fontUnderlined = -1;
		 
	int bgRed = -1, bgGreen = -1, bgBlue = -1;
	double thickness = -1;
	
	wxString type = node->GetAttribute(wxT("StyleType"), wxT(""));
	wxString name = node->GetAttribute(wxT("StyleName"), wxT(""));
	node->GetAttribute(wxT("Borders"), wxT("")).ToLong(&borders);
	node->GetAttribute(wxT("BorderThickness"), wxT("")).ToDouble(&thickness);
	
	wxString colorStr = node->GetAttribute(wxT("BorderColor"), wxT(""));
	wxSscanf(colorStr, wxT("%d,%d,%d"), &borderRed, &borderGreen, &borderBlue);
	
	colorStr = node->GetAttribute(wxT("BackgroundColor"), wxT(""));
	wxSscanf(colorStr, wxT("%d,%d,%d"), &bgRed, &bgGreen, &bgBlue);
		
	wxString fontStr = node->GetAttribute(wxT("Font"), wxT(""));
	wxSscanf(fontStr, wxT("%d,%d,%d,%d,%d"), &fontPointSize,
			&fontFamily, &fontStyle, &fontWeight, &fontUnderlined);
	wxString fontFaceName = fontStr.AfterLast(',');
	fontFaceName.Replace(wxT(" "), wxT(""));
	
	colorStr = node->GetAttribute(wxT("FontColor"), wxT(""));
	wxSscanf(colorStr, wxT("%d,%d,%d"), &fontRed, &fontGreen, &fontBlue);
	
	if(type != wxT("Text")) return false;	
	this->m_sName = name;
	
	if(borders >= 0 && borders <= 16)
		this->m_iBorder = borders;
	else
		this->m_iBorder = wxRP_NOBORDER;

	if(thickness >= 0)
		this->m_dBorderThickness = thickness;
	else
		this->m_dBorderThickness = 0.5;	
	
	if(borderRed >= 0 && borderRed <= 255 && borderBlue >= 0 && borderBlue <= 255 && borderGreen >= 0 && borderGreen <= 255)
	{
		wxColour color((unsigned char)borderRed, (unsigned char)borderBlue, (unsigned char)borderGreen);
		this->m_BorderColor = color;
	}
	else
		this->m_BorderColor = *wxBLACK;
		
	if(bgRed >= 0 && bgRed <= 255 && bgBlue >= 0 && bgBlue <= 255 && bgGreen >= 0 && bgGreen <= 255)
	{
		wxColour color((unsigned char)bgRed, (unsigned char)bgBlue, (unsigned char)bgGreen);
		this->m_BgColor = color;
	}
	else
		this->m_BgColor = wxNullColour;	
	
	bool fFontUnderlined = fontUnderlined;
	if(fontPointSize > 0)
	{
		wxFont font((int)fontPointSize, (int)fontFamily, (int)fontStyle, (int)fontWeight, fFontUnderlined, fontFaceName);
		this->m_font = font;
	}
	
	if(fontRed >= 0 && fontRed <= 255 && fontBlue >= 0 && fontBlue <= 255 && fontGreen >= 0 && fontGreen <= 255)
	{
		wxColour color((unsigned char)fontRed, (unsigned char)fontBlue, (unsigned char)fontGreen);
		this->m_FontColor = color;
	}
	else
		this->m_FontColor = *wxBLACK;
	
	return true;
}

///////////// wxReportImageStyle class

wxReportImageStyle::wxReportImageStyle()
{
	this->m_dLeftMargin = 0;
	this->m_dRightMargin = 0;
	this->m_dTopMargin = 0;
	this->m_dBottomMargin = 0;
}

wxReportImageStyle::wxReportImageStyle(const wxString& name)
{
	this->m_sName = name;
	this->m_dLeftMargin = 0;
	this->m_dRightMargin = 0;
	this->m_dTopMargin = 0;
	this->m_dBottomMargin = 0;
}

void wxReportImageStyle::SetMargins(double left, double right, double top, double bottom)
{
	if(left >= 0) this->m_dLeftMargin = left;		
	if(right >= 0) this->m_dRightMargin = right;		
	if(top >= 0) this->m_dTopMargin = top;		
	if(bottom >= 0)	this->m_dBottomMargin = bottom;
}

double wxReportImageStyle::GetLeftMargin()
{
	return this->m_dLeftMargin;
}

double wxReportImageStyle::GetRightMargin()
{
	return this->m_dRightMargin;
}

double wxReportImageStyle::GetTopMargin()
{
	return this->m_dTopMargin;
}

double wxReportImageStyle::GetBottomMargin()
{
	return this->m_dBottomMargin;
}

wxXmlNode* wxReportImageStyle::CreateXmlNode()
{
	wxXmlNode *node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Style"));
	
	node->AddAttribute( wxT("StyleType"), wxT("Image") );
	node->AddAttribute( wxT("StyleName"), this->m_sName );
	node->AddAttribute( wxT("Borders"), wxString::Format(wxT("%d"), this->m_iBorder) );
	node->AddAttribute( wxT("BorderThickness"), wxString::Format(wxT("%G"), this->m_dBorderThickness) );
	
	node->AddAttribute( wxT("BorderColor"), wxString::Format(wxT("%d,%d,%d"),
											this->m_BorderColor.Red(),
											this->m_BorderColor.Blue(),
											this->m_BorderColor.Green()) );
											
	node->AddAttribute( wxT("Margins"), wxString::Format(wxT("%G,%G,%G,%G"),
										this->m_dLeftMargin,
										this->m_dRightMargin,
										this->m_dTopMargin,
										this->m_dBottomMargin));
										
	return node;
}

bool wxReportImageStyle::RetrieveFromXmlNode(const wxXmlNode* node)
{
	long borders = -1, borderRed = -1, borderGreen = -1, borderBlue = -1;
	double thickness = -1, leftMargin = -1, rightMargin = -1, topMargin = -1, bottomMargin = -1;
	
	wxString type = node->GetAttribute(wxT("StyleType"), wxT(""));
	wxString name = node->GetAttribute(wxT("StyleName"), wxT(""));
	node->GetAttribute(wxT("Borders"), wxT("")).ToLong(&borders);
	node->GetAttribute(wxT("BorderThickness"), wxT("")).ToDouble(&thickness);
	
	wxString colorStr = node->GetAttribute(wxT("BorderColor"), wxT(""));
	wxSscanf(colorStr, wxT("%d, %d, %d"), &borderRed, &borderGreen, &borderBlue);
	
	wxString margins = node->GetAttribute(wxT("Margins"), wxT(""));
	wxStringTokenizer tokenizer(margins, wxT(","), wxTOKEN_STRTOK);
	tokenizer.GetNextToken().ToDouble(&leftMargin);
	tokenizer.GetNextToken().ToDouble(&rightMargin);
	tokenizer.GetNextToken().ToDouble(&topMargin);
	tokenizer.GetNextToken().ToDouble(&bottomMargin);
	/*wxSscanf(margins, wxT("%lf, %lf, %lf, %lf"), &leftMargin, &rightMargin,
			&topMargin, &bottomMargin);*/
	
	if(type != wxT("Image")) return false;
	
	this->m_sName = name;
			
	if(borders >= 0 && borders <= 16)
		this->m_iBorder = (int)borders;
	else
		this->m_iBorder = wxRP_NOBORDER;
		
	if(thickness >= 0)
		this->m_dBorderThickness = thickness;
	else
		this->m_dBorderThickness = 0.5;
		
	if(borderRed >= 0 && borderRed <= 255 && borderBlue >= 0 && borderBlue <= 255 && borderGreen >= 0 && borderGreen <= 255)
	{
		wxColour color((unsigned char)borderRed, (unsigned char)borderBlue, (unsigned char)borderGreen);
		this->m_BorderColor = color;
	}
	else
		this->m_BorderColor = *wxBLACK;
	
	if(leftMargin >= 0 && rightMargin >= 0 && topMargin >= 0 && bottomMargin >= 0)
	{
		this->m_dLeftMargin = leftMargin;
		this->m_dRightMargin = rightMargin;
		this->m_dTopMargin = topMargin;
		this->m_dBottomMargin = bottomMargin;
	}
	else
	{
		this->m_dLeftMargin = 0;
		this->m_dRightMargin = 0;
		this->m_dTopMargin = 0;
		this->m_dBottomMargin = 0;
	}
	
	return true;
}

///////////// wxReportTableStyle class

wxReportTableStyle::wxReportTableStyle()
{
	this->m_dLeftMargin = 0;
	this->m_dRightMargin = 0;
	this->m_dTopMargin = 0;
	this->m_dBottomMargin = 0;
	this->m_iBorder = wxRP_ALLINBORDER;
}

wxReportTableStyle::wxReportTableStyle(const wxString& name)
{
	this->m_sName = name;
	this->m_dLeftMargin = 0;
	this->m_dRightMargin = 0;
	this->m_dTopMargin = 0;
	this->m_dBottomMargin = 0;
	this->m_iBorder = wxRP_ALLINBORDER;
}

wxXmlNode* wxReportTableStyle::CreateXmlNode()
{
	wxXmlNode *node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Style"));
	
	node->AddAttribute( wxT("StyleType"), wxT("Table") );
	node->AddAttribute( wxT("StyleName"), this->m_sName );
	node->AddAttribute( wxT("Borders"), wxString::Format(wxT("%d"), this->m_iBorder) );
	node->AddAttribute( wxT("BorderThickness"), wxString::Format(wxT("%G"), this->m_dBorderThickness) );
	
	node->AddAttribute( wxT("BorderColor"), wxString::Format(wxT("%d,%d,%d"),
											this->m_BorderColor.Red(),
											this->m_BorderColor.Blue(),
											this->m_BorderColor.Green()) );
											
	node->AddAttribute( wxT("Margins"), wxString::Format(wxT("%G,%G,%G,%G"),
										this->m_dLeftMargin,
										this->m_dRightMargin,
										this->m_dTopMargin,
										this->m_dBottomMargin));										
	return node;
}

bool wxReportTableStyle::RetrieveFromXmlNode(const wxXmlNode* node)
{
	long borders = -1, borderRed = -1, borderGreen = -1, borderBlue = -1;
	double thickness = -1, leftMargin = -1, rightMargin = -1, topMargin = -1, bottomMargin = -1;
	
	wxString type = node->GetAttribute(wxT("StyleType"), wxT(""));
	wxString name = node->GetAttribute(wxT("StyleName"), wxT(""));
	node->GetAttribute(wxT("Borders"), wxT("")).ToLong(&borders);
	node->GetAttribute(wxT("BorderThickness"), wxT("")).ToDouble(&thickness);
	
	wxString colorStr = node->GetAttribute(wxT("BorderColor"), wxT(""));
	wxSscanf(colorStr, wxT("%d, %d, %d"), &borderRed, &borderGreen, &borderBlue);
	
	wxString margins = node->GetAttribute(wxT("Margins"), wxT(""));
	wxStringTokenizer tokenizer(margins, wxT(","), wxTOKEN_STRTOK);
	tokenizer.GetNextToken().ToDouble(&leftMargin);
	tokenizer.GetNextToken().ToDouble(&rightMargin);
	tokenizer.GetNextToken().ToDouble(&topMargin);
	tokenizer.GetNextToken().ToDouble(&bottomMargin);
	//wxSscanf(margins, wxT("%d, %d, %d, %d"), &leftMargin, &rightMargin,	&topMargin, &bottomMargin);
	
	if(type != wxT("Table")) return false;
	
	this->m_sName = name;
		
	if(borders >= 0 && borders <= 16)
		this->m_iBorder = (int)borders;
	else
		this->m_iBorder = wxRP_ALLINBORDER;
		
	if(thickness >= 0)
		this->m_dBorderThickness = thickness;
	else
		this->m_dBorderThickness = 0.5;
		
	if(borderRed >= 0 && borderRed <= 255 && borderBlue >= 0 && borderBlue <= 255 && borderGreen >= 0 && borderGreen <= 255)
	{
		wxColour color((unsigned char)borderRed, (unsigned char)borderBlue, (unsigned char)borderGreen);
		this->m_BorderColor = color;
	}
	else
		this->m_BorderColor = *wxBLACK;
	
	if(leftMargin >= 0 && rightMargin >= 0 && topMargin >= 0 && bottomMargin >= 0)
	{
		this->m_dLeftMargin = leftMargin;
		this->m_dRightMargin = rightMargin;
		this->m_dTopMargin = topMargin;
		this->m_dBottomMargin = bottomMargin;
	}
	else
	{
		this->m_dLeftMargin = 0;
		this->m_dRightMargin = 0;
		this->m_dTopMargin = 0;
		this->m_dBottomMargin = 0;
	}
	
	return true;
}