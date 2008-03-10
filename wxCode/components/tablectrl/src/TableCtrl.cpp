////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        TableCtrl.cpp                                                 //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     1998                                                          //
// Copyright:   (c) 1998-2008 Jan Knepper                                     //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#include <wx/wxprec.h>

#include <wx/renderer.h>

#include <algorithm>

#include "wx/BitmapDC.hpp"
#include "wx/TableCtrl.hpp"



////////////////////////////////////////////////////////////////////////////////
// WXW type Win32 API functions. These will have to be developed for X.       //
////////////////////////////////////////////////////////////////////////////////



#ifdef __WXMSW__



int  DrawText ( wxDC *  dc, LPCTSTR  str, int  length, wxRect &  rect, UINT  format )
{
   HDC         hdc   = ( HDC ) dc -> GetHDC ();
   RECT        temp;

   SetRect ( &temp, rect.GetX (), rect.GetY (), rect.GetX () + rect.GetWidth (), rect.GetY () + rect.GetHeight () );

   COLORREF    pb    = :: SetBkColor   ( hdc, dc -> GetTextBackground ().GetPixel () );
   COLORREF    pf    = :: SetTextColor ( hdc, dc -> GetTextForeground ().GetPixel () );
   int         h     = :: DrawText     ( hdc, str, length, &temp, format );

   if ( format & DT_CALCRECT )
      rect  = wxRect ( temp.left, temp.top, temp.right - temp.left, temp.bottom - temp.top );

   :: SetTextColor ( hdc, pf );
   :: SetBkColor   ( hdc, pb );

   return ( h );
}



bool  DrawFocusRect ( wxDC *  dc, const wxRect &  rect )
{
   RECT     temp;

   :: SetRect        ( &temp, rect.GetX (), rect.GetY (), rect.GetX () + rect.GetWidth (), rect.GetY () + rect.GetHeight () );

   return ( :: DrawFocusRect  ( ( HDC ) dc -> GetHDC (), &temp ) != FALSE );
}



void  FillSolidRect ( wxDC *  dc, const wxRect &  rect, const wxColour &  color )
{
   HDC         hdc   = ( HDC ) dc -> GetHDC ();
   COLORREF    clr   = :: GetBkColor ( hdc );
   RECT        temp;

   SetRect ( &temp, rect.GetX (), rect.GetY (), rect.GetX () + rect.GetWidth (), rect.GetY () + rect.GetHeight () );

   :: SetBkColor ( hdc, RGB ( color.Red (), color.Green (), color.Blue () ) );
   :: ExtTextOut ( hdc, 0, 0, ETO_OPAQUE, &temp, 0, 0, 0 );
   :: SetBkColor ( hdc, clr );
}


#if 0
bool  DrawState ( wxDC *  dc, const wxPoint &  point, const wxSize &  size, wxBitmap *  bitmap, UINT  flag )
{
   wxUxThemeEngine *    te = wxUxThemeEngine :: GetIfActive ();
   
   if ( te != 0 )
   {
      wxUxThemeHandle   t  ( dc -> GetWindow (), L"BUTTON" );
      RECT              r;
      
      :: SetRect ( &r, point.x, point.y, point.x + size.GetWidth  (), point.y + size.GetHeight () );
      
      te -> DrawThemeBackground ( t, static_cast < HDC > ( dc -> GetHDC () ), BP_CHECKBOX, CBS_UNCHECKEDNORMAL, &r, 0 );
   }
   else
   {
      HDC         hdc   = ( HDC ) dc -> GetHDC ();

      return ( :: DrawState ( hdc, 0, 0, reinterpret_cast < LPARAM > ( bitmap -> GetHBITMAP () ), 0, point.x, point.y, size.GetWidth (), size.GetHeight (), flag ) != FALSE );
   }
}
#endif


wxString  ResourceNameString ( const wxString &  name )
{
#ifdef _MSC_VER

   return ( wxT ( "\"" ) + name + wxT ( "\"" ) );

#else

   return ( name );

#endif
}



#else



int  DrawText ( wxDC *  dc, LPCTSTR  str, int  length, wxRect &  rect, UINT  format )
{
   if ( str == 0 )
      return (  0 );

// dc -> DrawText ( wxString ( str, length ), rect.X (), rect.Y () );
   dc -> DrawText ( str, rect.X (), rect.Y () );

   return (  0 );
}



#endif



DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_COLUMN_BEGIN_SIZE      )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_COLUMN_SIZING          )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_COLUMN_SIZED           )

DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_COLUMN_BEGIN_MOVE      )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_COLUMN_MOVING          )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_COLUMN_MOVED           )

DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_HEADER_BEGIN_SIZE      )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_HEADER_SIZING          )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_HEADER_SIZED           )

//DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_HEADER_LEFT_CLICK      )
//DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_HEADER_RIGHT_CLICK     )
//DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_HEADER_MIDDLE_CLICK    )

//DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_HEADER_LEFT_DCLICK     )
//DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_HEADER_RIGHT_DCLICK    )
//DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_HEADER_MIDDLE_DCLICK   )

DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_BEGIN_LEFT_DRAG        )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_BEGIN_RIGHT_DRAG       )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_BEGIN_MIDDLE_DRAG      )

DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_RECORD_SELECTED        )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_RECORD_DESELECTED      )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_RECORD_ACTIVATED       )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_RECORD_FOCUSED         )

DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_RECORD_LEFT_CLICK      )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_RECORD_RIGHT_CLICK     )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_RECORD_MIDDLE_CLICK    )

DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_RECORD_LEFT_DCLICK     )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_RECORD_RIGHT_DCLICK    )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_RECORD_MIDDLE_DCLICK   )

DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_CHECKBOX_CLICKED       )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_CHECKBOX_CHECKED       )

DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_RECORD_ATTRIBUTES      )

DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_KEY_DOWN               )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_KEY_UP                 )

DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_COLUMN_LEFT_CLICK      )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_COLUMN_RIGHT_CLICK     )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_COLUMN_MIDDLE_CLICK    )

DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_INSERT                 )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_UPDATE                 )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_DELETE                 )

DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_COPY                   )

DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_INSERTING              )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_UPDATING               )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_DELETING               )

DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_COPYING                )

DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_INSERTED               )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_UPDATED                )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_DELETED                )

DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_COPIED                 )

DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_SORT                   )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_SORT_ASCENDING         )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_SORT_DESCENDING        )

DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_FIND                   )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_SEARCH                 )

DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_DRAW_COLUMN_HEADER     )
DEFINE_EVENT_TYPE ( wxEVT_COMMAND_TABLE_DRAW_COLUMN_DATA       )



const int               FOCUSRECTTHICK =   1;
const int               CHECKBOXWIDTH  =  16;



////////////////////////////////////////////////////////////////////////////////
// wxNativeHeaderCtrl                                                         //
////////////////////////////////////////////////////////////////////////////////



class  wxNativeHeaderCtrl : public  wxControl
{
   private   :
      typedef wxControl    super;
      
   protected :
      wxSize               DoGetBestSize     () const;
      void                 DoSetSize         ( int, int, int, int, int );
      
   public    :                            // wxWindowMSW
      bool                 MSWOnNotify       ( int, WXLPARAM, WXLPARAM * );
   
   private   :
      int                  height;
      
   public    :
      wxNativeHeaderCtrl   ( wxWindow *, wxWindowID, const wxPoint & = wxDefaultPosition, const wxSize & = wxDefaultSize, const wxString & = wxT ( "wxNativeHeaderCtrl" ) );
      
      bool                 Create            ( wxWindow *, wxWindowID, const wxPoint &, const wxSize &, const wxString & );

   public    :
      void *               GetParam          ( NMHEADER * ) const;
      void *               GetParam          ( wxUint32 ) const;
      void *               GetParamByOrder   ( wxInt32 ) const;
      wxInt32              GetOrder          ( wxUint32 ) const;
};



wxSize  wxNativeHeaderCtrl :: DoGetBestSize () const
{
   return ( wxSize ( DEFAULT_ITEM_WIDTH, height + 2 ) );
}



void  wxNativeHeaderCtrl :: DoSetSize ( int  x, int  y, int  w, int  h, int  flags )
{
   super :: DoSetSize ( x, y, w, h, flags );
}



bool  wxNativeHeaderCtrl :: MSWOnNotify ( int  id, WXLPARAM  lparam, WXLPARAM *  result )
{
   if ( ! super :: MSWOnNotify ( id, lparam, result ) )
      if ( GetParent () != 0 )
         return ( GetParent () -> MSWOnNotify ( id, lparam, result ) );

   return ( true );
}



wxNativeHeaderCtrl :: wxNativeHeaderCtrl ( wxWindow *  _window, wxWindowID  _id, const wxPoint &  _point, const wxSize &  _size, const wxString &  _title )
   : wxControl ()
{
   Create ( _window, _id, _point, _size, _title );
}


   
bool  wxNativeHeaderCtrl :: Create ( wxWindow *  _window, wxWindowID  _id, const wxPoint &  _point, const wxSize &  _size, const wxString &  _title )
{
   height   = wxRendererNative :: Get ().GetHeaderButtonHeight ( this );

   if ( ! CreateControl ( _window, _id, _point, _size, wxBORDER_NONE, wxDefaultValidator, _title ) )
      return ( false );
      
   if ( ! MSWCreateControl ( WC_HEADER, WS_CHILD | HDS_DRAGDROP | HDS_BUTTONS | HDS_HORZ | HDS_HOTTRACK | HDS_FULLDRAG, _point, _size, _title ) )
      return ( false );
      
   return ( true );   
}



void *  wxNativeHeaderCtrl :: GetParam ( NMHEADER *  header ) const
{
   if ( header == 0 )
      return (  0 );
      
   if ( header -> pitem == 0 )
      return (  0 );
      
   if ( ( header -> pitem -> mask & HDI_LPARAM ) != 0 )
      return ( reinterpret_cast < void * > ( header -> pitem -> lParam ) );
      
   return ( GetParam ( header -> iItem ) );
}



void *  wxNativeHeaderCtrl :: GetParam ( wxUint32  index ) const
{
   HDITEM   item;
   
   item.mask   = HDI_LPARAM;
   
   Header_GetItem ( static_cast < HWND > ( m_hWnd ), index, &item );
   
   return ( reinterpret_cast < void * > ( item.lParam ) );
}



void *  wxNativeHeaderCtrl :: GetParamByOrder ( wxInt32  order ) const
{
   HWND           hwnd  = static_cast < HWND > ( m_hWnd );
   const size_t   ITEMS = Header_GetItemCount ( hwnd );
   HDITEM         item;
   
   for ( size_t  i = 0 ; i < ITEMS ; ++i )
   {
      item.mask   = HDI_LPARAM | HDI_ORDER;
      
      if ( ( Header_GetItem ( hwnd, i, &item ) != FALSE ) &&
           ( item.iOrder                       == order )    )
         return ( reinterpret_cast < void * > ( item.lParam ) );
   }
   
   return (  0 );
}



wxInt32  wxNativeHeaderCtrl :: GetOrder ( wxUint32  index ) const
{
   HDITEM   item;
   
   item.mask   = HDI_ORDER;
   
   Header_GetItem ( static_cast < HWND > ( m_hWnd ), index, &item );
   
   return ( item.iOrder );
}



////////////////////////////////////////////////////////////////////////////////
// wxTableCtrl :: HeaderCtrl                                                  //
////////////////////////////////////////////////////////////////////////////////



class  wxTableCtrl :: HeaderCtrl : public  wxControl
{
   private   :
      typedef wxControl    super;
      
   private   :
      DECLARE_EVENT_TABLE  ()
      
   public    :
      void                 Refresh           ( bool = true, const wxRect * = 0 );
      
   protected :
      wxSize               DoGetBestSize     () const;
//    void                 DoSetSize         ( int, int, int, int, int );
//    
   public    :                            // wxWindowMSW
      bool                 MSWOnNotify       ( int, WXLPARAM, WXLPARAM * );
   
   private   :
      wxTableCtrl *        control;
      wxNativeHeaderCtrl * header;
   
   private   :
      void                 OnSize            ( wxSizeEvent & );
         
   private   :
      Column *             GetColumn         ( NMHEADER * ) const;
      Column *             GetColumn         ( wxUint32 ) const;
      Column *             GetColumnByOrder  ( wxInt32 ) const;
      
   public    :
      HeaderCtrl  ( wxTableCtrl *, wxWindowID, const wxPoint & = wxDefaultPosition, const wxSize & = wxDefaultSize, const wxString & = wxT ( "wxTableCtrl :: HeaderCtrl" ) );
      
      bool                 Create            ( wxTableCtrl *, wxWindowID, const wxPoint &, const wxSize &, const wxString & );
      
      void                 Load              ( Header * );
      void                 Left              ( int );
};



BEGIN_EVENT_TABLE ( wxTableCtrl :: HeaderCtrl, wxControl )
   EVT_SIZE ( wxTableCtrl :: HeaderCtrl :: OnSize  )
END_EVENT_TABLE   ()



void  wxTableCtrl :: HeaderCtrl :: Refresh ( bool  erase, const wxRect *  rect )
{
   if ( header != 0 )
      header -> Refresh ( erase, rect );
}



wxSize  wxTableCtrl :: HeaderCtrl :: DoGetBestSize () const
{
   if ( header != 0 )
      return ( header -> GetBestSize () );
      
   return ( super :: DoGetBestSize () );
}



bool  wxTableCtrl :: HeaderCtrl :: MSWOnNotify ( int  id, WXLPARAM  lparam, WXLPARAM *  result )
{
   NMHDR *     nmhdr    = reinterpret_cast < NMHDR * > ( lparam );
   HWND        hwnd     = static_cast < HWND > ( header -> GetHWND () );
   
   if ( nmhdr -> hwndFrom != hwnd )
      return ( super :: MSWOnNotify ( id, lparam, result ) );
   
   if ( ( nmhdr -> code < HDN_LAST ) || ( nmhdr -> code > HDN_FIRST ) )
      return ( super :: MSWOnNotify ( id, lparam, result ) );
      
   NMHEADER *  nmheader = reinterpret_cast < NMHEADER * > ( nmhdr );
   HDITEM *    item     = nmheader -> pitem;
   Column *    column   = GetColumn ( nmheader );
   
   switch ( nmhdr -> code )
   {
      case HDN_BEGINTRACK     :
         if ( ( column != 0 ) && ( column -> Resize () ) )
         {
            wxTableEvent   te ( control, wxEVT_COMMAND_TABLE_COLUMN_BEGIN_SIZE, column );

            ProcessEvent   ( te );

            if ( ! te.IsAllowed () )
               *result = TRUE;
         }
         else
            *result = TRUE;
         
         break;
         
      case HDN_ENDTRACK       :
//    case HDN_ITEMCHANGED    :
         if ( ( column != 0 ) && ( ( item -> mask & HDI_WIDTH ) != 0 ) )
         {
            column -> Width ( item -> cxy );
            
            control -> ColumnSized ( column );
         }
         
         break;
      
      case HDN_TRACK          :
      case HDN_ITEMCHANGING   :
         {
            wxTableEvent   te ( control, wxEVT_COMMAND_TABLE_COLUMN_SIZING, column );
            
            ProcessEvent   ( te );
            
            if ( ! te.IsAllowed () )
               *result  = TRUE;
         }
         
         break;
            
      case HDN_BEGINDRAG      :
         {
            wxTableEvent   te ( control, wxEVT_COMMAND_TABLE_COLUMN_BEGIN_MOVE, column );

            ProcessEvent   ( te );

            if ( ! te.IsAllowed () )
               *result = TRUE;
            else
               column -> SetState ( Column :: State_DOWN );
         }
         
         break;
         
      case HDN_ENDDRAG        :
         if ( column != 0 )
         {
            const wxUint32    ORDER    = nmheader -> pitem -> iOrder;
            Column *          help     = GetColumnByOrder ( ORDER );
            bool              before   = ORDER <= header -> GetOrder ( nmheader -> iItem );
            
            column -> SetState ( Column :: State_UP );

            if ( help == 0 )
            {
               help     = GetColumnByOrder ( ORDER - 1 );
               before   = false;
            }
            
            control -> header.Exchange ( column, help, before );
            
            control -> ColumnMoved ( column );
         }
         
         break;
         
      case HDN_ITEMCLICK      :
         {
            static wxEventType    ET   [] =
            {
               wxEVT_COMMAND_TABLE_COLUMN_LEFT_CLICK  ,
               wxEVT_COMMAND_TABLE_COLUMN_RIGHT_CLICK ,
               wxEVT_COMMAND_TABLE_COLUMN_MIDDLE_CLICK,
            };
            
            wxTableEvent   te ( control, ET [ nmheader -> iButton ], column );
            
            ProcessEvent   ( te );
            
            if ( ! te.IsAllowed () )
               *result  = TRUE;
            else
            {   
               if ( column == 0 )
                  column   = GetColumn ( nmheader -> iItem );

               if ( column != 0 )
               {
                  control -> SortColumn ( column );
                  
                  const wxTable :: Record :: Sequence
                              sort  = control -> record -> GetSort ( column -> Reference () );
                              
                  if ( sort != wxTable :: Record :: Sequence_NONE )
                  {
                     HDITEM   item;
                     
                     item.mask   = HDI_FORMAT;
                     
                     Header_GetItem ( hwnd, nmheader -> iItem, &item );
               
                     item.fmt &= ~( HDF_SORTUP | HDF_SORTDOWN );
                     item.fmt |= ( sort == wxTable :: Record :: Sequence_ASCENDING ) ? HDF_SORTDOWN : HDF_SORTUP;
                     
                     Header_SetItem ( hwnd, nmheader -> iItem, &item );
                     
                     const size_t   COLS  = Header_GetItemCount ( hwnd );
                     
                     for ( size_t  i = 0 ; i < COLS ; ++i )
                     {
                        if ( i == static_cast < size_t > ( nmheader -> iItem ) )
                           continue;
                           
                        Header_GetItem ( hwnd, i, &item );
                        item.fmt &= ~( HDF_SORTUP | HDF_SORTDOWN );
                        Header_SetItem ( hwnd, i, &item );
                     }
                  }
               }
            }
         }
                  
         break;
         
      default                 :
         return ( super :: MSWOnNotify ( id, lparam, result ) );
   }
   
   return ( true );
}



void  wxTableCtrl :: HeaderCtrl :: OnSize ( wxSizeEvent &  se )
{
   if ( header != 0 )
      header -> SetSize ( - control -> left, 0, se.GetSize ().GetWidth () + control -> left, se.GetSize ().GetHeight (), wxSIZE_FORCE );
   
   se.Skip  ();
}



wxTableCtrl :: Column *  wxTableCtrl :: HeaderCtrl :: GetColumn ( NMHEADER *  nmh ) const
{
   if ( header != 0 )
      return ( static_cast < Column * > ( header -> GetParam ( nmh ) ) );
   else
      return (  0 );
}



wxTableCtrl :: Column *  wxTableCtrl :: HeaderCtrl :: GetColumn ( wxUint32  index ) const
{
   if ( header != 0 )
      return ( static_cast < Column * > ( header -> GetParam ( index ) ) );
   else
      return (  0 );
}



wxTableCtrl :: Column *  wxTableCtrl :: HeaderCtrl :: GetColumnByOrder ( wxInt32  order ) const
{
   if ( header != 0 )
      return ( static_cast < Column * > ( header -> GetParamByOrder ( order ) ) );
   else
      return (  0 );
}



wxTableCtrl :: HeaderCtrl :: HeaderCtrl ( wxTableCtrl *  _window, wxWindowID  _id, const wxPoint &  _point, const wxSize &  _size, const wxString &  _title )
   : wxControl ()
{
   control  = 0;
   header   = 0;
   
   Create ( _window, _id, _point, _size, _title );
}


   
bool  wxTableCtrl :: HeaderCtrl :: Create ( wxTableCtrl *  _window, wxWindowID  _id, const wxPoint &  _point, const wxSize &  _size, const wxString &  _title )
{
   control  = _window;
   
   if ( ! CreateControl ( _window, _id, _point, _size, wxBORDER_NONE, wxDefaultValidator, _title ) )
      return ( false );
      
   if ( ! MSWCreateControl ( WC_STATIC, WS_CHILD, _point, _size, _title ) )
      return ( false );
      
   header   = new  wxNativeHeaderCtrl  ( this, wxID_ANY );
   
   // Hook the wxNativeHeaderCtrl's event handler to use wxTableCtrl :: HeaderCtrl's event hander as the next.
   
   header   -> SetNextHandler ( this );

   return ( true );   
}



void  wxTableCtrl :: HeaderCtrl :: Load ( Header *  _header )
{
   HWND  hwnd  = static_cast < HWND > ( header -> GetHWND () );
   int   count = 0;
   
   while ( Header_GetItemCount ( hwnd ) > 0 )
      Header_DeleteItem ( hwnd, 0 );

   if ( control -> headerimagelist != 0 )      
      Header_SetImageList ( hwnd, control -> headerimagelist -> GetHIMAGELIST () );
   else
      Header_SetImageList ( hwnd, 0 );

   for ( Header :: iterator  i = _header -> begin () ; i != _header -> end () ; ++i )
   {
      Column *    c     = *i;
      
      if ( ! c -> Show () )
         continue;
         
      HDITEM      item;
      
      ZeroMemory ( &item, sizeof ( HDITEM ) );
      
      item.mask         = HDI_TEXT | HDI_FORMAT | HDI_WIDTH | HDI_LPARAM;
      item.pszText      = const_cast < wxChar * > ( c -> Desc ().c_str () );
      item.cchTextMax   = c -> Desc ().Length ();
      item.cxy          = c -> Rect ().GetWidth ();
      item.fmt          = HDF_LEFT | HDF_STRING;

      if      ( c -> AlignHeader () & DT_CENTER ) 
         item.fmt   |= HDF_CENTER;
      else if ( c -> AlignHeader () & DT_RIGHT  )
         item.fmt   |= HDF_RIGHT;

      // Hook the Column up via lParam.
      
      item.lParam       = reinterpret_cast < LPARAM > ( c );
      
      // Image?
      
      if ( ( item.iImage = c -> Image () ) != -1 )
      {
         item.mask  |= HDI_IMAGE;
         item.fmt   |= HDF_IMAGE;
      }
      
//    if ( control -> drawevents )
//       item.fmt |= HDF_OWNERDRAW;
         
      // Sort.
      
      const wxTable :: Record :: Sequence    
                  sort  = control -> record -> GetSort ( c -> Reference () );
                  
      if ( sort != wxTable :: Record :: Sequence_NONE )
         item.fmt |= ( sort == wxTable :: Record :: Sequence_ASCENDING ) ? HDF_SORTDOWN : HDF_SORTUP;
                           
      if ( Header_InsertItem ( hwnd, count++, &item ) == -1 )
         wxLogDebug ( "Error: %d", :: GetLastError () );
   }
}



void  wxTableCtrl :: HeaderCtrl :: Left ( int  left )
{
   const wxRect   rect  = GetClientRect ();
   
   header -> SetSize ( - left, 0, rect.GetWidth () + left, rect.GetHeight (), wxSIZE_FORCE );
}



////////////////////////////////////////////////////////////////////////////////
// wxTableCtrl :: Column                                                      //
////////////////////////////////////////////////////////////////////////////////



wxTableCtrl :: Column :: Column ()
{
// number      = 0;
   state       = State_UP;
   name        = "";
   desc        = "";
   reference   = 0xFFFFFFFF;

   rect        = wxRect   ( 0, 0, 0, 0 );
   alignheader = 0;
   aligncolumn = 0;
   show        = false;
   tooltip     = false;
   resize      = false;
   image       = -1;
}



wxTableCtrl :: Column :: Column ( const Column &  that )
{
// number      = that.number;
   state       = that.state;
   name        = that.name;
   desc        = that.desc;
   reference   = that.reference;

   rect        = that.rect;
   alignheader = that.alignheader;
   aligncolumn = that.aligncolumn;
   show        = that.show;
   tooltip     = that.tooltip;
   resize      = that.resize;
   image       = that.image;
}



wxTableCtrl :: Column :: Column ( const wxString &  _name, const wxString &  _desc, size_t  _reference, const wxRect &  _rect, int  _alignheader )
{
// number      = 0;
   state       = State_UP;
   name        = _name;
   desc        = _desc;
   reference   = _reference;

   rect        = _rect;
   alignheader = _alignheader;
   aligncolumn = _alignheader;
   show        = false;
   tooltip     = false;
   resize      = false;
   image       = -1;
}


/*
int  wxTableCtrl :: Column :: operator == ( const Column  &that )
{
   return ( rect.X () == that.rect.X () );
}
*/

/*
int  wxTableCtrl :: Column :: operator < ( const Column  &that )
{
   return ( rect.X () <= that.rect.X () );
}
*/


void  wxTableCtrl :: Column :: SetState ( const State &  _state )
{
   state = _state;
}



const wxTableCtrl :: Column :: State  wxTableCtrl :: Column :: GetState () const
{
   return ( state );
}



const wxRect &  wxTableCtrl :: Column :: Rect () const
{
   return ( rect );
}



void  wxTableCtrl :: Column :: Rect ( const wxRect &  _rect )
{
   rect  = _rect;
}



//const iString  &wxTableCtrl :: Column :: Title () const
//{
//   return ( title );
//}



int  wxTableCtrl :: Column :: AlignHeader () const
{
   return ( alignheader );
}



void  wxTableCtrl :: Column :: AlignHeader ( int  _alignheader )
{
   alignheader = _alignheader;
}



int  wxTableCtrl :: Column :: AlignColumn () const
{
   return ( aligncolumn );
}



void  wxTableCtrl :: Column :: AlignColumn ( int  _aligncolumn )
{
   aligncolumn = _aligncolumn;
}



void  wxTableCtrl :: Column :: Show ( bool  _show )
{
   show  = _show;
}



void  wxTableCtrl :: Column :: ToolTip ( bool  _tooltip )
{
   tooltip  = _tooltip;
}



void  wxTableCtrl :: Column :: Resize ( bool  _resize )
{
   resize   = _resize;
}



void  wxTableCtrl :: Column :: Image ( int  _image )
{
   image    = _image;
}



void  wxTableCtrl :: Column :: Move ( const wxPoint &  point )
{
// rect.OffsetRect ( point );
   rect.SetPosition ( point );
}



void  wxTableCtrl :: Column :: Size ( const wxSize &  size )
{
   if ( size.GetWidth () >= 0 )
      rect = size;
}



void  wxTableCtrl :: Column :: Height ( long  height )
{
   if ( height >= 0 )
      rect.SetHeight ( height );
}



void  wxTableCtrl :: Column :: Width ( long  width )
{
   if ( width >= 0 )
      rect.SetWidth ( width );
}



///////////////////////////////////////////////////////////////////////////////
// wxTableCtrl :: Header                                                      //
///////////////////////////////////////////////////////////////////////////////



wxTableCtrl :: Header :: Header ()
   : std :: list < Column * > ()
{
   table    =  0;
   height   = 16;
}



wxTableCtrl :: Header :: ~Header ()
{
   FreeContents ();
}



void  wxTableCtrl :: Header :: FreeContents ()
{
   const iterator    END   = end ();

   for ( iterator   i = begin () ; i != END ; ++i )
      delete  ( *i );

   erase ( begin (), end () );
}



long  wxTableCtrl :: Header :: Init ( wxTable *  _table, long  h )
{
   FreeContents ();

   if ( ( table = _table ) == 0 )
//    return ( Height () );
      return ( h );

   const size_t   numberofcolumns   = table -> NumberOfColumns ();
   int            X                 =   0;

   for ( size_t  i = 0 ; i < numberofcolumns ; ++i )
   {
//    wxTable :: Column *  tc = table -> At ( i );
      
      // Only include in header list when visible!

//    if ( table -> FieldFlag ( i ) & TFF_SHOW )
      {
//?      const wxRect &    r  = table -> HeaderRect ( i );
         const wxRect      r  ( -1, -1, -1, -1 );
         int               w  = r.GetWidth  ();       // Get initial header width.
//       int               h  = r.GetHeight ();       // Get initial header height.
         int               x  = r.GetX      ();

         if ( w <  0 )                                // Use only if valid!
            w = 100;

         if ( x <= 0 )
            x  = X;
            
         Column *    column   = Add  ( table -> FieldName ( i ), table -> HeaderString ( i ), i, wxRect ( x, 0, w, h ), table -> HeaderDrawTextFormat ( i ) );

         if ( table -> FieldFlag ( i ) & TFF_SHOW )
         {         
            column -> Show ( true );
            X += w;
         }
         else
            column -> Show ( false );

         column -> Resize ( table -> FieldFlag ( i ) & TFF_SIZE );            
      }
   }

   SortByPosition ();
   RedoSize       ();
   
   return ( Height () );
}



wxTableCtrl :: Column *  wxTableCtrl :: Header :: PointIn ( const wxPoint &  point ) const
{
   const const_iterator    END   = end ();

   for ( const_iterator  i = begin () ; i != END ; ++i )
   {
      Column *    c  = *i;
      
      if ( ( c -> Show () ) && ( c -> Rect ().Contains ( point ) ) )
         return ( c );
   }

   return (  0 );
}



wxTableCtrl :: Column *  wxTableCtrl :: Header :: PointSize ( const wxPoint &  point, int  play ) const
{
   const wxPoint           tmp ( point.x - play, point.y );
   const const_iterator    END   = end ();

   for ( const_iterator  i = begin () ; i != END ; ++i )
   {
      Column *    c  = *i;
      
      if ( ! c -> Show () )
         continue;
         
      const wxRect &    r = c -> Rect ();

      if ( r.Contains ( tmp ) )
      {
         if ( ( point.x >= ( ( r.GetX () + r.GetWidth () ) - play ) ) &&
              ( point.x <= ( ( r.GetX () + r.GetWidth () ) + play ) )    )
            return ( c );

         return (  0 );
      }
   }

   return (  0 );
}



wxTableCtrl :: Column *  wxTableCtrl :: Header :: Find ( long  x, bool  before ) const
{
   const_iterator    END   = end ();

   if ( before )
      for ( const_iterator  i = begin () ; i != END ; ++i )
      {
         Column *    c  = *i;
         
         if ( ! c -> Show () )
            continue;
            
         const wxRect &    r  = c -> Rect ();

         // Check if column start before or at 'x'.
         
         if ( (   r.GetX ()                   <= x ) &&
              ( ( r.GetX () + r.GetWidth () ) >  x )    )
            return ( c );                  
      }
   else
      for ( const_iterator  i = begin () ; i != END ; ++i )
      {
         Column *    c  = *i;
         
         if ( ! c -> Show () )
            continue;
            
         const wxRect &    r  = c -> Rect ();

         // Check if column start before or at 'x'.
         
         if ( (   r.GetX ()                   <= x ) &&
              ( ( r.GetX () + r.GetWidth () ) >  x )    )
         {
            ++i;
            
            if ( i != END )
               return ( c );
            else
               break;
         }
      }

   return (  0 );
}



wxTableCtrl :: Column *  wxTableCtrl :: Header :: Find ( size_t  reference ) const
{
   const_iterator    END   = end ();
   
   for ( const_iterator  i = begin () ; i != END ; ++i )
   {
      Column *    c  = *i;
      
      if ( c -> Reference () == reference )
         return ( c );
   }

   return (  0 );
}



void  wxTableCtrl :: Header :: SortByPosition ()
{
// :: sort ( begin (), end (), Column :: Position () );
// :: sort ( begin (), end (), less < Column * > () );
// sort ();

   if ( size () < 2 )
      return;

   // The simple and basic sort algorithm to sort the 'Column's'
   // on X position.

   const iterator    END   = --end  ();
   const iterator    END2  = end ();

   for ( iterator  i = begin () ; i != END ; ++i )
   {
      iterator   s = i;
      iterator   j = i;

      ++j;                                // Skip first.
      for ( ; j != END2 ; ++j )
      {
         if ( ( ( *s ) -> Rect ().GetX () >= 0 ) &&
              ( ( *j ) -> Rect ().GetX () >= 0 )    )
         {
            if ( ( *s ) -> Rect ().GetX () > ( *j ) -> Rect ().GetX () )
               s = j;
         }
         else
         {
            if ( ( ( *s ) -> Rect ().GetX () <  0 ) &&
                 ( ( *j ) -> Rect ().GetX () >= 0 )    )
               s = j;
         }
      }

      if ( s != i )
      {
         Column *    help = *i;         // Only swap the value (pointer)!

         *i = *s;
         *s = help;
      }
   }
}



void  wxTableCtrl :: Header :: Exchange ( Column *  move, Column *  part, bool  before )
{
   if ( move == part )
      return;
      
   iterator   i0 = find ( begin (), end (), move );
   iterator   i1 = find ( begin (), end (), part );

   erase  ( i0 );                         // Remove from the list.

   if ( ! before )
      ++i1;

   insert ( i1, move );                   // Insert before 'before'.

   RedoSize ();
}



void  wxTableCtrl :: Header :: RedoSize ()
{
   const iterator    END   = end ();
   Column *          help;
   long              x     = 0;
   long              h     = 0;

   for ( iterator  i = begin () ; i != END ; ++i )
   {
      help  = *i;

      help -> Move ( wxPoint ( x, 0 ) );

//?   table -> HeaderRect  ( help -> Reference (), help -> Rect () );

      if ( help -> Show () )
      {
         x += help -> Rect ().GetWidth ();

         // Keep track of the highest height.

         if ( h < help -> Rect ().GetHeight () )
            h = help -> Rect ().GetHeight ();
      }
   }

   // Set the height for all the header parts to the highest height.

   Height ( h );
}



long  wxTableCtrl :: Header :: Width ()
{
   const iterator    END   = end ();
   long              width = 0;

   for ( iterator  i = begin () ; i != END ; ++i )
   {
      Column *    c  = *i;
      
      if ( c -> Show () )
         width += c -> Rect ().GetWidth ();
   }

   return ( width );
}



void  wxTableCtrl :: Header :: Height ( long  _height )
{
   height   = _height;
   
   const iterator    END   = end ();
   Column *          help;

   for ( iterator  i = begin () ; i != END ; ++i )
   {
      help  = *i;
      
      help  -> Height ( height );
      
//?   table -> HeaderRect ( help -> Reference (), help -> Rect () );
   }
}



wxTableCtrl :: Column *  wxTableCtrl :: Header :: Add ( const wxString &  name, const wxString &  desc, size_t  reference, const wxRect &  rect, int  alignheader )
{
   push_back   ( new  Column ( name, desc, reference, rect, alignheader ) );
   
   return ( back () );
}



////////////////////////////////////////////////////////////////////////////////
// wxTableCtrl :: ToolTip                                                     //
////////////////////////////////////////////////////////////////////////////////



wxTableCtrl :: ToolTip :: ToolTip ( const wxString &  _tip, const wxRect &  _rect )
   : wxString ( _tip )
{
   rect  = _rect;
}   



const bool  wxTableCtrl :: ToolTip :: Contains ( const wxPoint &  point )
{
   return ( rect.Contains ( point ) );
}



////////////////////////////////////////////////////////////////////////////////
// wxTableCtrl :: ToolTipVector                                               //
////////////////////////////////////////////////////////////////////////////////



void  wxTableCtrl :: ToolTipVector :: Clear ()
{
   clear ();
}



void  wxTableCtrl :: ToolTipVector :: Add ( const ToolTip &  tooltip )
{
   push_back ( tooltip );
}



size_t  wxTableCtrl :: ToolTipVector :: IndexOf ( const wxPoint &  point )
{
   const size_t   SIZE  = size   ();
   
   for ( size_t  i = 0 ; i < SIZE ; ++i )
      if ( at ( i ).Contains ( point ) )
         return ( i );
         
   return ( static_cast < size_t > ( -1 ) );
}



const wxString &  wxTableCtrl :: ToolTipVector :: String ( size_t  index ) const
{
   static wxString   es ( wxEmptyString );
   
   if ( empty () )
      return ( es );
      
   if ( index >= size () )
      return ( es );
      
   return ( at ( index ) );
}



////////////////////////////////////////////////////////////////////////////////
// wxTableCtrl :: Body                                                         //
////////////////////////////////////////////////////////////////////////////////



const UINT              KEY_CONTROL          = 0x0001L;
const UINT              KEY_SHIFT            = 0x0002L;



const int               IDM_EXCHANGE         = 1100;
const int               IDM_EXCHANGE_FAST    = IDM_EXCHANGE + wxTableCtrl :: Body :: Mode_FAST;
const int               IDM_EXCHANGE_MERGE   = IDM_EXCHANGE + wxTableCtrl :: Body :: Mode_MERGE;
const int               IDM_EXCHANGE_ROLL    = IDM_EXCHANGE + wxTableCtrl :: Body :: Mode_ROLL;
const int               IDM_EXCHANGE_SQUASH  = IDM_EXCHANGE + wxTableCtrl :: Body :: Mode_SQUASH;

const int               IDM_EXCHANGE_BEGIN   = IDM_EXCHANGE_FAST;
const int               IDM_EXCHANGE_END     = IDM_EXCHANGE_SQUASH;



const size_t            wxTableCtrl :: Body :: nrow   = ~0U;



void  wxTableCtrl :: Body :: DataObject :: LoadData ()
{
   const wxTable :: Cursor *  current  = record -> CursorCurrent  ();
   const int                  COLUMNS  = table -> NumberOfColumns ();
// LPTSTR                     string   = ( LPTSTR ) GlobalLock ( *hglobal );
// int                        index    = 0;
   int                        count    = 0;
   wxString                   text;

// *string  = _T ( '\0' );
   text.Empty   ();

   if ( cursor != 0 )
   {
      record -> CursorSet   ( cursor );
      record -> Synchronize ();

      for ( int  i = 0 ; i < COLUMNS ; i++ )
      {
         if ( i > 0 )
            text  += "\n";

         text += table -> HeaderString   ( i );
         text += " : ";
         text += table -> FieldString    ( i );
      }
   }
   else
   {
      if ( ! table -> Select ().IsInverted () )
      {
         const wxTable :: Cursor *  c  = table -> Select ().FindFirst ();

//       while ( ( c != 0 ) && ( tmp.GetLength () < GLOBAL ) )
         while ( c != 0 )
         {
            if ( count > 0 )
               text += "\n";

            record -> CursorSet   ( c );
            record -> Synchronize ();

            for ( int  i = 0 ; i < COLUMNS ; i++ )
            {
               if ( i > 0 )
                  text += "\t";

               text += table -> FieldString    ( i );
            }

            count++;

            c  = table -> Select ().FindNext ( c );
         }
      }
      else
      {
         wxTable :: Record :: Result   result   = record -> GetFst ();

         while ( result == wxTable :: Record :: Result_OK )
         {
            if ( table -> Select ().Find ( record -> CursorCurrent () ) )
            {
               if ( count > 0 )
                  text += "\n";

               record -> Synchronize ();

               for ( int  i = 0 ; i < COLUMNS ; i++ )
               {
                  if ( i > 0 )
                     text += "\t";

                  text += table -> FieldString  ( i );
               }

               count++;
            }

            result   = record -> GetNxt ();
         }
      }
   }

// _tcsncpy ( string, tmp, index = __min ( tmp.GetLength (), GLOBAL ) );
// *( string + index ) = _T ( '\0' );
//
// GlobalUnlock ( *hglobal );

   record -> CursorSet     ( current );
   record -> Synchronize   ();

   SetText  ( text );
}



wxTableCtrl :: Body :: DataObject :: DataObject ()
   : wxTextDataObject ()
{
// global   = GlobalAlloc  ( GMEM_DDESHARE, GLOBAL );
}



wxTableCtrl :: Body :: DataObject :: ~DataObject ()
{
// GlobalFree ( global );
}



void  wxTableCtrl :: Body :: DataObject :: SetData ( wxTable *  _table, wxTable :: Cursor *  _cursor )
{
   table    = _table;
   record   = _table -> GetRecord ();
   cursor   = _cursor;

   LoadData ();
}



const long              wxTableCtrl :: Body :: DEFAULTSTYLE  = wxHSCROLL | wxVSCROLL;



BEGIN_EVENT_TABLE ( wxTableCtrl :: Body , wxWindow  )
   EVT_KILL_FOCUS       ( wxTableCtrl :: Body :: OnKillFocus       )
   EVT_ERASE_BACKGROUND ( wxTableCtrl :: Body :: OnEraseBackground )
   EVT_PAINT            ( wxTableCtrl :: Body :: OnPaint           )
   EVT_SET_FOCUS        ( wxTableCtrl :: Body :: OnSetFocus        )
   EVT_SIZE             ( wxTableCtrl :: Body :: OnSize            )

   EVT_CHAR             ( wxTableCtrl :: Body :: OnChar            )
// EVT_SCROLL           ( wxTableCtrl :: Body :: OnScroll          )
   EVT_SCROLLWIN        ( wxTableCtrl :: Body :: OnScroll          )
// EVT_COMMAND_SCROLL   ( wxTableCtrl :: Body :: OnScroll          )
   EVT_KEY_DOWN         ( wxTableCtrl :: Body :: OnKeyDown         )
   EVT_KEY_UP           ( wxTableCtrl :: Body :: OnKeyUp           )

   EVT_LEFT_DCLICK      ( wxTableCtrl :: Body :: OnLeftDClick      )
   EVT_LEFT_DOWN        ( wxTableCtrl :: Body :: OnLeftDown        )
   EVT_LEFT_UP          ( wxTableCtrl :: Body :: OnLeftUp          )

   EVT_MOTION           ( wxTableCtrl :: Body :: OnMotion          )
   EVT_MOUSEWHEEL       ( wxTableCtrl :: Body :: OnMouseWheel      )

   EVT_RIGHT_DOWN       ( wxTableCtrl :: Body :: OnRightDown       )
   EVT_RIGHT_UP         ( wxTableCtrl :: Body :: OnRightUp         )

   EVT_MENU_RANGE       ( IDM_EXCHANGE_BEGIN ,
                          IDM_EXCHANGE_END   ,
                          wxTableCtrl :: Body :: OnCommandExchange )
END_EVENT_TABLE   ()



void  wxTableCtrl :: Body :: Refresh ( bool  erase, const wxRect *  rect )
{
   super :: Refresh ( erase, rect );
}



void  wxTableCtrl :: Body :: DoSize ( bool  reset )
{
   if ( table == 0 )
   {
//    :: ShowScrollBar ( ( HWND ) GetHWND (), SB_BOTH, false );

      return;
   }

   wxClientDC  dc        ( this );
   int         w;
   int         h;

// prev_font = dc.SelectObject ( &font );
   dc.SetFont ( *font );

   rect  = GetClientRect   ();
// :: GetTextExtentPoint32 ( &dc, _T ( "M" ), 1, size );
   GetTextExtent  ( _T ( "M" ), &w, &h, 0, 0, font );

   text_height = h + text_top + text_bottom;

   const long    numberofrecords = record -> NumberOfKeys   ( record -> GetIndex () );  // JAK 20020808
   int           page            = rect.GetHeight () / text_height;



   // Vertical scrollbar.

   if ( control -> GetStyleEx2 () & ITCS_VSCROLLER )
;//    :: ShowScrollBar ( ( HWND ) GetHWND (), SB_VERT, false );
   else
   {
      if ( page < numberofrecords )
      {
         if ( table -> GetCapability () & wxTable :: Capability_PERCENTAGE )
         {
//          :: ShowScrollBar ( ( HWND ) GetHWND (), SB_VERT, false );
/*
            SCROLLINFO     tmp   =
            {
               sizeof ( SCROLLINFO )   ,
               SIF_ALL                 ,
               table -> ScrollPosMin (),  // 0
               table -> ScrollPosMax (),  // numberofrecords - 1  ,
               page                    ,
               0                       ,
            };

            if ( ! reset )                   // If no reset, don't set the pos.
               tmp.fMask &= ~SIF_POS;

            vertscrollinfo = tmp;

            SetScrollInfo ( SB_VERT, &vertscrollinfo );
*/
            int   pos   = reset ? 0 : GetScrollPos ( wxVERTICAL );

            SetScrollbar   ( wxVERTICAL, pos, page, record -> ScrollPosMax () - record -> ScrollPosMin (), false );
         }
         else
            ;
      }
      else
;//       :: ShowScrollBar ( ( HWND ) GetHWND (), SB_VERT, false );
   }


   // Horizontal scrollbar.
   // Always set the horizontal scroll bar info, even if there is no scroll
   // bar to display as HorzScroll depends on the data!

   int            width = rect.GetWidth () - ( ( control -> styleex & ITCS_CHECKBOXES ) ? CHECKBOXWIDTH : 0 );
/*
   SCROLLINFO     tmp   =
   {
      sizeof ( SCROLLINFO )                  ,
      SIF_ALL                                ,
      0                                      ,
      headerlist -> Width ()                 ,
      width                                  ,
      0
   };

   if ( ! reset )
      tmp.fMask &= ~SIF_POS;

   horzscrollinfo = tmp;

   SetScrollInfo ( SB_HORZ, &horzscrollinfo );
*/
   int   pos   = reset ? 0 : GetScrollPos ( wxHORIZONTAL );

   SetScrollbar ( wxHORIZONTAL, pos, width, header -> Width (), true );

   if ( header -> Width () > width )
;//    :: ShowScrollBar ( ( HWND ) GetHWND (), SB_HORZ, true  );
   else
   {
      if ( control -> left != 0 )
      {
         control -> left = 0;             // No horizontal scrolling!

         control -> DrawHeader   ();      // Refresh table header.
      }

//    :: ShowScrollBar ( ( HWND ) GetHWND (), SB_HORZ, false );
   }



// dc.SelectObject ( prev_font );

   visible.Allocate ( page, cursor );
}



void  wxTableCtrl :: Body :: DoPaintDC ( wxDC  *dc, size_t  row, const Fill &  fill )
{
   if ( lock )
      return;

   rect  = GetClientRect  ();

   wxBitmapDC           bdc         ( dc, rect );
   wxTable :: Record :: Result      
                        result;
   wxTable :: Cursor *  help        = 0;

   bdc.SetFont    ( *font );
   bdc.SetWindow  ( dc -> GetWindow () );

   rows        = ( text_height != 0 ) ? rect.GetHeight () / text_height : 0;
   cursor_row  = nrow;                           // Reset 'cursor_row'.

   // row : The cursor row. This can exceed the available rows after a resize!
   //
   // ROWS: The number of rows besides the cursor row.
   // A   : The number of requested rows above the cursor.
   // a   : The number of available rows above the cursor.
   // B   : The number of requested rows below the cursor.
   // b   : The number of available rows below the cursor.

   const size_t         ROWS        = rows - 1;
   const int            A           = ( ( row != nrow ) && ( row <= ROWS ) ) ? row : 0;
   const int            B           = ROWS - A;
   size_t               a           = 0;
   size_t               b           = 0;
   size_t               r           = 0;

   FillSolidRect  ( &bdc, wxRect ( 0, 0, rect.GetWidth (), rect.GetHeight () ), backgroundcolor );
   bdc.SetTextBackground   ( backgroundcolor );
   bdc.SetTextForeground   ( foregroundcolor );
// bdc.DrawRectangle       ( wxRect ( 0, 0, rect.GetWidth (), rect.GetHeight () ) );
// bdc.Clear               ();

   if ( ( table != 0 ) && ( cursor != 0 ) )
   {
      help  = record -> CursorCreate    ();
      *help = *cursor;

      record   -> SetIndex ( control -> index );

      switch ( fill )
      {
         case Fill_RESET   :
            // Go by 'cursor'. The 'cursor' has to be displayed!

            visible.Invalidate   ();

            break;

         case Fill_NORMAL  :
            // Go by 'visible.Top ()'.

            if ( row == -1 )
               *help = *visible.Top ();

            break;

         case Fill_SORT    :
            // Go by 'cursor' if visible, otherwise to by 'visible.Top ()'.

            if ( ( help -> IsValid () ) && ( visible.IndexOf ( *help ) != wxTable :: CursorVector :: npos ) )
               ;
            else
               *help = *visible.Top ();

            break;
      }

      visible.Invalidate   ();

      if ( ( help -> IsValid ()                                             ) &&
           ( record -> CursorSet ( help ) == wxTable :: Record :: Result_OK )    )
      {
         a = record -> GetPrvEx ( A );
         record -> CursorSet ( help );
         b = record -> GetNxtEx ( B + A - a );

         if ( ( a + b ) < ROWS )
         {
            record   -> CursorSet ( help );

            a = record  -> GetPrvEx ( ROWS - b );
         }
      }
      else
      {
         help -> Invalidate ();

         a  = 0;
         b  = rows;

         if ( record -> GetFst () == wxTable :: Record :: Result_OK )
            *help = *record -> CursorCurrent ();
      }

      // Perform the actual display.

      if ( help -> IsValid () )
      {
         // Paint the lines above the cursor line (if any).

         if ( ( ( r = ( a - 1 ) ) >= 0 ) && ( record -> CursorSet ( help ) == wxTable :: Record :: Result_OK ) )
         {
            result = record -> GetPrv ();
//          while ( ( r >= 0 ) && ( result == wxTable :: Record :: Result_OK ) )
            while ( ( r != nrow ) && ( result == wxTable :: Record :: Result_OK ) )
            {
               DoPaintLineDC  ( &bdc, r-- );

               result = record -> GetPrv ();
            }
         }

         // Paint the lines below the cursor line (if any).

         if ( ( ( r = ( a + 1 ) ) < rows ) && ( record -> CursorSet ( help ) == wxTable :: Record :: Result_OK ) )
         {
            result = record -> GetNxt ();
            while ( ( r < rows ) && ( result == wxTable :: Record :: Result_OK ) )
            {
               DoPaintLineDC  ( &bdc, r++ );

               result = record -> GetNxt ();
            }
         }

         // Paint the cursor line.

         if ( record -> CursorSet ( help ) == wxTable :: Record :: Result_OK )
            DoPaintLineDC ( &bdc, a );
      }

      delete  help;

      // Read the 'cursor' record if 'cursor' is valid!

      if ( cursor -> IsValid () )
         record -> CursorSet ( cursor );
   }



   // Grid lines.

   if ( ( control -> styleex & ITCS_GRIDLINES ) != 0 )
   {
      bdc.SetPen ( wxPen ( wxSystemSettings :: GetColour ( wxSYS_COLOUR_3DLIGHT ), 1, wxSOLID ) );

      if ( control -> styleex & ITCS_GRIDVERTICAL   )
      {
         const Header :: Iterator   END   = header -> end ();

         for ( Header :: Iterator  i = header -> begin () ; i != END ; ++i )
         {
            Column *          c  = *i;
            
            if ( ! c -> Show () )
               continue;
               
            const wxRect &    r  = c -> Rect ();
            int               x  = r.GetX () + r.GetWidth () - 1 - control -> left;

            if ( control -> styleex & ITCS_CHECKBOXES )
               x += CHECKBOXWIDTH;

            if ( ( x >= 0 ) && ( x < rect.GetWidth () ) )
               bdc.DrawLine      ( x, 0, x, rect.GetHeight () );
         }
      }

      if ( control -> styleex & ITCS_GRIDHORIZONTAL )
      {
         for ( size_t  r = 0 ; r < rows ; ++r )
         {
            int   y  = ( r + 1 ) * text_height;

            bdc.DrawLine   ( 0, y, rect.GetWidth (), y );
         }
      }
   }



   // Output.

   dc -> Blit ( 0, 0, rect.GetWidth (), rect.GetHeight (), &bdc, 0, 0, wxCOPY );
}



void  wxTableCtrl :: Body :: DoPaintLineDC ( wxDC *  dc, size_t  row, int  index )
{
   if ( index == -1 )
      index = row;
      
   Header :: Iterator   i;
   bool                 focusrect   = ( ( *( visible [ index ] ) = *record -> CursorCurrent () ) == *cursor );
   wxColor              fore        = foregroundcolor;
   wxColor              back        = backgroundcolor;
// const wxFont &       font        = dc -> GetFont ();

// record   -> GetBLOb     ();
   record   -> Synchronize ();

// TODO: wxTable :: TextColor???
//?if ( table -> GetCapability () & wxTable :: Capability_TEXTCOLOR )
//?   fore  = table -> TextColor ();

   // Determine the text- and background color for this line.

   if ( ( (     control -> styleex & ITCS_MULTISELECT     ) &&
          ( table -> Select ().Find ( visible [ index ] ) )    ) ||
        ( (     control -> styleex & ITCS_MULTISELECT     ) &&
          ( table -> Select ().Size () == 0               ) &&
          ( focusrect                                     )    ) ||
        ( ( ! ( control -> styleex & ITCS_MULTISELECT )   ) &&
          ( focusrect                                     )    )    )
   {
      if ( focus )
      {
#ifdef __WXMSW__
         fore  = wxSystemSettings :: GetColour ( wxSYS_COLOUR_HIGHLIGHTTEXT );
         back  = wxSystemSettings :: GetColour ( wxSYS_COLOUR_HIGHLIGHT     );
#else
         fore  = wxSystemSettings :: GetColour ( wxSYS_COLOUR_WINDOWTEXT    );
         back  = wxSystemSettings :: GetColour ( wxSYS_COLOUR_HIGHLIGHT     );
#endif
      }
      else
      {
         fore  = wxSystemSettings :: GetColour ( wxSYS_COLOUR_WINDOWTEXT    );
         back  = wxSystemSettings :: GetColour ( wxSYS_COLOUR_3DFACE        );
      }
   }
   
   wxTableEvent   te ( control, wxEVT_COMMAND_TABLE_RECORD_ATTRIBUTES, 0, wxDefaultPosition, table, record, visible [ index ], focusrect );
   
   te.SetTextBackground ( back );
   te.SetTextForeground ( fore );
   te.SetFont           ( *font );
   
   if ( ProcessEvent ( te ) )
   {
      back  = te.GetTextBackground  ();
      fore  = te.GetTextForeground  ();
      
      if ( te.GetFont () != *font )
         dc -> SetFont ( te.GetFont () );
   }

   dc -> SetTextBackground ( back );
   dc -> SetTextForeground ( fore );
// dc -> Clear             ();

   FillSolidRect  ( dc, wxRect ( 0, row * text_height, rect.GetWidth (), text_height ), back );

   const int                  DX    = ( control -> styleex & ITCS_CHECKBOXES ) ? CHECKBOXWIDTH : 0;
   const Header :: Iterator   END   = header -> end ();

   for ( i = header -> begin () ; i != END ; ++i )
   {
      Column *          column   = *i;
      
      if ( ! column -> Show () )
         continue;
         
      const wxRect &    head     = column -> Rect ();
      wxRect            text     ( wxPoint ( head.GetX () + text_left - control -> left + DX, row * text_height ), wxSize ( head.GetWidth () - text_left - text_right, text_height ) );

      // Only perform DrawText if the text to draw will become visible!

      if ( rect.Intersects ( text ) )
      {
         bool  processed   = false;
         
         if ( control -> drawevents )
            processed   = ProcessEvent ( wxTableEvent ( control, wxEVT_COMMAND_TABLE_DRAW_COLUMN_DATA, dc, column, text, table, record, cursor, focusrect ) );

         if ( ! processed )
         {
            DWORD   ref             = ( *i ) -> Reference ();
            UINT    drawtextformat  = table -> FieldDrawTextFormat   ( ref );
            DWORD   type            = table -> FieldType             ( ref );

            if ( ( type == TFT_NULL ) || ( type & TFT_TEXT ) )
            {
               DrawText ( dc, table -> FieldString ( ref ), -1, text, drawtextformat | DT_NOPREFIX );
            }
            else
               if ( type & TFT_BITMAP )
                  ;
               else
                  if ( type & TFT_ENHMETAFILE )
                  {
   /*
                     const BYTE       *data     = table -> FieldData     ( ref, 0 );
                     const DWORD       size     = table -> FieldDataSize ( ref );

                     if ( data != 0 )
                     {
      //                iEnhMetaFile     *metafile = new  iEnhMetaFile ( AfxGetResourceHandle (), MAKEINTRESOURCE ( 840 ) );
      //                iEnhMetaFile     *metafile = ( iEnhMetaFile * ) table -> FieldData ( ref, 0 );
                        iEnhMetaFile     *metafile = new  iEnhMetaFile ( data, size );

                        if ( ( metafile != 0 ) && ( ! text.IsRectEmpty () ) )
                        {
                           wxRect    bounds   ( metafile -> Bounds () );

                           if ( bounds.IsRectEmpty () )
                              dc -> PlayMetaFile ( *metafile, text );
                           else
                           {
                              double   dx = ( double ) text.W () / ( double ) bounds.W ();
                              double   dy = ( double ) text.H () / ( double ) bounds.H ();
                              long     x  = text.X ();
                              long     y  = text.Y ();
                              long     w  = text.W ();
                              long     h  = text.H ();

                              if ( dx >= dy )
                                 w = ( double ) bounds.W () * dy;
                              else
                                 h = ( double ) bounds.H () * dx;

                              if ( drawtextformat & DT_CENTER )
                                 x = text.X () + ( text.W () - w ) / 2;
                              else
                                 if ( drawtextformat & DT_RIGHT )
                                    x = text.X () + text.W () - w;

                              if ( drawtextformat & DT_VCENTER )
                                 y = text.Y () + ( text.H () - h ) / 2;
                              else
                                 if ( drawtextformat & DT_BOTTOM )
                                    y = text.Y () + text.H () - h;

                              dc -> PlayMetaFile ( *metafile, &wxRect ( x, y, w, h ) );
                           }

                           delete  metafile;
                        }
                     }
   */
                  }
         }
      }
   }

   if ( control -> styleex & ITCS_CHECKBOXES )
   {
      wxTable :: SetAdapter &    sa       = table -> Check ();
      bool                       checked  = false;

      if ( sa.IsActive () )
         checked  = sa.Find ( visible [ index ] );
      else
      {
         wxTableEvent   te ( control, wxEVT_COMMAND_TABLE_CHECKBOX_CHECKED, 0, wxDefaultPosition, table, record, visible [ index ], focusrect );
         
         if ( ProcessEvent ( te ) )
            checked  = te.IsChecked ();
      }
         
      FillSolidRect  ( dc, wxRect  ( 0, row * text_height, 16, text_height ), back );
//    DrawState      ( dc, wxPoint ( 0, row * text_height ), wxSize ( 16, 16 ), checked ? checkbox_checked : checkbox_unchecked, DST_BITMAP | DSS_NORMAL );
      DrawCheckBox   ( dc, wxRect ( 0, row * text_height, 16, 16 ), checked );
   }

   // Check if the 'cursor' is the visible row just drawn.
   // If so, set the 'cursor_row' and DrawFocusRect.

   if ( focusrect )
   {
      DoFocusRect ( dc, row );
      cursor_row  = index;
   }

   dc -> SetFont           ( *font );
   dc -> SetTextBackground ( backgroundcolor );
   dc -> SetTextForeground ( foregroundcolor );
}



void  wxTableCtrl :: Body :: DoPaint ()
{
   wxClientDC  dc ( this );

   DoPaintDC ( &dc );
}



void  wxTableCtrl :: Body :: DoPaintLine ( size_t  row )
{
   wxClientDC  dc    ( this );
   wxBitmapDC  bdc   ( &dc, wxRect ( 0, row * text_height, rect.GetWidth (), text_height ) );

   bdc.SetFont    ( *font );
   bdc.SetWindow  ( dc.GetWindow () );

   DoPaintLineDC ( &bdc, 0, row );

   bdc.Update  ();
}



void  wxTableCtrl :: Body :: DoFocusRect ( wxDC *  dc, int  row )
{
   dc -> SetTextBackground ( backgroundcolor );
   dc -> SetTextForeground ( foregroundcolor );
// dc -> DrawFocusRect     ( wxRect (  0, row * text_height, rect.Width (), text_height ) );

// RECT     temp;
//
// :: SetRect        ( &temp, 0, row * text_height, rect.W (), row * text_height + text_height );
// :: DrawFocusRect  ( ( HDC ) dc -> GetHDC (), &temp );

   :: DrawFocusRect  ( dc, wxRect (  0, row * text_height, rect.GetWidth (), text_height ) );
}



void  wxTableCtrl :: Body :: DrawCheckBox ( wxDC *  dc, const wxRect &  rect, bool  checked )
{
   wxRendererNative :: Get ().DrawCheckBox ( dc -> GetWindow (), *dc, rect, checked ? wxCONTROL_CHECKED : 0 );
   
#if 0   
#ifdef __WXMSW__   
   wxUxThemeEngine *    te = wxUxThemeEngine :: GetIfActive ();
   
   if ( te != 0 )
   {
      wxUxThemeHandle   t  ( dc -> GetWindow (), L"BUTTON" );
      RECT              r;
      
      :: SetRect ( &r, rect.x, rect.y, rect.GetRight  (), rect.GetBottom () );
      
      te -> DrawThemeBackground ( t, static_cast < HDC > ( dc -> GetHDC () ), BP_CHECKBOX, checked ? CBS_CHECKEDNORMAL : CBS_UNCHECKEDNORMAL, &r, 0 );
   }
   else
   {
      HDC         hdc   = ( HDC ) dc -> GetHDC ();

      :: DrawState ( hdc, 0, 0, reinterpret_cast < LPARAM > ( checked ? checkbox_checked -> GetHBITMAP () : checkbox_unchecked -> GetHBITMAP () ), 0, rect.x, rect.y, rect.GetWidth (), rect.GetHeight (), DST_BITMAP | DSS_NORMAL );
   }
#else
   #error "Checkbox drawing code to insert here"
#endif   
#endif
}



void  wxTableCtrl :: Body :: SyncScrollBar ()
{
   if ( table == 0 )
      return;

// SCROLLINFO     tmp   =
// {
//    sizeof ( SCROLLINFO ),
// };
   wxRect         rect  = GetClientRect   ();

   // Vertical scrollbar.

   if ( control -> styleex & ITCS_VSCROLLER )
      ;
   else
   {
      SetScrollbar   ( wxVERTICAL, record -> ScrollPosByCursor ( visible.Top () ), rect.GetHeight () / text_height, record -> ScrollPosMax () - record -> ScrollPosMin () );
/*
      GetScrollInfo  ( SB_VERT, &tmp, SIF_ALL );
      GetClientRect  ( &rect );

      tmp.fMask   = SIF_POS | SIF_RANGE | SIF_PAGE;
      tmp.nMin    = table -> ScrollPosMin      ();    // 0;
      tmp.nMax    = table -> ScrollPosMax      ();    // table -> NumberOfRecords () - 1;
      tmp.nPage   = rect.H () / text_height;
      tmp.nPos    = table -> ScrollPosByCursor ( visible.Top () );

      vertscrollinfo = tmp;

      SetScrollInfo ( SB_VERT, &tmp, true );
*/
   }


   // Horizontal scrollbar.

   SetScrollbar  ( wxHORIZONTAL, GetScrollPos ( wxHORIZONTAL ), GetScrollThumb ( wxHORIZONTAL ), header -> Width () );
/*
   GetScrollInfo ( SB_HORZ, &tmp, SIF_ALL );

   tmp.fMask = SIF_RANGE;
   tmp.nMin  = 0;
   tmp.nMax  = headerlist -> Width ();
   tmp.nPos  = 0;

   horzscrollinfo = tmp;

   SetScrollInfo ( SB_HORZ, &tmp, true );
*/
// CScrollBar    *bar   = GetScrollBarCtrl   ( SB_VERT );
//
// if ( bar != 0 )
//    bar -> Invalidate ();
}



bool  wxTableCtrl :: Body :: HorzScroll ( long  scroll )
{
   if ( scroll == 0 )
      return ( false );
/*
   SCROLLINFO   tmp =
   {
      sizeof ( SCROLLINFO ),
   };

   GetScrollInfo ( SB_HORZ, &tmp, SIF_ALL );
*/
   const long  POS   = GetScrollPos    ( wxHORIZONTAL );
   const long  RANGE = GetScrollRange  ( wxHORIZONTAL );

   // Determine what the movement should be. Move either 'scroll' or the
   // maximum left to scroll.

   int   move = 0;

   if ( scroll > 0 )
      move = wxMin ( RANGE - POS, scroll );

   if ( scroll < 0 )
      move = wxMax ( 0     - POS, scroll );

   if ( move != 0 )
   {
/*
      tmp.nPos += move;

      SetScrollInfo ( SB_HORZ, &tmp, SIF_POS );    // Set the new thumb position.
*/

      SetScrollPos ( wxHORIZONTAL, POS + move );

//    OnHScroll ( SB_THUMBPOSITION, tmp.nPos, 0 ); // Call the handler.
      wxScrollWinEvent     se ( wxEVT_SCROLLWIN_THUMBRELEASE, POS, wxHORIZONTAL );

      se.SetEventObject ( this );

      OnHScroll ( se );

      return ( true );
   }

   return ( false );
}



void  wxTableCtrl :: Body :: SelectRangeStart ()
{
   if ( begin == 0 )
   {
      begin = record -> CursorCreate   ();
      end   = record -> CursorCreate   ();

      *begin   = *end   = *cursor;
   }
}



void  wxTableCtrl :: Body :: SelectRangeStop ()
{
   if ( begin != 0 )
   {
      delete  begin;
      delete  end;

      begin = 0;
      end   = 0;
   }
}



void  wxTableCtrl :: Body :: SelectRange ( const wxTable :: Cursor *  b, const wxTable :: Cursor *  e, bool  select )
{
   if ( ( begin == 0 ) || ( end == 0 ) )
      return;

   wxTable :: Cursor *  backup      = record -> CursorCreate ();
   wxTable :: Cursor *  cursor      = record -> CursorCreate ();
   int                  direction   = ( record -> ScrollPosByCursor ( b ) < record -> ScrollPosByCursor ( e ) );
   wxTable :: Record :: Result    
                        result      = record -> CursorSet    ( b );

   while ( ( result == wxTable :: Record :: Result_OK ) && ( ( *cursor = *record -> CursorCurrent () ) != *e ) )
   {
//    table -> Select   ( cursor, select );
      table -> Select   ().Select ( cursor, select );

      if ( direction )
         result   = record -> GetNxt ();
      else
         result   = record -> GetPrv ();
   }

   if ( ( select ) && ( *cursor == *e ) )
      table -> Select   ().Select ( cursor, select );

   record -> CursorSet   ( backup );

   *end  = *e;

   delete  cursor;
   delete  backup;
}



void  wxTableCtrl :: Body :: SelectRangeExt ( const wxTable :: Cursor *  ext )
{
   wxTable :: Cursor *  backup   = record -> CursorCreate       ();
   long                 b        = record -> ScrollPosByCursor  ( begin  );
   long                 e        = record -> ScrollPosByCursor  ( end    );
   long                 c        = record -> ScrollPosByCursor  ( ext    );

   // Determine what the selection extension is doing.

   if ( b < e )
   {
      if ( c < b )
      {
         table -> Select ().All ( false );

         SelectRange ( begin, ext );
      }
      else
         if ( c > e )
            SelectRange ( end, ext );
         else
            SelectRange ( end, ext, false );
   }
   else
   {
      if ( c > b )
      {
         table -> Select ().All ( false );

         SelectRange ( begin, ext );
      }
      else
         if ( c < e )
            SelectRange ( end, ext );
         else
            SelectRange ( end, ext, false );
   }

   record   -> CursorSet ( backup );

   delete  backup;
}



wxTable :: Cursor *  wxTableCtrl :: Body :: GetCursorAtPoint ( const wxPoint &  point )
{
   wxTable :: Cursor *  c  = visible [ point.y / text_height ];
   
   if ( c == 0 )
      return (  0 );
      
   if ( ! c -> IsValid () )
      return (  0 );
      
   return ( c );
}



const bool  wxTableCtrl :: Body :: IsFocus ( const wxTable :: Cursor *  check ) const
{
   if ( ( cursor == 0 ) || ( check == 0 ) )
      return ( false );
      
   return ( *cursor == *check );
}



bool  wxTableCtrl :: Body :: _DoSearch ( const wxString &  text, bool  match_case, bool  direction, const bool &  _first )
{
   bool              first = _first;
   const wxString    TEXT  = text.Upper ();

   while ( ( first                                                                        ) ||
           ( (   direction ) && ( record -> GetNxt () == wxTable :: Record :: Result_OK ) ) ||
           ( ( ! direction ) && ( record -> GetPrv () == wxTable :: Record :: Result_OK ) )    )
   {
      record   -> Synchronize ();

      const Header :: Iterator   END   = header -> end ();

      for ( Header :: Iterator  i = header -> begin () ; i != END ; ++i )
      {
         Column *    c     = *i;
         
         if ( ! c -> Show () )
            continue;
            
         size_t      ref   = c -> Reference ();
         DWORD       type  = table -> FieldType ( ref );

         if ( ( type == TFT_NULL ) || ( type & TFT_TEXT ) )
         {
            const wxString    str   = table -> FieldString ( ref );
            const wxString    STR   = str.Upper ();

            if ( ( ! str.IsEmpty ()                                        ) &&
                 ( ( (   match_case ) && ( str.Find ( text ) != -1 ) ) ||
                   ( ( ! match_case ) && ( STR.Find ( TEXT ) != -1 ) )    )    )
            {
               *cursor = *record -> CursorCurrent ();

               DoPaintFill    ();
               RecordFocused  ( wxDefaultPosition );
               RelationSync   ();

               return ( true );
            }
         }
      }

      first = false;
   }

   return ( false );
}



bool  wxTableCtrl :: Body :: _CursorTo ( long  row, bool  controldown )
{
   wxTable :: Cursor *    
               c     = visible [ row ];

   if ( ( c != 0 ) && ( c -> IsValid () ) )
   {
         // Check if the user clicked on the current cursor record.
         // If not:
         // - See if the current cursor is visible and repaint the cursor row.
         // - Assign a new cursor.
         // - Synchronize the table relations.

         if ( *cursor != *c )
         {
            size_t      tmp;

            if ( ( tmp = visible.IndexOf ( *cursor ) ) != wxTable :: CursorVector :: npos )
            {
               record   -> CursorSet   ( cursor );

               cursor   -> Invalidate  ();

               DoPaintLine    ( tmp );
               
               ProcessEvent   ( wxTableEvent ( control, wxEVT_COMMAND_TABLE_RECORD_DESELECTED, 0, down, table, record, cursor ) );
            }

            *cursor = *visible [ cursor_row = row ];
//          DoFocusRect ( &dc, cursor_row );

            record   -> CursorSet ( cursor );

            RelationSync   ();
            
            ProcessEvent   ( wxTableEvent ( control, wxEVT_COMMAND_TABLE_RECORD_SELECTED, 0, down, table, record, cursor ) );
         }

         if ( control -> styleex & ITCS_MULTISELECT )
         {
//          if ( flag & MK_CONTROL )
            if ( controldown )
               table -> Select ().Toggle ( cursor );
//          else
//             table -> Select   ( cursor );
         }

         record   -> CursorSet   ( cursor );
         DoPaintLine    ( cursor_row );

         // Send Focused event.
         
//       wxLogDebug ( "%s offset before: %ld", __FUNCTION__, static_cast < wxTable :: DefaultCursor * > ( cursor ) -> Offset () );
         RecordFocused  ( down );
//       wxLogDebug ( "%s offset after : %ld", __FUNCTION__, static_cast < wxTable :: DefaultCursor * > ( cursor ) -> Offset () );
         
//       DoPaintLine    ( cursor_row );
         RelationSync   ();
         
         return ( true );
   }
   
   return ( false );
}



void  wxTableCtrl :: Body :: OnKillFocus ( wxFocusEvent & )
{
   focus = false;                         // Lost the focus.

   control  -> DrawFocusRect  ( false );

   if ( control -> styleex & ITCS_MULTISELECT )
      DoPaint  ();
   else
      if ( ( cursor != 0 ) && ( ( cursor_row = visible.IndexOf ( *cursor ) ) != wxTable :: CursorVector :: npos ) )
         DoPaintLine ( cursor_row );
         

// super :: OnKillFocus ( window );
}



void  wxTableCtrl :: Body :: OnEraseBackground ( wxEraseEvent & )
{
}



void  wxTableCtrl :: Body :: OnPaint ( wxPaintEvent & )
{
   wxPaintDC   dc ( this );

   DoPaintDC ( &dc, nrow, fill );

   fill  = Fill_NORMAL;

   SyncScrollBar  ();

   RelationSync   ();
}



void  wxTableCtrl :: Body :: OnSetFocus ( wxFocusEvent & )
{
   focus = true;                          // Gained the focus.

   control  -> DrawFocusRect  ( true );

   if ( control -> styleex & ITCS_MULTISELECT )
      DoPaint  ();
   else
      if ( ( cursor != 0 ) && ( ( cursor_row = visible.IndexOf ( *cursor ) ) != wxTable :: CursorVector :: npos ) )
         DoPaintLine ( cursor_row );

// super :: OnSetFocus ( window );
}



void  wxTableCtrl :: Body :: OnSize ( wxSizeEvent & )
{
   DoSize ( false );
}



void  wxTableCtrl :: Body :: OnChar ( wxKeyEvent & )
{
// if ( table -> Index () >= 0 )
   if ( control -> index >= 0 )
   {
//    iEdit   *edit  = new  iEdit ( this, wxRect (  10,  10, 100, 14 ), WS_VISIBLE | WS_BORDER, "", -1 );
//
//    edit -> SetFocus ();
   }

// iWnd :: OnChar ( keycode, repeat, flag );
}



void  wxTableCtrl :: Body :: OnScroll ( wxScrollWinEvent &  se )
{
   if ( table == 0 )
      return;

   switch ( se.GetOrientation () )
   {
      case wxHORIZONTAL :
         OnHScroll ( se );

         break;

      case wxVERTICAL   :
         OnVScroll ( se );

         break;

      default           :
//       iMessageBox :: Report ( iMessageBox :: ButtonOk, __FILE__, this, "%d %d %d", wxHORIZONTAL, wxVERTICAL, se.GetOrientation () );

         break;
   }
}



void  wxTableCtrl :: Body :: OnHScroll ( const wxScrollWinEvent &  se ) // UINT  code, UINT  pos, CScrollBar * )
{
   if ( FindFocus () != this )
      SetFocus ();

// MessageBeep ( MB_OK );
/*
   SCROLLINFO   tmp =
   {
      sizeof ( SCROLLINFO ),
   };

   GetScrollInfo ( SB_HORZ, &tmp, SIF_ALL );
*/
   const int      CODE  = se.GetEventType ();
   const int      POS   = GetScrollPos    ( wxHORIZONTAL ); //se.GetPosition  ();
   const int      RANGE = GetScrollRange  ( wxHORIZONTAL );
   const int      THUMB = GetScrollThumb  ( wxHORIZONTAL );
   int            pos   = POS;

   if      ( CODE == wxEVT_SCROLLWIN_LINEUP   )
   {
      if ( pos > 0 )
      {
         Column *    part  = header -> Find ( pos - 1, true );

         if ( part != 0 )
            control -> left = pos = part -> Rect ().GetX ();
         else
            control -> left = pos = 0;

         control -> DrawHeader  ();

         DoPaint  ();

         SetScrollPos ( wxHORIZONTAL, pos );
      }
   }
   else if ( CODE == wxEVT_SCROLLWIN_LINEDOWN )
   {
      if ( pos < ( RANGE - ( THUMB - 1 ) ) )
      {
         Column *    part  = header -> Find ( pos, false );

         if ( part != 0 )
            control -> left   = pos = part -> Rect ().GetX ();
         else
            control -> left   = pos = RANGE - ( THUMB - 1 );

         if ( pos > ( RANGE - ( THUMB - 1 ) ) )
            control -> left   = pos = RANGE - ( THUMB - 1 );

         control -> DrawHeader ();

         DoPaint ();

//       SetScrollInfo ( SB_HORZ, &tmp, SIF_POS );
         SetScrollPos ( wxHORIZONTAL, pos );
      }
   }
   else if ( CODE == wxEVT_SCROLLWIN_PAGEUP   )
   {
      if ( pos > 0 )
      {
         // Scroll either: nPage - 1, or what is left to scroll.

         int   scroll = wxMin ( THUMB - 1, pos - 0 );

         pos -= scroll;

         control -> left = pos;

         control -> DrawHeader ();

         DoPaint ();

//       SetScrollInfo ( SB_HORZ, &tmp, SIF_POS );
         SetScrollPos ( wxHORIZONTAL, pos );
      }
   }
   else if ( CODE == wxEVT_SCROLLWIN_PAGEDOWN )
   {
      if ( pos < RANGE )
      {
         int   scroll = wxMin ( THUMB - 1, RANGE - ( THUMB - 1 ) - pos );

         pos += scroll;

         control -> left = pos;

         control -> DrawHeader ();

         DoPaint ();

//          SetScrollInfo ( SB_HORZ, &tmp, SIF_POS );
         SetScrollPos ( wxHORIZONTAL, pos );
      }
   }
   else if ( CODE == wxEVT_SCROLLWIN_THUMBRELEASE )
   {
//    pos = tmp.nTrackPos;

      control -> left = pos;

      control -> DrawHeader ();

      DoPaint ();

//       SetScrollInfo ( SB_HORZ, &tmp, SIF_POS );
      SetScrollPos ( wxHORIZONTAL, pos );
   }
   else if ( CODE == wxEVT_SCROLLWIN_THUMBTRACK )
   {
      if ( table -> GetCapability () & wxTable :: Capability_HSCROLLTRACK )
      {
//       pos = tmp.nTrackPos;
         pos = se.GetPosition ();

         control -> left = pos;

         control -> DrawHeader ();

         DoPaint ();

//       SetScrollInfo ( SB_HORZ, &tmp, SIF_POS );
         SetScrollPos ( wxHORIZONTAL, pos );
      }
   }
   else if ( CODE == wxEVT_SCROLLWIN_TOP )
   {
   }
   else if ( CODE == wxEVT_SCROLLWIN_BOTTOM )
   {
   }
// else if ( CODE == wxEVT_SCROLLWIN_ENDSCROLL )
// {
// }

   control  -> DrawFocusRect  ( true );
}



void  wxTableCtrl :: Body :: OnVScroll ( const wxScrollWinEvent &  se ) // UINT  code, UINT  pos, CScrollBar * )
{
   if ( FindFocus () != this )
      SetFocus ();

   if ( control -> styleex & ITCS_VSCROLLER )
      return;
/*
   SCROLLINFO   tmp =
   {
      sizeof ( SCROLLINFO ),
   };

   GetScrollInfo ( SB_VERT, &tmp, SIF_ALL );
*/
// MessageBeep ( MB_OK );

   const int      CODE  = se.GetEventType ();
   const int      POS   = GetScrollPos    ( wxVERTICAL ); //se.GetPosition  ();
   const int      RANGE = GetScrollRange  ( wxVERTICAL );
   const int      THUMB = GetScrollThumb  ( wxVERTICAL );
   int            pos   = POS;

   if ( CODE == wxEVT_SCROLLWIN_LINEUP )
   {
      if ( pos > 0 )
      {
         --pos;

         if ( record -> CursorSet ( visible.Top () ) == wxTable :: Record :: Result_OK )
         {
            if ( record -> GetPrv () == wxTable :: Record :: Result_OK )
            {
               visible.Top ( *record -> CursorCurrent () );

               DoPaint ();
            }
         }
         else
            if ( table -> GetCapability () & wxTable :: Capability_VSCROLLROUGH )
            {
               pos   = 0;

               record -> GetFst ();
               visible.Top ( *record -> CursorCurrent () );

               DoPaint ();
            }

//       SetScrollInfo ( SB_VERT, &tmp, SIF_POS );
         SetScrollPos ( wxVERTICAL, pos );
      }
   }
   else if ( CODE == wxEVT_SCROLLWIN_LINEDOWN )
   {
      if ( pos < ( RANGE - ( THUMB - 1 ) ) )
      {
         ++pos;

         if ( record -> CursorSet ( visible.Top () ) == wxTable :: Record :: Result_OK )
         {
            if ( record -> GetNxt () == wxTable :: Record :: Result_OK )
            {
               visible.Top ( *record -> CursorCurrent () );

               DoPaint ();
            }
         }
         else
            if ( table -> GetCapability () & wxTable :: Capability_VSCROLLROUGH )
            {
               pos   = RANGE - ( THUMB - 1 );

               record   -> GetLst ();
               record   -> GetPrvEx ( THUMB - 1 );

               visible.Top ( *record -> CursorCurrent () );

               DoPaint ();
            }

//       SetScrollInfo ( SB_VERT, &tmp, SIF_POS );
         SetScrollPos ( wxVERTICAL, pos );
      }
   }
   else if ( CODE == wxEVT_SCROLLWIN_PAGEUP )
   {
      if ( pos > 0 )
      {
         // Scroll either: nPage - 1, or what is left to scroll.

         int   scroll = wxMin ( THUMB - 1, pos - 0 );

         pos  -= scroll;

         record   -> CursorSet ( visible.Top () );
         if ( record -> GetPrvEx ( scroll ) > 0 )
         {
            visible.Top ( *record -> CursorCurrent () );

            DoPaint ();
         }

//       SetScrollInfo ( SB_VERT, &tmp, SIF_POS );
         SetScrollPos ( wxVERTICAL, pos );
      }
   }
   else if ( CODE == wxEVT_SCROLLWIN_PAGEDOWN )
   {
      if ( pos < RANGE )
      {
         int   scroll = wxMin ( THUMB - 1, RANGE - ( THUMB - 1 ) - pos );

         pos  += scroll;

         record   -> CursorSet ( visible.Top () );
         if ( record -> GetNxtEx ( scroll ) > 0 )
         {
            visible.Top ( *record -> CursorCurrent () );

            DoPaint ();
         }

//          SetScrollInfo ( SB_VERT, &tmp, SIF_POS );
         SetScrollPos ( wxVERTICAL, pos );
      }
   }
   else if ( CODE == wxEVT_SCROLLWIN_THUMBRELEASE )
   {
      pos   = se.GetPosition  ();

      if ( record -> ScrollGetByPos ( pos ) == wxTable :: Record :: Result_OK )
      {
         if ( table -> GetCapability () & wxTable :: Capability_VSCROLLROUGH )
         {
            if ( pos <= 0 )
               record   -> GetFst ();

//          if ( pos >= ( RANGE - ( THUMB - 1 ) ) )
            if ( pos >= ( RANGE - THUMB ) )
            {
               record   -> GetLst   ();
               record   -> GetPrvEx ( THUMB - 1 );
            }
         }

         visible.Top ( *record -> CursorCurrent () );

         DoPaint ();
      }

//    tmp.nPos = tmp.nTrackPos;
//    SetScrollInfo ( SB_VERT, &tmp, SIF_POS );
      SetScrollPos ( wxVERTICAL, pos );
   }
   else if ( CODE == wxEVT_SCROLLWIN_THUMBTRACK )
   {
      pos   = se.GetPosition  ();

      if ( table -> GetCapability () & wxTable :: Capability_VSCROLLTRACK )
      {
         if ( record -> ScrollGetByPos ( pos ) == wxTable :: Record :: Result_OK )
         {
            visible.Top ( *record -> CursorCurrent () );

            DoPaint ();
         }

         SetScrollPos ( wxVERTICAL, pos );
      }
   }
   else if ( CODE == wxEVT_SCROLLWIN_TOP )
   {
      record   -> GetFst ();
      visible.Top ( *record -> CursorCurrent () );

      DoPaint ();

      SetScrollPos ( wxVERTICAL, 0 );
   }
   else if ( CODE == wxEVT_SCROLLWIN_BOTTOM )
   {
      record   -> GetLst ();
      visible.Top ( *record -> CursorCurrent () );

      DoPaint ();

      SetScrollPos ( wxVERTICAL, RANGE );
   }
// else if ( CODE == wxEVT_SCROLLWIN_ENDSCROLL )
// {
// }
}



void  wxTableCtrl :: Body :: OnKeyDown ( wxKeyEvent &  ke ) /*UINT  keycode, UINT  repeat, UINT  flag )*/
{
   if ( ProcessEvent ( wxTableEvent ( control, wxEVT_COMMAND_TABLE_KEY_DOWN, ke, ke.GetPosition (), table, record, cursor ) ) )
      return;
      
   if ( ( table == 0 ) || ( cursor == 0 ) )
      return;

// int   row;

// MessageBeep ( MB_OK );
// iMessageBox :: Report ( iMessageBox :: ButtonOk, __FILE__, *this, "OnKeyDown %u, %u, %u", keycode, repeat, flag );

   switch ( ke.GetKeyCode () )
   {
      case WXK_UP       :
         if ( KeyCursor ( ke ) )
            break;

         if ( ( record -> CursorSet ( cursor ) == wxTable :: Record :: Result_OK ) &&
              ( record -> GetPrv    ()         == wxTable :: Record :: Result_OK )    )
         {
            *cursor = *record -> CursorCurrent ();

//          if ( ( control -> styleex & ITCS_MULTISELECT ) && ( GetKeyState ( WXK_SHIFT ) & 0x8000 ) )
            if ( ( control -> styleex & ITCS_MULTISELECT ) && ( ke.ShiftDown () ) )
               SelectRangeExt ( cursor );

            wxClientDC  dc ( this );

//          DoFocusRect    ( &dc, cursor_row );

            if ( cursor_row > 0 )
               cursor_row--;

            DoPaintDC      ( &dc, cursor_row );
            SyncScrollBar  ();

            record   -> CursorSet ( cursor );

            RecordFocused  ( wxDefaultPosition );
            RelationSync   ();
         }
         else
            record   -> CursorSet ( cursor );

         break;

      case WXK_DOWN     :
         if ( KeyCursor ( ke ) )
            break;

         if ( ( record -> CursorSet ( cursor ) == wxTable :: Record :: Result_OK ) &&
              ( record -> GetNxt    ()         == wxTable :: Record :: Result_OK )    )
         {
            *cursor = *record -> CursorCurrent ();

//          if ( ( control -> styleex & ITCS_MULTISELECT ) && ( GetKeyState ( WXK_SHIFT ) & 0x8000 ) )
            if ( ( control -> styleex & ITCS_MULTISELECT ) && ( ke.ShiftDown () ) )
               SelectRangeExt ( cursor );

            wxClientDC  dc ( this );

//          DoFocusRect    ( &dc, cursor_row );

            if ( cursor_row < ( rows - 1 ) )
               cursor_row++;

            DoPaintDC      ( &dc, cursor_row );
            SyncScrollBar  ();

            record   -> CursorSet ( cursor );

            RecordFocused  ( wxDefaultPosition );
            RelationSync   ();
         }
         else
            record   -> CursorSet ( cursor );

         break;

      case WXK_PAGEUP   :
         if ( KeyCursor ( ke ) )
            break;

         if ( ( record -> CursorSet ( cursor )                            == wxTable :: Record :: Result_OK ) &&
              ( record -> GetPrvEx  ( GetScrollThumb ( wxVERTICAL ) - 1 ) >  0                              )    )
         {
            *cursor = *record -> CursorCurrent ();

//          if ( ( control -> styleex & ITCS_MULTISELECT ) && ( GetKeyState ( WXK_SHIFT ) & 0x8000 ) )
            if ( ( control -> styleex & ITCS_MULTISELECT ) && ( ke.ShiftDown () ) )
               SelectRangeExt ( cursor );

            wxClientDC  dc ( this );

//          DoFocusRect    ( &dc, cursor_row );

            DoPaintDC      ( &dc, cursor_row = 0 );
            SyncScrollBar  ();

            record -> CursorSet ( cursor );

            RecordFocused  ( wxDefaultPosition );
            RelationSync   ();
         }
         else
            record   -> CursorSet ( cursor );

         break;

      case WXK_PAGEDOWN :
         if ( KeyCursor ( ke ) )
            break;

         if ( ( record -> CursorSet ( cursor )                            == wxTable :: Record :: Result_OK ) &&
              ( record -> GetNxtEx  ( GetScrollThumb ( wxVERTICAL ) - 1 ) >  0                              )    )
         {
            *cursor = *record -> CursorCurrent ();

//          if ( ( control -> styleex & ITCS_MULTISELECT ) && ( GetKeyState ( WXK_SHIFT ) & 0x8000 ) )
            if ( ( control -> styleex & ITCS_MULTISELECT ) && ( ke.ShiftDown () ) )
               SelectRangeExt ( cursor );

            wxClientDC  dc ( this );

//          DoFocusRect    ( &dc, cursor_row );

            DoPaintDC      ( &dc, cursor_row = std :: min ( static_cast < size_t > ( rows - 1 ), record -> NumberOfKeys ( record -> GetIndex () ) - 1 ) );   // JAK 20020808
            SyncScrollBar  ();

            record   -> CursorSet ( cursor );

            RecordFocused  ( wxDefaultPosition );
            RelationSync   ();
         }
         else
            record   -> CursorSet ( cursor );

         break;

      case WXK_HOME     :
         if ( KeyCursor ( ke ) )
            break;

         if ( record -> GetFst () == wxTable :: Record :: Result_OK )
         {
            *cursor = *record -> CursorCurrent ();

//          if ( ( control -> styleex & ITCS_MULTISELECT ) && ( GetKeyState ( WXK_SHIFT ) & 0x8000 ) )
            if ( ( control -> styleex & ITCS_MULTISELECT ) && ( ke.ShiftDown () ) )
               SelectRangeExt ( cursor );

            wxClientDC  dc ( this );

//          DoFocusRect    ( &dc, cursor_row );
    
            DoPaintDC      ( &dc, cursor_row = 0 );
            SyncScrollBar  ();

            record   -> CursorSet ( cursor );

            RecordFocused  ( wxDefaultPosition );
            RelationSync   ();
         }
         else
         {
            cursor   -> Invalidate  ();
            visible.Invalidate   ();
         }

         break;

      case WXK_END      :
         if ( KeyCursor ( ke ) )
            break;

         if ( record -> GetLst () == wxTable :: Record :: Result_OK )
         {
            *cursor = *record -> CursorCurrent ();

//          if ( ( control -> styleex & ITCS_MULTISELECT ) && ( GetKeyState ( WXK_SHIFT ) & 0x8000 ) )
            if ( ( control -> styleex & ITCS_MULTISELECT ) && ( ke.ShiftDown () ) )
               SelectRangeExt ( cursor );

            wxClientDC  dc ( this );

//          DoFocusRect    ( &dc, cursor_row );

            DoPaintDC      ( &dc, cursor_row = std :: min ( static_cast < size_t > ( rows - 1 ), record -> NumberOfKeys ( record -> GetIndex () ) - 1 ) );   // JAK 20020808
            SyncScrollBar  ();

            record -> CursorSet ( cursor );

            RecordFocused  ( wxDefaultPosition );
            RelationSync   ();
         }
         else
         {
            cursor   -> Invalidate  ();
            visible.Invalidate   ();
         }

         break;

      case WXK_LEFT     :
         break;

      case WXK_RIGHT    :
         break;

      case WXK_INSERT   :
//       if ( GetKeyState ( WXK_CONTROL ) & 0x8000 )
         if ( ke.ControlDown () )
            control -> CommandCopy     ();
         else
            control -> CommandInsert   ();

         break;

      case WXK_DELETE   :
         control -> CommandDelete   ();

         break;

      case WXK_RETURN   :
         control -> CommandUpdate   ();

         break;
/*
      case WXK_CONTROL  :
         keydown |= KEY_CONTROL;

         break;

      case WXK_SHIFT    :
         keydown |= KEY_SHIFT;

         break;
*/
      case 'A'          :
//       if ( ( control -> styleex & ITCS_MULTISELECT ) && ( GetKeyState ( WXK_CONTROL ) & 0x8000 ) )
         if ( ( control -> styleex & ITCS_MULTISELECT ) && ( ke.ControlDown () ) )
         {
            table -> Select ().All ( true );

            DoPaint  ();
         }

         break;

      case 'F'          :
//       if ( GetKeyState ( WXK_CONTROL ) & 0x8000 )
         if ( ke.ControlDown () )
         {
//          if ( GetKeyState ( WXK_SHIFT ) & 0x8000 )
            if ( ke.ShiftDown () )
               control -> CommandSearch  ();
            else
               control -> CommandFind    ();
         }

         break;

      case 'S'          :
//       if ( GetKeyState ( WXK_CONTROL ) & 0x8000 )
         if ( ke.ControlDown () )
            control -> CommandSearch   ();

         break;

      default        :
//       iMessageBox :: Report ( iMessageBox :: ButtonOk, __FILE__, *this, "default" );
//       MessageBeep ( MB_OK );

         break;
   }

// iWnd :: OnKeyDown ( keycode, repeat, flag );
}



void  wxTableCtrl :: Body :: OnKeyUp ( wxKeyEvent &  ke )
{
   if ( ProcessEvent ( wxTableEvent ( control, wxEVT_COMMAND_TABLE_KEY_UP, ke, ke.GetPosition (), table, record, cursor ) ) )
      return;
      
   ke.Skip  ();
}



void  wxTableCtrl :: Body :: OnLeftDClick ( wxMouseEvent &  me )
{
   if ( ( table == 0 ) || ( text_height <= 0 ) )
      return;

   const wxPoint  pos      = me.GetPosition ();
   wxTable :: Cursor *
                  c        = GetCursorAtPoint ( pos );
   const bool     focus    = IsFocus ( c );
   Column *       column   = control -> FindColumn ( pos );
            
   // See if there is a valid record at the position on screen.
   // If there is, read it!
   
   if ( c != 0 )
      record -> CursorSet ( c );
   
   ProcessEvent ( wxTableEvent ( control, wxEVT_COMMAND_TABLE_RECORD_LEFT_DCLICK , column, me.GetPosition (), table, record, c, focus ) );
   ProcessEvent ( wxTableEvent ( control, wxEVT_COMMAND_TABLE_RECORD_ACTIVATED   , column, me.GetPosition (), table, record, c, focus ) );
}



void  wxTableCtrl :: Body :: OnLeftDown ( wxMouseEvent &  me )
{
   if ( FindFocus () != this )
      SetFocus ();

   if ( ( table == 0 ) || ( cursor == 0 ) )
      return;
      
   const wxPoint  pos      = me.GetPosition ();
   wxTable :: Cursor *
                  c        = GetCursorAtPoint ( pos );
   const bool     focus    = IsFocus ( c );
   Column *       column   = control -> FindColumn ( pos );
   const int      row      = pos.y / text_height;
   
   // See if there is a valid record at the position on screen.
   // If there is, read it!
   
   if ( c != 0 )
      record -> CursorSet ( c );
   
   if ( ProcessEvent ( wxTableEvent ( control, wxEVT_COMMAND_TABLE_RECORD_LEFT_CLICK, column, me.GetPosition (), table, record, c, focus ) ) )
      return;

   // Remember where the left button went down.

// down  = wxPoint ( me.GetX (), me.GetY () );
   down  = me.GetPosition  ();
   move  = wxPoint ( 0, 0 );              // Not moved relative to 'point' yet!

// const int   row   = me.GetY () / text_height;
// wxTable :: Cursor *    
//             c     = visible [ row ];

   // Check if the left button has been clicked on a row that actually
   // contains record. If not, do nothing!

   if ( c != 0 )
   {
//    if ( flag & MK_LBUTTON )
//    {
         if ( control -> styleex & ITCS_CHECKBOXES )
         {
            if ( ( me.GetX () >= 0 ) && ( me.GetX () < CHECKBOXWIDTH ) )
            {
               // Check box is being clicked! Read the record associated with the checkbox!
               
//             record   -> CursorSet ( c );
               
               wxTableEvent   te ( control, wxEVT_COMMAND_TABLE_CHECKBOX_CLICKED, 0, down, table, record, c, IsFocus ( c ) );

               ProcessEvent   ( te );

               if ( te.IsAllowed () )
               {
                  wxClientDC  dc       ( this );
                  wxTable :: SetAdapter &
                              sa       = table -> Check ();
                  bool        checked  = false;

                  if ( sa.IsActive () )
                  {
                     sa.Toggle   ( c );
                     checked  = sa.Find ( c );
                  }
                  else
                     checked  = te.IsChecked ();
                     
//?               dc.DrawState   ( iPoint ( 0, row * text_height ), iSize ( 16, 16 ), ( table -> Check ().Find ( visible [ row ] ) ) ? checkbox_checked : checkbox_unchecked, DST_BITMAP | DSS_NORMAL );
//                DrawState   ( &dc, wxPoint ( 0, row * text_height ), wxSize ( 16, 16 ), checked ? checkbox_checked : checkbox_unchecked, DST_BITMAP | DSS_NORMAL );
                  DrawCheckBox   ( &dc, wxRect ( 0, row * text_height, 16, 16 ), checked );
               }

               return;
            }
         }

         if ( control -> styleex & ITCS_MULTISELECT )
         {
//          if ( flag & MK_SHIFT )
            if ( me.ShiftDown () )
            {
               if ( begin == 0 )
                  SelectRangeStart  ();

               table -> Select   ().All ( false );

               // Select the range.

               SelectRange ( begin, c );

               DoPaint  ();
            }
            else
            {
               if ( begin != 0 )
                  SelectRangeStop   ();

//             if ( flag & MK_CONTROL )
               if ( me.ControlDown () )
               {
                  if ( table -> Select ().Size () == 0 )
                     table -> Select ().Insert ( cursor );
               }
               else
               {
                  if ( ! table -> Select ().Find ( c ) )
                  {
                     table -> Select ().All ( false );

                     if ( ( table -> Select ().Size () > 1 ) || ( ! table -> Select ().Find ( cursor ) ) )
                        DoPaint  ();
                  }
               }
            }
         }

         _CursorTo ( row, me.ControlDown () );
//    }
   }

// super :: OnLButtonDown ( flag, point );
}



void  wxTableCtrl :: Body :: OnLeftUp ( wxMouseEvent &  me )
{
   if ( control -> styleex & ITCS_MULTISELECT )
   {
//    if ( ! ( flag & ( MK_CONTROL | MK_SHIFT ) ) )
      if ( ! ( me.ControlDown () | me.ShiftDown () ) )
      {
         if ( ( move.x < wxSystemSettings :: GetMetric ( wxSYS_DRAG_X ) ) &&
              ( move.y < wxSystemSettings :: GetMetric ( wxSYS_DRAG_Y ) ) &&
              ( table -> Select ().Size () > 1             )    )
         {
            table -> Select   ().All    ( false );
            table -> Select   ().Insert ( cursor );

            DoPaint  ();

            return;
         }
      }
   }

   move  = wxPoint ( 0, 0 );

// iWnd :: OnLButtonUp ( flag, point );
}



void  wxTableCtrl :: Body :: OnMotion ( wxMouseEvent &  me )
{
   if ( me.LeftIsDown () )
   {
      long     x  = labs ( me.GetX () - down.x );
      long     y  = labs ( me.GetY () - down.y );

      if ( move.x < x )
         move.x = x;

      if ( move.y < y )
         move.y = y;

      if ( control -> styleex & ITCS_DRAGANDDROP )
      {
         if ( ( x >= wxSystemSettings :: GetMetric ( wxSYS_DRAG_X ) ) ||
              ( y >= wxSystemSettings :: GetMetric ( wxSYS_DRAG_Y ) )    )
         {
            wxTableEvent   te ( control, wxEVT_COMMAND_TABLE_BEGIN_LEFT_DRAG, 0, me.GetPosition () );

            ProcessEvent   ( te );

            if ( te.IsAllowed () )
            {
               dataobject  -> SetData     ( table, ( ( control -> styleex & ITCS_MULTISELECT ) && ( table -> Select ().Size () > 1 ) ) ? 0 : cursor );

               dropsource  -> SetData     ( *dataobject );
               dropsource  -> DoDragDrop  ();
            }
         }
      }
   }
   else 
      if ( me.Moving () )                 // Moving, no buttons down!
         UpdateToolTip ( me.GetPosition () );

// iWnd :: OnMouseMove ( flag, point );
}



void  wxTableCtrl :: Body :: OnMouseWheel ( wxMouseEvent &  me )
{
   if ( FindFocus () != this )
      SetFocus ();

   if ( control -> styleex & ITCS_VSCROLLER )
      return;
/*
   SCROLLINFO   tmp =
   {
      sizeof ( SCROLLINFO ),
   };

   GetScrollInfo ( SB_VERT, &tmp, SIF_ALL );
*/

   const int   DELTA       = me.GetWheelRotation ();
   const int   WHEELDELTA  = me.GetWheelDelta    ();

   if ( DELTA >= 0 )
   {
      int   scroll   = DELTA / WHEELDELTA;

      record   -> CursorSet ( visible.Top () );
      if ( record -> GetPrvEx ( scroll ) > 0 )
      {
         visible.Top ( *record -> CursorCurrent () );

         DoPaint  ();
      }

      SyncScrollBar ();
   }
   else
   {
      int   scroll   = - DELTA / WHEELDELTA;

      record   -> CursorSet ( visible.Top () );
      if ( record -> GetNxtEx ( scroll ) > 0 )
      {
         visible.Top ( *record -> CursorCurrent () );

         DoPaint  ();
      }

      SyncScrollBar ();
   }
}



void  wxTableCtrl :: Body :: OnRightDown ( wxMouseEvent &  me )
{
// iWnd :: OnRButtonDown ( flag, point );
}



void  wxTableCtrl :: Body :: OnRightUp ( wxMouseEvent &  me )
{
   wxTable :: Cursor * 
                  c  = GetCursorAtPoint ( me.GetPosition () );
   
   wxTableEvent   te ( control, wxEVT_COMMAND_TABLE_RECORD_RIGHT_CLICK, 0, me.GetPosition (), table, record, c, IsFocus ( c ) );
   
   ProcessEvent   ( te );
}



void  wxTableCtrl :: Body :: OnCommandInsert ()
{
// control  -> insertcallback.Perform  ( this );
   control  -> CommandInsert  ();
}



void  wxTableCtrl :: Body :: OnCommandUpdate ()
{
// control  -> updatecallback.Perform  ( this );
   control  -> CommandUpdate  ();
}



void  wxTableCtrl :: Body :: OnCommandDelete ()
{
// iMessageBox :: Report ( iMessageBox :: ButtonOk, __FUNC__, GetSafeHwnd (), "Delete" );
// control  -> deletecallback.Perform  ( this );
   control  -> CommandDelete  ();
}



void  wxTableCtrl :: Body :: OnCommandCopy ()
{
// control  -> copycallback.Perform ( this );
   control  -> CommandCopy ();
}



void  wxTableCtrl :: Body :: OnCommandFind ()
{
// control  -> findcallback.Perform ( this );
   control  -> CommandFind ();
}



void  wxTableCtrl :: Body :: OnCommandSearch ()
{
// control  -> Search ();
   control  -> CommandSearch  ();
}



void  wxTableCtrl :: Body :: OnCommandExchange ( wxCommandEvent &  ce )
{
   switch ( ce.GetId () )
   {
      case IDM_EXCHANGE_FAST     :
         mode  = Mode_FAST;

         break;

      case IDM_EXCHANGE_MERGE    :
         mode  = Mode_MERGE;

         break;

      case IDM_EXCHANGE_ROLL     :
         mode  = Mode_ROLL;

         break;

      case IDM_EXCHANGE_SQUASH   :
         mode  = Mode_SQUASH;

         break;
   }
}



void  wxTableCtrl :: Body :: OnCommandSort ( UINT  id )
{
   if ( control != 0 )
      control -> OnCommandSort ( id );
}



void  wxTableCtrl :: Body :: OnCommandSubTable ( UINT  id )
{
   if ( control != 0 )
      control -> OnCommandSubTable ( id );
}



void  wxTableCtrl :: Body :: OnNoSort ()
{
   if ( control != 0 )
      control -> OnNoSort ();
}



///////////////////////////////////////////////////////////////////////////////
// Check if 'cursor' is valid and visible. If it is, return and let the      //
// calling member (OnKeyDown) handle the movement.                           //
// If 'cursor' is valid, but not visible. Make it visible, but prevent the   //
// calling member (OnKeyDown) from performing any movement.                  //
// If 'cursor' is not valid, make a valid and visible cursor and prevent the //
// calling member (OnKeyDown) from performing any movement.                  //
///////////////////////////////////////////////////////////////////////////////



bool  wxTableCtrl :: Body :: KeyCursor ( const wxKeyEvent &  ke )
{
   if ( table == 0 )
      return ( true );

   if ( cursor != 0 )
   {
      if ( control -> styleex & ITCS_MULTISELECT )
      {
//       if ( GetKeyState ( WXK_SHIFT ) & 0x8000 )
         if ( ke.ShiftDown () )
         {
            if ( begin != 0 )
            {
               ;
            }
            else
            {
               if ( (   table -> Select ().Size () == 0         ) ||
                    ( ( table -> Select ().Size () == 1    ) &&
                      ( table -> Select ().Find ( cursor ) )    )    )
               {
                  SelectRangeStart  ();

                  table -> Select   ().Insert ( begin );
               }
               else
               {
                  table -> Select   ().All    ( false );

                  SelectRangeStart  ();

                  table -> Select   ().Insert ( begin );

//                DoPaint  ();
               }
            }
         }
         else
         {
            if ( begin != 0 )
               SelectRangeStop   ();

            table -> Select   ().All ( false );

            DoPaint  ();
         }
      }

      // Is 'cursor' valid?

      if ( cursor -> IsValid () )
      {
         // Is 'cursor' visible?

         if ( visible.IndexOf ( *cursor ) != wxTable :: CursorVector :: npos )
         {
            // Should be able to 'Set' to cursor.
            // If so, return false so the calling member handles the movement.

            if ( record -> CursorSet ( cursor ) == wxTable :: Record :: Result_OK )
               return ( false );
         }
         else
         {
            // Make 'cursor' visible if possible.

            if ( record -> CursorSet ( cursor ) == wxTable :: Record :: Result_OK )
            {
               visible.Top ( *cursor );

               wxClientDC  dc ( this );

               DoPaintDC     ( &dc );

               SyncScrollBar ();

               return ( true );
            }
         }
      }

      *cursor = *visible.Top ();

      if ( cursor -> IsValid () )
      {
         visible.Top ( *cursor );

         wxClientDC  dc ( this );

         DoPaintDC      ( &dc );

         SyncScrollBar  ();               // Synchronize the scrollbar.
         RecordFocused  ( wxDefaultPosition );
         RelationSync   ();

         return ( true );
      }
      else
         return ( true );
   }

   return ( false );
}



bool  wxTableCtrl :: Body :: ProcessEvent ( const wxTableEvent &  _te )
{
   wxTableEvent   te ( _te );
   
   return ( super :: ProcessEvent ( te ) );
}



bool  wxTableCtrl :: Body :: ProcessEvent ( wxTableEvent &  _te )
{
   return ( super :: ProcessEvent ( _te ) );
}



bool  wxTableCtrl :: Body :: RecordFocused ( const wxPoint &  point )
{
   return ( ProcessEvent ( wxTableEvent ( control, wxEVT_COMMAND_TABLE_RECORD_FOCUSED, 0, point, table, record, cursor, true ) ) );
}



void  wxTableCtrl :: Body :: CursorCurrent ()
{
   *cursor  = *record   -> CursorCurrent ();
}



void  wxTableCtrl :: Body :: RelationSync ()
{
#if 0
//?if ( control -> oncursorchange.IsSet () )
//?   control -> oncursorchange.Perform ( control );

   if ( control -> relation == 0 )
      return;

#ifdef __JAK_TABLE_RELATION__
   if ( table -> CursorSet ( cursor, record ) == wxTable :: Result_OK )
      table -> RelationSync       ();
   else
      table -> RelationDeactivate ();
#endif

   for ( iWindowSet :: Iterator  i = control -> relation -> Begin () ; i != control -> relation -> End () ; i++ )
   {
      wxTableCtrl    *tc = dynamic_cast < wxTableCtrl * > ( *i );

      if ( tc != 0 )
         tc -> Refresh ( ITCF_REFRESH_RELATION );
   }
#endif
}



void  wxTableCtrl :: Body :: UpdateToolTip ( const wxPoint &  point )
{
   if ( tooltip == 0 )
      return;

   if ( text_height == 0 )
      return;
            
   const size_t            index = point.y / text_height;
   
   // See if the cursor is on a different row!
   
   if ( index == static_cast < size_t > ( ( tooltippoint.y / text_height ) ) )
      return;
      
   tooltippoint   = point;
   
   wxTable :: Cursor *     tmp   = visible [ index ];
   
   if ( ( tmp != 0 ) && ( tmp -> IsValid () ) )
   {
      const int   COLUMNS  = table -> NumberOfColumns ();
      int         count    = 0;
      wxString    text;

      record   -> CursorSet   ( tmp );
      record   -> Synchronize ();

      text.Empty  ();
      
      for ( int  i = 0 ; i < COLUMNS ; ++i )
      {
         if ( (   table -> FieldFlag ( i ) &  TFF_SHOW_TOOLTIP      ) &&
              ( ( table -> FieldType ( i ) == TFT_NULL         ) ||
                ( table -> FieldType ( i ) == TFT_TEXT         )    )    )
         {
            if ( count > 0 )
               text += "\n";

            text += table -> HeaderString   ( i );
            text += " : ";
            text += table -> FieldString    ( i );

            ++count;
         }
      }

      tooltip  -> SetWindow   ( 0 );
      tooltip  -> SetTip      ( text );
      tooltip  -> SetWindow   ( this );

      record   -> CursorSet   ( cursor );    // Restore cursor!
      record   -> Synchronize ();
   }
   else
      tooltip  -> SetTip   ( wxEmptyString );
}



wxTableCtrl :: Body :: Body ( wxWindow *  _window, wxWindowID  _id, const wxPoint &  _point, const wxSize &  _size, long  _style, const wxString &  _title, Header *  _header )
   : wxWindow ( _window, _id, _point, _size, _style | wxWANTS_CHARS | wxHSCROLL | wxVSCROLL, _title )
{
   control              = dynamic_cast < wxTableCtrl * > ( _window );

   table                =  0;
   cursor               =  0;
   header               = _header;
   mode                 = Mode_DEFAULT;

   font                 = new  wxFont ( wxSystemSettings :: GetFont ( wxSYS_DEFAULT_GUI_FONT ) );

   text_top             =  2;             // Via Property some day.
   text_bottom          =  2;
   text_left            =  2;
   text_right           =  2;

   down                 = wxPoint ( -1, -1 );   // Don't know yet where the cursor is.
   move                 = wxPoint (  0,  0 );   // Not moved yet either!

// left                 =  0;             // Not horizontal scrolled yet!
   text_height          =  0;             // Calculated.
   cursor_row           = nrow;           // No active cursor yet.
   focus                = false;          // Doesn't have the focus yet!
// keydown              =  0;             // No <Ctrl> <Shift> or <Alt> down yet!
   begin                =  0;             // No range selection active yet!
   end                  =  0;

   tooltip              =  0;
   tooltippoint         = wxDefaultPosition;

   foregroundcolor      = wxSystemSettings :: GetColour ( wxSYS_COLOUR_WINDOWTEXT );
   backgroundcolor      = wxSystemSettings :: GetColour ( wxSYS_COLOUR_WINDOW );

   dropsource           = new  wxDropSource  ( this );
   dataobject           = new  DataObject    ();
#if 0
#ifdef __WXMSW__
   checkbox_unchecked   = new  wxBitmap ( ResourceNameString ( RNS_TABLE_CHECKBOX_UNCHECKED ) );
   checkbox_checked     = new  wxBitmap ( ResourceNameString ( RNS_TABLE_CHECKBOX_CHECKED   ) );
#else
   checkbox_unchecked   = 0;
   checkbox_checked     = 0;
#endif
#endif
   lock                 = false;
// fill                 = Fill_NORMAL;
   fill                 = Fill_RESET;

   SetCursor      ( *wxSTANDARD_CURSOR );
}



wxTableCtrl :: Body :: ~Body ()
{
   delete  dataobject;
   delete  dropsource;

   delete  end;
   delete  begin;
   delete  cursor;

   delete  font;
}



void  wxTableCtrl :: Body :: Exchange ( const wxRect &  _r0, const wxRect &  _r1 )
{
   wxRect         rect     = GetClientRect   ();
   const int      DX       = ( control -> styleex & ITCS_CHECKBOXES ) ? CHECKBOXWIDTH : 0;
// wxRect         r0       = _r0 - wxPoint ( control -> left - DX, 0 );  // Count with horizontal scrolling!
   wxRect         r0       ( _r0.GetX () - control -> left + DX, _r0.GetY (), _r0.GetWidth  (), _r0.GetHeight () );
// wxRect         r1       = _r1 - wxPoint ( control -> left - DX, 0 );
   wxRect         r1       ( _r1.GetX () - control -> left + DX, _r1.GetY (), _r1.GetWidth  (), _r1.GetHeight () );
   bool           refresh  = false;

   // Limit the size of the rectangles to what is available in the client
   // area of the 'Body' window!

   if ( r0.GetX () < rect.GetX () )
   {
      r0.SetX ( rect.GetX () );

      refresh = true;
   }
   if ( r0.GetRight () > rect.GetRight () )
   {
      r0.SetRight ( rect.GetRight () );

      refresh = true;
   }

   if ( r1.GetX () < rect.GetX () )
   {
      r1.SetX ( rect.GetX () );

      refresh = true;
   }
   if ( r1.GetRight () > rect.GetRight () )
   {
      r1.SetRight ( rect.GetRight () );

      refresh = true;
   }



   wxRect         ax;
   wxRect         a0          ( r0 );        // Area
   wxRect         a1          ( r1 );        // Area
   long           m0          = 0;           // Move of drag area.
   long           m1          = 0;           // Move of counter area.
   long           w1          = 0;           // Width of counter area.

   // Determine the rect's or area's that should be moved. It is very
   // important to realize that 'r0' (which define's the rectangle of the
   // dragged header) and 'r1' (which define's the rectangle of the header
   // it is going to use to take it's location do not necessarily have to
   // be adjacent. So depending on how the rectangles relate to each other
   // the area's that will be moved are determined.

   if ( r0.GetX () < r1.GetX () )            // Move right.
   {
      m0 = _r1.GetX () + _r1.GetWidth () - _r0.GetWidth () - _r0.GetX ();
      m1 = - _r0.GetWidth ();

      w1 = r1.GetRight () - r0.GetRight ();

      a1 = wxRect ( wxPoint ( r0.GetX () + r0.GetWidth (), rect.GetY () ), wxSize ( w1                                      , rect.GetHeight () ) );

      ax = wxRect ( wxPoint ( r0.GetX ()                 , rect.GetY () ), wxSize ( r1.GetX () + r1.GetWidth () - r0.GetX (), rect.GetHeight () ) );
   }
   else                                      // Move left.
   {
      m0 = _r1.GetX () - _r0.GetX ();
      m1 = _r0.GetWidth ();

      w1 = r0.GetX () - r1.GetX ();

      a1 = wxRect ( wxPoint ( r1.GetX (), rect.GetY () ), wxSize ( w1                                      , rect.GetHeight () ) );

      ax = wxRect ( wxPoint ( r1.GetX (), rect.GetY () ), wxSize ( r0.GetX () + r0.GetWidth () - r1.GetX (), rect.GetHeight () ) );
   }

   a0 = wxRect ( wxPoint ( r0.GetX (), rect.GetY () ), wxSize ( r0.GetWidth (), rect.GetHeight () ) );

   // Create a wxBitmapDC (copy) of the actual window contents as it is supposed 
   // to be. This is necessary for the rare case that the Windows Task Manager
   // or an other window is visible and over one of the rectangles moved! In 
   // such cases a copy of the actual screen would be taken! Creating a 
   // wxBitmapDC and performing a DoPaintDC (to create an in memory copy of the 
   // content prevents this from happening.
   
   wxClientDC  dc    ( this );
   wxBitmapDC  bdc   ( &dc, rect.GetSize () );
   
   bdc.SetWindow  ( dc.GetWindow () );
   
   DoPaintDC ( &bdc );

   if ( ( cursor_row = visible.IndexOf ( *cursor ) ) != wxTable :: CursorVector :: npos )
      DoFocusRect ( &dc, cursor_row );

   // Output Device Context.

   wxBitmapDC     cdx   ( &dc, ax.GetWidth (), ax.GetHeight () );

   // Bitmaps for the two defined area's.

   wxBitmapDC     cd0   ( &dc, a0.GetWidth (), a0.GetHeight () );
   wxBitmapDC     cd1   ( &dc, a1.GetWidth (), a1.GetHeight () );

   cd0.Blit   ( 0                    , 0        , a0.GetWidth (), a0.GetHeight (), &bdc , a0.GetX (), a0.GetY (), wxCOPY );

   cd1.Blit   ( 0                    , 0        , a1.GetWidth (), a1.GetHeight (), &bdc , a1.GetX (), a1.GetY (), wxCOPY );

   switch ( mode )
   {
      default          :
      case Mode_FAST   :
         break;

      case Mode_MERGE  :
         {
            const int    STEP = wxMin ( abs ( m0 ), abs ( m1 ) );

            for ( int  s = 0 ; s < STEP ; ++s )
            {
               cdx.Blit   ( 0                                      , 0         , ax.GetWidth (), ax.GetHeight (), &cdx, 0, 0, wxSET  );

               cdx.Blit   ( a0.GetX () - ax.GetX () + m0 * s / STEP, 0         , a0.GetWidth (), a0.GetHeight (), &cd0, 0, 0, wxCOPY );

               cdx.Blit   ( a1.GetX () - ax.GetX () + m1 * s / STEP, 0         , a1.GetWidth (), a1.GetHeight (), &cd1, 0, 0, wxAND  );

               dc .Blit   ( ax.GetX ()                             , ax.GetY (), ax.GetWidth (), ax.GetHeight (), &cdx, 0, 0, wxCOPY );

               wxMilliSleep   (   1 );
            }
         }

         break;

      case Mode_ROLL   :
         {
            const int    STEP = abs ( m0 );

            for ( int   s = 0 ; s < STEP ; s++ )
            {
               int   dm = m0 * s / STEP;

               cdx.Blit   ( 0                           , 0, ax.GetWidth (), ax.GetHeight (), &cdx, 0, 0, wxSET  );

               cdx.Blit   ( a0.GetX () - ax.GetX () + dm, 0, a0.GetWidth (), a0.GetHeight (), &cd0, 0, 0, wxCOPY );

               if ( m0 > 0 )              // Roll right.
               {
                  if ( dm                      > 0 )
                     cdx.Blit   ( 0                           , 0,                  dm, a1.GetHeight (), &cd1, a1.GetWidth () - dm, 0, wxAND  );
                  if ( ( a1.GetWidth () - dm ) > 0 )
                     cdx.Blit   ( a1.GetX () - ax.GetX () + dm, 0, a1.GetWidth () - dm, a1.GetHeight (), &cd1,  0                 , 0, wxAND  );
               }
               else                       // Roll left.
               {
                  if ( ( a1.GetWidth () + dm ) > 0 )
                     cdx.Blit   ( 0                                            , 0, a1.GetWidth () + dm, a1.GetHeight (), &cd1, abs ( dm )  , 0, wxAND  );
                  if ( abs ( dm )              > 0 )
                     cdx.Blit   ( a0.GetX () + a0.GetWidth () - ax.GetX () + dm, 0, abs ( dm )         , a1.GetHeight (), &cd1,  0          , 0, wxAND  );
               }

               dc .Blit   ( ax.GetX (), ax.GetY (), ax.GetWidth (), ax.GetHeight (), &cdx, 0, 0, wxCOPY );

               wxMilliSleep (   1 );
            }
         }

         break;

      case Mode_SQUASH :
#if 0 // Still to do...
         {
            const int    STEP = i_min ( abs ( m0 ), abs ( m1 ) );

            for ( int  s = 0 ; s < STEP ; s++ )
            {
               cdx.BitBlt ( 0      , 0      , ax.W (), ax.H (), 0   , 0, 0, WHITENESS );

               bdc.SelectObject ( b0 );
               cdx.BitBlt ( a0.X () - ax.X () + m0 * s / STEP, 0, a0.Width (), a0.Height (), &bdc, 0      , 0        , SRCCOPY    );

               bdc.SelectObject ( b1 );
               if ( m0 > 0 )
               {
                  int   dm    = m0 * s / STEP;
                  int   start = 0;
                  int   width =

                  while ( start < a1.Width () )
                  {
                     cdx.BitBlt ( a1.X () - ax.X () + dm, 0, width, a1.Height (), &bdc, start, 0, SRCAND );
                     start += dm;
                  }
               }
               else
               {
               }
               cdx.BitBlt ( a1.X () - ax.X () + m1 * s / STEP, 0, a1.Width (), a1.Height (), &bdc, 0      , 0        , SRCAND     );

               dc .BitBlt ( ax.X (), ax.Y (), ax.W (), ax.H (), &cdx, 0, 0, SRCCOPY );

   //          Sleep ( 500 );
   //          Sleep (   5 );
            }
         }
#endif
         break;

   }

   dc .Blit   ( a0.GetX () + m0, a0.GetY (), a0.GetWidth (), a0.GetHeight (), &cd0, 0      , 0        , wxCOPY );

   dc .Blit   ( a1.GetX () + m1, a1.GetY (), a1.GetWidth (), a1.GetHeight (), &cd1, 0      , 0        , wxCOPY );

   if ( cursor_row >= 0 )
      DoFocusRect ( &dc, cursor_row );

// cdx.SelectObject ( prev_bitmapx );
// cdx.DeleteDC     ();
// bdc.SelectObject ( prev_bitmapc );
// bdc.DeleteDC     ();

   if ( refresh )
      DoPaint ();

/* Original code, works great except that it also copies parts outside the ClientRect.

   if ( r0.X () > r1.X () )
   {
      r0 = _r1;
      r1 = _r0;
   }

   bdc.CreateCompatibleDC        ( &dc );
   bitmap.CreateCompatibleBitmap ( &dc, r0.Width (), rect.Height () );

   prev_bitmap = bdc.SelectObject ( &bitmap );

   bdc.BitBlt ( 0                    , 0        , r0.Width (), rect.Height (), &dc , r0.X (), rect.Y (), SRCCOPY );

   dc .BitBlt ( r0.X ()              , rect.Y (), r1.Width (), rect.Height (), &dc , r1.X (), rect.Y (), SRCCOPY );

   dc .BitBlt ( r0.X () + r1.Width (), rect.Y (), r0.Width (), rect.Height (), &bdc, 0      , 0        , SRCCOPY );

   bdc.SelectObject ( prev_bitmap );
   bdc.DeleteDC     ();
*/
}



void  wxTableCtrl :: Body :: Table ( wxTable *  _table )
{
   if ( ( table = _table ) != 0 )
   {
      record   = table -> GetRecord ();

      if ( cursor != 0 )
         delete  cursor;

      cursor   = record -> CursorCreate  ();
      
      visible  .Clear   ();
      
      if ( control -> styleex & ITCS_MULTISELECT )
         if ( table -> Select ().Size () == 0 )
            table -> Select ().Insert ( cursor );
   }
   else
   {
      record   = 0;

      if ( cursor != 0 )
         delete  cursor;

      cursor   = 0;

      visible  .Clear   ();
      
      if ( control -> styleex & ITCS_MULTISELECT )
         table -> Select ().All ( false );
   }

   DoSize ( true );
}



wxTable *  wxTableCtrl :: Body :: Table () const
{
   return ( table );
}



void  wxTableCtrl :: Body :: SetForegroundColor ( const wxColor &  _foregroundcolor )
{
   foregroundcolor   = _foregroundcolor;
}



void  wxTableCtrl :: Body :: SetBackgroundColor ( const wxColor &  _backgroundcolor )
{
   backgroundcolor   = _backgroundcolor;
}



const bool  wxTableCtrl :: Body :: GetToolTip () const
{
   return ( tooltip != 0 );
}



void  wxTableCtrl :: Body :: SetToolTip ( const bool &  flag )
{
   if ( flag )
   {
      if ( tooltip == 0 )
         super :: SetToolTip ( tooltip = new  wxToolTip ( wxEmptyString ) );
   }
   else
      super :: SetToolTip ( tooltip = 0 );
}



bool  wxTableCtrl :: Body :: Lock () const
{
   return ( lock );
}



void  wxTableCtrl :: Body :: Lock ( bool  _lock )
{
   lock  = _lock;
}



void  wxTableCtrl :: Body :: SetFill ( const Fill &  _fill )
{
   fill  = _fill;
}



void  wxTableCtrl :: Body :: DoPaintSort ()
{
   wxClientDC     dc ( this );

   DoPaintDC     ( &dc, cursor_row );
   SyncScrollBar ();
}



void  wxTableCtrl :: Body :: DoPaintFill ()
{
   wxClientDC     dc ( this );

   DoPaintDC     ( &dc, cursor_row, Fill_RESET );
   SyncScrollBar ();
}



bool  wxTableCtrl :: Body :: Get ( const wxTable :: Record :: Find &  find )
{
   if ( record -> Get ( find ) == wxTable :: Record :: Result_OK )
   {
      *cursor = *record -> CursorCurrent ();

      DoPaintFill    ();
      RelationSync   ();

      return ( true );
   }

   return ( false );
}



bool  wxTableCtrl :: Body :: Get ( wxTable :: Cursor *  cursor )
{
   if ( record -> CursorSet ( cursor ) == wxTable :: Record :: Result_OK )
   {
      DoPaintFill    ();
      RelationSync   ();

      return ( true );
   }

   return ( false );
}



bool  wxTableCtrl :: Body :: GetFst ()
{
   if ( record -> GetFst () == wxTable :: Record :: Result_OK )
   {
      *cursor = *record -> CursorCurrent ();

      DoPaintFill    ();
      RelationSync   ();

      return ( true );
   }

   return ( false );
}



bool  wxTableCtrl :: Body :: GetLst ()
{
   if ( record -> GetLst () == wxTable :: Record :: Result_OK )
   {
      *cursor = *record -> CursorCurrent ();

      DoPaintFill    ();
      RelationSync   ();

      return ( true );
   }

   return ( false );
}



bool  wxTableCtrl :: Body :: Reset ()
{
   cursor   -> Invalidate        ();
   record   -> CursorInvalidate  ();
   
   DoPaintFill    ();
   RelationSync   ();
   
   return ( true );
}



bool  wxTableCtrl :: Body :: InsertRecord ()
{
   wxTableEvent   te ( control, wxEVT_COMMAND_TABLE_INSERTING, 0, wxDefaultPosition, table, record, record -> CursorCurrent (), false );
   
   ProcessEvent   ( te );

   if ( ! te.IsAllowed () )
      return ( false );
      
   if ( record -> Insert () == wxTable :: Record :: Result_OK )
   {
      *cursor = *record -> CursorCurrent ();

      DoPaintFill    ();
      RelationSync   ();
 
      ProcessEvent   ( wxTableEvent ( control, wxEVT_COMMAND_TABLE_INSERTED, 0, wxDefaultPosition, table, record, cursor, false ) );

      return ( true );
   }

   return ( false );
}



bool  wxTableCtrl :: Body :: UpdateRecord ()
{
   wxTableEvent   te ( control, wxEVT_COMMAND_TABLE_UPDATING, 0, wxDefaultPosition, table, record, record -> CursorCurrent (), false );
   
   ProcessEvent   ( te );

   if ( ! te.IsAllowed () )
      return ( false );
      
   if ( record -> Update () == wxTable :: Record :: Result_OK )
   {
      *cursor = *record -> CursorCurrent ();

      DoPaintFill    ();
      RelationSync   ();

      ProcessEvent   ( wxTableEvent ( control, wxEVT_COMMAND_TABLE_UPDATED, 0, wxDefaultPosition, table, record, cursor, false ) );

      return ( true );
   }

   return ( false );
}



bool  wxTableCtrl :: Body :: DeleteRecord ()
{
   wxTableEvent   te ( control, wxEVT_COMMAND_TABLE_DELETING, 0, wxDefaultPosition, table, record, record -> CursorCurrent (), false );
   
   ProcessEvent   ( te );

   if ( ! te.IsAllowed () )
      return ( false );
      
// iMessageBox :: Report ( iMessageBox :: ButtonOk, __FUNC__, GetSafeHwnd (), "Delete" );
   bool     result   = false;

   if ( ( control -> styleex & ITCS_MULTISELECT ) && ( table -> Select ().Size () != 0 ) )
   {
      const wxTable :: Cursor *     current  = record -> CursorCurrent ();
      int                           deleted  = 0;
      int                           remained = 0;
      const wxTable :: Cursor *     c        = table -> Select ().FindFirst   ();

      while ( c != 0 )
      {
         record -> CursorSet   ( c );

         if ( record -> Delete () == wxTable :: Record :: Result_OK )
            ++deleted;
         else
            ++remained;

         c  = table -> Select ().FindNext ( c );
      }

      record   -> CursorSet ( current );

      result   = ( deleted > 0 );
   }
   else
      result   = ( record -> Delete () == wxTable :: Record :: Result_OK );



   if ( result )
   {
      if ( record -> GetNxt () == wxTable :: Record :: Result_OK )
         *cursor = *record -> CursorCurrent ();
      else
         if ( record -> GetPrv () == wxTable :: Record :: Result_OK )
         {
            *cursor = *record -> CursorCurrent ();

            if ( cursor_row > 0 )
               --cursor_row;
         }
         else
            cursor -> Invalidate ();

      DoPaintFill    ();
      RelationSync   ();

      ProcessEvent   ( wxTableEvent ( control, wxEVT_COMMAND_TABLE_DELETED, 0, wxDefaultPosition, table, record, cursor, false ) );

      return ( true );
   }

   return ( false );
}



bool  wxTableCtrl :: Body :: CopyRecord ()
{
   wxTableEvent   te ( control, wxEVT_COMMAND_TABLE_COPYING, 0, wxDefaultPosition, table, record, record -> CursorCurrent (), false );
   
   ProcessEvent   ( te );

   if ( ! te.IsAllowed () )
      return ( false );
      
   if ( record -> Insert () == wxTable :: Record :: Result_OK )
   {
      *cursor = *record -> CursorCurrent ();

      DoPaintFill    ();
      RelationSync   ();

      ProcessEvent   ( wxTableEvent ( control, wxEVT_COMMAND_TABLE_COPIED, 0, wxDefaultPosition, table, record, cursor, false ) );

      return ( true );
   }

   return ( false );
}



bool  wxTableCtrl :: Body :: Find ( const bool &  ge )
{
   record   -> SetIndex ( control -> index );

   if ( (   record -> Get  ( wxTable :: Record :: Find_LE ) == wxTable :: Record :: Result_OK      ) ||
        ( ( ge                                                                                ) &&
          ( record -> Get  ( wxTable :: Record :: Find_GE ) == wxTable :: Record :: Result_OK )    )    )
   {
      *cursor = *record -> CursorCurrent ();

      DoPaintFill    ();
      RelationSync   ();
   }

   return ( true );
}



bool  wxTableCtrl :: Body :: Search ( const wxString &  text, const bool &  match_case, const bool &  direction, const bool &  first, const bool &  wrap )
{
   if ( ! _DoSearch ( text, match_case, direction, first ) && ( wrap ) )
   {
      if ( direction )
         record   -> GetFst ();
      else
         record   -> GetLst ();

      if ( _DoSearch ( text, match_case, direction, true ) )
         return ( true );
   }

   record   -> CursorSet ( cursor );

   DoPaintFill ();

   return ( false );
}



long  wxTableCtrl :: Body :: HitTest ( const wxPoint &  point ) const
{
   const long                 r  = point.y / text_height;
   const wxTable :: Cursor *  c  = visible [ r ];
   
   if ( c == 0 )
      return ( -1 );
      
   if ( ! c -> IsValid () )
      return ( -1 );
      
   return ( r );
}



bool  wxTableCtrl :: Body :: CursorTo ( long  row )
{
   return ( _CursorTo ( row, false ) );
}



void  wxTableCtrl :: Body :: Refresh ( const wxTable :: Cursor &  cursor )
{
   const size_t   row   = visible.IndexOf ( cursor );
   
   if ( row == nrow )
      return;
      
   DoPaintLine ( row );
}



///////////////////////////////////////////////////////////////////////////////
// wxTableCtrl                                                                //
///////////////////////////////////////////////////////////////////////////////



const UINT              IDM_CHECK_TOGGLE  = 1101;
const UINT              IDM_CHECK_ALL     = 1102;
const UINT              IDM_CHECK_NONE    = 1103;
const UINT              IDM_CHECK_INVERT  = 1104;



const long              wxTableCtrl :: DEFAULTSTYLE  = 0;



IMPLEMENT_DYNAMIC_CLASS ( wxTableCtrl  , wxControl )



BEGIN_EVENT_TABLE ( wxTableCtrl  , wxControl )
   EVT_ERASE_BACKGROUND ( wxTableCtrl :: OnEraseBackground   )
   EVT_PAINT            ( wxTableCtrl :: OnPaint             )
   EVT_SET_FOCUS        ( wxTableCtrl :: OnSetFocus          )
   EVT_SIZE             ( wxTableCtrl :: OnSize              )

   EVT_KEY_DOWN         ( wxTableCtrl :: OnKeyDown           )

   EVT_LEFT_DCLICK      ( wxTableCtrl :: OnLeftDClick        )
   EVT_LEFT_DOWN        ( wxTableCtrl :: OnLeftDown          )
   EVT_LEFT_UP          ( wxTableCtrl :: OnLeftUp            )

   EVT_MOTION           ( wxTableCtrl :: OnMotion            )

   EVT_RIGHT_UP         ( wxTableCtrl :: OnRightUp           )

   EVT_TIMER            ( 1, wxTableCtrl :: OnTimer          )
END_EVENT_TABLE   ()



void  wxTableCtrl :: Refresh ( bool  erasebackground, const wxRect *  rect )
{
   super :: Refresh ( erasebackground, rect );
}



void  wxTableCtrl :: DoSize ()
{
   const wxRect   rect    = GetClientRectEx ();
   long           height;

   if ( head.IsEmpty () )                 // Not used before?
   {
      wxClientDC     dc ( this );

      height         = dc.GetCharHeight   () + 8;
      min_height     = height                - 4;
   }
   else
      height = head.GetHeight ();         // Use head height, it might be
                                          // resized!

   head  = wxRect ( rect.GetX (), rect.GetY (), rect.GetWidth (), height );

   wxRect   r  ( rect.GetX (), rect.GetY () + height, rect.GetWidth (), rect.GetHeight () - height );

   if ( styleex & ITCS_VSCROLLER )
   {
      int   w  = wxSystemSettings :: GetMetric ( wxSYS_VSCROLL_X );

      r.SetWidth ( r.GetWidth () - w );

//    scroller -> MoveWindow ( wxRect ( rect.W () - w, rect.Y () + height, w, rect.H () - height ), FALSE );
   }

   if ( nativeheader != 0 )
      nativeheader -> SetSize ( head.GetSize () );
      
   if ( body != 0 )
      body -> SetSize ( r );
//    body -> MoveWindow  ( r, FALSE );
}



void  wxTableCtrl :: DrawBackground ( wxDC * )
{
#if 0
#ifdef wxUSE_UXTHEME

   wxUxThemeHandle      theme    ( this, L"ListView" );
   wxUxThemeEngine *    engine   = wxUxThemeEngine :: Get ();
   
   if ( engine -> IsThemeBackgroundPartiallyTransparent ( theme, LVP_LISTSORTEDDETAIL, state ) )
      engine -> DrawThemeParentBackground ( GetHWND (), dc -> GetHDC (), rect );
      
#endif
#endif
}



void  wxTableCtrl :: DrawFocusRect ( wxDC  *dc, const bool &  focus )
{
   if ( styleex & ITCS_FOCUSRECT )
   {
      wxRect         rect  = GetClientRect   ();
      const wxPen &  prev  = dc -> GetPen    ();

      dc -> SetPen ( wxPen ( focus ? *wxBLACK : *wxWHITE, FOCUSRECTTHICK, wxSOLID ) );

      rect.Deflate   ( 1, 1 );

      dc -> DrawRectangle  ( 0, 0, rect.GetWidth (), rect.GetHeight () );

//    dc -> MoveTo         ( 0        , 0         );
//    dc -> LineTo         ( rect.W (), 0         );
//    dc -> LineTo         ( rect.W (), rect.H () );
//    dc -> LineTo         ( 0        , rect.H () );
//    dc -> LineTo         ( 0        , 0         );

      dc -> SetPen ( prev );
   }
}



void  wxTableCtrl :: DrawFocusRect ( bool  focus )
{
   wxClientDC     dc ( this );

   DrawFocusRect  ( &dc, focus );
}



void  wxTableCtrl :: DrawSortArrow ( wxDC *  dc, const Column :: State &  state, const wxTable :: Record :: Sequence &  sort, const wxRect &  rect, const bool &  textright )
{
   wxPoint        point  ( textright ? 4 : rect.GetWidth () - 14, rect.GetY () + rect.GetHeight () / 4 - 1 );
   const int      h      = rect.GetHeight () / 2;
   const int      b      = 7;
   const int      b2     = b / 2;
   const wxPen *  light  = &white;
   const wxPen *  shadow = &gray;
   const wxPen &  prev   = dc -> GetPen ();

   dc -> SetPen ( black );

   switch ( state )
   {
      case Column :: State_UP    :
      default                    :
         light  = &white;
         shadow = &gray;

         break;

      case Column :: State_DOWN  :
         light  = &white;
         shadow = &gray;

         point += wxPoint ( 1, 1 );

         break;

      case Column :: State_MOVE  :
         light  = &white;
         shadow = &black;

         break;
   }

   switch ( sort )
   {
      case wxTable :: Record :: Sequence_ASCENDING    :
         dc -> SetPen       ( *light  );
         dc -> DrawLine     ( point.x + b - b2  , point.y + h     , point.x + b     , point.y         );

         dc -> SetPen       ( *shadow );
         dc -> DrawLine     ( point.x           , point.y         , point.x + b + 1 , point.y         );
         dc -> DrawLine     ( point.x + b2      , point.y + h     , point.x         , point.y         );

         break;

      case wxTable :: Record :: Sequence_DESCENDING   :
         dc -> SetPen       ( *shadow );
         dc -> DrawLine     ( point.x + b2      , point.y         , point.x         , point.y + h     );

         dc -> SetPen       ( *light  );
         dc -> DrawLine     ( point.x + b - b2  , point.y         , point.x + b     , point.y + h     );
         dc -> DrawLine     ( point.x           , point.y + h     , point.x + b + 1 , point.y + h     );

         break;

      case wxTable :: Record :: Sequence_NONE         :
      default                                         :

         break;
   }

   dc -> SetPen ( prev );
}



void  wxTableCtrl :: DrawColumn ( wxDC *  dc, const DrawHeaderFlag &  flag, const Column *  column, wxFont *  font )
{
   if ( dc == 0 )
      return;

   const int               DXY         = ( styleex & ITCS_FOCUSRECT  ) ? FOCUSRECTTHICK : 0;
   const int               DX          = ( styleex & ITCS_CHECKBOXES ) ? CHECKBOXWIDTH  : 0;
// const wxRect            RECT        = column -> Rect     () + wxPoint ( - left + DXY + DX, DXY );
   const wxRect &          R           = column -> Rect     ();
   const wxRect            RECT        ( R.GetX () - left + DXY + DX, R.GetY () + DXY, R.GetWidth  (), R.GetHeight () );
   Column :: State         state       = column -> GetState ();
   wxRect                  rect        = RECT;
// wxFont                 *prev_font   = 0;
   const wxPen *           tl          = &white;
   const wxPen *           right       = &black;
   const wxPen *           bottom      = &black;
   const wxPen &           prev_pen    = dc -> GetPen ();
   int                     bkmode      = wxSOLID;  // OPAQUE;
   wxBitmapDC              bdc         ( dc, rect.GetSize () );

   bdc.SetWindow  ( dc -> GetWindow () );
   
   dc -> SetPen   ( black );

// rect = wxPoint ( 0, 0 );
   rect.SetPosition  ( wxPoint ( 0, 0 ) );

   // Create the right point font.

   if ( font == 0 )
      font = this -> font;

   // Setup a compatible display where the 'Column' will be drawn into first.

// bdc.CreateCompatibleDC        ( dc );
// bitmap.CreateCompatibleBitmap ( dc, rect.Width  (), rect.Height () );

// prev_bitmap = bdc.SelectObject ( &bitmap );
// prev_font   = bdc.SelectObject (  font   );
   bdc.SetFont ( *font );


   switch ( flag )
   {
      default                  :
      case DrawHeaderFlag_INIT :
         bkmode    = wxSOLID;             // OPAQUE;

         break;

      case DrawHeaderFlag_SHOW :
         bkmode    = wxTRANSPARENT;

         break;

      case DrawHeaderFlag_HIDE :
         bkmode    = wxTRANSPARENT;

         break;
   }

   bdc.SetTextBackground   ( wxSystemSettings :: GetColour ( wxSYS_COLOUR_BTNFACE    ) );
   bdc.SetTextForeground   ( wxSystemSettings :: GetColour ( wxSYS_COLOUR_WINDOWTEXT ) );
   bdc.SetBackgroundMode   ( bkmode );

   if ( state != Column :: State_MOVE )
      FillSolidRect ( &bdc, rect, bdc.GetTextBackground () );
   else
      FillSolidRect ( &bdc, rect, *wxWHITE );

   switch ( state )
   {
      case Column :: State_UP    :
      default                    :
         tl     = &white;
         right  = &black;
         bottom = &black;

         break;

      case Column :: State_DOWN  :
         tl     = &gray;
         right  = &gray;
         bottom = &gray;

         break;

      case Column :: State_MOVE  :
         tl     = &black;
         right  = &black;
         bottom = &white;

         break;
   }

   bdc.SetPen     ( *tl     );
   bdc.DrawLine   ( rect.GetX      ()                  , rect.GetY () + rect.GetHeight () - 2, rect.GetX ()                       , rect.GetY ()                         );
   bdc.DrawLine   ( rect.GetX      ()                  , rect.GetY ()                        , rect.GetX () + rect.GetWidth () - 2, rect.GetY ()                         );

   bdc.SetPen     ( *bottom );
   bdc.DrawLine   ( rect.GetX      ()                  , rect.GetY () + rect.GetHeight () - 1, rect.GetX () + rect.GetWidth () - 1, rect.GetY () + rect.GetHeight () - 1 );

   bdc.SetPen     ( *right  );
   bdc.DrawLine   ( rect.GetX () + rect.GetWidth () - 1, rect.GetY ()                        , rect.GetX () + rect.GetWidth () - 1, rect.GetY () + rect.GetHeight ()     );

   bdc.SetPen     (  gray   );
   bdc.DrawLine   ( rect.GetX ()                    + 1, rect.GetY () + rect.GetHeight () - 2, rect.GetX () + rect.GetWidth () - 2, rect.GetY () + rect.GetHeight () - 2 );
   bdc.DrawLine   ( rect.GetX () + rect.GetWidth () - 2, rect.GetY () + rect.GetHeight () - 2, rect.GetX () + rect.GetWidth () - 2, rect.GetY ()                     - 1 );

   // Title.

   const wxTable :: Record :: Sequence    
                           sort        = record -> GetSort ( column -> Reference () );
   wxRect                  text        ( rect );
// const int               ARROWBASE   =  7;
   const int               ARROWWIDTH  = 15;

   if ( ( sort == wxTable :: Record :: Sequence_ASCENDING  ) ||
        ( sort == wxTable :: Record :: Sequence_DESCENDING )    )
   {
      // Sort arrow should be draw at left or right of the text.
      // Sort arrow requires at least 16 points.
      // Free this space from the text rectangle.

      text.SetRight ( text.GetRight () - ARROWWIDTH );

      // If the text is right aligned, move the text rectangle.

      if ( column -> AlignHeader () & DT_RIGHT  )
         text.Offset ( ARROWWIDTH, 0 );            // Move it.
   }

   text.Deflate   ( 4, 4 );

   switch ( state )
   {
      case Column :: State_UP    :
         break;

      case Column :: State_DOWN  :
         text.SetPosition ( wxPoint ( text.GetX () + 1, text.GetY () + 1 ) );

         break;

      case Column :: State_MOVE  :
         text.SetPosition ( wxPoint ( text.GetX () - 2, text.GetY () ) );

         break;
   }

   bool  processed   = false;
   
   if ( drawevents )
      processed   = ProcessEvent ( wxTableEvent ( this, wxEVT_COMMAND_TABLE_DRAW_COLUMN_HEADER, &bdc, column, text, table ) );
      
   if ( ! processed )
   {
      DrawText ( &bdc, column -> Desc (), -1, text, ( column -> AlignHeader () | DT_NOPREFIX | DT_WORDBREAK ) & ~DT_SINGLELINE );
//    wxLogDebug ( "%d,%d-%d,%d %s", text.GetX (), text.GetY (), text.GetWidth (), text.GetHeight (), column -> Name ().c_str () );

//    DrawSortArrow ( &bdc, state, column -> Reference (), rect.Height (), CPoint ( rect.Right  () - rect.Height  () / 2, rect.Top    () + rect.Height  () / 4 ) );
   }

   if ( ( sort == wxTable :: Record :: Sequence_ASCENDING  ) ||
         ( sort == wxTable :: Record :: Sequence_DESCENDING )    )
      DrawSortArrow ( &bdc, state, sort, rect, ( column -> AlignHeader () & DT_RIGHT ) == DT_RIGHT );
   
   switch ( flag )
   {
      case DrawHeaderFlag_INIT :
      default                  :
         dc -> Blit ( RECT.GetX (), RECT.GetY (), RECT.GetWidth  (), RECT.GetHeight (), &bdc, 0, 0, wxCOPY   );

         break;

      case DrawHeaderFlag_SHOW :
         if ( state != Column :: State_MOVE )
            dc -> Blit ( RECT.GetX (), RECT.GetY (), RECT.GetWidth  (), RECT.GetHeight (), &bdc, 0, 0, wxCOPY   );
         else
         {
//          dc -> Blit ( RECT.X (), RECT.Y (), RECT.W (), RECT.H (), 0   , 0, 0, wxINVERT );
            dc -> Blit ( RECT.GetX (), RECT.GetY (), RECT.GetWidth  (), RECT.GetHeight (),  dc , 0, 0, wxINVERT );
            dc -> Blit ( RECT.GetX (), RECT.GetY (), RECT.GetWidth  (), RECT.GetHeight (), &bdc, 0, 0, wxXOR    );
         }

         break;

      case DrawHeaderFlag_HIDE :
//       dc -> Blit ( RECT.X (), RECT.Y (), RECT.W (), RECT.H (), 0   , 0, 0, wxINVERT );
         dc -> Blit ( RECT.GetX (), RECT.GetY (), RECT.GetWidth  (), RECT.GetHeight (),  dc , 0, 0, wxINVERT );
         dc -> Blit ( RECT.GetX (), RECT.GetY (), RECT.GetWidth  (), RECT.GetHeight (), &bdc, 0, 0, wxXOR    );

         break;
   }

   // Deselect font and pen.

// bdc.SelectObject ( prev_font   );
// bdc.SelectObject ( prev_bitmap );
// bdc.DeleteDC     ();

// dc -> SelectObject ( prev_pen );
   dc -> SetPen ( prev_pen );
}



void  wxTableCtrl :: DrawHeader ( wxDC *  dc )
{
   const int         DXY         = ( styleex & ITCS_FOCUSRECT  ) ? FOCUSRECTTHICK : 0;
   const int         DX          = ( styleex & ITCS_CHECKBOXES ) ? CHECKBOXWIDTH  : 0;

   if ( nativeheader != 0 )
   {  
      if ( ( styleex & ITCS_CHECKBOXES ) == 0 )
         return;
         
      const wxRect   r  ( DXY, DXY, DX, head.GetHeight () );
         
      wxRendererNative :: Get ().DrawHeaderButton ( this, *dc, r, 0 );
      
      return;
   }
      
   wxRect            back        = GetClientRectEx ();   // Get client rect - focus rect.
   const wxFont &    prev_font   = dc -> GetFont ();
   const wxPen &     prev_pen    = dc -> GetPen  ();

   back.SetHeight ( head.GetHeight () );  // Only use header height.

// wxLogDebug  ( "DrawHeader: %d,%d-%d,%d", back.GetX (), back.GetY (), back.GetWidth (), back.GetHeight () );
   
   FillSolidRect ( dc, back, wxSystemSettings :: GetColour ( wxSYS_COLOUR_BTNFACE ) );

   if ( table == 0 )
      return;

   dc -> SetPen  ( black );
   dc -> SetFont ( *font );



   const int                  play     = wxSystemSettings :: GetMetric ( wxSYS_ICON_X ) / 4;

   // Remove all ToolTips!
   
   tooltipvector.Clear  ();

   const Header :: Iterator   END      = header.end  ();
   Column *                   part     = 0;
   wxRect                     rect;

   for ( Header :: Iterator  i = header.begin () ; i != END ; ++i )
   {
      part  = *i;
      
      if ( ! part -> Show () )
         continue;

      const wxRect &    R  = part -> Rect ();
      
      rect  = wxRect ( R.GetX () - left + DXY + DX, R.GetY () + DXY, R.GetWidth  (), head.GetHeight () );
//    rect  = wxRect ( part -> Rect () - wxPoint ( left - DXY - DX, - DXY );

//    wxLogDebug ( "Column: %d,%d-%d,%d", rect.GetX (), rect.GetY (), rect.GetWidth (), rect.GetHeight () );

      if ( back.Intersects ( rect ) )
         DrawColumn ( dc, DrawHeaderFlag_INIT, part, font );

      if ( tooltip != 0 )
      {
         // Setup ToolTips for the headers.

         wxString       tooltext;
         wxRect         toolrect ( rect );

         toolrect.SetX      ( toolrect.GetX      () + play );
         toolrect.SetWidth  ( toolrect.GetWidth  () - play - play );
         toolrect.SetHeight ( toolrect.GetHeight () - play );

         if ( record -> CanSort ( part -> Reference () ) )
            tooltext.Printf ( "Click left mouse button to sort '%s'.\nDrag with mouse to move.\nClick right mouse button for properties.", part -> Desc () );
         else
            tooltext.Printf ( "Drag with mouse to move '%s'.\nClick right mouse button for properties."                                  , part -> Desc () );

         tooltipvector.Add ( ToolTip ( tooltext, toolrect ) );

         toolrect.SetX     ( toolrect.GetRight () );
         toolrect.SetWidth ( play + play );

         tooltext.Printf ( "Drag to resize '%s'.\nClick right mouse button for properties.", part -> Desc () );

         tooltipvector.Add ( ToolTip ( tooltext, toolrect ) );
      }
   }

   // Draw an 'empty' header part at the right in the empty room.
   const wxRect &    R  = ( part != 0 ) ? part -> Rect () : back;

   if ( ( R.GetX () + R.GetWidth () - left ) < ( back.GetX () + back.GetWidth () - DXY - DXY - DX ) )
   {
//    const Column   rest  ( "", "", -1, wxRect ( rect.GetX () + rect.GetWidth (), rect.GetY () + DXY, back.GetX () + back.GetWidth () - ( rect.GetX () + rect.GetWidth () ) - DXY - DXY - DX, rect.GetHeight () ), DT_LEFT );
      const Column   rest  ( "", "", ~0U, wxRect ( R.GetX () + R.GetWidth (), R.GetY (), back.GetX () + back.GetWidth () - DXY - DXY - DX - ( R.GetX () + R.GetWidth () - left ), R.GetHeight () ), DT_LEFT );

      DrawColumn ( dc, DrawHeaderFlag_INIT, &rest , font );
   }

   if ( styleex & ITCS_CHECKBOXES )
   {
      const Column   check ( "", "", ~0U, wxRect ( left - CHECKBOXWIDTH, 0, CHECKBOXWIDTH, R.GetHeight () ), DT_LEFT );

      DrawColumn ( dc, DrawHeaderFlag_INIT, &check, font );
   }

   if ( tooltip != 0 )
      tooltipvector.Add ( ToolTip ( "Drag to resize header", wxRect ( 0, back.GetHeight () - play, back.GetWidth (), play ) ) );

   if ( columnmove != 0 )
      DrawColumn ( dc, DrawHeaderFlag_SHOW, columnmove, font );

// dc -> SelectObject ( prev_font );
// dc -> SelectObject ( prev_pen  );
   dc -> SetFont  ( prev_font );
   dc -> SetPen   ( prev_pen  );
}



void  wxTableCtrl :: DrawHeader ()
{
   const int   DXY   = ( styleex & ITCS_FOCUSRECT  ) ? FOCUSRECTTHICK : 0;
   
   if ( nativeheader != 0 )
   {
      const int   DX = ( styleex & ITCS_CHECKBOXES ) ? CHECKBOXWIDTH : 0;
      
      nativeheader -> Move    ( DXY + DX, DXY );
      nativeheader -> Left    ( left );
      nativeheader -> Refresh ();
      nativeheader -> Update  ();
      
      wxClientDC  dc ( this );
      
      DrawHeader ( &dc );
   }
   else
   {      
   // const int   DX    = ( styleex & ITCS_CHECKBOXES ) ? CHECKBOXWIDTH  : 0;
      wxClientDC  dc    ( this );
      wxBitmapDC  bdc   ( &dc, head.GetWidth () + DXY + DXY, head.GetHeight () + DXY );

      bdc.SetWindow  ( dc.GetWindow () );
      
      DrawHeader ( &bdc );

      bdc.Update ();
   }
}



void  wxTableCtrl :: MoveColumn ( Column *  column, const wxPoint &  point )
{
   bool   first = false;

   if ( columnmove == 0 )
   {
      if ( abs ( start.x - point.x ) > wxSystemSettings :: GetMetric ( wxSYS_DRAG_X ) )
      {
//       SetCapture ();
//       CaptureMouse   ();

         wxClientDC     dc ( this );

         DrawColumn ( &dc, DrawHeaderFlag_HIDE, column );

         column -> SetState ( Column :: State_UP   );

         DrawColumn ( &dc, DrawHeaderFlag_SHOW, column );

         columnmove = new  Column ( *column );

         columnmove -> SetState ( Column :: State_MOVE );

         first = true;
      }
   }

   if ( columnmove != 0 )
   {
      const wxRect &    MR    = columnmove -> Rect ();
      wxPoint           move  ( rect.GetPosition () );

      move += wxPoint ( point.x - start.x, 0 );

      wxClientDC        dc    ( this );

      if ( ! first )
         DrawColumn ( &dc, DrawHeaderFlag_HIDE, columnmove );

      columnmove -> Move ( move );

      DrawColumn ( &dc, DrawHeaderFlag_SHOW, columnmove );

      // Some difficult code to insert here!

      Column *          tmp    = header.PointIn ( MR.GetTopLeft () );

      if ( tmp == 0 )
         return;

      if ( tmp == column )
         tmp   = header.PointIn ( wxPoint ( MR.GetLeft () + MR.GetWidth () - 1, MR.GetY () + MR.GetHeight () -1 ) );

      if ( ( tmp == 0 ) || ( tmp == column ) )
         return;

      wxRect   rect ( tmp -> Rect ().GetTopLeft (), MR.GetSize () );

//    if ( rect.PtInRect ( point ) )
      if ( rect.Contains ( wxPoint ( point.x, head.GetHeight () / 2 ) ) )
      {
         // Move!

         wxRect   r0  = column -> Rect ();
         wxRect   r1  = tmp    -> Rect ();

         body -> Exchange ( r0, r1 );
         
         if ( columnmove -> Rect ().GetX () <= r0.GetX () )
            header.Exchange ( column, tmp, true  );
         else
            header.Exchange ( column, tmp, false );

         DrawHeader ();
/*
 * JAK 19981005 =>
         iClientDC   dc          ( this );
         CDC         bdc;
         CBitmap     bitmap;
         CBitmap    *prev_bitmap;

         bdc.CreateCompatibleDC        ( &dc );
         bitmap.CreateCompatibleBitmap ( &dc, head.Width (), head.Height () );

         prev_bitmap = bdc.SelectObject ( &bitmap );

         DrawHeader ( &bdc );

         dc.BitBlt ( 0, 0, head.Width  (), head.Height (), &bdc, 0, 0, SRCCOPY );

         bdc.SelectObject ( prev_bitmap );

         bdc.DeleteDC ();
 * JAK 19981005 <=
 */
      }
   }
}



void  wxTableCtrl :: SizeColumn ( Column *  column, const wxPoint &  point )
{
// column -> Size ( CSize ( point.x - r.left, r.Height () ) );
   column -> Size ( wxSize ( rect.GetWidth () + point.x - start.x, rect.GetHeight () ) );
   header.RedoSize ();

   DrawHeader ();
}



void  wxTableCtrl :: ColumnMoved ( Column *  column )
{
   header.RedoSize   ();

   if ( body != 0 )
   {
      body -> Refresh    ();
      body -> DoSize     ( false );
   }

   wxTableEvent   te ( this, wxEVT_COMMAND_TABLE_COLUMN_MOVED, column );

   ProcessEvent   ( te );
}



void  wxTableCtrl :: ColumnSized ( Column *  column )
{
   header.RedoSize   ();
   
   if ( body != 0 )
   {
      body -> Refresh    ();
      body -> DoSize     ( false );
   }

   wxTableEvent   te ( this, wxEVT_COMMAND_TABLE_COLUMN_SIZED, column );

   ProcessEvent   ( te );
}



void  wxTableCtrl :: SortColumn ( Column *  column )
{
   const size_t          ref      = column -> Reference ();
// wxTable :: Sequence   sequence = table -> Sort ( ref );

   if ( record -> CanSort ( ref ) )
   {
      switch ( record -> GetSort ( ref ) )
      {
         case wxTable :: Record :: Sequence_ASCENDING    :
            record -> SetSort ( ref, wxTable :: Record :: Sequence_DESCENDING );

            break;

         case wxTable :: Record :: Sequence_DESCENDING   :
            record -> SetSort ( ref, wxTable :: Record :: Sequence_ASCENDING  );

            break;

         default                                         :
            if ( styleex & ITCS_MULTISELECT )
               body  -> SelectRangeStop   ();

            record -> SetSort ( ref, wxTable :: Record :: Sequence_ASCENDING  );

            break;
      }

      index = record -> GetIndex ();

      wxClientDC  dc ( this );
      
      DrawHeader ( &dc );

      if ( body != 0 )
         body -> DoPaintSort ();
//    body -> DoPaintFill ();
   }
}



wxTableCtrl :: Column *  wxTableCtrl :: FindColumn ( const wxPoint &  point )
{
   return ( header.Find ( point.x + left, true ) );
}



void  wxTableCtrl :: DrawBody ()
{
   if ( body != 0 )
      body -> DoPaintSort  ();
}



void  wxTableCtrl :: AutoHScrollStart ( const wxPoint &  point )
{
   AutoHScrollStop ();

   scroll_timer = new  wxTimer ( this, 1 );   // SetTimer ( 1, 100, 0 );
   scroll_point = point;

   scroll_timer -> Start ( 100 );
}



void  wxTableCtrl :: AutoHScrollStop ()
{
   if ( scroll_timer == 0 )
      return;

// KillTimer ( 1 );
   scroll_timer -> Stop ();

   delete  scroll_timer;

   scroll_timer = 0;
}



bool  wxTableCtrl :: ProcessEvent ( const wxTableEvent &  _te )
{
   wxTableEvent   te ( _te );
   
   return ( super :: ProcessEvent ( te ) );
}



bool  wxTableCtrl :: ProcessEvent ( wxTableEvent &  _te )
{
   return ( super :: ProcessEvent ( _te ) );
}



void  wxTableCtrl :: UpdateToolTip ( const wxPoint &  point )
{
   if ( tooltip == 0 )
      return;

   if ( point.y > head.GetHeight () )      
      return;
      
   const size_t   index = tooltipvector.IndexOf ( point );
   
   if ( index == tooltipindex )
      return;
      
   tooltipindex   = index;
   
   tooltip  -> SetWindow   ( 0 );
   
   if ( index != -1 )
      tooltip  -> SetTip      ( tooltipvector.String ( index ) );
   else
      tooltip  -> SetTip      ( wxEmptyString );
      
   tooltip  -> SetWindow   ( this );
}



void  wxTableCtrl :: OnEraseBackground ( wxEraseEvent &  ee )
{
// ee.GetDC ();
// ee.Skip ( false );
}



void  wxTableCtrl :: OnPaint ( wxPaintEvent &  pe )
{
// wxLogDebug  ( "%s", __FUNCTION__ );
// DoSize ();

   const int      DXY   = ( styleex & ITCS_FOCUSRECT ) ? FOCUSRECTTHICK : 0;
   wxPaintDC      dc    ( this );
   wxBitmapDC     bdc   ( &dc, head.GetWidth () + DXY + DXY, head.GetHeight () + DXY );
   
   bdc.SetWindow  ( dc.GetWindow () );
   
   DrawBackground ( &bdc );
   DrawHeader     ( &bdc );

   bdc.Update ();

   DrawBody       ();

   DrawFocusRect  ( &dc, FindFocus () == body );
}



void  wxTableCtrl :: OnSetFocus ( wxFocusEvent &  fe )
{
   if ( body != 0 )
      body -> SetFocus ();
}



void  wxTableCtrl :: OnSize ( wxSizeEvent &  se )
{
   DoSize ();

   se.Skip  ();
}



void  wxTableCtrl :: OnKeyDown ( wxKeyEvent &  ke )
{
   if ( body != 0 )
      body -> OnKeyDown ( ke );

// CWnd :: OnKeyDown ( keycode, repeat, flag );
}



void  wxTableCtrl :: OnLeftDClick ( wxMouseEvent & )
{
}



void  wxTableCtrl :: OnLeftDown ( wxMouseEvent &  me )
{
   ProcessEvent   ( wxTableEvent ( this, wxEVT_COMMAND_TABLE_COLUMN_LEFT_CLICK ) );

   if ( ( FindFocus () != this ) && ( FindFocus () != body ) )
      SetFocus ();

   AutoHScrollStop   ();                  // Stop auto horizontal scrolling!

// point += CPoint ( left, 0 );           // Count with horizontal scrolling!
   const int   X     = me.GetX ();
   const int   DXY   = ( styleex & ITCS_FOCUSRECT  ) ? FOCUSRECTTHICK : 0;
   const int   DX    = ( styleex & ITCS_CHECKBOXES ) ? CHECKBOXWIDTH  : 0;

   wxPoint     point ( X + left - DXY - DX, me.GetY () - DXY );

// if ( flag == MK_LBUTTON )              // Left Mouse Button is supposed to be down!
   if ( me.LeftDown () )
   {
      if ( cursortype == CursorType_SIZE_LEFT_RIGHT )
      {
         column   = header.PointSize ( point, wxSystemSettings :: GetMetric ( wxSYS_ICON_X ) / 4 );

         if ( ( column != 0 ) && ( column -> Resize () ) )
         {
            wxTableEvent   te ( this, wxEVT_COMMAND_TABLE_COLUMN_BEGIN_SIZE, column, me.GetPosition () );

            ProcessEvent   ( te );

            if ( te.IsAllowed () )
            {
   //          SetCapture ();
               CaptureMouse   ();

               start = point;
               rect  = column -> Rect ();

               SizeColumn ( column, point );
            }
            else
               column   = 0;
         }
         else
            column   = 0;

//       super :: OnLButtonDown ( flag, _point );

         return;
      }

      if ( cursortype == CursorType_SIZE_TOP_BOTTOM )
      {
         wxTableEvent   te ( this, wxEVT_COMMAND_TABLE_HEADER_BEGIN_SIZE, column, me.GetPosition () );

         ProcessEvent   ( te );

         if ( te.IsAllowed () )
         {
//          SetCapture ();
            CaptureMouse   ();

//          super :: OnLButtonDown ( flag, _point );
         }

         return;
      }

      if ( cursortype == CursorType_ARROW )
      {
         if ( ( DX > 0 ) && ( X < DX ) )
         {
//          super :: OnLButtonDown ( flag, _point );

            return;
         }

         column   = header.PointIn ( point );

         if ( column != 0 )
         {
            wxTableEvent   te ( this, wxEVT_COMMAND_TABLE_COLUMN_BEGIN_MOVE, column, me.GetPosition () );

            ProcessEvent   ( te );

            if ( te.IsAllowed () )
            {
   //          SetCapture ();
               CaptureMouse   ();

               start = point;
               rect  = column -> Rect ();

               wxClientDC  dc ( this );

               DrawColumn ( &dc, DrawHeaderFlag_HIDE, column );

               column -> SetState ( Column :: State_DOWN );

               DrawColumn ( &dc, DrawHeaderFlag_SHOW, column );
            }
            else
               column   = 0;

            return;
         }
      }
   }
}



void  wxTableCtrl :: OnLeftUp ( wxMouseEvent &  me )
{
   AutoHScrollStop ();                    // Stop auto horizontal scrolling!

#pragma message ( "JAK: SetFocus, have to check why this is not working!!!" )
// SetFocus        ();

// point += CPoint ( left, 0 );           // Count with horizontal scrolling!

   const int   X     = me.GetX ();
   const int   Y     = me.GetY ();
   const int   DXY   = ( styleex & ITCS_FOCUSRECT  ) ? FOCUSRECTTHICK : 0;
   const int   DX    = ( styleex & ITCS_CHECKBOXES ) ? CHECKBOXWIDTH  : 0;
   wxPoint     point ( X + left - DXY - DX, Y - DXY );

   if ( column != 0 )
   {
//    if ( FindCapture () == this )
         ReleaseMouse ();

      if ( cursortype == CursorType_ARROW )
      {
         wxClientDC  dc    ( this );

         if ( columnmove != 0 )
         {
            DrawColumn ( &dc, DrawHeaderFlag_HIDE, columnmove );

            delete  columnmove;

            columnmove = 0;

            wxTableEvent   te ( this, wxEVT_COMMAND_TABLE_COLUMN_MOVED, column, me.GetPosition () );

            ProcessEvent   ( te );
         }
         else
            SortColumn ( column );

   //    dc.SetROP2 ( R2_NOTXORPEN );
         DrawColumn ( &dc, DrawHeaderFlag_HIDE, column );

         column -> SetState ( Column :: State_UP );
//       table -> Sort ( ref, sequence );

   //    dc.SetROP2 ( R2_XORPEN    );
         DrawColumn ( &dc, DrawHeaderFlag_INIT, column );
      }

      if ( cursortype == CursorType_SIZE_LEFT_RIGHT )
         ColumnSized ( column );
   }

   if ( cursortype == CursorType_SIZE_TOP_BOTTOM )
   {
//    ReleaseCapture ();
      ReleaseMouse   ();

      DrawHeader ();

      wxTableEvent   te ( this, wxEVT_COMMAND_TABLE_HEADER_SIZED, column, me.GetPosition () );

      ProcessEvent   ( te );
   }

   column   = 0;
}



void  wxTableCtrl :: OnMotion ( wxMouseEvent &  me )
{
   AutoHScrollStop ();                    // Stop auto horizontal scrolling.

// point += CPoint ( left, 0 );           // Count with horizontal scrolling!
// CPoint   point = _point + CPoint ( left, 0 );

   const int   X     = me.GetX ();
   const int   Y     = me.GetY ();
   const int   DXY   = ( styleex & ITCS_FOCUSRECT  ) ? FOCUSRECTTHICK : 0;
   const int   DX    = ( styleex & ITCS_CHECKBOXES ) ? CHECKBOXWIDTH  : 0;
   wxPoint     point ( X + left - DXY - DX, Y - DXY );

   if ( me.LeftIsDown () )
   {
      if ( column != 0 )
      {
         if      ( cursortype == CursorType_SIZE_LEFT_RIGHT )
         {
            wxTableEvent   te ( this, wxEVT_COMMAND_TABLE_COLUMN_SIZING, column, me.GetPosition () );

            ProcessEvent   ( te );

            if ( te.IsAllowed () )
               SizeColumn ( column, point );
         }
         else if ( cursortype == CursorType_ARROW )
         {
            wxTableEvent   te ( this, wxEVT_COMMAND_TABLE_COLUMN_MOVING, column, me.GetPosition () );

            ProcessEvent   ( te );

            if ( te.IsAllowed () )
            {
               const wxRect   rect  = GetClientRectEx ();

               if ( ( ( X < 0                ) && body -> HorzScroll ( X                    ) ) ||
                    ( ( X > rect.GetWidth () ) && body -> HorzScroll ( X - rect.GetWidth () ) )    )
               {
                  // Recalculate point because 'left' might have changed!

                  point = wxPoint   ( X + left - DXY - DX, Y - DXY );

                  // Start auto horizontal scrolling.

                  AutoHScrollStart ( wxPoint ( X, Y ) );
               }

               MoveColumn ( column, point );
            }
         }

         me.Skip  ();

         return;
      }

      if ( cursortype == CursorType_SIZE_TOP_BOTTOM )
      {
         wxTableEvent   te ( this, wxEVT_COMMAND_TABLE_HEADER_SIZING, column, me.GetPosition () );

         ProcessEvent   ( te );

         if ( te.IsAllowed () )
         {
            const wxRect   rect  = GetClientRectEx ();

   //       if ( ( point.y >= 0 ) && ( point.y < ( rect.H () / 3 ) ) )  // JAK: 19990119, minimal header height.
            if ( ( point.y >= min_height ) && ( point.y < ( rect.GetHeight () / 3 ) ) )
            {
               head  .SetHeight ( point.y );
               header.Height    ( point.y );

               DrawHeader  ();
   /*
   * JAK 19981005 =>
               iClientDC   dc ( this );

               DrawHeader ( &dc );
   * JAK 19981005 <=
   */

               DoSize      ();

               DrawBody    ();
            }
         }
         
         me.Skip  ();

         return;
      }
   }

   // Check if the mouse pointer is moving inside the header.

   if ( point.y < head.GetHeight () )
   {
      if ( header.PointSize ( point, wxSystemSettings :: GetMetric ( wxSYS_ICON_X ) / 4 ) != 0 )
      {
//       if ( cursortype != CursorType_SIZE_LEFT_RIGHT )
//       {
            SetCursor ( cursorsizelr );

            cursortype = CursorType_SIZE_LEFT_RIGHT;
//       }

         UpdateToolTip  ( me.GetPosition () );
         
         me.Skip  ();

         return;
      }
   }

   if ( abs ( point.y - head.GetHeight () ) < ( wxSystemSettings :: GetMetric ( wxSYS_ICON_Y ) / 4 ) )
   {
      SetCursor ( cursorsizetb );

      cursortype = CursorType_SIZE_TOP_BOTTOM;
      
      UpdateToolTip  ( me.GetPosition () );
      
      me.Skip  ();

      return;
   }

// if ( cursortype == CursorType_SIZE_LEFT_RIGHT )
// {
      SetCursor ( cursorarrow );

      cursortype = CursorType_ARROW;
// }

   if ( me.Moving () )                    // Moving, no buttons down!
      UpdateToolTip ( me.GetPosition () );

   me.Skip  ();
}



//void  wxTableCtrl :: OnRButtonDown ( UINT  flag, CPoint  point )
//{
//   super :: OnRButtonDown ( flag, point );
//}



void  wxTableCtrl :: OnRightUp ( wxMouseEvent &  me )
{
   wxTableEvent   te ( this, wxEVT_COMMAND_TABLE_COLUMN_RIGHT_CLICK  , 0, me.GetPosition (), table, record, 0 );
   
   ProcessEvent ( te );
   
   if ( te.IsAllowed () )
   {
   }
   
#if 0
   // Make sure only the right mouse button is active.

   const int   X     = me.GetX ();
   const int   Y     = me.GetY ();
   const int   DXY   = ( styleex & ITCS_FOCUSRECT  ) ? FOCUSRECTTHICK : 0;
   const int   DX    = ( styleex & ITCS_CHECKBOXES ) ? CHECKBOXWIDTH  : 0;
   iPoint      point ( X + left - DXY - DX, Y - DXY );

// if ( ( flag & ( MK_LBUTTON | MK_RBUTTON | MK_MBUTTON ) ) == 0 )
   {
      if ( point.y < head.GetHeight () )
      {
#ifdef __CODEJOCK__
         iXtremeCoolMenu      coolmenu ( this );
#endif
         iPoint               tmp      ( X, Y );
         iMenu                popup;
         iMenu *              sort     = 0;

//       ClientToScreen ( &tmp );

//       popup.CreatePopupMenu ();

         if ( ( styleex & ITCS_CHECKBOXES ) && ( X >= 0 ) && ( X < CHECKBOXWIDTH ) )
         {
            popup.Append   ( IDM_CHECK_TOGGLE , SDA_TOGGLE   );
            popup.Append   ( IDM_CHECK_ALL    , SDA_ALL      );
            popup.Append   ( IDM_CHECK_NONE   , SDA_NONE     );
            popup.Append   ( IDM_CHECK_INVERT , SDA_INVERT   );
         }
         else
         {
#ifdef __JAK_VSQL__
   //       popup.Append   ( IDM_PROPERTY    , _T ( "&Properties" ) );
   //       popup.Append   ( IDM_PROPERTY    , SDS_TABLE_MENU_01    );
            popup.Append   ( IDM_VISUAL_SQL  , SDS_TABLE_MENU_04    );
#endif
   //       popup.Append   ( IDM_RESET       , _T ( "&Reset to default" ) );
            popup.Append   ( IDM_RESET       , SDS_TABLE_MENU_02    );

            if ( ( sort = CreateSortMenu ( IDM_TABLECTRL_SORT ) ) != 0 )
            {
               popup.AppendSeparator   ();
               popup.Append            ( IDM_TABLECTRL_SORT , SDM_SORT  , sort  );
            }
         }

//       popup.TrackPopupMenu    ( TPM_LEFTALIGN, tmp.x, tmp.y, this, 0 );
//
//       popup.DestroyMenu       ();

         PopupMenu   ( &popup, tmp );

//       delete  sort;

         return;
      }
   }

// super :: OnRButtonUp ( flag, point );
#endif
}



void  wxTableCtrl :: OnTimer ( wxTimerEvent &  te )
{
// OnMouseMove ( MK_LBUTTON, scroll_point );
}



void  wxTableCtrl :: OnCommandSort ( UINT  id )
{
   wxTable :: Record :: Sequence    sequence = wxTable :: Record :: Sequence_NONE;

   if ( id == IDM_TABLECTRL_SORT )
   {
      index = record -> GetIndex ();

      if ( record -> GetSequence () == wxTable :: Record :: Sequence_ASCENDING )
         sequence = wxTable :: Record :: Sequence_DESCENDING;
      else
         sequence = wxTable :: Record :: Sequence_ASCENDING;
   }
   else
      if ( ( id >= IDM_TABLECTRL_SORT_0 ) && ( id < IDM_TABLECTRL_SORT_ASCENDING ) )
      {
         index = id - IDM_TABLECTRL_SORT_0;

         if ( record -> GetSequence () == wxTable :: Record :: Sequence_ASCENDING )
            sequence = wxTable :: Record :: Sequence_DESCENDING;
         else
            sequence = wxTable :: Record :: Sequence_ASCENDING;
      }
      else
         if ( id == IDM_TABLECTRL_SORT_ASCENDING )
         {
            index    = record -> GetIndex  ();
            sequence = wxTable :: Record :: Sequence_ASCENDING;
         }
         else
            if ( ( id >= IDM_TABLECTRL_SORT_ASCENDING_0 ) && ( id < IDM_TABLECTRL_SORT_DESCENDING ) )
            {
               index    = id - IDM_TABLECTRL_SORT_ASCENDING_0;
               sequence = wxTable :: Record :: Sequence_ASCENDING;
            }
            else
               if ( id == IDM_TABLECTRL_SORT_DESCENDING )
               {
                  index    = record -> GetIndex ();
                  sequence = wxTable :: Record :: Sequence_DESCENDING;
               }
               else
                  if ( ( id >= IDM_TABLECTRL_SORT_DESCENDING_0 ) && ( id <= IDM_TABLECTRL_SORT_END ) )
                  {
                     index    = id - IDM_TABLECTRL_SORT_DESCENDING_0;
                     sequence = wxTable :: Record :: Sequence_DESCENDING;
                  }
                  else
                  {
                     wxMessageBox   ( wxString :: Format ( "Invalid sort id : %d", id ), "Error", wxOK | wxCENTRE, this );

                     return;
                  }

   if ( ( record -> GetIndex () != index ) || ( record -> GetSequence () != sequence ) )
   {
      record   -> SetIndex    ( index    );
      record   -> SetSequence ( sequence );

      wxClientDC  dc ( this );

      DrawHeader ( &dc );

      if ( body != 0 )
         body -> DoPaintSort  ();
   }
}



void  wxTableCtrl :: OnCommandSubTable ( UINT  id )
{
   SubTable ( id - IDM_SUBTABLE );
}



void  wxTableCtrl :: OnNoSort ()
{
   record   -> SetNoSort ();

   wxClientDC  dc ( this );

   DrawHeader ( &dc );

   if ( body != 0 )
      body -> DoPaintSort ();
}



wxRect  wxTableCtrl :: GetClientRectEx () const
{
   wxRect   rect  = GetClientRect ();

   if ( styleex & ITCS_FOCUSRECT )        // Make room for the Focus Rectangle!
//    rect.DeflateRect  ( FOCUSRECTTHICK, FOCUSRECTTHICK, FOCUSRECTTHICK, FOCUSRECTTHICK );
      rect.Deflate ( FOCUSRECTTHICK );
      
   return ( rect );
}



void  wxTableCtrl :: CreateHeader ()
{
   if ( nativeheader != 0 )
      return;
   
   nativeheader   = new  HeaderCtrl ( this, wxID_ANY );
   
   nativeheader   -> Load  ( &header );
}



void  wxTableCtrl :: DeleteHeader ()
{
   if ( nativeheader == 0 )
      return;
      
   delete  nativeheader;
   
   nativeheader   = 0;   
}



void  wxTableCtrl :: CreateBody ()
{
   body  = new  Body ( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, Body :: DEFAULTSTYLE, wxEmptyString, &header );

   DoSize   ();
   DrawBody ();
}



void  wxTableCtrl :: Init ()
{
   table                = 0;
   record               = 0;
   nativeheader         = 0;

   column               = 0;
   columnmove           = 0;

   left                 = 0;
   min_height           = 8;

   body                 = 0; //new  Body ( &headerlist );
   font                 = new  wxFont ( wxSystemSettings :: GetFont ( wxSYS_DEFAULT_GUI_FONT ) );

   cursortype           = CursorType_ARROW;
   cursorarrow          = *wxSTANDARD_CURSOR;
#ifdef __WXMSW__
   cursorsizelr         = wxCursor ( ResourceNameString ( RNS_RESIZELR ) );
   cursorsizetb         = wxCursor ( ResourceNameString ( RNS_RESIZETB ) );
#else
   cursorsizelr         = wxCursor ( iCursor :: Cursor_SIZEWE   ); // LoadCursor ( AfxGetInstanceHandle (), _T ( "ResizeLR" ) );
   cursorsizetb         = wxCursor ( iCursor :: Cursor_SIZENS   ); // LoadCursor ( AfxGetInstanceHandle (), _T ( "ResizeTB" ) );
#endif

   styleex              = 0;  // ITCS_DRAGANDDROP;

// index                = -1;                   // JAK 20020813
   index                =  0;                   // JAK 20020813
   sequence             = wxTable :: Record :: Sequence_ASCENDING;

// dblclkobject         = 0;
// dblclkmember         = 0;

// insertobject         = 0;
// insertmember         = 0;

// updateobject         = 0;
// updatemember         = 0;

// deleteobject         = 0;
// deletemember         = 0;

//?search               = 0;

   scroll_timer         = 0;
   scroll_point         = wxPoint ( 0, 0 );

   tooltip              = 0;
   tooltipindex         = ~0UL;     
   
   drawevents           = false;
   
   imagelist            = 0;
   imagelistowned       = false;
   headerimagelist      = 0;
   headerimagelistowned = false;

// relation       = 0;
}



wxTableCtrl :: wxTableCtrl ()
   : wxControl (),
   gray  ( wxSystemSettings :: GetColour ( wxSYS_COLOUR_3DSHADOW   ), 1, wxSOLID ),
   white ( wxSystemSettings :: GetColour ( wxSYS_COLOUR_3DHILIGHT  ), 1, wxSOLID ),
   black ( wxSystemSettings :: GetColour ( wxSYS_COLOUR_3DDKSHADOW ), 1, wxSOLID )
{
   Init     ();
}



wxTableCtrl :: wxTableCtrl ( wxWindow *  _window, wxWindowID  _id, const wxPoint &  _point, const wxSize &  _size, long  _style, const wxString &  _title )
   : wxControl (),
   gray  ( wxSystemSettings :: GetColour ( wxSYS_COLOUR_3DSHADOW   ), 1, wxSOLID ),
   white ( wxSystemSettings :: GetColour ( wxSYS_COLOUR_3DHILIGHT  ), 1, wxSOLID ),
   black ( wxSystemSettings :: GetColour ( wxSYS_COLOUR_3DDKSHADOW ), 1, wxSOLID )
{
   Init     ();
   Create   ( _window, _id, _point, _size, _style, _title );
}



wxTableCtrl :: ~wxTableCtrl ()
{
/*
   if ( relation != 0 )
   {
      delete  relation;

      relation = 0;
   }

   if ( tooltip != 0 )
   {
      delete  tooltip;

      tooltip  = 0;
   }
*/
   if ( imagelistowned )
      delete  imagelist;
      
   delete  font;

   delete  body;
}



bool  wxTableCtrl :: Create ( wxWindow *  _window, wxWindowID  _id, const wxPoint &  _point, const wxSize &  _size, long  _style, const wxString &  _title )
{
// Init  ();
   
   if ( ! wxControl :: Create ( _window, _id, _point, _size, _style, wxDefaultValidator, _title ) )
      return ( false );
      
   if ( _window != 0 )
      SetFont ( _window -> GetFont () );

   head  = wxRect ( 0, 0, 0, 0 );

   header.Init   ( 0, head.GetHeight () );

// CreateHeader   ();
   CreateBody     ();

   return ( true );
}



void  wxTableCtrl :: LoadHeader ()
{
   if ( nativeheader != 0 )
      nativeheader -> Load ( &header );
}



bool  wxTableCtrl :: ModifyStyleEx2 ( DWORD  remove, DWORD  add )
{
   styleex &= ~remove;
   styleex |=  add;

   if ( remove & ITCS_VSCROLLER )
   {
//?   delete  scroller;

//?   scroller = 0;

//?   body -> ModifyStyle ( 0, WS_VSCROLL );

      DoSize   ();
   }

   if ( add & ITCS_VSCROLLER )
   {
//?   body -> ModifyStyle ( WS_VSCROLL, 0 );

//?   scroller = new  iScroller  ( this, iRect (), WS_VISIBLE | WS_CHILD );

      DoSize   ();
   }

   return ( true );
}



bool  wxTableCtrl :: MultiSelect () const
{
   return ( styleex & ITCS_MULTISELECT );
}



wxTableCtrl &  wxTableCtrl :: MultiSelect ( bool  flag )
{
   if ( flag )
      styleex |=  ITCS_MULTISELECT;
   else
      styleex &= ~ITCS_MULTISELECT;

   return ( *this );
}



bool  wxTableCtrl :: DragAndDrop () const
{
   return ( ( styleex & ITCS_DRAGANDDROP ) != 0 );
}



wxTableCtrl &  wxTableCtrl :: DragAndDrop ( bool  flag )
{
   if ( flag )
      styleex |=  ITCS_DRAGANDDROP;
   else
      styleex &= ~ITCS_DRAGANDDROP;

   return ( *this );
}



bool  wxTableCtrl :: GridVertical () const
{
   return ( ( styleex & ITCS_GRIDVERTICAL ) != 0 );
}



wxTableCtrl &  wxTableCtrl :: GridVertical ( bool  flag )
{
   if ( flag )
      styleex |=  ITCS_GRIDVERTICAL;
   else
      styleex &= ~ITCS_GRIDVERTICAL;

   return ( *this );
}



bool  wxTableCtrl :: GridHorizontal () const
{
   return ( ( styleex & ITCS_GRIDHORIZONTAL ) != 0 );
}



wxTableCtrl &  wxTableCtrl :: GridHorizontal ( bool  flag )
{
   if ( flag )
      styleex |=  ITCS_GRIDHORIZONTAL;
   else
      styleex &= ~ITCS_GRIDHORIZONTAL;

   return ( *this );
}



bool  wxTableCtrl :: CheckBoxes () const
{
   return ( ( styleex & ITCS_CHECKBOXES ) != 0 );
}



wxTableCtrl &  wxTableCtrl :: CheckBoxes ( bool  flag )
{
   if ( flag )
      styleex |=  ITCS_CHECKBOXES;
   else
      styleex &= ~ITCS_CHECKBOXES;

   return ( *this );
}



bool  wxTableCtrl :: FocusRect () const
{
   return ( ( styleex & ITCS_FOCUSRECT ) != 0 );
}



wxTableCtrl &  wxTableCtrl :: FocusRect ( bool  flag )
{
   if ( flag )
      styleex |=  ITCS_FOCUSRECT;
   else
      styleex &= ~ITCS_FOCUSRECT;

   return ( *this );
}



void  wxTableCtrl :: Refresh ( const DWORD &  flag )
{
   if ( flag & ITCF_REFRESH_FOCUS_FIRST )
   {
      record   -> GetFst         ();

      body     -> CursorCurrent  ();
   }

   if ( flag & ITCF_REFRESH_RESET       )
      body  -> SetFill  ( wxTableCtrl :: Body :: Fill_RESET );
      
   if ( flag & ITCF_REFRESH_RELATION    )
      body  -> SetFill  ( wxTableCtrl :: Body :: Fill_RESET );
      
   super :: Refresh  ();
}



void  wxTableCtrl :: Refresh ( const wxTable :: Cursor &  cursor )
{
   body -> Refresh ( cursor );
}



void  wxTableCtrl :: Table ( wxTable *  _table )
{
   if ( ( table = _table ) != 0 )
      record   = table -> GetRecord ();
   else
      record   = 0;

   head.SetHeight ( header.Init ( table, head.GetHeight () ) );
   
   LoadHeader  ();

   if ( body != 0 )
      body -> Table ( table );

   // JAK 19990729
   // Load is now done after setting the table for the TableCtrl :: Body
   // because Load restores the latest 'Index' and 'Sequence'. Calling Load
   // before table set of Body cause disposition.

// Load ();                               // Load data from registry.

// RedrawWindow ();
// UpdateWindow ();
//?Invalidate   ();
}



const wxTable *  wxTableCtrl :: Table () const
{
   return ( table );
}



wxTable *  wxTableCtrl :: Table ()
{
   return ( table );
}



const wxTableCtrl :: Body :: Mode  wxTableCtrl :: GetMode () const
{
   if ( body != 0 )
      return ( body -> GetMode () );
      
   return ( Body :: Mode_DEFAULT );
}



void  wxTableCtrl :: SetMode ( const Body :: Mode &  _mode )
{
   if ( body != 0 )
      body -> SetMode ( _mode );
}



const wxColor &  wxTableCtrl :: GetForegroundColor () const
{
   return ( body -> GetForegroundColor () );
}



wxTableCtrl &  wxTableCtrl :: SetForegroundColor ( const wxColor &  _foregroundcolor )
{
   body -> SetForegroundColor ( _foregroundcolor );

   return ( *this );
}



const wxColor &  wxTableCtrl :: GetBackgroundColor () const
{
   return ( body -> GetBackgroundColor () );
}



wxTableCtrl &  wxTableCtrl :: SetBackgroundColor ( const wxColor &  _backgroundcolor )
{
   body -> SetBackgroundColor ( _backgroundcolor );

   return ( *this );
}



const bool  wxTableCtrl :: GetToolTip () const
{
   return ( tooltip != 0 );
}



wxTableCtrl &  wxTableCtrl :: SetToolTip ( const bool &  flag )
{
   if ( flag )
   {
      if ( tooltip == 0 )
         super :: SetToolTip  ( tooltip = new  wxToolTip ( wxEmptyString ) );
   }
   else
      super :: SetToolTip  ( tooltip = 0 );
      
   if ( body != 0 )
      body  -> SetToolTip ( flag );
      
   return ( *this );
}



const bool  wxTableCtrl :: GetNativeHeader () const
{
   return ( nativeheader != 0 );
}



wxTableCtrl &  wxTableCtrl :: SetNativeHeader ( bool  flag )
{
   if ( flag )
      CreateHeader ();
   else
      DeleteHeader ();
      
   return ( *this );
}



const bool  wxTableCtrl :: GetDrawEvents () const
{
   return ( drawevents );
}



wxTableCtrl &  wxTableCtrl :: SetDrawEvents ( const bool &  _drawevents )
{
   drawevents  = _drawevents;
   
   return ( *this );
}



bool  wxTableCtrl :: Lock () const
{
   if ( body != 0 )
      return ( body -> Lock () );

   return ( false );
}



void  wxTableCtrl :: Lock ( bool  _lock )
{
   if ( body != 0 )
      body -> Lock ( _lock );
}



bool  wxTableCtrl :: CursorSet ()
{
   if ( table == 0 )
      return ( false );

   return ( record -> CursorSet ( body -> cursor ) == wxTable :: Record :: Result_OK );
}



wxTable :: Action  wxTableCtrl :: Action4Record ()
{
   if ( table == 0 )
      return ( wxTable :: Action_NULL );

   return ( table -> Action4Record () );
}



wxTable :: Action  wxTableCtrl :: Action4Table ()
{
   if ( table == 0 )
      return ( wxTable :: Action_NULL );

   return ( table -> Action4Table () );
}



void  wxTableCtrl :: Index ( int  _index )
{
   index = _index;

   if ( table == 0 )
      return;

   record   -> SetIndex ( index );        // Set the index.

   index = record -> GetIndex ();         // Read it back! -1 might not be accepted!

   DrawHeader ();

   if ( body != 0 )
      body -> DoPaintFill ();
}



const int  wxTableCtrl :: Index () const
{
   return ( index );
}



void  wxTableCtrl :: Sequence ( const wxTable :: Record :: Sequence &  _sequence )
{
   sequence = _sequence;
   
   if ( table == 0 )
      return;
      
   record   -> SetSequence ( sequence );  // Set the sequence.
   
   sequence = record -> GetSequence ();   // Read it back! Might not be accepted!
   
   DrawHeader  ();
   
   if ( body != 0 )
      body -> DoPaintFill ();
}



const wxTable :: Record :: Sequence &  wxTableCtrl :: Sequence () const
{
   return ( sequence );
}



bool  wxTableCtrl :: Get ( const wxTable :: Record :: Find &  find )
{
   if ( ( table != 0 ) && ( body != 0 ) )
      return ( body -> Get ( find ) );

   return ( false );
}



bool  wxTableCtrl :: Get ( wxTable :: Cursor *  cursor )
{  
   if ( ( table != 0 ) && ( body != 0 ) )
      return ( body -> Get ( cursor ) );
      
   return ( false );
}



bool  wxTableCtrl :: GetFst ()
{
   if ( ( table != 0 ) && ( body != 0 ) )
      return ( body -> GetFst () );

   return ( false );
}



bool  wxTableCtrl :: GetLst ()
{
   if ( ( table != 0 ) && ( body != 0 ) )
      return ( body -> GetLst () );

   return ( false );
}



bool  wxTableCtrl :: Reset ()
{
   if ( ( table != 0 ) && ( body != 0 ) )
      return ( body -> Reset () );
      
   return ( false );
}



bool  wxTableCtrl :: InsertRecord ()
{
   if ( ( table != 0 ) && ( body != 0 ) )
      return ( body -> InsertRecord () );

   return ( false );
}



bool  wxTableCtrl :: UpdateRecord ()
{
   if ( ( table != 0 ) && ( body != 0 ) )
      return ( body -> UpdateRecord () );

   return ( false );
}



bool  wxTableCtrl :: DeleteRecord ()
{
   if ( ( table != 0 ) && ( body != 0 ) )
      return ( body -> DeleteRecord () );

   return ( false );
}



bool  wxTableCtrl :: CopyRecord ()
{
   if ( ( table != 0 ) && ( body != 0 ) )
      return ( body -> CopyRecord () );

   return ( false );
}



bool  wxTableCtrl :: Find ( bool  ge )
{
   if ( ( table != 0 ) && ( body != 0 ) )
      return ( body -> Find ( ge ) );

   return ( false );
}



bool  wxTableCtrl :: Search ( const wxString &  text, const bool &  match_case, const bool &  direction, const bool &  first, const bool &  wrap )
{
   if ( ( table != 0 ) && ( body != 0 ) )
      return ( body -> Search ( text, match_case, direction, first, wrap ) );
      
   return ( false );
}



long  wxTableCtrl :: HitTest ( const wxPoint &  point ) const
{
   if ( ( table != 0 ) && ( body != 0 ) )
      return ( body -> HitTest ( point ) );
      
   return ( -1 );
}



bool  wxTableCtrl :: CursorTo ( long  row )
{
   if ( ( table != 0 ) && ( body != 0 ) )
      return ( body -> CursorTo ( row ) );
      
   return ( false );
}



void  wxTableCtrl :: SubTable ( int  /*id*/ )
{
#pragma message ( "   JAK wxTableCtrl :: SubTable" )
#if 0 // def __JAK_TABLE_RELATION__

   if ( table -> GetRelationArray () == 0 )
      return;

   wxTableRelationArray *  relations   = table -> GetRelationArray   ();
   wxTableRelation *       relation    = relations -> At ( id );

   if ( relation == 0 )
      return;

   CursorSet   ();
   relation -> CopyRecord ( table -> Record () );

   wxTable *   slave    = relation  -> Slave ();
// bool        result   = relation  -> Synchronize ();

   // See if the sub table is sorted in the right index so it can be
   // synchronized with the parent. If not, a compliant index is enforced.
   // This is the index of the first 'Key' in the relation.

   if ( ! relation -> Synchronize () )
      slave -> RelationKey ( relation -> At ( 0 ) );

   if ( slave -> RelationKeyCompare ( slave -> Record () ) != 0 )
      slave -> GetFst   ( slave -> Record () );

   iCollectionTableBrowseDialog    *dlg   = new  iCollectionTableBrowseDialog ( GetParent (), wxRect (  40,  40, 320, 160 ), DS_MODALFRAME | DS_3DLOOK | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_POPUP | WS_THICKFRAME | WS_MAXIMIZEBOX, slave -> Desc (), NOID, slave );

   dlg   -> DoModal  ();

   delete  dlg;

#endif
}



void  wxTableCtrl :: StartEvent ( const wxEventType &  et, int  command )
{
   wxTableEvent   te ( this, et, 0, wxDefaultPosition, table, record, record -> CursorCurrent () );
   
   te.SetInt   ( command );
   
   ProcessEvent   ( te );
}



void  wxTableCtrl :: LeftDClick ( wxMouseEvent & )
{
//?dblclkcallback.Perform ( this );
}



void  wxTableCtrl :: CommandInsert ()
{
/*?
   if ( ( Action4Record () & wxTable :: Action_INSERT ) &&
        ( Action4Table  () & wxTable :: Action_INSERT )    )
   {
      if ( ! insertcallback.Perform ( this ) )
         if ( table != 0 )
            table -> Dialog ( GetParent (), this, wxTable :: Command_INSERT );
   }
?*/
}



void  wxTableCtrl :: CommandUpdate ()
{
/*?
   if ( ( Action4Record () & wxTable :: Action_UPDATE ) &&
        ( Action4Table  () & wxTable :: Action_UPDATE )    )
   {
      if ( ! updatecallback.Perform ( this ) )
         if ( table != 0 )
            table -> Dialog ( GetParent (), this, wxTable :: Command_UPDATE );
   }
?*/
}



void  wxTableCtrl :: CommandDelete ()
{
/*?
// iMessageBox :: Report ( iMessageBox :: ButtonOk, __FUNC__, GetSafeHwnd (), "Delete" );
   if ( ( Action4Record () & wxTable :: Action_DELETE ) &&
        ( Action4Table  () & wxTable :: Action_DELETE )    )
      deletecallback.Perform ( this );
?*/
}



void  wxTableCtrl :: CommandCopy ()
{
/*?
   if ( ( Action4Record () & wxTable :: Action_COPY ) &&
        ( Action4Table  () & wxTable :: Action_COPY )    )
      copycallback.Perform ( this );
?*/
}



void  wxTableCtrl :: CommandFind ()
{
/*?
   if ( Action4Table () & wxTable :: Action_FIND )
      findcallback.Perform ( this );
?*/
}



void  wxTableCtrl :: CommandSearch ()
{
/*?
   if ( Action4Table () & wxTable :: Action_SEARCH )
   {
      if ( searchcallback.Perform ( this ) )
         return;

      search   = new  SearchDialog  ( body );

      search   -> ShowWindow  ( SW_NORMAL );
   }
?*/
}


void  wxTableCtrl :: SetImageList ( wxImageList *  _imagelist, bool  _imagelistowned )
{
   if ( imagelistowned )
      delete  imagelist;
      
   imagelist      = _imagelist;
   imagelistowned = _imagelistowned;
}



void  wxTableCtrl :: SetHeaderImageList ( wxImageList *  _headerimagelist, bool  _headerimagelistowned )
{
   if ( headerimagelistowned )
      delete  headerimagelist;
      
   headerimagelist      = _headerimagelist;
   headerimagelistowned = _headerimagelistowned;
}



#if 0
iMenu *  wxTableCtrl :: CreateSortMenu ( UINT  id )
{
#if 0
   if ( ( table != 0 ) && ( table -> Record () != 0 ) && ( table -> Record () -> GetIndex () != 0 ) )
   {
      iMenu *                       menu           = new  iMenu ();
      iType :: Record :: Index *    index          = table -> Record () -> GetIndex ();
#ifdef __JAK_TABLE_RELATION__
      wxTableRelationKey *          relationkey    = table -> RelationKey ();
#endif
      const int                     USE            = index -> Use ();

      for ( int  i = 0 ; i < USE ; ++i )
#ifdef __JAK_TABLE_RELATION__
         if ( ( index -> At ( i ) -> IsUserVisible ()      ) &&
              ( ( relationkey                    == 0 ) ||
                ( relationkey -> Key4Index ( i ) != 0 )    )    )
#else
         if ( index -> At ( i ) -> IsUserVisible () )
#endif
            menu -> AppendCheckItem ( id + i + 1, index -> At ( i ) -> Desc () );

      if ( table -> Index () >= 0 )
      {
         if ( (   id == IDM_TABLECTRL_SORT                                                                               ) ||
              ( ( id == IDM_TABLECTRL_SORT_ASCENDING  ) && ( table -> GetSequence () == wxTable :: Sequence_ASCENDING  ) ) ||
              ( ( id == IDM_TABLECTRL_SORT_DESCENDING ) && ( table -> GetSequence () == wxTable :: Sequence_DESCENDING ) )    )

            menu -> Check ( id + table -> Index () + 1, true );
      }

      if ( table -> HasNoSort () )
      {
         menu  -> AppendSeparator ();
         menu  -> AppendCheckItem ( IDM_NOSORT, SDS_TABLE_MENU_00 );

         if ( table -> Index () == -1 )
            menu  -> Check ( IDM_NOSORT, true );
      }

      return ( menu );
   }
#endif
   return (  0 );
}



iMenu *  wxTableCtrl :: CreateSubTableMenu ()
{
#ifdef __JAK_TABLE_RELATION__

   if ( ( table != 0 ) && ( table -> GetRelationArray () != 0 ) )
   {
      iMenu *                    menu        = new  iMenu ();
      wxTableRelationArray *     relations   = table -> GetRelationArray ();
      const int                  USED        = relations -> Used ();

      for ( int  i = 0 ; i < USED ; ++i )
      {
         wxTableRelation *    relation = relations -> At ( i );

         menu  -> Append ( IDM_SUBTABLE + i, relation -> Slave () -> Desc () );
      }

      return ( menu );
   }

#endif

   return (  0 );
}



iWindowSet *  wxTableCtrl :: Relation ()
{
   return ( relation );
}



bool  wxTableCtrl :: RelationInsert ( wxTableCtrl *  _tablectrl )
{
   if ( relation == 0 )
      relation = new  iWindowSet ();

   return ( relation -> Insert ( _tablectrl ) );
}



bool  wxTableCtrl :: RelationDelete ( wxTableCtrl  *_tablectrl )
{
   if ( relation == 0 )
      return ( false );

   return ( relation -> Delete ( _tablectrl ) );
}
#endif


void  wxTableCtrl :: RefreshLayout ()
{
// header.Init ( table, head.GetHeight () );

   header.SortByPosition   ();
   header.RedoSize         ();
   
   head  .SetHeight        ( header.Height () );
   
   DrawHeader  ();
   DoSize      ();
   DrawBody    ();
}



///////////////////////////////////////////////////////////////////////////////
// wxTableEvent                                                              //
///////////////////////////////////////////////////////////////////////////////



wxTableEvent *  wxTableEvent :: Clone () const
{
   return ( new  wxTableEvent ( *this ) );
}



void  wxTableEvent :: Init ()
{
   dc       =  0;
   column   =  0;
   rect     = wxRect ( wxDefaultPosition, wxDefaultSize );
   table    =  0;
   record   =  0;
   cursor   =  0;
}



wxTableEvent :: wxTableEvent ()
   : wxNotifyEvent ()
{
   Init  ();
}



wxTableEvent :: wxTableEvent ( const wxTableEvent &  that )
   : wxNotifyEvent ( that )
{
   Init  ();
   
   dc          = that.dc;
   column      = that.column;
   rect        = that.rect; 
   table       = that.table;
   record      = that.record;
   cursor      = that.cursor;
   background  = that.background;
   foreground  = that.foreground;
   keyevent    = that.keyevent;
}



wxTableEvent :: wxTableEvent ( wxTableCtrl *  _window, const wxEventType &  _type, const wxTableCtrl :: Column *  _column, const wxPoint &  _point, wxTable *  _table, wxTable :: Record *  _record, const wxTable :: Cursor *  _cursor, const bool &  _focus )
   : wxNotifyEvent ( _type, _window -> GetId () )
{
   SetEventObject ( _window );

   Init  ();

   dc          = 0;
   column      = _column;
   rect        = wxRect ( _point, wxDefaultSize );
   table       = _table;
   record      = _record;
   cursor      = _cursor;
   focus       = _focus;
   background  = wxSystemSettings :: GetColour ( wxSYS_COLOUR_3DFACE        );
   foreground  = wxSystemSettings :: GetColour ( wxSYS_COLOUR_WINDOWTEXT    );
}



wxTableEvent :: wxTableEvent ( wxTableCtrl *  _window, const wxEventType &  _type, const wxKeyEvent &  _keyevent, const wxPoint &  _point, wxTable *  _table, wxTable :: Record *  _record, const wxTable :: Cursor *  _cursor, const bool &  _focus )
   : wxNotifyEvent ( _type, _window -> GetId () )
{
   SetEventObject ( _window );

   Init  ();

   dc          = 0;
   column      = 0;
   rect        = wxRect ( _point, wxDefaultSize );
   table       = _table;
   record      = _record;
   cursor      = _cursor;
   focus       = _focus;
   background  = wxSystemSettings :: GetColour ( wxSYS_COLOUR_3DFACE        );
   foreground  = wxSystemSettings :: GetColour ( wxSYS_COLOUR_WINDOWTEXT    );
   keyevent    = _keyevent;
}



wxTableEvent :: wxTableEvent ( wxTableCtrl *  _window, const wxEventType &  _type, wxDC *  _dc, const wxTableCtrl :: Column *  _column, const wxRect &  _rect, wxTable *  _table, wxTable :: Record *  _record, const wxTable :: Cursor *  _cursor, const bool &  _focus )
   : wxNotifyEvent ( _type, _window -> GetId () )
{
   SetEventObject ( _window );

   Init  ();

   dc          = _dc;
   column      = _column;
   rect        = _rect;
   table       = _table;
   record      = _record;
   cursor      = _cursor;
   focus       = _focus;
   background  = wxSystemSettings :: GetColour ( wxSYS_COLOUR_3DFACE        );
   foreground  = wxSystemSettings :: GetColour ( wxSYS_COLOUR_WINDOWTEXT    );
}



wxTableEvent :: ~wxTableEvent ()
{
}



wxTableCtrl *  wxTableEvent :: GetControl () const
{
   return ( dynamic_cast < wxTableCtrl * > ( GetEventObject () ) );
}



void  wxTableEvent :: SetPoint ( const wxPoint &  _point )
{
   rect     = wxRect ( _point, wxDefaultSize );
}



void  wxTableEvent :: SetColumn ( const wxTableCtrl :: Column *  _column )
{
   column   = _column;
}



wxTable *  wxTableEvent :: GetTable () const
{
   return ( table );
}



wxTable :: Record *  wxTableEvent :: GetRecord () const
{
   return ( record );
}



const wxTable :: Cursor *  wxTableEvent :: GetCursor () const
{
   return ( cursor );
}



void  wxTableEvent :: SetTextBackground ( const wxColor &  _background )
{
   background  = _background;
}



void  wxTableEvent :: SetTextForeground ( const wxColor &  _foreground )
{
   foreground  = _foreground;
}



void  wxTableEvent :: SetFont ( const wxFont &  _font )
{
   font  = _font;
}
