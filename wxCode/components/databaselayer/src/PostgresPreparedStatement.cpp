#include "../include/PostgresPreparedStatement.h"
#include "../include/PostgresDatabaseLayer.h"
#include "../include/DatabaseErrorCodes.h"

#ifdef __cplusplus 
extern "C" {
#endif
#include "server/mb/pg_wchar.h"
#ifdef __cplusplus 
}
#endif

#include "wx/tokenzr.h"

#include "wx/arrimpl.cpp"
WX_DEFINE_OBJARRAY(ArrayOfPostgresPreparedStatementWrappers);

PostgresPreparedStatement::PostgresPreparedStatement()
 : PreparedStatement()
{
}

PostgresPreparedStatement::PostgresPreparedStatement(PGconn* pDatabase, const wxString& strSQL, const wxString& strStatementName)
 : PreparedStatement()
{
  AddStatement(pDatabase, strSQL, strStatementName);
}


PostgresPreparedStatement::~PostgresPreparedStatement()
{
  Close();
}


void PostgresPreparedStatement::Close()
{
  CloseResultSets();
  m_Statements.Clear();
}

void PostgresPreparedStatement::AddStatement(PGconn* pDatabase, const wxString& strSQL, const wxString& strStatementName)
{
  PostgresPreparedStatementWrapper Statement(pDatabase, strSQL, strStatementName);
  Statement.SetEncoding(GetEncoding());
  m_Statements.push_back(Statement);
}

PostgresPreparedStatement* PostgresPreparedStatement::CreateStatement(PGconn* pDatabase, const wxString& strSQL)
{
  wxArrayString Queries = ParseQueries(strSQL);

  wxArrayString::iterator start = Queries.begin();
  wxArrayString::iterator stop = Queries.end();

  PostgresPreparedStatement* pStatement = new PostgresPreparedStatement();
  const char* strEncoding = pg_encoding_to_char(PQclientEncoding(pDatabase));
  wxCSConv conv((const wxChar*)strEncoding);
  pStatement->SetEncoding(&conv);
  while (start != stop)
  {
    wxString strName = PostgresPreparedStatement::GenerateRandomStatementName();
    pStatement->AddStatement(pDatabase, (*start), strName);
    wxCharBuffer nameBuffer = DatabaseStringConverter::ConvertToUnicodeStream(strName, strEncoding);
    wxCharBuffer sqlBuffer = DatabaseStringConverter::ConvertToUnicodeStream(TranslateSQL((*start)), strEncoding);
    PGresult* pResult = PQprepare(pDatabase, nameBuffer, sqlBuffer, 0, NULL);
    if (pResult == NULL)
    {
      delete pStatement;
      return NULL;
    }

    if (PQresultStatus(pResult) != PGRES_COMMAND_OK)
    {
      pStatement->SetErrorCode(PostgresDatabaseLayer::TranslateErrorCode(PQresultStatus(pResult)));
      pStatement->SetErrorMessage(DatabaseStringConverter::ConvertFromUnicodeStream(PQresultErrorMessage(pResult), strEncoding));
      PQclear(pResult);
      pStatement->ThrowDatabaseException();
      return pStatement;
    }
    PQclear(pResult);

    start++;
  }
    
  // Success?  Return the statement
  return pStatement;
}

// set field
void PostgresPreparedStatement::SetParamInt(int nPosition, int nValue)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex].SetParam(nPosition, nValue);
  }
}

void PostgresPreparedStatement::SetParamDouble(int nPosition, double dblValue)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex].SetParam(nPosition, dblValue);
  }
}

void PostgresPreparedStatement::SetParamString(int nPosition, const wxString& strValue)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex].SetParam(nPosition, strValue);
  }
}

void PostgresPreparedStatement::SetParamNull(int nPosition)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex].SetParam(nPosition);
  }
}

void PostgresPreparedStatement::SetParamBlob(int nPosition, const void* pData, long nDataLength)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex].SetParam(nPosition, pData, nDataLength);
  }
}

void PostgresPreparedStatement::SetParamDate(int nPosition, const wxDateTime& dateValue)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex].SetParam(nPosition, dateValue);
  }
}

void PostgresPreparedStatement::SetParamBool(int nPosition, bool bValue)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex].SetParam(nPosition, bValue);
  }
}

int PostgresPreparedStatement::GetParameterCount()
{
  int nParameters = 0;

  for (unsigned int i=0; i<(m_Statements.size()); i++)
  {
    nParameters += m_Statements[i].GetParameterCount();
  }
  return nParameters;
}

  
void PostgresPreparedStatement::RunQuery()
{
  // Iterate through the statements and have them run their queries
  for (unsigned int i=0; i<(m_Statements.size()); i++)
  {
    m_Statements[i].RunQuery();
    if (m_Statements[i].GetErrorCode() != DATABASE_LAYER_OK)
    {
      SetErrorCode(m_Statements[i].GetErrorCode());
      SetErrorMessage(m_Statements[i].GetErrorMessage());
      ThrowDatabaseException();
      return;
    }
  }
}

DatabaseResultSet* PostgresPreparedStatement::RunQueryWithResults()
{
  for (unsigned int i=0; i<(m_Statements.size()-1); i++)
  {
    m_Statements[i].RunQuery();
    if (m_Statements[i].GetErrorCode() != DATABASE_LAYER_OK)
    {
      SetErrorCode(m_Statements[i].GetErrorCode());
      SetErrorMessage(m_Statements[i].GetErrorMessage());
      ThrowDatabaseException();
      return NULL;
    }
  }
  PostgresPreparedStatementWrapper* pLastStatement = &(m_Statements[m_Statements.size()-1]);
  DatabaseResultSet* pResultSet = pLastStatement->RunQueryWithResults();
  if (pLastStatement->GetErrorCode() != DATABASE_LAYER_OK)
  {
    SetErrorCode(pLastStatement->GetErrorCode());
    SetErrorMessage(pLastStatement->GetErrorMessage());
    ThrowDatabaseException();
  }

  LogResultSetForCleanup(pResultSet);
  return pResultSet;
}

wxString PostgresPreparedStatement::GenerateRandomStatementName()
{
  // Just come up with a string prefixed with "databaselayer_" and 10 random characters
  wxString strReturn = _("databaselayer_");
  for (int i=0; i<10; i++)
  {
    strReturn << (int) (10.0*rand()/(RAND_MAX+1.0));
  }
  return strReturn;
}

int PostgresPreparedStatement::FindStatementAndAdjustPositionIndex(int* pPosition)
{
  if (m_Statements.size() == 0)
    return 0;
    
  // Go through all the elements in the vector
  // Get the number of parameters in each statement
  // Adjust the nPosition for the the broken up statements
  for (unsigned int i=0; i<m_Statements.size(); i++)
  {
    int nParametersInThisStatement = m_Statements[i].GetParameterCount();

    if (*pPosition > nParametersInThisStatement)
    {
      *pPosition -= nParametersInThisStatement;    // Decrement the position indicator by the number of parameters in this statement
    }
    else
    {
      // We're in the correct statement, return the index
      return i;
    }
  }
  return -1;
}

wxString PostgresPreparedStatement::TranslateSQL(const wxString& strOriginalSQL)
{
  int nParameterIndex = 1;
  wxString strReturn = strOriginalSQL;
  int nFound = strReturn.Replace(_("?"), wxString::Format(_("$%d"), nParameterIndex), false);
  while (nFound != 0)
  {
    nParameterIndex++;
    nFound = strReturn.Replace(_("?"), wxString::Format(_("$%d"), nParameterIndex), false);
  }
  return strReturn;
}

