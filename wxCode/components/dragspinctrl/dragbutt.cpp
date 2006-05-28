///////////////////////////////////////////////////////////////////////////////
// Name:        dragbutt.cpp
// Purpose:     implements wxDragCtrl as a composite control
// Author:      A.J. Lavin.
// Modified by:
// Created:     27.07.04
// RCS-ID:      $Id: dragbutt.cpp,v 1.1 2006-05-28 12:56:12 victor_gil Exp $
// Copyright:   (c) 2004 A.J. Lavin
// License:     wxWindows license
///////////////////////////////////////////////////////////////////////////////


#include "dragbutt.h"
#include <wx/dcmemory.h>
#include <wx/image.h>
#include <cstdlib>
#include <cmath>

#define DEBUG_DRAGBUTTON 0

#define EPSILON 0.00000000000000000000000001
#define EPSILON2 0.000000000000000000000000001
#define ZERO_NOTCH_WIDTH 2

#if DEBUG_DRAGBUTTON
#include <cstdio>
#endif
#include <iostream>
using namespace std;

const char* wxDRAGBUTTON_NAME = "wxDragButton";


namespace
{
#include "drag_arrows_normal.xpm"
#include "drag_arrows_disabled.xpm"

  const int DragButtonWidth = 16;
  const int DragButtonHeight = 25;
  
  const double DefaultMin = 0.0;
  const double DefaultMax = 100.0;
  const double DefaultValue = 50.0;
  const double DefaultAcceleration = 1.5;
  const int DefaultQuantum = 2;

  const int MaxDragRadius = 200;
  const unsigned int DefaultLogSteps = 10;

  struct DragButtonStyle
  {
    wxColour border;
    wxColour corner;
    wxColour hilight;
    wxColour shadow;
    char** arrowXpm;
    wxColour mask;
    wxColour top;
    wxColour bottom;
    bool active;
  };

  const DragButtonStyle NormalStyle =
  {
    wxColour( 165, 165, 165),
    wxColour( 210, 210, 212),
    wxColour( 255, 255, 255),
    wxColour( 210, 210, 212),
    drag_arrows_normal_xpm,
    wxColour( 255, 255, 255),
    wxColour( 255, 255, 255),
    wxColour( 218, 218, 220),
    false
  };

  const  DragButtonStyle MousedownStyle =
  {
    wxColour( 133, 136, 144),
    wxColour( 194, 196, 200),
    wxColour( 255, 255, 255),
    wxColour( 194, 196, 200),
    drag_arrows_normal_xpm,
    wxColour( 255, 255, 255),
    wxColour( 176, 179, 186),
    wxColour( 210, 211, 215),
    true
  };

  const  DragButtonStyle DisabledStyle =
  {
    wxColour( 133, 136, 144),
    wxColour( 194, 196, 200),
    wxColour( 255, 255, 255),
    wxColour( 194, 196, 200),
    drag_arrows_disabled_xpm,
    wxColour( 0, 0, 0),
    wxColour( 255, 255, 255),
    wxColour( 218, 218, 220),
    false
  };

  wxPoint GetCenterScreen();
  wxBitmap getBitmap( const DragButtonStyle& colors, int w, int h);
  wxPoint getArrowsPosition( int w, int h);
  wxImage getVerticalGradient( int w, 
                               int h,
                               const wxColour& top,
                               const wxColour& bottom);
  wxString GetTTipText(int );
  wxString GetToolTipTextLogarithmic( int precision);

  inline bool ClipValue( double min, double max, double& val)
  {
    if ( val < min)
      {
        val = min;
        return true;
      }
    else if ( val > max)
      {
        val = max;
        return true;
      }

    return false;
  }

  // Round given value to precision, measured in decimal places.
  // precision < 0 indicates precision < +/- 10^(-precision)
  double Round( double value, int precision)
  {
    double exponent = exp( - precision * log( 10));
    double mantissa = value / exponent;

    double result = round( mantissa) * exponent;

#if DEBUG_DRAGBUTTON
    fprintf( stderr, "Round( %f, %d) -> %f\n", value, precision, result);
#endif

    return result;
  }
}



IMPLEMENT_DYNAMIC_CLASS( wxDragButtonEvent, wxEvent)
DEFINE_EVENT_TYPE(wxEVT_DRAGBUTTON_DRAG)
DEFINE_EVENT_TYPE(wxEVT_DRAGBUTTON_PRECISION)
DEFINE_EVENT_TYPE(wxEVT_DRAGBUTTON_DOWN)
DEFINE_EVENT_TYPE(wxEVT_DRAGBUTTON_UP)
DEFINE_EVENT_TYPE(wxEVT_DRAGBUTTON_DCLICK)


IMPLEMENT_DYNAMIC_CLASS( wxDragButtonBase, wxStaticText)
BEGIN_EVENT_TABLE( wxDragButtonBase, wxStaticText)
  EVT_ERASE_BACKGROUND( wxDragButtonBase::OnEraseBackground)
  EVT_PAINT( wxDragButtonBase::OnPaint)
  EVT_SIZE( wxDragButtonBase::OnSize)
  EVT_LEFT_DOWN( wxDragButtonBase::OnLeftDown)
  EVT_LEFT_UP( wxDragButtonBase::OnLeftUp)
  EVT_LEFT_DCLICK( wxDragButtonBase::OnDClick)
  EVT_MOTION( wxDragButtonBase::OnMotion)
  EVT_KEY_DOWN( wxDragButtonBase::OnKeyDown)
  EVT_ENTER_WINDOW(wxDragButtonBase::OnEnterWin)
  EVT_LEAVE_WINDOW(wxDragButtonBase::OnLeaveWin)
END_EVENT_TABLE()


wxDragButtonBase::wxDragButtonBase()
{
  Init();
}


wxDragButtonBase::wxDragButtonBase( wxWindow *parent,
                                    wxWindowID id,
                                    const wxPoint& pos,
                                    const wxSize& size,
                                    long style,
                                    const wxString& name)
{
  
  
  Init();

  wxSize bs = DoGetBestSize();

  wxSize sz = size == wxDefaultSize ? bs : size;

  // Force border style "none".
  style &= ~wxBORDER_MASK;
  style |= wxBORDER_NONE;

  style |= wxWANTS_CHARS;

  if ( ! (style & (wxDB_HIDE_CURSOR | wxDB_FREEZE_CURSOR))) style |= wxDB_HIDE_CURSOR;
  
  if ( ! wxControl::Create( parent, 
                            id,
                            pos,
                            sz,
                            style & (~wxTAB_TRAVERSAL) ,
                            wxDefaultValidator,
                            name))
    {
      return;
    }

  m_value = DefaultValue;
  m_windowId = id;
  ticks_off_zero = 0;
  
  SetRange( DefaultMin, DefaultMax);

  SetSizeHints( bs.x, bs.y);
  this->style=style; 
}


void
wxDragButtonBase::SetValue( double val)
{
  m_value = val;
  ClipValue( m_min, m_max, m_value);

#if DEBUG_DRAGBUTTON
  fprintf( stderr, "wxDragButtonBase::SetValue: %f ->[%f, %f]-> %f\n",
           val,
           m_min,
           m_max,
           m_value);
#endif
}


void
wxDragButtonBase::SetRange(double minVal, double maxVal)
{
  double range = maxVal - minVal;

  if ( range < 0.0)
    {
      return;
    }

  m_min = minVal;
  m_max = maxVal;

  m_minPrecision = - static_cast< int>( round( log( fabs( range)) / log( 10)));

  //SetPrecision( m_precision);
  //SetValue( m_value);
}


void
wxDragButtonBase::OnEraseBackground( wxEraseEvent& event)
{
}


void
wxDragButtonBase::OnPaint( wxPaintEvent& event)
{
  wxPaintDC dc( this);
  
  DoDrawDragger( dc);

  if ( m_dragging && ( m_windowStyle & wxDB_FREEZE_CURSOR))
    {
      DoDrawFrozenCursor( dc);
    }
}


void
wxDragButtonBase::OnLeftDown( wxMouseEvent& event)
{
#if DEBUG_DRAGBUTTON
  fprintf ( stderr, "leftDown");
  if (m_windowStyle & wxDB_HIDE_CURSOR) fprintf ( stderr, ", hide cursor");
  if (m_windowStyle & wxDB_FREEZE_CURSOR) fprintf ( stderr, ", freeze cursor");
  fprintf ( stderr, "\n");
#endif

  m_downClick = event.GetPosition();
  m_dragStart = ScreenToClient( GetCenterScreen());
  m_dragLast = m_dragStart;
  m_value0 = m_value;

  if ( m_windowStyle & ( wxDB_HIDE_CURSOR | wxDB_FREEZE_CURSOR))
  {
    HideCursor();
  }
  WarpPointer( m_dragStart.x, m_dragStart.y);

  m_dragging = true;
  CaptureMouse();

  Refresh();

  wxDragButtonEvent dragEvent( wxEVT_DRAGBUTTON_DOWN, m_windowId);
  dragEvent.SetValue( m_value);
  dragEvent.SetPrecision( m_precision);
  dragEvent.SetEventObject( this);
  GetEventHandler()->ProcessEvent( dragEvent);
}


void
wxDragButtonBase::OnLeftUp( wxMouseEvent& event)
{
#if DEBUG_DRAGBUTTON
  fprintf ( stderr, "leftUp");
  if (m_windowStyle & wxDB_HIDE_CURSOR) fprintf ( stderr, ", hide cursor");
  if (m_windowStyle & wxDB_FREEZE_CURSOR) fprintf ( stderr, ", freeze cursor");
  fprintf ( stderr, "\n");
#endif

  m_dragging = false;
  ReleaseMouse();

  WarpPointer( m_downClick.x, m_downClick.y);

  if ( m_windowStyle & ( wxDB_HIDE_CURSOR | wxDB_FREEZE_CURSOR))
  {
    ShowCursor();
  }
  
  if ( m_windowStyle & wxDB_FREEZE_CURSOR)
    {
      RefreshAncestors();
    }

  Refresh();

  wxDragButtonEvent dragEvent( wxEVT_DRAGBUTTON_UP, m_windowId);
  dragEvent.SetValue( m_value);
  dragEvent.SetPrecision( m_precision);
  dragEvent.SetEventObject( this);
  wxPostEvent(this->GetParent(),dragEvent);
}


void
wxDragButtonBase::OnDClick( wxMouseEvent& event)
{
  wxDragButtonEvent dragEvent( wxEVT_DRAGBUTTON_DCLICK, m_windowId);
  dragEvent.SetValue( m_value);
  dragEvent.SetPrecision( m_precision);
  dragEvent.SetEventObject( this);
  wxPostEvent(this->GetParent(),dragEvent);
}


void
wxDragButtonBase::OnMotion( wxMouseEvent& event)
{
  if ( m_dragging)
    {
      wxPoint pos = event.GetPosition();

      wxASSERT( m_acceleration >= 1.0);

      // The accelerated mouse movement.
      double h;
      
      
      if ( m_acceleration == 1.0)
        {
          // Linear motion, no acceleration.  Base motion on the start
          // position of the drag, which gives nice clean motion.
    
          h = static_cast< double>( GetTotalDrag( pos) / m_quantum);
        }
          else
        {
          // Nonlinear motion, i.e., mouse acceleration. Base motion
          // on the last position of the cursor.
          
          int d = GetLastDrag( pos) / m_quantum;
          double m = pow( static_cast< double>( abs( d)), m_acceleration);
          h = d >= 0 ? m : - m;

#if DEBUG_DRAGBUTTON
          fprintf( stderr,
               "Acceleration: ( %d, %d)->( %d, %d): %d -> %f\n",
               m_dragLast.x, m_dragLast.y, pos.x, pos.y, d, h);
#endif
        }
    
          if ( h != 0.0)
        {
          if ( m_logarithmic)
            {
              if ( m_value < 0)
                {
                  h = - h;
                }
              m_value = m_value0 * pow( m_scale, h);
            }
          else
            {
              m_value = m_value0 + h * m_scale;
            }
    
          m_dragLast = pos;
          if ( m_acceleration > 1.0)
            {
              m_value0 = m_value;
            }
    
          if ( ClipValue( m_min, m_max, m_value)
               || abs( GetTotalDrag( pos)) > MaxDragRadius)
            {
              RecenterDrag();
            }
          
          
          
          cout<<"nueva vuelta"<<endl;
          if(m_logarithmic){
                if(m_value==0){
                      ticks_off_zero+= h * m_scale;
                      cout<<"ticks:"<<ticks_off_zero<<endl;
                }
                else{
                     ticks_off_zero = 0;
                }
                
                if(ticks_off_zero!=0){
                    if(ticks_off_zero >ZERO_NOTCH_WIDTH){ 
                        m_value=2;
                        ticks_off_zero = 0;
                    }
                    if(ticks_off_zero <-ZERO_NOTCH_WIDTH){
                        m_value=-2;
                        ticks_off_zero = 0;
                    }
                }
                
                if(fabs(m_value) < EPSILON){
                  if(m_value>0)                
                           ticks_off_zero = ZERO_NOTCH_WIDTH;
                  else      
                           ticks_off_zero = -ZERO_NOTCH_WIDTH;
                  cout<<"pongo a cero"<<endl;
                  m_value = 0;
                }           
                             
          }
         
          
          wxDragButtonEvent dragEvent( wxEVT_DRAGBUTTON_DRAG, m_windowId);
          dragEvent.SetValue( GetValue());
          dragEvent.SetPrecision( GetPrecision());
          wxPostEvent(this->GetParent(),dragEvent);
        }
    }
}


void
wxDragButtonBase::OnKeyDown( wxKeyEvent& event)
{
  switch( event.GetKeyCode())
    {
    case WXK_DOWN:
    case WXK_LEFT:
      SetPrecision( m_precision - 1);
      break;
    case WXK_UP:
    case WXK_RIGHT:
      SetPrecision( m_precision + 1);
      break;
    default:
      event.Skip();
      return;
      break;
    }

  if ( m_dragging)
    {
      RecenterDrag();
    }

  wxDragButtonEvent dragEvent( wxEVT_DRAGBUTTON_PRECISION, this->GetId());
  dragEvent.SetValue( m_value);
  dragEvent.SetPrecision( m_precision);
  wxPostEvent(this->GetParent(),dragEvent);
  
}


void
wxDragButtonBase::OnSize( wxSizeEvent& event)
{
  event.Skip();
}

void wxDragButtonBase::OnEnterWin (wxMouseEvent &event){
     if(style&64){
         last_cursor = this->GetCursor();     
         if( (style & wxDB_HORIZONTAL )){
            this->SetCursor(wxCursor(wxCURSOR_SIZEWE));
         }
         else{
            this->SetCursor(wxCursor(wxCURSOR_SIZENS));
         }
     }
}

void wxDragButtonBase::OnLeaveWin (wxMouseEvent &event){
     if(style&64)
     this->SetCursor(last_cursor);
}

wxString
wxDragButtonBase::ToString( double value, int precision)
{
  if ( precision > 0)
    {
      wxString format = wxString::Format( _T("%%.%df"), precision);
      return wxString::Format( format, value);
    }
  else
    {
      wxString format = wxString::Format( _T("%%.%dd"), - precision + 1);
      return wxString::Format( format, static_cast< int>( round( value)));
    }
}


wxString
wxDragButtonBase::ToStringE( double value, int precision)
{
  int e = precision > 0 ? precision : 0;

  wxString format = wxString::Format( _T("%%.%de"), e);
  return wxString::Format( format, value);
}


wxSize
wxDragButtonBase::DoGetBestSize() const
{
  return wxSize( DragButtonWidth, DragButtonHeight);
}

void
wxDragButtonBase::HideCursor()
{
  SetCursor( wxCursor( wxCURSOR_BLANK));
 
}


void
wxDragButtonBase::ShowCursor()
{
  SetCursor( wxNullCursor);
}


void
wxDragButtonBase::DoDrawDragger( wxPaintDC& dc)
{
  wxBitmap buttonBitmap;

  if ( m_dragging)
    {
      buttonBitmap = getMousedownBitmap();
    }
  else if ( ! IsEnabled())
    {
      buttonBitmap = getDisabledBitmap();
    }
  else
    {
      buttonBitmap = getNormalBitmap();
    }

  dc.DrawBitmap( buttonBitmap, 0, 0, false);
}


void
wxDragButtonBase::DoDrawFrozenCursor( wxDC& dc)
{
}


void
wxDragButtonBase::Init()
{
  m_maxSigFigs = DefaultMaxSigFigs;
  m_quantum =  DefaultQuantum;
  m_precision = DefaultPrecision;
  m_dragging = false;

  m_logSteps = DefaultLogSteps;
  m_logarithmic = false;
  m_dragging = false;

  m_acceleration = DefaultAcceleration;
}


bool
wxDragButtonBase::Hit( int x, int y) const
{
  int w;
  int h;
  GetClientSize( & w, & h);

  return
    x >= 0
    && y >= 0
    && x < w
    && y < h;
}


void
wxDragButtonBase::RefreshAncestors()
{
  wxWindow * pParent = GetParent();
  wxWindow * pTop = this;
  while ( pParent)
    {
      pTop = pParent;
      pParent = pParent->GetParent();

      pTop->Refresh();
    }
}


void
wxDragButtonBase::SetPrecision( int precision)
{
  if ( m_logarithmic)
    {
      if ( precision > m_maxSigFigs)
        {
          precision = m_maxSigFigs;
        }
  
      m_precision = precision < 0 ? 0 : precision;
      const double x = pow( 10.0, static_cast< double>( m_precision));
      const double beta = m_logSteps * x;
      m_scale = pow( 10.0, 1.0 / beta);

      if ( m_windowStyle & wxDB_SHOW_RANGE_POPUP)
        {
          SetToolTip( GetToolTipTextLogarithmic( m_precision));
        }
    }
  else  // linear
    {
      // Ensure that the new precision does not cause either extreme
      // of the range to exceed the maximum number of significant figures.
      int magnitude = GetMagnitude( wxMax( fabs( m_max), fabs( m_min)));
      int sigFigs = precision + magnitude;
      if ( sigFigs > m_maxSigFigs)
        {
          precision = m_maxSigFigs - magnitude;
        }

      m_precision = precision < m_minPrecision ? m_minPrecision : precision;
      m_scale = exp( - m_precision * log( 10.0));

      if ( m_windowStyle & wxDB_SHOW_RANGE_POPUP)
        {
          SetToolTip( GetTTipText(m_precision));
        }

      m_value = Round( m_value, precision);
    }
}


void
wxDragButtonBase::SetMaxSigFigs( int n)
{
  if ( n < 1)
    {
      n = 1;
    }

  m_maxSigFigs = n;

  SetPrecision( m_precision);
}


void
wxDragButtonBase::SetLogarithmic( bool logarithmic)
{
  if ( logarithmic == m_logarithmic)
    {
      return;
    }

  m_logarithmic = logarithmic;

  // Convert precision between logarithmic and linear.

  int newPrecision;
  if ( logarithmic)
    {
      newPrecision = m_precision + GetMagnitude( m_value);
    }
  else
    {
      newPrecision = m_precision - GetMagnitude( m_value);
    }

  SetPrecision( newPrecision);
}


void
wxDragButtonBase::SetAcceleration( double acceleration)
{
  if ( acceleration < 1.0)
    {
      return;
    }
  m_acceleration = acceleration;
}


int
wxDragButtonBase::GetTotalDrag( const wxPoint& mousePosition) const
{
  return IsVertical() 
    ? m_dragStart.y - mousePosition.y
    : mousePosition.x - m_dragStart.x;
}


int
wxDragButtonBase::GetLastDrag( const wxPoint& mousePosition) const
{
  return IsVertical() 
    ? m_dragLast.y - mousePosition.y
    : mousePosition.x - m_dragLast.x;
}


void
wxDragButtonBase::RecenterDrag()
{
  m_value0 = m_value;
  m_dragLast = m_dragStart;
  WarpPointer( m_dragStart.x, m_dragStart.y);
}


wxBitmap
wxDragButtonBase::getNormalBitmap()
{
  int w;
  int h;
  GetClientSize( & w, & h);

  if ( m_normal.Ok() && m_normal.GetWidth() == w && m_normal.GetHeight() == h)	
    {
      return m_normal;
    }

  m_normal = getBitmap( NormalStyle, w, h);

  return m_normal;
}


wxBitmap
wxDragButtonBase::getMousedownBitmap()
{
  int w;
  int h;
  GetClientSize( & w, & h);

  if ( m_down.Ok() && m_down.GetWidth() == w && m_down.GetHeight() == h)	
    {
      return m_down;
    }

  m_down = getBitmap( MousedownStyle, w, h);

  return m_down;
}


wxBitmap
wxDragButtonBase::getDisabledBitmap()
{
  int w;
  int h;
  GetClientSize( & w, & h);

  if ( m_disbit.Ok() && m_disbit.GetWidth() == w && m_disbit.GetHeight() == h)	
    {
      return m_disbit;
    }

  m_disbit = getBitmap( DisabledStyle, w, h);

  return m_disbit;
}


int
wxDragButtonBase::GetMagnitude( double x)
{
  double fx = fabs( x);
  if ( fx < 10.0)
    {
      return 0;
    }
  return static_cast< int>( log( fx) / log( 10));
}


namespace
{
  void drawTopLeft( wxDC& dc, const wxColour& color, int w, int h);
  void drawBottomRight( wxDC& dc, const wxColour& color, int w, int h);

  wxPoint GetCenterScreen()
  {
    int w;
    int h;
    wxDisplaySize( & w, & h); 
    return wxPoint( w / 2, h / 2);
  }

  wxBitmap getBitmap( const DragButtonStyle& style, int w, int h)
  {
    wxBitmap bitmap( w, h);

    wxMemoryDC bdc;
    bdc.SelectObject( bitmap);

    bdc.Clear();

    if ( style.active)
      {
        drawBottomRight( bdc, style.hilight, w, h);
        drawTopLeft( bdc, style.shadow, w, h);
      }
    else
      {
        drawTopLeft( bdc, style.hilight, w, h);
        drawBottomRight( bdc, style.shadow, w, h);
      }

    // Draw outer border.
    bdc.SetPen( * wxThePenList->FindOrCreatePen( style.border, 1, wxSOLID));
    bdc.DrawLine(     0,     0, w - 1,     0);
    bdc.DrawLine( w - 1,     0, w - 1, h - 1);
    bdc.DrawLine( w - 1, h - 1,     0, h - 1);
    bdc.DrawLine( 0,     h - 1,     0,     0);
  
    // Draw corners.
    bdc.SetPen( * wxThePenList->FindOrCreatePen( style.corner, 1, wxSOLID));
    bdc.DrawPoint(     0,     0);
    bdc.DrawPoint( w - 1,     0);
    bdc.DrawPoint( w - 1, h - 1);
    bdc.DrawPoint(     0, h - 1);

    // Draw gradient.
    bdc.DrawBitmap( wxBitmap( getVerticalGradient( w - 4, 
                                                   h - 4,
                                                   style.top,
                                                   style.bottom)),
                    2,
                    2,
                    false);

    // Draw arrows.
    wxBitmap arrows( style.arrowXpm);
    arrows.SetMask( new wxMask( wxBitmap( style.arrowXpm), style.mask));
    wxPoint pos = getArrowsPosition( w, h);
    bdc.DrawBitmap( arrows, pos.x, pos.y, true);

    return bitmap;
  }

  void drawTopLeft( wxDC& dc, const wxColour& color, int w, int h)
  {
    dc.SetPen( * wxThePenList->FindOrCreatePen( color, 1, wxSOLID));
    dc.DrawLine(     0,     1, w - 1,     1);
    dc.DrawLine(     1,     0,     1, h - 1);
  }

  void drawBottomRight( wxDC& dc, const wxColour& color, int w, int h)
  {
    dc.SetPen( * wxThePenList->FindOrCreatePen( color, 1, wxSOLID));
    dc.DrawLine(     0, h - 2, w - 1, h - 2);
    dc.DrawLine( w - 2,     0, w - 2, h - 1);
  }

  wxPoint getArrowsPosition( int w, int h)
  {
    return wxPoint( w / 2 - 4, h / 2 - 2);
  }

  wxImage getVerticalGradient( int w, 
                               int h,
                               const wxColour& top,
                               const wxColour& bottom)
  {
    if ( h <= 0 || w <= 0)
      {
        return wxImage();
      }

    wxImage image( w, h);

    unsigned char r = top.Red();
    unsigned char g = top.Green();
    unsigned char b = top.Blue();

    float dr = ( bottom.Red() - r) / float( h);
    float dg = ( bottom.Green() - g) / float( h);
    float db = ( bottom.Blue() - b) / float( h);

    float fr = r;
    float fg = g;
    float fb = b;

    unsigned char* buf = image.GetData();

    while( true)
      {
        for( int x = 0; x < w; ++ x)
          {
            * buf ++ = r;
            * buf ++ = g;
            * buf ++ = b;
          }

        if ( ! -- h)
          {
            break;
          }
    
        fr += dr;
        fg += dg;
        fb += db;
    
        r = (int)( fr + 0.5f);
        g = (int)( fg + 0.5f);
        b = (int)( fb + 0.5f);
      }

    return image;
  }

  wxString GetTTipText(int precision )
  {
    
    double value = exp( - precision * log( 10));

    return wxString( _T("+-")) + wxDragButton::ToString( value, precision);
  }

  wxString GetToolTipTextLogarithmic( int precision)
  {
    double value = 1.0 + exp( - precision * log( 10));

    return wxString( _T("*/ ")) + wxDragButton::ToString( value, precision);
  }
}
