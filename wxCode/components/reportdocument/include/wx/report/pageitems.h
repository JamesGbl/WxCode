#ifndef PAGEITEMS_H
#define PAGEITEMS_H

#include "wx/report/styles.h"
#include "wx/pen.h"
#include "wx/dynarray.h"

/**
 * \brief Converts the size in milimeters to size in pixels considering to the device context's PPI.
 * \param mm size in milimeters
 * \param dc currently used device context
 * \param toScreen tells if the current drawing task is to the screen or to the printer
 * \return the size in pixels
 */
inline int MM2PX(double mm, wxDC *dc, bool toScreen)
{
	double sx, sy;
	dc->GetUserScale(&sx, &sy);
	
	if(toScreen)
		sx = sy = 1.0;//1.001;
		
	return wxRound((((double)(dc->GetPPI().x) / 25.4) * mm) / sx);
}

inline void DrawBorder(wxDC *dc, int border, int x, int y, int w, int h)
{
	int x1, y1, x2, y2;
	
	if(border <= 0b1111 && border > 0)
	{
		if(border == 0b1111)
		{			
			dc->DrawRectangle(x, y, w, h);
		}
		else
		{
			for(int i=0b0001; i<0b1111; ++i)
			{
				int borderType = border & i;
				
				switch(borderType)
				{
					case wxRP_LEFTBORDER:
						x1 = x;
						y1 = y;
						x2 = x;
						y2 = y + h;
						break;
						
					case wxRP_RIGHTBORDER:
						x1 = x + w;
						y1 = y;
						x2 = x1;
						y2 = y + h;
						break;
						
					case wxRP_TOPBORDER:
						x1 = x;
						y1 = y;
						x2 = x + w;
						y2 = y;
						break;
						
					case wxRP_BOTTOMBORDER:
						x1 = x;
						y1 = y + h;
						x2 = x + w;
						y2 = y1;
				}
				
				if(borderType > 0)
					dc->DrawLine(x1, y1, x2, y2);
			}
		}
	}	
}

wxPoint CalcNegPos(const wxRealPoint& origPos, const wxPoint& pxPos, int w, int h, wxDC *dc);

/////////////////////////////////////////////////////////////////////////////////////////////
// wxReportPageItem Classes Declaration
/////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Representing the items on the page - base class.  
 */
class WXDLLIMPEXP_RP wxReportPageItem
{
protected:
	wxString m_sName;
	int m_iType;
	int m_iValueType;
	wxString m_sValue;
	bool m_fVariable;
	const void* m_pVariable;
	
public:
	/**
	 * \brief Default constructor.
	 */
	wxReportPageItem();
	/**
	 * \brief Constructor.
	 * \param name name of the item
	 * \param isVariable tells if the item will be connected with data source object
	 */
	wxReportPageItem(const wxString& name, bool isVariable = false);
	/**
	 * \brief Virtual destructor.
	 */
	virtual ~wxReportPageItem() {;}
	/**
	 * \brief Set name of the item.
	 * \param name name of the item
	 */
	void SetName(const wxString& name);
	/**
	 * \brief Virtual function. Do nothing for this class.
	 */
	virtual void SetVariable() {;}
	/**
	 * \brief Sets if the item will be filled from data.
	 * \param isVariable TRUE to enable, FALSE to disable
	 */
	void SetIfVariable(bool isVariable);
	/**
	 * \brief Get the name of the item.
	 * \return text string containing the name of the item
	 */
	const wxString& GetName();
	/**
	 * \brief Get the identification number associated with type of the item.
	 */
	int GetType();
	/**
	 * \brief Tells if the item is filled from data object or variable.
	 * \return TRUE if it is filled, FALSE if not
	 */
	bool IsVariable();
	/**
	 * \brief Refresh item's value if it is filled from data object, virtual function - do nothing for this class.
	 */
	virtual void RefreshVariable() {;}
	/**
	 * \brief Remove content of the item - virtual function - do nothing for this class.
	 */
	virtual void Clear() {;}
	/**
	 * \brief Create XML node of the item - virtual function - ever returns NULL for this class.
	 * \return ever returns NULL
	 */
	virtual wxXmlNode* CreateXmlNode() {return NULL;}
	/**
	 * \brief Retrieve item from the specified node - virtual function - ever returns FALSE for this class.
	 */
	virtual bool RetrieveFromXmlNode(const wxXmlNode* node) {return false;}
	/**
	 * \brief Draw item to the specified device context - virtual function - ever do nothing for this class.
	 * \param dc device context to draw
	 * \param toScreen tells if the drawing will be on the screnn or on the printer.
	 */
	virtual void DrawToDC(wxDC* dc, bool toScreen, const wxReportPageStyle& pageStyle) {;}
	
	FRIENDS
	friend class wxReportDocument;
};

WX_DECLARE_USER_EXPORTED_OBJARRAY(wxReportPageItem*, ItemsArray, WXDLLIMPEXP_RP);

/////////////////////////////////////////////////////////////////////////////////////////////
// wxReportPositionedItem Classes Declaration
/////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Representing the items with specified position on the page.
 */
class WXDLLIMPEXP_RP wxReportPositionedItem : public wxReportPageItem
{
protected:
	wxRealPoint m_position;
	
public:
	wxReportPositionedItem();
	wxReportPositionedItem(const wxString& name, double x, double y, bool isVariable);
	virtual ~wxReportPositionedItem() {;}
	/**
	 * \brief Sets the position of the item.
	 * \param posX
	 * \param posY
	 */
	void SetPosition(double posX, double posY);
	void SetPosition(const wxRealPoint& position);
	const wxRealPoint& GetPosition();
};

/////////////////////////////////////////////////////////////////////////////////////////////
// wxReportImageItem Classes Declaration
/////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Representing the image items.
 */
class WXDLLIMPEXP_RP wxReportImageItem : public wxReportPositionedItem
{
protected:
	wxReportImageStyle m_style;
	int m_iWidth;
	int m_iHeight;
	int m_iPPI;
		
public:
	/**
	 * \brief Default constructor.
	 */
	wxReportImageItem();
	/**
	 * \brief Constructor.
	 * \param name name of the image item
	 * \param path path to the image file
	 * \param x x-position of the image
	 * \param y y-position of the image
	 * \param PPI pixels per inch value of the image
	 * \param isVariable tells if the path of the image file will be loaded from the wxString source object
	 */
	wxReportImageItem(const wxString& name, const wxString& path, double x, double y, int PPI = 72, bool isVariable = false);
	/**
	 * \brief Default destructor.
	 */
	~wxReportImageItem() {;}
	/**
	 * \brief Sets the path to the image file.
	 * \param path path to the image file
	 */
	void SetPath(const wxString& path);
	/**
	 * \brief Sets the PPI value of the image
	 * \param ppi pixels per inch of the image
	 */
	void SetPPI(int ppi);
	/**
	 * \brief Sest the style of the image.
	 * \param style image style to set
	 */
	void SetStyle(const wxReportImageStyle& style);
	/**
	 * \brief Returns the path to the image file.
	 * \return path to the image file
	 */
	const wxString& GetPath();
	/**
	 * \brief Returns the PPI value of the image.
	 * \return pixels per inch value of the image
	 */
	int GetPPI();
	/**
	 * \brief Returns reference to the style associated with the image.
	 * \return reference to the style object of the item
	 */
	wxReportImageStyle& GetStyle();
	/**
	 * \brief Returns the width of the image in pixels.
	 * \return width of the image
	 */
	int GetWidth();
	/**
	 * \brief Returns the height of the image in pixels.
	 * \return height of the image
	 */
	int GetHeight();
	/**
	 * \brief Refresh the path to the image file, if it's filled from the data object.
	 */
	void RefreshVariable();
	/**
	 * \brief Clears the image - path to image file sets to empty string, the height and the width sets both to 0 and the pointer to the source data object sets to NULL
	 */
	void Clear();
	/**
	 * \brief Creates the XML node with image parameters.
	 * \return XML node for the current image
	 */
	wxXmlNode* CreateXmlNode();
	/**
	 * \brief Retrieves image item from the specified XML node
	 * \return TRUE on success, FALSE if fails
	 */
	bool RetrieveFromXmlNode(const wxXmlNode* node);
	/**
	 * \brief Draw image to the specified device context.
	 * \param dc device context for drawing
	 * \param toScreen tells if the image will be drawn to the screen or to the printer 
	 */
	void DrawToDC(wxDC *dc, bool toScreen, const wxReportPageStyle& pageStyle);
	
	FRIENDS
};

WX_DECLARE_EXPORTED_STRING_HASH_MAP(wxReportPageItem*, ItemsMap);

#endif // PAGEITEMS_H