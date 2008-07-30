#include "../include/MysqlPreparedStatementWrapper.h"
#include "../include/MysqlPreparedStatementResultSet.h"
#include "../include/MysqlDatabaseLayer.h"
#include "../include/DatabaseErrorCodes.h"

#include "errmsg.h"

MysqlPreparedStatementWrapper::MysqlPreparedStatementWrapper(MYSQL_STMT* pStatement)
 : DatabaseErrorReporter()
{
  m_pStatement = pStatement;
}

MysqlPreparedStatementWrapper::~MysqlPreparedStatementWrapper()
{
  Close();
}

void MysqlPreparedStatementWrapper::Close()
{
  if (m_pStatement != NULL)
  {
    mysql_stmt_close(m_pStatement);
    m_pStatement = NULL;
  }
}

// set field
void MysqlPreparedStatementWrapper::SetParam(int nPosition, int nValue)
{
  m_Parameters.SetParam(nPosition, nValue);
}

void MysqlPreparedStatementWrapper::SetParam(int nPosition, double dblValue)
{
  m_Parameters.SetParam(nPosition, dblValue);
}

void MysqlPreparedStatementWrapper::SetParam(int nPosition, const wxString& strValue)
{
  m_Parameters.SetParam(nPosition, strValue);
}

void MysqlPreparedStatementWrapper::SetParam(int nPosition)
{
  m_Parameters.SetParam(nPosition);
}

void MysqlPreparedStatementWrapper::SetParam(int nPosition, const void* pData, long nDataLength)
{
  m_Parameters.SetParam(nPosition, pData, nDataLength);
}

void MysqlPreparedStatementWrapper::SetParam(int nPosition, const wxDateTime& dateValue)
{
  m_Parameters.SetParam(nPosition, dateValue);
}

void MysqlPreparedStatementWrapper::SetParam(int nPosition, bool bValue)
{
  m_Parameters.SetParam(nPosition, bValue);
}

int MysqlPreparedStatementWrapper::GetParameterCount()
{
  return mysql_stmt_param_count(m_pStatement);
}

void MysqlPreparedStatementWrapper::RunQuery()
{
  MYSQL_BIND* pBoundParameters = m_Parameters.GetMysqlParameterBindings();

  int nBindReturn = mysql_stmt_bind_param(m_pStatement, pBoundParameters);
  if (nBindReturn != 0)
  {
    SetErrorCode(MysqlDatabaseLayer::TranslateErrorCode(mysql_stmt_errno(m_pStatement)));
    SetErrorMessage(ConvertFromUnicodeStream(mysql_stmt_error(m_pStatement)));
    if (pBoundParameters)
    {
      delete []pBoundParameters;
    }
    ThrowDatabaseException();
  }
  else
  {
    int nReturn = mysql_stmt_execute(m_pStatement);
    if (nReturn != 0)
    {
      SetErrorCode(MysqlDatabaseLayer::TranslateErrorCode(mysql_stmt_errno(m_pStatement)));
      SetErrorMessage(ConvertFromUnicodeStream(mysql_stmt_error(m_pStatement)));
      if (pBoundParameters)
      {
        delete []pBoundParameters;
      }
      ThrowDatabaseException();
    }
  }
  delete []pBoundParameters;
}

DatabaseResultSet* MysqlPreparedStatementWrapper::RunQueryWithResults()
{
  MysqlPreparedStatementResultSet* pResultSet = NULL;
  MYSQL_BIND* pBoundParameters = m_Parameters.GetMysqlParameterBindings();

  if (mysql_stmt_bind_param(m_pStatement, pBoundParameters))
  {
    SetErrorCode(MysqlDatabaseLayer::TranslateErrorCode(mysql_stmt_errno(m_pStatement)));
    SetErrorMessage(ConvertFromUnicodeStream(mysql_stmt_error(m_pStatement)));
    ThrowDatabaseException();
  }
  else
  {
    if (mysql_stmt_execute(m_pStatement) != 0)
    {
      SetErrorCode(MysqlDatabaseLayer::TranslateErrorCode(mysql_stmt_errno(m_pStatement)));
      SetErrorMessage(ConvertFromUnicodeStream(mysql_stmt_error(m_pStatement)));
      ThrowDatabaseException();
    }
    else
    {
      pResultSet = new MysqlPreparedStatementResultSet(m_pStatement);
      if (pResultSet)
        pResultSet->SetEncoding(GetEncoding());
    }
  }
  delete []pBoundParameters;
  
  return pResultSet;
}

