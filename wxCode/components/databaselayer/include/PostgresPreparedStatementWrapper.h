#ifndef __POSTGRESQL_PREPARED_STATEMENT_WRAPPER_H__
#define __POSTGRESQL_PREPARED_STATEMENT_WRAPPER_H__

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "libpq-fe.h"

#include "PostgresPreparedStatementParameterCollection.h"
#include "DatabaseErrorReporter.h"
#include "DatabaseStringConverter.h"

class DatabaseResultSet;

class PostgresPreparedStatementWrapper : public DatabaseErrorReporter, public DatabaseStringConverter
{
public:
  // ctor
  PostgresPreparedStatementWrapper(PGconn* pDatabase, const wxString& strSQL, const wxString& strStatementName);

  // dtor
  virtual ~PostgresPreparedStatementWrapper();

  // set field
  void SetParam(int nPosition, int nValue);
  void SetParam(int nPosition, double dblValue);
  void SetParam(int nPosition, const wxString& strValue);
  void SetParam(int nPosition);
  void SetParam(int nPosition, const void* pData, long nDataLength);
  void SetParam(int nPosition, const wxDateTime& dateValue);
  void SetParam(int nPosition, bool bValue);
  int GetParameterCount();
  
  void RunQuery();
  DatabaseResultSet* RunQueryWithResults();

private:
  PGconn* m_pDatabase;
  wxString m_strSQL;
  wxString m_strStatementName;

  PostgresPreparedStatementParameterCollection m_Parameters;
};

#endif // __POSTGRESQL_PREPARED_STATEMENT_WRAPPER_H__

