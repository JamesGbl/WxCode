#ifndef __MYSQL_DATABASE_LAYER_H__
#define __MYSQL_DATABASE_LAYER_H__

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/arrstr.h"

#include "DatabaseLayer.h"
#include "PreparedStatement.h"

#include "mysql.h"

WX_DECLARE_VOIDPTR_HASH_MAP(void*, PointerLookupMap);

class MysqlDatabaseLayer : public DatabaseLayer
{
public:
  // Information that can be specified for a MySQL database
  //  host or hostaddr
  //  port
  //  dbname
  //  user
  //  password
  // ctor
  MysqlDatabaseLayer();
  MysqlDatabaseLayer(const wxString& strDatabase);
  MysqlDatabaseLayer(const wxString& strServer, const wxString& strDatabase);
  MysqlDatabaseLayer(const wxString& strDatabase, const wxString& strUser, const wxString& strPassword);
  MysqlDatabaseLayer(const wxString& strServer, const wxString& strDatabase, const wxString& strUser, const wxString& strPassword);
  MysqlDatabaseLayer(MYSQL* pDatabase) { m_pDatabase = pDatabase; }

  // dtor
  virtual ~MysqlDatabaseLayer();

  // open database
  virtual bool Open(const wxString& strDatabase);
  virtual bool Open(const wxString& strServer, const wxString& strDatabase);
  virtual bool Open(const wxString& strDatabase, const wxString& strUser, const wxString& strPassword);
  virtual bool Open(const wxString& strServer, const wxString& strDatabase, const wxString& strUser, const wxString& strPassword);

  // close database
  virtual bool Close();
  
  // Is the connection to the database open?
  virtual bool IsOpen();

  // transaction support
  // transaction support
  virtual void BeginTransaction();
  virtual void Commit();
  virtual void RollBack();
  
  // query database
  virtual bool RunQuery(const wxString& strQuery, bool bParseQuery);
  virtual DatabaseResultSet* RunQueryWithResults(const wxString& strQuery);

  // PreparedStatement support
  virtual PreparedStatement* PrepareStatement(const wxString& strQuery);

  // Database schema API contributed by M. Szeftel (author of wxActiveRecordGenerator)
  virtual bool TableExists(const wxString& table);
  virtual bool ViewExists(const wxString& view);
  virtual wxArrayString GetTables();
  virtual wxArrayString GetViews();
  virtual wxArrayString GetColumns(const wxString& table);

  static int TranslateErrorCode(int nCode);

private:
  void InitDatabase();
  void ParseServerAndPort(const wxString& strServer);

  wxString m_strServer;
  wxString m_strDatabase;
  wxString m_strUser;
  wxString m_strPassword;
  int m_iPort;
    
  MYSQL* m_pDatabase;

#if wxUSE_UNICODE
  PointerLookupMap m_ResultSets;
#endif
};

#endif // __MYSQL_DATABASE_LAYER_H__

