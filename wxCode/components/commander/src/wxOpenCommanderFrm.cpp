//---------------------------------------------------------------------------
//
// Name:        wxOpenCommanderFrm.cpp
// Author:      Armando Urdiales González
// Created:     06/12/2006 12:53:20
// Description: wxOpenCommanderFrm class implementation
//
//---------------------------------------------------------------------------

#include "wxOpenCommanderFrm.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
#include "Images/Self_wxOpenCommanderFrm_XPM.xpm"
////Header Include End

#include "Images/add.xpm"
#include "Images/copy.xpm"
#include "Images/delete.xpm"
#include "Images/edit.xpm"
#include "Images/exec.xpm"
#include "Images/newFolder.xpm"
#include "Images/refresh.xpm"
#include "Images/remove.xpm"
#include "Images/computer.xpm"
#include "Images/filter.xpm"


//----------------------------------------------------------------------------
// wxOpenCommanderFrm
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(wxOpenCommanderFrm,wxFrame)
	////Manual Code Start
	EVT_LIST_ITEM_ACTIVATED(ID_WXLISTCTRL1, wxOpenCommanderFrm::OnList1ItemActivated)
	EVT_LIST_ITEM_ACTIVATED(ID_WXLISTCTRL2, wxOpenCommanderFrm::OnList2ItemActivated)
	EVT_LIST_KEY_DOWN(ID_WXLISTCTRL2,wxOpenCommanderFrm::WxListCtrl2KeyDown)
	EVT_LIST_KEY_DOWN(ID_WXLISTCTRL1,wxOpenCommanderFrm::WxListCtrl1KeyDown)
	EVT_TOOL(ID_TOOL_COPY, wxOpenCommanderFrm::OnToolButton)
	EVT_TOOL(ID_TOOL_DELETE, wxOpenCommanderFrm::OnToolButton)
	EVT_TOOL(ID_TOOL_RENAME, wxOpenCommanderFrm::OnToolButton)
	EVT_TOOL(ID_TOOL_NEWFOLDER, wxOpenCommanderFrm::OnToolButton)
	EVT_TOOL(ID_TOOL_EXEC, wxOpenCommanderFrm::OnToolButton)
	EVT_TOOL(ID_TOOL_GO	, wxOpenCommanderFrm::OnToolButton)
	EVT_TOOL(ID_TOOL_ADD	, wxOpenCommanderFrm::OnToolButton)
	EVT_TOOL(ID_TOOL_REMOVE	, wxOpenCommanderFrm::OnToolButton)
	EVT_TOOL(ID_TOOL_DRIVES	, wxOpenCommanderFrm::OnToolButton)
	EVT_TOOL(ID_TOOL_FILTER	, wxOpenCommanderFrm::OnToolButton)
	EVT_COMBOBOX(ID_TOOL_COMBO, wxOpenCommanderFrm::OnComboClick)
	EVT_TEXT_ENTER(ID_TOOL_COMBO, wxOpenCommanderFrm::OnComboClick)
	EVT_LIST_ITEM_FOCUSED(ID_WXLISTCTRL2,wxOpenCommanderFrm::WxListCtrl2ItemFocused)
	EVT_LIST_ITEM_FOCUSED(ID_WXLISTCTRL1,wxOpenCommanderFrm::WxListCtrl1ItemFocused)
	EVT_LIST_ITEM_SELECTED(ID_WXLISTCTRL1,wxOpenCommanderFrm::WxListCtrl1ItemFocused)
	EVT_LIST_ITEM_SELECTED(ID_WXLISTCTRL2,wxOpenCommanderFrm::WxListCtrl2ItemFocused)
	EVT_LIST_BEGIN_LABEL_EDIT(ID_WXLISTCTRL1,wxOpenCommanderFrm::WxListCtrlBeginLabelEdit)
	EVT_LIST_BEGIN_LABEL_EDIT(ID_WXLISTCTRL2,wxOpenCommanderFrm::WxListCtrlBeginLabelEdit)
	EVT_LIST_END_LABEL_EDIT(ID_WXLISTCTRL1,wxOpenCommanderFrm::WxListCtrlEndLabelEdit)
	EVT_LIST_END_LABEL_EDIT(ID_WXLISTCTRL2,wxOpenCommanderFrm::WxListCtrlEndLabelEdit)
	EVT_LIST_BEGIN_DRAG(ID_WXLISTCTRL1,wxOpenCommanderFrm::WxListCtrlBeginDrag)
	EVT_LIST_BEGIN_DRAG(ID_WXLISTCTRL2,wxOpenCommanderFrm::WxListCtrlBeginDrag)
	EVT_CONTEXT_MENU(wxOpenCommanderFrm::onContextMenu)
	//EVT_CHAR_HOOK(wxOpenCommanderFrm::OnCharHook)
	////Manual Code End
	
	EVT_CLOSE(wxOpenCommanderFrm::OnClose)
	EVT_MENU(ID_MNU_ADDTAB_1080, wxOpenCommanderFrm::Mnu_addTab_onClick)
	EVT_MENU(ID_MNU_REMOVETAB_1082, wxOpenCommanderFrm::Mnu_removeTab_onClick)
	EVT_MENU(ID_MNU_NEWFOLDER_1049, wxOpenCommanderFrm::Mnu_newFolder_onClick)
	EVT_MENU(ID_MNU_COPY_1046, wxOpenCommanderFrm::Mnu_copy_onClick)
	EVT_MENU(ID_MNU_DELETE_1047, wxOpenCommanderFrm::Mnu_delete_onClick)
	EVT_MENU(ID_MNU_RENAME_1048, wxOpenCommanderFrm::Mnu_rename_onClick)
	EVT_MENU(ID_MNU_MASSIVERENAME_1089, wxOpenCommanderFrm::Mnu_MassiveRename_onClick)
	EVT_MENU(ID_MNU_FILTER_1088, wxOpenCommanderFrm::Mnu_filter_onClick)
	EVT_MENU(ID_MNU_EXECUTE_1051, wxOpenCommanderFrm::Mnu_execute_onClick)
	EVT_MENU(ID_MNU_EXIT_1005, wxOpenCommanderFrm::Mnu_exit_onClick)
	EVT_MENU(ID_MNU_COPY_1084, wxOpenCommanderFrm::Mnu_Copy_onClick1)
	EVT_MENU(ID_MNU_PASTE_1085, wxOpenCommanderFrm::Mnu_paste_onClick)
	EVT_MENU(ID_MNU_LANGUAGE_1064, wxOpenCommanderFrm::MnuLanguage_onClick)
	EVT_MENU(ID_MNU_HOTKEYS_1065, wxOpenCommanderFrm::Mnu_hotKeys_onClick)
	EVT_MENU(ID_MNU_HELP_1087, wxOpenCommanderFrm::Mnu_Help_onClick)
	EVT_MENU(ID_MNU_CHECKUPDATES_1086, wxOpenCommanderFrm::Mnu_checkUpdates_onClick)
	EVT_MENU(ID_MNU_ABOUT_1007, wxOpenCommanderFrm::Mnu_about_onClick)
	
	EVT_NOTEBOOK_PAGE_CHANGED(ID_WXNOTEBOOK2,wxOpenCommanderFrm::WxNotebook2PageChanged)
	
	EVT_NOTEBOOK_PAGE_CHANGED(ID_WXNOTEBOOK1,wxOpenCommanderFrm::WxNotebook1PageChanged)
END_EVENT_TABLE()
////Event Table End

using namespace std;

bool DragAndDropFile::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& fileNames)
{
   wxString pathDest = m_cCommanderDest.getActualPath();
   m_wxFrame.copyThread(pathDest, fileNames, &m_cCommanderDest);
   return true;
}

bool DragAndDropButton::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& fileNames)
{
   wxMessageBox("Not yet implemented", "wxOpenCommander");
   return true;
}

wxOpenCommanderFrm::wxOpenCommanderFrm(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style)
{  
   
	CreateGUIControls();

	lastListCtrlUsed = WxListCtrl2;
	lastNoteBookUsed = WxNotebook2;
	lastCCommanderUsed = &cCommander2;
	
   lang.setLangsDir(".\\lang");
   isReadingConfig=true;
   readConfig();
   isReadingConfig=false;
   updateControlsLanguage();

	WxStatusBar->SetStatusText("wxOpenCommander");
	
	WxListCtrl2->SetBackgroundColour(wxColour(255, 255, 255));
   WxListCtrl1->SetBackgroundColour(wxColour(240, 240, 240));

   ListCtlUpdate();   
}

wxOpenCommanderFrm::~wxOpenCommanderFrm()
{
   tray->RemoveIcon();
   delete(tray);
}

void wxOpenCommanderFrm::CreateGUIControls()
{
	//Do not add custom code here
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxToolBar = new wxToolBar(this, ID_WXTOOLBAR, wxPoint(0,0), wxSize(792,29));

	WxStatusBar = new wxStatusBar(this, ID_WXSTATUSBAR);

	WxGridSizer = new wxGridSizer(0, 1, 0, 0);
	this->SetSizer(WxGridSizer);
	this->SetAutoLayout(true);

	WxSplitterWindow1 = new wxSplitterWindow(this, ID_WXSPLITTERWINDOW1, wxPoint(0,0), wxSize(608,205));
	WxGridSizer->Add(WxSplitterWindow1,0,wxALIGN_CENTER | wxALL,0);

	WxNotebook1 = new wxNotebook(WxSplitterWindow1, ID_WXNOTEBOOK1, wxPoint(5,5),wxSize(289,195));

	WxNotebook2 = new wxNotebook(WxSplitterWindow1, ID_WXNOTEBOOK2, wxPoint(304,6),wxSize(289,193));

	WxMenuBar1 = new wxMenuBar();
	wxMenu *ID_MNU_FILES_1004_Mnu_Obj = new wxMenu(0);
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_ADDTAB_1080, wxT("&Add Tab"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_REMOVETAB_1082, wxT("Re&move Tab"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILES_1004_Mnu_Obj->AppendSeparator();
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_NEWFOLDER_1049, wxT("&New folder"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_COPY_1046, wxT("&Copy"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_DELETE_1047, wxT("&Delete"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_RENAME_1048, wxT("&Rename"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_MASSIVERENAME_1089, wxT("Massive rename"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILES_1004_Mnu_Obj->Append(ID_MNU_FILTER_1088, wxT("&Filter"), wxT(""), wxITEM_NORMAL);
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
	ID_MNU_HELP_1006_Mnu_Obj->Append(ID_MNU_HELP_1087, wxT("&Help"), wxT(""), wxITEM_NORMAL);
	ID_MNU_HELP_1006_Mnu_Obj->Append(ID_MNU_CHECKUPDATES_1086, wxT("&Check updates"), wxT(""), wxITEM_NORMAL);
	ID_MNU_HELP_1006_Mnu_Obj->Append(ID_MNU_ABOUT_1007, wxT("About as... (F1)"), wxT(""), wxITEM_NORMAL);
	WxMenuBar1->Append(ID_MNU_HELP_1006_Mnu_Obj, wxT("&Help"));
	SetMenuBar(WxMenuBar1);

	WxSplitterWindow1->SplitVertically(WxNotebook1,WxNotebook2,289);

	SetStatusBar(WxStatusBar);
	WxToolBar->Realize();
	SetToolBar(WxToolBar);
	SetTitle(wxT("wxOpenCommander"));
	SetIcon(Self_wxOpenCommanderFrm_XPM);
	
	GetSizer()->Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();
	
	////GUI Items Creation End
	
	 WxListCtrl1 = new wxOpenCommanderListCtrl(WxNotebook1, ID_WXLISTCTRL1, wxDefaultPosition, wxSize(399,450), wxLC_REPORT | wxLC_VIRTUAL | wxLC_EDIT_LABELS, &cCommander1, lang);
    WxListCtrl2 = new wxOpenCommanderListCtrl(WxNotebook2, ID_WXLISTCTRL2, wxDefaultPosition, wxSize(399,450), wxLC_REPORT | wxLC_VIRTUAL | wxLC_EDIT_LABELS, &cCommander2, lang);

	 WxSplitterWindow1->SplitVertically(WxNotebook1,WxNotebook2,385);
        
    WxListCtrl1->SetSingleStyle(wxLB_EXTENDED);
    WxListCtrl2->SetSingleStyle(wxLB_EXTENDED);

    wxStandardPaths wxPaths;

    WxToolBar->AddSeparator();
    WxToolBar->AddTool(ID_TOOL_NEWFOLDER, lang["New Folder"], newFolder_xpm, lang["New Folder"]);
    WxToolBar->AddTool(ID_TOOL_COPY, lang["Copy"], copy_xpm, lang["Copy"]);
    WxToolBar->AddTool(ID_TOOL_DELETE, lang["Delete"], delete_xpm, lang["Delete"]);
    WxToolBar->AddTool(ID_TOOL_RENAME, lang["Rename"], edit_xpm, lang["Rename"]);
    WxToolBar->AddTool(ID_TOOL_EXEC, lang["Exec"], exec_xpm, lang["Exec"]);
    combo = new wxComboBox(WxToolBar, ID_TOOL_COMBO, wxEmptyString, wxDefaultPosition, wxSize(600,wxDefaultCoord) );
    combo->Append(_T(wxPaths.GetDocumentsDir()));
    
    WxToolBar->AddControl(combo);

    WxToolBar->AddSeparator();

    WxToolBar->AddTool(ID_TOOL_GO, lang["Go"], refresh_xpm, lang["Refresh"]);
    WxToolBar->AddTool(ID_TOOL_ADD, lang["Add"], add_xpm, lang["Add"]);
    WxToolBar->AddTool(ID_TOOL_REMOVE, lang["Delete"], remove_xpm, lang["Delete"]);
    WxToolBar->AddTool(ID_TOOL_DRIVES, lang["Devices"], computer_xpm, lang["Devices"]);
    WxToolBar->AddTool(ID_TOOL_FILTER, lang["Filter"], filter_xpm, lang["Filter"]);

    
    wxMenu* optMenu = WxMenuBar1->GetMenu(0);
    wxMenuItem* subItem = optMenu->FindItem(ID_MNU_NEWFOLDER_1049);
    subItem->SetBitmap(newFolder_xpm);
    subItem = optMenu->FindItem(ID_MNU_COPY_1046);
    subItem->SetBitmap(copy_xpm);
    subItem = optMenu->FindItem(ID_MNU_DELETE_1047);
    subItem->SetBitmap(delete_xpm);
    subItem = optMenu->FindItem(ID_MNU_RENAME_1048);
    subItem->SetBitmap(edit_xpm);
    subItem = optMenu->FindItem(ID_MNU_FILTER_1088);
    subItem->SetBitmap(filter_xpm);    
    subItem = optMenu->FindItem(ID_MNU_EXECUTE_1051);
    subItem->SetBitmap(exec_xpm);


    tray = new wxOpenCommanderTaskBar(this, lang);
    tray->SetIcon(Self_wxOpenCommanderFrm_XPM, wxT("wxOpenCommander"));

    WxToolBar->Realize();
    WxToolBar->SetRows(1);
    
    WxListCtrl1->DragAcceptFiles(true);
    WxListCtrl1->SetDropTarget(new DragAndDropFile(*this, cCommander1));

    WxListCtrl2->DragAcceptFiles(true);
    WxListCtrl2->SetDropTarget(new DragAndDropFile(*this, cCommander2));
    
    WxToolBar->DragAcceptFiles(true);
    WxToolBar->SetDropTarget(new DragAndDropButton(*this, WxToolBar));
    
    updateControlsLanguage();
    
    WxListCtrl1->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(wxOpenCommanderFrm::OnWxListCtrl1MouseLeftDown), NULL, this);
    WxListCtrl2->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(wxOpenCommanderFrm::OnWxListCtrl2MouseLeftDown), NULL, this);

    WxSplitterWindow1->SetSashPosition(0);
}

void wxOpenCommanderFrm::updateControlsLanguage()
{
   wxMenu* optMenu = WxMenuBar1->GetMenu(0);
   WxMenuBar1->Replace(0, optMenu, lang["&Files"]);
	WxMenuBar1->SetLabel(ID_MNU_ADDTAB_1080, lang["&Add Tab"] + " (Ctrl + T)");
	WxMenuBar1->SetLabel(ID_MNU_REMOVETAB_1082, lang["Re&move Tab"] + " (Ctrl + F4)");
   WxMenuBar1->SetLabel(ID_MNU_NEWFOLDER_1049, lang["&New folder"] + " (F7)");
   WxMenuBar1->SetLabel(ID_MNU_COPY_1046, lang["&Copy"] + " (F5)");
   WxMenuBar1->SetLabel(ID_MNU_DELETE_1047, lang["&Delete"] + " (Del)");
   WxMenuBar1->SetLabel(ID_MNU_RENAME_1048, lang["&Rename"] + " (F2)");
   WxMenuBar1->SetLabel(ID_MNU_MASSIVERENAME_1089, lang["Massive rename"]);
   
   WxMenuBar1->SetLabel(ID_MNU_FILTER_1088, lang["&Filter"] + " (F8)");
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
   WxMenuBar1->SetLabel(ID_MNU_HELP_1087, lang["&Help"] + " (F1)");
   WxMenuBar1->SetLabel(ID_MNU_CHECKUPDATES_1086, lang["&Check updates"]);
   WxMenuBar1->SetLabel(ID_MNU_ABOUT_1007, lang["About as..."]);
   
   WxToolBar->SetToolShortHelp(ID_TOOL_NEWFOLDER, lang["New Folder"]);
   WxToolBar->SetToolShortHelp(ID_TOOL_COPY, lang["Copy"]);
   WxToolBar->SetToolShortHelp(ID_TOOL_DELETE, lang["Delete"]);
   WxToolBar->SetToolShortHelp(ID_TOOL_RENAME, lang["Rename"]);
   WxToolBar->SetToolShortHelp(ID_TOOL_EXEC, lang["Exec"]);
   WxToolBar->SetToolShortHelp(ID_TOOL_GO, lang["Refresh"]);
   WxToolBar->SetToolShortHelp(ID_TOOL_ADD, lang["Add"]);
   WxToolBar->SetToolShortHelp(ID_TOOL_REMOVE, lang["Delete"]);

   WxToolBar->SetToolShortHelp(ID_TOOL_GO, lang["Go"]);
   WxToolBar->SetToolShortHelp(ID_TOOL_ADD, lang["Add"]);
   WxToolBar->SetToolShortHelp(ID_TOOL_REMOVE, lang["Delete"]);
   WxToolBar->SetToolShortHelp(ID_TOOL_DRIVES, lang["Devices"]);
   WxToolBar->SetToolShortHelp(ID_TOOL_FILTER, lang["Filter"]);

   addColumns(WxListCtrl1);
   addColumns(WxListCtrl2);
}

void wxOpenCommanderFrm::addColumns(wxListCtrl* WxListCtrl)
{
   while (WxListCtrl->GetColumnCount()>0) WxListCtrl->DeleteColumn(0);

   WxListCtrl->InsertColumn(0, lang["Files"], wxLIST_FORMAT_LEFT, 212);
   WxListCtrl->InsertColumn(1, lang["Extension"], wxLIST_FORMAT_LEFT, 65);
   WxListCtrl->InsertColumn(2, lang["Size"], wxLIST_FORMAT_RIGHT, 65);
   WxListCtrl->InsertColumn(3, lang["Date"], wxLIST_FORMAT_RIGHT, 100);
   
   int size = 20;
   
   wxImageList* imageList; // = WxListCtrl->GetImageList(wxIMAGE_LIST_SMALL);
   //if (imageList != NULL) delete(imageList); // MEMORY LEAK
   imageList = new wxImageList(size, size, true); // MEMORY LEAK
   
   imageList->Add(wxArtProvider::GetIcon(wxART_NORMAL_FILE, wxART_CMN_DIALOG, wxSize(size, size)));
   imageList->Add(wxArtProvider::GetIcon(wxART_FOLDER, wxART_CMN_DIALOG, wxSize(size, size)));
   imageList->Add(wxArtProvider::GetIcon(wxART_EXECUTABLE_FILE , wxART_CMN_DIALOG, wxSize(size, size)));
   imageList->Add(wxArtProvider::GetIcon(wxART_HARDDISK, wxART_CMN_DIALOG, wxSize(size, size)));
   imageList->Add(wxArtProvider::GetIcon(wxART_CDROM, wxART_CMN_DIALOG, wxSize(size, size)));
   imageList->Add(wxArtProvider::GetIcon(wxART_FLOPPY, wxART_CMN_DIALOG, wxSize(size, size)));
   imageList->Add(wxArtProvider::GetIcon(wxART_REMOVABLE, wxART_CMN_DIALOG, wxSize(size, size)));
   
   WxListCtrl->AssignImageList(imageList, wxIMAGE_LIST_SMALL);
}

void wxOpenCommanderFrm::OnClose(wxCloseEvent& event)
{
   Close();
}

void wxOpenCommanderFrm::Close()
{
   writeConfig();
   Destroy();
}

void wxOpenCommanderFrm::itemExec(cCommander *CCommander, wxNotebook* WxNotebook, wxListCtrl *WxListCtrl, wxString &directory, wxString &itemName)
{  
   wxDir dir;
   wxString path = directory;
         
   if (directory.Right(1) == "\\")
      path = directory + itemName;
   else 
   {
      if (itemName != "")
        path = directory + "\\" + itemName;  
   }
   
   if (itemName == "..")
      path = directory.BeforeLast(wxT('\\'));
   
   if (dir.Exists(path))
   {
     directory=path;
     setListCtrl(CCommander, WxNotebook, WxListCtrl, directory);
     return;
   }  
   #ifdef __WXMSW__
   if (!wxFile::Exists(path))
   {      
      driversMap drives = wxGetDrives();
      if (drives.find(itemName) == drives.end()) 
         setListCtrlDevices(CCommander, WxListCtrl);
      else 
      {
         directory=itemName;
         setListCtrl(CCommander, WxNotebook, WxListCtrl, itemName);
      }
      return;
   }
   #endif
   if (itemName == "..") 
      return;
   Exec(directory, itemName);
}

void wxOpenCommanderFrm::OnList1ItemActivated(wxListEvent& event)
{
   wxString itemName = event.GetText();
   wxString lastPath = cCommander1.getActualPath();
   if (cCommander1.getListDevices())
   {
      lastPath = itemName;
      itemName = "";
   }
   itemExec(&cCommander1, WxNotebook1, WxListCtrl1, lastPath, itemName);

   WxNotebook1->SetPageText(WxNotebook1->GetSelection(), getLastDir(lastPath));
}

void wxOpenCommanderFrm::OnList2ItemActivated(wxListEvent& event)
{
   wxString itemName = event.GetText();
   wxString lastPath = cCommander2.getActualPath();
   if (cCommander2.getListDevices()) 
   {
      lastPath = itemName;
      itemName = "";
   }
   itemExec(&cCommander2, WxNotebook2, WxListCtrl2, lastPath, itemName);

   WxNotebook2->SetPageText(WxNotebook2->GetSelection(), getLastDir(lastPath));
}

void wxOpenCommanderFrm::Mnu_about_onClick(wxCommandEvent& event)
{
   wxAboutDialogInfo info;
   info.SetName(_T("wxOpenCommander "));
   info.SetVersion("0.1 (BETA)");
   info.SetCopyright(wxString::FromAscii("(C) 2007 wxOpenCommander GPL License \n"));
   info.AddDeveloper(_T("Armando Urdiales González"));
   info.SetWebSite(_T("http://www.wxOpenCommander.com/"), _T("wxOpenCommander web site"));
   info.AddArtist(_T("Translate by:"));
   info.AddArtist(_T("\n\n\Jens Saathoff     (German/Deutsch)"));
   info.AddArtist(_T("\n\n\n\n\Developed with:"));
   info.AddArtist(_T("\n\n\nwxDev-C++ (C++ IDE)"));
   info.AddArtist(_T("\n\nwxWidgets (Library)"));
   info.AddArtist(_T("\n\nMingW (C++ Compiler)"));
   info.AddArtist(_T("\n\nNSIS (Nullsoft Scriptable Install System)"));
   info.AddArtist(_T("\n\nGraphic Designer Silvestre Herrera (Gnome Themes - Gion and Neu)"));

   info.SetLicence(wxString::FromAscii(
         "                GPL License\n"
         "                =================\n"
         "\n"
         "  Everyone is permitted to copy and distribute copies\n"
         "  of this program.\n"
         "\n"
         "                GPL GENERAL PUBLIC LICENSE\n"
         "     TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION\n"
         "\n"
             ));
   wxAboutBox(info);
}

void wxOpenCommanderFrm::Mnu_copy_onClick(wxCommandEvent& event)
{
   copyThread();
}

void wxOpenCommanderFrm::Mnu_delete_onClick(wxCommandEvent& event)
{
   long item = 0;
   bool blnErr=false;
   if (wxMessageBox(lang["Do you like DELETE selected files?"], "wxOpenCommander", wxYES_NO, this)!=wxYES) return;
   while (item != -1)
   {
      item = lastListCtrlUsed->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
      if ( item != -1 )
      {
         wxString itemName = lastListCtrlUsed->GetItemText(item);
         WxStatusBar->SetStatusText(lang["Delete"] + " " + itemName + "...");
         wxString strPath = lastCCommanderUsed->getActualPath();
         if (!deleteDirFile(strPath, itemName)) blnErr = true;
      }
   }
   if (blnErr) wxMessageBox(lang["There are some files read only"], "wxOpenCommander", wxOK | wxICON_ERROR , this);
   ListCtlUpdate();
}

void wxOpenCommanderFrm::Mnu_rename_onClick(wxCommandEvent& event)
{
   long item = lastListCtrlUsed->GetNextItem(0, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
   if ( item != -1 )
      lastListCtrlUsed->EditLabel(item);
}

void wxOpenCommanderFrm::Mnu_newFolder_onClick(wxCommandEvent& event)
{
   wxTextEntryDialog execWnd(this, lang["Folder name :"], lang["Create folder"], lang["New Folder"], wxOK | wxCANCEL | wxCENTRE);
	execWnd.ShowModal();
	wxString folderName = execWnd.GetValue();
	if (folderName.IsEmpty() || folderName==lang["New Folder"]) return;
	folderName = lastCCommanderUsed->getActualPath() + "\\" + folderName;
   wxMkdir(folderName);
   ListCtlUpdate();
}

void wxOpenCommanderFrm::Mnu_MassiveRename_onClick(wxCommandEvent& event)
{
   wxString massiveName;
   wxString caption = lang["Rename ALL SELECTED files"] + "\n" + lang["Example:"] + "\n   " + lang["NewFileName (%n).ext"] + "\n      " + lang["%n - Number of order"];
   wxTextEntryDialog execWnd(this, caption, lang["Rename ALL SELECTED files"], lang["NewFileName (%n).ext"], wxOK | wxCANCEL | wxCENTRE);
   do 
   {
	   if (execWnd.ShowModal()==wxID_CANCEL) return;
	   massiveName = execWnd.GetValue();
	   if (massiveName.IsEmpty() || massiveName==lang["NewFileName (%n).ext"]) return;
	   if (massiveName.Find('%n')== -1) wxMessageBox(lang["You must put the order number '%n'"], lang["Error"]);
   }
	while (massiveName.Find('%n') < 0);
	
	massiveName.Replace("%n", "%d", true);
	
   long item = 0;
   long fileNum = 0;
   bool blnErr = false;
   wxString itemName;
   wxString strPath;
   wxString newFileName;
   while (item != -1)
   {
      item = lastListCtrlUsed->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
      if ( item != -1 )
      {
         fileNum++;
         itemName = lastListCtrlUsed->GetItemText(item);
         strPath = lastCCommanderUsed->getActualPath();
         newFileName = massiveName.Format(massiveName, fileNum);
         if (!renameDirFile(strPath, itemName, newFileName)) blnErr = true;
      }
   }
   if (blnErr) wxMessageBox(lang["There are some files read only"], "wxOpenCommander", wxOK | wxICON_ERROR , this);
   ListCtlUpdate();
}

void wxOpenCommanderFrm::Mnu_exit_onClick(wxCommandEvent& event)
{
   Close();
}

void wxOpenCommanderFrm::ListCtlUpdate()
{
   wxString lastPath;
   
   lastPath = cCommander1.getActualPath();  
   setListCtrl(&cCommander1, WxNotebook1, WxListCtrl1, lastPath);
   WxNotebook1->SetPageText(WxNotebook1->GetSelection(), getLastDir(lastPath));
   
   lastPath = cCommander2.getActualPath();
   setListCtrl(&cCommander2, WxNotebook2, WxListCtrl2, lastPath);
   WxNotebook2->SetPageText(WxNotebook2->GetSelection(), getLastDir(lastPath));
}

void wxOpenCommanderFrm::setListCtrl(cCommander* CCommander, wxNotebook* WxNotebook, wxListCtrl *WxListCtrl, wxString &directory)
{   
   CCommander->setActualPath(directory);
   WxListCtrl->SetItemCount(CCommander->getFileDirCount());
   WxListCtrl->Refresh();
   
   if (CCommander->getActualFilter() == "")
      WxListCtrl->SetTextColour(wxColour(0,0,0));
   else
      WxListCtrl->SetTextColour(wxColour(0,0,255));
   
   if (directory.Right(1) == "\\")
      combo->SetValue(directory);
   else
      combo->SetValue(directory + "\\");

    WxNotebook->SetPageText(WxNotebook->GetSelection(), getLastDir(directory));

    wxString numDirFiles;
    numDirFiles << CCommander->getFileDirCount() - 1;

    WxStatusBar->SetStatusText(numDirFiles + " " + lang["Directories and Files"]);
}

void wxOpenCommanderFrm::setListCtrlDevices(cCommander* CCommander, wxListCtrl *WxListCtrl)
{
   driversMap devices = wxGetDrives();
   
   CCommander->setDevices(true);
   WxListCtrl->SetItemCount(devices.size());
   WxListCtrl->Refresh();
}

void wxOpenCommanderFrm::OnListCtlKey(cCommander* CCommander, wxNotebook* WxNotebook, wxListCtrl *WxListCtrl, wxString &directory, wxListEvent& event)
{
   // User exec External Programs Keys
   hotKeyMap::iterator iter;
   for ( iter = keysMap.begin(); iter != keysMap.end(); iter++ )
   {
      long keyCode = iter->second.keyCode;
      if (keyCode >= WXK_F1 && keyCode <= WXK_F24 && (wxGetKeyState(WXK_ALT) || wxGetKeyState(WXK_CONTROL)))
         continue;
      else
      {
         if (keyCode == event.GetKeyCode())
         {
            wxString strCommand = iter->second.program;
            if (directory.Right(1) != "\\") directory += "\\";
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
          itemExec(CCommander, WxNotebook, WxListCtrl, directory, itemName);  
       }
       break;
       case WXK_RIGHT:
          WxListCtrl2->SetFocus();
          WxListCtrl2ItemFocused(event);
       break;
       case WXK_LEFT:
          WxListCtrl1->SetFocus();
          WxListCtrl1ItemFocused(event);
       break;
       case WXK_DELETE:
          Mnu_delete_onClick(event);
       break;
       case WXK_F1:
          Mnu_Help_onClick(event);
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
       case WXK_F8:
          Mnu_filter_onClick(event);
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
       /*
       default:
       {
          wxString key;
          key << event.GetKeyCode();
          wxMessageBox(key);
       }*/
    }
}

void wxOpenCommanderFrm::WxListCtrl1KeyDown(wxListEvent& event)
{
   wxString lastPath = cCommander1.getActualPath();
	OnListCtlKey(&cCommander1, WxNotebook1, WxListCtrl1, lastPath, event);
}

void wxOpenCommanderFrm::WxListCtrl2KeyDown(wxListEvent& event)
{
   wxString lastPath = cCommander2.getActualPath();
	OnListCtlKey(&cCommander2, WxNotebook2, WxListCtrl2, lastPath, event);
}

void wxOpenCommanderFrm::OnToolButton(wxCommandEvent& event)
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
        ListCtlUpdate();
        break;
      case ID_TOOL_ADD:
        comboClick(event, true);
        break;
      case ID_TOOL_REMOVE:
        {
         wxString strValue = combo->GetValue();
         combo->Delete(combo->GetCurrentSelection());
         combo->SetValue(strValue);
        }
        break;
      case ID_TOOL_DRIVES:
         setListCtrlDevices(lastCCommanderUsed, lastListCtrlUsed);
        break;
      case ID_TOOL_FILTER:
         Mnu_filter_onClick(event);
        break;
   }
}

void wxOpenCommanderFrm::OnComboClick(wxCommandEvent& event)
{
   comboClick(event, false);
}

void wxOpenCommanderFrm::comboClick(wxCommandEvent& event, bool add)
{
   wxString strCombo = combo->GetValue();
   if (strCombo.Right(1) == "\\")
      strCombo = strCombo.Left(strCombo.Length()-1);

   wxString strPath = lastCCommanderUsed->getActualPath();
   if (strCombo.IsEmpty()) return setListCtrl(lastCCommanderUsed, lastNoteBookUsed, lastListCtrlUsed, strPath);
   wxDir dir;
   if (!dir.Exists(strCombo)) return;
   lastCCommanderUsed->setActualPath(strCombo);
   setListCtrl(lastCCommanderUsed, lastNoteBookUsed, lastListCtrlUsed, strCombo);
   if (add) addDirsCombo(strCombo);
}

void wxOpenCommanderFrm::addDirsCombo(wxString& strCombo)
{
   if (strCombo.Right(1) != "\\")
      strCombo = strCombo + "\\";

   if (combo->FindString(strCombo) == wxNOT_FOUND )
      combo->Append(strCombo);
}

void wxOpenCommanderFrm::WxListCtrl1ItemFocused(wxListEvent& event)
{
   if (lastCCommanderUsed == &cCommander1 && lastNoteBookUsed == WxNotebook1 && lastListCtrlUsed == WxListCtrl1) return;
      
	lastListCtrlUsed = WxListCtrl1;
	lastNoteBookUsed = WxNotebook1;
	lastCCommanderUsed = &cCommander1;
	wxString strPath = cCommander1.getActualPath();
   if (strPath.Right(1) == "\\")
      combo->SetValue(strPath);
   else
      combo->SetValue(strPath + "\\");
   wxString numDirFiles;
   numDirFiles << lastListCtrlUsed->GetItemCount()-1;
   WxStatusBar->SetStatusText(numDirFiles + " " + lang["Directories and Files"]);

   WxListCtrl2->SetBackgroundColour(wxColour(240, 240, 240));
   WxListCtrl1->SetBackgroundColour(wxColour(255, 255, 255));
   WxListCtrl2->Refresh();
   WxListCtrl1->Refresh();
}

void wxOpenCommanderFrm::WxListCtrl2ItemFocused(wxListEvent& event)
{
   if (lastCCommanderUsed == &cCommander2 && lastNoteBookUsed == WxNotebook2 && lastListCtrlUsed == WxListCtrl2) return;
   
	lastListCtrlUsed = WxListCtrl2;
	lastNoteBookUsed = WxNotebook2;
	lastCCommanderUsed = &cCommander2;
   wxString strPath = cCommander2.getActualPath();
   if (strPath.Right(1) == "\\")
      combo->SetValue(strPath);
   else
      combo->SetValue(strPath + "\\");
   wxString numDirFiles;
   numDirFiles << lastListCtrlUsed->GetItemCount()-1;
   WxStatusBar->SetStatusText(numDirFiles + " " + lang["Directories and Files"]);
   
   WxListCtrl1->SetBackgroundColour(wxColour(240, 240, 240));
   WxListCtrl2->SetBackgroundColour(wxColour(255, 255, 255));
   WxListCtrl1->Refresh();
   WxListCtrl2->Refresh();
}

void wxOpenCommanderFrm::Mnu_execute_onClick(wxCommandEvent& event)
{
   wxStandardPaths wxPaths;
   wxString stdConfPath(wxPaths.GetUserDataDir());
   if (!wxDir::Exists(stdConfPath)) wxMkdir(stdConfPath);

   wxFileConfig config("wxOpenCommander", "wxOpenCommander", stdConfPath + "\\config.ini", stdConfPath + "\\config.ini", wxCONFIG_USE_LOCAL_FILE);

   wxString strLastExecCmd;
   config.Read("LastExecCommand", &strLastExecCmd);
   
	wxTextEntryDialog execWnd(this, lang["Write the command for execute in :"] + lastCCommanderUsed->getActualPath() + "\\", lang["Exec command"], strLastExecCmd, wxOK | wxCANCEL | wxCENTRE);
	execWnd.ShowModal();
	wxString command = execWnd.GetValue();
	if (command.IsEmpty()) return;
	wxString path =lastCCommanderUsed->getActualPath();
   
   wxString pathCommand = path + "\\" + command;  
   #ifdef __WXMSW__
   
      if (!wxFile::Exists(pathCommand)) pathCommand = command;

      STARTUPINFO sinfo;
      PROCESS_INFORMATION pinfo;
      memset(&sinfo,0,sizeof(STARTUPINFO));
      memset(&pinfo,0,sizeof(PROCESS_INFORMATION));
      sinfo.cb = sizeof(STARTUPINFO);

      CreateProcess(0, (char*)pathCommand.mb_str(), 0, 0, 0, 0, 0, (char*)path.mb_str(), &sinfo, &pinfo);

   #else
      wxExecute(pathCommand, wxEXEC_ASYNC);
   #endif

   config.Write("LastExecCommand", command);
}

void wxOpenCommanderFrm::Mnu_filter_onClick(wxCommandEvent& event)
{
   wxTextEntryDialog execWnd(this, lang["Enter the text for filter the files : Example (*.EXE)"], lang["Filter dialog"], lastCCommanderUsed->getActualFilter(), wxOK | wxCANCEL | wxCENTRE);
	if (execWnd.ShowModal() == wxID_CANCEL) return;
	wxString filter = execWnd.GetValue();
   filter = (filter == "*.*" || filter == "*" ? "" : filter); 
	lastCCommanderUsed->setActualFilter(filter);
	ListCtlUpdate();
}

void wxOpenCommanderFrm::Mnu_addTab_onClick(wxCommandEvent& event)
{
   lastCCommanderUsed->addPath(lastCCommanderUsed->getActualPath());
   lastCCommanderUsed->setActualPath(lastCCommanderUsed->getPathsCount()-1, false);
   wxString strPath = lastCCommanderUsed->getActualPath();
   setListCtrl(lastCCommanderUsed, lastNoteBookUsed, lastListCtrlUsed, strPath);
   lastNoteBookUsed->AddPage(lastListCtrlUsed, getLastDir(lastCCommanderUsed->getActualPath()));
   lastNoteBookUsed->ChangeSelection(lastNoteBookUsed->GetPageCount()-1);
   lastListCtrlUsed->SetFocus();
}

void wxOpenCommanderFrm::Mnu_removeTab_onClick(wxCommandEvent& event)
{
   if (lastNoteBookUsed->GetPageCount() > 1)
   {
      lastCCommanderUsed->removePath(lastNoteBookUsed->GetSelection());
      lastNoteBookUsed->RemovePage(lastNoteBookUsed->GetSelection());
      lastListCtrlUsed->SetFocus();
   }
}

void onThreadFinish(wxThread* thread, void* contextParam, void* parent)
{
   CTimer* finishTimer;
   wxOpenCommanderFrm* parentWindow;
   finishTimer = (CTimer*)contextParam;
   parentWindow = (wxOpenCommanderFrm*)parent;
   parentWindow->ListCtlUpdate();
   delete(finishTimer);
}

void wxOpenCommanderFrm::copyThread(wxString& strPathDest,  const wxArrayString& fileNames, const cCommander* pCCommanderDest = NULL)
{
   if (lastCCommanderUsed==pCCommanderDest) return; // If you move the file into the same List don't copy
   if (lastCCommanderUsed->getListDevices()) return; 

   size_t nFiles = fileNames.GetCount();
   if (nFiles < 1) return;
   
   vectorCopyParams copyParamsVector;
   
   for (size_t n = 0; n < nFiles; n++ )
   {
      wxString& actualFile(fileNames[n]);

      copyParams copyPaths;
      copyPaths.sourcePath = actualFile.BeforeLast(wxT('\\'));
      copyPaths.item = actualFile.AfterLast(wxT('\\'));
      copyPaths.newPath = strPathDest;
      copyParamsVector.push_back(copyPaths);
   }

   CopyDlg copyDlg(NULL, lang);
   copyDlg.setAutoInit(true);
   copyDlg.setAutoClose(true);
   copyDlg.showModal(copyParamsVector);
   //copyDlg.ShowModal();
   this->ListCtlUpdate();
   wxWakeUpIdle();
   
   
   /*
   size_t totalSize = 0;
   long numFiles = 0;
   CThread* thread = new CThread();
   thread->setParent((void*) this);
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
   thread->setFinishCallBackFunc(onThreadFinish, (void*)timer);
   thread->Create();
   thread->Run();
   */
}

void wxOpenCommanderFrm::copyThread()
{
   long item = 0;
   bool areItems=false;
   vectorCopyParams copyParamsVector;

   while (item != -1)
   {
      item = lastListCtrlUsed->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
      if ( item != -1 )
      {
         areItems = true;
         wxString itemName = lastListCtrlUsed->GetItemText(item);
         wxString filePath;
         wxString strPath1 = cCommander1.getActualPath();
         if (strPath1.Right(1) == "\\")
            strPath1 = strPath1.BeforeLast(wxT('\\'));
         wxString strPath2 = cCommander2.getActualPath();
         if (strPath2.Right(1) == "\\")
            strPath2 = strPath2.BeforeLast(wxT('\\'));

         copyParams copyPaths;
         if (lastListCtrlUsed == WxListCtrl1)
         {
            copyPaths.sourcePath = strPath1;
            copyPaths.item = itemName;
            copyPaths.newPath = strPath2;
            copyParamsVector.push_back(copyPaths);
         }
         else
         {
            copyPaths.sourcePath = strPath2;
            copyPaths.item = itemName;
            copyPaths.newPath = strPath1;
            copyParamsVector.push_back(copyPaths);
         }

      }
   }
   if (areItems)
   {
      CopyDlg copyDlg(this, lang);
      copyDlg.showModal(copyParamsVector);
      //copyDlg.ShowModal();
      this->ListCtlUpdate();
   }
   wxWakeUpIdle();
}

/*
 * MnuLanguage_onClick
 */
void wxOpenCommanderFrm::MnuLanguage_onClick(wxCommandEvent& event)
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

void wxOpenCommanderFrm::readConfig()
{
   wxString language;
   wxStandardPaths wxPaths;
   wxString stdConfPath(wxPaths.GetUserDataDir());

   wxFileConfig config("wxOpenCommander", "wxOpenCommander", stdConfPath + "\\config.ini", stdConfPath + "\\config.ini", wxCONFIG_USE_LOCAL_FILE);
 
   // restore frame position and size
   int x = config.Read(_T("/WindowPosition/X"), 10),
       y = config.Read(_T("/WindowPosition/Y"), 10),
       w = config.Read(_T("/WindowPosition/W"), 775),
       h = config.Read(_T("/WindowPosition/H"), 450);
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
     cCommander1.addPath(wxPaths.GetDocumentsDir());
     strPath = cCommander1.getActualPath();
     WxNotebook1->AddPage(WxListCtrl1, getLastDir(strPath), true);
	  WxNotebook1->AdvanceSelection(false);
   }
   while (blnExist)
   {
      config.Read("Tabs/Left/Dir" + nameNum, &strPath);
      cCommander1.addPath(strPath);
      WxNotebook1->AddPage(WxListCtrl1, getLastDir(strPath));
      WxNotebook1->AdvanceSelection(false);
      nameNum = "";
      j++;
      nameNum << j;
      blnExist = config.Exists("Tabs/Left/Dir" + nameNum);
   }
   cCommander1.setActualPath(WxNotebook1->GetSelection(), false);

   j = 0;
   nameNum = "0";
   strPath = "";   
   blnExist = config.Exists("Tabs/Right/Dir" + nameNum);
   if (!blnExist)
   {
     cCommander2.addPath(wxPaths.GetDocumentsDir());
     strPath = cCommander2.getActualPath();
	  WxNotebook2->AddPage(WxListCtrl2, getLastDir(strPath), true);
     WxNotebook2->AdvanceSelection(false);
   }
   while (blnExist)
   {
      config.Read("Tabs/Right/Dir" + nameNum, &strPath);
      cCommander2.addPath(strPath);
      WxNotebook2->AddPage(WxListCtrl2, getLastDir(strPath));
      WxNotebook2->AdvanceSelection(false);
      nameNum = "";
      j++;
      nameNum << j;
      blnExist = config.Exists("Tabs/Right/Dir" + nameNum);
   }
   cCommander2.setActualPath(WxNotebook2->GetSelection(), false);
   
   config.SetPath("");

   config.Read("Language", &language);

   wxString strCombo;
   wxString actualDir;
   for (int i=50; i > 0; i--)
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

void wxOpenCommanderFrm::writeConfig()
{
   wxStandardPaths wxPaths;
   wxString stdConfPath(wxPaths.GetUserDataDir());
   if (!wxDir::Exists(stdConfPath)) wxMkdir(stdConfPath);

   wxFileConfig config("wxOpenCommander", "wxOpenCommander", stdConfPath + "\\config.ini", stdConfPath + "\\config.ini", wxCONFIG_USE_LOCAL_FILE);
   
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
   config.DeleteGroup("Tabs");
   for (int j = 0; j <= cCommander1.getPathsCount()-1; j++)
   {
      actualDir << j;
      config.Write("Tabs/Left/Dir" + actualDir, cCommander1.getPath(j));
      actualDir = "";
   }
   for (int i = 0; i <= cCommander2.getPathsCount()-1; i++)
   {
      actualDir << i;
      config.Write("Tabs/Right/Dir" + actualDir, cCommander2.getPath(i));
      actualDir = "";
   }
      
   config.Write("Language", lang.getActualLang());
   
   config.DeleteGroup("DirCombo");
   for (int i=combo->GetCount()-1; i > 0; i--)
   {
      actualDir << i;
      config.Write("DirCombo/History" + actualDir, combo->GetString(i));
      actualDir = "";
   }
}

void wxOpenCommanderFrm::Mnu_hotKeys_onClick(wxCommandEvent& event)
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

void wxOpenCommanderFrm::WxListCtrlBeginLabelEdit(wxListEvent& event)
{
   wxListItem itemCtrl = event.GetItem();

   if (itemCtrl.GetId()==0)
       event.Veto();
}

void wxOpenCommanderFrm::WxListCtrlEndLabelEdit(wxListEvent& event)
{
   wxString newFileName = event.m_item.m_text.c_str();
   wxListItem itemCtrl = event.GetItem();
   wxString itemName = lastListCtrlUsed->GetItemText(itemCtrl.GetId());

   if (!event.IsEditCancelled())
   {
      wxString strPath = lastCCommanderUsed->getActualPath();
	   if (!renameDirFile(strPath, itemName, newFileName))
         event.Veto();
      else
      {
         lastCCommanderUsed->refreshFileDir();
         lastListCtrlUsed->Refresh();
      }
   }
}

void wxOpenCommanderFrm::WxNotebook1PageChanged(wxNotebookEvent& event)
{
   if (isReadingConfig) return; // The program init Now then we don't catch event because the program load all tabs...
   int actualTab = event.GetSelection();

   wxListEvent listEvent;
   WxListCtrl1ItemFocused(listEvent);
   
   cCommander1.setActualPath(actualTab);
   wxString strPath = cCommander1.getActualPath();
   lastListCtrlUsed->DeleteAllItems();
   setListCtrl(&cCommander1, WxNotebook1, WxListCtrl1, strPath);   
}

void wxOpenCommanderFrm::WxNotebook2PageChanged(wxNotebookEvent& event)
{
   if (isReadingConfig) return; // The program init Now then we don't catch event because the program load all tabs...
   int actualTab = event.GetSelection();
   
   wxListEvent listEvent;
   WxListCtrl2ItemFocused(listEvent);
      
   lastListCtrlUsed->DeleteAllItems();
   cCommander2.setActualPath(actualTab);
   wxString strPath = cCommander2.getActualPath();
   setListCtrl(&cCommander2, WxNotebook2, WxListCtrl2, strPath); 
}

void wxOpenCommanderFrm::WxListCtrlBeginDrag(wxListEvent& event)
{
	wxFileDataObject fileDataObject;
	long item = 0;
   while (item != -1)
   {
      item = lastListCtrlUsed->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
      if ( item != -1 )
      {
         wxString itemName = lastListCtrlUsed->GetItemText(item);
         wxString strPath = lastCCommanderUsed->getActualPath();
         if (strPath.Right(1) == "\\")
            strPath = strPath.BeforeLast(wxT('\\'));
         fileDataObject.AddFile(strPath + "\\" + itemName);
      }
   }
	wxDropSource dragSource( this );
	dragSource.SetData(fileDataObject);
	dragSource.DoDragDrop(TRUE);
}

void wxOpenCommanderFrm::copyToClipboard()
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
            wxString strPath = lastCCommanderUsed->getActualPath();
            if (strPath.Right(1) == "\\")
               strPath = strPath.BeforeLast(wxT('\\'));
            pFileDataObject->AddFile(strPath + "\\" + itemName);
         }
      }
      wxTheClipboard->SetData( pFileDataObject );
      wxTheClipboard->Close();
   }
}

void wxOpenCommanderFrm::pasteFromClipboard()
{
   wxTheClipboard->UsePrimarySelection();
   if (wxTheClipboard->Open())
   {
	   wxFileDataObject fileDataObject;
      if (wxTheClipboard->GetData(fileDataObject))
      {
         wxString strPath = lastCCommanderUsed->getActualPath();
         if (strPath.Right(1) == "\\")
            strPath = strPath.BeforeLast(wxT('\\'));
         copyThread(strPath, fileDataObject.GetFilenames());
      }
      wxTheClipboard->Close();
   }  
}

void wxOpenCommanderFrm::OnWxListCtrl1MouseLeftDown(wxMouseEvent& event)
{
   wxListEvent listEvent;
   WxListCtrl1ItemFocused(listEvent);
   event.Skip();
}

void wxOpenCommanderFrm::OnWxListCtrl2MouseLeftDown(wxMouseEvent& event)
{
   wxListEvent listEvent;
   WxListCtrl2ItemFocused(listEvent);
   event.Skip();
}

void wxOpenCommanderFrm::Mnu_Copy_onClick1(wxCommandEvent& event)
{
   copyToClipboard();
}

void wxOpenCommanderFrm::Mnu_paste_onClick(wxCommandEvent& event)
{
   pasteFromClipboard();
}

void wxOpenCommanderFrm::Mnu_checkUpdates_onClick(wxCommandEvent& event)
{
   wxString path = ".";
   wxString program = "webupdater.exe";
   
	Exec(path, program);
	Close();
}

void wxOpenCommanderFrm::Mnu_Help_onClick(wxCommandEvent& event)
{
   wxString path = ".\\help";
   wxString program = "index.chm";

	Exec(path, program);	
}

void wxOpenCommanderFrm::onContextMenu(wxContextMenuEvent& event)
{      
   wxWindow* menuWindow = wxFindWindowAtPoint(event.GetPosition());
   int winId = menuWindow->GetId();
   
   if (winId == ID_WXNOTEBOOK1 || winId == ID_WXLISTCTRL1)
   {
      wxListEvent event;
      WxListCtrl1ItemFocused(event);
   }
   
   if (winId == ID_WXNOTEBOOK2 || winId == ID_WXLISTCTRL2 )
   {
      wxListEvent event;
      WxListCtrl2ItemFocused(event);
   }
   
   if (menuWindow->GetId() == ID_WXNOTEBOOK1 || menuWindow->GetId() == ID_WXNOTEBOOK2)
   {
      wxMenu* menu = new wxMenu;

	   menu->Append(ID_MNU_ADDTAB_1080, wxT(lang["&Add Tab"] + " (Ctrl + T)"), wxT(""), wxITEM_NORMAL);
	   menu->Append(ID_MNU_REMOVETAB_1082, wxT(lang["Re&move Tab"] + " (Ctrl + F4)"), wxT(""), wxITEM_NORMAL);
      
      this->PopupMenu(menu);
      return;
   } 		

   wxMenu* menu = new wxMenu;
   
	menu->Append(ID_MNU_COPY_1084, wxT(lang["&Copy"] + " (Ctrl + C)"), wxT(""), wxITEM_NORMAL);
	menu->Append(ID_MNU_PASTE_1085, wxT(lang["&Paste"] + " (Ctrl + V)"), wxT(""), wxITEM_NORMAL);

	menu->AppendSeparator();   
   
   wxMenuItem itemNewFolfer(menu,ID_MNU_NEWFOLDER_1049, wxT(lang["&New folder"]), wxT(""), wxITEM_NORMAL);
   wxBitmap imgNewFolder(newFolder_xpm);
   itemNewFolfer.SetBitmap(imgNewFolder);
   menu->Append(&itemNewFolfer);
   
   wxMenuItem itemCopy(menu,ID_MNU_COPY_1046, wxT(lang["Copy to..."]), wxT(""), wxITEM_NORMAL);
   wxBitmap imgCopy(copy_xpm);
   itemCopy.SetBitmap(imgCopy);
   menu->Append(&itemCopy);

   wxMenuItem itemDel(menu,ID_MNU_DELETE_1047, wxT(lang["&Delete"]), wxT(""), wxITEM_NORMAL);
   wxBitmap imgDel(delete_xpm);
   itemDel.SetBitmap(imgDel);
   menu->Append(&itemDel);

   wxMenuItem itemRen(menu,ID_MNU_RENAME_1048, wxT(lang["&Rename"]), wxT(""), wxITEM_NORMAL);
   wxBitmap imgRen(edit_xpm);
   itemRen.SetBitmap(imgRen);
   menu->Append(&itemRen);

   wxMenuItem itemFil(menu,ID_MNU_FILTER_1088, wxT(lang["&Filter"]), wxT(""), wxITEM_NORMAL);
   wxBitmap imgFil(filter_xpm);
   itemFil.SetBitmap(imgFil);
   menu->Append(&itemFil);
	
   this->PopupMenu(menu);   

}

