//////////////////////////////////////////////////////////////////////////////
// File:        crashprint.h
// Purpose:     wxCrashPrint class
// Maintainer:  Wyo
// Created:     2004-09-28
// RCS-ID:      $Id: crashprint.h,v 1.1 2004-10-01 18:39:06 wyo Exp $
// Copyright:   (c) wxCode
// Licence:     wxWidgets licence
//////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CRASHPRINT_H_
#define _WX_CRASHPRINT_H_

#ifdef __GNUG__
    #pragma implementation "crashprint.h"
#endif

//----------------------------------------------------------------------------
// information
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------
//! wxWidgets headers


//============================================================================
// declarations
//============================================================================

//----------------------------------------------------------------------------
//!


//----------------------------------------------------------------------------
//! wxCrashPrint
class wxCrashPrint {

public:

    //! default constructor
    wxCrashPrint (int flags = 0, const wxString &fname = wxEmptyString) {}

    //! create constructor
    wxCrashPrint () {
    }

    // format and print report
    Report ();

private:

    //! variables
    int m_flags;
    wxString m_fname;

};

#endif // _WX_CRASHPRINT_H_

