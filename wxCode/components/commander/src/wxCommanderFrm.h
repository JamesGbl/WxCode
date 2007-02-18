//---------------------------------------------------------------------------
//
// Name:        wxCommanderFrm.h
// Author:      Armando Urdiales Gonz�lez
// Created:     06/12/2006 12:53:20
// Description: wxCommanderFrm class declaration
//
//---------------------------------------------------------------------------

#ifndef __WXCOMMANDERFRM_h__
#define __WXCOMMANDERFRM_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/frame.h>
#else
	#include <wx/wxprec.h>
#endif

#define wxALIGN_CENTER wxEXPAND

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/splitter.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/toolbar.h>
////Header Include End

#include "wxCommanderUtils.h"
#include <wx/stdpaths.h>
#include <wx/dir.h>
#include <wx/filefn.h>
//#include <wx/file.h>
#include <wx/utils.h>
#include <wx/mimetype.h>
#include <wx/stattext.h>
#include <wx/textdlg.h>
#if wxUSE_ABOUTDLG
    #include <wx/aboutdlg.h>
#endif // wxUSE_ABOUTDLG
#include <wx/imaglist.h>
#include <wx/image.h>
#include <wx/bitmap.h>
#include <wx/settings.h>
#include <wx/datetime.h>

#include <wx/listctrl.h>

#include <wx/clipbrd.h>

#include <wx/choicdlg.h>

// Read/Write Config
#include <wx/config.h>
#include <wx/confbase.h>
#include <wx/fileconf.h>
#include <wx/msw/regconf.h>
//

#include "hotKeysDlg.h"

#include "taskbar.h"
#include "multiLang.h"
#include "CTimer.h"
#include "CThread.h"

#include <vector>

typedef vector<wxString> vectorString;

#include "Images/icon1.xpm"
//#include "Images/icon2.xpm"
#include "Images/icon3.xpm"
//#include "Images/icon4.xpm"
#include "Images/icon5.xpm"
#include "Images/hardDisk.xpm"
#include "Images/dvd.xpm"
#include "Images/floppy.xpm"

////Dialog Style Start
#undef wxCommanderFrm_STYLE
#define wxCommanderFrm_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

using namespace std;

class wxCommanderFrm : public wxFrame
{
	private:
		DECLARE_EVENT_TABLE();

	public:
		wxCommanderFrm(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("wxCommander"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCommanderFrm_STYLE);
		virtual ~wxCommanderFrm();
      void Mnu_about_onClick(wxCommandEvent& event);
      void Mnu_exit_onClick(wxCommandEvent& event);
		void WxListCtrl1KeyDown(wxListEvent& event);
		void WxListCtrl2KeyDown(wxListEvent& event);
		void WxListCtrl1ItemFocused(wxListEvent& event);
		void WxListCtrl2ItemFocused(wxListEvent& event);
      void Mnu_execute_onClick(wxCommandEvent& event);
	   void MnuLanguage_onClick(wxCommandEvent& event);
	   void Mnu_hotKeys_onClick(wxCommandEvent& event);
		void WxListCtrl1EndLabelEdit(wxListEvent& event);
		void WxListCtrl1BeginLabelEdit(wxListEvent& event);
		void WxListCtrl2BeginLabelEdit(wxListEvent& event);
		void WxListCtrl2EndLabelEdit(wxListEvent& event);
		void WxNotebook1PageChanging(wxNotebookEvent& event);
		void WxNotebook2PageChanging(wxNotebookEvent& event);
		void WxNotebook1PageChanged(wxNotebookEvent& event);
		void WxNotebook2PageChanged(wxNotebookEvent& event);
	void Mnu_addTab_onClick(wxCommandEvent& event);
	void Mnu_removeTab_onClick(wxCommandEvent& event);
	private:
		//Do not add custom control declarations
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxMenuBar *WxMenuBar1;
		wxNotebook *WxNotebook2;
		wxNotebook *WxNotebook1;
		wxSplitterWindow *WxSplitterWindow1;
		wxGridSizer *WxGridSizer;
		wxStatusBar *WxStatusBar;
		wxToolBar *WxToolBar;
		////GUI Control Declaration End
		wxListCtrl* WxListCtrl1;
		wxListCtrl* WxListCtrl2;
		wxListCtrl* lastListCtrlUsed;
		wxNotebook* lastNoteBookUsed;
		vectorString* lastVectorStringUsed;
		wxString* strPathLstCtrl;
		wxComboBox* combo;
		wxString strPathLstCtrl1;
		wxString strPathLstCtrl2;
		wxImageList* imageList;
		CTimer* timer;
      CThread* thread;
	   wxCommanderTaskBar* tray;
	   multiLang lang;
	   hotKeyMap keysMap;
	   vectorString aPaths1;
	   vectorString aPaths2;
	   
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_MNU_FILES_1004 = 1004,
			ID_MNU_ADDTAB_1080 = 1080,
			ID_MNU_REMOVETAB_1082 = 1082,
			ID_MNU_NEWFOLDER_1049 = 1049,
			ID_MNU_COPY_1046 = 1046,
			ID_MNU_DELETE_1047 = 1047,
			ID_MNU_RENAME_1048 = 1048,
			ID_MNU_EXECUTE_1051 = 1051,
			ID_MNU_EXIT_1005 = 1005,
			ID_MNU_EDIT_1083 = 1083,
			ID_MNU_COPY_1084 = 1084,
			ID_MNU_PASTE_1085 = 1085,
			ID_MNU_OPTIONS_1063 = 1063,
			ID_MNU_LANGUAGE_1064 = 1064,
			ID_MNU_HOTKEYS_1065 = 1065,
			ID_MNU_HELP_1006 = 1006,
			ID_MNU_ABOUT_1007 = 1007,
			
			ID_WXNOTEBOOK2 = 1079,
			ID_WXNOTEBOOK1 = 1077,
			ID_WXSPLITTERWINDOW1 = 1076,
			ID_WXSTATUSBAR = 1044,
			ID_WXTOOLBAR = 1043,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_, //don't remove this value unless you have other enum values
			ID_WXLISTCTRL1 = 9998,
			ID_WXLISTCTRL2 = 9999,			
			ID_TOOL_COPY = 10000,
			ID_TOOL_DELETE = 10001,
			ID_TOOL_RENAME = 10002,
			ID_TOOL_NEWFOLDER = 10003,
			ID_TOOL_EXEC = 10004,
			ID_TOOL_GO = 10005,
			ID_TOOL_ADD = 10006,
			ID_TOOL_REMOVE = 10007,
         ID_TOOL_COMBO = 10100
		};

	private:
         void Mnu_copy_onClick(wxCommandEvent& event);
         void Mnu_delete_onClick(wxCommandEvent& event);
         void Mnu_rename_onClick(wxCommandEvent& event);
         void Mnu_newFolder_onClick(wxCommandEvent& event);
         void OnList1ItemActivated(wxListEvent& event);
         void OnList2ItemActivated(wxListEvent& event);
         void OnTaskBarClick(wxTaskBarIconEvent& event);
         void OnListCtlKey(wxListCtrl *WxListCtrl, wxString &directory, wxListEvent& event);
         void OnToolButton(wxCommandEvent& event);
         void OnComboClick(wxCommandEvent& event);
         void OnClose(wxCloseEvent& event);
         void OnWxListCtrl1MouseLeftDown(wxMouseEvent& event);
         void OnWxListCtrl2MouseLeftDown(wxMouseEvent& event);         
         void comboClick(wxCommandEvent& event, bool add = false);
  	      void CreateGUIControls();
	      void addColumns(wxListCtrl *WxListCtrl);
	      void addDirsCombo(wxString& strCombo);
	      void setListCtrl(wxListCtrl *WxListCtrl, wxString &directory);
	      void setListCtrl(wxListCtrl *WxListCtrl, driversMap &drives);
	      void itemExec(wxListCtrl *WxListCtrl, wxString &directory, wxString &itemName);
	      void updateControlsLanguage();
	      void copyThread();
	      void readConfig();
	      void writeConfig();
	      void Close();
         void WxListCtrlBeginLabelEdit(wxListEvent& event);
         void WxListCtrlEndLabelEdit(wxListEvent& event);
         void WxListCtrlBeginDrag(wxListEvent& event);
         void copyToClipboard();
         void pasteFromClipboard();
   public:
         void ListCtlUpdate();
         void copyThread(wxString& strPathDest,  const wxArrayString& fileNames);
	void Mnu_Copy_onClick1(wxCommandEvent& event);
	void Mnu_paste_onClick(wxCommandEvent& event);
};

class DragAndDropFile : public wxFileDropTarget
{
  public:
    DragAndDropFile(wxCommanderFrm& theWindow, wxString& strPathDest) : wxFileDropTarget(), m_strPathDest(strPathDest), m_wxFrame(theWindow) {}
    virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& fileNames);
  private:
   wxString& m_strPathDest;
   wxCommanderFrm& m_wxFrame;
};

#endif
