/////////////////////////////////////////////////////////////////////////////
// Name:        dcbuf.cpp
// Purpose:     wxDCbuf class
// Author:      Klaas Holwerda
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "dcclbuf.h"
#endif

#include "gtk/dcclbuf.h"
#include "wx/dcmemory.h"
#include "wx/image.h"
#include "wx/module.h"

#include "wx/gtk/win_gtk.h"

#include <math.h>               // for floating-point functions


//-----------------------------------------------------------------------------
// local defines
//-----------------------------------------------------------------------------

#define USE_PAINT_REGION 1

//-----------------------------------------------------------------------------
// local data
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// constants
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

static inline double dmax(double a, double b) { return a > b ? a : b; }
static inline double dmin(double a, double b) { return a < b ? a : b; }

static inline double DegToRad(double deg) { return (deg * M_PI) / 180.0; }

//-----------------------------------------------------------------------------
// temporary implementation of the missing GDK function
//-----------------------------------------------------------------------------

#include "gdk/gdkprivate.h"

//-----------------------------------------------------------------------------
// wxWindowDCbuf
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxWindowDCbuf, wxDC)

wxWindowDCbuf::wxWindowDCbuf():wxWindowDC()
{

}

wxWindowDCbuf::wxWindowDCbuf( wxWindow *window ):wxWindowDC( window )
{
  
}

wxWindowDCbuf::~wxWindowDCbuf()
{
 
}

void wxWindowDCbuf::SelectObject(const wxBitmap& bitmap)
{
	m_buffer_dc.SelectObject(bitmap);
}

void wxWindowDCbuf::DoDrawLine( wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2 )
{
	wxWindowDC::DoDrawLine(x1,y1,x2,y2);
	m_buffer_dc.DrawLine(x1,y1,x2,y2); 
}

void wxWindowDCbuf::DoCrossHair( wxCoord x, wxCoord y )
{
	wxWindowDC::DoCrossHair(x,y);
	m_buffer_dc.CrossHair(x,y);
}

void wxWindowDCbuf::DoDrawArc( wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2,
                            wxCoord xc, wxCoord yc )
{
	wxWindowDC::DoDrawArc(x1,y1,x2,y2,xc,yc);
	m_buffer_dc.DrawArc(x1,y1,x2,y2,xc,yc);
}

void wxWindowDCbuf::DoDrawEllipticArc( wxCoord x, wxCoord y, wxCoord width, wxCoord height, double sa, double ea )
{
	wxWindowDC::DoDrawEllipticArc(x,y,width,height,sa,ea);
	m_buffer_dc.DrawEllipticArc(x,y,width,height,sa,ea);
}

void wxWindowDCbuf::DoDrawPoint( wxCoord x, wxCoord y )
{
	wxWindowDC::DoDrawPoint(x,y);
	m_buffer_dc.DrawPoint(x,y); 
}

void wxWindowDCbuf::DoDrawLines( int n, wxPoint points[], wxCoord xoffset, wxCoord yoffset )
{
	wxWindowDC::DoDrawLines(n,points,xoffset,yoffset);
	m_buffer_dc.DrawLines(n,points,xoffset,yoffset);
}

void wxWindowDCbuf::DoDrawPolygon( int n, wxPoint points[], wxCoord xoffset, wxCoord yoffset, int fillStyle )
{
	wxWindowDC::DoDrawPolygon(n,points,xoffset,yoffset,fillStyle);
	m_buffer_dc.DrawPolygon(n,points,xoffset,yoffset,fillStyle);
}

void wxWindowDCbuf::DoDrawRectangle( wxCoord x, wxCoord y, wxCoord width, wxCoord height )
{
	wxWindowDC::DoDrawRectangle(x,y,width,height);
	m_buffer_dc.DrawRectangle(x,y,width,height);
}

void wxWindowDCbuf::DoDrawRoundedRectangle( wxCoord x, wxCoord y, wxCoord width, wxCoord height, double radius )
{
	wxWindowDC::DoDrawRoundedRectangle(x,y,width,height,radius);
	m_buffer_dc.DrawRoundedRectangle(x,y,width,height,radius);
}

void wxWindowDCbuf::DoDrawEllipse( wxCoord x, wxCoord y, wxCoord width, wxCoord height )
{
	wxWindowDC::DoDrawEllipse(x,y,width,height);
	m_buffer_dc.DrawEllipse(x,y,width,height);
}

void wxWindowDCbuf::DoDrawIcon( const wxIcon &icon, wxCoord x, wxCoord y )
{
	wxWindowDC::DoDrawIcon(icon,x,y);
	m_buffer_dc.DrawIcon(icon,x,y);
}

void wxWindowDCbuf::DoDrawBitmap( const wxBitmap &bitmap,
                               wxCoord x, wxCoord y,
                               bool useMask )
{
	wxWindowDC::DoDrawBitmap(bitmap,x,y,useMask);
	m_buffer_dc.DrawBitmap(bitmap,x,y,useMask);
}

bool wxWindowDCbuf::DoBlit( wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
                         wxDC *source, wxCoord xsrc, wxCoord ysrc,
                         int logical_func, bool useMask )
{
	m_buffer_dc.Blit(xdest,ydest,width,height,source,xsrc,ysrc,logical_func,useMask);
	return wxWindowDC::DoBlit(xdest,ydest,width,height,source,xsrc,ysrc,logical_func,useMask);
}

void wxWindowDCbuf::DoDrawText( const wxString &text, wxCoord x, wxCoord y )
{
	wxWindowDC::DoDrawText(text,x,y);
	m_buffer_dc.DrawText(text,x,y);
}

void wxWindowDCbuf::DoDrawRotatedText( const wxString &text, wxCoord x, wxCoord y, double angle )
{
	wxWindowDC::DoDrawRotatedText(text,x,y,angle);
	m_buffer_dc.DrawRotatedText(text,x,y,angle);
}


void wxWindowDCbuf::Clear()
{
    wxWindowDC::Clear();
    m_buffer_dc.Clear(); 
}

void wxWindowDCbuf::SetFont( const wxFont &font )
{
	wxWindowDC::SetFont(font);
	m_buffer_dc.SetFont(font);
}

void wxWindowDCbuf::SetPen( const wxPen &pen )
{
	wxWindowDC::SetPen(pen);
	m_buffer_dc.SetPen(pen); 
}

void wxWindowDCbuf::SetBrush( const wxBrush &brush )
{
	wxWindowDC::SetBrush(brush);
	m_buffer_dc.SetBrush(brush); 
}

void wxWindowDCbuf::SetBackground( const wxBrush &brush )
{
	wxWindowDC::SetBackground(brush);
	m_buffer_dc.SetBackground(brush);
}

void wxWindowDCbuf::SetLogicalFunction( int function )
{
	wxWindowDC::SetLogicalFunction(function);
	m_buffer_dc.SetLogicalFunction(function);
}

void wxWindowDCbuf::SetTextForeground( const wxColour &col )
{
	wxWindowDC::SetTextForeground(col);
   m_buffer_dc.SetTextForeground(col);
}

void wxWindowDCbuf::SetTextBackground( const wxColour &col )
{
	wxWindowDC::SetTextBackground(col);
   m_buffer_dc.SetTextBackground(col);
} 

void wxWindowDCbuf::SetBackgroundMode( int mode )
{
	wxWindowDC::SetBackgroundMode(mode);
	m_buffer_dc.SetBackgroundMode(mode);
}

void wxWindowDCbuf::SetPalette( const wxPalette& palette)
{
	wxWindowDC::SetPalette(palette);
}

void wxWindowDCbuf::DoSetClippingRegion( wxCoord x, wxCoord y, wxCoord width, wxCoord height )
{ 
	wxWindowDC::DoSetClippingRegion(x,y,width,height);
	m_buffer_dc.SetClippingRegion(x,y,width,height); 
}

void wxWindowDCbuf::DoSetClippingRegionAsRegion( const wxRegion &region  )
{
	wxWindowDC::DoSetClippingRegionAsRegion(region);
	m_buffer_dc.SetClippingRegion(region);
 
}

void wxWindowDCbuf::DestroyClippingRegion()
{
	wxWindowDC::DestroyClippingRegion();
	m_buffer_dc.DestroyClippingRegion();
}

#if wxUSE_SPLINES

void wxWindowDCbuf::DoDrawSpline( wxList *points )
{
	wxWindowDC::DoDrawSpline(points);
	m_buffer_dc.DrawSpline(points); 
}

#endif // wxUSE_SPLINE

//-----------------------------------------------------------------------------
// wxPaintDC
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxPaintDCbuf,wxWindowDCbuf)

wxPaintDCbuf::wxPaintDCbuf()
  : wxWindowDCbuf()
{
}

wxPaintDCbuf::wxPaintDCbuf( wxWindow *win )
  : wxWindowDCbuf( win )
{
#if USE_PAINT_REGION
    if (!win->m_clipPaintRegion)
        return;
        
    m_paintClippingRegion = win->GetUpdateRegion();
    GdkRegion *region = m_paintClippingRegion.GetRegion();
    if ( region )
    {
        m_currentClippingRegion.Union( m_paintClippingRegion );

        gdk_gc_set_clip_region( m_penGC, region );
        gdk_gc_set_clip_region( m_brushGC, region );
        gdk_gc_set_clip_region( m_textGC, region );
        gdk_gc_set_clip_region( m_bgGC, region );
    }
#endif
}

//-----------------------------------------------------------------------------
// wxClientDC
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxClientDCbuf,wxWindowDCbuf)

wxClientDCbuf::wxClientDCbuf()
  : wxWindowDCbuf()
{
}

wxClientDCbuf::wxClientDCbuf( wxWindow *win )
  : wxWindowDCbuf( win )
{
}

