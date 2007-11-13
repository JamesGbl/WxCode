// Soli Deo Gloria!



#include <wx/wxprec.h>

#include "ODBCTable.hpp"



////////////////////////////////////////////////////////////////////////////////
// wxDbTableEx                                                                //
////////////////////////////////////////////////////////////////////////////////



wxDbTableEx :: wxDbTableEx ( wxDb *  _db, const wxString &  _table, const wxUint16  _cols, const wxString &  _query, bool  _only, const wxString &  _path )
   : wxDbTable ( _db, _table, _cols, _query, _only, _path )
{
}


   
bool  wxDbTableEx :: UpdateColumns ( HSTMT  cursor )
{
   // Bind each column of the table to a memory address for fetching data

   const wxUint16    COLS     = GetNumberOfColumns ();
   wxDbColDef *      coldefs  = GetColDefs ();
   wxDb *            db       = GetDb ();
   RETCODE           rc;
   
   for ( wxUint16  i = 0 ; i < COLS ; ++i )
   {
      wxDbColDef &   cd = coldefs [ i ];

      if ( ( rc = SQLBindCol ( cursor, i + 1, cd.SqlCtype, static_cast < UCHAR * > ( cd.PtrDataObj ), cd.SzDataObj, &cd.CbValue ) ) != SQL_SUCCESS )
         return ( db -> DispAllErrors ( db -> GetHENV (), db -> GetHDBC (), cursor ) );
   }

   return ( true );
}



bool  wxDbTableEx :: SetPos ( wxInt16  pos )
{
   wxDb *      db       = GetDb ();
   RETCODE     rc;
   
   if ( ( rc = SQLSetPos ( GetCursor (), pos, SQL_REFRESH, SQL_LOCK_NO_CHANGE ) ) != SQL_SUCCESS )
   {
      wxLogDebug ( "SetPos : %d failed %d", pos, rc );
      return ( db -> DispAllErrors ( db -> GetHENV (), db -> GetHDBC (), GetCursor () ) );
   }
      
   wxLogDebug ( "SetPos: %d OK", pos );
// UpdateColumns ( GetCursor () );

   return ( true );      
}



////////////////////////////////////////////////////////////////////////////////
// ODBCTable :: ODBCCursor                                                    //
////////////////////////////////////////////////////////////////////////////////


#if 0
const bool  ODBCTable :: ODBCCursor :: IsValid () const
{
   return ( super :: IsValid () && ( offset != 0 ) );
}



ODBCTable :: ODBCCursor :: ODBCCursor ()
   : GenericCursor < HSTMT > ()
{
}


   
ODBCTable :: ODBCCursor :: ODBCCursor ( HSTMT  _stmt )
   : GenericCursor < HSTMT > ( _stmt )
{
}


   
ODBCTable :: ODBCCursor &  ODBCTable :: ODBCCursor :: operator = ( const ODBCCursor &  that )
{
   memcpy ( offset, that.offset, sizeof ( HSTMT ) );
   
   valid    = that.valid;
   
   return ( *this );
}



HSTMT *  ODBCTable :: ODBCCursor :: Statement ()
{
   return ( &offset );
}
#endif


////////////////////////////////////////////////////////////////////////////////
// ODBCTable :: ODBCRecord                                                    //
////////////////////////////////////////////////////////////////////////////////



void  ODBCTable :: ODBCRecord :: _Index ( int  _index )
{
   super :: _Index ( _index );
}



const size_t  ODBCTable :: ODBCRecord :: __NumberOfRecords () const
{
   return ( dbtable.Count () );
}



const size_t  ODBCTable :: ODBCRecord :: __NumberOfKeys ( int ) const
{
   return ( 0 );
}



ODBCTable :: ODBCRecord :: Result  ODBCTable :: ODBCRecord :: __GetFst ( int )
{
   if ( dbtable.GetFirst () )
   {
      row   = 1;
      
      return ( Result_OK );
   }
   
   return ( Result_EOT );
}



ODBCTable :: ODBCRecord :: Result  ODBCTable :: ODBCRecord :: __GetNxt ()
{
   if ( dbtable.GetNext () )
   {
      ++row;
      
      return ( Result_OK );
   }
   
   return ( Result_EOT );
}



long  ODBCTable :: ODBCRecord :: __GetNxtEx ( long  count )
{
   if ( ( row + 1 ) <= dbtable.Count () )
   {
   long     c  = 0;
   
   while ( ( c < count ) && ( dbtable.GetNext () ) )
      {
         ++row;
      ++c;
      }
      
   return ( c );
   }
   
   return (  0 );
}



ODBCTable :: ODBCRecord :: Result  ODBCTable :: ODBCRecord :: __GetPrv ()
{
   if ( dbtable.GetPrev () )
   {
      --row;
      
      return ( Result_OK );
   }
   
   return ( Result_EOT );
}



long  ODBCTable :: ODBCRecord :: __GetPrvEx ( long  count )
{
   if ( ( row - 1 ) >= 1 )
   {
   long     c  = 0;
   
   while ( ( c < count ) && ( dbtable.GetPrev () ) )
      {
         --row;
      ++c;
      }
      
   return ( c );
   }
   
   return (  0 );
}



ODBCTable :: ODBCRecord :: Result  ODBCTable :: ODBCRecord :: __GetLst ( int )
{
   if ( dbtable.GetLast () )
   {
      row   = dbtable.Count ();
      
      return ( Result_OK );
   }
   
   return ( Result_EOT );
}



ODBCTable :: ODBCRecord :: Result  ODBCTable :: ODBCRecord :: __Get ( const Find & )
{
   return ( Result_KEY_VALUE_NOT_FOUND );
}



ODBCTable :: Cursor *  ODBCTable :: ODBCRecord :: __CursorCreate ()
{
// return ( new  ODBCCursor ( dbtable.GetCursor () ) );
   DefaultCursor *   dc = new  DefaultCursor ( row );
   
   if ( row <= 0 )
      dc -> Invalidate ();
      
   return ( dc );
}



const ODBCTable :: Cursor *  ODBCTable :: ODBCRecord :: __CursorCurrent ()
{
   cursor.Offset ( row );
   
   return ( &cursor );
}



void  ODBCTable :: ODBCRecord :: __CursorInvalidate ()
{
   cursor.Invalidate ();
}



ODBCTable :: ODBCRecord :: Result  ODBCTable :: ODBCRecord :: __CursorSet ( const Cursor *  _cursor )
{
   const DefaultCursor *   dc = static_cast < const DefaultCursor * > ( _cursor );
   
   if ( ( dc          != 0 ) &&
        ( dc -> IsValid () )    )
   {
      cursor   = *dc;
      
      dbtable.SetPos    ( cursor.Offset () );
      
      return ( Result_OK );
   }
   
   return ( Result_EOT );
}



ODBCTable :: ODBCRecord :: Result  ODBCTable :: ODBCRecord :: __ScrollGetByPos ( long  pos )
{
   return ( Result_EOT );
}



long  ODBCTable :: ODBCRecord :: __ScrollPosByCursor  ( const Cursor *  cursor )
{
   return ( static_cast < const DefaultCursor * > ( cursor ) -> Offset () );
}



ODBCTable :: ODBCRecord :: ODBCRecord ( ODBCTable *  _table )
   : Record ( _table ),
   dbtable  ( _table -> dbtable )
{
   row      = -1;
   cursor   .Offset     ( row );
   cursor   .Invalidate ();
}


#if 0   
const wxChar *  ODBCTable :: ODBCRecord :: FieldString ( size_t  c )
{
// table -> GetDbTable () -> GetColumn ( c ).GetValue ();
}
#endif


////////////////////////////////////////////////////////////////////////////////
// ODBCTable                                                                  //
////////////////////////////////////////////////////////////////////////////////



const wxChar *  ODBCTable :: Name () const
{
   return ( dbtable.GetTableName () );
}



const wxChar *  ODBCTable :: Desc () const
{
   return ( dbtable.GetTableName () );
}



size_t  ODBCTable :: NumberOfColumns ()
{
   return ( dbtable.GetNumberOfColumns () );
}



ODBCTable :: Column *  ODBCTable :: At ( size_t )
{
   return (  0 );
}



const wxChar *  ODBCTable :: FieldName ( size_t  i )
{
// return ( dbtable.GetColumn ( i ).GetName () );
   return ( coldefs [ i ].ColName );
}



const wxChar *  ODBCTable :: FieldFullName ( size_t  i )
{
   static wxString   str;

// str.Printf ( "%s.%s", Name (), dbtable.GetColumn ( i ).GetName () );
   str.Printf ( "%s.%s", Name (), coldefs [ i ].ColName );
      
   return ( str );
}



const wxChar *  ODBCTable :: HeaderString ( size_t  i )
{
// return ( dbtable.GetColumn ( i ).GetName () );
   return ( coldefs [ i ].ColName );
}



wxUint32  ODBCTable :: HeaderDrawTextFormat ( size_t )
{
   return ( DT_LEFT );
}



ODBCTable :: Capability  ODBCTable :: GetCapability () const
{
   return ( static_cast < Capability > ( Capability_PERCENTAGE | Capability_HSCROLLTRACK | Capability_VSCROLLTRACK ) );
}



const wxChar *  ODBCTable :: FieldString ( size_t  i )
{
   static wxString   str;
   
// return ( record -> FieldString ( i ) );
   if ( coldefs [ i ].PtrDataObj != 0 )
   {
      wxVariant   v  = dbtable.GetColumn ( i );
      
      if ( v.IsNull () )
         str   = wxEmptyString;
      else
         str   = v.GetString ();
   }
   else
      str.Empty ();
            
   return ( str );
}



wxUint32  ODBCTable :: FieldDrawTextFormat ( size_t )
{
   return ( DT_LEFT );
}
     


ODBCTable :: ODBCTable ( wxDb *  _db, const wxString &  _name, const wxUint16  _cols, const wxString &  _directory )
   : wxTable ( record = new  ODBCRecord ( this ) ),
   dbtable  ( _db, _name, _cols, wxEmptyString, ! wxDB_QUERY_ONLY, _directory )
{
// dbtable  .SetWhereClause   ( wxEmptyString );
// dbtable  .SetOrderByClause ( wxEmptyString );
// dbtable  .SetFromClause    ( wxEmptyString );
// dbtable  .Query            ();
   wxUint16    cols  = _cols;

   dbtable  .SetColDefs       ( _db -> GetColumns ( _name, &cols ), cols );
   dbtable  .Open             ( false, false );
   dbtable  .QueryBySqlStmt   ( "SELECT * from " + _name );
   
   
   
   HSTMT ** t  = new ( HSTMT * [ dbtable.Count () ] );
   size_t   i  = 0;
   
   coldefs  = dbtable.GetColDefs ();
   
   wxLogDebug ( "==================" );
   bool  stat  = dbtable.GetFirst ();
   while ( stat )
   {
      t  [ i ] = dbtable.GetNewCursor ();
      *t [ i ] = dbtable.GetCursor (); 
      ++i;
      
      wxLogDebug ( "%3d ============", i - 1 );
      for ( size_t  c = 0 ; c < cols ; ++c )
         if ( coldefs [ c ].PtrDataObj != 0 )
            wxLogDebug ( "%d %s", c, dbtable.GetColumn ( c ).GetString ().c_str () );
         else
            wxLogDebug ( "%d <NULL>", c );
         
      stat  = dbtable.GetNext ();
   }
#if 0
   dbtable.GetFirst ();
   
   dbtable.SetPos ( 2 );
   for ( int  i = 1 ; i <= dbtable.Count () ; ++i )
      dbtable.SetPos ( i );
#endif
   wxLogDebug ( "=== Random Lookup ===" );   
   
   for ( size_t  x = 0 ; x < 4 ; ++x )
   {
      i  = rand () % dbtable.Count ();
      
      dbtable.SetCursor       ( t [ i ] );
      if ( dbtable.GetNext () )
//    if ( dbtable.UpdateColumns ( t [ i ] ) )
      {
         wxLogDebug ( "%3d ============", i );
         for ( size_t  c = 0 ; c < cols ; ++c )
            if ( coldefs [ c ].PtrDataObj != 0 )
               wxLogDebug ( "%d %s", c, dbtable.GetColumn ( c ).GetString ().c_str () );
            else
               wxLogDebug ( "%d <NULL>", c );
      }
   }
   
   wxLogDebug ( "%d", dbtable.Count () );
}
