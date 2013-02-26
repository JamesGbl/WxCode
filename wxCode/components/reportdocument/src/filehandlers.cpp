#include "wx/report/filehandlers.h"

//////////// wxReportFileHandler class

wxReportFileHandler::wxReportFileHandler()
{
	this->m_iFormatId = 0;
	this->m_pPages = NULL;
}

const int& wxReportFileHandler::GetFormatId() const
{
	return this->m_iFormatId;
}

const wxString& wxReportFileHandler::GetFormatSuffix() const
{
	return this->m_sFormatSuffix;
}