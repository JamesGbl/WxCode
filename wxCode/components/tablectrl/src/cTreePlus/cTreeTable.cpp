////////////////////////////////////////////////////////////////////////////////
// Soli Deo Gloria!                                                           //
//                                                                            //
// Name:        cTreeTable.cpp                                                //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007                                                          //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#include <wx/wxprec.h>

#include "wx/cTreePlus/cTreeTable.hpp"



void  cTreeTable :: RecordEx :: _Index ( int  _index )
{
   super :: _Index ( _index );
   
   record.SetDefaultIndex  ( _index );
}



const size_t  cTreeTable :: RecordEx :: __NumberOfRecords () const
{ 
   return ( record.GetRecordCount () );
}



const size_t  cTreeTable :: RecordEx :: __NumberOfKeys ( int )  const
{
   // Record count for now. This should be done via ctdbGetNumberOfIndexEntries!

   return ( record.GetRecordCount () );
}



wxTable :: Record :: Result  cTreeTable :: RecordEx :: __GetFst ( int  index )
{
// record.SetDefaultIndex ( index );
   
   if ( record.First () )
      return ( Result_OK );

   return ( Result_EOT );
}



wxTable :: Record :: Result  cTreeTable :: RecordEx :: __GetNxt ()
{
   if ( record.Next  () )
      return ( Result_OK );

   return ( Result_EOT );
}



long  cTreeTable :: RecordEx :: __GetNxtEx ( long  count )
{
   long     result   = 0;

   while ( ( result < count ) &&
           ( record.Next () )    )
      ++result;

   return ( result );
}



wxTable :: Record :: Result  cTreeTable :: RecordEx :: __GetPrv ()
{
   if ( record.Prev  () )
      return ( Result_OK );

   return ( Result_EOT );
}



long  cTreeTable :: RecordEx :: __GetPrvEx ( long  count )
{
   long     result   = 0;

   while ( ( result < count ) &&
           ( record.Prev () )    )
      ++result;
   
   return ( result );
}



wxTable :: Record :: Result  cTreeTable :: RecordEx :: __GetLst ( int  index )
{
// record.SetDefaultIndex ( index );
   
   if ( record.Last  () )
      return ( Result_OK );

   return ( Result_EOT );
}



wxTable :: Record :: Result  cTreeTable :: RecordEx :: __Get ( const Find &  find )
{
   CTBOOL   stat  = NO;
   
   switch ( find )
   {
      case Find_EQ   :
         stat  = record.Find ( CTFIND_EQ );
         
         break;
         
      case Find_LE   :
         stat  = record.Find ( CTFIND_LE );
         
         break;
         
      case Find_LT   :
         stat  = record.Find ( CTFIND_LT );
         
         break;
         
      case Find_GE   :
         stat  = record.Find ( CTFIND_GE );
         
         break; 
         
      case Find_GT   :
         stat  = record.Find ( CTFIND_GT );
         
         break;
   }
   
   wxLogDebug ( "%s offset: %ld", __FUNCTION__, record.GetRecordPos () );
   
   return ( ( stat != NO ) ? Result_OK : Result_EOT );
}



wxTable :: Record :: Result  cTreeTable :: RecordEx :: __Insert ()
{
   wxASSERT ( 0 );
   
   return ( Result_EOT );
}



wxTable :: Record :: Result  cTreeTable :: RecordEx :: __Update ()
{
   wxASSERT ( 0 );
   
   return ( Result_EOT );
}



wxTable :: Record :: Result  cTreeTable :: RecordEx :: __Delete ()
{
   wxASSERT ( 0 );
   
   return ( Result_EOT );
}



wxTable :: Record :: Result  cTreeTable :: RecordEx :: __Lock ()                   
{ 
   wxASSERT ( 0 );
   
   return ( Result_LOCK_ERROR ); 
}



wxTable :: Record :: Result  cTreeTable :: RecordEx :: __Lock ( const Cursor * )   
{ 
   wxASSERT ( 0 );
   
   return ( Result_LOCK_ERROR ); 
}



wxTable :: Record :: Result  cTreeTable :: RecordEx :: __Unlock () 
{ 
   wxASSERT ( 0 );
   
   return ( Result_LOCK_ERROR ); 
}



wxTable :: Record :: Result  cTreeTable :: RecordEx :: __Unlock ( const Cursor * )   
{ 
   wxASSERT ( 0 );
   
   return ( Result_LOCK_ERROR ); 
}



wxTable :: Cursor *  cTreeTable :: RecordEx :: __CursorCreate ()
{
   return ( CursorCurrent () -> Clone () );
}



const wxTable :: Cursor *  cTreeTable :: RecordEx :: __CursorCurrent ()
{
   const CTOFFSET    offset   = record.GetRecordPos ();
   
// if ( ( ! record.IsNew () ) && ( offset != 0 ) )
   if ( offset != 0 )                     // 0 means no offset, offset always > 0! FairCom 3.5.12.3
      cursor.Offset ( offset );
   else
      cursor.Invalidate ();

   return ( &cursor );
}



wxTable :: Record :: Result  cTreeTable :: RecordEx :: __CursorSet ( const Cursor *  cursor )
{
   if ( cursor -> IsValid () )
   {
      try
      {
         record.SeekRecord  ( static_cast < const CursorEx * > ( cursor ) -> Offset () );
         
         return ( Result_OK );
      }
      catch ( CTException &  e )
      {
         wxLogDebug  ( "__CursorSet - CTException: %d - %s", e.GetErrorCode (), e.GetErrorMsg () );
      }
   }

   return ( Result_EOT );
}



void  cTreeTable :: RecordEx :: __CursorInvalidate ()
{
   cursor.Invalidate ();
}



wxTable :: Record :: Result  cTreeTable :: RecordEx :: __ScrollGetByPos ( long  thumb )
{
   try
   {
      const CTUINT64    RC = record.GetRecordCount ();
      const CTUINT64    SP = static_cast < CTUINT64 > ( thumb ) * 100LL / RC;
      
      if ( GetSequence () == Sequence_ASCENDING )
         record.AtPercentile ( SP );
      else
         record.AtPercentile ( 100 - SP );

      return ( Result_OK );
   }
   catch ( CTException &  e )
   {
      wxLogDebug  ( "__ScrollGetByPos - CTException: %d - %s", e.GetErrorCode (), e.GetErrorMsg () );
   }

   return ( Result_EOT );
}



long  cTreeTable :: RecordEx :: __ScrollPosByCursor ( const Cursor *  cursor )
{
   try
   {
      const CTOFFSET    offset            = record.GetRecordPos   ();
      const VRLEN       LEN               = 512;
      LONG              span              =   0;
      VRLEN             firstlen          = LEN;
      VRLEN             keylen            = LEN;
      byte              first    [ LEN ];
      byte              key      [ LEN ];

      if ( cursor -> IsValid () )
      {
         // Suspect EstimateSpan is not always working properly.
         // When called on the same key which should return 0 an exception
         // occurs.
         // When called on the first and second key sometimes an exception
         // occurs.

         try
         {
            if ( GetSequence () == Sequence_ASCENDING )
               record.First ();
            else
               record.Last  ();
               
            record.BuildTargetKey   ( CTFIND_EQ, first, &firstlen );
            record.SeekRecord       ( static_cast < const CursorEx * > ( cursor ) -> Offset () );
            record.BuildTargetKey   ( CTFIND_EQ, key  , &keylen   );

            if ( ( keylen == firstlen ) && ( memcmp ( key, first, keylen ) == 0 ) )
               span  = 0;
            else
               span  = record.EstimateSpan ( first, key );

//          wxLogDebug  ( "Span: %ld", span );
         }
         catch ( CTException &  e )
         {
            wxLogDebug ( "EstimateSpan: %d: %s", e.GetErrorCode (), e.GetErrorMsg () );
         }

         record.SeekRecord ( offset );

//       return ( ( span * 100 ) / record.GetRecordCount () );
         return ( span );
      }
   }
   catch ( CTException &  e )
   {
      wxLogDebug  ( "__ScrollPosByCursor - CTException: %d - %s", e.GetErrorCode (), e.GetErrorMsg () );
   }

   return ( -1 );
}



const bool  cTreeTable :: RecordEx :: CanSort ( size_t  index ) const
{
   CTTable &   table = static_cast < cTreeTable * > ( GetTable () ) -> GetTable ();   
   const NINT  I     = table.GetIndexCount ();
   
   for ( NINT  i = 0 ; i < I ; ++i )
   {
      CTIndex     ii = table.GetIndex ( i );
      
      if ( ii.GetSegment ( 0 ).GetField ().GetNumber () == index )
         return ( true );
   }
   
   return ( false );
}



const cTreeTable :: Record :: Sequence  cTreeTable :: RecordEx :: GetSort ( size_t  index ) const
{
   if ( index == -1 )
      return ( Sequence_NONE );
      
   CTTable &   table = static_cast < cTreeTable * > ( GetTable () ) -> GetTable ();   
   
   if ( index >= table.GetFieldCount () )
      return ( Sequence_NONE );
      
   if ( GetIndex () == GetIndexId4 ( index ) )
      return ( GetSequence () );
      
   return ( Sequence_NONE );
}



void  cTreeTable :: RecordEx :: SetSort ( size_t  index, const Sequence &  sequence )
{
   SetIndex    ( GetIndexId4 ( index ) );
   SetSequence ( sequence );
}



const bool  cTreeTable :: RecordEx :: HasNoSort () const
{
   return ( false );
}



const cTreeTable :: RecordEx :: Sequence  cTreeTable :: RecordEx :: GetNoSort () const
{
   return ( Sequence_NONE );
}



void  cTreeTable :: RecordEx :: SetNoSort ( const Sequence & )
{
}



const int  cTreeTable :: RecordEx :: GetIndexId4 ( size_t  index ) const
{
   CTTable &   table = static_cast < cTreeTable * > ( GetTable () ) -> GetTable ();   
   const NINT  I     = table.GetIndexCount ();
   
   for ( NINT  i = 0 ; i < I ; ++i )
   {
      CTIndex     ii = table.GetIndex ( i );
      
      if ( ii.GetSegment ( 0 ).GetField ().GetNumber () == index )
         return ( i );
   }
   
   return ( -1 );
}



#if 0
cTreeTable :: RecordEx :: RecordEx ( cTreeTable *  _table )
   : Record ( _table ),
   record   ( _table -> table )
{
}
#endif


cTreeTable :: RecordEx :: RecordEx ( cTreeTable *  _table, CTRecord &  _record )
   : Record ( _table ),
   record   ( _record )
{
}



////////////////////////////////////////////////////////////////////////////////
// cTreeTable                                                                 //
////////////////////////////////////////////////////////////////////////////////



const wxChar *  cTreeTable :: Name () const
{
   return ( table.GetName ().c_str () );
}



const wxChar *  cTreeTable :: Desc () const
{
   return ( table.GetName ().c_str () );
}



size_t  cTreeTable :: NumberOfColumns ()
{
   return ( table.GetFieldCount () );
}



cTreeTable :: Column *  cTreeTable :: At ( size_t  i )
{
// return ( super :: At ( i ) );
   return (  0 );
}



const wxChar *  cTreeTable :: FieldName ( size_t  i )
{
   static wxString   str;
   
   str   = table.GetField ( i ).GetName ().c_str ();
   
   return ( str );
}



const wxChar *  cTreeTable :: FieldFullName ( size_t  i )
{
   static wxString   str;
   
   str   = table.GetField ( i ).GetName ().c_str ();
   
   return ( str );
}



const wxChar *  cTreeTable :: HeaderString ( size_t  i )
{
   static wxString   str;
   
   str   = table.GetField ( i ).GetName ().c_str ();
   
   return ( str );
}



UINT  cTreeTable :: HeaderDrawTextFormat ( size_t  i )
{
   return ( DrawTextFormat ( i ) );
}



wxTable :: Capability  cTreeTable :: GetCapability () const
{
   return ( static_cast < Capability > ( /*Capability_VSCROLLTRACK |*/ Capability_HSCROLLTRACK  | Capability_PERCENTAGE | Capability_VSCROLLROUGH ) );
}



wxUint32  cTreeTable :: FieldFlag ( size_t  i ) const
{  
   return ( TFF_SHOW | TFF_SIZE );
}



const wxChar *  cTreeTable :: FieldString ( size_t  i )
{
   static wxString   str;
   
   str   = record.record.GetFieldAsString ( i ).c_str ();
   
   return ( str );
}



UINT  cTreeTable :: FieldDrawTextFormat ( size_t  i )
{
   return ( DrawTextFormat ( i ) );
}



UINT  cTreeTable :: DrawTextFormat ( size_t  i ) const
{
   switch ( record.record.GetFieldType ( i ) )
   {
      case CT_CHAR		:
      case CT_CHARU		:
      case CT_INT2		:
      case CT_INT2U		:
      case CT_INT4		:
      case CT_INT4U		:
      case CT_SFLOAT	   :
      case CT_DFLOAT	   :
      case CT_TIMES		:
         return ( DT_RIGHT );
         
      case CT_ARRAY		:
         return ( DT_LEFT );
         
      case CT_FSTRING	:
      case CT_STRING	   :
      case CT_2STRING	:
      case CT_4STRING   :
         return ( DT_LEFT );
   }
   
   return ( DT_LEFT );
}


#if 0
cTreeTable :: cTreeTable ( CTTable &  _table )
   : wxTable ( &record ),
   table    ( _table ),
   record   ( this )
{
}
#endif


cTreeTable :: cTreeTable ( CTTable &  _table, CTRecord &  _record )
   : wxTable ( &record ),
   table    ( _table ),
   record   ( this, _record )
{
}



void  cTreeTable :: Synchronize ()
{
}
