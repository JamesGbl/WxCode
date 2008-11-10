/////////////////////////////////////////////////////////////////////////////
// Name:        MainFrame.h
// Purpose:     Definitions of MainFrame - mainWindow of demo application
// Author:      Moskvichev Andrey V.
// Created:     2008/10/09
// RCS-ID:      $Id: wxAdvTable.h,v 1.3 2008/10/09 16:42:58 frm Exp $
// Copyright:   (c) 2008 Moskvichev A.V.
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef MAINFRAME_H_
#define MAINFRAME_H_

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "wxadvtable.h"

class ControlPanel : public wxPanel
{
public:
	ControlPanel(wxWindow *parent, wxAdvTable *_table);
	virtual ~ControlPanel();

private:
	wxAdvTable *table;

	DECLARE_EVENT_TABLE()
};

/**
 * Demo application main frame.
 */
class MainFrame : public wxFrame
{
public:
	MainFrame();
	virtual ~MainFrame();

private:
	void CreateTableStructure();

	void OnExit(wxCommandEvent &ev);

	wxAdvTable *advTable;

	DECLARE_EVENT_TABLE()
};

#endif /*MAINFRAME_H_*/
