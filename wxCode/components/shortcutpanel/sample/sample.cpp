// OutlookBar : wxWindows project sample

#include <wx/wx.h>
#include <wx/splitter.h>

#include "Shortcut.h"
#include "ShortcutGroup.h"
#include "ShortcutPanel.h"
#include "one.xpm"
#include "exit.xpm"
#include "three.xpm"
#include "four.xpm"

#define ID_CMD_ONE   10000
#define ID_CMD_THREE 10002
#define ID_CMD_FOUR  10003


///////////////////////////////////// DECLARATION ///////////////////////////////////////

class ShortcutPanelApp : public wxApp
{
public:
	virtual bool OnInit();
};

class ShortcutPanelFrame : public wxFrame
{
public:
  ShortcutPanelFrame(const wxString& strTitle);
  
  void OnOne(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);
  void OnThree(wxCommandEvent& event);
  void OnFour(wxCommandEvent& event);

  DECLARE_EVENT_TABLE()
};

///////////////////////////////////// IMPLEMENTATION ///////////////////////////////////////

IMPLEMENT_APP(ShortcutPanelApp)

BEGIN_EVENT_TABLE(ShortcutPanelFrame, wxFrame)
  EVT_MENU(ID_CMD_ONE, ShortcutPanelFrame::OnOne)
  EVT_MENU(wxID_EXIT, ShortcutPanelFrame::OnExit)
  EVT_MENU(ID_CMD_THREE, ShortcutPanelFrame::OnThree)
  EVT_MENU(ID_CMD_FOUR, ShortcutPanelFrame::OnFour)
END_EVENT_TABLE()
  
bool ShortcutPanelApp::OnInit()
{
	ShortcutPanelFrame *frame = new ShortcutPanelFrame(_("ShortcutPanel Sample"));

	SetTopWindow(frame);
	frame->Show(true);
	
	return true;
}

ShortcutPanelFrame::ShortcutPanelFrame(const wxString& strTitle)
  : wxFrame(NULL, wxID_ANY, strTitle, wxDefaultPosition, wxSize(450, 350))
{
  wxSplitterWindow* pSplitter = new wxSplitterWindow( this, -1, wxDefaultPosition, wxSize(100, 100), wxSP_3DBORDER|wxSP_3DSASH|wxNO_BORDER );
  ShortcutPanel* pShortCutBar = new ShortcutPanel(pSplitter, -1, wxDefaultPosition, wxSize(100, 400));
  wxPanel* pPanel = new wxPanel( pSplitter, -1, wxDefaultPosition, wxSize(100, 80), wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  pSplitter->SplitVertically(pShortCutBar, pPanel, 150);
  
  pShortCutBar->AddGroup(_("First"));
  pShortCutBar->AddGroup(_("Second"));
  pShortCutBar->AddGroup(_("Third"));
  int FirstGroupId = pShortCutBar->FindGroup(_("First"));
  int SecondGroupId = pShortCutBar->FindGroup(_("Second"));
  int ThirdGroupId = pShortCutBar->FindGroup(_("Third"));
  
  wxBitmap* pBitmapOne = new wxBitmap(one_xpm);
  wxBitmap* pBitmapTwo = new wxBitmap(exit_xpm);
  wxBitmap* pBitmapThree = new wxBitmap(three_xpm);
  wxBitmap* pBitmapFour = new wxBitmap(four_xpm);
  
  pShortCutBar->AddShortcut(FirstGroupId, new Shortcut(ID_CMD_ONE, wxString(_("One")), *pBitmapOne));
  pShortCutBar->AddShortcut(FirstGroupId, new Shortcut(wxID_EXIT, wxString(_("Exit")), *pBitmapTwo));
  pShortCutBar->AddShortcut(FirstGroupId, new Shortcut(ID_CMD_THREE, wxString(_("Three")), *pBitmapThree));
  pShortCutBar->AddShortcut(SecondGroupId, new Shortcut(ID_CMD_FOUR, wxString(_("Four")), *pBitmapFour));

  delete pBitmapOne;
  delete pBitmapTwo;
  delete pBitmapThree;
  delete pBitmapFour;
  
  pShortCutBar->SelectGroup(FirstGroupId);
}

void ShortcutPanelFrame::OnOne(wxCommandEvent& event)
{
  ::wxMessageBox("One");
}

void ShortcutPanelFrame::OnExit(wxCommandEvent& event)
{
  Close(true);
}

void ShortcutPanelFrame::OnThree(wxCommandEvent& event)
{
  ::wxMessageBox("Three");
}

void ShortcutPanelFrame::OnFour(wxCommandEvent& event)
{
  ::wxMessageBox("Four");
}

