/////////////////////////////////////////////////////////////////////////////
// Name:        sha512.cpp
// Purpose:     A simple SHA-512 algorithm
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

#if wxHAS_INT64

// includes
#include "wx/crypstrm/sha512.h"
#include "wx/sstream.h"
#include "wx/mstream.h"

// Data space size
#define SHA512_DATA_PAD_SPACE_0         128
#define SHA512_DATA_PAD_SPACE_1         112

// Initial SHA512 Hash value
#define SHA512_INIT_0                   wxULongLong(0x6a09e667,0xf3bcc908)
#define SHA512_INIT_1                   wxULongLong(0xbb67ae85,0x84caa73b)
#define SHA512_INIT_2                   wxULongLong(0x3c6ef372,0xfe94f82b)
#define SHA512_INIT_3                   wxULongLong(0xa54ff53a,0x5f1d36f1)
#define SHA512_INIT_4                   wxULongLong(0x510e527f,0xade682d1)
#define SHA512_INIT_5                   wxULongLong(0x9b05688c,0x2b3e6c1f)
#define SHA512_INIT_6                   wxULongLong(0x1f83d9ab,0xfb41bd6b)
#define SHA512_INIT_7                   wxULongLong(0x5be0cd19,0x137e2179)

// Operation definition
#define SHL(x,r)                        (x << r)
#define SHR(x,r)                        (x >> r)
#define ROTL(x,r)                       ((x << r) | (x >> (64 - r)))
#define ROTR(x,r)                       ((x >> r) | (x << (64 - r)))
#define CH(x,y,z)                       ((x & y) | ((~x) & z))
#define MAJ(x,y,z)                      ((x & y) | (x & z) | (y & z))
#define SUM0_512(x)                     (ROTR(x,28) ^ ROTR(x,34) ^ ROTR(x,39))
#define SUM1_512(x)                     (ROTR(x,14) ^ ROTR(x,18) ^ ROTR(x,41))
#define SIGMA0_512(x)                   (ROTR(x,1) ^ ROTR(x,8) ^ SHR(x,7))
#define SIGMA1_512(x)                   (ROTR(x,19) ^ ROTR(x,61) ^ SHR(x,6))


// Constants for elaborations in t
static const wxULongLong K_512[] =
    { wxULongLong(0x428a2f98,0xd728ae22), wxULongLong(0x71374491,0x23ef65cd), wxULongLong(0xb5c0fbcf,0xec4d3b2f), wxULongLong(0xe9b5dba5,0x8189dbbc),
      wxULongLong(0x3956c25b,0xf348b538), wxULongLong(0x59f111f1,0xb605d019), wxULongLong(0x923f82a4,0xaf194f9b), wxULongLong(0xab1c5ed5,0xda6d8118),
      wxULongLong(0xd807aa98,0xa3030242), wxULongLong(0x12835b01,0x45706fbe), wxULongLong(0x243185be,0x4ee4b28c), wxULongLong(0x550c7dc3,0xd5ffb4e2),
      wxULongLong(0x72be5d74,0xf27b896f), wxULongLong(0x80deb1fe,0x3b1696b1), wxULongLong(0x9bdc06a7,0x25c71235), wxULongLong(0xc19bf174,0xcf692694),
      wxULongLong(0xe49b69c1,0x9ef14ad2), wxULongLong(0xefbe4786,0x384f25e3), wxULongLong(0x0fc19dc6,0x8b8cd5b5), wxULongLong(0x240ca1cc,0x77ac9c65),
      wxULongLong(0x2de92c6f,0x592b0275), wxULongLong(0x4a7484aa,0x6ea6e483), wxULongLong(0x5cb0a9dc,0xbd41fbd4), wxULongLong(0x76f988da,0x831153b5),
      wxULongLong(0x983e5152,0xee66dfab), wxULongLong(0xa831c66d,0x2db43210), wxULongLong(0xb00327c8,0x98fb213f), wxULongLong(0xbf597fc7,0xbeef0ee4),
      wxULongLong(0xc6e00bf3,0x3da88fc2), wxULongLong(0xd5a79147,0x930aa725), wxULongLong(0x06ca6351,0xe003826f), wxULongLong(0x14292967,0x0a0e6e70),
      wxULongLong(0x27b70a85,0x46d22ffc), wxULongLong(0x2e1b2138,0x5c26c926), wxULongLong(0x4d2c6dfc,0x5ac42aed), wxULongLong(0x53380d13,0x9d95b3df),
      wxULongLong(0x650a7354,0x8baf63de), wxULongLong(0x766a0abb,0x3c77b2a8), wxULongLong(0x81c2c92e,0x47edaee6), wxULongLong(0x92722c85,0x1482353b),
      wxULongLong(0xa2bfe8a1,0x4cf10364), wxULongLong(0xa81a664b,0xbc423001), wxULongLong(0xc24b8b70,0xd0f89791), wxULongLong(0xc76c51a3,0x0654be30),
      wxULongLong(0xd192e819,0xd6ef5218), wxULongLong(0xd6990624,0x5565a910), wxULongLong(0xf40e3585,0x5771202a), wxULongLong(0x106aa070,0x32bbd1b8),
      wxULongLong(0x19a4c116,0xb8d2d0c8), wxULongLong(0x1e376c08,0x5141ab53), wxULongLong(0x2748774c,0xdf8eeb99), wxULongLong(0x34b0bcb5,0xe19b48a8),
      wxULongLong(0x391c0cb3,0xc5c95a63), wxULongLong(0x4ed8aa4a,0xe3418acb), wxULongLong(0x5b9cca4f,0x7763e373), wxULongLong(0x682e6ff3,0xd6b2b8a3),
      wxULongLong(0x748f82ee,0x5defb2fc), wxULongLong(0x78a5636f,0x43172f60), wxULongLong(0x84c87814,0xa1f0ab72), wxULongLong(0x8cc70208,0x1a6439ec),
      wxULongLong(0x90befffa,0x23631e28), wxULongLong(0xa4506ceb,0xde82bde9), wxULongLong(0xbef9a3f7,0xb2c67915), wxULongLong(0xc67178f2,0xe372532b),
      wxULongLong(0xca273ece,0xea26619c), wxULongLong(0xd186b8c7,0x21c0c207), wxULongLong(0xeada7dd6,0xcde0eb1e), wxULongLong(0xf57d4f7f,0xee6ed178),
      wxULongLong(0x06f067aa,0x72176fba), wxULongLong(0x0a637dc5,0xa2c898a6), wxULongLong(0x113f9804,0xbef90dae), wxULongLong(0x1b710b35,0x131c471b),
      wxULongLong(0x28db77f5,0x23047d84), wxULongLong(0x32caab7b,0x40c72493), wxULongLong(0x3c9ebe0a,0x15c9bebc), wxULongLong(0x431d67c4,0x9c100d4c),
      wxULongLong(0x4cc5d4be,0xcb3e42b6), wxULongLong(0x597f299c,0xfc657e2a), wxULongLong(0x5fcb6fab,0x3ad6faec), wxULongLong(0x6c44198c,0x4a475817) };


bool wxSHA512::Hash(wxInputStream &input, wxSHA512Hash &hash)
{
    // Get and verify the length of the stream
    m_length = input.GetLength();
    if((m_length == wxInvalidOffset) || (m_length == 0))
        return false;

    // Initialize H[0,..,7]
    hash.H[0] = SHA512_INIT_0;
    hash.H[1] = SHA512_INIT_1;
    hash.H[2] = SHA512_INIT_2;
    hash.H[3] = SHA512_INIT_3;
    hash.H[4] = SHA512_INIT_4;
    hash.H[5] = SHA512_INIT_5;
    hash.H[6] = SHA512_INIT_6;
    hash.H[7] = SHA512_INIT_7;

    // Initialize data variables
    m_readed = 0;
    m_N = (m_length / SHA512_DATA_PAD_SPACE_1) + 1;
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
        h = hash.H[7];

        // First step SHA512 [0,..,15]
        size_t t;
        for(t = 0; t < 16; t++)
        {
            // Read data to W[t]
            if((input.Read(&W[t], GetWSize()).LastRead() != GetWSize()) && ((m_length - m_readed) != input.LastRead()))
                return false;
            W[t] = wxUINT64_SWAP_ON_LE(W[t].GetValue());

            // Wt = Mt(i)
            M(t, input.LastRead());
            m_readed += input.LastRead();

            ComputeTempHash(t);
        }

        // Second step SHA512 [16,..,79]
        for(; t < 80; t++)
        {
            W16_79(t);
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
        hash.H[7] += h;
    }
    return true;
}

bool wxSHA512::Hash(const wxString &input, wxSHA512Hash &hash)
{
    wxStringInputStream sis(input);
    return Hash(sis, hash);
}

bool wxSHA512::Hash(const void *input, const size_t &input_size, wxSHA512Hash &hash)
{
    wxMemoryInputStream mis(input, input_size);
    return Hash(mis, hash);
}

void wxSHA512::ComputeTempHash(const size_t &t)
{
    T1 = h + SUM1_512(e) + CH(e,f,g) + K_512[t] + W[t];
    T2 = SUM0_512(a) + MAJ(a,b,c);
    h = g;
    g = f;
    f = e;
    e = d + T1;;
    d = c;
    c = b;
    b = a;
    a = T1 + T2;
}

void wxSHA512::W16_79(const size_t &t)
{
    W[t] = SIGMA1_512(W[t - 2]) + W[t - 7] + SIGMA0_512(W[t - 15]) + W[t - 16];
}

wxString wxSHA512Hash::ToString()
{
    wxString sHash;
    // Convert the binary hash to string.
    sHash.Printf(wxT("%.8x%.8x %.8x%.8x %.8x%.8x %.8x%.8x %.8x%.8x %.8x%.8x %.8x%.8x %.8x%.8x"),
        H[0].GetHi(), H[0].GetLo(), H[1].GetHi(), H[1].GetLo(), H[2].GetHi(), H[2].GetLo(), H[3].GetHi(), H[3].GetLo(),
        H[4].GetHi(), H[4].GetLo(), H[5].GetHi(), H[5].GetLo(), H[6].GetHi(), H[6].GetLo(), H[7].GetHi(), H[7].GetLo());
    return sHash;
}

#endif // wxHAS_INT64
