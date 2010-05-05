//
// Name     : dbwx.cpp
// Purpose  : Minimal database sample using wxDb/wxDbTable classes.
//            Opens database, opens a table, queries the table,
//            displays query results in a wxTextCtrl.
// Author   : Arne Morken
// Copyright: (C) 2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/string.h>
#include <wx/db.h>
#include <wx/dbtable.h>
#define __SQL
#include <odbcinst.h>

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
        wxTextCtrl *mTextCtrl;
}; // MyFrame

IMPLEMENT_APP(Main)

bool Main::OnInit(void)
{
    MyFrame *frame = new MyFrame();
    frame->Show(TRUE);
    return frame;
}

MyFrame::MyFrame()
    : wxFrame(NULL, 1001, wxT("minimaldb - wx"), wxDefaultPosition, wxDefaultSize,
              wxDEFAULT_FRAME_STYLE)
{
    mTextCtrl = new wxTextCtrl(this, -1, wxEmptyString,
                               wxPoint(0, 0), wxSize(660, 450), wxTE_MULTILINE);
    wxFont *font = new wxFont(9, wxMODERN, wxNORMAL, wxNORMAL);
    mTextCtrl->SetFont(*font);
    CreateStatusBar(2);
    SetStatusText(wxT("minimaldb - wx"), 0);
    SetStatusText(wxT("Copyright (C) 2002, 2010 MindMatters - www.mindmatters.no, Gary Harris"), 1);
    SetSize(-1, -1, 660, 450);

    if(!DoDB())
        *mTextCtrl << _("DoDB failed\n");
}

bool MyFrame::DoDB()
{
    wxString dsn = wxT("TESTDB");
    wxString dbfile = wxT("TESTDB.gdb");
    wxString user = wxT("SYSDBA");
    wxString pwd = wxT("masterkey");
    wxString table_name = wxT("TESTTAB");
    wxString odbc_driver = wxT("INTERSOLV InterBase ODBC Driver (*.gdb)");

    // Add data source
    wxString reg_cmd = wxT("DSN=") + dsn + wxT("@") +
                       wxT("DB=") + dbfile + wxT("@") +
                       wxT("UID=") + user + wxT("@") +
                       wxT("PWD=") + pwd + wxT("@") +
                       wxT("LockTimeOut=-1") + wxT("@@");
    for(unsigned int i = 0; i < reg_cmd.Length(); i++)
        if(reg_cmd[i] == '@')
            reg_cmd[i] = '\0';
    if(!SQLConfigDataSource(0, ODBC_ADD_DSN, odbc_driver, reg_cmd))
        return FALSE;

    // Open database
    wxDbConnectInf con_inf;
    if(!con_inf.AllocHenv())
        return FALSE;
    con_inf.SetDsn(dsn);
    con_inf.SetUid(user);
    con_inf.SetAuthStr(pwd);
    wxDb *db = new wxDb(con_inf.GetHenv());
    if(!db->Open(con_inf.GetDsn(), con_inf.GetUserID(), con_inf.GetPassword()))
        return FALSE;

//    // Open table
//    UWORD num_cols;
//    wxDbColInf *col_inf = db->GetColumns(table_name, &num_cols);
//    if(!col_inf)
//        return FALSE;
//    if(!db->TableExists(table_name))
//        return FALSE;
//    wxDbTable *table = new wxDbTable(db, table_name, num_cols, NULL, !wxDB_QUERY_ONLY);
//
//    // Define column data
//    int pkey;
//
//    // TODO Fix me.
////    wxChar *data = new wxChar[col_inf[1].columnSize + 1];
////    table->SetColDefs(0, col_inf[0].colName, DB_DATA_TYPE_INTEGER,
////                      &pkey, SQL_C_SLONG,
////                      col_inf[0].columnSize, TRUE, TRUE, TRUE);
////    table->SetColDefs(1, col_inf[1].colName, DB_DATA_TYPE_VARCHAR,
////                      data, SQL_C_CHAR,
////                      col_inf[1].columnSize, FALSE, TRUE, TRUE);
//    wxChar *data = new wxChar[col_inf[1].columnLength + 1];
//    table->SetColDefs(0, col_inf[0].colName, DB_DATA_TYPE_INTEGER, &pkey, SQL_C_SLONG, col_inf[0].columnLength, TRUE, TRUE, TRUE);
//    table->SetColDefs(1, col_inf[1].colName, DB_DATA_TYPE_VARCHAR, data, SQL_C_CHAR, col_inf[1].columnLength, FALSE, TRUE, TRUE);
//    if(!table->Open())
//        return FALSE;
//
//    // Query
//    table->SetWhereClause(wxT(""));
//    table->SetOrderByClause(wxT(""));
//    table->SetFromClause(wxT(""));
//    table->Query();
//    while(table->GetNext())
////        *mTextCtrl << wxT("pkey:") << pkey << wxT(",data:") << data << wxT("\n");
//        *mTextCtrl << wxT("pkey:") << pkey << wxT(",data:") << data << wxT("\n");

    return TRUE;
}

