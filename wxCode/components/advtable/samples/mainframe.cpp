/////////////////////////////////////////////////////////////////////////////
// Name:        MainFrame.cpp
// Purpose:     
// Author:      Moskvichev Andrey V.
// Created:     2008/10/09
// RCS-ID:      $Id: wxAdvTable.h,v 1.3 2008/10/09 16:42:58 frm Exp $
// Copyright:   (c) 2008 Moskvichev A.V.
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "MainFrame.h"
#include <wx/aui/aui.h>

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
END_EVENT_TABLE()

MainFrame::MainFrame()
: wxFrame(NULL, wxID_ANY, wxT("wxAdvTable demo"), wxDefaultPosition, wxSize(800, 445))
{
	wxAuiManager *auiManager = new wxAuiManager(this);
	
	advTable = new wxAdvTable(this, wxID_ANY);
	auiManager->AddPane(advTable, wxAuiPaneInfo().Center().Caption(wxT("wxAdvTable")));
	
	auiManager->Update();
	
	Centre();
	
	// Create main menu
	wxMenuBar *menuBar = new wxMenuBar();
	
	wxMenu *menuFile = new wxMenu();
	menuFile->Append(wxID_EXIT, wxT("E&xit"));
	
	menuBar->Append(menuFile, wxT("&File"));
	
	SetMenuBar(menuBar);
	
	CreateTableStructure();
}

MainFrame::~MainFrame()
{
}

void MainFrame::OnExit(wxCommandEvent &ev)
{
	Close();
}

void MainFrame::CreateTableStructure()
{
	wxAdvHdrCell rows[] = {
		wxAdvHdrCell(wxT("Row I")),
		wxAdvHdrCell(wxT("Row II")).Sub(wxAdvHdrCell(wxT("Row II-I")).Sub(wxT("Row II-I-I")).Sub(wxT("Row II-I-II"))).Sub(wxT("Row II-II")).Sub(wxT("Row II-III")),
	};
	wxAdvHdrCell cols[] = {
		wxAdvHdrCell(wxT("Col I")).Size(100),
		wxAdvHdrCell(wxT("Col II")),
		wxAdvHdrCell(wxT("Col III")),
		wxAdvHdrCell(wxT("Col IV")).Sub(wxT("Col IV-I")).Sub(wxT("Col IV-II")).Sub(wxT("Col\nIV-III")),
		wxAdvHdrCell(wxT("Col V")),
	};
			
	advTable->Create(rows, 2, cols, 5, wxT("Test table"));	
	 
	wxASSERT_MSG(advTable->GetRowCount() == 2, wxT("advTable->GetRowCount() == 2"));
	wxASSERT_MSG(advTable->GetColCount() == 5, wxT("advTable->GetColCount() == 5"));
	
	wxASSERT_MSG(!advTable->GetRow(0).IsComposite(), wxT("!advTable->GetRow(0).IsComposite()"));
	wxASSERT_MSG(advTable->GetRow(1).IsComposite(), wxT("advTable->GetRow(1).IsComposite()"));
}
