/////////////////////////////////////////////////////////////////////////////
// Name:        dcbuf.cpp
// Purpose:     wxDCbuf class
// Author:      Klaas Holwerda
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCCLIENT_BUF_H_
#define _WX_DCCLIENT_BUF_H_

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma interface "dcclientbuf.h"
#endif

#include "msw/dcbuf.h"
#include "wx/dynarray.h"

// ----------------------------------------------------------------------------
// array types
// ----------------------------------------------------------------------------

// this one if used by wxPaintDC only
struct WXDLLEXPORT wxPaintDCInfo;

//WX_DECLARE_EXPORTED_OBJARRAY(wxPaintDCInfo, wxArrayDCInfo);

// ----------------------------------------------------------------------------
// DC classes
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxWindowDCbuf : public wxDCbuf
{
    DECLARE_DYNAMIC_CLASS(wxWindowDCbuf)

public:
    wxWindowDCbuf();

    // Create a DC corresponding to the whole window
    wxWindowDCbuf(wxWindow *win);

    virtual ~wxWindowDCbuf();
};

class WXDLLEXPORT wxClientDCbuf : public wxWindowDCbuf
{
    DECLARE_DYNAMIC_CLASS(wxClientDCbuf)

public:
    wxClientDCbuf();

    // Create a DC corresponding to the client area of the window
    wxClientDCbuf(wxWindow *win);

    virtual ~wxClientDCbuf();
};

class WXDLLEXPORT wxPaintDCbuf : public wxWindowDCbuf
{
    DECLARE_DYNAMIC_CLASS(wxPaintDCbuf)

public:
    wxPaintDCbuf();

    // Create a DC corresponding for painting the window in OnPaint()
    wxPaintDCbuf(wxWindow *win);

    virtual ~wxPaintDCbuf();

protected:
    static wxArrayDCInfo ms_cache;

    // find the entry for this DC in the cache (keyed by the window)
    wxPaintDCInfo *FindInCache(size_t *index = NULL) const;
};

#endif
    // _WX_DCCLIENT_BUF_H_
