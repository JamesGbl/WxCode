////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        ODBCTable.hpp                                                 //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007                                                          //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef ODBCTABLE_HPP
#define ODBCTABLE_HPP



#include <wx/dynarray.h>
#include <wx/dbtable.h>
#include <wx/Table.hpp>

#include "ODBCTable.h"



class  wxODBCEnv : public  wxObject
{
   private   :
      typedef wxObject     super;
      
   public    :
      enum  Fetch
      {
         Fetch_FIRST    = SQL_FETCH_FIRST ,
         Fetch_NEXT     = SQL_FETCH_NEXT  ,
      };
      
      
      
   private   :
      SQLHENV              env;
      
   public    :
      wxODBCEnv   ();
      ~wxODBCEnv  ();

      operator             SQLHENV     ()          { return ( env ); }
      
      bool                 DataSources ( wxString &, wxString &, const Fetch & = Fetch_NEXT );
};



class  wxODBCDbc : public  wxObject
{
   private   :
      typedef wxObject     super;
      
   private   :
      wxODBCEnv &          env;
      SQLHDBC              dbc;
      
   public    :
      wxODBCDbc   ( wxODBCEnv & );
      ~wxODBCDbc  ();

      operator             SQLHDBC     ()          { return ( dbc ); }
      
      bool                 Connect     ( const wxString &, const wxString &, const wxString & );
      bool                 GetTables   ( wxODBCTableArray & );
      bool                 GetColumns  ( wxODBCColumnArray &, const wxString & );
};



class  wxODBCStatement : public  wxObject
{
   private   :
      typedef wxObject        super;

   public    :
      enum  Orientation
      {
         Orientation_NXT            = SQL_FETCH_NEXT           ,
         Orientation_FST            = SQL_FETCH_FIRST          ,
         Orientation_LST            = SQL_FETCH_LAST           ,
         Orientation_PRV            = SQL_FETCH_PRIOR          ,
         Orientation_ABSOLUTE       = SQL_FETCH_ABSOLUTE       ,
         Orientation_RELATIVE       = SQL_FETCH_RELATIVE       ,
         Orientation_BOOKMARK       = SQL_FETCH_BOOKMARK       ,
      };
      
      
      
      enum  CursorType
      {
         CursorType_FORWARD_ONLY    = SQL_CURSOR_FORWARD_ONLY  ,
         CursorType_KEYSET_DRIVEN   = SQL_CURSOR_KEYSET_DRIVEN ,
         CursorType_CURSOR_DYNAMIC  = SQL_CURSOR_DYNAMIC       ,
         CursorType_STATIC          = SQL_CURSOR_STATIC        ,
         
         CursorType_DEFAULT         = SQL_CURSOR_TYPE_DEFAULT  ,
      };
      
      
      
   private   :
      wxODBCDbc &             dbc;
      SQLHSTMT                stmt;
      
   protected :
      bool                    ExecDirect     ( const wxString & );
      
   public    :
      wxODBCStatement   ( wxODBCDbc & );
      ~wxODBCStatement  ();

      operator                SQLHSTMT       ()          { return ( stmt ); }
      
      bool                    Tables         ();
      bool                    Columns        ( const wxString & );
      bool                    SelectAll      ( const wxString & );
      wxUint32                CountAll       ( const wxString & );
      
      bool                    Fetch          ();
      bool                    FetchScroll    ( const Orientation &, wxInt32 = 0 );
      
      bool                    GetData        ( size_t, wxString & );
      bool                    GetData        ( size_t, short & );
      bool                    GetData        ( size_t, long & );
      bool                    GetData        ( size_t, wxUint32 & );
      
      bool                    SetCursorType  ( const CursorType & );
      
      static const wxChar *   GetString      ( const Orientation & );
};



class  wxODBCColumn : public  wxObject
{
   private   :
      typedef wxObject     super;
 
   private   :
      wxString             name;
      short                type;
      wxString             typestr;
      long                 length;
      short                buffer;
      short                decimals;
      short                radix;
      short                nullable;
      wxString             remarks;   
        
   public    :
      wxODBCColumn   ();
      wxODBCColumn   ( const wxString &, short, const wxString &, long, short, short, short, short, const wxString & );
      
      const wxString &     GetName     () const       { return ( name    ); }
      const short &        GetType     () const       { return ( type    ); }
      const wxString &     GetTypeStr  () const       { return ( typestr ); }
      
      const wxString &     GetRemarks  () const       { return ( remarks ); }
};



WX_DECLARE_OBJARRAY  ( wxODBCColumn , wxODBCColumnArray  );



class  wxODBCTable : public  wxObject
{
   private   :
      typedef wxObject     super;
      
   private   :
      wxString             name;
      wxString             type;
      wxString             remarks;
      
//    wxODBCColumnArray *  columns;
      
   public    :
      wxODBCTable ();
      wxODBCTable ( const wxString &, const wxString &, const wxString & );
      
      const wxString &     GetName     () const       { return ( name    ); }
      const wxString &     GetType     () const       { return ( type    ); }
      const wxString &     GetRemarks  () const       { return ( remarks ); }
};



WX_DECLARE_OBJARRAY  ( wxODBCTable  , wxODBCTableArray   );



class  ODBCTable : public  wxTable
{
   private   :
      typedef wxTable   super;
      
   public    :
#if 0   
      class  CursorEx : public  GenericCursor < HSTMT >
      {
         private   :
            typedef GenericCursor < HSTMT >
                              super;
                              
         public    :
            const bool        IsValid     () const;
            
         public    :
            CursorEx ();            
            CursorEx ( const CursorEx & );
            CursorEx ( HSTMT );
            
            CursorEx &        operator = ( const CursorEx & );
            
            HSTMT *           Statement  ();
      };
#endif      
      
      
      class  RecordEx : public  Record
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

         private   :
            size_t &          cols;
            size_t &          rows;
            wxODBCStatement & stmt;
            long              row;
            DefaultCursor     cursor;
            
         public    :
            RecordEx ( ODBCTable * );
            
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
      
      wxUint32          FieldFlag            ( size_t ) const;

      const wxChar *    FieldString          ( size_t );
      wxUint32          FieldDrawTextFormat  ( size_t );
      
   private   :
      RecordEx *        record;
      wxODBCDbc *       dbc;
      wxString          name;
      wxODBCColumnArray ca;
      size_t            cols;
      size_t            rows;
      wxODBCStatement   stmt;
      
   public    :
      ODBCTable   ( wxODBCDbc *, const wxString & );
      ~ODBCTable  ();
};



#endif
