/////////////////////////////////////////////////////////////////////////////
// Name:        contrib/samples/ogl/ogledit/ogledit.cpp
// Purpose:     OGLEdit sample app
// Author:      Julian Smart
// Modified by:
// Created:     12/07/98
// RCS-ID:      $Id: ogledit.cpp,v 1.1 2007/03/28 15:15:49 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "precomp.h"
#include <wx/aboutdlg.h>

#include "app.h"
#include "palette.h"
#include "doc.h"
#include "view.h"

#ifndef __WXMSW__
    #include "ogl.xpm"
#endif

// A macro needed for some compilers (AIX) that need 'main' to be defined
// in the application itself.
IMPLEMENT_APP(MyApp)

MyApp::MyApp(void) : wxAppEx()
{
}

MyApp::~MyApp(void)
{
   delete wxDocManager::GetDocumentManager();
}

// The `main program' equivalent, creating the windows and returning the
// main frame
bool MyApp::OnInit(void)
{
    // CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "program");
    ::wxOGLInitialize();

    SetAppName(wxT("ogledit"));
    SetAppDisplayName(wxT("OGLEdit Demo"));

    //// Create a document manager
    wxDocManager* docManager = CreateDocManager();

  //// Create the main frame window
  MyFrame* frame = new MyFrame(docManager, NULL, GetAppDisplayName(), wxPoint(0, 0), wxSize(500, 400), wxDEFAULT_FRAME_STYLE);

  //// Give it an icon
  frame->SetIcon(wxICON(ogl));

    // Make a menubar
    wxMenuBar* menu_bar = new wxMenuBar();
    wxMenu* menu;

    menu = new wxMenu();
    menu->Append(wxID_NEW);
    menu->Append(wxID_OPEN);
    menu->Append(wxID_CLOSE, wxString::Format(wxT("%s\t%s"), 
       wxGetStockLabel(wxID_CLOSE).wx_str(),
       wxAcceleratorEntry(wxACCEL_CMD, 'W').ToString().wx_str()
       ));
    menu->Append(wxID_SAVE);
    menu->Append(wxID_SAVEAS);
    menu->AppendSeparator();
    menu->Append(wxID_PRINT, wxString::Format(wxT("%s\t%s"), 
       wxGetStockLabel(wxID_PRINT).wx_str(),
       wxAcceleratorEntry(wxACCEL_CMD, 'P').ToString().wx_str()
       ));
    menu->Append(wxID_PRINT_SETUP, _("Print &Setup..."));
    menu->Append(wxID_PREVIEW, wxString::Format(wxT("%s\t%s"), 
       wxString(_("Print Pre&view")).wx_str(),
       wxAcceleratorEntry(wxACCEL_CMD | wxACCEL_SHIFT, 'P').ToString().wx_str()
       ));
    menu->AppendSeparator();
    menu->Append(wxID_EXIT, wxString::Format(wxT("%s\t%s"), 
       wxGetStockLabel(wxID_EXIT).wx_str(),
       wxAcceleratorEntry(wxACCEL_CMD, 'Q').ToString().wx_str()
       ));
    menu_bar->Append(menu, wxGetStockLabel(wxID_FILE));
    // A nice touch: a history of files visited. Use this menu.
    docManager->FileHistoryUseMenu(menu);

    menu = new wxMenu();
    menu->Append(wxID_UNDO);
    menu->Append(wxID_REDO);
    menu->AppendSeparator();
    menu->Append(wxID_CUT);
    menu->Append(wxID_CLEAR);
    menu->AppendSeparator();
    menu->Append(OGLEDIT_CHANGE_BACKGROUND_COLOUR, _("Change &background colour"));
    menu->Append(OGLEDIT_EDIT_LABEL, _("Edit &label"));
    frame->editMenu = menu;
    menu_bar->Append(menu, wxGetStockLabel(wxID_EDIT));

    menu = new wxMenu();
    menu->Append(OGLEDIT_ABOUT);
    menu_bar->Append(menu, wxGetStockLabel(wxID_HELP));

    frame->canvas = frame->CreateCanvas(NULL, frame);
    frame->palette = wxGetApp().CreatePalette(frame);
    docManager->CreateDocument(wxEmptyString, wxDOC_NEW);

    // Associate the menu bar with the frame
    frame->SetMenuBar(menu_bar);

#if wxUSE_STATUSBAR
    frame->CreateStatusBar()->PushStatusText(_("Ready"));
#endif // wxUSE_STATUSBAR

  frame->Centre();
  frame->Show();

  return true;
}

wxDocManager* MyApp::CreateDocManager()
{
   wxDocManager* docManager = new wxDocManager();
    //// Create a template relating drawing documents to their views
   (void) new wxDocTemplate(docManager, wxT("Diagram"), wxT("*.dia"), wxEmptyString, wxT("dia"), wxT("Diagram Doc"), wxT("Diagram View"),
          wxCLASSINFO(DiagramDocument), wxCLASSINFO(DiagramView));

    // If we've only got one window, we only get to edit
    // one document at a time.
    docManager->SetMaxDocsOpen(1);
    return docManager;
}

int MyApp::OnExit(void)
{
    wxOGLCleanUp();
    return base::OnExit();
}

MyFrame* MyApp::GetMainFrame(void)
{
    return wxStaticCast(GetTopWindow(), MyFrame);
}

/*
 * This is the top-level window of the application.
 */

IMPLEMENT_CLASS(MyFrame, wxDocParentFrame)

BEGIN_EVENT_TABLE(MyFrame, wxDocParentFrame)
    EVT_MENU(OGLEDIT_ABOUT, MyFrame::OnAbout)
    EVT_SIZE(MyFrame::OnSize)
END_EVENT_TABLE()

MyFrame::MyFrame(wxDocManager *manager, wxFrame *frame, const wxString& title,
    const wxPoint& pos, const wxSize& size, long style):
  wxDocParentFrame(manager, frame, wxID_ANY, title, pos, size, style)
{
  canvas = NULL;
  palette = NULL;
  editMenu = NULL;
}

MyFrame::~MyFrame()
{
   ::wxOGLCleanUp();
}

void MyFrame::OnSize(wxSizeEvent& event)
{
  if (canvas && palette)
  {
    wxCoord cw, ch;
    GetClientSize(&cw, &ch);
    wxCoord paletteX = 0;
    wxCoord paletteY = 0;
    wxCoord paletteW = 30;
    wxCoord paletteH = ch;
    wxCoord canvasX = paletteX + paletteW;
    wxCoord canvasY = 0;
    wxCoord canvasW = cw - paletteW;
    wxCoord canvasH = ch;

    palette->SetSize(paletteX, paletteY, paletteW, paletteH);
    canvas->SetSize(canvasX, canvasY, canvasW, canvasH);
  }
  event.Skip();
}

// Intercept menu commands
void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxAboutDialogInfo info;
#if (wxVERSION_NUMBER < 2900)
    info.SetName(wxGetApp().GetAppDisplayName());
#endif
    info.SetCopyright(wxT("Copyright (c) 1996 Julian Smart"));
    info.AddDeveloper(wxT("Julian Smart"));
    info.SetDescription(
       wxString(_("To draw a shape, select a shape on the toolbar and left-click on the canvas.\nTo draw a line, right-drag between shapes\n\n")) +
       wxVERSION_STRING);
    info.SetWebSite(wxT("http://wxcode.sf.net/showcomp.php?name=ogl"));
    info.SetLicense(wxT("wxWindows"));
    ::wxAboutBox(info, this);
}

// Creates a canvas. Called by OnInit as a child of the main window
MyCanvas *MyFrame::CreateCanvas(wxView *view, wxFrame *parent)
{
  wxCoord width, height;
  parent->GetClientSize(&width, &height);

  // Non-retained canvas
  MyCanvas *canvas = new MyCanvas(view, parent, wxID_ANY, wxPoint(0, 0), wxSize(width, height), 0);
  canvas->SetCursor(wxCursor(wxCURSOR_HAND));

  // Give it scrollbars
  canvas->SetScrollbars(20, 20, 50, 50);

  return canvas;
}
