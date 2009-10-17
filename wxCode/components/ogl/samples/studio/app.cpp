/////////////////////////////////////////////////////////////////////////////
// Name:        Studio.cpp
// Purpose:     Studio application class
// Author:      Julian Smart
// Modified by:
// Created:     27/7/98
// RCS-ID:      $Id: studio.cpp,v 1.1 2007/03/28 15:15:51 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:
/////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include "wx/ogl/ogl.h" // base header of OGL, includes and adjusts wx/deprecated/setup.h

#include "app.h"
#include "view.h"
#include "doc.h"
#include "mainfrm.h"
#include "cspalette.h"
#include "project.h"
#include "symbols.h"

#include "bitmaps/new.xpm"
#include "bitmaps/open.xpm"
#include "bitmaps/save.xpm"
#include "bitmaps/copy.xpm"
#include "bitmaps/cut.xpm"
#include "bitmaps/paste.xpm"
#include "bitmaps/print.xpm"
#include "bitmaps/help.xpm"
#include "bitmaps/undo.xpm"
#include "bitmaps/redo.xpm"

#include "bitmaps/alignl.xpm"
#include "bitmaps/alignr.xpm"
#include "bitmaps/alignt.xpm"
#include "bitmaps/alignb.xpm"
#include "bitmaps/horiz.xpm"
#include "bitmaps/vert.xpm"
#include "bitmaps/copysize.xpm"
#include "bitmaps/linearrow.xpm"
#include "bitmaps/newpoint.xpm"
#include "bitmaps/cutpoint.xpm"
#include "bitmaps/straight.xpm"

#include "studio.xpm"

IMPLEMENT_APP(csApp)

csApp::csApp()
{
    m_docManager = NULL;
    m_diagramPalette = NULL;
    m_diagramToolBar = NULL;
    m_projectTreeCtrl = NULL;
    m_diagramPaletteSashWindow = NULL;
    m_projectSashWindow = NULL;
    m_symbolDatabase = NULL;
    m_pointSizeComboBox = NULL;
    m_zoomComboBox = NULL;
    m_shapeEditMenu = NULL;

    // Configuration
    m_mainFramePos.x = 20;
    m_mainFramePos.y = 20;
    m_mainFrameSize.x = 500;
    m_mainFrameSize.y = 400;
    m_gridStyle = csGRID_STYLE_INVISIBLE;
    m_gridSpacing = 5;
}

csApp::~csApp()
{
}

// Initialise this in OnInit, not statically
bool csApp::OnInit(void)
{
#if wxUSE_WX_RESOURCES
    if (!wxResourceParseFile(wxT("studio_resources.wxr")))
    {
        wxMessageBox(wxT("Could not find or parse resource file: studio_resources.wxr"), wxT("Studio"));
        return false;
    }
#endif
    SetAppName(wxT("OGL Studio"));
#if wxUSE_MS_HTML_HELP && !defined(__WXUNIVERSAL__)
    m_helpController = new wxWinHelpController;
#else
    m_helpController = new wxHtmlHelpController;
#endif

    m_helpController->Initialize(wxT("studio.hlp"));

    ReadOptions();

    wxOGLInitialize();

    InitSymbols();

    //// Create a document manager
    m_docManager = new wxDocManager;

    //// Create a template relating drawing documents to their views
    (void) new wxDocTemplate(m_docManager, wxT("Diagram"), wxT("*.dia"), wxEmptyString, wxT("dia"), wxT("Diagram Doc"), wxT("Diagram View"),
            CLASSINFO(csDiagramDocument), CLASSINFO(csDiagramView));

    // Create the main frame window.
    // Note that we use a frame style that doesn't have wxCLIP_CHILDREN in it
    // (the default frame style contains wxCLIP_CHILDREN), otherwise the client
    // area doesn't refresh properly when we change its position, under Windows.

#define wxDEFAULT_FRAME_STYLE_NO_CLIP \
    (wxDEFAULT_FRAME_STYLE & ~wxCLIP_CHILDREN)

    csFrame* frame = new csFrame(m_docManager, NULL, wxID_ANY, GetAppName(), m_mainFramePos, m_mainFrameSize,
                     wxDEFAULT_FRAME_STYLE_NO_CLIP | wxHSCROLL | wxVSCROLL);

    // Give it an icon
    frame->SetIcon(wxIcon(studio_xpm));

    // Make a menubar
    wxMenu *fileMenu = new wxMenu;

    fileMenu->Append(wxID_NEW, wxT("&New...\tCtrl+N"));
    fileMenu->Append(wxID_OPEN, wxT("&Open...\tCtrl+O"));

    fileMenu->AppendSeparator();

    fileMenu->Append(wxID_PRINT, wxT("&Print...\tCtrl+P"));
    fileMenu->Append(wxID_PRINT_SETUP, wxT("Print &Setup..."));
    fileMenu->Append(wxID_PREVIEW, wxT("Print Pre&view"));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, wxT("E&xit"));

    // A history of files visited. Use this menu.
    m_docManager->FileHistoryUseMenu(fileMenu);

    wxMenu *viewMenu = new wxMenu;
    viewMenu->Append(ID_CS_SETTINGS, wxT("&Settings..."));

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_HELP, wxT("&Help Contents\tF1"));
    helpMenu->Append(ID_CS_ABOUT, wxT("&About"));

    wxMenuBar *menuBar = new wxMenuBar;

    menuBar->Append(fileMenu, wxT("&File"));
    menuBar->Append(viewMenu, wxT("&View"));
    menuBar->Append(helpMenu, wxT("&Help"));

    // Associate the menu bar with the frame
    frame->SetMenuBar(menuBar);

    // Load the file history
    wxConfig config(wxT("OGL Studio"), wxT("wxWidgets"));
    m_docManager->FileHistoryLoad(config);

#if wxUSE_STATUSBAR
    frame->CreateStatusBar();
#endif // wxUSE_STATUSBAR

    // The ordering of these is important for layout purposes
    CreateDiagramToolBar(frame);
    CreatePalette(frame);

    /*
    CreateProjectWindow(frame);
    FillProjectTreeCtrl();
    */

    // Create the shape editing menu
    m_shapeEditMenu = new ShapeEditMenu;
    m_shapeEditMenu->Append(ID_CS_EDIT_PROPERTIES, wxT("Edit properties"));
    m_shapeEditMenu->AppendSeparator();
    m_shapeEditMenu->Append(ID_CS_ROTATE_CLOCKWISE, wxT("Rotate clockwise"));
    m_shapeEditMenu->Append(ID_CS_ROTATE_ANTICLOCKWISE, wxT("Rotate anticlockwise"));
    m_shapeEditMenu->AppendSeparator();
    m_shapeEditMenu->Append(ID_CS_CUT, wxT("Cut"));

    frame->Show(true);

    SetTopWindow(frame);

    return true;
}

int csApp::OnExit(void)
{
    WriteOptions();

    delete m_symbolDatabase;
    m_symbolDatabase = NULL;

    delete m_docManager;
    m_docManager = NULL;

    delete m_shapeEditMenu;
    m_shapeEditMenu = NULL;

    delete m_helpController;
    m_helpController = NULL;

    wxOGLCleanUp();

    return 0;
}

/*
 * Centralised code for creating a document frame.
 * Called from view.cpp, when a view is created.
 */

wxMDIChildFrame *csApp::CreateChildFrame(wxDocument *doc, wxView *view, wxMenu** editMenuRet)
{
    //// Make a child frame
    csMDIChildFrame *subframe = new csMDIChildFrame(doc, view, ((wxDocMDIParentFrame*)GetTopWindow()), wxID_ANY, wxT("Child Frame"),
                                                    wxPoint(10, 10), wxSize(300, 300), wxDEFAULT_FRAME_STYLE);

#ifdef __WXMSW__
    subframe->SetIcon(wxString(wxT("chart")));
#endif
#ifdef __X__
    subframe->SetIcon(wxIcon(wxT("doc.xbm")));
#endif

    //// Make a menubar
    wxMenu *fileMenu = new wxMenu;

    fileMenu->Append(wxID_NEW, wxT("&New...\tCtrl+N"));
    fileMenu->Append(wxID_OPEN, wxT("&Open...\tCtrl+O"));
    fileMenu->Append(wxID_CLOSE, wxT("&Close\tCtrl+W"));
    fileMenu->Append(wxID_SAVE, wxT("&Save\tCtrl+S"));
    fileMenu->Append(wxID_SAVEAS, wxT("Save &As...\tF12"));

    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_PRINT, wxT("&Print...\tCtrl+P"));
    fileMenu->Append(wxID_PRINT_SETUP, wxT("Print &Setup..."));
    fileMenu->Append(wxID_PREVIEW, wxT("Print Pre&view"));

    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, wxT("E&xit"));

    wxMenu* editMenu = new wxMenu;
    editMenu->Append(wxID_UNDO, wxT("&Undo\tCtrl+Z"));
    editMenu->Append(wxID_REDO, wxT("&Redo\tCtrl+Y"));
    editMenu->AppendSeparator();
    editMenu->Append(wxID_CUT, wxT("Cu&t\tCtrl+X"));
    editMenu->Append(wxID_COPY, wxT("&Copy\tCtrl+C"));
    editMenu->Append(wxID_PASTE, wxT("&Paste\tCtrl+V"));
    editMenu->Append(wxID_DUPLICATE, wxT("&Duplicate\tCtrl+D"));
    editMenu->AppendSeparator();
    editMenu->Append(wxID_CLEAR, wxT("Cle&ar\tDelete"));
    editMenu->Append(ID_CS_SELECT_ALL, wxT("Select A&ll\tCtrl+A"));
    editMenu->AppendSeparator();
    editMenu->Append(ID_CS_EDIT_PROPERTIES, wxT("Edit P&roperties..."));

    *editMenuRet = editMenu;

    m_docManager->FileHistoryUseMenu(fileMenu);
    m_docManager->FileHistoryAddFilesToMenu(fileMenu);

    doc->GetCommandProcessor()->SetEditMenu(editMenu);

    wxMenu *viewMenu = new wxMenu;
    viewMenu->Append(ID_CS_SETTINGS, wxT("&Settings..."));

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_HELP, wxT("&Help Contents\tF1"));
    helpMenu->Append(ID_CS_ABOUT, wxT("&About"));

    wxMenuBar *menuBar = new wxMenuBar;

    menuBar->Append(fileMenu, wxT("&File"));
    menuBar->Append(editMenu, wxT("&Edit"));
    menuBar->Append(viewMenu, wxT("&View"));
    menuBar->Append(helpMenu, wxT("&Help"));

    //// Associate the menu bar with the frame
    subframe->SetMenuBar(menuBar);

    return subframe;
}

// Creates a canvas. Called by OnInit as a child of the main window
csCanvas *csApp::CreateCanvas(wxView *view, wxMDIChildFrame *parent)
{
    int width, height;
    parent->GetClientSize(&width, &height);

    // Non-retained canvas
    csCanvas *canvas = new csCanvas((csDiagramView*) view, parent, 1000, wxPoint(0, 0), wxSize(width, height), wxSUNKEN_BORDER);

    wxColour bgColour(wxT("WHITE"));
    canvas->SetBackgroundColour(bgColour);

    wxCursor cursor(wxCURSOR_HAND);
    canvas->SetCursor(cursor);

    // Give it scrollbars
    canvas->SetScrollbars(20, 20, 100, 100);

    return canvas;
}

void csApp::InitToolBar(wxToolBar* toolBar)
{
    wxBitmap* bitmaps[10];

    bitmaps[0] = new wxBitmap( new_xpm );
    bitmaps[1] = new wxBitmap( open_xpm );
    bitmaps[2] = new wxBitmap( save_xpm );
    bitmaps[3] = new wxBitmap( copy_xpm );
    bitmaps[4] = new wxBitmap( cut_xpm );
    bitmaps[5] = new wxBitmap( paste_xpm );
    bitmaps[6] = new wxBitmap( print_xpm );
    bitmaps[7] = new wxBitmap( help_xpm );
    bitmaps[8] = new wxBitmap( undo_xpm );
    bitmaps[9] = new wxBitmap( redo_xpm );

    toolBar->AddTool(wxID_NEW, *bitmaps[0], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("New file"));
    toolBar->AddTool(wxID_OPEN, *bitmaps[1], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("Open file"));
    toolBar->AddTool(wxID_SAVE, *bitmaps[2], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("Save file"));
    toolBar->AddSeparator();
    toolBar->AddTool(wxID_PRINT, *bitmaps[6], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("Print"));
    toolBar->AddSeparator();
    toolBar->AddTool(wxID_COPY, *bitmaps[3], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("Copy"));
    toolBar->AddTool(wxID_CUT, *bitmaps[4], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("Cut"));
    toolBar->AddTool(wxID_PASTE, *bitmaps[5], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("Paste"));
    toolBar->AddSeparator();
    toolBar->AddTool(wxID_UNDO, *bitmaps[8], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("Undo"));
    toolBar->AddTool(wxID_REDO, *bitmaps[9], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("Redo"));
    toolBar->AddSeparator();
    toolBar->AddTool(wxID_HELP, *bitmaps[7], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("Help"));

    toolBar->Realize();

    toolBar->EnableTool(wxID_COPY, false);
    toolBar->EnableTool(wxID_PASTE, false);
    toolBar->EnableTool(wxID_PRINT, false);
    toolBar->EnableTool(wxID_UNDO, false);
    toolBar->EnableTool(wxID_REDO, false);

    int i;
    for (i = 0; i < 10; i++)
        delete bitmaps[i];
}

// Create and initialise the diagram toolbar
void csApp::CreateDiagramToolBar(wxFrame* parent)
{
    // First create a layout window
    wxSashLayoutWindow* win = new wxSashLayoutWindow(parent, ID_LAYOUT_WINDOW_DIAGRAM_TOOLBAR, wxDefaultPosition, wxSize(200, 30), wxNO_BORDER|wxSW_3D|wxCLIP_CHILDREN);
    win->SetDefaultSize(wxSize(10000, 30));
    win->SetOrientation(wxLAYOUT_HORIZONTAL);
    win->SetAlignment(wxLAYOUT_TOP);
    win->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));

    m_diagramToolBarSashWindow = win;
    m_diagramToolBarSashWindow->Show(false);

    // Create the actual toolbar
    m_diagramToolBar = new wxToolBar(win, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxNO_BORDER|wxTB_FLAT);

    wxBitmap* bitmaps[11];

    bitmaps[0] = new wxBitmap( alignl_xpm );
    bitmaps[1] = new wxBitmap( alignr_xpm );
    bitmaps[2] = new wxBitmap( alignt_xpm );
    bitmaps[3] = new wxBitmap( alignb_xpm );
    bitmaps[4] = new wxBitmap( horiz_xpm );
    bitmaps[5] = new wxBitmap( vert_xpm );
    bitmaps[6] = new wxBitmap( copysize_xpm );
    bitmaps[7] = new wxBitmap( linearrow_xpm );
    bitmaps[8] = new wxBitmap( newpoint_xpm );
    bitmaps[9] = new wxBitmap( cutpoint_xpm );
    bitmaps[10] = new wxBitmap( straight_xpm );

    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_ALIGNL, *bitmaps[0], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("Align left"));
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_ALIGNR, *bitmaps[1], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("Align right"));
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_ALIGNT, *bitmaps[2], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("Align top"));
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_ALIGNB, *bitmaps[3], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("Align bottom"));
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_ALIGN_HORIZ, *bitmaps[4], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("Align horizontally"));
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_ALIGN_VERT, *bitmaps[5], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("Align vertically"));
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_COPY_SIZE, *bitmaps[6], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("Copy size"));
    m_diagramToolBar->AddSeparator();
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_LINE_ARROW, *bitmaps[7], wxNullBitmap, true, wxDefaultCoord, wxDefaultCoord, NULL, wxT("Toggle arrow"));
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_NEW_POINT, *bitmaps[8], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("New line point"));
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_CUT_POINT, *bitmaps[9], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("Cut line point"));
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_STRAIGHTEN, *bitmaps[10], wxNullBitmap, false, wxDefaultCoord, wxDefaultCoord, NULL, wxT("Straighten lines"));

    m_diagramToolBar->Realize();

    int i;
    for (i = 0; i < 11; i++)
        delete bitmaps[i];

    // Create a combobox for point size
    int maxPointSize = 40;
    wxString *pointSizes = new wxString[maxPointSize];
    for (i = 1; i <= maxPointSize; i++)
    {
        pointSizes[i-1].Printf(wxT("%d"), i);
    }

    int controlX = 260;
    int pointSizeW = 40;
    int pointSizeH = 18;
    int zoomW = 60;
    int zoomH = 18;
#ifdef __WXMOTIF__
    controlX += 75;
    pointSizeW = 60;
    pointSizeH = 22;
    zoomW = 60;
    zoomH = 22;
#endif

    m_pointSizeComboBox = new wxComboBox(m_diagramToolBar, ID_WINDOW_POINT_SIZE_COMBOBOX,
        wxEmptyString, wxPoint(controlX, 1), wxSize(pointSizeW, pointSizeH), maxPointSize, pointSizes);
    delete[] pointSizes;

#ifdef __WXGTK__
    m_diagramToolBar->AddControl(m_pointSizeComboBox);
#endif

    m_pointSizeComboBox->SetSelection(10 - 1);

    // Create a combobox for zooming
    int maxZoom = 200;
    int minZoom = 5;
    int increment = 5;
    int noStrings = (maxZoom - minZoom)/5 ;
    wxString *zoomStrings = new wxString[noStrings];
    for (i = 0; i < noStrings; i ++)
    {
        zoomStrings[noStrings - i - 1].Printf(wxT("%d%%"), (i*increment + minZoom));
    }

    controlX += pointSizeW + 10;

    m_zoomComboBox = new wxComboBox(m_diagramToolBar, ID_WINDOW_ZOOM_COMBOBOX,
        wxEmptyString, wxPoint(controlX, 1), wxSize(zoomW, zoomH), noStrings, zoomStrings);
    delete[] zoomStrings;

#ifdef __WXGTK__
    m_diagramToolBar->AddControl(m_zoomComboBox);
#endif

    // i = (zoom - minZoom)/increment
    // index = noStrings - i - 1
    // 100%
    i = (100 - minZoom)/increment;
    m_zoomComboBox->SetSelection(noStrings - i - 1);
}

// Read/write configuration information
bool csApp::ReadOptions()
{
    wxConfig config(wxT("OGL Studio"), wxT("wxWidgets"));

    config.Read(wxT("mainX"), & m_mainFramePos.x);
    config.Read(wxT("mainY"), & m_mainFramePos.y);
    config.Read(wxT("mainWidth"), & m_mainFrameSize.x);
    config.Read(wxT("mainHeight"), & m_mainFrameSize.y);
    config.Read(wxT("gridStyle"), & m_gridStyle);
    config.Read(wxT("gridSpacing"), & m_gridSpacing);

    return true;
}

bool csApp::WriteOptions()
{
    wxConfig config(wxT("OGL Studio"), wxT("wxWidgets"));

    config.Write(wxT("mainX"), (long) m_mainFramePos.x);
    config.Write(wxT("mainY"), (long) m_mainFramePos.y);
    config.Write(wxT("mainWidth"), (long) m_mainFrameSize.x);
    config.Write(wxT("mainHeight"), (long) m_mainFrameSize.y);
    config.Write(wxT("gridStyle"), (long) m_gridStyle);
    config.Write(wxT("gridSpacing"), (long) m_gridSpacing);

    m_docManager->FileHistorySave(config);

    return true;
}
