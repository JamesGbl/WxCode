#include "../include/OdbcDatabaseLayer.h"
#include "../include/OdbcPreparedStatement.h"
#include "../include/OdbcResultSet.h"
#include "../include/DatabaseErrorCodes.h"


// ctor()
OdbcDatabaseLayer::OdbcDatabaseLayer()
 : DatabaseLayer()
{
   m_bIsConnected = false;
   ResetErrorCodes();

   SQLRETURN nRet = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_sqlEnvHandle);
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }

   nRet = SQLSetEnvAttr(m_sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }

   nRet = SQLAllocHandle(SQL_HANDLE_DBC, m_sqlEnvHandle, &m_sqlHDBC);
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }

   m_sqlStatementHandle = NULL;

   m_strDSN = wxEmptyString;
   m_strUser = wxEmptyString;
   m_strPassword = wxEmptyString;
   m_strConnection = wxEmptyString;
#if wxUSE_GUI
   m_bPrompt = false;
   m_pParent = NULL;
#endif
}

OdbcDatabaseLayer::~OdbcDatabaseLayer()
{
   Close();

   SQLRETURN nRet = SQLFreeHandle(SQL_HANDLE_DBC, m_sqlHDBC);
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }

   nRet = SQLFreeHandle(SQL_HANDLE_ENV, m_sqlEnvHandle);
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }
}

bool OdbcDatabaseLayer::Open( )
{
   ResetErrorCodes();

   if ( !m_strDSN.IsEmpty() )
   {
     //wxCharBuffer dsnCharBuffer = ConvertToUnicodeStream(m_strDSN);
     //wxCharBuffer userCharBuffer = ConvertToUnicodeStream(m_strUser);
     //wxCharBuffer passwordCharBuffer = ConvertToUnicodeStream(m_strPassword);
     void* dsnCharBuffer = (void*)m_strDSN.c_str();
     void* userCharBuffer = (void*)m_strUser.c_str();
     void* passwordCharBuffer = (void*)m_strPassword.c_str();
     
     SQLRETURN nRet;
     nRet = SQLConnect(m_sqlHDBC, (SQLTCHAR FAR*)(const char*)dsnCharBuffer,
                SQL_NTS, (SQLTCHAR FAR*)(const char*)userCharBuffer, SQL_NTS,
                (SQLTCHAR FAR*)(const char*)passwordCharBuffer, SQL_NTS);
     if ( nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO )
     {
       InterpretErrorCodes( nRet );
       ThrowDatabaseException();
     }
   }
   else if ( !m_strConnection.IsEmpty() )
   {
     SQLTCHAR buff[8192];
     SQLSMALLINT iLen;

     memset(buff, 0, 8192*sizeof(SQLTCHAR));
     
     //wxCharBuffer connectionCharBuffer = ConvertToUnicodeStream(m_strConnection);
     void* connectionCharBuffer = (void*)m_strConnection.c_str();
#if wxUSE_GUI
     SQLRETURN nRet = SQLDriverConnect(m_sqlHDBC, m_pParent ? (SQLHWND)m_pParent->GetHandle() : NULL, (SQLTCHAR*)(const char*)connectionCharBuffer,
         (SQLSMALLINT)m_strConnection.Length(), (SQLTCHAR*)buff, 8192, &iLen, m_bPrompt ? SQL_DRIVER_PROMPT : SQL_DRIVER_NOPROMPT);
#else
     SQLRETURN nRet = SQLDriverConnect(m_sqlHDBC, NULL, (SQLTCHAR*)(const char*)connectionCharBuffer,
         (SQLSMALLINT)m_strConnection.Length(), (SQLTCHAR*)buff, 8192, &iLen, SQL_DRIVER_NOPROMPT);
#endif

     if ( nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO )
     {
       InterpretErrorCodes( nRet );
       ThrowDatabaseException();
     }
   }
   else
   {
     return false;
   }

   m_bIsConnected = true;    
        
   return true;
}

bool OdbcDatabaseLayer::Open( const wxString& strConnection )
{
    m_strDSN = wxEmptyString;
    m_strUser = wxEmptyString;
    m_strPassword = wxEmptyString;
    m_strConnection = strConnection;
#if wxUSE_GUI
    m_bPrompt = false;
    m_pParent = NULL;
#endif

    return Open();
}

#if wxUSE_GUI
bool OdbcDatabaseLayer::Open(const wxString& strConnection, bool bPromptForInfo, wxWindow* parent)
{
    m_strConnection = strConnection;
    m_bPrompt = bPromptForInfo;
    m_pParent = parent;
    m_strDSN = wxEmptyString;
    m_strUser = wxEmptyString;
    m_strPassword = wxEmptyString;

    return Open();
}
#endif

bool OdbcDatabaseLayer::Open( const wxString& strDSN, const wxString& strUser, const wxString& strPassword )
{
    m_strDSN = strDSN;
    m_strUser = strUser;
    m_strPassword = strPassword;
    m_strConnection = wxEmptyString;
#if wxUSE_GUI
    m_bPrompt = false;
    m_pParent = NULL;
#endif
        
    return Open();
}

bool OdbcDatabaseLayer::Close()
{
   ResetErrorCodes();

   CloseResultSets();
   CloseStatements();

   if (m_bIsConnected) 
   {
      SQLRETURN nRet = SQLDisconnect(m_sqlHDBC);
      if ( nRet != SQL_SUCCESS )
      {
        InterpretErrorCodes( nRet );
        ThrowDatabaseException();
      }

      m_bIsConnected=false;
   }

   return true;
}

bool OdbcDatabaseLayer::IsOpen()
{
  return m_bIsConnected;
}

void OdbcDatabaseLayer::BeginTransaction()
{
   ResetErrorCodes();

   SQLRETURN nRet = SQLSetConnectAttr(m_sqlHDBC, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, 0);
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }
}

void OdbcDatabaseLayer::Commit()
{
   ResetErrorCodes();

   SQLRETURN nRet = SQLEndTran(SQL_HANDLE_DBC, m_sqlHDBC, SQL_COMMIT);
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }

   nRet = SQLSetConnectAttr(m_sqlHDBC, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_IS_INTEGER);
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }
}

void OdbcDatabaseLayer::RollBack()
{
   ResetErrorCodes();

   SQLRETURN nRet = SQLEndTran(SQL_HANDLE_DBC, m_sqlHDBC, SQL_ROLLBACK);
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }
   
   nRet = SQLSetConnectAttr(m_sqlHDBC, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_IS_INTEGER);
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }
}

int OdbcDatabaseLayer::RunQuery( const wxString& strQuery, bool bParseQuery )
{
   ResetErrorCodes();

   //wxPrintf("Running: '%s'\n", strQuery.c_str());
   OdbcPreparedStatement* pStatement = (OdbcPreparedStatement*)PrepareStatement( strQuery, bParseQuery );

   if ( pStatement )
   {
     try
     {
       int nRows = pStatement->RunQuery();
       wxDELETE( pStatement );
       return nRows;
     }
     catch (...)
     {
   puts("5");
       SetErrorCode(pStatement->GetErrorCode());
       SetErrorMessage(pStatement->GetErrorMessage());
       wxDELETE( pStatement );
       ThrowDatabaseException();
       return DATABASE_LAYER_QUERY_RESULT_ERROR;
     }
   }
   else
     return DATABASE_LAYER_QUERY_RESULT_ERROR;
}

DatabaseResultSet* OdbcDatabaseLayer::RunQueryWithResults(const wxString& strQuery)
{
   ResetErrorCodes();

   OdbcPreparedStatement* pStatement = (OdbcPreparedStatement*)PrepareStatement( strQuery, true );

   if ( pStatement )
   {
     try
     {
       pStatement->SetOneTimer(true);
       DatabaseResultSet* pResults = pStatement->RunQueryWithResults(false /*false for "Don't log this result set for cleanup*/);
       LogResultSetForCleanup(pResults);
       return pResults;
     }
     catch (...)
     {
       SetErrorCode(pStatement->GetErrorCode());
       SetErrorMessage(pStatement->GetErrorMessage());
       wxDELETE( pStatement );
       ThrowDatabaseException();
       return NULL;
     }
   }
   else
     return NULL;
}

SQLHANDLE OdbcDatabaseLayer::allocStmth()
{
    ResetErrorCodes();

    SQLHANDLE handle = NULL;
        
    SQLRETURN nRet = SQLAllocHandle (SQL_HANDLE_STMT, m_sqlHDBC, &handle);
    if ( nRet != SQL_SUCCESS )
    {
        InterpretErrorCodes( nRet );
        ThrowDatabaseException();
    }
    return handle;
}

PreparedStatement* OdbcDatabaseLayer::PrepareStatement( const wxString& strQuery )
{
  PreparedStatement* pStatement = PrepareStatement(strQuery, true);
  LogStatementForCleanup(pStatement);
  return pStatement;
}

PreparedStatement* OdbcDatabaseLayer::PrepareStatement( const wxString& strQuery, bool bParseQuery )
{
    ResetErrorCodes();

    wxArrayString QueryArray;
    if (bParseQuery)
      QueryArray = ParseQueries(strQuery);
    else
      QueryArray.push_back(strQuery);

    OdbcPreparedStatement* pReturnStatement = new OdbcPreparedStatement(m_sqlEnvHandle, m_sqlHDBC);

    if (pReturnStatement)
        pReturnStatement->SetEncoding(GetEncoding());

    for (unsigned int i=0; i<(QueryArray.size()); i++)
    {
#if wxUSE_UNICODE
        void* sqlBuffer = (void*)(QueryArray[i].c_str());
#else
        wxCharBuffer sqlBuffer = ConvertToUnicodeStream(QueryArray[i]);
#endif
        //wxPrintf(_("Preparing statement: '%s'\n"), sqlBuffer);

        SQLHSTMT pSqlStatement = allocStmth();
        SQLRETURN nRet = SQLPrepare(pSqlStatement, (SQLTCHAR*)(const char*)sqlBuffer, SQL_NTS);
        if ( nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO )
        {
            InterpretErrorCodes( nRet );
            SQLFreeStmt(pSqlStatement, SQL_CLOSE);
            ThrowDatabaseException();
            return NULL;
        }

        if ( pSqlStatement )
            pReturnStatement->AddPreparedStatement( pSqlStatement );
    }

    return pReturnStatement;
}

bool OdbcDatabaseLayer::TableExists(const wxString& table)
{
  bool bReturn = false;
  // Use SQLTables
  SQLHSTMT pStatement = allocStmth();
  wxCharBuffer tableBuffer = ConvertToUnicodeStream(table);
  wxString tableType = _("TABLE");
  wxCharBuffer tableTypeBuffer = ConvertToUnicodeStream(tableType);
  int tableTypeBufferLength = GetEncodedStreamLength(tableType);
  SQLRETURN nRet = SQLTables(pStatement,
      NULL, 0,
      NULL, 0,
      (SQLTCHAR*)(const char*)tableBuffer, SQL_NTS,
      (SQLTCHAR*)(const char*)tableTypeBuffer, tableTypeBufferLength);

  if (nRet != SQL_SUCCESS)
  {
    InterpretErrorCodes( nRet );
    SQLFreeStmt(pStatement, SQL_CLOSE);
    ThrowDatabaseException();
    return false;
  }

  nRet = SQLFetch(pStatement);
  if (nRet == SQL_SUCCESS || nRet == SQL_SUCCESS_WITH_INFO)
    bReturn = true;

  SQLFreeStmt(pStatement, SQL_CLOSE);
  
  return bReturn;
}

bool OdbcDatabaseLayer::ViewExists(const wxString& view)
{
  bool bReturn = false;
  // Use SQLTables
  SQLHSTMT pStatement = allocStmth();
  wxCharBuffer viewBuffer = ConvertToUnicodeStream(view);
  wxString tableType = _("VIEW");
  wxCharBuffer tableTypeBuffer = ConvertToUnicodeStream(tableType);
  int tableTypeBufferLength = GetEncodedStreamLength(tableType);
  SQLRETURN nRet = SQLTables(pStatement,
      NULL, 0,
      NULL, 0,
      (SQLTCHAR*)(const char*)viewBuffer, SQL_NTS,
      (SQLTCHAR*)(const char*)tableTypeBuffer, tableTypeBufferLength);

  if (nRet != SQL_SUCCESS)
  {
    InterpretErrorCodes( nRet );
    SQLFreeStmt(pStatement, SQL_CLOSE);
    ThrowDatabaseException();
    return false;
  }

  nRet = SQLFetch(pStatement);
  if (nRet == SQL_SUCCESS || nRet == SQL_SUCCESS_WITH_INFO)
    bReturn = true;

  SQLFreeStmt(pStatement, SQL_CLOSE);
  
  return bReturn;
}

wxArrayString OdbcDatabaseLayer::GetTables()
{
  wxArrayString returnArray;
  SQLHSTMT pStatement = allocStmth();
  wxString tableType = _("TABLE");
  wxCharBuffer tableTypeBuffer = ConvertToUnicodeStream(tableType);
  int tableTypeBufferLength = GetEncodedStreamLength(tableType);
  SQLRETURN nRet = SQLTables(pStatement,
      NULL, 0,
      NULL, 0,
      NULL, 0,
      (SQLTCHAR*)(const char*)tableTypeBuffer, tableTypeBufferLength);

  if (nRet != SQL_SUCCESS)
  {
    InterpretErrorCodes( nRet );
    SQLFreeStmt(pStatement, SQL_CLOSE);
    ThrowDatabaseException();
    return returnArray;
  }

  nRet = SQLFetch(pStatement);
  while (nRet == SQL_SUCCESS || nRet == SQL_SUCCESS_WITH_INFO)
  {
    SQLTCHAR buff[8192];

    memset(buff, 0, 8192*sizeof(SQLTCHAR));

    SQLINTEGER  col_size         = 8192;
    SQLINTEGER  real_size        = 0;
    int nField = 3;

    SQLRETURN nGetDataReturn = SQLGetData( pStatement, nField, SQL_C_CHAR, buff,
      col_size, &real_size );
    if ( nGetDataReturn != SQL_SUCCESS && nGetDataReturn != SQL_SUCCESS_WITH_INFO )
    {
      InterpretErrorCodes(nRet);
      SQLFreeStmt(pStatement, SQL_CLOSE);
      ThrowDatabaseException();
      return returnArray;
    }
    wxString strTable = ConvertFromUnicodeStream((const char*)buff);
    returnArray.Add(strTable);
    nRet = SQLFetch(pStatement);
  }

  SQLFreeStmt(pStatement, SQL_CLOSE);
  
  return returnArray;
}

wxArrayString OdbcDatabaseLayer::GetViews()
{
  wxArrayString returnArray;
  SQLHSTMT pStatement = allocStmth();
  wxString tableType = _("VIEW");
  wxCharBuffer tableTypeBuffer = ConvertToUnicodeStream(tableType);
  int tableTypeBufferLength = GetEncodedStreamLength(tableType);
  SQLRETURN nRet = SQLTables(pStatement,
      NULL, 0,
      NULL, 0,
      NULL, 0,
      (SQLTCHAR*)(const char*)tableTypeBuffer, tableTypeBufferLength);

  if (nRet != SQL_SUCCESS)
  {
    InterpretErrorCodes( nRet );
    SQLFreeStmt(pStatement, SQL_CLOSE);
    ThrowDatabaseException();
    return returnArray;
  }

  nRet = SQLFetch(pStatement);
  while (nRet == SQL_SUCCESS || nRet == SQL_SUCCESS_WITH_INFO)
  {
    SQLTCHAR buff[8192];

    memset(buff, 0, 8192*sizeof(SQLTCHAR));

    SQLINTEGER  col_size         = 8192;
    SQLINTEGER  real_size        = 0;
    int nField = 3;

    SQLRETURN nGetDataReturn = SQLGetData( pStatement, nField, SQL_C_CHAR, buff,
      col_size, &real_size );
    if ( nGetDataReturn != SQL_SUCCESS && nGetDataReturn != SQL_SUCCESS_WITH_INFO )
    {
      InterpretErrorCodes(nRet);
      SQLFreeStmt(pStatement, SQL_CLOSE);
      ThrowDatabaseException();
      return returnArray;
    }
    wxString strView = ConvertFromUnicodeStream((const char*)buff);
    returnArray.Add(strView);
    nRet = SQLFetch(pStatement);
  }

  SQLFreeStmt(pStatement, SQL_CLOSE);
  
  return returnArray;
}

wxArrayString OdbcDatabaseLayer::GetColumns(const wxString& table)
{
  wxArrayString returnArray;
  // Use SQLColumns
  SQLHSTMT pStatement = allocStmth();
  wxCharBuffer tableBuffer = ConvertToUnicodeStream(table);
  int tableBufferLength = GetEncodedStreamLength(table);
  SQLRETURN nRet = SQLColumns(pStatement,
      NULL, 0,
      NULL, 0,
      (SQLTCHAR*)(const char*)tableBuffer, tableBufferLength,
      NULL, 0);

  if (nRet != SQL_SUCCESS)
  {
    InterpretErrorCodes( nRet );
    SQLFreeStmt(pStatement, SQL_CLOSE);
    ThrowDatabaseException();
    return returnArray;
  }

  nRet = SQLFetch(pStatement);
  while (nRet == SQL_SUCCESS || nRet == SQL_SUCCESS_WITH_INFO)
  {
    SQLTCHAR buff[8192];

    memset(buff, 0, 8192*sizeof(SQLTCHAR));

    SQLINTEGER  col_size         = 8192;
    SQLINTEGER  real_size        = 0;
    int nField = 4;

    SQLRETURN nGetDataReturn = SQLGetData( pStatement, nField, SQL_C_CHAR, buff,
      col_size, &real_size );
    if ( nGetDataReturn != SQL_SUCCESS && nGetDataReturn != SQL_SUCCESS_WITH_INFO )
    {
      InterpretErrorCodes(nRet);
      SQLFreeStmt(pStatement, SQL_CLOSE);
      ThrowDatabaseException();
      return returnArray;
    }
    wxString strColumn = ConvertFromUnicodeStream((const char*)buff);
    returnArray.Add(strColumn);
    nRet = SQLFetch(pStatement);
  }

  return returnArray;
}

void OdbcDatabaseLayer::InterpretErrorCodes( long nCode, SQLHSTMT stmth_ptr )
{
  wxLogDebug(_("OdbcDatabaseLayer::InterpretErrorCodes()\n"));

  //if ((nCode != SQL_SUCCESS) ) // && (nCode != SQL_SUCCESS_WITH_INFO))
  {
    SQLINTEGER iNativeCode;
    SQLTCHAR strState[ERR_STATE_LEN];
    SQLTCHAR strBuffer[ERR_BUFFER_LEN];
    SQLSMALLINT iMsgLen;

    memset(strState, 0, ERR_STATE_LEN*sizeof(SQLTCHAR));
    memset(strBuffer, 0, ERR_BUFFER_LEN*sizeof(SQLTCHAR));

    if (stmth_ptr)
      SQLGetDiagRec(SQL_HANDLE_STMT, stmth_ptr, 1, strState, &iNativeCode, 
        strBuffer, ERR_BUFFER_LEN, &iMsgLen);  
    else
      SQLGetDiagRec(SQL_HANDLE_DBC, m_sqlHDBC, 1, strState, &iNativeCode,
        strBuffer, ERR_BUFFER_LEN, &iMsgLen);  
 
    SetErrorCode((int)iNativeCode);
    //SetErrorMessage(ConvertFromUnicodeStream((char*)strBuffer));
    SetErrorMessage(wxString((wxChar*)strBuffer));
  }
}

