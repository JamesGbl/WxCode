#include "../include/PostgresDatabaseLayer.h"
#include "../include/PostgresResultSet.h"
#include "../include/PostgresPreparedStatement.h"
#include "../include/DatabaseErrorCodes.h"
#include "../include/DatabaseLayerException.h"

#ifdef __cplusplus 
extern "C" {
#endif
#include "server/mb/pg_wchar.h"
#ifdef __cplusplus 
}
#endif

// ctor
PostgresDatabaseLayer::PostgresDatabaseLayer()
 : DatabaseLayer()
{
  m_strServer = _("localhost");
  m_strUser = _("");
  m_strPassword = _("");
  m_strDatabase = _("");
  m_strPort = _("");
}

PostgresDatabaseLayer::PostgresDatabaseLayer(const wxString& strDatabase)
 : DatabaseLayer()
{
  m_strServer = _("localhost");
  m_strUser = _("");
  m_strPassword = _("");
  m_strPort = _("");

  Open(strDatabase);
}

PostgresDatabaseLayer::PostgresDatabaseLayer(const wxString& strServer, const wxString& strDatabase)
 : DatabaseLayer()
{
  m_strServer = strServer;
  m_strUser = _("");
  m_strPassword = _("");
  m_strPort = _("");

  Open(strDatabase);
}

PostgresDatabaseLayer::PostgresDatabaseLayer(const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
 : DatabaseLayer()
{
  m_strServer = _("localhost");
  m_strUser = strUser;
  m_strPassword = strPassword;
  m_strPort = _("");

  Open(strDatabase);
}

PostgresDatabaseLayer::PostgresDatabaseLayer(const wxString& strServer, const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
 : DatabaseLayer()
{
  m_strServer = strServer;
  m_strUser = strUser;
  m_strPassword = strPassword;
  m_strPort = _("");

  Open(strDatabase);
}

PostgresDatabaseLayer::PostgresDatabaseLayer(const wxString& strServer, int nPort, const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
 : DatabaseLayer()
{
  m_strServer = strServer;
  m_strUser = strUser;
  m_strPassword = strPassword;
  m_strPort = wxString::Format(_("%d"), nPort);

  Open(strDatabase);
}

// dtor
PostgresDatabaseLayer::~PostgresDatabaseLayer()
{
  Close();
}

// open database
bool PostgresDatabaseLayer::Open()
{
  ResetErrorCodes();

  wxCharBuffer serverCharBuffer;
  const char* pHost = NULL;
  wxCharBuffer pDatabaseBuffer = ConvertToUnicodeStream(m_strDatabase);
  const char* pDatabase = pDatabaseBuffer;
  wxCharBuffer userCharBuffer;
  const char* pUser = NULL;
  wxCharBuffer passwordCharBuffer;
  const char* pPassword = NULL;
  const char* pTty = NULL;
  const char* pOptions = NULL;
  wxCharBuffer portCharBuffer;
  const char* pPort = NULL;
  
  if (m_strServer != _("localhost") && m_strServer != _(""))
  {
    serverCharBuffer = ConvertToUnicodeStream(m_strServer);
    pHost = serverCharBuffer;
  }
  
  if (m_strUser != _(""))
  {
    userCharBuffer = ConvertToUnicodeStream(m_strUser);
    pUser = userCharBuffer;
  }

  if (m_strPassword != _(""))
  {
    passwordCharBuffer = ConvertToUnicodeStream(m_strPassword);
    pPassword = passwordCharBuffer;
  }

  if (m_strPort != _(""))
  {
    portCharBuffer = ConvertToUnicodeStream(m_strPort);
    pPort = portCharBuffer;
  }

  m_pDatabase = PQsetdbLogin(pHost, pPort, pOptions, pTty, pDatabase, pUser, pPassword);
  if (PQstatus(m_pDatabase) == CONNECTION_BAD)
  {
    SetErrorCode(PostgresDatabaseLayer::TranslateErrorCode(PQstatus(m_pDatabase)));
    SetErrorMessage(ConvertFromUnicodeStream(PQerrorMessage(m_pDatabase)));
    ThrowDatabaseException();
    return false;
  }

  PQsetClientEncoding(m_pDatabase, "UTF-8");
  wxCSConv conv((const wxChar*)(pg_encoding_to_char(PQclientEncoding(m_pDatabase))));
  SetEncoding(&conv);
  
  return true;
}

bool PostgresDatabaseLayer::Open(const wxString& strDatabase)
{
  m_strDatabase = strDatabase;
  return Open();
}

bool PostgresDatabaseLayer::Open(const wxString& strServer, const wxString& strDatabase)
{
  m_strServer = strServer;
  m_strUser = _("");
  m_strPassword = _("");
  m_strDatabase = strDatabase;
  m_strPort = _("");
  return Open();
}

bool PostgresDatabaseLayer::Open(const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
{
  m_strServer = _("localhost");
  m_strUser = strUser;
  m_strPassword = strPassword;
  m_strDatabase = strDatabase;
  m_strPort = _("");
  return Open();
}

bool PostgresDatabaseLayer::Open(const wxString& strServer, const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
{
  m_strServer = strServer;
  m_strUser = strUser;
  m_strPassword = strPassword;
  m_strDatabase = strDatabase;
  m_strPort = _("");
  return Open();
}

bool PostgresDatabaseLayer::Open(const wxString& strServer, int nPort, const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
{
  m_strServer = strServer;
  m_strUser = strUser;
  m_strPassword = strPassword;
  m_strDatabase = strDatabase;
  SetPort(nPort);
  return Open();
}

// close database
bool PostgresDatabaseLayer::Close()
{
  CloseResultSets();
  CloseStatements();
  
  if (m_pDatabase)
  {
    PQfinish(m_pDatabase);
    m_pDatabase = NULL;
  }

  return true;
}

void PostgresDatabaseLayer::SetPort(int nPort)
{
  m_strPort = wxString::Format(_("%d"), nPort);
}

bool PostgresDatabaseLayer::IsOpen()
{
  if (m_pDatabase)
    return (PQstatus(m_pDatabase) != CONNECTION_BAD);
  else
    return false;
}
  
// transaction support
void PostgresDatabaseLayer::BeginTransaction()
{
  RunQuery(_("BEGIN"), false);
}

void PostgresDatabaseLayer::Commit()
{
  RunQuery(_("COMMIT"), false);
}

void PostgresDatabaseLayer::RollBack()
{
  RunQuery(_("ROLLBACK"), false);
}

  
// query database
bool PostgresDatabaseLayer::RunQuery(const wxString& strQuery, bool WXUNUSED(bParseQuery))
{
  // PostgreSQL takes care of parsing the queries itself so bParseQuery is ignored

  ResetErrorCodes();

  wxCharBuffer sqlBuffer = ConvertToUnicodeStream(strQuery);
  PGresult* pResultCode = PQexec(m_pDatabase, sqlBuffer);
  if ((pResultCode == NULL) || (PQresultStatus(pResultCode) != PGRES_COMMAND_OK))
  {
    SetErrorCode(PostgresDatabaseLayer::TranslateErrorCode(PQresultStatus(pResultCode)));
    SetErrorMessage(ConvertFromUnicodeStream(PQerrorMessage(m_pDatabase)));
    PQclear(pResultCode);
    ThrowDatabaseException();
    return false;
  }
  else
  {
    PQclear(pResultCode);
    return true;
  }
}

DatabaseResultSet* PostgresDatabaseLayer::RunQueryWithResults(const wxString& strQuery)
{
  ResetErrorCodes();

  wxCharBuffer sqlBuffer = ConvertToUnicodeStream(strQuery);
  PGresult* pResultCode = PQexec(m_pDatabase, sqlBuffer);
  if ((pResultCode == NULL))
  {
    SetErrorCode(PostgresDatabaseLayer::TranslateErrorCode(PQstatus(m_pDatabase)));
    SetErrorMessage(ConvertFromUnicodeStream(PQerrorMessage(m_pDatabase)));
    PQclear(pResultCode);
    ThrowDatabaseException();
    return NULL;
  }
  else
  {
    PostgresResultSet* pResultSet = new PostgresResultSet(pResultCode);
    pResultSet->SetEncoding(GetEncoding());
    LogResultSetForCleanup(pResultSet);
    return pResultSet;
  }
}

// PreparedStatement support
PreparedStatement* PostgresDatabaseLayer::PrepareStatement(const wxString& strQuery)
{
  ResetErrorCodes();

  PostgresPreparedStatement* pStatement = PostgresPreparedStatement::CreateStatement(m_pDatabase, strQuery);
  LogStatementForCleanup(pStatement);
  return pStatement;
}

bool PostgresDatabaseLayer::TableExists(const wxString& table)
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
    wxString query = _("SELECT COUNT(*) FROM information_schema.tables WHERE table_type='BASE TABLE' AND table_name=?;");
    pStatement = PrepareStatement(query);
    if (pStatement)
    {
      pStatement->SetParamString(1, table);
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

bool PostgresDatabaseLayer::ViewExists(const wxString& view)
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
    wxString query = _("SELECT COUNT(*) FROM information_schema.tables WHERE table_type='VIEW' AND table_name=?;");
    pStatement = PrepareStatement(query);
    if (pStatement)
    {
      pStatement->SetParamString(1, view);
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

wxArrayString PostgresDatabaseLayer::GetTables()
{
  wxArrayString returnArray;

  DatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    wxString query = _("SELECT table_name FROM information_schema.tables WHERE table_type='BASE TABLE' AND table_schema='public';");
    pResult = ExecuteQuery(query);

    while (pResult->Next())
    {
      returnArray.Add(pResult->GetResultString(1));
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

wxArrayString PostgresDatabaseLayer::GetViews()
{
  wxArrayString returnArray;

  DatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    wxString query = _("SELECT table_name FROM information_schema.tables WHERE table_type='VIEW' AND table_schema='public';");
    pResult = ExecuteQuery(query);

    while (pResult->Next())
    {
      returnArray.Add(pResult->GetResultString(1));
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

wxArrayString PostgresDatabaseLayer::GetColumns(const wxString& table)
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
    wxString query = _("SELECT column_name FROM information_schema.columns WHERE table_name=? ORDER BY ordinal_position;");
    pStatement = PrepareStatement(query);
    if (pStatement)
    {
      pStatement->SetParamString(1, table);
      pResult = pStatement->ExecuteQuery();
      if (pResult)
      {
        while (pResult->Next())
        {
          returnArray.Add(pResult->GetResultString(1));
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

int PostgresDatabaseLayer::TranslateErrorCode(int nCode)
{
  // Ultimately, this will probably be a map of Postgresql database error code values to DatabaseLayer values
  // For now though, we'll just return error
  return nCode;
  //return DATABASE_LAYER_ERROR;
}

