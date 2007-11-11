//---------------------------------------------------------------------------
//
// Name:        CopyDelDlg.cpp
// Author:      Armando Urdiales González
// Created:     25/03/2007 12:23:12
// Description: CopyDelDlg class implementation
//
//---------------------------------------------------------------------------

#include "CopyDlg.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
#include "Images/Self_CopyDlg_XPM.xpm"
////Header Include End

//----------------------------------------------------------------------------
// CopyDlg
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(CopyDlg,wxDialog)
	////Manual Code Start
	EVT_LIST_ITEM_SELECTED(ID_WXLISTCTRL1, CopyDlg::WxListCtrl1ItemSelected)
	EVT_LIST_ITEM_SELECTED(ID_WXLISTCTRL2, CopyDlg::WxListCtrl2ItemSelected)
	////Manual Code End
	
	EVT_CLOSE(CopyDlg::OnClose)
	EVT_BUTTON(ID_BTNCANCEL,CopyDlg::btnCancelClick)
	EVT_BUTTON(ID_BTNCOPY,CopyDlg::btnCopyClick)
END_EVENT_TABLE()
////Event Table End

CopyDlg::CopyDlg(wxWindow *parent, multiLang& language, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style) , lang(language)
{
	CreateGUIControls();
	
	thread = NULL;
}

CopyDlg::~CopyDlg()
{
   //delete(WxListCtrl1->GetImageList(wxIMAGE_LIST_SMALL)); // MEMORY LEAK
   //delete(WxListCtrl1->GetImageList(wxIMAGE_LIST_SMALL));// MEMORY LEAK
   
   // Memory LEAK -- Prox Ver...
   //destroyThread();
   
   delete(ckExistFiles);
   delete(ckReadFiles);
   delete(lblDetails);
   delete(lblDetails2);
   delete(lblDetails3);
   delete(lblDetails4);
   delete(btnCancel);
   delete(btnCopy);
   delete(lblDestination);
   delete(WxGauge1);
   delete(lblSource);
   delete(WxListCtrl1);
   delete(WxListCtrl2);   
} 

void CopyDlg::CreateGUIControls()
{
	//Do not add custom code between
        //GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	SetTitle(wxT("Copy Files (wxOpenCommander)"));
	SetIcon(Self_CopyDlg_XPM);
	SetSize(8,8,750,525);
	Center();
	

	lblDetails4 = new wxStaticText(this, ID_LBLDETAILS4, wxT(""), wxPoint(543,215), wxDefaultSize, 0, wxT("lblDetails4"));

	lblDetails3 = new wxStaticText(this, ID_LBLDETAILS3, wxT(""), wxPoint(359,214), wxDefaultSize, 0, wxT("lblDetails3"));

	lblDetails2 = new wxStaticText(this, ID_LBLDETAILS2, wxT(""), wxPoint(6,216), wxDefaultSize, 0, wxT("lblDetails2"));

	ckExistFiles = new wxCheckBox(this, ID_CKEXISTFILES, wxT("Overwrite existing files"), wxPoint(11,464), wxSize(223,17), 0, wxDefaultValidator, wxT("ckExistFiles"));

	ckReadFiles = new wxCheckBox(this, ID_CKREADFILES, wxT("Overwrite Read only files"), wxPoint(275,464), wxSize(248,17), 0, wxDefaultValidator, wxT("ckReadFiles"));

	lblDetails = new wxStaticText(this, ID_LBLDETAILS, wxT(""), wxPoint(7,199), wxDefaultSize, 0, wxT("lblDetails"));

	btnCancel = new wxButton(this, ID_BTNCANCEL, wxT("&Cancel"), wxPoint(634,458), wxSize(98,28), 0, wxDefaultValidator, wxT("btnCancel"));

	btnCopy = new wxButton(this, ID_BTNCOPY, wxT("Copy"), wxPoint(532,458), wxSize(98,28), 0, wxDefaultValidator, wxT("btnCopy"));

	lblDestination = new wxStaticText(this, ID_LBLDESTINATION, wxT("Destination"), wxPoint(9,261), wxDefaultSize, wxALIGN_CENTRE, wxT("lblDestination"));
	lblDestination->SetFont(wxFont(11, wxSWISS, wxNORMAL,wxNORMAL, FALSE));

	WxGauge1 = new wxGauge(this, ID_WXGAUGE1, 100, wxPoint(9,236), wxSize(721,21), wxGA_HORIZONTAL, wxDefaultValidator, wxT("WxGauge1"));
	WxGauge1->SetRange(100);
	WxGauge1->SetValue(0);

	lblSource = new wxStaticText(this, ID_LBLSOURCE, wxT("Source"), wxPoint(9,3), wxDefaultSize, wxALIGN_CENTRE, wxT("lblSource"));
	lblSource->SetFont(wxFont(11, wxSWISS, wxNORMAL,wxNORMAL, FALSE));
	////GUI Items Creation End
	
	SetTitle(lang["Copy Files"] + " (wxOpenCommander)");
	ckReadFiles->SetLabel(lang["Overwrite Read only files"]);
	ckExistFiles->SetLabel(lang["Overwrite existing files"]);
   lblDestination->SetLabel(lang["Destination"]);
   lblSource->SetLabel(lang["Source"] + ":");
	btnCancel->SetLabel(lang["Cancel"]);
	btnCopy->SetLabel(lang["Copy"]);
	
	WxListCtrl1 = new wxOpenCommanderListCtrl(this, ID_WXLISTCTRL1, wxPoint(7,25), wxSize(724,168), wxLC_REPORT | wxLC_VIRTUAL, &cCommander1, lang);
   WxListCtrl2 = new wxListCtrl(this, ID_WXLISTCTRL2, wxPoint(8,282), wxSize(724,170), wxLC_REPORT);

	autoInit = false;
	autoClose = false;
	blnCopping = false;
	blnCanceled = false;
	totalSize=0;
	actualSize=0;
	numTotaFiles=0;
	numActualFile=0;

   addColumns(WxListCtrl1);
   addColumns(WxListCtrl2);
}

void CopyDlg::addColumns(wxListCtrl* WxListCtrl)
{
   while (WxListCtrl->GetColumnCount()>0) WxListCtrl->DeleteColumn(0);

   WxListCtrl->InsertColumn(0, lang["Files"], wxLIST_FORMAT_LEFT, 700);
   
   int size = 20;
   wxImageList* imageList; //= WxListCtrl->GetImageList(wxIMAGE_LIST_SMALL);
   //if (imageList != NULL) delete(imageList); // MEMORY LEAK
   imageList = new wxImageList(size, size, true); // MEMORY LEAK

   imageList->Add(wxArtProvider::GetIcon(wxART_NORMAL_FILE, wxART_CMN_DIALOG, wxSize(size, size)));
   imageList->Add(wxArtProvider::GetIcon(wxART_FOLDER, wxART_CMN_DIALOG, wxSize(size, size)));
   imageList->Add(wxArtProvider::GetIcon(wxART_EXECUTABLE_FILE , wxART_CMN_DIALOG, wxSize(size, size)));

   WxListCtrl->AssignImageList(imageList, wxIMAGE_LIST_SMALL);
}

void CopyDlg::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
}

int CopyDlg::onBeginCopyFile(const wxString& sourcePath, const wxString& destinationPath)
{  
   bool blnExistFile = wxFile::Exists(destinationPath);
   if (blnExistFile)
   {
      if (!ckExistFiles->GetValue()) 
      {
         errors[sourcePath] = lang["File already exist in destination"];
         return ABORT_COPY_FILE;      
      }
      if (ckReadFiles->GetValue())
      {
         SetFileAttributes(destinationPath.c_str(), FILE_ATTRIBUTE_NORMAL); // Make it not read only, so we can overwrite it
      }
      else
      {
        DWORD dwFileAttributes = GetFileAttributes(destinationPath.c_str());        
        if (dwFileAttributes & FILE_ATTRIBUTE_READONLY) 
        {
           errors[sourcePath] = lang["File is Read only in destination"];
           return ABORT_COPY_FILE;
        }
      }
   }  
   
   wxFileName file(sourcePath);

   long long fileSize = getFileSize(sourcePath);

   // Avance the gauge the haft size of the actual file. 
   long long estimatedSize = actualSize + (fileSize/2);
   int percent;
   if (totalSize) percent = (estimatedSize * 100) / totalSize;

   WxGauge1->SetValue(percent);
   lblDetails->SetLabel(lang["Path"] + ": " + file.GetPath());
   lblDetails2->SetLabel(lang["File"] + ": " + file.GetFullName());
   lblDetails3->SetLabel(lang["Size"] + ": " + formatFileSize(actualSize) + " / " + formatFileSize(totalSize));
   lblDetails4->SetLabel(lang["File"] + ": " + LongLongTowxString(numActualFile+1) + " / " + LongLongTowxString(numTotaFiles));
   
   return (blnCanceled? ABORT_COPY_PROCESS : COPY_FILE_OK);
}

void CopyDlg::onEndCopyFile(bool copy, const wxString& sourcePath, const wxString& destinationPath)
{
   if (!copy) return;
   
   //wxString strDir = sourcePath.BeforeLast(wxT('\\'));
   //cCommander1.removeFileDir(strDir);
   cCommander1.removeFileDir(sourcePath);
   WxListCtrl1->SetItemCount(cCommander1.getFileDirCount());
   WxListCtrl1->Refresh();
 
   int item = WxListCtrl2->GetItemCount();
   WxListCtrl2->InsertItem(item, destinationPath,0);
   // Update the wxGauge.
   long long fileSize = getFileSize(destinationPath);
   numActualFile++;
   actualSize = actualSize + fileSize;
   int percent = (actualSize * 100) / totalSize;
   WxGauge1->SetValue(percent);
   
   wxWakeUpIdle();
}

void CopyDlg::onCopyThreadFinish()
{
   int item = WxListCtrl1->GetItemCount();
   
   if (!item)
      lblDetails2->SetLabel("");
   else
   {
      lblDetails2->SetLabel(lang["Copy finished with some errors"]);
      wxBell();
      wxMessageBox(lang["Copy finished with some errors"], lang["Copy finished with some errors"], wxICON_ERROR | wxCENTRE);
   }
   lblDetails->SetLabel("");
   //lblDetails2->SetLabel("");

   if (actualSize == totalSize) WxGauge1->SetValue(100);

   btnCopy->Enable(!blnCanceled && item != 0);
   blnCopping = false;
   btnCancel->SetLabel(lang["Close"]);
   
   #ifdef __WXMSW__
      if (autoClose && !item) SendMessage((HWND)GetHWND(), WM_CLOSE, (WPARAM)TRUE, (LPARAM)NULL);
   #endif
}

void onThreadDirRecursiveFinish(void* thread, void* contextParam, void* parent)
{
   CopyDlg* parentWindow;
   parentWindow = (CopyDlg*)parent;
   CThread* cThread = (CThread*)thread;
   long long totalSize = cThread->totalSizeRecursive;
   return parentWindow->onDirRecursiveFinish(totalSize);
}

void onThreadCopyFinish(void* thread, void* contextParam, void* parent)
{
   CopyDlg* parentWindow;
   parentWindow = (CopyDlg*)parent;
   parentWindow->onCopyThreadFinish();   
   return;
}

int onThreadBeginCopyFile(void* parent, const wxString& sourcePath, const wxString& destinationPath)
{
   CopyDlg* parentWindow;
   parentWindow = (CopyDlg*)parent;
   return parentWindow->onBeginCopyFile(sourcePath, destinationPath);
}

void onThreadEndCopyFile(void* parent, bool copy, const wxString& sourcePath, const wxString& destinationPath)
{
   CopyDlg* parentWindow;
   parentWindow = (CopyDlg*)parent;
   return parentWindow->onEndCopyFile(copy, sourcePath, destinationPath);
}

/*
void CopyDlg::destroyThread()
{
   if (thread != NULL)
   {
      thread->TestDestroy();
      thread->Wait();
      thread = NULL;
   }
}
*/

void CopyDlg::btnCopyClick(wxCommandEvent& event)
{   
	size_t nFiles = m_pathsCopy.size();
   if (nFiles < 0) return;
   
   //destroyThread();
   thread = new CThread();
   thread->setParent((void*) this);

   thread->setVectorCopyParams(m_pathsCopy);
   thread->setBeginCopyFileCallBackFunc(onThreadBeginCopyFile);
   thread->setEndCopyFileCallBackFunc(onThreadEndCopyFile);
   thread->setFinishCallBackFunc(onThreadCopyFinish, NULL);
   thread->Create();
   thread->Run();
   btnCopy->Enable(false);
   blnCopping = true;
}

void CopyDlg::btnCancelClick(wxCommandEvent& event)
{
   if (!blnCopping)
      Destroy();
   else   
      blnCanceled = true;   
}

void CopyDlg::setAutoInit(bool init)
{
   autoInit = init;
}

void CopyDlg::setAutoClose(bool close)
{
   autoClose = close;
}

void CopyDlg::setTotalSize(long long size)
{
   totalSize = size;
}

void CopyDlg::showModal(vectorCopyParams pathsCopy)
{
   Show();
   SetTitle(lang["Loading files (Please wait...)"]);

   m_pathsCopy = pathsCopy;
   updateSourceListCtrl();
   ShowModal();
}

void CopyDlg::updateSourceListCtrl()
{
   wxArrayString aFilesPath;
   
   for (size_t i = 0 ; i < m_pathsCopy.size(); i++)
   {
      wxString filePath = m_pathsCopy[i].sourcePath;
     // if (filePath.Right(1) == "\\")
     //   filePath = m_pathsCopy[i].sourcePath + m_pathsCopy[i].item;
     // else
        filePath = m_pathsCopy[i].sourcePath + "\\" + m_pathsCopy[i].item;

      aFilesPath.Add(filePath);
   }
   
   //Calculating number and size of files
   //CThread* thread = new CThread();
   //destroyThread();
   thread = new CThread();
   thread->setParent((void*) this);
   
   thread->addPathsRecursive(&cCommander1, aFilesPath);
   thread->setFinishCallBackFunc(onThreadDirRecursiveFinish, NULL);
   thread->Create();
   thread->Run();

   //wxBeginBusyCursor();

   btnCopy->Enable(false);
   btnCancel->Enable(false);
}

void CopyDlg::onDirRecursiveFinish(long long totalSizeRecursive)
{
   SetTitle(lang["Copy Files"] + " (wxOpenCommander)");
   totalSize = totalSizeRecursive;
   numTotaFiles = cCommander1.getFileDirCount();
   WxListCtrl1->showPathAndFile = true;
   WxListCtrl1->SetItemCount(numTotaFiles);

   if (m_pathsCopy.size())
      lblDestination->SetLabel(lang["Destination"] + ": " + m_pathsCopy[0].newPath + "\\");
   lblDetails->SetLabel("");
   lblDetails2->SetLabel("");
   lblDetails3->SetLabel(lang["Size"] + ": 0 / " + formatFileSize(totalSize));
   lblDetails4->SetLabel(lang["File"] + ": 0 / " + LongLongTowxString(numTotaFiles));  
   
   //wxEndBusyCursor();
   btnCopy->Enable(true);
   btnCancel->Enable(true);

   if (autoInit)
   {
      wxCommandEvent event;
      btnCopyClick(event);
   } 
}

void CopyDlg::WxListCtrl1ItemSelected(wxListEvent& event)
{
   if (!blnCopping)
   lblDetails->SetLabel(errors[event.GetText()]);
}

void CopyDlg::WxListCtrl2ItemSelected(wxListEvent& event)
{
   if (!blnCopping)
   lblDetails->SetLabel(lang["File copied sucessfuly"]);
}
