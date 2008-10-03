#include "../include/MysqlDatabaseLayer.h"
#include "../include/MysqlPreparedStatement.h"
#include "../include/MysqlPreparedStatementResultSet.h"
#include "../include/DatabaseErrorCodes.h"
#include "../include/DatabaseLayerException.h"

#include <wx/tokenzr.h>

// ctor
MysqlDatabaseLayer::MysqlDatabaseLayer()
 : DatabaseLayer()
{
  InitDatabase();
  m_strServer = _("localhost");
  m_iPort = 3306; // default
  m_strDatabase = _("");
  m_strUser = _("");
  m_strPassword = _("");
}

MysqlDatabaseLayer::MysqlDatabaseLayer(const wxString& strDatabase)
 : DatabaseLayer()
{
  InitDatabase();
  m_strServer = _("localhost");
  m_iPort = 3306; // default
  m_strUser = _("");
  m_strPassword = _("");
  Open(strDatabase);
}

MysqlDatabaseLayer::MysqlDatabaseLayer(const wxString& strServer, const wxString& strDatabase)
 : DatabaseLayer()
{
  InitDatabase();
  ParseServerAndPort(strServer);
  m_strUser = _("");
  m_strPassword = _("");
  Open(strDatabase);
}

MysqlDatabaseLayer::MysqlDatabaseLayer(const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
 : DatabaseLayer()
{
  InitDatabase();
  m_strServer = _("localhost");
  m_iPort = 3306; // default
  m_strUser = strUser;
  m_strPassword = strPassword;
  Open(strDatabase);
}

MysqlDatabaseLayer::MysqlDatabaseLayer(const wxString& strServer, const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
 : DatabaseLayer()
{
  InitDatabase();
  ParseServerAndPort(strServer);
  m_strUser = strUser;
  m_strPassword = strPassword;
  Open(strDatabase);
}

// dtor
MysqlDatabaseLayer::~MysqlDatabaseLayer()
{
  Close();
  //mysql_close(m_pDatabase);
  //delete m_pDatabase;
  //mysql_library_end();
  mysql_server_end();
}

// open database
void MysqlDatabaseLayer::InitDatabase()
{
  //char *server_options[] = { "mysql_test", "--defaults-file=my.cnf" };
  //int num_elements = sizeof(server_options)/ sizeof(char *);

  //char *server_groups[] = { "libmysqld_server", "libmysqld_client" };
  //mysql_server_init(num_elements, server_options, server_groups);
  //m_pDatabase = new MYSQL();
  //mysql_library_init();
  //mysql_init(m_pDatabase);
//  mysql_server_init( 0, NULL, NULL );
  m_pDatabase = mysql_init(NULL);
}

// open database
bool MysqlDatabaseLayer::Open(const wxString& strServer, const wxString& strDatabase)
{
  ParseServerAndPort(strServer);
  return Open(strDatabase);
}

bool MysqlDatabaseLayer::Open(const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
{
  m_strUser = strUser;
  m_strPassword = strPassword;
  return Open(strDatabase);
}

bool MysqlDatabaseLayer::Open(const wxString& strServer, const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
{
  ParseServerAndPort(strServer);
  m_strUser = strUser;
  m_strPassword = strPassword;
  return Open(strDatabase);
}

bool MysqlDatabaseLayer::Open(const wxString& strDatabase)
{
  m_strDatabase = strDatabase;
  
  wxCharBuffer serverCharBuffer = ConvertToUnicodeStream(m_strServer);
  wxCharBuffer userCharBuffer = ConvertToUnicodeStream(m_strUser);
  wxCharBuffer passwordCharBuffer = ConvertToUnicodeStream(m_strPassword);
  wxCharBuffer databaseNameCharBuffer = ConvertToUnicodeStream(m_strDatabase);
  long connectFlags = 0; 
#if MYSQL_VERSION_ID >= 40100 
  #if !defined ulong 
    #define ulong unsigned long 
  #endif 
  connectFlags |= CLIENT_MULTI_RESULTS; 
  connectFlags |= CLIENT_MULTI_STATEMENTS; 
#endif 
   if (mysql_real_connect(m_pDatabase, serverCharBuffer, userCharBuffer, passwordCharBuffer, databaseNameCharBuffer, m_iPort, NULL/*socket*/, connectFlags) != NULL)   
  {
#if wxUSE_UNICODE
    const char* sqlStatement = "SET CHARACTER_SET_CLIENT=utf8, "
                 "CHARACTER_SET_CONNECTION=utf8, "
                 "CHARACTER_SET_RESULTS=utf8;";

    mysql_real_query(m_pDatabase, sqlStatement, strlen(sqlStatement));
    wxCSConv conv(_("UTF-8"));
    SetEncoding(&conv);
#endif

    return true;
  }
  else
  {
    SetErrorCode(MysqlDatabaseLayer::TranslateErrorCode(mysql_errno(m_pDatabase)));
    SetErrorMessage(ConvertFromUnicodeStream(mysql_error(m_pDatabase)));
    ThrowDatabaseException();
    return false;
  }
}

void MysqlDatabaseLayer::ParseServerAndPort(const wxString& strServer)
{
  int portIndicator = strServer.Find(_(":"));
  if (portIndicator > -1)
  {
    m_strServer = strServer.SubString(0, portIndicator-1);
    m_iPort = wxAtoi(strServer.SubString(portIndicator+1, strServer.Length()-1));
  }
  else
  {
    m_strServer = strServer;
    m_iPort = 3306; // default
  }
}

// close database
bool MysqlDatabaseLayer::Close()
{
  CloseResultSets();
  CloseStatements();

  ResetErrorCodes();
  if (m_pDatabase)
  {
    mysql_close(m_pDatabase);
    m_pDatabase = NULL;
  }
//  mysql_server_end();
  //delete m_pDatabase;
  //m_pDatabase = NULL;
  return true;
}

  
bool MysqlDatabaseLayer::IsOpen()
{
  return (m_pDatabase != NULL);
}

// transaction support
void MysqlDatabaseLayer::BeginTransaction()
{
  ResetErrorCodes();

  int nReturn = mysql_autocommit(m_pDatabase, 0);
  if (nReturn != 0)
  {
    SetErrorCode(MysqlDatabaseLayer::TranslateErrorCode(mysql_errno(m_pDatabase)));
    SetErrorMessage(ConvertFromUnicodeStream(mysql_error(m_pDatabase)));
    ThrowDatabaseException();
  }
}

void MysqlDatabaseLayer::Commit()
{
  ResetErrorCodes();

  int nReturn = mysql_commit(m_pDatabase);
  if (nReturn != 0)
  {
    SetErrorCode(MysqlDatabaseLayer::TranslateErrorCode(mysql_errno(m_pDatabase)));
    SetErrorMessage(ConvertFromUnicodeStream(mysql_error(m_pDatabase)));
    ThrowDatabaseException();
  }
  nReturn = mysql_autocommit(m_pDatabase, 1);
  if (nReturn != 0)
  {
    SetErrorCode(MysqlDatabaseLayer::TranslateErrorCode(mysql_errno(m_pDatabase)));
    SetErrorMessage(ConvertFromUnicodeStream(mysql_error(m_pDatabase)));
    ThrowDatabaseException();
  }
}

void MysqlDatabaseLayer::RollBack()
{
  ResetErrorCodes();

  int nReturn = mysql_rollback(m_pDatabase);
  if (nReturn != 0)
  {
    SetErrorCode(MysqlDatabaseLayer::TranslateErrorCode(mysql_errno(m_pDatabase)));
    SetErrorMessage(ConvertFromUnicodeStream(mysql_error(m_pDatabase)));
    ThrowDatabaseException();
  }
  nReturn = mysql_autocommit(m_pDatabase, 1);
  if (nReturn != 0)
  {
    SetErrorCode(MysqlDatabaseLayer::TranslateErrorCode(mysql_errno(m_pDatabase)));
    SetErrorMessage(ConvertFromUnicodeStream(mysql_error(m_pDatabase)));
    ThrowDatabaseException();
  }
}

  
// query database
int MysqlDatabaseLayer::RunQuery(const wxString& strQuery, bool bParseQuery)
{
  ResetErrorCodes();

  wxArrayString QueryArray;
  if (bParseQuery)
    QueryArray = ParseQueries(strQuery);
  else
    QueryArray.push_back(strQuery);

  wxArrayString::iterator start = QueryArray.begin();
  wxArrayString::iterator stop = QueryArray.end();

  while (start != stop)
  {
    wxCharBuffer sqlBuffer = ConvertToUnicodeStream(*start);
    //puts(sqlBuffer);
    int nReturn = mysql_query(m_pDatabase, sqlBuffer);
    if (nReturn != 0)
    {
      SetErrorCode(MysqlDatabaseLayer::TranslateErrorCode(mysql_errno(m_pDatabase)));
      SetErrorMessage(ConvertFromUnicodeStream(mysql_error(m_pDatabase)));
      ThrowDatabaseException();
      return DATABASE_LAYER_QUERY_RESULT_ERROR;
    }
    start++;
  }
  return mysql_affected_rows(m_pDatabase);
}

DatabaseResultSet* MysqlDatabaseLayer::RunQueryWithResults(const wxString& strQuery)
{
  ResetErrorCodes();

  wxArrayString QueryArray = ParseQueries(strQuery);

  int nArraySize = QueryArray.size();
  MysqlPreparedStatementResultSet* pResultSet = NULL;
  for (int i=0; i<nArraySize; i++)
  {
    wxString strCurrentQuery = QueryArray[i];
    MYSQL_STMT* pMysqlStatement = mysql_stmt_init(m_pDatabase);
    if (pMysqlStatement != NULL)
    {
      wxCharBuffer sqlBuffer = ConvertToUnicodeStream(strCurrentQuery);
      //puts(sqlBuffer);
      wxString sqlUTF8((const char*)sqlBuffer, wxConvUTF8);
      if (mysql_stmt_prepare(pMysqlStatement, sqlBuffer, sqlUTF8.Length()) == 0)
      {
        int nReturn = mysql_stmt_execute(pMysqlStatement);
        if (nReturn != 0)
        {
          SetErrorCode(MysqlDatabaseLayer::TranslateErrorCode(mysql_stmt_errno(pMysqlStatement)));
          SetErrorMessage(ConvertFromUnicodeStream(mysql_stmt_error(pMysqlStatement)));

          // Clean up after ourselves
          mysql_stmt_free_result(pMysqlStatement);
          mysql_stmt_close(pMysqlStatement);

          ThrowDatabaseException();
          return NULL;
        }
      }
      else
      {
        SetErrorCode(MysqlDatabaseLayer::TranslateErrorCode(mysql_errno(m_pDatabase)));
        SetErrorMessage(ConvertFromUnicodeStream(mysql_error(m_pDatabase)));
        ThrowDatabaseException();
      }
      if (i == nArraySize-1)
      {
        pResultSet = new MysqlPreparedStatementResultSet(pMysqlStatement, true);
        if (pResultSet)
          pResultSet->SetEncoding(GetEncoding());
#if wxUSE_UNICODE
        //wxPrintf(_("Allocating statement at %d\n"), pMysqlStatement);
       // m_ResultSets[pResultSet] = pMysqlStatement;
#endif
        LogResultSetForCleanup(pResultSet);
        return pResultSet;
      }

      mysql_stmt_free_result(pMysqlStatement);
      mysql_stmt_close(pMysqlStatement);
    }
    else
    {
      SetErrorCode(MysqlDatabaseLayer::TranslateErrorCode(mysql_errno(m_pDatabase)));
      SetErrorMessage(ConvertFromUnicodeStream(mysql_error(m_pDatabase)));
      ThrowDatabaseException();
      return NULL;
    }
  }
  LogResultSetForCleanup(pResultSet);
  return pResultSet;
}

// PreparedStatement support
PreparedStatement* MysqlDatabaseLayer::PrepareStatement(const wxString& strQuery)
{
  ResetErrorCodes();

  wxArrayString QueryArray = ParseQueries(strQuery);

  wxArrayString::iterator start = QueryArray.begin();
  wxArrayString::iterator stop = QueryArray.end();

  MysqlPreparedStatement* pStatement = new MysqlPreparedStatement();
  if (pStatement)
    pStatement->SetEncoding(GetEncoding());
  while (start != stop)
  {
    MYSQL_STMT* pMysqlStatement = mysql_stmt_init(m_pDatabase);
    if (pMysqlStatement != NULL)
    {
      wxCharBuffer sqlBuffer = ConvertToUnicodeStream((*start));
      //puts(sqlBuffer);
      if (mysql_stmt_prepare(pMysqlStatement, sqlBuffer, GetEncodedStreamLength((*start))) == 0)
      {
        pStatement->AddPreparedStatement(pMysqlStatement);
      }
      else
      {
        SetErrorCode(MysqlDatabaseLayer::TranslateErrorCode(mysql_errno(m_pDatabase)));
        SetErrorMessage(ConvertFromUnicodeStream(mysql_error(m_pDatabase)));
        ThrowDatabaseException();
      }
    }
    else
    {
      SetErrorCode(MysqlDatabaseLayer::TranslateErrorCode(mysql_errno(m_pDatabase)));
      SetErrorMessage(ConvertFromUnicodeStream(mysql_error(m_pDatabase)));
      ThrowDatabaseException();
      return NULL;
    }
    start++;
  }
  LogStatementForCleanup(pStatement);
  return pStatement;
}

bool MysqlDatabaseLayer::TableExists(const wxString& table)
{
  bool bReturn = false;
/*  
  // This is the way that I'd prefer to retrieve the list of tables
  // Unfortunately MySQL returns both tables and view together
  // So we have to try a SQL call (which may be MySQL version dependent)
  wxCharBuffer tableBuffer = ConvertToUnicodeStream(table);
  MYSQL_RES* pResults = mysql_list_tables(m_pDatabase, tableBuffer);
  if (pResults != NULL)
  {
    MYSQL_ROW currentRow = NULL;
    while ((currentRow = mysql_fetch_row(pResults)) != NULL)
    {
      wxString strTable = ConvertFromUnicodeStream(currentRow[0]);
      if (strTable == table)
        bReturn = true;
    }
    mysql_free_result(pResults);
  }
*/
  // Keep these variables outside of scope so that we can clean them up
  //  in case of an error
  PreparedStatement* pStatement = NULL;
  DatabaseResultSet* pResult = NULL;

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    wxString query = _("SHOW TABLE STATUS WHERE Comment != 'VIEW' AND Name=?;");
    pStatement = PrepareStatement(query);
    if (pStatement)
    {
      pStatement->SetParamString(1, table);
      pResult = pStatement->ExecuteQuery();
      if (pResult)
      {
        if (pResult->Next())
        {
          wxString strTable = pResult->GetResultString(1);
          if (table == strTable)
            bReturn = true;
        }
      }
    }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  }
  catch (DatabaseLayerException& e)
  {
    if (pResult != NULL)
    {
      CloseResultSet(pResult);
      pResult = NULL;
    }

    if (pStatement != NULL)
    {
      CloseStatement(pStatement);
      pStatement = NULL;
    }

    throw e;
  }
#endif

  if (pResult != NULL)
  {
    CloseResultSet(pResult);
    pResult = NULL;
  }

  if (pStatement != NULL)
  {
    CloseStatement(pStatement);
    pStatement = NULL;
  }

  return bReturn;
}

bool MysqlDatabaseLayer::ViewExists(const wxString& view)
{
  bool bReturn = false;
/*  
  // This is the way that I'd prefer to retrieve the list of tables
  // Unfortunately MySQL returns both tables and view together
  // So we have to try a SQL call (which may be MySQL version dependent)
  wxCharBuffer viewBuffer = ConvertToUnicodeStream(view);
  MYSQL_RES* pResults = mysql_list_tables(m_pDatabase, viewBuffer);
  if (pResults != NULL)
  {
    MYSQL_ROW currentRow = NULL;
    while ((currentRow = mysql_fetch_row(pResults)) != NULL)
    {
      wxString strView = ConvertFromUnicodeStream(currentRow[0]);
      if (strView == view)
        bReturn = true;
    }
    mysql_free_result(pResults);
  }
*/
  // Keep these variables outside of scope so that we can clean them up
  //  in case of an error
  PreparedStatement* pStatement = NULL;
  DatabaseResultSet* pResult = NULL;

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    wxString query = _("SHOW TABLE STATUS WHERE Comment = 'VIEW' AND Name=?;");
    pStatement = PrepareStatement(query);
    if (pStatement)
    {
      pStatement->SetParamString(1, view);
      pResult = pStatement->ExecuteQuery();
      if (pResult)
      {
        if (pResult->Next())
        {
          wxString strView = pResult->GetResultString(1);
          if (view == strView)
            bReturn = true;
        }
      }
    }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  }
  catch (DatabaseLayerException& e)
  {
    if (pResult != NULL)
    {
      CloseResultSet(pResult);
      pResult = NULL;
    }

    if (pStatement != NULL)
    {
      CloseStatement(pStatement);
      pStatement = NULL;
    }

    throw e;
  }
#endif

  if (pResult != NULL)
  {
    CloseResultSet(pResult);
    pResult = NULL;
  }

  if (pStatement != NULL)
  {
    CloseStatement(pStatement);
    pStatement = NULL;
  }

  return bReturn;
}

wxArrayString MysqlDatabaseLayer::GetTables()
{
  wxArrayString returnArray;

  if (mysql_get_server_version(m_pDatabase) >= 50010)
  {
    DatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
    try
    {
#endif
      wxString query = _("SHOW TABLE STATUS WHERE Comment != 'VIEW';");
      pResult = ExecuteQuery(query);

      while (pResult->Next())
      {
        wxString table = pResult->GetResultString(1).Trim();
        if (!table.IsEmpty())
          returnArray.Add(table);
      }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
    }
    catch (DatabaseLayerException& e)
    {
      if (pResult != NULL)
      {
        CloseResultSet(pResult);
        pResult = NULL;
      }

      throw e;
    }
#endif

    if (pResult != NULL)
    {
      CloseResultSet(pResult);
      pResult = NULL;
    }
  }

  // If no tables have been found, then try the MySQL API directly
  //  This may pick up VIEWS as well as tables unfortunately
  if (returnArray.Count() == 0)
  {
    MYSQL_RES* pResults = mysql_list_tables(m_pDatabase, NULL);
    if (pResults != NULL)
    {
      MYSQL_ROW currentRow = NULL;
      while ((currentRow = mysql_fetch_row(pResults)) != NULL)
      {
        wxString strTable = ConvertFromUnicodeStream(currentRow[0]);
        returnArray.Add(strTable);
      }
      mysql_free_result(pResults);
    }
  }

  return returnArray;
}

wxArrayString MysqlDatabaseLayer::GetViews()
{
  wxArrayString returnArray;

  if (mysql_get_server_version(m_pDatabase) >= 50010)
  {
    DatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
    try
    {
#endif
      wxString query = _("SHOW TABLE STATUS WHERE Comment = 'VIEW';");
      pResult = ExecuteQuery(query);

      while (pResult->Next())
      {
        returnArray.Add(pResult->GetResultString(1).Trim());
      }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
    }
    catch (DatabaseLayerException& e)
    {
      if (pResult != NULL)
      {
        CloseResultSet(pResult);
        pResult = NULL;
      }

      throw e;
    }
#endif

    if (pResult != NULL)
    {
      CloseResultSet(pResult);
      pResult = NULL;
    }
  }

  return returnArray;
}

wxArrayString MysqlDatabaseLayer::GetColumns(const wxString& table)
{
  wxArrayString returnArray;
  // Keep these variables outside of scope so that we can clean them up
  //  in case of an error
  DatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    wxString query = wxString::Format(_("SHOW COLUMNS FROM %s;"), table.c_str());
    pResult = ExecuteQuery(query);

    while (pResult->Next())
    {
      returnArray.Add(pResult->GetResultString(1).Trim());
    }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  }
  catch (DatabaseLayerException& e)
  {
    if (pResult != NULL)
    {
      CloseResultSet(pResult);
      pResult = NULL;
    }

    throw e;
  }
#endif

  if (pResult != NULL)
  {
    CloseResultSet(pResult);
    pResult = NULL;
  }

 
  return returnArray;
}

int MysqlDatabaseLayer::TranslateErrorCode(int nCode)
{
  // Ultimately, this will probably be a map of Mysql database error code values to DatabaseLayer values
  // For now though, we'll just return error
  return nCode;
  //return DATABASE_LAYER_ERROR;
}

