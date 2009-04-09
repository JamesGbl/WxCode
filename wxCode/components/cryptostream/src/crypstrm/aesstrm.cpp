/////////////////////////////////////////////////////////////////////////////
// Name:        aesstrm.cpp
// Purpose:     A simple AES cipher/decipher algorithm
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
#include "wx/crypstrm/aesstrm.h"
#include "wx/sstream.h"
#include "wx/crypstrm/sha256.h"

#define XTIME(x)    ((x & 0x80)?((x << 1) ^ 0x1B):(x << 1))
#define XTIME3(x)   (XTIME(x) ^ x)
#define XTIME5(x)   (XTIME((XTIME(x))) ^ x)
#define XTIME6(x)   (XTIME((XTIME3(x))))
#define XTIME8(x)   (XTIME((XTIME((XTIME(x))))))


// Substitution Table
static const wxUint8 SBox[] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

// Reverse Substitution Table
static const wxUint8 RSBox[] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

// Round Constants
static const wxUint8 RCon[] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
    0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
    0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
    0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
    0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
    0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
    0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
    0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
    0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
    0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
    0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
    0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
    0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
    0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
    0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
    0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb };


//-----------------------------------------------------------------------------
// wxAESBase
//-----------------------------------------------------------------------------

wxAESBase::wxAESBase()
{
    SetKey(wxAESUnknown, NULL);
}

wxAESBase::~wxAESBase()
{
}


void wxAESBase::SetKey(wxAESKeyType type, void *key)
{
    m_type = type;

    switch(m_type)
    {
    case wxAES128:
        m_keylength = 16;
        m_schedulersize = 176;
        m_rounds = 10;
        break;
    case wxAES192:
        m_keylength = 24;
        m_schedulersize = 208;
        m_rounds = 12;
        break;
    case wxAES256:
        m_keylength = 32;
        m_schedulersize = 240;
        m_rounds = 14;
        break;
    default:
        m_keylength = 0;
        m_schedulersize = 0;
        m_rounds = 0;
        return;
    }

    // Set first key scheduler
    size_t cont;
    for(cont = 0; cont < m_keylength; cont++)
        m_keyscheduler[cont] = ((wxUint8*)key)[cont];

    // Key expansion
    KeyExpansion();
}

void wxAESBase::SetKey(wxAESKeyType type, const wxInputStream &key)
{
    wxSHA256Hash hash;
    wxSHA256 sha256;
    // Hash the input string
    sha256.Hash((wxInputStream&)key, hash);
    // Adjust the hash to be correctly readed byte per byte (architecture dependent)
    hash.H[0] = wxUINT32_SWAP_ON_BE(hash.H[0]);
    hash.H[1] = wxUINT32_SWAP_ON_BE(hash.H[1]);
    hash.H[2] = wxUINT32_SWAP_ON_BE(hash.H[2]);
    hash.H[3] = wxUINT32_SWAP_ON_BE(hash.H[3]);
    hash.H[4] = wxUINT32_SWAP_ON_BE(hash.H[4]);
    hash.H[5] = wxUINT32_SWAP_ON_BE(hash.H[5]);
    hash.H[6] = wxUINT32_SWAP_ON_BE(hash.H[6]);
    hash.H[7] = wxUINT32_SWAP_ON_BE(hash.H[7]);
    // Passes the key to the binary key processor
    SetKey(type, (wxUint8*)&hash);
}

void wxAESBase::SetKey(wxAESKeyType type, const wxString &key)
{
    wxStringInputStream sis(key);
    SetKey(type, sis);
}

void wxAESBase::SetKeyFrom(const wxAESBase &base)
{
    // Copy data from another object
    m_type = base.m_type;
    m_keylength = base.m_keylength;
    m_schedulersize = base.m_schedulersize;
    //m_rowlength = base.m_rowlength;
    m_rounds = base.m_rounds;
    memmove(m_keyscheduler, base.m_keyscheduler, sizeof(m_keyscheduler));
}


// Key expansion functions
void wxAESBase::KeyExpansion()
{
    size_t cont;
    for(cont = m_keylength; cont < m_schedulersize; cont += 4)
    {
        wxUint8 *curscheduler = &m_keyscheduler[cont];
        wxUint8 *last_key = &m_keyscheduler[cont - 4];
        wxUint8 *last_round = &m_keyscheduler[cont - m_keylength];
        size_t mod = cont % m_keylength;

        if(mod == 0)
            RotSubRoundAddWord(curscheduler, last_key, last_round, cont);
        else
        {
            if((m_type > wxAES192) && (mod == wxAES_STATE_SIZE))
                SubAddWord(curscheduler, last_key, last_round);
            else
                AddRoundWord(curscheduler, last_round, last_key);
        }
    }
}

void wxAESBase::AddRoundWord(wxUint8 *&curscheduler, wxUint8 *&last_round, wxUint8 *&last_key)
{
    curscheduler[0] = last_round[0] ^ last_key[0];
    curscheduler[1] = last_round[1] ^ last_key[1];
    curscheduler[2] = last_round[2] ^ last_key[2];
    curscheduler[3] = last_round[3] ^ last_key[3];
}

void wxAESBase::RotSubRoundAddWord(wxUint8 *&curscheduler, wxUint8 *&last_key, wxUint8 *&last_round, const size_t &cont)
{
    curscheduler[0] =  SBox[last_key[1]] ^ RCon[cont / m_keylength] ^ last_round[0];
    curscheduler[1] =  SBox[last_key[2]] ^ last_round[1];
    curscheduler[2] =  SBox[last_key[3]] ^ last_round[2];
    curscheduler[3] =  SBox[last_key[0]] ^ last_round[3];
}

void wxAESBase::SubAddWord(wxUint8 *&curscheduler, wxUint8 *&last_key, wxUint8 *&last_round)
{
    curscheduler[0] =  SBox[last_key[0]] ^ last_round[0];
    curscheduler[1] =  SBox[last_key[1]] ^ last_round[1];
    curscheduler[2] =  SBox[last_key[2]] ^ last_round[2];
    curscheduler[3] =  SBox[last_key[3]] ^ last_round[3];
}


//-----------------------------------------------------------------------------
// wxAESCipher
//-----------------------------------------------------------------------------

// Cipher function
bool wxAESCipher::Cipher(wxInputStream &plaintext, wxOutputStream &ciphertext, bool with_size)
{
    m_psize = with_size;
    // Gets the length of the input stream
    m_ptsize = plaintext.GetLength();
    // Test for minimal conditions
    if((m_type != wxAESBase::wxAESUnknown) && (m_ptsize != wxInvalidOffset) && (m_ptsize > 0))
    {
        m_ptcount = 0;
        // Cipher data from source stream (plaintext) to destination (ciphertext)
        while(m_ptcount < m_ptsize)
        {
            if(!Add(plaintext))
                return false;

            for(m_pround = 1; m_pround < m_rounds; m_pround++)
            {
                SubShift();
                MixAdd();
            }

            if(!SubShiftAdd(ciphertext))
                return false;

            m_ptcount += wxAES_STATE_SIZE;
        }
        return true;
    }
    else
        return false;
}

bool wxAESCipher::Add(wxInputStream &plaintext)
{
    if((m_psize) && (m_ptcount == 0))
    {
        // Copy the length of the stream to the first working buffer
        *((typeof(m_ptsize)*)m_state) = wxUINT64_SWAP_ON_BE(m_ptsize);
        // Copy datas to the first working buffer
        if(plaintext.Read(&m_state[sizeof(m_ptsize)], wxAES_STATE_SIZE - sizeof(m_ptsize)).LastRead() == 0)
            return false;
    }
    else
    {
        // Copy datas to the first working buffer
        if(plaintext.Read(m_state, wxAES_STATE_SIZE).LastRead() == 0)
            return false;
    }
    // AddRoundKey(0)
    size_t cont;
    for(cont = 0; cont < wxAES_STATE_SIZE; cont++)
        m_state[cont] ^= m_keyscheduler[cont];
    return true;
}

void wxAESCipher::SubShift()
{
    // First column
    m_tstate[ 0] = SBox[m_state[ 0]];
    m_tstate[ 4] = SBox[m_state[ 4]];
    m_tstate[ 8] = SBox[m_state[ 8]];
    m_tstate[12] = SBox[m_state[12]];
    // Second column
    m_tstate[13] = SBox[m_state[ 1]];
    m_tstate[ 1] = SBox[m_state[ 5]];
    m_tstate[ 5] = SBox[m_state[ 9]];
    m_tstate[ 9] = SBox[m_state[13]];
    // Third column
    m_tstate[10] = SBox[m_state[ 2]];
    m_tstate[14] = SBox[m_state[ 6]];
    m_tstate[ 2] = SBox[m_state[10]];
    m_tstate[ 6] = SBox[m_state[14]];
    // Fourth column
    m_tstate[ 7] = SBox[m_state[ 3]];
    m_tstate[11] = SBox[m_state[ 7]];
    m_tstate[15] = SBox[m_state[11]];
    m_tstate[ 3] = SBox[m_state[15]];
}

void wxAESCipher::MixAdd()
{
    wxUint8 *curscheduler = &m_keyscheduler[wxAES_STATE_SIZE * m_pround];
    wxUint8 t1;
    // First column
    t1 = m_tstate[0] ^ m_tstate[1] ^ m_tstate[2] ^ m_tstate[3];
    m_state[ 0] = m_tstate[ 0] ^ t1 ^ XTIME((m_tstate[ 0] ^ m_tstate[ 1])) ^ curscheduler[ 0];
    m_state[ 1] = m_tstate[ 1] ^ t1 ^ XTIME((m_tstate[ 1] ^ m_tstate[ 2])) ^ curscheduler[ 1];
    m_state[ 2] = m_tstate[ 2] ^ t1 ^ XTIME((m_tstate[ 2] ^ m_tstate[ 3])) ^ curscheduler[ 2];
    m_state[ 3] = m_tstate[ 3] ^ t1 ^ XTIME((m_tstate[ 3] ^ m_tstate[ 0])) ^ curscheduler[ 3];
    // Second column
    t1 = m_tstate[4] ^ m_tstate[5] ^ m_tstate[6] ^ m_tstate[7];
    m_state[ 4] = m_tstate[ 4] ^ t1 ^ XTIME((m_tstate[ 4] ^ m_tstate[ 5])) ^ curscheduler[ 4];
    m_state[ 5] = m_tstate[ 5] ^ t1 ^ XTIME((m_tstate[ 5] ^ m_tstate[ 6])) ^ curscheduler[ 5];
    m_state[ 6] = m_tstate[ 6] ^ t1 ^ XTIME((m_tstate[ 6] ^ m_tstate[ 7])) ^ curscheduler[ 6];
    m_state[ 7] = m_tstate[ 7] ^ t1 ^ XTIME((m_tstate[ 7] ^ m_tstate[ 4])) ^ curscheduler[ 7];
    // Third column
    t1 = m_tstate[8] ^ m_tstate[9] ^ m_tstate[10] ^ m_tstate[11];
    m_state[ 8] = m_tstate[ 8] ^ t1 ^ XTIME((m_tstate[ 8] ^ m_tstate[ 9])) ^ curscheduler[ 8];
    m_state[ 9] = m_tstate[ 9] ^ t1 ^ XTIME((m_tstate[ 9] ^ m_tstate[10])) ^ curscheduler[ 9];
    m_state[10] = m_tstate[10] ^ t1 ^ XTIME((m_tstate[10] ^ m_tstate[11])) ^ curscheduler[10];
    m_state[11] = m_tstate[11] ^ t1 ^ XTIME((m_tstate[11] ^ m_tstate[ 8])) ^ curscheduler[11];
    // Fourth column
    t1 = m_tstate[12] ^ m_tstate[13] ^ m_tstate[14] ^ m_tstate[15];
    m_state[12] = m_tstate[12] ^ t1 ^ XTIME((m_tstate[12] ^ m_tstate[13])) ^ curscheduler[12];
    m_state[13] = m_tstate[13] ^ t1 ^ XTIME((m_tstate[13] ^ m_tstate[14])) ^ curscheduler[13];
    m_state[14] = m_tstate[14] ^ t1 ^ XTIME((m_tstate[14] ^ m_tstate[15])) ^ curscheduler[14];
    m_state[15] = m_tstate[15] ^ t1 ^ XTIME((m_tstate[15] ^ m_tstate[12])) ^ curscheduler[15];
}

bool wxAESCipher::SubShiftAdd(wxOutputStream &ciphertext)
{
    SubShift();
    // AddRoundKey(m_rounds)
    size_t cont;
    wxUint8 *curscheduler = &m_keyscheduler[wxAES_STATE_SIZE * m_rounds];
    for(cont = 0; cont < wxAES_STATE_SIZE; cont++)
        m_state[cont] = m_tstate[cont] ^ curscheduler[cont];
    // Copy datas to the output stream
    return (ciphertext.Write(m_state, wxAES_STATE_SIZE).LastWrite() == wxAES_STATE_SIZE);
}


//-----------------------------------------------------------------------------
// wxAESDecipher
//-----------------------------------------------------------------------------

// Decipher function
bool wxAESDecipher::Decipher(wxInputStream &ciphertext, wxOutputStream &plaintext, const wxLongLong &cipherlength)
{
    if(cipherlength >= 0)
    {
        m_ptsize = cipherlength;
        m_psize = false;
    }
    else
        m_psize = true;

    if(m_type != wxAESBase::wxAESUnknown)
    {
        m_ptcount = 0;
        // Decipher data from source stream (ciphertext) to destination (plaintext)
        do
        {
            Add(ciphertext);

            for(m_pround = m_rounds - 1; m_pround > 0; m_pround--)
            {
                InvSubShiftAdd();
                InvMix();
            }

            InvSubShiftAdd(plaintext);

            m_ptcount += wxAES_STATE_SIZE;
        }while(m_ptcount < m_ptsize);
        return true;
    }
    else
        return false;
}

bool wxAESDecipher::Add(wxInputStream &ciphertext)
{
    // Copy datas to the first working buffer
    if(ciphertext.Read(m_state, wxAES_STATE_SIZE).LastRead() != wxAES_STATE_SIZE)
        return false;
    // AddRoundKey(m_rounds)
    size_t cont = 0;
    wxUint8 *curscheduler = &m_keyscheduler[wxAES_STATE_SIZE * m_rounds];
    for(; cont < wxAES_STATE_SIZE; cont++)
        m_state[cont] ^= curscheduler[cont];
    return true;
}

void wxAESDecipher::InvSubShiftAdd()
{
    wxUint8 *curscheduler = &m_keyscheduler[wxAES_STATE_SIZE * m_pround];
    // First column
    m_tstate[ 0] = RSBox[m_state[ 0]] ^ curscheduler[ 0];
    m_tstate[ 4] = RSBox[m_state[ 4]] ^ curscheduler[ 4];
    m_tstate[ 8] = RSBox[m_state[ 8]] ^ curscheduler[ 8];
    m_tstate[12] = RSBox[m_state[12]] ^ curscheduler[12];
    // Second column
    m_tstate[ 5] = RSBox[m_state[ 1]] ^ curscheduler[ 5];
    m_tstate[ 9] = RSBox[m_state[ 5]] ^ curscheduler[ 9];
    m_tstate[13] = RSBox[m_state[ 9]] ^ curscheduler[13];
    m_tstate[ 1] = RSBox[m_state[13]] ^ curscheduler[ 1];
    // Third column
    m_tstate[10] = RSBox[m_state[ 2]] ^ curscheduler[10];
    m_tstate[14] = RSBox[m_state[ 6]] ^ curscheduler[14];
    m_tstate[ 2] = RSBox[m_state[10]] ^ curscheduler[ 2];
    m_tstate[ 6] = RSBox[m_state[14]] ^ curscheduler[ 6];
    // Fourth column
    m_tstate[15] = RSBox[m_state[ 3]] ^ curscheduler[15];
    m_tstate[ 3] = RSBox[m_state[ 7]] ^ curscheduler[ 3];
    m_tstate[ 7] = RSBox[m_state[11]] ^ curscheduler[ 7];
    m_tstate[11] = RSBox[m_state[15]] ^ curscheduler[11];
}

void wxAESDecipher::InvMix()
{
    wxUint8 t1;
    // First column
    t1 = XTIME8(m_tstate[0]) ^ XTIME8(m_tstate[1]) ^ XTIME8(m_tstate[2]) ^ XTIME8(m_tstate[3]);
    m_state[ 0] = t1 ^ XTIME6(m_tstate[ 0]) ^ XTIME3(m_tstate[ 1]) ^ XTIME5(m_tstate[ 2]) ^ m_tstate[ 3];
    m_state[ 1] = t1 ^ m_tstate[ 0] ^ XTIME6(m_tstate[ 1]) ^ XTIME3(m_tstate[ 2]) ^ XTIME5(m_tstate[ 3]);
    m_state[ 2] = t1 ^ XTIME5(m_tstate[ 0]) ^ m_tstate[ 1] ^ XTIME6(m_tstate[ 2]) ^ XTIME3(m_tstate[ 3]);
    m_state[ 3] = t1 ^ XTIME3(m_tstate[ 0]) ^ XTIME5(m_tstate[ 1]) ^ m_tstate[ 2] ^ XTIME6(m_tstate[ 3]);
    // Second column
    t1 = XTIME8(m_tstate[4]) ^ XTIME8(m_tstate[5]) ^ XTIME8(m_tstate[6]) ^ XTIME8(m_tstate[7]);
    m_state[ 4] = t1 ^ XTIME6(m_tstate[ 4]) ^ XTIME3(m_tstate[ 5]) ^ XTIME5(m_tstate[ 6]) ^ m_tstate[ 7];
    m_state[ 5] = t1 ^ m_tstate[ 4] ^ XTIME6(m_tstate[ 5]) ^ XTIME3(m_tstate[ 6]) ^ XTIME5(m_tstate[ 7]);
    m_state[ 6] = t1 ^ XTIME5(m_tstate[ 4]) ^ m_tstate[ 5] ^ XTIME6(m_tstate[ 6]) ^ XTIME3(m_tstate[ 7]);
    m_state[ 7] = t1 ^ XTIME3(m_tstate[ 4]) ^ XTIME5(m_tstate[ 5]) ^ m_tstate[ 6] ^ XTIME6(m_tstate[ 7]);
    // Third column
    t1 = XTIME8(m_tstate[8]) ^ XTIME8(m_tstate[9]) ^ XTIME8(m_tstate[10]) ^ XTIME8(m_tstate[11]);
    m_state[ 8] = t1 ^ XTIME6(m_tstate[ 8]) ^ XTIME3(m_tstate[ 9]) ^ XTIME5(m_tstate[10]) ^ m_tstate[11];
    m_state[ 9] = t1 ^ m_tstate[ 8] ^ XTIME6(m_tstate[ 9]) ^ XTIME3(m_tstate[10]) ^ XTIME5(m_tstate[11]);
    m_state[10] = t1 ^ XTIME5(m_tstate[ 8]) ^ m_tstate[ 9] ^ XTIME6(m_tstate[10]) ^ XTIME3(m_tstate[11]);
    m_state[11] = t1 ^ XTIME3(m_tstate[ 8]) ^ XTIME5(m_tstate[ 9]) ^ m_tstate[10] ^ XTIME6(m_tstate[11]);
    // Fourth column
    t1 = XTIME8(m_tstate[12]) ^ XTIME8(m_tstate[13]) ^ XTIME8(m_tstate[14]) ^ XTIME8(m_tstate[15]);
    m_state[12] = t1 ^ XTIME6(m_tstate[12]) ^ XTIME3(m_tstate[13]) ^ XTIME5(m_tstate[14]) ^ m_tstate[15];
    m_state[13] = t1 ^ m_tstate[12] ^ XTIME6(m_tstate[13]) ^ XTIME3(m_tstate[14]) ^ XTIME5(m_tstate[15]);
    m_state[14] = t1 ^ XTIME5(m_tstate[12]) ^ m_tstate[13] ^ XTIME6(m_tstate[14]) ^ XTIME3(m_tstate[15]);
    m_state[15] = t1 ^ XTIME3(m_tstate[12]) ^ XTIME5(m_tstate[13]) ^ m_tstate[14] ^ XTIME6(m_tstate[15]);
}

bool wxAESDecipher::InvSubShiftAdd(wxOutputStream &plaintext)
{
    m_pround = 0;
    InvSubShiftAdd();

    // Copy datas to the output stream
    if((m_psize) && (m_ptcount == 0))
    {
        // Copy the length of the stream from the first working buffer
        m_ptsize = wxUINT64_SWAP_ON_BE(*((typeof(m_ptsize)*)m_tstate));
        // Copy datas to the output stream
        if(m_ptsize < (typeof(m_ptsize))(wxAES_STATE_SIZE - sizeof(m_ptsize)))
            return (plaintext.Write(&m_tstate[sizeof(m_ptsize)], m_ptsize.GetValue()).LastWrite() == m_ptsize.GetValue());
        else
            return (plaintext.Write(&m_tstate[sizeof(m_ptsize)], (wxAES_STATE_SIZE - sizeof(m_ptsize))).LastWrite() ==
                (wxAES_STATE_SIZE - sizeof(m_ptsize)));
    }
    else
    {
        // Copy datas to the output stream
        if(m_ptsize - m_ptcount < wxAES_STATE_SIZE)
            return (plaintext.Write(m_tstate, m_ptsize.GetValue() - m_ptcount.GetValue()).LastWrite() ==
                (m_ptsize.GetValue() - m_ptcount.GetValue()));
        else
            return (plaintext.Write(m_tstate, wxAES_STATE_SIZE).LastWrite() == wxAES_STATE_SIZE);
    }
}

#endif // wxHAS_INT64
