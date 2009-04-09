/////////////////////////////////////////////////////////////////////////////
// Name:        uint128.h
// Purpose:     A simple class to represent a 128 bit number
// Author:      Giovanni Mittone
// Created:     2009/01/11
// RCS-ID:      $Id$
// Copyright:   (c) 2009 Giovanni Mittone
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef UINT128_H
#define UINT128_H

// Required include
#include "wx/crypstrm/cryptodef.h"
#include "wx/longlong.h"

/**
 * This class represents a 128 bits unsigned integer. Currently, I'm developing it but is working good.
 * Only binary operators ar not implemented, except for shift ones; math end comparison operators are fully working.
 */
class WXDLLIMPEXP_CRYPTOSTREAM wxUint128
{
public:
    /**
     * Default constructor. It initializes the number to 0.
     */
    wxUint128();
    /**
     * This constructor initializes the number accordingly to an hi-part and a lo-part wxULongLong numbers.
     */
    wxUint128(const wxULongLong &high, const wxULongLong &low);
    /**
     * This is the copy constructor. It initializes a wxUint128 class with the value of an other wxUint128 class, as a regular
     * assignment operator.
     */
    wxUint128(const wxUint128 &arg);

    /**
     * Get the hi-part of the number; this part is represented with a wxULongLong number.
     */
    wxULongLong GetHi(){ return m_phigh; };
    /**
     * Get the lo-part of the number; this part is represented with a wxULongLong number.
     */
    wxULongLong GetLo(){ return m_plow; };

    // Operators
    wxUint128 &operator = (const wxUint128 &arg);
    wxUint128 &operator = (const wxULongLong &arg);
    wxUint128 &operator += (const wxUint128 &arg);
    wxUint128 &operator += (const wxULongLong &arg){ *this += wxUint128(0, arg); return *this; };
    wxUint128 operator + (const wxUint128 &arg);
    wxUint128 operator + (const wxULongLong &arg){ return *this + wxUint128(0, arg); };
    wxUint128 &operator ++ ();
    wxUint128 operator ++ (int);
    wxUint128 &operator -= (const wxUint128 &arg);
    wxUint128 &operator -= (const wxULongLong &arg){ *this -= wxUint128(0, arg); return *this; };
    wxUint128 operator - (const wxUint128 &arg);
    wxUint128 operator - (const wxULongLong &arg){ return *this - wxUint128(0, arg); };
    wxUint128 &operator -- ();
    wxUint128 operator -- (int);
    wxUint128 &operator *= (const wxUint128 &arg);
    wxUint128 &operator *= (const wxULongLong &arg){ *this *= wxUint128(0, arg); return *this; };
    wxUint128 operator * (const wxUint128 &arg);
    wxUint128 operator * (const wxULongLong &arg){ return *this * wxUint128(0, arg); };
    wxUint128 &operator /= (const wxUint128 &arg);
    wxUint128 &operator /= (const wxULongLong &arg){ *this /= wxUint128(0, arg); return *this; };
    wxUint128 operator / (const wxUint128 &arg);
    wxUint128 operator / (const wxULongLong &arg){ return *this / wxUint128(0, arg); };
    wxUint128 &operator %= (const wxUint128 &arg);
    wxUint128 &operator %= (const wxULongLong &arg){ *this %= wxUint128(0, arg); return *this; };
    wxUint128 operator % (const wxUint128 &arg);
    wxUint128 operator % (const wxULongLong &arg){ return *this % wxUint128(0, arg); };

    wxUint128 &operator >>= (const size_t &arg);
    wxUint128 operator >> (const size_t &arg);
    wxUint128 &operator <<= (const size_t &arg);
    wxUint128 operator << (const size_t &arg);

    bool operator > (const wxUint128 &arg);
    bool operator > (const wxULongLong &arg){ return *this > wxUint128(0, arg); };
    bool operator < (const wxUint128 &arg);
    bool operator < (const wxULongLong &arg){ return *this < wxUint128(0, arg); };
    bool operator == (const wxUint128 &arg);
    bool operator == (const wxULongLong &arg){ return *this == wxUint128(0, arg); };
    bool operator != (const wxUint128 &arg);
    bool operator != (const wxULongLong &arg){ return *this != wxUint128(0, arg); };
    bool operator >= (const wxUint128 &arg);
    bool operator >= (const wxULongLong &arg){ return *this >= wxUint128(0, arg); };
    bool operator <= (const wxUint128 &arg);
    bool operator <= (const wxULongLong &arg){ return *this <= wxUint128(0, arg); };

private:
    wxULongLong m_phigh, m_plow;

    void Divide(const wxUint128& divisorIn, wxUint128& quotient, wxUint128& remainder);
};

#endif // UINT128_H
