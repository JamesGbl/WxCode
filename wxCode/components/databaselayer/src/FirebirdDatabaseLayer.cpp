#include "../include/FirebirdDatabaseLayer.h"
#include "../include/FirebirdResultSet.h"
#include "../include/DatabaseErrorCodes.h"
#include "../include/DatabaseLayerException.h"

#include "wx/tokenzr.h"
#include "wx/regex.h"

// ctor()
FirebirdDatabaseLayer::FirebirdDatabaseLayer()
 : DatabaseLayer()
{
  m_pDatabase = NULL;
  m_pTransaction = NULL;

  m_strServer = _("localhost");
  m_strDatabase = _("");
  m_strRole = wxEmptyString;
}

FirebirdDatabaseLayer::FirebirdDatabaseLayer(const wxString& strDatabase)
 : DatabaseLayer()
{
  m_pDatabase = NULL;
  m_pTransaction = NULL;

  m_strServer = _("localhost");
  m_strUser = _("");
  m_strPassword = _("");
  m_strRole = wxEmptyString;

  Open(strDatabase);
}

FirebirdDatabaseLayer::FirebirdDatabaseLayer(const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
 : DatabaseLayer()
{
  m_pDatabase = NULL;
  m_pTransaction = NULL;

  m_strServer = _("");  // assume embedded database in this case
  m_strUser = strUser;
  m_strPassword = strPassword;
  m_strRole = wxEmptyString;

  Open(strDatabase);
}

FirebirdDatabaseLayer::FirebirdDatabaseLayer(const wxString& strServer, const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
 : DatabaseLayer()
{
  m_pDatabase = NULL;
  m_pTransaction = NULL;

  m_strServer = strServer;
  m_strUser = strUser;
  m_strPassword = strPassword;
  m_strRole = wxEmptyString;

  Open(strDatabase);
}

FirebirdDatabaseLayer::FirebirdDatabaseLayer(const wxString& strServer, const wxString& strDatabase, const wxString& strUser, const wxString& strPassword, const wxString& strRole)
 : DatabaseLayer()
{
  m_pDatabase = NULL;
  m_pTransaction = NULL;

  m_strServer = strServer;
  m_strUser = strUser;
  m_strPassword = strPassword;
  m_strRole = strRole;

  Open(strDatabase);
}

// dtor()
FirebirdDatabaseLayer::~FirebirdDatabaseLayer()
{
  Close();
}

// open database
bool FirebirdDatabaseLayer::Open(const wxString& strDatabase)
{
  m_strDatabase = strDatabase;
  return Open();
}

bool FirebirdDatabaseLayer::Open(const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
{
  m_strUser = strUser;
  m_strPassword = strPassword;

  return Open(strDatabase);
}

bool FirebirdDatabaseLayer::Open(const wxString& strServer, const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
{
  m_strServer = strServer;
  m_strUser = strUser;
  m_strPassword = strPassword;

  return Open(strDatabase);
}

bool FirebirdDatabaseLayer::Open()
{
  ResetErrorCodes();

  // Set up the connection parameter string
  //int nParameterStringLength;
  //char szParameterString[512];
  //char* pParameterString = szParameterString;

  // Firebird accepts all the other ISO_8859 encoding names but ISO-8859-1 needs a little tweaking to be recognized
  //wxString encodingName = wxLocale::GetSystemEncodingName();
  
  //wxCharBuffer systemEncoding;
  //if (encodingName == wxT("ISO-8859-1"))
  // systemEncoding = "ISO8859_1";
  //else
  // systemEncoding = encodingName.mb_str(*wxConvCurrent);
  wxCharBuffer systemEncoding = "UTF-8";

  wxCSConv conv(_("UTF-8"));
  SetEncoding(&conv);
  
  //char* pDpb = new char(512);
  char* pDpb;
  short nDpbLength = 0;
  wxCharBuffer userCharBuffer = ConvertToUnicodeStream(m_strUser);
  wxCharBuffer passwordCharBuffer = ConvertToUnicodeStream(m_strPassword);
  wxCharBuffer roleCharBuffer = ConvertToUnicodeStream(m_strRole);
  
  pDpb = (char*)0;
  
  if (m_strRole == wxEmptyString)
  {
    isc_expand_dpb(&pDpb, &nDpbLength, isc_dpb_user_name, (const char*)userCharBuffer,
        isc_dpb_password, (const char*)passwordCharBuffer, isc_dpb_lc_ctype, (const char*)systemEncoding, NULL);
  }
  else
  {
    isc_expand_dpb(&pDpb, &nDpbLength, isc_dpb_user_name, (const char*)userCharBuffer,
        isc_dpb_password, (const char*)passwordCharBuffer, isc_dpb_lc_ctype, (const char*)systemEncoding, 
        isc_dpb_sql_role_name, (const char*)roleCharBuffer, NULL);
  }
    
  // Combine the server and databsae path strings to pass into the isc_attach_databse function
  wxString strDatabaseUrl;
  if (m_strServer.IsEmpty())
    strDatabaseUrl = m_strDatabase; // Embedded database, just supply the file name
  else
    strDatabaseUrl = m_strServer + _(":") + m_strDatabase;
 
  m_pDatabase = NULL;
  m_pTransaction = NULL;

  wxCharBuffer urlBuffer = ConvertToUnicodeStream(strDatabaseUrl);
  //int nReturn = isc_attach_database(m_Status, 0, urlBuffer, &m_pDatabase, nParameterStringLength, szParameterString);
  int nReturn = isc_attach_database(m_Status, 0, (char*)(const char*)urlBuffer, &m_pDatabase, nDpbLength, pDpb);
  if (nReturn != 0)
  {
    InterpretErrorCodes();
    ThrowDatabaseException();

    return false;
  }
  return true;
}

// close database  
bool FirebirdDatabaseLayer::Close()
{
  CloseResultSets();
  CloseStatements();

  if (m_pDatabase)
  {
    if (m_pTransaction)
    {
      isc_rollback_transaction(m_Status, &m_pTransaction);
      m_pTransaction = NULL;
    }

    int nReturn = isc_detach_database(m_Status, &m_pDatabase);
    m_pDatabase = NULL;
    if (nReturn != 0)
    {
      InterpretErrorCodes();
      ThrowDatabaseException();
      return false;
    }
  }

  return true;
}

bool FirebirdDatabaseLayer::IsOpen()
{
  return (m_pDatabase != NULL);
}

// transaction support
void FirebirdDatabaseLayer::BeginTransaction()
{
  ResetErrorCodes();

  wxLogDebug(_("Beginning transaction"));
  if (m_pDatabase)
  {
    m_pTransaction = 0L;
    int nReturn = isc_start_transaction(m_Status, &m_pTransaction, 1, &m_pDatabase, 0 /*tpb_length*/, NULL/*tpb*/);
    if (nReturn != 0)
    {
      InterpretErrorCodes();
      ThrowDatabaseException();
    }
  }
}

void FirebirdDatabaseLayer::Commit()
{
  ResetErrorCodes();

  wxLogDebug(_("Committing transaction"));
  if (m_pDatabase && m_pTransaction)
  {
    int nReturn = isc_commit_transaction(m_Status, &m_pTransaction);
    if (nReturn != 0)
    {
      InterpretErrorCodes();
      ThrowDatabaseException();
    }
    else
    {
      // We're done with the transaction, so set it to NULL so that we know that a new transaction must be started if we run any queries
      m_pTransaction = NULL;
    }
  }
}

void FirebirdDatabaseLayer::RollBack()
{
  ResetErrorCodes();

  wxLogDebug(_("Rolling back transaction"));
  if (m_pDatabase && m_pTransaction)
  {
    int nReturn = isc_rollback_transaction(m_Status, &m_pTransaction);
    if (nReturn != 0)
    {
      InterpretErrorCodes();
      ThrowDatabaseException();
    }
    else
    {
      // We're done with the transaction, so set it to NULL so that we know that a new transaction must be started if we run any queries
      m_pTransaction = NULL;
    }
  }
}

// query database
int FirebirdDatabaseLayer::RunQuery(const wxString& strQuery, bool bParseQuery)
{
  ResetErrorCodes();
  if (m_pDatabase != NULL)
  {
    wxCharBuffer sqlDebugBuffer = ConvertToUnicodeStream(strQuery);
    wxLogDebug(_("Running query: \"%s\"\n"), (const char*)sqlDebugBuffer);

    wxArrayString QueryArray;
    if (bParseQuery)
      QueryArray = ParseQueries(strQuery);
    else
      QueryArray.push_back(strQuery);

    wxArrayString::iterator start = QueryArray.begin();
    wxArrayString::iterator stop = QueryArray.end();

    int nRows = 1;
    if (QueryArray.size() > 0)
    {
      bool bQuickieTransaction = false;
    
      if (m_pTransaction == NULL)
      {
        // If there's no transaction is progress, run this as a quick one-timer transaction
        bQuickieTransaction = true;
      }

      if (bQuickieTransaction)
      {
        BeginTransaction();
        if (GetErrorCode() != DATABASE_LAYER_OK)
        {
          wxLogError(_("Unable to start transaction"));
          ThrowDatabaseException();
          return DATABASE_LAYER_QUERY_RESULT_ERROR;
        }
      }

      while (start != stop)
      {
        wxCharBuffer sqlBuffer = ConvertToUnicodeStream(*start);
        //int nReturn = isc_dsql_execute_immediate(m_Status, &m_pDatabase, &m_pTransaction, 0, (char*)(const char*)sqlBuffer, SQL_DIALECT_CURRENT, NULL);
        int nReturn = isc_dsql_execute_immediate(m_Status, &m_pDatabase, &m_pTransaction, GetEncodedStreamLength(*start), (char*)(const char*)sqlBuffer, SQL_DIALECT_CURRENT, NULL);
        if (nReturn != 0)
        {
          InterpretErrorCodes();
          // Manually try to rollback the transaction rather than calling the member RollBack function
          //  so that we can ignore the error messages
          isc_rollback_transaction(m_Status, &m_pTransaction);
          m_pTransaction = NULL;

          ThrowDatabaseException();
          return DATABASE_LAYER_QUERY_RESULT_ERROR;
        }
        start++;
      }

      if (bQuickieTransaction)
      {
        Commit();
        if (GetErrorCode() != DATABASE_LAYER_OK)
        {
          ThrowDatabaseException();
          return DATABASE_LAYER_QUERY_RESULT_ERROR;
        }
      }
    }

    return nRows;
  }
  else
  {
    wxLogError(_("Database handle is NULL"));
    return DATABASE_LAYER_QUERY_RESULT_ERROR;
  }
}

DatabaseResultSet* FirebirdDatabaseLayer::RunQueryWithResults(const wxString& strQuery)
{
  ResetErrorCodes();
  if (m_pDatabase != NULL)
  {
    wxCharBuffer sqlDebugBuffer = ConvertToUnicodeStream(strQuery);
    wxLogDebug(_("Running query: \"%s\""), (const char*)sqlDebugBuffer);
    
    wxArrayString QueryArray = ParseQueries(strQuery);

    if (QueryArray.size() > 0)
    {
      bool bQuickieTransaction = false;
    
      if (m_pTransaction == NULL)
      {
        // If there's no transaction is progress, run this as a quick one-timer transaction
        bQuickieTransaction = true;
      }

      if (QueryArray.size() > 1)
      {
        if (bQuickieTransaction)
        {
          BeginTransaction();
          if (GetErrorCode() != DATABASE_LAYER_OK)
          {
            wxLogError(_("Unable to start transaction"));
            ThrowDatabaseException();
            return NULL;
          }
        }
      
        // Assume that only the last statement in the array returns the result set
        for (unsigned int i=0; i<QueryArray.size()-1; i++)
        {
          RunQuery(QueryArray[i], false);
          if (GetErrorCode() != DATABASE_LAYER_OK)
          {
            ThrowDatabaseException();
            return NULL;
          }
        }
      
        // Now commit all the previous queries before calling the query that returns a result set
        if (bQuickieTransaction)
        {
          Commit();
          if (GetErrorCode() != DATABASE_LAYER_OK)
          {
            ThrowDatabaseException();
            return NULL;
          }
        }
      } // End check if there are more than one query in the array
      
      isc_tr_handle pQueryTransaction = NULL;
      bool bManageTransaction = false;
      if (bQuickieTransaction)
      {
        bManageTransaction = true;
        int nReturn = isc_start_transaction(m_Status, &pQueryTransaction, 1, &m_pDatabase, 0 /*tpb_length*/, NULL/*tpb*/);
        if (nReturn != 0)
        {
          InterpretErrorCodes();
          ThrowDatabaseException();
        }
      }
      else
      {
        pQueryTransaction = m_pTransaction;
      }
      
      isc_stmt_handle pStatement = NULL;
      int nReturn = isc_dsql_allocate_statement(m_Status, &m_pDatabase, &pStatement);
      if (nReturn != 0)
      {
        InterpretErrorCodes();

        // Manually try to rollback the transaction rather than calling the member RollBack function
        //  so that we can ignore the error messages
        isc_rollback_transaction(m_Status, &pQueryTransaction);

        ThrowDatabaseException();
        return NULL;
      }
      
      wxCharBuffer sqlBuffer = ConvertToUnicodeStream(QueryArray[QueryArray.size()-1]);
      nReturn = isc_dsql_prepare(m_Status, &pQueryTransaction, &pStatement, 0, (char*)(const char*)sqlBuffer, SQL_DIALECT_CURRENT, NULL);
      if (nReturn != 0)
      {
        InterpretErrorCodes();

        // Manually try to rollback the transaction rather than calling the member RollBack function
        //  so that we can ignore the error messages
        isc_rollback_transaction(m_Status, &pQueryTransaction);

        ThrowDatabaseException();
        return NULL;
      }

//--------------------------------------------------------------

      XSQLDA* pOutputSqlda = (XSQLDA*)malloc(XSQLDA_LENGTH(1));
      pOutputSqlda->sqln = 1;
      pOutputSqlda->version = SQLDA_VERSION1;

      // Make sure that we have enough space allocated for the result set
      nReturn = isc_dsql_describe(m_Status, &pStatement, SQL_DIALECT_CURRENT, pOutputSqlda);
      if (nReturn != 0)
      {
        free(pOutputSqlda);
        InterpretErrorCodes();

        // Manually try to rollback the transaction rather than calling the member RollBack function
        //  so that we can ignore the error messages
        isc_rollback_transaction(m_Status, &pQueryTransaction);

        ThrowDatabaseException();
        return NULL;
      }

      if (pOutputSqlda->sqld > pOutputSqlda->sqln)
      {
        int nColumns = pOutputSqlda->sqld;
        free(pOutputSqlda);
        pOutputSqlda = (XSQLDA*)malloc(XSQLDA_LENGTH(nColumns));
        pOutputSqlda->sqln = nColumns;
        pOutputSqlda->version = SQLDA_VERSION1;
        nReturn = isc_dsql_describe(m_Status, &pStatement, SQL_DIALECT_CURRENT, pOutputSqlda);
        if (nReturn != 0)
        {
          free(pOutputSqlda);
          InterpretErrorCodes();

          // Manually try to rollback the transaction rather than calling the member RollBack function
          //  so that we can ignore the error messages
          isc_rollback_transaction(m_Status, &pQueryTransaction);

          ThrowDatabaseException();
          return NULL;
        }
      }

      // Create the result set object
      FirebirdResultSet* pResultSet = new FirebirdResultSet(m_pDatabase, pQueryTransaction, pStatement, pOutputSqlda, true, bManageTransaction);
      pResultSet->SetEncoding(GetEncoding());
      if (pResultSet->GetErrorCode() != DATABASE_LAYER_OK)
      {
        SetErrorCode(pResultSet->GetErrorCode());
        SetErrorMessage(pResultSet->GetErrorMessage());
    
        // Manually try to rollback the transaction rather than calling the member RollBack function
        //  so that we can ignore the error messages
        isc_rollback_transaction(m_Status, &pQueryTransaction);

        // Wrap the result set deletion in try/catch block if using exceptions.
        //We want to make sure the original error gets to the user
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
        try
        {
#endif
        delete pResultSet;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
        }
        catch (DatabaseLayerException& e)
        {
        }
#endif

        ThrowDatabaseException();
      }
  
      // Now execute the SQL
      nReturn = isc_dsql_execute(m_Status, &pQueryTransaction, &pStatement, SQL_DIALECT_CURRENT, NULL);
      if (nReturn != 0)
      {
        InterpretErrorCodes();

        // Manually try to rollback the transaction rather than calling the member RollBack function
        //  so that we can ignore the error messages
        isc_rollback_transaction(m_Status, &pQueryTransaction);

        // Wrap the result set deletion in try/catch block if using exceptions.
        //  We want to make sure the isc_dsql_execute error gets to the user
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
        try
        {
#endif
        delete pResultSet;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
        }
        catch (DatabaseLayerException& e)
        {
        }
#endif
    
        ThrowDatabaseException();
        return NULL;
      }
      
//--------------------------------------------------------------

      LogResultSetForCleanup(pResultSet);
      return pResultSet;
    }
    else
      return NULL;
  }
  else
  {
    wxLogError(_("Database handle is NULL"));
    return NULL;
  }
}

PreparedStatement* FirebirdDatabaseLayer::PrepareStatement(const wxString& strQuery)
{
  ResetErrorCodes();
  
  FirebirdPreparedStatement* pStatement = FirebirdPreparedStatement::CreateStatement(m_pDatabase, m_pTransaction, strQuery, GetEncoding());
  if (pStatement && (pStatement->GetErrorCode() != DATABASE_LAYER_OK))
  {
    SetErrorCode(pStatement->GetErrorCode());
    SetErrorMessage(pStatement->GetErrorMessage());
    wxDELETE(pStatement); // This sets the pointer to NULL after deleting it

    ThrowDatabaseException();
    return NULL;
  }

  LogStatementForCleanup(pStatement);
  return pStatement;
}

bool FirebirdDatabaseLayer::TableExists(const wxString& table)
{
  // Initialize variables
  bool bReturn = false;
  // Keep these variables outside of scope so that we can clean them up
  //  in case of an error
  PreparedStatement* pStatement = NULL;
  DatabaseResultSet* pResult = NULL;

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    wxString tableUpperCase = table.Upper();
    wxString query = _("SELECT COUNT(*) FROM RDB$RELATIONS WHERE RDB$SYSTEM_FLAG=0 AND RDB$VIEW_BLR IS NULL AND RDB$RELATION_NAME=?;");
    pStatement = PrepareStatement(query);
    if (pStatement)
    {
      pStatement->SetParamString(1, tableUpperCase);
      pResult = pStatement->ExecuteQuery();
      if (pResult)
      {
        if (pResult->Next())
        {
          if(pResult->GetResultInt(1) != 0)
          {
            bReturn = true;
          }
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

bool FirebirdDatabaseLayer::ViewExists(const wxString& view)
{
  // Initialize variables
  bool bReturn = false;
  // Keep these variables outside of scope so that we can clean them up
  //  in case of an error
  PreparedStatement* pStatement = NULL;
  DatabaseResultSet* pResult = NULL;

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    wxString viewUpperCase = view.Upper();
    wxString query = _("SELECT COUNT(*) FROM RDB$RELATIONS WHERE RDB$SYSTEM_FLAG=0 AND RDB$VIEW_BLR IS NOT NULL AND RDB$RELATION_NAME=?;");
    pStatement = PrepareStatement(query);
    if (pStatement)
    {
      pStatement->SetParamString(1, viewUpperCase);
      pResult = pStatement->ExecuteQuery();
      if (pResult)
      {
        if (pResult->Next())
        {
          if(pResult->GetResultInt(1) != 0)
          {
            bReturn = true;
          }
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

wxArrayString FirebirdDatabaseLayer::GetTables()
{
  wxArrayString returnArray;

  DatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    wxString query = _("SELECT RDB$RELATION_NAME FROM RDB$RELATIONS WHERE RDB$SYSTEM_FLAG=0 AND RDB$VIEW_BLR IS NULL");
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

wxArrayString FirebirdDatabaseLayer::GetViews()
{
  wxArrayString returnArray;

  DatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    wxString query = _("SELECT RDB$RELATION_NAME FROM RDB$RELATIONS WHERE RDB$SYSTEM_FLAG=0 AND RDB$VIEW_BLR IS NOT NULL");
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

wxArrayString FirebirdDatabaseLayer::GetColumns(const wxString& table)
{
  // Initialize variables
  wxArrayString returnArray;
  // Keep these variables outside of scope so that we can clean them up
  //  in case of an error
  PreparedStatement* pStatement = NULL;
  DatabaseResultSet* pResult = NULL;

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    wxString tableUpperCase = table.Upper();
    wxString query = _("SELECT RDB$FIELD_NAME FROM RDB$RELATION_FIELDS WHERE RDB$RELATION_NAME=?;");
    pStatement = PrepareStatement(query);
    if (pStatement)
    {
      pStatement->SetParamString(1, tableUpperCase);
      pResult = pStatement->ExecuteQuery();
      if (pResult)
      {
        while (pResult->Next())
        {
          returnArray.Add(pResult->GetResultString(1).Trim());
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

  return returnArray;
}

int FirebirdDatabaseLayer::TranslateErrorCode(int nCode)
{
  // Ultimately, this will probably be a map of Firebird database error code values to DatabaseLayer values
  // For now though, we'll just return the original error code
  return nCode;
}

wxString FirebirdDatabaseLayer::TranslateErrorCodeToString(int nCode, ISC_STATUS_ARRAY status)
{
  char szError[512];
  wxString strReturn;
  if (nCode < -900)  // Error codes less than -900 indicate that it wasn't a SQL error but an ibase system error
  {
    long* pVector = (long*)status;
    fb_interpret(szError, 512, (const ISC_STATUS**)&pVector);
    //puts(szError);
    strReturn = wxString::Format(_("%s\n"), szError);
    while (fb_interpret(szError, 512, (const ISC_STATUS**)&pVector))
    {
      //puts(szError);
      strReturn += wxString::Format(_("%s\n"), szError);
    }
  }
  else
  {
    isc_sql_interprete(nCode, szError, sizeof(szError));
    wxCharBuffer systemEncoding = wxLocale::GetSystemEncodingName().mb_str(*wxConvCurrent);
    strReturn = DatabaseStringConverter::ConvertFromUnicodeStream(szError, (const char*)systemEncoding);
  }
  //puts(strReturn.mb_str(wxConvUTF8));
  return strReturn;
}

void FirebirdDatabaseLayer::InterpretErrorCodes()
{
  wxLogDebug(_("FirebirdDatabaseLayer::InterpretErrorCodes()"));

  long nSqlCode = isc_sqlcode(m_Status);
  SetErrorMessage(FirebirdDatabaseLayer::TranslateErrorCodeToString(nSqlCode, m_Status));
  if (nSqlCode < -900)  // Error codes less than -900 indicate that it wasn't a SQL error but an ibase system error
  {
    SetErrorCode(FirebirdDatabaseLayer::TranslateErrorCode(m_Status[1]));
  }
  else
  {
    SetErrorCode(FirebirdDatabaseLayer::TranslateErrorCode(nSqlCode));
  }
}

