///////////////////////////////////////////////////////////////////////////////
// Name:        sheetdemo.h
// Purpose:     wxsheet control wxWidgets sample
// Author:      Michael Bedward
// Modified by: John Labenski
// RCS-ID:      $Id: sheetdemo.h,v 1.9 2007/07/06 02:54:38 jrl1 Exp $
// Copyright:   (c) Michael Bedward, Julian Smart
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////


#ifndef sheetdemo_h
#define sheetdemo_h

class wxSheet;

class GridApp : public wxApp
{
public:
    bool OnInit();
};


class GridFrame : public wxFrame
{
public:
    wxSheet        *grid;
    wxTextCtrl     *logWin;
    wxLogTextCtrl  *logger;
    wxString       logBuf;

    void SetDefaults();

    void ToggleRowLabels( wxCommandEvent& event );
    void ToggleColLabels( wxCommandEvent& event );
    void ToggleEditing( wxCommandEvent& event );
    void ToggleRowSizing( wxCommandEvent& event );
    void ToggleColSizing( wxCommandEvent& event );
    void ToggleGridSizing( wxCommandEvent& event );
    void ToggleGridLines( wxCommandEvent& event );
    void OnHideShowSelRowsCols( wxCommandEvent& event );
    void AutoSizeCols( wxCommandEvent& event );
    void CellOverflow( wxCommandEvent& event );
    void ResizeCell( wxCommandEvent& event );
    void SetLabelColour( wxCommandEvent& event );
    void SetLabelTextColour( wxCommandEvent& event );
    void SetLabelFont( wxCommandEvent& event );
    void OnScrollbarsShown( wxCommandEvent& event );
    void SetRowLabelHorizAlignment( wxCommandEvent& event );
    void SetRowLabelVertAlignment( wxCommandEvent& event );
    void SetColLabelHorizAlignment( wxCommandEvent& event );
    void SetColLabelVertAlignment( wxCommandEvent& event );
    void SetGridLineColour( wxCommandEvent& event );

    void SetCellFgColour( wxCommandEvent& event );
    void SetCellBgColour( wxCommandEvent& event );

    void OnCopyPaste( wxCommandEvent &event );
    void InsertRow( wxCommandEvent& event );
    void InsertCol( wxCommandEvent& event );
    void DeleteSelectedRows( wxCommandEvent& event );
    void DeleteSelectedCols( wxCommandEvent& event );
    void ClearGrid( wxCommandEvent& event );
    void EnableGrid( wxCommandEvent& event );
    void SelectionMode( wxCommandEvent& event );

    void DeselectCell( wxCommandEvent& event );
    void DeselectCol( wxCommandEvent& event );
    void DeselectRow( wxCommandEvent& event );
    void DeselectAll( wxCommandEvent& event );
    void SelectCell( wxCommandEvent& event );
    void SelectCol( wxCommandEvent& event );
    void SelectRow( wxCommandEvent& event );
    void SelectAll( wxCommandEvent& event );
    void OnAddToSelectToggle( wxCommandEvent& event );

    void OnLabelLeftDown( wxSheetEvent& event );
    void OnCellLeftDown( wxSheetEvent& event );
    void OnCellRightDown( wxSheetEvent& event );
    void OnRowSize( wxSheetCellSizeEvent& event );
    void OnColSize( wxSheetCellSizeEvent& event );
    void OnSelectCell( wxSheetEvent& event );
    void OnRangeSelect( wxSheetRangeSelectEvent& event );
    void OnCellValueChanged( wxSheetEvent& event );

    void OnEditorShown( wxSheetEvent& event );
    void OnEditorHidden( wxSheetEvent& event );

    void OnSheetSplitter( wxSheetSplitterEvent& event );

    void OnSetHighlightWidth( wxCommandEvent& event );
    void OnSetROHighlightWidth( wxCommandEvent& event );

public:
    GridFrame();
    ~GridFrame();

    void OnQuit( wxCommandEvent& event);
    void About( wxCommandEvent& event);
    void OnVTable( wxCommandEvent& event);
    void OnBugsTable( wxCommandEvent& event);
    void OnSmallGrid( wxCommandEvent& event);

    enum
    {
        ID_TOGGLEROWLABELS = 100,
        ID_TOGGLECOLLABELS,
        ID_TOGGLEEDIT,
        ID_TOGGLEROWSIZING,
        ID_TOGGLECOLSIZING,
        ID_TOGGLEROWSIZINGVETO,
        ID_TOGGLECOLSIZINGVETO,
        ID_TOGGLEGRIDSIZING,
        ID_TOGGLEHORIZGRIDLINES,
        ID_TOGGLEVERTGRIDLINES,
        ID_HIDESELROWS,
        ID_HIDESELCOLS,
        ID_SHOWSELROWS,
        ID_SHOWSELCOLS,
        ID_AUTOSIZECOLS,
        ID_CELLOVERFLOW,
        ID_RESIZECELL,
        ID_SETLABELCOLOUR,
        ID_SETLABELTEXTCOLOUR,
        ID_SETLABEL_FONT,

        ID_SCROLLBARS_MENU,
        ID_SCROLLBARS_AS_NEEDED,
        ID_SCROLLBARS_HORIZ_NEVER,
        ID_SCROLLBARS_VERT_NEVER,
        ID_SCROLLBARS_NEVER,
        ID_SCROLLBARS_HORIZ_ALWAYS,
        ID_SCROLLBARS_VERT_ALWAYS,
        ID_SCROLLBARS_ALWAYS,

        ID_ROWLABELALIGN,
        ID_ROWLABELHORIZALIGN,
        ID_ROWLABELVERTALIGN,

        ID_COLLABELALIGN,
        ID_COLLABELHORIZALIGN,
        ID_COLLABELVERTALIGN,
        ID_GRIDLINECOLOUR,
        ID_INSERTROW,
        ID_INSERTCOL,
        ID_DELETEROW,
        ID_DELETECOL,
        ID_CLEARGRID,
        ID_ENABLEGRID,
        ID_CHANGESEL,
        ID_SELNONE,
        ID_SELCELLS,
        ID_SELROWS,
        ID_SELCOLS,
        ID_SELSINGLE,
        ID_SET_CELL_FG_COLOUR,
        ID_SET_CELL_BG_COLOUR,
        ID_ABOUT,
        ID_VTABLE,
        ID_BUGS_TABLE,
        ID_SMALL_GRID,
        ID_SELECT_UNSELECT,
        ID_SELECT_ALL,
        ID_SELECT_ROW,
        ID_SELECT_COL,
        ID_SELECT_CELL,
        ID_DESELECT_ALL,
        ID_DESELECT_ROW,
        ID_DESELECT_COL,
        ID_DESELECT_CELL,

        ID_SET_HIGHLIGHT_WIDTH,
        ID_SET_RO_HIGHLIGHT_WIDTH,

        ID_TESTFUNC
    };

    wxLog *m_logOld;

    // add the cells to selection when using commands from select menu?
    bool m_addToSel;

    DECLARE_EVENT_TABLE()
};

class MyGridCellRendererRefData : public wxSheetCellStringRendererRefData
{
public:
    MyGridCellRendererRefData() { }
    virtual void Draw(wxSheet& grid, const wxSheetCellAttr& attr, wxDC& dc,
                      const wxRect& rect, const wxSheetCoords& coords,
                      bool isSelected);

    bool Copy(const MyGridCellRendererRefData& other) { return wxSheetCellStringRendererRefData::Copy(other); }
    DECLARE_SHEETOBJREFDATA_COPY_CLASS(MyGridCellRendererRefData, wxSheetCellRendererRefData)
};

// ----------------------------------------------------------------------------
// demonstration of virtual table which doesn't store all of its data in
// memory
// ----------------------------------------------------------------------------

class BigGridTable : public wxSheetTable
{
public:
    BigGridTable(long sizeGrid) { m_sizeGrid = sizeGrid; }

    int GetNumberRows() { return m_sizeGrid; }
    int GetNumberCols() { return m_sizeGrid; }
    virtual wxString GetValue( const wxSheetCoords& coords );

private:
    long m_sizeGrid;
};

class BigGridFrame : public wxFrame
{
public:
    BigGridFrame(long sizeGrid);

    wxSheet*       m_grid;
private:
    BigGridTable* m_table;
};

// ----------------------------------------------------------------------------
// MyGridCellAttrProvider : an example of custom attr provider
//   makes all odd rows appear grey and uses rainbow colours for the col headers
// ----------------------------------------------------------------------------

class MyGridCellAttrProvider : public wxSheetCellAttrProvider
{
public:
    MyGridCellAttrProvider(wxSheet *sheet);

    virtual wxSheetCellAttr GetAttr( const wxSheetCoords& coords,
                                     wxSheetAttr_Type kind );

private:
    wxSheet *m_sheet;
};

// ----------------------------------------------------------------------------
// another, more realistic, grid example: shows typed columns and more
// ----------------------------------------------------------------------------

class BugsGridTable : public wxSheetTable
{
public:
    BugsGridTable();

    virtual int GetNumberRows();
    virtual int GetNumberCols();
    virtual bool HasValue( const wxSheetCoords& coords );
    virtual wxString GetValue( const wxSheetCoords& coords );
    virtual void SetValue( const wxSheetCoords& coords, const wxString& value );

    virtual wxString GetTypeName( const wxSheetCoords& coords );
    virtual bool CanGetValueAs( const wxSheetCoords& coords, const wxString& typeName );
    virtual bool CanSetValueAs( const wxSheetCoords& coords, const wxString& typeName );

    virtual long GetValueAsLong( const wxSheetCoords& coords );
    virtual bool GetValueAsBool( const wxSheetCoords& coords );

    virtual void SetValueAsLong( const wxSheetCoords& coords, long value );
    virtual void SetValueAsBool( const wxSheetCoords& coords, bool value );
};

class BugsGridFrame : public wxFrame
{
public:
    BugsGridFrame();
};


#endif // sheetdemo_h
