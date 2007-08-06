//
// Copyright (C) 2007 Arne Steinarson <arst at users dot sourceforge dot net>
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any
// damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute
// it freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must
//    not claim that you wrote the original software. If you use this
//    software in a product, an acknowledgment in the product
//    documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must
//    not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source
//    distribution.
//


#ifndef WXSTRCONV_H
#define WXSTRCONV_H

#include "wx/strconv.h"

// String conversions, attempting to make it sane...
//
// Goals:
// 1 - thread safe (use only stack or temp alloc wxCharBuffer)
// 2 - direct syntax: return a read-only pointer, no need for further casts
// 3 - returned value is alive only for the expression
//
// AC - Ansi      - local 8 bit char set (Ansi Code page)
// WC - wchar_t   - wide char, 16-bit under Win, 32 bit under Unix
// U8 - UTF8      - UTF8 encoded unicode
// WX - wxChar    - current wxChar (char/wchar_t/wxUniChar)

// Multibyte - MB means UTF8 in MB2WX and WX2MB macros

// It can be that converting from an input expression to const char* is
// expensive in an expression: 
//   _is7bit((const char*)pu8s) ? (const char*)pu8s : (const char*)_U82AC((const char*)pu8s)
// Maybe a template could solve it, but then if it is a temporary?

bool _is7bit(const char *ps);
bool _is7bit(const wchar_t *pws);

// Wide char => Ansi code page (Will substitute characters out of range with '?')
wxCharBuffer _WC2AC( const wchar_t *pws );
#define WC2AC(pws) ((const char*)_WC2AC((const wchar_t*)pws))

// UTF8 => Ansi code page (Will substitute characters out of range with '?')
wxCharBuffer _U82AC( const char *pu8s );
#define U82AC(pu8s) (_is7bit((const char*)pu8s) ? (const char*)pu8s : (const char*)_U82AC((const char*)pu8s))


// Wide char => UTF8 
wxCharBuffer _WC2U8( const wchar_t *pws );
#define WC2U8(pws) ((const char*)_WC2U8((const wchar_t*)pws))

// Ansi => UTF8 
wxCharBuffer _AC2U8( const char *ps );
#define AC2U8(ps) (_is7bit((const char*)ps)?(const char*)ps:(const char*)_AC2U8((const char*)ps))


// Ansi char => Wide char 
wxWCharBuffer _AC2WC( const char *ps );
#define AC2WC(ps) ((const wchar_t*)_AC2WC((const char*)ps))

// UTF8 => Wide char  
wxWCharBuffer _U82WC( const char *pu8s );
#define U82WC(pu8s) ((const wchar_t*)_U82WC((const char*)pu8s))


// Adjust all above to current wxChar setting
#if !defined(wxUSE_UNICODE) || wxUSE_UNICODE==0
	// Ansi case: wxChar is char
	#define WX2AC(s) ((const char*)s) 
	#define WX2WC(s) AC2WC(s) 
	#define WX2U8(s) AC2U8(s) 
	#define AC2WX(s) ((const char*)s) 
	#define WC2WX(s) WC2AC(s) 
	#define U82WX(s) U82AC(s)

	// Meaning of MB is UTF8 
	#define WX2MB(s) AC2U8(s)
	#define MB2WX(s) U82AC(s) 
#else 
	#if !defined(wxUSE_UNICODE_UTF8) || wxUSE_UNICODE_UTF8==0
		// 2.4/2.6/2.8 Unicode case: wxChar is char_t
		#define WX2AC(s) WC2AC(s) 
		#define WX2WC(s) ((const wchar_t*)s) 
		#define WX2U8(s) WC2U8(s) 
		#define AC2WX(s) AC2WC(s) 
		#define WC2WX(s) ((const wchar_t*)s) 
		#define U82WX(s) U82WC(s) 

		// Meaning of MB is UTF8 
		#define WX2MB(s) WC2U8(s)
		#define MB2WX(s) U82WC(s) 
	#else
		// 3.0/3.?/... UTF8 case: wxChar is wxUniChar (?) 
		#define WX2AC(s) U82AC(s) 
		#define WX2WC(s) U82WC(s) 
		#define WX2U8(s) ((const char*)s) 
		#define AC2WX(s) AC2U8(s) 
		#define WC2WX(s) WC2U8(s) 
		#define U82WX(s) ((const char*)s) 

		// Meaning of MB is UTF8 
		#define WX2MB(s) ((const char*)s)
		#define MB2WX(s) ((const char*)s) 
	#endif
#endif


// Test the conversion routines above
void ConvTest();

#endif // WXSTRCONV_H


