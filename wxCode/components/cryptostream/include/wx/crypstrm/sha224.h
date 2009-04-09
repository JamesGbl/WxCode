/////////////////////////////////////////////////////////////////////////////
// Name:        sha224.h
// Purpose:     A simple SHA-224 algorithm
// Author:      Giovanni Mittone
// Created:     2009/01/11
// RCS-ID:      $Id$
// Copyright:   (c) 2009 Giovanni Mittone
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef SHA224_H
#define SHA224_H

// Required include
#include "wx/crypstrm/cryptodef.h"
#include "wx/crypstrm/shabase.h"
#include "wx/stream.h"


/**
 * This class represents an SHA 224 bits binary buffer.
 */
typedef class
{
public:
    /**
     * The SHA 224 bits binary buffer as an array of 7 32 bits variables.
     */
    wxUint32 H[7];
    /**
     * Converts the binary hash buffer to a human readable string.
     * @return A wxString representing a human readable hash value.
     */
    wxString ToString();
} wxSHA224Hash;


/**
 * This is the SHA-224 class (output hash size is 224 bits).
 * \n\n
 * @code
 * // The following code, show how to hash a file

 * wxSHA224Hash hash;
 * wxSHA224 sha224;

 * wxFileInputStream fis(wxT("name.ext"));

 * sha224.Hash(fis, hash);
 * @endcode
 */
class WXDLLIMPEXP_CRYPTOSTREAM wxSHA224 : public wxSHABase512
{
public:
    /**
     * This is the default constructor. It does nothing itself, it only extend the wxSHABase512 constructor.
     * @see wxSHABase512(const wxSHAType &Wsize)
     */
    wxSHA224() : wxSHABase512(wxSHABase512::wxSHA224) {};

    /**
     * Perform the hash calculation over a generic input stream. The input stream length must be determinable.
     * @param input a generic input stream buffer
     * @param hash a wxSHA224Hash valid class; the resulting hash value is stored here.
     * @return True on success, false if an error occurred.
     */
    bool Hash(wxInputStream &input, wxSHA224Hash &hash);
    /**
     * Perform the hash calculation over a wxString. It initialize a string input stream and passes it to the generic hash
     * calculation function.
     * @param input a generic wxString.
     * @param hash a wxSHA224Hash valid class; the resulting hash value is stored here.
     * @return True on success, false if an error occurred.
     * @see Hash(wxInputStream &input, wxSHA224Hash &hash)
     */
    bool Hash(const wxString &input, wxSHA224Hash &hash);
    /**
     * Perform the hash calculation over a binary buffer. It initialize a memory input stream and passes it to the generic
     * hash calculation function.
     * @param input a binary buffer pointer.
     * @param input_size the binary buffer data length.
     * @param hash a wxSHA224Hash valid class; the resulting hash value is stored here.
     * @return True on success, false if an error occurred.
     * @see Hash(wxInputStream &input, wxSHA224Hash &hash)
     */
    bool Hash(const void *input, const size_t &input_size, wxSHA224Hash &hash);

private:
    wxUint32 a, b, c, d, e, f, g, h, T1, T2, H7;

    inline void ComputeTempHash(const size_t &t);
    inline void W16_63(const size_t &t);
};

#endif // SHA224_H
