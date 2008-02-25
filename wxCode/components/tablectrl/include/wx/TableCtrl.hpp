////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        TableCtrl.hpp                                                 //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     1998                                                          //
// Copyright:   (c) 1998-2008 Jan Knepper                                     //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef TABLECTRL_HPP
#define TABLECTRL_HPP



#include <wx/window.h>
#include <wx/timer.h>
#include <wx/dnd.h>
#include <wx/tooltip.h>

#include <vector>
#include <list>

#include <wx/Table.hpp>
#include <wx/TableCtrl.h>



typedef void         ( wxEvtHandler :: *wxTableEventFunction  )  ( wxTableEvent & );



#define wxTableEventHandler(func) \
   (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxTableEventFunction, &func)



#define EVT_TABLE(func) \
   wx__DECLARE_EVT0(wxEVT_TABLE,NADTableEventHandler(func))



BEGIN_DECLARE_EVENT_TYPES  ()
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_COLUMN_BEGIN_SIZE   , 1000 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_COLUMN_SIZING       , 1001 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_COLUMN_SIZED        , 1002 )
   
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_COLUMN_BEGIN_MOVE   , 1003 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_COLUMN_MOVING       , 1004 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_COLUMN_MOVED        , 1005 )
   
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_HEADER_BEGIN_SIZE   , 1006 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_HEADER_SIZING       , 1007 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_HEADER_SIZED        , 1008 )

// DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_HEADER_LEFT_CLICK   , 1009 )
// DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_HEADER_RIGHT_CLICK  , 1010 )
// DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_HEADER_MIDDLE_CLICK , 1011 )
   
// DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_HEADER_LEFT_DCLICK  , 1012 )
// DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_HEADER_RIGHT_DCLICK , 1013 )
// DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_HEADER_MIDDLE_DCLICK, 1014 )

   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_BEGIN_LEFT_DRAG     , 1015 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_BEGIN_RIGHT_DRAG    , 1016 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_BEGIN_MIDDLE_DRAG   , 1017 )

   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_RECORD_SELECTED     , 1018 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_RECORD_DESELECTED   , 1019 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_RECORD_ACTIVATED    , 1020 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_RECORD_FOCUSED      , 1021 )
   
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_RECORD_LEFT_CLICK   , 1022 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_RECORD_RIGHT_CLICK  , 1023 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_RECORD_MIDDLE_CLICK , 1024 )
   
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_RECORD_LEFT_DCLICK  , 1025 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_RECORD_RIGHT_DCLICK , 1026 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_RECORD_MIDDLE_DCLICK, 1027 )

   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_CHECKBOX_CLICKED    , 1028 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_CHECKBOX_CHECKED    , 1029 )
   
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_RECORD_ATTRIBUTES   , 1030 )
   
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_KEY_DOWN            , 1031 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_KEY_UP              , 1032 )

   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_COLUMN_LEFT_CLICK   , 1033 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_COLUMN_RIGHT_CLICK  , 1034 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_COLUMN_MIDDLE_CLICK , 1035 )

   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_INSERT              , 1036 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_UPDATE              , 1037 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_DELETE              , 1038 )

   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_COPY                , 1039 )
   
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_INSERTING           , 1040 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_UPDATING            , 1041 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_DELETING            , 1042 )

   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_COPYING             , 1043 )

   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_INSERTED            , 1044 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_UPDATED             , 1045 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_DELETED             , 1046 )

   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_COPIED              , 1047 )

   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_SORT                , 1048 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_SORT_ASCENDING      , 1049 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_SORT_DESCENDING     , 1050 )
   
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_FIND                , 1051 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_SEARCH              , 1052 )
   
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_DRAW_COLUMN_HEADER  , 1053 )
   DECLARE_EVENT_TYPE   ( wxEVT_COMMAND_TABLE_DRAW_COLUMN_DATA    , 1054 )
END_DECLARE_EVENT_TYPES    ()



#define wx__DECLARE_TABLEEVT(evt, id, fn) \
    wx__DECLARE_EVT1(wxEVT_COMMAND_TABLE_ ## evt, id, wxTableEventHandler(fn))



#define EVT_TABLE_COLUMN_BEGIN_SIZE(id, fn)              wx__DECLARE_TABLEEVT(COLUMN_BEGIN_SIZE    , id, fn)
#define EVT_TABLE_COLUMN_SIZING(id, fn)                  wx__DECLARE_TABLEEVT(COLUMN_SIZING        , id, fn)
#define EVT_TABLE_COLUMN_SIZED(id, fn)                   wx__DECLARE_TABLEEVT(COLUMN_SIZED         , id, fn)
#define EVT_TABLE_COLUMN_BEGIN_MOVE(id, fn)              wx__DECLARE_TABLEEVT(COLUMN_BEGIN_MOVE    , id, fn)
#define EVT_TABLE_COLUMN_MOVING(id, fn)                  wx__DECLARE_TABLEEVT(COLUMN_MOVING        , id, fn)
#define EVT_TABLE_COLUMN_MOVED(id, fn)                   wx__DECLARE_TABLEEVT(COLUMN_MOVED         , id, fn)
#define EVT_TABLE_HEADER_BEGIN_SIZE(id, fn)              wx__DECLARE_TABLEEVT(HEADER_BEGIN_MOVE    , id, fn)
#define EVT_TABLE_HEADER_SIZING(id, fn)                  wx__DECLARE_TABLEEVT(HEADER_SIZING        , id, fn)
#define EVT_TABLE_HEADER_SIZED(id, fn)                   wx__DECLARE_TABLEEVT(HEADER_SIZED         , id, fn)

#define EVT_TABLE_RECORD_BEGIN_LEFT_DRAG(id, fn)         wx__DECLARE_TABLEEVT(BEGIN_LEFT_DRAG      , id, fn)
#define EVT_TABLE_RECORD_BEGIN_RIGHT_DRAG(id, fn)        wx__DECLARE_TABLEEVT(BEGIN_RIGHT_DRAG     , id, fn)
#define EVT_TABLE_RECORD_BEGIN_MIDDLE_DRAG(id, fn)       wx__DECLARE_TABLEEVT(BEGIN_MIDDLE_DRAG    , id, fn)

#define EVT_TABLE_RECORD_SELECTED(id, fn)                wx__DECLARE_TABLEEVT(RECORD_SELECTED      , id, fn)
#define EVT_TABLE_RECORD_DESELECTED(id, fn)              wx__DECLARE_TABLEEVT(RECORD_DESELECTED    , id, fn)
#define EVT_TABLE_RECORD_ACTIVATED(id, fn)               wx__DECLARE_TABLEEVT(RECORD_ACTIVATED     , id, fn)
#define EVT_TABLE_RECORD_FOCUSED(id, fn)                 wx__DECLARE_TABLEEVT(RECORD_FOCUSED       , id, fn)

#define EVT_TABLE_RECORD_LEFT_CLICK(id, fn)              wx__DECLARE_TABLEEVT(RECORD_LEFT_CLICK    , id, fn)
#define EVT_TABLE_RECORD_RIGHT_CLICK(id, fn)             wx__DECLARE_TABLEEVT(RECORD_RIGHT_CLICK   , id, fn)
#define EVT_TABLE_RECORD_MIDDLE_CLICK(id, fn)            wx__DECLARE_TABLEEVT(RECORD_MIDDLE_CLICK  , id, fn)

#define EVT_TABLE_RECORD_LEFT_DCLICK(id, fn)             wx__DECLARE_TABLEEVT(RECORD_LEFT_DCLICK   , id, fn)
#define EVT_TABLE_RECORD_RIGHT_DCLICK(id, fn)            wx__DECLARE_TABLEEVT(RECORD_RIGHT_DCLICK  , id, fn)
#define EVT_TABLE_RECORD_MIDDLE_DCLICK(id, fn)           wx__DECLARE_TABLEEVT(RECORD_MIDDLE_DCLICK , id, fn)

#define EVT_TABLE_CHECKBOX_CLICKED(id, fn)               wx__DECLARE_TABLEEVT(CHECKBOX_CLICKED     , id, fn)
#define EVT_TABLE_CHECKBOX_CHECKED(id, fn)               wx__DECLARE_TABLEEVT(CHECKBOX_CHECKED     , id, fn)

#define EVT_TABLE_RECORD_ATTRIBUTES(id, fn)              wx__DECLARE_TABLEEVT(RECORD_ATTRIBUTES    , id, fn)

#define EVT_TABLE_KEY_DOWN(id, fn)                       wx__DECLARE_TABLEEVT(KEY_DOWN             , id, fn)
#define EVT_TABLE_KEY_UP(id, fn)                         wx__DECLARE_TABLEEVT(KEY_UP               , id, fn)

#define EVT_TABLE_COLUMN_LEFT_CLICK(id, fn)              wx__DECLARE_TABLEEVT(COLUMN_LEFT_CLICK    , id, fn)
#define EVT_TABLE_COLUMN_RIGHT_CLICK(id, fn)             wx__DECLARE_TABLEEVT(COLUMN_RIGHT_CLICK   , id, fn)
#define EVT_TABLE_COLUMN_MIDDLE_CLICK(id, fn)            wx__DECLARE_TABLEEVT(COLUMN_MIDDLE_CLICK  , id, fn)

#define EVT_TABLE_INSERT(id, fn)                         wx__DECLARE_TABLEEVT(INSERT               , id, fn)
#define EVT_TABLE_UPDATE(id, fn)                         wx__DECLARE_TABLEEVT(UPDATE               , id, fn)
#define EVT_TABLE_DELETE(id, fn)                         wx__DECLARE_TABLEEVT(DELETE               , id, fn)

#define EVT_TABLE_COPY(id, fn)                           wx__DECLARE_TABLEEVT(COPY                 , id, fn)

#define EVT_TABLE_INSERTING(id, fn)                      wx__DECLARE_TABLEEVT(INSERTING            , id, fn)
#define EVT_TABLE_UPDATING(id, fn)                       wx__DECLARE_TABLEEVT(UPDATING             , id, fn)
#define EVT_TABLE_DELETING(id, fn)                       wx__DECLARE_TABLEEVT(DELETING             , id, fn)

#define EVT_TABLE_COPYING(id, fn)                        wx__DECLARE_TABLEEVT(COPYING              , id, fn)

#define EVT_TABLE_INSERTED(id, fn)                       wx__DECLARE_TABLEEVT(INSERTED             , id, fn)
#define EVT_TABLE_UPDATED(id, fn)                        wx__DECLARE_TABLEEVT(UPDATED              , id, fn)
#define EVT_TABLE_DELETED(id, fn)                        wx__DECLARE_TABLEEVT(DELETED              , id, fn)

#define EVT_TABLE_COPIED(id, fn)                         wx__DECLARE_TABLEEVT(COPIED               , id, fn)

#define EVT_TABLE_SORT(id, fn)                           wx__DECLARE_TABLEEVT(SORT                 , id, fn)
#define EVT_TABLE_SORT_ASCENDING(id, fn)                 wx__DECLARE_TABLEEVT(SORT_ASCENDING       , id, fn)
#define EVT_TABLE_SORT_DESCENDING(id, fn)                wx__DECLARE_TABLEEVT(SORT_DESCENDING      , id, fn)

#define EVT_TABLE_FIND(id, fn)                           wx__DECLARE_TABLEEVT(FIND                 , id, fn)
#define EVT_TABLE_SEARCH(id, fn)                         wx__DECLARE_TABLEEVT(SEARCH               , id, fn)

#define EVT_TABLE_DRAW_COLUMN_HEADER(id, fn)             wx__DECLARE_TABLEEVT(DRAW_COLUMN_HEADER   , id, fn)
#define EVT_TABLE_DRAW_COLUMN_DATA(id, fn)               wx__DECLARE_TABLEEVT(DRAW_COLUMN_DATA     , id, fn)



class  wxTableCtrl : public  wxControl
{
   private   :
      typedef wxControl          super;

   public    :
      class  Column
      {
         public    :
            enum  State
            {
               State_UP       = 0x0000,
               State_DOWN     = 0x0001,
               State_MOVE     = 0x0002,
            };



         private   :
//          long                 number;
            wxString             name;             // Internal name (for development).
            wxString             desc;             // External name (for user).
            size_t               reference;

            State                state;

            wxRect               rect;
            int                  alignheader;
            int                  aligncolumn;
            bool                 show;
            bool                 tooltip;
            bool                 resize;
            int                  image;

         public    :
            Column   ();
            Column   ( const Column & );
            Column   ( const wxString &, const wxString &, size_t, const wxRect &, int );

            const wxString &     Name           () const                   { return ( name );      }
            const wxString &     Desc           () const                   { return ( desc );      }
            const size_t         Reference      () const                   { return ( reference ); }
            
            void                 SetState       ( const State & );
            const State          GetState       () const;

            const wxRect &       Rect           () const;
            void                 Rect           ( const wxRect & );
            int                  AlignHeader    () const;
            void                 AlignHeader    ( int );
            int                  AlignColumn    () const;
            void                 AlignColumn    ( int );
            void                 Show           ( bool );
            const bool           Show           () const                   { return ( show );      }
            void                 ToolTip        ( bool );
            const bool           ToolTip        () const                   { return ( tooltip );   }
            void                 Resize         ( bool );
            const bool           Resize         () const                   { return ( resize );    }
            void                 Image          ( int );
            const int            Image          () const                   { return ( image );     }
            
            

            void                 Move           ( const wxPoint & );
            void                 Size           ( const wxSize & );
            void                 Height         ( long );
            void                 Width          ( long );
      };



      class  Header : public  std :: list < Column * >
      {
         private   :
            typedef std :: list < Column * >
                                 super;

         private   :
            wxTable *            table;
            long                 height;

         private   :
         public    :
            typedef iterator     Iterator;

         public    :
            Header            ();
            virtual  ~Header  ();

            void                 FreeContents   ();
            long                 Init           ( wxTable *, long = 0 );

            Column *             PointIn        ( const wxPoint & ) const;
            Column *             PointSize      ( const wxPoint &, int ) const;

            Column *             Find           ( long, bool ) const;
            Column *             Find           ( size_t ) const;

            void                 SortByPosition ();
            void                 Exchange       ( Column *, Column *, bool );
            void                 RedoSize       ();

            long                 Width          ();
            const long           Height         () const                         { return ( height ); }
            void                 Height         ( long );
            
            Column *             Add            ( const wxString &, const wxString &, size_t, const wxRect &, int );
      };



   private   :
      class  ToolTip : public  wxString
      {
         private   :
            wxRect               rect;
            
         public    :
            ToolTip  ( const wxString &, const wxRect & );   
            
            const bool           Contains ( const wxPoint & );
      };
      
      
      
      class  ToolTipVector : private  std :: vector < ToolTip >
      {
         public    :
            void                 Clear    ();
            void                 Add      ( const ToolTip & );
            size_t               IndexOf  ( const wxPoint & );
            const wxString &     String   ( size_t ) const;
      };



   public    :
      class  HeaderCtrl;
      
      
      
      class  Body : public  wxWindow
      {
         private   :
            typedef wxWindow         super;

         public    :
            enum  Mode
            {
               Mode_FAST      =  0,
               Mode_MERGE     =  1,
               Mode_ROLL      =  2,
               Mode_SQUASH    =  3,
               
               Mode_DEFAULT   = Mode_MERGE,
            };



            enum  Fill
            {
               Fill_RESET  =  0,                // Repaint with visible cursor (if valid).
               Fill_NORMAL =  1,                // Normal repaint, cursor could be visible or not.
               Fill_SORT   =  2,                // Repaint based on cursor (if valid and visible)
            };                                  // or first visible row.



            class  DataObject : public  wxTextDataObject
            {
               private   :
                  wxTable *            table;
                  wxTable :: Record *  record;
                  wxTable :: Cursor *  cursor;

               private   :
                  void                 LoadData             ();

               public    :
                  DataObject  ();
                  ~DataObject ();

                  void                 SetData              ( wxTable *, wxTable :: Cursor * = 0 );
            };



         public    :
            static const long          DEFAULTSTYLE;
            
         public    :
            static const size_t        nrow;
            
         public    :                            // wxWindow
            void                       Refresh              ( bool = true, const wxRect * = 0 );

         private   :
            wxTableCtrl *              control;                      // The parent window!

            wxTable *                  table;
            wxTable :: Record *        record;
            wxTable :: Cursor *        cursor;
            wxTable :: CursorVector    visible;
            Header *                   header;
            Mode                       mode;
            wxFont *                   font;

            int                        text_top;
            int                        text_bottom;
            int                        text_left;
            int                        text_right;

            wxRect                     rect;                         // For GetClientRect.
            wxPoint                    down;                         // Cursor position.
            wxPoint                    move;                         // Maximum the cursor moved from 'point'.
            size_t                     text_height;                  // Height of 1 line.
            size_t                     rows;                         // rect.H () / text_height;
            size_t                     cursor_row;                   // Row for 'cursor'.
            bool                       focus;
            wxTable :: Cursor *        begin;                        // For range selection VK_SHIFT+MK_LBUTTON.
            wxTable :: Cursor *        end;                          //

            wxToolTip *                tooltip;
            wxPoint                    tooltippoint;

            wxColor                    foregroundcolor;
            wxColor                    backgroundcolor;

            wxDropSource *             dropsource;
            DataObject *               dataobject;

//          wxBitmap *                 checkbox_unchecked;
//          wxBitmap *                 checkbox_checked;

            bool                       lock;
            Fill                       fill;

         private   :
            void                       DoSize               ( bool );
            void                       DoPaintDC            ( wxDC *, size_t = nrow, const Fill & = Fill_NORMAL );
            void                       DoPaintLineDC        ( wxDC *, size_t, int = -1 );
            void                       DoPaint              ();
            void                       DoPaintLine          ( size_t );
            void                       DoFocusRect          ( wxDC *, int );
            
            void                       DrawCheckBox         ( wxDC *, const wxRect &, bool );

            void                       SyncScrollBar        ();

            bool                       HorzScroll           ( long );

            void                       SelectRangeStart     ();
            void                       SelectRangeStop      ();
            void                       SelectRange          ( const wxTable :: Cursor *, const wxTable :: Cursor *, const bool = true );
            void                       SelectRangeExt       ( const wxTable :: Cursor * );

            wxTable :: Cursor *        GetCursorAtPoint     ( const wxPoint & );
            const bool                 IsFocus              ( const wxTable :: Cursor * ) const;

            bool                       _DoSearch            ( const wxString &, bool = true , bool = true, const bool & = false );
            
            bool                       _CursorTo            ( long, bool );

         protected :
            friend class               wxTableCtrl;

         private   :
      //    DECLARE_CLASS        ( Body )
            DECLARE_EVENT_TABLE  ()

            void                       OnKillFocus          ( wxFocusEvent & );
            void                       OnEraseBackground    ( wxEraseEvent & );
            void                       OnPaint              ( wxPaintEvent & );
            void                       OnSetFocus           ( wxFocusEvent & );
            void                       OnSize               ( wxSizeEvent & );

            void                       OnChar               ( wxKeyEvent & );

            void                       OnScroll             ( wxScrollWinEvent & );
            void                       OnHScroll            ( const wxScrollWinEvent & );
            void                       OnVScroll            ( const wxScrollWinEvent & );
            void                       OnKeyDown            ( wxKeyEvent & );
            void                       OnKeyUp              ( wxKeyEvent & );

            void                       OnLeftDClick         ( wxMouseEvent & );
            void                       OnLeftDown           ( wxMouseEvent & );
            void                       OnLeftUp             ( wxMouseEvent & );
            void                       OnMotion             ( wxMouseEvent & );
            void                       OnMouseWheel         ( wxMouseEvent & );
            void                       OnRightDown          ( wxMouseEvent & );
            void                       OnRightUp            ( wxMouseEvent & );

      //    UINT                       OnGetDlgCode         ();

            void                       OnCommandInsert      ();
            void                       OnCommandUpdate      ();
            void                       OnCommandDelete      ();
            void                       OnCommandCopy        ();
            void                       OnCommandFind        ();
            void                       OnCommandSearch      ();

            void                       OnProperty           ();

            void                       OnReset              ();

            void                       OnVisualSQL          ();

            void                       OnCommandExchange    ( wxCommandEvent & );
            void                       OnCommandSort        ( UINT );
            void                       OnCommandSubTable    ( UINT );
            void                       OnNoSort             ();


         private   :
            bool                       KeyCursor            ( const wxKeyEvent & );
            
            bool                       ProcessEvent         ( const wxTableEvent & );
            bool                       ProcessEvent         ( wxTableEvent & );
            bool                       RecordFocused        ( const wxPoint & );

            void                       CursorCurrent        ();
            void                       RelationSync         ();

            void                       UpdateToolTip        ( const wxPoint & );
            
         public    :
            Body           ( wxWindow *, wxWindowID, const wxPoint & = wxDefaultPosition, const wxSize & = wxDefaultSize, long = DEFAULTSTYLE, const wxString & = wxEmptyString, Header * = 0 );
            virtual  ~Body ();

            void                       Exchange             ( const wxRect &, const wxRect & );

            void                       Table                ( wxTable * );
            wxTable *                  Table                () const;
            
            const Mode &               GetMode              () const                { return ( mode ); }
            void                       SetMode              ( const Mode &  _mode ) { mode   = _mode;  }

            inline const wxColor &     GetForegroundColor   () const                { return ( foregroundcolor ); }
            void                       SetForegroundColor   ( const wxColor & );
            inline const wxColor &     GetBackgroundColor   () const                { return ( backgroundcolor ); }
            void                       SetBackgroundColor   ( const wxColor & );

            const bool                 GetToolTip           () const;
            void                       SetToolTip           ( const bool & );
            
            bool                       Lock                 () const;
            void                       Lock                 ( bool );

            void                       SetFill              ( const Fill & );

            void                       DoPaintSort          ();
            void                       DoPaintFill          ();

            bool                       Get                  ( const wxTable :: Record :: Find & );
            bool                       Get                  ( wxTable :: Cursor * );
            bool                       GetFst               ();
            bool                       GetLst               ();
            
            bool                       Reset                ();

            bool                       InsertRecord         ();
            bool                       UpdateRecord         ();
            bool                       DeleteRecord         ();

            bool                       CopyRecord           ();
            
            bool                       Find                 ( const bool & = false );
            bool                       Search               ( const wxString &, const bool & = true , const bool & = true, const bool & = false, const bool & = false );
            
            long                       HitTest              ( const wxPoint & ) const;
            bool                       CursorTo             ( long );
            
            void                       Refresh              ( const wxTable :: Cursor & );
      };




   private   :
      class  Foot;
      class  Property;
      class  SearchDialog;

      friend class               Body;
      friend class               Property;
      friend class               SearchDialog;
      friend class               wxTableEvent;
      friend class               HeaderCtrl;



   private   :
      enum  DrawHeaderFlag
      {
         DrawHeaderFlag_INIT        = 0,
         DrawHeaderFlag_HIDE        = 1,
         DrawHeaderFlag_SHOW        = 2,
      };

      enum  CursorType
      {
         CursorType_ARROW           = 0,
         CursorType_SIZE_LEFT_RIGHT = 1,
         CursorType_SIZE_TOP_BOTTOM = 2,
      };

   public    :
      enum  TableStyle
      {
         TableStyle_DEFAULT         = 0,
         TableStyle_MICROSOFT       = 1,
         TableStyle_NETSCAPE        = 2,
         TableStyle_JAK             = 3,
      };

      enum  Storage
      {
         Storage_WINDOWPATH         = 0,
         Storage_GLOBAL             = 1,
      };



   public    :
      static const long          DEFAULTSTYLE;
      
   public    :                            // wxEvtHandler
      bool                       ProcessEvent         ( wxEvent &  event )    { return ( super :: ProcessEvent ( event ) ); }
      
   public    :                            // wxWindow
      void                       Refresh              ( bool = true, const wxRect * = 0 );

   private   :
      wxTable *                  table;
      wxTable :: Record *        record;
      wxRect                     head;
      Header                     header;
      HeaderCtrl *               nativeheader;

      Column *                   column;
      Column *                   columnmove;
      Column *                   columnfind;
      wxFont *                   font_header;      // Font used int the table header.
      wxPoint                    start;            // Start position of cursor for move/size.
      wxRect                     rect;             // Original 'rect' before move/size.
      int                        left;             // For horizontal scrolling.
      int                        min_height;       // Minimal height of header.

      Body *                     body;
//    iScroller *                scroller;
      wxFont *                   font;
      const wxPen                gray;
      const wxPen                white;
      const wxPen                black;
      CursorType                 cursortype;

      wxCursor                   cursorarrow;
//    wxCursor                   cursorsize;
      wxCursor                   cursorsizelr;     // Left-Right.
      wxCursor                   cursorsizetb;     // Top-Bottom.

      DWORD                      styleex;

      int                        index;
      wxTable :: Record :: Sequence     
                                 sequence;

      wxTimer *                  scroll_timer;     // Horizontal autoscroll.
      wxPoint                    scroll_point;

      wxToolTip *                tooltip;
      size_t                     tooltipindex;
      ToolTipVector              tooltipvector;
      
      bool                       drawevents;

      wxImageList *              imagelist;
      bool                       imagelistowned;
      wxImageList *              headerimagelist;
      bool                       headerimagelistowned;
//    iWindowSet *               relation;

   private   :
      void                       DoSize               ();

      void                       DrawBackground       ( wxDC * );
      
      void                       DrawFocusRect        ( wxDC *, const bool & );
      void                       DrawFocusRect        ( bool );

      void                       DrawSortArrow        ( wxDC *, const Column :: State &, const wxTable :: Record :: Sequence &, const wxRect &, const bool & );
      void                       DrawColumn           ( wxDC *, const DrawHeaderFlag &, const Column *, wxFont * = 0 );
      void                       DrawHeader           ( wxDC * );
      void                       DrawHeader           ();
      void                       MoveColumn           ( Column *, const wxPoint & );
      void                       SizeColumn           ( Column *, const wxPoint & );
      
      void                       ColumnMoved          ( Column * );
      void                       ColumnSized          ( Column * );
      
      void                       SortColumn           ( Column * );
      
      Column *                   FindColumn           ( const wxPoint & );

      void                       DrawBody             ();

      void                       AutoHScrollStart     ( const wxPoint & );
      void                       AutoHScrollStop      ();

      bool                       ProcessEvent         ( const wxTableEvent & );
      bool                       ProcessEvent         ( wxTableEvent & );
      
      void                       UpdateToolTip        ( const wxPoint & );
      
   protected :
      DECLARE_DYNAMIC_CLASS   ( wxTableCtrl )
//    DECLARE_NO_COPY_CLASS   ( wxTableCtrl )
      DECLARE_EVENT_TABLE     ()

   protected :
//    int                        OnCreate             ( LPCREATESTRUCT );
//    void                       OnDestroy            ();
      void                       OnEraseBackground    ( wxEraseEvent & );
      void                       OnPaint              ( wxPaintEvent & );
      void                       OnSetFocus           ( wxFocusEvent & );
      void                       OnSize               ( wxSizeEvent & );

//    void                       OnNcDestroy          ();
//    void                       OnChar               ( UINT, UINT, UINT );
      void                       OnKeyDown            ( wxKeyEvent & );

      void                       OnLeftDClick         ( wxMouseEvent & );
      void                       OnLeftDown           ( wxMouseEvent & );
      void                       OnLeftUp             ( wxMouseEvent & );
      void                       OnMotion             ( wxMouseEvent & );
      void                       OnRightUp            ( wxMouseEvent & );

      void                       OnTimer              ( wxTimerEvent & );

//    void                       OnDrawItem           ( int, LPDRAWITEMSTRUCT );

//    UINT                       OnGetDlgCode         ();

      /////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////

   public    :
      void                       OnCommandSort        ( UINT );
      void                       OnCommandSubTable    ( UINT );
      void                       OnNoSort             ();

   protected :
//    void                       OnProperty           ();
//    void                       OnReset              ();
//    void                       OnVisualSQL          ();

//    void                       OnCommandCheck       ( UINT );

      wxRect                     GetClientRectEx      () const;

      void                       CreateHeader         ();
      void                       DeleteHeader         ();
      void                       CreateBody           ();
      void                       Init                 ();

   public    :
      wxTableCtrl    ();
      wxTableCtrl    ( wxWindow *, wxWindowID = wxID_ANY, const wxPoint & = wxDefaultPosition, const wxSize & = wxDefaultSize, long = DEFAULTSTYLE, const wxString & = wxEmptyString );
      ~wxTableCtrl   ();

      bool                       Create               ( wxWindow *, wxWindowID = wxID_ANY, const wxPoint & = wxDefaultPosition, const wxSize & = wxDefaultSize, long = DEFAULTSTYLE, const wxString & = wxEmptyString );
      
      Header &                   GetHeader            ()                      { return ( header ); }
      void                       LoadHeader           ();
      
      const int                  GetLeft              () const                { return ( left ); }
      void                       SetLeft              ( const int &  _left )  { left   = _left; }

      const DWORD                GetStyleEx2          () const                { return ( styleex ); }
      bool                       ModifyStyleEx2       ( DWORD, DWORD );

      const int                  GetIndex             () const                { return ( index ); }

//    const iWindowSet *         GetRelation          () const                { return ( relation ); }

      bool                       MultiSelect          () const;
      wxTableCtrl &              MultiSelect          ( bool );

      bool                       DragAndDrop          () const;
      wxTableCtrl &              DragAndDrop          ( bool );

      bool                       GridVertical         () const;
      wxTableCtrl &              GridVertical         ( bool );

      bool                       GridHorizontal       () const;
      wxTableCtrl &              GridHorizontal       ( bool );

      bool                       GridLines            () const;
      wxTableCtrl &              GridLines            ( bool );

      bool                       CheckBoxes           () const;
      wxTableCtrl &              CheckBoxes           ( bool );

      bool                       FocusRect            () const;
      wxTableCtrl &              FocusRect            ( bool );

      void                       Refresh              ( const DWORD & );
      void                       Refresh              ( const wxTable :: Cursor & );

      void                       Table                ( wxTable * );
      const wxTable *            Table                () const;
      wxTable *                  Table                ();
      
      const Body :: Mode         GetMode              () const;
      void                       SetMode              ( const Body :: Mode & );

      const wxColor &            GetForegroundColor   () const;
      wxTableCtrl &              SetForegroundColor   ( const wxColor & );
      const wxColor &            GetBackgroundColor   () const;
      wxTableCtrl &              SetBackgroundColor   ( const wxColor & );
      
      const bool                 GetToolTip           () const;
      wxTableCtrl &              SetToolTip           ( const bool & );
      
      const bool                 GetDrawEvents        () const;
      wxTableCtrl &              SetDrawEvents        ( const bool & );
      
      const bool                 GetNativeHeader      () const;
      wxTableCtrl &              SetNativeHeader      ( bool );

      bool                       Lock                 () const;
      void                       Lock                 ( bool );

      bool                       CursorSet            ();
      wxTable :: Action          Action4Record        ();
      wxTable :: Action          Action4Table         ();

      void                       Index                ( int );
      const int                  Index                () const;
      
      void                       Sequence             ( const wxTable :: Record :: Sequence & );
      const wxTable :: Record :: Sequence &
                                 Sequence             () const;

      bool                       Get                  ( const wxTable :: Record :: Find & );
      bool                       Get                  ( wxTable :: Cursor * );
      bool                       GetFst               ();
      bool                       GetLst               ();
      
      bool                       Reset                ();
      
      bool                       InsertRecord         ();
      bool                       UpdateRecord         ();
      bool                       DeleteRecord         ();

      bool                       CopyRecord           ();

      bool                       Find                 ( bool = false );
      bool                       Search               ( const wxString &, const bool & = true , const bool & = true, const bool & = false, const bool & = false );
      
      long                       HitTest              ( const wxPoint & ) const;
      bool                       CursorTo             ( long );
      
      void                       SubTable             ( int );

      void                       StartEvent           ( const wxEventType &, int );
      
      void                       LeftDClick           ( wxMouseEvent & );

      void                       CommandInsert        ();
      void                       CommandUpdate        ();
      void                       CommandDelete        ();

      void                       CommandCopy          ();

      void                       CommandFind          ();

      void                       CommandSearch        ();
      
      void                       SetImageList         ( wxImageList *, bool = false );
      void                       SetHeaderImageList   ( wxImageList *, bool = false );
      
#if 0
      iMenu *                    CreateSortMenu       ( UINT );
      iMenu *                    CreateSubTableMenu   ();

      iWindowSet *               Relation             ();
      bool                       RelationInsert       ( wxTableCtrl * );
      bool                       RelationDelete       ( wxTableCtrl * );
#endif
      void                       RefreshLayout        ();
};



class  wxTableEvent : public  wxNotifyEvent 
{
   private   :
      typedef wxNotifyEvent   
                        super;

   public    :
      wxTableEvent *    Clone             () const;

   private   :
      wxRect            rect;
      wxDC *            dc;
      const wxTableCtrl :: Column *  
                        column;
      wxTable *         table;
      wxTable :: Record *
                        record;
      const wxTable :: Cursor *
                        cursor;

      bool              focus;                        
      wxColor           background;
      wxColor           foreground;
      wxFont            font;
      
      wxKeyEvent        keyevent;

   private   :
      void              Init              ();

   public    :
      wxTableEvent   ();
      wxTableEvent   ( const wxTableEvent & );
      wxTableEvent   ( wxTableCtrl *, const wxEventType &, const wxTableCtrl :: Column * = 0, const wxPoint & = wxDefaultPosition, wxTable * = 0, wxTable :: Record * = 0, const wxTable :: Cursor * = 0, const bool & = false );
      wxTableEvent   ( wxTableCtrl *, const wxEventType &, const wxKeyEvent &, const wxPoint &, wxTable *, wxTable :: Record *, const wxTable :: Cursor *, const bool & = false );
      wxTableEvent   ( wxTableCtrl *, const wxEventType &, wxDC *, const wxTableCtrl :: Column *, const wxRect &, wxTable * = 0, wxTable :: Record * = 0, const wxTable :: Cursor * = 0, const bool & = false );
      ~wxTableEvent  ();

      wxTableCtrl *     GetControl        () const;
      
      const wxRect &    GetRect           () const                               { return ( rect ); }
      void              SetRect           ( const wxRect & );
      void              SetPoint          ( const wxPoint & );
      
      wxDC *            GetDC             () const                               { return ( dc ); }
      
      const wxTableCtrl :: Column *      
                        GetColumn         () const                               { return ( column ); }
      void              SetColumn         ( const wxTableCtrl :: Column * );
                        
      wxTable *         GetTable          () const;
      wxTable :: Record *    
                        GetRecord         () const;
      const wxTable :: Cursor *
                        GetCursor         () const;
                        
      const bool &      GetFocus          () const                               { return ( focus ); }
      
      const wxColor &   GetTextBackground () const                               { return ( background ); }
      void              SetTextBackground ( const wxColor & );
      const wxColor &   GetTextForeground () const                               { return ( foreground ); }
      void              SetTextForeground ( const wxColor & );
      wxFont            GetFont           () const                               { return ( font ); }
      void              SetFont           ( const wxFont & );
      
      const wxKeyEvent &   
                        GetKeyEvent       () const                               { return ( keyevent ); }
};



#endif
