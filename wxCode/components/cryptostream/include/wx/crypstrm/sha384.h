/////////////////////////////////////////////////////////////////////////////
// Name:        sha384.h
// Purpose:     A simple SHA-384 algorithm
// Author:      Giovanni Mittone
// Created:     2009/01/11
// RCS-ID:      $Id$
// Copyright:   (c) 2009 Giovanni Mittone
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#if !defined SHA384_H && wxHAS_INT64
#define SHA384_H

// Required include
#include "wx/crypstrm/cryptodef.h"
#include "wx/crypstrm/shabase.h"
#include "wx/stream.h"


/**
 * This class represents an SHA 384 bits binary buffer.
 */
typedef class
{
public:
    /**
     * The SHA 384 bits binary buffer as an array of 6 64 bits variables.
     */
    wxULongLong H[6];
    /**
     * Converts the binary hash buffer to a human readable string.
     * @return A wxString representing a human readable hash value.
     */
    wxString ToString();
} wxSHA384Hash;


/**
 * This is the SHA-384 class (output hash size is 384 bits).
 * \n\n
 * @code
 * // The following code, show how to hash a file

 * wxSHA384Hash hash;
 * wxSHA384 sha384;

 * wxFileInputStream fis(wxT("name.ext"));

 * sha384.Hash(fis, hash);
 * @endcode
 */
class WXDLLIMPEXP_CRYPTOSTREAM wxSHA384 : public wxSHABase1024
{
public:
    /**
     * This is the default constructor. It does nothing itself, it only extend the wxSHABase1024 constructor.
     * @see wxSHABase1024(const wxSHAType &Wsize)
     */
    wxSHA384() : wxSHABase1024(wxSHABase1024::wxSHA384) {};

    /**
     * Perform the hash calculation over a generic input stream. The input stream length must be determinable.
     * @param input a generic input stream buffer
     * @param hash a wxSHA384Hash valid class; the resulting hash value is stored here.
     * @return True on success, false if an error occurred.
     */
    bool Hash(wxInputStream &input, wxSHA384Hash &hash);
    /**
     * Perform the hash calculation over a wxString. It initialize a string input stream and passes it to the generic hash
     * calculation function.
     * @param input a generic wxString.
     * @param hash a wxSHA384Hash valid class; the resulting hash value is stored here.
     * @return True on success, false if an error occurred.
     * @see Hash(wxInputStream &input, wxSHA384Hash &hash)
     */
    bool Hash(const wxString &input, wxSHA384Hash &hash);
    /**
     * Perform the hash calculation over a binary buffer. It initialize a memory input stream and passes it to the generic
     * hash calculation function.
     * @param input a binary buffer pointer.
     * @param input_size the binary buffer data length.
     * @param hash a wxSHA384Hash valid class; the resulting hash value is stored here.
     * @return True on success, false if an error occurred.
     * @see Hash(wxInputStream &input, wxSHA384Hash &hash)
     */
    bool Hash(const void *input, const size_t &input_size, wxSHA384Hash &hash);

private:
    wxULongLong a, b, c, d, e, f, g, h, T1, T2, H6, H7;

    inline void ComputeTempHash(const size_t &t);
    inline void W16_79(const size_t &t);
};

#endif // SHA384_H
