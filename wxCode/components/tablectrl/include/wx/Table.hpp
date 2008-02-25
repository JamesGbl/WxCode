////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        Table.hpp                                                     //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     1998                                                          //
// Copyright:   (c) 1998-2008 Jan Knepper                                     //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef TABLE_HPP
#define TABLE_HPP



#include <vector>
#include <set>

#include <wx/Table.h>



///////////////////////////////////////////////////////////////////////////////
// Table Field Type                                                          //
///////////////////////////////////////////////////////////////////////////////



const wxUint32          TFT_NULL          = 0x00000000L;
const wxUint32          TFT_TEXT          = 0x00000001L;
const wxUint32          TFT_BITMAP        = 0x00000002L;
//const wxUint32          TFT_METAFILE      = 0x00000004L;
const wxUint32          TFT_ENHMETAFILE   = 0x00000008L;



///////////////////////////////////////////////////////////////////////////////
// Table Field Flag                                                          //
///////////////////////////////////////////////////////////////////////////////



const wxUint32          TFF_NULL          = 0x00000000L;
const wxUint32          TFF_SHOW          = 0x00000001L;
const wxUint32          TFF_SHOW_TOOLTIP  = 0x00000002L;
const wxUint32          TFF_SIZE          = 0x00000004L;
const wxUint32          TFF_USER_0        = 0x00000008L;
const wxUint32          TFF_USER_1        = 0x00000010L;
const wxUint32          TFF_USER_2        = 0x00000020L;
const wxUint32          TFF_USER_3        = 0x00000040L;
const wxUint32          TFF_USER_4        = 0x00000080L;
const wxUint32          TFF_USER_5        = 0x00000100L;



class  wxTable
{
   public    :
      enum
      {
         Reference_NOSORT           = 0xFFFFFFFFL,
      };

      enum  Capability
      {
         Capability_NULL            = 0x0000,

         Capability_PERCENTAGE      = 0x0001,
         Capability_VSCROLLTRACK    = 0x0002,
         Capability_HSCROLLTRACK    = 0x0004,
         Capability_VSCROLLROUGH    = 0x0008,      // Percentages are only rough indication.

         Capability_TEXTCOLOR       = 0x0010,
      };

      enum  Command
      {
         Command_NULL               =     0,
         Command_INSERT             = 10001,
         Command_UPDATE             = 10002,
         Command_DELETE             = 10003,
         Command_COPY               = 10004,

         Command_SHOW               = 10005,

         Command_SUBTABLE           = 11000,

         Command_SORT               = 12000,
         Command_SORT_ASCENDING     = 12100,
         Command_SORT_DESCENDING    = 12200,

         Command_FIND               = 13000,
         Command_SEARCH             = 14000,
      };

      enum  Action
      {
         Action_NULL                = 0x0000,
         Action_INSERT              = 0x0001,
         Action_UPDATE              = 0x0002,
         Action_DELETE              = 0x0004,
         Action_COPY                = 0x0008,

         Action_SUBTABLE            = 0x0010,

         Action_SHOW                = 0x0020,
         Action_SORT                = 0x0040,
         Action_FIND                = 0x0080,

         Action_SEARCH              = 0x0800,
         Action_SEARCH_WRAP_AROUND  = 0x0100,
         Action_SEARCH_UP           = 0x0200,

         Action_SELECT              = 0x1000,

         Action_COMMON              = Action_INSERT | Action_UPDATE | Action_DELETE | Action_COPY | Action_FIND,

         Action_ALL                 = 0xFFFF & ~( Action_SEARCH_UP | Action_SELECT )                           ,
      };

//    enum  FindFlag
//    {
//       FindFlag_SELECTED          = 0x00000001,
//       FindFlag_CHECKED           = 0x00000002,
//    };






      class  Column
      {
         private   :
            wxString                name;
            wxString                desc;
            wxUint32                type;    // TFT_NULL, TFT_TEXT, TFT_BITMAP, TFT_ENHMETAFILE

         protected :
//          iRect                   rect;

            wxUint32                header;
            wxUint32                field;

            wxUint32                tff;     // TFF_NULL, TFF_SHOW, TFF_SHOW_TOOLTIP

         private   :                         // Not Implemented!
            Column &                operator =           ( const Column & );

         public    :
            Column            ();
//          Column            ( const Column & );
            Column            ( const wxString &, const wxString &, wxUint32 = TFF_SHOW  | TFF_SIZE );
            virtual  ~Column  ();

            const wxString &        Name                 () const                            { return ( name ); }
            const wxString &        Desc                 () const                            { return ( desc ); }
            Column &                Name                 ( const wxString & );
            Column &                Desc                 ( const wxString & );
            
            const wxUint32          Type                 () const                            { return ( type ); }

            const wxUint32          HeaderDrawTextFormat () const                            { return ( header ); }
            void                    HeaderDrawTextFormat ( wxUint32  _header )               { header = _header; }

            const wxUint32          FieldDrawTextFormat  () const                            { return ( field );  }
            void                    FieldDrawTextFormat  ( wxUint32  _field )                { field  = _field; }

            const wxUint32          TFF                  () const                            { return ( tff  ); }
            void                    ModifyTFF            ( wxUint32  remove, wxUint32  add ) { tff &= ~remove; tff |= add; }
            
            const bool              Check                () const;
            void                    Check                ( bool );
      };



      class  ColumnVector : protected  std :: vector < Column * >
      {
         private   :
            typedef std :: vector < Column * >
                                    super;
                                    
         public    :
            static const size_type  npos;
            
         private   :
            wxString                name;
            wxString                desc;
            
         private   :                      // Not Implemented!
            ColumnVector  ();
            ColumnVector  ( const ColumnVector & );

         public    :
            ColumnVector  ( const wxString &, const wxString & );
            ~ColumnVector ();

            virtual Column *        At          ( size_t  index )             { return ( at ( index ) ); }
            const size_t            IndexOf     ( const Column * ) const;
            
            const wxString &        Name        () const                      { return ( name ); }
            const wxString &        Desc        () const                      { return ( desc ); }
      };



      class  Cursor
      {
         protected :
            virtual Cursor &     Assign      ( const Cursor & )               = 0;
            virtual int          Compare     ( const Cursor & ) const         = 0;

         protected :
            Cursor           ();
            Cursor           ( const Cursor & );
            
         public    :
            virtual  ~Cursor ();

         public    :
            virtual Cursor *     Clone       () const                         = 0;
            
            virtual Cursor &     operator =  ( const Cursor &  that )         { return ( Assign  ( that ) ); }
#if 0
            virtual Cursor &     operator ++ ();
            virtual Cursor &     operator -- ();
            virtual Cursor &     operator += ( long );
            virtual Cursor &     operator -= ( long );
#endif
            bool                 operator == ( const Cursor &  that ) const   { return ( Compare ( that ) == 0 ); }
            bool                 operator != ( const Cursor &  that ) const   { return ( Compare ( that ) != 0 ); }
#if 0            
            bool                 operator <  ( const Cursor &  that ) const   { return ( Compare ( that ) <  0 ); }
            bool                 operator <= ( const Cursor &  that ) const   { return ( Compare ( that ) <= 0 ); }
            bool                 operator >= ( const Cursor &  that ) const   { return ( Compare ( that ) >= 0 ); }
            bool                 operator >  ( const Cursor &  that ) const   { return ( Compare ( that ) >  0 ); }
#endif
            virtual const bool   IsValid     () const                         = 0;
            virtual void         Invalidate  ()                               = 0;
      };



      template < class  T > 
      class  GenericCursor : public  Cursor
      {
         private   :
            typedef Cursor    super;
            
            typedef GenericCursor < T >
                              This;
            
         public    :
            typedef T         Type;
            
         protected :
            This &            Assign      ( const Cursor &  _that )
            {
               const This &   that  = static_cast < const This & > ( _that );
               offset   = that.offset;
//             valid    = true;
               valid    = that.valid;

               return ( *this );
            }
            
            
            
            int               Compare     ( const Cursor &  _that ) const
            {
               const This &   that  = static_cast < const This & > ( _that );
               
               if ( ! valid )
                  return ( -2 );

               if ( ! that.valid )
                  return (  2 );

               if ( offset < that.offset )
                  return ( -1 );

               if ( offset > that.offset )
                  return (  1 );

               return (  0 );
            }
            
            
            
         public    :
            This *            Clone       () const
            {
               return ( new  This ( *this ) );
            }
            
            
            
            const bool        IsValid     () const
            {
               return ( valid );
            }
            
            
            
            void              Invalidate  ()
            {
               offset   = 0;
               valid    = false;
            }
            
            
            
         protected :
            T                 offset;
            bool              valid;
            
         public    :
            GenericCursor  ()
               : Cursor ()
            {
               offset   = 0;
               valid    = false;            
            }
            
            
            
            GenericCursor  ( const This &  that )
               : Cursor ( that )
            {
               offset   = that.offset;
               valid    = that.valid;
            }
            
            
            
            GenericCursor  ( const T &  _offset )
               : Cursor ()
            {
               offset   = _offset;
               valid    = true;
            }
            
            
            
            GenericCursor < T > &
                              operator =  ( const GenericCursor < T > &  that )
            {
               offset   = that.offset;
               valid    = that.valid;
               
               return ( *this );
            }
            
            
            
            const T           Offset      () const
            {
               return ( offset );
            }
            
            
            
            void              Offset      ( const T &  _offset )
            {
               offset   = _offset;
               valid    = true;
            }
      };



      typedef GenericCursor < wxUint32 >
                              DefaultCursor;
      typedef GenericCursor < wxUint64 >
                              DefaultLargeCursor;



      class  CursorVector : private  std :: vector < Cursor * >
      {
         private   :
            typedef std :: vector < Cursor * >
                              super;
                              
         public    :
            static const size_type  
                              npos;
            
         private   :
            size_type         active;              // Active number of 'Cursor *'.

         private   :                               // Not Implemented!
            CursorVector           ( const CursorVector & );

         public    :
            CursorVector           ();
            virtual  ~CursorVector ();

            void              Allocate    ( size_type, const Cursor * );
            void              Clear       ();
            int               Active      () const                         { return ( active ); }

            const Cursor *    operator [] ( size_type ) const;
            Cursor *          operator [] ( size_type );
            size_type         IndexOf     ( const Cursor & );
            bool              Find        ( const Cursor & );

            const Cursor *    Top         () const;
            void              Top         ( const Cursor & );
            void              Invalidate  ();
      };



      class  Set
      {
         protected :
            Set           ();

         public    :
            virtual  ~Set ();

            virtual bool            Insert      ( const Cursor * )         = 0;
            virtual bool            Toggle      ( const Cursor * )         = 0;
            virtual bool            Delete      ( const Cursor * )         = 0;
            virtual const bool      Find        ( const Cursor * ) const   = 0;

            virtual const Cursor *  First       () const                   = 0;
            virtual const Cursor *  Next        ( const Cursor * ) const   = 0;

            virtual const size_t    Size        () const                   = 0;

            virtual bool            All         ()                         = 0;
            virtual bool            None        ()                         = 0;
            virtual bool            Invert      ()                         = 0;
            virtual const bool      Inverted    () const                   = 0;
      };



      template < class  T >
      class  GenericSet : public  Set
      {
         private   :
            typedef Set             super;
            
         public    :
            typedef GenericSet < T >
                                    This;
            typedef GenericCursor < T >
                                    ThisCursor;
            
         private   :
            typedef std :: set < T, std :: less < T > > 
                                    Data;
                                    
         public    :
            bool                    Insert      ( const Cursor * );
            bool                    Toggle      ( const Cursor * );
            bool                    Delete      ( const Cursor * );
            const bool              Find        ( const Cursor * ) const;

            const size_t            Size        () const;

            const ThisCursor *      First       () const;
            const ThisCursor *      Next        ( const Cursor * ) const;

            bool                    All         ();
            bool                    None        ();
            bool                    Invert      ();
            const bool              Inverted    () const;

         private   :
            Data                    data;
            bool                    inverted;
            mutable ThisCursor      cursor;

         public    :
            GenericSet  ();
            ~GenericSet ();
      };
      
      
      
      typedef GenericSet < unsigned long >
                                    DefaultSet;



      class  SetAdapter  
      {
         private   :
            wxTable *         table;
            Set *             set;

         public    :
            SetAdapter  ();

            void              Table             ( wxTable * );

            const bool        IsActive          () const                   { return ( set != 0 ); }
            
            const Set *       Active            () const                   { return ( set ); }
            Set *             Active            ()                         { return ( set ); }
            void              Use               ( Set * );

            const bool        Find              ( const Cursor * ) const;
            bool              Insert            ( const Cursor * );
            bool              Delete            ( const Cursor * );
            bool              Toggle            ( const Cursor * );
            bool              Select            ( const Cursor *, const bool & );
            bool              All               ( const bool & = true );
            bool              Invert            ();
            const bool        IsInverted        () const;
            const size_t      Size              () const;
            const Cursor *    FindFirst         () const;
            const Cursor *    FindNext          ( const Cursor * ) const;
      };



      class  Record
      {
         public    :
            enum  Find
            {
               Find_EQ                    =  0,
               Find_LE                    =  1,
               Find_LT                    =  2,
               Find_GE                    =  3,
               Find_GT                    =  4,
            };



            enum  Sequence
            {
               Sequence_NONE              =  0,
               Sequence_ASCENDING         =  1,
               Sequence_DESCENDING        = -1,
            };



            enum  Result
            {
               Result_OK                  =     0,
               Result_INVALID_OPERATION   =     1,
               Result_KEY_VALUE_NOT_FOUND =     4,
               Result_DUPLICATE_KEY_VALUE =     5,
               Result_POSITION_NOT_SET    =     8,
               Result_EOT                 =     9,
               Result_LOCK_ERROR          =    81,

               Result_NOT_IN_INDEX        = 10000,
            };



         protected :
            wxTable *               table;
            int                     index;               // Index the table is sorted
                                                         // on. -1 means 'no sort'.
            Sequence                sequence;            // Sequence, the table is
                                                         // sorted ascending or descending.
         private   :
            const size_t            _NumberOfRecords     () const;
            const size_t            _NumberOfKeys        ( int = -1 ) const;

            Result                  _GetFst              ( int = -1 );
            Result                  _GetNxt              ();
            long                    _GetNxtEx            ( long = 0 );
            Result                  _GetPrv              ();
            long                    _GetPrvEx            ( long = 0 );
            Result                  _GetLst              ( int = -1 );

            Result                  _Get                 ( const Find &, int = -1 );

            Result                  _Insert              ();
            Result                  _Update              ();
            Result                  _Delete              ();

            Result                  _Lock                ();
            Result                  _Lock                ( const Cursor * );
            Result                  _Unlock              ();
            Result                  _Unlock              ( const Cursor * );

            Cursor *                _CursorCreate        ();
            const Cursor *          _CursorCurrent       ();
            Result                  _CursorSet           ( const Cursor * );

            Result                  _ScrollGetByPos      ( long );
            long                    _ScrollPosByCursor   ( const Cursor * );

            long                    _ScrollPosMin        ();
            long                    _ScrollPosMax        ();

         protected :
            virtual void            _Index               ( int )                          = 0;

         protected :
            virtual const size_t    __NumberOfRecords    () const                         = 0;
            virtual const size_t    __NumberOfKeys       ( int = -1 ) const               = 0;

            virtual Result          __GetFst             ( int = -1 )                     = 0;
            virtual Result          __GetNxt             ()                               = 0;
            virtual long            __GetNxtEx           ( long = 0 )                     = 0;
            virtual Result          __GetPrv             ()                               = 0;
            virtual long            __GetPrvEx           ( long = 0 )                     = 0;
            virtual Result          __GetLst             ( int = -1 )                     = 0;

            virtual Result          __Get                ( const Find & )                 = 0;

            virtual Result          __Insert             ();
            virtual Result          __Update             ();
            virtual Result          __Delete             ();

            virtual Result          __Lock               ()                               { return ( Result_LOCK_ERROR ); }
            virtual Result          __Lock               ( const Cursor * )               { return ( Result_LOCK_ERROR ); }
            virtual Result          __Unlock             ()                               { return ( Result_LOCK_ERROR ); }
            virtual Result          __Unlock             ( const Cursor * )               { return ( Result_LOCK_ERROR ); }

            virtual Cursor *        __CursorCreate       ()                               = 0;
            virtual const Cursor *  __CursorCurrent      ()                               = 0;
            virtual void            __CursorInvalidate   ()                               = 0;
            virtual Result          __CursorSet          ( const Cursor * )               = 0;

            virtual Result          __ScrollGetByPos     ( long )                         = 0;
            virtual long            __ScrollPosByCursor  ( const Cursor * )               = 0;

            virtual long            __ScrollPosMin       ()                               { return (  0 ); }
            virtual long            __ScrollPosMax       ()                               { return ( NumberOfKeys ( index ) - 1 ); }
// TODO: __KeySpan
//???       virtual long            __KeySpan            ( iRecord *, iRecord *, int )    = 0;

            virtual void            __Synchronize        ();

         public    :
            Record            ( wxTable * );  
            virtual  ~Record  ();

            wxTable *               GetTable             () const                         { return ( table ); }
            
            virtual const bool      CanSort              ( size_t ) const;
            virtual const Sequence  GetSort              ( size_t ) const;
            virtual void            SetSort              ( size_t, const Sequence & );

            virtual const bool      HasNoSort            () const;
            virtual const Sequence  GetNoSort            () const;
            virtual void            SetNoSort            ( const Sequence & = Sequence_ASCENDING );

            const int               GetIndex             () const;
            void                    SetIndex             ( int );

            const Sequence          GetSequence          () const                         { return ( sequence ); }
            void                    SetSequence          ( const Sequence &  _sequence )  { sequence  = _sequence; }

            const size_t            NumberOfRecords      () const;
            const size_t            NumberOfKeys         ( int = -1 ) const;

            Result                  GetFst               ( int = -1 );
            Result                  GetNxt               ();
            long                    GetNxtEx             ( long = 0 );
            Result                  GetPrv               ();
            long                    GetPrvEx             ( long = 0 );
            Result                  GetLst               ( int = -1 );

            Result                  Get                  ( const Find &, int = -1  );

            size_t                  NumberOfBLObs        () const;
            Result                  GetBLOb              ( size_t );
            
            Result                  Insert               ();
            Result                  Update               ();
            Result                  Delete               ();

            Result                  Lock                 ();
            Result                  Lock                 ( const Cursor * );
            Result                  Unlock               ();
            Result                  Unlock               ( const Cursor * );

            Cursor *                CursorCreate         ();
            const Cursor *          CursorCurrent        ();
            Result                  CursorSet            ( const Cursor * );
            void                    CursorInvalidate     ();

            Result                  ScrollGetByPos       ( long );
            long                    ScrollPosByCursor    ( const Cursor * );

            long                    ScrollPosMin         ()                               { return ( 0 ); }
            long                    ScrollPosMax         ()                               { return ( NumberOfKeys ( GetIndex () ) ); }

            void                    Synchronize          ();
      };



   private   :

   protected :
      Record *                record;

      wxUint32                action4table;

      SetAdapter              select;
      SetAdapter              check;

#ifdef __JAK_TABLE_RELATION__

      // Sub Table Relations, i.e. tables that 'could' be sub-tables of this table.

      wxTableRelationArray *  relationarray;

      // Defines if this table is actively acting as a sub-table.

      wxTableRelationKey *    relationkey;

      long                    relationlock;

      Record *                first;
      Record *                last;
#endif

   private   :                            // Not Implemented.
      wxTable            ( const wxTable & );
      
      wxTable &               operator =           ( const wxTable & );

   protected :
      wxTable            ( Record * );

   protected :
      const Record &          _Record              () const;
      virtual void            _Record              ( Record * );
      
   public    :
      virtual  ~wxTable ();

      virtual const Record *  GetRecord            () const             { return ( record ); }
      virtual Record *        GetRecord            ()                   { return ( record ); }

      virtual const wxChar *  Name                 () const             = 0;     // Internal.
      virtual const wxChar *  Desc                 () const             = 0;     // External.

      const wxChar *          DescOrName           () const;

      virtual size_t          NumberOfColumns      ()                   = 0;

//    virtual wxUint32        Reference            ( long )             = 0;

      virtual Column *        At                   ( size_t )           = 0;
      
      virtual const wxChar *  FieldName            ( size_t )           = 0;
      virtual const wxChar *  FieldFullName        ( size_t )           = 0;

      virtual const wxChar *  HeaderString         ( size_t )           = 0;
      virtual wxUint32        HeaderDrawTextFormat ( size_t )           = 0;
      virtual void            HeaderDrawTextFormat ( size_t, wxUint32 );

//    virtual const iRect &   HeaderRect           ( DWORD );
//    virtual void            HeaderRect           ( DWORD, const iRect & );

//    virtual int             HeaderHeight         ();
//    virtual void            HeaderHeight         ( int );

      virtual Action          Action4Record        ();
      Action                  Action4Table         ();
      void                    Action4Table         ( const Action & );
      void                    Action4Table         ( wxUint32 );
      void                    ModifyAction4Table   ( const Action &, const Action & );
      void                    ModifyAction4Table   ( wxUint32, wxUint32 );

      virtual Capability      GetCapability        () const             = 0;
//    virtual bool            HasPercentage        () const             = 0;
//    virtual bool            HasScrollTrack       () const             = 0;

      virtual wxUint32        FieldFlag            ( size_t ) const;
      virtual wxUint32        FieldType            ( size_t ) const;
      virtual const wxChar *  FieldString          ( size_t )           = 0;
      virtual UINT            FieldDrawTextFormat  ( size_t )           = 0;
      virtual void            FieldDrawTextFormat  ( size_t, wxUint32 );
      virtual void *          FieldData            ( size_t, size_t );
      virtual wxUint32        FieldDataSize        ( size_t );

//    virtual void            Reference            ( const iReference & );

      SetAdapter &            Select               ()                   { return ( select ); }
      SetAdapter &            Check                ()                   { return ( check  ); }

#ifdef __JAK_TABLE_RELATION__
      wxTableRelationArray *  GetRelationArray     () const             { return ( relationarray ); }
      void                    RelationInsert       ( wxTableRelation * );
      void                    RelationDelete       ( wxTableRelation * );

      bool                    RelationKey          ( wxTableRelationKey * );
      wxTableRelationKey *    RelationKey          () const             { return ( relationkey   ); }
      void                    RelationKeyReset     ()                   { relationkey  = 0; }
      int                     RelationKeyCompare   ( iRecord * );
      void                    RelationSync         ();
      void                    RelationDeactivate   ();
      long                    RelationLock         ();
      long                    RelationUnlock       ();
      bool                    RelationActive       () const;
#endif
};



template < class  T >
bool  wxTable :: GenericSet < T > :: Insert ( const Cursor *  _cursor )
{
   const ThisCursor *   cursor   = static_cast < const ThisCursor * > ( _cursor );
   
   if ( ! inverted )
      return ( data.insert ( cursor -> Offset () ).second );
   else
      return ( data.erase  ( cursor -> Offset () ) == 1 );
}



template < class  T >
bool  wxTable :: GenericSet < T > :: Toggle ( const Cursor *  _cursor )
{
   const ThisCursor *   cursor   = static_cast < const ThisCursor * > ( _cursor );
   
   if ( data.find ( cursor -> Offset () ) == data.end () )
      return ( data.insert ( cursor -> Offset () ).second );
   else
      return ( data.erase  ( cursor -> Offset () ) == 1 );
}



template < class  T >
bool  wxTable :: GenericSet < T > :: Delete ( const Cursor *  _cursor )
{
   const ThisCursor *   cursor   = static_cast < const ThisCursor * > ( _cursor );
   
   if ( ! inverted )
      return ( data.erase  ( cursor -> Offset () ) == 1 );
   else
      return ( data.insert ( cursor -> Offset () ).second );
}



template < class  T >
const bool  wxTable :: GenericSet < T > :: Find ( const Cursor *  _cursor ) const
{
   const ThisCursor *   cursor   = static_cast < const ThisCursor * > ( _cursor );
   
   if ( ! inverted )
      return ( data.find ( cursor -> Offset () ) != data.end () );
   else
      return ( data.find ( cursor -> Offset () ) == data.end () );
}



template < class  T >
const typename wxTable :: GenericSet < T > :: ThisCursor *  wxTable :: GenericSet < T > :: First () const
{
   Data :: const_iterator     i = data.begin   ();

   if ( i != data.end () )
   {
      cursor.Offset  ( *i );

      return ( &cursor );
   }

   return (  0 );
}



template < class  T >
const typename wxTable :: GenericSet < T > :: ThisCursor *  wxTable :: GenericSet < T > :: Next ( const Cursor *  _next ) const
{
   const ThisCursor *      next  = static_cast < const ThisCursor * > ( _next );
   Data :: const_iterator  i     = data.find ( next -> Offset () );

   if ( ++i != data.end () )
   {
      cursor.Offset  ( *i );

      return ( &cursor );
   }

   return (  0 );
}



template < class  T >
const size_t  wxTable :: GenericSet < T > :: Size () const
{
   return ( data.size () );
}



template < class  T >
bool  wxTable :: GenericSet < T > :: All ()
{
   data.erase  ( data.begin (), data.end () );

   inverted = TRUE;

   return ( TRUE );
}



template < class  T >
bool  wxTable :: GenericSet < T > :: None ()
{
   data.erase  ( data.begin (), data.end () );

   inverted = FALSE;

   return ( TRUE );
}



template < class  T >
bool  wxTable :: GenericSet < T > :: Invert ()
{
   inverted = ! inverted;

   return ( TRUE );
}



template < class  T >
const bool  wxTable :: GenericSet < T > :: Inverted () const
{
   return ( inverted );
}



template < class  T >
wxTable :: GenericSet < T > :: GenericSet ()
   : Set ()
{
   inverted = false;
}



template < class  T >
wxTable :: GenericSet < T > :: ~GenericSet ()
{
}



///////////////////////////////////////////////////////////////////////////////
// wxTablePosition.                                                          //
///////////////////////////////////////////////////////////////////////////////



class  wxTablePosition
{
   public    :
      enum  Flag
      {
         Flag_AUTO   = 0x0001,
         Flag_MANUAL = 0x0002,
      };



   private   :
      Flag                 flag;          // Automatic or Manual.

      wxTable *            table;
      wxTable :: Record *  record;
      wxTable :: Cursor *  cursor;
      int                  index;

      void                 Init           ( const Flag &, wxTable *, wxTable :: Record * );

   public    :
      wxTablePosition            (               wxTable * = 0, wxTable :: Record * = 0 );
      wxTablePosition            ( const Flag &, wxTable *    , wxTable :: Record * = 0 );
      virtual  ~wxTablePosition  ();

      virtual void         Backup         ();
      virtual void         Restore        ();

      virtual void         Manual         ();
      virtual void         Auto           ();
};



#endif
