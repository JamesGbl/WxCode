/////////////////////////////////////////////////////////////////////////////
// Name:        sha256.h
// Purpose:     A simple SHA-256 algorithm
// Author:      Giovanni Mittone
// Created:     2009/01/11
// RCS-ID:      $Id$
// Copyright:   (c) 2009 Giovanni Mittone
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef SHA256_H
#define SHA256_H

// Required include
#include "wx/crypstrm/cryptodef.h"
#include "wx/crypstrm/shabase.h"
#include "wx/stream.h"


/**
 * This class represents an SHA 256 bits binary buffer.
 */
typedef class
{
public:
    /**
     * The SHA 256 bits binary buffer as an array of 8 32 bits variables.
     */
    wxUint32 H[8];
    /**
     * Converts the binary hash buffer to a human readable string.
     * @return A wxString representing a human readable hash value.
     */
    wxString ToString();
} wxSHA256Hash;


/**
 * This is the SHA-256 class (output hash size is 256 bits).
 * \n\n
 * @code
 * // The following code, show how to hash a file

 * wxSHA256Hash hash;
 * wxSHA256 sha256;

 * wxFileInputStream fis(wxT("name.ext"));

 * sha256.Hash(fis, hash);
 * @endcode
 */
class WXDLLIMPEXP_CRYPTOSTREAM wxSHA256 : public wxSHABase512
{
public:
    /**
     * This is the default constructor. It does nothing itself, it only extend the wxSHABase512 constructor.
     * @see wxSHABase512(const wxSHAType &Wsize)
     */
    wxSHA256() : wxSHABase512(wxSHABase512::wxSHA256) {};

    /**
     * Perform the hash calculation over a generic input stream. The input stream length must be determinable.
     * @param input a generic input stream buffer
     * @param hash a wxSHA256Hash valid class; the resulting hash value is stored here.
     * @return True on success, false if an error occurred.
     */
    bool Hash(wxInputStream &input, wxSHA256Hash &hash);
    /**
     * Perform the hash calculation over a wxString. It initialize a string input stream and passes it to the generic hash
     * calculation function.
     * @param input a generic wxString.
     * @param hash a wxSHA256Hash valid class; the resulting hash value is stored here.
     * @return True on success, false if an error occurred.
     * @see Hash(wxInputStream &input, wxSHA256Hash &hash)
     */
    bool Hash(const wxString &input, wxSHA256Hash &hash);
    /**
     * Perform the hash calculation over a binary buffer. It initialize a memory input stream and passes it to the generic
     * hash calculation function.
     * @param input a binary buffer pointer.
     * @param input_size the binary buffer data length.
     * @param hash a wxSHA256Hash valid class; the resulting hash value is stored here.
     * @return True on success, false if an error occurred.
     * @see Hash(wxInputStream &input, wxSHA256Hash &hash)
     */
    bool Hash(const void *input, const size_t &input_size, wxSHA256Hash &hash);

private:
    wxUint32 a, b, c, d, e, f, g, h, T1, T2;

    inline void ComputeTempHash(const size_t &t);
    inline void W16_63(const size_t &t);
};

#endif // SHA256_H
