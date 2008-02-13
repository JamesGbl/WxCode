////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        FakeTable.hpp                                                 //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007/10/07                                                    //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef FAKETABLE_HPP
#define FAKETABLE_HPP



#include <wx/Table.hpp>

#include "FakeTable.h"



class  FakeTable : public  wxTable
{
   private   :
      typedef wxTable   super;
      
   public    :
      class  FakeRecord : public  Record
      {
         private   :
            typedef Record    super;
            
         protected :                      // Record
            void              _Index               ( int );

         protected :                      // Record
            const size_t      __NumberOfRecords    () const;
            const size_t      __NumberOfKeys       ( int = -1 ) const;

            Result            __GetFst             ( int = -1 );
            Result            __GetNxt             ();
            long              __GetNxtEx           ( long = 0 );
            Result            __GetPrv             ();
            long              __GetPrvEx           ( long = 0 );
            Result            __GetLst             ( int = -1 );

            Result            __Get                ( const Find & );

            Cursor *          __CursorCreate       ();
            const Cursor *    __CursorCurrent      ();
            void              __CursorInvalidate   ();
            Result            __CursorSet          ( const Cursor * );

            Result            __ScrollGetByPos     ( long );
            long              __ScrollPosByCursor  ( const Cursor * );

         public    :
            const bool        CanSort              ( size_t ) const;
            const Sequence    GetSort              ( size_t ) const;
            void              SetSort              ( size_t, const Sequence & );
            
         private   :
            size_t            r;
            size_t            c;
            
            size_t            i;
            
            DefaultCursor     cursor;
            
         public    :
            FakeRecord  ( FakeTable *, size_t, size_t );
            
            const wxChar *    FieldString          ( size_t );
      };
      
      
      
   public    :      
      const wxChar *    Name                 () const;
      const wxChar *    Desc                 () const;

      size_t            NumberOfColumns      ();

      Column *          At                   ( size_t );
      
      const wxChar *    FieldName            ( size_t );
      const wxChar *    FieldFullName        ( size_t );

      const wxChar *    HeaderString         ( size_t );
      wxUint32          HeaderDrawTextFormat ( size_t );
      
      Capability        GetCapability        () const;

      const wxChar *    FieldString          ( size_t );
      wxUint32          FieldDrawTextFormat  ( size_t );
      
   private   :
      FakeRecord *      record;
      
      size_t            r;
      size_t            c;
      
   public    :
      FakeTable   ( size_t, size_t );
      ~FakeTable  ();
};



#endif
