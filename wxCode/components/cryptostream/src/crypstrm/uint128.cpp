/////////////////////////////////////////////////////////////////////////////
// Name:        uint128.cpp
// Purpose:     A simple class to represent a 128 bit number
// Author:      Giovanni Mittone
// Created:     2009/01/11
// RCS-ID:      $Id$
// Copyright:   (c) 2009 Giovanni Mittone
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#include "wx/crypstrm/uint128.h"

#define BITS            128
#define MAX_SHIFT       BITS
#define MAX_SHIFT_PART  64

// Constructors of the class
wxUint128::wxUint128()
{
    m_phigh = m_plow = 0;
}

wxUint128::wxUint128(const wxULongLong &high, const wxULongLong &low)
{
    m_phigh = high;
    m_plow = low;
}

wxUint128::wxUint128(const wxUint128 &arg)
{
    m_phigh = arg.m_phigh;
    m_plow = arg.m_plow;
}


// Operators
wxUint128 &wxUint128::operator = (const wxUint128 &arg)
{
    m_phigh = arg.m_phigh;
    m_plow = arg.m_plow;
    return *this;
}

wxUint128 &wxUint128::operator = (const wxULongLong &arg)
{
    m_phigh = 0;
    m_plow = arg;
    return *this;
}

wxUint128 &wxUint128::operator += (const wxUint128 &arg)
{
    wxULongLong temp = m_plow;
    // Add the low-part of the number
    m_plow += arg.m_plow;
    // Add the high-part of the number
    m_phigh += arg.m_phigh;
    // Test for low-part overflow
    if(m_plow < temp)
        m_phigh++;
    return *this;
}

wxUint128 wxUint128::operator + (const wxUint128 &arg)
{
    wxUint128 res = *this;
    res += arg;
    return res;
}

wxUint128 &wxUint128::operator ++ ()
{
    wxULongLong temp = m_plow;
    m_plow++;
    if(m_plow < temp)
        m_phigh++;
    return *this;
}

wxUint128 wxUint128::operator ++ (int arg)
{
    wxUint128 res = *this;
    ++(*this);
    return res;
}

wxUint128 &wxUint128::operator -= (const wxUint128 &arg)
{
    wxULongLong temp = m_plow;
    // Subtract the low-part of the number
    m_plow -= arg.m_plow;
    // Subtract the high-part of the number
    m_phigh -= arg.m_phigh;
    // Test for low-part overflow
    if(m_plow > temp)
        m_phigh--;
    return *this;
}

wxUint128 wxUint128::operator - (const wxUint128 &arg)
{
    wxUint128 res = *this;
    res -= arg;
    return res;
}

wxUint128 &wxUint128::operator -- ()
{
    wxULongLong temp = m_plow;
    m_plow--;
    if(m_plow > temp)
        m_phigh--;
    return *this;
}

wxUint128 wxUint128::operator -- (int arg)
{
    wxUint128 res = *this;
    --(*this);
    return res;
}

wxUint128 &wxUint128::operator *= (const wxUint128 &arg)
{
    // This code is taken from wxULongLong class

    wxUint128 t = *this;
    wxUint128 q = arg;

    m_phigh = m_plow = 0;

    size_t counter = 0;
    do
    {
        if ((q.m_plow & 1) != 0)
            *this += t;
        q >>= 1;
        t <<= 1;
        counter++;
    }
    while((counter < BITS) && ((q.m_phigh != 0) || (q.m_plow != 0)));

    return *this;
}

wxUint128 wxUint128::operator * (const wxUint128 &arg)
{
    wxUint128 res = *this;
    res *= arg;
    return res;
}

wxUint128 &wxUint128::operator /= (const wxUint128 &arg)
{
    wxUint128 remainder;
    Divide(arg, *this, remainder);
    return *this;
}

wxUint128 wxUint128::operator / (const wxUint128 &arg)
{
    wxUint128 res, remainder;
    Divide(arg, res, remainder);
    return res;
}

wxUint128 &wxUint128::operator %= (const wxUint128 &arg)
{
    wxUint128 res;
    Divide(arg, res, *this);
    return *this;
}

wxUint128 wxUint128::operator % (const wxUint128 &arg)
{
    wxUint128 res, remainder;
    Divide(arg, res, remainder);
    return res;
}


wxUint128 &wxUint128::operator >>= (const size_t &arg)
{
    if(arg < MAX_SHIFT)
    {
        if(arg < MAX_SHIFT_PART)
        {
            m_plow = (m_plow >> arg) | (m_phigh << (MAX_SHIFT_PART - arg));
            m_phigh = (m_phigh >> arg);
        }
        else
        {
            m_plow = m_phigh >> (arg - MAX_SHIFT_PART);
            m_phigh = 0;
        }
    }
    else
        // Reset the number
        m_phigh = m_plow = 0;
    return *this;
}

wxUint128 wxUint128::operator >> (const size_t &arg)
{
    wxUint128 res = *this;
    res >>= arg;
    return res;
}

wxUint128 &wxUint128::operator <<= (const size_t &arg)
{
    if(arg < MAX_SHIFT)
    {
        if(arg < MAX_SHIFT_PART)
        {
            m_phigh = (m_phigh << arg) | (m_plow >> (MAX_SHIFT_PART - arg));
            m_plow = (m_plow << arg);
        }
        else
        {
            m_phigh = m_plow << (arg - MAX_SHIFT_PART);
            m_plow = 0;
        }
    }
    else
        // Reset the number
        m_phigh = m_plow = 0;
    return *this;
}

wxUint128 wxUint128::operator << (const size_t &arg)
{
    wxUint128 res = *this;
    res <<= arg;
    return res;
}


bool wxUint128::operator > (const wxUint128 &arg)
{
    return !(*this < arg) && (*this != arg);
}

bool wxUint128::operator < (const wxUint128 &arg)
{
    wxUint128 temp = *this;
    temp -= arg;
    // Verifica che il risultato della sottrazione sia maggiore rispetto al valore iniziale
    return ((m_phigh < temp.m_phigh) || ((m_phigh == temp.m_phigh) && (m_plow < temp.m_plow)));
}

bool wxUint128::operator == (const wxUint128 &arg)
{
    return (m_phigh == arg.m_phigh) && (m_plow == arg.m_plow);
}

bool wxUint128::operator != (const wxUint128 &arg)
{
    return (m_phigh != arg.m_phigh) || (m_plow != arg.m_plow);
}

bool wxUint128::operator >= (const wxUint128 &arg)
{
    return (*this == arg) || (*this > arg);
}

bool wxUint128::operator <= (const wxUint128 &arg)
{
    return (*this == arg) || (*this < arg);
}

void wxUint128::Divide(const wxUint128& divisorIn, wxUint128& quotient, wxUint128& remainder)
{
    // This code is taken from wxULongLong class

    if((divisorIn.m_plow == 0) && (divisorIn.m_phigh == 0))
    {
        // provoke division by zero error
        wxULongLong dummy;
        dummy = divisorIn.m_plow / divisorIn.m_phigh;
    }

    // init everything
     wxUint128 dividend = *this, divisor = divisorIn;

    quotient = 0;
    remainder = 0;

    // check for some particular cases
    if(divisor > dividend)
        remainder = dividend;
    else
    {
        if(divisor == dividend)
            quotient = 1;
        else
        {
            size_t nBits = BITS;
            wxUint128 d;
            while(remainder < divisor)
            {
                remainder <<= 1;

                if(dividend.m_phigh.GetHi() & 0x80000000)
                    remainder.m_plow |= 1;

                d = dividend;
                dividend <<= 1;

                nBits--;
            }

            // undo the last loop iteration
            dividend = d;
            remainder >>= 1;
            nBits++;

            for(size_t i = 0; i < nBits; i++)
            {
                remainder <<= 1;
                if(dividend.m_phigh.GetHi() & 0x80000000)
                    remainder.m_plow |= 1;

                wxUint128 t = remainder - divisor;
                dividend <<= 1;
                quotient <<= 1;
                if((t.m_phigh.GetHi() & 0x80000000) == 0)
                {
                    quotient.m_plow |= 1;
                    remainder = t;
                }
            }
        }
    }
}
