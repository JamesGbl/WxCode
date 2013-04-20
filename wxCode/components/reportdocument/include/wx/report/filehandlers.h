#ifndef FILEHANDLERS_H
#define FILEHANDLERS_H

#include "wx/report/pagelayout.h"

/////////// wxReportFileHandler class

/**
 * \brief Base class for output file handlers.
 */
class WXDLLIMPEXP_RP wxReportFileHandler
{
protected:
	int m_iFormatId;
	wxString m_sFormatSuffix;
	PagesArray *m_pPages;
	
public:
	/**
	 * \brief Default constructor.
	 */
	wxReportFileHandler();
	/**
	 * \brief Default destructor.
	 */
	~wxReportFileHandler() {;}
	/**
	 * \brief Save print report to the specified output file.
	 * \param pages pointer to the array of pages to save
	 * \param fileName name of the output file
	 * \param DPI
	 * \param printData
	 */
	virtual void SaveToFile(PagesArray *pages, const wxString& fileName, int DPI, const wxPrintData& printData) {;}
	/**
	 * \brief Get the handler ID associated with this handler.
	 * \return handler ID
	 */
	const int& GetFormatId() const;
	/**
	 * \brief Get the file suffix of this format.
	 * \return file suffix
	 */
	const wxString& GetFormatSuffix() const;
};

WX_DECLARE_USER_EXPORTED_OBJARRAY(wxReportFileHandler*, HandlersArray, WXDLLIMPEXP_RP);

#endif