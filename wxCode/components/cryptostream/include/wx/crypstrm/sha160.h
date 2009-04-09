/////////////////////////////////////////////////////////////////////////////
// Name:        sha160.h
// Purpose:     A simple SHA-160 algorithm (SHA-1)
// Author:      Giovanni Mittone
// Created:     2009/01/11
// RCS-ID:      $Id$
// Copyright:   (c) 2009 Giovanni Mittone
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef SHA160_H
#define SHA160_H

// Required include
#include "wx/crypstrm/cryptodef.h"
#include "wx/crypstrm/shabase.h"
#include "wx/stream.h"


/**
 * This class represents an SHA 160 bits binary buffer.
 */
typedef class
{
public:
    /**
     * The SHA 160 bits binary buffer as an array of 5 32 bits variables.
     */
    wxUint32 H[5];
    /**
     * Converts the binary hash buffer to a human readable string.
     * @return A wxString representing a human readable hash value.
     */
    wxString ToString();
} wxSHA160Hash;


/**
 * This is the SHA-1 class (it's called wxSHA160 because of the output hash size in bits).
 * \n\n
 * @code
 * // The following code, show how to hash a file

 * wxSHA160Hash hash;
 * wxSHA160 sha160;

 * wxFileInputStream fis(wxT("name.ext"));

 * sha160.Hash(fis, hash);
 * @endcode
 */
class WXDLLIMPEXP_CRYPTOSTREAM wxSHA160 : public wxSHABase512
{
public:
    /**
     * This is the default constructor. It does nothing itself, it only extend the wxSHABase512 constructor.
     * @see wxSHABase512(const wxSHAType &Wsize)
     */
    wxSHA160() : wxSHABase512(wxSHABase512::wxSHA160) {};

    /**
     * Perform the hash calculation over a generic input stream. The input stream length must be determinable.
     * @param input a generic input stream buffer
     * @param hash a wxSHA160Hash valid class; the resulting hash value is stored here.
     * @return True on success, false if an error occurred.
     */
    bool Hash(wxInputStream &input, wxSHA160Hash &hash);
    /**
     * Perform the hash calculation over a wxString. It initialize a string input stream and passes it to the generic hash
     * calculation function.
     * @param input a generic wxString.
     * @param hash a wxSHA160Hash valid class; the resulting hash value is stored here.
     * @return True on success, false if an error occurred.
     * @see Hash(wxInputStream &input, wxSHA160Hash &hash)
     */
    bool Hash(const wxString &input, wxSHA160Hash &hash);
    /**
     * Perform the hash calculation over a binary buffer. It initialize a memory input stream and passes it to the generic
     * hash calculation function.
     * @param input a binary buffer pointer.
     * @param input_size the binary buffer data length.
     * @param hash a wxSHA160Hash valid class; the resulting hash value is stored here.
     * @return True on success, false if an error occurred.
     * @see Hash(wxInputStream &input, wxSHA160Hash &hash)
     */
    bool Hash(const void *input, const size_t &input_size, wxSHA160Hash &hash);

private:
    wxUint32 a, b, c, d, e, T;

    inline void W16_79(const size_t &t);
};

#endif // SHA160_H
