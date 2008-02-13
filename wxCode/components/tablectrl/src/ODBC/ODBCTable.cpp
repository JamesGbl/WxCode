////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        ODBCTable.cpp                                                 //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007                                                          //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#include <wx/wxprec.h>

#include <wx/arrimpl.cpp>

#include "wx/ODBC/ODBCTable.hpp"



////////////////////////////////////////////////////////////////////////////////
// wxODBCEnv                                                                  //
////////////////////////////////////////////////////////////////////////////////



wxODBCEnv :: wxODBCEnv ()
   : wxObject ()
{
   if ( SQLAllocEnv ( &env ) != SQL_SUCCESS )
      wxLogDebug ( "Could not SQLAllocEnv" );
}



wxODBCEnv :: ~wxODBCEnv ()
{
   if ( SQLFreeEnv ( env ) != SQL_SUCCESS )
      wxLogDebug ( "Could not SQLFreeEnv" );
}



bool  wxODBCEnv :: DataSources ( wxString &  dsn, wxString &  desc, const Fetch &  fetch )
{
   const size_t   DESC  = 1024;
   SWORD          dsnl;
   SWORD          descl;
   const bool     rc    = SQLDataSources ( env, fetch, reinterpret_cast < SQLCHAR * > ( dsn.GetWriteBuf ( SQL_MAX_DSN_LENGTH + 1 ) ), SQL_MAX_DSN_LENGTH, &dsnl, reinterpret_cast < SQLCHAR * > ( desc.GetWriteBuf ( DESC + 1 ) ), DESC, &descl ) == SQL_SUCCESS;
   
   if ( rc )
   {
      dsn .UngetWriteBuf ( dsnl  );
      desc.UngetWriteBuf ( descl );
   }
   else
   {
      dsn .UngetWriteBuf ();
      desc.UngetWriteBuf ();
   }
   
   return ( rc );
}



////////////////////////////////////////////////////////////////////////////////
// wxODBCDbc                                                                  //
////////////////////////////////////////////////////////////////////////////////



wxODBCDbc :: wxODBCDbc ( wxODBCEnv &  _env )
   : wxObject (),
   env   ( _env )
{
   if ( SQLAllocConnect ( env, &dbc ) != SQL_SUCCESS )
      wxLogDebug ( "Could not SQLAllocConnect" );
}



wxODBCDbc :: ~wxODBCDbc ()
{
   if ( SQLFreeConnect ( dbc ) != SQL_SUCCESS )
      wxLogDebug ( "Could not SQLFreeConnect" );
}



bool  wxODBCDbc :: Connect ( const wxString &  dsn, const wxString &  userid, const wxString &  passwd )
{
   SQLRETURN   rc = SQLSetConnectOption ( dbc, SQL_ODBC_CURSORS, SQL_CUR_USE_IF_NEEDED );
   
   return ( SQLConnect ( dbc, reinterpret_cast < SQLCHAR * > ( const_cast < wxChar * > ( dsn.c_str () ) ), SQL_NTS, reinterpret_cast < SQLCHAR * > ( const_cast < wxChar * > ( userid.c_str () ) ), SQL_NTS, reinterpret_cast < SQLCHAR * > ( const_cast < wxChar * > ( passwd.c_str () ) ), SQL_NTS ) == SQL_SUCCESS );
}



bool  wxODBCDbc :: GetTables ( wxODBCTableArray &  ta )
{
   ta.Empty ();
   
   wxODBCStatement   stmt  ( *this );
  
   if ( ! stmt.Tables () )
      return ( false );
      
   wxString    name;
   wxString    type;
   wxString    remarks;
   
   while ( stmt.Fetch () )
   {
      stmt.GetData ( 3, name    );
      stmt.GetData ( 4, type    );
      stmt.GetData ( 5, remarks );
      
      ta.Add ( wxODBCTable ( name, type, remarks ) );
   }
   
   return ( true );
}



bool  wxODBCDbc :: GetColumns ( wxODBCColumnArray &  ca, const wxString &  table )
{
   ca.Empty ();
   
   wxODBCStatement   stmt  ( *this );
  
   if ( ! stmt.Columns ( table ) )
      return ( false );
      
   wxString    name;
   short       type;
   wxString    typestr;
   long        length;
   short       buffer;
   short       decimals;
   short       radix;
   short       nullable;
   wxString    remarks;   
   
   while ( stmt.Fetch () )
   {
      stmt.GetData (  4, name     );
      stmt.GetData (  5, type     );
      stmt.GetData (  6, typestr  );
      stmt.GetData (  7, length   );
      stmt.GetData (  8, buffer   );
      stmt.GetData (  9, decimals );
      stmt.GetData ( 10, radix    );
      stmt.GetData ( 11, nullable );
      stmt.GetData ( 12, remarks  );
      
      ca.Add ( wxODBCColumn ( name, type, typestr, length, buffer, decimals, radix, nullable, remarks ) );
   }

#if 0
   SQLRETURN   rc;
   wxString    cmd   ( "SELECT * FROM " );
   
   cmd  += "\"" + table + "\"";
   cmd  += " WHERE 1 = 2";
   
   rc  = SQLPrepare ( stmt, reinterpret_cast < SQLCHAR * > ( const_cast < wxChar * > ( cmd.c_str () ) ), cmd.Length () );
   
   short    cols;
   
   if ( ( rc = SQLNumResultCols ( stmt, &cols ) ) != SQL_SUCCESS )
      return ( false );
      
   for ( short  c = 0 ; c < cols ; ++c )
   {
   }
#endif
   
   return ( true );
}



////////////////////////////////////////////////////////////////////////////////
// wxODBCStatement                                                            //
////////////////////////////////////////////////////////////////////////////////



bool  wxODBCStatement :: ExecDirect ( const wxString &  cmd )
{
   return ( SQLExecDirect ( stmt, reinterpret_cast < SQLCHAR * > ( const_cast < wxChar * > ( cmd.c_str () ) ), SQL_NTS ) == SQL_SUCCESS );
}



wxODBCStatement :: wxODBCStatement ( wxODBCDbc &  _dbc )
   : wxObject (),
   dbc   ( _dbc )
{
   if ( SQLAllocStmt ( dbc, &stmt ) != SQL_SUCCESS )
      wxLogDebug ( "Could not SQLAllocStmt" );
}



wxODBCStatement :: ~wxODBCStatement ()
{
   if ( SQLFreeStmt ( stmt, SQL_CLOSE ) != SQL_SUCCESS )
      wxLogDebug ( "Could not SQLFreeStmt" );
}



bool  wxODBCStatement :: Tables ()
{
   return ( SQLTables ( stmt, 0, 0, 0, 0, 0, 0, 0, 0 ) == SQL_SUCCESS );
}



bool  wxODBCStatement :: Columns ( const wxString &  table )
{
   return ( SQLColumns ( stmt, 0, 0, 0, 0, reinterpret_cast < SQLCHAR * > ( const_cast < wxChar * > ( table.c_str () ) ), SQL_NTS, 0, 0 ) == SQL_SUCCESS );
// return ( SQLColumns ( stmt, reinterpret_cast < SQLCHAR * > ( "" ), 0, 0, 0, 0, 0, 0, 0 ) == SQL_SUCCESS );
}



bool  wxODBCStatement :: SelectAll ( const wxString &  table )
{
   return ( ExecDirect ( "SELECT * FROM " + table ) );
}



wxUint32  wxODBCStatement :: CountAll ( const wxString &  table )
{
   wxODBCStatement   count ( dbc );
   
   if ( ! count.ExecDirect ( "SELECT COUNT(*) FROM " + table ) )
      return (  0 );

   if ( ! count.Fetch () )
      return (  0 );
      
   wxUint32    r;
   
   if ( ! count.GetData ( 1, r ) )
      return (  0 );
      
   return ( r );
}



bool  wxODBCStatement :: Fetch ()
{
   SQLRETURN   rc = SQLFetch ( stmt );
   
   return ( ( rc == SQL_SUCCESS ) || ( rc == SQL_SUCCESS_WITH_INFO ) );
}



bool  wxODBCStatement :: FetchScroll ( const Orientation &  orientation, wxInt32  offset )
{
   if ( ( orientation == Orientation_RELATIVE ) || ( orientation == Orientation_ABSOLUTE ) )
      wxASSERT ( offset > 0 );
   
   SQLRETURN   rc = SQLFetchScroll ( stmt, orientation, offset );
   
// if ( rc != SQL_SUCCESS )
      wxLogDebug ( "%s %s (%d) %ld", __FUNCTION__, GetString ( orientation ), rc, offset );
   
   return ( ( rc == SQL_SUCCESS ) || ( rc == SQL_SUCCESS_WITH_INFO ) );
}



bool  wxODBCStatement :: GetData ( size_t  c, wxString &  str )
{
   const SQLLEN   L  = 1024;
   SQLLEN         l;
   bool           r  = SQLGetData ( stmt, c, SQL_C_CHAR, str.GetWriteBuf ( L ), L, &l ) == SQL_SUCCESS;

   if ( ( l >= 0 ) && ( l < L ) )
      str.UngetWriteBuf ( l );
   else
      str.UngetWriteBuf ( 0 );
   
   return ( r );
}



bool  wxODBCStatement :: GetData ( size_t  c, short &  v )
{
   SQLLEN   l;
   bool     r  = SQLGetData ( stmt, c, SQL_C_SSHORT, &v, 0, &l ) == SQL_SUCCESS;

   return ( r );
}



bool  wxODBCStatement :: GetData ( size_t  c, long &  v )
{
   SQLLEN   l;
   bool     r  = SQLGetData ( stmt, c, SQL_C_SLONG, &v, 0, &l ) == SQL_SUCCESS;

   return ( r );
}



bool  wxODBCStatement :: GetData ( size_t  c, wxUint32 &  v )
{
   SQLLEN   l;
   bool     r  = SQLGetData ( stmt, c, SQL_C_ULONG, &v, sizeof ( v ), &l ) == SQL_SUCCESS;

   return ( r );
}



bool  wxODBCStatement :: SetCursorType ( const CursorType &  cursortype )
{
   return ( SQLSetStmtAttr ( stmt, SQL_ATTR_CURSOR_TYPE, reinterpret_cast < SQLPOINTER > ( cursortype ), 0 ) == SQL_SUCCESS );
}



const wxChar *  wxODBCStatement :: GetString ( const Orientation &  orientation )
{
   static const wxChar *   ORIENTATION [] =
   {
      "?"         ,
      "NXT"       ,
      "FST"       ,
      "LST"       ,
      "PRV"       ,
      "ABSOLUTE"  ,
      "RELATIVE"  ,
      "BOOKMARK"  ,
   };
   
   return ( ORIENTATION [ orientation ] );
}



////////////////////////////////////////////////////////////////////////////////
// wxODBCColumn                                                               //
////////////////////////////////////////////////////////////////////////////////



wxODBCColumn :: wxODBCColumn ()
   : wxObject ()
{
}



wxODBCColumn :: wxODBCColumn ( const wxString &  _name, short  _type, const wxString &  _typestr, long  _length, short  _buffer, short  _decimals, short  _radix, short  _nullable, const wxString &  _remarks )
   : wxObject ()
{
   name     = _name;
   type     = _type;
   typestr  = _typestr;
   length   = _length;
   buffer   = _buffer;
   decimals = _decimals;
   radix    = _radix;
   nullable = _nullable;
   
   remarks  = _remarks;
}



////////////////////////////////////////////////////////////////////////////////
// wxODBCColumnArray                                                          //
////////////////////////////////////////////////////////////////////////////////



WX_DEFINE_OBJARRAY   ( wxODBCColumnArray );



////////////////////////////////////////////////////////////////////////////////
// wxODBCTable                                                                //
////////////////////////////////////////////////////////////////////////////////



wxODBCTable :: wxODBCTable ()
   : wxObject ()
{
}



wxODBCTable :: wxODBCTable ( const wxString &  _name, const wxString &  _type, const wxString &  _remarks )
   : wxObject ()
{
   name     = _name;
   type     = _type;
   remarks  = _remarks;
}



////////////////////////////////////////////////////////////////////////////////
// wxODBCTableArray                                                           //
////////////////////////////////////////////////////////////////////////////////



WX_DEFINE_OBJARRAY   ( wxODBCTableArray   );



////////////////////////////////////////////////////////////////////////////////
// ODBCTable :: CursorEx                                                      //
////////////////////////////////////////////////////////////////////////////////


#if 0
const bool  ODBCTable :: CursorEx :: IsValid () const
{
   return ( super :: IsValid () && ( offset != 0 ) );
}



ODBCTable :: CursorEx :: CursorEx ()
   : GenericCursor < HSTMT > ()
{
}


   
ODBCTable :: CursorEx :: CursorEx ( HSTMT  _stmt )
   : GenericCursor < HSTMT > ( _stmt )
{
}


   
ODBCTable :: CursorEx &  ODBCTable :: CursorEx :: operator = ( const CursorEx &  that )
{
   memcpy ( offset, that.offset, sizeof ( HSTMT ) );
   
   valid    = that.valid;
   
   return ( *this );
}



HSTMT *  ODBCTable :: CursorEx :: Statement ()
{
   return ( &offset );
}
#endif


////////////////////////////////////////////////////////////////////////////////
// ODBCTable :: RecordEx                                                    //
////////////////////////////////////////////////////////////////////////////////



void  ODBCTable :: RecordEx :: _Index ( int  _index )
{
   super :: _Index ( _index );
}



const size_t  ODBCTable :: RecordEx :: __NumberOfRecords () const
{
   return ( rows );
}



const size_t  ODBCTable :: RecordEx :: __NumberOfKeys ( int ) const
{
   return ( 0 );
}



ODBCTable :: RecordEx :: Result  ODBCTable :: RecordEx :: __GetFst ( int )
{
   if ( stmt.FetchScroll ( wxODBCStatement :: Orientation_FST ) )
   {
      row   = 0;
      
      return ( Result_OK );
   }
   
   return ( Result_EOT );
}



ODBCTable :: RecordEx :: Result  ODBCTable :: RecordEx :: __GetNxt ()
{
   if ( stmt.FetchScroll ( wxODBCStatement :: Orientation_NXT ) )
   {
      ++row;
      
      wxASSERT ( row < rows );
      
      return ( Result_OK );
   }
      
   return ( Result_EOT );
}



long  ODBCTable :: RecordEx :: __GetNxtEx ( long  count )
{
   long     c  = 0;
   
   while ( ( c < count ) && ( stmt.FetchScroll ( wxODBCStatement :: Orientation_NXT ) ) )
   {
      ++row;
      ++c;
   }
      
   wxLogDebug  ( "%s %d/%d", __FUNCTION__, c, count );
   
   return ( c );
}



ODBCTable :: RecordEx :: Result  ODBCTable :: RecordEx :: __GetPrv ()
{
   if ( stmt.FetchScroll ( wxODBCStatement :: Orientation_PRV ) )
   {
      --row;
      
      wxASSERT ( row >= 0 );
      
      return ( Result_OK );
   }
      
   return ( Result_EOT );
}



long  ODBCTable :: RecordEx :: __GetPrvEx ( long  count )
{
   long  c  = 0;
   
   while ( ( c < count ) && ( stmt.FetchScroll ( wxODBCStatement :: Orientation_PRV ) ) )
   {
      --row;
      ++c;
   }
   
   wxLogDebug  ( "%s %d/%d", __FUNCTION__, c, count );
   
   return ( c );
}



ODBCTable :: RecordEx :: Result  ODBCTable :: RecordEx :: __GetLst ( int )
{
   if ( stmt.FetchScroll ( wxODBCStatement :: Orientation_LST ) )
   {
      row   = rows - 1;
      
      return ( Result_OK );
   }
      
   return ( Result_EOT );
}



ODBCTable :: RecordEx :: Result  ODBCTable :: RecordEx :: __Get ( const Find & )
{
   return ( Result_KEY_VALUE_NOT_FOUND );
}



ODBCTable :: Cursor *  ODBCTable :: RecordEx :: __CursorCreate ()
{
   DefaultCursor *   dc = new  DefaultCursor ( row + 1 );
   
   if ( row < 0 )
      dc -> Invalidate ();
      
   return ( dc );
}



const ODBCTable :: Cursor *  ODBCTable :: RecordEx :: __CursorCurrent ()
{
   cursor.Offset ( row + 1 );
   
   return ( &cursor );
}



void  ODBCTable :: RecordEx :: __CursorInvalidate ()
{
   cursor.Invalidate ();
}



ODBCTable :: RecordEx :: Result  ODBCTable :: RecordEx :: __CursorSet ( const Cursor *  _cursor )
{
   const DefaultCursor *   dc = static_cast < const DefaultCursor * > ( _cursor );
   
   if ( ( dc          != 0 ) &&
        ( dc -> IsValid () )    )
   {
      cursor   = *dc;
   
      // (SQL)FetchScroll is funny. It will not return data when used with 
      // SQL_FETCH_RELATIVE if the cursor is not with in the set, i.e. past 
      // GetLst!
      // So, try to fetch the record, if it fails, try to do a GetFst and than
      // try to fetch again.
      
      if ( stmt.FetchScroll ( wxODBCStatement :: Orientation_ABSOLUTE, cursor.Offset () ) )
//    if ( (   stmt.FetchScroll ( wxODBCStatement :: Orientation_ABSOLUTE, cursor.Offset ()   )    ) ||
//         ( ( stmt.FetchScroll ( wxODBCStatement :: Orientation_FST )                        ) &&
//           ( stmt.FetchScroll ( wxODBCStatement :: Orientation_ABSOLUTE, cursor.Offset () ) )    )    )
      {
         row   = cursor.Offset () - 1;
         
         return ( Result_OK );
      }
   }
   
   return ( Result_EOT );
}



ODBCTable :: RecordEx :: Result  ODBCTable :: RecordEx :: __ScrollGetByPos ( long  pos )
{
   return ( Result_EOT );
}



long  ODBCTable :: RecordEx :: __ScrollPosByCursor  ( const Cursor *  cursor )
{
   return ( static_cast < const DefaultCursor * > ( cursor ) -> Offset () );
}



ODBCTable :: RecordEx :: RecordEx ( ODBCTable *  _table )
   : Record ( _table ),
   cols  ( _table -> cols ),
   rows  ( _table -> rows ),
   stmt  ( _table -> stmt )
{
   row   =  0;
   cursor   .Offset     ( row + 1 );
   cursor   .Invalidate ();
}


#if 0   
const wxChar *  ODBCTable :: RecordEx :: FieldString ( size_t  c )
{
// table -> GetDbTable () -> GetColumn ( c ).GetValue ();
}
#endif


////////////////////////////////////////////////////////////////////////////////
// ODBCTable                                                                  //
////////////////////////////////////////////////////////////////////////////////



const wxChar *  ODBCTable :: Name () const
{
   return ( name );
}



const wxChar *  ODBCTable :: Desc () const
{
   return ( name );
}



size_t  ODBCTable :: NumberOfColumns ()
{
   return ( cols );
}



ODBCTable :: Column *  ODBCTable :: At ( size_t )
{
   return (  0 );
}



const wxChar *  ODBCTable :: FieldName ( size_t  i )
{
// return ( dbtable.GetColumn ( i ).GetName () );
   return ( ca [ i ].GetName () );
}



const wxChar *  ODBCTable :: FieldFullName ( size_t  i )
{
   static wxString   str;

// str.Printf ( "%s.%s", Name (), dbtable.GetColumn ( i ).GetName () );
   str.Printf ( "%s.%s", Name (), ca [ i ].GetName () );
      
   return ( str );
}



const wxChar *  ODBCTable :: HeaderString ( size_t  i )
{
// return ( dbtable.GetColumn ( i ).GetName () );
   return ( ca [ i ].GetName () );
}



wxUint32  ODBCTable :: HeaderDrawTextFormat ( size_t )
{
   return ( DT_LEFT );
}



ODBCTable :: Capability  ODBCTable :: GetCapability () const
{
   return ( static_cast < Capability > ( Capability_PERCENTAGE | Capability_HSCROLLTRACK | Capability_VSCROLLTRACK ) );
}



wxUint32  ODBCTable :: FieldFlag ( size_t ) const
{
   return ( TFF_SHOW | TFF_SHOW_TOOLTIP );
}



const wxChar *  ODBCTable :: FieldString ( size_t  i )
{
   static wxString   str;
   
// return ( record -> FieldString ( i ) );
   if ( ! stmt.GetData ( i + 1, str ) )
      str.Empty ();
            
   return ( str );
}



wxUint32  ODBCTable :: FieldDrawTextFormat ( size_t )
{
   return ( DT_LEFT );
}
     


ODBCTable :: ODBCTable ( wxODBCDbc *  _dbc, const wxString &  _name )
   : wxTable ( record = new  RecordEx ( this ) ),
   stmt  ( *_dbc )
{
   dbc   = _dbc;
   name  = _name;
   
   dbc   -> GetColumns ( ca, name );

   cols  = ca.Count ();
   rows  = stmt.CountAll   ( name );
  
   stmt.SetCursorType   ( wxODBCStatement :: CursorType_STATIC );
   stmt.SelectAll       ( name );
#if 0
   SQLINTEGER  v;

   SQLGetInfo ( *dbc, SQL_FETCH_DIRECTION, &v, sizeof ( v ), 0 );
#endif   
}



ODBCTable :: ~ODBCTable ()
{
   delete  record;
}
