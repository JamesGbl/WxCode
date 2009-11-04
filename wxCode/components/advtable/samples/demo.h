/////////////////////////////////////////////////////////////////////////////
// Name:        demo.h
// Purpose:     demo application declarations
// Author:      Moskvichev Andrey V.
// Created:     2008/10/09
// RCS-ID:      $Id: wxAdvTable.h,v 1.3 2008/10/09 16:42:58 frm Exp $
// Copyright:   (c) 2008-2009 Moskvichev A.V.
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef DEMO_H_
#define DEMO_H_

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "wx/wxadvtable.h"

/**
 * Control panel is GUI object used to set
 * wxAdvTable properties.
 */
class ControlPanel : public wxPanel
{
public:
	ControlPanel(wxWindow *parent, wxAdvTable *advTable);
	virtual ~ControlPanel();

private:
	//
	// Event handlers
	//
	void OnChoiceSelectMode(wxCommandEvent &ev);
	void OnChoiceHighlightMode(wxCommandEvent &ev);
	void OnChoiceSortMode(wxCommandEvent &ev);

	void OnCheckShowRows(wxCommandEvent &ev);
	void OnCheckShowCols(wxCommandEvent &ev);
	void OnCheckShowCorner(wxCommandEvent &ev);

	void OnCheckSortByAnyRow(wxCommandEvent &ev);
	void OnCheckSortByAnyCol(wxCommandEvent &ev);

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

	//
	// Event handlers
	//
	void OnAbout(wxCommandEvent &ev);
	void OnExit(wxCommandEvent &ev);

	void OnGridLeftClick(wxGridEvent &ev);
	void OnGridRightClick(wxGridEvent &ev);
	void OnGridLeftDClick(wxGridEvent &ev);
	void OnGridRightDClick(wxGridEvent &ev);
	void OnGridRangeSelect(wxGridRangeSelectEvent &ev);
	void OnGridCellChange(wxGridEvent &ev);
	void OnGridSelectCell(wxGridEvent &ev);
	void OnGridEditorShown(wxGridEvent &ev);
	void OnGridEditorHidden(wxGridEvent &ev);

	void OnAdvTableHdrCellLeftClick(wxAdvHdrCellEvent &ev);
	void OnAdvTableHdrCellRightClick(wxAdvHdrCellEvent &ev);
	void OnAdvTableHdrCellLeftDClick(wxAdvHdrCellEvent &ev);
	void OnAdvTableHdrCellRightDClick(wxAdvHdrCellEvent &ev);


	wxAdvTable *m_advTable;
	ControlPanel *m_controlPanel;

	DECLARE_EVENT_TABLE()
};

#endif /*DEMO_H_*/
