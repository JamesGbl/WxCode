// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources)
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #include "mondrian.xpm"
#endif

#include "wxSpellCheckerDemo.h"

#include "MySpellCheckDialog.h"
#include "SentryLikeDialog.h"
#include "Outlook97LikeDialog.h"
#include "XmlSpellCheckDialog.h"
#include "AspellInterface.h"
#include "MySpellInterface.h"
#include "SpellCheckerOptionsDialog.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    FileNew = wxID_NEW,
    FileOpen = wxID_OPEN,
    FileQuit = wxID_EXIT,
    FileSpellCheckMozillaUI = 10000,
    FileSpellCheckOutlookUI = 10001,
    FileSpellCheckSentryUI = 10002,
    FileSpellCheckXmlUI = 10003,
    FileSpellCheckRightClick = 10004,
    FileUseAspell = 10005,
    FileUseMySpell = 10006,
    EditOptions = 10007
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(FileNew, MyFrame::OnNew)
    EVT_MENU(FileOpen, MyFrame::OnOpen)
    EVT_MENU(FileQuit,  MyFrame::OnQuit)
    EVT_MENU(FileUseAspell, MyFrame::OnUseAspell)
    EVT_UPDATE_UI(FileUseAspell, MyFrame::OnUpdateUseAspell)
    EVT_MENU(FileUseMySpell, MyFrame::OnUseMySpell)
    EVT_UPDATE_UI(FileUseMySpell, MyFrame::OnUpdateUseMySpell)
    EVT_MENU(FileSpellCheckMozillaUI, MyFrame::OnSpellCheckMozillaUI)
    EVT_MENU(FileSpellCheckOutlookUI, MyFrame::OnSpellCheckOutlookUI)
    EVT_MENU(FileSpellCheckSentryUI, MyFrame::OnSpellCheckSentryUI)
    EVT_MENU(FileSpellCheckXmlUI, MyFrame::OnSpellCheckXmlUI)
    EVT_MENU(EditOptions, MyFrame::OnEditOptions)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // create the main application window
    MyFrame *frame = new MyFrame(_T("wxSpellChecker Demo"));

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title)
{
    m_nSelectedSpellCheckEngine = MyFrame::USE_ASPELL;
    
    // set the frame icon
    SetIcon(wxICON(mondrian));

    textCtrl = new wxTextCtrl(this, -1, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    textCtrl->SetFocus();
    
    // create a menu bar
    wxMenu* menuFile = new wxMenu;

    menuFile->Append(FileNew, _T("New\tCtrl+N"), _T("Clear out the existing text"));
    menuFile->Append(FileOpen, _T("Open\tCtrl+O"), _T("Open an existing document"));
    menuFile->AppendSeparator();
    menuFile->AppendCheckItem(FileUseAspell, _T("Use Aspell"), _T("Spell check using the Aspell engine"));
    menuFile->AppendCheckItem(FileUseMySpell, _T("Use MySpell"), _T("Spell check using the MySpell engine"));
    menuFile->AppendSeparator();
    menuFile->Append(FileSpellCheckMozillaUI, _T("SpellCheck Mozilla-style\tF5"), _T("SpellCheck the current document using the Mozilla spellchecker look-and-feel"));
    menuFile->Append(FileSpellCheckOutlookUI, _T("SpellCheck Outlook-style\tF6"), _T("SpellCheck the current document using the Outlook 97 spellchecker look-and-feel"));
    menuFile->Append(FileSpellCheckSentryUI, _T("SpellCheck Sentry-style\tF7"), _T("SpellCheck the current document using the Wintertree Sentry spellchecker look-and-feel"));
    menuFile->Append(FileSpellCheckXmlUI, _T("SpellCheck XML-style\tF8"), _T("SpellCheck the current document using the XML dialog interface"));
    menuFile->AppendSeparator();
    menuFile->Append(FileQuit, _T("E&xit\tCtrl+Q"), _T("Quit this program"));
    
    wxMenu* menuEdit = new wxMenu;
    menuEdit->Append(EditOptions, _T("&Options"), _T("Edit the options for the currently selected spell checking engine"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, _T("&File"));
    menuBar->Append(menuEdit, _T("&Edit"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

    // create a status bar
    CreateStatusBar();
    SetStatusText(_T("Please type something in the text area or load a file to spell check"));
    
    m_bSpellCheckOnRightClick = false;

    m_pAspellInterface = new AspellInterface();
    m_pMySpellInterface = new MySpellInterface();
}



MyFrame::~MyFrame()
{
    if (m_pAspellInterface)
      delete m_pAspellInterface;
    
    if (m_pMySpellInterface)
      delete m_pMySpellInterface;
}

// event handlers
void MyFrame::OnUseAspell(wxCommandEvent& event)
{
  m_nSelectedSpellCheckEngine = MyFrame::USE_ASPELL;
}

void MyFrame::OnUpdateUseAspell(wxUpdateUIEvent& event)
{
  event.Check(m_nSelectedSpellCheckEngine == MyFrame::USE_ASPELL);
}

void MyFrame::OnUseMySpell(wxCommandEvent& event)
{
  m_nSelectedSpellCheckEngine = MyFrame::USE_MYSPELL;
}

void MyFrame::OnUpdateUseMySpell(wxUpdateUIEvent& event)
{
  event.Check(m_nSelectedSpellCheckEngine == MyFrame::USE_MYSPELL);
}

void MyFrame::OnNew(wxCommandEvent& WXUNUSED(event))
{
  if (textCtrl)
    textCtrl->Clear();
}

void MyFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
{
  wxString filename = ::wxFileSelector(_T("Please select a file"));
  if (!filename.empty())
  {
    if (textCtrl)
      textCtrl->LoadFile(filename);
  }
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnSpellCheckMozillaUI(wxCommandEvent& event)
{
  wxSpellCheckEngineInterface* pSpellChecker = ReturnSelectedSpellCheckEngine();
  if (pSpellChecker)
  {
    pSpellChecker->SetSpellCheckUserInterface(new MySpellingDialog(NULL));
    SpellCheck(pSpellChecker);
  }
}

void MyFrame::OnSpellCheckOutlookUI(wxCommandEvent& event)
{
  wxSpellCheckEngineInterface* pSpellChecker = ReturnSelectedSpellCheckEngine();
  if (pSpellChecker)
  {
    pSpellChecker->SetSpellCheckUserInterface(new Outlook97LikeDialog(NULL));
    SpellCheck(pSpellChecker);
  }
}

void MyFrame::OnSpellCheckSentryUI(wxCommandEvent& event)
{
  wxSpellCheckEngineInterface* pSpellChecker = ReturnSelectedSpellCheckEngine();
  if (pSpellChecker)
  {
    pSpellChecker->SetSpellCheckUserInterface(new SentryLikeDialog(NULL));
    SpellCheck(pSpellChecker);
  }
}

void MyFrame::OnSpellCheckXmlUI(wxCommandEvent& event)
{
  //::wxMessageBox(_T("XML Spell Checking interface not implemented yet"));

  wxSpellCheckEngineInterface* pSpellChecker = ReturnSelectedSpellCheckEngine();
  if (pSpellChecker)
  {
    pSpellChecker->SetSpellCheckUserInterface(new XmlSpellCheckDialog(NULL, "ID_DIALOG", "ID_DIALOG"));
    SpellCheck(pSpellChecker);
  }
}

void MyFrame::OnEditOptions(wxCommandEvent& event)
{
  // Create a really basic dialog that gets dynamically populated
  // with controls based on the m_pSpellCheckEngine->GetOptions();
  SpellCheckerOptionsDialog OptionsDialog(this, "Options", ReturnSelectedSpellCheckEngine()->GetOptions());
  if (OptionsDialog.ShowModal() == wxID_OK)
  {
    // Set the modified options
    OptionsMap* pOptionsMap = OptionsDialog.GetModifiedOptions();
    if (pOptionsMap)
    {
      for (OptionsMap::iterator it = pOptionsMap->begin(); it != pOptionsMap->end(); it++)
        ReturnSelectedSpellCheckEngine()->AddOptionToMap(it->second);
    }
  }
}

void MyFrame::SpellCheck(wxSpellCheckEngineInterface* pSpellChecker)
{
  long selectionFrom = -1, selectionTo = -1;
  wxString strText = textCtrl->GetStringSelection();
  if (!strText.empty())
  {
    // Get the range of the text controls's string value to replace
    textCtrl->GetSelection(&selectionFrom, &selectionTo);
  }
  else
  {
    // Since there was no text selected, grab the entire contents of the text control
    strText = textCtrl->GetValue();
  }
  
  pSpellChecker->InitializeSpellCheckEngine();
  wxString strNewText = pSpellChecker->CheckSpelling(strText);

  if (!strNewText.empty())  // If the new text string is empty, then assume that the user cancelled
  {
    if ((selectionFrom != -1) && (selectionTo != -1))
    {
      textCtrl->Replace(selectionFrom, selectionTo, strNewText);
    }
    else
    {
      textCtrl->SetValue(strNewText);
    }
  }
}

wxSpellCheckEngineInterface* MyFrame::ReturnSelectedSpellCheckEngine()
{
  wxSpellCheckEngineInterface* pEngine = NULL;
  if (m_nSelectedSpellCheckEngine == MyFrame::USE_ASPELL)
    pEngine = m_pAspellInterface;
  else if (m_nSelectedSpellCheckEngine == MyFrame::USE_MYSPELL)
    pEngine = m_pMySpellInterface;

  return pEngine;
}
