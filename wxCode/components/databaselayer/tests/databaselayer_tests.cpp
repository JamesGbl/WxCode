#include "MiniCppUnit.hxx"

#include <wx/filename.h>
#include <wx/file.h>
#include <wx/textfile.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/fileconf.h>
#include <wx/wfstream.h>
#include <wx/ffile.h>

#include "../include/DatabaseLayer.h"
#include "../include/DatabaseErrorCodes.h"
#include "../include/DatabaseLayerException.h"
#include "../include/ResultSetMetaData.h"

#ifdef USE_SQLITE
  #include "../include/SqliteDatabaseLayer.h"
#elif USE_FIREBIRD
  #include "../include/FirebirdDatabaseLayer.h"
#elif USE_POSTGRESQL
  #include "../include/PostgresDatabaseLayer.h"
#elif USE_MYSQL
  #include "../include/MysqlDatabaseLayer.h"
#elif USE_ORACLE
  #include "../include/OTLDatabaseLayer.h"
#elif USE_ODBC
  #include "../include/OdbcDatabaseLayer.h"
#elif USE_TDS
  #include "../include/TdsDatabaseLayer.h"
#endif

#include <wx/arrstr.h>
#include <map>
typedef std::map<int, wxArrayString> SQL_STATEMENT_ARRAY;

/***
  Previously, the contents of this file were in MyTest.h and the CVS history is in the attic
  ***/

/*
   To add a new test, here is a skeleton function that can be used.
   Also be sure to add the new function to the TEST_FIXTURE below.

      void skeletonForNewTest( void )
      {
        puts("skeleton");
        ASSERT( m_pDatabaseLayer != NULL );
        
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
            // Place the code for your test here
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

 */

  class MyTestSuite : public TestFixture<MyTestSuite>
  {
  public:
      TEST_FIXTURE( MyTestSuite )
      {
        TEST_CASE(testInsertRecords)
        TEST_CASE(testInsertRecordsWithPreparedStatements)
        TEST_CASE(testMultipartInserts)
        TEST_CASE(testMultipartInsertsWithResults)
        TEST_CASE(testMultipartStatements)
        TEST_CASE(testResults)
        TEST_CASE(testPreparedStatementResults)  // Blob
        TEST_CASE(testSelectCorrectRecord)
        TEST_CASE(testSelectCorrectRecordFromPreparedStatement)
        TEST_CASE(testRollback)
        TEST_CASE(testRollbackComplexSequence)
        TEST_CASE(testDateTimeParameters)  // DateTime
        TEST_CASE(testDateTimeParametersBefore1970)  // DateTime
        TEST_CASE(testDateResultSetField)  // DateTime
        TEST_CASE(testBinaryBlobParametersSimpleRetrieval)  // Blob
        TEST_CASE(testBinaryBlobParametersPreparedRetrieval)  // Blob
        TEST_CASE(testChangePreparedStatementParameters)
        TEST_CASE(testSetPreparedStatementParametersOutOfOrder)
        TEST_CASE(testManyBlobRecordRetrieval)  // Blob
        TEST_CASE(testLoopingPreparedStatement)
        TEST_CASE(testLoopingPreparedStatementRetrieval)
        TEST_CASE(testResultSetNext)
        TEST_CASE(testFieldNotInResultSetError)
        TEST_CASE(testNullValues)  // DateTime, Blob
        TEST_CASE(testNullParameters)  // DateTime, Blob
        TEST_CASE(testSingleLinePreparedStatement)
        TEST_CASE(testEncodingSupport)
        TEST_CASE(testParameterEncodingSupport);
        TEST_CASE(testResultsByIndex)
        TEST_CASE(testPreparedStatementResultsByIndex)  // Blob
        TEST_CASE(testResultSetMetaData)  // DateTime, Blob
        TEST_CASE(testResultSetMetaDataForNullValues)  // DateTime, Blob
        TEST_CASE(testResultSetAlias)
        TEST_CASE(testViewAlias)
        TEST_CASE(testSemicolonInsideStatement)
        TEST_CASE(testDatabaseLayerWrappers)
        TEST_CASE(testPreparedStatementWrappers)
        TEST_CASE(testTextRemnants)
        TEST_CASE(testDatabaseGarbageCollection)
        TEST_CASE(testIsFieldNull)  // DateTime, Blob
        TEST_CASE(testDateWithoutTime) // DateTime
        TEST_CASE(testDateWithoutTimePreparedStatement) // DateTime
        TEST_CASE(testTableExists)
        TEST_CASE(testViewExists)
        TEST_CASE(testGetTables)
        TEST_CASE(testGetViews)
        TEST_CASE(testGetColumns)
        TEST_CASE(testCloseBeforeDelete)
        TEST_CASE(testIsOpen)
        TEST_CASE(testSingleResult)
        TEST_CASE(testSingleResultNoResults)
        TEST_CASE(testSingleResultNonUniqueResults)
        TEST_CASE(testResultArrayInt)
        TEST_CASE(testResultArrayString)
        TEST_CASE(testResultArrayLong)
#if wxCHECK_VERSION(2, 7, 0)
        TEST_CASE(testResultArrayDouble)
#endif
        TEST_CASE(testMemoryBufferBlobParameter)
        TEST_CASE(testSmallMemoryBuffer)
        TEST_CASE(testLargeMemoryBuffer)
        TEST_CASE(testReusePreparedStatementWithResults)
        TEST_CASE(testRunQueryReturnValue)
        TEST_CASE(testPreparedStatementRunQueryReturnValue)
        TEST_CASE(testBadPreparedStatement)
        TEST_CASE(testResultSetNoRecords)
        TEST_CASE(testSpecialCharactersInPreparedStatement)
      }

    private:
      DatabaseLayer* m_pDatabaseLayer;
      wxString m_strPath;

      // The map will have a numeric key where #define's are used to make the
      //  code more readable.  The map's values will be wxArrayString instances
      //  because some actions will require more than one query to be made
      //  (for instance the tables will autonumber fields and foreign key
      //  constraints in the testRollbackComplexSequence test)
      #define CREATE_TABLE1 1
      #define CREATE_TABLE2 2
      #define CREATE_TABLE3 3
      #define CREATE_TABLE4 4
      #define CREATE_TABLE5 5
      #define CREATE_TABLE6 6
      #define CREATE_TABLE7 7
      #define CREATE_TABLE8 8
      #define CREATE_COMPLEX_ROLLBACK_TABLES 9
      #define DROP_COMPLEX_ROLLBACK_TABLES 10
      #define INSERT_INTO_TABLE3 11
      #define CREATE_TABLE9 12
      #define INSERT_INTO_TABLE9 13
      #define CREATE_VIEW1 14

      SQL_STATEMENT_ARRAY m_SqlMap; // Database specific SQL statements to be run on the tests
      wxArrayString m_DeleteStatements; // Statements such as "DROP TABLE tableX;" to be run during tearDown

  public:
      void setUp()
      {
		  wxGetEnv(_("PATH"), &m_strPath);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
        try
        {
#endif

        bool databaseSettingsLoaded = LoadSettings();
        if (!databaseSettingsLoaded)
        {
          fprintf(stderr, "Error loading database settings");
          m_pDatabaseLayer = NULL;
          return;
        }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
        try
        {
#endif
        SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE1);
        if (createTableSql != m_SqlMap.end())
        {
          for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
          {
            if (m_pDatabaseLayer)
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
          }
        }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
        }
        catch (DatabaseLayerException& e)
        {
          // Don't stop on error here since this could be a "table already exists" error
          wxString errorMessage = wxString::Format(_("Error (%d): %s"), e.GetErrorCode(), e.GetErrorMessage().c_str());
          //puts(errorMessage.mb_str());
        }
#endif

        if (m_pDatabaseLayer)
          m_pDatabaseLayer->RunQuery(_("DELETE FROM table1;")); // Be extra sure the table is empty

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
        }
        catch (DatabaseLayerException& e)
        {
          wxString errorMessage = wxString::Format(_("Error (%d): %s"), e.GetErrorCode(), e.GetErrorMessage().c_str());
          //puts(errorMessage.mb_str());
          m_pDatabaseLayer = NULL;
          return;
        }
#endif
      }

      void tearDown()
      {
		  wxSetEnv(_("PATH"), m_strPath);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
        try
        {
#endif
        if (m_pDatabaseLayer)
        {
          // Close and prepared statements and result sets that may have been left open
          m_pDatabaseLayer->CloseResultSets();
          m_pDatabaseLayer->CloseStatements();
          
          // Remove all the tables unique to each of the unit tests
          wxArrayString::iterator start = m_DeleteStatements.begin();
          wxArrayString::iterator stop = m_DeleteStatements.end();
          while (start != stop)
          {
            m_pDatabaseLayer->RunQuery(*start);
            start++;
          }
          m_DeleteStatements.Clear();

          m_pDatabaseLayer->RunQuery(_("DROP TABLE table1;"));
        }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
        }
        catch (DatabaseLayerException& e)
        {
          puts("Exception caught during teardown");
          ProcessException(e, false);
        }
#endif

        // Make sure that the databaselayer gets deleted
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
        try
        {
#endif
          wxDELETE(m_pDatabaseLayer);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
        }
        catch (DatabaseLayerException& e)
        {
          puts("Exception caught during teardown");
          ProcessException(e, false);
        }
#endif
      }

      /* Test simple insert (and row count retrieval) works
       */
      void testInsertRecords( void )
      {
        puts("testInsertRecords");
        ASSERT( m_pDatabaseLayer != NULL );
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          // Get the initial row count
          wxPrintf(_("Checking if row count = 0\n"));
          int nCount = GetRowCount();
          ASSERT_EQUALS( 0, nCount );
        
          // Add a new row to the database table
          m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('ONE', 'TWO');"));
          DatabaseErrorCheck(m_pDatabaseLayer);

          // Get the updated row count
          wxPrintf(_("Checking if row count = 1\n"));
          nCount = GetRowCount();
          ASSERT_EQUALS( 1, nCount );

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }
      
      /* Test insert using a prepared statement works
       */
      void testInsertRecordsWithPreparedStatements( void )
      {
        puts("testInsertRecordsWithPreparedStatements");
        ASSERT( m_pDatabaseLayer != NULL );
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          // Get the initial row count
          wxPrintf(_("Checking if row count = 0\n"));
          int nCount = GetRowCount();
          ASSERT_EQUALS( 0, nCount );
        
          // Add a new row to the database table
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("INSERT INTO table1 (column1, column2) VALUES (?, ?);"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            ASSERT(m_pDatabaseLayer->GetErrorCode() == DATABASE_LAYER_OK);
            if (m_pDatabaseLayer->GetErrorCode() != DATABASE_LAYER_OK)
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }
            else
            {
              pStatement->SetParamString(1, _("ONE"));
              DatabaseErrorCheck(pStatement);
              pStatement->SetParamString(2, _("TWO"));
              DatabaseErrorCheck(pStatement);
              pStatement->RunQuery();
              DatabaseErrorCheck(pStatement);
            }
            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }

          // Get the updated row count
          wxPrintf(_("Checking if row count = 1\n"));
          nCount = GetRowCount();
          ASSERT_EQUALS( 1, nCount );
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      /* Test SQL statement with multiple parts 
       */
      void testMultipartInserts( void )
      {
        puts("testMultipartInserts");
        ASSERT( m_pDatabaseLayer != NULL );
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          // Get the initial row count
          int nCount = GetRowCount();
          wxPrintf(_("Checking if row count = 0\n"));
          ASSERT_EQUALS( 0, nCount );
        
          // Add a new row to the database table
          wxString strSQL = _("INSERT INTO table1 (column1, column2) VALUES ('ONE', 'TWO');");
          strSQL += _("INSERT INTO table1 (column1, column2) VALUES ('ONE', 'TWO');");
          strSQL += _("INSERT INTO table1 (column1, column2) VALUES ('ONE', 'TWO');");

          m_pDatabaseLayer->RunQuery(strSQL);
          DatabaseErrorCheck(m_pDatabaseLayer);

          // Get the updated row count
          nCount = GetRowCount();
          wxPrintf(_("Checking if row count = 3\n"));
          ASSERT_EQUALS( 3, nCount );
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      /* Test SQL statement with multiple parts where the last SQL part retrieves a result set
       */
      void testMultipartInsertsWithResults( void )
      {
        puts("testMultipartInsertsWithResults");
        ASSERT( m_pDatabaseLayer != NULL );
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          // Get the initial row count
          int nCount = GetRowCount();
          ASSERT_EQUALS( 0, nCount );
        
          // Add a new row to the database table
          wxString strSQL = _("INSERT INTO table1 (column1, column2) VALUES ('ONE', 'TWO');");
          strSQL += _("INSERT INTO table1 (column1, column2) VALUES ('ONE', 'TWO');");
          strSQL += _("INSERT INTO table1 (column1, column2) VALUES ('ONE', 'TWO');");
          strSQL += _("SELECT COUNT(*) FROM table1;");

          DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(strSQL);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pResultSet != NULL);
          if (pResultSet)
          {
            DatabaseErrorCheck(pResultSet);
            if (pResultSet->Next())
            {
              DatabaseErrorCheck(pResultSet);
              nCount = pResultSet->GetResultInt(1);
              // Get the updated row count
              ASSERT_EQUALS( 3, nCount );
            }
            else
            {
              FAIL("Empty ResultSet");
            }
            m_pDatabaseLayer->CloseResultSet(pResultSet);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      /* Test Prepared SQL statement with multiple parts
       */
      void testMultipartStatements( void )
      {
        puts("testMultipartStatements");
        ASSERT( m_pDatabaseLayer != NULL );
        
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          // Get the initial row count
          int nCount = GetRowCount();
          ASSERT_EQUALS( 0, nCount );

          // Add the INSERT commands to the SQL statement
          wxString strSQL = _("INSERT INTO table1 (column1, column2) VALUES (?, ?); ");
          strSQL += _("INSERT INTO table1 (column1, column2) VALUES (?, ?); ");
          // Finish the statement with a SELECT to get the record count
          strSQL += _("SELECT COUNT(*) FROM table1 WHERE column1 = ?;");
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(strSQL);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            ASSERT_EQUALS( 5, pStatement->GetParameterCount() );
            pStatement->SetParamString(1, _("ONE"));
            DatabaseErrorCheck(pStatement);
            pStatement->SetParamString(2, _("TWO"));
            DatabaseErrorCheck(pStatement);
            pStatement->SetParamString(3, _("ONE"));
            DatabaseErrorCheck(pStatement);
            pStatement->SetParamString(4, _("TWO"));
            DatabaseErrorCheck(pStatement);
            pStatement->SetParamString(5, _("ONE"));
            DatabaseErrorCheck(pStatement);
            DatabaseResultSet* pResultSet = NULL;
            try
            {
              pResultSet = pStatement->RunQueryWithResults();
              DatabaseErrorCheck(pResultSet);
            }
            catch (DatabaseLayerException& e)
            {
              puts("Exception caught!");
              puts(e.GetErrorMessage().mb_str());
              ProcessException(e);
            }
            DatabaseErrorCheck(pStatement);
            ASSERT(pResultSet != NULL);
            if (pResultSet)
            {
              DatabaseErrorCheck(pResultSet);
              if (pResultSet->Next())
              {
                DatabaseErrorCheck(pResultSet);
                int nCount = pResultSet->GetResultInt(1);
                wxPrintf(_("Asserting count = 2\n"));
                ASSERT_EQUALS( 2, nCount );
              }
              else
              {
                FAIL("Empty ResultSet");
              }
              m_pDatabaseLayer->CloseResultSet(pResultSet);
            }
            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }


      /* Test result set from simple insert
       */
      void testResults( void )
      {
        puts("testResults");
        ASSERT( m_pDatabaseLayer != NULL );
        
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          // Add a table just for this test
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE2);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table2;"));

          m_pDatabaseLayer->RunQuery(_("DELETE FROM table2;")); // Be extra sure the table is empty

          m_pDatabaseLayer->RunQuery(_("INSERT INTO table2 (IntCol, DoubleCol, StringCol) VALUES (10, 1.234, 'Test1');"));

          DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT * FROM table2;"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pResultSet != NULL);
          if (pResultSet)
          {
            DatabaseErrorCheck(pResultSet);
            if (pResultSet->Next())
            {
              DatabaseErrorCheck(pResultSet);
              wxPrintf(_("Asserting IntCol = 10\n"));
              ASSERT_EQUALS( 10, pResultSet->GetResultInt(_("IntCol")));
              wxPrintf(_("Asserting DoubleCol = 1.234\n"));
              ASSERT_EQUALS_EPSILON( 1.234, pResultSet->GetResultDouble(_("DoubleCol")), 0.000001);
              wxPrintf(_("Asserting StringCol = 'Test1'\n"));
              ASSERT_EQUALS(wxString(_("Test1")), pResultSet->GetResultString(_("StringCol")));
            }
            else
            {
              FAIL("Empty ResultSet");
            }
            m_pDatabaseLayer->CloseResultSet(pResultSet);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }
      
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }
      
      /* Test result set from a Prepared statement
       */
      void testPreparedStatementResults( void )
      {
        puts("testPreparedStatementResults");
        ASSERT( m_pDatabaseLayer != NULL );
        
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif

          // Add a table just for this test
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE6);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
          DatabaseErrorCheck(m_pDatabaseLayer);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table6;"));

          m_pDatabaseLayer->RunQuery(_("DELETE FROM table6;")); // Be extra sure the table is empty
          DatabaseErrorCheck(m_pDatabaseLayer);

          wxString strSQL = _("INSERT INTO table6 (IntCol, DoubleCol, StringCol, BlobCol) VALUES (?, ?, ?, ?); SELECT * FROM table6;");
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(strSQL);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            wxPrintf(_("Asserting parameter count = 4\n"));
            ASSERT_EQUALS( 4, pStatement->GetParameterCount() );
            pStatement->SetParamInt(1, 10);
            DatabaseErrorCheck(pStatement);
            pStatement->SetParamDouble(2, 1.234);
            DatabaseErrorCheck(pStatement);
            pStatement->SetParamString(3, _("Test2"));
            DatabaseErrorCheck(pStatement);
            wxString strBlob = _("Test Blob");
            // Use the length + 1 so that we get the null terminating character at the end of the string
            wxCharBuffer charBuffer = strBlob.mb_str(*wxConvCurrent);
            pStatement->SetParamBlob(4, (const unsigned char*)(const char*)charBuffer, strBlob.Length()+1);
            DatabaseErrorCheck(pStatement);

            DatabaseResultSet* pResultSet = pStatement->RunQueryWithResults();
            DatabaseErrorCheck(pStatement);
            ASSERT(pResultSet != NULL);
            if (pResultSet)
            {
              DatabaseErrorCheck(pResultSet);
              if (pResultSet->Next())
              {
                DatabaseErrorCheck(pResultSet);
                wxPrintf(_("Asserting IntCol = 10\n"));
                ASSERT_EQUALS( 10, pResultSet->GetResultInt(_("IntCol")));
                wxPrintf(_("Asserting DoubleCol = 1.234\n"));
                ASSERT_EQUALS_EPSILON( 1.234, pResultSet->GetResultDouble(_("DoubleCol")), 0.000001);
                wxPrintf(_("Asserting StringCol = 'Test2'\n"));
                ASSERT_EQUALS(wxString(_("Test2")), pResultSet->GetResultString(_("StringCol")));
                wxMemoryBuffer buffer;
                pResultSet->GetResultBlob(_("BlobCol"), buffer);
                wxString strResult((char*)buffer.GetData(), *wxConvCurrent);
                wxPrintf(_("Asserting BlobCol = 'Test Blob'\n"));
                ASSERT_EQUALS( strBlob, strResult );
              }
              else
              {
                FAIL("Empty ResultSet");
              }
              m_pDatabaseLayer->CloseResultSet(pResultSet);
            }
            else
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }
            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }
      
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      /* Test retrieving a particular record from a query
       */
      void testSelectCorrectRecord()
      {
        puts("testSelectCorrectRecord");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          // Get initial row count
          int nCount = GetRowCount();
          ASSERT_EQUALS( 0, nCount );
        
          // Add 3 rows
          m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('1', 'ONE');"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('2', 'TWO');"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('3', 'THREE');"));
          DatabaseErrorCheck(m_pDatabaseLayer);

          // Get the updated row count
          nCount = GetRowCount();
          ASSERT_EQUALS( 3, nCount );

          DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT * FROM table1 WHERE column1 = '2';"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pResultSet != NULL);
          if (pResultSet)
          {
            DatabaseErrorCheck(pResultSet);
            if (pResultSet->Next())
            {
              DatabaseErrorCheck(pResultSet);
              ASSERT_EQUALS(wxString(_("TWO")), pResultSet->GetResultString(_("column2")));
            }
            else
            {
              FAIL("Empty ResultSet");
            }
            m_pDatabaseLayer->CloseResultSet(pResultSet);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }
      
      /* Test retrieving a particular record from a Prepared statement
       */
      void testSelectCorrectRecordFromPreparedStatement()
      {
        puts("testSelectCorrectRecordFromPreparedStatement");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          // Get initial row count
          int nCount = GetRowCount();
          ASSERT_EQUALS( 0, nCount );
        
          // Add 3 rows
          m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('1', 'ONE');"));
          m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('2', 'TWO');"));
          m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('3', 'THREE');"));
          DatabaseErrorCheck(m_pDatabaseLayer);

          // Get the updated row count
          nCount = GetRowCount();
          ASSERT_EQUALS( 3, nCount );

          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("SELECT * FROM table1 WHERE column1 = ?;"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            pStatement->SetParamString(1, _("2"));
            DatabaseErrorCheck(pStatement);
            DatabaseResultSet* pResultSet = pStatement->RunQueryWithResults();
            DatabaseErrorCheck(pStatement);
            ASSERT(pResultSet != NULL);
            if (pResultSet)
            {
              DatabaseErrorCheck(pResultSet);
              if (pResultSet->Next())
              {
                DatabaseErrorCheck(pResultSet);
                ASSERT_EQUALS(wxString(_("TWO")), pResultSet->GetResultString(_("column2")));
              }
              else
              {
                FAIL("Empty ResultSet");
              }
              m_pDatabaseLayer->CloseResultSet(pResultSet);
            }
            else
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }
            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }
      
      /* Test that a rollback really undoes a SQL insert
       */
      void testRollback( void )
      {
        puts("testRollback");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          // Get initial row count
          int nCount = GetRowCount();
          ASSERT_EQUALS( 0, nCount );
        
          // Add a row
          m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('ONE', 'TWO');"));
          DatabaseErrorCheck(m_pDatabaseLayer);

          // Get the updated row count
          nCount = GetRowCount();
          ASSERT_EQUALS( 1, nCount );

          // Add a row in a transaction and roll it back
          m_pDatabaseLayer->BeginTransaction();
          DatabaseErrorCheck(m_pDatabaseLayer);
          m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('ONE', 'TWO');"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          m_pDatabaseLayer->RollBack();
          DatabaseErrorCheck(m_pDatabaseLayer);

          // Confirm that the row was NOT added
          int nCount2 = GetRowCount();
          ASSERT_EQUALS(nCount, nCount2);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      /* Test that a rollback really undoes a SQL insert
       */
      void testRollbackComplexSequence( void )
      {
        puts("testRollbackComplexSequence");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_COMPLEX_ROLLBACK_TABLES);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              // Some of the database backend need to create procedures for the autoincrement columns
              // Because of this, we run the queries with the "don't parse" flag
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i), false);
            }
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            wxPrintf(_("Exception caught while creating tables.  Ignoring...\n"));
            ProcessException(e, false);
          }
#endif
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE secondaryTable;"));
          m_DeleteStatements.push_back(_("DROP TABLE mainTable;"));

          SQL_STATEMENT_ARRAY::iterator dropTableSql = m_SqlMap.find(DROP_COMPLEX_ROLLBACK_TABLES);
          if (dropTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*dropTableSql).second.size(); i++)
            {
              m_DeleteStatements.push_back((*dropTableSql).second.Item(i));
            }
          }

          // Get initial row count
          //wxPrintf(_("Getting mainTable row count\n"));
          int nCount = GetRowCount(_("mainTable"));
          ASSERT_EQUALS( 0, nCount );
          //wxPrintf(_("Getting secondaryTable row count\n"));
          nCount = GetRowCount(_("secondaryTable"));
          ASSERT_EQUALS( 0, nCount );
          int nPrimaryKey = -1;

          // Add a row in a transaction and roll it back
          m_pDatabaseLayer->BeginTransaction();
          DatabaseErrorCheck(m_pDatabaseLayer);
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("INSERT INTO mainTable (description) VALUES (?); SELECT mainId FROM mainTable WHERE description = ?;"));
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            DatabaseErrorCheck(pStatement);
            pStatement->SetParamString(1, _("ONE"));
            pStatement->SetParamString(2, _("ONE"));
            DatabaseErrorCheck(pStatement);
            DatabaseResultSet* pResultSet = pStatement->RunQueryWithResults();
            DatabaseErrorCheck(pStatement);
            ASSERT(pResultSet != NULL);
            if (pResultSet)
            {
              DatabaseErrorCheck(pResultSet);
              if (pResultSet->Next())
              {
                nPrimaryKey = pResultSet->GetResultInt(_("mainId"));
              }
              else
              {
                FAIL("Empty ResultSet");
              }
              m_pDatabaseLayer->CloseResultSet(pResultSet);
            }
            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          nCount = GetRowCount(_("mainTable"));
          ASSERT_EQUALS(1, nCount);

          ASSERT(-1 != nPrimaryKey);
          if (nPrimaryKey != -1)
          {
            PreparedStatement* pStatement2 = m_pDatabaseLayer->PrepareStatement(_("INSERT INTO secondaryTable (foreignKey, description) VALUES (?, ?);"));
            ASSERT(pStatement2 != NULL);
            if (pStatement2)
            {
              DatabaseErrorCheck(pStatement2);
              pStatement2->SetParamInt(1, nPrimaryKey);
              pStatement2->SetParamString(2, _("DESC"));
              DatabaseErrorCheck(pStatement2);
              pStatement2->RunQuery();
              nCount = GetRowCount(_("secondaryTable"));
              ASSERT_EQUALS(1, nCount);

              m_pDatabaseLayer->CloseStatement(pStatement2);
            }
          }
          
          DatabaseErrorCheck(m_pDatabaseLayer);
          m_pDatabaseLayer->RollBack();
          DatabaseErrorCheck(m_pDatabaseLayer);

          // Confirm that the row was NOT added
          nCount = GetRowCount(_("mainTable"));
          ASSERT_EQUALS(0, nCount);
          nCount = GetRowCount(_("secondaryTable"));
          ASSERT_EQUALS(0, nCount);

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }


      /* Test that a wxDateTime is saved to and restored from the database correctly
       */
      void testDateTimeParameters()
      {
        puts("testDateTimeParameters");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE3);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
          m_pDatabaseLayer->RunQuery(_("DELETE FROM table3;")); // Be extra sure the table is empty
          DatabaseErrorCheck(m_pDatabaseLayer);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table3;"));

          wxDateTime TimeNow = wxDateTime::Now();

          // Use a prepared statement to insert a wxDateTime into the database
          wxString strSQL = _("INSERT INTO table3 (IntCol, DateCol) VALUES (1, ?);");
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(strSQL);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            ASSERT_EQUALS( 1, pStatement->GetParameterCount() );

            pStatement->SetParamDate(1, TimeNow);
            DatabaseErrorCheck(pStatement);
            pStatement->RunQuery();
            DatabaseErrorCheck(pStatement);

            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }

          // Sleep for a second to make sure there is a little lag between the time that TimeNow was created
          //  and the date is retrieved
          ::wxMilliSleep(1000);

          // Pull it out by simple query
          DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT DateCol FROM table3 WHERE IntCol = 1"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pResultSet != NULL);
          if (pResultSet)
          {
            DatabaseErrorCheck(pResultSet);
            if (pResultSet->Next())
            {
              DatabaseErrorCheck(pResultSet);
              wxDateTime RetrievedTime = pResultSet->GetResultDate(_("DateCol"));
              wxPrintf(_("TimeNow: \"%s\" compared to \"%s\"\n"), 
                    (const char*)TimeNow.Format().c_str(), (const char*)RetrievedTime.Format().c_str());
              ASSERT(RetrievedTime.IsEqualTo(TimeNow));
            }
            else
            {
              FAIL("Empty ResultSet");
            }
            m_pDatabaseLayer->CloseResultSet(pResultSet);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }
          
          // Now pull it out by prepared statement
          pStatement = m_pDatabaseLayer->PrepareStatement(_("SELECT DateCol FROM table3 WHERE IntCol = ?"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            pStatement->SetParamInt(1, 1);
            DatabaseErrorCheck(pStatement);
            pResultSet = pStatement->RunQueryWithResults();
            DatabaseErrorCheck(pStatement);
            ASSERT(pResultSet != NULL);
            if (pResultSet)
            {
              DatabaseErrorCheck(pResultSet);
              if (pResultSet->Next())
              {
                DatabaseErrorCheck(pResultSet);
                wxDateTime RetrievedTime = pResultSet->GetResultDate(_("DateCol"));
                wxPrintf(_("TimeNow: \"%s\" compared to \"%s\"\n"), 
                    (const char*)TimeNow.Format().c_str(), (const char*)RetrievedTime.Format().c_str());
                ASSERT(RetrievedTime.IsEqualTo(TimeNow));
              }
              else
              {
                FAIL("Empty ResultSet");
              }
              m_pDatabaseLayer->CloseResultSet(pResultSet);
            }
            else
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }

            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      /* Test that a wxDateTime is saved to and restored from the database correctly
       */
      void testDateTimeParametersBefore1970()
      {
        puts("testDateTimeParametersBefore1970");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE3);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table3;"));

          m_pDatabaseLayer->RunQuery(_("DELETE FROM table3;")); // Be extra sure the table is empty
          wxDateTime TimeBefore1970(21 /*day of month*/, wxDateTime::Jun /*month*/, 1904 /*year*/, 16 /*hour*/, 42 /*minute*/, 23 /*second*/, 0 /*milliseconds*/);

          // Use a prepared statement to insert a wxDateTime into the database
          wxString strSQL = _("INSERT INTO table3 (IntCol, DateCol) VALUES (1, ?);");
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(strSQL);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            ASSERT_EQUALS( 1, pStatement->GetParameterCount() );

            pStatement->SetParamDate(1, TimeBefore1970);
            DatabaseErrorCheck(pStatement);
            pStatement->RunQuery();
            DatabaseErrorCheck(pStatement);

            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }

          // Pull it out by simple query
          DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT DateCol FROM table3 WHERE IntCol = 1"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pResultSet != NULL);
          if (pResultSet)
          {
            DatabaseErrorCheck(pResultSet);
            if (pResultSet->Next())
            {
              DatabaseErrorCheck(pResultSet);
              wxDateTime RetrievedTime = pResultSet->GetResultDate(_("DateCol"));
              wxPrintf(_("TimeBefore1970: \"%s\" compared to \"%s\"\n"), 
                  (const char*)TimeBefore1970.Format().c_str(), (const char*)RetrievedTime.Format().c_str());
              ASSERT(RetrievedTime.IsEqualTo(TimeBefore1970));
            }
            else
            {
              FAIL("Empty ResultSet");
            }
            m_pDatabaseLayer->CloseResultSet(pResultSet);
          }

          // Now pull it out by prepared statement
          pStatement = m_pDatabaseLayer->PrepareStatement(_("SELECT DateCol FROM table3 WHERE IntCol = ?"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            pStatement->SetParamInt(1, 1);
            DatabaseErrorCheck(pStatement);
            pResultSet = pStatement->RunQueryWithResults();
            DatabaseErrorCheck(pStatement);
            ASSERT(pResultSet != NULL);
            if (pResultSet)
            {
              DatabaseErrorCheck(pResultSet);
              if (pResultSet->Next())
              {
                DatabaseErrorCheck(pResultSet);
                wxDateTime RetrievedTime = pResultSet->GetResultDate(_("DateCol"));
                wxPrintf(_("TimeBefore1970: \"%s\" compared to \"%s\"\n"), 
                    (const char*)TimeBefore1970.Format().c_str(), (const char*)RetrievedTime.Format().c_str());
                ASSERT(RetrievedTime.IsEqualTo(TimeBefore1970));
              }
              else
              {
                FAIL("Empty ResultSet");
              }
              m_pDatabaseLayer->CloseResultSet(pResultSet);
            }
            else
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }

            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }
          
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif

        }
      }

      void testDateResultSetField()
      {
        puts("testDateResultSetField");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE3);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table3;"));

          m_pDatabaseLayer->RunQuery(_("DELETE FROM table3;")); // Be extra sure the table is empty
          
          wxDateTime ExpectedTime(21 /*day of month*/, wxDateTime::Jan /*month*/, 2006 /*year*/, 16 /*hour*/, 42 /*minute*/, 23 /*second*/, 0 /*milliseconds*/);
          
          SQL_STATEMENT_ARRAY::iterator insertIntoTableSql = m_SqlMap.find(INSERT_INTO_TABLE3);
          if (insertIntoTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*insertIntoTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*insertIntoTableSql).second.Item(i));
            }
          }

          DatabaseErrorCheck(m_pDatabaseLayer);
          DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT * FROM table3;"));
          ASSERT(pResultSet != NULL);
          if (pResultSet)
          {
            DatabaseErrorCheck(pResultSet);
            if (pResultSet->Next())
            {
              wxDateTime RetrievedTime = pResultSet->GetResultDate(_("DateCol"));
              wxPrintf(_("ExpectedTime: \"%s\" compared to \"%s\"\n"), 
                    (const char*)ExpectedTime.Format().c_str(), (const char*)RetrievedTime.Format().c_str());
              ASSERT(RetrievedTime.IsEqualTo(ExpectedTime));
            }
            else
            {
              FAIL("Empty ResultSet");
            }
            m_pDatabaseLayer->CloseResultSet(pResultSet);
          }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif

        }
      }

      /* Test that a binary blob (from a file) is saved to and restored from the database correctly
       */
      void testBinaryBlobParametersSimpleRetrieval()
      {
        puts("testBinaryBlobParametersSimpleRetrieval");
        // Save one of the wxWidgets samples image files into a database table
        // Retrieve the image from the database and save it to a file
        // Do a MD5 sum on the original file and the new file to make sure that they're equal

        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE4);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table4;"));

          m_pDatabaseLayer->RunQuery(_("DELETE FROM table4;")); // Be extra sure the table is empty

          // Use a prepared statement to insert a binary BLOB into the database
          wxString strSQL = _("INSERT INTO table4 (IntCol, BlobCol) VALUES (1, ?);");
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(strSQL);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            ASSERT_EQUALS( 1, pStatement->GetParameterCount() );

            wxFile FileIn(_("wxwin.png"));
            int nDataLength = FileIn.Length();
            wxMemoryBuffer BufferIn;
            int nBytesRead = FileIn.Read(BufferIn.GetWriteBuf(nDataLength), nDataLength);
            ASSERT_EQUALS(nBytesRead, nDataLength);
            pStatement->SetParamBlob(1, (unsigned char*)BufferIn.GetData(), nDataLength);
            DatabaseErrorCheck(pStatement);
            pStatement->RunQuery();
            DatabaseErrorCheck(pStatement);
            m_pDatabaseLayer->CloseStatement(pStatement);

            DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT BlobCol FROM table4 WHERE IntCol = 1"));
            DatabaseErrorCheck(m_pDatabaseLayer);
            ASSERT(pResultSet != NULL);
            if (pResultSet)
            {
              DatabaseErrorCheck(pResultSet);
              if (pResultSet->Next())
              {
                DatabaseErrorCheck(pResultSet);
                wxMemoryBuffer BufferOut;
                void* pBlob = pResultSet->GetResultBlob(_("BlobCol"), BufferOut);
                ASSERT_EQUALS(nDataLength, (int)BufferOut.GetBufSize());
                ASSERT_EQUALS(nDataLength, (int)BufferOut.GetDataLen());

                // Iterate through each byte and compare it to the source
                bool bMatch = TRUE;
                char* pOriginalBuffer = (char*)(BufferIn.GetData());
                char* pRetrievedBuffer = (char*)pBlob;
                wxString errorMessage;
                for (int i=0; i<nDataLength && bMatch; i++)
                {
                  if (*pOriginalBuffer != *pRetrievedBuffer)
                  {
                    errorMessage.Printf(_("Retrieved Blob does NOT match original Blob.  Error at byte %d"), i);
                    bMatch = FALSE;
                  }
                  pOriginalBuffer++;
                  pRetrievedBuffer++;
                }
                ASSERT_MESSAGE( bMatch, errorMessage.mb_str() );
              }
              else
              {
                m_pDatabaseLayer->CloseResultSet(pResultSet);
                FAIL("Empty ResultSet");
              }
              m_pDatabaseLayer->CloseResultSet(pResultSet);
            }
            else
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif

        }
      }


                  
      /* Test that a binary blob (from a file) is saved to and restored from the database correctly
       */
      void testBinaryBlobParametersPreparedRetrieval()
      {
        puts("testBinaryBlobParametersPreparedRetrieval");
        // Save one of the wxWidgets samples image files into a database table
        // Retrieve the image from the database and save it to a file
        // Do a MD5 sum on the original file and the new file to make sure that they're equal

        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE4);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table4;"));

          m_pDatabaseLayer->RunQuery(_("DELETE FROM table4;")); // Be extra sure the table is empty

          // Use a prepared statement to insert a binary BLOB into the database
          wxString strSQL = _("INSERT INTO table4 (IntCol, BlobCol) VALUES (1, ?);");
          int nDataLength = 0;
          wxMemoryBuffer BufferIn;
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(strSQL);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            ASSERT_EQUALS( 1, pStatement->GetParameterCount() );

            wxFile FileIn(_("wxwin.png"));
            nDataLength = FileIn.Length();
            int nBytesRead = FileIn.Read(BufferIn.GetWriteBuf(nDataLength), nDataLength);
            ASSERT_EQUALS(nBytesRead, nDataLength);
            pStatement->SetParamBlob(1, (unsigned char*)BufferIn.GetData(), nDataLength);
            DatabaseErrorCheck(pStatement);
            pStatement->RunQuery();
            DatabaseErrorCheck(pStatement);
            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }

          pStatement = m_pDatabaseLayer->PrepareStatement(_("SELECT BlobCol FROM table4 WHERE IntCol = 1"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            DatabaseResultSet* pResultSet = pStatement->RunQueryWithResults();
            ASSERT(pResultSet != NULL);
            if (pResultSet)
            {
              DatabaseErrorCheck(pResultSet);
              if (pResultSet->Next())
              {
                DatabaseErrorCheck(pResultSet);
                wxMemoryBuffer BufferOut;
                void* pBlob = pResultSet->GetResultBlob(_("BlobCol"), BufferOut);
                ASSERT_EQUALS(nDataLength, (int)BufferOut.GetBufSize());
                ASSERT_EQUALS(nDataLength, (int)BufferOut.GetDataLen());

                // Iterate through each byte and compare it to the source
                bool bMatch = TRUE;
                char* pOriginalBuffer = (char*)(BufferIn.GetData());
                char* pRetrievedBuffer = (char*)pBlob;
                wxString errorMessage;
                for (int i=0; i<nDataLength && bMatch; i++)
                {
                  if (*pOriginalBuffer != *pRetrievedBuffer)
                  {
                    errorMessage.Printf(_("Retrieved Blob does NOT match original Blob.  Error at byte %d"), i);
                    bMatch = FALSE;
                  }
                  pOriginalBuffer++;
                  pRetrievedBuffer++;
                }
                ASSERT_MESSAGE( bMatch, errorMessage.mb_str() );
              }
              else
              {
                FAIL("Empty ResultSet");
              }
              m_pDatabaseLayer->CloseResultSet(pResultSet);
            }
            else
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }

            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif

        }
      }


      /* Test using a prepared statement multiple times changing parameters between running
       */
      void testChangePreparedStatementParameters( void )
      {
        puts("testChangePreparedStatementParameters");
        ASSERT( m_pDatabaseLayer != NULL );
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          m_pDatabaseLayer->RunQuery(_("DELETE FROM table1;")); // Be extra sure the table is empty
          // Get the initial row count
          int nCount = GetRowCount();
          ASSERT_EQUALS( 0, nCount );
        
          // Add two rows to the database table
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("INSERT INTO table1 (column1, column2) VALUES (?, ?);"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            pStatement->SetParamString(1, _("ONE"));
            DatabaseErrorCheck(pStatement);
            pStatement->SetParamString(2, _("TWO"));
            DatabaseErrorCheck(pStatement);
            pStatement->RunQuery();
            DatabaseErrorCheck(pStatement);

            pStatement->SetParamString(1, _("THREE"));
            DatabaseErrorCheck(pStatement);
            pStatement->SetParamString(2, _("FOUR"));
            DatabaseErrorCheck(pStatement);
            pStatement->RunQuery();
            DatabaseErrorCheck(pStatement);
            
            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }

          // Get the updated row count
          nCount = GetRowCount();
          ASSERT_EQUALS( 2, nCount );

          // Verify that two records are in the table and the contents are ("ONE", "TWO") and ("THREE", "FOUR") respectively
          DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT * FROM table1 ORDER BY column1;"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pResultSet != NULL);
          if (pResultSet)
          {
            DatabaseErrorCheck(pResultSet);
            // First row
            if (pResultSet->Next())
            {
              DatabaseErrorCheck(pResultSet);
              ASSERT_EQUALS(wxString(_("ONE")), pResultSet->GetResultString(_("column1")));
              ASSERT_EQUALS(wxString(_("TWO")), pResultSet->GetResultString(_("column2")));
            }
            else
            {
              FAIL("Empty ResultSet");
            }
            
            // Second row
            if (pResultSet->Next())
            {
              DatabaseErrorCheck(pResultSet);
              ASSERT_EQUALS(wxString(_("THREE")), pResultSet->GetResultString(_("column1")));
              ASSERT_EQUALS(wxString(_("FOUR")), pResultSet->GetResultString(_("column2")));
            }
            else
            {
              FAIL("Empty ResultSet");
            }

            m_pDatabaseLayer->CloseResultSet(pResultSet);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      /* Test setting prepared statement parameters out of order
       */
      void testSetPreparedStatementParametersOutOfOrder( void )
      {
        puts("testSetPreparedStatementParametersOutOfOrder");
        ASSERT( m_pDatabaseLayer != NULL );
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          // Get the initial row count
          int nCount = GetRowCount();
          ASSERT_EQUALS( 0, nCount );
        
          // Add a new row to the database table
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("INSERT INTO table1 (column1, column2) VALUES (?, ?);"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            pStatement->SetParamString(2, _("TWO"));
            DatabaseErrorCheck(pStatement);
            pStatement->SetParamString(1, _("ONE"));
            DatabaseErrorCheck(pStatement);
            pStatement->RunQuery();
            DatabaseErrorCheck(pStatement);

            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }

          // Get the updated row count
          nCount = GetRowCount();
          ASSERT_EQUALS( 1, nCount );
 
          DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT * FROM table1;"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pResultSet != NULL);
          if (pResultSet)
          {
            DatabaseErrorCheck(pResultSet);
            // First row
            if (pResultSet->Next())
            {
              DatabaseErrorCheck(pResultSet);
              ASSERT_EQUALS(wxString(_("ONE")), pResultSet->GetResultString(_("column1")));
              ASSERT_EQUALS(wxString(_("TWO")), pResultSet->GetResultString(_("column2")));
            }
            else
            {
              FAIL("Empty ResultSet");
            }
            
            m_pDatabaseLayer->CloseResultSet(pResultSet);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      
      /* Test that a binary blob (from a file) is saved to and restored from the database correctly
       */
      void testManyBlobRecordRetrieval()
      {
        puts("testManyBlobRecordRetrieval");
        // Save one of the wxWidgets samples image files into a database table
        // Retrieve the image from the database and save it to a file
        // Do a MD5 sum on the original file and the new file to make sure that they're equal

        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE4);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table4;"));

          m_pDatabaseLayer->RunQuery(_("DELETE FROM table4;")); // Be extra sure the table is empty

#ifdef TEST_THOUSAND_BLOBS
          int nBlobRecords = 1000;
#else
          int nBlobRecords = 10;
#endif
          wxFile FileIn(_("wxwin.png"));
          int nDataLength = FileIn.Length();
          wxMemoryBuffer BufferIn;
          int nBytesRead = FileIn.Read(BufferIn.GetWriteBuf(nDataLength), nDataLength);
          ASSERT_EQUALS(nBytesRead, nDataLength);

          // Use a prepared statement to insert a binary BLOB into the database
          wxString strSQL = _("INSERT INTO table4 (IntCol, BlobCol) VALUES (1, ?);");
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(strSQL);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            ASSERT_EQUALS( 1, pStatement->GetParameterCount() );

            pStatement->SetParamBlob(1, (unsigned char*)BufferIn.GetData(), nDataLength);
            DatabaseErrorCheck(pStatement);
            for (int i=0; i<nBlobRecords; i++)
            {
              //wxPrintf(_("Inserting BLOB record\n");
              pStatement->RunQuery();
              DatabaseErrorCheck(pStatement);
            }
            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }

          pStatement = m_pDatabaseLayer->PrepareStatement(_("SELECT BlobCol FROM table4 WHERE IntCol = 1"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            DatabaseResultSet* pResultSet = pStatement->RunQueryWithResults();
            ASSERT(pResultSet != NULL);
            if (pResultSet)
            {
              DatabaseErrorCheck(pResultSet);
              int nRecordsRetrieved = 0;
              while (pResultSet->Next())
              {
                DatabaseErrorCheck(pResultSet);
                nRecordsRetrieved++;
                wxMemoryBuffer BufferOut;
                void* pBlob = pResultSet->GetResultBlob(_("BlobCol"), BufferOut);
                ASSERT_EQUALS(nDataLength, (int)BufferOut.GetBufSize());
                ASSERT_EQUALS(nDataLength, (int)BufferOut.GetDataLen());

                // Iterate through each byte and compare it to the source
                bool bMatch = TRUE;
                char* pOriginalBuffer = (char*)(BufferIn.GetData());
                char* pRetrievedBuffer = (char*)pBlob;
                wxString errorMessage;
                for (int i=0; i<nDataLength && bMatch; i++)
                {
                  if (*pOriginalBuffer != *pRetrievedBuffer)
                  {
                    errorMessage.Printf(_("Retrieved Blob does NOT match original Blob.  Error at byte %d"), i);
                    bMatch = FALSE;
                  }
                  pOriginalBuffer++;
                  pRetrievedBuffer++;
                }
                ASSERT_MESSAGE( bMatch, errorMessage.mb_str() );
              }
              ASSERT_EQUALS( nBlobRecords, nRecordsRetrieved );
              m_pDatabaseLayer->CloseResultSet(pResultSet);
            }
            else
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }

            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif

        }
      }

      /* Test looping re-using a prepared statement through a loop
       */
      void testLoopingPreparedStatement()
      {
        puts("testLoopingPreparedStatement");
        // Save one of the wxWidgets samples image files into a database table
        // Retrieve the image from the database and save it to a file
        // Do a MD5 sum on the original file and the new file to make sure that they're equal

        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          m_pDatabaseLayer->RunQuery(_("DELETE FROM table1;")); // Be extra sure the table is empty
          // Get the initial row count
          int nCount = GetRowCount();
          ASSERT_EQUALS( 0, nCount );
        
          wxString strColumnOneValues[] = { _("One"), _("Two"), _("Three"), _("Four") };
          wxString strColumnTwoValues[] = { _("1"), _("2"), _("3"), _("4") };
          
          // Add two rows to the database table
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("INSERT INTO table1 (column1, column2) VALUES (?, ?);"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            for (unsigned int i=0; i<WXSIZEOF(strColumnOneValues); i++)
            {
              pStatement->SetParamString(1, strColumnOneValues[i]);
              DatabaseErrorCheck(pStatement);
              pStatement->SetParamString(2, strColumnTwoValues[i]);
              DatabaseErrorCheck(pStatement);
              pStatement->RunQuery();
              DatabaseErrorCheck(pStatement);
            }
            m_pDatabaseLayer->CloseStatement(pStatement);
            
            nCount = GetRowCount();
            ASSERT_EQUALS((int)WXSIZEOF(strColumnOneValues), nCount);

            DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT * FROM table1 ORDER BY column2"));
            DatabaseErrorCheck(m_pDatabaseLayer);
            ASSERT(pResultSet != NULL);
            if (pResultSet)
            {
              DatabaseErrorCheck(pResultSet);
              for (unsigned int nCounter = 0; nCounter<WXSIZEOF(strColumnOneValues); nCounter++)
              {
                if (pResultSet->Next())
                {
                  DatabaseErrorCheck(pResultSet);
                  wxString strColumn1 = pResultSet->GetResultString(_("column1"));
                  wxString strColumn2 = pResultSet->GetResultString(_("column2"));

                  ASSERT_EQUALS(strColumnOneValues[nCounter], strColumn1);
                  ASSERT_EQUALS(strColumnTwoValues[nCounter], strColumn2);
                }
                else
                {
                  FAIL("Empty ResultSet");
                }
              }
              m_pDatabaseLayer->CloseResultSet(pResultSet);
            }
            else
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }


      /* Test looping re-using a prepared statement through a loop
       */
      void testLoopingPreparedStatementRetrieval()
      {
        puts("testLoopingPreparedStatementRetrieval");
        // Save one of the wxWidgets samples image files into a database table
        // Retrieve the image from the database and save it to a file
        // Do a MD5 sum on the original file and the new file to make sure that they're equal

        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          m_pDatabaseLayer->RunQuery(_("DELETE FROM table1;")); // Be extra sure the table is empty
          DatabaseErrorCheck(m_pDatabaseLayer);
          // Get the initial row count
          int nCount = GetRowCount();
          ASSERT_EQUALS( 0, nCount );
        
          wxString strColumnOneValues[] = { _("One"), _("Two"), _("Three"), _("Four") };
          wxString strColumnTwoValues[] = { _("1"), _("2"), _("3"), _("4") };
          
          // Add two rows to the database table
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("INSERT INTO table1 (column1, column2) VALUES (?, ?);"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            for (unsigned int i=0; i<WXSIZEOF(strColumnOneValues); i++)
            {
              pStatement->SetParamString(1, strColumnOneValues[i]);
              DatabaseErrorCheck(pStatement);
              pStatement->SetParamString(2, strColumnTwoValues[i]);
              DatabaseErrorCheck(pStatement);
              pStatement->RunQuery();
              DatabaseErrorCheck(pStatement);
            }
            m_pDatabaseLayer->CloseStatement(pStatement);
            
            nCount = GetRowCount();
            ASSERT_EQUALS((int)WXSIZEOF(strColumnOneValues), nCount);

            pStatement = m_pDatabaseLayer->PrepareStatement(_("SELECT * FROM table1 WHERE column1 = ?"));
            DatabaseErrorCheck(m_pDatabaseLayer);
            ASSERT(pStatement != NULL);
            if (pStatement)
            {
              for (unsigned int i=0; i<WXSIZEOF(strColumnOneValues); i++)
              {
                pStatement->SetParamString(1, strColumnOneValues[i]);
                DatabaseErrorCheck(pStatement);
                DatabaseResultSet* pResultSet = pStatement->RunQueryWithResults();
                DatabaseErrorCheck(pStatement);
                ASSERT(pResultSet != NULL);
                if (pResultSet)
                {
                  DatabaseErrorCheck(pResultSet);
                  if (pResultSet->Next())
                  {
                    DatabaseErrorCheck(pResultSet);
                    wxString strColumn1 = pResultSet->GetResultString(_("column1"));
                    wxString strColumn2 = pResultSet->GetResultString(_("column2"));
  
                    ASSERT_EQUALS(strColumnOneValues[i], strColumn1);
                    ASSERT_EQUALS(strColumnTwoValues[i], strColumn2);
                  }
                  else
                  {
                    ASSERT_MESSAGE(false, wxString::Format(_("No records found for column1 = '%s'\n"), strColumnOneValues[i].c_str()).mb_str());
                  }
                  m_pDatabaseLayer->CloseResultSet(pResultSet);
                }
                else
                {
                  puts("Error generating DatabaseResultSet");
                  FAIL("Error generating DatabaseResultSet");
                }
              }
              m_pDatabaseLayer->CloseStatement(pStatement);
            }
            else
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      /* Test to make sure that DatabaseResultSet::Next returns false when it should
       */
      void testResultSetNext()
      {
        puts("testResultSetNext");
        ASSERT( m_pDatabaseLayer != NULL );
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          // Get the initial row count
          int nCount = GetRowCount();
          wxPrintf(_("Checking if row count = 0\n"));
          ASSERT_EQUALS( 0, nCount );
        
          // Add a new row to the database table
          wxString strSQL = _("INSERT INTO table1 (column1, column2) VALUES ('ONE', 'TWO');");

          m_pDatabaseLayer->RunQuery(strSQL);
          DatabaseErrorCheck(m_pDatabaseLayer);
          m_pDatabaseLayer->RunQuery(strSQL);
          DatabaseErrorCheck(m_pDatabaseLayer);
          m_pDatabaseLayer->RunQuery(strSQL);
          DatabaseErrorCheck(m_pDatabaseLayer);

          // Get the updated row count
          nCount = GetRowCount();
          wxPrintf(_("Checking if row count = 3\n"));
          ASSERT_EQUALS( 3, nCount );

          DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT * FROM table1"));
          DatabaseErrorCheck(m_pDatabaseLayer);

          ASSERT(pResultSet != NULL);
          if (pResultSet)
          {
            DatabaseErrorCheck(pResultSet);
            int nNextCounter = 0;
            while (pResultSet->Next())
            {
              DatabaseErrorCheck(pResultSet);
              nNextCounter++;
            }
            wxPrintf(_("Checking if %d == %d\n"), nCount, nNextCounter);
            ASSERT(nCount == nNextCounter);

            m_pDatabaseLayer->CloseResultSet(pResultSet);
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }
      
      void testSqlSyntaxError()
      {
        puts("testSqlSyntaxError");
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          m_pDatabaseLayer->RunQuery(_("SELECT SELECT FROM table1;"));
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          FAIL("SQL Syntax error should have been thrown");
#else
          ASSERT_EQUALS( DATABASE_LAYER_SQL_SYNTAX_ERROR, m_pDatabaseLayer->GetErrorCode() );
#endif
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ASSERT_EQUALS( DATABASE_LAYER_SQL_SYNTAX_ERROR, e.GetErrorCode() );
          }
#endif
      }

      void testPreparedStatementSqlSyntaxError()
      {
        puts("testPreparedStatementSqlSyntaxError");
        PreparedStatement* pStatement = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          pStatement = m_pDatabaseLayer->PrepareStatement(_("SELECT SELECT FROM table1;"));
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          FAIL("SQL Syntax error should have been thrown");
#else
          ASSERT_EQUALS( DATABASE_LAYER_SQL_SYNTAX_ERROR, m_pDatabaseLayer->GetErrorCode() );
#endif
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ASSERT_EQUALS( DATABASE_LAYER_SQL_SYNTAX_ERROR, e.GetErrorCode() );
          }
#endif
          if (pStatement != NULL)
          {
            m_pDatabaseLayer->CloseStatement(pStatement);
          }
      }
      
      void testInvalidUserError()
      {
        puts("testInvalidUserError");
        ASSERT_MESSAGE(true, "Placeholder");
      }
      
      void testBadPasswordError()
      {
        puts("testBadPasswordError");
        ASSERT_MESSAGE(true, "Placeholder");
      }
      
      void testReadOnlyError()
      {
        puts("testReadOnlyError");
        ASSERT_MESSAGE(true, "Placeholder");
      }
      
      void testFieldNotInResultSetError()
      {
        puts("testFieldNotInResultSetError");
        ASSERT( m_pDatabaseLayer != NULL );
        DatabaseResultSet* pResultSet = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT column1 FROM table1;"));
          
          ASSERT(pResultSet != NULL);
          wxString strValue = pResultSet->GetResultString(_("column2"));
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          FAIL("DatabaseLayer error should have been thrown");
#else
          ASSERT_EQUALS( DATABASE_LAYER_FIELD_NOT_IN_RESULTSET, m_pDatabaseLayer->GetErrorCode() );
#endif
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ASSERT_EQUALS( DATABASE_LAYER_FIELD_NOT_IN_RESULTSET, e.GetErrorCode() );
          }
#endif
          if (pResultSet != NULL)
            m_pDatabaseLayer->CloseResultSet(pResultSet);
      }

      void testNullValues()
      {
        puts("testNullValues");
        ASSERT( m_pDatabaseLayer != NULL );
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif

          // Add a table just for this test
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE7);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table7;"));
          
          m_pDatabaseLayer->RunQuery(_("DELETE FROM table7;")); // Be extra sure the table is empty

          // Insert 1 record in the table with a key followed by all NULL values
          m_pDatabaseLayer->RunQuery(_("INSERT INTO table7 (KeyCol, IntCol, DoubleCol, StringCol, BlobCol, DateCol) VALUES (1, NULL, NULL, NULL, NULL, NULL);"));

          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("SELECT * FROM table7 WHERE KeyCol = 1;"));
          ASSERT(pStatement != NULL);
          DatabaseErrorCheck(pStatement);
          if (pStatement)
          {
            DatabaseResultSet* pResultSet = pStatement->RunQueryWithResults();
            ASSERT(pResultSet != NULL);
            DatabaseErrorCheck(pResultSet);
            if (pResultSet)
            {
              if (pResultSet->Next())
              {
                int intValue = pResultSet->GetResultInt(_("IntCol"));
                ASSERT_EQUALS(0, intValue);

                wxString stringValue = pResultSet->GetResultString(_("StringCol"));
                ASSERT_EQUALS(wxString(_("")), stringValue);
                
                double doubleValue = pResultSet->GetResultDouble(_("DoubleCol"));
                ASSERT_EQUALS_EPSILON(0, doubleValue, 0.0001);
                
                wxDateTime dateValue = pResultSet->GetResultDate(_("DateCol"));
                ASSERT(!dateValue.IsValid());
                
                wxMemoryBuffer BufferOut;
                void* pBlob = pResultSet->GetResultBlob(_("BlobCol"), BufferOut);
                
                ASSERT_MESSAGE((pBlob == NULL), "Blob field was not NULL which was expected");
              }
              else
              {
                m_pDatabaseLayer->CloseResultSet(pResultSet);
                m_pDatabaseLayer->CloseStatement(pStatement);
                FAIL("Error: No results found when inserting a table row with all NULL values");
              }

              m_pDatabaseLayer->CloseResultSet(pResultSet);
            }
            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif

        }
      }

      void testNullParameters()
      {
        puts("testNullParameters");
        ASSERT( m_pDatabaseLayer != NULL );
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif

          // Add a table just for this test
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE7);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table7;"));
          
          m_pDatabaseLayer->RunQuery(_("DELETE FROM table7;")); // Be extra sure the table is empty

          // Insert 1 record in the table with a key followed by all NULL values
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("INSERT INTO table7 (KeyCol, IntCol, DoubleCol, StringCol, BlobCol, DateCol) VALUES (1, ?, ?, ?, ?, ?);"));
          ASSERT(pStatement != NULL);
          DatabaseErrorCheck(pStatement);
          if (pStatement)
          {
            pStatement->SetParamNull(1);
            pStatement->SetParamNull(2);
            pStatement->SetParamNull(3);
            pStatement->SetParamNull(4);
            pStatement->SetParamNull(5);
            pStatement->RunQuery();

            m_pDatabaseLayer->CloseStatement(pStatement);
            pStatement = NULL;
          }

          DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT * FROM table7 WHERE KeyCol = 1;"));
          ASSERT(pResultSet != NULL);
          DatabaseErrorCheck(pResultSet);
          if (pResultSet)
          {
            if (pResultSet->Next())
            {
              int intValue = pResultSet->GetResultInt(_("IntCol"));
              ASSERT_EQUALS(0, intValue);

              wxString stringValue = pResultSet->GetResultString(_("StringCol"));
              ASSERT_EQUALS(wxString(_("")), stringValue);
              
              double doubleValue = pResultSet->GetResultDouble(_("DoubleCol"));
              ASSERT_EQUALS_EPSILON(0, doubleValue, 0.0001);
              
              wxDateTime dateValue = pResultSet->GetResultDate(_("DateCol"));
              ASSERT(!dateValue.IsValid());
              
              wxMemoryBuffer BufferOut;
              void* pBlob = pResultSet->GetResultBlob(_("BlobCol"), BufferOut);
              
              ASSERT_MESSAGE((pBlob == NULL), "Blob field was not NULL which was expected");
            }
            else
            {
              FAIL("Error: No results found when inserting a table row with all NULL values");
            }

            m_pDatabaseLayer->CloseResultSet(pResultSet);
          }
          
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif

        }
      }

      void testSingleLinePreparedStatement()
      {
        puts("testSingleLinePreparedStatement");
        ASSERT( m_pDatabaseLayer != NULL );
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          // Get the initial row count
          int nCount = GetRowCount();
          ASSERT_EQUALS( 0, nCount );
        
          // Add a new row to the database table
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("INSERT INTO table1 (column1, column2) VALUES (?, ?); ; "));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            ASSERT(m_pDatabaseLayer->GetErrorCode() == DATABASE_LAYER_OK);
            if (m_pDatabaseLayer->GetErrorCode() != DATABASE_LAYER_OK)
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }
            else
            {
              pStatement->SetParamString(1, _("ONE"));
              DatabaseErrorCheck(pStatement);
              pStatement->SetParamString(2, _("TWO"));
              DatabaseErrorCheck(pStatement);
              pStatement->RunQuery();
              DatabaseErrorCheck(pStatement);
            }
            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }

          // Get the updated row count
          nCount = GetRowCount();
          ASSERT_EQUALS( 1, nCount );
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      void testEncodingSupport( void )
     {
       puts("testEncodingSupport");
       ASSERT( m_pDatabaseLayer != NULL );
       if (m_pDatabaseLayer)
       {
         wxMBConv* oldEncoding = wxConvCurrent;
         wxConvCurrent = &wxConvISO8859_1;

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
         try
         {
#endif

          // Add a table just for this test
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
         SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE5);
         if (createTableSql != m_SqlMap.end())
         {
           for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
           {
             m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
           }
         }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          
         // Make sure that the tearDown function knows to remove this test table
         m_DeleteStatements.push_back(_("DROP TABLE table5;"));

         m_pDatabaseLayer->RunQuery(_("DELETE FROM table5;")); // Be extra sure the table is empty

         DatabaseErrorCheck(m_pDatabaseLayer);

         wxString sep = _("\t");
         wxFileInputStream fileIn(_("test.dat"));
#if wxUSE_UNICODE
         wxTextInputStream textIn(fileIn, sep, wxConvISO8859_1);
#else
         wxTextInputStream textIn(fileIn, sep);
#endif
         wxString testString(textIn.ReadLine());

         // Add a new row to the database table
         wxString strSQL = _("INSERT INTO table5 (stringField) VALUES ('") + testString + _("');");
         m_pDatabaseLayer->RunQuery(strSQL);
         DatabaseErrorCheck(m_pDatabaseLayer);

         strSQL = _("SELECT stringField FROM table5");
         DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(strSQL);
         DatabaseErrorCheck(m_pDatabaseLayer);
         ASSERT(pResultSet != NULL);
         if (pResultSet)
         {
           DatabaseErrorCheck(pResultSet);
           if (pResultSet->Next())
           {
             DatabaseErrorCheck(pResultSet);
             wxString returnString = pResultSet->GetResultString(_("stringField"));
             ASSERT_EQUALS( testString, returnString );
             wxFileOutputStream out(_("test_out.dat"));
#if wxUSE_UNICODE
             wxTextOutputStream testOut(out, wxEOL_NATIVE, wxConvISO8859_1);
#else
             wxTextOutputStream testOut(out);
#endif
             testOut.WriteString(returnString);
             out.Close();

             wxStructStat fileStats;
             wxStat(_("test_out.dat"), &fileStats);
             ASSERT_EQUALS(10, (int)fileStats.st_size);
           }
           else
           {
             m_pDatabaseLayer->CloseResultSet(pResultSet);
             FAIL("No records found");
           }
           m_pDatabaseLayer->CloseResultSet(pResultSet);
         }
         else
         {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
         }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
         }
         catch (DatabaseLayerException& e)
         {
           ProcessException(e);
         }
#endif

         wxConvCurrent = oldEncoding;
       }
     }


      void testParameterEncodingSupport( void )
     {
       puts("testParameterEncodingSupport");
       ASSERT( m_pDatabaseLayer != NULL );
       if (m_pDatabaseLayer)
       {
         wxMBConv* oldEncoding = wxConvCurrent;
         wxConvCurrent = &wxConvISO8859_1;

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
         try
         {
#endif

          // Add a table just for this test
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
         SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE5);
         if (createTableSql != m_SqlMap.end())
         {
           for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
           {
             m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
           }
         }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          
         // Make sure that the tearDown function knows to remove this test table
         m_DeleteStatements.push_back(_("DROP TABLE table5;"));

         m_pDatabaseLayer->RunQuery(_("DELETE FROM table5;")); // Be extra sure the table is empty

         DatabaseErrorCheck(m_pDatabaseLayer);

         wxString sep = _("\t");
         wxFileInputStream fileIn(_("test.dat"));
#if wxUSE_UNICODE
         wxTextInputStream textIn(fileIn, sep, wxConvISO8859_1);
#else
         wxTextInputStream textIn(fileIn, sep);
#endif
         wxString testString(textIn.ReadLine());

         // Add a new row to the database table
         PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("INSERT INTO table5 (stringField) VALUES (?);"));
         DatabaseErrorCheck(m_pDatabaseLayer);
         ASSERT(pStatement != NULL);
         if (pStatement)
         {
           pStatement->SetParamString(1, testString);
           pStatement->RunQuery();
           m_pDatabaseLayer->CloseStatement(pStatement);
         }

         DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT stringField FROM table5"));
         DatabaseErrorCheck(m_pDatabaseLayer);
         ASSERT(pResultSet != NULL);
          if (pResultSet)
         {
           DatabaseErrorCheck(pResultSet);
           if (pResultSet->Next())
           {
             DatabaseErrorCheck(pResultSet);
             wxString returnString = pResultSet->GetResultString(_("stringField"));
             ASSERT_EQUALS( testString, returnString );
             wxFileOutputStream out(_("test_out.dat"));
#if wxUSE_UNICODE
             wxTextOutputStream testOut(out, wxEOL_NATIVE, wxConvISO8859_1);
#else
             wxTextOutputStream testOut(out);
#endif
             testOut.WriteString(returnString);
             out.Close();

             wxStructStat fileStats;
             wxStat(_("test_out.dat"), &fileStats);
             ASSERT_EQUALS(10, (int)fileStats.st_size);
           }
           else
           {
             m_pDatabaseLayer->CloseResultSet(pResultSet);
             FAIL("No records found");
           }
           m_pDatabaseLayer->CloseResultSet(pResultSet);
         }
         else
         {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
         }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
         }
         catch (DatabaseLayerException& e)
         {
           ProcessException(e);
         }
#endif

         wxConvCurrent = oldEncoding;
       }
     }

      /* Test result set from simple insert
       */
      void testResultsByIndex( void )
      {
        puts("testResultsByIndex");
        ASSERT( m_pDatabaseLayer != NULL );
        
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          // Add a table just for this test
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE2);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table2;"));

          m_pDatabaseLayer->RunQuery(_("DELETE FROM table2;")); // Be extra sure the table is empty

          m_pDatabaseLayer->RunQuery(_("INSERT INTO table2 (IntCol, DoubleCol, StringCol) VALUES (10, 1.234, 'Test3');"));

          DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT IntCol, DoubleCol, StringCol FROM table2;"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pResultSet != NULL);
          if (pResultSet)
          {
            DatabaseErrorCheck(pResultSet);
            if (pResultSet->Next())
            {
              DatabaseErrorCheck(pResultSet);
              wxPrintf(_("Asserting IntCol = 10\n"));
              ASSERT_EQUALS( 10, pResultSet->GetResultInt(1));
              wxPrintf(_("Asserting DoubleCol = 1.234\n"));
              ASSERT_EQUALS_EPSILON( 1.234, pResultSet->GetResultDouble(2), 0.000001);
              wxPrintf(_("Asserting StringCol = 'Test3'\n"));
              ASSERT_EQUALS(wxString(_("Test3")), pResultSet->GetResultString(3));
            }
            else
            {
              FAIL("Empty ResultSet");
            }
            m_pDatabaseLayer->CloseResultSet(pResultSet);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }
      
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif

        }
      }
      
      /* Test result set from a Prepared statement
       */
      void testPreparedStatementResultsByIndex( void )
      {
        puts("testPreparedStatementResultsByIndex");
        ASSERT( m_pDatabaseLayer != NULL );
        
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif

          // Add a table just for this test
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE6);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
          DatabaseErrorCheck(m_pDatabaseLayer);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table6;"));

          m_pDatabaseLayer->RunQuery(_("DELETE FROM table6;")); // Be extra sure the table is empty
          DatabaseErrorCheck(m_pDatabaseLayer);

          wxString strSQL = _("INSERT INTO table6 (IntCol, DoubleCol, StringCol, BlobCol) VALUES (?, ?, ?, ?); SELECT IntCol, DoubleCol, StringCol, BlobCol FROM table6;");
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(strSQL);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            wxPrintf(_("Asserting parameter count = 4\n"));
            ASSERT_EQUALS( 4, pStatement->GetParameterCount() );
            pStatement->SetParamInt(1, 10);
            DatabaseErrorCheck(pStatement);
            pStatement->SetParamDouble(2, 1.234);
            DatabaseErrorCheck(pStatement);
            pStatement->SetParamString(3, _("Test4"));
            DatabaseErrorCheck(pStatement);
            wxString strBlob = _("Test Blob");
            // Use the length + 1 so that we get the null terminating character at the end of the string
            wxCharBuffer charBuffer = strBlob.mb_str(*wxConvCurrent);
            pStatement->SetParamBlob(4, (const unsigned char*)(const char*)charBuffer, strBlob.Length()+1);
            DatabaseErrorCheck(pStatement);

            DatabaseResultSet* pResultSet = pStatement->RunQueryWithResults();
            DatabaseErrorCheck(pStatement);
            ASSERT(pResultSet != NULL);
            if (pResultSet)
            {
              DatabaseErrorCheck(pResultSet);
              if (pResultSet->Next())
              {
                DatabaseErrorCheck(pResultSet);
                wxPrintf(_("Asserting IntCol = 10\n"));
                ASSERT_EQUALS( 10, pResultSet->GetResultInt(1));
                wxPrintf(_("Asserting DoubleCol = 1.234\n"));
                ASSERT_EQUALS_EPSILON( 1.234, pResultSet->GetResultDouble(2), 0.000001);
                wxPrintf(_("Asserting StringCol = 'Test4'\n"));
                ASSERT_EQUALS(wxString(_("Test4")), pResultSet->GetResultString(3));
                wxMemoryBuffer buffer;
                pResultSet->GetResultBlob(4, buffer);
                wxString strResult((char*)buffer.GetData(), *wxConvCurrent);
                wxPrintf(_("Asserting BlobCol = 'Test Blob'\n"));
                ASSERT_EQUALS( strBlob, strResult );
              }
              else
              {
                FAIL("Empty ResultSet");
              }
              m_pDatabaseLayer->CloseResultSet(pResultSet);
            }
            else
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }
            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }
      
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif

        }
      }


      /* Test result set metadata
       */
      void testResultSetMetaData( void )
      {
        puts("testResultSetMetaData");
        ASSERT( m_pDatabaseLayer != NULL );
        
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
            CreateTable8();
            PopulateTable8();

            DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT * FROM table8"));
            ASSERT(pResultSet != NULL);
            DatabaseErrorCheck(pResultSet);
            if (pResultSet)
            {
              // Now do the meta data test
              ResultSetMetaData* pMetaData = pResultSet->GetMetaData();
              ASSERT(pMetaData != NULL);
              if (pMetaData)
              {
                // Check column count
                ASSERT_EQUALS(5, pMetaData->GetColumnCount());
                
                // Check column types
                ASSERT_EQUALS((int)(ResultSetMetaData::COLUMN_INTEGER), pMetaData->GetColumnType(1));
                ASSERT_EQUALS((int)(ResultSetMetaData::COLUMN_DOUBLE), pMetaData->GetColumnType(2));
                ASSERT_EQUALS((int)(ResultSetMetaData::COLUMN_STRING), pMetaData->GetColumnType(3));
                ASSERT_EQUALS((int)(ResultSetMetaData::COLUMN_BLOB), pMetaData->GetColumnType(4));
                ASSERT_EQUALS((int)(ResultSetMetaData::COLUMN_DATE), pMetaData->GetColumnType(5));

                // Check column names
                ASSERT_EQUALS(wxString(_("INTCOL")), pMetaData->GetColumnName(1).Upper());
                ASSERT_EQUALS(wxString(_("DOUBLECOL")), pMetaData->GetColumnName(2).Upper());
                ASSERT_EQUALS(wxString(_("STRINGCOL")), pMetaData->GetColumnName(3).Upper());
                ASSERT_EQUALS(wxString(_("BLOBCOL")), pMetaData->GetColumnName(4).Upper());
                ASSERT_EQUALS(wxString(_("DATECOL")), pMetaData->GetColumnName(5).Upper());

                pResultSet->CloseMetaData(pMetaData);
              }
              m_pDatabaseLayer->CloseResultSet(pResultSet);
            }
          
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif

        }
      }


      /* Test result set metadata
       */
      void testResultSetMetaDataForNullValues( void )
      {
        puts("testResultSetMetaDataForNullValues");
        ASSERT( m_pDatabaseLayer != NULL );
        
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  

          // Add a table just for this test
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE8);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif

          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table8;"));

          m_pDatabaseLayer->RunQuery(_("DELETE FROM table8;")); // Be extra sure the table is empty
          DatabaseErrorCheck(m_pDatabaseLayer);

          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("INSERT INTO table8 (IntCol, DoubleCol, StringCol, BlobCol, DateCol) VALUES (?, ?, ?, ?, ?)"));
          
          ASSERT(pStatement != NULL);
          DatabaseErrorCheck(pStatement);
          if (pStatement)
          {
            pStatement->SetParamNull(1);
            DatabaseErrorCheck(pStatement);
            pStatement->SetParamNull(2);
            DatabaseErrorCheck(pStatement);
            pStatement->SetParamNull(3);
            DatabaseErrorCheck(pStatement);
            pStatement->SetParamNull(4);
            DatabaseErrorCheck(pStatement);
            pStatement->SetParamNull(5);
            DatabaseErrorCheck(pStatement);
            pStatement->RunQuery();
            m_pDatabaseLayer->CloseStatement(pStatement);
            
            DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT * FROM table8"));
            ASSERT(pResultSet != NULL);
            DatabaseErrorCheck(pResultSet);
            if (pResultSet)
            {
              // Now do the meta data test
              ResultSetMetaData* pMetaData = pResultSet->GetMetaData();
              ASSERT(pMetaData != NULL);
              if (pMetaData)
              {
                // Check column count
                ASSERT_EQUALS(5, pMetaData->GetColumnCount());
                
                // Check column types
                ASSERT_EQUALS((int)(ResultSetMetaData::COLUMN_INTEGER), pMetaData->GetColumnType(1));
                ASSERT_EQUALS((int)(ResultSetMetaData::COLUMN_DOUBLE), pMetaData->GetColumnType(2));
                ASSERT_EQUALS((int)(ResultSetMetaData::COLUMN_STRING), pMetaData->GetColumnType(3));
                ASSERT_EQUALS((int)(ResultSetMetaData::COLUMN_BLOB), pMetaData->GetColumnType(4));
                ASSERT_EQUALS((int)(ResultSetMetaData::COLUMN_DATE), pMetaData->GetColumnType(5));

                // Check column names
                ASSERT_EQUALS(wxString(_("INTCOL")), pMetaData->GetColumnName(1).Upper());
                ASSERT_EQUALS(wxString(_("DOUBLECOL")), pMetaData->GetColumnName(2).Upper());
                ASSERT_EQUALS(wxString(_("STRINGCOL")), pMetaData->GetColumnName(3).Upper());
                ASSERT_EQUALS(wxString(_("BLOBCOL")), pMetaData->GetColumnName(4).Upper());
                ASSERT_EQUALS(wxString(_("DATECOL")), pMetaData->GetColumnName(5).Upper());

                pResultSet->CloseMetaData(pMetaData);
              }
              m_pDatabaseLayer->CloseResultSet(pResultSet);
            }
          }
          
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif

        }
      }


      /* Test NULL values in result set metadata
       */
      void testNullResultSetMetaData( void )
      {
        puts("testNullResultSetMetaData");
        ASSERT( m_pDatabaseLayer != NULL );
        
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  

          // Add a table just for this test
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE7);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif

          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table7;"));

          m_pDatabaseLayer->RunQuery(_("DELETE FROM table7;")); // Be extra sure the table is empty
          DatabaseErrorCheck(m_pDatabaseLayer);

          // Insert 1 record in the table with a key followed by all NULL values
          m_pDatabaseLayer->RunQuery(_("INSERT INTO table7 (KeyCol, IntCol, DoubleCol, StringCol, BlobCol, DateCol) VALUES (1, NULL, NULL, NULL, NULL, NULL)"));

          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("SELECT * FROM table7 WHERE KeyCol = 1"));
          ASSERT(pStatement != NULL);
          DatabaseErrorCheck(pStatement);
          if (pStatement)
          {
            DatabaseResultSet* pResultSet = pStatement->RunQueryWithResults();
            ASSERT(pResultSet != NULL);
            DatabaseErrorCheck(pResultSet);
            if (pResultSet)
            {
              if (pResultSet->Next())
              {
                int intValue = pResultSet->GetResultInt(_("IntCol"));
                ASSERT_EQUALS(0, intValue);

                wxString stringValue = pResultSet->GetResultString(_("StringCol"));
                ASSERT_EQUALS(wxString(_("")), stringValue);
                
                double doubleValue = pResultSet->GetResultDouble(_("DoubleCol"));
                ASSERT_EQUALS_EPSILON(0, doubleValue, 0.0001);
                
                wxDateTime dateValue = pResultSet->GetResultDate(_("DateCol"));
                ASSERT(!dateValue.IsValid());
                
                wxMemoryBuffer BufferOut;
                void* pBlob = pResultSet->GetResultBlob(_("BlobCol"), BufferOut);
                
                ASSERT_MESSAGE(pBlob == NULL,"Blob field was not NULL which was expected");
              }
              else
              {
                FAIL("Error: No results found when inserting a table row with all NULL values");
              }

              m_pDatabaseLayer->CloseResultSet(pResultSet);
            }
            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif

        }
      }

      /* Test alias columns in result set
       */
      void testResultSetAlias( void )
      {
        puts("testResultSetAlias");
        ASSERT( m_pDatabaseLayer != NULL );
        
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
            // Place the code for your test here
          // Add a new row to the database table
          m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('ONE', 'TWO');"));
          DatabaseErrorCheck(m_pDatabaseLayer);

          // Get the updated row count
          int nCount = GetRowCount();
          wxPrintf(_("Checking if row count = 1\n"));
          ASSERT_EQUALS( 1, nCount );

          DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT table1.column1 AS aliasColumn FROM table1;"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pResultSet != NULL);
          if (pResultSet)
          {
            DatabaseErrorCheck(pResultSet);
            if (pResultSet->Next())
            {
              DatabaseErrorCheck(pResultSet);
              wxString strValue = pResultSet->GetResultString(_("aliasColumn"));
              DatabaseErrorCheck(pResultSet);
              ASSERT_EQUALS(wxString(_("ONE")), strValue);
            }
            else
            {
              FAIL("Empty ResultSet");
            }
            m_pDatabaseLayer->CloseResultSet(pResultSet);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      /* Test alias columns in result set
         - Currently having issues with the correct VIEW syntax with MySQL and Firebird
       */
      void testViewAlias( void )
      {
        puts("testViewAlias");
        ASSERT( m_pDatabaseLayer != NULL );
        
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
            // Place the code for your test here
          // Add a new row to the database table
          m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('ONE', 'TWO');"));
          DatabaseErrorCheck(m_pDatabaseLayer);

          // Get the updated row count
          int nCount = GetRowCount();
          wxPrintf(_("Checking if row count = 1\n"));
          ASSERT_EQUALS( 1, nCount );

          // Add a view just for this test
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_VIEW1);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
          DatabaseErrorCheck(m_pDatabaseLayer);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "view already exists" error
            ProcessException(e, false);
          }
#endif
          
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP VIEW view1;"));

          DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT col1 FROM view1;"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pResultSet != NULL);
          if (pResultSet)
          {
            DatabaseErrorCheck(pResultSet);
            if (pResultSet->Next())
            {
              DatabaseErrorCheck(pResultSet);
              wxString strValue = pResultSet->GetResultString(_("col1"));
              DatabaseErrorCheck(pResultSet);
              ASSERT_EQUALS(wxString(_("ONE")), strValue);
            }
            else
            {
              FAIL("Empty ResultSet");
            }
            m_pDatabaseLayer->CloseResultSet(pResultSet);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      void testSemicolonInsideStatement( void )
      {
        puts("testSemicolonInsideStatement");
        ASSERT( m_pDatabaseLayer != NULL );
        
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
          wxString strContentWithSemicolon = _("Field with a ; within the string");
          wxString strContentWithQuote = _("Field with a \" within the string");
          wxString strSQL = _("INSERT INTO table1 VALUES ('") + strContentWithSemicolon + _("', '") + strContentWithQuote + _("');");
          m_pDatabaseLayer->RunQuery(strSQL);

          DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT * FROM table1;"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pResultSet != NULL);
          if (pResultSet)
          {
            DatabaseErrorCheck(pResultSet);
            if (pResultSet->Next())
            {
              DatabaseErrorCheck(pResultSet);
              ASSERT_EQUALS(strContentWithSemicolon, pResultSet->GetResultString(_("column1")));
              ASSERT_EQUALS(strContentWithQuote , pResultSet->GetResultString(_("column2")));
            }
            else
            {
              FAIL("Empty ResultSet");
            }
            m_pDatabaseLayer->CloseResultSet(pResultSet);
          }
          else
          {
            FAIL(m_pDatabaseLayer->GetErrorMessage());
          }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      /* Test the "ExecuteUpdate" and "ExecuteQuery" DatabaseLayer function wrappers
       */
      void testDatabaseLayerWrappers( void )
      {
        puts("testDatabaseLayerWrappers");
        ASSERT( m_pDatabaseLayer != NULL );
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          // Get the initial row count
          int nCount = GetRowCount();
          ASSERT_EQUALS( 0, nCount );
        
          // Add a new row to the database table
          m_pDatabaseLayer->ExecuteUpdate(_("INSERT INTO table1 (column1, column2) VALUES ('ONE', 'TWO');"));
          DatabaseErrorCheck(m_pDatabaseLayer);

          // Get the updated row count
          nCount = GetRowCount();
          ASSERT_EQUALS( 1, nCount );

          DatabaseResultSet* pResultSet = m_pDatabaseLayer->ExecuteQuery(_("SELECT COUNT(*) FROM table1"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pResultSet != NULL);
          if (pResultSet)
          {
            DatabaseErrorCheck(pResultSet);
            if (pResultSet->Next())
            {
              DatabaseErrorCheck(pResultSet);
              nCount = pResultSet->GetResultInt(1);
            }
            else
            {
              m_pDatabaseLayer->CloseResultSet(pResultSet);
              FAIL("No records found");
            }
            m_pDatabaseLayer->CloseResultSet(pResultSet);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }
          
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }
      
      /* Test the "ExecuteUpdate" and "ExecuteQuery" PreparedStatement function wrappers
       */
      void testPreparedStatementWrappers( void )
      {
        puts("testPreparedStatementWrappers");
        ASSERT( m_pDatabaseLayer != NULL );
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          // Get the initial row count
          int nCount = GetRowCount();
          ASSERT_EQUALS( 0, nCount );
        
          // Add a new row to the database table
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("INSERT INTO table1 (column1, column2) VALUES ('ONE', 'TWO');"));
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            pStatement->ExecuteUpdate();
            DatabaseErrorCheck(pStatement);
            m_pDatabaseLayer->CloseStatement(pStatement);
            pStatement = NULL;
          }

          // Get the updated row count
          nCount = GetRowCount();
          ASSERT_EQUALS( 1, nCount );

          pStatement = m_pDatabaseLayer->PrepareStatement(_("SELECT COUNT(*) FROM table1"));
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            DatabaseResultSet* pResultSet = pStatement->ExecuteQuery();
            ASSERT(pResultSet != NULL);
            if (pResultSet)
            {
              DatabaseErrorCheck(pResultSet);
              if (pResultSet->Next())
              {
                DatabaseErrorCheck(pResultSet);
                nCount = pResultSet->GetResultInt(1);
              }
              else
              {
                m_pDatabaseLayer->CloseResultSet(pResultSet);
                FAIL("No records found");
              }
              m_pDatabaseLayer->CloseResultSet(pResultSet);
            }
            else
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }
            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }
      
      /* A user reported a problem where remnants of previous string contents
       *  were showed up in subsequent result set retrievals
       */
      void testTextRemnants( void )
      {
        puts("testTextRemnants");
        ASSERT( m_pDatabaseLayer != NULL );
        
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
            // Insert test data into the table
            wxString testString1 = _("TEXT 001");
            wxString testString2 = _("TEXT TEXT 002");
            wxString testString3 = _("TEXT 003");
            m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 VALUES ('1', '") + testString1 + _("');"));
            DatabaseErrorCheck(m_pDatabaseLayer);
            m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 VALUES ('2', '") + testString2 + _("');"));
            DatabaseErrorCheck(m_pDatabaseLayer);
            m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 VALUES ('3', '") + testString3 + _("');"));
            DatabaseErrorCheck(m_pDatabaseLayer);

            DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT column2 FROM table1 ORDER BY column1;"));
            DatabaseErrorCheck(m_pDatabaseLayer);
            ASSERT(pResultSet != NULL);
            if (pResultSet)
            {
              DatabaseErrorCheck(pResultSet);
              // First
              if (pResultSet->Next())
              {
                DatabaseErrorCheck(pResultSet);
                ASSERT_EQUALS(testString1, pResultSet->GetResultString(1));
              }
              else
              {
                FAIL("First record NOT found");
              }
              // Second
              if (pResultSet->Next())
              {
                DatabaseErrorCheck(pResultSet);
                ASSERT_EQUALS(testString2, pResultSet->GetResultString(1));
              }
              else
              {
                FAIL("Second record NOT found");
              }
              // Third record
              if (pResultSet->Next())
              {
                DatabaseErrorCheck(pResultSet);
                ASSERT_EQUALS(testString3, pResultSet->GetResultString(1));
              }
              else
              {
                FAIL("Third record NOT found");
              }
              m_pDatabaseLayer->CloseResultSet(pResultSet);
            }
            else
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }
    
      /* Test the ability for the databaselayer to keep track of the prepared statements
       *  and result sets.  If deleting a databaselayer without cleaning up the database
       *  objects doesn't cause an error on tear down then it worked as expected
       */
      void testDatabaseGarbageCollection( void )
      {
        puts("testDatabaseGarbageCollection");
        ASSERT( m_pDatabaseLayer != NULL );
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          // Get the initial row count
          int nCount = GetRowCount();
          ASSERT_EQUALS( 0, nCount );
        
          // Add a new row to the database table
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("INSERT INTO table1 (column1, column2) VALUES ('ONE', 'TWO');"));
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            pStatement->ExecuteUpdate();
            DatabaseErrorCheck(pStatement);
            //m_pDatabaseLayer->CloseStatement(pStatement); /**/
            pStatement = NULL;
          }

          // Get the updated row count
          nCount = GetRowCount();
          ASSERT_EQUALS( 1, nCount );

          pStatement = m_pDatabaseLayer->PrepareStatement(_("SELECT COUNT(*) FROM table1"));
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            DatabaseResultSet* pResultSet = pStatement->ExecuteQuery();
            ASSERT(pResultSet != NULL);
            if (pResultSet)
            {
              DatabaseErrorCheck(pResultSet);
              if (pResultSet->Next())
              {
                DatabaseErrorCheck(pResultSet);
                nCount = pResultSet->GetResultInt(1);
              }
              else
              {
                m_pDatabaseLayer->CloseResultSet(pResultSet);
                FAIL("No records found");
              }
              ResultSetMetaData* pMetaData = pResultSet->GetMetaData();
              ASSERT(pMetaData != NULL);
              //m_pDatabaseLayer->CloseResultSet(pResultSet); /**/
            }
            else
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }
            m_pDatabaseLayer->CloseStatement(pStatement);/**/
          }
          
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      void testIsFieldNull()
      {
        puts("testIsFieldNull");
        ASSERT( m_pDatabaseLayer != NULL );
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif

          // Add a table just for this test
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE7);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table7;"));
          
          m_pDatabaseLayer->RunQuery(_("DELETE FROM table7;")); // Be extra sure the table is empty
          DatabaseErrorCheck(m_pDatabaseLayer);

          // Insert 1 record in the table with a key followed by all NULL values
          // Testing if NULL values are detected
          m_pDatabaseLayer->RunQuery(_("INSERT INTO table7 (KeyCol, IntCol, DoubleCol, StringCol, BlobCol, DateCol) VALUES (1, NULL, NULL, NULL, NULL, NULL);"));

          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("SELECT * FROM table7 WHERE KeyCol = 1;"));
          ASSERT(pStatement != NULL);
          DatabaseErrorCheck(pStatement);
          if (pStatement)
          {
            DatabaseResultSet* pResultSet = pStatement->RunQueryWithResults();
            ASSERT(pResultSet != NULL);
            DatabaseErrorCheck(pResultSet);
            if (pResultSet)
            {
              if (pResultSet->Next())
              {
                //puts("IntCol should be NULL");
                ASSERT(pResultSet->IsFieldNull(_("IntCol")));
                //puts("StringCol should be NULL");
                ASSERT(pResultSet->IsFieldNull(_("StringCol")));
                //puts("DoubleCol should be NULL");
                ASSERT(pResultSet->IsFieldNull(_("DoubleCol")));
                //puts("DateCol should be NULL");
                ASSERT(pResultSet->IsFieldNull(_("DateCol")));
                //puts("BlobCol should be NULL");
                ASSERT(pResultSet->IsFieldNull(_("BlobCol")));
              }
              else
              {
                FAIL("Error: No results found when inserting a table row with all NULL values");
              }

              //puts("Closing result set");
              m_pDatabaseLayer->CloseResultSet(pResultSet);
            }
            //puts("Closing statement");
            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          
          // Now test for non-NULL values
          m_pDatabaseLayer->RunQuery(_("DELETE FROM table7;")); // Empty the table
          DatabaseErrorCheck(m_pDatabaseLayer);
 
          wxDateTime dateNow = wxDateTime::Now();

          //puts("Adding new data to table7");
          pStatement = m_pDatabaseLayer->PrepareStatement(_("INSERT INTO table7 (KeyCol, IntCol, DoubleCol, StringCol, BlobCol, DateCol) VALUES (1, ?, ?, ?, ?, ?);"));
          ASSERT(pStatement != NULL);
          DatabaseErrorCheck(pStatement);
          if (pStatement)
          {
            //puts("Setting parameter 1");
            pStatement->SetParamInt(1, 0);
            //puts("Setting parameter 2");
            pStatement->SetParamDouble(2, 0.00);
            //puts("Setting parameter 3");
            pStatement->SetParamString(3, _(""));

            //puts("Setting parameter 4");
            wxString strBlob = _("Test Blob");
            // Use the length + 1 so that we get the null terminating character at the end of the string
            wxCharBuffer charBuffer = strBlob.mb_str(*wxConvCurrent);
            pStatement->SetParamBlob(4, (const unsigned char*)(const char*)charBuffer, strBlob.Length()+1);

            //puts("Setting parameter 5");
            pStatement->SetParamDate(5, dateNow);

            //puts("Running query");
            pStatement->RunQuery();

            //puts("Closing statement");
            m_pDatabaseLayer->CloseStatement(pStatement);
            pStatement = NULL;
          }

          //puts("Running query with results");
          DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT * FROM table7 WHERE KeyCol = 1;"));
          //puts("Finished running query with results");
          ASSERT(pResultSet != NULL);
          DatabaseErrorCheck(pResultSet);
          if (pResultSet)
          {
            //puts("Going to the next record");
            if (pResultSet->Next())
            {
              //puts("IntCol should NOT be NULL");
              ASSERT(pResultSet->IsFieldNull(_("IntCol")) == false);
              ASSERT_EQUALS(0, pResultSet->GetResultInt(_("IntCol")));

              //puts("StringCol should NOT be NULL");
              ASSERT(pResultSet->IsFieldNull(_("StringCol")) == false);
              ASSERT_EQUALS(wxString(_("")), pResultSet->GetResultString(_("StringCol")));
              
              //puts("DoubleCol should NOT be NULL");
              ASSERT(pResultSet->IsFieldNull(_("DoubleCol")) == false);
              ASSERT_EQUALS_EPSILON(0.00, pResultSet->GetResultDouble(_("DoubleCol")), 0.0001);
              
              //puts("DateCol should NOT be NULL");
              ASSERT(pResultSet->IsFieldNull(_("DateCol")) == false);
              ASSERT(dateNow == pResultSet->GetResultDate(_("DateCol")));
              
              //puts("BlobCol should NOT be NULL");
              ASSERT(pResultSet->IsFieldNull(_("BlobCol")) == false);

              //puts("GetResultBlob");
              wxMemoryBuffer BufferOut;
              void* pBlob = pResultSet->GetResultBlob(_("BlobCol"), BufferOut);
                
              ASSERT_MESSAGE((pBlob != NULL),"Blob field was NULL which was NOT expected");
            }
            else
            {
              FAIL("Error: No results found when inserting a table row with all NULL values");
            }

            m_pDatabaseLayer->CloseResultSet(pResultSet);
          }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      void testDateWithoutTime()
      {
        puts("testDateWithoutTime");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE9);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table9;"));

          m_pDatabaseLayer->RunQuery(_("DELETE FROM table9;")); // Be extra sure the table is empty
          
          wxDateTime ExpectedTime(21 /*day of month*/, wxDateTime::Jan /*month*/, 2006 /*year*/, 0 /*hour*/, 0 /*minute*/, 0 /*second*/, 0 /*milliseconds*/);
          SQL_STATEMENT_ARRAY::iterator insertIntoTableSql = m_SqlMap.find(INSERT_INTO_TABLE9);
          if (insertIntoTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*insertIntoTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*insertIntoTableSql).second.Item(i));
            }
          }

          DatabaseErrorCheck(m_pDatabaseLayer);
          DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT * FROM table9;"));
          ASSERT(pResultSet != NULL);
          if (pResultSet)
          {
            DatabaseErrorCheck(pResultSet);
            if (pResultSet->Next())
            {
              wxDateTime RetrievedTime = pResultSet->GetResultDate(_("DateCol"));
              wxPrintf(_("ExpectedTime: \"%s\" compared to \"%s\"\n"), 
                    (const char*)ExpectedTime.Format().c_str(), (const char*)RetrievedTime.Format().c_str());
              ASSERT(RetrievedTime.IsEqualTo(ExpectedTime));
            }
            else
            {
              FAIL("Empty ResultSet");
            }
            m_pDatabaseLayer->CloseResultSet(pResultSet);
          }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif

        }
      }

      void testDateWithoutTimePreparedStatement()
      {
        puts("testDateWithoutTimePreparedStatement");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE9);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table9;"));

          m_pDatabaseLayer->RunQuery(_("DELETE FROM table9;")); // Be extra sure the table is empty
          
          wxDateTime OriginalDate(21 /*day of month*/, wxDateTime::Jan /*month*/, 2006 /*year*/, 10 /*hour*/, 11 /*minute*/, 12 /*second*/, 0 /*milliseconds*/);
          wxDateTime ExpectedDate(21 /*day of month*/, wxDateTime::Jan /*month*/, 2006 /*year*/, 0 /*hour*/, 0 /*minute*/, 0 /*second*/, 0 /*milliseconds*/);
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("INSERT INTO table9 (IntCol, DateCol) VALUES (1, ?);"));

          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            DatabaseErrorCheck(pStatement);
            pStatement->SetParamDate(1, OriginalDate);
            DatabaseErrorCheck(pStatement);
            pStatement->RunQuery();
            DatabaseErrorCheck(pStatement);

            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT * FROM table9;"));
          ASSERT(pResultSet != NULL);
          if (pResultSet)
          {
            DatabaseErrorCheck(pResultSet);
            if (pResultSet->Next())
            {
              wxDateTime RetrievedDate = pResultSet->GetResultDate(_("DateCol"));
              wxPrintf(_("ExpectedDate: \"%s\" compared to \"%s\"\n"), 
                    (const char*)ExpectedDate.Format().c_str(), (const char*)RetrievedDate.Format().c_str());
              ASSERT(RetrievedDate.IsSameDate(ExpectedDate));
            }
            else
            {
              FAIL("Empty ResultSet");
            }
            m_pDatabaseLayer->CloseResultSet(pResultSet);
          }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif

        }
      }

      void testTableExists()
      {
        puts("testTableExists");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
          ASSERT(m_pDatabaseLayer->TableExists(_("table1")) == true);
          ASSERT(m_pDatabaseLayer->TableExists(_("tablewho")) == false);
        }
      }
      
      void testViewExists()
      {
        puts("testViewExists");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_VIEW1);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
          DatabaseErrorCheck(m_pDatabaseLayer);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "view already exists" error
            ProcessException(e, false);
          }
#endif
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP VIEW view1;"));

          ASSERT(m_pDatabaseLayer->ViewExists(_("view1")) == true);
          ASSERT(m_pDatabaseLayer->ViewExists(_("tablewho")) == false);
        }
      }
      
      void testGetTables()
      {
        puts("testGetTables");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
          wxArrayString tables = m_pDatabaseLayer->GetTables();
          int tableCount = tables.GetCount();
          ASSERT( tableCount >= 1 );
          ASSERT( tables.Index(_("table1"), false) != wxNOT_FOUND );
        }
      }
      
      void testGetViews()
      {
        puts("testGetViews");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_VIEW1);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
          DatabaseErrorCheck(m_pDatabaseLayer);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "view already exists" error
            ProcessException(e, false);
          }
#endif
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP VIEW view1;"));

          wxArrayString views = m_pDatabaseLayer->GetViews();
          int viewCount = views.GetCount();
          ASSERT( viewCount >= 1 );
          ASSERT( views.Index(_("view1"), false) != wxNOT_FOUND );
        }
      }
      
      void testGetColumns()
      {
        puts("testGetColumns");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
          wxArrayString columns = m_pDatabaseLayer->GetColumns(_("table1"));
          int columnCount = columns.GetCount();
          ASSERT_EQUALS( 2, columnCount );

          if (columnCount > 0)
            ASSERT( wxString(_("column1")).CmpNoCase(columns[0]) == 0 );

          if (columnCount > 1)
            ASSERT( wxString(_("column2")).CmpNoCase(columns[1]) == 0 );
        }
      }

      // Test to make sure that calling Close on the DatabaseLayer before deleting it doesn't cause problems
      void testCloseBeforeDelete()
      {
        puts("testCloseBeforeDelete");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
          // To avoid leaving remnant tables around delete table1 before we close the database
          m_pDatabaseLayer->RunQuery(_("DROP TABLE table1;"));

          m_pDatabaseLayer->Close();
          delete m_pDatabaseLayer;
          m_pDatabaseLayer = NULL;
        }
      }

      void testIsOpen()
      {
        puts("testIsOpen");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
          ASSERT(m_pDatabaseLayer->IsOpen());

          // To avoid leaving remnant tables around delete table1 before we close the database
          m_pDatabaseLayer->RunQuery(_("DROP TABLE table1;"));

          m_pDatabaseLayer->Close();
          ASSERT(!(m_pDatabaseLayer->IsOpen()));

          wxDELETE(m_pDatabaseLayer);
        }
      }

      void testSingleResult()
      {
        puts("testSingleResult");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
          CreateTable8();
          PopulateTable8();

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          int intValue = m_pDatabaseLayer->GetSingleResultInt(_("SELECT * FROM table8 WHERE IntCol = 10;"), 1);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT_EQUALS(10, intValue);
          intValue = m_pDatabaseLayer->GetSingleResultInt(_("SELECT * FROM table8 WHERE IntCol = 10;"), _("IntCol"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT_EQUALS(10, intValue);

          wxString stringValue = m_pDatabaseLayer->GetSingleResultString(_("SELECT * FROM table8 WHERE IntCol = 10;"), 3);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT_EQUALS(wxString(_("Test")), stringValue);
          stringValue = m_pDatabaseLayer->GetSingleResultString(_("SELECT * FROM table8 WHERE IntCol = 10;"), _("StringCol"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT_EQUALS(wxString(_("Test")), stringValue);
          
          long longValue = m_pDatabaseLayer->GetSingleResultLong(_("SELECT * FROM table8 WHERE IntCol = 10;"), 1);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT_EQUALS(10L, longValue);
          longValue = m_pDatabaseLayer->GetSingleResultLong(_("SELECT * FROM table8 WHERE IntCol = 10;"), _("IntCol"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT_EQUALS(10L, longValue);
          
          //bool boolValue = m_pDatabaseLayer->GetSingleResultBool(_("SELECT * FROM table8 WHERE IntCol = 10;"), 1);
          
          wxDateTime dateTime = m_pDatabaseLayer->GetSingleResultDate(_("SELECT * FROM table8 WHERE IntCol = 10;"), 5);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(dateTime.IsValid());
          dateTime = m_pDatabaseLayer->GetSingleResultDate(_("SELECT * FROM table8 WHERE IntCol = 10;"), _("DateCol"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(dateTime.IsValid());
          
          wxMemoryBuffer buffer;
          void* blobValue = m_pDatabaseLayer->GetSingleResultBlob(_("SELECT * FROM table8 WHERE IntCol = 10;"), 4, buffer);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(blobValue != NULL);
          blobValue = m_pDatabaseLayer->GetSingleResultBlob(_("SELECT * FROM table8 WHERE IntCol = 10;"), _("BlobCol"), buffer);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(blobValue != NULL);
          
          double doubleValue = m_pDatabaseLayer->GetSingleResultDouble(_("SELECT * FROM table8 WHERE IntCol = 10;"), 2);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT_EQUALS_EPSILON(1.234, doubleValue, 0.0001);
          doubleValue = m_pDatabaseLayer->GetSingleResultDouble(_("SELECT * FROM table8 WHERE IntCol = 10;"), _("DoubleCol"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT_EQUALS_EPSILON(1.234, doubleValue, 0.0001);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }
      
      void testSingleResultNoResults()
      {
        puts("testSingleResultNoResults");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
          CreateTable8();
          PopulateTable8();

          // Int
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
          m_pDatabaseLayer->GetSingleResultInt(_("SELECT * FROM table8 WHERE IntCol = 1;"), 1);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          FAIL("DatabaseLayer error should have been thrown");
#else
          ASSERT_EQUALS( DATABASE_LAYER_NO_ROWS_FOUND, m_pDatabaseLayer->GetErrorCode() );
#endif
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ASSERT_EQUALS( DATABASE_LAYER_NO_ROWS_FOUND, e.GetErrorCode() );
          }
#endif

          // String 
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
          wxString stringValue = m_pDatabaseLayer->GetSingleResultString(_("SELECT * FROM table8 WHERE IntCol = 1;"), 3);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          FAIL("DatabaseLayer error should have been thrown");
#else
          ASSERT_EQUALS( DATABASE_LAYER_NO_ROWS_FOUND, m_pDatabaseLayer->GetErrorCode() );
#endif
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ASSERT_EQUALS( DATABASE_LAYER_NO_ROWS_FOUND, e.GetErrorCode() );
          }
#endif

          // Long
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
          m_pDatabaseLayer->GetSingleResultLong(_("SELECT * FROM table8 WHERE IntCol = 1;"), 1);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          FAIL("DatabaseLayer error should have been thrown");
#else
          ASSERT_EQUALS( DATABASE_LAYER_NO_ROWS_FOUND, m_pDatabaseLayer->GetErrorCode() );
#endif
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ASSERT_EQUALS( DATABASE_LAYER_NO_ROWS_FOUND, e.GetErrorCode() );
          }
#endif

          // Date
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
          wxDateTime dateTime = m_pDatabaseLayer->GetSingleResultDate(_("SELECT * FROM table8 WHERE IntCol = 1;"), 5);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          FAIL("DatabaseLayer error should have been thrown");
#else
          ASSERT_EQUALS( DATABASE_LAYER_NO_ROWS_FOUND, m_pDatabaseLayer->GetErrorCode() );
#endif
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ASSERT_EQUALS( DATABASE_LAYER_NO_ROWS_FOUND, e.GetErrorCode() );
          }
#endif

          // Blob
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
          wxMemoryBuffer buffer;
          m_pDatabaseLayer->GetSingleResultBlob(_("SELECT * FROM table8 WHERE IntCol = 1;"), 4, buffer);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          FAIL("DatabaseLayer error should have been thrown");
#else
          ASSERT_EQUALS( DATABASE_LAYER_NO_ROWS_FOUND, m_pDatabaseLayer->GetErrorCode() );
#endif
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ASSERT_EQUALS( DATABASE_LAYER_NO_ROWS_FOUND, e.GetErrorCode() );
          }
#endif

          // Double
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
          m_pDatabaseLayer->GetSingleResultDouble(_("SELECT * FROM table8 WHERE IntCol = 1;"), 2);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          FAIL("DatabaseLayer error should have been thrown");
#else
          ASSERT_EQUALS( DATABASE_LAYER_NO_ROWS_FOUND, m_pDatabaseLayer->GetErrorCode() );
#endif
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ASSERT_EQUALS( DATABASE_LAYER_NO_ROWS_FOUND, e.GetErrorCode() );
          }
#endif
        }
      }
      
      void testSingleResultNonUniqueResults()
      {
        puts("testSingleResultNonUniqueResults");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
          CreateTable8();
          PopulateTable8();
          PopulateTable8();

          // Int
          int nResult = m_pDatabaseLayer->GetSingleResultInt(_("SELECT * FROM table8 WHERE IntCol = 10;"), 1, false);
          ASSERT_EQUALS( 10, nResult );
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          m_pDatabaseLayer->GetSingleResultInt(_("SELECT * FROM table8 WHERE IntCol = 10;"), 1);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          FAIL("DatabaseLayer error should have been thrown");
#else
          ASSERT_EQUALS( DATABASE_LAYER_NON_UNIQUE_RESULTSET, m_pDatabaseLayer->GetErrorCode() );
#endif
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ASSERT_EQUALS( DATABASE_LAYER_NON_UNIQUE_RESULTSET, e.GetErrorCode() );
          }
#endif

          // String 
          wxString stringValue = m_pDatabaseLayer->GetSingleResultString(_("SELECT * FROM table8 WHERE IntCol = 10;"), 3, false);
          ASSERT_EQUALS( wxString(_("Test")), stringValue );
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
          stringValue = m_pDatabaseLayer->GetSingleResultString(_("SELECT * FROM table8 WHERE IntCol = 10;"), 3);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          FAIL("DatabaseLayer error should have been thrown");
#else
          ASSERT_EQUALS( DATABASE_LAYER_NON_UNIQUE_RESULTSET, m_pDatabaseLayer->GetErrorCode() );
#endif
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ASSERT_EQUALS( DATABASE_LAYER_NON_UNIQUE_RESULTSET, e.GetErrorCode() );
          }
#endif

          // Long
          long lReturn = m_pDatabaseLayer->GetSingleResultLong(_("SELECT * FROM table8 WHERE IntCol = 10;"), 1, false);
          ASSERT_EQUALS( 10L, lReturn );
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
          m_pDatabaseLayer->GetSingleResultLong(_("SELECT * FROM table8 WHERE IntCol = 10;"), 1);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          FAIL("DatabaseLayer error should have been thrown");
#else
          ASSERT_EQUALS( DATABASE_LAYER_NON_UNIQUE_RESULTSET, m_pDatabaseLayer->GetErrorCode() );
#endif
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ASSERT_EQUALS( DATABASE_LAYER_NON_UNIQUE_RESULTSET, e.GetErrorCode() );
          }
#endif

          // Date
          wxDateTime dateTime = m_pDatabaseLayer->GetSingleResultDate(_("SELECT * FROM table8 WHERE IntCol = 10;"), 5, false);
          ASSERT( dateTime.IsValid() );
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
          dateTime = m_pDatabaseLayer->GetSingleResultDate(_("SELECT * FROM table8 WHERE IntCol = 10;"), 5);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          FAIL("DatabaseLayer error should have been thrown");
#else
          ASSERT_EQUALS( DATABASE_LAYER_NON_UNIQUE_RESULTSET, m_pDatabaseLayer->GetErrorCode() );
#endif
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ASSERT_EQUALS( DATABASE_LAYER_NON_UNIQUE_RESULTSET, e.GetErrorCode() );
          }
#endif

          // Blob
          wxMemoryBuffer buffer;
          void* pReturn = m_pDatabaseLayer->GetSingleResultBlob(_("SELECT * FROM table8 WHERE IntCol = 10;"), 4, buffer, false);
          ASSERT( pReturn != NULL );
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
          m_pDatabaseLayer->GetSingleResultBlob(_("SELECT * FROM table8 WHERE IntCol = 10;"), 4, buffer);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          FAIL("DatabaseLayer error should have been thrown");
#else
          ASSERT_EQUALS( DATABASE_LAYER_NON_UNIQUE_RESULTSET, m_pDatabaseLayer->GetErrorCode() );
#endif
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ASSERT_EQUALS( DATABASE_LAYER_NON_UNIQUE_RESULTSET, e.GetErrorCode() );
          }
#endif

          // Double
          double dblResult = m_pDatabaseLayer->GetSingleResultDouble(_("SELECT * FROM table8 WHERE IntCol = 10;"), 2, false);
          ASSERT_EQUALS_EPSILON( 1.234, dblResult, 0.0001 );
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
          m_pDatabaseLayer->GetSingleResultDouble(_("SELECT * FROM table8 WHERE IntCol = 10;"), 2);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          FAIL("DatabaseLayer error should have been thrown");
#else
          ASSERT_EQUALS( DATABASE_LAYER_NON_UNIQUE_RESULTSET, m_pDatabaseLayer->GetErrorCode() );
#endif
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ASSERT_EQUALS( DATABASE_LAYER_NON_UNIQUE_RESULTSET, e.GetErrorCode() );
          }
#endif
        }
      }

      void testResultArrayInt()
      {
        puts("testResultArrayInt");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
          CreateTable8();
          PopulateTable8();
          PopulateTable8();
          
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          wxArrayInt intArray = m_pDatabaseLayer->GetResultsArrayInt(_("SELECT * FROM table8 WHERE IntCol = 10;"), 1);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT_EQUALS( 2, (int)intArray.GetCount() );
          ASSERT_EQUALS( 10, intArray[0]);
          ASSERT_EQUALS( 10, intArray[1]);

          intArray = m_pDatabaseLayer->GetResultsArrayInt(_("SELECT * FROM table8 WHERE IntCol = 10;"), _("IntCol"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT_EQUALS( 2, (int)intArray.GetCount() );
          ASSERT_EQUALS( 10, intArray[0]);
          ASSERT_EQUALS( 10, intArray[1]);

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }
      
      void testResultArrayString()
      {
        puts("testResultArrayString");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
          CreateTable8();
          PopulateTable8();
          PopulateTable8();
          PopulateTable8();
          
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          wxArrayString stringArray = m_pDatabaseLayer->GetResultsArrayString(_("SELECT * FROM table8 WHERE IntCol = 10;"), 3);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT_EQUALS( 3, (int)stringArray.GetCount() );
          ASSERT_EQUALS( wxString(_("Test")), stringArray[0]);
          ASSERT_EQUALS( wxString(_("Test")), stringArray[1]);
          ASSERT_EQUALS( wxString(_("Test")), stringArray[2]);
          
          stringArray = m_pDatabaseLayer->GetResultsArrayString(_("SELECT * FROM table8 WHERE IntCol = 10;"), _("StringCol"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT_EQUALS( 3, (int)stringArray.GetCount() );
          ASSERT_EQUALS( wxString(_("Test")), stringArray[0]);
          ASSERT_EQUALS( wxString(_("Test")), stringArray[1]);
          ASSERT_EQUALS( wxString(_("Test")), stringArray[2]);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }
      
      void testResultArrayLong()
      {
        puts("testResultArrayLong");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
          CreateTable8();
          PopulateTable8();
          PopulateTable8();
          PopulateTable8();
          PopulateTable8();
          
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          wxArrayLong longArray = m_pDatabaseLayer->GetResultsArrayLong(_("SELECT * FROM table8 WHERE IntCol = 10;"), 1);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT_EQUALS( 4, (int)longArray.GetCount() );
          ASSERT_EQUALS( 10L, longArray[0]);
          ASSERT_EQUALS( 10L, longArray[1]);
          ASSERT_EQUALS( 10L, longArray[2]);
          ASSERT_EQUALS( 10L, longArray[3]);

          longArray = m_pDatabaseLayer->GetResultsArrayLong(_("SELECT * FROM table8 WHERE IntCol = 10;"), _("IntCol"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT_EQUALS( 4, (int)longArray.GetCount() );
          ASSERT_EQUALS( 10L, longArray[0]);
          ASSERT_EQUALS( 10L, longArray[1]);
          ASSERT_EQUALS( 10L, longArray[2]);
          ASSERT_EQUALS( 10L, longArray[3]);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

#if wxCHECK_VERSION(2, 7, 0)
      void testResultArrayDouble()
      {
        puts("testResultArrayDouble");
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
          CreateTable8();
          PopulateTable8();
          PopulateTable8();
          PopulateTable8();
          PopulateTable8();
          PopulateTable8();

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          wxArrayDouble doubleArray = m_pDatabaseLayer->GetResultsArrayDouble(_("SELECT * FROM table8 WHERE IntCol = 10;"), 2);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT_EQUALS( 5, (int)doubleArray.GetCount() );
          ASSERT_EQUALS_EPSILON(1.234, doubleArray[0], 0.0001);
          ASSERT_EQUALS_EPSILON(1.234, doubleArray[1], 0.0001);
          ASSERT_EQUALS_EPSILON(1.234, doubleArray[2], 0.0001);
          ASSERT_EQUALS_EPSILON(1.234, doubleArray[3], 0.0001);
          ASSERT_EQUALS_EPSILON(1.234, doubleArray[4], 0.0001);

          doubleArray = m_pDatabaseLayer->GetResultsArrayDouble(_("SELECT * FROM table8 WHERE IntCol = 10;"), _("DoubleCol"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT_EQUALS( 5, (int)doubleArray.GetCount() );
          ASSERT_EQUALS_EPSILON(1.234, doubleArray[0], 0.0001);
          ASSERT_EQUALS_EPSILON(1.234, doubleArray[1], 0.0001);
          ASSERT_EQUALS_EPSILON(1.234, doubleArray[2], 0.0001);
          ASSERT_EQUALS_EPSILON(1.234, doubleArray[3], 0.0001);
          ASSERT_EQUALS_EPSILON(1.234, doubleArray[4], 0.0001);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }
#endif
      
      /* Test that a binary blob (from a file) is saved to and restored from the database correctly
       */
      void testMemoryBufferBlobParameter()
      {
        puts("testMemoryBufferBlobParameter");
        // Save one of the wxWidgets samples image files into a database table
        // Retrieve the image from the database and save it to a file
        // Do a MD5 sum on the original file and the new file to make sure that they're equal

        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE4);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table4;"));

          m_pDatabaseLayer->RunQuery(_("DELETE FROM table4;")); // Be extra sure the table is empty

          // Use a prepared statement to insert a binary BLOB into the database
          wxString strSQL = _("INSERT INTO table4 (IntCol, BlobCol) VALUES (1, ?);");
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(strSQL);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            ASSERT_EQUALS( 1, pStatement->GetParameterCount() );

            wxFile FileIn(_("wxwin.png"));
            int nDataLength = FileIn.Length();
            wxMemoryBuffer BufferIn(nDataLength);
            int nBytesRead = FileIn.Read(BufferIn.GetWriteBuf(nDataLength), nDataLength);
            ASSERT_EQUALS(nBytesRead, nDataLength);
            pStatement->SetParamBlob(1, BufferIn);
            DatabaseErrorCheck(pStatement);
            pStatement->RunQuery();
            DatabaseErrorCheck(pStatement);
            m_pDatabaseLayer->CloseStatement(pStatement);

            DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT BlobCol FROM table4 WHERE IntCol = 1"));
            DatabaseErrorCheck(m_pDatabaseLayer);
            ASSERT(pResultSet != NULL);
            if (pResultSet)
            {
              DatabaseErrorCheck(pResultSet);
              if (pResultSet->Next())
              {
                DatabaseErrorCheck(pResultSet);
                wxMemoryBuffer BufferOut;
                void* pBlob = pResultSet->GetResultBlob(_("BlobCol"), BufferOut);
                ASSERT_EQUALS(nDataLength, (int)BufferOut.GetBufSize());
                ASSERT_EQUALS(nDataLength, (int)BufferOut.GetDataLen());

                // Iterate through each byte and compare it to the source
                bool bMatch = TRUE;
                char* pOriginalBuffer = (char*)(BufferIn.GetData());
                char* pRetrievedBuffer = (char*)pBlob;
                wxString errorMessage;
                for (int i=0; i<nDataLength && bMatch; i++)
                {
                  if (*pOriginalBuffer != *pRetrievedBuffer)
                  {
                    errorMessage.Printf(_("Retrieved Blob does NOT match original Blob.  Error at byte %d"), i);
                    bMatch = FALSE;
                  }
                  pOriginalBuffer++;
                  pRetrievedBuffer++;
                }
                ASSERT_MESSAGE( bMatch, errorMessage.mb_str() );
              }
              else
              {
                m_pDatabaseLayer->CloseResultSet(pResultSet);
                FAIL("Empty ResultSet");
              }
              m_pDatabaseLayer->CloseResultSet(pResultSet);
            }
            else
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif

        }
      }

      /* Test that a binary blob (from a file) is saved to and restored from the database correctly
       */
      void testSmallMemoryBuffer()
      {
        puts("testSmallMemoryBuffer");
        // Save one of the wxWidgets samples image files into a database table
        // Retrieve the image from the database and save it to a file
        // Do a MD5 sum on the original file and the new file to make sure that they're equal

        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE4);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table4;"));

          m_pDatabaseLayer->RunQuery(_("DELETE FROM table4;")); // Be extra sure the table is empty

          // Use a prepared statement to insert a binary BLOB into the database
          wxString strSQL = _("INSERT INTO table4 (IntCol, BlobCol) VALUES (1, ?);");
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(strSQL);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            ASSERT_EQUALS( 1, pStatement->GetParameterCount() );

            wxString fileName = wxFileName::CreateTempFileName(_T("temp"), (wxFile*)NULL);
            //wxPuts(fileName);
            wxFFile* FileIn = CreateFileWithRandomContents(fileName, 266);
            int nDataLength = FileIn->Length();
            wxMemoryBuffer BufferIn;
            int nBytesRead = FileIn->Read(BufferIn.GetWriteBuf(nDataLength), nDataLength);
            FileIn->Close();
            delete FileIn;
            //wxRemoveFile(fileName); - The temp file will get cleaned up automatically

            ASSERT_EQUALS(nBytesRead, nDataLength);
            pStatement->SetParamBlob(1, (unsigned char*)BufferIn.GetData(), nDataLength);
            DatabaseErrorCheck(pStatement);
            pStatement->RunQuery();
            DatabaseErrorCheck(pStatement);
            m_pDatabaseLayer->CloseStatement(pStatement);

            DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT BlobCol FROM table4 WHERE IntCol = 1"));
            DatabaseErrorCheck(m_pDatabaseLayer);
            ASSERT(pResultSet != NULL);
            if (pResultSet)
            {
              DatabaseErrorCheck(pResultSet);
              if (pResultSet->Next())
              {
                DatabaseErrorCheck(pResultSet);
                wxMemoryBuffer BufferOut;
                void* pBlob = pResultSet->GetResultBlob(_("BlobCol"), BufferOut);
              	m_pDatabaseLayer->CloseResultSet(pResultSet);
                ASSERT_EQUALS(nDataLength, (int)BufferOut.GetBufSize());
                ASSERT_EQUALS(nDataLength, (int)BufferOut.GetDataLen());

                // Iterate through each byte and compare it to the source
                bool bMatch = TRUE;
                char* pOriginalBuffer = (char*)(BufferIn.GetData());
                char* pRetrievedBuffer = (char*)pBlob;
                wxString errorMessage;
                for (int i=0; i<nDataLength && bMatch; i++)
                {
                  if (*pOriginalBuffer != *pRetrievedBuffer)
                  {
                    errorMessage.Printf(_("Retrieved Blob does NOT match original Blob.  Error at byte %d"), i);
                    bMatch = FALSE;
                  }
                  pOriginalBuffer++;
                  pRetrievedBuffer++;
                }
                ASSERT_MESSAGE( bMatch, errorMessage.mb_str() );
              }
              else
              {
                m_pDatabaseLayer->CloseResultSet(pResultSet);
                FAIL("Empty ResultSet");
              }
            }
            else
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif

        }
      }

      /* Test that a binary blob (from a file) is saved to and restored from the database correctly
       */
      void testLargeMemoryBuffer()
      {
        puts("testLargeMemoryBuffer");
        // Save one of the wxWidgets samples image files into a database table
        // Retrieve the image from the database and save it to a file
        // Do a MD5 sum on the original file and the new file to make sure that they're equal

        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE4);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table4;"));

          m_pDatabaseLayer->RunQuery(_("DELETE FROM table4;")); // Be extra sure the table is empty

          // Use a prepared statement to insert a binary BLOB into the database
          wxString strSQL = _("INSERT INTO table4 (IntCol, BlobCol) VALUES (1, ?);");
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(strSQL);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            ASSERT_EQUALS( 1, pStatement->GetParameterCount() );

            wxString fileName = wxFileName::CreateTempFileName(_T("temp"), (wxFile*)NULL);
            //wxPuts(fileName);
            wxFFile* FileIn = CreateFileWithRandomContents(fileName, 65555);
            int nDataLength = FileIn->Length();
            wxMemoryBuffer BufferIn;
            int nBytesRead = FileIn->Read(BufferIn.GetWriteBuf(nDataLength), nDataLength);
            FileIn->Close();
            delete FileIn;
            //wxRemoveFile(fileName); - The temp file will get cleaned up automatically

            ASSERT_EQUALS(nBytesRead, nDataLength);
            pStatement->SetParamBlob(1, (unsigned char*)BufferIn.GetData(), nDataLength);
            DatabaseErrorCheck(pStatement);
            pStatement->RunQuery();
            DatabaseErrorCheck(pStatement);
            m_pDatabaseLayer->CloseStatement(pStatement);

            DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT BlobCol FROM table4 WHERE IntCol = 1"));
            DatabaseErrorCheck(m_pDatabaseLayer);
            ASSERT(pResultSet != NULL);
            if (pResultSet)
            {
              DatabaseErrorCheck(pResultSet);
              if (pResultSet->Next())
              {
                DatabaseErrorCheck(pResultSet);
                wxMemoryBuffer BufferOut;
                void* pBlob = pResultSet->GetResultBlob(_("BlobCol"), BufferOut);
              	m_pDatabaseLayer->CloseResultSet(pResultSet);
                ASSERT_EQUALS(nDataLength, (int)BufferOut.GetBufSize());
                ASSERT_EQUALS(nDataLength, (int)BufferOut.GetDataLen());

                // Iterate through each byte and compare it to the source
                bool bMatch = TRUE;
                char* pOriginalBuffer = (char*)(BufferIn.GetData());
                char* pRetrievedBuffer = (char*)pBlob;
                wxString errorMessage;
                for (int i=0; i<nDataLength && bMatch; i++)
                {
                  if (*pOriginalBuffer != *pRetrievedBuffer)
                  {
                    errorMessage.Printf(_("Retrieved Blob does NOT match original Blob.  Error at byte %d"), i);
                    bMatch = FALSE;
                  }
                  pOriginalBuffer++;
                  pRetrievedBuffer++;
                }
                ASSERT_MESSAGE( bMatch, errorMessage.mb_str() );
              }
              else
              {
                m_pDatabaseLayer->CloseResultSet(pResultSet);
                FAIL("Empty ResultSet");
              }
            }
            else
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif

        }
      }

      void testReusePreparedStatementWithResults()
      {
        puts("testReusePreparedStatementWithResults");
        ASSERT( m_pDatabaseLayer != NULL );
        
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif

          // Add a table just for this test
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE6);
          if (createTableSql != m_SqlMap.end())
          {
            for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
            {
              m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
            }
          }
          DatabaseErrorCheck(m_pDatabaseLayer);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            // Don't stop on error here since this could be a "table already exists" error
            ProcessException(e, false);
          }
#endif
          // Make sure that the tearDown function knows to remove this test table
          m_DeleteStatements.push_back(_("DROP TABLE table6;"));

          m_pDatabaseLayer->RunQuery(_("DELETE FROM table6;")); // Be extra sure the table is empty
          DatabaseErrorCheck(m_pDatabaseLayer);

          wxString strInsertSQL = _("INSERT INTO table6 (IntCol, DoubleCol, StringCol, BlobCol) VALUES (?, ?, ?, ?);");
          // Use a prepared statement to insert values into the database
          PreparedStatement* pInsertStatement = m_pDatabaseLayer->PrepareStatement(strInsertSQL);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pInsertStatement != NULL);
          if (pInsertStatement)
          {
            wxPrintf(_("Asserting parameter count = 4\n"));
            ASSERT_EQUALS( 4, pInsertStatement->GetParameterCount() );
            for (int i=1; i<=5; i++)
            {
              pInsertStatement->SetParamInt(1, i);
              DatabaseErrorCheck(pInsertStatement);
              pInsertStatement->SetParamDouble(2, 1.234);
              DatabaseErrorCheck(pInsertStatement);
              pInsertStatement->SetParamString(3, wxString::Format(_("Test%d"), i));
              DatabaseErrorCheck(pInsertStatement);
              wxString strBlob = wxString::Format(_("Test Blob %d"), i);
              // Use the length + 1 so that we get the null terminating character at the end of the string
              wxCharBuffer charBuffer = strBlob.mb_str(*wxConvCurrent);
              pInsertStatement->SetParamBlob(4, (const unsigned char*)(const char*)charBuffer, strBlob.Length()+1);
              DatabaseErrorCheck(pInsertStatement);

              pInsertStatement->RunQuery();
              DatabaseErrorCheck(pInsertStatement);
            }

            // Close the insert prepared statement
            m_pDatabaseLayer->CloseStatement(pInsertStatement);
          }

          // Use a prepared statement to retrieve the values from the database
          wxString strSelectSQL = _("SELECT * FROM table6 WHERE StringCol = ?;"); 
          PreparedStatement* pSelectStatement = m_pDatabaseLayer->PrepareStatement(strSelectSQL);
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pSelectStatement != NULL);
          if (pSelectStatement)
          {
            ASSERT_EQUALS( 1, pSelectStatement->GetParameterCount() );
            for (int i=5; i>0; i--)
            {
              wxString strStringCol = wxString::Format(_("Test%d"), i);
              wxPrintf(_("SELECT * FROM table6 WHERE StringCol = '%s';\n"), strStringCol.c_str());
              pSelectStatement->SetParamString(1, strStringCol);
              DatabaseResultSet* pResultSet = pSelectStatement->RunQueryWithResults();
              ASSERT(pResultSet != NULL);
              if (pResultSet)
              {
                DatabaseErrorCheck(pResultSet);
                if (pResultSet->Next())
                {
                  DatabaseErrorCheck(pResultSet);
                  wxPrintf(_("Asserting IntCol = 10\n"));
                  ASSERT_EQUALS( i, pResultSet->GetResultInt(_("IntCol")));
                  wxPrintf(_("Asserting DoubleCol = 1.234\n"));
                  ASSERT_EQUALS_EPSILON( 1.234, pResultSet->GetResultDouble(_("DoubleCol")), 0.000001);
                  wxPrintf(_("Asserting StringCol = 'Test2'\n"));
                  ASSERT_EQUALS(strStringCol, pResultSet->GetResultString(_("StringCol")));
                  wxMemoryBuffer buffer;
                  pResultSet->GetResultBlob(_("BlobCol"), buffer);
                  wxString strResult((char*)buffer.GetData(), *wxConvCurrent);
                  wxString strBlob = wxString::Format(_("Test Blob %d"), i);
                  wxPrintf(_("Asserting BlobCol = 'Test Blob %d'\n"), i);
                  ASSERT_EQUALS( strBlob, strResult );
                }
                else
                {
                  FAIL("Empty ResultSet");
                }
                m_pDatabaseLayer->CloseResultSet(pResultSet);
              }
              else
              {
                puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
                FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
              }
            }

            // Close the select prepared statement
            m_pDatabaseLayer->CloseStatement(pSelectStatement);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }
      
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      void testRunQueryReturnValue()
      {
        puts("testRunQueryReturnValue");
        ASSERT( m_pDatabaseLayer != NULL );
        
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
            // Place the code for your test here
            m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('ONE', 'TWO');"));
            m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('THREE', 'FOUR');"));
            m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('THREE', 'FOUR');"));
            m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('FIVE', 'SIX');"));
            m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('FIVE', 'SIX');"));
            m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('FIVE', 'SIX');"));
            DatabaseErrorCheck(m_pDatabaseLayer);

            int rows = m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('SEVEN', 'EIGHT');"));
            ASSERT_EQUALS(1, rows);

            rows = m_pDatabaseLayer->RunQuery(_("UPDATE table1 SET column2 = '4' WHERE column1 = 'THREE';"));
            ASSERT_EQUALS(2, rows);

            rows = m_pDatabaseLayer->RunQuery(_("DELETE FROM table1 WHERE column1 = 'FIVE';"));
            ASSERT_EQUALS(3, rows);

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      void testPreparedStatementRunQueryReturnValue()
      {
        puts("testPreparedStatementRunQueryReturnValue");
        ASSERT( m_pDatabaseLayer != NULL );
        
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
            // Place the code for your test here
            m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('ONE', 'TWO');"));
            m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('THREE', 'FOUR');"));
            m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('THREE', 'FOUR');"));
            m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('FIVE', 'SIX');"));
            m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('FIVE', 'SIX');"));
            m_pDatabaseLayer->RunQuery(_("INSERT INTO table1 (column1, column2) VALUES ('FIVE', 'SIX');"));
            DatabaseErrorCheck(m_pDatabaseLayer);

            PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("INSERT INTO table1 (column1, column2) VALUES ('SEVEN', 'EIGHT');"));
            DatabaseErrorCheck(m_pDatabaseLayer);
            ASSERT(pStatement != NULL);
            if (pStatement)
            {
              int rows = pStatement->RunQuery();
              ASSERT_EQUALS(1, rows);
              m_pDatabaseLayer->CloseStatement(pStatement);
            }

            pStatement = m_pDatabaseLayer->PrepareStatement(_("UPDATE table1 SET column2 = '4' WHERE column1 = 'THREE';"));
            DatabaseErrorCheck(m_pDatabaseLayer);
            ASSERT(pStatement != NULL);
            if (pStatement)
            {
              int rows = pStatement->RunQuery();
              ASSERT_EQUALS(2, rows);
              m_pDatabaseLayer->CloseStatement(pStatement);
            }

            pStatement = m_pDatabaseLayer->PrepareStatement(_("DELETE FROM table1 WHERE column1 = 'FIVE';"));
            DatabaseErrorCheck(m_pDatabaseLayer);
            ASSERT(pStatement != NULL);
            if (pStatement)
            {
              int rows = pStatement->RunQuery();
              ASSERT_EQUALS(3, rows);
              m_pDatabaseLayer->CloseStatement(pStatement);
            }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      void testBadPreparedStatement( void )
      {
        puts("testBadPreparedStatement");
        ASSERT( m_pDatabaseLayer != NULL );
        
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
            // Prepare a statement with intential typos
          PreparedStatement* pStatement = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          pStatement = m_pDatabaseLayer->PrepareStatement(_("SELECT * FORM table1;"));
#ifdef USE_ODBC // For some reason ODBC doesn't through the error until running the query
          if (pStatement && (pStatement->GetErrorCode() == DATABASE_LAYER_OK))
            pStatement->RunQuery();
#endif
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          if (pStatement)
          {
            m_pDatabaseLayer->CloseStatement(pStatement);
            pStatement = NULL;
          }
          FAIL("DatabaseLayer error should have been thrown");
#else
          ASSERT( DATABASE_LAYER_OK != m_pDatabaseLayer->GetErrorCode() );
#endif
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            if (pStatement)
            {
              m_pDatabaseLayer->CloseStatement(pStatement);
              pStatement = NULL;
            }
            ASSERT( DATABASE_LAYER_OK != e.GetErrorCode() );
          }
#endif

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      /* Test to make sure that DatabaseResultSet doesn't throw an 
	    error if there are no records returned
       */
      void testResultSetNoRecords()
      {
        puts("testResultSetNext");
        ASSERT( m_pDatabaseLayer != NULL );
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          // Get the initial row count
          int nCount = GetRowCount();
          wxPrintf(_("Checking if row count = 0\n"));
          ASSERT_EQUALS( 0, nCount );
        
          DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(_("SELECT * FROM table1"));
          DatabaseErrorCheck(m_pDatabaseLayer);

          ASSERT(pResultSet != NULL);
          if (pResultSet)
          {
            DatabaseErrorCheck(pResultSet);
            while (pResultSet->Next())
            {
              FAIL("The result set should have no records");
            }

            m_pDatabaseLayer->CloseResultSet(pResultSet);
          }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      void testSpecialCharactersInPreparedStatement( void )
      {
        puts("testSpecialCharactersInPreparedStatement");
        ASSERT( m_pDatabaseLayer != NULL );
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif
          // Get the initial row count
          wxPrintf(_("Checking if row count = 0\n"));
          int nCount = GetRowCount();
          ASSERT_EQUALS( 0, nCount );
        
          // Add a new row to the database table
          wxString specialCharacterString = _("' '', \", ?'");
          PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("INSERT INTO table1 (column1, column2) VALUES (") + specialCharacterString + _(", ?);"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            ASSERT(m_pDatabaseLayer->GetErrorCode() == DATABASE_LAYER_OK);
            if (m_pDatabaseLayer->GetErrorCode() != DATABASE_LAYER_OK)
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }
            else
            {
              DatabaseErrorCheck(pStatement);
              ASSERT_EQUALS( 1, pStatement->GetParameterCount() );
              pStatement->SetParamString(1, _("Test"));
              DatabaseErrorCheck(pStatement);
              pStatement->RunQuery();
              DatabaseErrorCheck(pStatement);
            }
            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }

          // Get the updated row count
          wxPrintf(_("Checking if row count = 1\n"));
          nCount = GetRowCount();
          ASSERT_EQUALS( 1, nCount );

          // Verify that the correct value was put in the database
          pStatement = m_pDatabaseLayer->PrepareStatement(_("SELECT * FROM table1 WHERE column1 = ") + specialCharacterString + _(" and column2 = ?;"));
          DatabaseErrorCheck(m_pDatabaseLayer);
          ASSERT(pStatement != NULL);
          if (pStatement)
          {
            ASSERT(m_pDatabaseLayer->GetErrorCode() == DATABASE_LAYER_OK);
            if (m_pDatabaseLayer->GetErrorCode() != DATABASE_LAYER_OK)
            {
              puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
              FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
            }
            else
            {
              DatabaseErrorCheck(pStatement);
              ASSERT_EQUALS( 1, pStatement->GetParameterCount() );
              pStatement->SetParamString(1, _("Test"));
              DatabaseErrorCheck(pStatement);
              DatabaseResultSet* pResultSet = pStatement->RunQueryWithResults();
              DatabaseErrorCheck(pStatement);
              ASSERT(pResultSet != NULL);
              if (pResultSet)
              {
                DatabaseErrorCheck(pResultSet);
                if (pResultSet->Next())
                {
                  DatabaseErrorCheck(pResultSet);
                  ASSERT_EQUALS( _(" ', \", ?"), pResultSet->GetResultString(_("column1")) );
                  ASSERT_EQUALS(_("Test"), pResultSet->GetResultString(_("column2")));
                }
                else
                {
                  FAIL("Empty ResultSet");
                }
                m_pDatabaseLayer->CloseResultSet(pResultSet);
              }
            }
            m_pDatabaseLayer->CloseStatement(pStatement);
          }
          else
          {
            puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
            FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
          }

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }

      
// Helper function to reduce the redundancy in getting the row count which is a common action
//  in the tests
      int GetRowCount(const wxString& strTable = _("table1"))
      {
        int nCount = -1;
        wxString strSQL = wxString::Format(_("SELECT COUNT(*) FROM %s"), strTable.c_str());
        DatabaseResultSet* pResultSet = m_pDatabaseLayer->RunQueryWithResults(strSQL);
        DatabaseErrorCheck(m_pDatabaseLayer);
        ASSERT(pResultSet != NULL);
        if (pResultSet)
        {
          DatabaseErrorCheck(pResultSet);
          if (pResultSet->Next())
          {
            DatabaseErrorCheck(pResultSet);
            nCount = pResultSet->GetResultInt(1);
          }
          else
          {
            m_pDatabaseLayer->CloseResultSet(pResultSet);
            FAIL("No records found");
          }
          m_pDatabaseLayer->CloseResultSet(pResultSet);
        }
        else
        {
          puts(m_pDatabaseLayer->GetErrorMessage().mb_str());
          FAIL(m_pDatabaseLayer->GetErrorMessage().mb_str());
        }

        return nCount;
      }

      void DatabaseErrorCheck(DatabaseErrorReporter* pReporter)
      {
        ASSERT( DATABASE_LAYER_OK == pReporter->GetErrorCode());
        if (pReporter->GetErrorCode() != DATABASE_LAYER_OK)
        {
          wxString errorMessage = wxString::Format(_("Error (%d): %s"), pReporter->GetErrorCode(), pReporter->GetErrorMessage().c_str());
          puts(errorMessage.mb_str());
        }
      }

      void ProcessException(DatabaseLayerException& e, bool bFail = true)
      {
        wxString errorMessage = wxString::Format(_("Error (%d): %s"), e.GetErrorCode(), e.GetErrorMessage().c_str());
        //puts(errorMessage.mb_str());
        if (bFail)
        {
          ASSERT_MESSAGE(false, errorMessage.mb_str());
        }
      }


      
      /* Skeleton for unit test function
       */
      void skeletonForNewTest( void )
      {
        puts("skeleton");
        ASSERT( m_pDatabaseLayer != NULL );
        
        if (m_pDatabaseLayer)
        {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          try
          {
#endif  
            // Place the code for your test here
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
          }
          catch (DatabaseLayerException& e)
          {
            ProcessException(e);
          }
#endif
        }
      }



      bool LoadSettings()
      {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
        try
        {
#endif  
        wxString settingsFileName = _("settings.conf");
        if (wxFileName::FileExists(settingsFileName) == false)
        {
          puts("Settings file not found.  Please copy settings.sample to settings.conf and change settings for you local system");
          return false;
        }
        wxFileInputStream fileIn(settingsFileName);
        if (fileIn.Ok() == false)
        {
          FAIL("Settings file not found.  Please copy settings.sample to settings.conf and change settings for you local system");
          return false;
        }
        wxFileConfig config(fileIn);
#ifdef USE_SQLITE
        m_pDatabaseLayer = NULL;
        wxString strDatabase;
        if (config.Read(_("/SQLite/database"), &strDatabase))
        {
        
          if (LoadSqlMap(_("SQLite")) == false)
          {
            wxPrintf(_("Error loading database specific SQL statements"));
            FAIL("Error loading database specific SQL statements");
            return false;
          }

          m_pDatabaseLayer = new SqliteDatabaseLayer(strDatabase);
        }
        else
        {
          wxPrintf(_("SQLite database file not defined in settings file"));
          FAIL("SQLite database file not defined in settings file");
          return false;
        }
#elif USE_FIREBIRD
        m_pDatabaseLayer = NULL;

		wxString strLibraryLocation;
        if (config.Read(_("/Firebird/library_location"), &strLibraryLocation))
        {
#ifdef WIN32
          wxString strSeparator = _(";");
#else
          wxString strSeparator = _(":");
#endif
          wxSetEnv(_("PATH"), strLibraryLocation + strSeparator + m_strPath);
		}

		if (!FirebirdDatabaseLayer::IsAvailable())
        {
          wxPrintf(_("Firebird database backend is not available"));
          FAIL("Firebird database backend is not available");
          return false;
        }
        wxString strServer;
        if (config.Read(_("/Firebird/server"), &strServer) == false)
        {
          wxPrintf(_("Firebird database server not defined in settings file"));
  #ifndef USE_FIREBIRD_EMBEDDED
          FAIL("Firebird database server not defined in settings file");
          return false;
  #endif
        }
        wxString strDatabase;
        if (config.Read(_("/Firebird/database"), &strDatabase) == false)
        {
          wxPrintf(_("Firebird database not defined in settings file"));
          FAIL("Firebird database not defined in settings file");
          return false;
        }
        wxString strUser;
        if (config.Read(_("/Firebird/user"), &strUser) == false)
        {
          wxPrintf(_("Firebird user not defined in settings file"));
          FAIL("Firebird user not defined in settings file");
          return false;
        }
        wxString strPassword;
        if (config.Read(_("/Firebird/password"), &strPassword) == false)
        {
          wxPrintf(_("Firebird password not defined in settings file"));
          FAIL("Firebird password not defined in settings file");
          return false;
        }
        
        if (LoadSqlMap(_("Firebird")) == false)
        {
          wxPrintf(_("Error loading database specific SQL statements"));
          FAIL("Error loading database specific SQL statements");
          return false;
        }

  #ifdef USE_FIREBIRD_EMBEDDED
        m_pDatabaseLayer = new FirebirdDatabaseLayer(strDatabase, strUser, strPassword);
  #else
        m_pDatabaseLayer = new FirebirdDatabaseLayer(strServer, strDatabase, strUser, strPassword);
        // Alternatively, you can do something like the following:
        //m_pDatabaseLayer = new FirebirdDatabaseLayer();
        //((FirebirdDatabaseLayer*)m_pDatabaseLayer)->SetServer(strServer);
        //((FirebirdDatabaseLayer*)m_pDatabaseLayer)->SetDatabase(strDatabase);
        //((FirebirdDatabaseLayer*)m_pDatabaseLayer)->SetUser(strUser);
        //((FirebirdDatabaseLayer*)m_pDatabaseLayer)->SetPassword(strPassword);
        //((FirebirdDatabaseLayer*)m_pDatabaseLayer)->Open();
  #endif
#elif USE_POSTGRESQL
        m_pDatabaseLayer = NULL;

		wxString strLibraryLocation;
        if (config.Read(_("/PostgreSQL/library_location"), &strLibraryLocation))
        {
#ifdef WIN32
          wxString strSeparator = _(";");
#else
          wxString strSeparator = _(":");
#endif
          wxSetEnv(_("PATH"), strLibraryLocation + strSeparator + m_strPath);
		}

        if (!PostgresDatabaseLayer::IsAvailable())
        {
          wxPrintf(_("PostgreSQL database backend is not available"));
          FAIL("PostgreSQL database backend is not available");
          return false;
        }
        wxString strDatabase; // This is the only mandatory setting
        if (config.Read(_("/PostgreSQL/database"), &strDatabase) == false)
        {
          wxPrintf(_("PostgreSQL database not defined in settings file"));
          FAIL("PostgreSQL database not defined in settings file");
          return false;
        }
        // Optional settings
        bool haveServerInfo = false;
        bool haveUserInfo = false;
        wxString strServer;
        if (config.Read(_("/PostgreSQL/server"), &strServer, wxEmptyString))
          haveServerInfo = true;
        wxString strUser;
        if (config.Read(_("/PostgreSQL/user"), &strUser, wxEmptyString))
          haveUserInfo = true;
        wxString strPassword;
        config.Read(_("/PostgreSQL/password"), &strPassword, wxEmptyString);
        int nPort = 5432;
        config.Read(_("/PostgreSQL/port"), &nPort, 5432);
        
        if (LoadSqlMap(_("PostgreSQL")) == false)
        {
          wxPrintf(_("Error loading database specific SQL statements"));
          FAIL("Error loading database specific SQL statements");
          return false;
        }

        if (haveServerInfo && haveUserInfo)
          m_pDatabaseLayer = new PostgresDatabaseLayer(strServer, nPort, strDatabase, strUser, strPassword);
        else if (haveServerInfo && !haveUserInfo)
          m_pDatabaseLayer = new PostgresDatabaseLayer(strServer, strDatabase);
        else if (haveUserInfo && !haveServerInfo)
          m_pDatabaseLayer = new PostgresDatabaseLayer(strDatabase, strUser, strPassword);
        else // We just have the database name
          m_pDatabaseLayer = new PostgresDatabaseLayer(strDatabase);
#elif USE_MYSQL
        m_pDatabaseLayer = NULL;

		wxString strLibraryLocation;
        if (config.Read(_("/MySQL/library_location"), &strLibraryLocation))
        {
#ifdef WIN32
          wxString strSeparator = _(";");
#else
          wxString strSeparator = _(":");
#endif
          wxSetEnv(_("PATH"), strLibraryLocation + strSeparator + m_strPath);
		}

        if (!MysqlDatabaseLayer::IsAvailable())
        {
          wxPrintf(_("MySQL database backend is not available"));
          FAIL("MySQL database backend is not available");
          return false;
        }
        wxString strServer;
        if (config.Read(_("/MySQL/server"), &strServer) == false)
        {
          wxPrintf(_("MySQL database server not defined in settings file"));
          FAIL("MySQL database server not defined in settings file");
          return false;
        }
        wxString strDatabase;
        if (config.Read(_("/MySQL/database"), &strDatabase) == false)
        {
          wxPrintf(_("MySQL database not defined in settings file"));
          FAIL("MySQL database not defined in settings file");
          return false;
        }
        wxString strUser;
        if (config.Read(_("/MySQL/user"), &strUser) == false)
        {
          wxPrintf(_("MySQL user not defined in settings file"));
          FAIL("MySQL user not defined in settings file");
          return false;
        }
        wxString strPassword;
        if (config.Read(_("/MySQL/password"), &strPassword) == false)
        {
          wxPrintf(_("MySQL password not defined in settings file"));
          FAIL("MySQL password not defined in settings file");
          return false;
        }
        
        if (LoadSqlMap(_("MySQL")) == false)
        {
          wxPrintf(_("Error loading database specific SQL statements"));
          FAIL("Error loading database specific SQL statements");
          return false;
        }

        m_pDatabaseLayer = new MysqlDatabaseLayer(strServer, strDatabase, strUser, strPassword);
#elif USE_ORACLE
        m_pDatabaseLayer = NULL;

		wxString strLibraryLocation;
        if (config.Read(_("/Oracle/library_location"), &strLibraryLocation))
        {
#ifdef WIN32
          wxString strSeparator = _(";");
#else
          wxString strSeparator = _(":");
#endif
          wxSetEnv(_("PATH"), strLibraryLocation + strSeparator + m_strPath);
		}

        wxString strUser;
        if (config.Read(_("/Oracle/user"), &strUser) == false)
        {
          wxPrintf(_("Oracle user not defined in settings file"));
          FAIL("Oracle user not defined in settings file");
          return false;
        }
        wxString strPassword;
        if (config.Read(_("/Oracle/password"), &strPassword) == false)
        {
          wxPrintf(_("Oracle password not defined in settings file"));
          FAIL("Oracle password not defined in settings file");
          return false;
        }
        wxString strDatabase;
        bool bDatabaseSpecified = false;
        if (config.Read(_("/Oracle/database"), &strDatabase) == true)
        {
          bDatabaseSpecified = true;
        }
        
        if (LoadSqlMap(_("Oracle")) == false)
        {
          wxPrintf(_("Error loading database specific SQL statements"));
          FAIL("Error loading database specific SQL statements");
          return false;
        }

        //m_pDatabaseLayer = new OracleDatabaseLayer(strServer, strDatabase, strUser, strPassword);
        //m_pDatabaseLayer = new OracleDatabaseLayer(strUser, strPassword, true);
        if (bDatabaseSpecified)
          m_pDatabaseLayer = new OTLDatabaseLayer(strUser, strPassword, strDatabase);
        else
          m_pDatabaseLayer = new OTLDatabaseLayer(strUser, strPassword, true);
        
#elif USE_ODBC
        m_pDatabaseLayer = NULL;

		wxString strLibraryLocation;
        if (config.Read(_("/ODBC/library_location"), &strLibraryLocation))
        {
#ifdef WIN32
          wxString strSeparator = _(";");
#else
          wxString strSeparator = _(":");
#endif
          wxSetEnv(_("PATH"), strLibraryLocation + strSeparator + m_strPath);
		}

        if (!OdbcDatabaseLayer::IsAvailable())
        {
          wxPrintf(_("ODBC database backend is not available"));
          FAIL("ODBC database backend is not available");
          return false;
        }
        wxString strDSN;
        if (config.Read(_("/ODBC/DSN"), &strDSN) == false)
        {
          wxPrintf(_("ODBC DSN not defined in settings file"));
          FAIL("ODBC DSN not defined in settings file");
          return false;
        }
        wxString strDbType;
        if (config.Read(_("/ODBC/DbType"), &strDbType) == false)
        {
          wxPrintf(_("ODBC DbType not defined in settings file"));
          FAIL("ODBC DbType not defined in settings file");
          return false;
        }

        if (LoadSqlMap(strDbType) == false)
        {
          wxPrintf(_("Error loading database specific SQL statements"));
          FAIL("Error loading database specific SQL statements");
          return false;
        }

        // Optional settings
        bool haveUserInfo = false;
        wxString strUser;
        if (config.Read(_("/ODBC/user"), &strUser, wxEmptyString))
          haveUserInfo = true;
        wxString strPassword;
        config.Read(_("/ODBC/password"), &strPassword, wxEmptyString);


        m_pDatabaseLayer = new OdbcDatabaseLayer();
        if (haveUserInfo)
          ((OdbcDatabaseLayer*)m_pDatabaseLayer)->Open(strDSN, strUser, strPassword);
        else // We just have the DSN
          ((OdbcDatabaseLayer*)m_pDatabaseLayer)->Open(strDSN, wxEmptyString, wxEmptyString);
#elif USE_TDS
        m_pDatabaseLayer = NULL;

		wxString strLibraryLocation;
        if (config.Read(_("/Tds/library_location"), &strLibraryLocation))
        {
#ifdef WIN32
          wxString strSeparator = _(";");
#else
          wxString strSeparator = _(":");
#endif
          wxSetEnv(_("PATH"), strLibraryLocation + strSeparator + m_strPath);
		}

        wxString strServer;
        if (config.Read(_("/Tds/server"), &strServer) == false)
        {
          wxPrintf(_("Tds database server not defined in settings file"));
          FAIL("Tds database server not defined in settings file");
          return false;
        }
        wxString strDatabase;
        if (config.Read(_("/Tds/database"), &strDatabase) == false)
        {
          wxPrintf(_("Tds database not defined in settings file"));
          FAIL("Tds database not defined in settings file");
          return false;
        }
        wxString strUser;
        if (config.Read(_("/Tds/user"), &strUser) == false)
        {
          wxPrintf(_("Tds user not defined in settings file"));
          FAIL("Tds user not defined in settings file");
          return false;
        }
        wxString strPassword;
        if (config.Read(_("/Tds/password"), &strPassword) == false)
        {
          wxPrintf(_("Tds password not defined in settings file"));
          FAIL("Tds password not defined in settings file");
          return false;
        }
        wxString strTdsVersion;
        int nTdsVersion = TdsDatabaseLayer::TDS_80;
        if (config.Read(_("/Tds/version"), &strTdsVersion) == false)
        {
          wxPrintf(_("Tds version not defined in settings file.  Defaulting to 8.0"));
        }
        else
        {
          if (strTdsVersion == _("4.2"))
            nTdsVersion = TdsDatabaseLayer::TDS_42;
          else if (strTdsVersion == _("4.6"))
            nTdsVersion = TdsDatabaseLayer::TDS_46;
          else if (strTdsVersion == _("5.0"))
            nTdsVersion = TdsDatabaseLayer::TDS_50;
          else if (strTdsVersion == _("7.0"))
            nTdsVersion = TdsDatabaseLayer::TDS_70;
        }
        
        if (LoadSqlMap(_("TDS")) == false)
        {
          wxPrintf(_("Error loading database specific SQL statements"));
          FAIL("Error loading database specific SQL statements");
          return false;
        }

        m_pDatabaseLayer = new TdsDatabaseLayer(strServer, strDatabase, strUser, strPassword, nTdsVersion);
#endif

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
        }
        catch (DatabaseLayerException& e)
        {
          ProcessException(e, false);
          wxDELETE(m_pDatabaseLayer);
          return false;
        }
#endif
        ASSERT(m_pDatabaseLayer != NULL);
        if (m_pDatabaseLayer == NULL)
          return false;

        return true;
    }

      
    //  so we're going to store a map based on the database type
    bool LoadSqlMap(const wxString& dbType)
    {
      if (dbType.CmpNoCase(_("Firebird")) == 0)
      {
// Firebird
        wxArrayString createTable1Scripts;
        createTable1Scripts.Add(_("CREATE TABLE table1 (column1 VARCHAR(50), column2 VARCHAR(50));"));
        m_SqlMap[CREATE_TABLE1] = createTable1Scripts;

        wxArrayString createTable2Scripts;
        createTable2Scripts.Add(_("CREATE TABLE table2 (IntCol int, DoubleCol double precision, StringCol VARCHAR(50));"));
        m_SqlMap[CREATE_TABLE2] = createTable2Scripts;

        wxArrayString createTable3Scripts;
        createTable3Scripts.Add(_("CREATE TABLE table3 (IntCol int, DateCol TIMESTAMP);"));
        m_SqlMap[CREATE_TABLE3] = createTable3Scripts;

        wxArrayString insertIntoTable3Scripts;
        insertIntoTable3Scripts.Add(_("INSERT INTO table3 (IntCol, DateCol) VALUES (1, '01/21/06 16:42:23');"));
        m_SqlMap[INSERT_INTO_TABLE3] = insertIntoTable3Scripts;

        wxArrayString createTable4Scripts;
        createTable4Scripts.Add(_("CREATE TABLE table4 (IntCol int, BlobCol BLOB);"));
        m_SqlMap[CREATE_TABLE4] = createTable4Scripts;

        wxArrayString createTable5Scripts;
        createTable5Scripts.Add(_("CREATE TABLE table5 (stringField VARCHAR(255));"));
        m_SqlMap[CREATE_TABLE5] = createTable5Scripts;

        wxArrayString createTable6Scripts;
        createTable6Scripts.Add(_("CREATE TABLE table6 (IntCol int, DoubleCol double precision, StringCol VARCHAR(50), BlobCol BLOB);"));
        m_SqlMap[CREATE_TABLE6] = createTable6Scripts;

        wxArrayString createTable7Scripts;
        createTable7Scripts.Add(_("CREATE TABLE table7 (KeyCol int, IntCol int, DoubleCol double precision, StringCol VARCHAR(50), BlobCol BLOB, DateCol TIMESTAMP);"));
        m_SqlMap[CREATE_TABLE7] = createTable7Scripts;

        wxArrayString createTable8Scripts;
        createTable8Scripts.Add(_("CREATE TABLE table8 (IntCol int, DoubleCol double precision, StringCol VARCHAR(50), BlobCol BLOB, DateCol TIMESTAMP);"));
        m_SqlMap[CREATE_TABLE8] = createTable8Scripts;

        wxArrayString createComplexRollbackTablesScripts;
        createComplexRollbackTablesScripts.Add(_("CREATE GENERATOR GEN_PK_MAIN_TABLE;"));
        createComplexRollbackTablesScripts.Add(_("CREATE TABLE mainTable (mainId INTEGER NOT NULL, description VARCHAR(50));"));
        createComplexRollbackTablesScripts.Add(_("CREATE TRIGGER BI_MAINTABLE FOR mainTABLE ACTIVE BEFORE INSERT AS BEGIN IF(NEW.mainId IS NULL) THEN NEW.mainId = GEN_ID(GEN_PK_MAIN_TABLE, 1); END"));
        createComplexRollbackTablesScripts.Add(_("CREATE GENERATOR GEN_PK_SECONDARY_TABLE;"));
        createComplexRollbackTablesScripts.Add(_("CREATE TABLE secondaryTable (secondaryTableId INTEGER NOT NULL, foreignKey INTEGER, description VARCHAR(50));"));
        createComplexRollbackTablesScripts.Add(_("CREATE TRIGGER BI_SECONDARYTABLE FOR secondaryTABLE ACTIVE BEFORE INSERT AS BEGIN IF(NEW.secondaryTableId IS NULL) THEN NEW.secondaryTableId = GEN_ID(GEN_PK_SECONDARY_TABLE, 1); END"));
        m_SqlMap[CREATE_COMPLEX_ROLLBACK_TABLES] = createComplexRollbackTablesScripts;

        wxArrayString dropComplexRollbackTablesScripts;
        dropComplexRollbackTablesScripts.Add(_("DROP GENERATOR GEN_PK_SECONDARY_TABLE;"));
        dropComplexRollbackTablesScripts.Add(_("DROP GENERATOR GEN_PK_MAIN_TABLE;"));
        m_SqlMap[DROP_COMPLEX_ROLLBACK_TABLES] = dropComplexRollbackTablesScripts;

        wxArrayString createTable9Scripts;
        createTable9Scripts.Add(_("CREATE TABLE table9 (IntCol int, DateCol DATE);"));
        m_SqlMap[CREATE_TABLE9] = createTable9Scripts;

		wxArrayString insertIntoTable9Scripts;
		insertIntoTable9Scripts.Add(_("INSERT INTO table9 (IntCol, DateCol) VALUES (1, '01/21/2006');"));
		m_SqlMap[INSERT_INTO_TABLE9] = insertIntoTable9Scripts;

        wxArrayString createView1Scripts;
        createView1Scripts.Add(_("CREATE VIEW view1 (col1) AS SELECT table1.COLUMN1 FROM table1;"));
        m_SqlMap[CREATE_VIEW1] = createView1Scripts;
      }
      else if (dbType.CmpNoCase(_("MySQL")) == 0)
      {
// MySQL
        wxArrayString createTable1Scripts;
        //createTable1Scripts.Add(_("CREATE TABLE table1 (column1 VARCHAR(50), column2 VARCHAR(50)) TYPE=InnoDB;"));
        createTable1Scripts.Add(_("CREATE TABLE table1 (column1 VARCHAR(50), column2 TEXT) TYPE=InnoDB;")); // Using TEXT column for testTextRemnants
        m_SqlMap[CREATE_TABLE1] = createTable1Scripts;

        wxArrayString createTable2Scripts;
        createTable2Scripts.Add(_("CREATE TABLE table2 (IntCol int, DoubleCol double, StringCol VARCHAR(50));"));
        m_SqlMap[CREATE_TABLE2] = createTable2Scripts;

        wxArrayString createTable3Scripts;
        createTable3Scripts.Add(_("CREATE TABLE table3 (IntCol int, DateCol DATETIME);"));
        m_SqlMap[CREATE_TABLE3] = createTable3Scripts;

        wxArrayString insertIntoTable3Scripts;
        insertIntoTable3Scripts.Add(_("INSERT INTO table3 (IntCol, DateCol) VALUES (1, '06/01/21 16:42:23');"));
        m_SqlMap[INSERT_INTO_TABLE3] = insertIntoTable3Scripts;

        wxArrayString createTable4Scripts;
        createTable4Scripts.Add(_("CREATE TABLE table4 (IntCol int, BlobCol MEDIUMBLOB);"));
        m_SqlMap[CREATE_TABLE4] = createTable4Scripts;

        wxArrayString createTable5Scripts;
        createTable5Scripts.Add(_("CREATE TABLE table5 (stringField VARCHAR(255)) CHARACTER SET latin1 COLLATE latin1_danish_ci;"));
        m_SqlMap[CREATE_TABLE5] = createTable5Scripts;

        wxArrayString createTable6Scripts;
        createTable6Scripts.Add(_("CREATE TABLE table6 (IntCol int, DoubleCol double, StringCol VARCHAR(50), BlobCol BLOB);"));
        m_SqlMap[CREATE_TABLE6] = createTable6Scripts;

        wxArrayString createTable7Scripts;
        createTable7Scripts.Add(_("CREATE TABLE table7 (KeyCol int, IntCol int, DoubleCol double, StringCol VARCHAR(50), BlobCol BLOB, DateCol DATETIME);"));
        m_SqlMap[CREATE_TABLE7] = createTable7Scripts;

        wxArrayString createTable8Scripts;
        createTable8Scripts.Add(_("CREATE TABLE table8 (IntCol int, DoubleCol double, StringCol VARCHAR(50), BlobCol BLOB, DateCol DATETIME);"));
        m_SqlMap[CREATE_TABLE8] = createTable8Scripts;

        wxArrayString createComplexRollbackTablesScripts;
        createComplexRollbackTablesScripts.Add(_("CREATE TABLE mainTable (mainId int AUTO_INCREMENT PRIMARY KEY, description VARCHAR(50)) TYPE=InnoDB;"));
        createComplexRollbackTablesScripts.Add(_("CREATE TABLE secondaryTable (secondaryTableId int AUTO_INCREMENT PRIMARY KEY, foreignKey int, description VARCHAR(50)) TYPE=InnoDB;"));
        m_SqlMap[CREATE_COMPLEX_ROLLBACK_TABLES] = createComplexRollbackTablesScripts;

        //wxArrayString dropComplexRollbackTablesScripts;
        //dropComplexRollbackTablesScripts.Add(_(""));
        //m_SqlMap[DROP_COMPLEX_ROLLBACK_TABLES] = dropComplexRollbackTablesScripts;

        wxArrayString createTable9Scripts;
        createTable9Scripts.Add(_("CREATE TABLE table9 (IntCol int, DateCol DATE);"));
        m_SqlMap[CREATE_TABLE9] = createTable9Scripts;

		wxArrayString insertIntoTable9Scripts;
		insertIntoTable9Scripts.Add(_("INSERT INTO table9 (IntCol, DateCol) VALUES (1, '2006/01/21');"));
		m_SqlMap[INSERT_INTO_TABLE9] = insertIntoTable9Scripts;

        wxArrayString createView1Scripts;
        createView1Scripts.Add(_("CREATE VIEW view1 (col1) AS SELECT table1.COLUMN1 FROM table1;"));
        m_SqlMap[CREATE_VIEW1] = createView1Scripts;
      }
      else if (dbType.CmpNoCase(_("PostgreSQL")) == 0)
      {
// PostgreSQL
        wxArrayString createTable1Scripts;
        createTable1Scripts.Add(_("CREATE TABLE table1 (column1 VARCHAR(50), column2 VARCHAR(50));"));
        m_SqlMap[CREATE_TABLE1] = createTable1Scripts;

        wxArrayString createTable2Scripts;
        createTable2Scripts.Add(_("CREATE TABLE table2 (IntCol int, DoubleCol float, StringCol VARCHAR(50));"));
        m_SqlMap[CREATE_TABLE2] = createTable2Scripts;

        wxArrayString createTable3Scripts;
        createTable3Scripts.Add(_("CREATE TABLE table3 (IntCol int, DateCol TIMESTAMP);"));
        m_SqlMap[CREATE_TABLE3] = createTable3Scripts;

        wxArrayString insertIntoTable3Scripts;
        insertIntoTable3Scripts.Add(_("INSERT INTO table3 (IntCol, DateCol) VALUES (1, '01/21/06 16:42:23');"));
        m_SqlMap[INSERT_INTO_TABLE3] = insertIntoTable3Scripts;

        wxArrayString createTable4Scripts;
        createTable4Scripts.Add(_("CREATE TABLE table4 (IntCol int, BlobCol bytea);"));
        m_SqlMap[CREATE_TABLE4] = createTable4Scripts;

        wxArrayString createTable5Scripts;
        createTable5Scripts.Add(_("CREATE TABLE table5 (stringField VARCHAR(255));"));
        m_SqlMap[CREATE_TABLE5] = createTable5Scripts;

        wxArrayString createTable6Scripts;
        createTable6Scripts.Add(_("CREATE TABLE table6 (IntCol int, DoubleCol float, StringCol VARCHAR(50), BlobCol bytea);"));
        m_SqlMap[CREATE_TABLE6] = createTable6Scripts;

        wxArrayString createTable7Scripts;
        createTable7Scripts.Add(_("CREATE TABLE table7 (KeyCol int, IntCol int, DoubleCol float, StringCol VARCHAR(50), BlobCol bytea, DateCol TIMESTAMP);"));
        m_SqlMap[CREATE_TABLE7] = createTable7Scripts;

        wxArrayString createTable8Scripts;
        createTable8Scripts.Add(_("CREATE TABLE table8 (IntCol int, DoubleCol float, StringCol VARCHAR(50), BlobCol bytea, DateCol TIMESTAMP);"));
        m_SqlMap[CREATE_TABLE8] = createTable8Scripts;

        wxArrayString createComplexRollbackTablesScripts;
        createComplexRollbackTablesScripts.Add(_("CREATE TABLE mainTable (mainId SERIAL PRIMARY KEY, description VARCHAR(50));"));
        createComplexRollbackTablesScripts.Add(_("CREATE TABLE secondaryTable (secondaryTableId SERIAL PRIMARY KEY, foreignKey int, description VARCHAR(50));"));
        m_SqlMap[CREATE_COMPLEX_ROLLBACK_TABLES] = createComplexRollbackTablesScripts;

        //wxArrayString dropComplexRollbackTablesScripts;
        //dropComplexRollbackTablesScripts.Add(_(""));
        //m_SqlMap[DROP_COMPLEX_ROLLBACK_TABLES] = dropComplexRollbackTablesScripts;

        wxArrayString createTable9Scripts;
        createTable9Scripts.Add(_("CREATE TABLE table9 (IntCol int, DateCol DATE);"));
        m_SqlMap[CREATE_TABLE9] = createTable9Scripts;

		wxArrayString insertIntoTable9Scripts;
		insertIntoTable9Scripts.Add(_("INSERT INTO table9 (IntCol, DateCol) VALUES (1, '01/21/2006');"));
		m_SqlMap[INSERT_INTO_TABLE9] = insertIntoTable9Scripts;

        wxArrayString createView1Scripts;
        createView1Scripts.Add(_("CREATE VIEW view1 AS SELECT table1.COLUMN1 AS col1 FROM table1;"));
        m_SqlMap[CREATE_VIEW1] = createView1Scripts;
      }
      else if (dbType.CmpNoCase(_("SQLite")) == 0)
      {
// SQLite
        wxArrayString createTable1Scripts;
        createTable1Scripts.Add(_("CREATE TABLE table1 (column1 VARCHAR(50), column2 VARCHAR(50));"));
        m_SqlMap[CREATE_TABLE1] = createTable1Scripts;

        wxArrayString createTable2Scripts;
        createTable2Scripts.Add(_("CREATE TABLE table2 (IntCol int, DoubleCol double, StringCol VARCHAR(50));"));
        m_SqlMap[CREATE_TABLE2] = createTable2Scripts;

        wxArrayString createTable3Scripts;
        createTable3Scripts.Add(_("CREATE TABLE table3 (IntCol int, DateCol TIMESTAMP);"));
        m_SqlMap[CREATE_TABLE3] = createTable3Scripts;

        wxArrayString insertIntoTable3Scripts;
        insertIntoTable3Scripts.Add(_("INSERT INTO table3 (IntCol, DateCol) VALUES (1, '01/21/06 16:42:23');"));
        m_SqlMap[INSERT_INTO_TABLE3] = insertIntoTable3Scripts;

        wxArrayString createTable4Scripts;
        createTable4Scripts.Add(_("CREATE TABLE table4 (IntCol int, BlobCol BLOB);"));
        m_SqlMap[CREATE_TABLE4] = createTable4Scripts;

        wxArrayString createTable5Scripts;
        createTable5Scripts.Add(_("CREATE TABLE table5 (stringField VARCHAR(255));"));
        m_SqlMap[CREATE_TABLE5] = createTable5Scripts;

        wxArrayString createTable6Scripts;
        createTable6Scripts.Add(_("CREATE TABLE table6 (IntCol int, DoubleCol double, StringCol VARCHAR(50), BlobCol BLOB);"));
        m_SqlMap[CREATE_TABLE6] = createTable6Scripts;

        wxArrayString createTable7Scripts;
        createTable7Scripts.Add(_("CREATE TABLE table7 (KeyCol int, IntCol int, DoubleCol double, StringCol VARCHAR(50), BlobCol BLOB, DateCol TIMESTAMP);"));
        m_SqlMap[CREATE_TABLE7] = createTable7Scripts;

        wxArrayString createTable8Scripts;
        createTable8Scripts.Add(_("CREATE TABLE table8 (IntCol int, DoubleCol double, StringCol VARCHAR(50), BlobCol BLOB, DateCol TIMESTAMP);"));
        m_SqlMap[CREATE_TABLE8] = createTable8Scripts;

        wxArrayString createComplexRollbackTablesScripts;
        createComplexRollbackTablesScripts.Add(_("CREATE TABLE mainTable (mainId int PRIMARY KEY, description VARCHAR(50));"));
        createComplexRollbackTablesScripts.Add(_("CREATE TABLE secondaryTable (secondaryTableId int PRIMARY KEY, foreignKey int, description VARCHAR(50));"));
        m_SqlMap[CREATE_COMPLEX_ROLLBACK_TABLES] = createComplexRollbackTablesScripts;

        //wxArrayString dropComplexRollbackTablesScripts;
        //dropComplexRollbackTablesScripts.Add(_(""));
        //m_SqlMap[DROP_COMPLEX_ROLLBACK_TABLES] = dropComplexRollbackTablesScripts;

        wxArrayString createTable9Scripts;
        createTable9Scripts.Add(_("CREATE TABLE table9 (IntCol int, DateCol DATE);"));
        m_SqlMap[CREATE_TABLE9] = createTable9Scripts;

		wxArrayString insertIntoTable9Scripts;
		insertIntoTable9Scripts.Add(_("INSERT INTO table9 (IntCol, DateCol) VALUES (1, '01/21/2006');"));
		m_SqlMap[INSERT_INTO_TABLE9] = insertIntoTable9Scripts;

        wxArrayString createView1Scripts;
        createView1Scripts.Add(_("CREATE VIEW view1 AS SELECT table1.COLUMN1 AS col1 FROM table1;"));
        m_SqlMap[CREATE_VIEW1] = createView1Scripts;
      }
      else if (dbType.CmpNoCase(_("Oracle")) == 0)
      {
// Oracle - These SQL statements haven't been validated to work
        wxArrayString createTable1Scripts;
        createTable1Scripts.Add(_("CREATE TABLE table1 (column1 VARCHAR(50), column2 VARCHAR(50));"));
        m_SqlMap[CREATE_TABLE1] = createTable1Scripts;

        wxArrayString createTable2Scripts;
        createTable2Scripts.Add(_("CREATE TABLE table2 (IntCol int, DoubleCol double precision, StringCol VARCHAR(50));"));
        m_SqlMap[CREATE_TABLE2] = createTable2Scripts;

        wxArrayString createTable3Scripts;
        createTable3Scripts.Add(_("CREATE TABLE table3 (IntCol int, DateCol TIMESTAMP);"));
        m_SqlMap[CREATE_TABLE3] = createTable3Scripts;

        wxArrayString insertIntoTable3Scripts;
        insertIntoTable3Scripts.Add(_("INSERT INTO table3 (IntCol, DateCol) VALUES (1, to_timestamp('01/21/2006 16:42:23', 'MM/DD/YYYY HH24:MI:SS'));"));
        m_SqlMap[INSERT_INTO_TABLE3] = insertIntoTable3Scripts;

        wxArrayString createTable4Scripts;
        createTable4Scripts.Add(_("CREATE TABLE table4 (IntCol int, BlobCol BLOB);"));
        m_SqlMap[CREATE_TABLE4] = createTable4Scripts;

        wxArrayString createTable5Scripts;
        createTable5Scripts.Add(_("CREATE TABLE table5 (stringField VARCHAR(255));"));
        m_SqlMap[CREATE_TABLE5] = createTable5Scripts;

        wxArrayString createTable6Scripts;
        createTable6Scripts.Add(_("CREATE TABLE table6 (IntCol int, DoubleCol double precision, StringCol VARCHAR(50), BlobCol BLOB);"));
        m_SqlMap[CREATE_TABLE6] = createTable6Scripts;

        wxArrayString createTable7Scripts;
        createTable7Scripts.Add(_("CREATE TABLE table7 (KeyCol int, IntCol int, DoubleCol double precision, StringCol VARCHAR(50), BlobCol BLOB, DateCol TIMESTAMP);"));
        m_SqlMap[CREATE_TABLE7] = createTable7Scripts;

        wxArrayString createTable8Scripts;
        createTable8Scripts.Add(_("CREATE TABLE table8 (IntCol int, DoubleCol double precision, StringCol VARCHAR(50), BlobCol BLOB, DateCol TIMESTAMP);"));
        m_SqlMap[CREATE_TABLE8] = createTable8Scripts;

        wxArrayString createComplexRollbackTablesScripts;
        createComplexRollbackTablesScripts.Add(_("CREATE SEQUENCE GEN_PK_MAIN_TABLE;"));
        createComplexRollbackTablesScripts.Add(_("CREATE TABLE mainTable (mainId INTEGER NOT NULL, description VARCHAR(50));"));
        createComplexRollbackTablesScripts.Add(_("CREATE TRIGGER BI_MAINTABLE FOR mainTABLE ACTIVE BEFORE INSERT AS BEGIN IF(NEW.mainId IS NULL) THEN NEW.mainId = GEN_PK_MAIN_TABLE.NEXTVAL; END"));
        createComplexRollbackTablesScripts.Add(_("CREATE SEQUENCE GEN_PK_SECONDARY_TABLE;"));
        createComplexRollbackTablesScripts.Add(_("CREATE TABLE secondaryTable (secondaryTableId INTEGER NOT NULL, foreignKey INTEGER, description VARCHAR(50));"));
        createComplexRollbackTablesScripts.Add(_("CREATE TRIGGER BI_SECONDARYTABLE FOR secondaryTABLE ACTIVE BEFORE INSERT AS BEGIN IF(NEW.secondaryTableId IS NULL) THEN NEW.secondaryTableId = GEN_PK_SECONDARY_TABLE.NEXTVAL); END"));
        m_SqlMap[CREATE_COMPLEX_ROLLBACK_TABLES] = createComplexRollbackTablesScripts;

        //wxArrayString dropComplexRollbackTablesScripts;
        //dropComplexRollbackTablesScripts.Add(_(""));
        //m_SqlMap[DROP_COMPLEX_ROLLBACK_TABLES] = dropComplexRollbackTablesScripts;

        wxArrayString createTable9Scripts;
        createTable9Scripts.Add(_("CREATE TABLE table9 (IntCol int, DateCol DATE);"));
        m_SqlMap[CREATE_TABLE9] = createTable9Scripts;

		wxArrayString insertIntoTable9Scripts;
		insertIntoTable9Scripts.Add(_("INSERT INTO table9 (IntCol, DateCol) VALUES (1, '01/21/2006');"));
		m_SqlMap[INSERT_INTO_TABLE9] = insertIntoTable9Scripts;

        wxArrayString createView1Scripts;
        createView1Scripts.Add(_("CREATE VIEW view1 AS SELECT table1.COLUMN1 AS col1 FROM table1;"));
        m_SqlMap[CREATE_VIEW1] = createView1Scripts;
      }
      else if (dbType.CmpNoCase(_("TDS")) == 0)
      {
// TDS
        wxArrayString createTable1Scripts;
        createTable1Scripts.Add(_("CREATE TABLE table1 (column1 VARCHAR(50), column2 VARCHAR(50));"));
        m_SqlMap[CREATE_TABLE1] = createTable1Scripts;

        wxArrayString createTable2Scripts;
        createTable2Scripts.Add(_("CREATE TABLE table2 (IntCol int, DoubleCol float, StringCol VARCHAR(50));"));
        m_SqlMap[CREATE_TABLE2] = createTable2Scripts;

        wxArrayString createTable3Scripts;
        createTable3Scripts.Add(_("CREATE TABLE table3 (IntCol int, DateCol DATETIME);"));
        m_SqlMap[CREATE_TABLE3] = createTable3Scripts;

        wxArrayString insertIntoTable3Scripts;
        insertIntoTable3Scripts.Add(_("INSERT INTO table3 (IntCol, DateCol) VALUES (1, '01/21/06 16:42:23');"));
        m_SqlMap[INSERT_INTO_TABLE3] = insertIntoTable3Scripts;

        wxArrayString createTable4Scripts;
        createTable4Scripts.Add(_("CREATE TABLE table4 (IntCol int, BlobCol image);"));
        m_SqlMap[CREATE_TABLE4] = createTable4Scripts;

        wxArrayString createTable5Scripts;
        createTable5Scripts.Add(_("CREATE TABLE table5 (stringField VARCHAR(255));"));
        m_SqlMap[CREATE_TABLE5] = createTable5Scripts;

        wxArrayString createTable6Scripts;
        createTable6Scripts.Add(_("CREATE TABLE table6 (IntCol int, DoubleCol float, StringCol VARCHAR(50), BlobCol image);"));
        m_SqlMap[CREATE_TABLE6] = createTable6Scripts;

        wxArrayString createTable7Scripts;
        createTable7Scripts.Add(_("CREATE TABLE table7 (KeyCol int, IntCol int, DoubleCol float, StringCol VARCHAR(50), BlobCol image, DateCol DATETIME);"));
        m_SqlMap[CREATE_TABLE7] = createTable7Scripts;

        wxArrayString createTable8Scripts;
        createTable8Scripts.Add(_("CREATE TABLE table8 (IntCol int, DoubleCol float, StringCol VARCHAR(50), BlobCol image, DateCol DATETIME);"));
        m_SqlMap[CREATE_TABLE8] = createTable8Scripts;

        wxArrayString createComplexRollbackTablesScripts;
        createComplexRollbackTablesScripts.Add(_("CREATE TABLE mainTable (mainId int IDENTITY, description VARCHAR(50));"));
        createComplexRollbackTablesScripts.Add(_("CREATE TABLE secondaryTable (secondaryTableId int IDENTITY, foreignKey int, description VARCHAR(50));"));
        m_SqlMap[CREATE_COMPLEX_ROLLBACK_TABLES] = createComplexRollbackTablesScripts;

        //wxArrayString dropComplexRollbackTablesScripts;
        //dropComplexRollbackTablesScripts.Add(_(""));
        //m_SqlMap[DROP_COMPLEX_ROLLBACK_TABLES] = dropComplexRollbackTablesScripts;

        wxArrayString createTable9Scripts;
        createTable9Scripts.Add(_("CREATE TABLE table9 (IntCol int, DateCol DATETIME);"));
        m_SqlMap[CREATE_TABLE9] = createTable9Scripts;

		wxArrayString insertIntoTable9Scripts;
		insertIntoTable9Scripts.Add(_("INSERT INTO table9 (IntCol, DateCol) VALUES (1, '01/21/2006');"));
		m_SqlMap[INSERT_INTO_TABLE9] = insertIntoTable9Scripts;

        wxArrayString createView1Scripts;
        createView1Scripts.Add(_("CREATE VIEW view1 AS SELECT table1.COLUMN1 AS col1 FROM table1;"));
        m_SqlMap[CREATE_VIEW1] = createView1Scripts;
      }
      else
      {
        // When adding new databsae backends to this check, it might be best just to copy the SQLite SQL block
        //  and change the SQL as needed.

        return false;
      }
      return true;
    }

    void CreateTable8()
    {
      ASSERT( m_pDatabaseLayer != NULL );
        
      if (m_pDatabaseLayer)
      {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
       try
       {
#endif  

       // Add a table just for this test
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
       try
       {
#endif
       SQL_STATEMENT_ARRAY::iterator createTableSql = m_SqlMap.find(CREATE_TABLE8);
       if (createTableSql != m_SqlMap.end())
       {
         for (unsigned int i=0; i<(*createTableSql).second.size(); i++)
         {
           m_pDatabaseLayer->RunQuery((*createTableSql).second.Item(i));
         }
       }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
       }
       catch (DatabaseLayerException& e)
       {
         // Don't stop on error here since this could be a "table already exists" error
         ProcessException(e, false);
       }
#endif

       // Make sure that the tearDown function knows to remove this test table
       m_DeleteStatements.push_back(_("DROP TABLE table8;"));

       m_pDatabaseLayer->RunQuery(_("DELETE FROM table8;")); // Be extra sure the table is empty
       DatabaseErrorCheck(m_pDatabaseLayer);
          
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
       }
       catch (DatabaseLayerException& e)
       {
         ProcessException(e);
       }
#endif
      }
    }

    void PopulateTable8()
    {
      ASSERT( m_pDatabaseLayer != NULL );
        
     if (m_pDatabaseLayer)
     {
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
       try
       {
#endif  
       PreparedStatement* pStatement = m_pDatabaseLayer->PrepareStatement(_("INSERT INTO table8 (IntCol, DoubleCol, StringCol, BlobCol, DateCol) VALUES (?, ?, ?, ?, ?)"));
          
       ASSERT(pStatement != NULL);
       DatabaseErrorCheck(pStatement);
       if (pStatement)
       {
         pStatement->SetParamInt(1, 10);
         DatabaseErrorCheck(pStatement);
         pStatement->SetParamDouble(2, 1.234);
         DatabaseErrorCheck(pStatement);
         pStatement->SetParamString(3, _("Test"));
         DatabaseErrorCheck(pStatement);

         wxString strBlob = _("Test Blob");
         // Use the length + 1 so that we get the null terminating character at the end of the string
         wxCharBuffer charBuffer = strBlob.mb_str(*wxConvCurrent);
         pStatement->SetParamBlob(4, (const unsigned char*)(const char*)charBuffer, strBlob.Length()+1);

         // Maybe we should use a REAL blob like this...
         //wxFile FileIn(_("wxwin.png"));
         //int nDataLength = FileIn.Length();
         //wxMemoryBuffer BufferIn;
         //int nBytesRead = FileIn.Read(BufferIn.GetWriteBuf(nDataLength), nDataLength);
         //ASSERT_EQUALS(nBytesRead, nDataLength);
         //pStatement->SetParamBlob(4, (unsigned char*)BufferIn.GetData(), nDataLength);

         DatabaseErrorCheck(pStatement);

         wxDateTime TimeNow = wxDateTime::Now();
         pStatement->SetParamDate(5, TimeNow);
         DatabaseErrorCheck(pStatement);
         pStatement->RunQuery();
         m_pDatabaseLayer->CloseStatement(pStatement);
       }
          
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
       }
       catch (DatabaseLayerException& e)
       {
         ProcessException(e);
       }
#endif
     }
    }

    wxFFile* CreateFileWithRandomContents(const wxString& fileName, size_t length)
    {
      wxFFile file(fileName, _T("wb"));
      wxFFileOutputStream out(file);

      srand(time(NULL));
      for (size_t i=0; i<length; i++)
        out.PutC(65 + (rand() % 60));

      out.Close();
      file.Close();

      wxFFile* returnFile = new wxFFile(fileName, _T("rb"));
      return returnFile;
    }
  };

REGISTER_FIXTURE( MyTestSuite );

