#include "wx/report/printout.h"

void wxReportPrintout::SetPages(PagesArray* pagesArray)
{
	if(pagesArray)
		this->m_pages = pagesArray;
}

bool wxReportPrintout::OnBeginDocument(int startPage, int endPage)
{
	this->m_startPage = startPage;
	this->m_endPage = endPage;
	
	bool status = (this->m_pages != NULL) && wxPrintout::OnBeginDocument(this->m_startPage, this->m_endPage);
	return status;
}

bool wxReportPrintout::OnPrintPage(int pageNum)
{
	wxReportPage *page = this->m_pages->Item(pageNum - 1);
	wxDC *dc = this->GetDC();
	
	/*if(this->IsPreview())
	{
		this->
		
		
		int sx, sy, px, py;
		this->GetPPIScreen(&sx, &sy);
		this->GetPPIPrinter(&px, &py);	
		double scale = (double)px / (double)sx;
	}
	else
	{
		int sx, sy, px, py;
		this->GetPPIScreen(&sx, &sy);
		this->GetPPIPrinter(&px, &py);	
		double scale = (double)px / (double)sx;
	}*/
	
	int sx, sy, px, py;
	this->GetPPIScreen(&sx, &sy);
	this->GetPPIPrinter(&px, &py);	
	double scale = (double)px / (double)sx;
		
	int pageWidth, pageHeight, w, h;
	dc->GetSize(&w, &h);
	GetPageSizePixels(&pageWidth, &pageHeight);
	double overallScale = scale * (double)(w/(double)pageWidth);
	dc->SetUserScale(overallScale, overallScale);
	page->DrawToDC(dc, this->IsPreview());
	return true;
}

void wxReportPrintout::GetPageInfo(int* minPage, int* maxPage, int* pageFrom, int* pageTo)
{
	*minPage = 1;
	*maxPage = this->m_pages->GetCount();
	*pageFrom = 1;
	*pageTo = this->m_pages->GetCount(); 
}

bool wxReportPrintout::HasPage(int pageNum)
{
	return (pageNum >= this->m_startPage && pageNum <= this->m_endPage);
}