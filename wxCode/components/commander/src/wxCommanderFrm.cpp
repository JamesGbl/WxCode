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
	EVT_TOOL(ID_TOOL_ADD	, wxCommanderFrm::OnToolButton)
	EVT_TOOL(ID_TOOL_REMOVE	, wxCommanderFrm::OnToolButton)
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
	EVT_LIST_BEGIN_DRAG(ID_WXLISTCTRL1,wxCommanderFrm::WxListCtrlBeginDrag)
	EVT_LIST_BEGIN_DRAG(ID_WXLISTCTRL2,wxCommanderFrm::WxListCtrlBeginDrag)
	//EVT_CHAR_HOOK(wxCommanderFrm::OnCharHook)
	////Manual Code End
	
	EVT_CLOSE(wxCommanderFrm::OnClose)
	EVT_MENU(ID_MNU_ADDTAB_1080, wxCommanderFrm::Mnu_addTab_onClick)
	EVT_MENU(ID_MNU_REMOVETAB_1082, wxCommanderFrm::Mnu_removeTab_onClick)
	EVT_MENU(ID_MNU_NEWFOLDER_1049, wxCommanderFrm::Mnu_newFolder_onClick)
	EVT_MENU(ID_MNU_COPY_1046, wxCommanderFrm::Mnu_copy_onClick)
	EVT_MENU(ID_MNU_DELETE_1047, wxCommanderFrm::Mnu_delete_onClick)
	EVT_MENU(ID_MNU_RENAME_1048, wxCommanderFrm::Mnu_rename_onClick)
	EVT_MENU(ID_MNU_EXECUTE_1051, wxCommanderFrm::Mnu_execute_onClick)
	EVT_MENU(ID_MNU_EXIT_1005, wxCommanderFrm::Mnu_exit_onClick)
	EVT_MENU(ID_MNU_COPY_1084, wxCommanderFrm::Mnu_Copy_onClick1)
	EVT_MENU(ID_MNU_PASTE_1085, wxCommanderFrm::Mnu_paste_onClick)
	EVT_MENU(ID_MNU_LANGUAGE_1064, wxCommanderFrm::MnuLanguage_onClick)
	EVT_MENU(ID_MNU_HOTKEYS_1065, wxCommanderFrm::Mnu_hotKeys_onClick)
	EVT_MENU(ID_MNU_ABOUT_1007, wxCommanderFrm::Mnu_about_onClick)
	
	EVT_NOTEBOOK_PAGE_CHANGED(ID_WXNOTEBOOK2,wxCommanderFrm::WxNotebook2PageChanged)
	
	EVT_NOTEBOOK_PAGE_CHANGED(ID_WXNOTEBOOK1,wxCommanderFrm::WxNotebook1PageChanged)
END_EVENT_TABLE()
////Event Table End

using namespace std;

bool DragAndDropFile::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& fileNames)
{
   m_wxFrame.copyThread(m_strPathDest, fileNames);
   return true;
}

wxCommanderFrm::wxCommanderFrm(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style)
{   
	CreateGUIControls();

   lang.setLangsDir(".\\lang");
   readConfig();
   updateControlsLanguage();

	lastListCtrlUsed = WxListCtrl1;
	lastNoteBookUsed = WxNotebook1;
	strPathLstCtrl = &strPathLstCtrl1;
	lastVectorStringUsed = &aPaths1;

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

	WxToolBar = new wxToolBar(this, ID_WXTOOLBAR, wxPoint(0,0), wxSize(792,29));

	WxStatusBar = new wxStatusBar(this, ID_WXSTATUSBAR);

	WxGridSizer = new wxGridSizer(1, 1, 0, 0);
	this->SetSizer(WxGridSizer);
	this->SetAutoLayout(true);

	WxSplitterWindow1 =new wxSplitterWindow(this, ID_WXSPLITTERWINDOW1, wxPoint(0,0), wxSize(299,406) );
	WxGridSizer->Add(WxSplitterWindow1,0,wxALIGN_CENTER | wxALL,0);

	WxNotebook1 = new wxNotebook(WxSplitterWindow1, ID_WXNOTEBOOK1, wxPoint(5,5),wxSize(289,193));

	WxNotebook2 = new wxNotebook(WxSplitterWindow1, ID_WXNOTEBOOK2, wxPoint(304,5),wxSize(289,193));

	WxMenuBar1 = new wxMenuBar();
	wxMenu *ID_MNU_FILES_1004_Mnu_Obj = new wxMenu(0);
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_ADDTAB_1080, wxT("&Add Tab"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_REMOVETAB_1082, wxT("Re&move Tab"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILES_1004_Mnu_Obj->AppendSeparator();
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_NEWFOLDER_1049, wxT("&New folder"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_COPY_1046, wxT("&Copy"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_DELETE_1047, wxT("&Delete"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_RENAME_1048, wxT("&Rename"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILES_1004_Mnu_Obj->AppendSeparator();
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_EXECUTE_1051, wxT("E&xecute"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILES_1004_Mnu_Obj->AppendSeparator();
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_EXIT_1005, wxT("&Exit"), wxT(""), wxITEM_NORMAL);
	WxMenuBar1->Append(ID_MNU_FILES_1004_Mnu_Obj, wxT("&Files"));
	
	wxMenu *ID_MNU_EDIT_1083_Mnu_Obj = new wxMenu(0);
	ID_MNU_EDIT_1083_Mnu_Obj->Append(ID_MNU_COPY_1084, wxT("&Copy"), wxT(""), wxITEM_NORMAL);
	ID_MNU_EDIT_1083_Mnu_Obj->Append(ID_MNU_PASTE_1085, wxT("&Paste"), wxT(""), wxITEM_NORMAL);
	WxMenuBar1->Append(ID_MNU_EDIT_1083_Mnu_Obj, wxT("&Edit"));
	
	wxMenu *ID_MNU_OPTIONS_1063_Mnu_Obj = new wxMenu(0);
	ID_MNU_OPTIONS_1063_Mnu_Obj->Append(ID_MNU_LANGUAGE_1064, wxT("&Language"), wxT(""), wxITEM_NORMAL);
	ID_MNU_OPTIONS_1063_Mnu_Obj->Append(ID_MNU_HOTKEYS_1065, wxT("&Hot Keys"), wxT(""), wxITEM_NORMAL);
	WxMenuBar1->Append(ID_MNU_OPTIONS_1063_Mnu_Obj, wxT("&Options"));
	
	wxMenu *ID_MNU_HELP_1006_Mnu_Obj = new wxMenu(0);
	ID_MNU_HELP_1006_Mnu_Obj->Append(ID_MNU_ABOUT_1007, wxT("About as... (F1)"), wxT(""), wxITEM_NORMAL);
	WxMenuBar1->Append(ID_MNU_HELP_1006_Mnu_Obj, wxT("&Help"));
	SetMenuBar(WxMenuBar1);

	WxSplitterWindow1->SplitVertically(WxNotebook1,WxNotebook2,289);

	SetStatusBar(WxStatusBar);
	WxToolBar->Realize();
	SetToolBar(WxToolBar);
	SetTitle(wxT("wxCommander"));
	SetIcon(Self_wxCommanderFrm_XPM);
	
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();
	
	////GUI Items Creation End
	
	 WxListCtrl1 = new wxListCtrl(WxNotebook1, ID_WXLISTCTRL1, wxDefaultPosition, wxSize(399,450), wxLC_REPORT | wxLC_EDIT_LABELS);
    WxListCtrl2 = new wxListCtrl(WxNotebook2, ID_WXLISTCTRL2, wxDefaultPosition, wxSize(399,450), wxLC_REPORT | wxLC_EDIT_LABELS);

	 WxSplitterWindow1->SplitVertically(WxNotebook1,WxNotebook2,385);

	 wxStandardPaths wxPaths;

    if (strPathLstCtrl1.IsEmpty()) strPathLstCtrl1 = wxPaths.GetDocumentsDir();
    if (strPathLstCtrl2.IsEmpty()) strPathLstCtrl2 = wxPaths.GetDocumentsDir();
        
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
    combo = new wxComboBox(WxToolBar, ID_TOOL_COMBO, wxEmptyString, wxDefaultPosition, wxSize(600,wxDefaultCoord) );
    combo->Append(_T(wxPaths.GetDocumentsDir()));
    WxToolBar->AddControl(combo);

    WxToolBar->AddSeparator();

    img.LoadFile(".\\Images\\refresh.bmp", wxBITMAP_TYPE_BMP);
    if ( img.Ok() ) WxToolBar->AddTool(ID_TOOL_GO, lang["Go"], img, lang["Go"]);
    img.LoadFile(".\\Images\\add.bmp", wxBITMAP_TYPE_BMP);
    if ( img.Ok() ) WxToolBar->AddTool(ID_TOOL_ADD, lang["Add"], img, lang["Add"]);
    img.LoadFile(".\\Images\\remove.bmp", wxBITMAP_TYPE_BMP);
    if ( img.Ok() ) WxToolBar->AddTool(ID_TOOL_REMOVE, lang["Remove"], img, lang["Remove"]);

    tray = new wxCommanderTaskBar(this);
    tray->SetIcon(Self_wxCommanderFrm_XPM, wxT("wxCommander"));

    WxToolBar->Realize();
    WxToolBar->SetRows(1);
    
    WxListCtrl1->DragAcceptFiles(true);
    WxListCtrl1->SetDropTarget(new DragAndDropFile(*this, strPathLstCtrl1));

    WxListCtrl2->DragAcceptFiles(true);
    WxListCtrl2->SetDropTarget(new DragAndDropFile(*this, strPathLstCtrl2));
    
    updateControlsLanguage();
    
    WxListCtrl1->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(wxCommanderFrm::OnWxListCtrl1MouseLeftDown), NULL, this);
    WxListCtrl2->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(wxCommanderFrm::OnWxListCtrl2MouseLeftDown), NULL, this);

    WxSplitterWindow1->SetSashPosition(0);
}

void wxCommanderFrm::updateControlsLanguage()
{
   wxMenu* optMenu = WxMenuBar1->GetMenu(0);
   WxMenuBar1->Replace(0, optMenu, lang["&Files"]);
	WxMenuBar1->SetLabel(ID_MNU_ADDTAB_1080, lang["&Add Tab"] + " (Ctrl + T)");
	WxMenuBar1->SetLabel(ID_MNU_REMOVETAB_1082, lang["Re&move Tab"] + " (Ctrl + F4)");
   WxMenuBar1->SetLabel(ID_MNU_NEWFOLDER_1049, lang["&New folder"] + " (F7)");
   WxMenuBar1->SetLabel(ID_MNU_COPY_1046, lang["&Copy"] + " (F5)");
   WxMenuBar1->SetLabel(ID_MNU_DELETE_1047, lang["&Delete"] + " (Del)");
   WxMenuBar1->SetLabel(ID_MNU_RENAME_1048, lang["&Rename"] + " (F2)");
   WxMenuBar1->SetLabel(ID_MNU_EXECUTE_1051, lang["&Execute"] + " (F9)");
   WxMenuBar1->SetLabel(ID_MNU_EXIT_1005, lang["&Exit"]);
   optMenu = WxMenuBar1->GetMenu(1);
   WxMenuBar1->Replace(1, optMenu, lang["&Edit"]);
   WxMenuBar1->SetLabel(ID_MNU_COPY_1084, lang["&Copy"] + " (Ctrl + C)");
   WxMenuBar1->SetLabel(ID_MNU_PASTE_1085, lang["&Paste"] + " (Ctrl + V)");   
   optMenu = WxMenuBar1->GetMenu(2);
   WxMenuBar1->Replace(2, optMenu, lang["&Options"]);
   WxMenuBar1->SetLabel(ID_MNU_LANGUAGE_1064, lang["&Language"]);
   WxMenuBar1->SetLabel(ID_MNU_HOTKEYS_1065, lang["Hot Keys"]);
   optMenu = WxMenuBar1->GetMenu(3);
   WxMenuBar1->Replace(3, optMenu, lang["&Help"]);
   WxMenuBar1->SetLabel(ID_MNU_ABOUT_1007, lang["About as..."] + " (F1)");
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

   wxIcon icons[6];
   icons[0] = wxIcon(icon1_xpm);
   icons[1] = wxIcon(icon3_xpm);
   icons[2] = wxIcon(icon5_xpm);
   icons[3] = wxIcon(hardDisk_xpm);
   icons[4] = wxIcon(dvd_xpm);
   icons[5] = wxIcon(floppy_xpm);

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
   #ifdef __WXMSW__
   if (!wxFile::Exists(path))
   {      
      driversMap drives = wsGetDrives();
      if (drives.find(itemName) == drives.end()) 
         setListCtrl(WxListCtrl, drives);
      else 
      {
         directory=itemName;
         setListCtrl(WxListCtrl, itemName);
      }
      return;
   }
   #endif
   if (itemName == "..") 
      return;
   Exec(directory, itemName);
}

void wxCommanderFrm::OnList1ItemActivated(wxListEvent& event)
{
   wxString itemName = event.GetText();
   itemExec(WxListCtrl1, strPathLstCtrl1, itemName);

   WxNotebook1->SetPageText(WxNotebook1->GetSelection(), getLastDir(strPathLstCtrl1));
}

void wxCommanderFrm::OnList2ItemActivated(wxListEvent& event)
{
   wxString itemName = event.GetText();
   itemExec(WxListCtrl2, strPathLstCtrl2, itemName);

   WxNotebook2->SetPageText(WxNotebook2->GetSelection(), getLastDir(strPathLstCtrl2));
}

void wxCommanderFrm::Mnu_about_onClick(wxCommandEvent& event)
{
   wxAboutDialogInfo info;
   info.SetName(_T("wxCommander "));
   info.SetVersion("0.1 (BETA)");
   info.SetCopyright(wxString::FromAscii("(C) 2006 wxCommander wxWidgets License \n"));
   info.AddDeveloper(_T("Armando Urdiales González"));
   info.SetWebSite(_T("http://www.wikiLinux-es.org/"), _T("AURGO web site"));
   info.AddArtist(_T("Developed with"));
   info.AddArtist(_T("\n\n\nwxDev-C++ (C++ IDE)"));
   info.AddArtist(_T("\n\nwxWidgets (Library)"));
   info.AddArtist(_T("\n\nMingW (C++ Compiler)"));
   info.SetLicence(wxString::FromAscii(
         "                wxWIDGETS License\n"
         "                =================\n"
         "\n"
         "  Everyone is permitted to copy and distribute copies\n"
         "  of this program.\n"
         "\n"
         "                wxWIDGETS GENERAL PUBLIC LICENSE\n"
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
   
   WxNotebook1->SetPageText(WxNotebook1->GetSelection(), getLastDir(strPathLstCtrl1));
   WxNotebook2->SetPageText(WxNotebook2->GetSelection(), getLastDir(strPathLstCtrl2));
}

void wxCommanderFrm::setListCtrl(wxListCtrl *WxListCtrl, wxString &directory)
{
    wxDir dir;
    wxString filename;
    wxString wxStrFileName;

    if (!dir.Exists(directory)) 
    {
       WxListCtrl->DeleteAllItems();
       WxListCtrl->InsertItem(0, "..",2);       
       return;
    }
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
    lastNoteBookUsed->SetPageText(lastNoteBookUsed->GetSelection(), getLastDir(*strPathLstCtrl));
}

void wxCommanderFrm::setListCtrl(wxListCtrl *WxListCtrl, driversMap &drives)
{
   int numItems = 0;
   
   WxListCtrl->DeleteAllItems();

   driversMap::iterator iter;
   for( iter = drives.begin(); iter != drives.end(); iter++ )
   {
     numItems = WxListCtrl->GetItemCount();
     switch (iter->second)
     {
    	  case DRIVE_REMOVABLE:
           if (iter->first == "A:\\" || iter->first == "B:\\")
              WxListCtrl->InsertItem(numItems, iter->first, 5);
           else
              WxListCtrl->InsertItem(numItems, iter->first, 3);  
           WxListCtrl->SetItem(numItems, 1, lang["Removable"]);          
        break;
    	  case DRIVE_CDROM:
           WxListCtrl->InsertItem(numItems, iter->first, 4);
           WxListCtrl->SetItem(numItems, 1, lang["CD/DVD"]);          
        break;
        default:
           WxListCtrl->InsertItem(numItems, iter->first, 3);
           WxListCtrl->SetItem(numItems, 1, lang["Hard Disk"]);          
     }
   }
}

void wxCommanderFrm::OnListCtlKey(wxListCtrl *WxListCtrl, wxString &directory, wxListEvent& event)
{
   // User exec External Programs Keys
   hotKeyMap::iterator iter;
   for ( iter = keysMap.begin(); iter != keysMap.end(); iter++ )
   {
      long keyCode = iter->second.keyCode;
      if (keyCode >= WXK_F1 && keyCode <= WXK_F24 && wxGetKeyState(WXK_ALT))
         continue;
      else
      {
         if (keyCode == event.GetKeyCode())
         {
            wxString strCommand = iter->second.program;
            if (strPathLstCtrl->Right(1) != "\\") directory += "\\";
            strCommand.Replace("%1", directory, true);
            strCommand.Replace("%2", event.GetText(), true);
            wxExecute(strCommand);
         }
      }
   }
   
   // Hot Keys
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
       case WXK_F5:
          copyThread();
       break;
       case WXK_F7:
          Mnu_newFolder_onClick(event);
       break;
       case WXK_F9:
          Mnu_execute_onClick(event);
       break;
       case (int)'T':
       case (int)'t':
          if (wxGetKeyState(WXK_CONTROL))
             Mnu_addTab_onClick(event);
       break;
       case WXK_F4:
          if (wxGetKeyState(WXK_CONTROL))
             Mnu_removeTab_onClick(event);
       break;
       case (int)'C':
       case (int)'c':
            if (wxGetKeyState(WXK_CONTROL))
               copyToClipboard();
       break; 
       case (int)'V':
       case (int)'v':
            if (wxGetKeyState(WXK_CONTROL))
               pasteFromClipboard();
       break;
       //default:
       //{
          //wxString key;
          //key << event.GetKeyCode();
          //wxMessageBox(key);
       //}
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
      case ID_TOOL_ADD:
        comboClick(event, true);
        break;
      case ID_TOOL_REMOVE:
         wxString strValue = combo->GetValue();
         combo->Delete(combo->GetCurrentSelection());
         combo->SetValue(strValue);
        break;
   }
}

void wxCommanderFrm::OnComboClick(wxCommandEvent& event)
{
   comboClick(event, false);
}

void wxCommanderFrm::comboClick(wxCommandEvent& event, bool add)
{
   wxString strCombo = combo->GetValue();
   if (strCombo.Right(1) == "\\")
      strCombo = strCombo.Left(strCombo.Length()-1);

   if (strCombo.IsEmpty()) return setListCtrl(lastListCtrlUsed, *strPathLstCtrl);
   wxDir dir;
   if (!dir.Exists(strCombo)) return;
   *strPathLstCtrl = strCombo;
   setListCtrl(lastListCtrlUsed, strCombo);
   if (add) addDirsCombo(strCombo);
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
	lastNoteBookUsed = WxNotebook1;
	strPathLstCtrl = &strPathLstCtrl1;
	lastVectorStringUsed = &aPaths1;
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
	lastNoteBookUsed = WxNotebook2;
	strPathLstCtrl = &strPathLstCtrl2;
	lastVectorStringUsed = &aPaths2;
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

void wxCommanderFrm::Mnu_addTab_onClick(wxCommandEvent& event)
{
   lastVectorStringUsed->push_back(*strPathLstCtrl);
   lastNoteBookUsed->AddPage(lastListCtrlUsed, "");
   lastNoteBookUsed->AdvanceSelection(false);
   lastNoteBookUsed->SetPageText(lastNoteBookUsed->GetPageCount()-1, getLastDir(*strPathLstCtrl));
   lastListCtrlUsed->SetFocus();
}

void wxCommanderFrm::Mnu_removeTab_onClick(wxCommandEvent& event)
{
   if (lastNoteBookUsed->GetPageCount() > 1)
   {
      lastVectorStringUsed->erase(lastVectorStringUsed->begin()+lastNoteBookUsed->GetSelection());
      lastNoteBookUsed->RemovePage(lastNoteBookUsed->GetSelection());
      lastNoteBookUsed->AdvanceSelection(true);
      lastListCtrlUsed->SetFocus();
   }
}

void onThreadFinish(wxThread* thread, void* contextParam, void* parent)
{
   CTimer* finishTimer;
   wxCommanderFrm* parentWindow;
   finishTimer = (CTimer*)contextParam;
   parentWindow = (wxCommanderFrm*)parent;
   parentWindow->ListCtlUpdate();
   delete(finishTimer);
}

void wxCommanderFrm::copyThread(wxString& strPathDest,  const wxArrayString& fileNames)
{
   size_t nFiles = fileNames.GetCount();
   if (nFiles < 1) return;
   
   size_t totalSize = 0;
   long numFiles = 0;
   thread = new CThread();
   timer = new CTimer(lang);
   
   for (size_t n = 0; n < nFiles; n++ )
   {
      wxString& actualFile(fileNames[n]);
      wxString sourcePath = actualFile.BeforeLast(wxT('\\'));
      wxString fileName = actualFile.AfterLast(wxT('\\'));

      thread->copy(sourcePath, fileName, strPathDest);
      timer->addDir(strPathDest, fileName);
      totalSize+=getDirSize(actualFile, numFiles);
   }
   timer->totalSize = totalSize;
   timer->numFiles = numFiles;
   thread->setFinishCallBackFunc(onThreadFinish, (void*)timer, (void*)this);
   thread->Create();
   thread->Run();
}

void wxCommanderFrm::copyThread()
{
   long item = 0;
   bool areItems=false;
   thread = new CThread();
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
      thread->setFinishCallBackFunc(onThreadFinish, (void*)timer, (void*)this);
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
   long index;
   config.SetPath("HotKeys");
   bool bCont = config.GetFirstEntry(strProgram, index);
   while ( bCont )
   {
     int pairNum = keysMap.size();
     int intCode = config.Read(_T("/HotKeys/" + strProgram), 50);
     keysMap[pairNum].program = strProgram;
     keysMap[pairNum].keyCode = intCode;
     bCont = config.GetNextEntry(strProgram, index);
   }
   
   config.SetPath("");
   int j = 0;
   wxString nameNum = "0";   
   wxString strPath = "";
   bool blnExist = config.Exists("Tabs/Left/Dir" + nameNum);
   if (!blnExist) 
   {
     aPaths1.push_back(strPathLstCtrl1);
     WxNotebook1->AddPage(WxListCtrl1, getLastDir(strPathLstCtrl1), true);
	  WxNotebook1->AdvanceSelection(false);
   }
   while (blnExist)
   {
      config.Read("Tabs/Left/Dir" + nameNum, &strPath);
      aPaths1.push_back(strPath);
      strPathLstCtrl1=(j>0 ? aPaths1[j-1] : aPaths1[j]);
      WxNotebook1->AddPage(WxListCtrl1, getLastDir(strPath));
      WxNotebook1->AdvanceSelection(false);
      nameNum = "";
      j++;
      nameNum << j;
      blnExist = config.Exists("Tabs/Left/Dir" + nameNum);
   }

   j = 0;
   nameNum = "0";
   strPath = "";   
   blnExist = config.Exists("Tabs/Right/Dir" + nameNum);
   if (!blnExist)
   {
     aPaths2.push_back(strPathLstCtrl2);
	  WxNotebook2->AddPage(WxListCtrl2, getLastDir(strPathLstCtrl2), true);
     WxNotebook2->AdvanceSelection(false);
   }
   while (blnExist)
   {
      config.Read("Tabs/Right/Dir" + nameNum, &strPath);
      aPaths2.push_back(strPath);
      strPathLstCtrl2=(j>0 ? aPaths2[j-1] : aPaths2[j]);
      WxNotebook2->AddPage(WxListCtrl2, getLastDir(strPath));
      WxNotebook2->AdvanceSelection(false);
      nameNum = "";
      j++;
      nameNum << j;
      blnExist = config.Exists("Tabs/Right/Dir" + nameNum);
   }
   
   config.SetPath("");

   config.Read("Language", &language);

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
   
   wxString actualDir;
   int intTabSelLeft = WxNotebook1->GetSelection();
   int intTabSelRight = WxNotebook2->GetSelection();

   aPaths1[intTabSelLeft] =  strPathLstCtrl1;
   aPaths2[intTabSelRight] =  strPathLstCtrl2;
   
   config.DeleteGroup("Tabs");
   vectorString::iterator iterTab;
   int j = 0;
   for (iterTab = aPaths1.begin(); iterTab != aPaths1.end(); iterTab++)
   {
      actualDir << j;
      config.Write("Tabs/Left/Dir" + actualDir, aPaths1[j]);
      actualDir = "";
      j++;
   }
   j = 0;
   for (iterTab = aPaths2.begin(); iterTab != aPaths2.end(); iterTab++)
   {
      actualDir << j;
      config.Write("Tabs/Right/Dir" + actualDir, aPaths2[j]);
      actualDir = "";
      j++;
   }
   
   config.Write("Language", lang.getActualLang());
   
   config.DeleteGroup("DirCombo");
   for (int i=combo->GetCount()-1; i > 0; i--)
   {
      config.Write("DirCombo/History" + actualDir, combo->GetString(i));
      actualDir = "";
   }
}

void wxCommanderFrm::Mnu_hotKeys_onClick(wxCommandEvent& event)
{
   hotKeysDlg* dlgHotKeys = new hotKeysDlg(this);
   dlgHotKeys->setLanguage(lang);
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

void wxCommanderFrm::WxNotebook1PageChanged(wxNotebookEvent& event)
{
   int oldTab = event.GetOldSelection();
   int actualTab = event.GetSelection();
   
   if (oldTab != -1)
      aPaths1[oldTab] = strPathLstCtrl1;

   lastListCtrlUsed = WxListCtrl1;
	lastNoteBookUsed = WxNotebook1;
	strPathLstCtrl = &strPathLstCtrl1;
   lastVectorStringUsed = &aPaths1;
   
   (*strPathLstCtrl) = (*lastVectorStringUsed)[actualTab];
   setListCtrl(lastListCtrlUsed, *strPathLstCtrl);
}

void wxCommanderFrm::WxNotebook2PageChanged(wxNotebookEvent& event)
{
   int oldTab = event.GetOldSelection();
   int actualTab = event.GetSelection();

   if (oldTab != -1)
      aPaths2[oldTab] = strPathLstCtrl2;
   
   lastListCtrlUsed = WxListCtrl2;
	lastNoteBookUsed = WxNotebook2;
	strPathLstCtrl = &strPathLstCtrl2;
   lastVectorStringUsed = &aPaths2;

   (*strPathLstCtrl) = (*lastVectorStringUsed)[actualTab];
   setListCtrl(lastListCtrlUsed, *strPathLstCtrl);   
}

void wxCommanderFrm::WxListCtrlBeginDrag(wxListEvent& event)
{
	wxFileDataObject fileDataObject;
	long item = 0;
   while (item != -1)
   {
      item = lastListCtrlUsed->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
      if ( item != -1 )
      {
         wxString itemName = lastListCtrlUsed->GetItemText(item);
         fileDataObject.AddFile(*strPathLstCtrl + "\\" + itemName);
      }
   }
	wxDropSource dragSource( this );
	dragSource.SetData(fileDataObject);
	dragSource.DoDragDrop(TRUE);
}

void wxCommanderFrm::copyToClipboard()
{
   wxTheClipboard->UsePrimarySelection();
   if (wxTheClipboard->Open())
   {
      wxFileDataObject* pFileDataObject = new wxFileDataObject();
	   long item = 0;
      while (item != -1)
      {
         item = lastListCtrlUsed->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
         if ( item != -1 )
         {
            wxString itemName = lastListCtrlUsed->GetItemText(item);
            pFileDataObject->AddFile(*strPathLstCtrl + "\\" + itemName);
         }
      }
      wxTheClipboard->SetData( pFileDataObject );
      wxTheClipboard->Close();
   }
}

void wxCommanderFrm::pasteFromClipboard()
{
   wxTheClipboard->UsePrimarySelection();
   if (wxTheClipboard->Open())
   {
	   wxFileDataObject fileDataObject;
      if (wxTheClipboard->GetData(fileDataObject))
      {
         copyThread(*strPathLstCtrl, fileDataObject.GetFilenames());
      }
      wxTheClipboard->Close();
   }  
}

void wxCommanderFrm::OnWxListCtrl1MouseLeftDown(wxMouseEvent& event)
{
   wxListEvent listEvent;
   WxListCtrl1ItemFocused(listEvent);
   event.Skip();
}

void wxCommanderFrm::OnWxListCtrl2MouseLeftDown(wxMouseEvent& event)
{
   wxListEvent listEvent;
   WxListCtrl2ItemFocused(listEvent);
   event.Skip();
}

void wxCommanderFrm::Mnu_Copy_onClick1(wxCommandEvent& event)
{
   copyToClipboard();
}

void wxCommanderFrm::Mnu_paste_onClick(wxCommandEvent& event)
{
   pasteFromClipboard();
}
