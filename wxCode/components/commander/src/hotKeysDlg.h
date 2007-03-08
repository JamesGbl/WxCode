//---------------------------------------------------------------------------
//
// Name:        hotKeysDlg.h
// Author:      Armando Urdiales González
// Created:     21/01/2007 17:26:36
// Description: hotKeysDlg class declaration
//
//---------------------------------------------------------------------------

#ifndef __HOTKEYSDLG_h__
#define __HOTKEYSDLG_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/dialog.h>
#else
	#include <wx/wxprec.h>
#endif

//Do not add custom headers between
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/listctrl.h>
////Header Include End

#include <wx/filedlg.h>
#include "wxOpenCommanderUtils.h"
#include "multiLang.h"

#include <map>

////Dialog Style Start
#undef hotKeysDlg_STYLE
#define hotKeysDlg_STYLE wxCAPTION | wxDIALOG_NO_PARENT | wxCLOSE_BOX
////Dialog Style End

#define READ_MODE 0
#define ADD_MODE 1
#define EDIT_MODE 2

struct structHotKeys
{
   wxString program;
   long keyCode;
};

typedef map <int, structHotKeys, less<int> > hotKeyMap;

class EventTextCtrl : public wxTextCtrl
{
public:
    EventTextCtrl(wxWindow *parent, wxWindowID id, const wxString &value,
               const wxPoint &pos, const wxSize &size, int style = 0)
        : wxTextCtrl(parent, id, value, pos, size, style)
    {
    }

    void OnChar(wxKeyEvent& event);
    long actualKeyCode;
private:
    DECLARE_EVENT_TABLE()
};

class hotKeysDlg : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();

	public:
		hotKeysDlg(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Hot Keys Dialog"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = hotKeysDlg_STYLE);
		virtual ~hotKeysDlg();
		void CancelBtnClick(wxCommandEvent& event);
		void addBtnClick(wxCommandEvent& event);
		void openFileDlgClick(wxCommandEvent& event);
		void editCancelBtnClick(wxCommandEvent& event);
		void saveBtnClick(wxCommandEvent& event);
		void editBtnClick(wxCommandEvent& event);
		void WxListCtrlItemActivated(wxListEvent& event);
		void delBtnClick(wxCommandEvent& event);

	private:
		//Do not add custom control declarations between
                //GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxButton *okBtn;
		wxStaticText *lblHotKey;
		wxButton *saveBtn;
		wxButton *editCancelBtn;
		wxButton *openFileBtn;
		wxTextCtrl *txtProgram;
		wxStaticText *lblProgram;
		wxButton *delBtn;
		wxButton *editBtn;
		wxButton *addBtn;
		wxStaticBox *WxStaticBox1;
		wxButton *CancelBtn;
		wxListCtrl *WxListCtrl;
		////GUI Control Declaration End

	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_OKBTN = 1023,
			ID_LBLHOTKEY = 1022,
			ID_SAVEBTN = 1021,
			ID_EDITCANCELBTN = 1020,
			ID_OPENFILEBTN = 1018,
			ID_TXTPROGRAM = 1017,
			ID_LBLPROGRAM = 1016,
			ID_DELBTN = 1015,
			ID_EDITBTN = 1014,
			ID_ADDBTN = 1013,
			ID_WXSTATICBOX1 = 1009,
			ID_CANCELBTN = 1003,
			ID_WXLISTCTRL = 1001,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_, //don't remove this value unless you have other enum values
			ID_WXEDIT = 2001
		};
	private:
      int mode;
      EventTextCtrl *txtHotKey;
      hotKeyMap m_keysMap;
      multiLang lang;
	private:
      void OkBtnClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
		void setState();
		void OnChar(wxKeyEvent& event);
		void WxListCtrlSelected(wxListEvent& event);
		void refreshListCtrl();
	public:
      hotKeyMap getKeysMap();
      void setKeysMap(hotKeyMap keysMap);
      void setLanguage(multiLang parent_lang);
};

#endif
