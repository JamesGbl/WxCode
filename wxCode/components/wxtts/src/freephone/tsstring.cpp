/////////////////////////////////////////////////////////////////////////////
// Name:        tsstring.cpp
// Purpose:     tsString class
// Author:      Vadim Zeitlin
// Modified by:
// Created:     29/01/98
// RCS-ID:      $$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "tsstring.h"
#endif

/*
 * About ref counting:
 *  1) all empty strings use g_strEmpty, nRefs = -1 (set in Init())
 *  2) AllocBuffer() sets nRefs to 1, Lock() increments it by one
 *  3) Unlock() decrements nRefs and frees memory if it goes to 0
 */

// ===========================================================================
// headers, declarations, constants
// ===========================================================================

#include "tsstring.h"

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// ---------------------------------------------------------------------------
// static class variables definition
// ---------------------------------------------------------------------------

#ifdef  TS_STD_STRING_COMPATIBILITY
  const size_t tsString::npos = TS_STRING_MAXLEN;
#endif

// ===========================================================================
// static class data, special inlines
// ===========================================================================

// for an empty string, GetStringData() will return this address
static int g_strEmpty[] = { -1,     // ref count (locked)
                             0,     // current length
                             0,     // allocated memory
                             0 };   // string data
// empty string shares memory with g_strEmpty
static tsStringData *g_strNul = (tsStringData*)&g_strEmpty;
// empty C style string: points to 'string data' byte of g_strEmpty
extern const char *g_szNul = (const char *)(&g_strEmpty[3]);

// ===========================================================================
// global functions
// ===========================================================================

#ifdef  TS_STD_STRING_COMPATIBILITY

// MS Visual C++ version 5.0 provides the new STL headers as well as the old
// iostream ones.
//
// ATTN: you can _not_ use both of these in the same program!
#if 0 // def  _MSC_VER
  #include  <iostream>
  #define   NAMESPACE   std::
#else
  #include  <iostream.h>
  #define   NAMESPACE
#endif  //Visual C++

NAMESPACE istream& operator>>(NAMESPACE istream& is, tsString& TSUNUSED(str))
{
#if 0
  int w = is.width(0);
  if ( is.ipfx(0) ) {
    NAMESPACE streambuf *sb = is.rdbuf();
    str.erase();
    while ( true ) {
      int ch = sb->sbumpc ();
      if ( ch == EOF ) {
        is.setstate(NAMESPACE ios::eofbit);
        break;
      }
      else if ( isspace(ch) ) {
        sb->sungetc();
        break;
      }
      
      str += ch;
      if ( --w == 1 )
        break;
    }
  }

  is.isfx();
  if ( str.length() == 0 )
    is.setstate(NAMESPACE ios::failbit);
#endif
  return is;
}

#endif  //std::string compatibility

// ===========================================================================
// tsString class core
// ===========================================================================

// ---------------------------------------------------------------------------
// construction
// ---------------------------------------------------------------------------

// construct an empty string
tsString::tsString()
{
  Init();
}

// copy constructor
tsString::tsString(const tsString& stringSrc)
{
  TSASSERT( stringSrc.GetStringData()->IsValid() );

  if ( stringSrc.IsEmpty() ) {
    // nothing to do for an empty string
    Init();
  }
  else {
    m_pchData = stringSrc.m_pchData;            // share same data
    GetStringData()->Lock();                    // => one more copy
  }
}

// constructs string of <nLength> copies of character <ch>
tsString::tsString(char ch, size_t nLength)
{
  Init();

  if ( nLength > 0 ) {
    AllocBuffer(nLength);

    TSASSERT( sizeof(char) == 1 );  // can't use memset if not

    memset(m_pchData, ch, nLength);
  }
}

// takes nLength elements of psz starting at nPos
void tsString::InitWith(const char *psz, size_t nPos, size_t nLength)
{
  Init();

  TSASSERT( nPos <= Strlen(psz) );

  if ( nLength == TS_STRING_MAXLEN )
    nLength = Strlen(psz + nPos);

  if ( nLength > 0 ) {
    // trailing '\0' is written in AllocBuffer()
    AllocBuffer(nLength);
    memcpy(m_pchData, psz + nPos, nLength*sizeof(char));
  }
}
        
// take first nLength characters of C string psz
// (default value of TS_STRING_MAXLEN means take all the string)
tsString::tsString(const char *psz, size_t nLength)
{
  InitWith(psz, 0, nLength);
}

// the same as previous constructor, but for compilers using unsigned char
tsString::tsString(const unsigned char* psz, size_t nLength)
{
  InitWith((const char *)psz, 0, nLength);
} 
 
#ifdef  TS_STD_STRING_COMPATIBILITY

// ctor from a substring
tsString::tsString(const tsString& s, size_t nPos, size_t nLen)
{
  InitWith(s.c_str(), nPos, nLen == npos ? 0 : nLen);
}

// poor man's iterators are "void *" pointers
tsString::tsString(const void *pStart, const void *pEnd)
{
  InitWith((const char *)pStart, 0, 
           (const char *)pEnd - (const char *)pStart);
}

#endif  //std::string compatibility

// from wide string
tsString::tsString(const wchar_t *pwz)
{
  // first get necessary size
  size_t nLen = wcstombs(NULL, pwz, 0);

  // empty?
  if ( nLen != 0 ) {
    AllocBuffer(nLen);
    wcstombs(m_pchData, pwz, nLen);
  }
  else {
    Init();
  }
}

// ---------------------------------------------------------------------------
// memory allocation
// ---------------------------------------------------------------------------

// allocates memory needed to store a C string of length nLen
void tsString::AllocBuffer(size_t nLen)
{
  TSASSERT( nLen >  0         );    //
  TSASSERT( nLen <= INT_MAX-1 );    // max size (enough room for 1 extra)

  // allocate memory:
  // 1) one extra character for '\0' termination
  // 2) sizeof(tsStringData) for housekeeping info
  tsStringData* pData = (tsStringData*)new char[sizeof(tsStringData) +
                                                (nLen + 1)*sizeof(char)];
  pData->nRefs        = 1;
  pData->data()[nLen] = '\0';
  pData->nDataLength  = nLen;
  pData->nAllocLength = nLen;
  m_pchData           = pData->data();  // data starts after tsStringData
}

// releases the string memory and reinits it
void tsString::Reinit()
{
  GetStringData()->Unlock();
  Init();
}

// wrapper around tsString::Reinit
void tsString::Empty()
{
  if ( GetStringData()->nDataLength != 0 )
    Reinit();

  TSASSERT( GetStringData()->nDataLength == 0 );
  TSASSERT( GetStringData()->nAllocLength == 0 );
}

// must be called before changing this string
void tsString::CopyBeforeWrite()
{
  tsStringData* pData = GetStringData();

  if ( pData->IsShared() ) {
    pData->Unlock();                // memory not freed because shared
    AllocBuffer(pData->nDataLength);
    memcpy(m_pchData, pData->data(), (pData->nDataLength + 1)*sizeof(char));
  }

  TSASSERT( !pData->IsShared() );  // we must be the only owner
}

// must be called before replacing contents of this string
void tsString::AllocBeforeWrite(size_t nLen)
{
  TSASSERT( nLen != 0 );  // doesn't make any sense

  // must not share string and must have enough space
  register tsStringData* pData = GetStringData();
  if ( pData->IsShared() || (nLen > pData->nAllocLength) ) {
    // can't work with old buffer, get new one
    pData->Unlock();
    AllocBuffer(nLen);
  }

  TSASSERT( !GetStringData()->IsShared() );  // we must be the only owner
}

// get the pointer to writable buffer of (at least) nLen bytes
char *tsString::GetWriteBuf(size_t nLen)
{
  AllocBeforeWrite(nLen);
  return m_pchData;
}

// dtor frees memory if no other strings use it
tsString::~tsString()
{
  GetStringData()->Unlock();
}

// ---------------------------------------------------------------------------
// data access
// ---------------------------------------------------------------------------

// all functions are inline in string.h

// ---------------------------------------------------------------------------
// assignment operators
// ---------------------------------------------------------------------------

// helper function: does real copy 
void tsString::AssignCopy(size_t nSrcLen, const char *pszSrcData)
{
  if ( nSrcLen == 0 ) {
    Reinit();
  }
  else {
    AllocBeforeWrite(nSrcLen);
    memcpy(m_pchData, pszSrcData, nSrcLen*sizeof(char));
    GetStringData()->nDataLength = nSrcLen;
    m_pchData[nSrcLen] = '\0';
  }
}

// assigns one string to another
tsString& tsString::operator=(const tsString& stringSrc)
{
  // don't copy string over itself
  if ( m_pchData != stringSrc.m_pchData ) {
    if ( stringSrc.GetStringData()->IsEmpty() ) {
      Reinit();
    }
    else {
      // adjust references
      GetStringData()->Unlock();
      m_pchData = stringSrc.m_pchData;
      GetStringData()->Lock();
    }
  }

  return *this;
}

// assigns a single character
tsString& tsString::operator=(char ch)
{
  AssignCopy(1, &ch);
  return *this;
}

// assigns C string
tsString& tsString::operator=(const char *psz)
{
  AssignCopy(Strlen(psz), psz);
  return *this;
}

// same as 'signed char' variant
tsString& tsString::operator=(const unsigned char* psz)
{
  *this = (const char *)psz;
  return *this;
}

tsString& tsString::operator=(const wchar_t *pwz)
{
  tsString str(pwz);
  *this = str;
  return *this;
}

// ---------------------------------------------------------------------------
// string concatenation
// ---------------------------------------------------------------------------

// concatenate two sources
// NB: assume that 'this' is a new tsString object
void tsString::ConcatCopy(int nSrc1Len, const char *pszSrc1Data,
                        int nSrc2Len, const char *pszSrc2Data)
{
  int nNewLen = nSrc1Len + nSrc2Len;
  if ( nNewLen != 0 )
  {
    AllocBuffer(nNewLen);
    memcpy(m_pchData, pszSrc1Data, nSrc1Len*sizeof(char));
    memcpy(m_pchData + nSrc1Len, pszSrc2Data, nSrc2Len*sizeof(char));
  }
}

// add something to this string
void tsString::ConcatSelf(int nSrcLen, const char *pszSrcData)
{
  // concatenating an empty string is a NOP
  if ( nSrcLen != 0 ) {
    register tsStringData *pData = GetStringData();

    // alloc new buffer if current is too small
    if ( pData->IsShared() || 
         pData->nDataLength + nSrcLen > pData->nAllocLength ) {
      // we have to grow the buffer, use the ConcatCopy routine
      // (which will allocate memory)
      tsStringData* pOldData = GetStringData();
      ConcatCopy(pOldData->nDataLength, m_pchData, nSrcLen, pszSrcData);
      pOldData->Unlock();
    }
    else {
      // fast concatenation when buffer big enough
      memcpy(m_pchData + pData->nDataLength, pszSrcData, nSrcLen*sizeof(char));
      pData->nDataLength += nSrcLen;

      // should be enough space
      TSASSERT( pData->nDataLength <= pData->nAllocLength );

      m_pchData[pData->nDataLength] = '\0';   // put terminating '\0'
    }
  }
}

/*
 * string may be concatenated with other string, C string or a character
 */

void tsString::operator+=(const tsString& string)
{
  ConcatSelf(string.Len(), string);
}

void tsString::operator+=(const char *psz)
{
  ConcatSelf(Strlen(psz), psz);
}

void tsString::operator+=(char ch)
{
  ConcatSelf(1, &ch);
}

/*
 * Same as above but return the result
 */  

tsString& tsString::operator<<(const tsString& string)
{
  ConcatSelf(string.Len(), string);
  return *this;
}

tsString& tsString::operator<<(const char *psz)
{
  ConcatSelf(Strlen(psz), psz);
  return *this;
}

tsString& tsString::operator<<(char ch)
{
  ConcatSelf(1, &ch);
  return *this;
}

/* 
 * concatenation functions come in 5 flavours:
 *  string + string
 *  char   + string      and      string + char
 *  C str  + string      and      string + C str
 */

tsString operator+(const tsString& string1, const tsString& string2)
{
  tsString s;
  s.ConcatCopy(string1.GetStringData()->nDataLength, string1.m_pchData,
               string2.GetStringData()->nDataLength, string2.m_pchData);
  return s;
}

tsString operator+(const tsString& string1, char ch)
{
  tsString s;
  s.ConcatCopy(string1.GetStringData()->nDataLength, string1.m_pchData, 1, &ch);
  return s;
}

tsString operator+(char ch, const tsString& string)
{
  tsString s;
  s.ConcatCopy(1, &ch, string.GetStringData()->nDataLength, string.m_pchData);
  return s;
}

tsString operator+(const tsString& string, const char *psz)
{
  tsString s;
  s.ConcatCopy(string.GetStringData()->nDataLength, string.m_pchData,
               Strlen(psz), psz);
  return s;
}

tsString operator+(const char *psz, const tsString& string)
{
  tsString s;
  s.ConcatCopy(Strlen(psz), psz,
               string.GetStringData()->nDataLength, string.m_pchData);
  return s;
}

// ===========================================================================
// other common string functions
// ===========================================================================

// ---------------------------------------------------------------------------
// simple sub-string extraction
// ---------------------------------------------------------------------------

// helper function: clone the data attached to this string
void tsString::AllocCopy(tsString& dest, int nCopyLen, int nCopyIndex) const
{
  if ( nCopyLen == 0 )
  {
    dest.Init();
  }
  else
  {
    dest.AllocBuffer(nCopyLen);
    memcpy(dest.m_pchData, m_pchData + nCopyIndex, nCopyLen*sizeof(char));
  }
}

// extract string of length nCount starting at nFirst
// default value of nCount is 0 and means "till the end"
tsString tsString::Mid(size_t nFirst, size_t nCount) const
{
  // out-of-bounds requests return sensible things
  if ( nCount == 0 )
    nCount = GetStringData()->nDataLength - nFirst;

  if ( nFirst + nCount > (size_t)GetStringData()->nDataLength )
    nCount = GetStringData()->nDataLength - nFirst;
  if ( nFirst > (size_t)GetStringData()->nDataLength )
    nCount = 0;

  tsString dest;
  AllocCopy(dest, nCount, nFirst);
  return dest;
}

// extract nCount last (rightmost) characters
tsString tsString::Right(size_t nCount) const
{
  if ( nCount > (size_t)GetStringData()->nDataLength )
    nCount = GetStringData()->nDataLength;

  tsString dest;
  AllocCopy(dest, nCount, GetStringData()->nDataLength - nCount);
  return dest;
}

// get all characters after the last occurence of ch
// (returns the whole string if ch not found)
tsString tsString::Right(char ch) const
{
  tsString str;
  int iPos = Find(ch, TRUE);
  if ( iPos == TSNOT_FOUND )
    str = *this;
  else
    str = c_str() + iPos + 1;

  return str;
}

// extract nCount first (leftmost) characters
tsString tsString::Left(size_t nCount) const
{
  if ( nCount > (size_t)GetStringData()->nDataLength )
    nCount = GetStringData()->nDataLength;

  tsString dest;
  AllocCopy(dest, nCount, 0);
  return dest;
}

// get all characters before the first occurence of ch
// (returns the whole string if ch not found)
tsString tsString::Left(char ch) const
{
  tsString str;
  for ( const char *pc = m_pchData; *pc != '\0' && *pc != ch; pc++ )
    str += *pc;

  return str;
}

/// get all characters before the last occurence of ch
/// (returns empty string if ch not found)
tsString tsString::Before(char ch) const
{
  tsString str;
  int iPos = Find(ch, TRUE);
  if ( iPos != TSNOT_FOUND && iPos != 0 )
    str = tsString(c_str(), iPos);

  return str;
}

/// get all characters after the first occurence of ch
/// (returns empty string if ch not found)
tsString tsString::After(char ch) const
{
  tsString str;
  int iPos = Find(ch);
  if ( iPos != TSNOT_FOUND )
    str = c_str() + iPos + 1;

  return str;
}

// replace first (or all) occurences of some substring with another one
uint tsString::Replace(const char *szOld, const char *szNew, bool bReplaceAll)
{
  uint uiCount = 0;   // count of replacements made

  uint uiOldLen = Strlen(szOld);

  tsString strTemp;
  const char *pCurrent = m_pchData;
  const char *pSubstr;           
  while ( *pCurrent != '\0' ) {
    pSubstr = strstr(pCurrent, szOld);
    if ( pSubstr == NULL ) {
      // strTemp is unused if no replacements were made, so avoid the copy
      if ( uiCount == 0 )
        return 0;

      strTemp += pCurrent;    // copy the rest
      break;                  // exit the loop
    }
    else {
      // take chars before match
      strTemp.ConcatSelf(pSubstr - pCurrent, pCurrent);
      strTemp += szNew;
      pCurrent = pSubstr + uiOldLen;  // restart after match

      uiCount++;

      // stop now?
      if ( !bReplaceAll ) {
        strTemp += pCurrent;    // copy the rest
        break;                  // exit the loop
      }
    }
  }

  // only done if there were replacements, otherwise would have returned above
  *this = strTemp;

  return uiCount;
}

bool tsString::IsAscii() const
{
  const char *s = (const char*) *this;
  while(*s){
    if(!isascii(*s)) return(FALSE);
    s++;
  }
  return(TRUE);
}
  
bool tsString::IsWord() const
{
  const char *s = (const char*) *this;
  while(*s){
    if(!isalpha(*s)) return(FALSE);
    s++;
  }
  return(TRUE);
}
  
bool tsString::IsNumber() const
{
  const char *s = (const char*) *this;
  while(*s){
    if(!isdigit(*s)) return(FALSE);
    s++;
  }
  return(TRUE);
}

// kludge: we don't have declaraton of tsStringData here, so we add offsets
//         manually to get to the "length" field of tsStringData structure
bool   tsString::IsEmpty() const  { return Len() == 0;                        }

tsString tsString::Strip(stripType w) const
{
    tsString s = *this;
    if ( w & leading ) s.Trim(FALSE);
    if ( w & trailing ) s.Trim(TRUE);
    return s;
}

/// case-insensitive strcmp() (platform independent)
int Stricmp(const char *psz1, const char *psz2)
{
#if     defined(_MSC_VER)
  return _stricmp(psz1, psz2);
#elif defined(__BORLANDC__)
  return stricmp(psz1, psz2);
#elif   defined(__UNIX__) || defined(__GNUWIN32__)
  return strcasecmp(psz1, psz2);
#else
  // almost all compilers/libraries provide this function (unfortunately under
  // different names), that's why we don't implement our own which will surely
  // be more efficient than this code (uncomment to use):
  /*
    register char c1, c2;
    do {
      c1 = tolower(*psz1++);
      c2 = tolower(*psz2++);
    } while ( c1 && (c1 == c2) );

    return c1 - c2;
  */
  
  #error  "Please define string case-insensitive compare for your OS/compiler"
#endif  // OS/compiler
}

// ---------------------------------------------------------------------------
// case conversion
// ---------------------------------------------------------------------------

tsString& tsString::MakeUpper()
{
  CopyBeforeWrite();

  for ( char *p = m_pchData; *p; p++ )
    *p = (char)toupper(*p);

  return *this;
}

tsString& tsString::MakeLower()
{
  CopyBeforeWrite();
  
  for ( char *p = m_pchData; *p; p++ )
    *p = (char)tolower(*p);

  return *this;
}

// ---------------------------------------------------------------------------
// trimming and padding
// ---------------------------------------------------------------------------

// trims spaces (in the sense of isspace) from left or right side
tsString& tsString::Trim(bool bFromRight)
{
  CopyBeforeWrite();

  if ( bFromRight )
  {
    // find last non-space character
    char *psz = m_pchData + GetStringData()->nDataLength - 1;
    while ( isspace(*psz) && (psz >= m_pchData) )
      psz--;

    // truncate at trailing space start
    *++psz = '\0';
    GetStringData()->nDataLength = psz - m_pchData;
  }
  else
  {
    // find first non-space character
    const char *psz = m_pchData;
    while ( isspace(*psz) )
      psz++;

    // fix up data and length
    int nDataLength = GetStringData()->nDataLength - (psz - m_pchData);
    memmove(m_pchData, psz, (nDataLength + 1)*sizeof(char));
    GetStringData()->nDataLength = nDataLength;
  }

  return *this;
}

// adds nCount characters chPad to the string from either side
tsString& tsString::Pad(size_t nCount, char chPad, bool bFromRight)
{
  tsString s(chPad, nCount);

  if ( bFromRight )
    *this += s;
  else
  {
    s += *this;
    *this = s;
  }

  return *this;
}

// truncate the string
tsString& tsString::Truncate(size_t uiLen)
{
  *(m_pchData + uiLen) = '\0';
  GetStringData()->nDataLength = uiLen;

  return *this;
}

// ---------------------------------------------------------------------------
// finding (return TSNOT_FOUND if not found and index otherwise)
// ---------------------------------------------------------------------------

// find a character
int tsString::Find(char ch, bool bFromEnd) const
{
  const char *psz = bFromEnd ? strrchr(m_pchData, ch) : strchr(m_pchData, ch);

  return (psz == NULL) ? TSNOT_FOUND : psz - m_pchData;
}

// find a sub-string (like strstr)
int tsString::Find(const char *pszSub) const
{
  const char *psz = strstr(m_pchData, pszSub);

  return (psz == NULL) ? TSNOT_FOUND : psz - m_pchData;
}

// ---------------------------------------------------------------------------
// formatted output
// ---------------------------------------------------------------------------
int tsString::Printf(const char *pszFormat, ...)
{
  va_list argptr;
  va_start(argptr, pszFormat);

  int iLen = PrintfV(pszFormat, argptr);

  va_end(argptr);

  return iLen;
}

int tsString::PrintfV(const char* pszFormat, va_list argptr)
{
  static char s_szScratch[1024];

  int iLen = vsprintf(s_szScratch, pszFormat, argptr);
  AllocBeforeWrite(iLen);
  strcpy(m_pchData, s_szScratch);

  return iLen;
}

// ---------------------------------------------------------------------------
// standard C++ library string functions
// ---------------------------------------------------------------------------
#ifdef  TS_STD_STRING_COMPATIBILITY

tsString& tsString::insert(size_t nPos, const tsString& str)
{
  TSASSERT( nPos <= Len() );

  tsString strTmp;
  char *pc = strTmp.GetWriteBuf(Len() + str.Len());
  strncpy(pc, c_str(), nPos);
  strcpy(pc + nPos, str);
  strcpy(pc + nPos + str.Len(), c_str() + nPos);
  *this = strTmp;
    
  return *this; 
}

size_t tsString::find(const tsString& str, size_t nStart) const
{
  TSASSERT( nStart <= Len() );

  const char *p = strstr(c_str() + nStart, str);
  
  return p == NULL ? npos : p - c_str();
}

// VC++ 1.5 can't cope with the default argument in the header.
#if ! (defined(_MSC_VER) && !defined(__WIN32__))
size_t tsString::find(const char* sz, size_t nStart, size_t n) const
{
  return find(tsString(sz, n == npos ? 0 : n), nStart);
}
#endif
        
size_t tsString::find(char ch, size_t nStart) const
{
  TSASSERT( nStart <= Len() );

  const char *p = strchr(c_str() + nStart, ch);
  
  return p == NULL ? npos : p - c_str();
}

size_t tsString::rfind(const tsString& str, size_t nStart) const
{
  TSASSERT( nStart <= Len() );

  // # could be quicker than that
  const char *p = c_str() + (nStart == npos ? Len() : nStart);
  while ( p >= c_str() + str.Len() ) {
    if ( strncmp(p - str.Len(), str, str.Len()) == 0 )
      return p - str.Len() - c_str();
    p--;
  }
  
  return npos;
}
        
// VC++ 1.5 can't cope with the default argument in the header.
#if ! (defined(_MSC_VER) && !defined(__WIN32__))
size_t tsString::rfind(const char* sz, size_t nStart, size_t n) const
{
  return rfind(tsString(sz, n == npos ? 0 : n), nStart);
}

size_t tsString::rfind(char ch, size_t nStart) const
{
  TSASSERT( nStart <= Len() );

  const char *p = strrchr(c_str() + nStart, ch);
  
  return p == NULL ? npos : p - c_str();
}
#endif

tsString tsString::substr(size_t nStart, size_t nLen) const
{
  // npos means 'take all'
  if ( nLen == npos )
    nLen = 0;

  TSASSERT( nStart + nLen <= Len() );

  return tsString(c_str() + nStart, nLen == npos ? 0 : nLen);
}

tsString& tsString::erase(size_t nStart, size_t nLen)
{
  tsString strTmp(c_str(), nStart);
  if ( nLen != npos ) {
    TSASSERT( nStart + nLen <= Len() );

    strTmp.append(c_str() + nStart + nLen);
  }

  *this = strTmp;
  return *this;
}

tsString& tsString::replace(size_t nStart, size_t nLen, const char *sz)
{
  TSASSERT( nStart + nLen <= Strlen(sz) );

  tsString strTmp;
  if ( nStart != 0 )
    strTmp.append(c_str(), nStart);
  strTmp += sz;
  strTmp.append(c_str() + nStart + nLen);
  
  *this = strTmp;
  return *this;
}

tsString& tsString::replace(size_t nStart, size_t nLen, size_t nCount, char ch)
{
  return replace(nStart, nLen, tsString(ch, nCount));
}

tsString& tsString::replace(size_t nStart, size_t nLen, 
                        const tsString& str, size_t nStart2, size_t nLen2)
{
  return replace(nStart, nLen, str.substr(nStart2, nLen2));
}

tsString& tsString::replace(size_t nStart, size_t nLen, 
                        const char* sz, size_t nCount)
{
  return replace(nStart, nLen, tsString(sz, nCount));
}

#endif  //std::string compatibility

// ----------------------------------------------------------------------------
// debug helper
// ----------------------------------------------------------------------------

#ifdef  __DEBUG__

// this function is called when an assert fails
void tsOnAssert(const char *szFile, int nLine, const char *szMsg)
{
  // this variable can be set to true to suppress "assert failure" messages
  static s_bNoAsserts = FALSE;

  char szBuf[LOG_BUFFER_SIZE];
  sprintf(szBuf, _("Assert failed in file %s at line %d"), szFile, nLine);
  if ( szMsg != NULL ) {
    strcat(szBuf, ": ");
    strcat(szBuf, szMsg);
  }
  else {
    strcat(szBuf, ".");
  }

  // send it to the normal log destination
#ifdef _WINDOWS
  OutputDebugString(szBuf);
#else
  printf(szBuf);
#endif
}

#endif  //DEBUG

