/////////////////////////////////////////////////////////////////////////////
// Name:        dcbuf.cpp
// Purpose:     wxDCbuf class
// Author:      Klaas Holwerda
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GTK_BUFDCCLIENTH__
#define __GTK_BUFDCCLIENTH__

#ifdef __GNUG__
#pragma interface
#endif

#include "wx/dcclient.h"
#include "wx/dcmemory.h"
#include "wx/window.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxWindowDCbuf;
class wxPaintDCbuf;
class wxClientDCbuf;

//-----------------------------------------------------------------------------
// wxWindowDCbuf
//-----------------------------------------------------------------------------

class wxWindowDCbuf : public wxWindowDC
{
public:
    wxWindowDCbuf();
    wxWindowDCbuf( wxWindow *win );

    ~wxWindowDCbuf();

    void SelectObject(const wxBitmap& bitmap);	 

//protected:
    virtual void DoDrawLine( wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2 );
    virtual void DoCrossHair( wxCoord x, wxCoord y );
    virtual void DoDrawArc( wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2,
                            wxCoord xc, wxCoord yc );
    virtual void DoDrawEllipticArc( wxCoord x, wxCoord y, wxCoord width, wxCoord height,
                                    double sa, double ea );
    virtual void DoDrawPoint( wxCoord x, wxCoord y );

    virtual void DoDrawLines(int n, wxPoint points[],
                             wxCoord xoffset, wxCoord yoffset);
    virtual void DoDrawPolygon(int n, wxPoint points[],
                               wxCoord xoffset, wxCoord yoffset,
                               int fillStyle = wxODDEVEN_RULE);

    virtual void DoDrawRectangle( wxCoord x, wxCoord y, wxCoord width, wxCoord height );
    virtual void DoDrawRoundedRectangle( wxCoord x, wxCoord y, wxCoord width, wxCoord height, double radius = 20.0 );
    virtual void DoDrawEllipse( wxCoord x, wxCoord y, wxCoord width, wxCoord height );

    virtual void DoDrawIcon( const wxIcon &icon, wxCoord x, wxCoord y );
    virtual void DoDrawBitmap( const wxBitmap &bitmap, wxCoord x, wxCoord y,
                               bool useMask = FALSE );

    virtual bool DoBlit( wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
                         wxDC *source, wxCoord xsrc, wxCoord ysrc,
                         int logical_func = wxCOPY, bool useMask = FALSE );

    virtual void DoDrawText( const wxString &text, wxCoord x, wxCoord y );
    virtual void DoDrawRotatedText(const wxString& text, wxCoord x, wxCoord y,
                                   double angle);
    virtual void Clear();

    virtual void SetFont( const wxFont &font );
    virtual void SetPen( const wxPen &pen );
    virtual void SetBrush( const wxBrush &brush );
    virtual void SetBackground( const wxBrush &brush );
    virtual void SetLogicalFunction( int function );
    virtual void SetTextForeground( const wxColour &col );
    virtual void SetTextBackground( const wxColour &col );
    virtual void SetBackgroundMode( int mode );
    virtual void SetPalette( const wxPalette& palette );

    virtual void DoSetClippingRegion( wxCoord x, wxCoord y, wxCoord width, wxCoord height );
    virtual void DestroyClippingRegion();
    virtual void DoSetClippingRegionAsRegion( const wxRegion &region  );

#if wxUSE_SPLINES
    virtual void DoDrawSpline( wxList *points );
#endif
 

private:
    DECLARE_DYNAMIC_CLASS(wxWindowDCbuf)
	 
    wxMemoryDC m_buffer_dc;
	 
};

//-----------------------------------------------------------------------------
// wxPaintDCbuf
//-----------------------------------------------------------------------------

class wxPaintDCbuf : public wxWindowDCbuf
{
public:
    wxPaintDCbuf();
    wxPaintDCbuf( wxWindow *win );

private:
    DECLARE_DYNAMIC_CLASS(wxPaintDCbuf)
};

//-----------------------------------------------------------------------------
// wxClientDCbuf
//-----------------------------------------------------------------------------

class wxClientDCbuf : public wxWindowDCbuf
{
public:
    wxClientDCbuf();
    wxClientDCbuf( wxWindow *win );

private:
    DECLARE_DYNAMIC_CLASS(wxClientDCbuf)
};

#endif // __GTK_BUFDCCLIENTH__
