/////////////////////////////////////////////////////////////////////////////
// Name:        ogl.h
// Purpose:     OGL main include
// Author:      Julian Smart
// Modified by:
// Created:     12/07/98
// RCS-ID:      $Id: ogl.h,v 1.1 2007/03/28 15:15:47 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _OGL_OGL_H_
#define _OGL_OGL_H_

#ifdef WXMAKINGDLL_OGL
    #define WXDLLIMPEXP_OGL WXEXPORT
#elif defined(WXUSINGDLL_OGL)
    #define WXDLLIMPEXP_OGL WXIMPORT
#else // not making nor using DLL
    #define WXDLLIMPEXP_OGL
#endif
#if defined(__WINDOWS__) && defined(__GNUC__)
    #define WXDLLIMPEXP_FWD_OGL
#else
    #define WXDLLIMPEXP_FWD_OGL WXDLLIMPEXP_OGL
#endif

#if (wxVERSION_NUMBER < 2900)
enum wxPenStyle
{
    wxPENSTYLE_SOLID = wxSOLID,
    wxPENSTYLE_DOT = wxDOT,
    wxPENSTYLE_SHORT_DASH = wxSHORT_DASH,
    wxPENSTYLE_TRANSPARENT = wxTRANSPARENT,
};
enum wxBrushStyle
{
    wxBRUSHSTYLE_SOLID = wxSOLID,
    wxBRUSHSTYLE_TRANSPARENT = wxTRANSPARENT,
    wxBRUSHSTYLE_BDIAGONAL_HATCH = wxBDIAGONAL_HATCH,
    wxBRUSHSTYLE_CROSSDIAG_HATCH = wxCROSSDIAG_HATCH,
    wxBRUSHSTYLE_FDIAGONAL_HATCH = wxFDIAGONAL_HATCH,
    wxBRUSHSTYLE_HORIZONTAL_HATCH = wxHORIZONTAL_HATCH,
    wxBRUSHSTYLE_VERTICAL_HATCH = wxVERTICAL_HATCH,
    wxBRUSHSTYLE_CROSS_HATCH = wxCROSS_HATCH,
};
#endif

#include <wx/ogl/basic.h>      // Basic shapes
#include <wx/ogl/basicp.h>
#include <wx/ogl/lines.h>      // Lines and splines
#include <wx/ogl/linesp.h>
#include <wx/ogl/divided.h>    // Vertically-divided rectangle
#include <wx/ogl/composit.h>   // Composite images
#include <wx/ogl/canvas.h>     // wxShapeCanvas for displaying objects
#include <wx/ogl/ogldiag.h>    // wxDiagram

#include <wx/ogl/bmpshape.h>
#include <wx/ogl/constrnt.h>
#include <wx/ogl/drawn.h>
#include <wx/ogl/drawnp.h>
#include <wx/ogl/mfutils.h>
#include <wx/ogl/misc.h>

// TODO: replace with wxModule implementation
extern WXDLLIMPEXP_OGL void wxOGLInitialize();
extern WXDLLIMPEXP_OGL void wxOGLCleanUp();

#endif
 // _OGL_OGL_H_
