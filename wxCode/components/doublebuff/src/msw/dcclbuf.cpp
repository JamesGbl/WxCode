/////////////////////////////////////////////////////////////////////////////
// Name:        dcbuf.cpp
// Purpose:     wxClientDCbuf class
// Author:      Klaas Holwerda
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "dcclbuf.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/string.h"
#include "wx/log.h"
#include "wx/window.h"

#include "wx/msw/private.h"

#include "msw/dcclbuf.h"

// ----------------------------------------------------------------------------
// array/list types
// ----------------------------------------------------------------------------


struct WXDLLEXPORT wxPaintDCInfo
{
    wxPaintDCInfo(wxWindow *win, wxDC *dc)
    {
        hwnd = win->GetHWND();
        hdc = dc->GetHDC();
        count = 1;
    }

    WXHWND    hwnd;       // window for this DC
    WXHDC     hdc;        // the DC handle
    size_t    count;      // usage count
};

#include "wx/arrimpl.cpp"

//WX_DEFINE_OBJARRAY(wxArrayDCInfo);

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

    IMPLEMENT_DYNAMIC_CLASS(wxWindowDCbuf, wxDCbuf)
    IMPLEMENT_DYNAMIC_CLASS(wxClientDCbuf, wxWindowDCbuf)
    IMPLEMENT_DYNAMIC_CLASS(wxPaintDCbuf, wxWindowDCbuf)

// ----------------------------------------------------------------------------
// global variables
// ----------------------------------------------------------------------------

static PAINTSTRUCT g_paintStruct;

#ifdef __WXDEBUG__
//   int g_isPainting = 0;
#endif // __WXDEBUG__

// ===========================================================================
// implementation
// ===========================================================================

// ----------------------------------------------------------------------------
// wxWindowDCbuf
// ----------------------------------------------------------------------------

wxWindowDCbuf::wxWindowDCbuf()
{
  m_canvas = NULL;
}

wxWindowDCbuf::wxWindowDCbuf(wxWindow *the_canvas)
{
  m_canvas = the_canvas;
  m_hDC = (WXHDC) ::GetWindowDC(GetWinHwnd(the_canvas) );
  m_hDCCount++;

  SetBackground(wxBrush(m_canvas->GetBackgroundColour(), wxSOLID));
}

wxWindowDCbuf::~wxWindowDCbuf()
{
  if (m_canvas && m_hDC)
  {
    SelectOldObjects(m_hDC);

    if ( !::ReleaseDC(GetWinHwnd(m_canvas), GetHdc()) )
    {
        wxLogLastError(wxT("ReleaseDC"));
    }

    m_hDC = 0;
  }

  m_hDCCount--;
}

// ----------------------------------------------------------------------------
// wxClientDCbuf
// ----------------------------------------------------------------------------

wxClientDCbuf::wxClientDCbuf()
{
  m_canvas = NULL;
}

wxClientDCbuf::wxClientDCbuf(wxWindow *the_canvas)
{
  m_canvas = the_canvas;
  m_hDC = (WXHDC) ::GetDC(GetWinHwnd(the_canvas));

  // the background mode is only used for text background
  // and is set in DrawText() to OPAQUE as required, other-
  // wise always TRANSPARENT, RR
  ::SetBkMode( GetHdc(), TRANSPARENT );

  SetBackground(wxBrush(m_canvas->GetBackgroundColour(), wxSOLID));
}

wxClientDCbuf::~wxClientDCbuf()
{
  if ( m_canvas && GetHdc() )
  {
    SelectOldObjects(m_hDC);

    ::ReleaseDC(GetWinHwnd(m_canvas), GetHdc());
    m_hDC = 0;
  }
}

wxArrayDCInfo wxPaintDCbuf::ms_cache;

wxPaintDCbuf::wxPaintDCbuf()
{
    m_canvas = NULL;
    m_hDC = 0;
}

wxPaintDCbuf::wxPaintDCbuf(wxWindow *canvas)
{
    wxCHECK_RET( canvas, wxT("NULL canvas in wxPaintDCbuf ctor") );

#ifdef __WXDEBUG__
    if ( g_isPainting <= 0 )
    {
        wxFAIL_MSG( wxT("wxPaintDCbuf may be created only in EVT_PAINT handler!") );

        return;
    }
#endif // __WXDEBUG__

    m_canvas = canvas;

    // do we have a DC for this window in the cache?
    wxPaintDCInfo *info = FindInCache();
    if ( info )
    {
        m_hDC = info->hdc;
        info->count++;
    }
    else // not in cache, create a new one
    {
        m_hDC = (WXHDC)::BeginPaint(GetWinHwnd(m_canvas), &g_paintStruct);
        ms_cache.Add(new wxPaintDCInfo(m_canvas, this));
    }

    // the background mode is only used for text background
    // and is set in DrawText() to OPAQUE as required, other-
    // wise always TRANSPARENT, RR
    ::SetBkMode( GetHdc(), TRANSPARENT );

    SetBackground(wxBrush(m_canvas->GetBackgroundColour(), wxSOLID));
}

wxPaintDCbuf::~wxPaintDCbuf()
{
    if ( m_hDC )
    {
        SelectOldObjects(m_hDC);

        size_t index;
        wxPaintDCInfo *info = FindInCache(&index);

        wxCHECK_RET( info, wxT("existing DC should have a cache entry") );

        if ( !--info->count )
        {
            ::EndPaint(GetWinHwnd(m_canvas), &g_paintStruct);

            ms_cache.Remove(index);

            // Reduce the number of bogus reports of non-freed memory
            // at app exit
            if (ms_cache.IsEmpty())
                ms_cache.Clear();
        }
        //else: cached DC entry is still in use

        // prevent the base class dtor from ReleaseDC()ing it again
        m_hDC = 0;
    }
}

wxPaintDCInfo *wxPaintDCbuf::FindInCache(size_t *index) const
{
    wxPaintDCInfo *info = NULL;
    size_t nCache = ms_cache.GetCount();
    for ( size_t n = 0; n < nCache; n++ )
    {
        info = &ms_cache[n];
        if ( info->hwnd == m_canvas->GetHWND() )
        {
            if ( index )
                *index = n;
            break;
        }
    }

    return info;
}
