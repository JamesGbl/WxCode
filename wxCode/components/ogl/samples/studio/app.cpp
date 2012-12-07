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

#include <wx/ogl/ogl.h> // base header of OGL, includes and adjusts wx/deprecated/setup.h
#include "wxext.h"

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

#ifndef __WXMSW__
    #include "ogl.xpm"
#endif

IMPLEMENT_APP(csApp)

csApp::csApp() : wxAppEx()
{
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
    delete wxDocManager::GetDocumentManager();
}

// Initialise this in OnInit, not statically
bool csApp::OnInit(void)
{
    //CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "program");
    SetAppName(wxT("oglstudio"));
    SetAppDisplayName(wxT("OGL Studio"));
    wxFileSystem::AddHandler(new wxZipFSHandler());
    //bool ok = ::wxInitXRC();
#if wxUSE_MS_HTML_HELP && !defined(__WXUNIVERSAL__)
    m_helpController = new wxWinHelpController();
#else
    m_helpController = new wxHtmlHelpController();
#endif

    m_helpController->Initialize(wxT("studio.hlp"));

    ReadOptions();

    ::wxOGLInitialize();

    InitSymbols();

    //// Create a document manager
    wxDocManager* docManager = new wxDocManager();

    //// Create a template relating drawing documents to their views
    (void) new wxDocTemplate(docManager, wxT("Diagram"), wxT("*.dia"), wxEmptyString, wxT("dia"), wxT("Diagram Doc"), wxT("Diagram View"),
            wxCLASSINFO(csDiagramDocument), wxCLASSINFO(csDiagramView));

    // Create the main frame window.
    // Note that we use a frame style that doesn't have wxCLIP_CHILDREN in it
    // (the default frame style contains wxCLIP_CHILDREN), otherwise the client
    // area doesn't refresh properly when we change its position, under Windows.

#define wxDEFAULT_FRAME_STYLE_NO_CLIP \
    (wxDEFAULT_FRAME_STYLE & ~wxCLIP_CHILDREN)

    csFrame* frame = new csFrame(docManager, NULL, wxID_ANY, GetAppDisplayName(), m_mainFramePos, m_mainFrameSize,
                     wxDEFAULT_FRAME_STYLE_NO_CLIP | wxHSCROLL | wxVSCROLL);

    // Give it an icon
    frame->SetIcon(wxICON(ogl));

    // Make a menubar
    wxMenuBar* menuBar = new wxMenuBar();
    wxMenu* menu;

    // File menu
    menu = new wxMenu();
    menu->Append(wxID_NEW);
    menu->Append(wxID_OPEN);
    menu->AppendSeparator();
    menu->Append(wxID_PRINT_SETUP, _("Print &Setup..."));
    menu->AppendSeparator();
    menu->Append(wxID_EXIT, wxString::Format(wxT("%s\t%s"), 
       wxGetStockLabel(wxID_EXIT).wx_str(),
       wxAcceleratorEntry(wxACCEL_CTRL, 'Q').ToString().wx_str()
       ));
    menuBar->Append(menu, wxGetStockLabel(wxID_FILE));

    // A history of files visited. Use this menu.
    docManager->FileHistoryUseMenu(menu);

    // View menu
    menu = new wxMenu();
    menu->Append(ID_CS_SETTINGS, _("&Settings..."));
    menuBar->Append(menu, _("&View"));

    // Help menu
    menu = new wxMenu();
    menu->Append(wxID_HELP);
    menu->Append(wxID_ABOUT);
    menuBar->Append(menu, wxGetStockLabel(wxID_HELP));

    // Associate the menu bar with the frame
    frame->SetMenuBar(menuBar);

    // Load the file history
    wxConfig config(wxT("OGL Studio"), wxT("wxWidgets"));
    docManager->FileHistoryLoad(config);

#if wxUSE_STATUSBAR
    frame->CreateStatusBar()->PushStatusText(_("Ready"));
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
    m_shapeEditMenu->Append(wxID_PROPERTIES, _("Edit properties"));
    m_shapeEditMenu->AppendSeparator();
    m_shapeEditMenu->Append(ID_CS_ROTATE_CLOCKWISE, _("Rotate clockwise"));
    m_shapeEditMenu->Append(ID_CS_ROTATE_ANTICLOCKWISE, _("Rotate anticlockwise"));
    m_shapeEditMenu->AppendSeparator();
    m_shapeEditMenu->Append(wxID_CUT, _("Cut"));

    frame->Show();

    docManager->CreateDocument(wxEmptyString, wxDOC_NEW);

    return true;
}

int csApp::OnExit(void)
{
    WriteOptions();

    wxDELETE(m_symbolDatabase);
    wxDELETE(m_shapeEditMenu);
    wxDELETE(m_helpController);

    ::wxOGLCleanUp();

    return 0;
}

/*
 * Centralised code for creating a document frame.
 * Called from view.cpp, when a view is created.
 */

wxDocMDIChildFrame* csApp::CreateChildFrame(wxDocument *doc, wxMenu** editMenuRet)
{
    //// Make a child frame
    csMDIChildFrame *subframe = new csMDIChildFrame(doc, wxStaticCast(GetTopWindow(), wxMDIParentFrame), wxID_ANY, wxEmptyString,
                                                    wxPoint(10, 10), wxSize(300, 300));

#ifdef __X__
    subframe->SetIcon(wxIcon(wxT("doc.xbm")));
#else
    subframe->SetIcon(wxICON(ogl));
#endif

    // Make a menubar
    wxMenuBar* menuBar = new wxMenuBar();
    wxMenu* menu;

    // File menu
    menu = new wxMenu();
    menu->Append(wxID_NEW);
    menu->Append(wxID_OPEN);
    menu->Append(wxID_CLOSE, wxString::Format(wxT("%s\t%s"), 
       wxGetStockLabel(wxID_CLOSE).wx_str(),
       wxAcceleratorEntry(wxACCEL_CTRL, 'W').ToString().wx_str()
       ));
    menu->Append(wxID_SAVE);
    menu->Append(wxID_SAVEAS);
    menu->AppendSeparator();
    menu->Append(wxID_PRINT, wxString::Format(wxT("%s\t%s"), 
       wxGetStockLabel(wxID_PRINT).wx_str(),
       wxAcceleratorEntry(wxACCEL_CTRL, 'P').ToString().wx_str()
       ));
    menu->Append(wxID_PRINT_SETUP, _("Print &Setup..."));
    menu->Append(wxID_PREVIEW, wxString::Format(wxT("%s\t%s"), 
       wxString(_("Print Pre&view")).wx_str(),
       wxAcceleratorEntry(wxACCEL_CTRL | wxACCEL_SHIFT, 'P').ToString().wx_str()
       ));

    menu->AppendSeparator();
    menu->Append(wxID_EXIT, wxString::Format(wxT("%s\t%s"), 
       wxGetStockLabel(wxID_EXIT).wx_str(),
       wxAcceleratorEntry(wxACCEL_CTRL, 'Q').ToString().wx_str()
       ));
    menuBar->Append(menu, wxGetStockLabel(wxID_FILE));
    doc->GetDocumentManager()->FileHistoryUseMenu(menu);
    doc->GetDocumentManager()->FileHistoryAddFilesToMenu(menu);

    // Edit menu
    menu = new wxMenu();
    menu->Append(wxID_UNDO);
    menu->Append(wxID_REDO);
    menu->AppendSeparator();
    menu->Append(wxID_CUT);
    menu->Append(wxID_COPY);
    menu->Append(wxID_PASTE);
    menu->Append(wxID_DUPLICATE, wxString::Format(wxT("%s\t%s"), 
       wxString(_("&Duplicate")).wx_str(),
       wxAcceleratorEntry(wxACCEL_CTRL, 'D').ToString().wx_str()
       ));
    menu->AppendSeparator();
    menu->Append(wxID_CLEAR, wxString::Format(wxT("%s\t%s"), 
       wxGetStockLabel(wxID_CLEAR).wx_str(),
       wxAcceleratorEntry(wxACCEL_NORMAL, WXK_DELETE).ToString().wx_str()
       ));
    menu->Append(wxID_SELECTALL, wxString::Format(wxT("%s\t%s"), 
       wxGetStockLabel(wxID_SELECTALL).wx_str(),
       wxAcceleratorEntry(wxACCEL_CTRL, 'A').ToString().wx_str()
       ));
    menu->AppendSeparator();
    menu->Append(wxID_PROPERTIES, _("Edit P&roperties..."));
    menuBar->Append(menu, wxGetStockLabel(wxID_EDIT));
    *editMenuRet = menu;
    doc->GetCommandProcessor()->SetEditMenu(menu);

    // View menu
    menu = new wxMenu();
    menu->Append(ID_CS_SETTINGS, _("&Settings..."));
    menuBar->Append(menu, _("&View"));

    // Help menu
    menu = new wxMenu();
    menu->Append(wxID_HELP, wxString::Format(wxT("%s\t%s"), 
       wxString(_("&Help Contents")).wx_str(),
       wxAcceleratorEntry(wxACCEL_NORMAL, WXK_HELP).ToString().wx_str()
       ));
    menu->Append(wxID_ABOUT);
    menuBar->Append(menu, wxGetStockLabel(wxID_HELP));

    // Associate the menu bar with the frame
    subframe->SetMenuBar(menuBar);

    return subframe;
}

// Creates a canvas. Called by OnInit as a child of the main window
csCanvas *csApp::CreateCanvas(wxView *view)
{
    // Non-retained canvas
    csCanvas *canvas = new csCanvas(wxStaticCast(view, csDiagramView), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);

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

    toolBar->AddTool(wxID_NEW, wxEmptyString, *bitmaps[0], wxNullBitmap, wxITEM_NORMAL, _("New file"));
    toolBar->AddTool(wxID_OPEN, wxEmptyString, *bitmaps[1], wxNullBitmap, wxITEM_NORMAL, _("Open file"));
    toolBar->AddTool(wxID_SAVE, wxEmptyString, *bitmaps[2], wxNullBitmap, wxITEM_NORMAL, _("Save file"));
    toolBar->AddSeparator();
    toolBar->AddTool(wxID_PRINT, wxEmptyString, *bitmaps[6], wxNullBitmap, wxITEM_NORMAL, _("Print"));
    toolBar->AddSeparator();
    toolBar->AddTool(wxID_COPY, wxEmptyString, *bitmaps[3], wxNullBitmap, wxITEM_NORMAL, wxGetStockLabel(wxID_COPY, wxSTOCK_NOFLAGS));
    toolBar->AddTool(wxID_CUT, wxEmptyString, *bitmaps[4], wxNullBitmap, wxITEM_NORMAL, wxGetStockLabel(wxID_CUT, wxSTOCK_NOFLAGS));
    toolBar->AddTool(wxID_PASTE, wxEmptyString, *bitmaps[5], wxNullBitmap, wxITEM_NORMAL, wxGetStockLabel(wxID_PASTE, wxSTOCK_NOFLAGS));
    toolBar->AddSeparator();
    toolBar->AddTool(wxID_UNDO, wxEmptyString, *bitmaps[8], wxNullBitmap, wxITEM_NORMAL, wxGetStockLabel(wxID_UNDO, wxSTOCK_NOFLAGS));
    toolBar->AddTool(wxID_REDO, wxEmptyString, *bitmaps[9], wxNullBitmap, wxITEM_NORMAL, wxGetStockLabel(wxID_REDO, wxSTOCK_NOFLAGS));
    toolBar->AddSeparator();
    toolBar->AddTool(wxID_HELP, wxEmptyString, *bitmaps[7], wxNullBitmap, wxITEM_NORMAL, wxGetStockLabel(wxID_HELP, wxSTOCK_NOFLAGS));

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

    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_ALIGNL, wxEmptyString, *bitmaps[0], wxNullBitmap, wxITEM_NORMAL, _("Align left"));
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_ALIGNR, wxEmptyString, *bitmaps[1], wxNullBitmap, wxITEM_NORMAL, _("Align right"));
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_ALIGNT, wxEmptyString, *bitmaps[2], wxNullBitmap, wxITEM_NORMAL, _("Align top"));
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_ALIGNB, wxEmptyString, *bitmaps[3], wxNullBitmap, wxITEM_NORMAL, _("Align bottom"));
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_ALIGN_HORIZ, wxEmptyString, *bitmaps[4], wxNullBitmap, wxITEM_NORMAL, _("Align horizontally"));
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_ALIGN_VERT, wxEmptyString, *bitmaps[5], wxNullBitmap, wxITEM_NORMAL, _("Align vertically"));
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_COPY_SIZE, wxEmptyString, *bitmaps[6], wxNullBitmap, wxITEM_NORMAL, _("Copy size"));
    m_diagramToolBar->AddSeparator();
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_LINE_ARROW, wxEmptyString, *bitmaps[7], wxNullBitmap, wxITEM_NORMAL, _("Toggle arrow"));
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_NEW_POINT, wxEmptyString, *bitmaps[8], wxNullBitmap, wxITEM_NORMAL, _("New line point"));
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_CUT_POINT, wxEmptyString, *bitmaps[9], wxNullBitmap, wxITEM_NORMAL, _("Cut line point"));
    m_diagramToolBar->AddTool(DIAGRAM_TOOLBAR_STRAIGHTEN, wxEmptyString, *bitmaps[10], wxNullBitmap, wxITEM_NORMAL, _("Straighten lines"));

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

    wxDocManager::GetDocumentManager()->FileHistorySave(config);

    return true;
}
