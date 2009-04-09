/////////////////////////////////////////////////////////////////////////////
// Name:        shabase.cpp
// Purpose:     SHA base class for 512 bits block based algorithm
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
#include "wx/crypstrm/shabase.h"


//-----------------------------------------------------------------------------
// wxSHABase512
//-----------------------------------------------------------------------------

// Constructor and destructor
wxSHABase512::wxSHABase512(const wxSHAType &type)
{
    W = new wxUint32[type];
}

wxSHABase512::~wxSHABase512()
{
    if(W)
        delete W;
}

void wxSHABase512::M(const size_t &t, const size_t &last_readed)
{
    wxULongLong diff = m_length - m_readed;
    if((diff >= 0) && (last_readed != 0))
    {
        if(diff == 0)
        {
            W[t] = ((t == 0)?0x00000000:0x80000000);
            return;
        }
        if(diff == 1)
        {
            W[t] &= 0xFF000000;
            W[t] |= 0x00800000;
            return;
        }
        if(diff == 2)
        {
            W[t] &= 0xFFFF0000;
            W[t] |= 0x00008000;
            return;
        }
        if(diff == 3)
        {
            W[t] &= 0xFFFFFF00;
            W[t] |= 0x00000080;
            return;
        }
    }
    else
    {
        if(i == (m_N - 1))
        {
            if(t < 14)
                W[t] = 0x00000000;
            else
            {
                if(t == 14)
                    W[t] = wxUINT32_SWAP_ON_BE((wxUint32)((m_length.GetHi() << 3) | (m_length.GetLo() >> 29)));
                else
                    W[t] = wxUINT32_SWAP_ON_BE((wxUint32)(m_length.GetLo() << 3));
            }
        }
        else
            W[t] = ((t == 14)?0x80000000:0x00000000);
    }
}


//-----------------------------------------------------------------------------
// wxSHABase1024
//-----------------------------------------------------------------------------
#if wxHAS_INT64

wxULongLong operator ~ (const wxULongLong &arg)
{
    return wxULongLong(~arg.GetHi(), ~arg.GetLo());
}


// Constructor and destructor
wxSHABase1024::wxSHABase1024(const wxSHAType &type)
{
    W = new wxULongLong[type];
}

wxSHABase1024::~wxSHABase1024()
{
    if(W)
        delete W;
}

void wxSHABase1024::M(const size_t &t, const size_t &last_readed)
{
    wxUint128 diff = m_length - m_readed;
    if((diff >= 0) && (last_readed != 0))
    {
        if(diff == 0)
        {
            W[t] = ((t == 0)?wxULongLong(0x00000000,0x00000000):wxULongLong(0x80000000,0x00000000));
            return;
        }
        if(diff == 1)
        {
            W[t] &= wxULongLong(0xFF000000,0x00000000);
            W[t] |= wxULongLong(0x00800000,0x00000000);
            return;
        }
        if(diff == 2)
        {
            W[t] &= wxULongLong(0xFFFF0000,0x00000000);
            W[t] |= wxULongLong(0x00008000,0x00000000);
            return;
        }
        if(diff == 3)
        {
            W[t] &= wxULongLong(0xFFFFFF00,0x00000000);
            W[t] |= wxULongLong(0x00000080,0x00000000);
            return;
        }
        if(diff == 4)
        {
            W[t] &= wxULongLong(0xFFFFFFFF,0x00000000);
            W[t] |= wxULongLong(0x00000000,0x80000000);
            return;
        }
        if(diff == 5)
        {
            W[t] &= wxULongLong(0xFFFFFFFF,0xFF000000);
            W[t] |= wxULongLong(0x00000000,0x00800000);
            return;
        }
        if(diff == 6)
        {
            W[t] &= wxULongLong(0xFFFFFFFF,0xFFFF0000);
            W[t] |= wxULongLong(0x00000000,0x00008000);
            return;
        }
        if(diff == 7)
        {
            W[t] &= wxULongLong(0xFFFFFFFF,0xFFFFFF00);
            W[t] |= wxULongLong(0x00000000,0x00000080);
            return;
        }
    }
    else
    {
        if(i == (m_N - 1))
        {
            if(t < 14)
                W[t] = wxULongLong(0x00000000,0x00000000);
            else
            {
                if(t == 14)
                    W[t] = wxUINT64_SWAP_ON_BE((wxULongLong)((m_length.GetHi() << 3) | (m_length.GetLo() >> 61)));
                else
                    W[t] = wxUINT64_SWAP_ON_BE((wxULongLong)(m_length.GetLo() << 3));
            }
        }
        else
            W[t] = ((t == 14)?wxULongLong(0x80000000,0x00000000):wxULongLong(0x00000000,0x00000000));
    }
}

#endif // wxHAS_INT64
