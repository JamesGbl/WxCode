//////////////////////////////////////////////////////////////////////////////
// File:        crashprint.h
// Purpose:     wxCrashPrint class
// Maintainer:  Wyo
// Created:     2004-09-28
// RCS-ID:      $Id: crashprint.h,v 1.3 2004-10-05 16:05:29 wyo Exp $
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

    //! constructor
    wxCrashPrint (int flags = 0, const wxString &fname = wxEmptyString);

    //! destructor
    ~wxCrashPrint () {};

    // format and print report
    void Report ();

private:

    //! variables
    int m_flags;
    wxString m_fname;

    wxString m_appname;

    const int m_maxCount = 100;
    void *m_btBuffer [m_maxCount];
    char **m_btStrings [m_maxCount];

};

#endif // _WX_CRASHPRINT_H_

