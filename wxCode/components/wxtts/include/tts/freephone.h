/////////////////////////////////////////////////////////////////////////////
// Name:        freephone.h
// Purpose:     wxFreePhone class, for driving FreePhone
// Author:      Julian Smart
// Modified by:
// Created:     10/6/98
// Copyright:   Julian Smart
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _FREEPHONE_H_
#define _FREEPHONE_H_

#ifdef _AFXDLL
#  define __FP_MFC__
#else
// Set your framework (here or in the makefile). If all are undefined, we're
// using native Windows or Unix.
// #define __FP_MFC__
// #define __FP_WXWIN__
#endif

#if defined(__WXMSW__) || defined(__WXGTK__) || defined(__WXMOTIF__)
#ifndef __FP_WXWIN__
#define __FP_WXWIN__
#endif
#endif

#ifdef __FP_MFC__
#include <afx.h>
#elif defined(__FP_WXWIN__)
#include <wx/wx.h>
#include <wx/string.h>
#endif

#ifdef __FP_MFC__
#   ifndef CStringRef
#       define CStringRef CString&
#   endif
#   ifndef Bool
#       define Bool bool
#   endif
#elif defined(__FP_WXWIN__)
#   ifndef CString
#       define CString wxString
#       define CStringRef wxString&
#   endif
#   ifndef Bool
#       define Bool bool
#   endif
#else
#   ifndef CString
#       define CString char*
#       define CStringRef char*
#   endif
#   ifndef Bool
#       define Bool bool
#   endif
#   ifndef TRUE
#       define TRUE 1
#       define FALSE 0
#   endif
#endif

#if !defined(__FP_MFC__) && !defined(__FP_WXWIN__)
#define FP_STRING_ASSIGN(a, b) delete[] a; a = copystring(b)
#define FP_STRING_DELETE(a) delete[] a
#define FP_STRING_ISEMPTY(a) (a == NULL)
#else
#define FP_STRING_ASSIGN(a, b) a = b
#define FP_STRING_DELETE(a)
#define FP_STRING_ISEMPTY(a) (a == "")
#endif

class tsFreePhoneImplementation;
class wxFreePhone
{
public:
    wxFreePhone(const CStringRef dictionaryPath = "");
    ~wxFreePhone();

    // Public interface
public:

    // Initialises dictionary etc.
    Bool Init();

    // Cleans up dictionary etc.
    Bool CleanUp();

#if defined(__FP_MFC__) || defined(__FP_WXWIN__)
    Bool TextToPhoneme(const CStringRef text, CStringRef phonemes);
#else
    Bool TextToPhoneme(const CStringRef text, char** phonemes);
#endif

    Bool TextToPhonemeFile(const CStringRef text, const CStringRef phonemeFile);

protected:
    // Implementation
    Bool WriteToFile(const CStringRef filename, const CStringRef text);

#if defined(__FP_MFC__) || defined(__FP_WXWIN__)
    Bool ReadFromFile(const CStringRef filename, CStringRef text);
#else
    Bool ReadFromFile(const CStringRef filename, char** text);
#endif

    CString GetTempFilename() const ;
    Bool FileExists (const char *filename) const ;
    static char *copystring (const char *s);

protected:
    tsFreePhoneImplementation* m_implementation;
    CString             m_dictionaryPath; // E.g. "thing/english"
    CString             m_dictionaryFile; // .dct
    CString             m_keywordFile;    // .key
    CString             m_indexFile;      // .idx
};

#endif

