#ifndef PRINTOUT_H
#define PRINTOUT_H

#include "wx/report/pagelayout.h"
#include "wx/print.h"
#include "wx/printdlg.h"

/**
 * \brief Class for printing operations. Derived form wxPrintout class.
 */
class WXDLLIMPEXP_RP wxReportPrintout : public wxPrintout
{
protected:
	int m_startPage;
	int m_endPage;
	PagesArray *m_pages;
	
public:
	/**
	 * \brief Constructor. 
	 * \param title
	 */
	wxReportPrintout(const wxString& title) : wxPrintout(title) {m_pages = NULL;}
	/**
	 * \brief Default dectructor. 
	 */
	~wxReportPrintout() {;}
	/**
	 * \brief Sets the source array of pages to print.
	 * \param pagesArray pointer to the pages array
	 */
	void SetPages(PagesArray *pagesArray);
	/**
	 * \brief Called automatically by print framework before the printing of the each copy will started.
	 * \param startPage
	 * \param endPage
	 * \return TRUE on succes, otherwise returns FALSE
	 */
	bool OnBeginDocument(int startPage, int endPage);
	/**
	 * \brief Called automatically by print framework before the printing of the each page will started
	 * \param pageNum
	 * \return TRUE on succes, otherwise returns FALSE
	 */
	bool OnPrintPage(int pageNum);
	/**
	 * \brief Called automatically by print framework. Sets the print range.
	 * \param minPage
	 * \param maxPage
	 * \param pageFrom
	 * \param pageTo
	 */
	void GetPageInfo(int *minPage, int *maxPage, int *pageFrom, int *pageTo);
	/**
	 * \brief Called automatically by print framework. Checks if there is still some page to print.
	 * \param pageNum
	 * \return FALSE when the all pages have been printed.
	 */
	bool HasPage(int pageNum);
};

#endif // PRINTOUT_H