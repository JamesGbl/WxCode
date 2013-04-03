///////////////////////////////////////////////////////////////////////////////
// Name:        griddemo.cpp
// Purpose:     Grid control wxWidgets sample
// Author:      Michael Bedward
// Modified by: John Labenski
// RCS-ID:      $Id: sheetdemo.cpp,v 1.18 2007/12/12 05:22:39 jrl1 Exp $
// Copyright:   (c) Michael Bedward, Julian Smart
// Licence:     wxWidgets license
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma implementation
    #pragma interface
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/colordlg.h>
#include <wx/fontdlg.h>
#include <wx/artprov.h>
#include <wx/stopwatch.h>

#if wxCHECK_VERSION(2,7,0)
    #include <wx/numdlg.h>
#endif //wxCHECK_VERSION(2,7,0)

#include "wx/sheet/sheet.h"
#include "wx/sheet/sheetspt.h"

#include "sheetdemo.h"

//#include <crtdbg.h> // FOR MSW DEBUG

// ----------------------------------------------------------------------------
// wxWin macros
// ----------------------------------------------------------------------------

IMPLEMENT_APP( GridApp )

// ============================================================================
// Preformance and Acuuracy Tests
// ============================================================================
//int s_random_val = 0;
//#define RAND(max_val) ((s_random_val = (1366*s_random_val + 150889) % 714025) ? (max_val*s_random_val/714025) : (max_val*s_random_val/714025))

// psuedo random number generator, nicely repeatable
class PsuedoRandom
{
public:
    // generate random numbers starting with seed from 0 to max-1
    PsuedoRandom(int seed = 0, int max = 100) : m_value(seed), m_max(max) {}

    int Get()
    {
        //m_value = (4096*m_value + 150889) % 714025;
        m_value = (1366*m_value + 150889) % 714025;
        return int((m_max * m_value) / 714025.0);
    }
    int Get(int max)
    {
        m_value = (1366*m_value + 150889) % 714025;
        return int((max * m_value) / 714025.0);
    }

    long m_value, m_max;
};


void TestwxSheetValueProvider_SetValue(int numRows, int numCols, wxSheetValueProviderBase* values)
{
    wxString v(wxT("1")), val;
    PsuedoRandom rnd(0, wxMin(numRows, numCols));
    int n, count = numRows * numCols * 3;
    wxLogDebug(wxT("Start TestwxSheetValueProvider_SetValue\n"));
    wxStopWatch sw;

#if 1
    for (n = 0; n < count; n++)
        values->SetValue(wxSheetCoords(0,rnd.Get(numCols)), v);
    values->ClearValues();
    for (n = 0; n < count; n++)
        values->SetValue(wxSheetCoords(0,rnd.Get(numCols)), v);
    values->ClearValues();
    for (n = 0; n < count; n++)
        values->SetValue(wxSheetCoords(0,rnd.Get(numCols)), v);
#else
    for (n = 0; n < count; n++)
        values->SetValue(wxSheetCoords(rnd.Get(numRows),rnd.Get(numCols)), v);

    for (n = 0; n < count; n++)
        val = values->GetValue(wxSheetCoords(rnd.Get(numRows),rnd.Get(numCols)));
    for (n = 0; n < count; n++)
        val = values->GetValue(wxSheetCoords(rnd.Get(numRows),rnd.Get(numCols)));
    for (n = 0; n < count; n++)
        val = values->GetValue(wxSheetCoords(rnd.Get(numRows),rnd.Get(numCols)));
#endif
    wxLogDebug(wxT("End TestwxSheetValueProvider_SetValue Time %ld\n"), sw.Time());
    fflush(stdout); fflush(stderr);
}

void TestwxSheetValueProvider()
{
    int numRows = 500, numCols = numRows;
    PsuedoRandom rnd(0, wxMin(numRows, numCols));

    wxStopWatch sw;
    wxLogDebug(wxT("Start TestwxSheetValueProvider\n"));
    {
        wxLogDebug(wxT("Start wxSheetValueProviderString\n"));
        wxSheetValueProviderString values(numRows, numCols, 0);
        TestwxSheetValueProvider_SetValue(numRows, numCols, &values);
    }
    {
        wxLogDebug(wxT("Start wxSheetValueProviderSparseString\n"));
        wxSheetValueProviderSparseString values(numRows, numCols, 0);
        TestwxSheetValueProvider_SetValue(numRows, numCols, &values);
    }
    {
        wxLogDebug(wxT("Start wxSheetValueProviderSparseStringTest\n"));
        wxSheetValueProviderSparseStringTest values(numRows, numCols, 0);
        TestwxSheetValueProvider_SetValue(numRows, numCols, &values);
    }

    {
        wxLogDebug(wxT("Start wxSheetValueProviderString col preference\n"));
        wxSheetValueProviderString values(numRows, numCols, wxSHEET_ValueProviderColPref);
        TestwxSheetValueProvider_SetValue(numRows, numCols, &values);
    }
    {
        wxLogDebug(wxT("Start wxSheetValueProviderSparseString col preference\n"));
        wxSheetValueProviderSparseString values(numRows, numCols, wxSHEET_ValueProviderColPref);
        TestwxSheetValueProvider_SetValue(numRows, numCols, &values);
    }
    {
        wxLogDebug(wxT("Start wxSheetValueProviderSparseStringTest col preference\n"));
        wxSheetValueProviderSparseStringTest values(numRows, numCols, wxSHEET_ValueProviderColPref);
        TestwxSheetValueProvider_SetValue(numRows, numCols, &values);
    }

/*
    {
        wxSheetValueProviderString values(numRows, numCols, 0);
        for (r = 0; r < numRows; r++)
        {
            for (c = 0; c < numCols; c++)
                values.SetValue(wxSheetCoords(r,c), wxT("Hello"));
        }
    }
*/

//

    wxLogDebug(wxT("TestwxSheetValueProvider Time %ld\n"), sw.Time()); fflush(stdout);
}

void TestFullRedrawSpeed(wxSheet* sheet, int count)
{
    wxRect rect(wxPoint(0,0), sheet->GetGridWindow()->GetSize());
    //wxRect rect(1, 1, 1, 1);
    wxStopWatch sw;
    wxPrintf(wxT("Start Full Redraw Test "));

    for (int n = 0; n < count; n++)
    {
        sheet->GetGridWindow()->Refresh(true); //, &rect);
        wxYield();
    }

    wxPrintf(wxT("Time %ld\n"), sw.Time()); fflush(stdout);
}

void TestMoveCursorSpeed(wxSheet* sheet)
{
    wxStopWatch sw;
    wxPrintf(wxT("Start Cursor Speed Test "));

    int rows = sheet->GetNumberRows()/2;
    int cols = sheet->GetNumberCols()/2;
    for (int r = 0; r < rows; r++)
    {
        sheet->SetGridCursorCell(wxSheetCoords(r, 0));
        wxYield();
        for (int c = 1; c < cols; c++)
        {
            sheet->MoveCursorRight(false);
            wxYield();
        }
    }

    wxPrintf(wxT("Time %ld\n"), sw.Time()); fflush(stdout);
}

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// GridApp
// ----------------------------------------------------------------------------

bool GridApp::OnInit()
{
    //_CrtSetBreakAlloc(2061);

    //TestwxSheetValueProvider();

    GridFrame *frame = new GridFrame;
    frame->SetSize(800, 600);
    frame->Show( true );

    wxYield(); // make sure it's all created

    //TestFullRedrawSpeed(frame->grid, 1000);
    //TestMoveCursorSpeed(frame->grid);

    return true;
}

// ----------------------------------------------------------------------------
// GridFrame
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE( GridFrame, wxFrame )
    EVT_MENU( ID_TOGGLEROWLABELS,  GridFrame::ToggleRowLabels )
    EVT_MENU( ID_TOGGLECOLLABELS,  GridFrame::ToggleColLabels )
    EVT_MENU( ID_TOGGLEEDIT, GridFrame::ToggleEditing )
    EVT_MENU( ID_TOGGLEROWSIZING, GridFrame::ToggleRowSizing )
    EVT_MENU( ID_TOGGLECOLSIZING, GridFrame::ToggleColSizing )
    EVT_MENU( ID_TOGGLEGRIDSIZING, GridFrame::ToggleGridSizing )
    EVT_MENU( ID_TOGGLEHORIZGRIDLINES, GridFrame::ToggleGridLines )
    EVT_MENU( ID_TOGGLEVERTGRIDLINES, GridFrame::ToggleGridLines )
    EVT_MENU( ID_HIDESELROWS, GridFrame::OnHideShowSelRowsCols )
    EVT_MENU( ID_HIDESELCOLS, GridFrame::OnHideShowSelRowsCols )
    EVT_MENU( ID_SHOWSELROWS, GridFrame::OnHideShowSelRowsCols )
    EVT_MENU( ID_SHOWSELCOLS, GridFrame::OnHideShowSelRowsCols )

    EVT_MENU( ID_AUTOSIZECOLS,          GridFrame::AutoSizeCols )
    EVT_MENU( ID_CELLOVERFLOW,          GridFrame::CellOverflow )
    EVT_MENU( ID_RESIZECELL,            GridFrame::ResizeCell )

    EVT_MENU( ID_SETLABELCOLOUR,        GridFrame::SetLabelColour )
    EVT_MENU( ID_SETLABELTEXTCOLOUR,    GridFrame::SetLabelTextColour )
    EVT_MENU( ID_SETLABEL_FONT,         GridFrame::SetLabelFont )
    EVT_MENU( ID_GRIDLINECOLOUR,        GridFrame::SetGridLineColour )
    EVT_MENU( ID_SET_CELL_FG_COLOUR,    GridFrame::SetCellFgColour )
    EVT_MENU( ID_SET_CELL_BG_COLOUR,    GridFrame::SetCellBgColour )

    EVT_MENU( ID_SCROLLBARS_MENU,           GridFrame::OnScrollbarsShown )
    EVT_MENU( ID_SCROLLBARS_AS_NEEDED,      GridFrame::OnScrollbarsShown )
    EVT_MENU( ID_SCROLLBARS_HORIZ_NEVER,    GridFrame::OnScrollbarsShown )
    EVT_MENU( ID_SCROLLBARS_VERT_NEVER,     GridFrame::OnScrollbarsShown )
    EVT_MENU( ID_SCROLLBARS_NEVER,          GridFrame::OnScrollbarsShown )
    EVT_MENU( ID_SCROLLBARS_HORIZ_ALWAYS,   GridFrame::OnScrollbarsShown )
    EVT_MENU( ID_SCROLLBARS_VERT_ALWAYS,    GridFrame::OnScrollbarsShown )
    EVT_MENU( ID_SCROLLBARS_ALWAYS,         GridFrame::OnScrollbarsShown )

    EVT_MENU( ID_ROWLABELHORIZALIGN,    GridFrame::SetRowLabelHorizAlignment )
    EVT_MENU( ID_ROWLABELVERTALIGN,     GridFrame::SetRowLabelVertAlignment )
    EVT_MENU( ID_COLLABELHORIZALIGN,    GridFrame::SetColLabelHorizAlignment )
    EVT_MENU( ID_COLLABELVERTALIGN,     GridFrame::SetColLabelVertAlignment )

    EVT_MENU( wxID_COPY,        GridFrame::OnCopyPaste )
    EVT_MENU( wxID_PASTE,       GridFrame::OnCopyPaste )
    EVT_MENU( ID_INSERTROW,     GridFrame::InsertRow )
    EVT_MENU( ID_INSERTCOL,     GridFrame::InsertCol )
    EVT_MENU( ID_DELETEROW,     GridFrame::DeleteSelectedRows )
    EVT_MENU( ID_DELETECOL,     GridFrame::DeleteSelectedCols )
    EVT_MENU( ID_CLEARGRID,     GridFrame::ClearGrid )
    EVT_MENU( ID_ENABLEGRID,    GridFrame::EnableGrid )

    EVT_MENU( ID_SELNONE,   GridFrame::SelectionMode )
    EVT_MENU( ID_SELCELLS,  GridFrame::SelectionMode )
    EVT_MENU( ID_SELROWS,   GridFrame::SelectionMode )
    EVT_MENU( ID_SELCOLS,   GridFrame::SelectionMode )
    EVT_MENU( ID_SELSINGLE, GridFrame::SelectionMode )

    EVT_MENU( ID_ABOUT,         GridFrame::About )
    EVT_MENU( wxID_EXIT,        GridFrame::OnQuit )
    EVT_MENU( ID_VTABLE,        GridFrame::OnVTable)
    EVT_MENU( ID_BUGS_TABLE,    GridFrame::OnBugsTable)
    EVT_MENU( ID_SMALL_GRID,    GridFrame::OnSmallGrid)

    EVT_MENU( ID_DESELECT_CELL,     GridFrame::DeselectCell)
    EVT_MENU( ID_DESELECT_COL,      GridFrame::DeselectCol)
    EVT_MENU( ID_DESELECT_ROW,      GridFrame::DeselectRow)
    EVT_MENU( ID_DESELECT_ALL,      GridFrame::DeselectAll)
    EVT_MENU( ID_SELECT_CELL,       GridFrame::SelectCell)
    EVT_MENU( ID_SELECT_COL,        GridFrame::SelectCol)
    EVT_MENU( ID_SELECT_ROW,        GridFrame::SelectRow)
    EVT_MENU( ID_SELECT_ALL,        GridFrame::SelectAll)
    EVT_MENU( ID_SELECT_UNSELECT,   GridFrame::OnAddToSelectToggle)

    EVT_MENU( ID_SET_HIGHLIGHT_WIDTH,    GridFrame::OnSetHighlightWidth)
    EVT_MENU( ID_SET_RO_HIGHLIGHT_WIDTH, GridFrame::OnSetROHighlightWidth)

    EVT_SHEET_LABEL_LEFT_DOWN( -1, GridFrame::OnLabelLeftDown )
    EVT_SHEET_CELL_LEFT_DOWN( -1, GridFrame::OnCellLeftDown )
    EVT_SHEET_CELL_RIGHT_DOWN( -1, GridFrame::OnCellRightDown )
    EVT_SHEET_ROW_SIZE(   -1, GridFrame::OnRowSize )
    EVT_SHEET_ROW_SIZING( -1, GridFrame::OnRowSize )
    EVT_SHEET_ROW_SIZED(  -1, GridFrame::OnRowSize )
    EVT_SHEET_COL_SIZE(   -1, GridFrame::OnColSize )
    EVT_SHEET_COL_SIZING( -1, GridFrame::OnColSize )
    EVT_SHEET_COL_SIZED(  -1, GridFrame::OnColSize )
    EVT_SHEET_SELECTED_CELL( -1, GridFrame::OnSelectCell )
    EVT_SHEET_RANGE_SELECTING( -1, GridFrame::OnRangeSelect )
    EVT_SHEET_RANGE_SELECTED( -1, GridFrame::OnRangeSelect )

    EVT_SHEET_CELL_VALUE_CHANGING( -1, GridFrame::OnCellValueChanged )
    EVT_SHEET_CELL_VALUE_CHANGED( -1, GridFrame::OnCellValueChanged )

    EVT_SHEET_EDITOR_ENABLED( -1, GridFrame::OnEditorShown )
    EVT_SHEET_EDITOR_DISABLED( -1, GridFrame::OnEditorHidden )

    EVT_SHEET_SPLIT_BEGIN(         -1, GridFrame::OnSheetSplitter )
    EVT_SHEET_SPLIT_CHANGING(      -1, GridFrame::OnSheetSplitter )
    EVT_SHEET_SPLIT_CHANGED(       -1, GridFrame::OnSheetSplitter )
    EVT_SHEET_SPLIT_DOUBLECLICKED( -1, GridFrame::OnSheetSplitter )
    EVT_SHEET_SPLIT_UNSPLIT(       -1, GridFrame::OnSheetSplitter )
    EVT_SHEET_SPLIT_CREATE_SHEET(  -1, GridFrame::OnSheetSplitter )
END_EVENT_TABLE()


GridFrame::GridFrame()
          : wxFrame( (wxFrame *)NULL, -1, _T("wxWidgets grid class demo"),
                     wxDefaultPosition, wxDefaultSize )
{
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append( ID_VTABLE, _T("&Virtual table test\tCtrl-A"));
    fileMenu->Append( ID_BUGS_TABLE, _T("&Bugs table test\tCtrl-B"));
    fileMenu->Append( ID_SMALL_GRID, _T("&Small Grid test\tCtrl-S"));
    fileMenu->AppendSeparator();
    fileMenu->Append( wxID_EXIT, _T("E&xit\tAlt-X") );

    wxMenu *viewMenu = new wxMenu;
    viewMenu->Append( ID_TOGGLEROWLABELS,  _T("&Row labels"), _T(""), wxITEM_CHECK );
    viewMenu->Append( ID_TOGGLECOLLABELS,  _T("&Col labels"), _T(""), wxITEM_CHECK );
    viewMenu->Append( ID_TOGGLEEDIT,  _T("&Editable"), _T(""), wxITEM_CHECK );
    viewMenu->Append( ID_TOGGLEROWSIZING, _T("Ro&w drag-resize"), _T(""), wxITEM_CHECK );
    viewMenu->Append( ID_TOGGLECOLSIZING, _T("C&ol drag-resize"), _T(""), wxITEM_CHECK );
    viewMenu->Append( ID_TOGGLEROWSIZINGVETO, _T("Row drag-resize event veto"), _T(""), wxITEM_CHECK );
    viewMenu->Append( ID_TOGGLECOLSIZINGVETO, _T("Col drag-resize event veto"), _T(""), wxITEM_CHECK );
    viewMenu->Append( ID_TOGGLEGRIDSIZING, _T("&Grid drag-resize"), _T(""), wxITEM_CHECK );
    viewMenu->Append( ID_TOGGLEHORIZGRIDLINES, _T("&Horiz Grid Lines"), _T(""), wxITEM_CHECK );
    viewMenu->Append( ID_TOGGLEVERTGRIDLINES, _T("&Vert Grid Lines"), _T(""), wxITEM_CHECK );

    viewMenu->Append( ID_HIDESELROWS, _T("Hide selected rows") );
    viewMenu->Append( ID_HIDESELCOLS, _T("Hide selected cols") );
    viewMenu->Append( ID_SHOWSELROWS, _T("Show selected rows") );
    viewMenu->Append( ID_SHOWSELCOLS, _T("Show selected cols") );

    viewMenu->Append( ID_SET_HIGHLIGHT_WIDTH, _T("Set Cell H&ighlight Width..."), _T("") );
    viewMenu->Append( ID_SET_RO_HIGHLIGHT_WIDTH, _T("Set Cell RO Highlight Width..."), _T("") );
    viewMenu->Append( ID_AUTOSIZECOLS, _T("&Auto-size cols") );
    viewMenu->Append( ID_CELLOVERFLOW, _T("Over&flow cells"), _T(""), wxITEM_CHECK );
    viewMenu->Append( ID_RESIZECELL, _T("Resi&ze cell (7,1)"), _T(""), wxITEM_CHECK );

    wxMenu *scrollbarMenu = new wxMenu;
    viewMenu->Append( ID_SCROLLBARS_MENU, _T("Show or hide scrollbars"),
                      scrollbarMenu,
                      _T("Show or hide the scrollbars") );

    scrollbarMenu->AppendRadioItem( ID_SCROLLBARS_AS_NEEDED, _T("As needed") );
    scrollbarMenu->AppendRadioItem( ID_SCROLLBARS_HORIZ_NEVER, _T("Never show horizontal") );
    scrollbarMenu->AppendRadioItem( ID_SCROLLBARS_VERT_NEVER, _T("Never show vertical") );
    scrollbarMenu->AppendRadioItem( ID_SCROLLBARS_NEVER, _T("Never show either scrollbar") );
    scrollbarMenu->AppendRadioItem( ID_SCROLLBARS_HORIZ_ALWAYS, _T("Always show horiz scrollbar") );
    scrollbarMenu->AppendRadioItem( ID_SCROLLBARS_VERT_ALWAYS, _T("Always show vert scrollbar") );
    scrollbarMenu->AppendRadioItem( ID_SCROLLBARS_ALWAYS, _T("Always show both scrollbars") );

    wxMenu *rowLabelMenu = new wxMenu;

    viewMenu->Append( ID_ROWLABELALIGN, _T("R&ow label alignment"),
                      rowLabelMenu,
                      _T("Change alignment of row labels") );

    rowLabelMenu->Append( ID_ROWLABELHORIZALIGN, _T("&Horizontal") );
    rowLabelMenu->Append( ID_ROWLABELVERTALIGN, _T("&Vertical") );

    wxMenu *colLabelMenu = new wxMenu;

    viewMenu->Append( ID_COLLABELALIGN, _T("Col l&abel alignment"),
                      colLabelMenu,
                      _T("Change alignment of col labels") );

    colLabelMenu->Append( ID_COLLABELHORIZALIGN, _T("&Horizontal") );
    colLabelMenu->Append( ID_COLLABELVERTALIGN, _T("&Vertical") );

    wxMenu *colMenu = new wxMenu;
    colMenu->Append( ID_SETLABELCOLOUR,     _T("Set row &label colour...") );
    colMenu->Append( ID_SETLABELTEXTCOLOUR, _T("Set row label &text colour...") );
    colMenu->Append( ID_SETLABEL_FONT,      _T("Set row label fo&nt...") );
    colMenu->Append( ID_GRIDLINECOLOUR,     _T("Set &Grid line colour...") );
    colMenu->Append( ID_SET_CELL_FG_COLOUR, _T("Set default cell &foreground colour...") );
    colMenu->Append( ID_SET_CELL_BG_COLOUR, _T("Set default cell &background colour...") );

    wxMenu *editMenu = new wxMenu;
    editMenu->Append( wxID_COPY, _T("&Copy\tCtrl-C") );
    editMenu->Append( wxID_PASTE, _T("&Paste\tCtrl-V") );
    editMenu->Append( ID_INSERTROW, _T("Insert &row") );
    editMenu->Append( ID_INSERTCOL, _T("Insert c&olumn") );
    editMenu->Append( ID_DELETEROW, _T("Delete selected ro&ws") );
    editMenu->Append( ID_DELETECOL, _T("Delete selected co&ls") );
    editMenu->Append( ID_CLEARGRID, _T("Cl&ear grid cell contents") );
    editMenu->AppendCheckItem( ID_ENABLEGRID, _T("E&nable grid") );
    editMenu->Check(ID_ENABLEGRID, true);

    wxMenu *selectMenu = new wxMenu;
    selectMenu->Append( ID_SELECT_UNSELECT, _T("Add new cells to the selection"),
                        _T("When off, old selection is deselected before ")
                        _T("selecting the new cells"), wxITEM_CHECK );
    selectMenu->Append( ID_SELECT_ALL, _T("Select all"));
    selectMenu->Append( ID_SELECT_ROW, _T("Select row 2"));
    selectMenu->Append( ID_SELECT_COL, _T("Select col 2"));
    selectMenu->Append( ID_SELECT_CELL, _T("Select cell (3, 1)"));
    selectMenu->Append( ID_DESELECT_ALL, _T("Deselect all"));
    selectMenu->Append( ID_DESELECT_ROW, _T("Deselect row 2"));
    selectMenu->Append( ID_DESELECT_COL, _T("Deselect col 2"));
    selectMenu->Append( ID_DESELECT_CELL, _T("Deselect cell (3, 1)"));
    wxMenu *selectionMenu = new wxMenu;
    selectMenu->Append( ID_CHANGESEL, _T("Change &selection mode"),
                      selectionMenu,
                      _T("Change selection mode") );

    selectionMenu->AppendRadioItem( ID_SELNONE,   _T("Select &None") );
    selectionMenu->AppendRadioItem( ID_SELCELLS,  _T("Select &Cells") );
    selectionMenu->AppendRadioItem( ID_SELROWS,   _T("Select &Rows") );
    selectionMenu->AppendRadioItem( ID_SELCOLS,   _T("Select C&ols") );
    selectionMenu->AppendRadioItem( ID_SELSINGLE, _T("Select &Single Cell") );
    selectionMenu->Check(ID_SELCELLS, true);

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append( ID_ABOUT, _T("&About wxSheet demo") );

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( fileMenu, _T("&File") );
    menuBar->Append( viewMenu, _T("&View") );
    menuBar->Append( colMenu,  _T("&Colours") );
    menuBar->Append( editMenu, _T("&Edit") );
    menuBar->Append( selectMenu, _T("&Select") );
    menuBar->Append( helpMenu, _T("&Help") );

    SetMenuBar( menuBar );

    m_addToSel = false;

    wxSplitterWindow *splitter = new wxSplitterWindow(this, -1);
    splitter->SetMinimumPaneSize(20);
    splitter->SetSashGravity(0.9);

    // to use the splitter simply create one, then your sheet as a child
    // and initialize the splitter with the sheet.
    wxSheetSplitter* sheetSplitter = new wxSheetSplitter(splitter, -1,
                                              wxPoint(0, 0), wxDefaultSize);

    grid = new wxSheet( sheetSplitter,
                       -1,
                       wxPoint( 0, 0 ),
                       wxDefaultSize );

    sheetSplitter->Initialize(grid);

    logWin = new wxTextCtrl( splitter,
                             -1,
                             wxEmptyString,
                             wxDefaultPosition,
                             wxDefaultSize,
                             wxTE_MULTILINE );

    logger = new wxLogTextCtrl( logWin );
    m_logOld = logger->SetActiveTarget( logger );
    //logger->SetTimestamp( NULL );

/*
    int nrows = 200;
    int ncols = 500;

    grid->CreateGrid( nrows, ncols );
    grid->BeginBatch();
        //((wxSheetSelection *)grid->GetSpannedBlocks())->SetOptions(wxSHEET_SELECTION_MULTIPLE_SEL);
        int j = 0;
        for (int i = 0; i < nrows/10; i++) {
                j = (i % 2 == 0) ? 1 : 0;

                for (; j < 450; j=j+3+1) {
                        grid->SetCellSpan( wxSheetCoords(i, j), wxSheetCoords(1, 3) );
                        grid->SetCellValue( wxSheetCoords(i, j), _T("Merged") );
                }
                //((wxSheetSelection *)grid->GetSpannedBlocks())->Minimize();
               wxPrintf(wxT("Spanning %d count %d\n"), i, grid->GetSpannedBlocks()->GetCount()); fflush(stdout);
        }
    grid->EndBatch();
*/

    // this will create a grid and, by default, an associated grid
    // table for strings
    grid->CreateGrid( 0, 0 );
    grid->AppendRows(100);
    grid->AppendCols(100);

    int ir = grid->GetNumberRows();
    grid->DeleteRows(0, ir);
    grid->AppendRows(ir);

    grid->SetRowHeight( 0, 60 );
    grid->SetCellValue( wxSheetCoords(0, 0), _T("Ctrl+Home\nwill go to\nthis cell") );

    grid->SetCellValue( wxSheetCoords(0, 1), _T("A long piece of text to demonstrate wrapping.") );
    grid->SetAttrRenderer( wxSheetCoords(0, 1), wxSheetCellRenderer(new wxSheetCellAutoWrapStringRendererRefData()));
    grid->SetAttrEditor( wxSheetCoords(0, 1), wxSheetCellEditor(new wxSheetCellAutoWrapStringEditorRefData()));

    grid->SetCellValue( wxSheetCoords(0, 2), _T("Edit change vetoed by event") );
    grid->SetCellValue( wxSheetCoords(0, 3), _T("Read only") );
    grid->SetAttrReadOnly( wxSheetCoords(0, 3), true );

    grid->SetCellValue( wxSheetCoords(0, 4), _T("Can veto edit this cell") );

    grid->SetCellValue( wxSheetCoords(0, 5), _T("\nPress\nCtrl+arrow\nto skip over\ncells") );

    grid->SetCellValue( wxSheetCoords(0, 8), _T("Rotated") );
    grid->SetAttrOrientation( wxSheetCoords(0, 8), wxSHEET_AttrOrientVert );

    grid->SetRowHeight( 99, 60 );
    grid->SetCellValue( wxSheetCoords(99, 99), _T("Ctrl+End\nwill go to\nthis cell") );
    grid->SetCellValue( wxSheetCoords(1, 0), _T("This default cell will overflow into neighboring cells, but not if you turn overflow off."));

    grid->SetAttrForegroundColour( wxSheetCoords(1, 2), *wxRED);
    grid->SetAttrBackgroundColour( wxSheetCoords(1, 2), *wxGREEN);

    grid->SetCellValue( wxSheetCoords(1, 4), _T("I'm in the middle"));

    grid->SetCellValue( wxSheetCoords(2, 2), _T("red"));

    grid->SetAttrForegroundColour( wxSheetCoords(2, 2), *wxRED);
    grid->SetCellValue( wxSheetCoords(3, 3), _T("green on grey"));
    grid->SetAttrForegroundColour( wxSheetCoords(3, 3), *wxGREEN);
    grid->SetAttrBackgroundColour( wxSheetCoords(3, 3), *wxLIGHT_GREY);

    grid->SetCellValue( wxSheetCoords(4, 4), _T("a weird looking cell"));
    grid->SetAttrAlignment( wxSheetCoords(4, 4), wxSHEET_AttrAlignCenter);
    grid->SetAttrRenderer( wxSheetCoords(4, 4), wxSheetCellRenderer(new MyGridCellRendererRefData()));

    grid->SetCellValue( wxSheetCoords(3, 0), _T("0"));
    grid->SetAttrRenderer( wxSheetCoords(3, 0), wxSheetCellRenderer(new wxSheetCellBoolRendererRefData()));
    grid->SetAttrEditor( wxSheetCoords(3, 0), wxSheetCellEditor(new wxSheetCellBoolEditorRefData()));

    wxSheetCellAttr attr(true);
    attr.SetForegroundColour(*wxBLUE);
    grid->SetGridColAttr(5, attr);
    attr = wxSheetCellAttr(true);
    attr.SetBackgroundColour(*wxRED);
    grid->SetGridRowAttr(5, attr);

    grid->SetCellValue( wxSheetCoords(2, 4), _T("a wider column"));
    grid->SetColWidth(4, 120);
    grid->SetMinimalColWidth(4, 120);

    grid->SetAttrForegroundColour( wxSheetCoords(5, 8), *wxGREEN);
    grid->SetCellValue( wxSheetCoords(5, 8), _T("Bg from row attr\nText colour from cell attr"));
    grid->SetCellValue( wxSheetCoords(5, 5), _T("Bg from row attr Text col from col attr and this text is so long that it covers over many many empty cells but is broken by one that isn't"));

    grid->SetColFormatFloat(6);
    grid->SetCellValue( wxSheetCoords(0, 6), _T("3.1415"));
    grid->SetCellValue( wxSheetCoords(1, 6), _T("1415"));
    grid->SetCellValue( wxSheetCoords(2, 6), _T("12345.67890"));

    grid->SetAttrEditor( wxSheetCoords(0, 6), wxSheetCellEditor(new wxSheetCellFloatEditorRefData(-1, -1)) );

    grid->SetColFormatFloat(7, 6, 2);
    grid->SetCellValue( wxSheetCoords(0, 7), _T("3.1415"));
    grid->SetCellValue( wxSheetCoords(1, 7), _T("1415"));
    grid->SetCellValue( wxSheetCoords(2, 7), _T("12345.67890"));

    const wxString choices[] =
    {
        _T("Please select a choice"),
        _T("This takes two cells"),
        _T("Another choice"),
    };
    grid->SetAttrEditor( wxSheetCoords(4, 0), wxSheetCellEditor(new wxSheetCellChoiceEditorRefData(WXSIZEOF(choices), choices)));
    grid->SetCellSpan( wxSheetCoords(4, 0), wxSheetCoords(1, 2));
    grid->SetCellValue( wxSheetCoords(4, 0), choices[0]);
    grid->SetAttrOverflow( wxSheetCoords(4, 0), false);

    grid->SetCellSpan( wxSheetCoords(7, 1), wxSheetCoords(3, 4));
    grid->SetAttrAlignment( wxSheetCoords(7, 1), wxSHEET_AttrAlignCenter);
    grid->SetCellValue( wxSheetCoords(7, 1), _T("Big box!"));

    grid->SetCellSpan( wxSheetCoords(26, 1), wxSheetCoords(3, 2));
    grid->SetCellSpan( wxSheetCoords(26, 4), wxSheetCoords(3, 2));
    grid->SetCellSpan( wxSheetCoords(26, 7), wxSheetCoords(3, 2));

    // ---------------------------------------------------------
    // Extra wxSheet tests

    grid->SetCellSpan(wxSheetCoords(-1, 1), wxSheetCoords(1, 2));
    grid->SetCellSpan(wxSheetCoords(3, -1), wxSheetCoords(2, 1));

    // By default labels are read-only, allow editing
    //  note: shown below are 3 ways to do the same thing, the SetAttrReadOnly
    //        is the preferred one.
    grid->GetDefaultRowLabelAttr().SetReadOnly(false);
    grid->GetAttr(wxColLabelSheetCoords, wxSHEET_AttrDefault).SetReadOnly(false);
    grid->SetAttrReadOnly(wxCornerLabelSheetCoords, false, wxSHEET_AttrDefault);

    // set arbitrary attr values, colours in this case for labels
    grid->SetAttrBackgroundColour(wxSheetCoords(5, -1), *wxGREEN);
    grid->SetAttrBackgroundColour(wxSheetCoords(-1, 5), *wxGREEN);
    grid->SetCornerLabelValue(wxT("Hello"));

    // can set new renderers/editor this way
    grid->SetCellValue(wxSheetCoords(0,-1), wxT("Autowrap\nString\nEdit/Ren"));
    grid->SetAttrEditor(wxSheetCoords(0,-1), wxSheetCellEditor(new wxSheetCellAutoWrapStringEditorRefData()));
    grid->SetAttrRenderer(wxSheetCoords(0,-1), wxSheetCellRenderer(new wxSheetCellAutoWrapStringRendererRefData()));

    // or use the get ren/edit for typename and ref prexisting ones
    grid->SetCellValue(wxSheetCoords(2,-1), wxT("String Ren"));
    grid->SetAttrRenderer(wxSheetCoords(2,-1), grid->GetDefaultRendererForType(wxSHEET_VALUE_STRING));

    grid->SetCellValue(wxSheetCoords(5, 2), wxT("Label Ren"));
    grid->SetAttrRenderer(wxSheetCoords(5, 2), grid->GetDefaultRendererForType(wxSHEET_VALUE_LABEL));

    grid->SetAttrEditor(wxSheetCoords(-1, 3), grid->GetDefaultEditorForType(wxSHEET_VALUE_BOOL));
    grid->SetAttrRenderer(wxSheetCoords(-1, 3), grid->GetDefaultRendererForType(wxSHEET_VALUE_BOOL));

    grid->SetCellValue(wxSheetCoords(-1, 4), wxT("Choice Edit"));
    grid->SetAttrEditor(wxSheetCoords(-1, 4), wxSheetCellEditor(new wxSheetCellChoiceEditorRefData(WXSIZEOF(choices), choices)));

    // FIXME - what to do about sizing this control?
    grid->SetCellValue(wxSheetCoords(6, 1), wxT("Choice Edit"));
    grid->SetAttrEditor(wxSheetCoords(6, 1), grid->GetAttrEditor(wxSheetCoords(-1, 4)) );

    // bitmap renderer test
    //wxBitmap bmp(wxArtProvider::GetBitmap(wxART_FOLDER));
    wxBitmap bmp(16, 16);
    {
        wxMemoryDC memDC;           // draw a clean bitmap filling size
        memDC.SelectObject(bmp);
        memDC.SetBrush(*wxBLUE_BRUSH);
        memDC.SetPen(*wxBLACK_PEN);
        memDC.DrawRectangle(0, 0, 16, 16);
        memDC.SelectObject(wxNullBitmap);
    }

    grid->SetCellValue(wxSheetCoords(10, 0), wxT("The below 3x3 grid tests the wxSheetCellBitmapRendererRefData, the text fills free space"));

    wxSheetCellRenderer bmpRenderer(new wxSheetCellBitmapRendererRefData(bmp, wxSHEET_AttrAlignTop|wxSHEET_AttrAlignLeft|wxSHEET_BMPREN_BMPLEFT));
      grid->SetAttrRenderer(wxSheetCoords(11, 0), bmpRenderer);
      grid->SetAttrAlignment(wxSheetCoords(11, 0), wxSHEET_AttrAlignTop|wxSHEET_AttrAlignLeft);
      grid->SetCellValue(wxSheetCoords(11, 0), wxT("bmpTL, txtTL"));
    bmpRenderer= wxSheetCellRenderer(new wxSheetCellBitmapRendererRefData(bmp, wxSHEET_AttrAlignTop|wxSHEET_AttrAlignCenterHoriz|wxSHEET_BMPREN_BMPABOVE));
      grid->SetAttrRenderer(wxSheetCoords(11, 1), bmpRenderer);
      grid->SetAttrAlignment(wxSheetCoords(11, 1), wxSHEET_AttrAlignTop|wxSHEET_AttrAlignCenterHoriz);
      grid->SetCellValue(wxSheetCoords(11, 1), wxT("bmpTC, txtTC"));
    bmpRenderer= wxSheetCellRenderer(new wxSheetCellBitmapRendererRefData(bmp, wxSHEET_AttrAlignTop|wxSHEET_AttrAlignRight|wxSHEET_BMPREN_BMPRIGHT));
      grid->SetAttrRenderer(wxSheetCoords(11, 2), bmpRenderer);
      grid->SetAttrAlignment(wxSheetCoords(11, 2), wxSHEET_AttrAlignTop|wxSHEET_AttrAlignRight);
      grid->SetCellValue(wxSheetCoords(11, 2), wxT("bmpTR, txtTR"));

    bmpRenderer = wxSheetCellRenderer(new wxSheetCellBitmapRendererRefData(bmp, wxSHEET_AttrAlignCenterVert|wxSHEET_AttrAlignLeft|wxSHEET_BMPREN_BMPLEFT));
      grid->SetAttrRenderer(wxSheetCoords(12, 0), bmpRenderer);
      grid->SetAttrAlignment(wxSheetCoords(12, 0), wxSHEET_AttrAlignCenterVert|wxSHEET_AttrAlignCenterHoriz);
      grid->SetCellValue(wxSheetCoords(12, 0), wxT("bmpCL, txtCC"));
    bmpRenderer= wxSheetCellRenderer(new wxSheetCellBitmapRendererRefData(bmp, wxSHEET_AttrAlignCenterVert|wxSHEET_AttrAlignCenterHoriz));
      grid->SetAttrRenderer(wxSheetCoords(12, 1), bmpRenderer);
      grid->SetAttrAlignment(wxSheetCoords(12, 1), wxSHEET_AttrAlignCenterVert|wxSHEET_AttrAlignCenterHoriz);
      grid->SetCellValue(wxSheetCoords(12, 1), wxT("bmpCC, txtCC"));
    bmpRenderer= wxSheetCellRenderer(new wxSheetCellBitmapRendererRefData(bmp, wxSHEET_AttrAlignCenterVert|wxSHEET_AttrAlignRight|wxSHEET_BMPREN_BMPRIGHT));
      grid->SetAttrRenderer(wxSheetCoords(12, 2), bmpRenderer);
      grid->SetAttrAlignment(wxSheetCoords(12, 2), wxSHEET_AttrAlignCenterVert|wxSHEET_AttrAlignCenterHoriz);
      grid->SetCellValue(wxSheetCoords(12, 2), wxT("bmpCR, txtCC"));

    bmpRenderer = wxSheetCellRenderer(new wxSheetCellBitmapRendererRefData(bmp, wxSHEET_AttrAlignBottom|wxSHEET_AttrAlignLeft|wxSHEET_BMPREN_BMPLEFT));
      grid->SetAttrRenderer(wxSheetCoords(13, 0), bmpRenderer);
      grid->SetAttrAlignment(wxSheetCoords(13, 0), wxSHEET_AttrAlignBottom|wxSHEET_AttrAlignRight);
      grid->SetCellValue(wxSheetCoords(13, 0), wxT("bmpBL, txtBR"));
    bmpRenderer= wxSheetCellRenderer(new wxSheetCellBitmapRendererRefData(bmp, wxSHEET_AttrAlignBottom|wxSHEET_AttrAlignCenterHoriz|wxSHEET_BMPREN_BMPBELOW));
      grid->SetAttrRenderer(wxSheetCoords(13, 1), bmpRenderer);
      grid->SetAttrAlignment(wxSheetCoords(13, 1), wxSHEET_AttrAlignBottom|wxSHEET_AttrAlignCenterHoriz);
      grid->SetCellValue(wxSheetCoords(13, 1), wxT("bmpBC, txtBC"));
    bmpRenderer= wxSheetCellRenderer(new wxSheetCellBitmapRendererRefData(bmp, wxSHEET_AttrAlignBottom|wxSHEET_AttrAlignRight|wxSHEET_BMPREN_BMPRIGHT));
      grid->SetAttrRenderer(wxSheetCoords(13, 2), bmpRenderer);
      grid->SetAttrAlignment(wxSheetCoords(13, 2), wxSHEET_AttrAlignBottom|wxSHEET_AttrAlignLeft);
      grid->SetCellValue(wxSheetCoords(13, 2), wxT("bmpBR, txtBL"));

    grid->SetCellValue(wxSheetCoords(14, 0), wxT("The below 3x3 grid tests the wxSheetCellBitmapRendererRefData, the bitmap fills free space"));

    bmpRenderer= wxSheetCellRenderer(new wxSheetCellBitmapRendererRefData(bmp, wxSHEET_AttrAlignTop|wxSHEET_AttrAlignLeft|wxSHEET_BMPREN_BMPLEFT|wxSHEET_BMPREN_BMPEXPAND));
      grid->SetAttrRenderer(wxSheetCoords(15, 0), bmpRenderer);
      grid->SetAttrAlignment(wxSheetCoords(15, 0), wxSHEET_AttrAlignTop|wxSHEET_AttrAlignLeft);
      grid->SetCellValue(wxSheetCoords(15, 0), wxT("bmpTL, txtTL"));
    bmpRenderer= wxSheetCellRenderer(new wxSheetCellBitmapRendererRefData(bmp, wxSHEET_AttrAlignTop|wxSHEET_AttrAlignCenterHoriz|wxSHEET_BMPREN_BMPABOVE|wxSHEET_BMPREN_BMPEXPAND));
      grid->SetAttrRenderer(wxSheetCoords(15, 1), bmpRenderer);
      grid->SetAttrAlignment(wxSheetCoords(15, 1), wxSHEET_AttrAlignTop|wxSHEET_AttrAlignCenterHoriz);
      grid->SetCellValue(wxSheetCoords(15, 1), wxT("bmpTC, txtTC"));
    bmpRenderer= wxSheetCellRenderer(new wxSheetCellBitmapRendererRefData(bmp, wxSHEET_AttrAlignTop|wxSHEET_AttrAlignRight|wxSHEET_BMPREN_BMPRIGHT|wxSHEET_BMPREN_BMPEXPAND));
      grid->SetAttrRenderer(wxSheetCoords(15, 2), bmpRenderer);
      grid->SetAttrAlignment(wxSheetCoords(15, 2), wxSHEET_AttrAlignTop|wxSHEET_AttrAlignRight);
      grid->SetCellValue(wxSheetCoords(15, 2), wxT("bmpTR, txtTR"));

    bmpRenderer = wxSheetCellRenderer(new wxSheetCellBitmapRendererRefData(bmp, wxSHEET_AttrAlignCenterVert|wxSHEET_AttrAlignLeft|wxSHEET_BMPREN_BMPLEFT|wxSHEET_BMPREN_BMPEXPAND));
      grid->SetAttrRenderer(wxSheetCoords(16, 0), bmpRenderer);
      grid->SetAttrAlignment(wxSheetCoords(16, 0), wxSHEET_AttrAlignCenterVert|wxSHEET_AttrAlignCenterHoriz);
      grid->SetCellValue(wxSheetCoords(16, 0), wxT("bmpCL, txtCC"));
    bmpRenderer= wxSheetCellRenderer(new wxSheetCellBitmapRendererRefData(bmp, wxSHEET_AttrAlignCenterVert|wxSHEET_AttrAlignCenterHoriz|wxSHEET_BMPREN_BMPEXPAND));
      grid->SetAttrRenderer(wxSheetCoords(16, 1), bmpRenderer);
      grid->SetAttrAlignment(wxSheetCoords(16, 1), wxSHEET_AttrAlignCenterVert|wxSHEET_AttrAlignCenterHoriz);
      grid->SetCellValue(wxSheetCoords(16, 1), wxT("bmpCC, txtCC"));
    bmpRenderer= wxSheetCellRenderer(new wxSheetCellBitmapRendererRefData(bmp, wxSHEET_AttrAlignCenterVert|wxSHEET_AttrAlignRight|wxSHEET_BMPREN_BMPRIGHT|wxSHEET_BMPREN_BMPEXPAND));
      grid->SetAttrRenderer(wxSheetCoords(16, 2), bmpRenderer);
      grid->SetAttrAlignment(wxSheetCoords(16, 2), wxSHEET_AttrAlignCenterVert|wxSHEET_AttrAlignCenterHoriz);
      grid->SetCellValue(wxSheetCoords(16, 2), wxT("bmpCR, txtCC"));

    bmpRenderer = wxSheetCellRenderer(new wxSheetCellBitmapRendererRefData(bmp, wxSHEET_AttrAlignBottom|wxSHEET_AttrAlignLeft|wxSHEET_BMPREN_BMPLEFT|wxSHEET_BMPREN_BMPEXPAND));
      grid->SetAttrRenderer(wxSheetCoords(17, 0), bmpRenderer);
      grid->SetAttrAlignment(wxSheetCoords(17, 0), wxSHEET_AttrAlignBottom|wxSHEET_AttrAlignRight);
      grid->SetCellValue(wxSheetCoords(17, 0), wxT("bmpBL, txtBR"));
    bmpRenderer= wxSheetCellRenderer(new wxSheetCellBitmapRendererRefData(bmp, wxSHEET_AttrAlignBottom|wxSHEET_AttrAlignCenterHoriz|wxSHEET_BMPREN_BMPBELOW|wxSHEET_BMPREN_BMPEXPAND));
      grid->SetAttrRenderer(wxSheetCoords(17, 1), bmpRenderer);
      grid->SetAttrAlignment(wxSheetCoords(17, 1), wxSHEET_AttrAlignBottom|wxSHEET_AttrAlignCenterHoriz);
      grid->SetCellValue(wxSheetCoords(17, 1), wxT("bmpBC, txtBC"));
    bmpRenderer= wxSheetCellRenderer(new wxSheetCellBitmapRendererRefData(bmp, wxSHEET_AttrAlignBottom|wxSHEET_AttrAlignRight|wxSHEET_BMPREN_BMPRIGHT|wxSHEET_BMPREN_BMPEXPAND));
      grid->SetAttrRenderer(wxSheetCoords(17, 2), bmpRenderer);
      grid->SetAttrAlignment(wxSheetCoords(17, 2), wxSHEET_AttrAlignBottom|wxSHEET_AttrAlignLeft);
      grid->SetCellValue(wxSheetCoords(17, 2), wxT("bmpBR, txtBL"));

    // text alignment tests, a 3x3 grid
    grid->SetCellValue(wxSheetCoords(10, 4), wxT("The below 3x3 grid tests text alignment"));

    grid->SetCellValue(wxSheetCoords(11, 4), wxT("TopLeft"));
    grid->SetCellValue(wxSheetCoords(11, 5), wxT("TopCenter"));
    grid->SetCellValue(wxSheetCoords(11, 6), wxT("TopRight"));
    grid->SetCellValue(wxSheetCoords(12, 4), wxT("CenterLeft"));
    grid->SetCellValue(wxSheetCoords(12, 5), wxT("CenterCenter"));
    grid->SetCellValue(wxSheetCoords(12, 6), wxT("CenterRight"));
    grid->SetCellValue(wxSheetCoords(13, 4), wxT("BottomLeft"));
    grid->SetCellValue(wxSheetCoords(13, 5), wxT("BottomCenter"));
    grid->SetCellValue(wxSheetCoords(13, 6), wxT("BottomRight"));

    grid->SetAttrAlignment(wxSheetCoords(11, 4), wxSHEET_AttrAlignTop|wxSHEET_AttrAlignLeft);
    grid->SetAttrAlignment(wxSheetCoords(11, 5), wxSHEET_AttrAlignTop|wxSHEET_AttrAlignCenterHoriz);
    grid->SetAttrAlignment(wxSheetCoords(11, 6), wxSHEET_AttrAlignTop|wxSHEET_AttrAlignRight);
    grid->SetAttrAlignment(wxSheetCoords(12, 4), wxSHEET_AttrAlignCenterVert|wxSHEET_AttrAlignLeft);
    grid->SetAttrAlignment(wxSheetCoords(12, 5), wxSHEET_AttrAlignCenterVert|wxSHEET_AttrAlignCenterHoriz);
    grid->SetAttrAlignment(wxSheetCoords(12, 6), wxSHEET_AttrAlignCenterVert|wxSHEET_AttrAlignRight);
    grid->SetAttrAlignment(wxSheetCoords(13, 4), wxSHEET_AttrAlignBottom|wxSHEET_AttrAlignLeft);
    grid->SetAttrAlignment(wxSheetCoords(13, 5), wxSHEET_AttrAlignBottom|wxSHEET_AttrAlignCenterHoriz);
    grid->SetAttrAlignment(wxSheetCoords(13, 6), wxSHEET_AttrAlignBottom|wxSHEET_AttrAlignRight);

    // rotated text alignment tests, a 3x3 grid
    grid->SetCellValue(wxSheetCoords(14, 4), wxT("The below 3x3 grid tests rotated text alignment"));

    grid->SetCellValue(wxSheetCoords(15, 4), wxT("TopLeft"));
    grid->SetCellValue(wxSheetCoords(15, 5), wxT("TopCenter"));
    grid->SetCellValue(wxSheetCoords(15, 6), wxT("TopRight"));
    grid->SetCellValue(wxSheetCoords(16, 4), wxT("CenterLeft"));
    grid->SetCellValue(wxSheetCoords(16, 5), wxT("CenterCenter"));
    grid->SetCellValue(wxSheetCoords(16, 6), wxT("CenterRight"));
    grid->SetCellValue(wxSheetCoords(17, 4), wxT("BottomLeft"));
    grid->SetCellValue(wxSheetCoords(17, 5), wxT("BottomCenter"));
    grid->SetCellValue(wxSheetCoords(17, 6), wxT("BottomRight"));

    grid->SetAttrOrientation(wxSheetCoords(15, 4), wxSHEET_AttrOrientVert);
    grid->SetAttrOrientation(wxSheetCoords(15, 5), wxSHEET_AttrOrientVert);
    grid->SetAttrOrientation(wxSheetCoords(15, 6), wxSHEET_AttrOrientVert);
    grid->SetAttrOrientation(wxSheetCoords(16, 4), wxSHEET_AttrOrientVert);
    grid->SetAttrOrientation(wxSheetCoords(16, 5), wxSHEET_AttrOrientVert);
    grid->SetAttrOrientation(wxSheetCoords(16, 6), wxSHEET_AttrOrientVert);
    grid->SetAttrOrientation(wxSheetCoords(17, 4), wxSHEET_AttrOrientVert);
    grid->SetAttrOrientation(wxSheetCoords(17, 5), wxSHEET_AttrOrientVert);
    grid->SetAttrOrientation(wxSheetCoords(17, 6), wxSHEET_AttrOrientVert);

    grid->SetAttrAlignment(wxSheetCoords(15, 4), wxSHEET_AttrAlignTop|wxSHEET_AttrAlignLeft);
    grid->SetAttrAlignment(wxSheetCoords(15, 5), wxSHEET_AttrAlignTop|wxSHEET_AttrAlignCenterHoriz);
    grid->SetAttrAlignment(wxSheetCoords(15, 6), wxSHEET_AttrAlignTop|wxSHEET_AttrAlignRight);
    grid->SetAttrAlignment(wxSheetCoords(16, 4), wxSHEET_AttrAlignCenterVert|wxSHEET_AttrAlignLeft);
    grid->SetAttrAlignment(wxSheetCoords(16, 5), wxSHEET_AttrAlignCenterVert|wxSHEET_AttrAlignCenterHoriz);
    grid->SetAttrAlignment(wxSheetCoords(16, 6), wxSHEET_AttrAlignCenterVert|wxSHEET_AttrAlignRight);
    grid->SetAttrAlignment(wxSheetCoords(17, 4), wxSHEET_AttrAlignBottom|wxSHEET_AttrAlignLeft);
    grid->SetAttrAlignment(wxSheetCoords(17, 5), wxSHEET_AttrAlignBottom|wxSHEET_AttrAlignCenterHoriz);
    grid->SetAttrAlignment(wxSheetCoords(17, 6), wxSHEET_AttrAlignBottom|wxSHEET_AttrAlignRight);




    // Editor tests Col 10
    grid->SetCellValue(wxSheetCoords(-1, 9), wxT("Editor/Renderer types"));
    grid->SetCellValue(wxSheetCoords(0, 9), wxT("StringEditor   StringRenderer"));
    grid->SetCellValue(wxSheetCoords(1, 9), wxT("AWStringEditor AWStringRenderer"));
    grid->SetCellValue(wxSheetCoords(2, 9), wxT("NumberEditor   NumberRenderer"));
    grid->SetCellValue(wxSheetCoords(3, 9), wxT("NumberEditor   NumberRenderer"));
    grid->SetCellValue(wxSheetCoords(4, 9), wxT("FloatEditor    FloatRenderer"));
    grid->SetCellValue(wxSheetCoords(5, 9), wxT("FloatEditor    FloatRenderer"));
    grid->SetCellValue(wxSheetCoords(6, 9), wxT("BoolEditor     BoolRenderer"));
    grid->SetCellValue(wxSheetCoords(7, 9), wxT("ChoiceEditor   StringRenderer"));
    grid->SetCellValue(wxSheetCoords(8, 9), wxT("ChoiceEditor   StringRenderer"));
    grid->SetCellValue(wxSheetCoords(9, 9), wxT("EnumEditor     EnumRenderer"));
    grid->AutoSizeCol(9, false); // make the text fit nicely

    const wxString enumStrings(wxT("Red,Green,Blue"));

    grid->SetCellValue(wxSheetCoords(-1, 10), wxT("Test"));
    grid->SetAttrEditor(wxSheetCoords(0, 10), wxSheetCellEditor(new wxSheetCellTextEditorRefData()));
    grid->SetAttrEditor(wxSheetCoords(1, 10), wxSheetCellEditor(new wxSheetCellAutoWrapStringEditorRefData()));
    grid->SetAttrEditor(wxSheetCoords(2, 10), wxSheetCellEditor(new wxSheetCellNumberEditorRefData()));
    grid->SetAttrEditor(wxSheetCoords(3, 10), wxSheetCellEditor(new wxSheetCellNumberEditorRefData(0, 100)));
    grid->SetAttrEditor(wxSheetCoords(4, 10), wxSheetCellEditor(new wxSheetCellFloatEditorRefData()));
    grid->SetAttrEditor(wxSheetCoords(5, 10), wxSheetCellEditor(new wxSheetCellFloatEditorRefData(4,4)));
    grid->SetAttrEditor(wxSheetCoords(6, 10), wxSheetCellEditor(new wxSheetCellBoolEditorRefData()));
    grid->SetAttrEditor(wxSheetCoords(7, 10), wxSheetCellEditor(new wxSheetCellChoiceEditorRefData(3, choices, false)));
    grid->SetAttrEditor(wxSheetCoords(8, 10), wxSheetCellEditor(new wxSheetCellChoiceEditorRefData(3, choices, true)));
    grid->SetAttrEditor(wxSheetCoords(9, 10), wxSheetCellEditor(new wxSheetCellEnumEditorRefData(enumStrings)));

    grid->SetAttrRenderer(wxSheetCoords(0, 10), wxSheetCellRenderer(new wxSheetCellStringRendererRefData()));
    grid->SetAttrRenderer(wxSheetCoords(1, 10), wxSheetCellRenderer(new wxSheetCellAutoWrapStringRendererRefData()));
    grid->SetAttrRenderer(wxSheetCoords(2, 10), wxSheetCellRenderer(new wxSheetCellNumberRendererRefData()));
    grid->SetAttrRenderer(wxSheetCoords(3, 10), wxSheetCellRenderer(new wxSheetCellNumberRendererRefData()));
    grid->SetAttrRenderer(wxSheetCoords(4, 10), wxSheetCellRenderer(new wxSheetCellFloatRendererRefData()));
    grid->SetAttrRenderer(wxSheetCoords(5, 10), wxSheetCellRenderer(new wxSheetCellFloatRendererRefData(4,4)));
    grid->SetAttrRenderer(wxSheetCoords(6, 10), wxSheetCellRenderer(new wxSheetCellBoolRendererRefData()));
    grid->SetAttrRenderer(wxSheetCoords(7, 10), wxSheetCellRenderer(new wxSheetCellStringRendererRefData()));
    grid->SetAttrRenderer(wxSheetCoords(8, 10), wxSheetCellRenderer(new wxSheetCellStringRendererRefData()));
    grid->SetAttrRenderer(wxSheetCoords(9, 10), wxSheetCellRenderer(new wxSheetCellEnumRendererRefData(enumStrings)));

    // ---------------------------------------------------------

#if 1
    splitter->SplitHorizontally(sheetSplitter, logWin, 400);
#else
    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    topSizer->Add( sheetSplitter, 1, wxEXPAND );
    topSizer->Add( logWin, 0, wxEXPAND );
    SetAutoLayout( true );
    SetSizer( topSizer );
    topSizer->Fit( this );
    //topSizer->SetSizeHints( this );  FIXME - no size hints for now
#endif

    Centre();
    SetDefaults();
}


GridFrame::~GridFrame()
{
    delete wxLog::SetActiveTarget(m_logOld);
}

void GridFrame::SetDefaults()
{
    GetMenuBar()->Check( ID_TOGGLEROWLABELS, true );
    GetMenuBar()->Check( ID_TOGGLECOLLABELS, true );
    GetMenuBar()->Check( ID_TOGGLEEDIT, true );
    GetMenuBar()->Check( ID_TOGGLEROWSIZING, true );
    GetMenuBar()->Check( ID_TOGGLECOLSIZING, true );
    GetMenuBar()->Check( ID_TOGGLEGRIDSIZING, true );
    GetMenuBar()->Check( ID_TOGGLEHORIZGRIDLINES, true );
    GetMenuBar()->Check( ID_TOGGLEVERTGRIDLINES, true );
    GetMenuBar()->Check( ID_CELLOVERFLOW, true );
}


void GridFrame::ToggleRowLabels( wxCommandEvent& WXUNUSED(ev) )
{
    if ( GetMenuBar()->IsChecked( ID_TOGGLEROWLABELS ) )
    {
        grid->SetRowLabelWidth( WXSHEET_DEFAULT_ROW_LABEL_WIDTH );
    }
    else
    {
        grid->SetRowLabelWidth( 0 );
    }
}


void GridFrame::ToggleColLabels( wxCommandEvent& WXUNUSED(ev) )
{
    if ( GetMenuBar()->IsChecked( ID_TOGGLECOLLABELS ) )
    {
        grid->SetColLabelHeight( WXSHEET_DEFAULT_COL_LABEL_HEIGHT );
    }
    else
    {
        grid->SetColLabelHeight( 0 );
    }
}

void GridFrame::ToggleEditing( wxCommandEvent& WXUNUSED(ev) )
{
    grid->SetAreaEditable( GetMenuBar()->IsChecked( ID_TOGGLEEDIT ) ?
                           wxSHEET_CELL_ANY : wxSHEET_CELL_NONE );
}


void GridFrame::ToggleRowSizing( wxCommandEvent& WXUNUSED(ev) )
{
    grid->EnableDragRowSize(
        GetMenuBar()->IsChecked( ID_TOGGLEROWSIZING ) );
}


void GridFrame::ToggleColSizing( wxCommandEvent& WXUNUSED(ev) )
{
    grid->EnableDragColSize(
        GetMenuBar()->IsChecked( ID_TOGGLECOLSIZING ) );
}

void GridFrame::ToggleGridSizing( wxCommandEvent& WXUNUSED(ev) )
{
    grid->EnableDragGridSize(
        GetMenuBar()->IsChecked( ID_TOGGLEGRIDSIZING ) );
}

void GridFrame::ToggleGridLines( wxCommandEvent& WXUNUSED(ev) )
{
    grid->EnableGridLines(
        (GetMenuBar()->IsChecked( ID_TOGGLEHORIZGRIDLINES ) ? wxHORIZONTAL : 0) |
        (GetMenuBar()->IsChecked( ID_TOGGLEVERTGRIDLINES ) ? wxVERTICAL : 0) );
}

void GridFrame::OnHideShowSelRowsCols( wxCommandEvent& event )
{
    bool is_rows = (event.GetId() == ID_HIDESELROWS) ||
                   (event.GetId() == ID_SHOWSELROWS);
    bool is_hide = (event.GetId() == ID_HIDESELROWS) ||
                   (event.GetId() == ID_HIDESELCOLS);

    wxSheetSelection* sel = grid->GetSelection();
    int n, count = sel->GetCount();
    for (n = 0; n < count; n++)
    {
        wxSheetBlock block = sel->GetBlock(n);
        if (is_rows)
        {
            if (block.GetWidth() >= grid->GetNumberCols())
            {
                for (int r = block.GetTop(); r <= block.GetBottom(); r++)
                {
                    if (is_hide)
                        grid->SetRowHeight(r, 0, true);
                    else if (grid->GetRowHeight(r) == 0)
                        grid->SetRowHeight(r, grid->GetDefaultRowHeight());
                }
            }
        }
        else
        {
            if (block.GetHeight() >= grid->GetNumberRows())
            {
                for (int c = block.GetLeft(); c <= block.GetRight(); c++)
                {
                    if (is_hide)
                        grid->SetColWidth(c, 0, true);
                    else if (grid->GetColWidth(c) == 0)
                        grid->SetColWidth(c, grid->GetDefaultColWidth());
                }
            }
        }
    }
}

void GridFrame::OnSetHighlightWidth( wxCommandEvent& WXUNUSED(ev) )
{
    wxString choices[] = { _T("0"), _T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"), _T("10")};

    wxSingleChoiceDialog dlg(this, _T("Choose the thickness of the highlight pen:"),
                             _T("Pen Width"), 11, choices);

    wxPen pen(grid->GetCursorCellHighlightPen());
    dlg.SetSelection(pen.GetWidth());
    if (dlg.ShowModal() == wxID_OK)
    {
        pen.SetWidth(dlg.GetSelection());
        grid->SetCursorCellHighlightPen(pen);
    }
}

void GridFrame::OnSetROHighlightWidth( wxCommandEvent& WXUNUSED(ev) )
{
    wxString choices[] = { _T("0"), _T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"), _T("10")};

    wxSingleChoiceDialog dlg(this, _T("Choose the thickness of the highlight pen:"),
                             _T("Pen Width"), 11, choices);

    wxPen pen(grid->GetCursorCellHighlightROPen());
    dlg.SetSelection(pen.GetWidth());
    if (dlg.ShowModal() == wxID_OK)
    {
        pen.SetWidth(dlg.GetSelection());
        grid->SetCursorCellHighlightROPen(pen);
    }
}

void GridFrame::AutoSizeCols( wxCommandEvent& WXUNUSED(ev) )
{
    grid->AutoSizeCols();
    grid->Refresh();
}

void GridFrame::CellOverflow( wxCommandEvent& ev )
{
    grid->SetAttrOverflow(wxGridCellSheetCoords, ev.IsChecked(), wxSHEET_AttrDefault);
    //grid->Refresh();
}

void GridFrame::ResizeCell( wxCommandEvent& ev )
{
    if (ev.IsChecked())
        grid->SetCellSpan( wxSheetCoords(7, 1), wxSheetCoords(5, 5) );
    else
        grid->SetCellSpan( wxSheetCoords(7, 1), wxSheetCoords(1, 5) );
    //grid->Refresh();
}

void GridFrame::SetLabelColour( wxCommandEvent& WXUNUSED(ev) )
{
    wxColour colour = wxGetColourFromUser(this,
        grid->GetAttrBackgroundColour( wxRowLabelSheetCoords, wxSHEET_AttrDefault ));

    if ( colour.Ok() )
        grid->SetAttrBackgroundColour( wxRowLabelSheetCoords, colour, wxSHEET_AttrDefault );
}

void GridFrame::SetLabelTextColour( wxCommandEvent& WXUNUSED(ev) )
{
    wxColour colour = wxGetColourFromUser(this,
        grid->GetAttrForegroundColour( wxRowLabelSheetCoords, wxSHEET_AttrDefault ));

    if ( colour.Ok() )
        grid->SetAttrForegroundColour( wxRowLabelSheetCoords, colour, wxSHEET_AttrDefault );
}

void GridFrame::SetLabelFont( wxCommandEvent& WXUNUSED(ev) )
{
    wxFont font = wxGetFontFromUser(this);

    if ( font.Ok() )
    {
        grid->SetAttrFont( wxRowLabelSheetCoords, font, wxSHEET_AttrDefault );
    }
}

void GridFrame::OnScrollbarsShown( wxCommandEvent& ev )
{
    switch (ev.GetId())
    {
        case ID_SCROLLBARS_HORIZ_NEVER  : grid->SetScrollBarMode(wxSheet::SB_HORIZ_NEVER); break;
        case ID_SCROLLBARS_VERT_NEVER   : grid->SetScrollBarMode(wxSheet::SB_VERT_NEVER); break;
        case ID_SCROLLBARS_NEVER        : grid->SetScrollBarMode(wxSheet::SB_NEVER); break;
        case ID_SCROLLBARS_HORIZ_ALWAYS : grid->SetScrollBarMode(wxSheet::SB_HORIZ_ALWAYS); break;
        case ID_SCROLLBARS_VERT_ALWAYS  : grid->SetScrollBarMode(wxSheet::SB_VERT_ALWAYS); break;
        case ID_SCROLLBARS_ALWAYS       : grid->SetScrollBarMode(wxSheet::SB_ALWAYS); break;
        case ID_SCROLLBARS_AS_NEEDED    :
        default                         : grid->SetScrollBarMode(wxSheet::SB_AS_NEEDED); break;
    }

    grid->AdjustScrollbars(); // always call this to update SB settings
}

void GridFrame::SetRowLabelHorizAlignment( wxCommandEvent& WXUNUSED(ev) )
{
    int align = grid->GetAttrAlignment(wxRowLabelSheetCoords, wxSHEET_AttrDefault);
    int horiz;

    switch ( align & wxSHEET_AttrAlignHoriz_Mask )
    {
        case wxSHEET_AttrAlignLeft:
            horiz = wxSHEET_AttrAlignCenterHoriz;
            break;

        case wxSHEET_AttrAlignCenterHoriz:
            horiz = wxSHEET_AttrAlignRight;
            break;

        case wxSHEET_AttrAlignRight:
        default:
            horiz = wxSHEET_AttrAlignLeft;
            break;
    }
    align &= ~wxSHEET_AttrAlignHoriz_Mask; // clear old
    align |= horiz;

    grid->SetAttrAlignment( wxRowLabelSheetCoords, align, wxSHEET_AttrDefault );
}

void GridFrame::SetRowLabelVertAlignment( wxCommandEvent& WXUNUSED(ev) )
{
    int align = grid->GetAttrAlignment(wxRowLabelSheetCoords, wxSHEET_AttrDefault);
    int vert;

    switch ( align & wxSHEET_AttrAlignVert_Mask )
    {
        case wxSHEET_AttrAlignTop:
            vert = wxSHEET_AttrAlignCenterVert;
            break;

        case wxSHEET_AttrAlignCenterVert:
            vert = wxSHEET_AttrAlignBottom;
            break;

        case wxSHEET_AttrAlignBottom:
        default:
            vert = wxSHEET_AttrAlignTop;
            break;
    }

    align &= ~wxSHEET_AttrAlignVert_Mask; // clear old
    align |= vert;
    grid->SetAttrAlignment( wxRowLabelSheetCoords, align, wxSHEET_AttrDefault );
}


void GridFrame::SetColLabelHorizAlignment( wxCommandEvent& WXUNUSED(ev) )
{
    int align = grid->GetAttrAlignment(wxColLabelSheetCoords, wxSHEET_AttrDefault);
    int horiz;

    switch ( align & wxSHEET_AttrAlignHoriz_Mask )
    {
        case wxSHEET_AttrAlignLeft:
            horiz = wxSHEET_AttrAlignCenterHoriz;
            break;

        case wxSHEET_AttrAlignCenterHoriz:
            horiz = wxSHEET_AttrAlignRight;
            break;

        case wxSHEET_AttrAlignRight:
        default:
            horiz = wxSHEET_AttrAlignLeft;
            break;
    }

    align &= ~wxSHEET_AttrAlignHoriz_Mask;
    align |= horiz;
    grid->SetAttrAlignment( wxColLabelSheetCoords, align, wxSHEET_AttrDefault );
}


void GridFrame::SetColLabelVertAlignment( wxCommandEvent& WXUNUSED(ev) )
{
    int align = grid->GetAttrAlignment(wxColLabelSheetCoords, wxSHEET_AttrDefault);
    int vert;

    switch ( align & wxSHEET_AttrAlignVert_Mask )
    {
        case wxSHEET_AttrAlignTop:
            vert = wxSHEET_AttrAlignCenterVert;
            break;

        case wxSHEET_AttrAlignCenterVert:
            vert = wxSHEET_AttrAlignBottom;
            break;

        case wxSHEET_AttrAlignBottom:
        default:
            vert = wxSHEET_AttrAlignTop;
            break;
    }

    align &= ~wxSHEET_AttrAlignVert_Mask;
    align |= vert;
    grid->SetAttrAlignment( wxColLabelSheetCoords, align, wxSHEET_AttrDefault );
}


void GridFrame::SetGridLineColour( wxCommandEvent& WXUNUSED(ev) )
{
    wxColour colour = wxGetColourFromUser(this, grid->GetGridLineColour(), wxT("Set grid line colour"));

    if ( colour.Ok() )
        grid->SetGridLineColour( colour );
}

void GridFrame::OnCopyPaste( wxCommandEvent& ev )
{
    if (!grid->HasFocus()) // not for virtual or bugs test windows
        return;

    switch (ev.GetId())
    {
        case wxID_COPY :
        {
            grid->CopyCurrentSelectionToClipboard();
            break;
        }
        case wxID_PASTE :
        {
            grid->PasteFromClipboard();
            break;
        }
    }
}

void GridFrame::InsertRow( wxCommandEvent& WXUNUSED(ev) )
{
    grid->InsertRows( grid->GetGridCursorRow(), 1 );
}

void GridFrame::InsertCol( wxCommandEvent& WXUNUSED(ev) )
{
    grid->InsertCols( grid->GetGridCursorCol(), 1 );
}

void GridFrame::DeleteSelectedRows( wxCommandEvent& WXUNUSED(ev) )
{
    if ( grid->HasSelection() )
    {
        grid->BeginBatch();
        for ( int n = 0; n < grid->GetNumberRows(); )
        {
            if ( grid->IsRowSelected( n ) )
                grid->DeleteRows( n, 1 );
            else
                n++;
        }
        grid->EndBatch();
    }
}

void GridFrame::DeleteSelectedCols( wxCommandEvent& WXUNUSED(ev) )
{
    if ( grid->HasSelection() )
    {
        grid->BeginBatch();
        for ( int n = 0; n < grid->GetNumberCols(); )
        {
            if ( grid->IsColSelected( n ) )
            {
                grid->DeleteCols( n, 1 );
            }
            else
                n++;
        }
        grid->EndBatch();
    }
}

void GridFrame::ClearGrid( wxCommandEvent& WXUNUSED(ev) )
{
    grid->ClearValues();
}
void GridFrame::EnableGrid( wxCommandEvent& ev )
{
    grid->Enable(ev.IsChecked());
}

void GridFrame::SelectionMode( wxCommandEvent& ev )
{
    switch (ev.GetId())
    {
        case ID_SELNONE   : grid->SetSelectionMode( wxSHEET_SelectNone   ); break;
        case ID_SELCELLS  : grid->SetSelectionMode( wxSHEET_SelectCells  ); break;
        case ID_SELROWS   : grid->SetSelectionMode( wxSHEET_SelectRows   ); break;
        case ID_SELCOLS   : grid->SetSelectionMode( wxSHEET_SelectCols   ); break;
        case ID_SELSINGLE : grid->SetSelectionMode( wxSHEET_SelectSingle ); break;
        default : break;
    }
}

void GridFrame::SetCellFgColour( wxCommandEvent& WXUNUSED(ev) )
{
    wxColour colour = wxGetColourFromUser(this,
        grid->GetAttrForegroundColour(wxGridCellSheetCoords, wxSHEET_AttrDefault));

    if ( colour.Ok() )
    {
        grid->SetAttrForegroundColour(wxGridCellSheetCoords, colour, wxSHEET_AttrDefault);
        grid->Refresh();
    }
}

void GridFrame::SetCellBgColour( wxCommandEvent& WXUNUSED(ev) )
{
    wxColour colour = wxGetColourFromUser(this,
        grid->GetAttrBackgroundColour(wxGridCellSheetCoords, wxSHEET_AttrDefault));

    if ( colour.Ok() )
    {
        // Check the new Refresh function by passing it a rectangle
        // which exactly fits the grid.
        wxRect r(wxPoint(0, 0), grid->GetSize());
        grid->SetAttrBackgroundColour(wxGridCellSheetCoords, colour, wxSHEET_AttrDefault);
        grid->Refresh(true, &r);
    }
}

void GridFrame::DeselectCell(wxCommandEvent& WXUNUSED(event))
{
      grid->DeselectCell(wxSheetCoords(3, 1));
}

void GridFrame::DeselectCol(wxCommandEvent& WXUNUSED(event))
{
      grid->DeselectCol(2);
}

void GridFrame::DeselectRow(wxCommandEvent& WXUNUSED(event))
{
      grid->DeselectRow(2);
}

void GridFrame::DeselectAll(wxCommandEvent& WXUNUSED(event))
{
      grid->ClearSelection();
}

void GridFrame::SelectCell(wxCommandEvent& WXUNUSED(event))
{
      grid->SelectBlock(wxSheetBlock(3, 1, 1, 1), m_addToSel);
}

void GridFrame::SelectCol(wxCommandEvent& WXUNUSED(event))
{
      grid->SelectCol(2, m_addToSel);
}

void GridFrame::SelectRow(wxCommandEvent& WXUNUSED(event))
{
      grid->SelectRow(2, m_addToSel);
}

void GridFrame::SelectAll(wxCommandEvent& WXUNUSED(event))
{
      grid->SelectAll();
}

void GridFrame::OnAddToSelectToggle(wxCommandEvent& event)
{
    m_addToSel = event.IsChecked();
}

void GridFrame::OnLabelLeftDown( wxSheetEvent& ev )
{
    logBuf = _T("");
    if ( ev.GetCoords().GetRow() != -1 )
    {
        logBuf << _T("Left click on row label ") << ev.GetCoords().GetRow();
    }
    else if ( ev.GetCoords().GetCol() != -1 )
    {
        logBuf << _T("Left click on col label ") << ev.GetCoords().GetCol();
    }
    else
    {
        logBuf << _T("Left click on corner label");
    }

    logBuf << _T(" point (") << ev.GetPosition().x << _T(", ")
                             << ev.GetPosition().y << _T(")");

    wxSheet* sheet = wxDynamicCast(ev.GetEventObject(), wxSheet);
    wxSize cellSize = sheet->GetCellSize(ev.GetCoords());
    logBuf << _T(" size (") << cellSize.x << _T(", ") << cellSize.y << _T(")");

    if ( ev.Selecting() )   logBuf << _T(" (selecting)");
    if ( ev.ShiftDown() )   logBuf << _T(" (shift down)");
    if ( ev.ControlDown() ) logBuf << _T(" (control down)");
    if ( ev.AltDown() )     logBuf << _T(" (alt down)");
    if ( ev.MetaDown() )    logBuf << _T(" (meta down)");

    wxLogMessage( wxT("%s"), logBuf.c_str() );

    // you must call event skip if you want default grid processing
    ev.Skip();
}


void GridFrame::OnCellLeftDown( wxSheetEvent& ev )
{
    logBuf.Printf(_T("Left click at row %d, col %d, point (%d, %d)"),
              ev.GetCoords().GetRow(), ev.GetCoords().GetCol(),
              ev.GetPosition().x, ev.GetPosition().y);

    wxSheet* sheet = wxDynamicCast(ev.GetEventObject(), wxSheet);
    wxSize cellSize = sheet->GetCellSize(ev.GetCoords());
    logBuf << _T(" size (") << cellSize.x << _T(", ") << cellSize.y << _T(")");

    if ( ev.Selecting() )   logBuf << _T(" (selecting)");
    if ( ev.ShiftDown() )   logBuf << _T(" (shift down)");
    if ( ev.ControlDown() ) logBuf << _T(" (control down)");
    if ( ev.AltDown() )     logBuf << _T(" (alt down)");
    if ( ev.MetaDown() )    logBuf << _T(" (meta down)");

    wxLogMessage( wxT("%s"), logBuf.c_str() );

    // you must call event skip if you want default grid processing
    // (cell highlighting etc.)
    ev.Skip();
}

void GridFrame::OnCellRightDown( wxSheetEvent& ev )
{
    logBuf.Printf(_T("Right click at row %d, col %d, point (%d, %d), scrpoint(%d, %d)"),
              ev.GetCoords().GetRow(), ev.GetCoords().GetCol(),
              ev.GetPosition().x, ev.GetPosition().y,
              ev.GetScrolledPosition().x, ev.GetScrolledPosition().y);

    if ( ev.Selecting() )   logBuf << _T(" (selecting)");
    if ( ev.ShiftDown() )   logBuf << _T(" (shift down)");
    if ( ev.ControlDown() ) logBuf << _T(" (control down)");
    if ( ev.AltDown() )     logBuf << _T(" (alt down)");
    if ( ev.MetaDown() )    logBuf << _T(" (meta down)");

    wxLogMessage( wxT("%s"), logBuf.c_str() );

    // you must call event skip if you want default grid processing
    // (cell highlighting etc.)
    //
    ev.Skip();
}

void GridFrame::OnRowSize( wxSheetCellSizeEvent& ev )
{
    wxSheetCoords coords(ev.GetCoords());

    wxSheet* sheet = wxDynamicCast(ev.GetEventObject(), wxSheet);
    wxString s = wxString::Format(wxT("T=%d B=%d B-T+1=%d = H=%d"),
        sheet->GetRowTop(coords.GetRow()), sheet->GetRowBottom(coords.GetRow()),
        sheet->GetRowBottom(coords.GetRow()) - sheet->GetRowTop(coords.GetRow()) + 1,
        sheet->GetRowHeight(coords.GetRow()));

    logBuf = _T("");

    if (ev.GetEventType() == wxEVT_SHEET_ROW_SIZE)
        logBuf << _T("Resize row ");
    else if (ev.GetEventType() == wxEVT_SHEET_ROW_SIZING)
    {
        logBuf << _T("Resizing row ");
        if (GetMenuBar()->IsChecked(ID_TOGGLEROWSIZINGVETO))
        {
            logBuf << _T(" Evt vetoed ");
            ev.Veto();
        }
    }
    else
        logBuf << _T("Resized row ");

    logBuf << ev.GetCoords().GetRow() << _T(" size ") << ev.GetSize();

    wxLogMessage( wxT("%s - %s"), logBuf.c_str(), s.c_str() );

    ev.Skip();
}


void GridFrame::OnColSize( wxSheetCellSizeEvent& ev )
{
    wxSheetCoords coords(ev.GetCoords());

    wxSheet* sheet = wxDynamicCast(ev.GetEventObject(), wxSheet);
    wxString s = wxString::Format(wxT("L=%d R=%d R-L+1=%d = W=%d"),
        sheet->GetColLeft(coords.GetCol()), sheet->GetColRight(coords.GetCol()),
        sheet->GetColRight(coords.GetCol()) - sheet->GetColLeft(coords.GetCol()) + 1,
        sheet->GetColWidth(coords.GetCol()));

    logBuf = _T("");

    if (ev.GetEventType() == wxEVT_SHEET_COL_SIZE)
        logBuf << _T("Resize col ");
    else if (ev.GetEventType() == wxEVT_SHEET_COL_SIZING)
    {
       logBuf << _T("Resizing col ");
       if (GetMenuBar()->IsChecked(ID_TOGGLECOLSIZINGVETO))
       {
           logBuf << _T(" Evt vetoed ");
           ev.Veto();
       }
    }
    else
       logBuf << _T("Resized col ");

    logBuf << ev.GetCoords().GetCol() << _T(" size ") << ev.GetSize();

    wxLogMessage( wxT("%s - %s"), logBuf.c_str(), s.c_str() );

    ev.Skip();
}


void GridFrame::OnSelectCell( wxSheetEvent& ev )
{
    logBuf = _T("");

    logBuf << _T("Cursor ");

    logBuf << _T("cell at row ") << ev.GetCoords().GetRow()
           << _T(" col ") << ev.GetCoords().GetCol()
           << _T(" ( ControlDown: ")<< (ev.ControlDown() ? wxT('T'): wxT('F'))
           << _T(", ShiftDown: ")<< (ev.ShiftDown() ? wxT('T'): wxT('F'))
           << _T(", AltDown: ")<< (ev.AltDown() ? wxT('T'): wxT('F'))
           << _T(", MetaDown: ")<< (ev.MetaDown() ? wxT("T"):wxT("F")) << _T(" )");
    wxLogMessage( wxT("%s"), logBuf.c_str() );

    // you must call Skip() if you want the default processing
    // to occur in wxSheet
    ev.Skip();
}

void GridFrame::OnRangeSelect( wxSheetRangeSelectEvent& ev )
{
    logBuf = _T("");

    if (ev.GetEventType() == wxEVT_SHEET_RANGE_SELECTING)
    {
        if ( ev.Selecting() )
            logBuf << _T("Selecting ");
        else
            logBuf << _T("Deselecting ");
    }
    else
    {
        if ( ev.Selecting() )
            logBuf << _T("Selected ");
        else
            logBuf << _T("Deselected ");
    }

    wxSheetBlock block(ev.GetBlock());

    logBuf << _T("cells from row ") << block.GetTop()
           << _T(" col ") << block.GetLeft()
           << _T(" to row ") << block.GetBottom()
           << _T(" col ") << block.GetRight()
           << _T(" ( ControlDown: ")<< (ev.ControlDown() ? wxT('T'):wxT('F'))
           << _T(", ShiftDown: ")<< (ev.ShiftDown() ? wxT('T'):wxT('F'))
           << _T(", AltDown: ")<< (ev.AltDown() ? wxT('T'):wxT('F'))
           << _T(", MetaDown: ")<< (ev.MetaDown() ? wxT('T'):wxT('F')) << _T(" )");
    wxLogMessage( wxT("%s"), logBuf.c_str() );

    ev.Skip();
}

void GridFrame::OnCellValueChanged( wxSheetEvent& ev )
{
    logBuf = _T("");
    if (ev.GetEventType() == wxEVT_SHEET_CELL_VALUE_CHANGING)
        logBuf  << _T("Value changing for cell at");
    else if (ev.GetEventType() == wxEVT_SHEET_CELL_VALUE_CHANGED)
        logBuf  << _T("Value changed for cell at");

    logBuf  << _T(" row ") << ev.GetCoords().GetRow()
            << _T(" col ") << ev.GetCoords().GetCol();

    if (ev.GetEventType() == wxEVT_SHEET_CELL_VALUE_CHANGING)
        logBuf  << _T(" New value:'") << ev.GetString() << wxT("'");

    if (ev.GetCoords() == wxSheetCoords(0, 2))
    {
        logBuf << _T(" Event was vetoed");
        ev.Veto();
    }

    wxLogMessage( wxT("%s"), logBuf.c_str() );

    ev.Skip();
}

void GridFrame::OnEditorShown( wxSheetEvent& ev )
{

    if ( (ev.GetCoords().GetCol() == 4) &&
         (ev.GetCoords().GetRow() == 0) &&
     (wxMessageBox(_T("Are you sure you wish to edit this cell"),
                   _T("Checking"),wxYES_NO|wxICON_QUESTION) == wxNO ) ) {

     ev.Veto();
     return;
    }

    wxLogMessage( wxT("Cell editor shown.") );

    ev.Skip();
}

void GridFrame::OnEditorHidden( wxSheetEvent& ev )
{
    if ( (ev.GetCoords().GetCol() == 4) &&
         (ev.GetCoords().GetRow() == 0) &&
     (wxMessageBox(_T("Are you sure you wish to finish editing this cell"),
                   _T("Checking"),wxYES_NO|wxICON_QUESTION) == wxNO ) ) {

        ev.Veto();
        return;
    }

    wxLogMessage( wxT("Cell editor hidden.") );

    ev.Skip();
}

void GridFrame::OnSheetSplitter( wxSheetSplitterEvent& ev )
{
    logBuf = _T("");

    if (ev.GetEventType() == wxEVT_SHEET_SPLIT_BEGIN)
        logBuf  << _T("Splitting beginning");
    else if (ev.GetEventType() == wxEVT_SHEET_SPLIT_CHANGING)
        logBuf  << _T("Splitter position changing");
    else if (ev.GetEventType() == wxEVT_SHEET_SPLIT_CHANGED)
        logBuf  << _T("Splitter position changed");
    else if (ev.GetEventType() == wxEVT_SHEET_SPLIT_DOUBLECLICKED)
        logBuf  << _T("Splitter doubleclicked");
    else if (ev.GetEventType() == wxEVT_SHEET_SPLIT_UNSPLIT)
        logBuf  << _T("Splitter unsplit");
    else if (ev.GetEventType() == wxEVT_SHEET_SPLIT_CREATE_SHEET)
        logBuf  << _T("Splitter create sheet");

    logBuf  << _T(" Sash pos ") << ev.GetSashPosition()
            << _T(" Is vertical ") << ev.IsVerticalSplit();

    wxLogMessage( wxT("%s"), logBuf.c_str() );

    ev.Skip();
}

void GridFrame::About(  wxCommandEvent& WXUNUSED(ev) )
{
    (void)wxMessageBox( _T("\n\nwxSheet demo \n\n")
                        _T("Michael Bedward \n")
                        _T("mbedward@ozemail.com.au \n\n"),
                        _T("About"),
                        wxOK|wxICON_INFORMATION );
}


void GridFrame::OnQuit( wxCommandEvent& WXUNUSED(ev) )
{
    Close( true );
}

void GridFrame::OnBugsTable(wxCommandEvent& )
{
    BugsGridFrame *frame = new BugsGridFrame;
    frame->Show(true);
}

void GridFrame::OnSmallGrid(wxCommandEvent& )
{
    wxFrame* frame = new wxFrame(NULL, -1, _T("A Small Grid"),
                                 wxDefaultPosition, wxSize(640, 480));
    wxPanel* panel = new wxPanel(frame, -1);
    wxSheet* grid = new wxSheet(panel, -1, wxPoint(10,10), wxSize(400,400),
                              wxWANTS_CHARS | wxSIMPLE_BORDER);
    grid->CreateGrid(3,3);
    frame->Show(true);
}

void GridFrame::OnVTable(wxCommandEvent& )
{
    static long s_sizeGrid = 10000;

#ifdef __WXMOTIF__
    // MB: wxGetNumberFromUser doesn't work properly for wxMotif
    wxString s;
    s << s_sizeGrid;
    s = wxGetTextFromUser( _T("Size of the table to create"),
                           _T("Size:"),
                           s );

    s.ToLong( &s_sizeGrid );

#else
    s_sizeGrid = wxGetNumberFromUser(_T("Size of the table to create"),
                                     _T("Size: "),
                                     _T("wxSheetDemo question"),
                                     s_sizeGrid,
                                     0, 32000, this);
#endif

    if ( s_sizeGrid != -1 )
    {
        BigGridFrame* win = new BigGridFrame(s_sizeGrid);
        win->Show(true);

        wxYield(); wxYield();
        wxPrintf(wxT("Start "));

        //frame->grid->MakeCellVisible(wxSheetCoords(99,99));
        wxRect rect(wxPoint(0,0), win->m_grid->GetGridWindow()->GetSize());
        wxStopWatch sw;

        for (int n = 0; n < 000; n++)
        {
            win->m_grid->GetGridWindow()->Refresh(true, &rect);
            wxYield();
        }
        wxPrintf(wxT("Time %ld\n"), sw.Time());

    }
}

// ----------------------------------------------------------------------------
// MyGridCellRenderer
// ----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(MyGridCellRendererRefData, wxSheetCellStringRendererRefData)

// do something that the default renderer doesn't here just to show that it is
// possible to alter the appearance of the cell beyond what the attributes
// allow
void MyGridCellRendererRefData::Draw(wxSheet& grid, const wxSheetCellAttr& attr,
                              wxDC& dc, const wxRect& rect,
                              const wxSheetCoords& coords,
                              bool isSelected)
{
    wxSheetCellStringRendererRefData::Draw(grid, attr, dc, rect, coords, isSelected);

    dc.SetPen(*wxGREEN_PEN);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawEllipse(rect);
}

// ----------------------------------------------------------------------------
// MyGridCellAttrProvider
// ----------------------------------------------------------------------------

MyGridCellAttrProvider::MyGridCellAttrProvider(wxSheet *sheet)
{
    m_sheet = sheet; // we'll need the default attr to create new attrs

    // Note: Sometimes it's better to define a handful if attributes and return
    // the one you want based on some simple row/col calculations instead of
    // storing the attributes in the arrays. Other times it might be best
    // to create or modify attributes on the fly.
}

wxSheetCellAttr MyGridCellAttrProvider::GetAttr(const wxSheetCoords& coords,
                                                wxSheetAttr_Type kind )
{
    if ( coords.IsGridCell() )
    {
        // Get the attr for cell if set, else fall back to default attr.
        wxSheetCellAttr attr = wxSheetCellAttrProvider::GetAttr(coords, kind);
        if (!attr.Ok())
            attr = m_sheet->GetDefaultAttr(wxSheetCoords(0,0));

        // Modify attr on the fly for the background colour
        if (coords.GetRow() % 2)
            attr.SetBackgroundColour(*wxLIGHT_GREY);
        else
            attr.SetBackgroundColour(*wxWHITE);
    }
    else if ( coords.IsColLabelCell() )
    {
        // let's create a brand new attribute from some simple calculations
        wxSheetCellAttr attr(true);
        // remember, we always need the default attr!
        attr.SetDefaultAttr(m_sheet->GetDefaultAttr(wxColLabelSheetCoords));

        int r = coords.GetCol() % 255;
        int g = abs(127 - (coords.GetCol() % 255));
        int b = 255 - (coords.GetCol() % 255);
        attr.SetForegroundColour(wxColour(r, g, b));

        return attr;
    }

    // just return whatever in the arrays, it's ok to return wxNullSheetCellAttr
    return wxSheetCellAttrProvider::GetAttr(coords, kind);
}

// ============================================================================
// BigGridFrame and BigGridTable:  Sample of a non-standard table
// ============================================================================

wxString BigGridTable::GetValue( const wxSheetCoords& coords )
{
    return wxString::Format(wxT("(%d, %d)"), coords.GetRow(), coords.GetCol());
}

BigGridFrame::BigGridFrame(long sizeGrid)
            : wxFrame(NULL, -1, _T("Plugin Virtual Table"),
                      wxDefaultPosition, wxSize(500, 450))
{
    m_grid = new wxSheet(this, -1, wxDefaultPosition, wxDefaultSize);
    m_table = new BigGridTable(sizeGrid);
    m_table->SetAttrProvider(new MyGridCellAttrProvider(m_grid), true);
    m_grid->SetTable(m_table, true);

#if defined __WXMOTIF__
    // MB: the grid isn't getting a sensible default size under wxMotif
    int cw, ch;
    GetClientSize( &cw, &ch );
    m_grid->SetSize( cw, ch );
#endif
}

// ============================================================================
// BugsGridFrame: a "realistic" table
// ============================================================================

// ----------------------------------------------------------------------------
// bugs table data
// ----------------------------------------------------------------------------

enum Columns
{
    Col_Id,
    Col_Summary,
    Col_Severity,
    Col_Priority,
    Col_Platform,
    Col_Opened,
    Col_Max
};

enum Severity
{
    Sev_Wish,
    Sev_Minor,
    Sev_Normal,
    Sev_Major,
    Sev_Critical,
    Sev_Max
};

static const wxString severities[] =
{
    _T("wishlist"),
    _T("minor"),
    _T("normal"),
    _T("major"),
    _T("critical"),
};

static struct BugsGridData
{
    int id;
    wxChar summary[80];
    Severity severity;
    int prio;
    wxChar platform[12];
    bool opened;
} gs_dataBugsGrid [] =
{
    { 18, _T("foo doesn't work"), Sev_Major, 1, _T("wxMSW"), true },
    { 27, _T("bar crashes"), Sev_Critical, 1, _T("all"), false },
    { 45, _T("printing is slow"), Sev_Minor, 3, _T("wxMSW"), true },
    { 68, _T("Rectangle() fails"), Sev_Normal, 1, _T("wxMSW"), false },
};

static const wxChar *headers[Col_Max] =
{
    _T("Id"),
    _T("Summary"),
    _T("Severity"),
    _T("Priority"),
    _T("Platform"),
    _T("Opened?"),
};

// ----------------------------------------------------------------------------
// BugsGridTable
// ----------------------------------------------------------------------------

wxString BugsGridTable::GetTypeName(const wxSheetCoords& coords)
{
    if (!ContainsGridCell(coords))
        return wxSheetTable::GetTypeName(coords);

    switch ( coords.GetCol() )
    {
        case Col_Id:
        case Col_Priority:
            return wxSHEET_VALUE_NUMBER;

        case Col_Severity:
            // fall thorugh (TODO should be a list)

        case Col_Summary:
            return wxString::Format(_T("%s:80"), wxSHEET_VALUE_STRING);

        case Col_Platform:
            return wxString::Format(_T("%s:all,MSW,GTK,other"), wxSHEET_VALUE_CHOICE);

        case Col_Opened:
            return wxSHEET_VALUE_BOOL;
    }

    wxFAIL_MSG(_T("unknown column"));

    return wxEmptyString;
}

int BugsGridTable::GetNumberRows()
{
    return WXSIZEOF(gs_dataBugsGrid);
}

int BugsGridTable::GetNumberCols()
{
    return Col_Max;
}

bool BugsGridTable::HasValue( const wxSheetCoords& )
{
    return true;
}

wxString BugsGridTable::GetValue( const wxSheetCoords& coords )
{
    switch (coords.GetCellCoordsType())
    {
        case wxSHEET_CELL_GRID :
        {
            const BugsGridData& gd = gs_dataBugsGrid[coords.GetRow()];
            switch ( coords.GetCol() )
            {
                case Col_Id:
                    return wxString::Format(wxT("%d"), gd.id);
                case Col_Priority:
                    return wxString::Format(wxT("%d"), gd.prio);
                case Col_Opened:
                    return wxString::Format(wxT("%d"), gd.opened ? 1 : 0);

                case Col_Severity:
                    return severities[gd.severity];

                case Col_Summary:
                    return gd.summary;

                case Col_Platform:
                    return gd.platform;
            }
        }
        case wxSHEET_CELL_ROWLABEL    : break;
        case wxSHEET_CELL_COLLABEL    : return headers[coords.GetCol()];
        case wxSHEET_CELL_CORNERLABEL : break;
        default : break;
    }

    return wxSheetTable::GetValue(coords);
}

void BugsGridTable::SetValue( const wxSheetCoords& coords, const wxString& value )
{
    switch (coords.GetCellCoordsType())
    {
        case wxSHEET_CELL_GRID :
        {
            BugsGridData& gd = gs_dataBugsGrid[coords.GetRow()];
            switch ( coords.GetCol() )
            {
                case Col_Id:
                case Col_Priority:
                case Col_Opened:
                    wxFAIL_MSG(_T("unexpected column"));
                    break;

                case Col_Severity:
                {
                    size_t n;
                    for ( n = 0; n < WXSIZEOF(severities); n++ )
                    {
                        if ( severities[n] == value )
                        {
                            gd.severity = (Severity)n;
                            break;
                        }
                    }

                    if ( n == WXSIZEOF(severities) )
                    {
                        wxLogWarning(_T("Invalid severity value '%s'."),
                                     value.c_str());
                        gd.severity = Sev_Normal;
                    }
                    break;
                }
                case Col_Summary:
                    wxStrncpy(gd.summary, value, WXSIZEOF(gd.summary));
                    break;

                case Col_Platform:
                    wxStrncpy(gd.platform, value, WXSIZEOF(gd.platform));
                    break;

                default : break;
            }

            return;
        }
        case wxSHEET_CELL_ROWLABEL    : break;
        case wxSHEET_CELL_COLLABEL    : break;
        case wxSHEET_CELL_CORNERLABEL : break;
        default : break;
    }

    // this errors out if we get here (which we shouldn't)
    wxSheetTable::SetValue(coords, value);
}

bool BugsGridTable::CanGetValueAs( const wxSheetCoords& coords, const wxString& typeName )
{
    if (!ContainsGridCell(coords))
        return wxSheetTable::CanGetValueAs(coords, typeName);

    if ( typeName == wxSHEET_VALUE_STRING )
    {
        return true;
    }
    else if ( typeName == wxSHEET_VALUE_BOOL )
    {
        return coords.GetCol() == Col_Opened;
    }
    else if ( typeName == wxSHEET_VALUE_NUMBER )
    {
        return coords.GetCol() == Col_Id || coords.GetCol() == Col_Priority || coords.GetCol() == Col_Severity;
    }
    else
    {
        return false;
    }
}

bool BugsGridTable::CanSetValueAs( const wxSheetCoords& coords, const wxString& typeName )
{
    return CanGetValueAs(coords, typeName);
}

long BugsGridTable::GetValueAsLong( const wxSheetCoords& coords )
{
    if (!ContainsGridCell(coords))
        return wxSheetTable::GetValueAsLong(coords);

    const BugsGridData& gd = gs_dataBugsGrid[coords.GetRow()];

    switch ( coords.GetCol() )
    {
        case Col_Id:
            return gd.id;

        case Col_Priority:
            return gd.prio;

        case Col_Severity:
            return gd.severity;

        default:
            wxFAIL_MSG(_T("unexpected column"));
            return -1;
    }
}

bool BugsGridTable::GetValueAsBool( const wxSheetCoords& coords )
{
    if (!ContainsGridCell(coords))
        return wxSheetTable::GetValueAsBool(coords);

    if ( coords.GetCol() == Col_Opened )
    {
        return gs_dataBugsGrid[coords.GetRow()].opened;
    }
    else
    {
        wxFAIL_MSG(_T("unexpected column"));

        return false;
    }
}

void BugsGridTable::SetValueAsLong( const wxSheetCoords& coords, long value )
{
    if (!ContainsGridCell(coords))
    {
        wxSheetTable::SetValueAsLong(coords, value);
        return;
    }

    BugsGridData& gd = gs_dataBugsGrid[coords.GetRow()];

    switch ( coords.GetCol() )
    {
        case Col_Priority:
            gd.prio = value;
            break;

        default:
            wxFAIL_MSG(_T("unexpected column"));
    }
}

void BugsGridTable::SetValueAsBool( const wxSheetCoords& coords, bool value )
{
    if (!ContainsGridCell(coords))
    {
        wxSheetTable::SetValueAsBool(coords, value);
        return;
    }

    if ( coords.GetCol() == Col_Opened )
    {
        gs_dataBugsGrid[coords.GetRow()].opened = value;
    }
    else
    {
        wxFAIL_MSG(_T("unexpected column"));
    }
}

BugsGridTable::BugsGridTable()
{
}

// ----------------------------------------------------------------------------
// BugsGridFrame
// ----------------------------------------------------------------------------

BugsGridFrame::BugsGridFrame()
             : wxFrame(NULL, -1, _T("Bugs table"),
                       wxDefaultPosition, wxSize(500, 300))
{
    wxSheet *grid = new wxSheet(this, -1, wxDefaultPosition);
    wxSheetTable *table = new BugsGridTable();
    table->SetAttrProvider(new MyGridCellAttrProvider(grid), true);
    grid->SetTable(table, true);

    wxSheetCellAttr attrRO(true),
                    attrRangeEditor(true),
                    attrCombo(true);

    attrRO.SetReadOnly(true);
    attrRangeEditor.SetEditor(wxSheetCellEditor(new wxSheetCellNumberEditorRefData(1, 5)));
    attrCombo.SetEditor(wxSheetCellEditor(new wxSheetCellChoiceEditorRefData(WXSIZEOF(severities),
                                                                     severities)));

    grid->SetGridColAttr(Col_Id, attrRO);
    grid->SetGridColAttr(Col_Priority, attrRangeEditor);
    grid->SetGridColAttr(Col_Severity, attrCombo);

    grid->Fit();
    SetClientSize(grid->GetSize());
}
