#ifndef STYLES_H
#define STYLES_H

#include "wx/report/defs.h"
#include "wx/wx.h"
#include "wx/xml/xml.h"

/**
 * \brief Base class for the item's styles classes.
 */
class WXDLLIMPEXP_RP wxReportStyle
{
protected:
	wxString m_sName;
	int m_iBorder;
	double m_dBorderThickness;
	wxColour m_BorderColor;
	wxColour m_BgColor;
		
public:
	/**
	 * \brief Default constructor. 
	 */	
	wxReportStyle();
	/**
	 * \brief Constructor.
	 * \param name Identification name.
	 */
	wxReportStyle(const wxString& name);
	/**
	 * \brief Virtual destructor.
	 */
	virtual ~wxReportStyle() {;}
	/**
	 * \brief Sets the identification name.
	 * \param name Name of the style.
	 */	
	void SetName(const wxString& name);
	/**
	 * \brief Sets a border around the item.
	 * \param borderValue Sets the sides of border lines. Could be set by summing of border sides identificators. Possible values:
	 * \li wxRP_NOBORDER: no border line
	 * \li wxRP_LEFTBORDER: left border line
	 * \li wxRP_RIGHTBORDER: right border line
	 * \li wxRP_TOPBORDER: top border line
	 * \li wxRP_BOTTOMBORDER: bottom border line
	 * \li wxRP_ALLBORDER: bottom border line
	 * \param color color of the border lines
	 * \param thickness thickness of the border lines
	 */
	void SetBorder(int borderValue, wxColour color = *wxBLACK, double thickness = 0.5);
	/**
	 * \brief Sets the color of the border.
	 * \param color border color to ser
	 */
	void SetBorderColor(const wxColour& color);
	/**
	 * \brief Sets the color of the background.
	 * \param color background color to set
	 */
	void SetBackgroundColor(const wxColour& color);
	/**
	 * \brief Sets the width of the border line.
	 * \param thickness width of the line
	 */
	void SetBorderThickness(double thickness);
	/**
	 * \brief Return the identification name of the style.
	 * \return string with the name
	 */
	const wxString& GetName() const;
	/**
	 * \brief Returns a border value.
	 * \return sume of the border sides identificators
	 */
	const int& GetBorder() const;
	/**
	 * \brief Returns the color of the border line.
	 * \return color of the line
	 */
	const wxColour& GetBorderColor() const;
	/**
	 * \brief Returns the color of the background.
	 * \return color of the background
	 */
	const wxColour& GetBackgroundColor() const;
	/**
	 * \brief Returns the width of the border line.
	 * \return width of the line
	 */
	const double& GetBorderThickness() const;
};

/**
 * \brief Represents the style of the page.
 */
class WXDLLIMPEXP_RP wxReportPageStyle : public wxReportStyle
{
protected:
	int m_dSizeX;
	int m_dSizeY;
	double m_dLeftMargin;
	double m_dRightMargin;
	double m_dTopMargin;
	double m_dBottomMargin;
	
public:	
	/**
	 * \brief Default constructor.
	 */
	wxReportPageStyle();
	/**
	 * \brief Constructor.
	 * \param name Identification name of the style.
	 */
	wxReportPageStyle(const wxString& name);
	/**
	 * \brief Default destructor.
	 */
	~wxReportPageStyle() {;}
	/**
	 * \brief Sets the margins of the page.
	 * \param left left margin
	 * \param right right margin
	 * \param top top margin
	 * \param bottom bottom margin
	 */
	void SetMargins(double left, double right, double top, double bottom);
	/**
	 * \brief Seth the width if the page.
	 * \param width page width
	 */
	void SetWidth(double width);
	/**
	 * \brief Sets the height of the page.
	 * \param height page height
	 */
	void SetHeight(double height);

	/*!
	 * \brief Get page size.
	 * \return  page size
	 */
	wxRealPoint GetSize() const;
	/**
	 * \brief Gets the left margin of the page.
	 * \return left margin
	 */
	double GetLeftMargin() const;
	/**
	 * \brief Gest the right margin of the page.
	 * \return righ margin
	 */
	double GetRightMargin() const;
	/**
	 * \brief Gest the top margin of the page.
	 * \return top margin
	 */
	double GetTopMargin() const;
	/**
	 * \brief Gets the bootm margin of the page.
	 * \return bottom margin
	 */
	double GetBottomMargin() const;
	/**
	 * \brief Create the XML node of the page style.
	 * \return xml node
	 */
	wxXmlNode* CreateXmlNode();
	/**
	 * \brief Retrieve the page style of the specified XML node.
	 * \param node
	 * \return TRUE on success, otherwise FALSE
	 */
	bool RetrieveFromXmlNode(const wxXmlNode *node);
};

/**
 * \brief Represents the style of the text paragraph.
 */
class WXDLLIMPEXP_RP wxReportParagraphStyle : public wxReportStyle
{
protected:
	int m_iTextAlign;
	double m_dLeftIndent;
	double m_dRightIndent;
	double m_dLineHeight;
	bool m_fLineHeightFromFont;
	double m_dParagraphsSpace;
		
public:
	/**
	 * \brief Default constructor.
	 */
	wxReportParagraphStyle();
	/**
	 * \brief Constructor.
	 * \param name identification name of the style
	 */
	wxReportParagraphStyle(const wxString& name);
	/**
	 * \brief Default destructor.
	 */
	~wxReportParagraphStyle() {;}
	/**
	 * \brief Set align of the text in the paragraph.
	 * \param textAlign Possible values:
	 * \li wxRP_LEFTALIGN: left align
	 * \li wxRP_RIGHTALIGN: right align
	 * \li wxRP_CENTERALIGN: center align
	 * \li wxRP_JUSTIFY: justify
	 */
	void SetTextAlign(int textAlign);
	/**
	 * \brief Set the left and right indent of the paragraph.
	 * \param leftIndent left indent
	 * \param rightIndent right indent
	 */
	void SetIndent(double leftIndent, double rightIndent);
	/**
	 * \brief Sets the height of the line. Defaultly is calculate from the size of the used font.
	 * \param lineHeight height of the line
	 */
	void SetLineHeight(double lineHeight);
	/**
	 * \brief Sets the space between paragraphs.
	 * \param space space between paragraphs
	 */
	void SetParagraphsSpace(double space);
	/**
	 * \brief Return the text align value.
	 * \return text align value
	 */
	int GetTextAlign();
	/**
	 * \brief Get the left indent of the text in the paragraph.
	 * \return left indent value
	 */
	double GetLeftIndent();
	/**
	 * \brief Get the right indent of the text in the paragraph.
	 * \return right indent value
	 */
	double GetRightIndent();
	/**
	 * \brief Returns the height of the line.
	 * \return height of the line
	 */
	double GetLineHeight();
	/**
	 * \brief Returns the space between paragraphs.
	 * \return space between two paragraphs
	 */
	double GetParagraphsSpace();
	/**
	 * \brief Creates the XML node of the paragraph style.
	 * \return XML with the paragraph
	 */
	wxXmlNode* CreateXmlNode();
	/**
	 * \brief Retrieves paragraph style from the XML node.
	 * \param node Pointer to the XML node from which the styl will be retrieved.
	 * \return Returns TRUE on succes, otherwise FALSE.
	 */
	bool RetrieveFromXmlNode(const wxXmlNode *node);
	
	friend class wxReportTextParagraph;
};

/**
 * \brief Represents the style of the text, i.e. font and color of the font.
 */
class WXDLLIMPEXP_RP wxReportTextStyle : public wxReportStyle
{
protected:
	wxFont m_font;
	wxColor m_FontColor;
	
public:
	/**
	 * \brief Default constructor.
	 */
	wxReportTextStyle();
	/**
	 * \brief Constructor.
	 * \param name Identification name of the style.
	 * \param font fot of the text
	 * \param textColor color of the text
	 * \param backgroundColor background colr
	 * \param borderType border around the text
	 * \param borderColor color of the border line
	 */
	wxReportTextStyle(const wxString& name, const wxFont& font, const wxColour& textColor = *wxBLACK, const wxColour& backgroundColor = wxNullColour, int borderType = wxRP_NOBORDER, const wxColour& borderColor = *wxBLACK);
	/**
	 * \brief Default destructor.
	 */
	~wxReportTextStyle() {;}
	bool operator== (const wxReportTextStyle& textStyle) const;
	/**
	 * \brief Sets the font of the text.
	 * \param font font to set
	 */
	void SetFont(const wxFont& font);
	/**
	 * \brief Sets the color of the text.
	 * \param color color to set
	 */
	void SetTextColor(const wxColor& color);
	/**
	 * \brief Returns the color of the text.
	 * \return text color
	 */
	const wxFont& GetFont() const;
	/**
	 * \brief Returns the size of the font.
	 * \return font size in points
	 */
	int GetFontSize() const;
	/**
	 * \brief Returns the color of the text.
	 * \return text color
	 */
	const wxColor& GetTextColor() const;
	/**
	 * \brief Creates the XML for the style.
	 * \return XML node with the style
	 */
	wxXmlNode* CreateXmlNode();
	/**
	 * \brief Retrieves the style from given XML node.
	 * \param node XML node to retrieve
	 * \return TRUE on success, FALSE if fails
	 */
	bool RetrieveFromXmlNode(const wxXmlNode *node);
	
	friend class wxReportTextItem;
	friend class wxReportTextParagraph;
	friend class wxReportTextValue;
};

/**
 * \brief Represents the style of the image
 */
class WXDLLIMPEXP_RP wxReportImageStyle : public wxReportStyle
{
protected:
	double m_dLeftMargin;
	double m_dRightMargin;
	double m_dTopMargin;
	double m_dBottomMargin;
	
public:
	/**
	 * \brief Default constructor.
	 */
	wxReportImageStyle();
	/**
	 * \brief Constructor.
	 * \param name Identification name of the style.
	 */
	wxReportImageStyle(const wxString& name);
	/**
	 * \brief Default destructor. 
	 */
	~wxReportImageStyle() {;}
	/**
	 * \brief Set margins of the page.
	 * \param left left margin
	 * \param right right margin
	 * \param top top margin
	 * \param bottom bottom margin
	 */
	void SetMargins(double left, double right, double top, double bottom);
	/**
	 * \brief Returns the left margin.
	 * \return left margin value
	 */
	double GetLeftMargin();
	/**
	 * \brief Returns the right margin.
	 * \return right margin value
	 */
	double GetRightMargin();
	/**
	 * \brief Returns the top margin.
	 * \return top margin value
	 */
	double GetTopMargin();
	/**
	 * \brief Returns the bottom margin.
	 * \return bottom margin value
	 */
	double GetBottomMargin();
	/**
	 * \brief Creates the XML for the style.
	 * \return XML node with the style
	 */
	wxXmlNode* CreateXmlNode();
	/**
	 * \brief Retrieves the style from the given XML node.
	 * \param node XML node to retrieve
	 * \return TRUE on success, FALSE if fails
	 */
	bool RetrieveFromXmlNode(const wxXmlNode *node);
	/**
	 * \brief Sets the background color - does nothing.
	 */
	void SetBackgroundColor() {;}
};

/**
 * \brief Represents the style of the table.
 */
class WXDLLIMPEXP_RP wxReportTableStyle : public wxReportStyle
{
protected:
	double m_dLeftMargin;
	double m_dRightMargin;
	double m_dTopMargin;
	double m_dBottomMargin;
	
public:
	/**
	 * \brief Default constructor.
	 */
	wxReportTableStyle();
	/**
	 * \brief Constructor.
	 * \param name Identification name of the style.
	 */
	wxReportTableStyle(const wxString& name);
	/**
	 * \brief Default destructor. 
	 */
	~wxReportTableStyle() {;}
	/**
	 * \brief Creates the XML for the style.
	 * \return XML node with the style
	 */
	wxXmlNode* CreateXmlNode();
	/**
	 * \brief Retrieves the style from the given XML node.
	 * \param node XML node to retrieve
	 * \return TRUE on success, FALSE if fails
	 */
	bool RetrieveFromXmlNode(const wxXmlNode *node);
};

#endif // STYLES_H
