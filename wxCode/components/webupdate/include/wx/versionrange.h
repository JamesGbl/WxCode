/////////////////////////////////////////////////////////////////////////////
// Name:        version.h
// Purpose:     wxVersion, wxVersionRange
// Author:      Francesco Montorsi
// Modified by:
// Created:     2006-07-4
// RCS-ID:      $Id: version.h,v 1.1.1.1 2006/12/12 09:41:00 frm Exp $
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _VERSION_H_
#define _VERSION_H_

// Includes
#include "wx/tokenzr.h"

class wxVersion;
class wxVersionRange;

extern wxVersion wxEmptyVersion;
extern wxVersionRange wxEmptyVersionRange;


// ----------------------------------------------------------------------------
// wxVersion
// ----------------------------------------------------------------------------

//! A version number in form a.b.c where a, b, c can be any positive value.
class wxVersion : public wxObject
{
    DECLARE_DYNAMIC_CLASS(wxVersion)

public:
    wxVersion(size_t major = 0, size_t minor = 0, size_t release = 0)
        { m_nMajor=major; m_nMinor=minor; m_nRelease=release; }
    wxVersion(const wxString &str)
        { Set(str); }

public:     // operators

    wxVersion &operator=(const wxString &str)
        { Set(str); return *this; }
    operator wxString() const
        { return GetAsString(); }


    // NB:
    //       1.2.3 > 1.2.0    and    3.0.0 > 1.9.9

    bool operator >(const wxVersion &ver) const;
    bool operator <(const wxVersion &ver) const;
    bool operator==(const wxVersion &ver) const;

    bool operator!=(const wxVersion &ver) const
        { return !(*this == ver); }
    bool operator >=(const wxVersion &ver) const
        { return *this > ver || *this == ver; }
    bool operator <=(const wxVersion &ver) const
        { return *this < ver || *this == ver; }


public:         // setters

    void Set(size_t major, size_t minor, size_t release)
        { m_nMajor=major; m_nMinor=minor; m_nRelease=release; }

    bool Set(const wxString &str);

public:         // getters

    wxString GetAsString() const
        { return wxString::Format(wxT("%d.%d.%d"), m_nMajor, m_nMinor, m_nRelease); }

    bool IsOk() const
    {
        return m_nMajor != 0 ||
               m_nMinor != 0 ||
               m_nRelease != 0;
    }

    size_t GetMajor() const
        { return m_nMajor; }
    size_t GetMinor() const
        { return m_nMinor; }
    size_t GetRelease() const
        { return m_nRelease; }

protected:

    // NOTE: it's not required that these have a 0-9 range;
    //       this class supports also versions like 2.11.999
    size_t m_nMajor, m_nMinor, m_nRelease;
};



// ----------------------------------------------------------------------------
// wxVersionRange
// ----------------------------------------------------------------------------

//! A range of versions in form a.b.c where a, b, c are in range [0-9].
class wxVersionRange : public wxObject
{
    DECLARE_DYNAMIC_CLASS(wxVersionRange)

public:
    wxVersionRange()
        {}
    wxVersionRange(const wxVersion &start, const wxVersion &end)
        { m_start=start; m_end=end; }
    wxVersionRange(const wxString &str)
        { Set(str); }

public:     // operators

    wxVersionRange &operator=(const wxString &str)
        { Set(str); return *this; }
    operator wxString() const
        { return GetAsString(); }
    bool operator==(const wxVersionRange &ver) const
        { return m_start==ver.m_start && m_end==ver.m_end; }
    bool operator!=(const wxVersionRange &ver) const
        { return !(*this == ver); }

public:     // misc

    bool IsOk() const
    {
        return m_start.IsOk() && m_end.IsOk();
    }

    bool Contains(const wxVersion &ver) const;
    bool OverlapsWith(const wxVersionRange &ver) const;

public:         // setters

    void Set(const wxVersion &start, const wxVersion &end)
        { m_start=start; m_end=end; }

    bool Set(const wxString &str);

public:         // getters

    //! Returns the version range as a string.
    wxString GetAsString() const;

    //! Returns the start of this range.
    //! This is always less or equal the end version.
    wxVersion GetStart() const
        { return m_start; }

    //! Returns the end of this range.
    //! This is always greater or equal the start version.
    wxVersion GetEnd() const
        { return m_end; }

protected:

    wxVersion m_start, m_end;
};

// the array
WX_DECLARE_OBJARRAY(wxVersionRange, wxVersionRangeArray);


#endif      // _VERSION_H_

