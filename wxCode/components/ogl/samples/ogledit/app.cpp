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
#include "wx/aboutdlg.h"

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

MyApp::MyApp(void)
{
  frame = NULL;
  myDocManager= NULL;
}

// The `main program' equivalent, creating the windows and returning the
// main frame
bool MyApp::OnInit(void)
{
  wxOGLInitialize();

  SetAppName(wxT("OGLEdit Demo"));
  //// Create a document manager
  myDocManager = new wxDocManager;

  //// Create a template relating drawing documents to their views
  (void) new wxDocTemplate(myDocManager, wxT("Diagram"), wxT("*.dia"), wxEmptyString, wxT("dia"), wxT("Diagram Doc"), wxT("Diagram View"),
          CLASSINFO(DiagramDocument), CLASSINFO(DiagramView));

  // If we've only got one window, we only get to edit
  // one document at a time.
  myDocManager->SetMaxDocsOpen(1);

  //// Create the main frame window
  frame = new MyFrame(myDocManager, NULL, GetAppName(), wxPoint(0, 0), wxSize(500, 400), wxDEFAULT_FRAME_STYLE);

  //// Give it an icon
  frame->SetIcon(wxICON(ogl));

  //// Make a menubar
  wxMenu *file_menu = new wxMenu;

  file_menu->Append(wxID_NEW);
  file_menu->Append(wxID_OPEN);
  file_menu->Append(wxID_CLOSE, wxGetStockLabel(wxID_CLOSE) + wxT("\tCtrl-W"));
  file_menu->Append(wxID_SAVE);
  file_menu->Append(wxID_SAVEAS);
  file_menu->AppendSeparator();
  file_menu->Append(wxID_PRINT, wxGetStockLabel(wxID_PRINT) + wxT("\tCtrl-P"));
  file_menu->Append(wxID_PRINT_SETUP, _("Print &Setup..."));
  file_menu->Append(wxID_PREVIEW, _("Print Pre&view"));

  wxMenu *edit_menu = new wxMenu();
  edit_menu->Append(wxID_UNDO);
  edit_menu->Append(wxID_REDO);
  edit_menu->AppendSeparator();
  edit_menu->Append(wxID_CUT);
  edit_menu->AppendSeparator();
  edit_menu->Append(OGLEDIT_CHANGE_BACKGROUND_COLOUR, _("Change &background colour"));
  edit_menu->Append(OGLEDIT_EDIT_LABEL, _("Edit &label"));

  frame->editMenu = edit_menu;

  file_menu->AppendSeparator();
  file_menu->Append(wxID_EXIT, wxGetStockLabel(wxID_EXIT) + wxT("\tCtrl-Q"));

  // A nice touch: a history of files visited. Use this menu.
  myDocManager->FileHistoryUseMenu(file_menu);

  wxMenu *help_menu = new wxMenu();
  help_menu->Append(OGLEDIT_ABOUT);

  wxMenuBar *menu_bar = new wxMenuBar();

  menu_bar->Append(file_menu, _("&File"));
  if (edit_menu)
    menu_bar->Append(edit_menu, _("&Edit"));
  menu_bar->Append(help_menu, _("&Help"));

  frame->canvas = frame->CreateCanvas(NULL, frame);
  frame->palette = wxGetApp().CreatePalette(frame);
  myDocManager->CreateDocument(wxEmptyString, wxDOC_NEW);

  //// Associate the menu bar with the frame
  frame->SetMenuBar(menu_bar);

#if wxUSE_STATUSBAR
  frame->CreateStatusBar(1);
#endif // wxUSE_STATUSBAR

  frame->Centre(wxBOTH);
  frame->Show(true);

  return true;
}

int MyApp::OnExit(void)
{
    wxOGLCleanUp();
    delete myDocManager;
    return 0;
}

/*
 * This is the top-level window of the application.
 */

IMPLEMENT_CLASS(MyFrame, wxDocParentFrame)

BEGIN_EVENT_TABLE(MyFrame, wxDocParentFrame)
    EVT_MENU(OGLEDIT_ABOUT, MyFrame::OnAbout)
    EVT_SIZE(MyFrame::OnSize)
    EVT_CLOSE(MyFrame::OnCloseWindow)
END_EVENT_TABLE()

MyFrame::MyFrame(wxDocManager *manager, wxFrame *frame, const wxString& title,
    const wxPoint& pos, const wxSize& size, long type):
  wxDocParentFrame(manager, frame, wxID_ANY, title, pos, size, type)
{
  canvas = NULL;
  palette = NULL;
  editMenu = NULL;
}

void MyFrame::OnSize(wxSizeEvent& event)
{
  if (canvas && palette)
  {
    int cw, ch;
    GetClientSize(&cw, &ch);
    int paletteX = 0;
    int paletteY = 0;
    int paletteW = 30;
    int paletteH = ch;
    int canvasX = paletteX + paletteW;
    int canvasY = 0;
    int canvasW = cw - paletteW;
    int canvasH = ch;

    palette->SetSize(paletteX, paletteY, paletteW, paletteH);
    canvas->SetSize(canvasX, canvasY, canvasW, canvasH);
  }
  event.Skip();
}

void MyFrame::OnCloseWindow(wxCloseEvent& event)
{
  wxDocParentFrame::OnCloseWindow(event);
  if (!event.GetVeto())
  {
    wxOGLCleanUp();
  }
}

// Intercept menu commands
void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxAboutDialogInfo info;
    info.SetCopyright(wxT("Copyright (c) 1996 Julian Smart"));
    info.AddDeveloper(wxT("Julian Smart"));
    info.SetDescription(
       wxString(_("To draw a shape, select a shape on the toolbar and left-click on the canvas.\nTo draw a line, right-drag between shapes\n\n")) +
       wxVERSION_STRING);
    info.SetWebSite(wxT("http://wxcode.sf.net/showcomp.php?name=ogl"));
    info.SetLicense(wxT("wxWindows"));
    ::wxAboutBox(info);
}

// Creates a canvas. Called by OnInit as a child of the main window
MyCanvas *MyFrame::CreateCanvas(wxView *view, wxFrame *parent)
{
  int width, height;
  parent->GetClientSize(&width, &height);

  // Non-retained canvas
  MyCanvas *canvas = new MyCanvas(view, parent, wxID_ANY, wxPoint(0, 0), wxSize(width, height), 0);
  canvas->SetCursor(wxCursor(wxCURSOR_HAND));

  // Give it scrollbars
  canvas->SetScrollbars(20, 20, 50, 50);

  return canvas;
}

MyFrame *GetMainFrame(void)
{
  return wxGetApp().frame;
}
