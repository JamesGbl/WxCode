////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        FakeTable.cpp                                                 //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007/10/07                                                    //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#include <wx/wxprec.h>

#include "FakeTable.hpp"



////////////////////////////////////////////////////////////////////////////////
// FakeTable :: FakeRecord                                                    //
////////////////////////////////////////////////////////////////////////////////



void  FakeTable :: FakeRecord :: _Index ( int  _index )
{
   super :: _Index ( _index );
}



const size_t  FakeTable :: FakeRecord :: __NumberOfRecords () const
{
   return ( r );
}



const size_t  FakeTable :: FakeRecord :: __NumberOfKeys ( int ) const
{
   return ( r );
}



FakeTable :: FakeRecord :: Result  FakeTable :: FakeRecord :: __GetFst ( int )
{
   i  = 0;
   
   return ( Result_OK );
}



FakeTable :: FakeRecord :: Result  FakeTable :: FakeRecord :: __GetNxt ()
{
   if ( ( i + 1 ) >= r )
      return ( Result_EOT );
   
   ++i;
   
   return ( Result_OK );
}



long  FakeTable :: FakeRecord :: __GetNxtEx ( long  x )
{
   if ( ( i + 1 ) >= r )
      return (  0 );
      
   size_t   p  = i;

   if ( ( i + x ) < r )
      i += x;
   else
      i  = r - 1;
   
   return ( i - p );
}



FakeTable :: FakeRecord :: Result  FakeTable :: FakeRecord :: __GetPrv ()
{
   if ( i == 0 )
      return ( Result_EOT );
      
   --i;
   
   return ( Result_OK );
}



long  FakeTable :: FakeRecord :: __GetPrvEx ( long  x )
{
   if ( i == 0 )
      return (  0 );
   
   long     p  = i;
   
   if ( i > x )
      i -= x;
   else
      i  = 0;   

   return ( p - i );
}



FakeTable :: FakeRecord :: Result  FakeTable :: FakeRecord :: __GetLst ( int )
{
   i  = r - 1;
   
   return ( Result_OK );
}



FakeTable :: FakeRecord :: Result  FakeTable :: FakeRecord :: __Get ( const Find & )
{
   return ( Result_KEY_VALUE_NOT_FOUND );
}



FakeTable :: Cursor *  FakeTable :: FakeRecord :: __CursorCreate ()
{
   return ( new  DefaultCursor ( i ) );
}



const FakeTable :: Cursor *  FakeTable :: FakeRecord :: __CursorCurrent ()
{
   cursor.Offset ( i );
   
   return ( &cursor );
}



void  FakeTable :: FakeRecord :: __CursorInvalidate ()
{
   cursor.Invalidate ();
}



FakeTable :: FakeRecord :: Result  FakeTable :: FakeRecord :: __CursorSet ( const Cursor *  cursor )
{
   const DefaultCursor *   dc = static_cast < const DefaultCursor * > ( cursor );
   
   if ( ( dc               != 0 ) &&
        ( dc -> IsValid ()      ) &&
        ( dc -> Offset  () >= 0 ) &&
        ( dc -> Offset  () <  r )    )
   {
      i  = dc -> Offset ();
      
      return ( Result_OK );
   }
   
   return ( Result_EOT );
}



FakeTable :: FakeRecord :: Result  FakeTable :: FakeRecord :: __ScrollGetByPos ( long  pos )
{
   if ( ( pos >= 0 ) && ( pos < r ) )
   {
      i  = pos;
      
      return ( Result_OK );
   }
   
   return ( Result_EOT );
}



long  FakeTable :: FakeRecord :: __ScrollPosByCursor  ( const Cursor *  cursor )
{
   return ( static_cast < const DefaultCursor * > ( cursor ) -> Offset () );
}



const bool  FakeTable :: FakeRecord :: CanSort ( size_t  i ) const
{
   return ( i == 0 );
}



const FakeTable :: FakeRecord :: Sequence  FakeTable :: FakeRecord :: GetSort ( size_t  i ) const
{
   if ( i == 0 )
      return ( GetSequence () );
   
   return ( Sequence_NONE );
}



void  FakeTable :: FakeRecord :: SetSort ( size_t  i, const Sequence &  _sequence )
{
   if ( i == 0 )
      SetSequence ( _sequence );
}



FakeTable :: FakeRecord :: FakeRecord ( FakeTable *  _table, size_t  _r, size_t  _c )
   : Record ( _table )
{
   r        = _r;
   c        = _c;
   
   i        =  0;
   
   sequence = Sequence_ASCENDING;
}


   
const wxChar *  FakeTable :: FakeRecord :: FieldString ( size_t  c )
{
   static wxString    str;
   
   str.Printf ( "(%u,%u)", i, c );
   
   return ( str );
}



////////////////////////////////////////////////////////////////////////////////
// FakeTable                                                                  //
////////////////////////////////////////////////////////////////////////////////



const wxChar *  FakeTable :: Name () const
{
   return ( "FakeTable" );
}



const wxChar *  FakeTable :: Desc () const
{
   return ( "FakeTable" );
}



size_t  FakeTable :: NumberOfColumns ()
{
   return ( c );
}



FakeTable :: Column *  FakeTable :: At ( size_t )
{
   return (  0 );
}



const wxChar *  FakeTable :: FieldName ( size_t  i )
{
   static wxString   str;

   str.Printf ( "Column %u", i );
      
   return ( str );
}



const wxChar *  FakeTable :: FieldFullName ( size_t  i )
{
   static wxString   str;

   str.Printf ( "%s.Column %u", Desc (), i );
      
   return ( str );
}



const wxChar *  FakeTable :: HeaderString ( size_t  i )
{
   static wxString   str;

   str.Printf ( "Column %u", i );
      
   return ( str );
}



wxUint32  FakeTable :: HeaderDrawTextFormat ( size_t )
{
   return ( DT_LEFT );
}



FakeTable :: Capability  FakeTable :: GetCapability () const
{
   return ( static_cast < Capability > ( Capability_PERCENTAGE | Capability_HSCROLLTRACK | Capability_VSCROLLTRACK ) );
}



const wxChar *  FakeTable :: FieldString ( size_t  i )
{
   return ( record -> FieldString ( i ) );
}



wxUint32  FakeTable :: FieldDrawTextFormat ( size_t )
{
   return ( DT_LEFT );
}
     


FakeTable :: FakeTable ( size_t  _r, size_t  _c )
   : wxTable ( record = new  FakeRecord ( this, _r, _c ) )
{
   r  = _r;
   c  = _c;
}



FakeTable :: ~FakeTable ()
{
   delete  record;
}
