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
	EVT_LIST_ITEM_SELECTED(ID_WXLISTCTRL,hotKeysDlg::WxListCtrlSelected)
	EVT_BUTTON(ID_OKBTN,hotKeysDlg::OkBtnClick)
	////Manual Code End
	EVT_BUTTON(ID_OKBTN,hotKeysDlg::CancelBtnClick)
	EVT_BUTTON(ID_SAVEBTN,hotKeysDlg::saveBtnClick)
	EVT_BUTTON(ID_EDITCANCELBTN,hotKeysDlg::editCancelBtnClick)
	EVT_BUTTON(ID_OPENFILEBTN,hotKeysDlg::openFileDlgClick)
	EVT_BUTTON(ID_DELBTN,hotKeysDlg::delBtnClick)
	EVT_BUTTON(ID_EDITBTN,hotKeysDlg::editBtnClick)
	EVT_BUTTON(ID_ADDBTN,hotKeysDlg::addBtnClick)
	EVT_BUTTON(ID_CANCELBTN,hotKeysDlg::CancelBtnClick)
END_EVENT_TABLE()
////Event Table End

BEGIN_EVENT_TABLE(EventTextCtrl, wxTextCtrl)
    EVT_CHAR(EventTextCtrl::OnChar)
END_EVENT_TABLE()

void EventTextCtrl::OnChar(wxKeyEvent& event)
{
  wxString key;
  int keyCode = event.GetKeyCode();
  
  key = keyCodeToString(keyCode);
  SetValue(key.Upper());
  actualKeyCode = (key.empty()?0:keyCode);
  event.Skip();
}

hotKeysDlg::hotKeysDlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

hotKeysDlg::~hotKeysDlg()
{
	delete(saveBtn);
	delete(editCancelBtn);
	delete(openFileBtn);
	delete(txtHotKey);
	delete(lblHotKey);
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
	

	okBtn = new wxButton(this, ID_OKBTN, wxT("Ok"), wxPoint(303,312), wxSize(75,25), 0, wxDefaultValidator, wxT("okBtn"));

	lblHotKey = new wxStaticText(this, ID_LBLHOTKEY, wxT("Press Key (Ctrl + ? or F?)"), wxPoint(10,278), wxDefaultSize, 0, wxT("lblHotKey"));
	lblHotKey->SetFont(wxFont(9, wxSWISS, wxNORMAL,wxNORMAL, FALSE, wxT("Verdana")));

	saveBtn = new wxButton(this, ID_SAVEBTN, wxT("Save"), wxPoint(293,276), wxSize(73,25), 0, wxDefaultValidator, wxT("saveBtn"));

	editCancelBtn = new wxButton(this, ID_EDITCANCELBTN, wxT("Cancel"), wxPoint(367,276), wxSize(75,25), 0, wxDefaultValidator, wxT("editCancelBtn"));

	openFileBtn = new wxButton(this, ID_OPENFILEBTN, wxT(""), wxPoint(420,251), wxSize(23,21), 0, wxDefaultValidator, wxT("openFileBtn"));
	openFileBtn->SetFont(wxFont(20, wxSWISS, wxNORMAL,wxNORMAL, FALSE, wxT("Arial Black")));

	txtProgram = new wxTextCtrl(this, ID_TXTPROGRAM, wxT(""), wxPoint(9,252), wxSize(408,20), 0, wxDefaultValidator, wxT("txtProgram"));

	lblProgram = new wxStaticText(this, ID_LBLPROGRAM, wxT("Program:"), wxPoint(11,227), wxDefaultSize, 0, wxT("lblProgram"));
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

	WxStaticBox1 = new wxStaticBox(this, ID_WXSTATICBOX1, wxT("Hot Keys for Launch other Programs"), wxPoint(2,201), wxSize(455,107));
	WxStaticBox1->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, FALSE, wxT("Verdana")));

	CancelBtn = new wxButton(this, ID_CANCELBTN, wxT("Cancel"), wxPoint(380,312), wxSize(75,25), 0, wxDefaultValidator, wxT("CancelBtn"));

	WxListCtrl = new wxListCtrl(this, ID_WXLISTCTRL, wxPoint(0,0), wxSize(454,192), wxLC_REPORT | wxLC_EDIT_LABELS | wxLC_SINGLE_SEL);
	WxListCtrl->InsertColumn(0,wxT("Keys"),wxLIST_FORMAT_LEFT,125 );
	WxListCtrl->InsertColumn(0,wxT("Programs"),wxLIST_FORMAT_LEFT,300 );
	////GUI Items Creation End

	txtHotKey = new EventTextCtrl((wxWindow*)this, ID_WXEDIT, wxT(""), wxPoint(173,277), wxSize(116,19), wxTE_READONLY | wxTE_CENTRE );
	
	mode=0;
	setState();
}

void hotKeysDlg::setLanguage(multiLang parent_lang)
{
   lang = parent_lang;
   
   SetTitle(lang["Hot Keys Dialog"]);
	okBtn->SetLabel(lang["Ok"]);
	lblHotKey->SetLabel(lang["Press Key (Ctrl + ? or F?)"]);
	saveBtn->SetLabel(lang["Save"]);
	editCancelBtn->SetLabel(lang["Cancel"]);
	lblProgram->SetLabel(lang["Programs:"]);
	delBtn->SetToolTip(lang["Delete"]);
	editBtn->SetLabel(lang["Edit"]);
	editBtn->SetToolTip(lang["Edit"]);
	addBtn->SetToolTip(lang["Add"]);
	WxStaticBox1->SetLabel(lang["Hot Keys for Launch other Programs"]);
	CancelBtn->SetLabel(lang["Cancel"]);
	
	while (WxListCtrl->GetColumnCount()>0) WxListCtrl->DeleteColumn(0);
	WxListCtrl->InsertColumn(0,lang["Keys"],wxLIST_FORMAT_LEFT,125 );
	WxListCtrl->InsertColumn(0,lang["Programs"],wxLIST_FORMAT_LEFT,300 );
	
}

void hotKeysDlg::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
}

void hotKeysDlg::refreshListCtrl()
{
   WxListCtrl->DeleteAllItems();
   hotKeyMap::iterator iter;
   for( iter = m_keysMap.begin(); iter != m_keysMap.end(); iter++ )
   {
      WxListCtrl->InsertItem(iter->first, iter->second.program);
      WxListCtrl->SetItem(iter->first, 1, keyCodeToString(iter->second.keyCode));
   }
}

void hotKeysDlg::setState()
{
	openFileBtn->Enable(mode != READ_MODE);
	txtProgram->Enable(mode != READ_MODE);
	txtHotKey->Enable(mode != READ_MODE);
   saveBtn->Enable(mode != READ_MODE);
	editCancelBtn->Enable(mode != READ_MODE);

	delBtn->Enable(mode == READ_MODE);
	editBtn->Enable(mode == READ_MODE);
	addBtn->Enable(mode == READ_MODE);
	CancelBtn->Enable(mode == READ_MODE);
	okBtn->Enable(mode == READ_MODE);
	WxListCtrl->Enable(mode == READ_MODE);
}

void hotKeysDlg::CancelBtnClick(wxCommandEvent& event)
{
	this->EndDialog(wxCANCEL);
}

void hotKeysDlg::OkBtnClick(wxCommandEvent& event)
{
	this->EndDialog(wxID_OK);
}

void hotKeysDlg::addBtnClick(wxCommandEvent& event)
{
	mode = ADD_MODE;
	setState();
	txtProgram->SetValue("");
	txtHotKey->SetValue("");
}

void hotKeysDlg::openFileDlgClick(wxCommandEvent& event)
{
   wxFileDialog fileDialog(this, wxT("Choose a Program"), wxT(""), wxT(""), wxT("*.*"), wxOPEN | wxFILE_MUST_EXIST);
	fileDialog.ShowModal();
	txtProgram->SetValue(fileDialog.GetDirectory() + "\\" + fileDialog.GetFilename());
}

void hotKeysDlg::editCancelBtnClick(wxCommandEvent& event)
{
	mode = READ_MODE;
	txtProgram->SetValue("");
	txtHotKey->SetValue("");
	setState();
}

void hotKeysDlg::saveBtnClick(wxCommandEvent& event)
{
   int numItem = -1;
	if (mode == ADD_MODE)
       numItem = WxListCtrl->GetItemCount();
	else
      numItem = WxListCtrl->GetNextItem(numItem, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
   
   m_keysMap[numItem].program = txtProgram->GetValue();
   m_keysMap[numItem].keyCode = txtHotKey->actualKeyCode;
   
   refreshListCtrl();

	mode = READ_MODE;
	setState();
}

void hotKeysDlg::editBtnClick(wxCommandEvent& event)
{
	mode = EDIT_MODE;
	setState();
}

void hotKeysDlg::WxListCtrlSelected(wxListEvent& event)
{
   long item = -1;
   item = WxListCtrl->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
   wxListItem itemCtrl = event.GetItem();
   
   txtProgram->SetValue(m_keysMap[item].program);
   txtHotKey->SetValue(keyCodeToString(m_keysMap[item].keyCode));
}

void hotKeysDlg::delBtnClick(wxCommandEvent& event)
{
      long item = -1;
      item = WxListCtrl->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
      if (item != -1)
      {
         WxListCtrl->DeleteItem(item);
         m_keysMap.erase(item);
      }
}

hotKeyMap hotKeysDlg::getKeysMap()
{
   return m_keysMap;
}

void hotKeysDlg::setKeysMap(hotKeyMap keysMap)
{
   m_keysMap = keysMap;
   refreshListCtrl();
}
