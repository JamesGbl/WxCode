/////////////////////////////////////////////////////////////////////////////
// Name:        shabase.h
// Purpose:     SHA base class for 512 bits block based algorithm
// Author:      Giovanni Mittone
// Created:     2009/01/11
// RCS-ID:      $Id$
// Copyright:   (c) 2009 Giovanni Mittone
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef SHABASE_H
#define SHABASE_H

// Required include
#include "wx/crypstrm/cryptodef.h"
#include "wx/longlong.h"
#include "wx/crypstrm/uint128.h"

/**
 * This is the base class for SHA hash function with a 1024 bits working buffer (SHA-160, SHA-224 and SHA-256).
 * Users should not use it.
 */
class WXDLLIMPEXP_CRYPTOSTREAM wxSHABase512
{
protected:
    typedef enum wxSHAType { wxSHA160 = 80, wxSHA224 = 64, wxSHA256 = 64};

    wxSHABase512(const wxSHAType &Wsize);
    ~wxSHABase512();

    wxUint32 *W;
    wxULongLong m_length, m_readed, m_N, i;

    size_t GetWSize(){ return 4; };
    void M(const size_t &t, const size_t &lr);
};


#if wxHAS_INT64

wxULongLong operator ~ (const wxULongLong &arg);

/**
 * This is the base class for SHA hash function with a 1024 bits working buffer (SHA-384 and SHA-512).
 * Users should not use it.
 */
class WXDLLIMPEXP_CRYPTOSTREAM wxSHABase1024
{
protected:
    typedef enum wxSHAType { wxSHA384 = 64, wxSHA512 = 64};

    wxSHABase1024(const wxSHAType &Wsize);
    ~wxSHABase1024();

    wxULongLong *W;
    wxUint128 m_length, m_readed, m_N, i;

    size_t GetWSize(){ return 8; };
    void M(const size_t &t, const size_t &lr);
};

#endif // wxHAS_INT64

#endif // SHABASE_H
