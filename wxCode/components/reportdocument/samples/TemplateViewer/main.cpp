#include "main.h"
#include "wx/dcbuffer.h"

// initialize the application
IMPLEMENT_APP(MainApp);

////////////////////////////////////////////////////////////////////////////////
// application class implementation 
////////////////////////////////////////////////////////////////////////////////

bool MainApp::OnInit()
{
	SetTopWindow( new MainFrame( NULL ) );
	GetTopWindow()->Show();
	
	// true = enter the main loop
	return true;
}

////////////////////////////////////////////////////////////////////////////////
// main application frame implementation 
////////////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame(wxWindow *parent) : MainFrameBase( parent )
{
	/*int w = 0, h = 0;
	this->GetClientSize(&w, &h);
	w = 0.7*h;
	this->m_canvas->SetSize(w, h);*/
	this->m_canvas->SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	this->m_canvas->SetFocus();
	this->m_canvas->Refresh(false);
}

MainFrame::~MainFrame()
{	
}

void MainFrame::DrawPage()
{
	this->m_canvas->Refresh(false);
}

void MainFrame::OnPaint(wxPaintEvent& event)
{
	int pageNumber = this->m_spinPages->GetValue() - 1;
	//wxDC dc1;
	
	wxBitmap bm(420, 594, 24);
	wxMemoryDC memdc(bm);
	memdc.SetBackground(*wxWHITE_BRUSH);
	memdc.Clear();
	memdc.SetUserScale(0.53,0.53);
	
	if(pageNumber >= 0 && pageNumber < prt.GetPagesCount())
	{
		prt.GetPage(pageNumber)->DrawToDC(&memdc, true);		
	}
	
	memdc.SelectObject(wxNullBitmap);
	
	wxAutoBufferedPaintDC dc(this->m_canvas);
	//dc.SetLogicalScale(1.89, 1.89);
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	dc.DrawBitmap(bm, 0, 0);
	
	/*if(pageNumber >= 0 && pageNumber < prt.GetPagesCount())
	{
		prt.GetPage(pageNumber)->DrawToDC(&dc, false);		
	}*/			
}

void MainFrame::OnPrint(wxCommandEvent& event)
{
	prt.Print(this);
}

void MainFrame::OnPageSetup(wxCommandEvent& event)
{
	wxPageSetupDialogData data(g_printData);
	wxPageSetupDialog dlg(this, &data);
	if(dlg.ShowModal() == wxID_OK)
	{
		g_printData = dlg.GetPageSetupDialogData().GetPrintData();
		prt.SetPrintData(dlg.GetPageSetupDialogData().GetPrintData());
	}
}

void MainFrame::OnCloseFrame(wxCloseEvent& event)
{
	Destroy();
}

void MainFrame::OnExitClick(wxCommandEvent& event)
{
	Destroy();
}

void MainFrame::OnOpen(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Open layout"));
	dlg.SetWildcard(wxT("XML files (*.xml)|*.xml"));
	
	if(dlg.ShowModal() == wxID_OK)
	{
		if(prt.LoadLayoutFromXML(dlg.GetPath()))
		{
			this->m_spinPages->SetRange(1, prt.GetPagesCount());
			this->m_spinPages->SetValue(1);
			this->DrawPage();
		}
		else
		{
			this->m_spinPages->SetRange(0, 0);
			this->m_spinPages->SetValue(0);
			this->m_spinPages->Enable(false);
			wxMessageBox(wxT("Layout opening failed!"), wxT("Error"), wxOK|wxICON_ERROR|wxCENTRE, this);
		}
	}
}

void MainFrame::OnPageNumber(wxCommandEvent& event)
{
	this->DrawPage();
}

void MainFrame::OnPageNumber(wxSpinEvent& event)
{
	this->DrawPage();
}

void MainFrame::OnPreview(wxCommandEvent& event)
{
	this->prt.ShowPrintPreview(this);
}