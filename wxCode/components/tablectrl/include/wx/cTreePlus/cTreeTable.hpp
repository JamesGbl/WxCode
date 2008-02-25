////////////////////////////////////////////////////////////////////////////////
// Soli Deo Gloria!                                                           //
//                                                                            //
// Name:        cTreePlusView.hpp                                             //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007                                                          //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef CTREETABLE_HPP
#define CTREETABLE_HPP



#include <wx/Table.hpp>

#include <ctdbsdk.hpp>                    // cTreePlus DB SDK.

#undef SIZEOF
#undef GetRecord

#include "cTreeTable.h"



class  cTreeTable : public  wxTable
{
   private   :
      typedef wxTable   super;

   public    :
      class  RecordEx : public  Record
      {
         private   :
            typedef Record       super;

         private   :
            typedef DefaultLargeCursor
                                 CursorEx;
                                 
         public    :
            friend class         cTreeTable;

         protected :                      // wxTable :: Record
            void                 _Index               ( int );
            
         protected :
            const size_t         __NumberOfRecords    () const;
            const size_t         __NumberOfKeys       ( int = -1 ) const;

            Result               __GetFst             ( int = -1 );
            Result               __GetNxt             ();
            long                 __GetNxtEx           ( long = 0 );
            Result               __GetPrv             ();
            long                 __GetPrvEx           ( long = 0 );
            Result               __GetLst             ( int = -1 );

            Result               __Get                ( const Find & );

            Result               __Insert             ();
            Result               __Update             ();
            Result               __Delete             ();

            Result               __Lock               ();
            Result               __Lock               ( const Cursor * );
            Result               __Unlock             ();
            Result               __Unlock             ( const Cursor * );

            Cursor *             __CursorCreate       ();
            const Cursor *       __CursorCurrent      ();
            void                 __CursorInvalidate   ();
            Result               __CursorSet          ( const Cursor * );

            Result               __ScrollGetByPos     ( long );
            long                 __ScrollPosByCursor  ( const Cursor * );
           
         public    :
            const bool           CanSort              ( size_t ) const;
            const Sequence       GetSort              ( size_t ) const;
            void                 SetSort              ( size_t, const Sequence & );

            const bool           HasNoSort            () const;
            const Sequence       GetNoSort            () const;
            void                 SetNoSort            ( const Sequence & = Sequence_ASCENDING );
         
         private   :
            CTRecord &           record;
            CursorEx             cursor;
            
         private   :
            const int            GetIndexId4          ( size_t ) const;

         public    :
            RecordEx ( cTreeTable *, CTRecord & );

            virtual CTRecord &   GetRecord            ()                   { return ( record ); }
            virtual CTRecord &   GetRecord            () const             { return ( record ); }
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

      wxUint32          FieldFlag            ( size_t ) const;
      const wxChar *    FieldString          ( size_t );
      UINT              FieldDrawTextFormat  ( size_t );

   protected :
      UINT              DrawTextFormat       ( size_t ) const;

   protected :
      CTTable &         table;
      RecordEx          record;
                        
   public    :
      cTreeTable  ( CTTable &, CTRecord & );
      
      CTTable &         GetTable             ()                   { return ( table ); }
      CTTable &         GetTable             () const             { return ( table ); }
      
      RecordEx &        GetRecordEx          ()                   { return ( record ); }
      
      virtual void      Synchronize          ();
};



#endif
