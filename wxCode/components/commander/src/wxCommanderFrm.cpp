//---------------------------------------------------------------------------
//
// Name:        wxCommanderFrm.cpp
// Author:      Armando Urdiales González
// Created:     06/12/2006 12:53:20
// Description: wxCommanderFrm class implementation
//
//---------------------------------------------------------------------------

#include "wxCommanderFrm.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
#include "Images/Self_wxCommanderFrm_XPM.xpm"
////Header Include End

//----------------------------------------------------------------------------
// wxCommanderFrm
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(wxCommanderFrm,wxFrame)
	////Manual Code Start
	EVT_LIST_ITEM_ACTIVATED(ID_WXLISTCTRL1, wxCommanderFrm::OnList1ItemActivated)
	EVT_LIST_ITEM_ACTIVATED(ID_WXLISTCTRL2, wxCommanderFrm::OnList2ItemActivated)
	EVT_LIST_KEY_DOWN(ID_WXLISTCTRL2,wxCommanderFrm::WxListCtrl2KeyDown)
	EVT_LIST_KEY_DOWN(ID_WXLISTCTRL1,wxCommanderFrm::WxListCtrl1KeyDown)
	EVT_TOOL(ID_TOOL_COPY, wxCommanderFrm::OnToolButton)
	EVT_TOOL(ID_TOOL_DELETE, wxCommanderFrm::OnToolButton)
	EVT_TOOL(ID_TOOL_RENAME, wxCommanderFrm::OnToolButton)
	EVT_TOOL(ID_TOOL_NEWFOLDER, wxCommanderFrm::OnToolButton)
	EVT_TOOL(ID_TOOL_EXEC, wxCommanderFrm::OnToolButton)
	EVT_TOOL(ID_TOOL_GO	, wxCommanderFrm::OnToolButton)
	EVT_COMBOBOX(ID_TOOL_COMBO, wxCommanderFrm::OnComboClick)
	EVT_TEXT_ENTER(ID_TOOL_COMBO, wxCommanderFrm::OnComboClick)
	EVT_LIST_ITEM_FOCUSED(ID_WXLISTCTRL2,wxCommanderFrm::WxListCtrl2ItemFocused)
	EVT_LIST_ITEM_FOCUSED(ID_WXLISTCTRL1,wxCommanderFrm::WxListCtrl1ItemFocused)
	EVT_LIST_ITEM_SELECTED(ID_WXLISTCTRL1,wxCommanderFrm::WxListCtrl1ItemFocused)
	EVT_LIST_ITEM_SELECTED(ID_WXLISTCTRL2,wxCommanderFrm::WxListCtrl2ItemFocused)
	EVT_LIST_BEGIN_LABEL_EDIT(ID_WXLISTCTRL1,wxCommanderFrm::WxListCtrlBeginLabelEdit)
	EVT_LIST_BEGIN_LABEL_EDIT(ID_WXLISTCTRL2,wxCommanderFrm::WxListCtrlBeginLabelEdit)
	EVT_LIST_END_LABEL_EDIT(ID_WXLISTCTRL1,wxCommanderFrm::WxListCtrlEndLabelEdit)
	EVT_LIST_END_LABEL_EDIT(ID_WXLISTCTRL2,wxCommanderFrm::WxListCtrlEndLabelEdit)
	////Manual Code End
	
	EVT_CLOSE(wxCommanderFrm::OnClose)
	EVT_MENU(ID_MNU_NEWFOLDER_1049, wxCommanderFrm::Mnu_newFolder_onClick)
	EVT_MENU(ID_MNU_COPY_1046, wxCommanderFrm::Mnu_copy_onClick)
	EVT_MENU(ID_MNU_DELETE_1047, wxCommanderFrm::Mnu_delete_onClick)
	EVT_MENU(ID_MNU_RENAME_1048, wxCommanderFrm::Mnu_rename_onClick)
	EVT_MENU(ID_MNU_EXECUTE_1051, wxCommanderFrm::Mnu_execute_onClick)
	EVT_MENU(ID_MNU_EXIT_1005, wxCommanderFrm::Mnu_exit_onClick)
	EVT_MENU(ID_MNU_LANGUAGE_1064, wxCommanderFrm::MnuLanguage_onClick)
	EVT_MENU(ID_MNU_HOTKEYS_1065, wxCommanderFrm::Mnu_hotKeys_onClick)
	EVT_MENU(ID_MNU_ABOUT_1007, wxCommanderFrm::Mnu_about_onClick)
END_EVENT_TABLE()
////Event Table End

using namespace std;

wxCommanderFrm::wxCommanderFrm(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style)
{
   lang.setLangsDir(".\\lang");

	CreateGUIControls();

   readConfig();
   updateControlsLanguage();

	lastListCtrlUsed = WxListCtrl1;
	strPathLstCtrl = &strPathLstCtrl1;

	WxStatusBar->SetStatusText("wxCommander");

   ListCtlUpdate();
}

wxCommanderFrm::~wxCommanderFrm()
{
   tray->RemoveIcon();
   delete(tray);
}

void wxCommanderFrm::CreateGUIControls()
{
	//Do not add custom code here
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxGridSizer = new wxGridSizer(1, 1, 0, 0);
	this->SetSizer(WxGridSizer);
	this->SetAutoLayout(true);

	WxSplitterWindow1 =new wxSplitterWindow(this, ID_WXSPLITTERWINDOW1, wxPoint(0,0), wxSize(409,920) );
	WxGridSizer->Add(WxSplitterWindow1,0,wxALIGN_CENTER | wxALL,0);

	WxListCtrl1 = new wxListCtrl(WxSplitterWindow1, ID_WXLISTCTRL1, wxPoint(5,5), wxSize(399,450), wxLC_REPORT | wxLC_EDIT_LABELS);

	WxListCtrl2 = new wxListCtrl(WxSplitterWindow1, ID_WXLISTCTRL2, wxPoint(414,5), wxSize(399,450), wxLC_REPORT | wxLC_EDIT_LABELS);

	WxToolBar = new wxToolBar(this, ID_WXTOOLBAR, wxPoint(0,0), wxSize(792,29));

	WxStatusBar = new wxStatusBar(this, ID_WXSTATUSBAR);

	WxMenuBar1 = new wxMenuBar();
	wxMenu *ID_MNU_FILES_1004_Mnu_Obj = new wxMenu(0);
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_NEWFOLDER_1049, wxT("&New folder (F7)"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_COPY_1046, wxT("&Copy         (F5)"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_DELETE_1047, wxT("&Delete      (Del)"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_RENAME_1048, wxT("&Rename      (F2)"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILES_1004_Mnu_Obj->AppendSeparator();
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_EXECUTE_1051, wxT("Execute     (F9)"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILES_1004_Mnu_Obj->AppendSeparator();
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_EXIT_1005, wxT("&Exit"), wxT(""), wxITEM_NORMAL);
	WxMenuBar1->Append(ID_MNU_FILES_1004_Mnu_Obj, wxT("&Files"));
	
	wxMenu *ID_MNU_OPTIONS_1063_Mnu_Obj = new wxMenu(0);
	ID_MNU_OPTIONS_1063_Mnu_Obj->Append(ID_MNU_LANGUAGE_1064, wxT("Language"), wxT(""), wxITEM_NORMAL);
	ID_MNU_OPTIONS_1063_Mnu_Obj->Append(ID_MNU_HOTKEYS_1065, wxT("Hot Keys"), wxT(""), wxITEM_NORMAL);
	WxMenuBar1->Append(ID_MNU_OPTIONS_1063_Mnu_Obj, wxT("Options"));
	
	wxMenu *ID_MNU_HELP_1006_Mnu_Obj = new wxMenu(0);
	ID_MNU_HELP_1006_Mnu_Obj->Append(ID_MNU_ABOUT_1007, wxT("About as... (F1)"), wxT(""), wxITEM_NORMAL);
	WxMenuBar1->Append(ID_MNU_HELP_1006_Mnu_Obj, wxT("&Help"));
	SetMenuBar(WxMenuBar1);

	WxSplitterWindow1->SplitVertically(WxListCtrl1,WxListCtrl2,385);

	SetStatusBar(WxStatusBar);
	WxToolBar->Realize();
	SetToolBar(WxToolBar);
	SetTitle(wxT("wxCommander"));
	SetIcon(Self_wxCommanderFrm_XPM);
	
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();
	
	////GUI Items Creation End
	wxStandardPaths wxPaths;
    if (strPathLstCtrl1.IsEmpty() || strPathLstCtrl2.IsEmpty())
    {
       if (strPathLstCtrl1.IsEmpty()) strPathLstCtrl1 = wxPaths.GetDocumentsDir();
       if (strPathLstCtrl2.IsEmpty()) strPathLstCtrl2 = wxPaths.GetDocumentsDir();
    }
    WxListCtrl1->SetSingleStyle(wxLB_MULTIPLE);
    WxListCtrl2->SetSingleStyle(wxLB_MULTIPLE);
    addColumns(WxListCtrl1);
    addColumns(WxListCtrl2);

    WxToolBar->AddSeparator();

    wxBitmap img(".\\Images\\newFolder.bmp", wxBITMAP_TYPE_BMP);
    if ( img.Ok() ) WxToolBar->AddTool(ID_TOOL_NEWFOLDER, lang["New Folder"], img, lang["New Folder"]);
    img.LoadFile(".\\Images\\copy.bmp", wxBITMAP_TYPE_BMP);
    if ( img.Ok() ) WxToolBar->AddTool(ID_TOOL_COPY, lang["Copy"], img, lang["Copy"]);
    img.LoadFile(".\\Images\\delete.bmp", wxBITMAP_TYPE_BMP);
    if ( img.Ok() ) WxToolBar->AddTool(ID_TOOL_DELETE, lang["Delete"], img, lang["Delete"]);
    img.LoadFile(".\\Images\\rename.bmp", wxBITMAP_TYPE_BMP);
    if ( img.Ok() ) WxToolBar->AddTool(ID_TOOL_RENAME, lang["Rename"], img, lang["Rename"]);
    img.LoadFile(".\\Images\\exec.bmp", wxBITMAP_TYPE_BMP);
    if ( img.Ok() ) WxToolBar->AddTool(ID_TOOL_EXEC, lang["Exec"], img, lang["Exec"]);
    combo = new wxComboBox(WxToolBar, ID_TOOL_COMBO, wxEmptyString, wxDefaultPosition, wxSize(650,wxDefaultCoord) );
    combo->Append(_T(wxPaths.GetDocumentsDir()));
    WxToolBar->AddControl(combo);

    WxToolBar->AddSeparator();

    img.LoadFile(".\\Images\\add.bmp", wxBITMAP_TYPE_BMP);
    if ( img.Ok() ) WxToolBar->AddTool(ID_TOOL_GO, lang["Go"], img, lang["Go"]);

    tray = new wxCommanderTaskBar(this);
    tray->SetIcon(Self_wxCommanderFrm_XPM, wxT("wxCommander"));

    WxToolBar->Realize();
    WxToolBar->SetRows(1);

    updateControlsLanguage();

    WxSplitterWindow1->SetSashPosition(0);
}

void wxCommanderFrm::updateControlsLanguage()
{
   wxMenu* optMenu = WxMenuBar1->GetMenu(0);
   WxMenuBar1->Replace(0, optMenu, lang["&Files"]);
   WxMenuBar1->SetLabel(ID_MNU_NEWFOLDER_1049, lang["&New folder (F7)"]);
   WxMenuBar1->SetLabel(ID_MNU_COPY_1046, lang["&Copy         (F5)"]);
   WxMenuBar1->SetLabel(ID_MNU_DELETE_1047, lang["&Delete      (Del)"]);
   WxMenuBar1->SetLabel(ID_MNU_RENAME_1048, lang["&Rename      (F2)"]);
   WxMenuBar1->SetLabel(ID_MNU_EXECUTE_1051, lang["Execute     (F9)"]);
   WxMenuBar1->SetLabel(ID_MNU_EXIT_1005, lang["&Exit"]);
   optMenu = WxMenuBar1->GetMenu(1);
   WxMenuBar1->Replace(1, optMenu, lang["&Options"]);
   WxMenuBar1->SetLabel(ID_MNU_LANGUAGE_1064, lang["&Language"]);
   optMenu = WxMenuBar1->GetMenu(2);
   WxMenuBar1->Replace(2, optMenu, lang["&Help"]);
   WxMenuBar1->SetLabel(ID_MNU_ABOUT_1007, lang["About as... (F1)"]);
   addColumns(WxListCtrl1);
   addColumns(WxListCtrl2);
}

void wxCommanderFrm::addColumns(wxListCtrl *WxListCtrl)
{
   while (WxListCtrl->GetColumnCount()>0) WxListCtrl->DeleteColumn(0);

   WxListCtrl->InsertColumn(0, lang["Files"], wxLIST_FORMAT_LEFT, 212);
   WxListCtrl->InsertColumn(1, lang["Size"], wxLIST_FORMAT_RIGHT, 65);
   WxListCtrl->InsertColumn(2, lang["Date"], wxLIST_FORMAT_RIGHT, 100);

   int size = 20;
   imageList = new wxImageList(size, size, true);

   //wxIcon icons[5];
   //icons[0] = wxIcon(icon1_xpm);
   //icons[1] = wxIcon(icon2_xpm);
   //icons[2] = wxIcon(icon3_xpm);
   //icons[3] = wxIcon(icon4_xpm);
   //icons[4] = wxIcon(icon5_xpm);

   wxIcon icons[3];
   icons[0] = wxIcon(icon1_xpm);
   icons[1] = wxIcon(icon3_xpm);
   icons[2] = wxIcon(icon5_xpm);


   int sizeOrig = icons[0].GetWidth();
   for ( size_t i = 0; i < WXSIZEOF(icons); i++ )
   {
       if ( size == sizeOrig )
           imageList->Add(icons[i]);
       else
           imageList->Add(wxBitmap(wxBitmap(icons[i]).ConvertToImage().Rescale(size, size)));
   }

   WxListCtrl->AssignImageList(imageList, wxIMAGE_LIST_SMALL);
}

void wxCommanderFrm::OnClose(wxCloseEvent& event)
{
   Close();
}

void wxCommanderFrm::Close()
{
   writeConfig();
   Destroy();
}

void wxCommanderFrm::itemExec(wxListCtrl *WxListCtrl, wxString &directory, wxString &itemName)
{
   wxDir dir;
   wxString path;
   if (directory.Right(1) == "\\")
      path = directory + itemName;
   else
      path = directory + "\\" + itemName;

   if (itemName == "..")
      path = directory.BeforeLast(wxT('\\'));
   if (dir.Exists(path))
   {
     directory=path;
     setListCtrl(WxListCtrl, directory);
     return;
   }
   if (itemName == "..") return;
      Exec(directory, itemName);
}

void wxCommanderFrm::OnList1ItemActivated(wxListEvent& event)
{
   wxString itemName = event.GetText();
   itemExec(WxListCtrl1, strPathLstCtrl1, itemName);
}

void wxCommanderFrm::OnList2ItemActivated(wxListEvent& event)
{
   wxString itemName = event.GetText();
   itemExec(WxListCtrl2, strPathLstCtrl2, itemName);
}

void wxCommanderFrm::Mnu_about_onClick(wxCommandEvent& event)
{
   wxAboutDialogInfo info;
   info.SetName(_T("wxCommander "));
   info.SetVersion("0.1");
   info.SetCopyright(wxString::FromAscii("(C) 2006 wxCommander GNU License \n"));
   info.AddDeveloper(_T("Armando Urdiales González"));
   info.SetWebSite(_T("http://www.wikiLinux-es.org/"), _T("AURGO web site"));
   info.AddArtist(_T("Developed with"));
   info.AddArtist(_T("\n\n\nwxDev-C++ (C++ IDE)"));
   info.AddArtist(_T("\n\nwxWidgets (Library)"));
   info.AddArtist(_T("\n\nMingW (C++ Compiler)"));
   info.SetLicence(wxString::FromAscii(
         "                GNU GENERAL PUBLIC LICENSE Version 2, June 1991\n"
         "                ===============================================\n"
         "\n"
         "  Everyone is permitted to copy and distribute copies\n"
         "  of this program.\n"
         "\n"
         "                GNU GENERAL PUBLIC LICENSE Version 2 \n"
         "     TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION\n"
         "\n"
             ));
   wxAboutBox(info);
}

void wxCommanderFrm::Mnu_copy_onClick(wxCommandEvent& event)
{
   copyThread();
}

void wxCommanderFrm::Mnu_delete_onClick(wxCommandEvent& event)
{
   long item = 0;
   if (wxMessageBox(lang["Do you like DELETE selected files?"], "wxCommander", wxYES_NO, this)!=wxYES) return;
   while (item != -1)
   {
      item = lastListCtrlUsed->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
      if ( item != -1 )
      {
         wxString itemName = lastListCtrlUsed->GetItemText(item);
         WxStatusBar->SetStatusText(lang["Delete"] + " " + itemName + "...");
         if (!deleteDirFile(*strPathLstCtrl, itemName)) wxMessageBox(lang["There are some files read only"], "wxCommander", wxOK | wxICON_ERROR , this);
      }
   }
   ListCtlUpdate();
   WxStatusBar->SetStatusText("wxCommander");
}

void wxCommanderFrm::Mnu_rename_onClick(wxCommandEvent& event)
{
   long item = lastListCtrlUsed->GetNextItem(0, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
   if ( item != -1 )
      lastListCtrlUsed->EditLabel(item);
}

void wxCommanderFrm::Mnu_newFolder_onClick(wxCommandEvent& event)
{
   wxTextEntryDialog execWnd(this, lang["Folder name :"], lang["Create folder"], lang["New Folder"], wxOK | wxCANCEL | wxCENTRE);
	execWnd.ShowModal();
	wxString folderName = execWnd.GetValue();
	if (folderName.IsEmpty() || folderName==lang["New Folder"]) return;
	folderName = (*strPathLstCtrl) + "\\" + folderName;
   wxMkdir(folderName);
   ListCtlUpdate();
}

void wxCommanderFrm::Mnu_exit_onClick(wxCommandEvent& event)
{
   Close();
}

void wxCommanderFrm::ListCtlUpdate()
{
   setListCtrl(WxListCtrl1, strPathLstCtrl1);
   setListCtrl(WxListCtrl2, strPathLstCtrl2);
}

void wxCommanderFrm::setListCtrl(wxListCtrl *WxListCtrl, wxString &directory)
{
    wxDir dir;
    wxString filename;
    wxString wxStrFileName;

    if (!dir.Exists(directory)) return;
    dir.Open(directory);
    if (!dir.IsOpened()) return;

    bool cont = dir.GetFirst(&filename);
    WxListCtrl->DeleteAllItems();
    WxListCtrl->InsertItem(0, "..",2);
    vector<wxString> aFiles;
    int numItems = 0;
    while (cont)
    {
       wxStrFileName = directory + "\\" + filename;
       if (dir.Exists(wxStrFileName))
       {
          numItems = WxListCtrl->GetItemCount();
          WxListCtrl->InsertItem(numItems, filename, 1);
          WxListCtrl->SetItem(numItems, 1, "<DIR>");
          WxListCtrl->SetItem(numItems, 2, getModificationTime(wxStrFileName));
       }
       else
         aFiles.push_back(filename);
       cont = dir.GetNext(&filename);
    }
    for (size_t i = 0 ; i < aFiles.size(); i++)
    {
       numItems = WxListCtrl->GetItemCount();
       wxStrFileName = directory + "\\" + aFiles[i];
       WxListCtrl->InsertItem(numItems, aFiles[i], 0);
       WxListCtrl->SetItem(numItems, 1, formatFileSize(getFileSize(wxStrFileName)));
       WxListCtrl->SetItem(numItems, 2, getModificationTime(wxStrFileName));
    }

    if (strPathLstCtrl->Right(1) == "\\")
       combo->SetValue(*strPathLstCtrl);
    else
       combo->SetValue(*strPathLstCtrl + "\\");
    wxString numDirFiles;
    numDirFiles << numItems;
    WxStatusBar->SetStatusText(numDirFiles + " " + lang["Directories and Files"]);
}

void OnChar(wxKeyEvent& event)
{
   
}

void wxCommanderFrm::OnListCtlKey(wxListCtrl *WxListCtrl, wxString &directory, wxListEvent& event)
{
    wxString key;
    key << event.GetKeyCode();
    switch (event.GetKeyCode())
    {
       case WXK_BACK:
       {
          wxString itemName = "..";
          itemExec(WxListCtrl, directory, itemName);
       }
       break;
       case WXK_DELETE:
          Mnu_delete_onClick(event);
       break;
       case WXK_F1:
          Mnu_about_onClick(event);
       break;
       case WXK_F2:
          Mnu_rename_onClick(event);
       break;  
       #ifdef __WXMSW__
       case WXK_F4:
          wxExecute("notepad.exe " + directory + "\\" + event.GetText());
       break;
       #endif
       case WXK_F5:
          copyThread();
       break;
       case WXK_F7:
          Mnu_newFolder_onClick(event);
       break;
       case WXK_F9:
          Mnu_execute_onClick(event);
       break;
       /*default:
       {
          wxMessageBox(key);
          hotKeyMap::iterator iter;
          for( iter = keysMap.begin(); iter != keysMap.end(); iter++ )
          {
             key << iter->second.keyCode;
             wxMessageBox(key);
             if (iter->second.keyCode == event.GetKeyCode())
                wxExecute(iter->second.program);
          }
       }*/
    }
}

void wxCommanderFrm::WxListCtrl1KeyDown(wxListEvent& event)
{
	OnListCtlKey(WxListCtrl1, strPathLstCtrl1, event);
}

void wxCommanderFrm::WxListCtrl2KeyDown(wxListEvent& event)
{
	OnListCtlKey(WxListCtrl2, strPathLstCtrl2, event);
}

void wxCommanderFrm::OnToolButton(wxCommandEvent& event)
{
   switch (event.GetId())
   {
      case ID_TOOL_COPY:
        copyThread();
        break;
      case ID_TOOL_DELETE:
        Mnu_delete_onClick(event);
        break;
      case ID_TOOL_RENAME:
        Mnu_rename_onClick(event);
        break;
      case ID_TOOL_NEWFOLDER:
        Mnu_newFolder_onClick(event);
        break;
      case ID_TOOL_EXEC:
        Mnu_execute_onClick(event);
        break;
      case ID_TOOL_GO:
        OnComboClick(event);
        break;
   }
}

void wxCommanderFrm::OnComboClick(wxCommandEvent& event)
{
   wxString strCombo = combo->GetValue();
   if (strCombo.Right(1) == "\\")
      strCombo = strCombo.Left(strCombo.Length()-1);

   if (strCombo.IsEmpty()) return setListCtrl(lastListCtrlUsed, *strPathLstCtrl);
   wxDir dir;
   if (!dir.Exists(strCombo)) return;
   *strPathLstCtrl = strCombo;
   setListCtrl(lastListCtrlUsed, strCombo);
   addDirsCombo(strCombo);
}

void wxCommanderFrm::addDirsCombo(wxString& strCombo)
{
   if (strCombo.Right(1) != "\\")
      strCombo = strCombo + "\\";

   if (combo->FindString(strCombo) == wxNOT_FOUND )
      combo->Append(strCombo);
}

void wxCommanderFrm::WxListCtrl1ItemFocused(wxListEvent& event)
{
	lastListCtrlUsed = WxListCtrl1;
	strPathLstCtrl = &strPathLstCtrl1;
   if (strPathLstCtrl->Right(1) == "\\")
      combo->SetValue(*strPathLstCtrl);
   else
      combo->SetValue(*strPathLstCtrl + "\\");
   wxString numDirFiles;
   numDirFiles << lastListCtrlUsed->GetItemCount()-1;
   WxStatusBar->SetStatusText(numDirFiles + " " + lang["Directories and Files"]);
}

void wxCommanderFrm::WxListCtrl2ItemFocused(wxListEvent& event)
{
	lastListCtrlUsed = WxListCtrl2;
	strPathLstCtrl = &strPathLstCtrl2;
   if (strPathLstCtrl->Right(1) == "\\")
      combo->SetValue(*strPathLstCtrl);
   else
      combo->SetValue(*strPathLstCtrl + "\\");
   wxString numDirFiles;
   numDirFiles << lastListCtrlUsed->GetItemCount()-1;
   WxStatusBar->SetStatusText(numDirFiles + " " + lang["Directories and Files"]);
}

void wxCommanderFrm::Mnu_execute_onClick(wxCommandEvent& event)
{
   wxConfig config("wxCommander");
   wxString strLastExecCmd;
   config.Read("LastExecCommand", &strLastExecCmd);
   config.Write("Language", lang.getActualLang());
	wxTextEntryDialog execWnd(this, lang["Write the command for execute in :"] + *strPathLstCtrl + "\\", lang["Exec command"], strLastExecCmd, wxOK | wxCANCEL | wxCENTRE);
	execWnd.ShowModal();
	wxString command = execWnd.GetValue();
	if (command.IsEmpty()) return;
	wxString execCommand = "Exec.bat " + *strPathLstCtrl + " " + command;
   wxShell(execCommand);
   config.Write("LastExecCommand", command);
}

void onThreadFinish(wxThread* thread, void* contextParam)
{
   CTimer* finishTimer;
   finishTimer = (CTimer*)contextParam;
   delete(finishTimer);
}

void wxCommanderFrm::copyThread()
{
   long item = 0;
   bool areItems=false;
   thread = new wxSimpleThread();
   timer = new CTimer(lang);
   size_t totalSize = 0;
   long numFiles = 0;
   while (item != -1)
   {
      item = lastListCtrlUsed->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
      if ( item != -1 )
      {
         areItems = true;
         wxString itemName = lastListCtrlUsed->GetItemText(item);
         wxString filePath;
         if (lastListCtrlUsed == WxListCtrl1)
         {
            thread->copy(strPathLstCtrl1, itemName, strPathLstCtrl2);
            timer->addDir(strPathLstCtrl2, itemName);
            filePath = strPathLstCtrl1 + "\\" + itemName;
         }
         else
         {
            thread->copy(strPathLstCtrl2, itemName, strPathLstCtrl1);
            timer->addDir(strPathLstCtrl1, itemName);
            filePath = strPathLstCtrl2 + "\\" + itemName;
         }
         totalSize+=getDirSize(filePath, numFiles);
      }
   }
   if (areItems)
   {
      timer->totalSize = totalSize;
      timer->numFiles = numFiles;
      thread->setFinishCallBackFunc(onThreadFinish, (void*)timer);
      thread->Create();
      thread->Run();
   }
   else
   {
      delete(thread);
      delete(timer);
   }
   wxWakeUpIdle();
   //ListCtlUpdate();
}

/*
 * MnuLanguage_onClick
 */
void wxCommanderFrm::MnuLanguage_onClick(wxCommandEvent& event)
{
   wxArrayString langsList = lang.getListLangs();
   langsList.Insert("Default (English)", 0);
   wxString language = wxGetSingleChoice(lang["Select your language"], lang["Language selector"], langsList, this);
   if (language.IsEmpty()) return;
   lang.setActualLang(language);
   lang.updateLangMap();
   updateControlsLanguage();
   ListCtlUpdate();
}

void wxCommanderFrm::readConfig()
{
   wxString language;
   wxConfig config("wxCommander");
   
   // restore frame position and size
   int x = config.Read(_T("/WindowPosition/X"), 50),
       y = config.Read(_T("/WindowPosition/Y"), 50),
       w = config.Read(_T("/WindowPosition/W"), 350),
       h = config.Read(_T("/WindowPosition/H"), 200);
   Move(x, y);
   SetClientSize(w, h);
  
   wxString strProgram;
   long keyCode;
   config.SetPath("HotKeys");
   bool bCont = config.GetFirstEntry(strProgram, keyCode);
   while ( bCont )
   {
     int pairNum = keysMap.size();
     keysMap[pairNum].program = strProgram;
     keysMap[pairNum].keyCode = keyCode;
     bCont = config.GetNextEntry(strProgram, keyCode);
   }
   
   config.SetPath("");

   config.Read("Language", &language);
   config.Read("LeftPath", &strPathLstCtrl1);
   config.Read("RightPath", &strPathLstCtrl2);

   wxString strCombo;
   wxString actualDir;
   for (int i=15; i > 0; i--)
   {
      actualDir << i;
      if (config.Exists("DirCombo/History" + actualDir))
      {
         config.Read("DirCombo/History" + actualDir, &strCombo);
         addDirsCombo(strCombo);
      }
      actualDir = "";
   }
 
   lang.setActualLang(language);
   lang.updateLangMap();
}

void wxCommanderFrm::writeConfig()
{
   wxConfig config("wxCommander");
   
   // save the frame position
   int x, y, w, h;
   GetClientSize(&w, &h);
   GetPosition(&x, &y);
   config.Write(_T("WindowPosition/X"), (long) x);
   config.Write(_T("WindowPosition/Y"), (long) y);
   config.Write(_T("WindowPosition/W"), (long) w);
   config.Write(_T("WindowPosition/H"), (long) h);
   
   config.DeleteGroup("HotKeys");
   hotKeyMap::iterator iter;
   for( iter = keysMap.begin(); iter != keysMap.end(); iter++ )
   {
      config.Write(_T("HotKeys/" + iter->second.program), (long) iter->second.keyCode);
   }
   
   config.Write("Language", lang.getActualLang());
   config.Write("LeftPath", strPathLstCtrl1);
   config.Write("RightPath", strPathLstCtrl2);
   config.DeleteGroup("DirCombo");
   wxString actualDir;
   int j=0;
   for (int i=combo->GetCount()-1; i > 0 && j <= 15; i--)
   {
      j++;
      actualDir << j;
      config.Write("DirCombo/History" + actualDir, combo->GetString(i));
      actualDir = "";
   }
}

void wxCommanderFrm::Mnu_hotKeys_onClick(wxCommandEvent& event)
{
   hotKeysDlg* dlgHotKeys = new hotKeysDlg(this);
   dlgHotKeys->setKeysMap(keysMap);
   if (dlgHotKeys->ShowModal() == wxID_OK)
   {
      keysMap = dlgHotKeys->getKeysMap();
   }
   delete(dlgHotKeys);
}

void wxCommanderFrm::WxListCtrlBeginLabelEdit(wxListEvent& event)
{
   wxListItem itemCtrl = event.GetItem();

   if (itemCtrl.GetId()==0)
       event.Veto();
}

void wxCommanderFrm::WxListCtrlEndLabelEdit(wxListEvent& event)
{
   wxString newFileName = event.m_item.m_text.c_str();
   wxListItem itemCtrl = event.GetItem();
   wxString itemName = lastListCtrlUsed->GetItemText(itemCtrl.GetId());

   if (!event.IsEditCancelled())
   {
	   if (!renameDirFile(*strPathLstCtrl, itemName, newFileName))
         event.Veto();
   }
}

