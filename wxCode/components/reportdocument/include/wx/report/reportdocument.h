#ifndef WXREPORTDOCUMENT_H
#define WXREPORTDOCUMENT_H

#include "wx/report/pagelayout.h"
#include "wx/report/filehandlers.h"
#include "wx/report/printout.h"
#include "wx/print.h"
#include <wx/stream.h>

//////////// wxReportDocument class

/**
 * \brief Main class of the library. Encapsulates the all pages in the report document.
 */
class WXDLLIMPEXP_RP wxReportDocument
{	
protected:
	PagesArray m_arrPages;
	HandlersArray m_arrHandlers;
	ItemsArray m_arHeader;
	ItemsArray m_arFooter;
	bool m_fAutoRefresh;
	wxReportPageStyle m_pageStyle;
	wxReportPage *m_pActivePage;
	int m_iActivePageIndex;
	int m_iUnitType;
	int m_iDPI;
	wxReportTextItem m_pageNumberFormat;
	wxPrintData m_printData;
	wxString m_sVersion;
	
	template<class T> bool DoSetVariable(const wxString& name, int pageIndex, const T& var, int varType);
	template<class T> bool DoSetVariableForTable(const wxString& name, bool toRow, int index, const T& array);
	void DoAddItemToHeader(wxReportPageItem *pItem);
	void DoAddItemToFooter(wxReportPageItem *pItem);
	
	void DoCopy(const wxReportDocument& report);
	void DoDestroy();
	void DoChangeItem(int from, int to, wxReportPageItem *item = NULL, const wxString& itemName = wxEmptyString);
	//void DoRemovePageNumbering(int from, int to);
	void DoDivideTable(const wxReportTableItem& tableItem, int pageIndex, int itemIndex);
	
	inline wxReportPageItem* CreateItemCopy(const wxReportPageItem *item)
	{
		wxReportPageItem *newItem = NULL;
		switch(item->m_iType)
		{
			case wxRP_TEXT:
				newItem = new wxReportTextItem(*((wxReportTextItem*)item));
				break;
				
			case wxRP_TABLE:
				newItem = new wxReportTableItem(*((wxReportTableItem*)item));
				break;
				
			case wxRP_IMAGE:
				newItem = new wxReportImageItem(*((wxReportImageItem*)item));
				break;
					
			case wxRP_LINESHAPE:
				newItem = new wxReportLineShape(*((wxReportLineShape*)item));
				break;
					
			case wxRP_RECTANGLESHAPE:
				newItem = new wxReportRectangleShape(*((wxReportRectangleShape*)item));
				break;
					
			case wxRP_CIRCLESHAPE:
				newItem = new wxReportCircleShape(*((wxReportCircleShape*)item));
				break;
		}
		return newItem;
	}

public:
	/**
	 * \brief Default constructor.
	 */	
	wxReportDocument();
	/**
	 * \brief Copy constructor.
	 * \param report 
	 */
	wxReportDocument(const wxReportDocument& report);
	/**
	 * \brief Default destructor.
	 */	
	~wxReportDocument();
	/**
	 * \brief Assigment operator.
	 * \param report wxReportDocument object to assign.
	 */
	wxReportDocument& operator= (const wxReportDocument& report);
	/**
	 * \brief Sets a style for the current page.
	 * \param pageStyle page style to set
	 */
	void SetPageStyle(const wxReportPageStyle& pageStyle);
	/**
	 * \brief Sets the active page.
	 * \param pageIndex index of the page in pages array
	 */
	void SetActivePage(int pageIndex);
	/**
	 * \brief Set data object for specified page item.
	 * \param var source data object to connect with item
	 * \param name identification name of the item
	 * \param pageIndex index of the page where the item is situated
	 * \return TRUE on success, otherwise FALSE
	 */
	bool SetVariable(const short& var, const wxString& name, int pageIndex = -1);
	/**
	 * \brief Set data object for specified page item.
	 * \param var source data object to connect with item
	 * \param name identification name of the item
	 * \param pageIndex index of the page where the item is situated
	 * \return TRUE on success, otherwise FALSE
	 */
	bool SetVariable(const int& var, const wxString& name, int pageIndex = -1);
	/**
	 * \brief Set data object for specified page item.
	 * \param var source data object to connect with item
	 * \param name identification name of the item
	 * \param pageIndex index of the page where the item is situated
	 * \return TRUE on success, otherwise FALSE
	 */
	bool SetVariable(const long& var, const wxString& name, int pageIndex = -1);
	/**
	 * \brief Set data object for specified page item.
	 * \param name identification name of the item
	 * \param var source data object to connect with item
	 * \param pageIndex index of the page where the item is situated
	 * \return TRUE on success, otherwise FALSE
	 */
	bool SetVariable(const float& var, const wxString& name, int pageIndex = -1);
	/**
	 * \brief Set data object for specified page item.
	 * \param var source data object to connect with item
	 * \param name identification name of the item
	 * \param pageIndex index of the page where the item is situated
	 * \return TRUE on success, otherwise FALSE
	 */
	bool SetVariable(const double& var, const wxString& name, int pageIndex = -1);
	/**
	 * \brief Set data object for specified page item.
	 * \param var source data object to connect with item
	 * \param name identification name of the item
	 * \param pageIndex index of the page where the item is situated
	 * \return TRUE on success, otherwise FALSE
	 */
	bool SetVariable(const char& var, const wxString& name, int pageIndex = -1);
	/**
	 * \brief Set data object for specified page item.
	 * \param var source data object to connect with item
	 * \param name identification name of the item
	 * \param pageIndex index of the page where the item is situated
	 * \return TRUE on success, otherwise FALSE
	 */
	bool SetVariable(const wxString& var, const wxString& name, int pageIndex = -1);
	/**
	 * \brief Set source data array for column in specified table item.
	 * \param name identification name of the table
	 * \param columnIndex index of the column in the table
	 * \param array source data array to connect with column
	 * \return TRUE on success, otherwise FALSE
	 */
	bool SetVariableForTableColumn(const wxString& name, int columnIndex, const wxArrayShort& array);
	/**
	 * \brief Set source data array for column in specified table item.
	 * \param name identification name of the table
	 * \param columnIndex index of the column in the table
	 * \param array source data array to connect with column
	 * \return TRUE on success, otherwise FALSE
	 */
	bool SetVariableForTableColumn(const wxString& name, int columnIndex, const wxArrayInt& array);
	/**
	 * \brief Set source data array for column in specified table item.
	 * \param name identification name of the table
	 * \param columnIndex index of the column in the table
	 * \param array source data array to connect with column
	 * \return TRUE on success, otherwise FALSE
	 */
	bool SetVariableForTableColumn(const wxString& name, int columnIndex, const wxArrayLong& array);
	/**
	 * \brief Set source data array for column in specified table item.
	 * \param name identification name of the table
	 * \param columnIndex index of the column in the table
	 * \param array source data array to connect with column
	 * \return TRUE on success, otherwise FALSE
	 */
	bool SetVariableForTableColumn(const wxString& name, int columnIndex, const wxArrayDouble& array);
	/**
	 * \brief Set source data array for column in specified table item.
	 * \param name identification name of the table
	 * \param columnIndex index of the column in the table
	 * \param array source data array to connect with column
	 * \return TRUE on success, otherwise FALSE
	 */
	bool SetVariableForTableColumn(const wxString& name, int columnIndex, const wxArrayString& array);
	/**
	 * \brief Set source data array for the row in the specified table item.
	 * \param name identification name of the table
	 * \param rowIndex index of the row in the table
	 * \param array source data array to connect with row
	 * \return TRUE on success, otherwise FALSE
	 */
	bool SetVariableForTableRow(const wxString& name, int rowIndex, const wxArrayShort& array);
	/**
	 * \brief Set source data array for the row in the specified table item.
	 * \param name identification name of the table
	 * \param rowIndex index of the row in the table
	 * \param array source data array to connect with row
	 * \return TRUE on success, otherwise FALSE
	 */
	bool SetVariableForTableRow(const wxString& name, int rowIndex, const wxArrayInt& array);
	/**
	 * \brief Set source data array for the row in the specified table item.
	 * \param name identification name of the table
	 * \param rowIndex index of the row in the table
	 * \param array source data array to connect with row
	 * \return TRUE on success, otherwise FALSE
	 */
	bool SetVariableForTableRow(const wxString& name, int rowIndex, const wxArrayLong& array);
	/**
	 * \brief Set source data array for the row in the specified table item.
	 * \param name identification name of the table
	 * \param rowIndex index of the row in the table
	 * \param array source data array to connect with row
	 * \return TRUE on success, otherwise FALSE
	 */
	bool SetVariableForTableRow(const wxString& name, int rowIndex, const wxArrayDouble& array);
	/**
	 * \brief Set source data array for the row in the specified table item.
	 * \param name identification name of the table
	 * \param rowIndex index of the row in the table
	 * \param array source data array to connect with row
	 * \return TRUE on success, otherwise FALSE
	 */
	bool SetVariableForTableRow(const wxString& name, int rowIndex, const wxArrayString& array);
	/**
	 * \brief Append new empty pages to the document.
	 * \param count count of pages to insert
	 */
	void AddPage(int count = 1);
	/**
	 * \brief Append new page defined by passed page layout.
	 * \param source page layout
	 */	
	void AddPage(const wxReportPage& layout); // -1 to end
	/**
	 * \brief Insert new empty pages from specified index.
	 * \param index position where the page will be inserted
	 * \param count number of pages to insert
	 */	
	void AddPageAt(int index, int count = 1);
	/**
	 * \brief Insert new page defined by passed page layout at the specified index.
	 * \param layout page to insert
	 * \param index position where the page will be inserted
	 */	
	void AddPageAt(const wxReportPage& layout, int index);
	/**
	 * \brief Clear page at specified index.
	 * \param index index of the page
	 */	
	void ReplacePage(int index);
	/**
	 * \brief Replace page at specified index by passed page layout.
	 * \param layout page to insert
	 * \param index index of the replaced page
	 */	
	void ReplacePage(const wxReportPage& layout, int index);
	/**
	 * \brief Remove all pages form the document.
	 */	
	void DeleteAllPages();
	/**
	 * \brief Remove page at specified index.
	 * \param index index of the page to delete
	 */	
	void DeletePage(int index);
	/**
	 * \brief Remove specified range of pages.
	 * \param indexFrom start of the range
	 * \param indexTo end of the range
	 */	
	void DeletePages(int indexFrom, int indexTo);
	/**
	 * \brief Insert item to the active page.
	 * \param textItem item to insert
	 */
	void AddItem(const wxReportTextItem& textItem);
	/**
	 * \brief Insert item to the active page.
	 * \param imageItem item to insert
	 */
	void AddItem(const wxReportImageItem& imageItem);
	/**
	 * \brief Insert item to the active page.
	 * \param tableItem item to insert
	 */
	void AddItem(const wxReportTableItem& tableItem);
	/**
	 * \brief Insert item to the active page.
	 * \param lineShape item to insert
	 */
	void AddItem(const wxReportLineShape& lineShape);
	/**
	 * \brief Insert item to the active page.
	 * \param rectShape item to insert
	 */
	void AddItem(const wxReportRectangleShape& rectShape);
	/**
	 * \brief Insert item to the active page.
	 * \param circleShape item to insert
	 */
	void AddItem(const wxReportCircleShape& circleShape);
	/**
	 * \brief Insert item to the active page.
	 * \param item pointer to added item
	 */
	void AddItem(wxReportPageItem *item);
	/**
	 * \brief Add specified item to all pages in the document.
	 * \param item pointer to added item
	 */	
	void AddItemToAllPages(wxReportPageItem *item);
	/**
	 * \brief Add specified item to the specified page.
	 * \param item pointer to added item
	 * \param pageNumber number of page where the item will be added
	 */	
	void AddItemToPage(wxReportPageItem *item, int pageNumber);
	/**
	 * \brief Add item to specified range of pages.
	 * \param item pointer to added item
	 * \param indexFrom index of starting page
	 * \param indexTo index of ending page
	 */	
	void AddItemToPages(wxReportPageItem *item, int indexFrom, int indexTo);
	/**
	 * \brief Insert item to the active page.
	 * \param textItem item to insert
	 */
	void AddItemToHeader(const wxReportTextItem& textItem);
	/**
	 * \brief Insert item to the header.
	 * \param tableItem item to insert
	 */
	void AddItemToHeader(const wxReportTableItem& tableItem);
	/**
	 * \brief Insert item to the header.
	 * \param imageItem item to insert
	 */
	void AddItemToHeader(const wxReportImageItem& imageItem);
	/**
	 * \brief Insert item to the header.
	 * \param lineShape item to insert
	 */
	void AddItemToHeader(const wxReportLineShape& lineShape);
	/**
	 * \brief Insert item to the header.
	 * \param rectShape item to insert
	 */
	void AddItemToHeader(const wxReportRectangleShape& rectShape);
	/**
	 * \brief Insert item to the header.
	 * \param circleShape item to insert
	 */
	void AddItemToHeader(const wxReportCircleShape& circleShape);
	/**
	 * \brief Insert item to the footer.
	 * \param textItem item to insert
	 */
	void AddItemToFooter(const wxReportTextItem& textItem);
	/**
	 * \brief Insert item to the footer.
	 * \param tableItem item to insert
	 */
	void AddItemToFooter(const wxReportTableItem& tableItem);
	/**
	 * \brief Insert item to the footer.
	 * \param imageItem item to insert
	 */
	void AddItemToFooter(const wxReportImageItem& imageItem);
	/**
	 * \brief Insert item to the footer.
	 * \param lineShape item to insert
	 */
	void AddItemToFooter(const wxReportLineShape& lineShape);
	/**
	 * \brief Insert item to the footer.
	 * \param rectShape item to insert
	 */
	void AddItemToFooter(const wxReportRectangleShape& rectShape);
	/**
	 * \brief Insert item to the footer.
	 * \param circleShape item to insert
	 */
	void AddItemToFooter(const wxReportCircleShape& circleShape);
	/**
	 * \brief Replace item specified by its name on all pages in the document.
	 * \param replacedItemName name of replaced item
	 * \param newItem pointer of new item
	 */	
	void ReplaceItemByName(const wxString& replacedItemName, wxReportPageItem *newItem);
	/**
	 * \brief Replace item specified by its name on the specified page.
	 * \param replacedItemName name of replaced item
	 * \param newItem pointer of new item
	 * \param pageNumber number of page on which the specified item will be replaced
	 */	
	void ReplaceItemByName(const wxString& replacedItemName, wxReportPageItem *newItem, int pageNumber);
	/**
	 * \brief Replace item specified by its name on the specified range of pages.
	 * \param replacedItemName name of replaced item
	 * \param newItem pointer of new item
	 * \param indexFrom index of starting page
	 * \param indexTo index of ending page
	 */	
	void ReplaceItemByName(const wxString& replacedItemName, wxReportPageItem *newItem, int indexFrom, int indexTo);
	/**
	 * \brief Replace item specified by its position on all pages.
	 * \param positionIndex position index of replaced item
	 * \param newItem pointer of new item
	 */	
	void ReplaceItemByPosition(int positionIndex, wxReportPageItem *newItem);
	/**
	 * \brief Replace item specified by its position on the specified page.
	 * \param positionIndex position index of replaced item
	 * \param newItem pointer of new item
	 * \param pageNumber number of page on which the specified item will be replaced
	 * \param indexTo number of ending page
	 */	
	void ReplaceItemByPosition(int positionIndex, wxReportPageItem *newItem, int pageNumber);
	/**
	 * \brief Replace item specified by its position on the spicified range of pages.
	 * \param positionIndex position index of replaced item
	 * \param newItem pointer of new item
	 * \param indexFrom index of starting page
	 * \param indexTo index of ending page
	 */	
	void ReplaceItemByPosition(int positionIndex, wxReportPageItem *newItem, int indexFrom, int indexTo);
	/**
	 * \brief Remove items from all pages in the document.
	 * \param itemName name of removed item
	 */	
	void DeleteItemByName(const wxString& itemName);
	/**
	 * \brief Remove item from specified page.
	 * \param itemName name of removed item
	 * \param pageNumber number of page from which the item will bew removed
	 */	
	void DeleteItemByName(const wxString& itemName, int pageNumber);
	/**
	 * \brief Remove item from specified range of pages.
	 * \param itemName name of removed item
	 * \param indexFrom index of starting page
	 * \param indexTo index of ending page
	 */	
	void DeleteItemByName(const wxString& itemName, int indexFrom, int indexTo);
	/**
	 * \brief Inserts page numbering to all apges.
	 * \param format text string with the format of the numbering, e. g. "Page No. %d" where %d means the number of the page 
	 * \param textStyle text style of the numbering
	 * \param posX horizontal position of the number
	 * \param posY vertical position of the number
	 * \param atPlace document section on which the number will be located
	 * \param fromPage page at which the numbering begins
	 */
	void InsertPageNumbering(const wxString& format, const wxReportTextStyle& textStyle, double posX, double posY, int atPlace = wxRP_FOOTER, int fromPage = 0);
	///**
	// * \brief Remove page numbering from all pages.
	// */
	//void RemovePageNumbering();
	///**
	// * \brief Removes page numbering from the specified range of pages.
	// * \param pageFrom index of the first page in range 
	// * \param pageTo index of the last page in range
	// */
	//void RemovePageNumbering(int pageFrom, int pageTo);
	/**
	 * \brief Returns pointer to specified document page.
	 * \param pageNumber index of the page
	 * \return pointer to document page
	 */
	wxReportPage* GetPage(int pageNumber);
	/**
	 * \brief Returns the number of pages in the document.
	 * \return number of pages
	 */
	int GetPagesCount();
	//void SetUnits(int unitType);
	/**
	 * \brief Adds the output file handler.
	 * \param handler pointer to added file handler
	 */
	void AddOutputHandler(wxReportFileHandler* handler);
	/**
	 * \brief Return pointer to the stored file handler specified by its output file suffix.
	 * \param suffix output file suffix
	 * \return pointer to the stored handler
	 */
	wxReportFileHandler* GetHandlerBySuffix(const wxString& suffix);
	/**
	 * \brief Refresh values of all items connected with data sources.
	 */
	void RefreshVariables();
	/**
	 * \brief Activate or deactivate automatic refreshing of items connected with data sources.
	 * \param refresh TRUE to enable, FALSE to disable
	 */
	void SetAutoRefresh(bool refresh);
	/**
	 * \brief Set print data object.
	 * \param printData print data object to set
	 */
	void SetPrintData(const wxPrintData& printData);
	/**
	 * \brief Returns the current print data object.
	 * \return currnet print data object
	 */
	const wxPrintData& GetPrintData();
	/**
	 * \brief Sets the DPI value of document.
	 * \param dpi DPI value to set
	 */
	void SetDPI(int dpi);
	/**
	 * \brief Returns the DPI value of the document.
	 * \return DPI value of the document
	 */
	const int& GetDPI();
	/**
	 * \brief Shows print setup dialog and then starts printig of the document.
	 * \param parent pointer to parent window to show print setup dialog
	 */
	void Print(wxWindow *parent);
	/**
	 * \brief Shows the print preview window.
	 * \param parent pointer to parent window to show print preview window
	 * \param title title of print preview window
	 * \param size size of the previes frame
	 * \param zoom preview zoom in percents
	 * \param maximized show preview frame maximized
	 */
	void ShowPrintPreview(wxWindow *parent, const wxSize& size = wxDefaultSize, int zoom = 100, const wxString& title = wxT("Print preview"), bool maximized = false);
	/**
	 * \brief Save report to output file. Tries to find handler in handlers array identified by the suffix of specified file name or foramat ID. If the suitable handler isn't found than false value is returned.
	 * \param fileName output file name to save
	 * \param formatId out file format ID
	 * \return TRUE on succes, FALSE if fails
	 */
	bool SaveReport(const wxString& fileName, int formatId = 0);
	/**
	 * \brief Save the current document layout.
	 * \param path file name to save
	 * \return TRUE on succes, FALSE if fails
	 */
	bool SaveLayoutToXML(const wxString& path);
	/**
	 * \brief Save the current document layout.
	 * \param out output stream into which the layout will be save
	 * \return TRUE on succes, FALSE if fails
	 */
	bool SaveLayoutToXML(wxOutputStream& out);
	/**
	 * \brief Retrieve document from the specified XML file.
	 * \param path file name to load
	 * \return TRUE on succes, FALSE if fails
	 */
	bool LoadLayoutFromXML(const wxString& path);
	/**
	 * \brief Retrieve document from the specified XML file.
	 * \param in input stream from which the layout will be loaded
	 * \return TRUE on succes, FALSE if fails
	 */
	bool LoadLayoutFromXML(wxInputStream& in);
	
	/*!
	 * \brief Get the library's version.
	 * \return Current wxReportDocument library's version string
	 */
	const wxString& GetVersion();
};

#endif // WXREPORTDOCUMENT_H
