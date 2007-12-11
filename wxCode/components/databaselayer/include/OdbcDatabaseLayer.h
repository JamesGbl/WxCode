#ifndef __ODBC_DATABASE_LAYER_H__
#define __ODBC_DATABASE_LAYER_H__

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

#include <sql.h>
#include <sqlext.h>

#define ERR_BUFFER_LEN 1024
#define ERR_STATE_LEN 10


class OdbcDatabaseLayer : public DatabaseLayer
{
public:
  // ctor()
  OdbcDatabaseLayer();
  
  // dtor()
  virtual ~OdbcDatabaseLayer();
  
  // open database
  virtual bool Open();
  virtual bool Open(const wxString& strConnection);
  virtual bool Open(const wxString& strDSN, const wxString& strUser, const wxString& strPassword);
#if wxUSE_GUI
  virtual bool Open(const wxString& strConnection, bool bPromptForInfo, wxWindow* parent = NULL);
#endif

  // close database  
  virtual bool Close();
  
  // Is the connection to the database open?
  virtual bool IsOpen();

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

private:
  virtual PreparedStatement* PrepareStatement(const wxString& strQuery, bool bParseQuery);
  
  SQLHENV m_sqlEnvHandle;
  SQLHDBC m_sqlHDBC;
  SQLHSTMT m_sqlStatementHandle;

  wxString m_strDSN;
  wxString m_strUser;
  wxString m_strPassword;

  wxString m_strConnection;
#if wxUSE_GUI
  bool m_bPrompt;
  wxWindow* m_pParent;
#endif

  bool m_bIsConnected;

public:

  // error handling
  void InterpretErrorCodes( long nCode, SQLHSTMT stmth_ptr=NULL );

  SQLHANDLE allocStmth();
};

#endif // __ODBC_DATABASE_LAYER_H__

