/////////////////////////////////////////////////////////////////////////////
// Name:        archive.cpp
// Purpose:     Streams for archive formats
// Author:      Mike Wetherell
// RCS-ID:      $Id: archive.cpp,v 1.2 2004-06-28 11:25:16 chiclero Exp $
// Copyright:   (c) Mike Wetherell
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
  #pragma implementation "archive.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#ifndef WX_PRECOMP
  #include "wx/defs.h"
#endif

#include "wx/archive.h"


IMPLEMENT_ABSTRACT_CLASS(wxArchiveEntry, wxObject)
IMPLEMENT_ABSTRACT_CLASS(wxArchiveClassFactory, wxObject)


/////////////////////////////////////////////////////////////////////////////
// wxArchiveInputStream

wxArchiveInputStream::wxArchiveInputStream(wxInputStream& stream,
                                           wxMBConv& conv)
  : wxFilterInputStream(stream),
    m_conv(conv)
{
}


/////////////////////////////////////////////////////////////////////////////
// wxArchiveOutputStream

wxArchiveOutputStream::wxArchiveOutputStream(wxOutputStream& stream,
                                             wxMBConv& conv)
  : wxFilterOutputStream(stream),
    m_conv(conv)
{
}

