#ifndef __ODBC_INTERFACES_H__
#define __ODBC_INTERFACES_H__

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/dynlib.h"

#include <sql.h>
#include <sqlext.h>

typedef SQLRETURN (wxSTDCALL *SQLAllocHandleType)(SQLSMALLINT, SQLHANDLE, SQLHANDLE*);
typedef SQLRETURN (wxSTDCALL *SQLSetEnvAttrType)(SQLHENV, SQLINTEGER, SQLPOINTER, SQLINTEGER);
typedef SQLRETURN (wxSTDCALL *SQLFreeHandleType)(SQLSMALLINT, SQLHANDLE);
typedef SQLRETURN (wxSTDCALL *SQLConnectType)(SQLHDBC, SQLTCHAR*, SQLSMALLINT,
  SQLTCHAR*, SQLSMALLINT, SQLTCHAR*, SQLSMALLINT);
typedef SQLRETURN (wxSTDCALL *SQLDriverConnectType)(SQLHDBC, SQLHWND, SQLTCHAR*, 
  SQLSMALLINT, SQLTCHAR*, SQLSMALLINT, SQLSMALLINT*, SQLUSMALLINT);
typedef SQLRETURN (wxSTDCALL *SQLDisconnectType)(SQLHDBC);
typedef SQLRETURN (wxSTDCALL *SQLSetConnectAttrType)(SQLHDBC, SQLINTEGER, SQLPOINTER, SQLINTEGER);
typedef SQLRETURN (wxSTDCALL *SQLEndTranType)(SQLSMALLINT, SQLHANDLE, SQLSMALLINT);
typedef SQLRETURN (wxSTDCALL *SQLPrepareType)(SQLHSTMT, SQLTCHAR*, SQLINTEGER);
typedef SQLRETURN (wxSTDCALL *SQLFreeStmtType)(SQLHSTMT, SQLUSMALLINT);
typedef SQLRETURN (wxSTDCALL *SQLTablesType)(SQLHSTMT, SQLTCHAR*, SQLSMALLINT, SQLTCHAR*,
  SQLSMALLINT, SQLTCHAR*, SQLSMALLINT, SQLTCHAR*, SQLSMALLINT);
typedef SQLRETURN (wxSTDCALL *SQLFetchType)(SQLHSTMT);
typedef SQLRETURN (wxSTDCALL *SQLGetDataType)(SQLHSTMT, SQLUSMALLINT, SQLSMALLINT,
  SQLPOINTER, SQLLEN, SQLLEN*);
typedef SQLRETURN (wxSTDCALL *SQLColumnsType)(SQLHSTMT, SQLTCHAR*, SQLSMALLINT, SQLTCHAR*,
  SQLSMALLINT, SQLTCHAR*, SQLSMALLINT, SQLTCHAR*, SQLSMALLINT);
typedef SQLRETURN (wxSTDCALL *SQLGetDiagRecType)(SQLSMALLINT, SQLHANDLE, SQLSMALLINT, SQLTCHAR*,
  SQLINTEGER*, SQLTCHAR*, SQLSMALLINT, SQLSMALLINT*);
typedef SQLRETURN (wxSTDCALL *SQLNumParamsType)(SQLHSTMT, SQLSMALLINT*);
typedef SQLRETURN (wxSTDCALL *SQLExecuteType)(SQLHSTMT);
typedef SQLRETURN (wxSTDCALL *SQLParamDataType)(SQLHSTMT, SQLPOINTER*);
typedef SQLRETURN (wxSTDCALL *SQLPutDataType)(SQLHSTMT, SQLPOINTER, SQLLEN);
typedef SQLRETURN (wxSTDCALL *SQLRowCountType)(SQLHSTMT, SQLLEN*);
typedef SQLRETURN (wxSTDCALL *SQLNumResultColsType)(SQLHSTMT, SQLSMALLINT*);
typedef SQLRETURN (wxSTDCALL *SQLDescribeParamType)(SQLHSTMT, SQLUSMALLINT, SQLSMALLINT*,
  SQLULEN*, SQLSMALLINT*, SQLSMALLINT*);
typedef SQLRETURN (wxSTDCALL *SQLBindParameterType)(SQLHSTMT, SQLUSMALLINT, SQLSMALLINT,
  SQLSMALLINT, SQLSMALLINT, SQLULEN, SQLSMALLINT, SQLPOINTER, SQLLEN, SQLLEN*);
typedef SQLRETURN (wxSTDCALL *SQLColAttributesType)(SQLHSTMT, SQLUSMALLINT, SQLUSMALLINT,
  SQLPOINTER, SQLSMALLINT, SQLSMALLINT*, SQLLEN*);
typedef SQLRETURN (wxSTDCALL *SQLColAttributeType)(SQLHSTMT, SQLUSMALLINT, SQLUSMALLINT,
  SQLPOINTER, SQLSMALLINT, SQLSMALLINT*, SQLPOINTER);
typedef SQLRETURN (wxSTDCALL *SQLDescribeColType)(SQLHSTMT, SQLUSMALLINT, SQLTCHAR*, SQLSMALLINT,
  SQLSMALLINT*, SQLSMALLINT*, SQLULEN*, SQLSMALLINT*, SQLSMALLINT*);


class OdbcInterface {
public:
  OdbcInterface() { }
  bool Init();

  SQLAllocHandleType GetSQLAllocHandle() { return m_pSQLAllocHandle; }
  SQLSetEnvAttrType GetSQLSetEnvAttr() { return m_pSQLSetEnvAttr; }
  SQLFreeHandleType GetSQLFreeHandle() { return m_pSQLFreeHandle; }
  SQLConnectType GetSQLConnect() { return m_pSQLConnect; }
  SQLDriverConnectType GetSQLDriverConnect() { return m_pSQLDriverConnect; }
  SQLDisconnectType GetSQLDisconnect() { return m_pSQLDisconnect; }
  SQLSetConnectAttrType GetSQLSetConnectAttr() { return m_pSQLSetConnectAttr; }
  SQLEndTranType GetSQLEndTran() { return m_pSQLEndTran; }
  SQLPrepareType GetSQLPrepare() { return m_pSQLPrepare; }
  SQLFreeStmtType GetSQLFreeStmt() { return m_pSQLFreeStmt; }
  SQLTablesType GetSQLTables() { return m_pSQLTables; }
  SQLFetchType GetSQLFetch() { return m_pSQLFetch; }
  SQLGetDataType GetSQLGetData() { return m_pSQLGetData; }
  SQLColumnsType GetSQLColumns() { return m_pSQLColumns; }
  SQLGetDiagRecType GetSQLGetDiagRec() { return m_pSQLGetDiagRec; }
  SQLNumParamsType GetSQLNumParams() { return m_pSQLNumParams; }
  SQLExecuteType GetSQLExecute() { return m_pSQLExecute; }
  SQLParamDataType GetSQLParamData() { return m_pSQLParamData; }
  SQLPutDataType GetSQLPutData() { return m_pSQLPutData; }
  SQLRowCountType GetSQLRowCount() { return m_pSQLRowCount; }
  SQLNumResultColsType GetSQLNumResultCols() { return m_pSQLNumResultCols; }
  SQLDescribeParamType GetSQLDescribeParam() { return m_pSQLDescribeParam; }
  SQLBindParameterType GetSQLBindParameter() { return m_pSQLBindParameter; }
  SQLColAttributesType GetSQLColAttributes() { return m_pSQLColAttributes; }
  SQLColAttributeType GetSQLColAttribute() { return m_pSQLColAttribute; }
  SQLDescribeColType GetSQLDescribeCol() { return m_pSQLDescribeCol; }


private:
  wxDynamicLibrary m_OdbcDLL;

  SQLAllocHandleType m_pSQLAllocHandle;
  SQLSetEnvAttrType m_pSQLSetEnvAttr;
  SQLFreeHandleType m_pSQLFreeHandle;
  SQLConnectType m_pSQLConnect;
  SQLDriverConnectType m_pSQLDriverConnect;
  SQLDisconnectType m_pSQLDisconnect;
  SQLSetConnectAttrType m_pSQLSetConnectAttr;
  SQLEndTranType m_pSQLEndTran;
  SQLPrepareType m_pSQLPrepare;
  SQLFreeStmtType m_pSQLFreeStmt;
  SQLTablesType m_pSQLTables;
  SQLFetchType m_pSQLFetch;
  SQLGetDataType m_pSQLGetData;
  SQLColumnsType m_pSQLColumns;
  SQLGetDiagRecType m_pSQLGetDiagRec;
  SQLNumParamsType m_pSQLNumParams;
  SQLExecuteType m_pSQLExecute;
  SQLParamDataType m_pSQLParamData;
  SQLPutDataType m_pSQLPutData;
  SQLRowCountType m_pSQLRowCount;
  SQLNumResultColsType m_pSQLNumResultCols;
  SQLDescribeParamType m_pSQLDescribeParam;
  SQLBindParameterType m_pSQLBindParameter;
  SQLColAttributesType m_pSQLColAttributes;
  SQLColAttributeType m_pSQLColAttribute;
  SQLDescribeColType m_pSQLDescribeCol;
};

#endif // __ODBC_INTERFACES_H__
