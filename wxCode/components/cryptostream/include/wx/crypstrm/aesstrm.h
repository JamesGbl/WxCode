/////////////////////////////////////////////////////////////////////////////
// Name:        aesstrm.h
// Purpose:     A simple AES cipher/decipher algorithm
// Author:      Giovanni Mittone
// Created:     2009/01/11
// RCS-ID:      $Id$
// Copyright:   (c) 2009 Giovanni Mittone
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef AES_H
#define AES_H

// Required include
#include "cryptodef.h"
#include "wx/stream.h"
#include "wx/longlong.h"

#if wxHAS_INT64

#define wxAES_STATE_WORD    4
#define wxAES_NB            4
#define wxAES_STATE_SIZE    (wxAES_STATE_WORD * wxAES_NB)

/**
 * This is the base class for the AES algorithm. It's used for manage the encryption or decryption key.
 * To use AES classes, your compiler must support 64 bits numbers or, if it doesn't, the wxLongLong emulation.
 */
class WXDLLIMPEXP_CRYPTOSTREAM wxAESBase
{
public:
    /** Key type enumeration.
     * This represents the 4 supported key type.
     */
    typedef enum wxAESKeyType
    {
        wxAESUnknown = 0,   /**< null key; this means no encryption o decryption will be done. */
        wxAES128 = 16,      /**< 128 bits key length. */
        wxAES192 = 24,      /**< 192 bits key length. */
        wxAES256 = 32       /**< 256 bits key length. */
    };

    /**
     * Constructor of the class. It sets the encryption key to none; tring to encrypt data with this setting will result in a
     * non-encrypted output buffer.
     */
    wxAESBase();
    /**
     * Destructor of the class. Actually, it does nothing.
     */
    ~wxAESBase();

    /**
     * The copy constructor: acts exactly in the same way as SetKeyFrom.
     * @see SetKeyFrom(const wxAESBase &base)
     */
    wxAESBase(const wxAESBase &base){ SetKeyFrom(base); };

    /**
     * Set the encryption or decryption key using a user provided binary buffer. If a key is already been set, calling this
     * function will overwrite the old one.
     * @param type enumeration identifing the key type (128, 1292, 256 bits).
     * @param key a binary buffer identifing the encryption or decryption key; it size must be big enough to contain the
     * encryption key.
     * @see wxAESKeyType
     * @see SetKey(wxAESKeyType type, const wxInputStream &key)
     */
    void SetKey(wxAESKeyType type, void *key);
    /**
     * Set the encryption or decryption key using a user provided input stream. If a key is already been set, calling this
     * function will overwrite the old one. The input stream content is hashed through the wxSHA256::Hash function and the result
     * is passed to the binary SetKey function. Passing an empty input buffer, will not result in an error.
     * @param type enumeration identifing the key type (128, 1292, 256 bits).
     * @param key an input stream identifing the encryption or decryption key.
     * @see wxAESKeyType
     */
    void SetKey(wxAESKeyType type, const wxInputStream &key);
    /**
     * Set the encryption or decryption key using a user provided string. If a key is already been set, calling this
     * function will overwrite the old one. The string is passed to the input stream SetKey function.
     * @param type enumeration identifing the key type (128, 1292, 256 bits).
     * @param key a string identifing the encryption or decryption key.
     * @see wxAESKeyType
     * @see SetKey(wxAESKeyType type, const wxInputStream &key)
     */
    void SetKey(wxAESKeyType type, const wxString &key);
    /**
     * Set the encryption or decryption key from an other wxAESBase class.
     * @param base a wxAESBase valid class.
     */
    void SetKeyFrom(const wxAESBase &base);

    /**
     * Test for a valid key.
     * @return True if a valid key is been set, false otherwise.
     */
    bool IsKeySet(){ return (m_type != wxAESUnknown); };
    /**
     * Returns the key type set by the user with SetKey or SetKeyFrom.
     * @return A wxAESKeyType enumeration value.
     * @see wxAESKeyType
     */
    wxAESKeyType GetKeyType(){ return m_type; };

protected:
    wxAESKeyType m_type;
    wxUint8 m_state[wxAES_STATE_SIZE], m_tstate[wxAES_STATE_SIZE], m_keyscheduler[240];
    size_t m_keylength, m_schedulersize, m_rounds;
    wxLongLong m_ptsize, m_ptcount;

private:
    // Key expansion function
    inline void KeyExpansion();
    inline void AddRoundWord(wxUint8 *&curscheduler, wxUint8 *&last_round, wxUint8 *&last_key);
    inline void RotSubRoundAddWord(wxUint8 *&curscheduler, wxUint8 *&last_key, wxUint8 *&last_round, const size_t &cont);
    inline void SubAddWord(wxUint8 *&curscheduler, wxUint8 *&last_key, wxUint8 *&last_round);
};

/**
 * This is the encryption class for the AES algorithm. It's used for encrypt stream buffers.
 * To use AES classes, your compiler must support 64 bits numbers or, if it doesn't, the wxLongLong emulation.
 * \n\n
 * @code
 * // The following code, show how to encrypt a file and send through a valid socket
 * // connected in loopback using port 12345 (client part; server in wxAESDecipher).

 * wxAESCipher cipher;
 * cipher.SetKey(wxAESBase::wxAES256, wxT("my super secret password!"));

 * wxFileInputStream fis(wxT("name1.ext"));

 * wxIPV4address ip;
 * ip.Hostname(wxT("127.0.0.1"));
 * ip.Service(12345);
 * wxSocketClient sc;
 * sc.Connect(ip);
 * wxSocketOutputStream sos(sc);

 * cipher.Cipher(fis, sos);
 * @endcode
 */
class WXDLLIMPEXP_CRYPTOSTREAM wxAESCipher : public wxAESBase
{
public:
    /**
     * Use this function to encrypt an input buffer to an output buffer. If with_size is true (default), the input stream
     * length is included in the encrypted buffer (the input stream buffer length must be determinable or it will result in
     * an error).
     * @param plaintext innput stream buffer.
     * @param ciphertext output stream buffer.
     * @param with_size determines if the input stream length sould be included into the encrypted buffer.
     * @return True if the encryption ends successfully, false if an error occurred.
     */
    bool Cipher(wxInputStream &plaintext, wxOutputStream &ciphertext, bool with_size = true);

private:
    bool m_psize;
    size_t m_pround;

    bool Add(wxInputStream &plaintext);
    inline void SubShift();
    inline void MixAdd();
    bool SubShiftAdd(wxOutputStream &ciphertext);
};

/**
 * This is the decryption class for the AES algorithm. It's used for decrypt stream buffers.
 * To use AES classes, your compiler must support 64 bits numbers or, if it doesn't, the wxLongLong emulation.
 * \n\n
 * @code
 * // The following code, shows how to decrypt datas from a valid socket
 * // connected in loopback using port 12345 to a file (server part; client in wxAESCipher).

 * wxAESDecipher decipher;
 * decipher.SetKey(wxAESBase::wxAES256, wxT("my super secret password!"));

 * wxFileOutputStream fos(wxT("name2.ext"));

 * wxIPV4address ip;
 * ip.Hostname(wxT("127.0.0.1"));
 * ip.Service(12345);
 * wxSocketServer ss(ip);
 * wxSocketBase _ss = ss.Accept();
 * wxSocketInputStream sis(_ss);

 * decipher.Decipher(sis, fos);
 * @endcode
 */
class WXDLLIMPEXP_CRYPTOSTREAM wxAESDecipher : public wxAESBase
{
public:
    /**
     * Use this function to decrypt an input buffer to an output buffer. If cipher_length is -1 (default), the input stream
     * length is retrieved from the encrypted buffer (only if with_size param of Cipher funztion was true);
     * otherwise cipher_length identifies the length of decrypted datas.
     * @param ciphertext innput stream buffer.
     * @param plaintext output stream buffer.
     * @param cipher_length identify the output stream length.
     * @return True if the decryption ends successfully, false if an error occurred.
     */
    bool Decipher(wxInputStream &ciphertext, wxOutputStream &plaintext, const wxLongLong &cipherlength = -1);

private:
    bool m_psize;
    size_t m_pround;

    bool Add(wxInputStream &ciphertext);
    inline void InvSubShiftAdd();
    inline void InvMix();
    bool InvSubShiftAdd(wxOutputStream &plaintext);
};


#endif // wxHAS_INT64
#endif // AES_H
