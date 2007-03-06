/////////////////////////////////////////////////////////////////////////////
// Name:        version.cpp
// Purpose:     wxVersion, wxVersionRange
// Author:      Francesco Montorsi
// Modified by:
// Created:     2006-06-29
// RCS-ID:      $Id: version.cpp,v 1.1.1.1 2006/12/12 09:39:34 frm Exp $
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

// includes
#include "wx/versionrange.h"


// RTTI
IMPLEMENT_DYNAMIC_CLASS(wxVersion, wxObject)
IMPLEMENT_DYNAMIC_CLASS(wxVersionRange, wxObject)


// wx array implementations
#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_OBJARRAY(wxVersionRangeArray);

// static
wxVersion wxEmptyVersion;
wxVersionRange wxEmptyVersionRange;



// ----------------------------------------------------------------------------
// wxVersion
// ----------------------------------------------------------------------------

bool wxVersion::operator >(const wxVersion &v) const
{
    if (m_nMajor > v.m_nMajor)
        return true;
    if (m_nMajor == v.m_nMajor && m_nMinor > v.m_nMinor)
        return true;
    if (m_nMajor == v.m_nMajor && m_nMinor == v.m_nMinor && m_nRelease > v.m_nRelease)
        return true;
    return false;
}

bool wxVersion::operator <(const wxVersion &v) const
{
    if (m_nMajor < v.m_nMajor)
        return true;
    if (m_nMajor == v.m_nMajor && m_nMinor < v.m_nMinor)
        return true;
    if (m_nMajor == v.m_nMajor && m_nMinor == v.m_nMinor && m_nRelease < v.m_nRelease)
        return true;
    return false;
}

bool wxVersion::operator ==(const wxVersion &ver) const
{
    return m_nMajor==ver.m_nMajor &&
           m_nMinor==ver.m_nMinor &&
           m_nRelease==ver.m_nRelease;
}

bool wxVersion::Set(const wxString &str)
{
    // reset to initial state so that IsOk() will fail
    // on invalid strings
    m_nMajor = m_nMinor = m_nRelease = 0;

    // extract the single version numbers in string format
    wxString major = str.BeforeFirst(wxT('.'));
    wxString minor = str.AfterFirst(wxT('.')).BeforeFirst(wxT('.'));
    wxString release = str.AfterFirst(wxT('.')).AfterFirst(wxT('.'));

    if (major.IsEmpty() || !major.IsNumber() ||
        (!minor.IsEmpty() && !minor.IsNumber()) ||
        (!release.IsEmpty() && !release.IsNumber()))
        return false;           // invalid version format !
    if (minor.IsEmpty())
        minor = wxT("0");               // allow version formats of the type "1" = "1.0.0"
    if (release.IsEmpty())
        release = wxT("0");             // allow version formats of the type "1.2" = "1.2.0"

    // then convert them in numbers
    major.ToULong((unsigned long *)&m_nMajor);
    minor.ToULong((unsigned long *)&m_nMinor);
    release.ToULong((unsigned long *)&m_nRelease);

    return IsOk();
}



// ----------------------------------------------------------------------------
// wxVersionRange
// ----------------------------------------------------------------------------

bool wxVersionRange::Set(const wxString &str)
{
    // reset to initial state so that IsOk() will fail
    // on invalid strings
    m_start.Set(0, 0, 0);
    m_end.Set(0, 0, 0);

    if (str.IsEmpty())
        return false;

    // the key characters are the wildcard 'x', range separator '-'
    // comparison operators lt,le,eq,ge,gt
    if (str.Contains(wxT('-')))
    {
        // if using start-end syntax, the wildcard 'x' is not allowed!
        if (str.Contains(wxT('x')))
            return false;

        // it's allowed to specify less than three digits per version:
        // 1.2-3.4  ==>  1.2.0-3.4.9
        wxString start(str.BeforeFirst(wxT('-'))),
                 end(str.AfterFirst(wxT('-')));
        if (start.Freq(wxT('.')) < 2)
            start += wxT(".0");
        if (end.Freq(wxT('.')) < 2)
            end += wxT(".9");

        m_start.Set(start);
        m_end.Set(end);
    }
    else if (str.Contains(wxT('x')))
    {
        // if using wildcard 'x', the start-end syntax is not allowed!
        if (str.Contains(wxT('-')))
            return false;

        // it's allowed to specify less than three digits per version:
        // 1.x  ==>  1.x.x
        wxString ver(str);
        if (ver.Freq(wxT('.')) < 2)
            ver += wxT(".x");

        wxString start(ver), end(ver);
        start.Replace(wxT("x"), wxT("0"));
        end.Replace(wxT("x"), wxT("9"));

        m_start.Set(start);
        m_end.Set(end);
    }
    else
    {
        wxString op(str), ver(str);

        op = op.Remove(2).MakeLower().Trim();       // keep only first two chars
        ver = ver.Mid(2).Trim(false);                // remove first two chars

        if (op == wxT("lt"))
        {
            // lt 2.3.4 ==> 0.0.1-2.3.3
            m_start.Set(0,0,1);
            m_end.Set(ver);
            m_end.Set(m_end.GetMajor(), m_end.GetMinor(), m_end.GetRelease()-1);
        }
        else if (op == wxT("le"))
        {
            // le 2.3.4 ==> 0.0.1-2.3.4
            m_start.Set(0,0,1);
            m_end.Set(ver);
        }
        else if (op == wxT("eq"))
        {
            // eq 2.3.4 ==> 2.3.4-2.3.4
            m_start.Set(ver);
            m_end.Set(ver);
        }
        else if (op == wxT("ge"))
        {
            // ge 2.3.4 ==> 2.3.4-9.9.9
            m_start.Set(ver);
            m_end.Set(9,9,9);
        }
        else if (op == wxT("gt"))
        {
            // gt 2.3.4 ==> 2.3.5-9.9.9
            m_start.Set(ver);
            m_start.Set(m_start.GetMajor(), m_start.GetMinor(), m_start.GetRelease()+1);
            m_end.Set(9,9,9);
        }
        else
        {
            // a simple version, not a range ?
            m_start.Set(str);
            m_end.Set(str);
        }
    }

    return IsOk();
}

wxString wxVersionRange::GetAsString() const
{
    if (m_start == m_end)
        return m_start.GetAsString();

    // can we use the 'x' wildcard to represent this range ?
    bool canUseXforRelease = m_start.GetRelease() == 0 && m_end.GetRelease() == 9;
    bool canUseXforMinor = m_start.GetMinor() == 0 && m_end.GetMinor() == 9;
    bool canUseXforMajor = m_start.GetMajor() == 0 && m_end.GetMajor() == 9;
    bool eqRelease = m_start.GetRelease() == m_end.GetRelease();
    bool eqMinor = m_start.GetMinor() == m_end.GetMinor();
    bool eqMajor = m_start.GetMajor() == m_end.GetMajor();

    // to be able to use the 'x' wildcard, either one of the canUseX or eq
    // conditions defined above must be true for all the three digits
    if (!(canUseXforRelease || eqRelease) ||
        !(canUseXforMinor || eqMinor) ||
        !(canUseXforMajor || eqMajor))
    {
        // cannot use the 'x' wildcard!
        return wxString::Format(wxT("%s-%s"),
                                m_start.GetAsString().c_str(),
                                m_end.GetAsString().c_str());
    }

#define APPEND_DIGIT(n)     \
    if (canUseXfor##n)      \
        str += wxT("x.");   \
    else                    \
        str += wxString::Format(wxT("%d."), m_start.Get##n());

    wxString str;
    APPEND_DIGIT(Major);
    APPEND_DIGIT(Minor);
    APPEND_DIGIT(Release);
    return str.RemoveLast();
}

bool wxVersionRange::Contains(const wxVersion &ver) const
{
    if (m_start <= ver && m_end >= ver)
        return true;
    return false;
}

bool wxVersionRange::OverlapsWith(const wxVersionRange &ver) const
{
    if (ver.Contains(m_start) || ver.Contains(m_end))
        return true;
    return false;
}



