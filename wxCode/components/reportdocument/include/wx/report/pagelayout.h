#ifndef PAGELAYOUT_H
#define PAGELAYOUT_H

#include "wx/report/tableelements.h"
#include "wx/report/shapeitems.h"
#include "wx/hashmap.h"

/**
 * \brief Represents the layout of the single page. The class encapsulates the items on the page.
 */
class WXDLLIMPEXP_RP wxReportPage
{
protected:
	wxReportPageStyle m_style;
	ItemsArray m_arItems;
	ItemsArray m_arHeader;
	ItemsArray m_arFooter;
	ItemsArray m_arPageNumberItems;
	ItemsMap m_mapVariables;
	bool m_fPageNumberActive;
	bool m_fDelete;
	wxReportTextItem m_pageNumber;
	int m_iPageNumber;
	int FindIndex(const wxString& name) const;
	void DoCopy(const wxReportPage& sourcePage);
	void AddRemoveVariables(wxReportTextItem* pItem, bool add = true);
	void AddRemoveVariables(wxReportTableItem* pItem, bool add = true);
	
public:
	/**
	 * \brief Default constructor.
	 */
	wxReportPage();
	/**
	 * \brief Copy constructor.
	 * \param sourcePage source page
	 */
	wxReportPage(const wxReportPage& sourcePage);
	/**
	 * \brief Default destructor.
	 */
	~wxReportPage();
	/**
	 * \brief Assigment operator.
	 * \param sourcePage page to assign
	 */
	wxReportPage& operator= (const wxReportPage& sourcePage);
	/**
	 * \brief Sets the style of the page.
	 * \param style page style to set
	 */
	void SetStyle(const wxReportPageStyle& style);
	/**
	 * \brief Add empty item with its identification name.
	 * \param name name of the new item
	 */
	void AddItem(const wxString& name);
	/**
	 * \brief Add text item to the page.
	 * \param item text item to insert
	 */
	void AddItem(const wxReportTextItem& item);
	/**
	 * \brief Add image item to the page.
	 * \param image text item to insert
	 */
	void AddItem(const wxReportImageItem& item);
	/**
	 * \brief Add table item, to the page.
	 * \param item table item to insert
	 */
	void AddItem(const wxReportTableItem& item);
	/**
	 * \brief Add line shape to the page.
	 * \param lineShape hsape to insert
	 */
	void AddItem(const wxReportLineShape& lineShape);
	/**
	 * \brief Add rectangle shape to the page.
	 * \param rectShape shape to insert
	 */
	void AddItem(const wxReportRectangleShape& rectShape);
	/**
	 * \brief Add circle shape to the page.
	 * \param circleShape shape to insert
	 */
	void AddItem(const wxReportCircleShape& circleShape);
	/**
	 * \brief Add item passed as the pointer to its base class.
	 */
	void AddItem(const wxReportPageItem *item, int place);
	/**
	 * \brief Add item to the specified position at the array of page's items.
	 */
	//void AddItemAt(wxReportPageItem *item, int index);
	/**
	 * \brief Add text item to the specified position.
	 * \param item text item to insert
	 * \param index index where the will be inserted
	 */
	void AddItemAt(const wxReportTextItem& item, int index);
	/**
	 * \brief Add image item to the specified position.
	 * \param item text item to insert
	 * \param index index where the will be inserted
	 */
	void AddItemAt(const wxReportImageItem& item, int index);
	/**
	 * \brief Add table item to the specified position.
	 * \param item text item to insert
	 * \param index index where the will be inserted
	 */
	void AddItemAt(const wxReportTableItem& item, int index);
	/**
	 * \brief Delete specified item from the page. Do nothing where the item not found.
	 * \param name of the item to delete
	 */
	void DeleteItem(const wxString& name);
	/**
	 * \brief Delete item on the specified position.
	 * \param index of the item to delete
	 */
	void DeleteItemAt(int index);
	/**
	 * \brief Get number of items on the page.
	 * \return number of items on the page
	 */
	int GetCount() const;
	/**
	 * \brief Get the pointer to the item specified by its identification name.
	 * \param name of the item
	 */
	const wxReportPageItem* GetItem(const wxString& name) const;
	/**
	 * \brief Returns the text string containing the page number of the current page.
	 * \return text string with the page number
	 */
	wxString GetPageNumberString() const;
	/**
	 * \brief Get the pointer to the item specified by its position at the items array.
	 * \param index index of the item
	 * \return constant pointer to the specified item
	 */
	const wxReportPageItem* GetItem(int index) const;
	/**
	 * \brief Get the reference to current style of the page.
	 * \return reference to the page style
	 */
	wxReportPageStyle& GetStyle();
	/**
	 * \brief Refresh values of all items on the page which are connected to source data objects.
	 */
	void RefreshVariables();
	/**
	 * \brief Actualizes the page number.
	 */
	void RefreshPageNumbers();
	/**
	 * \brief Creates the XML node containing the layout of the current page.
	 * \return XML node with layout
	 */
	wxXmlNode* CreateXmlNode();
	/**
	 * \brief Retrive page layout from the specified node.
	 * \param XML node with the page layout
	 */
	bool RetrieveFromXmlNode(const wxXmlNode* node);
	/**
	 * \brief Draw content of the page to the specified device content.
	 * \param dc device context
	 * \param toScreen tells if the page will be drawn to the screen or to the printer
	 */
	void DrawToDC(wxDC* dc, bool toScreen);
	
	friend class wxReportDocument;
};

WX_DECLARE_USER_EXPORTED_OBJARRAY(wxReportPage*, PagesArray, WXDLLIMPEXP_RP);

#endif // PAGELAYOUT_H
