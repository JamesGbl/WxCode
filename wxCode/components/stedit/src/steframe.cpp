///////////////////////////////////////////////////////////////////////////////
// Name:        steframe.cpp
// Purpose:     wxSTEditorFrame
// Author:      John Labenski, parts taken from wxGuide by Otto Wyss
// Modified by:
// Created:     11/05/2002
// RCS-ID:
// Copyright:   (c) John Labenski, Otto Wyss
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include <wx/srchctrl.h>

#include "wx/stedit/stedit.h"
#include "wx/stedit/steframe.h"
#include "wx/stedit/steart.h"
#include "wx/stedit/stetree.h"

#include "wxext.h"

//-----------------------------------------------------------------------------
// wxSTEditorFrame
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(wxSTEditorFrame, wxFrame)

BEGIN_EVENT_TABLE(wxSTEditorFrame, wxFrame)
    EVT_MENU_OPEN             (wxSTEditorFrame::OnMenuOpen)
    EVT_MENU                  (wxID_ANY, wxSTEditorFrame::OnMenu)
    EVT_SEARCHCTRL_SEARCH_BTN (ID_STE_TOOLBAR_FIND_CTRL, wxSTEditorFrame::OnMenu) // wxCommandEvent so we can treat it like a menu
    EVT_TEXT_ENTER            (ID_STE_TOOLBAR_FIND_CTRL, wxSTEditorFrame::OnMenu) // wxCommandEvent so we can treat it like a menu

    //EVT_STE_CREATED           (wxID_ANY, wxSTEditorFrame::OnSTECreatedEvent)
    EVT_STE_STATE_CHANGED     (wxID_ANY, wxSTEditorFrame::OnSTEState)
    EVT_STC_UPDATEUI          (wxID_ANY, wxSTEditorFrame::OnSTCUpdateUI)
    EVT_STE_POPUPMENU         (wxID_ANY, wxSTEditorFrame::OnSTEPopupMenu)

    EVT_STN_PAGE_CHANGED      (wxID_ANY, wxSTEditorFrame::OnNotebookPageChanged)

    EVT_CLOSE                 (wxSTEditorFrame::OnClose)
END_EVENT_TABLE()

void wxSTEditorFrame::Init()
{
    m_steNotebook      = NULL;
    m_steSplitter      = NULL;
    m_mainSplitter     = NULL;
    m_sideSplitter     = NULL;
    m_sideNotebook     = NULL;
    m_steTreeCtrl      = NULL;
    m_sideSplitterWin1 = NULL;
    m_sideSplitterWin2 = NULL;
}

bool wxSTEditorFrame::Create(wxWindow *parent, wxWindowID id,
                             const wxString& title,
                             const wxPoint& pos, const wxSize& size,
                             long  style,
                             const wxString& name)
{
    m_titleBase = title;

    if (!wxFrame::Create(parent, id, title, pos, size, style, name))
        return false;

    // Set the frame's icons
    SetIcons(wxSTEditorArtProvider::GetDialogIconBundle());

    ::wxFrame_SetInitialPosition(this, pos, size);
#ifdef x__WXDEBUG__
    wxCommandEvent menu(wxEVT_COMMAND_MENU_SELECTED);
    menu.SetId(ID_STE_PROPERTIES);
    //menu.SetId(ID_STE_EXPORT);
    //menu.SetId(wxID_OPEN);
    //menu.SetId(wxID_SAVEAS);
    //menu.SetId(ID_STE_PREFERENCES);
    //menu.SetId(ID_STE_EXPORT);
    ::wxPostEvent(this, menu);
#endif
    return true;
}

wxSTEditorFrame::~wxSTEditorFrame()
{
    SetSendSTEEvents(false);
    if (GetToolBar() && (GetToolBar() == GetOptions().GetToolBar())) // remove for safety
        GetOptions().SetToolBar(NULL);
    if (GetMenuBar() && (GetMenuBar() == GetOptions().GetMenuBar())) // remove for file history
        GetOptions().SetMenuBar(NULL);
    if (GetStatusBar() && (GetStatusBar() == GetOptions().GetStatusBar()))
        GetOptions().SetStatusBar(NULL);

    // This stuff always gets saved when the frame closes
    wxConfigBase *config = GetConfigBase();
    if (config && GetOptions().HasConfigOption(STE_CONFIG_FILEHISTORY))
        GetOptions().SaveFileConfig(*config);

    if (config && GetOptions().HasConfigOption(STE_CONFIG_FINDREPLACE) &&
        GetOptions().GetFindReplaceData())
        GetOptions().GetFindReplaceData()->SaveConfig(*config,
                      GetOptions().GetConfigPath(STE_OPTION_CFGPATH_FINDREPLACE));
}

bool wxSTEditorFrame::Destroy()
{
    SetSendSTEEvents(false);
    if (GetToolBar() && (GetToolBar() == GetOptions().GetToolBar())) // remove for safety
        GetOptions().SetToolBar(NULL);
    if (GetMenuBar() && (GetMenuBar() == GetOptions().GetMenuBar())) // remove for file history
        GetOptions().SetMenuBar(NULL);
    if (GetStatusBar() && (GetStatusBar() == GetOptions().GetStatusBar()))
        GetOptions().SetStatusBar(NULL);

    return wxFrame::Destroy();
}
void wxSTEditorFrame::SetSendSTEEvents(bool send)
{
    if (GetEditorNotebook())
        GetEditorNotebook()->SetSendSTEEvents(send);
    else if (GetEditorSplitter())
        GetEditorSplitter()->SetSendSTEEvents(send);
    else if (GetEditor())
        GetEditor()->SetSendSTEEvents(send);
}

void wxSTEditorFrame::CreateOptions( const wxSTEditorOptions& options )
{
    m_options = options;
    wxConfigBase *config = GetConfigBase();
    wxSTEditorMenuManager *steMM = GetOptions().GetMenuManager();

    if (steMM)
    {
        steMM->InitAcceleratorArray();
    }

    if (steMM && GetOptions().HasFrameOption(STF_CREATE_MENUBAR))
    {
        wxMenuBar *menuBar = GetMenuBar() ? GetMenuBar() : new wxMenuBar(wxMB_DOCKABLE);
        steMM->CreateMenuBar(menuBar, true);

        if (menuBar)
        {
            SetMenuBar(menuBar);
            wxAcceleratorHelper::SetAcceleratorTable(this, *steMM->GetAcceleratorArray());
            wxAcceleratorHelper::SetAccelText(menuBar, *steMM->GetAcceleratorArray());

            if (GetOptions().HasFrameOption(STF_CREATE_FILEHISTORY) && !GetOptions().GetFileHistory())
            {
                // if has file open then we can use wxFileHistory to save them
                wxMenu* menu = NULL;
                wxMenuItem* item = menuBar->FindItem(wxID_OPEN, &menu);
                if (item)
                {
                    for (size_t i = 0; i < menu->GetMenuItemCount(); i++)
                    {
                       if (menu->GetMenuItems().Item(i)->GetData() == item)
                       {
                          wxMenu* submenu = new wxMenu();
                          menu->Insert(i + 1, wxID_ANY, _("Open &Recent"), submenu);
                          GetOptions().SetFileHistory(new wxFileHistory(9), false);
                          GetOptions().GetFileHistory()->UseMenu(submenu);
                          if (config)
                          {
                              GetOptions().LoadFileConfig(*config);
                          }
                          break;
                       }
                    }
                }
            }

            GetOptions().SetMenuBar(menuBar);
        }
    }
    if (steMM && GetOptions().HasFrameOption(STF_CREATE_TOOLBAR))
    {
        wxToolBar* toolBar = (GetToolBar() != NULL) ? GetToolBar() : CreateToolBar();
        steMM->CreateToolBar(toolBar);
        GetOptions().SetToolBar(toolBar);
    }
    if ((GetStatusBar() == NULL) && GetOptions().HasFrameOption(STF_CREATE_STATUSBAR))
    {
        CreateStatusBar(1);
        GetOptions().SetStatusBar(GetStatusBar());
    }
    if (steMM)
    {
        if (GetOptions().HasEditorOption(STE_CREATE_POPUPMENU))
        {
            wxMenu* menu = steMM->CreateEditorPopupMenu();

            wxAcceleratorHelper::SetAccelText(menu, *steMM->GetAcceleratorArray());
            GetOptions().SetEditorPopupMenu(menu, false);
        }
        if (GetOptions().HasSplitterOption(STS_CREATE_POPUPMENU))
            GetOptions().SetSplitterPopupMenu(steMM->CreateSplitterPopupMenu(), false);
        if (GetOptions().HasNotebookOption(STN_CREATE_POPUPMENU))
            GetOptions().SetNotebookPopupMenu(steMM->CreateNotebookPopupMenu(), false);
    }

    if (!m_sideSplitter && GetOptions().HasFrameOption(STF_CREATE_SIDEBAR))
    {
        m_sideSplitter = new wxSplitterWindow(this, ID_STF_SIDE_SPLITTER);
        m_sideSplitter->SetMinimumPaneSize(10);
        m_sideNotebook = new wxNotebook(m_sideSplitter, ID_STF_SIDE_NOTEBOOK);
        m_steTreeCtrl  = new wxSTEditorTreeCtrl(m_sideNotebook, ID_STF_FILE_TREECTRL);
        m_sideNotebook->AddPage(m_steTreeCtrl, _("Files"));
        m_sideSplitterWin1 = m_sideNotebook;
    }

    if (!m_steNotebook && GetOptions().HasFrameOption(STF_CREATE_NOTEBOOK))
    {
        m_mainSplitter = new wxSplitterWindow(m_sideSplitter ? (wxWindow*)m_sideSplitter : (wxWindow*)this, ID_STF_MAIN_SPLITTER);
        m_mainSplitter->SetMinimumPaneSize(10);

        m_steNotebook = new wxSTEditorNotebook(m_mainSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                               wxCLIP_CHILDREN);
        m_steNotebook->CreateOptions(m_options);
        (void)m_steNotebook->InsertEditorSplitter(-1, wxID_ANY, GetOptions().GetDefaultFileName(), true);
        // update after adding a single page
        m_steNotebook->UpdateAllItems();
        m_mainSplitter->Initialize(m_steNotebook);
        m_sideSplitterWin2 = m_mainSplitter;

        if (m_steTreeCtrl)
            m_steTreeCtrl->SetSTENotebook(m_steNotebook);
    }
    else if (!m_steSplitter && GetOptions().HasFrameOption(STF_CREATE_SINGLEPAGE))
    {
        m_mainSplitter = new wxSplitterWindow(m_sideSplitter ? (wxWindow*)m_sideSplitter : (wxWindow*)this, ID_STF_MAIN_SPLITTER);
        m_mainSplitter->SetMinimumPaneSize(10);

        m_steSplitter = new wxSTEditorSplitter(m_mainSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
        m_steSplitter->CreateOptions(m_options);
        m_mainSplitter->Initialize(m_steSplitter);
    }
    //else user will set up the rest

    if (GetOptions().HasFrameOption(STF_CREATE_SIDEBAR) && GetSideSplitter() && m_sideSplitterWin1 && m_sideSplitterWin2)
    {
        GetSideSplitter()->SplitVertically(m_sideSplitterWin1, m_sideSplitterWin2, 200);
    }

#if wxUSE_DRAG_AND_DROP
    SetDropTarget(new wxSTEditorFrameFileDropTarget(this));
#endif //wxUSE_DRAG_AND_DROP

    if (GetOptions().HasConfigOption(STE_CONFIG_FINDREPLACE) && config)
    {
        if (GetOptions().GetFindReplaceData() &&
            !GetOptions().GetFindReplaceData()->HasLoadedConfig())
            GetOptions().GetFindReplaceData()->LoadConfig(*config);
    }

    if (config)
        LoadConfig(*config);

    UpdateAllItems();

    // if we've got an editor let it update gui
    wxSTEditor *editor = GetEditor();
    if (editor)
        editor->UpdateAllItems();
}

wxSTEditor *wxSTEditorFrame::GetEditor(int page) const
{
    wxSTEditorSplitter *splitter = GetEditorSplitter(page);
    return splitter ? splitter->GetEditor() : (wxSTEditor*)NULL;
}

wxSTEditorSplitter *wxSTEditorFrame::GetEditorSplitter(int page) const
{
    return GetEditorNotebook() ? GetEditorNotebook()->GetEditorSplitter(page) : m_steSplitter;
}

void wxSTEditorFrame::UpdateAllItems()
{
    UpdateItems(GetOptions().GetEditorPopupMenu(), GetOptions().GetMenuBar(),
                                                   GetOptions().GetToolBar());
    UpdateItems(GetOptions().GetNotebookPopupMenu());
    UpdateItems(GetOptions().GetSplitterPopupMenu());
}
void wxSTEditorFrame::UpdateItems(wxMenu *menu, wxMenuBar *menuBar, wxToolBar *toolBar)
{
    if (!menu && !menuBar && !toolBar) return;

    STE_MM::DoEnableItem(menu, menuBar, toolBar, ID_STF_SHOW_SIDEBAR, GetSideSplitter() != NULL);
    STE_MM::DoCheckItem(menu, menuBar, toolBar, ID_STF_SHOW_SIDEBAR, (GetSideSplitter() != NULL) && GetSideSplitter()->IsSplit());
}

void wxSTEditorFrame::LoadConfig(wxConfigBase &config, const wxString &configPath_)
{
    wxString configPath = wxSTEditorOptions::FixConfigPath(configPath_, false);

    if (GetMenuBar() && GetMenuBar()->FindItem(ID_STF_SHOW_SIDEBAR))
    {
        long val = 0;
        if (config.Read(configPath + wxT("/ShowSidebar"), &val))
        {
            wxSTEditorMenuManager::DoCheckItem(NULL, GetMenuBar(), NULL,
                                               ID_STF_SHOW_SIDEBAR, val != 0);
            // send fake event to HandleEvent
            wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, ID_STF_SHOW_SIDEBAR);
            evt.SetInt(int(val));
            HandleMenuEvent(evt);
        }
    }

    wxString str;
    if (config.Read(configPath + wxT("/FrameSize"), &str))
    {
        wxRect rect = GetRect();
        long lrect[4] = { rect.x, rect.y, rect.width, rect.height };
        wxArrayString arrStr = wxStringTokenize(str, wxT(","));
        if (arrStr.GetCount() == 4u)
        {
            for (size_t n = 0; n < 4; n++)
                arrStr[n].ToLong(&lrect[n]);

            wxRect cfgRect((int)lrect[0], (int)lrect[1], (int)lrect[2], (int)lrect[3]);
            cfgRect = cfgRect.Intersect(wxGetClientDisplayRect());

            if ((cfgRect != rect) && (cfgRect.width>=100) && (cfgRect.height>=100))
                SetSize(cfgRect);
        }
    }
}
void wxSTEditorFrame::SaveConfig(wxConfigBase &config, const wxString &configPath_)
{
    wxString configPath = wxSTEditorOptions::FixConfigPath(configPath_, false);
    if (GetMenuBar() && GetMenuBar()->FindItem(ID_STF_SHOW_SIDEBAR))
    {
        wxString val = GetMenuBar()->IsChecked(ID_STF_SHOW_SIDEBAR) ? wxT("1") : wxT("0");
        config.Write(configPath + wxT("/ShowSidebar"), val);
    }

    wxRect rect = GetRect();
    if ((rect.x>=0) && (rect.y>=0) && (rect.width>=100) && (rect.height>=100))
       config.Write(configPath + wxT("/FrameSize"), wxString::Format(wxT("%d,%d,%d,%d"), rect.x, rect.y, rect.width, rect.height));
}

wxConfigBase* wxSTEditorFrame::GetConfigBase()
{
    return wxConfigBase::Get(false);
}

void wxSTEditorFrame::OnNotebookPageChanged(wxNotebookEvent &WXUNUSED(event))
{
    wxSTEditor *editor = GetEditor();
    wxString title = m_titleBase;
    wxSTEditorMenuManager *steMM = GetOptions().GetMenuManager();

    if (editor)
    {
        if ( steMM && !steMM->HasEnabledEditorItems())
            steMM->EnableEditorItems(true, NULL, GetMenuBar(), GetToolBar());

        title = MakeTitle(editor);
    }
    else
    {
        if (steMM && steMM->HasEnabledEditorItems())
            steMM->EnableEditorItems(false, NULL, GetMenuBar(), GetToolBar());
    }

    SetTitle(title);
}

void wxSTEditorFrame::OnSTECreated(wxCommandEvent &event)
{
    event.Skip();
    if (m_steTreeCtrl != NULL)
        m_steTreeCtrl->UpdateFromNotebook();
}

void wxSTEditorFrame::OnSTEPopupMenu(wxSTEditorEvent &event)
{
    event.Skip();
    wxSTEditor *editor = event.GetEditor();
    UpdateItems(editor->GetOptions().GetEditorPopupMenu());
}

wxString wxSTEditorFrame::MakeTitle(const wxSTEditor* editor) const
{
    wxFileName filename = editor->GetFileName() ;
    const wxString modified = editor->IsModified() ? wxMODIFIED_ASTERISK : wxEmptyString;
    return wxString::Format(wxT("%s - %s"),
        (filename.GetFullPath(GetOptions().GetDisplayPathSeparator()) + modified).wx_str(),
        m_titleBase.wx_str());
}

void wxSTEditorFrame::OnSTEState(wxSTEditorEvent &event)
{
    event.Skip();
    wxSTEditor *editor = event.GetEditor();

    if ( event.HasStateChange(STE_FILENAME | STE_MODIFIED | STE_EDITABLE) )
    {
        SetTitle(MakeTitle(editor));

        //UpdateFileTreeCtrl();
        if (event.HasStateChange(STE_FILENAME) && GetOptions().GetFileHistory())
        {
            if (wxFileExists(event.GetString()))
                GetOptions().GetFileHistory()->AddFileToHistory( event.GetString() );
        }
    }
}

void wxSTEditorFrame::OnSTCUpdateUI(wxStyledTextEvent &event)
{
    event.Skip();
    if (!GetStatusBar()) // nothing to do
        return;

    wxStyledTextCtrl* editor = wxStaticCast(event.GetEventObject(), wxStyledTextCtrl);
    STE_TextPos pos   = editor->GetCurrentPos();
    int line  = editor->GetCurrentLine() + 1; // start at 1
    int lines = editor->GetLineCount();
    int col   = editor->GetColumn(pos) + 1;   // start at 1
    int chars = editor->GetLength();

    wxString txt = wxString::Format(wxT("Line %6d of %6d, Col %4d, Chars %6d  "), line, lines, col, chars);
    txt += editor->GetOvertype() ? wxT("[OVR]") : wxT("[INS]");

    if (txt != GetStatusBar()->GetStatusText()) // minor flicker reduction
        SetStatusText(txt, 0);
}

void wxSTEditorFrame::OnMenuOpen(wxMenuEvent &WXUNUSED(event))
{
    // might need to update the preferences, the rest should be ok though
    wxSTEditor* editor = NULL;
    wxWindow* win = wxWindow::FindFocus();

    // see if there's an editor that's a child of this and has the focus
    if (win != NULL)
    {
        editor = wxDynamicCast(win, wxSTEditor);
        if (editor)
        {
            wxWindow* parent = editor->GetParent();
            while (parent && (parent != this))
                parent = parent->GetParent();

            if (parent != this)
                editor = NULL;
        }
    }

    // just use the currently focused editor notebook
    if (editor == NULL)
        editor = GetEditor();

    if (editor && GetMenuBar())
        editor->UpdateItems(NULL, GetMenuBar());
}

void wxSTEditorFrame::OnMenu(wxCommandEvent &event)
{
    wxSTERecursionGuard guard(m_rGuard_OnMenu);
    if (guard.IsInside()) return;

    if (!HandleMenuEvent(event))
        event.Skip();
}

bool wxSTEditorFrame::HandleMenuEvent(wxCommandEvent &event)
{
    wxSTERecursionGuard guard(m_rGuard_HandleMenuEvent);
    if (guard.IsInside()) return false;

    int win_id  = event.GetId();
    wxSTEditor *editor = GetEditor();

    // menu items that the frame handles before children
    switch (win_id)
    {
        case ID_STE_SAVE_PREFERENCES :
        {
            // we save everything the children do and more
            wxConfigBase *config = GetConfigBase();
            if (config)
            {
                SaveConfig(*config, GetOptions().GetConfigPath(STE_OPTION_CFGPATH_FRAME));
                GetOptions().SaveConfig(*config);
            }

            return true;
        }
    }

    // Try the children to see if they'll handle the event first
    if (GetEditorNotebook() && GetEditorNotebook()->HandleMenuEvent(event))
        return true;

    if (editor)
    {
        if (wxDynamicCast(editor->GetParent(), wxSTEditorSplitter) &&
            wxDynamicCast(editor->GetParent(), wxSTEditorSplitter)->HandleMenuEvent(event))
            return true;
        if (editor->HandleMenuEvent(event))
            return true;
    }

    if ((win_id >= wxID_FILE1) && (win_id <= wxID_FILE9))
    {
        if (GetOptions().GetFileHistory())
        {
            wxFileName fileName = GetOptions().GetFileHistory()->GetHistoryFile(win_id-wxID_FILE1);
            bool ok;

            if (GetEditorNotebook())
            {
                ok = GetEditorNotebook()->LoadFile(fileName);
            }
            else if (editor)
            {
                ok = editor->LoadFile(fileName);
            }
            else
            {
                ok = false;
            }
            if (!ok)
            {
               wxMessageBox(wxString::Format(_("Error opening file: '%s'"),
                              fileName.GetFullPath(GetOptions().GetDisplayPathSeparator()).wx_str()),
                        STE_APPDISPLAYNAME, wxOK|wxICON_ERROR , this);
            }
        }

        return true;
    }

    switch (win_id)
    {
        case ID_STE_SHOW_FULLSCREEN :
            ShowFullScreen(event.IsChecked());
            return true;
        case ID_STF_SHOW_SIDEBAR :
            ShowSidebar(event.IsChecked());
            return true;
        case wxID_EXIT :
        {
            if (GetEditorNotebook())
            {
                if (!GetEditorNotebook()->QuerySaveIfModified())
                    return true;
            }
            else if (editor && (editor->QuerySaveIfModified(true) == wxCANCEL))
                return true;

            Destroy();
            return true;
        }
        case wxID_ABOUT :
            wxSTEditorAboutDialog(this);
            return true;
        default : break;
    }

    return false;
}

void wxSTEditorFrame::ShowFullScreen(bool on)
{
    //long style = wxFULLSCREEN_NOBORDER|wxFULLSCREEN_NOCAPTION;
    long style = wxFULLSCREEN_ALL;
    wxFrame::ShowFullScreen(on, style);
}

void wxSTEditorFrame::ShowSidebar(bool on)
{
    if (GetSideSplitter() && m_sideSplitterWin1 && m_sideSplitterWin2)
    {
        if (on)
        {
            if (!GetSideSplitter()->IsSplit())
            {
                GetSideSplitter()->SplitVertically(m_sideSplitterWin1, m_sideSplitterWin2, 100);
                GetSideNotebook()->Show();
            }
        }
        else if (GetSideSplitter()->IsSplit())
        {
            GetSideSplitter()->Unsplit(m_sideSplitterWin1);
        }
        UpdateAllItems();
    }
}

void wxSTEditorFrame::OnClose( wxCloseEvent &event )
{
    int style = event.CanVeto() ? wxYES_NO|wxCANCEL : wxYES_NO;

    if (GetEditorNotebook())
    {
        if (!GetEditorNotebook()->QuerySaveIfModified(style))
        {
            event.Veto(true);
            return;
        }
    }
    else if (GetEditor() && (GetEditor()->QuerySaveIfModified(true, style) == wxCANCEL))
    {
        event.Veto(true);
        return;
    }

    // **** Shutdown so that the focus event doesn't crash the editors ****
    SetSendSTEEvents(false);
    event.Skip();
}

//-----------------------------------------------------------------------------
// wxSTEditorFileDropTarget
//-----------------------------------------------------------------------------
#if wxUSE_DRAG_AND_DROP

bool wxSTEditorFrameFileDropTarget::OnDropFiles(wxCoord WXUNUSED(x), wxCoord WXUNUSED(y),
                                                const wxArrayString& filenames)
{
    wxCHECK_MSG(m_owner, false, wxT("Invalid drop target"));
    const size_t count = filenames.GetCount();
    if (count == 0)
        return false;

    // see if it has a notebook and use it to load the files
    if (m_owner->GetEditorNotebook())
    {
        wxArrayString files = filenames;
        m_owner->GetEditorNotebook()->LoadFiles(&files);
    }
    else if (m_owner->GetEditor())
        m_owner->GetEditor()->LoadFile(filenames[0]);

    return true;
}

#endif //wxUSE_DRAG_AND_DROP
