/////////////////////////////////////////////////////////////////////////////
// Name:        bmpshape.cpp
// Purpose:     Bitmap shape class
// Author:      Julian Smart
// Modified by: Troels K
// Created:     12/07/98
// RCS-ID:      $Id: bmpshape.cpp,v 1.1 2007/03/28 15:15:56 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include <wx/ogl/ogl.h>

/*
 * Bitmap object
 *
 */

IMPLEMENT_DYNAMIC_CLASS(wxBitmapShape, wxRectangleShape)

wxBitmapShape::wxBitmapShape():wxRectangleShape(100.0, 50.0)
{
  m_filename = wxEmptyString;
}

wxBitmapShape::~wxBitmapShape()
{
}

void wxBitmapShape::OnDraw(wxDC& dc)
{
  if (!m_bitmap.IsOk())
    return;

  int x, y;
  x = WXROUND(m_xpos - m_bitmap.GetWidth() / 2.0);
  y = WXROUND(m_ypos - m_bitmap.GetHeight() / 2.0);
  dc.DrawBitmap(m_bitmap, x, y, true);
}

void wxBitmapShape::SetSize(double w, double h, bool WXUNUSED(recursive))
{
  if (m_bitmap.IsOk())
  {
    w = m_bitmap.GetWidth();
    h = m_bitmap.GetHeight();
  }

  SetAttachmentSize(w, h);

  m_width = w;
  m_height = h;
  SetDefaultRegionSize();
}

#if wxUSE_PROLOGIO
void wxBitmapShape::WriteAttributes(wxExpr *clause)
{
  // Can't really save the bitmap; so instantiate the bitmap
  // at a higher level in the application, from a symbol library.
  wxRectangleShape::WriteAttributes(clause);
  clause->AddAttributeValueString(wxT("filename"), m_filename);
}

void wxBitmapShape::ReadAttributes(wxExpr *clause)
{
  wxRectangleShape::ReadAttributes(clause);
  clause->GetAttributeValue(wxT("filename"), m_filename);
}
#endif

// Does the copying for this object
void wxBitmapShape::Copy(wxShape& copy)
{
  wxRectangleShape::Copy(copy);

  wxASSERT(wxDynamicCast(&copy, wxBitmapShape));

  wxBitmapShape& bitmapCopy = (wxBitmapShape&) copy;

  bitmapCopy.m_bitmap = m_bitmap;
  bitmapCopy.SetFilename(m_filename);
}

void wxBitmapShape::SetBitmap(const wxBitmap& bm)
{
  m_bitmap = bm;
  if (m_bitmap.IsOk())
    SetSize(m_bitmap.GetWidth(), m_bitmap.GetHeight());
}


