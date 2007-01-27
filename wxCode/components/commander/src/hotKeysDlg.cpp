//---------------------------------------------------------------------------
//
// Name:        hotKeysDlg.cpp
// Author:      Armando Urdiales González
// Created:     21/01/2007 17:26:36
// Description: hotKeysDlg class implementation
//
//---------------------------------------------------------------------------

#include "hotKeysDlg.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
#include "Images/Self_hotKeysDlg_XPM.xpm"
////Header Include End

//----------------------------------------------------------------------------
// hotKeysDlg
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(hotKeysDlg,wxDialog)
	////Manual Code Start
	////Manual Code End
	EVT_BUTTON(ID_SAVEBTN,hotKeysDlg::saveBtnClick)
	EVT_BUTTON(ID_EDITCANCELBTN,hotKeysDlg::editCancelBtnClick)
	EVT_BUTTON(ID_OPENFILEBTN,hotKeysDlg::openFileDlgClick)
	EVT_BUTTON(ID_DELBTN,hotKeysDlg::delBtnClick)
	EVT_BUTTON(ID_EDITBTN,hotKeysDlg::editBtnClick)
	EVT_BUTTON(ID_ADDBTN,hotKeysDlg::addBtnClick)
	EVT_BUTTON(ID_CANCELBTN,hotKeysDlg::CancelBtnClick)
END_EVENT_TABLE()
////Event Table End

hotKeysDlg::hotKeysDlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

hotKeysDlg::~hotKeysDlg()
{
   delete(wxOpenFileDialog);
	delete(saveBtn);
	delete(editCancelBtn);
	delete(openFileBtn);
	delete(txtProgram);
	delete(lblProgram);
	delete(delBtn);
	delete(editBtn);
	delete(addBtn);
	delete(WxStaticBox1);
	delete(CancelBtn);
	delete(okBtn);
	delete(WxListCtrl);
}

void hotKeysDlg::CreateGUIControls()
{
	//Do not add custom code between
        //GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	SetTitle(wxT("Hot Keys Dialog"));
	SetIcon(Self_hotKeysDlg_XPM);
	SetSize(8,8,468,374);
	Center();


	wxOpenFileDialog =  new wxFileDialog(this, wxT("Choose a Program"), wxT(""), wxT(""), wxT("*.*"), wxOPEN | wxFILE_MUST_EXIST);

	saveBtn = new wxButton(this, ID_SAVEBTN, wxT("Save"), wxPoint(293,276), wxSize(73,25), 0, wxDefaultValidator, wxT("saveBtn"));

	editCancelBtn = new wxButton(this, ID_EDITCANCELBTN, wxT("Cancel"), wxPoint(367,276), wxSize(75,25), 0, wxDefaultValidator, wxT("editCancelBtn"));

	openFileBtn = new wxButton(this, ID_OPENFILEBTN, wxT(""), wxPoint(420,251), wxSize(23,21), 0, wxDefaultValidator, wxT("openFileBtn"));
	openFileBtn->SetFont(wxFont(20, wxSWISS, wxNORMAL,wxNORMAL, FALSE, wxT("Arial Black")));

	txtProgram = new wxTextCtrl(this, ID_TXTPROGRAM, wxT(""), wxPoint(9,252), wxSize(408,20), 0, wxDefaultValidator, wxT("txtProgram"));

	lblProgram = new wxStaticText(this, ID_LBLPROGRAM, wxT("Program:"), wxPoint(14,227), wxDefaultSize, 0, wxT("lblProgram"));
	lblProgram->SetFont(wxFont(9, wxSWISS, wxNORMAL,wxNORMAL, FALSE, wxT("Verdana")));

	delBtn = new wxButton(this, ID_DELBTN, wxT("-"), wxPoint(417,217), wxSize(27,26), 0, wxDefaultValidator, wxT("delBtn"));
	delBtn->SetToolTip(wxT("Delete"));
	delBtn->SetFont(wxFont(20, wxSWISS, wxNORMAL,wxNORMAL, FALSE, wxT("Arial Black")));

	editBtn = new wxButton(this, ID_EDITBTN, wxT("Edit"), wxPoint(379,217), wxSize(38,26), 0, wxDefaultValidator, wxT("editBtn"));
	editBtn->SetToolTip(wxT("Edit"));
	editBtn->SetFont(wxFont(10, wxSWISS, wxNORMAL,wxNORMAL, FALSE, wxT("Arial Black")));

	addBtn = new wxButton(this, ID_ADDBTN, wxT("+"), wxPoint(353,217), wxSize(27,26), 0, wxDefaultValidator, wxT("addBtn"));
	addBtn->SetToolTip(wxT("Add"));
	addBtn->SetFont(wxFont(20, wxSWISS, wxNORMAL,wxNORMAL, FALSE, wxT("Arial Black")));

	WxStaticBox1 = new wxStaticBox(this, ID_WXSTATICBOX1, wxT("Hot Keys for Launch other Programs"), wxPoint(1,200), wxSize(455,107));
	WxStaticBox1->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, FALSE, wxT("Verdana")));

	CancelBtn = new wxButton(this, ID_CANCELBTN, wxT("Cancel"), wxPoint(381,312), wxSize(75,25), 0, wxDefaultValidator, wxT("CancelBtn"));

	okBtn = new wxButton(this, ID_OKBTN, wxT("Ok"), wxPoint(304,312), wxSize(73,25), 0, wxDefaultValidator, wxT("okBtn"));

	WxListCtrl = new wxListCtrl(this, ID_WXLISTCTRL, wxPoint(1,1), wxSize(454,192), wxLC_REPORT | wxLC_AUTOARRANGE | wxLC_EDIT_LABELS | wxLC_SINGLE_SEL | wxLC_SORT_ASCENDING);
	WxListCtrl->InsertColumn(0,wxT("Keys"),wxLIST_FORMAT_LEFT,125 );
	WxListCtrl->InsertColumn(0,wxT("Programs"),wxLIST_FORMAT_LEFT,300 );
	////GUI Items Creation End

	mode=0;
	setState();
}

void hotKeysDlg::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
}

void hotKeysDlg::setState()
{
	openFileBtn->Enable(mode != READ_MODE);
	txtProgram->Enable(mode != READ_MODE);
   saveBtn->Enable(mode != READ_MODE);
	editCancelBtn->Enable(mode != READ_MODE);

	delBtn->Enable(mode == READ_MODE);
	editBtn->Enable(mode == READ_MODE);
	addBtn->Enable(mode == READ_MODE);
	CancelBtn->Enable(mode == READ_MODE);
	okBtn->Enable(mode == READ_MODE);
	WxListCtrl->Enable(mode == READ_MODE);
}

/*
 * CancelBtnClick
 */
void hotKeysDlg::CancelBtnClick(wxCommandEvent& event)
{
	this->EndDialog(0);
}

/*
 * addBtnClick
 */
void hotKeysDlg::addBtnClick(wxCommandEvent& event)
{
	// insert your code here
	mode = ADD_MODE;
	setState();
	txtProgram->SetValue("");
}

/*
 * openFileDlgClick
 */
void hotKeysDlg::openFileDlgClick(wxCommandEvent& event)
{
	// insert your code here
	wxOpenFileDialog->ShowModal();
	txtProgram->SetValue(wxOpenFileDialog->GetDirectory() + "\\" + wxOpenFileDialog->GetFilename());
}

/*
 * editCancelBtnClick
 */
void hotKeysDlg::editCancelBtnClick(wxCommandEvent& event)
{
	// insert your code here
	mode = READ_MODE;
	txtProgram->SetValue("");
	setState();
}

/*
 * saveBtnClick
 */
void hotKeysDlg::saveBtnClick(wxCommandEvent& event)
{
	// insert your code here
	if (mode == ADD_MODE)
	{
       int numItems = WxListCtrl->GetItemCount();
       WxListCtrl->InsertItem(numItems, txtProgram->GetValue());
   }
	else
	{
      long item = -1;
      item = WxListCtrl->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
      if ( item != -1 )
         WxListCtrl->SetItem(item, 0, txtProgram->GetValue());
   }

	mode = READ_MODE;
	setState();
}

/*
 * editBtnClick
 */
void hotKeysDlg::editBtnClick(wxCommandEvent& event)
{
	// insert your code here
	mode = EDIT_MODE;
	setState();
}

/*
 * WxListCtrlSelected
 */
void hotKeysDlg::WxListCtrlSelected(wxListEvent& event)
{
   long item = -1;
   item = WxListCtrl->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
   txtProgram->SetValue(WxListCtrl->GetItemText(item));
}

//wxAcceleratorTable

/*
 * delBtnClick
 */
void hotKeysDlg::delBtnClick(wxCommandEvent& event)
{
      long item = -1;
      item = WxListCtrl->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
      if (item != -1)
         WxListCtrl->DeleteItem(item);
}

