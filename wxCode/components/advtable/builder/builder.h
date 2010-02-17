/////////////////////////////////////////////////////////////////////////////
// Name:	builder.h
// Purpose: wxAdvTable builder application declarations
// Author:	Moskvichev Andrey V.
// Created:	20.09.2009
// Copyright:	(c) 2009-2010 Moskvichev Andrey V.
// Licence:	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef BUILDER_H_
#define BUILDER_H_

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "wx/wxadvtable.h"

#include <wx/docview.h>
#include <wx/txtstrm.h>

#include "propertypage.h"

class MainFrame;

class wxAdvTableSerializer
{
public:
	static wxString SaveHdrCell(wxAdvHdrCell *hdrCell);

	static bool LoadHdrCell(wxAdvHdrCell &hdrCell, const wxString &text);
	static void LoadHdrCells(const wxString &text, wxAdvHdrCellArray &out);

private:
	static wxString AlignVertical(wxAdvHdrCell *hdrCell);
	static wxString AlignHorizontal(wxAdvHdrCell *hdrCell);

	static wxString SaveText(const wxString &text);
	static wxString LoadText(const wxString &text);

	static wxString GetArg(const wxString &text);
};

/**
 * Builder document.
 */
class BuilderDocument : public wxDocument
{
	DECLARE_DYNAMIC_CLASS(BuilderDocument)
public:
	BuilderDocument();
	virtual ~BuilderDocument();

	void SetTable(wxAdvTable *table);

	void Commit(wxAdvTable *table, wxAdvHdrCellArray &rows, wxAdvHdrCellArray &cols);

	// save/load methods
	virtual wxOutputStream& SaveObject(wxOutputStream& stream);
	virtual wxInputStream& LoadObject(wxInputStream& stream);

	// update methods
	void BeginUpdate();
	void EndUpdate();

private:
	int m_updateCount;

	wxAdvTable *m_table;
};

/**
 * Builder document view.
 */
class BuilderView : public wxView
{
	DECLARE_DYNAMIC_CLASS(BuilderView)
public:
	BuilderView();
	virtual ~BuilderView();

	virtual bool OnCreate(wxDocument *doc, long flags);
    virtual void OnDraw(wxDC *dc);
    virtual void OnUpdate(wxView *sender, wxObject *hint = (wxObject *) NULL);
    virtual bool OnClose(bool deleteWindow = true);
};

/**
 * Panel for wxAdvTable.
 */
class TablePanel : public wxPanel
{
public:
	TablePanel(MainFrame *parent);
	virtual ~TablePanel();

	wxAdvTable *GetTable();
	void EmptyTable();

	void InsertRows();
	void InsertCols();

private:
	void CreatePopupMenus();

	void OnAdvTableHdrCellRightClick(wxAdvHdrCellEvent &ev);
	void OnAdvTableHdrCellLeftClick(wxAdvHdrCellEvent &ev);

	void OnHdrCellAddSub(wxCommandEvent &ev);
	void OnHdrCellDelete(wxCommandEvent &ev);

	wxAdvTable *m_table;

	wxMenu *m_popupMenuHdrCell;

	wxAdvHdrCell *m_hdrCell;

	DECLARE_EVENT_TABLE()
};

/**
 * Builder main frame.
 */
class MainFrame : public wxDocParentFrame
{
	DECLARE_CLASS(MainFrame)
public:
	MainFrame(wxDocManager *docManager);
	virtual ~MainFrame();

	void SetDocument(BuilderDocument *doc);

	void UpdateGUI();
	void OnCloseDocument();

	PropertyPage *GetPropertyPage();

	static MainFrame *Get();

private:
	void CreateMenuBar();

	void OnExit(wxCommandEvent &ev);
	void OnAbout(wxCommandEvent &ev);
	void OnMenuInsertRows(wxCommandEvent &ev);
	void OnMenuInsertCols(wxCommandEvent &ev);

	PropertyPage *m_propPage;
	TablePanel *m_tablePanel;

	BuilderDocument *m_doc;

	DECLARE_EVENT_TABLE()
};

#endif /*BUILDER_H_*/
