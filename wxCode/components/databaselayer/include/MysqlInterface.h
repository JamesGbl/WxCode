#ifndef __MYSQL_INTERFACES_H__
#define __MYSQL_INTERFACES_H__

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/dynlib.h"

#include "mysql.h"

typedef void (wxSTDCALL *MysqlServerEndType)(void);
typedef MYSQL* (wxSTDCALL *MysqlInitType)(MYSQL*);
typedef MYSQL* (wxSTDCALL *MysqlRealConnectType)(MYSQL*, const char*, const char*,
  const char*, const char*, unsigned int, const char*, unsigned long);
typedef int (wxSTDCALL *MysqlRealQueryType)(MYSQL*, const char*, unsigned long);
typedef const char* (wxSTDCALL *MysqlErrorType)(MYSQL*);
typedef unsigned int (wxSTDCALL *MysqlErrnoType)(MYSQL*);
typedef void (wxSTDCALL *MysqlCloseType)(MYSQL*);
typedef my_bool (wxSTDCALL *MysqlAutoCommitType)(MYSQL*, my_bool);
typedef my_bool (wxSTDCALL *MysqlCommitType)(MYSQL*);
typedef my_bool (wxSTDCALL *MysqlRollbackType)(MYSQL*);
typedef int (wxSTDCALL *MysqlQueryType)(MYSQL*, const char*);
typedef my_ulonglong(wxSTDCALL *MysqlAffectedRowsType)(MYSQL*);
typedef MYSQL_STMT* (wxSTDCALL *MysqlStmtInitType)(MYSQL*);
typedef int (wxSTDCALL *MysqlStmtPrepareType)(MYSQL_STMT*, const char*, unsigned long);
typedef int (wxSTDCALL *MysqlStmtExecuteType)(MYSQL_STMT*);
typedef const char* (wxSTDCALL *MysqlStmtErrorType)(MYSQL_STMT*);
typedef unsigned int (wxSTDCALL *MysqlStmtErrnoType)(MYSQL_STMT*);
typedef my_bool (wxSTDCALL *MysqlStmtFreeResultType)(MYSQL_STMT*);
typedef my_bool (wxSTDCALL *MysqlStmtCloseType)(MYSQL_STMT*);
typedef MYSQL_RES* (wxSTDCALL *MysqlListTablesType)(MYSQL*, const char*);
typedef MYSQL_ROW (wxSTDCALL *MysqlFetchRowType)(MYSQL_RES*);
typedef void (wxSTDCALL *MysqlFreeResultType)(MYSQL_RES*);
typedef unsigned long (wxSTDCALL *MysqlGetServerVersionType)(MYSQL*);
typedef MYSQL_RES* (wxSTDCALL *MysqlStmtResultMetadataType)(MYSQL_STMT*);
typedef unsigned int (wxSTDCALL *MysqlNumFieldsType)(MYSQL_RES*);
typedef unsigned long (wxSTDCALL *MysqlStmtParamCountType)(MYSQL_STMT*);
typedef my_bool (wxSTDCALL *MysqlStmtBindParamType)(MYSQL_STMT*, MYSQL_BIND*);
typedef int (wxSTDCALL *MysqlStmtFetchType)(MYSQL_STMT*);
typedef my_bool (wxSTDCALL *MysqlStmtBindResultType)(MYSQL_STMT*, MYSQL_BIND*);


class MysqlInterface
{
public:
  MysqlInterface() { }
  bool Init();

  MysqlServerEndType GetMysqlServerEnd() { return m_pMysqlServerEnd; }
  MysqlInitType GetMysqlInit() { return m_pMysqlInit; }
  MysqlRealConnectType GetMysqlRealConnect() { return m_pMysqlRealConnect; }
  MysqlRealQueryType GetMysqlRealQuery() { return m_pMysqlRealQuery; }
  MysqlErrorType GetMysqlError() { return m_pMysqlError; }
  MysqlErrnoType GetMysqlErrno() { return m_pMysqlErrno; }
  MysqlCloseType GetMysqlClose() { return m_pMysqlClose; }
  MysqlAutoCommitType GetMysqlAutoCommit() { return m_pMysqlAutoCommit; }
  MysqlCommitType GetMysqlCommit() { return m_pMysqlCommit; }
  MysqlRollbackType GetMysqlRollback() { return m_pMysqlRollback; }
  MysqlQueryType GetMysqlQuery() { return m_pMysqlQuery; }
  MysqlAffectedRowsType GetMysqlAffectedRows() { return m_pMysqlAffectedRows; }
  MysqlStmtInitType GetMysqlStmtInit() { return m_pMysqlStmtInit; }
  MysqlStmtPrepareType GetMysqlStmtPrepare() { return m_pMysqlStmtPrepare; }
  MysqlStmtExecuteType GetMysqlStmtExecute() { return m_pMysqlStmtExecute; }
  MysqlStmtErrorType GetMysqlStmtError() { return m_pMysqlStmtError; }
  MysqlStmtErrnoType GetMysqlStmtErrno() { return m_pMysqlStmtErrno; }
  MysqlStmtFreeResultType GetMysqlStmtFreeResult() { return m_pMysqlStmtFreeResult; }
  MysqlStmtCloseType GetMysqlStmtClose() { return m_pMysqlStmtClose; }
  MysqlListTablesType GetMysqlListTables() { return m_pMysqlListTables; }
  MysqlFetchRowType GetMysqlFetchRow() { return m_pMysqlFetchRow; }
  MysqlFreeResultType GetMysqlFreeResult() { return m_pMysqlFreeResult; }
  MysqlGetServerVersionType GetMysqlGetServerVersion() { return m_pMysqlGetServerVersion; }
  MysqlStmtResultMetadataType GetMysqlStmtResultMetadata() { return m_pMysqlStmtResultMetadata; }
  MysqlNumFieldsType GetMysqlNumFields() { return m_pMysqlNumFields; }
  MysqlStmtParamCountType GetMysqlStmtParamCount() { return m_pMysqlStmtParamCount; }
  MysqlStmtBindParamType GetMysqlStmtBindParam() { return m_pMysqlStmtBindParam; }
  MysqlStmtFetchType GetMysqlStmtFetch() { return m_pMysqlStmtFetch; }
  MysqlStmtBindResultType GetMysqlStmtBindResult() { return m_pMysqlStmtBindResult; }
  
private:
  wxDynamicLibrary m_MysqlDLL;

  MysqlServerEndType m_pMysqlServerEnd;
  MysqlInitType m_pMysqlInit;
  MysqlRealConnectType m_pMysqlRealConnect;
  MysqlRealQueryType m_pMysqlRealQuery;
  MysqlErrorType m_pMysqlError;
  MysqlErrnoType m_pMysqlErrno;
  MysqlCloseType m_pMysqlClose;
  MysqlAutoCommitType m_pMysqlAutoCommit;
  MysqlCommitType m_pMysqlCommit;
  MysqlRollbackType m_pMysqlRollback;
  MysqlQueryType m_pMysqlQuery;
  MysqlAffectedRowsType m_pMysqlAffectedRows;
  MysqlStmtInitType m_pMysqlStmtInit;
  MysqlStmtPrepareType m_pMysqlStmtPrepare;
  MysqlStmtExecuteType m_pMysqlStmtExecute;
  MysqlStmtErrorType m_pMysqlStmtError;
  MysqlStmtErrnoType m_pMysqlStmtErrno;
  MysqlStmtFreeResultType m_pMysqlStmtFreeResult;
  MysqlStmtCloseType m_pMysqlStmtClose;
  MysqlListTablesType m_pMysqlListTables;
  MysqlFetchRowType m_pMysqlFetchRow;
  MysqlFreeResultType m_pMysqlFreeResult;
  MysqlGetServerVersionType m_pMysqlGetServerVersion;
  MysqlStmtResultMetadataType m_pMysqlStmtResultMetadata;
  MysqlNumFieldsType m_pMysqlNumFields;
  MysqlStmtParamCountType m_pMysqlStmtParamCount;
  MysqlStmtBindParamType m_pMysqlStmtBindParam;
  MysqlStmtFetchType m_pMysqlStmtFetch;
  MysqlStmtBindResultType m_pMysqlStmtBindResult;
};

#endif // __MYSQL_INTERFACES_H__
