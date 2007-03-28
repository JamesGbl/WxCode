//---------------------------------------------------------------------------
//
// Name:        CopyDlg.h
// Author:      Armando Urdiales González
// Created:     25/03/2007 12:23:12
// Description: CopyDlg class declaration
//
//---------------------------------------------------------------------------

#ifndef __COPYDLG_h__
#define __COPYDLG_h__

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
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/gauge.h>
#include <wx/stattext.h>
#include <wx/listctrl.h>
////Header Include End

#include "CThread.h"
#include "wxOpenCommanderFrm.h"
#include "wxOpenCommanderUtils.h"

////Dialog Style Start
#undef CopyDlg_STYLE
#define CopyDlg_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class CopyDlg : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
		void btnCancelClick(wxCommandEvent& event);
		void btnCopyClick(wxCommandEvent& event);
	public:
		CopyDlg(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("wxOpenCommander"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = CopyDlg_STYLE);
		virtual ~CopyDlg();
	
	private:
		//Do not add custom control declarations between 
                //GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxStaticText *lblDetails4;
		wxStaticText *lblDetails3;
		wxStaticText *lblDetails2;
		wxCheckBox *ckExistFiles;
		wxCheckBox *ckReadFiles;
		wxStaticText *lblDetails;
		wxButton *btnCancel;
		wxButton *btnCopy;
		wxStaticText *lblDestination;
		wxListCtrl *WxListCtrl2;
		wxGauge *WxGauge1;
		wxStaticText *lblSource;
		wxListCtrl *WxListCtrl1;
		////GUI Control Declaration End
		vectorCopyParams m_pathsCopy;
		long long totalSize;
		long long actualSize;
		long numTotaFiles;
		long numActualFile;
		bool blnCanceled;
		bool blnCopping;
		bool autoInit;
		bool autoClose;
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_LBLDETAILS4 = 1014,
			ID_LBLDETAILS3 = 1013,
			ID_LBLDETAILS2 = 1012,
			ID_CKEXISTFILES = 1011,
			ID_CKREADFILES = 1010,
			ID_LBLDETAILS = 1009,
			ID_BTNCANCEL = 1008,
			ID_BTNCOPY = 1007,
			ID_LBLDESTINATION = 1006,
			ID_WXLISTCTRL2 = 1005,
			ID_WXGAUGE1 = 1003,
			ID_LBLSOURCE = 1002,
			ID_WXLISTCTRL1 = 1001,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
	
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
		void addFilesInSourceList(const wxString& filePath);
      void updateSourceListCtrl();

	public:
      void setPathsToCopy(vectorCopyParams pathsCopy);
      bool onBeginCopyFile(const wxString& sourcePath, const wxString& destinationPath);
      void onEndCopyFile(bool copy, const wxString& sourcePath, const wxString& destinationPath);
      void onCopyThreadFinish();
      void setAutoInit(bool init);
      void setAutoClose(bool close);

};

void onThreadCopyFinish(wxThread* thread, void* contextParam, void* parent);
bool onThreadBeginCopyFile(const wxString& sourcePath, const wxString& destinationPath);
void onThreadEndCopyFile(bool copy, const wxString& sourcePath, const wxString& destinationPath);

#endif
