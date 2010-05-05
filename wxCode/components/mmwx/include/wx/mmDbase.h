//
// Name     : mmDbase
// Purpose  : Wrapper around wxDb/wxDbTable stuff.
// Author   : Arne Morken
// Copyright: (C) 1999-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence.
//

#ifndef _WXDBASE_H_INCLUDED
#define _WXDBASE_H_INCLUDED

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
#include <wx/list.h>
#include <wx/datetime.h>
#define __SQL
#include <odbcinst.h>

// Class for holding table column info
class mmDbaseColumn
{
  public:
    mmDbaseColumn(int type, int size);
   ~mmDbaseColumn() { if (mData) { delete mData; mData = NULL; } }

    bool SetDefault();
    //void Update(wxString& str);
    wxString Str();

    void* mData;
    int   mSize;
    int   mCType;
    int   mType;
};

WX_DECLARE_LIST(mmDbaseColumn, mmDbaseRow);

// Wrapper around a database table
class mmDbaseTable : public wxDbTable
{
  public:
    mmDbaseTable(wxDb* db, wxString& tabName,
		 int ncols, wxDbColInf* col_inf, wxString& defDir);
   ~mmDbaseTable();
    void           SetColumn(int colNo, wxString    val);
    void           SetColumn(int colNo, int         val);
    void           SetColumn(int colNo, long        val);
    void           SetColumn(int colNo, short       val);
    void           SetColumn(int colNo, bool        val);
    void           SetColumn(int colNo, float       val);
    void           SetColumn(int colNo, double      val);
    void           SetColumn(int colNo, wxDateTime& val);
    wxDbColInf&    GetColumnInfo(int i) { return mColInf[i]; }
    mmDbaseRow&    GetRow()             { return mRow; }
    mmDbaseColumn* GetColumn(int i)     { if (mRow.GetCount() <= 0)
                                            return NULL;
                                          return mRow[i];
                                        };
    unsigned int GetNumColumns()        { return mRow.GetCount(); }
    void DoQuery(wxString whereStr, wxString orderStr);

  private:
    wxDbColInf* mColInf;
    mmDbaseRow  mRow;
}; // mmDbaseTable

// A list of tables
WX_DECLARE_LIST(mmDbaseTable, mmDbaseTableList);

// Wrapper around some database stuff
class mmDbase
{
  public:
    mmDbase();
   ~mmDbase();
    bool CreateDataSource(wxString& odbcDriver, wxString& regCmd);
    bool Init(wxString dsn      = wxEmptyString,
              wxString user     = wxEmptyString,
              wxString password = wxEmptyString,
              wxString dbaseDir = wxEmptyString);
  //mmDbaseTable* CreateTable  (wxString tableName,mmDbaseRow& row); // UNTESTED
    mmDbaseTable* OpenTable    (wxString tableName);
    wxDb*         GetDBConn() { return mDB; }
    wxString      GetErrMsg() { return mDbaseErr; }

  private:
    wxDbConnectInf mDbConInf;
    wxDb*    mDB; // Connection to the database
    wxString mDbaseErr;
}; // mmDbase

// Temporary solution for Updating an Interbase database
extern bool UpdateDbase(mmDbase* db, mmDbaseTable* tab, int col, wxString id_name, wxString id);

#endif

