// Soli Deo Gloria!



#ifndef ODBCTABLE_HPP
#define ODBCTABLE_HPP



#include <wx/dbtable.h>
#include <wx/Table.hpp>

#include "ODBCTable.h"



class  wxDbTableEx : public  wxDbTable
{
   private   :
      typedef wxDbTable    super;
      
   public    :
      wxDbTableEx ( wxDb *, const wxString &, const wxUint16, const wxString & = wxEmptyString, bool = ! wxDB_QUERY_ONLY, const wxString & = wxEmptyString );

      bool                 UpdateColumns  ( HSTMT );
      bool                 SetPos         ( wxInt16 );
};



class  ODBCTable : public  wxTable
{
   private   :
      typedef wxTable   super;
      
   public    :
#if 0   
      class  ODBCCursor : public  GenericCursor < HSTMT >
      {
         private   :
            typedef GenericCursor < HSTMT >
                              super;
                              
         public    :
            const bool        IsValid     () const;
            
         public    :
            ODBCCursor  ();            
            ODBCCursor  ( const ODBCCursor & );
            ODBCCursor  ( HSTMT );
            
            ODBCCursor &      operator = ( const ODBCCursor & );
            
            HSTMT *           Statement  ();
      };
#endif      
      
      
      class  ODBCRecord : public  Record
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
            wxDbTableEx &     dbtable;
            long              row;
            DefaultCursor     cursor;
            
         public    :
            ODBCRecord  ( ODBCTable * );
            
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
      ODBCRecord *      record;
      mutable wxDbTableEx 
                        dbtable;
      wxDbColDef *      coldefs;
      
   public    :
      ODBCTable   ( wxDb *, const wxString &, const wxUint16, const wxString & );
};



#endif
