//
// Name     : mmDbase
// Purpose  : Wrapper around wxDb/wxDbTable stuff.
// Author   : Arne Morken
// Copyright: (C) 1999-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows license.
//

#include "mmDbase.h"
#include <wx/listimpl.cpp>

#ifdef __MMDEBUG__
extern wxTextCtrl *gDebug;
#endif

WX_DEFINE_LIST(mmDbaseRow)

//////////////////// mmDbase ////////////////////

mmDbase::mmDbase()
{
    mDB = NULL;
    mDbaseErr = wxT("");
} // mmDbase

mmDbase::~mmDbase()
{
    if(mDB)
    {
        mDB->CommitTrans();
        mDB->Close();
        wxDbFreeConnection(mDB);
    }
} // ~mmDbase

bool mmDbase::CreateDataSource(wxString &odbcDriver, wxString &regCmd)
// Create data source entries
{
#ifdef __MMDEBUG__
    *gDebug << wxT("odbcDriver:") << odbcDriver << wxT("\n");
    *gDebug << wxT("regCmd:") << regCmd << wxT("\n");
#endif
    for(unsigned int i = 0; i < regCmd.Length(); i++)
        if(regCmd[i] == '@')
            regCmd[i] = '\0';
    bool status = SQLConfigDataSource(0, ODBC_ADD_DSN, odbcDriver, regCmd);
    if(!status)
        mDbaseErr = wxT("Call to SQLConfigDataSource failed.\n");
    else
        mDbaseErr = wxT("");
    return status;
} // ConfigDataSource

bool mmDbase::Init(wxString dataSourceName, wxString user, wxString password, wxString dataSourceDir)
{
    if(!mDbConInf.AllocHenv())
    {
        mDbaseErr = wxT("Couldn't allocate space for database variables.\n");
        return FALSE;
    }
    mDbConInf.SetDsn(dataSourceName);
    mDbConInf.SetUid(user);
    mDbConInf.SetAuthStr(password);
    mDbConInf.SetDefaultDir(dataSourceDir);
#ifdef __MMDEBUG__
    *gDebug << wxT("dataSourceName:[") << mDbConInf.Dsn << wxT("]\n");
    *gDebug << wxT("user:[") << mDbConInf.Uid << wxT("]\n");
    *gDebug << wxT("password:[") << mDbConInf.AuthStr << wxT("]\n");
    *gDebug << wxT("dataSourceDir:[") << mDbConInf.DefaultDir << wxT("]\n");
#endif
    mDB = new wxDb(mDbConInf.GetHenv());
    if(!mDB->Open(mDbConInf.GetDsn(), mDbConInf.GetUserID(), mDbConInf.GetPassword()))
    {
        mDbaseErr = wxT("Couldn't open database.\n");
        return FALSE;
    }
    mDbaseErr = wxT("");
    return TRUE;
} // Init

mmDbaseTable *mmDbase::OpenTable(wxString tableName)
{
    if(!mDB)
    {
        mDbaseErr = wxT("No connection to database.");
        return NULL;
    }
    ::wxSetWorkingDirectory(mDbConInf.DefaultDir);
    UWORD num_cols;
    wxDbColInf *col_inf = mDB->GetColumns(tableName, &num_cols);
    if(!col_inf)
    {
        mDbaseErr = wxT("Couldn't find column information for ") + tableName + wxT("\n");
        return NULL;
    }
    if(!mDB->TableExists(tableName))
    {
        mDbaseErr = wxT("Couldn't find table ") + tableName + wxT("\n");
        delete[] col_inf;
        return NULL;
    }
    // Open the table, give it the col_inf object.
    mmDbaseTable *table = new mmDbaseTable(mDB, tableName, num_cols, col_inf,
                                           mDbConInf.DefaultDir);
    if(!table->Open())
    {
        mDbaseErr = wxT("Couldn't open table ") + tableName + wxT("\n");
        delete table; // col_inf is deleted here
        return NULL;
    }
    mDbaseErr = wxT("");
    return table;
} // OpenTable

/*
mmDbaseTable* mmDbase::CreateTable(wxString tableName, mmDbaseRow& row)
{ // NB! NOT TESTED!!!
  ::wxSetWorkingDirectory(mDbConInf.DefaultDir);
  wxDbColInf* cols = new wxDbColInf[row.GetCount()];
  for (unsigned int i=0; i<row.GetCount(); i++)
  {
    cols[i].sqlDataType = row[i]->mCType;
    wxStrcpy(cols[i].colName, (wxChar*)row[i]->mData);
  }
  mmDbaseTable* table = new mmDbaseTable(mDB, tableName, row.GetCount(), cols,mDbConInf.DefaultDir);
  if (!table->CreateTable())
  {
    mDbaseErr = "Kan ikke opprette "+tableName+"\n";
    delete table;
    return NULL;
  }
  mDbaseErr = "";
  return table;
} // CreateTable
*/

//////////////////// mmDbaseTable ////////////////////

mmDbaseTable::mmDbaseTable(wxDb *db, wxString &tabName, int ncols, wxDbColInf *col_inf, wxString &defDir)
    : wxDbTable(db, tabName, ncols, wxEmptyString, !wxDB_QUERY_ONLY, defDir)
{
    mColInf = col_inf;
    for(int i = 0; i < ncols; i++)
    {
#ifdef __MMDEBUG__
//        *gDebug << wxT("colNo=") << i << wxT(",type:") << mColInf[i].sqlDataType << wxT(",name:") << mColInf[i].colName << wxT(",size:") << (int)mColInf[i].columnSize << wxT("\n");
        *gDebug << wxT("colNo=") << i << wxT(",type:") << mColInf[i].sqlDataType << wxT(",name:") << mColInf[i].colName << wxT(",size:") << (int)mColInf[i].columnLength << wxT("\n");
#endif
//        mmDbaseColumn *col = new mmDbaseColumn(mColInf[i].sqlDataType, mColInf[i].columnSize);
        mmDbaseColumn *col = new mmDbaseColumn(mColInf[i].sqlDataType, mColInf[i].columnLength);
        mColInf[i].sqlDataType = col->mCType;
//        SetColDefs(i, mColInf[i].colName, col->mType, col->mData, col->mCType, mColInf[i].columnSize, i == 0, TRUE, TRUE);
        SetColDefs(i, mColInf[i].colName, col->mType, col->mData, col->mCType, mColInf[i].columnLength, i == 0, TRUE, TRUE);
        mRow.Append(col);
    }
} // mmDbaseTable

mmDbaseTable::~mmDbaseTable()
{
    mmDbaseRow::Node *node = mRow.GetFirst();
    while(node)
    {
        //delete node->GetData();
        delete node;
        node = mRow.GetFirst();
    }
    if(mColInf != NULL)
        delete[] mColInf;
} // ~mmDbaseTable

void mmDbaseTable::DoQuery(wxString whereStr, wxString orderStr)
{
    SetWhereClause(whereStr);
    SetOrderByClause(orderStr);
    SetFromClause(wxT(""));
    Query();
} // mmDbaseTable::DoQuery

void mmDbaseTable::SetColumn(int colNo, wxString val)
{
    mRow[colNo]->mCType = SQL_C_CHAR;
    mRow[colNo]->mType  = DB_DATA_TYPE_VARCHAR;
    if(mRow[colNo]->mData)
    {
//        wxStrncpy((wxChar*)mRow[colNo]->mData, val.c_str(), mColInf[colNo].columnSize);
        wxStrncpy((wxChar*)mRow[colNo]->mData, val.c_str(), mColInf[colNo].columnLength);
//        ((wxChar*)mRow[colNo]->mData)[mColInf[colNo].columnSize - 1] = '\0';
        ((wxChar*)mRow[colNo]->mData)[mColInf[colNo].columnLength - 1] = '\0';
    }
} // mmDbaseTable::SetColumn

void mmDbaseTable::SetColumn(int colNo, int val)
{
    mRow[colNo]->mCType = SQL_C_SLONG;
    mRow[colNo]->mType  = DB_DATA_TYPE_INTEGER;
    if(mRow[colNo]->mData)
        *((int*)mRow[colNo]->mData) = val;
} // mmDbaseTable::SetColumn

void mmDbaseTable::SetColumn(int colNo, long int val)
{
    SetColumn(colNo, (int)val);
} // mmDbaseTable::SetColumn

void mmDbaseTable::SetColumn(int colNo, short int val)
{
    SetColumn(colNo, (int)val);
} // mmDbaseTable::SetColumn

void mmDbaseTable::SetColumn(int colNo, bool val)
{
    SetColumn(colNo, (int)val);
} // mmDbaseTable::SetColumn

void mmDbaseTable::SetColumn(int colNo, float val)
{
    mRow[colNo]->mCType = SQL_C_FLOAT;
    mRow[colNo]->mType = DB_DATA_TYPE_FLOAT;
    if(mRow[colNo]->mData)
        *((float*)mRow[colNo]->mData) = val;
} // mmDbaseTable::SetColumn

void mmDbaseTable::SetColumn(int colNo, double val)
{
    mRow[colNo]->mCType = SQL_C_DOUBLE;
    mRow[colNo]->mType = DB_DATA_TYPE_FLOAT;
    if(mRow[colNo]->mData)
        *((double*)mRow[colNo]->mData) = val;
} // mmDbaseTable::SetColumn

void mmDbaseTable::SetColumn(int colNo, wxDateTime &val)
{
    mRow[colNo]->mCType = SQL_C_TIMESTAMP;
    mRow[colNo]->mType = DB_DATA_TYPE_DATE;
    if(mRow[colNo]->mData)
    {
        (*((TIMESTAMP_STRUCT*)mRow[colNo]->mData)).year     = val.GetYear();
        (*((TIMESTAMP_STRUCT*)mRow[colNo]->mData)).month    = val.GetMonth() + 1;
        (*((TIMESTAMP_STRUCT*)mRow[colNo]->mData)).day      = val.GetDay();
        (*((TIMESTAMP_STRUCT*)mRow[colNo]->mData)).hour     = val.GetHour();
        (*((TIMESTAMP_STRUCT*)mRow[colNo]->mData)).minute   = val.GetMinute();
        (*((TIMESTAMP_STRUCT*)mRow[colNo]->mData)).second   = val.GetSecond();
        (*((TIMESTAMP_STRUCT*)mRow[colNo]->mData)).fraction = 0;
    }
} // mmDbaseTable::SetColumn

//////////////////// mmDbaseColumn ////////////////////

mmDbaseColumn::mmDbaseColumn(int type, int size)
{
#ifdef __MMDEBUG__
    *gDebug << wxT("col type:") << type << wxT("\n");
#endif
    mData  = NULL;
    mCType = type;
    mSize  = size;
    switch(mCType)
    {
        case SQL_C_CHAR:
        case SQL_VARCHAR: // We need this - why?
            {
                mCType = SQL_C_CHAR;
                mType  = DB_DATA_TYPE_VARCHAR;
                mData  = new wxChar[mSize+1];
                break;
            }
        case SQL_C_LONG:
        case SQL_C_SLONG:
        case SQL_C_ULONG:
        case SQL_C_SHORT:
        case SQL_C_SSHORT:
        case SQL_C_USHORT:
        case SQL_C_TINYINT:
        case SQL_C_STINYINT:
        case SQL_C_UTINYINT:
        case SQL_C_BINARY:
        case SQL_C_BIT:
            {
                mCType = SQL_C_SLONG;
                mType  = DB_DATA_TYPE_INTEGER;
                mData  = new int(0);
                break;
            }
        case SQL_C_FLOAT:
            {
                mCType = SQL_C_FLOAT;
                mType  = DB_DATA_TYPE_FLOAT;
                mData  = new float(0.0);
                break;
            }
        case SQL_C_DOUBLE:
            {
                mCType = SQL_C_DOUBLE;
                mType  = DB_DATA_TYPE_FLOAT;
                mData  = new double(0.0);
                break;
            }
        case SQL_C_TIMESTAMP:
        case SQL_C_DATE:
        case SQL_C_TIME:
            {
                mCType = SQL_C_TIMESTAMP;
                mType  = DB_DATA_TYPE_DATE;
                mData  = new TIMESTAMP_STRUCT;
                (*((TIMESTAMP_STRUCT*)mData)).year     = 2000;
                (*((TIMESTAMP_STRUCT*)mData)).month    = 1;
                (*((TIMESTAMP_STRUCT*)mData)).day      = 1;
                (*((TIMESTAMP_STRUCT*)mData)).hour     = 0;
                (*((TIMESTAMP_STRUCT*)mData)).minute   = 0;
                (*((TIMESTAMP_STRUCT*)mData)).second   = 0;
                (*((TIMESTAMP_STRUCT*)mData)).fraction = 0;
                break;
            }
        default:
            {
                // Error msg here...
#ifdef __MMDEBUG__
                *gDebug << wxT("----------Illegal C type:") << mCType << wxT("\n");
#endif
                return;
            }
    } // switch
} // mmDbaseColumn

/*
void mmDbaseColumn::Update(wxString& str)
{
  if (!mData)
    return;

  switch(mType)
  {
    case DB_DATA_TYPE_VARCHAR:
    {
      wxStrncpy((wxChar*)mData, str.c_str(), mSize);
      break;
    }
    case DB_DATA_TYPE_INTEGER:
    {
      *(int*)(mData) = atoi(str);
      break;
    }
    case DB_DATA_TYPE_FLOAT:
    {
      *(float*)(mData) = atof(str);
      break;
    }
    case DB_DATA_TYPE_DATE: // str must be on format: YYYYMMDD
    {
      wxString yyyy,mm,dd;
      yyyy = str.Mid(0,4);
      mm   = str.Mid(4,2);
      dd   = str.Mid(6,2);
      (*(TIMESTAMP_STRUCT*)(mData)).year   = atoi(yyyy);
      (*(TIMESTAMP_STRUCT*)(mData)).month  = atoi(mm);
      (*(TIMESTAMP_STRUCT*)(mData)).day    = atoi(dd);
      (*(TIMESTAMP_STRUCT*)(mData)).hour   = 0;
      (*(TIMESTAMP_STRUCT*)(mData)).minute = 0;
      (*(TIMESTAMP_STRUCT*)(mData)).second = 0;
      (*(TIMESTAMP_STRUCT*)(mData)).fraction = 0;
      break;
    }
  } // switch
} // mmDbaseColumn::Update
*/

wxString mmDbaseColumn::Str()
{
    if(!mData)
        return wxT("");

    wxString str;
    switch(mType)
    {
        case DB_DATA_TYPE_VARCHAR:
            {
                str = wxString((wxChar*)mData).Left(mSize);
                str.Replace(wxT("'"), wxT("''"));
                break;
            }
        case DB_DATA_TYPE_INTEGER:
            {
                str.Printf(wxT("%d"), *(int*)mData);
                break;
            }
        case DB_DATA_TYPE_FLOAT:
            {
                str.Printf(wxT("%f"), *(float*)mData);
                break;
            }
        case DB_DATA_TYPE_DATE:
            {
                str.Printf(wxT("%d%d%d"), (*(TIMESTAMP_STRUCT*)(mData)).year, (*(TIMESTAMP_STRUCT*)(mData)).month, (*(TIMESTAMP_STRUCT*)(mData)).day);
                break;
            }
    }
    return str;
} // mmDbaseColumn::Str

///////////////////////////////////////////////////////////////////////////

bool UpdateDbase(mmDbase *db, mmDbaseTable *tab,
                 int col, wxString id_name, wxString id)
// Temporary solution
{
    wxString SQLstr;
    SQLstr = wxT("UPDATE ") + tab->GetTableName() + wxT(" SET ") + tab->GetColumnInfo(col).colName + wxT(" = '") + tab->GetColumn(col)->Str() + wxT("' WHERE ") + id_name + wxT(" = ") + id;
#ifdef __MMDEBUG__
    *gDebug << wxT("SQLstr:") << SQLstr << wxT("\n");
#endif
    if(!db->GetDBConn()->ExecSql(SQLstr.c_str()))
    {
        /*
        wxMessageBox("Feil ved database Update\n",
                     gAppName+" - Feil ved Update",
                     wxOK|wxCENTRE|wxICON_EXCLAMATION);
        	 */
        return FALSE;
    }
    else
        return TRUE;
} // UpdateDbase

