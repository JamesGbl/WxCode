#include "../include/FirebirdPreparedStatementWrapper.h"
#include "../include/FirebirdDatabaseLayer.h"
#include "../include/DatabaseErrorCodes.h"
#include "../include/DatabaseLayerException.h"
#include "../include/FirebirdResultSet.h"

FirebirdPreparedStatementWrapper::FirebirdPreparedStatementWrapper(isc_db_handle pDatabase, isc_tr_handle pTransaction, const wxString& strSQL)
 : DatabaseErrorReporter()
{
  m_pDatabase = pDatabase;
  m_pTransaction = pTransaction;
  m_strSQL = strSQL;
  m_pStatement = NULL;
  m_pParameters = NULL;
  m_pParameterCollection = NULL;
  m_bManageStatement = true;
  m_bManageTransaction = false;
 
  Prepare();

  if (GetErrorCode() != DATABASE_LAYER_OK)
  {
    ThrowDatabaseException();
  } 

}

FirebirdPreparedStatementWrapper::~FirebirdPreparedStatementWrapper()
{
  ResetErrorCodes();

  if (m_pParameterCollection)
  {
    wxDELETE(m_pParameterCollection);
  }

  if (m_pStatement && m_bManageStatement)
  {
    int nReturn = isc_dsql_free_statement(m_Status, &m_pStatement, DSQL_drop);
    if (nReturn != 0)
    {
      wxLogError(_("Error calling isc_dsql_free_statement"));
      InterpretErrorCodes();
      ThrowDatabaseException();
    }
  }
}

void FirebirdPreparedStatementWrapper::Prepare(const wxString& strSQL)
{
  m_strSQL = strSQL;
  Prepare();
}

void FirebirdPreparedStatementWrapper::Prepare()
{
  ResetErrorCodes();

  int nReturn = isc_dsql_allocate_statement(m_Status, &m_pDatabase, &m_pStatement);
  if (nReturn != 0)
  {
    InterpretErrorCodes();
    ThrowDatabaseException();
    return;
  }
  m_pParameters = (XSQLDA*)malloc(XSQLDA_LENGTH(1));
  if (m_pParameters == NULL)
  {
    InterpretErrorCodes();
    ThrowDatabaseException();
    return;
  }
  m_pParameters->version = SQLDA_VERSION1;
  m_pParameters->sqln = 1;

  wxCharBuffer sqlBuffer = ConvertToUnicodeStream(m_strSQL);
  nReturn = isc_dsql_prepare(m_Status, &m_pTransaction, &m_pStatement, 0, (char*)(const char*)sqlBuffer, SQL_DIALECT_CURRENT, m_pParameters);
  if (nReturn != 0)
  {
    InterpretErrorCodes();
    ThrowDatabaseException();
    return;
  }

  nReturn = isc_dsql_describe_bind(m_Status, &m_pStatement, SQL_DIALECT_CURRENT, m_pParameters);
  if (nReturn != 0)
  {
    InterpretErrorCodes();
    ThrowDatabaseException();
    return;
  }

  if (m_pParameters->sqld > m_pParameters->sqln)
  {
    int nParameters = m_pParameters->sqld;
    free(m_pParameters);
    m_pParameters = (XSQLDA*)malloc(XSQLDA_LENGTH(nParameters));
    m_pParameters->version = SQLDA_VERSION1;
    m_pParameters->sqln = nParameters;
    nReturn = isc_dsql_describe_bind(m_Status, &m_pStatement, SQL_DIALECT_CURRENT, m_pParameters);
    if (nReturn != 0)
    {
      InterpretErrorCodes();
      ThrowDatabaseException();
      return;
    }
  }
  m_pParameterCollection = new FirebirdParameterCollection(m_pParameters);
  m_pParameterCollection->SetEncoding(GetEncoding());
}

// set field
void FirebirdPreparedStatementWrapper::SetParam(int nPosition, int nValue)
{
  m_pParameterCollection->SetParam(nPosition, nValue);
}

void FirebirdPreparedStatementWrapper::SetParam(int nPosition, double dblValue)
{
  m_pParameterCollection->SetParam(nPosition, dblValue);
}

void FirebirdPreparedStatementWrapper::SetParam(int nPosition, const wxString& strValue)
{
  m_pParameterCollection->SetParam(nPosition, strValue);
}

void FirebirdPreparedStatementWrapper::SetParam(int nPosition)
{
  m_pParameterCollection->SetParam(nPosition);
}

void FirebirdPreparedStatementWrapper::SetParam(int nPosition, const void* pData, long nDataLength)
{
  m_pParameterCollection->SetParam(nPosition, m_pDatabase, m_pTransaction, pData, nDataLength);
}

void FirebirdPreparedStatementWrapper::SetParam(int nPosition, const wxDateTime& dateValue)
{
  m_pParameterCollection->SetParam(nPosition, dateValue);
}

void FirebirdPreparedStatementWrapper::SetParam(int nPosition, bool bValue)
{
  m_pParameterCollection->SetParam(nPosition, bValue);
}

int FirebirdPreparedStatementWrapper::GetParameterCount()
{
  if (m_pParameters == NULL)
    return 0;
  else
    return m_pParameters->sqld;
}
  
void FirebirdPreparedStatementWrapper::RunQuery()
{
  ResetErrorCodes();

  // Blob ID values are invalidated between execute calls, so re-create any BLOB parameters now
  m_pParameterCollection->ResetBlobParameters();
  
  int nReturn = isc_dsql_execute(m_Status, &m_pTransaction, &m_pStatement, SQL_DIALECT_CURRENT, m_pParameters);
  if (nReturn != 0)
  {
    InterpretErrorCodes();
    ThrowDatabaseException();
  }
}

DatabaseResultSet* FirebirdPreparedStatementWrapper::RunQueryWithResults()
{
  ResetErrorCodes();

  XSQLDA* pOutputSqlda = (XSQLDA*)malloc(XSQLDA_LENGTH(1));
  pOutputSqlda->sqln = 1;
  pOutputSqlda->version = SQLDA_VERSION1;

  // Make sure that we have enough space allocated for the result set
  int nReturn = isc_dsql_describe(m_Status, &m_pStatement, SQL_DIALECT_CURRENT, pOutputSqlda);
  if (nReturn != 0)
  {
    free(pOutputSqlda);
    InterpretErrorCodes();
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
    nReturn = isc_dsql_describe(m_Status, &m_pStatement, SQL_DIALECT_CURRENT, pOutputSqlda);
    if (nReturn != 0)
    {
      free(pOutputSqlda);
      InterpretErrorCodes();
      ThrowDatabaseException();
      return NULL;
    }
  }

  //isc_tr_handle pQueryTransaction = NULL;
  //nReturn = isc_start_transaction(m_Status, &pQueryTransaction, 1, &m_pDatabase, 0 /*tpb_length*/, NULL/*tpb*/);
  /*if (nReturn != 0)
  {
    InterpretErrorCodes();
    ThrowDatabaseException();
    return NULL;
  }*/

  // Create the result set object
  FirebirdResultSet* pResultSet = new FirebirdResultSet(m_pDatabase, m_pTransaction, m_pStatement, pOutputSqlda);
  if (pResultSet)
    pResultSet->SetEncoding(GetEncoding());
  if (pResultSet->GetErrorCode() != DATABASE_LAYER_OK)
  {
    SetErrorCode(pResultSet->GetErrorCode());
    SetErrorMessage(pResultSet->GetErrorMessage());
    
    // Wrap the result set deletion in try/catch block if using exceptions.
    // We want to make sure the original error gets to the user
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
  
  // Blob ID values are invalidated between execute calls, so re-create any BLOB parameters now
  m_pParameterCollection->ResetBlobParameters();
  
  // Now execute the SQL
  //nReturn = isc_dsql_execute2(m_Status, &m_pTransaction, &m_pStatement, 1, m_pParameters, pOutputSqlda);
  nReturn = isc_dsql_execute(m_Status, &m_pTransaction, &m_pStatement, SQL_DIALECT_CURRENT, m_pParameters);
  if (nReturn != 0)
  {
    InterpretErrorCodes();

    // Wrap the result set deletion in try/catch block if using exceptions.
    //We want to make sure the isc_dsql_execute2 error gets to the user
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

  m_bManageStatement = true;
  m_bManageTransaction = false;

  return pResultSet;
}

bool FirebirdPreparedStatementWrapper::IsSelectQuery()
{
  wxString strLocalCopy = m_strSQL;
  strLocalCopy.Trim(false);
  strLocalCopy.MakeUpper();
  return strLocalCopy.StartsWith(_("SELECT "));
}

void FirebirdPreparedStatementWrapper::InterpretErrorCodes()
{
  wxLogError(_("FirebirdPreparesStatementWrapper::InterpretErrorCodes()\n"));

  long nSqlCode = isc_sqlcode(m_Status);
  SetErrorCode(FirebirdDatabaseLayer::TranslateErrorCode(nSqlCode));
  SetErrorMessage(FirebirdDatabaseLayer::TranslateErrorCodeToString(nSqlCode, m_Status));
}

