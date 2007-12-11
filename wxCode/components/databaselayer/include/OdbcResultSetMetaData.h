#ifndef __ODBC_RESULT_SET_METADATA_H__
#define __ODBC_RESULT_SET_METADATA_H__

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "ResultSetMetaData.h"

#include <sql.h>
#include <sqlext.h>

class OdbcResultSetMetaData : public ResultSetMetaData
{
public:
  // ctor
  OdbcResultSetMetaData(SQLHSTMT sqlOdbcStatement);

  // dtor
  virtual ~OdbcResultSetMetaData() { }

  virtual int GetColumnType(int i);
  virtual int GetColumnSize(int i);
  virtual wxString GetColumnName(int i);
  virtual int GetColumnCount();
  
private:

  SQLHSTMT m_pOdbcStatement;
};

#endif // __ODBC_RESULT_SET_METADATA_H__
