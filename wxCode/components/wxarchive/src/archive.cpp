/////////////////////////////////////////////////////////////////////////////
// Name:        archive.cpp
// Purpose:     Streams for archive formats
// Author:      Mike Wetherell
// RCS-ID:      $Id: archive.cpp,v 1.1 2004-06-25 23:21:08 chiclero Exp $
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
IMPLEMENT_ABSTRACT_CLASS(wxArchiveCatalog, wxObject)
IMPLEMENT_ABSTRACT_CLASS(wxArchiveClassFactory, wxObject)


/////////////////////////////////////////////////////////////////////////////
// Archive catalog

wxArchiveCatalog::wxArchiveCatalog(wxMBConv& conv /*=wxConvFile*/)
  : m_conv(conv)
{
}



/////////////////////////////////////////////////////////////////////////////
// wxArchiveInputStream

wxArchiveInputStream::wxArchiveInputStream(wxInputStream& stream,
                                           wxMBConv& conv /*=wxConvFile*/)
  : wxFilterInputStream(stream),
    m_conv(conv)
{
}


/////////////////////////////////////////////////////////////////////////////
// wxArchiveOutputStream

wxArchiveOutputStream::wxArchiveOutputStream(wxOutputStream& stream,
                                             wxMBConv& conv /*=wxConvFile*/)
  : wxFilterOutputStream(stream),
    m_conv(conv)
{
}

