/////////////////////////////////////////////////////////////////////////////
// Name:        dcbuf.cpp
// Purpose:     wxDCbuf class
// Author:      Klaas Holwerda
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "dcbuf.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/window.h"
    #include "wx/dc.h"
    #include "wx/utils.h"
    #include "wx/dialog.h"
    #include "wx/app.h"
    #include "wx/bitmap.h"
    #include "wx/log.h"
    #include "wx/dcmemory.h"
    #include "wx/icon.h"
#endif

//#include "wx/dcprint.h"

#include <string.h>
#include <math.h>

#include "wx/msw/private.h" // needs to be before #include <commdlg.h>

#if wxUSE_COMMON_DIALOGS
    #include <commdlg.h>
#endif

#ifndef __WIN32__
    #include <print.h>
#endif

#include "msw/dcbuf.h"

// ===========================================================================
// implementation
// ===========================================================================

// ---------------------------------------------------------------------------
// wxDCbuf
// ---------------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(wxDCbuf, wxDC)

// Default constructor
wxDCbuf::wxDCbuf()
{
}


wxDCbuf::~wxDCbuf()
{
}

void wxDCbuf::SelectObject(const wxBitmap& bitmap)
{
	m_buffer_dc.SelectObject(bitmap);
}

void wxDCbuf::SetTextForeground(const wxColour& colour)
{
	wxDC::SetTextForeground(colour);
   m_buffer_dc.SetTextForeground(colour);
}

void wxDCbuf::SetTextBackground(const wxColour& colour)
{
	wxDC::SetTextBackground(colour);
   m_buffer_dc.SetTextBackground(colour);
}

void wxDCbuf::DoSetClippingRegion(wxCoord cx, wxCoord cy, wxCoord cw, wxCoord ch)
{
	wxDC::DoSetClippingRegion(cx,cy,cw,ch);
	m_buffer_dc.SetClippingRegion(cx,cy,cw,ch);
}

void wxDCbuf::DoSetClippingRegionAsRegion(const wxRegion& region)
{
	wxDC::DoSetClippingRegionAsRegion(region);
	m_buffer_dc.SetClippingRegion(region);
}

void wxDCbuf::DestroyClippingRegion()
{
	wxDC::DestroyClippingRegion();
	m_buffer_dc.DestroyClippingRegion();
}

// ---------------------------------------------------------------------------
// drawing
// ---------------------------------------------------------------------------

void wxDCbuf::Clear()
{
    wxDC::Clear();
    m_buffer_dc.Clear();
}

void wxDCbuf::DoFloodFill(wxCoord x, wxCoord y, const wxColour& col, int style)
{
	 wxDC::DoFloodFill(x,y,col,style);
	 m_buffer_dc.FloodFill(x,y,col,style);
}

void wxDCbuf::DoCrossHair(wxCoord x, wxCoord y)
{
	wxDC::DoCrossHair(x,y);
	m_buffer_dc.CrossHair(x,y);
}

void wxDCbuf::DoDrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2)
{
	wxDC::DoDrawLine(x1,y1,x2,y2);
	m_buffer_dc.DrawLine(x1,y1,x2,y2);
}

void wxDCbuf::DoDrawArc(wxCoord x1,wxCoord y1,wxCoord x2,wxCoord y2, wxCoord xc, wxCoord yc)
{
	wxDC::DoDrawArc(x1,y1,x2,y2,xc,yc);
	m_buffer_dc.DrawArc(x1,y1,x2,y2,xc,yc);
}

void wxDCbuf::DoDrawCheckMark(wxCoord x1, wxCoord y1,
                           wxCoord width, wxCoord height)
{
	wxDC::DoDrawCheckMark(x1,y1,width,height);
	m_buffer_dc.DrawCheckMark(x1,y1,width,height);
}

void wxDCbuf::DoDrawPoint(wxCoord x, wxCoord y)
{
	wxDC::DoDrawPoint(x,y);
	m_buffer_dc.DrawPoint(x,y);
}

void wxDCbuf::DoDrawPolygon(int n, wxPoint points[], wxCoord xoffset, wxCoord yoffset,int fillStyle)
{
	wxDC::DoDrawPolygon(n,points,xoffset,yoffset,fillStyle);
	m_buffer_dc.DrawPolygon(n,points,xoffset,yoffset,fillStyle);
}

void wxDCbuf::DoDrawLines(int n, wxPoint points[], wxCoord xoffset, wxCoord yoffset)
{
	wxDC::DoDrawLines(n,points,xoffset,yoffset);
	m_buffer_dc.DrawLines(n,points,xoffset,yoffset);
}

void wxDCbuf::DoDrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
	wxDC::DoDrawRectangle(x,y,width,height);
	m_buffer_dc.DrawRectangle(x,y,width,height);
}

void wxDCbuf::DoDrawRoundedRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height, double radius)
{
	wxDC::DoDrawRoundedRectangle(x,y,width,height,radius);
	m_buffer_dc.DrawRoundedRectangle(x,y,width,height,radius);
}

void wxDCbuf::DoDrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
	wxDC::DoDrawEllipse(x,y,width,height);
	m_buffer_dc.DrawEllipse(x,y,width,height);
}

// Chris Breeze 20/5/98: first implementation of DrawEllipticArc on Windows
void wxDCbuf::DoDrawEllipticArc(wxCoord x,wxCoord y,wxCoord w,wxCoord h,double sa,double ea)
{
	wxDC::DoDrawEllipticArc(x,y,w,h,sa,ea);
	m_buffer_dc.DrawEllipticArc(x,y,w,h,sa,ea);
}

void wxDCbuf::DoDrawIcon(const wxIcon& icon, wxCoord x, wxCoord y)
{
	wxDC::DoDrawIcon(icon,x,y);
	m_buffer_dc.DrawIcon(icon,x,y);
}

void wxDCbuf::DoDrawBitmap( const wxBitmap &bmp, wxCoord x, wxCoord y, bool useMask )
{
	wxDC::DoDrawBitmap(bmp,x,y,useMask);
	m_buffer_dc.DrawBitmap(bmp,x,y,useMask);
}

void wxDCbuf::DoDrawText(const wxString& text, wxCoord x, wxCoord y)
{
	wxDC::DoDrawText(text,x,y);
	m_buffer_dc.DrawText(text,x,y);
}


void wxDCbuf::DoDrawRotatedText(const wxString& text,
                             wxCoord x, wxCoord y,
                             double angle)
{
	wxDC::DoDrawRotatedText(text,x,y,angle);
	m_buffer_dc.DrawRotatedText(text,x,y,angle);
}

// ---------------------------------------------------------------------------
// set GDI objects
// ---------------------------------------------------------------------------

void wxDCbuf::SetPalette(const wxPalette& palette)
{
	wxDC::SetPalette(palette);
	m_buffer_dc.SetPalette(palette);
}

void wxDCbuf::SetFont(const wxFont& the_font)
{
	wxDC::SetFont(the_font);
	m_buffer_dc.SetFont(the_font);
}

void wxDCbuf::SetPen(const wxPen& pen)
{
	wxDC::SetPen(pen);
	m_buffer_dc.SetPen(pen);
}

void wxDCbuf::SetBrush(const wxBrush& brush)
{
	wxDC::SetBrush(brush);
	m_buffer_dc.SetBrush(brush);
}

void wxDCbuf::SetBackground(const wxBrush& brush)
{
	wxDC::SetBackground(brush);
	m_buffer_dc.SetBackground(brush);
}

void wxDCbuf::SetBackgroundMode(int mode)
{
	wxDC::SetBackgroundMode(mode);
	m_buffer_dc.SetBackgroundMode(mode);
}

void wxDCbuf::SetLogicalFunction(int function)
{
	wxDC::SetLogicalFunction(function);
	m_buffer_dc.SetLogicalFunction(function);
}

void wxDCbuf::SetMapMode(int mode)
{
	wxDC::SetMapMode(mode);
	m_buffer_dc.SetMapMode(mode);
}

void wxDCbuf::SetUserScale(double x, double y)
{
	wxDC::SetUserScale(x,y);
	m_buffer_dc.SetUserScale(x,y);
	wxDC::SetMapMode(m_mappingMode);
	m_buffer_dc.SetMapMode(m_mappingMode);
}

void wxDCbuf::SetAxisOrientation(bool xLeftRight, bool yBottomUp)
{
	wxDC::SetAxisOrientation(xLeftRight,yBottomUp);
	m_buffer_dc.SetAxisOrientation(xLeftRight,yBottomUp);
	wxDC::SetMapMode(m_mappingMode);
	m_buffer_dc.SetMapMode(m_mappingMode);
}

void wxDCbuf::SetSystemScale(double x, double y)
{
	wxDC::SetSystemScale(x,y);
	m_buffer_dc.SetSystemScale(x,y);
	wxDC::SetMapMode(m_mappingMode);
	m_buffer_dc.SetMapMode(m_mappingMode);
}

void wxDCbuf::SetLogicalScale(double x, double y)
{
	wxDC::SetLogicalScale(x,y);
	m_buffer_dc.SetLogicalScale(x,y);
}

void wxDCbuf::SetLogicalOrigin(wxCoord x, wxCoord y)
{
	wxDC::SetLogicalOrigin(x,y);
	m_buffer_dc.SetLogicalOrigin(x,y);
}

void wxDCbuf::SetDeviceOrigin(wxCoord x, wxCoord y)
{
	wxDC::SetDeviceOrigin(x,y);
	m_buffer_dc.SetDeviceOrigin(x,y);
}

bool wxDCbuf::DoBlit(wxCoord xdest, wxCoord ydest,
                  wxCoord width, wxCoord height,
                  wxDC *source, wxCoord xsrc, wxCoord ysrc,
                  int rop, bool useMask)
{
	m_buffer_dc.Blit(xdest,ydest,width,height,source,xsrc,ysrc,rop,useMask);
	return wxDC::DoBlit(xdest,ydest,width,height,source,xsrc,ysrc,rop,useMask);
}

#if wxUSE_SPLINES

void wxDCbuf::DoDrawSpline(wxList *list)
{
    wxDC::DoDrawSpline(list);
    m_buffer_dc.DrawSpline(list);
}

#endif // wxUSE_SPLINES

