/////////////////////////////////////////////////////////////////////////////
// Name:        sha160.cpp
// Purpose:     A simple SHA-160 algorithm (SHA-1)
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
#include "wx/crypstrm/sha160.h"
#include "wx/sstream.h"
#include "wx/mstream.h"

// Data space size
#define SHA160_DATA_PAD_SPACE_0     64
#define SHA160_DATA_PAD_SPACE_1     56

// Initial SHA160 Hash value
#define SHA160_INIT_0               0x67452301
#define SHA160_INIT_1               0xEFCDAB89
#define SHA160_INIT_2               0x98BADCFE
#define SHA160_INIT_3               0x10325476
#define SHA160_INIT_4               0xC3D2E1F0

// Operation definition
#define SHL(x,r)                    (x << r)
#define SHR(x,r)                    (x >> r)
#define ROTL(x,r)                   ((x << r) | (x >> (32 - r)))
#define ROTR(x,r)                   ((x >> r) | (x << (32 - r)))
#define CH(x,y,z)                   ((x & y) | ((~x) & z))
#define PARITY(x,y,z)               (x ^ y ^ z)
#define MAJ(x,y,z)                  ((x & y) | (x & z) | (y & z))


// Constants for elaborations in t
#define K0_19                       0x5A827999
#define K20_39                      0x6ED9EBA1
#define K40_59                      0x8F1BBCDC
#define K60_79                      0xCA62C1D6


bool wxSHA160::Hash(wxInputStream &input, wxSHA160Hash &hash)
{
    // Get and verify the length of the stream
    m_length = input.GetLength();
    if((m_length == wxInvalidOffset) || (m_length == 0))
        return false;

    // Initialize H[0,..,4]
    hash.H[0] = SHA160_INIT_0;
    hash.H[1] = SHA160_INIT_1;
    hash.H[2] = SHA160_INIT_2;
    hash.H[3] = SHA160_INIT_3;
    hash.H[4] = SHA160_INIT_4;

    // Initialize data variables
    m_readed = 0;
    m_N = (m_length / SHA160_DATA_PAD_SPACE_1) + 1;
    for(i = 0; i < m_N; i++)
    {
        // Initialize temporary HASH value
        a = hash.H[0];
        b = hash.H[1];
        c = hash.H[2];
        d = hash.H[3];
        e = hash.H[4];

        // First step SHA160 [0,..,15]
        size_t t;
        for(t = 0; t < 16; t++)
        {
            // Read data to W[t]
            if((input.Read(&W[t], GetWSize()).LastRead() != GetWSize()) && (input.LastRead() != (m_length - m_readed)))
                return false;
            W[t] = wxUINT32_SWAP_ON_LE(W[t]);
            // Wt = Mt(i)
            M(t, input.LastRead());
            m_readed += input.LastRead();

            // Compute temporary HASH
            T = ROTL(a,5) + CH(b,c,d) + e + K0_19 + W[t];
            e = d;
            d = c;
            c = ROTL(b,30);
            b = a;
            a = T;
        }

        // Second step SHA160 [16,..,19]
        for(; t < 20; t++)
        {
            // Wt = ROTL(1)(W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16])
            W16_79(t);

            // Compute temporary HASH
            T = ROTL(a,5) + CH(b,c,d) + e + K0_19 + W[t];
            e = d;
            d = c;
            c = ROTL(b,30);
            b = a;
            a = T;
        }

        // Third step SHA160 [20,..,39]
        for(; t < 40; t++)
        {
            // Wt = ROTL(1)(W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16])
            W16_79(t);

            // Compute temporary HASH
            T = ROTL(a,5) + PARITY(b,c,d) + e + K20_39 + W[t];
            e = d;
            d = c;
            c = ROTL(b,30);
            b = a;
            a = T;
        }

        // Fourth step SHA160 [40,..,59]
        for(; t < 60; t++)
        {
            // Wt = ROTL(1)(W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16])
            W16_79(t);

            // Compute temporary HASH
            T = ROTL(a,5) + MAJ(b,c,d) + e + K40_59 + W[t];
            e = d;
            d = c;
            c = ROTL(b,30);
            b = a;
            a = T;
        }

        // Fifth step SHA160 [69,..,79]
        for(; t < 80; t++)
        {
            // Wt = ROTL(1)(W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16])
            W16_79(t);

            // Compute temporary HASH
            T = ROTL(a,5) + PARITY(b,c,d) + e + K60_79 + W[t];
            e = d;
            d = c;
            c = ROTL(b,30);
            b = a;
            a = T;
        }

        // Update HASH
        hash.H[0] += a;
        hash.H[1] += b;
        hash.H[2] += c;
        hash.H[3] += d;
        hash.H[4] += e;
    }
    return true;
}

bool wxSHA160::Hash(const wxString &input, wxSHA160Hash &hash)
{
    wxStringInputStream sis(input);
    return Hash(sis, hash);
}

bool wxSHA160::Hash(const void *input, const size_t &input_size, wxSHA160Hash &hash)
{
    wxMemoryInputStream mis(input, input_size);
    return Hash(mis, hash);
}

void wxSHA160::W16_79(const size_t &t)
{
    W[t] = ROTL((W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]), 1);
}

wxString wxSHA160Hash::ToString()
{
    wxString sHash;
    // Convert the binary hash to string.
    sHash.Printf(wxT("%.8x %.8x %.8x %.8x %.8x"), H[0], H[1], H[2], H[3], H[4]);
    return sHash;
}
