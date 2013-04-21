#ifndef TEXTELEMENTS_H
#define TEXTELEMENTS_H

#include "wx/report/pageitems.h"

/////////////// wxReportTextValue class declaration

/**
 * \brief Encapsulates the text value and it's source.
 */
class WXDLLIMPEXP_RP wxReportTextValue : public wxReportPageItem
{
protected:
	wxReportTextStyle m_style;
	template<class T> void DoSetValue(const T& value, int typeId);
	
public:
	/**
	 * \brief Default constructor.
	 */
	wxReportTextValue();
	/**
	 * \brief Constructor.
	 * \param name name of the item
	 * \param isVariable tells if the item will be filled from data
	 */
	wxReportTextValue(const wxString& name, bool isVariable = false);
	/**
	 * \brief Default destructor
	 */
	virtual ~wxReportTextValue() {;}
	/**
	 * \brief Set value of the item and creates the connection with source object if needed.
	 * \param value source object
	 */
	void SetValue(const short& value);
	/**
	 * \brief Set value of the item and creates the connection with source object if needed.
	 * \param value source object
	 */
	void SetValue(const int& value);
	/**
	 * \brief Set value of the item and creates the connection with source object if needed.
	 * \param value source object
	 */
	void SetValue(const long& value);
	/**
	 * \brief Set value of the item and creates the connection with source object if needed.
	 * \param value source object
	 */
	void SetValue(const float& value);
	/**
	 * \brief Set value of the item and creates the connection with source object if needed.
	 * \param value source object
	 */
	void SetValue(const double& value);
	/**
	 * \brief Set value of the item and creates the connection with source object if needed.
	 * \param value source object
	 */
	void SetValue(const char* value);
	/**
	 * \brief Set value of the item and creates the connection with source object if needed.
	 * \param value source object
	 */
	void SetValue(const wxString& value);
	/**
	 * \brief Set value of the item and creates the connection with source object if needed.
	 * \param value source object
	 */
	/**
	 * \brief Sets the style of the text.
	 * \param style text style object
	 */
	void SetStyle(const wxReportTextStyle& style);
	/**
	 * \brief Returns the the text value of the item. 
	 * \return text value
	 */
	const wxString& GetValue() const;
	/**
	 * \brief Returns reference to the style of the item.
	 * \return reference to the text style
	 */
	wxReportTextStyle& GetStyle();
	/**
	 * \brief Refresh value of the item if it's connected with source data object.
	 */
	void RefreshVariable();
	/**
	 * \brief Creates XML node for this text value item.
	 * \return XML node with item parameters
	 */
	virtual wxXmlNode* CreateXmlNode();
	/**
	 * \brief Retrieves the current text value item form the specified XML node.
	 * \param node XML node to retrieve
	 * \return TRUE on succes, FALSE if fails
	 */
	virtual bool RetrieveFromXmlNode(wxXmlNode* node);
	
	FRIENDS
	friend class wxReportTextItem;
};

WX_DECLARE_USER_EXPORTED_OBJARRAY(wxReportTextValue*, TextValuesArray, WXDLLIMPEXP_RP);

/////////////// wxReportTextParagraph class declaration

/**
 * \brief Represents the single text paragraph. Encapsulates the text value's objects
 */
class WXDLLIMPEXP_RP wxReportTextParagraph : public wxReportPageItem
{
protected:
	wxReportParagraphStyle m_style;
	TextValuesArray m_arTextValues;
	void DoCopy(const wxReportTextParagraph &sourcePar);
	
public:
	/**
	 * \brief Default constructor.
	 */
	wxReportTextParagraph();
	/**
	 * \brief Copy constructor.
	 * \param sourcePar source text paragraph object
	 */
	wxReportTextParagraph(const wxReportTextParagraph& sourcePar);
	/**
	 * \brief Constructor.
	 * \param style style for the new text paragraph item
	 */
	wxReportTextParagraph(const wxReportParagraphStyle& style);
	/**
	 * \brief Default destructor.
	 */
	~wxReportTextParagraph();
	/**
	 * \brief Assigment operator.
	 * \param sourcePar text paragraph object to assign
	 */
	wxReportTextParagraph& operator= (const wxReportTextParagraph &sourcePar);
	/**
	 * \brief Adds new text value item to the paragraph. Creates copy of the passed item.
	 * \param text text value object to insert
	 */
	void AddText(const wxReportTextValue& text);
	/**
	 * \brief Adds new text value item to the paragraph. Inserts directly to the paragraph.
	 * \param text pointer to the text value object
	 */
	void AddText(wxReportTextValue* text);
	/**
	 * \brief Removes last added text value object.
	 */
	void RemoveLast();
	/**
	 * \brief Removes the text value object at specified index.
	 * \param index index of the item to remove
	 */
	void RemoveAt(int index);
	/**
	 * \brief Removes all text values from the paragraph.
	 */
	void RemoveAll();
	/**
	 * \brief Sets the style of the paragraph.
	 * \param style paragraph style object
	 */
	void SetStyle(const wxReportParagraphStyle& style);
	/*!
	 * \brief Set line height from font size
	 * \param fontPointSize line height size
	 */
	void SetLineHeightFromFontSize(int fontPointSize);
	/**
	 * \brief Returns the reference to the style object associated with the paragraph.
	 * \return reference to the paragraph style
	 */
	wxReportParagraphStyle& GetStyle();
	/**
	 * \brief Return the number of text value objects int the paragraph.
	 * \return number of text value objects
	 */
	int GetCount() const;
	/**
	 * \brief Returns the constant pointer to the text value object at specified index.
	 * \param index index of the object
	 * \return constant pointer to the text value object
	 */
	const wxReportTextValue* GetTextValue(int index) const;
	/**
	 * \brief Return the one word from the paragraph at specified index position. Word means the text string between two spaces.
	 * \param index position index of the word - started from 0
	 * \return new text value object with the word
	 */
	wxReportTextValue GetWord(int index);
	/**
	 * \brief Returns the number of the words in the paragraph (see the GetTextValue method reference).
	 * \return number of words
	 */
	int GetWordsCount();
	/**
	 * \brief Create XML node for the text paragraph.
	 * \return XML node with the paragraph
	 */
	wxXmlNode* CreateXmlNode();
	/**
	 * \brief Retrieves the current text paragraph form XML node.
	 * \param node XML node to retrieve
	 * \return TRUE on succes, FALSE if fails
	 */
	bool RetrieveFromXmlNode(wxXmlNode* node);
	
	FRIENDS
	friend class wxReportTextItem;
	friend class wxReportDocument;
};

WX_DECLARE_USER_EXPORTED_OBJARRAY(wxReportTextParagraph*, ParagraphsArray, WXDLLIMPEXP_RP);

/////////////////////////////////////////////////////////////////////////////////////////////
// wxReportTextItem Classes Declaration
/////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Represents the block of the text. It contains the text values arranged to the paragraph's objects.
 */
class WXDLLIMPEXP_RP wxReportTextItem : public wxReportPositionedItem
{
protected:
	double m_dWidth;
	double m_dHeight;
	bool m_fAutosize;
	wxReportStyle m_style;
	ParagraphsArray m_arParagraphs;
	wxReportTextParagraph m_LastParagraph;
	wxReportParagraphStyle m_activeParagraphStyle;
	wxReportTextStyle m_activeTextStyle;
	void DoCopy(const wxReportTextItem& sourceItem);
	template<class T> void DoAddVariable(const T& value, const wxString& name);
		
public:
	/**
	 * \brief Default constructor.
	 */
	wxReportTextItem();
	/*!
	 * \brief Constructor.
	 * \param name item's name
	 * \param x horizontal position of the text item
	 * \param y vertical position of the text item
	 */
	wxReportTextItem(const wxString& name, double x, double y);
	/**
	 * \brief Retrieve new text item form the specified XML node.
	 * \param node XML node to retrieve
	 */
	wxReportTextItem(const wxXmlNode *node);
	/**
	 * \brief Copy constructor.
	 * \param sourceItem source text item object
	 */
	wxReportTextItem(const wxReportTextItem& sourceItem);
	/**
	 * \brief Default destructor.
	 */
	~wxReportTextItem();
	/**
	 * \brief Assigment operator.
	 * \param sourceItem source text item to assign
	 */
	wxReportTextItem& operator= (const wxReportTextItem &sourceItem);
	/**
	 * \brief Sets the size of the text item in the default meausure units.
	 * \param width width of the item 
	 * \param height height of the item
	 */
	void SetSize(double width, double height);
	/**
	 * \brief Returns the size of the item.
	 * \return wxRealPoint object encapsulating the item's size
	 */
	wxRealPoint GetSize();
	/**
	 * \brief Add non-data text value.
	 * \param text text string to add
	 */
	void AddText(const wxString& text);
	/**
	 * \brief Adds text value object.
	 * \param textValue text value object to insert
	 */
	void AddText(const wxReportTextValue& textValue);
	/**
	 * \brief Add empty variable.
	 * \param name Identification name of the variable.
	 */
	void AddVariable(const wxString& name);
	/**
	 * \brief Adds new text value object filled from the specified variable or object.
	 * \param value constant reference to source object or variable
	 * \param name identificaton name of the text value object
	 */
	void AddVariable(const short& value, const wxString& name);
	/**
	 * \brief Adds new text value object filled from the specified variable or object.
	 * \param value constant reference to source object or variable
	 * \param name identificaton name of the text value object
	 */
	void AddVariable(const int& value, const wxString& name);
	/**
	 * \brief Adds new text value object filled from the specified variable or object.
	 * \param value constant reference to source object or variable
	 * \param name identificaton name of the text value object
	 */
	void AddVariable(const long& value, const wxString& name);
	/**
	 * \brief Adds new text value object filled from the specified variable or object.
	 * \param value constant reference to source object or variable
	 * \param name identificaton name of the text value object
	 */
	void AddVariable(const float& value, const wxString& name);
	/**
	 * \brief Adds new text value object filled from the specified variable or object.
	 * \param value constant reference to source object or variable
	 * \param name identificaton name of the text value object
	 */
	void AddVariable(const double& value, const wxString& name);
	/**
	 * \brief Adds new text value object filled from the specified variable or object.
	 * \param value constant reference to source object or variable
	 * \param name identificaton name of the text value object
	 */
	void AddVariable(const char& value, const wxString& name);
	/**
	 * \brief Adds new text value object filled from the specified variable or object.
	 * \param value constant reference to source object or variable
	 * \param name identificaton name of the text value object
	 */
	void AddVariable(const wxString& value, const wxString& name);
	/*!
	 * \brief Set item's style
	 * \param style style
	 */
	void SetStyle(const wxReportStyle& style);
	/*!
	 * \brief Get used style
	 * \return used style
	 */
	wxReportStyle GetStyle();
	/**
	 * \brief Set the style of the active text paragraph.
	 * \param style paragraph style to set
	 */
	void SetActiveParagraphStyle(const wxReportParagraphStyle& style);
	/**
	 * \brief Set active style of the text.
	 * \param style text style to set
	 */
	void SetActiveTextStyle(const wxReportTextStyle& style);
	/**
	 * \brief Sets the actual font of the text.
	 * \param font font to set
	 */
	void SetFont(const wxFont& font);
	/**
	 * \brief Sets the active font size.
	 * \param fontSize size of the font
	 */
	void SetFontSize(int fontSize);
	/**
	 * \brief Sets active font style
	 * \param fontStyle font style name
	 */
	void SetFontStyle(wxFontStyle fontStyle);
	/**
	 * \brief Activates text underlining.
	 * \param underlined TRUE to enable, FALSE to disable
	 */
	void SetFontUnderlined(bool underlined);
	/**
	 * \brief Sets the font color
	 * \param fontColor color to set
	 */
	void SetFontColor(const wxColour& fontColor);
	/**
	 * \brief Sets align of the text.
	 * \param alignType Possible values:
	 * \li wxRP_LEFT: left align
	 * \li wxRP_RIGHT: right align
	 * \li wxRP_CENTER: center
	 * \li wxRP_JUSTIFY: justify do whole width
	 */
	void SetTextAlign(int alignType);
	/**
	 * \brief Inserts new line.
	 */
	void AddNewLine();
	/**
	 * \brief Insert new paragraph.
	 */
	void AddNewParagraph();
	/**
	 * \brief Clear all text form the last paragraph.
	 */
	void ClearLastParagraph();
	/**
	 * \brief Clear all text from the paragraph specified by its index.
	 * \param index index of the paragraph to clear
	 */
	void ClearParagraph(int index);
	/**
	 * \brief Clear all text from the text item.
	 */
	void ClearAllText();
	/**
	 * \brief 
	 */
	void Clear();
	/**
	 * \brief Remove last paragraph.
	 */
	void RemoveLastParagraph();
	/**
	 * \brief Remove paragraph specified by its position.
	 */
	void RemoveParagraph(int index);
	/**
	 * \brief Remove last added text value.
	 */
	void RemoveLast();
	/**
	 * \brief Remove text at specified index
	 * \param index of removed text
	 */
	void RemoveText(int index);
	/**
	 * \brief Returns the pointer to the text value item at specified index.
	 * \param paragraphIndex index of the paragraph
	 * \param index of the text value in specified paragraph
	 * \return constant pointer to the text value item
	 */
	const wxReportTextValue* GetTextValue(int textIndex, int paragraphIndex = -1) const;
	/**
	 * \brief Get the number of paragraphs.
	 * \return number of the paragraph inside the text item
	 */
	int GetParagraphsCount();
	/**
	 * \brief Get the pointer to the specified paragraph.
	 * \param index index of the paragraph
	 * \return constant pointer to the paragraph
	 */
	const wxReportTextParagraph* GetParagraph(int index) const;
	//double CalculateWidth(wxDC *dc, int pxLeftBorder, int pxRightBorder, int pxPageWidth);
	double CalculateTopLeftPosition(wxDC *dc, bool toScreen, double itemWidth, double &itemHeight, int pxTopBorder, int pxBottomBorder, int pxPageHeight, int pxPageWidth);
	/**
	 * \brief Create XML node of the text item.
	 * \return XML node with item's parameters
	 */
	wxXmlNode* CreateXmlNode();
	/**
	 * \brief Retrieve item's content from the specified XML node.
	 * \param node XML node to retrieve
	 * \return TRUE on succes, FALSE if false
	 */
	bool RetrieveFromXmlNode(const wxXmlNode* node);
	/**
	 * \brief  text item to the specified device context.
	 * \param dc Device context to draw.
	 * \param toScreen tells if the page will be drawn to the screen or to the printer
	 */
	void DrawToDC(wxDC *dc, bool toScreen, const wxReportPageStyle& pageStyle);
	
	FRIENDS
	friend class wxReportDocument;
};

WX_DECLARE_OBJARRAY(wxSize, wxArraySizes);
WX_DECLARE_OBJARRAY(wxRect, wxArrayRects);

#endif // TEXTELEMENT_H
