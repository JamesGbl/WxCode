/***************************************************************
 * Name:      wxDemoViewerMain.cpp
 * Purpose:   Implements main application window class
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "wxDemoViewerMain.h"

BEGIN_EVENT_TABLE(wxDemoViewerFrame, wxFrame)
    EVT_CLOSE(wxDemoViewerFrame::OnClose)

    EVT_MENU(idMenuQuit, wxDemoViewerFrame::OnQuit)
    EVT_MENU(idMenuAbout, wxDemoViewerFrame::OnAbout)
    EVT_MENU(idMenuShowLog, wxDemoViewerFrame::OnMenuShowLog)
    EVT_MENU(idMenuShowDoc, wxDemoViewerFrame::OnMenuShowDoc)
    EVT_MENU(idMenuSearch, wxDemoViewerFrame::OnMenuSearch)
    EVT_MENU(idSourceSearchNext, wxDemoViewerFrame::OnSourceSearchNext)
    EVT_MENU(idSourceSearchPrev, wxDemoViewerFrame::OnSourceSearchPrev)
    EVT_MENU(idChangeSkin, wxDemoViewerFrame::OnChangeSkin)

    EVT_UPDATE_UI(idSourceSearchNext, wxDemoViewerFrame::OnUpdateSearchMenu)
    EVT_UPDATE_UI(idSourceSearchPrev, wxDemoViewerFrame::OnUpdateSearchMenu)

    EVT_BUTTON(idDescriptionTab, wxDemoViewerFrame::OnDescriptionTabClick)
    EVT_BUTTON(idSourceTab, wxDemoViewerFrame::OnSourceTabClick)
    EVT_BUTTON(idDocTab, wxDemoViewerFrame::OnDocTabClick)
    EVT_BUTTON(idSourceSearchNext, wxDemoViewerFrame::OnSourceSearchNext)
    EVT_BUTTON(idSourceSearchPrev, wxDemoViewerFrame::OnSourceSearchPrev)
    EVT_BUTTON(idKeywordsSearchCode, wxDemoViewerFrame::OnKeywordsSearchCode)
    EVT_BUTTON(idKeywordsSearchDocs, wxDemoViewerFrame::OnKeywordsSearchDocs)
    EVT_BUTTON(idLaunchButton, wxDemoViewerFrame::OnLaunchDemo)

    EVT_TEXT(idSourceSearchCtrl, wxDemoViewerFrame::OnSourceSearchKeyPress)
    EVT_TEXT_ENTER(idSourceSearchCtrl, wxDemoViewerFrame::OnSourceSearchNext)

    EVT_TEXT(idMainSearchCtrl, wxDemoViewerFrame::OnMainSearchKeyPress)

    EVT_TREE_SEL_CHANGED(idTreeList, wxDemoViewerFrame::OnTreeSelectionChanged)
    EVT_COMBOBOX(idSourceFiles, wxDemoViewerFrame::OnSourceFileChanged)

    EVT_END_PROCESS(idLaunchedDemo, wxDemoViewerFrame::OnTerminateDemo)
END_EVENT_TABLE()

wxDemoViewerFrame::wxDemoViewerFrame(wxFrame *frame, const wxString& title, wxString ewd): wxFrame(frame, -1, title)
{
    m_pLogWnd = new wxLogWindow(this, wxT("wxDemoViewer Log"), false);
    wxLog::SetActiveTarget(m_pLogWnd);

    wxLogMessage(wxT("Initializing image handlers"));
    wxInitAllImageHandlers();

    wxFileSystem::AddHandler(new wxArchiveFSHandler);

    m_lPID = 0;
    m_pProcessWatcher = NULL;
    m_sExecWorkingDir = ewd;

    wxString filename;

    if(wxFileName::FileExists(wxT("autostart.conf")))
    {
        wxFileInputStream fin(wxT("autostart.conf"));
        wxTextInputStream tin(fin);

        filename = tin.ReadLine();

        wxFileName fn(filename, wxPATH_UNIX);
        if(fn.FileExists())
        {
            filename = fn.GetFullName();
            wxFileName::SetCwd(fn.GetPath());
        }
        else { wxMessageBox(wxString::Format(wxT("File '%s' not found! Application will be terminated..."), filename.c_str()), wxT("Error"), wxICON_ERROR); Destroy(); return; }
    }

    if(filename.IsEmpty())
    {
        wxFileDialog ofd(this, wxT("Open file"), wxEmptyString, wxEmptyString, wxT("Demo Files (*.demo)|*.demo"), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);
        if(ofd.ShowModal()==wxID_OK)
        {
            wxString tmp = ofd.GetFilename();
            wxFileName fn(tmp, wxPATH_NATIVE);

            if(fn.FileExists()) filename = fn.GetFullPath();
        }
        else { Destroy(); return; }
    }

    if(!m_appData.loadData(filename)) { wxMessageBox(wxT("Failed to load demo file! Application will be terminated..."), wxT("Error"), wxICON_ERROR); Destroy(); return; }

    m_sMainWorkingDir = wxFileName::GetCwd();

    m_bHasDoc = !m_appData.docFile.IsEmpty();

    SetTitle(m_appData.treeRoot.title + wxT(" | wxDemoViewer"));

    wxLogMessage(wxT("Creating controls"));
    CreateControlsWithStyle();
    BuildTree();
}


wxDemoViewerFrame::~wxDemoViewerFrame()
{
    if(m_lPID!=0) wxProcess::Kill(m_lPID, wxSIGKILL, wxKILL_CHILDREN);
}

void wxDemoViewerFrame::CreateControlsWithStyle()
{
    int border;

    wxFont mainFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    SetFont(mainFont);

//  *** MENU ***
    wxLogMessage(wxT("Creating menu"));
    wxMenuBar *mbar = new wxMenuBar();
    wxMenu *fileMenu = new wxMenu(wxEmptyString);
    fileMenu->Append(idChangeSkin, wxT("&Change skin"));
    fileMenu->Append(idMenuQuit, wxT("&Quit\tCtrl+Q"));
    mbar->Append(fileMenu, wxT("&File"));

    wxMenu *searchMenu = new wxMenu(wxEmptyString);
    searchMenu->Append(idMenuSearch, wxT("&Search\tCtrl+F"));
    searchMenu->Append(idSourceSearchNext, wxT("&Search next\tF3"));
    searchMenu->Append(idSourceSearchPrev, wxT("&Search previous\tShift+F3"));
    mbar->Append(searchMenu, wxT("&Search"));

    wxMenu *helpMenu = new wxMenu(wxEmptyString);
    helpMenu->Append(idMenuShowLog, wxT("&Show Log\tCtrl+L"));
    if(m_bHasDoc) helpMenu->Append(idMenuShowDoc, wxT("&Documentation\tF1"));
    helpMenu->AppendSeparator();
    helpMenu->Append(idMenuAbout, wxT("&About"));
    mbar->Append(helpMenu, wxT("&Help"));

    SetMenuBar(mbar);

//  *** SIZERS ***
    wxBoxSizer *pSizerMain = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *pSizerLeft = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *pSizerTree = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *pSizerSearch = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *pSizerRight = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *pSizerSource = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *pSizerSourceFiles = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *pSizerKeywords = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *pSizerSourceSearch = new wxBoxSizer(wxHORIZONTAL);
    m_pSizerTabs = new wxBoxSizer(wxHORIZONTAL);
    m_pSizerContent = new wxBoxSizer(wxHORIZONTAL);
    m_pSizerSourceLists = new wxBoxSizer(wxHORIZONTAL);
    m_pSizerKeywordsSub = new wxBoxSizer(wxHORIZONTAL);

//  *** SPLITTER ***
    m_pSplitterWnd = new ownerDrawnSplitter(this, wxID_ANY, m_appData.splitterBackColor);
    pSizerMain->Add(m_pSplitterWnd, wxEXPAND);

    wxPanel *pLeftPanel = new wxPanel(m_pSplitterWnd, wxID_ANY);
    wxPanel *pRightPanel = new wxPanel(m_pSplitterWnd, wxID_ANY);

    m_pSplitterWnd->SplitVertically(pLeftPanel, pRightPanel, 230);

//  *** LEFT PANEL ***
    ownerDrawnPanel *pTreePanel = new ownerDrawnPanel(pLeftPanel, m_appData.stBackground[wxDemoViewerData::bgMainTree]);
    m_pTreeCtrl = new wxTreeCtrl(pTreePanel, idTreeList, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT | wxBORDER_NONE);
    m_pLogoImg = new logoPanel(pLeftPanel, m_appData.stBackground[wxDemoViewerData::bgMainLogo], m_appData.logoFile);
    ownerDrawnPanel *pSearchPanel = new ownerDrawnPanel(pLeftPanel, m_appData.stBackground[wxDemoViewerData::bgMainSearch]);
    m_pSearchTree = new ownerDrawnSearchCtrl(pSearchPanel, idMainSearchCtrl, m_appData.stControl[wxDemoViewerData::cMainSearch].textColor, m_appData.stControl[wxDemoViewerData::cMainSearch].backColor, m_appData.searchIcon);

    m_pTreeCtrl->SetBackgroundColour(m_appData.stControl[wxDemoViewerData::cMainTree].backColor);
    m_pTreeCtrl->SetForegroundColour(m_appData.stControl[wxDemoViewerData::cMainTree].textColor);
    m_pTreeCtrl->SetImageList(&m_appData.imgList);
    m_pTreeCtrl->SetFont(mainFont);

    pLeftPanel->SetSizer(pSizerLeft);
    pTreePanel->SetSizer(pSizerTree);
    pSearchPanel->SetSizer(pSizerSearch);

    pTreePanel->AppendBorder(m_pTreeCtrl, m_appData.stControl[wxDemoViewerData::cMainTree]);
    pSearchPanel->AppendBorder(m_pSearchTree, m_appData.stControl[wxDemoViewerData::cMainSearch]);

    if(m_appData.stControl[wxDemoViewerData::cMainTree].hasBorder) border = (m_appData.stControl[wxDemoViewerData::cMainTree].rounded) ? 5 : 3;
    else border = 0;

    pSizerTree->Add(m_pTreeCtrl, 1, wxEXPAND | wxALL, border);

    pSizerLeft->Add(new ownerDrawnStatic(pLeftPanel, m_appData.stStatic[wxDemoViewerData::sMainTitle]), 0, wxEXPAND, 0);
    pSizerLeft->Add(pTreePanel, 1, wxEXPAND, 0);
    pSizerLeft->Add(m_pLogoImg, 0, wxEXPAND, 0);
    pSizerLeft->Add(new ownerDrawnStatic(pLeftPanel, m_appData.stStatic[wxDemoViewerData::sMainSearchTitle]), 0, wxEXPAND, 0);

    if(m_appData.stControl[wxDemoViewerData::cMainSearch].hasBorder) border = (m_appData.stControl[wxDemoViewerData::cMainSearch].rounded) ? 5 : 4;
    else border = 2;

    pSizerSearch->Add(m_pSearchTree, 0, wxEXPAND | wxALL, border);
    pSizerLeft->Add(pSearchPanel, 0, wxEXPAND, 0);

//  *** RIGHT PANEL ***
    ownerDrawnPanel *pTabPanel = new ownerDrawnPanel(pRightPanel, m_appData.stBackground[wxDemoViewerData::bgTabsPanel], wxSize(-1, 25));
    m_pTabDescription = new ownerDrawnButton(pTabPanel, idDescriptionTab, m_appData.stButton[wxDemoViewerData::bTabDescription], true);
    m_pTabSourceCode = new ownerDrawnButton(pTabPanel, idSourceTab, m_appData.stButton[wxDemoViewerData::bTabSource], true);
    m_pTabDoc = new ownerDrawnButton(pTabPanel, idDocTab, m_appData.stButton[wxDemoViewerData::bTabDoc], true);

    pRightPanel->SetSizer(pSizerRight);
    pSizerRight->Add(pTabPanel, 0, wxEXPAND, 0);
    pSizerRight->Add(m_pSizerContent, 1, wxEXPAND, 0);

    pTabPanel->SetSizer(m_pSizerTabs);
    m_pSizerTabs->Add(m_pTabDescription, 0, wxEXPAND | wxTOP | wxRIGHT | wxLEFT, 2);
    m_pSizerTabs->Add(m_pTabSourceCode, 0, wxEXPAND | wxTOP | wxRIGHT | wxLEFT, 2);
    m_pSizerTabs->Add(m_pTabDoc, 0, wxEXPAND | wxTOP | wxRIGHT | wxLEFT, 2);

//  *** DESCRIPTION PAGE ***
    m_pDescrTab = new wxHtmlWindow(pRightPanel, wxID_ANY);

//  *** SOURCE CODE PAGE ***
    m_pSourceCodeTab = new ownerDrawnPanel(pRightPanel, m_appData.stBackground[wxDemoViewerData::bgSourceTab]);
    m_appData.stCodeViewer.backColor = m_appData.stControl[wxDemoViewerData::cSourceCodeViewer].backColor;
    m_pSourceCodeViewer = new wxSTC(m_pSourceCodeTab, wxID_ANY, m_appData.stCodeViewer);
    m_pSourceFiles = new ownerDrawnComboBox(m_pSourceCodeTab, idSourceFiles, &m_appData.imgList, m_appData.stControl[wxDemoViewerData::cSourceFiles].textColor, m_appData.stControl[wxDemoViewerData::cSourceFiles].backColor);
    m_pSourceKeywords = new ownerDrawnComboBox(m_pSourceCodeTab, idKeywords, &m_appData.imgList, m_appData.stControl[wxDemoViewerData::cSourceKeywords].textColor, m_appData.stControl[wxDemoViewerData::cSourceKeywords].backColor);
    m_pKeywordSearchCode = new ownerDrawnButton(m_pSourceCodeTab, idKeywordsSearchCode, m_appData.stButton[wxDemoViewerData::bKeywordsSearchCode], false);
    m_pKeywordSearchDocs = new ownerDrawnButton(m_pSourceCodeTab, idKeywordsSearchDocs, m_appData.stButton[wxDemoViewerData::bKeywordsSearchDocs], false);
    m_pLaunchButton = new ownerDrawnButton(m_pSourceCodeTab, idLaunchButton, m_appData.stButton[wxDemoViewerData::bLaunchButton], false);
    m_pSearchSourceCode = new ownerDrawnSearchCtrl(m_pSourceCodeTab, idSourceSearchCtrl, m_appData.stControl[wxDemoViewerData::cSourceSearch].textColor, m_appData.stControl[wxDemoViewerData::cSourceSearch].backColor, m_appData.searchIcon);
    m_pSourceCodeSearchPrev = new ownerDrawnButton(m_pSourceCodeTab, idSourceSearchPrev, m_appData.stButton[wxDemoViewerData::bSourceSearchPrev], false);
    m_pSourceCodeSearchNext = new ownerDrawnButton(m_pSourceCodeTab, idSourceSearchNext, m_appData.stButton[wxDemoViewerData::bSourceSearchNext], false);

    m_pSourceCodeTab->SetSizer(pSizerSource);

    if(m_appData.stControl[wxDemoViewerData::cSourceFiles].hasBorder) border = (m_appData.stControl[wxDemoViewerData::cSourceFiles].rounded) ? 5 : 4;
    else border = 3;

    pSizerSourceFiles->Add(new ownerDrawnStatic(m_pSourceCodeTab, m_appData.stStatic[wxDemoViewerData::sSourceFilesTitle]), 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 3);
    pSizerSourceFiles->Add(m_pSourceFiles, 1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, border);
    m_pSourceCodeTab->AppendBorder(m_pSourceFiles, m_appData.stControl[wxDemoViewerData::cSourceFiles]);


    if(m_appData.stControl[wxDemoViewerData::cSourceKeywords].hasBorder) border = (m_appData.stControl[wxDemoViewerData::cSourceKeywords].rounded) ? 5 : 4;
    else border = 3;

    m_pSizerKeywordsSub->Add(m_pSourceKeywords, 1, wxEXPAND, 0);
    m_pSizerKeywordsSub->Add(m_pKeywordSearchCode, 0, wxEXPAND | wxLEFT, border);
    m_pSizerKeywordsSub->Add(m_pKeywordSearchDocs, 0, wxEXPAND | wxLEFT, border);
    pSizerKeywords->Add(new ownerDrawnStatic(m_pSourceCodeTab, m_appData.stStatic[wxDemoViewerData::sKeywordsTitle]), 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 3);
    pSizerKeywords->Add(m_pSizerKeywordsSub, 1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, border);
    m_pSourceCodeTab->AppendBorder(m_pSourceKeywords, m_appData.stControl[wxDemoViewerData::sKeywordsTitle]);

    m_pSizerSourceLists->Add(pSizerSourceFiles, 3, wxEXPAND, 0);
    m_pSizerSourceLists->Add(pSizerKeywords, 4, wxEXPAND, 0);
    m_pSizerSourceLists->Add(m_pLaunchButton, 0, wxEXPAND | wxRIGHT | wxTOP, 3);

    if(m_appData.stControl[wxDemoViewerData::cSourceCodeViewer].hasBorder) border = (m_appData.stControl[wxDemoViewerData::cSourceCodeViewer].rounded) ? 5 : 4;
    else border = 3;

    m_pSourceCodeTab->AppendBorder(m_pSourceCodeViewer, m_appData.stControl[wxDemoViewerData::cSourceCodeViewer]);
    pSizerSource->Add(m_pSizerSourceLists, 0, wxEXPAND);
    pSizerSource->Add(m_pSourceCodeViewer, 1, wxEXPAND | wxALL, border);
    pSizerSource->Add(pSizerSourceSearch, 0, wxEXPAND);

    if(m_appData.stControl[wxDemoViewerData::cSourceSearch].hasBorder) border = (m_appData.stControl[wxDemoViewerData::cSourceSearch].rounded) ? 5 : 4;
    else border = 3;

    m_pSourceCodeTab->AppendBorder(m_pSearchSourceCode, m_appData.stControl[wxDemoViewerData::cSourceSearch]);

    pSizerSourceSearch->Add(m_pSearchSourceCode, 1, wxEXPAND | wxALL, border);
    pSizerSourceSearch->Add(m_pSourceCodeSearchPrev, 0, wxEXPAND | wxALL, 3);
    pSizerSourceSearch->Add(m_pSourceCodeSearchNext, 0, wxEXPAND | wxALL, 3);

//  *** DOCUMENTATION WINDOW PAGE ***
    m_pDocWindowTab = new wxHtmlHelpWindow(pRightPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxBORDER_NONE, wxHF_CONTENTS | wxHF_SEARCH | wxHF_FLAT_TOOLBAR);
    wxHtmlHelpController *pHelpControler = new wxHtmlHelpController(wxHF_DEFAULT_STYLE, pRightPanel);
    pHelpControler->SetHelpWindow(m_pDocWindowTab);

    m_pDocWindowTab->GetSplitterWindow()->SetSashPosition(200, false);
    m_pDocWindowTab->GetToolBar()->SetBackgroundColour(m_appData.docToolbarBackColor);

    wxCommandEvent event(wxEVT_COMMAND_TOOL_CLICKED, wxID_HTML_PANEL);
    m_pDocWindowTab->AddPendingEvent(event);

    if(m_bHasDoc)
    {
        pHelpControler->AddBook(m_appData.docFile, false);
        m_pDocWindowTab->Display(wxT("index.html"));
    } else {
        m_pSizerTabs->Hide(2);
        m_pSizerTabs->Layout();

        m_pSizerSourceLists->Hide(1);
        m_pSizerTabs->Layout();
    }

    m_pSizerContent->Add(m_pDescrTab, 1, wxEXPAND, 0);
    m_pSizerContent->Add(m_pSourceCodeTab, 1, wxEXPAND, 0);
    m_pSizerContent->Add(m_pDocWindowTab, 1, wxEXPAND, 0);

    m_pTabDescription->select();

    SetClientSize(800, 600);
    SetMinSize(wxSize(640, 480));
    Center();

    wxLogMessage(wxT("Window successfully created"));
}

void wxDemoViewerFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void wxDemoViewerFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void wxDemoViewerFrame::OnAbout(wxCommandEvent &event)
{
    aboutDialog dlg(this);

    dlg.ShowModal();
}

void wxDemoViewerFrame::OnMenuShowLog(wxCommandEvent &event)
{
    m_pLogWnd->GetFrame()->CenterOnParent();
    m_pLogWnd->Show();
}

void wxDemoViewerFrame::OnDescriptionTabClick(wxCommandEvent &event)
{
    m_bCanSearchCode = false;

    m_pTabSourceCode->resetStyle();
    m_pTabDoc->resetStyle();

    m_pSizerContent->Show((size_t)0);
    m_pSizerContent->Hide((size_t)1);
    m_pSizerContent->Hide((size_t)2);

    m_pSizerContent->Layout();
}

void wxDemoViewerFrame::OnSourceTabClick(wxCommandEvent &event)
{
    m_bCanSearchCode = true;

    m_pTabDescription->resetStyle();
    m_pTabDoc->resetStyle();

    m_pSizerContent->Hide((size_t)0);
    m_pSizerContent->Show((size_t)1);
    m_pSizerContent->Hide((size_t)2);

    m_pSizerContent->Layout();
}

void wxDemoViewerFrame::OnDocTabClick(wxCommandEvent &event)
{
    m_bCanSearchCode = false;

    m_pTabDescription->resetStyle();
    m_pTabSourceCode->resetStyle();

    m_pSizerContent->Hide((size_t)0);
    m_pSizerContent->Hide((size_t)1);
    m_pSizerContent->Show((size_t)2);

    m_pSizerContent->Layout();
}

void wxDemoViewerFrame::OnMenuSearch(wxCommandEvent &event)
{
    if(m_bCanSearchCode) m_pSearchSourceCode->Focus();
    else m_pSearchTree->Focus();
}

void wxDemoViewerFrame::OnSourceSearchNext(wxCommandEvent &event)
{
    wxString s = m_pSearchSourceCode->GetValue();
    if(!s.IsEmpty()) m_pSourceCodeViewer->SearchNext(s);
}

void wxDemoViewerFrame::OnSourceSearchPrev(wxCommandEvent &event)
{
    wxString s = m_pSearchSourceCode->GetValue();
    if(!s.IsEmpty()) m_pSourceCodeViewer->SearchPrev(s);
}

void wxDemoViewerFrame::OnSourceSearchKeyPress(wxCommandEvent &event)
{
    wxString s = m_pSearchSourceCode->GetValue();
    if(!s.IsEmpty()) { m_pSourceCodeViewer->ResetSearch(); m_pSourceCodeViewer->SearchNext(s); }
    else m_pSourceCodeViewer->ResetSearch();
}

void wxDemoViewerFrame::OnMainSearchKeyPress(wxCommandEvent &event)
{
    m_pTreeCtrl->DeleteAllItems();

    BuildTree(m_pSearchTree->GetValue());
}

void wxDemoViewerFrame::OnUpdateSearchMenu(wxUpdateUIEvent &event)
{
    event.Enable(m_bCanSearchCode);
}

void wxDemoViewerFrame::BuildTree(wxString search)
{
    wxDemoViewerTreeItemData *data = new wxDemoViewerTreeItemData(&m_appData.treeRoot);
    wxTreeItemId trid = m_pTreeCtrl->AddRoot(m_appData.treeRoot.title, m_appData.treeRoot.imgIndex, -1, data), tid = trid;

    wxDemoViewerDemoDataList::compatibility_iterator node = m_appData.demoList.GetFirst();

    m_pTreeCtrl->SetItemBold(trid);

    if(search.IsEmpty())
    {
        while(node)
        {
            wxDemoViewerDemoData *current = node->GetData();

            data = new wxDemoViewerTreeItemData(current);

            if(current->isDemo) m_pTreeCtrl->AppendItem(tid, current->title, current->imgIndex, -1, data);
            else { tid = m_pTreeCtrl->AppendItem(trid, current->title, current->imgIndex, -1, data); m_pTreeCtrl->SetItemBold(tid); }

            node = node->GetNext();
        }
    }
    else
    {
        while(node)
        {
            wxDemoViewerDemoData *current = node->GetData();

            data = new wxDemoViewerTreeItemData(current);

            if(current->isDemo) { if(current->title.Lower().Contains(search.Lower())) m_pTreeCtrl->AppendItem(tid, current->title, current->imgIndex, -1, data); }
            else {
                if(tid!=trid && !m_pTreeCtrl->HasChildren(tid)) m_pTreeCtrl->Delete(tid);

                tid = m_pTreeCtrl->AppendItem(trid, current->title, current->imgIndex, -1, data);
                m_pTreeCtrl->SetItemBold(tid);
            }

            node = node->GetNext();
        }
    }

    if(tid!=trid && !m_pTreeCtrl->HasChildren(tid)) m_pTreeCtrl->Delete(tid);

    m_pTreeCtrl->ExpandAll();

    m_pTreeCtrl->Unselect();
    m_pTreeCtrl->SelectItem(m_pTreeCtrl->GetRootItem(), true);
}

void wxDemoViewerFrame::OnTreeSelectionChanged(wxTreeEvent &event)
{
    wxTreeItemId sid = event.GetItem();

    wxDemoViewerTreeItemData *tiData = (wxDemoViewerTreeItemData*)m_pTreeCtrl->GetItemData(sid);
    if(tiData!=NULL)
    {
        if(tiData->data->isDemo)
        {
            m_pDescrTab->SetPage(wxT("<html><body bgcolor=") + m_appData.descriptionBackColor + wxT(" text=") + m_appData.descriptionTextColor + wxT("><h1>")+ tiData->data->title + wxT("</h1><div align=\"justify\">")+ tiData->data->description + wxT("</div></body></hmtl>"));

            if(tiData->data->sourceFiles.GetCount()>0)
            {
                m_pSourceFiles->setData(&tiData->data->sourceFiles);
                m_pSizerTabs->Show((size_t)1);
                SelectSourceFile();

                if(tiData->data->keywords.GetCount()>0)
                {
                    m_pSourceKeywords->setData(&tiData->data->keywords);
                    m_pSizerSourceLists->Show((size_t)1);

                    if(m_bHasDoc) m_pSizerKeywordsSub->Show((size_t)2);
                    else m_pSizerKeywordsSub->Hide((size_t)2);

                } else m_pSizerSourceLists->Hide((size_t)1);

                if(!tiData->data->exeFile.IsEmpty())
                {
                    wxString filename = tiData->data->exeFile;
                    if(!m_appData.addedExt.IsEmpty()) filename += m_appData.addedExt;

                    wxFileName fn(filename, wxPATH_UNIX);
                    if(fn.FileExists()) m_pSizerSourceLists->Show((size_t)2);
                    else m_pSizerSourceLists->Hide((size_t)2);
                }
                else m_pSizerSourceLists->Hide((size_t)2);

            } else m_pSizerTabs->Hide((size_t)1);

            m_pSizerKeywordsSub->Layout();
            m_pSizerSourceLists->Layout();
        }
        else
        {
            m_pDescrTab->SetPage(wxT("<html><body bgcolor=") + m_appData.descriptionBackColor + wxT(" text=") + m_appData.descriptionTextColor + wxT("><h1>")+ m_appData.treeRoot.title + wxT("</h1><div align=\"justify\">")+ m_appData.treeRoot.description + wxT("</div></body></hmtl>"));
            m_pSizerTabs->Hide((size_t)1);
        }

        m_pSizerTabs->Layout();
        m_pTabDescription->select();
    }
}

void wxDemoViewerFrame::SelectSourceFile()
{
    int selection = m_pSourceFiles->GetSelection();

    styleLexer *lex = NULL;
    if(selection!=wxNOT_FOUND)
    {
        ownerDrawnComboBoxData *data = (ownerDrawnComboBoxData*)m_pSourceFiles->GetClientData(selection);

        if((unsigned)data->lexer<m_appData.lexers.GetCount()) lex = m_appData.lexers[data->lexer];
        m_pSourceCodeViewer->LoadFile(data->value, lex);
    }
}

void wxDemoViewerFrame::OnSourceFileChanged(wxCommandEvent &event)
{
    SelectSourceFile();
}

void wxDemoViewerFrame::OnMenuShowDoc(wxCommandEvent &event)
{
    if(FindFocus()==m_pSourceCodeViewer)
    {
        wxString str = m_pSourceCodeViewer->GetWordUnderCursor();
        if(!str.IsEmpty()) m_pDocWindowTab->Display(str);
    }

    m_pTabDoc->select();
}

void wxDemoViewerFrame::OnKeywordsSearchCode(wxCommandEvent &event)
{
    int selection = m_pSourceKeywords->GetSelection();

    if(selection!=wxNOT_FOUND)
    {
        ownerDrawnComboBoxData *data = (ownerDrawnComboBoxData*)m_pSourceKeywords->GetClientData(selection);

        m_pSearchSourceCode->SetValue(data->text);
    }
}

void wxDemoViewerFrame::OnKeywordsSearchDocs(wxCommandEvent &event)
{
    int selection = m_pSourceKeywords->GetSelection();

    if(selection!=wxNOT_FOUND)
    {
        ownerDrawnComboBoxData *data = (ownerDrawnComboBoxData*)m_pSourceKeywords->GetClientData(selection);
        m_pDocWindowTab->Display(data->text);

        m_pTabDoc->select();
    }
}

void wxDemoViewerFrame::OnLaunchDemo(wxCommandEvent &event)
{
    wxTreeItemId sid = m_pTreeCtrl->GetSelection();

    wxDemoViewerTreeItemData *tiData = (wxDemoViewerTreeItemData*)m_pTreeCtrl->GetItemData(sid);
    if(tiData!=NULL)
    {
        if(m_lPID!=0)
        {
            m_pProcessWatcher->Detach();
            wxProcess::Kill(m_lPID, wxSIGKILL, wxKILL_CHILDREN);
            m_lPID = 0;
        }

        wxString filename = tiData->data->exeFile;

        if(!m_appData.addedExt.IsEmpty()) filename += m_appData.addedExt;

        wxFileName fn(filename, wxPATH_UNIX);
        if(fn.FileExists())
        {
            wxFileName::SetCwd(fn.GetPath());

#if defined(__WXGTK__)
            wxString command = (m_appData.openWith.IsEmpty()) ? wxT("./") : m_appData.openWith;
#else
            wxString command = (m_appData.openWith.IsEmpty()) ? wxT("") : m_appData.openWith;
#endif

            command += fn.GetFullName();

            m_pProcessWatcher = new wxProcess(this, idLaunchedDemo);
            m_lPID = wxExecute(command, wxEXEC_ASYNC | wxEXEC_MAKE_GROUP_LEADER, m_pProcessWatcher);

            if(m_lPID!=0) wxLogMessage(wxT("Execute command: '%s' - successful"), command.c_str());
            else wxLogError(wxT("Execute command: '%s' - failed"), command.c_str());

            wxFileName::SetCwd(m_sMainWorkingDir);
        }
        else wxMessageBox(wxString::Format(wxT("File '%s' not found!"), filename.c_str()), wxT("Warning"), wxICON_EXCLAMATION);
    }
}

void wxDemoViewerFrame::OnTerminateDemo(wxProcessEvent &event)
{
    delete m_pProcessWatcher;
    m_pProcessWatcher = NULL;

    m_lPID = 0;

    Raise();
}

void wxDemoViewerFrame::OnChangeSkin(wxCommandEvent &event)
{
    skinDialog dlg(this, m_sMainWorkingDir, m_sExecWorkingDir);

    dlg.ShowModal();
}
