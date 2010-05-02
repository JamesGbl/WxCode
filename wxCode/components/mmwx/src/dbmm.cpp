//
// Name     : dbmm.cpp
// Purpose  : Minimal database sample using mmDbase classes.
//            Opens database, opens a table, queries the table,
//            displays query results in a wxTextCtrl.
// Author   : Arne Morken
// Copyright: (C) 2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//

#include "mmDbase.h"

class Main: public wxApp
{
  public:
    bool OnInit(void);
}; // Main

class MyFrame: public wxFrame
{
  public:
    MyFrame();
    bool DoDB();

  private:
    wxTextCtrl* mTextCtrl;
}; // MyFrame

IMPLEMENT_APP(Main)

bool Main::OnInit(void)
{
  MyFrame* frame = new MyFrame();
  frame->Show(TRUE);
  return frame;
}

MyFrame::MyFrame()
: wxFrame(NULL, 1001, "minimaldb - mm", wxDefaultPosition, wxDefaultSize,
	  wxDEFAULT_FRAME_STYLE)
{
  mTextCtrl = new wxTextCtrl(this,-1,"",
                          wxPoint(0,0),wxSize(660,450),wxTE_MULTILINE);
  wxFont* font = new wxFont(9,wxMODERN,wxNORMAL,wxNORMAL);
  mTextCtrl->SetFont(*font);
  CreateStatusBar(2);
  SetStatusText("minimaldb - mm", 0);
  SetStatusText("Copyright (C) 2002 MindMatters - www.mindmatters.no", 1);
  SetSize(-1,-1,660,450);

  if (!DoDB())
    *mTextCtrl<<"DoDB failed\n";
}

bool MyFrame::DoDB()
{
  wxString dsn = "TESTDB";
  wxString dbfile = "TESTDB.gdb";
  wxString user = "SYSDBA";
  wxString pwd = "masterkey";
  wxString table_name = "TESTTAB";
  wxString odbc_driver = "INTERSOLV InterBase ODBC Driver (*.gdb)";

  // Add data source
  wxString reg_cmd = "DSN="+dsn+"@"+
                     "DB="+dbfile+"@"+
                     "UID="+user+"@"+
                     "PWD="+pwd+"@"+
		     "LockTimeOut=-1"+"@@";
  mmDbase* db = new mmDbase();
  if (!db->CreateDataSource(odbc_driver, reg_cmd))
    return FALSE;

  // Open database
  if (!db->Init(dsn,user,pwd))
    return FALSE;

  // Open table
  mmDbaseTable* table = db->OpenTable(table_name);
  if (!table)
    return FALSE;

  // Query
  table->DoQuery("","");
  while (table->GetNext())
    *mTextCtrl<<"pkey:"<<*(int*)table->GetColumn(0)->mData
           <<",data:"<<wxString((wxChar*)table->GetColumn(1)->mData)<<"\n";

  return TRUE;
}

