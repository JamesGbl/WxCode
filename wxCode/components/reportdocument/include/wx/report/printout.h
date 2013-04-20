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
	 * \param title printout title
	 */
	wxReportPrintout(const wxString& title) : wxPrintout(title) {m_pages = NULL;}
	/**
	 * \brief Default destructor. 
	 */
	~wxReportPrintout() {;}
	/**
	 * \brief Sets the source array of pages to print.
	 * \param pagesArray pointer to the pages array
	 */
	void SetPages(PagesArray *pagesArray);
	/**
	 * \brief Called automatically by print framework before the printing of the each copy will started.
	 * \param startPage starting page to print
	 * \param endPage ending page to print
	 * \return TRUE on succes, otherwise returns FALSE
	 */
	bool OnBeginDocument(int startPage, int endPage);
	/**
	 * \brief Called automatically by print framework before the printing of the each page will started
	 * \param pageNum number of printed page
	 * \return TRUE on succes, otherwise returns FALSE
	 */
	bool OnPrintPage(int pageNum);
	/**
	 * \brief Called automatically by print framework. Sets the print range.
	 * \param minPage pointer to value where 'min' page number will be stored
	 * \param maxPage pointer to value where 'max' page number will be stored
	 * \param pageFrom pointer to value where 'from' page number will be stored
	 * \param pageTo pointer to value where 'to' page number will be stored
	 */
	void GetPageInfo(int *minPage, int *maxPage, int *pageFrom, int *pageTo);
	/**
	 * \brief Called automatically by print framework. Checks if there is still some page to print.
	 * \param pageNum page number
	 * \return FALSE when the all pages have been printed.
	 */
	bool HasPage(int pageNum);
};

#endif // PRINTOUT_H