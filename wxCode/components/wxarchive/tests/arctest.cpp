///////////////////////////////////////////////////////////////////////////////
// Name:        tests/archive/archive.cpp
// Purpose:     Include the archive test template, and instantiate for tar
// Author:      Mike Wetherell
// RCS-ID:      $Id: arctest.cpp,v 1.1 2004-11-27 23:55:10 chiclero Exp $
// Copyright:   (c) 2004 Mike Wetherell
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "testprec.h"

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
#   include "wx/wx.h"
#endif

#include "archivetest.cpp"
#include "wx/tarstrm.h"

template class ArchiveTestCase<wxTarClassFactory>;
