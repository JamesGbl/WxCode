/////////////////////////////////////////////////////////////////////////////
// Name:        tsstring.h
// Purpose:     tsString class
// Author:      Vadim Zeitlin
// Modified by:
// Created:     29/01/98
// RCS-ID:      $Id: tsstring.h,v 1.1.1.1 2003-12-28 21:08:24 wyo Exp $
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _TS_TSSTRING_H_
#define _TS_TSSTRING_H_

#ifdef __GNUG__
#pragma interface "tsstring.h"
#endif

/* Dependencies (should be included before this header):
 *         string.h
 *         stdio.h
 *         stdarg.h
 *         limits.h
 */
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>

#include "tsdefs.h"
#include "tsdebug.h"

#define TSDLLEXPORT
#define TSNOT_FOUND (-1)

/// the type for various indexes (string, arrays, ...)
typedef unsigned int    uint;

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif


/** @name tsString library
    @memo Efficient tsString class [more or less] compatible with MFC CString,
          wxWindows tsString and std::string and some handy functions
          missing from string.h.
  */
//@{

// ---------------------------------------------------------------------------
// macros
// ---------------------------------------------------------------------------

/** @name Macros 
    @memo You can switch off tsString/std::string compatibility if desired
 */
/// compile the std::string compatibility functions
#define   TS_STD_STRING_COMPATIBILITY

/// maximum possible length for a string means "take all string" everywhere
//  (as sizeof(StringData) is unknown here we substract 100)
#define   TS_STRING_MAXLEN     (UINT_MAX - 100)

// 'naughty' cast
#define   TS_STRINGCAST (char *)(const char *)

// NB: works only inside tsString class
#define   TS_ASSERT_VALID_INDEX(i) TSASSERT( (unsigned)(i) < Len() )

// ---------------------------------------------------------------------------
/** @name Global functions complementing standard C string library 
    @memo replacements for strlen() and portable strcasecmp()
 */
// ---------------------------------------------------------------------------

/// checks whether the passed in pointer is NULL and if the string is empty
inline bool TSDLLEXPORT IsEmpty(const char *p) { return !p || !*p; }

/// safe version of strlen() (returns 0 if passed NULL pointer)
inline size_t  TSDLLEXPORT Strlen(const char *psz)
  { return psz ? strlen(psz) : 0; }

/// portable strcasecmp/_stricmp
int TSDLLEXPORT Stricmp(const char *, const char *);

// ---------------------------------------------------------------------------
// string data prepended with some housekeeping info (used by String class),
// is never used directly (but had to be put here to allow inlining)
// ---------------------------------------------------------------------------
struct TSDLLEXPORT tsStringData
{
  int     nRefs;        // reference count
  size_t  nDataLength,  // actual string length
          nAllocLength; // allocated memory size

  // mimics declaration 'char data[nAllocLength]'
  char* data() const { return (char*)(this + 1); }  

  // empty string has a special ref count so it's never deleted
  bool  IsEmpty()   const { return nRefs == -1; }
  bool  IsShared()  const { return nRefs > 1;   }
  bool  IsValid()   const { return nRefs != 0;  }

  // lock/unlock
  void  Lock()   { if ( !IsEmpty() ) nRefs++;                             }
  void  Unlock() { if ( !IsEmpty() && --nRefs == 0) delete (char *)this;  }
};

extern const char *g_szNul; // global pointer to empty string

// ---------------------------------------------------------------------------
/**
    This is (yet another one) String class for C++ programmers. It doesn't use
    any of "advanced" C++ features (i.e. templates, exceptions, namespaces...)
    thus you should be able to compile it with practicaly any C++ compiler.
    This class uses copy-on-write technique, i.e. identical strings share the
    same memory as long as neither of them is changed.

    This class aims to be as compatible as possible with the new standard
    std::string class, but adds some additional functions and should be
    at least as efficient than the standard implementation.
    
    Performance note: it's more efficient to write functions which take 
    "const String&" arguments than "const char *" if you assign the argument 
    to another string.

    It was compiled and tested under Win32, Linux (libc 5 & 6), Solaris 5.5.

    To do:
      - ressource support (string tables in ressources)
      - more wide character (UNICODE) support
      - regular expressions support
    
@memo     A non-template portable tsString class implementing copy-on-write.
@author   VZ
@version  1.3
*/
// ---------------------------------------------------------------------------
  class TSDLLEXPORT tsString
  {

public:
  /** @name constructors & dtor */
  //@{
    /// ctor for an empty string
  tsString();
    /// copy ctor
  tsString(const tsString& stringSrc);        
    /// string containing nRepeat copies of ch
  tsString(char ch, size_t nRepeat = 1);       
    /// ctor takes first nLength characters from C string
  tsString(const char *psz, size_t nLength = TS_STRING_MAXLEN);
    /// from C string (for compilers using unsigned char)
  tsString(const unsigned char* psz, size_t nLength = TS_STRING_MAXLEN);
    /// from wide (UNICODE) string
  tsString(const wchar_t *pwz);
    /// dtor is not virtual, this class must not be inherited from!
 ~tsString();
  //@}

  /** @name generic attributes & operations */
  //@{
    /// as standard strlen()
  size_t Len() const { return GetStringData()->nDataLength; }
    /// string contains any characters?
  bool IsEmpty() const;
    /// reinitialize string (and free data!)
  void Empty();
    /// Is an ascii value
  bool IsAscii() const;
    /// Is a number
  bool IsNumber() const;
    /// Is a word
  bool IsWord() const;
  //@}

  /** @name data access (all indexes are 0 based) */
  //@{
    /// read access
    char  GetChar(size_t n) const
  	  { TS_ASSERT_VALID_INDEX( n );  return m_pchData[n]; }
    /// read/write access
    char& GetWritableChar(size_t n)
  	  { TS_ASSERT_VALID_INDEX( n ); CopyBeforeWrite(); return m_pchData[n]; }
    /// write access
    void  SetChar(size_t n, char ch)
      { TS_ASSERT_VALID_INDEX( n ); CopyBeforeWrite(); m_pchData[n] = ch; }

    /// get last character
    char  Last() const
      { TSASSERT( !IsEmpty() ); return m_pchData[Len() - 1]; }
    /// get writable last character
    char& Last() 
      { TSASSERT( !IsEmpty() ); CopyBeforeWrite(); return m_pchData[Len()-1]; }

    /// operator version of GetChar
    char  operator[](size_t n) const
      { TS_ASSERT_VALID_INDEX( n ); return m_pchData[n]; }
    /// operator version of GetChar
    char  operator[](int n) const
      { TS_ASSERT_VALID_INDEX( n ); return m_pchData[n]; }
    /// operator version of GetWritableChar
    char& operator[](size_t n)
      { TS_ASSERT_VALID_INDEX( n ); CopyBeforeWrite(); return m_pchData[n]; }

    /// implicit conversion to C string
    operator const char*() const { return m_pchData; } 
    /// explicit conversion to C string (use this with printf()!)
    const char* c_str()   const { return m_pchData; }
    ///
    const char* GetData() const { return m_pchData; }
  //@}

  /** @name overloaded assignment */
  //@{
    ///
  tsString& operator=(const tsString& stringSrc);
    ///
  tsString& operator=(char ch);
    ///
  tsString& operator=(const char *psz);
    ///
  tsString& operator=(const unsigned char* psz);
    ///
  tsString& operator=(const wchar_t *pwz);
  //@}
  
  /** @name string concatenation */
  //@{
    /** @name in place concatenation */
    //@{
      /// string += string
  void operator+=(const tsString& string);
      /// string += C string
  void operator+=(const char *psz);
      /// string += char
  void operator+=(char ch);
    //@}
    /** @name concatenate and return the result
        left to right associativity of << allows to write 
        things like "str << str1 << str2 << ..."          */
    //@{
      /// as +=
  tsString& operator<<(const tsString& string);
      /// as +=
  tsString& operator<<(char ch);
      /// as +=
  tsString& operator<<(const char *psz);
    //@}
    
    /** @name return resulting string */
    //@{
      ///
  friend tsString operator+(const tsString& string1,  const tsString& string2);
      ///
  friend tsString operator+(const tsString& string, char ch);
      ///
  friend tsString operator+(char ch, const tsString& string);
      ///
  friend tsString operator+(const tsString& string, const char *psz);
      ///
  friend tsString operator+(const char *psz, const tsString& string);
    //@}
  //@}
  
  /** @name string comparison */
  //@{
    /** 
    case-sensitive comparaison
    @return 0 if equal, +1 if greater or -1 if less
    @see CmpNoCase, IsSameAs
    */
  int  Cmp(const char *psz) const { return strcmp(c_str(), psz); }
    /**
    case-insensitive comparaison, return code as for tsString::Cmp()
    @see: Cmp, IsSameAs
    */
  int  CmpNoCase(const char *psz) const { return Stricmp(c_str(), psz); }
    /**
    test for string equality, case-sensitive (default) or not
    @param   bCase is TRUE by default (case matters)
    @return  TRUE if strings are equal, FALSE otherwise
    @see     Cmp, CmpNoCase
    */
  bool IsSameAs(const char *psz, bool bCase = TRUE) const 
    { return !(bCase ? Cmp(psz) : CmpNoCase(psz)); }
  //@}
  
  /** @name other standard string operations */
  //@{
    /** @name simple sub-string extraction
     */
    //@{
      /** 
      return substring starting at nFirst of length 
      nCount (or till the end if nCount = default value)
      */
  tsString Mid(size_t nFirst, size_t nCount = TS_STRING_MAXLEN) const;  
      /// get first nCount characters
  tsString Left(size_t nCount) const;
      /// get all characters before the first occurence of ch
      /// (returns the whole string if ch not found)
  tsString Left(char ch) const;
      /// get all characters before the last occurence of ch
      /// (returns empty string if ch not found)
  tsString Before(char ch) const;
      /// get all characters after the first occurence of ch
      /// (returns empty string if ch not found)
  tsString After(char ch) const;
      /// get last nCount characters
  tsString Right(size_t nCount) const;
      /// get all characters after the last occurence of ch
      /// (returns the whole string if ch not found)
  tsString Right(char ch) const;
    //@}
    
    /** @name case conversion */
    //@{ 
      ///
  tsString& MakeUpper();
      ///
  tsString& MakeLower();
    //@}

    /** @name trimming/padding whitespace (either side) and truncating */
    //@{
      /// remove spaces from left or from right (default) side
  tsString& Trim(bool bFromRight = TRUE);
      /// add nCount copies chPad in the beginning or at the end (default)
  tsString& Pad(size_t nCount, char chPad = ' ', bool bFromRight = TRUE);
      /// truncate string to given length
  tsString& Truncate(size_t uiLen);
    //@}
    
    /** @name searching and replacing */
    //@{
      /// searching (return starting index, or -1 if not found)
  int Find(char ch, bool bFromEnd = FALSE) const;   // like strchr/strrchr
      /// searching (return starting index, or -1 if not found)
  int Find(const char *pszSub) const;               // like strstr
      /**
      replace first (or all) occurences of substring with another one
      @param  bReplaceAll: global replace (default) or only the first occurence
      @return the number of replacements made
      */
  uint Replace(const char *szOld, const char *szNew, bool bReplaceAll = TRUE);
    //@}
  //@}

  /** @name formated input/output */
  //@{
    /// as sprintf(), returns the number of characters written or < 0 on error
  int Printf(const char *pszFormat, ...);
    /// as vprintf(), returns the number of characters written or < 0 on error
  int PrintfV(const char* pszFormat, va_list argptr);
  //@}
  
  // get writable buffer of at least nLen characters
  char *GetWriteBuf(size_t nLen);

  /** @name wxWindows compatibility functions */
  //@{
    /// values for second parameter of CompareTo function
  enum caseCompare {exact, ignoreCase};
    /// values for first parameter of Strip function
  enum stripType {leading = 0x1, trailing = 0x2, both = 0x3};
    /// same as Printf()
  inline int sprintf(const char *pszFormat, ...)
  {
    va_list argptr;
    va_start(argptr, pszFormat);
    int iLen = PrintfV(pszFormat, argptr);
    va_end(argptr);
    return iLen;
  }

    /// same as Cmp
  inline int CompareTo(const char* psz, caseCompare cmp = exact) const
  { return cmp == exact ? Cmp(psz) : CmpNoCase(psz); }

    /// same as Mid (substring extraction)
  inline tsString  operator()(size_t start, size_t len) const { return Mid(start, len); }

    /// same as += or <<
  inline tsString& Append(const char* psz) { return *this << psz; }
  inline tsString& Append(char ch, int count = 1) { tsString str(ch, count); (*this) += str; return *this; }

    ///
  tsString& Prepend(const tsString& str) { *this = str + *this; return *this; }
    /// same as Len
  size_t Length() const { return Len(); }
    /// same as MakeLower
  void LowerCase() { MakeLower(); }
    /// same as MakeUpper
  void UpperCase() { MakeUpper(); }
    /// same as Trim except that it doesn't change this string
  tsString Strip(stripType w = trailing) const;

    /// same as Find (more general variants not yet supported)
  size_t Index(const char* psz) const { return Find(psz); }
  size_t Index(char ch)         const { return Find(ch);  }
    /// same as Truncate
  tsString& Remove(size_t pos) { return Truncate(pos); }
  tsString& RemoveLast() { return Truncate(Len() - 1); }

  // Robert Roebling
  
  tsString& Remove(size_t nStart, size_t nLen) { return erase( nStart, nLen ); }

  size_t First( const char ch ) const { return find(ch); }
  size_t First( const char* psz ) const { return find(psz); }
  size_t First( const tsString &str ) const { return find(str); }

  size_t Last( const char ch ) const { return rfind(ch,0); }
  size_t Last( const char* psz ) const { return rfind(psz,0); }
  size_t Last( const tsString &str ) const { return rfind(str,0); }

    /// same as IsEmpty
  bool IsNull() const { return IsEmpty(); }
  //@}

#ifdef  TS_STD_STRING_COMPATIBILITY
  /** @name std::string compatibility functions */
  
  /// an 'invalid' value for string index
  static const size_t npos;
        
  //@{
    /** @name constructors */
    //@{
      /// take nLen chars starting at nPos
      tsString(const tsString& s, size_t nPos, size_t nLen = npos);
      /// take all characters from pStart to pEnd
      tsString(const void *pStart, const void *pEnd);
    //@}
    /** @name lib.string.capacity */
    //@{
      /// return the length of the string
      size_t size() const { return Len(); }
      /// return the length of the string
      size_t length() const { return Len(); }
      /// return the maximum size of the string
      size_t max_size() const { return TS_STRING_MAXLEN; } 
      /// resize the string, filling the space with c if c != 0
      void resize(size_t nSize, char ch = '\0');
      /// delete the contents of the string
      void clear() { Empty(); }
      /// returns true if the string is empty
      bool empty() const { return IsEmpty(); }
    //@}
    /** @name lib.string.access */
    //@{
      /// return the character at position n
      char at(size_t n) const { return GetChar(n); }
      /// returns the writable character at position n
      char& at(size_t n) { return GetWritableChar(n); }
    //@}
    /** @name lib.string.modifiers */
    //@{
      /** @name append something to the end of this one */
      //@{
        /// append a string
        tsString& append(const tsString& str) 
          { *this += str; return *this; }
        /// append elements str[pos], ..., str[pos+n]
        tsString& append(const tsString& str, size_t pos, size_t n) 
          { ConcatSelf(n, str.c_str() + pos); return *this; }
        /// append first n (or all if n == npos) characters of sz
        tsString& append(const char *sz, size_t n = npos) 
          { ConcatSelf(n == npos ? Strlen(sz) : n, sz); return *this; }

        /// append n copies of ch
        tsString& append(size_t n, char ch) { return Pad(n, ch); }
      //@}
        
      /** @name replaces the contents of this string with another one */
      //@{
        /// same as `this_string = str'
        tsString& assign(const tsString& str) { return (*this) = str; }
        /// same as ` = str[pos..pos + n]
        tsString& assign(const tsString& str, size_t pos, size_t n) 
          { return *this = tsString((const char *)str + pos, n); }
        /// same as `= first n (or all if n == npos) characters of sz'
        tsString& assign(const char *sz, size_t n = npos) 
          { return *this = tsString(sz, n); }
        /// same as `= n copies of ch'
        tsString& assign(size_t n, char ch) 
          { return *this = tsString(ch, n); }

      //@}
        
      /** @name inserts something at position nPos into this one */  
      //@{
        /// insert another string
        tsString& insert(size_t nPos, const tsString& str);
        /// insert n chars of str starting at nStart (in str)
        tsString& insert(size_t nPos, const tsString& str, size_t nStart, size_t n)
	  	    { return insert(nPos, tsString((const char *)str + nStart, n)); }

        /// insert first n (or all if n == npos) characters of sz
        tsString& insert(size_t nPos, const char *sz, size_t n = npos)
          { return insert(nPos, tsString(sz, n)); }
        /// insert n copies of ch
        tsString& insert(size_t nPos, size_t n, char ch) 
          { return insert(nPos, tsString(ch, n)); }

      //@}
      
      /** @name deletes a part of the string */
      //@{
        /// delete characters from nStart to nStart + nLen
        tsString& erase(size_t nStart = 0, size_t nLen = npos);
      //@}
      
      /** @name replaces a substring of this string with another one */
      //@{
         /// replaces the substring of length nLen starting at nStart
         tsString& replace(size_t nStart, size_t nLen, const char* sz);
         /// replaces the substring with nCount copies of ch
         tsString& replace(size_t nStart, size_t nLen, size_t nCount, char ch);
         /// replaces a substring with another substring
         tsString& replace(size_t nStart, size_t nLen, 
                         const tsString& str, size_t nStart2, size_t nLen2);
         /// replaces the substring with first nCount chars of sz
         tsString& replace(size_t nStart, size_t nLen, 
                         const char* sz, size_t nCount);
      //@}
    //@}
         
    /// swap two strings
    void swap(tsString& str);

    /** @name string operations */
    //@{
      /** All find() functions take the nStart argument which specifies
          the position to start the search on, the default value is 0.
          
          All functions return npos if there were no match.
          
          @name string search 
      */
      //@{
        /**
            @name find a match for the string/character in this string 
        */
        //@{
          /// find a substring
          size_t find(const tsString& str, size_t nStart = 0) const;

          // VC++ 1.5 can't cope with this syntax.
#if ! (defined(_MSC_VER) && !defined(__WIN32__))
          /// find first n characters of sz
          size_t find(const char* sz, size_t nStart = 0, size_t n = npos) const;
#endif
          /// find the first occurence of character ch after nStart
          size_t find(char ch, size_t nStart = 0) const;

		  // wxWin compatibility
		  inline bool Contains(const tsString& str) { return (Find(str) != -1); }

        //@}
        
        /** 
          @name rfind() family is exactly like find() but works right to left
        */
        //@{
        /// as find, but from the end
        size_t rfind(const tsString& str, size_t nStart = npos) const;
        /// as find, but from the end
        // VC++ 1.5 can't cope with this syntax.
#if ! (defined(_MSC_VER) && !defined(__WIN32__))
        size_t rfind(const char* sz, size_t nStart = npos, 
                     size_t n = npos) const;
        /// as find, but from the end
        size_t rfind(char ch, size_t nStart = npos) const;
#endif
        //@}
        
        /**
          @name find first/last occurence of any character in the set
        */
        //@{
          ///
          size_t find_first_of(const tsString& str, size_t nStart = 0) const;
          ///
          size_t find_first_of(const char* sz, size_t nStart = 0) const;
          /// same as find(char, size_t)
          size_t find_first_of(char c, size_t nStart = 0) const;
          
          ///
          size_t find_last_of (const tsString& str, size_t nStart = npos) const;
          ///
          size_t find_last_of (const char* s, size_t nStart = npos) const;
          /// same as rfind(char, size_t)
          size_t find_last_of (char c, size_t nStart = npos) const;
        //@}
        
        /**
          @name find first/last occurence of any character not in the set
        */
        //@{
          ///
          size_t find_first_not_of(const tsString& str, size_t nStart = 0) const;
          ///
          size_t find_first_not_of(const char* s, size_t nStart = 0) const;
          ///
          size_t find_first_not_of(char ch, size_t nStart = 0) const;
          
          ///
          size_t find_last_not_of(const tsString& str, size_t nStart=npos) const;
          ///
          size_t find_last_not_of(const char* s, size_t nStart = npos) const;
          ///
          size_t find_last_not_of(char ch, size_t nStart = npos) const;
        //@}
      //@}
      
      /** 
        All compare functions return -1, 0 or 1 if the [sub]string 
        is less, equal or greater than the compare() argument.
        
        @name comparison
      */
      //@{
        /// just like strcmp()
        int compare(const tsString& str) const { return Cmp(str); }
        /// comparaison with a substring
        int compare(size_t nStart, size_t nLen, const tsString& str) const;
        /// comparaison of 2 substrings
        int compare(size_t nStart, size_t nLen,
                    const tsString& str, size_t nStart2, size_t nLen2) const;
        /// just like strcmp()
        int compare(const char* sz) const { return Cmp(sz); }
        /// substring comparaison with first nCount characters of sz
        int compare(size_t nStart, size_t nLen,
                    const char* sz, size_t nCount = npos) const;
      //@}
    tsString substr(size_t nStart = 0, size_t nLen = npos) const;
  //@}
#endif
    
protected:
  // points to data preceded by tsStringData structure with ref count info
  char *m_pchData;

  // accessor to string data
  tsStringData* GetStringData() const { return (tsStringData*)m_pchData - 1; }

  // string (re)initialization functions
    // initializes the string to the empty value (must be called only from
    // ctors, use Reinit() otherwise)
  void Init() { m_pchData = (char *)g_szNul; }
    // initializaes the string with (a part of) C-string
  void InitWith(const char *psz, size_t nPos = 0, size_t nLen = TS_STRING_MAXLEN);
    // as Init, but also frees old data
  inline void Reinit(); 

  // memory allocation
    // allocates memory for string of lenght nLen
  void AllocBuffer(size_t nLen);
    // copies data to another string
  void AllocCopy(tsString&, int, int) const;
    // effectively copies data to string
  void AssignCopy(size_t, const char *);
  
  // append a (sub)string
  void ConcatCopy(int nLen1, const char *src1, int nLen2, const char *src2);
  void ConcatSelf(int nLen, const char *src);

  // functions called before writing to the string: they copy it if there 
  // other references (should be the only owner when writing)
  void CopyBeforeWrite();
  void AllocBeforeWrite(size_t);
};

// ---------------------------------------------------------------------------
/** @name tsString comparaison functions 
    @memo Comparaisons are case sensitive
 */
// ---------------------------------------------------------------------------
//@{
inline bool operator==(const tsString& s1, const tsString& s2) { return s1.Cmp(s2) == 0; }
///
inline bool operator==(const tsString& s1, const char  * s2) { return s1.Cmp(s2) == 0; }
///
inline bool operator==(const char  * s1, const tsString& s2) { return s2.Cmp(s1) == 0; }
///
inline bool operator!=(const tsString& s1, const tsString& s2) { return s1.Cmp(s2) != 0; }
///
inline bool operator!=(const tsString& s1, const char  * s2) { return s1.Cmp(s2) != 0; }
///
inline bool operator!=(const char  * s1, const tsString& s2) { return s2.Cmp(s1) != 0; }
///
inline bool operator< (const tsString& s1, const tsString& s2) { return s1.Cmp(s2) <  0; }
///
inline bool operator< (const tsString& s1, const char  * s2) { return s1.Cmp(s2) <  0; }
///
inline bool operator< (const char  * s1, const tsString& s2) { return s2.Cmp(s1) >  0; }
///
inline bool operator> (const tsString& s1, const tsString& s2) { return s1.Cmp(s2) >  0; }
///
inline bool operator> (const tsString& s1, const char  * s2) { return s1.Cmp(s2) >  0; }
///
inline bool operator> (const char  * s1, const tsString& s2) { return s2.Cmp(s1) <  0; }
///
inline bool operator<=(const tsString& s1, const tsString& s2) { return s1.Cmp(s2) <= 0; }
///
inline bool operator<=(const tsString& s1, const char  * s2) { return s1.Cmp(s2) <= 0; }
///
inline bool operator<=(const char  * s1, const tsString& s2) { return s2.Cmp(s1) >= 0; }
///
inline bool operator>=(const tsString& s1, const tsString& s2) { return s1.Cmp(s2) >= 0; }
///
inline bool operator>=(const tsString& s1, const char  * s2) { return s1.Cmp(s2) >= 0; }
///
inline bool operator>=(const char  * s1, const tsString& s2) { return s2.Cmp(s1) <= 0; }
//@}
    
// ---------------------------------------------------------------------------
/** @name Global functions complementing standard C string library 
    @memo replacements for strlen() and portable strcasecmp()
 */
// ---------------------------------------------------------------------------

#ifdef  TS_STD_STRING_COMPATIBILITY

// fwd decl
class TSDLLEXPORT istream;

istream& TSDLLEXPORT operator>>(istream& is, tsString& str);

#endif  //std::string compatibility

#endif  // _TSSTRING_H_

//@}
