///////////////////////////////////////////////////////////////////////////////
// Name:        tests/arctest.cpp
// Purpose:     Include test.cpp and archivetest.cpp from wxWidgets
// Author:      Mike Wetherell
// RCS-ID:      $Id: arctest.cpp,v 1.2 2005-04-02 11:28:19 chiclero Exp $
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

#include "test.cpp"
#include "archivetest.cpp"
#include "wx/tarstrm.h"

// instantiate the archive test template for tars
template class ArchiveTestCase<wxTarClassFactory>;
