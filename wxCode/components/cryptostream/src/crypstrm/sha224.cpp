/////////////////////////////////////////////////////////////////////////////
// Name:        sha224.cpp
// Purpose:     A simple SHA-224 algorithm
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
#include "wx/crypstrm/sha224.h"
#include "wx/sstream.h"
#include "wx/mstream.h"

// Data space size
#define SHA224_DATA_PAD_SPACE_0         64
#define SHA224_DATA_PAD_SPACE_1         56

// Initial SHA224 Hash value
#define SHA224_INIT_0                   0xC1059ED8
#define SHA224_INIT_1                   0x367CD507
#define SHA224_INIT_2                   0x3070DD17
#define SHA224_INIT_3                   0xF70E5939
#define SHA224_INIT_4                   0xFFC00B31
#define SHA224_INIT_5                   0x68581511
#define SHA224_INIT_6                   0x64F98FA7
#define SHA224_INIT_7                   0xBEFA4FA4

// Operation definition
#define SHL(x,r)                        (x << r)
#define SHR(x,r)                        (x >> r)
#define ROTL(x,r)                       ((x << r) | (x >> (32 - r)))
#define ROTR(x,r)                       ((x >> r) | (x << (32 - r)))
#define CH(x,y,z)                       ((x & y) | ((~x) & z))
#define MAJ(x,y,z)                      ((x & y) | (x & z) | (y & z))
#define SUM0_256(x)                     (ROTR(x,2) ^ ROTR(x,13) ^ ROTR(x,22))
#define SUM1_256(x)                     (ROTR(x,6) ^ ROTR(x,11) ^ ROTR(x,25))
#define SIGMA0_256(x)                   (ROTR(x,7) ^ ROTR(x,18) ^ SHR(x,3))
#define SIGMA1_256(x)                   (ROTR(x,17) ^ ROTR(x,19) ^ SHR(x,10))


// Constants for elaborations in t
static const wxUint32 K_224[] =
    { 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
      0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
      0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
      0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
      0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
      0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
      0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
      0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };


bool wxSHA224::Hash(wxInputStream &input, wxSHA224Hash &hash)
{
    // Get and verify the length of the stream
    m_length = input.GetLength();
    if((m_length == wxInvalidOffset) || (m_length == 0))
        return false;

    // Initialize H[0,..,7]
    hash.H[0] = SHA224_INIT_0;
    hash.H[1] = SHA224_INIT_1;
    hash.H[2] = SHA224_INIT_2;
    hash.H[3] = SHA224_INIT_3;
    hash.H[4] = SHA224_INIT_4;
    hash.H[5] = SHA224_INIT_5;
    hash.H[6] = SHA224_INIT_6;
    H7        = SHA224_INIT_7;

    // Initialize data variables
    m_readed = 0;
    m_N = (m_length / SHA224_DATA_PAD_SPACE_1) + 1;
    for(i = 0; i < m_N; i++)
    {
        // Initialize temporary HASH value
        a = hash.H[0];
        b = hash.H[1];
        c = hash.H[2];
        d = hash.H[3];
        e = hash.H[4];
        f = hash.H[5];
        g = hash.H[6];
        h = H7;

        // First step SHA224 [0,..,15]
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

            ComputeTempHash(t);
        }

        // Second step SHA224 [16,..,64]
        for(; t < 64; t++)
        {
            W16_63(t);
            ComputeTempHash(t);
        }

        // Update HASH
        hash.H[0] += a;
        hash.H[1] += b;
        hash.H[2] += c;
        hash.H[3] += d;
        hash.H[4] += e;
        hash.H[5] += f;
        hash.H[6] += g;
        H7        += h;
    }
    return true;
}

bool wxSHA224::Hash(const wxString &input, wxSHA224Hash &hash)
{
    wxStringInputStream sis(input);
    return Hash(sis, hash);
}

bool wxSHA224::Hash(const void *input, const size_t &input_size, wxSHA224Hash &hash)
{
    wxMemoryInputStream mis(input, input_size);
    return Hash(mis, hash);
}

void wxSHA224::ComputeTempHash(const size_t &t)
{
    T1 = h + SUM1_256(e) + CH(e,f,g) + K_224[t] + W[t];
    T2 = SUM0_256(a) + MAJ(a,b,c);
    h = g;
    g = f;
    f = e;
    e = d + T1;
    d = c;
    c = b;
    b = a;
    a = T1 + T2;
}

void wxSHA224::W16_63(const size_t &t)
{
    W[t] = SIGMA1_256(W[t - 2]) + W[t - 7] + SIGMA0_256(W[t - 15]) + W[t - 16];
}

wxString wxSHA224Hash::ToString()
{
    wxString sHash;
    // Convert the binary hash to string.
    sHash.Printf(wxT("%.8x %.8x %.8x %.8x %.8x %.8x %.8x"), H[0], H[1], H[2], H[3], H[4], H[5], H[6]);
    return sHash;
}
