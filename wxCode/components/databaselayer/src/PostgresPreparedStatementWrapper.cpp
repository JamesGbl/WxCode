#include "../include/PostgresPreparedStatementWrapper.h"
#include "../include/PostgresResultSet.h"
#include "../include/PostgresDatabaseLayer.h"
#include "../include/DatabaseErrorCodes.h"

PostgresPreparedStatementWrapper::PostgresPreparedStatementWrapper(PGconn* pDatabase, const wxString& strSQL, const wxString& strStatementName)
 : DatabaseErrorReporter()
{
  m_pDatabase = pDatabase;
  m_strSQL = strSQL;
  m_strStatementName = strStatementName;
}

PostgresPreparedStatementWrapper::~PostgresPreparedStatementWrapper()
{
}

// set field
void PostgresPreparedStatementWrapper::SetParam(int nPosition, int nValue)
{
  m_Parameters.SetParam(nPosition, nValue);
}

void PostgresPreparedStatementWrapper::SetParam(int nPosition, double dblValue)
{
  m_Parameters.SetParam(nPosition, dblValue);
}

void PostgresPreparedStatementWrapper::SetParam(int nPosition, const wxString& strValue)
{
  m_Parameters.SetParam(nPosition, strValue);
}

void PostgresPreparedStatementWrapper::SetParam(int nPosition)
{
  m_Parameters.SetParam(nPosition);
}

void PostgresPreparedStatementWrapper::SetParam(int nPosition, const void* pData, long nDataLength)
{
  m_Parameters.SetParam(nPosition, pData, nDataLength);
}

void PostgresPreparedStatementWrapper::SetParam(int nPosition, const wxDateTime& dateValue)
{
  m_Parameters.SetParam(nPosition, dateValue);
}

void PostgresPreparedStatementWrapper::SetParam(int nPosition, bool bValue)
{
  m_Parameters.SetParam(nPosition, bValue);
}

int PostgresPreparedStatementWrapper::GetParameterCount()
{
  return m_strSQL.Freq('?');
}

int PostgresPreparedStatementWrapper::RunQuery()
{
  long nRows = -1;
  int nParameters = m_Parameters.GetSize();
  char** paramValues = m_Parameters.GetParamValues();
  int* paramLengths = m_Parameters.GetParamLengths();
  int* paramFormats = m_Parameters.GetParamFormats();
  int nResultFormat = 0; // 0 = text, 1 = binary (all or none on the result set, not column based)
  wxCharBuffer statementNameBuffer = ConvertToUnicodeStream(m_strStatementName);
  PGresult* pResult = PQexecPrepared(m_pDatabase, statementNameBuffer, nParameters, paramValues, paramLengths, paramFormats, nResultFormat);
  if (pResult != NULL)
  {
    ExecStatusType status = PQresultStatus(pResult);
    if ((status != PGRES_COMMAND_OK) && (status != PGRES_TUPLES_OK))
    {
      SetErrorCode(PostgresDatabaseLayer::TranslateErrorCode(status));
      SetErrorMessage(ConvertFromUnicodeStream(PQresultErrorMessage(pResult)));
    }

    if (GetErrorCode() == DATABASE_LAYER_OK)
    {
      wxString rowsAffected = ConvertFromUnicodeStream(PQcmdTuples(pResult));
      rowsAffected.ToLong(&nRows);
    }
    PQclear(pResult);
  }
  delete []paramValues;
  delete []paramLengths;
  delete []paramFormats;

  if (GetErrorCode() != DATABASE_LAYER_OK)
  {
    ThrowDatabaseException();
    return DATABASE_LAYER_QUERY_RESULT_ERROR;
  }

  return (int)nRows;
}

DatabaseResultSet* PostgresPreparedStatementWrapper::RunQueryWithResults()
{
  int nParameters = m_Parameters.GetSize();
  char** paramValues = m_Parameters.GetParamValues();
  int* paramLengths = m_Parameters.GetParamLengths();
  int* paramFormats = m_Parameters.GetParamFormats();
  int nResultFormat = 0; // 0 = text, 1 = binary (all or none on the result set, not column based)
  wxCharBuffer statementNameBuffer = ConvertToUnicodeStream(m_strStatementName);
  PGresult* pResult = PQexecPrepared(m_pDatabase, statementNameBuffer, nParameters, paramValues, paramLengths, paramFormats, nResultFormat);
  if (pResult != NULL)
  {
    ExecStatusType status = PQresultStatus(pResult);
    if ((status != PGRES_COMMAND_OK) && (status != PGRES_TUPLES_OK))
    {
      SetErrorCode(PostgresDatabaseLayer::TranslateErrorCode(status));
      SetErrorMessage(ConvertFromUnicodeStream(PQresultErrorMessage(pResult)));
    }
    else
    {
      delete []paramValues;
      delete []paramLengths;
      delete []paramFormats;

      PostgresResultSet* pResultSet = new PostgresResultSet(pResult);
      pResultSet->SetEncoding(GetEncoding());
      return pResultSet;
    }
    PQclear(pResult);
  }
  delete []paramValues;
  delete []paramLengths;
  delete []paramFormats;

  ThrowDatabaseException();

  return NULL;
}


