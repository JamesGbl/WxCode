/////////////////////////////////////////////////////////////////////////////
// Name:        sha512.h
// Purpose:     A simple SHA-512 algorithm
// Author:      Giovanni Mittone
// Created:     2009/01/11
// RCS-ID:      $Id$
// Copyright:   (c) 2009 Giovanni Mittone
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#if !defined SHA512_H && wxHAS_INT64

#define SHA512_H

// Required include
#include "wx/crypstrm/cryptodef.h"
#include "wx/crypstrm/shabase.h"
#include "wx/stream.h"


/**
 * This class represents an SHA 512 bits binary buffer.
 */
typedef class
{
public:
    /**
     * The SHA 512 bits binary buffer as an array of 8 64 bits variables.
     */
    wxULongLong H[8];
    /**
     * Converts the binary hash buffer to a human readable string.
     * @return A wxString representing a human readable hash value.
     */
    wxString ToString();
} wxSHA512Hash;


/**
 * This is the SHA-512 class (output hash size is 512 bits).
 * \n\n
 * @code
 * // The following code, show how to hash a file

 * wxSHA512Hash hash;
 * wxSHA512 sha512;

 * wxFileInputStream fis(wxT("name.ext"));

 * sha512.Hash(fis, hash);
 * @endcode
 */
class WXDLLIMPEXP_CRYPTOSTREAM wxSHA512 : public wxSHABase1024
{
public:
    /**
     * This is the default constructor. It does nothing itself, it only extend the wxSHABase1024 constructor.
     * @see wxSHABase1024(const wxSHAType &Wsize)
     */
    wxSHA512() : wxSHABase1024(wxSHABase1024::wxSHA512) {};

    /**
     * Perform the hash calculation over a generic input stream. The input stream length must be determinable.
     * @param input a generic input stream buffer
     * @param hash a wxSHA512Hash valid class; the resulting hash value is stored here.
     * @return True on success, false if an error occurred.
     */
    bool Hash(wxInputStream &input, wxSHA512Hash &hash);
    /**
     * Perform the hash calculation over a wxString. It initialize a string input stream and passes it to the generic hash
     * calculation function.
     * @param input a generic wxString.
     * @param hash a wxSHA512Hash valid class; the resulting hash value is stored here.
     * @return True on success, false if an error occurred.
     * @see Hash(wxInputStream &input, wxSHA512Hash &hash)
     */
    bool Hash(const wxString &input, wxSHA512Hash &hash);
    /**
     * Perform the hash calculation over a binary buffer. It initialize a memory input stream and passes it to the generic
     * hash calculation function.
     * @param input a binary buffer pointer.
     * @param input_size the binary buffer data length.
     * @param hash a wxSHA512Hash valid class; the resulting hash value is stored here.
     * @return True on success, false if an error occurred.
     * @see Hash(wxInputStream &input, wxSHA512Hash &hash)
     */
    bool Hash(const void *input, const size_t &input_size, wxSHA512Hash &hash);

private:
    wxULongLong a, b, c, d, e, f, g, h, T1, T2;

    inline void ComputeTempHash(const size_t &t);
    inline void W16_79(const size_t &t);
};

#endif // SHA512_H
