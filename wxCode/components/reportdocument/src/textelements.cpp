#include "wx/report/textelements.h"
#include "wx/arrimpl.cpp"
#include "math.h"

#define REFRESH_PARAGRAPH_STYLE

WX_DEFINE_USER_EXPORTED_OBJARRAY(TextValuesArray);
WX_DEFINE_USER_EXPORTED_OBJARRAY(ParagraphsArray);
WX_DEFINE_OBJARRAY(wxArraySizes);
WX_DEFINE_OBJARRAY(wxArrayRects);

/////////////// wxReportTextValue class

wxReportTextValue::wxReportTextValue()
{
	this->m_iType = wxRP_TEXTVALUE;
	this->m_sValue = wxT("");
}

wxReportTextValue::wxReportTextValue(const wxString& name, bool isVariable)
{
	this->m_sName = name;
	this->m_iType = wxRP_TEXTVALUE;
	this->m_fVariable = isVariable;
	this->m_pVariable = NULL;
	this->m_sValue = wxT("");
}

void wxReportTextValue::SetValue(const short& value)
{
	this->DoSetValue(value, wxRP_SHORT);
}

void wxReportTextValue::SetValue(const int& value)
{
	this->DoSetValue(value, wxRP_INT);
}

void wxReportTextValue::SetValue(const long& value)
{
	this->DoSetValue(value, wxRP_LONG);
}

void wxReportTextValue::SetValue(const float& value)
{
	this->DoSetValue(value, wxRP_FLOAT);
}

void wxReportTextValue::SetValue(const double& value)
{
	this->DoSetValue(value, wxRP_DOUBLE);
}

void wxReportTextValue::SetValue(const char* value)
{
	wxString val = wxT("");
	
	for(const char *pValue=value; *pValue!='\0'; ++pValue)
		val += *pValue;
}

void wxReportTextValue::SetValue(const wxString& value)
{
	this->m_sValue = value;
	this->m_iValueType = wxRP_WXSTRING;
	
	if(this->m_fVariable)
		this->m_pVariable = (const void*)&value;
}

template<class T> void wxReportTextValue::DoSetValue(const T& value, int typeId)
{
	wxString valFormat;
	
	if(typeId == wxRP_FLOAT || typeId == wxRP_DOUBLE)
		valFormat = wxT("%G");
	else
		valFormat = wxT("%d");
		
	this->m_sValue = wxString::Format(valFormat, value);
	this->m_iValueType = typeId;
	
	if(this->m_fVariable)
		this->m_pVariable = (const void*)&value;
}

void wxReportTextValue::SetStyle(const wxReportTextStyle& style)
{
	this->m_style = style;
}

wxReportTextStyle& wxReportTextValue::GetStyle()
{
	return this->m_style;
}

const wxString& wxReportTextValue::GetValue() const
{
	return this->m_sValue;
}

void wxReportTextValue::RefreshVariable()
{
	if(this->m_fVariable && this->m_pVariable)
	{
		switch(this->m_iValueType)
		{
			case wxRP_SHORT:
				this->m_sValue = wxString::Format(wxT("%d"), *((const short*)this->m_pVariable));
				break;
				
			case wxRP_INT:
				this->m_sValue = wxString::Format(wxT("%d"), *((const int*)this->m_pVariable));
				break;
				
			case wxRP_LONG:
				this->m_sValue = wxString::Format(wxT("%d"), *((const long*)this->m_pVariable));
				break;
				
			case wxRP_FLOAT:
				this->m_sValue = wxString::Format(wxT("%lf"), *((const float*)this->m_pVariable));
				break;
				
			case wxRP_DOUBLE:
				this->m_sValue = wxString::Format(wxT("%lf"), *((const double*)this->m_pVariable));
				break;
				
			case wxRP_WXSTRING:
				this->m_sValue = *(const wxString*)m_pVariable;
				break;
		}
	}
}

wxXmlNode* wxReportTextValue::CreateXmlNode()
{
	wxXmlNode *mainNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Text"));//this->m_Style.CreateXmlNode();
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
	mainNode->AddProperty(this->m_style.CreateXmlNode()->GetProperties());
	mainNode->AddChild(valueNode);	
	
	return mainNode;
}

bool wxReportTextValue::RetrieveFromXmlNode(wxXmlNode* node)
{
	try
	{
		int type = -1, variable = 0;
		
		wxSscanf(node->GetAttribute(wxT("Type"), wxT("")), wxT("%d"), &type);
		if(type == wxRP_TEXTVALUE)
			this->m_iType = type;
		else
			return false;
		
		this->m_sName = node->GetAttribute(wxT("Name"), wxT(""));
		
		wxSscanf(node->GetAttribute(wxT("Variable"), wxT("")), wxT("%d"), &variable);
		this->m_fVariable = variable;
		
		if(!this->m_style.RetrieveFromXmlNode(node))
			return false;
		
		/*if(this->m_fVariable)
			this->m_sValue = wxT("");
		else*/
			this->m_sValue = node->GetChildren()->GetChildren()->GetContent();
	}
	catch(...)
	{
		return false;
	}
	
	return true;
}

/////////////////// wxReportTextParagraph class

wxReportTextParagraph::wxReportTextParagraph()
{
	this->m_iType = wxRP_PARAGRAPH;
}

wxReportTextParagraph::wxReportTextParagraph(const wxReportTextParagraph& sourcePar)
{
	this->DoCopy(sourcePar);
}

wxReportTextParagraph::wxReportTextParagraph(const wxReportParagraphStyle& style)
{
	this->SetStyle(style);
	this->m_iType = wxRP_PARAGRAPH;
}

wxReportTextParagraph::~wxReportTextParagraph()
{
	this->RemoveAll();
}

wxReportTextParagraph& wxReportTextParagraph::operator= (const wxReportTextParagraph &sourcePar)
{
	if(this == &sourcePar)
        return *this;
	
	this->DoCopy(sourcePar);	
	return *this;
}

void wxReportTextParagraph::DoCopy(const wxReportTextParagraph& sourcePar)
{
	this->m_fVariable = sourcePar.m_fVariable;
	this->m_iType = sourcePar.m_iType;
	this->m_pVariable = NULL;
	this->m_sName = sourcePar.m_sName;
	this->m_sValue = wxT("");
	this->m_style = sourcePar.m_style;
	int textsCount = sourcePar.m_arTextValues.GetCount();
	
	for(int i=0; i<textsCount; ++i)
	{
		wxReportTextValue *pText = new wxReportTextValue(*(sourcePar.m_arTextValues.Item(i)));
		this->m_arTextValues.Add(pText);
	}
}

void wxReportTextParagraph::AddText(const wxReportTextValue& text)
{
	wxReportTextValue *value = new wxReportTextValue(text);
	this->m_arTextValues.Add(value);
	this->SetLineHeightFromFontSize(value->GetStyle().GetFontSize());
}

void wxReportTextParagraph::AddText(wxReportTextValue* text)
{
	if(text)
	{
		this->m_arTextValues.Add(text);
		this->SetLineHeightFromFontSize(text->GetStyle().GetFontSize());
	}
}

void wxReportTextParagraph::RemoveLast()
{
	int count = this->m_arTextValues.GetCount();	
	wxReportTextValue *pText = this->m_arTextValues.Last();
		
	delete pText;
	this->m_arTextValues.RemoveAt(count - 1);
}

void wxReportTextParagraph::RemoveAt(int index)
{
	wxReportTextValue *pText = this->m_arTextValues.Item(index);	
		
	delete pText;
	this->m_arTextValues.RemoveAt(index);
}

void wxReportTextParagraph::RemoveAll()
{
	int count = this->m_arTextValues.GetCount();
	
	for(int i=0; i<count; ++i)
		delete this->m_arTextValues.Item(i);
		
	this->m_arTextValues.Clear();
}

void wxReportTextParagraph::SetStyle(const wxReportParagraphStyle& style)
{
	this->m_style = style;
}

void wxReportTextParagraph::SetLineHeightFromFontSize(int fontPointSize)
{
	double mmSize = ceil((fontPointSize * 25.4) / 72.);
	
	if(mmSize > this->m_style.GetLineHeight())
		this->m_style.SetLineHeight(mmSize);
}

wxReportParagraphStyle& wxReportTextParagraph::GetStyle()
{
	return this->m_style;
}

int wxReportTextParagraph::GetCount() const
{
	return this->m_arTextValues.GetCount();
}

const wxReportTextValue* wxReportTextParagraph::GetTextValue(int index) const
{
	if(index >= 0 && index < (int)(this->m_arTextValues.GetCount()))
		return this->m_arTextValues.Item(index);
	else
		return NULL;
}

wxReportTextValue wxReportTextParagraph::GetWord(int index)
{
	int indexAcc = 0; //index range for whole words in all previous textvalues
	int nTexts = this->m_arTextValues.GetCount();
	
	for(int i=0; i<nTexts; ++i)
	{
		wxReportTextValue *pText = this->m_arTextValues.Item(i);
		wxString str = pText->GetValue();

		str.Replace(wxT("\t"), wxT(" "));
		
		while(str.Contains(wxT("  ")))
			str.Replace(wxT("  "), wxT(" "));
			
		if(str.EndsWith(wxT(" ")))
			str.Truncate(str.Length() - 1);
		
		int nSpaces = str.Freq(' '); // nSpaces = index range of the words in textvalue
				
		if(index <= nSpaces + indexAcc)
		{
			int strI = index - indexAcc;
			wxString retStr = str;
			for(int s=0; s<strI; ++s)
			{
				retStr = retStr.AfterFirst(' ');
			}
			
			retStr = retStr.BeforeFirst(' ');
			wxReportTextValue returnedText = *pText;
			returnedText.SetValue(retStr);
			return returnedText;
		}
		else
		{
			indexAcc += nSpaces;
			++indexAcc;
		}
	}
	
	return wxReportTextValue();
}

int wxReportTextParagraph::GetWordsCount()
{
	int nWords = 0;
	int nTexts = this->m_arTextValues.GetCount();
	
	for(int t=0; t<nTexts; ++t)
	{
		wxString text = this->GetTextValue(t)->GetValue();
		wxChar tab = 9;
		text.Replace(wxString(tab), wxT(" "));
		
		while(text.Contains(wxT("  ")))
			text.Replace(wxT("  "), wxT(" "));
			
		if(text.EndsWith(wxT(" ")))
			text.Truncate(text.Length() - 1);
			
		nWords += text.Freq(' ') + 1;
	}
	
	return nWords;
}

wxXmlNode* wxReportTextParagraph::CreateXmlNode()
{
	wxXmlNode *paragraphNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Paragraph"));
	paragraphNode->AddAttribute(wxT("Type"), wxString::Format(wxT("%d"), this->m_iType));
	paragraphNode->AddAttribute(wxT("Name"), this->m_sName);
	paragraphNode->AddProperty(this->m_style.CreateXmlNode()->GetAttributes());
	
	int count = this->m_arTextValues.GetCount();
	
	for(int i=0; i<count; ++i)
		paragraphNode->AddChild(this->m_arTextValues.Item(i)->CreateXmlNode());
	
	return paragraphNode;
}

bool wxReportTextParagraph::RetrieveFromXmlNode(wxXmlNode* node)
{
	try
	{
		int type = -1;
		
		wxSscanf(node->GetAttribute(wxT("Type"), wxT("")), wxT("%d"), &type);
		if(type == wxRP_PARAGRAPH)
			this->m_iType = type;
		else return false;
		
		this->m_sName = node->GetAttribute(wxT("Name"), wxT(""));		
		
		if(!this->m_style.RetrieveFromXmlNode(node)) return false;
		
		wxXmlNode *textNode = node->GetChildren();
		
		while(textNode)
		{
			wxReportTextValue *pText = new wxReportTextValue();
			if(pText->RetrieveFromXmlNode(textNode))
			{
				this->m_arTextValues.Add(pText);
			}
	
			textNode = textNode->GetNext();
		}
	}
	catch(...)
	{
		return false;
	}
	
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// wxReportTextItem Class Implementation
/////////////////////////////////////////////////////////////////////////////////////////////

wxReportTextItem::wxReportTextItem()
{
	this->m_arParagraphs.Add(new wxReportTextParagraph());
	this->m_iType = wxRP_TEXT;
	this->m_dHeight = 0.;
	this->m_dWidth = 0.;
}

wxReportTextItem::wxReportTextItem(const wxString& name, double x, double y)
{
	this->m_sName = name;
	this->m_iType = wxRP_TEXT;
	this->m_dHeight = 0.;
	this->m_dWidth = 0.;
	
	if(x >= 0 && y >= 0)
		this->m_position = wxRealPoint(x, y);
	else
		this->m_position = wxRealPoint(0, 0);
		
	this->m_arParagraphs.Add(new wxReportTextParagraph());
}

wxReportTextItem::wxReportTextItem(const wxXmlNode* node)
{
	this->RetrieveFromXmlNode(node);
}

wxReportTextItem::wxReportTextItem(const wxReportTextItem& sourceItem)
{
	this->DoCopy(sourceItem);
}

wxReportTextItem::~wxReportTextItem()
{
	int nPars = this->m_arParagraphs.GetCount();
	
	for(int i=0; i<nPars; ++i)
		delete this->m_arParagraphs.Item(i);
		
	this->m_arParagraphs.Clear();
}

wxReportTextItem& wxReportTextItem::operator= (const wxReportTextItem &sourceItem)
{
	if(this == &sourceItem)
		return *this;	
	
	this->DoCopy(sourceItem);
	return *this;
}

void wxReportTextItem::DoCopy(const wxReportTextItem& sourceItem)
{
	this->ClearAllText();
	this->m_arParagraphs.Clear();
	this->m_activeParagraphStyle = sourceItem.m_activeParagraphStyle;
	this->m_activeTextStyle = sourceItem.m_activeTextStyle;
	this->m_fVariable = sourceItem.m_fVariable;
	this->m_iType = sourceItem.m_iType;
	this->m_pVariable = NULL;
	this->m_position = sourceItem.m_position;
	this->m_sName = sourceItem.m_sName;
	this->m_sValue = wxT("");
	this->m_dHeight = sourceItem.m_dHeight;
	this->m_dWidth = sourceItem.m_dWidth;
	this->m_style = sourceItem.m_style;
	
	int parsCount = sourceItem.m_arParagraphs.GetCount();
	for(int i=0; i<parsCount; ++i)
	{
		wxReportTextParagraph *pPar = new wxReportTextParagraph(*(sourceItem.m_arParagraphs.Item(i)));
		this->m_arParagraphs.Add(pPar);
	}
}

void wxReportTextItem::SetSize(double width, double height)
{
	this->m_dWidth = width;
	this->m_dHeight = height;
}

wxRealPoint wxReportTextItem::GetSize()
{
	return wxRealPoint(this->m_dWidth, this->m_dHeight);
}

void wxReportTextItem::AddVariable(const wxString& name)
{
	wxReportTextValue* pText = new wxReportTextValue(name, true);
	wxReportTextParagraph *pPar = this->m_arParagraphs.Last();
	pPar->AddText(*pText);
}

void wxReportTextItem::AddVariable(const short& value, const wxString& name)
{
	this->DoAddVariable(value, name);
}

void wxReportTextItem::AddVariable(const int& value, const wxString& name)
{
	this->DoAddVariable(value, name);
}

void wxReportTextItem::AddVariable(const long& value, const wxString& name)
{
	this->DoAddVariable(value, name);
}

void wxReportTextItem::AddVariable(const float& value, const wxString& name)
{
	this->DoAddVariable(value, name);
}

void wxReportTextItem::AddVariable(const double& value, const wxString& name)
{
	this->DoAddVariable(value, name);
}

void wxReportTextItem::AddVariable(const char& value, const wxString& name)
{
	this->DoAddVariable(value, name);
}

void wxReportTextItem::AddVariable(const wxString& value, const wxString& name)
{
	this->DoAddVariable(value, name);
}

template<class T> void wxReportTextItem::DoAddVariable(const T& value, const wxString& name)
{
	wxReportTextValue* pText = new wxReportTextValue();
	pText->m_sName = name;
	pText->m_style = this->m_activeTextStyle;
	pText->m_fVariable = true;
	pText->SetValue(value);	
	
	wxReportTextParagraph *pPar = this->m_arParagraphs.Last();
	pPar->AddText(pText);
}

void wxReportTextItem::SetStyle(const wxReportStyle& style)
{
	this->m_style = style;
}

wxReportStyle wxReportTextItem::GetStyle()
{
	return this->m_style;
}

void wxReportTextItem::SetActiveParagraphStyle(const wxReportParagraphStyle& style)
{
	this->m_activeParagraphStyle = style;
	this->m_arParagraphs.Last()->m_style = style;
}

void wxReportTextItem::SetActiveTextStyle(const wxReportTextStyle& style)
{
	this->m_activeTextStyle = style;
}

void wxReportTextItem::SetFont(const wxFont& font)
{
	this->m_activeTextStyle.m_font = font;
}

void wxReportTextItem::SetFontSize(int fontSize)
{
	this->m_activeTextStyle.m_font.SetPointSize(fontSize);
}

void wxReportTextItem::SetFontStyle(wxFontStyle fontStyle)
{
	this->m_activeTextStyle.m_font.SetStyle(fontStyle);
}

void wxReportTextItem::SetFontUnderlined(bool underlined)
{
	this->m_activeTextStyle.m_font.SetUnderlined(underlined);
}

void wxReportTextItem::SetFontColor(const wxColour& fontColor)
{
	this->m_activeTextStyle.m_FontColor = fontColor;
}

void wxReportTextItem::SetTextAlign(int alignType)
{
	this->m_activeParagraphStyle.SetTextAlign(alignType);
	this->m_arParagraphs.Last()->m_style.SetTextAlign(alignType);
}

void wxReportTextItem::AddNewLine()
{
	wxReportTextValue *value = new wxReportTextValue();
	value->SetStyle(this->m_activeTextStyle);
	value->SetValue(wxT("/NewLine"));
	this->m_arParagraphs.Last()->AddText(value);
}

void wxReportTextItem::AddNewParagraph()
{
	wxReportTextParagraph *tPar = new wxReportTextParagraph( this->m_activeParagraphStyle);
	if(tPar) this->m_arParagraphs.Add(tPar);
}

void wxReportTextItem::AddText(const wxString& text)
{
	wxReportTextValue *pValue = new wxReportTextValue();
	pValue->SetStyle(this->m_activeTextStyle);
	pValue->SetValue(text);
	
	if(pValue)
	{
		this->m_arParagraphs.Last()->AddText(pValue);
	}
}

void wxReportTextItem::AddText(const wxReportTextValue& textValue)
{
	wxReportTextValue *pValue = new wxReportTextValue(textValue);
	if(pValue)
		this->m_arParagraphs.Last()->AddText(pValue);
}

void wxReportTextItem::ClearLastParagraph()
{
	this->m_arParagraphs.Last()->RemoveAll();
}

void wxReportTextItem::ClearParagraph(int index)
{
	this->m_arParagraphs.Item(index)->RemoveAll();
}

void wxReportTextItem::ClearAllText()
{
	int parsCount = this->m_arParagraphs.GetCount();
	
	for(int i=0; i<parsCount; i++)
		delete this->m_arParagraphs.Item(i);
	
	this->m_arParagraphs.Clear();
	//wxReportTextParagraph *pPar = new wxReportTextParagraph(this->m_activeParagraphStyle);
	this->m_arParagraphs.Add(new wxReportTextParagraph(this->m_activeParagraphStyle));
}

void wxReportTextItem::Clear()
{
	int parsCount = this->m_arParagraphs.GetCount();
	for(int i=0; i<parsCount; ++i)
	{
		delete this->m_arParagraphs.Item(i);
	}
	this->m_arParagraphs.Clear();
	
	this->m_pVariable = NULL;
	this->m_sValue = wxT("");
}

const wxReportTextValue* wxReportTextItem::GetTextValue(int textIndex, int paragraphIndex) const
{
	const wxReportTextValue *pText = NULL;
	
	if(paragraphIndex < (int)this->m_arParagraphs.GetCount())
	{
		wxReportTextParagraph *pPar;
		
		if(paragraphIndex < 0)
			pPar = this->m_arParagraphs.Last();
		else
			pPar = this->m_arParagraphs.Item(paragraphIndex);
			
		if(textIndex >= 0 && textIndex < pPar->GetCount())
			pText = pPar->GetTextValue(textIndex);
	}
	
	return pText;
}

void wxReportTextItem::RemoveLast()
{
	int count = this->m_arParagraphs.GetCount();
	
	if(this->m_arParagraphs.Last()->GetCount() > 0)
		this->m_arParagraphs.Last()->RemoveLast();
	else
	{
		delete this->m_arParagraphs.Item(count - 1);
		this->m_arParagraphs.RemoveAt(count - 1);
	}
}

void wxReportTextItem::RemoveLastParagraph()
{
	int count = this->m_arParagraphs.GetCount();
	delete this->m_arParagraphs.Item(count - 1);
	this->m_arParagraphs.RemoveAt(count - 1);
}

void wxReportTextItem::RemoveParagraph(int index)
{
	delete this->m_arParagraphs.Item(index);
	this->m_arParagraphs.RemoveAt(index);
}

void wxReportTextItem::RemoveText(int index)
{
}

int wxReportTextItem::GetParagraphsCount()
{
	return this->m_arParagraphs.GetCount();
}

const wxReportTextParagraph* wxReportTextItem::GetParagraph(int index) const
{
	if(index >= 0 && index < (int)(this->m_arParagraphs.GetCount()))
		return this->m_arParagraphs.Item(index);
	else
		return NULL;
}

/*double wxReportTextItem::CalculateWidth(wxDC* dc, int pxLeftBorder, int pxRightBorder, int pxPageWidth)
{
	
}*/

double wxReportTextItem::CalculateTopLeftPosition(wxDC *dc, bool toScreen, double itemWidth, double &itemHeight, int pxTopBorder, int pxBottomBorder, int pxPageHeight, int pxPageWidth)
{
	if((int)(this->m_position.y) == wxRP_TOP)
		return (double)pxTopBorder;
		
	int pos_y = this->m_position.y; 
	if(pos_y != wxRP_TOP && pos_y != wxRP_CENTER && pos_y != wxRP_BOTTOM)
		return MM2PX(this->m_position.y, dc, toScreen);
	
	double totalHeight = 0;
	int nPars = this->m_arParagraphs.GetCount();
	wxReportTextParagraph *pPar;
	wxReportTextValue *pText;
	//wxArrayDouble parWidths;
	double iw = itemWidth;
	
	for(int p=0; p<nPars; ++p)
	{
		pPar = this->m_arParagraphs.Item(p);
		int nTexts = pPar->GetCount();
		//int newLines = 0;
		double parWidth = 0;
		int parLines = 0;
		int pxLineHeight = MM2PX(pPar->GetStyle().GetLineHeight(), dc, toScreen);
		int pxParSpace = MM2PX(pPar->GetStyle().GetParagraphsSpace(), dc, toScreen);
		
		for(int t=0; t<nTexts; ++t)
		{
			pText = pPar->m_arTextValues.Item(t);
			wxString value = pText->GetValue();
			
			if(value == wxT("/NewLine"))
			{
				if(itemWidth == 0)
				{
					if(parWidth >= pxPageWidth)
						iw = pxPageWidth;
					else
						iw = parWidth;
				}
				
				parLines = (int)(parWidth / iw);
				
				if((int)parWidth % (int)iw > 0)
					++parLines;
				
				totalHeight += parLines * pxLineHeight;
				parWidth = 0;
				continue;
			}
			
			wxFont font = pText->GetStyle().GetFont();
			int w, h;
			dc->GetTextExtent(pText->GetValue(), &w, &h, NULL, NULL, &font);
			parWidth += w;
		}
		
		//parWidths.Add(parWidth);
		if(itemWidth == 0)
		{
			if(parWidth >= pxPageWidth)
				iw = pxPageWidth;
			else
				iw = parWidth;
		}
		parLines = (int)(parWidth / iw);
		if((int)parWidth % (int)iw > 0)
			++parLines;
		totalHeight += parLines * pxLineHeight + pxParSpace;
	}
	
	itemHeight = totalHeight;
	
	if((int)(this->m_position.y) == wxRP_CENTER)
		return (pxPageHeight - pxTopBorder - pxBottomBorder) / 2. - totalHeight / 2. + pxTopBorder;
		
	if((int)(this->m_position.y) == wxRP_BOTTOM)
		return (double)(pxPageHeight - pxBottomBorder - totalHeight);
		
	return 0.;
}

wxXmlNode* wxReportTextItem::CreateXmlNode()
{
	wxXmlNode *itemNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Item"));
	itemNode->AddAttribute( wxT("Type"), wxString::Format(wxT("%d"), this->m_iType) );
	itemNode->AddAttribute( wxT("Name"), this->m_sName);
	itemNode->AddAttribute( wxT("X"), wxString::Format(wxT("%G"), this->m_position.x) );
	itemNode->AddAttribute( wxT("Y"), wxString::Format(wxT("%G"), this->m_position.y) );
	itemNode->AddAttribute( wxT("Width"), wxString::Format(wxT("%G"), this->m_dWidth) );
	itemNode->AddAttribute( wxT("Height"), wxString::Format(wxT("%G"), this->m_dHeight) );
	
	int paragraphsCount = this->m_arParagraphs.GetCount();
	wxXmlNode *paragraphNode;
	
	for(int i=0; i<paragraphsCount; ++i)
	{
		paragraphNode = this->m_arParagraphs.Item(i)->CreateXmlNode();
		
		if(paragraphNode)
			itemNode->AddChild(paragraphNode);
	}
		
	return itemNode;
}

bool wxReportTextItem::RetrieveFromXmlNode(const wxXmlNode* node)
{
	long type = -1;
	double x = 0, y = 0, w = 0, h = 0;
	
	node->GetAttribute(wxT("Type"), wxT("")).ToLong(&type);	
	if(type != wxRP_TEXT) return false;
	
	wxString name = node->GetAttribute(wxT("Name"), wxT(""));
	wxSscanf(node->GetAttribute(wxT("X"), wxT("")), wxT("%lf"), &x);
	wxSscanf(node->GetAttribute(wxT("Y"), wxT("")), wxT("%lf"), &y);
	wxSscanf(node->GetAttribute(wxT("Width"), wxT("")), wxT("%lf"), &w);
	wxSscanf(node->GetAttribute(wxT("Height"), wxT("")), wxT("%lf"), &h);
	
	this->m_sName = name;
	this->m_position = wxRealPoint(x, y);
	this->m_dWidth = w;
	this->m_dHeight = h;
			
	int paragraphCount = this->m_arParagraphs.GetCount();
	for(int i=0; i<paragraphCount; ++i)
	{
		delete this->m_arParagraphs.Item(i);
	}
	this->m_arParagraphs.Clear();
		
	wxXmlNode *paragraphNode = node->GetChildren();
		
	while(paragraphNode)
	{
		wxReportTextParagraph *paragraph = new wxReportTextParagraph();
		
		if(paragraph->RetrieveFromXmlNode(paragraphNode))
			this->m_arParagraphs.Add(paragraph);
			
		paragraphNode = paragraphNode->GetNext();
	}
	
	return true;
}

void wxReportTextItem::DrawToDC(wxDC* dc, bool toScreen, const wxReportPageStyle& pageStyle)
{
	int parsCount = this->m_arParagraphs.GetCount();
	double totalHeight = 0.;
	double itemHeight = 0;
	int lm = MM2PX(pageStyle.GetLeftMargin(), dc, toScreen); // apge margins
	int rm = MM2PX(pageStyle.GetRightMargin(), dc, toScreen);
	int tm = MM2PX(pageStyle.GetTopMargin(), dc, toScreen);
	int bm = MM2PX(pageStyle.GetBottomMargin(), dc, toScreen);
	int pw = MM2PX(pageStyle.GetSize().x, dc, toScreen); // page width
	int ph = MM2PX(pageStyle.GetSize().y, dc, toScreen); // page height
	int iw = MM2PX(this->m_dWidth, dc, toScreen); // item width
	wxPoint textItemPosPx(MM2PX(m_position.x, dc, toScreen), MM2PX(m_position.y, dc, toScreen));
	wxPoint parPosPx = CalcNegPos(this->m_position, textItemPosPx, iw, MM2PX(this->m_dHeight, dc, toScreen), dc);
	parPosPx.x += lm; // position have to be increased of a margins
	parPosPx.y = this->CalculateTopLeftPosition(dc, toScreen, iw, itemHeight, tm, bm, ph, pw); // calculate y-position of the text item //+= tm;
	textItemPosPx = parPosPx;
	
	if(this->m_dWidth <= 0) // if item width = 0, than width = from start to mleft margin
		iw = pw - lm - parPosPx.x - rm;
	
	//wxPoint parPos(MM2PX(parPosMM.x, dc, toScreen), MM2PX(parPosMM.y, dc, toScreen));
	int blockWidth = iw;//MM2PX(iw, dc, toScreen);
	
	if(this->m_style.GetBorder() > 0)
	{
		wxColour borderColor = this->m_style.GetBorderColor();
		int borderWidth = MM2PX(this->m_style.GetBorderThickness(), dc, toScreen);
		wxPen pen(borderColor, borderWidth);
		pen.SetCap(wxCAP_BUTT);
		dc->SetPen(pen);
			
		wxColour bgColor = this->m_style.GetBackgroundColor();
		if(bgColor == wxNullColour)
			dc->SetBrush(*wxTRANSPARENT_BRUSH);
		else
		{
			dc->SetBrush(wxBrush(bgColor));
			//dc->SetLogicalFunction(wxSET   );
			if(this->m_style.GetBorder() == 0)
			{
				pen.SetColour(bgColor);
				dc->SetPen(pen);
			}
		}
			
		DrawBorder(dc, this->m_style.GetBorder(), textItemPosPx.x, textItemPosPx.y, iw, itemHeight);
	}
		
	for(int p=0; p<parsCount; ++p)
	{
		wxReportTextParagraph *pPar = this->m_arParagraphs.Item(p);
		int nLines = 0; // count of drawed text lines
		int tDrawn = 0; // count of drawed text values
		int lineWidth = 0; // width of the line
		bool drawLine = false; // flag, true when the line has to be drawn
		//int heightConst = pPar->m_style.GetLineHeight();
		double parHeight = 0.;
		double lineHeight = pPar->m_style.GetLineHeight(); //0.;
		//int lineHeight = MM2PX(pPar->m_style.GetLineHeight(), dc, toScreen);
		wxArraySizes strSizes;
		wxArrayRects strRects;
		wxArrayInt arrParIndexes;
		int textsCount = pPar->GetWordsCount();
		
		int t = 0;
		while(t < textsCount)
		{
			wxReportTextValue text = pPar->GetWord(t);
			bool fNewLn = false;
			
			if(text.m_sValue != wxT("/NewLine"))
			{
				int w, h;
				wxFont font = text.GetStyle().GetFont();
				dc->GetTextExtent(text.m_sValue + wxT(" "), &w, &h, NULL, NULL, &font); // calculates size of the string
				
				if(lineWidth + w <= blockWidth)
				{
					strSizes.Add(wxSize(w, h));
					lineWidth += w;
					++t;
				}
				else
					drawLine = true;
					
				if(t == textsCount)
					drawLine = true;
			}
			else
			{
				strSizes.Add(wxSize(0, 0));
				fNewLn = true;
				drawLine = true;
			}
			
			if(drawLine) // draw text line
			{
				int tx = parPosPx.x; // tx, ty = position of the string in the line
				int ty = parPosPx.y;
				
				if(nLines != 0)
					ty += parHeight;//nLines * lineHeight;
				
				int interleave = 0;
				int nSpaces;
				
				switch(pPar->m_style.GetTextAlign()) // set x position or word spacing for specified align
				{
					case wxRP_CENTERALIGN:
						tx += ((double)blockWidth - (double)lineWidth) / 2;
						break;
						
					case wxRP_JUSTIFY:
						nSpaces = t - tDrawn - 1;
						if(nSpaces > 0)
							interleave = ((double)blockWidth - (double)lineWidth) / (double)nSpaces;
						break;
						
					case wxRP_RIGHTALIGN:
						tx += blockWidth - lineWidth;
						break;
				}
				
				// calculate text position for defered rendering
				for(int td=tDrawn; td<t; ++td)
				{
					strRects.Add( wxRect( tx, ty, strSizes[td].x, strSizes[td].y ) );
					arrParIndexes.Add( td );
					tx += strSizes[td].x + interleave;
				}
				
				if(fNewLn)
				{
					++t;
					fNewLn = false;
				}
				
				parHeight += MM2PX(lineHeight, dc, toScreen);
				//lineHeight = 0;
				lineWidth = 0;
				drawLine = false;
				++nLines;
				tDrawn = t;
			}			
		}
		
		if(pPar->m_style.GetBorder() > 0) // draw box around the paragraph
		{
			int bx = parPosPx.x;
			int by = parPosPx.y;
			int bw = iw;//MM2PX(this->m_dWidth, dc, toScreen);
			int bh = parHeight;//nLines * MM2PX(pPar->m_style.GetLineHeight(), dc, toScreen);
			wxColour borderColor = pPar->m_style.GetBorderColor();
			int borderWidth = MM2PX(pPar->m_style.GetBorderThickness(), dc, toScreen);			
			wxPen pen(borderColor, borderWidth);
			pen.SetCap(wxCAP_BUTT);
			dc->SetPen(pen);
			
			wxColour bgColor = pPar->m_style.GetBackgroundColor();
			if(bgColor == wxNullColour)
				dc->SetBrush(*wxTRANSPARENT_BRUSH);
			else
			{
				dc->SetBrush(wxBrush(bgColor));
				if(pPar->m_style.GetBorder() == 0)
				{
					pen.SetColour(bgColor);
					dc->SetPen(pen);
				}
			}			
				
			DrawBorder(dc, pPar->m_style.GetBorder(), bx, by, bw, bh);
		}
		
		for(size_t ti=0; ti<arrParIndexes.GetCount(); ++ti)
		{
			wxSize tSize = strRects[ti].GetSize();
			wxPoint tPosition = strRects[ti].GetPosition();
			
			wxReportTextValue Td = pPar->GetWord( arrParIndexes[ti] );
			dc->SetFont(Td.m_style.GetFont());
			dc->SetTextForeground(Td.m_style.GetTextColor());
			
			wxColour bgColor = Td.m_style.GetBackgroundColor();
			if(bgColor != wxNullColour)
				dc->SetTextBackground(bgColor);		

			if(Td.m_style.GetBorder() > 0)
			{
				wxColour borderColor = Td.m_style.GetBorderColor();
				int borderWidth = MM2PX(Td.m_style.GetBorderThickness(), dc, toScreen);
				wxPen pen(borderColor, borderWidth);
				wxBrush brush(bgColor);
				pen.SetCap(wxCAP_BUTT);
				dc->SetPen(pen);
				if( bgColor != wxNullColour) dc->SetBrush(brush);
				dc->DrawRectangle(tPosition.x, tPosition.y, tSize.x, tSize.y);
			}			
			
			dc->DrawText(Td.m_sValue, tPosition.x, tPosition.y);
		}
		
		strRects.Clear();
		arrParIndexes.Clear();
		
		parPosPx.y += parHeight/*nLines * MM2PX(pPar->m_style.GetLineHeight(), dc, toScreen)*/ + MM2PX(pPar->m_style.GetParagraphsSpace(), dc, toScreen); // calculate position of the next paragraph, x-point will be the same
		totalHeight += parHeight;//nLines * MM2PX(pPar->m_style.GetLineHeight(), dc, toScreen);
		
		if(p != (int)(this->m_arParagraphs.GetCount()) - 1)
			totalHeight += MM2PX(pPar->m_style.GetParagraphsSpace(), dc, toScreen);
	}
}
