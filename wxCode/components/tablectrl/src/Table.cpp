////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        Table.cpp                                                     //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     1998                                                          //
// Copyright:   (c) 1998-2008 Jan Knepper                                     //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#include <wx/wxprec.h>

#include "wx/Table.hpp"



#if defined(_MFC_VER) && defined(_DEBUG)
#define new             DEBUG_NEW
#undef THIS_FILE
static char             THIS_FILE   [] = __FILE__;
#endif



////////////////////////////////////////////////////////////////////////////////
// wxTable :: Column                                                          //
////////////////////////////////////////////////////////////////////////////////



wxTable :: Column :: Column ()
{
   type     = TFT_TEXT;

   // Initialize 'rect' with all negative value's so TableControl will
   // reset them.

//?rect     = iRect ( -1, -1, -1, -1 );

   header   = DT_LEFT;
   field    = DT_LEFT;

   tff      = TFF_SHOW | TFF_SHOW_TOOLTIP | TFF_SIZE;
}



wxTable :: Column :: Column ( const wxString &  _name, const wxString &  _desc, wxUint32  _tff )
{
   name     = _name;
   desc     = _desc;
   type     = TFT_TEXT;

   // Initialize 'rect' with all negative value's so TableControl will
   // reset them.

   header   = DT_LEFT;
   field    = DT_LEFT;
   
   tff      = _tff;                          // TFF_SHOW
}



wxTable :: Column :: ~Column ()
{
}



wxTable :: Column &  wxTable :: Column :: Name ( const wxString &  _name )
{
   name  = _name;
   
   return ( *this );
}



wxTable :: Column &  wxTable :: Column :: Desc ( const wxString &  _desc )
{
   desc  = _desc;
   
   return ( *this );
}



const bool  wxTable :: Column :: Check () const
{
   return ( tff & TFF_SHOW );
}



void  wxTable :: Column :: Check ( bool  _check )
{
   if ( _check )
      tff  |=  TFF_SHOW;
   else
      tff  &= ~TFF_SHOW;
}



///////////////////////////////////////////////////////////////////////////////
// wxTable :: ColumnVector                                                   //
///////////////////////////////////////////////////////////////////////////////



const wxTable :: ColumnVector :: size_type   wxTable :: ColumnVector :: npos  = ~0UL;



wxTable :: ColumnVector :: ColumnVector ( const wxString &  _name, const wxString &  _desc )
   : std :: vector < Column * > ()
{
   name  = _name;
   desc  = _desc;
}



wxTable :: ColumnVector :: ~ColumnVector ()
{
}



const size_t  wxTable :: ColumnVector :: IndexOf ( const Column *  column ) const
{
   const size_t   SIZE  = size ();
   
   for ( size_t  i = 0 ; i < SIZE ; ++i )
      if ( at ( i ) == column )
         return ( i );
         
   return ( npos );
}



////////////////////////////////////////////////////////////////////////////////
// wxTable :: Cursor                                                          //
////////////////////////////////////////////////////////////////////////////////



wxTable :: Cursor &  wxTable :: Cursor :: Assign ( const Cursor &  that )
{
   return ( *this );
}



wxTable :: Cursor :: Cursor ()
{
}



wxTable :: Cursor :: Cursor ( const Cursor & )
{
}



wxTable :: Cursor :: ~Cursor ()
{
}



////////////////////////////////////////////////////////////////////////////////
// wxTable :: CursorVector                                                    //
////////////////////////////////////////////////////////////////////////////////



const wxTable :: CursorVector :: size_type   wxTable :: CursorVector :: npos  = ~0U;



wxTable :: CursorVector :: CursorVector ()
   : std :: vector < Cursor * > ()
{
   active = 0;
}



wxTable :: CursorVector :: ~CursorVector ()
{
   Clear ();
}



void  wxTable :: CursorVector :: Allocate ( size_type  _size, const Cursor *  cursor )
{
   const size_type   SIZE  = size ();
   const size_type   size  = _size + 1;

   if ( SIZE < size )
   {
      reserve ( size );

      for ( size_type  i = SIZE ; i < size ; ++i )
      {
         Cursor *    tmp   = cursor -> Clone ();
         
         tmp   -> Invalidate ();
         
         push_back ( tmp );
      }
   }

   active = size;
}



void  wxTable :: CursorVector :: Clear ()
{
   const size_t   s = size ();

   for ( size_t  i = 0 ; i < s ; ++i )
      delete  at ( i );
      
   super :: clear ();
   
   active = 0;
}



const wxTable :: Cursor *  wxTable :: CursorVector :: operator [] ( size_type  index ) const
{
   if ( ( index >= size () ) || ( index >= active ) )
      return (  0 );

   return ( at ( index ) );
}



wxTable :: Cursor *  wxTable :: CursorVector :: operator [] ( size_type  index )
{
   if ( ( index >= size () ) || ( index >= active ) )
      return (  0 );

   return ( at ( index ) );
}



wxTable :: CursorVector :: size_type  wxTable :: CursorVector :: IndexOf ( const wxTable :: Cursor &  cursor )
{
   iterator   i     = begin ();
   int        index = 0;

   while ( i != end () )
   {
      if ( *( *i ) == cursor )
         return ( index );

      ++i;
      ++index;
   }

   return ( npos );
}



bool  wxTable :: CursorVector :: Find ( const wxTable :: Cursor &  cursor )
{
   return ( ( IndexOf ( cursor ) != npos ) );
}



const wxTable :: Cursor *  wxTable :: CursorVector :: Top () const
{
   if ( size () > 0 )
      return ( front () );

   return (  0 );
}



void  wxTable :: CursorVector :: Top ( const Cursor  &cursor )
{
   if ( size () > 0 )
      *front () = cursor;
}



void  wxTable :: CursorVector :: Invalidate ()
{
   iterator   i     = begin ();

   while ( i != end () )
   {
      ( *i ) -> Invalidate ();

      ++i;
   }
}



////////////////////////////////////////////////////////////////////////////////
// wxTable :: Set                                                             //
////////////////////////////////////////////////////////////////////////////////



wxTable :: Set :: Set ()
{
}



wxTable :: Set :: ~Set ()
{
}



////////////////////////////////////////////////////////////////////////////////
// wxTable :: SetInterface                                                    //
////////////////////////////////////////////////////////////////////////////////



wxTable :: SetAdapter :: SetAdapter ()
{
   table = 0;
   set   = 0;
}



void  wxTable :: SetAdapter :: Table ( wxTable *  _table )
{
   table = _table;
}



void  wxTable :: SetAdapter :: Use ( Set *  _set )
{
   set   = _set;
}



const bool  wxTable :: SetAdapter :: Find ( const Cursor *  cursor ) const
{
   if ( set == 0 )
      return ( false );

   return ( set -> Find ( cursor ) );
}



bool  wxTable :: SetAdapter :: Insert ( const Cursor *  cursor )
{
   if ( set == 0 )
      return ( false );

   return ( set -> Insert ( cursor ) );
}



bool  wxTable :: SetAdapter :: Delete ( const Cursor *  cursor )
{
   if ( set == 0 )
      return ( false );

   return ( set -> Delete ( cursor ) );
}



bool  wxTable :: SetAdapter :: Toggle ( const Cursor *  cursor )
{
   if ( set == 0 )
      return ( false );

   return ( set -> Toggle ( cursor ) );
}



bool  wxTable :: SetAdapter :: Select ( const Cursor *  cursor, const bool &  select )
{
   if ( set == 0 )
      return ( false );

   if ( select )
      return ( set -> Insert ( cursor ) );
   else
      return ( set -> Delete ( cursor ) );
}



bool  wxTable :: SetAdapter :: All ( const bool &  all )
{
   if ( set == 0 )
      return ( false );

   if ( all )
      return ( set -> All  () );
   else
      return ( set -> None () );
}



bool  wxTable :: SetAdapter :: Invert ()
{
   if ( set == 0 )
      return ( false );

   return ( set -> Invert () );
}



const bool  wxTable :: SetAdapter :: IsInverted () const
{
   if ( set == 0 )
      return ( false );

   return ( set -> Inverted () );
}



const size_t  wxTable :: SetAdapter :: Size () const
{
   if ( set == 0 )
      return (  0 );

   if ( set -> Inverted () )
      return ( table -> GetRecord () -> NumberOfRecords () - set -> Size () );

   return ( set -> Size () );
}



const wxTable :: Cursor *  wxTable :: SetAdapter :: FindFirst () const
{
   return ( FindNext ( 0 ) );
}



const wxTable :: Cursor *  wxTable :: SetAdapter :: FindNext ( const wxTable :: Cursor *  cursor ) const
{
   if ( set == 0 )
      return (  0 );

   if ( cursor == 0 )
      return ( set -> First () );
   else
      return ( set -> Next ( cursor ) );
}



////////////////////////////////////////////////////////////////////////////////
// wxTable :: Record                                                          //
////////////////////////////////////////////////////////////////////////////////



const size_t  wxTable :: Record :: _NumberOfRecords () const
{
#ifdef __JAK_TABLE_RELATION__

   if ( relationkey == 0 )
      return ( __NumberOfRecords () );
   else
   {
      return ( __NumberOfRecords () );
   }

#else

   return ( __NumberOfRecords () );

#endif
}



const size_t  wxTable :: Record :: _NumberOfKeys ( int  index ) const
{
#ifdef __JAK_TABLE_RELATION__

   if ( relationkey == 0 )
      return ( __NumberOfKeys ( index ) );
   else
   {
      relationkey -> Copy ( first -> Pointer (), wxTableRelation :: Data_FST );
      relationkey -> Copy ( last  -> Pointer (), wxTableRelation :: Data_LST );

//    return ( __NumberOfKeys ( index ) );
      return ( __KeySpan ( first, last, index ) );
   }

#else

   return ( __NumberOfKeys ( index ) );

#endif
}



wxTable :: Record :: Result  wxTable :: Record :: _GetFst ( int  _index )
{
   if ( ( _index != -1 ) && ( _index != index ) )
      SetIndex ( _index );

#ifdef __JAK_TABLE_RELATION__

   if ( relationkey == 0 )
      return ( __GetFst () );
   else
   {
      Result      result;

      relationkey -> Copy ( Pointer (), wxTableRelation :: Data_FST );

      if ( ( result = __GetGe () ) != Result_OK )
         return ( result );

      if ( relationkey -> Compare ( Pointer () ) == 0 )
         return ( result );

      return ( Result_EOT );
   }

#else

   return ( __GetFst () );

#endif
}



wxTable :: Record :: Result  wxTable :: Record :: _GetNxt ()
{
#ifdef __JAK_TABLE_RELATION__

   if ( relationkey == 0 )
      return ( __GetNxt () );
   else
   {
      Result      result;

      if ( ( result = __GetNxt () ) != Result_OK )
         return ( result );

      if ( relationkey -> Compare ( Pointer () ) == 0 )
         return ( result );

      return ( Result_EOT );
   }

#else

   return ( __GetNxt () );

#endif
}



long  wxTable :: Record :: _GetNxtEx ( long  _count )
{
#ifdef __JAK_TABLE_RELATION__

   if ( relationkey == 0 )
      return ( __GetNxtEx ( _count ) );
   else
   {
      Cursor     *save     = CursorCreate ();
      long        count    = 0;

      while ( (     count                             <  _count    ) &&
              ( __GetNxt ()                           == Result_OK ) &&
              ( relationkey -> Compare ( Pointer () ) == 0         )    )
      {
         count++;

         *save = *CursorCurrent ();
      }

      if ( count < _count )
         CursorSet   ( save, _record );

      delete  save;

      return ( count );
   }

#else

   return ( __GetNxtEx ( _count ) );

#endif
}



wxTable :: Record :: Result  wxTable :: Record :: _GetPrv ()
{
#ifdef __JAK_TABLE_RELATION__

   if ( relationkey == 0 )
      return ( __GetPrv () );
   else
   {
      Result      result;

      if ( ( result = __GetPrv () ) != Result_OK )
         return ( result );

      if ( relationkey -> Compare ( Pointer () ) == 0 )
         return ( result );

      return ( Result_EOT );
   }

#else

   return ( __GetPrv () );

#endif
}



long  wxTable :: Record :: _GetPrvEx ( long  _count )
{
#ifdef __JAK_TABLE_RELATION__

   if ( relationkey == 0 )
      return ( __GetPrvEx ( _count ) );
   else
   {
      Cursor     *save     = CursorCreate ();
      long        count    = 0;

      while ( (     count                             <  _count    ) &&
              ( __GetPrv ()                           == Result_OK ) &&
              ( relationkey -> Compare ( Pointer () ) == 0         )    )
      {
         count++;

         *save = *CursorCurrent ();
      }

      if ( count < _count )
         CursorSet   ( save );

      delete  save;

      return ( count );
   }

#else

   return ( __GetPrvEx ( _count ) );

#endif
}



wxTable :: Record :: Result  wxTable :: Record :: _GetLst ( int  _index )
{
   if ( ( _index != -1 ) && ( _index != index ) )
      SetIndex ( _index );

#ifdef __JAK_TABLE_RELATION__

   if ( relationkey == 0 )
      return ( __GetLst () );
   else
   {
      Result      result;

      relationkey -> Copy ( Pointer (), wxTableRelation :: Data_LST );

      if ( ( result = __GetLe () ) != Result_OK )
         return ( result );

      if ( relationkey -> Compare ( Pointer () ) == 0 )
         return ( result );

      return ( Result_EOT );
   }

#else

   return ( __GetLst () );

#endif
}



wxTable :: Record :: Result  wxTable :: Record :: _Get ( const Find &  _find, int  _index )
{
   if ( ( _index != -1 ) && ( _index != index ) )
      SetIndex ( _index );

#ifdef __JAK_TABLE_RELATION__

   if ( relationkey == 0 )
      return ( __Get ( _find ) );
   else
   {
      Result      result;

      relationkey -> Copy ( Pointer (), wxTableRelation :: Data_ONLY );

      if ( ( result = __Get ( _find ) ) != Result_OK )
         return ( result );

      if ( relationkey -> Compare ( Pointer () ) == 0 )
         return ( result );

      return ( Result_EOT );
   }

#else

   return ( __Get ( _find ) );

#endif
}



wxTable :: Record :: Result  wxTable :: Record :: _Insert ()
{
   return ( __Insert () );
}



wxTable :: Record :: Result  wxTable :: Record :: _Update ()
{
   return ( __Update () );
}



wxTable :: Record :: Result  wxTable :: Record :: _Delete ()
{
   return ( __Delete () );
}



wxTable :: Record :: Result  wxTable :: Record :: _Lock ()
{
   return ( __Lock () );
}



wxTable :: Record :: Result  wxTable :: Record :: _Lock ( const Cursor  *cursor )
{
   return ( __Lock ( cursor ) );
}



wxTable :: Record :: Result  wxTable :: Record :: _Unlock ()
{
   return ( __Unlock () );
}



wxTable :: Record :: Result  wxTable :: Record :: _Unlock ( const Cursor  *cursor )
{
   return ( __Unlock ( cursor ) );
}



wxTable :: Cursor *  wxTable :: Record :: _CursorCreate ()
{
   return ( __CursorCreate () );
}



const wxTable :: Cursor *  wxTable :: Record :: _CursorCurrent ()
{
#ifdef __JAK_TABLE_RELATION__

   if ( relationkey == 0 )
      return ( __CursorCurrent () );
   else
   {
      const Cursor     *tmp   = __CursorCurrent ();

      if ( ! tmp -> IsValid () )
         return ( tmp );

//    if ( ( __CursorSet ( tmp )               == Result_OK ) &&
//         ( relationkey -> Compare ( record ) == 0         )    )
//    if ( relationkey -> Compare ( record ) == 0 )
//       return ( tmp );
//
//    __CursorInvalidate ();

      return ( tmp );
   }

#else

   return ( __CursorCurrent () );

#endif
}



wxTable :: Record :: Result  wxTable :: Record :: _CursorSet ( const Cursor *  cursor )
{
#ifdef __JAK_TABLE_RELATION__

   if ( relationkey == 0 )
      return ( __CursorSet ( cursor, record ) );
   else
   {
      Result      result;

      if ( ( result = __CursorSet ( cursor, record ) ) != Result_OK )
         return ( result );

      if ( relationkey -> Compare ( record -> Pointer () ) == 0 )
         return ( result );

      return ( Result_EOT );
   }

#else

   return ( __CursorSet ( cursor ) );

#endif
}



wxTable :: Record :: Result  wxTable :: Record :: _ScrollGetByPos ( long  offset )
{
#ifdef __JAK_TABLE_RELATION__

   if ( relationkey == 0 )
      return ( __ScrollGetByPos ( offset, record ) );
   else
   {
      Result      result;

      relationkey -> Copy  ( first -> Pointer (), wxTableRelation :: Data_FST );
      if ( ( result = __GetGe ( first ) ) != Result_OK )
         return ( result );

      return ( __ScrollGetByPos ( __ScrollPosByCursor ( CursorCurrent () ) + offset, record ) );
   }

#else

   return ( __ScrollGetByPos ( offset ) );

#endif
}



long  wxTable :: Record :: _ScrollPosByCursor ( const Cursor  *cursor )
{
#ifdef __JAK_TABLE_RELATION__

   if ( relationkey == 0 )
      return ( __ScrollPosByCursor ( cursor ) );
   else
   {
      Result      result;
      Cursor     *save     = CursorCreate ();
      long        pos      = __ScrollPosByCursor   ( cursor );

      relationkey -> Copy  ( first -> Pointer (), wxTableRelation :: Data_FST );
      if ( ( result = __GetGe ( first ) ) == Result_OK )
         pos  -= __ScrollPosByCursor ( CursorCurrent () );

      CursorSet   ( save, record );

      delete  save;

      return ( pos );
   }

#else

   return ( __ScrollPosByCursor ( cursor ) );

#endif
}



long  wxTable :: Record :: _ScrollPosMin ()
{
#ifdef __JAK_TABLE_RELATION__

   if ( relationkey == 0 )
      return ( __ScrollPosMin () );
   else
   {
      return ( __ScrollPosMin () );
   }

#else

   return ( __ScrollPosMin () );

#endif
}



long  wxTable :: Record :: _ScrollPosMax ()
{
#ifdef __JAK_TABLE_RELATION__

   if ( relationkey == 0 )
      return ( __ScrollPosMax () );
   else
   {
      return ( __ScrollPosMax () );
   }

#else

   return ( __ScrollPosMax () );

#endif
}



void  wxTable :: Record :: _Index ( int  _index )
{
   index = _index;
}



const wxTable :: Record &  wxTable :: _Record () const
{
   return ( *record );
}



void  wxTable :: _Record ( Record *  _record )
{
// record.Set ( pointer, size );
// record.Init ( iMemory :: Mode_REFERENCE, sizeof ( byte ), size, pointer );
   record = _record;
}



wxTable :: Record :: Result  wxTable :: Record :: __Insert ()
{
   return ( Result_INVALID_OPERATION );
}



wxTable :: Record :: Result  wxTable :: Record :: __Update ()
{
   return ( Result_INVALID_OPERATION );
}



wxTable :: Record :: Result  wxTable :: Record :: __Delete ()
{
   return ( Result_INVALID_OPERATION );
}


void  wxTable :: Record :: __Synchronize ()
{
}



wxTable :: Record :: Record ( wxTable *  _table )
{
   table          = _table;
   index          = 0;
   
   sequence       = Sequence_ASCENDING;
}


   
wxTable :: Record :: ~Record ()
{
}



const bool  wxTable :: Record :: CanSort ( size_t ) const
{
   return ( false );
}



const wxTable :: Record :: Sequence  wxTable :: Record :: GetSort ( size_t ) const
{
   return ( Sequence_NONE );
}



void  wxTable :: Record :: SetSort ( size_t, const Sequence & )
{
}



const bool  wxTable :: Record :: HasNoSort () const
{
   return ( false );
}



const wxTable :: Record :: Sequence  wxTable :: Record :: GetNoSort () const
{
   return ( ( index == -1 ) ? sequence : Sequence_NONE );
}



void  wxTable :: Record :: SetNoSort ( const Sequence &  _sequence )
{
   if ( HasNoSort () )
      index    = -1;
   else
      index    =  0;

   sequence = _sequence;
}



const int  wxTable :: Record :: GetIndex () const
{
   return ( index );
}



void  wxTable :: Record :: SetIndex ( int  _index )
{
   // Handle the special case which is when this table is acting as a sub-
   // table of an other table. Sorting (changing index) is only allowed to
   // those indexes defined in the relation!

#ifdef __JAK_TABLE_RELATION__

   if ( relationkey != 0 )
   {
      if ( _index != index )
      {
         wxTableRelation :: Key   *tmp  = relationkey  -> Key4Index   ( _index );

         if ( tmp == 0 )
            return;

         relationkey = tmp;
      }
   }

#endif

   index  =   _index;

   if ( ( index < 0 ) && ! HasNoSort () )
      index = 0;

   _Index ( index );                      // Call virtual member.
}



const size_t  wxTable :: Record :: NumberOfRecords () const
{
   return ( _NumberOfRecords () );
}



const size_t  wxTable :: Record :: NumberOfKeys ( int  _index ) const
{
   return ( _NumberOfKeys ( _index ) );
}



wxTable :: Record :: Result  wxTable :: Record :: GetFst ( int  _index )
{
   return ( ( sequence == Sequence_ASCENDING ) ? _GetFst ( _index ) : _GetLst ( _index ) );
}



wxTable :: Record :: Result  wxTable :: Record :: GetNxt ()
{
   return ( ( sequence == Sequence_ASCENDING ) ? _GetNxt () : _GetPrv () );
}



long  wxTable :: Record :: GetNxtEx ( long  count )
{
   return ( ( sequence == Sequence_ASCENDING ) ? _GetNxtEx ( count ) : _GetPrvEx ( count ) );
}



wxTable :: Record :: Result  wxTable :: Record :: GetPrv ()
{
   return ( ( sequence == Sequence_ASCENDING ) ? _GetPrv () : _GetNxt () );
}



long  wxTable :: Record :: GetPrvEx ( long  count )
{
   return ( ( sequence == Sequence_ASCENDING ) ? _GetPrvEx ( count ) : _GetNxtEx ( count ) );
}



wxTable :: Record :: Result  wxTable :: Record :: GetLst ( int  _index )
{
   return ( ( sequence == Sequence_ASCENDING ) ? _GetLst ( _index ) : _GetFst ( _index ) );
}



wxTable :: Record :: Result  wxTable :: Record :: Get ( const Find &  _find, int  _index )
{
   return ( _Get ( _find, _index ) );
}



size_t  wxTable :: Record :: NumberOfBLObs () const
{
   return (  0 );
}



wxTable :: Record :: Result  wxTable :: Record :: GetBLOb ( size_t )
{
   return ( Result_OK );
}



wxTable :: Record :: Result  wxTable :: Record :: Insert ()
{
   return ( _Insert () );
}



wxTable :: Record :: Result  wxTable :: Record :: Update ()
{
   return ( _Update () );
}



wxTable :: Record :: Result  wxTable :: Record :: Delete ()
{
   return ( _Delete () );
}



wxTable :: Record :: Result  wxTable :: Record :: Lock ()
{
   return ( _Lock () );
}



wxTable :: Record :: Result  wxTable :: Record :: Lock ( const Cursor *  cursor )
{
   return ( _Lock ( cursor ) );
}



wxTable :: Record :: Result  wxTable :: Record :: Unlock ()
{
   return ( _Unlock () );
}



wxTable :: Record :: Result  wxTable :: Record :: Unlock ( const Cursor *  cursor )
{
   return ( _Unlock ( cursor ) );
}



wxTable :: Cursor *  wxTable :: Record :: CursorCreate ()
{
   return ( _CursorCreate () );
}



const wxTable :: Cursor *  wxTable :: Record :: CursorCurrent ()
{
   return ( _CursorCurrent () );
}



wxTable :: Record :: Result  wxTable :: Record :: CursorSet ( const wxTable :: Cursor *  cursor )
{
   return ( _CursorSet ( cursor ) );
}



void  wxTable :: Record :: CursorInvalidate ()
{
}



wxTable :: Record :: Result  wxTable :: Record :: ScrollGetByPos ( long  pos )
{
   return ( ( sequence == Sequence_ASCENDING ) ? _ScrollGetByPos ( pos ) : _ScrollGetByPos ( _ScrollPosMax () - pos ) );
}



long  wxTable :: Record :: ScrollPosByCursor ( const Cursor *  cursor )
{
   return ( ( sequence == Sequence_ASCENDING ) ? _ScrollPosByCursor ( cursor ) : _ScrollPosMax () - _ScrollPosByCursor ( cursor ) );
}



void  wxTable :: Record :: Synchronize ()
{
   __Synchronize  ();
}



/////////////////////////////////////////////////////////////////////////////////
// wxTable                                                                     //
/////////////////////////////////////////////////////////////////////////////////



wxTable :: wxTable ( Record *  _record )
{
   record         = _record;

   action4table   = Action_NULL;

   select   .Table   ( this );
   check    .Table   ( this );

#ifdef __JAK_TABLE_RELATION__

   relationarray  = 0;
   relationkey    = 0;
   relationlock   = 0;

   first          = ( record != 0 ) ? record -> Create () : 0;
   last           = ( record != 0 ) ? record -> Create () : 0;
#endif
}




wxTable :: ~wxTable ()
{
#ifdef __JAK_TABLE_RELATION__
   delete  last;
   delete  first;

   delete  relationarray;
#endif
}



LPCTSTR  wxTable :: DescOrName () const
{
   if ( Desc () != 0 )
      return ( Desc () );

   return ( Name () );
}



void  wxTable :: HeaderDrawTextFormat ( size_t, wxUint32 )
{
}



wxTable :: Action  wxTable :: Action4Record ()
{
   return ( Action_NULL );
}



wxTable :: Action  wxTable :: Action4Table ()
{
   return ( static_cast < Action > ( action4table ) );
}



void  wxTable :: Action4Table ( const wxTable :: Action &  _action4table )
{
   action4table   = _action4table;
}



void  wxTable :: Action4Table ( wxUint32  _action4table )
{
   action4table   = static_cast < Action > ( _action4table );
}



void  wxTable :: ModifyAction4Table ( const wxTable :: Action &  _remove, const wxTable :: Action &  _add )
{
   action4table  &= ~_remove;
   action4table  |=  _add;
}



void  wxTable :: ModifyAction4Table ( wxUint32  _remove, wxUint32  _add )
{
   action4table  &= ~_remove;
   action4table  |=  _add;
}



wxUint32  wxTable :: FieldFlag ( size_t ) const
{
   return ( TFF_SHOW );
}



wxUint32  wxTable :: FieldType ( size_t ) const
{
   return ( TFT_NULL );
}



void  wxTable :: FieldDrawTextFormat ( size_t, wxUint32 )
{
}



void *  wxTable :: FieldData ( size_t, size_t )
{
   return (  0 );
}



size_t  wxTable :: FieldDataSize ( size_t )
{
   return (  0 );
}


#if 0
void  wxTable :: Synchronize ()
{
   if ( record != 0 )
      record -> Synchronize ();
}
#endif



#ifdef __JAK_TABLE_RELATION__



void  wxTable :: RelationInsert ( wxTableRelation  *_relation )
{
   if ( relationarray == 0 )
      relationarray  = new  wxTableRelationArray ();

   relationarray -> Add ( _relation );
}



void  wxTable :: RelationDelete ( wxTableRelation  *_relation )
{
   if ( relationarray == 0 )
      return;

   for ( int  i = 0 ; i < relationarray -> Used () ; i++ )
      if ( relationarray -> At ( i ) == _relation )
         relationarray -> Delete ( i );
}


/*
wxTableRelation  *wxTable :: RelationActive ( wxTableRelation  *_relation, int  key )
{
   wxTableRelation   *tmp   = relationkey;

   if ( ( relationkey = _relation -> At ( key ) ) != 0 )
      Index ( relationkey -> Index () );

   return ( tmp );
}
*/



bool  wxTable :: RelationKey ( wxTableRelationKey  *_relationkey )
{
   if ( relationlock > 0 )
      return ( false );

   L_TRACE3 ( 0, "%s: %p, %p", Name (), relationkey, _relationkey );
// relation = _relation;
// if ( relationkey != _relationkey )
   {
      if ( ( relationkey = _relationkey ) != 0 )
      {
         L_TRACE3 ( 0, "%s %d %s", relationkey -> Relation () -> Master () -> Name (), relationkey -> Index (), relationkey -> Relation () -> Slave () -> Name () );
         Index  ( relationkey -> Index () );

         if ( relationkey -> Compare ( record -> Pointer () ) != 0 )
            GetFst ( record );
      }

      L_TRACE0 ( 0, "true" );

      return ( true );
   }

// L_TRACE0 ( 0, "false" );

// return ( false );
}



int  wxTable :: RelationKeyCompare ( iRecord  *_record )
{
   if ( relationkey == 0 )
      return (  0 );

   if ( _record == 0 )
      _record  = record;

   return ( relationkey -> Compare ( _record -> Pointer () ) );
}



void  wxTable :: RelationSync ()
{
   if ( relationarray == 0 )
      return;

   const int      USED  = relationarray -> Used ();

   if ( USED <= 0 )
      return;

   for ( int  i = 0 ; i < USED ; i++ )
   {
      relationarray -> At ( i ) -> Synchronize  ();
      relationarray -> At ( i ) -> CopyRecord   ( Record () );
   }
}



void  wxTable :: RelationDeactivate ()
{
   if ( relationarray == 0 )
      return;

   const int      USED  = relationarray -> Used ();

   if ( USED <= 0 )
      return;

   for ( int  i = 0 ; i < USED ; i++ )
      relationarray -> At ( i ) -> Deactivate ();
}



long  wxTable :: RelationLock ()
{
   if ( relationkey != 0 )
      relationkey -> Relation () -> Master () -> RelationLock ();

   return ( ++relationlock );
}



long  wxTable :: RelationUnlock ()
{
   if ( relationkey != 0 )
      relationkey -> Relation () -> Master () -> RelationUnlock ();

   return ( ( relationlock > 0 ) ? --relationlock : relationlock );
}



bool  wxTable :: RelationActive () const
{
   if ( relationkey == 0 )
      return ( false );

   return ( relationkey -> Relation () -> Active () );
}



#endif                                    // __JAK_TABLE_RELATION__



////////////////////////////////////////////////////////////////////////////////
// wxTablePosition.                                                           //
////////////////////////////////////////////////////////////////////////////////



void  wxTablePosition :: Init ( const Flag &  _flag, wxTable *  _table, wxTable :: Record *  _record )
{
   flag  = _flag;
   table = _table;

   if ( _record != 0 )
      record = _record;
   else
      if ( _table != 0 )
         record = _table -> GetRecord ();
      else
         record = 0;

   cursor   = 0;

// if ( _flag == Flag_AUTO )              // JAK 20011221
//    Backup ();                          // JAK 20011221

   Backup   ();
}



wxTablePosition :: wxTablePosition ( wxTable *  _table, wxTable :: Record *  _record )
{
   Init ( Flag_AUTO, _table, _record );
}



wxTablePosition :: wxTablePosition ( const Flag &  _flag, wxTable *  _table, wxTable :: Record *  _record )
{
   Init ( _flag, _table, _record );
}



wxTablePosition :: ~wxTablePosition ()
{
   if ( flag == Flag_AUTO )               // Auto restore of position requested.
      Restore ();

   delete  cursor;
}



void  wxTablePosition :: Backup ()
{
   if ( table != 0 )
   {
      if ( cursor == 0 )
         cursor   = record -> CursorCreate ();     // Create a cursor with the current table position.
      else
         *cursor  = *record -> CursorCurrent ();   // or just assign the current cursor.

      index    = record -> GetIndex ();            // Remember the table index.
   }
}



void  wxTablePosition :: Restore ()
{
   if ( ( table != 0 ) && ( record != 0 ) )
   {
      record -> SetIndex   ( index );           // Restore the table index.
      record -> CursorSet  ( cursor );          // Restore the table
   }                                            // of original record.
}



void  wxTablePosition :: Manual ()
{
   flag = Flag_MANUAL;
}



void  wxTablePosition :: Auto ()
{
   flag = Flag_AUTO;
}

