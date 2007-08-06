
// Include std wx things
#include <wx/wxprec.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

// We need some more
#include "wx/log.h"
#include "wx/textctrl.h"
#include "wx/treectrl.h"

// These are for the layout
#include "wx/RlManager.h"
#include "wx/RlPainter.h"


// Style: 2 pixel sunken border, on all sides
#define RL_FRAME_MGR_STYLE (wxSUNKEN_BORDER|wxBORDER_DOUBLE|wxALL_DIR)

class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title) :
            // Initialize wxRlManager with a temporary background item
            wxFrame( NULL, -1, title ),
            m_rl_mgr(this,RL_FRAME_MGR_STYLE,RlClientTempBg)
    {
        // Creates layout
        Init( );
    }
    void Init();

private:
    wxRlManager m_rl_mgr;           // The layout manager
    RlWinStdPainter m_rl_painter;   // The painter

    void OnQuit(wxCommandEvent& event){
        Close( );
    }

    void OnAbout(wxCommandEvent& event){
        wxString msg = _T("wxRlLayout sample");
        wxMessageBox(msg, _("Welcome to..."));
    }

    DECLARE_EVENT_TABLE();
};


#define ID_MENU_QUIT  1000
#define ID_MENU_ABOUT 1001

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_MENU_QUIT, MyFrame::OnQuit)
EVT_MENU(ID_MENU_ABOUT, MyFrame::OnAbout)
END_EVENT_TABLE()


void MyFrame::Init( ){

    // We need to set the painter object
    m_rl_mgr.SetPainter( &m_rl_painter );

    // This causes the background item to be enclosed into a horizontal sizer.
    m_rl_mgr.GetRoot().Enclose( wxHORIZONTAL|wxEXPAND );

    // Cache root item for convenient access
    wxRlItem &root = m_rl_mgr.GetRoot();

    // A Tree control to the left. Proportion set to 1/3 of default
    wxTreeCtrl *ptctl = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    // ... setup tree code...
    wxTreeItemId tid = ptctl->AddRoot( _T("A root item") );
    wxTreeItemId tid1 = ptctl->AppendItem( tid, _("First child") );
    wxTreeItemId tid2 = ptctl->AppendItem( tid, _("Second child") );
    ptctl->AppendItem( tid1, _("First child") );
    ptctl->AppendItem( tid2, _("Second child") );
    wxRlItem *rli = new wxRlAppPane( _T("Tree Pane"), ptctl,
                                     CAP_BIT_BUTTON_DOWN, RL_DEF_PROP/3 );
    root.Insert( 0, rli );


    // To the right, a new vertical sizer to put the toolbar,
    // the editor and log pane in
    root.Insert( 1, new wxBoxSizer(wxVERTICAL) );

    wxToolBar *ptb = new wxToolBar(this, wxID_ANY, wxDefaultPosition,
                                   wxDefaultSize, wxTB_HORIZONTAL |
                                   wxNO_BORDER | wxTB_TEXT | wxTB_NOICONS);
    // ... setup toolbar code, add buttons, ..., make it simple, only text (other sample does bitmaps)
    ptb->AddTool( ID_MENU_ABOUT, _T("About"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, _T("About box") );
    ptb->AddTool( ID_MENU_QUIT, _T("Quit"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, _T("Quit program") );
    ptb->Realize( );
    rli = new wxRlToolPane( _T("Tool Pane"), wxHORIZONTAL|wxEXPAND, ptb, true );
    root.Part(1).Insert( 0, rli );

    // A wxTextCtrl under the toolbar. Note: No pane just insert window
    root.Part(1).Insert( 1, new wxTextCtrl(this,wxID_ANY,
                _T("Some initial text here...\nIn a multi line edit"),
                wxDefaultPosition, wxDefaultSize,
                wxTE_MULTILINE|wxBORDER_NONE), RL_DEF_PROP );

    // And the log pane.
    // Create a log window
    wxTextCtrl *log = new wxTextCtrl(this,wxID_ANY,wxEmptyString,
                wxDefaultPosition,wxDefaultSize,
                wxTE_MULTILINE|wxTE_READONLY|wxBORDER_NONE);
    wxLog::SetActiveTarget( new wxLogTextCtrl(log) );
    rli = new wxRlAppPane( _T("Log Pane"), log, 0 );
    root.Part(1).Insert( 2, rli );


#if wxUSE_MENUS
    // Create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(ID_MENU_QUIT, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(ID_MENU_ABOUT, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    Show( true );	// Without show here, wxGTK hangs in the call to SetMenubar
    SetMenuBar(mbar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(_("Hello world..."),0);
#endif // wxUSE_STATUSBAR

    // Have to inform layout manager that size has changed (after adding menu & statubar)
    m_rl_mgr.Update( );
}

// A trivial wxApp class
class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        MyFrame *pmf = new MyFrame( _T("wxRecursiveLayout Sample") );
        return true;
    }
};


IMPLEMENT_APP(MyApp);

