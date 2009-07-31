/////////////////////////////////////////////////////////////////////////////
// Name:        demo.h
// Purpose:
// Author:      Moskvichev Andrey V.
// Created:     2008/10/09
// RCS-ID:      $Id: wxAdvTable.h,v 1.3 2008/10/09 16:42:58 frm Exp $
// Copyright:   (c) 2008-2009 Moskvichev A.V.
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

#include "wx/wxadvtable.h"

class ControlPanel : public wxPanel
{
public:
	ControlPanel(wxWindow *parent, wxAdvTable *advTable);
	virtual ~ControlPanel();

private:
	void OnChoiceSelectMode(wxCommandEvent &ev);
	void OnChoiceHighlightMode(wxCommandEvent &ev);
	void OnCheckShowRows(wxCommandEvent &ev);
	void OnCheckShowCols(wxCommandEvent &ev);
	void OnCheckShowCorner(wxCommandEvent &ev);

	void OnSpinCtrlGridWidth(wxSpinEvent &ev);
	void OnSpinCtrlFocusedWidth(wxSpinEvent &ev);

	wxAdvTable *m_advTable;

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

	void OnAbout(wxCommandEvent &ev);
	void OnExit(wxCommandEvent &ev);

	void OnGridLeftClick(wxGridEvent &ev);
	void OnGridRightClick(wxGridEvent &ev);
	void OnGridLeftDClick(wxGridEvent &ev);
	void OnGridRightDClick(wxGridEvent &ev);
	void OnGridRangeSelect(wxGridRangeSelectEvent &ev);

	wxAdvTable *m_advTable;
	ControlPanel *m_controlPanel;

	DECLARE_EVENT_TABLE()
};

#endif /*MAINFRAME_H_*/
