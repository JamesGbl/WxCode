/******************************************************************************

   test8.cpp

    testing some unicode stuff for unicode support in wxJSON
    Copyright (C) 2007  Luciano Cattani

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


 This application is not a sample nor it isn't part of the wxJSON
 test suite.
 I wrote this test application just for learning something about
 the 'unicode world' which is, at time of writing, absolutely
 unknown by me.

 jul 2008 (version 1.0.0=
 --------
 Tests were successfull in both Unicode and ANSI mode

*****************************************************************************/

#include <stdio.h>
#include <wchar.h>


#include <wx/app.h>
#include <wx/string.h>
#include <wx/debug.h>
#include <wx/strconv.h>

#include "test.h"
#include "wx/jsonval.h"


// check the installed charsets in the wxCSConv class
//
// 19 jan 2008: seems that many charsets are installed in wxWidgets
// so I think I would not implement my own conversion routines
// Here is the output of the test on my system: Linux Fedora 6
// wxWidgets 2.8.7 unicode-debug:
// Constructing wxCSConv class using charset: US-ASCII   IsOK()? NO <---
// Constructing wxCSConv class using charset: ISO-8859-1   IsOK()? YES
// Constructing wxCSConv class using charset: ISO-8859-2   IsOK()? YES
// Constructing wxCSConv class using charset: ISO-8859-3   IsOK()? YES
// Constructing wxCSConv class using charset: ISO-8859-4   IsOK()? YES
// Constructing wxCSConv class using charset: ISO-8859-5   IsOK()? YES
// Constructing wxCSConv class using charset: ISO-8859-6   IsOK()? YES
// Constructing wxCSConv class using charset: ISO-8859-7   IsOK()? YES
// Constructing wxCSConv class using charset: ISO-8859-8   IsOK()? YES
// Constructing wxCSConv class using charset: ISO-8859-9   IsOK()? YES
// Constructing wxCSConv class using charset: ISO-8859-10   IsOK()? YES
// Constructing wxCSConv class using charset: ISO-8859-11   IsOK()? YES
// Constructing wxCSConv class using charset: ISO-8859-12   IsOK()? NO <---
// Constructing wxCSConv class using charset: ISO-8859-13   IsOK()? YES
// Constructing wxCSConv class using charset: ISO-8859-14   IsOK()? YES
// Constructing wxCSConv class using charset: ISO-8859-15   IsOK()? YES
// Constructing wxCSConv class using charset: ISO-8859-16   IsOK()? YES
// Constructing wxCSConv class using charset: KOI8-R   IsOK()? YES
// Constructing wxCSConv class using charset: KOI8-U   IsOK()? YES
// Constructing wxCSConv class using charset: KOI8-RU   IsOK()? YES
// Constructing wxCSConv class using charset: EUC-JP   IsOK()? YES
// Constructing wxCSConv class using charset: SHIFT_JIS   IsOK()? YES
// Constructing wxCSConv class using charset: ISO-2022-JP   IsOK()? YES
// Constructing wxCSConv class using charset: ISO-2022-JP-2   IsOK()? YES
// Constructing wxCSConv class using charset: ISO-2022-JP-1   IsOK()? NO <---
// Constructing wxCSConv class using charset: EUC-KR   IsOK()? YES
// Constructing wxCSConv class using charset: CP949   IsOK()? YES
// Constructing wxCSConv class using charset: ISO-2022-KR   IsOK()? YES
// Constructing wxCSConv class using charset: JOHAB   IsOK()? YES
// Constructing wxCSConv class using charset: UTF-8   IsOK()? YES
// Constructing wxCSConv class using charset: UCS-2   IsOK()? YES
// Constructing wxCSConv class using charset: UCS-2LE   IsOK()? YES
// Constructing wxCSConv class using charset: UCS-2BE   IsOK()? YES
// Constructing wxCSConv class using charset: UCS-4   IsOK()? YES
// Constructing wxCSConv class using charset: UCS-4LE   IsOK()? YES
// Constructing wxCSConv class using charset: UCS-4BE   IsOK()? YES
//
int Test7_1()
{
	static const wxChar* charsets[] = { _T("ASCII"),
		_T("US-ASCII"), _T("ISO-8859-1"), _T("ISO-8859-2"), _T("ISO-8859-3"),
		_T("ISO-8859-4"),_T("ISO-8859-5"),_T("ISO-8859-6"),_T("ISO-8859-7"),
		_T("ISO-8859-8"), _T("ISO-8859-9"), _T("ISO-8859-10"),
		_T("ISO-8859-11"), _T("ISO-8859-12"), _T("ISO-8859-13"),
		_T("ISO-8859-14"), _T("ISO-8859-15"), _T("ISO-8859-16"),
		_T("KOI8-R"), _T("KOI8-U"), _T("KOI8-RU"),

	// Japanese
	_T("EUC-JP"), _T("SHIFT_JIS"), _T("ISO-2022-JP"),
	_T("ISO-2022-JP-2"), _T("ISO-2022-JP-1"),

	// Corean
	_T("EUC-KR"), _T("CP949"), _T("ISO-2022-KR"), _T("JOHAB"),

	// Unicode

	_T("UTF-8" ), _T("UCS-2"), _T("UCS-2LE"), _T("UCS-2BE"),
	_T("UCS-4"), _T("UCS-4LE"), _T("UCS-4BE")
	};

	int numCharsets = sizeof( charsets ) / sizeof( wxChar* );
	for ( int i = 0; i < numCharsets; i++ )  {
		TestCout( _T("Constructing wxCSConv class using charset: " ));
		TestCout( charsets[i] );
		wxCSConv conv( charsets[i] );
		TestCout( _T("   IsOK()? " ));
		bool r = conv.IsOk();
		TestCout( r ? _T("YES\n") : _T("NO\n"));
	}
	return 0;
}


// check the wxJSON_USE_UNICODE macro
// 24 jul 2008: test is successfull on wxGTK-2.8.7
int Test7_2()
{
	// we determine the size of the wxChar data type which is
	// 1 (char) for ANSI builds and >1 for UNICODE builds
	wxChar c;
	int charLen = sizeof( c );
	TestCout( _T("Size of wxChar is "));
	TestCout( charLen, true );

	TestCout( _T("wxUSE_UNICODE is defined? " ));
#if defined( wxUSE_UNICODE )
	TestCout( _T("YES\n" ));
#else
	TestCout( _T("NO\n" ));
#endif

	TestCout( _T("wxJSON_USE_UNICODE is defined? " ));
#if defined( wxJSON_USE_UNICODE )
	TestCout( _T("YES\n" ));
#else
	TestCout( _T("NO\n" ));
#endif

	TestCout( _T("__UNICODE__ is defined? " ));
#if defined( __UNICODE__ )
	TestCout( _T("YES\n" ));
#else
	TestCout( _T("NO\n" ));
#endif

	// the test is successfull if wxJSON_USE_UNICODE is defined in a
	// Unicode build of the library and not defined in a ANSI build
	// in order to test this the function checks the size of wxChar:
	// if == 1 we are in an ANSI build
	// if  > 1 we are in a Unicode build
	bool uni = false;
#if defined( wxJSON_USE_UNICODE )
	uni = true;
#endif
	bool uniResult = false;
	if ( charLen > 1 )  {
		uniResult = true;
	}
	ASSERT( uni == uniResult)

	return 0;
}


// Performs some conversion from wide char strings to other encodings
// using the wxMBConv classes and checks the results against
// predefined buffers obtained using the 'iconv' program
// 
// Test32() converts a Unicode buffer of 22 chars to UCS-2/4
// and UTF8. Then tries to convert to other ISO-8859-n charsets
// but the conversion fails because not all chars can be
// converted to a single charset
	wchar_t uniBuff[] = {

	// basic latin
	0x61, 0x62, 0x63,           // 'abc'   pos  0
	0x41, 0x42, 0x43,           // 'ABC'   pos 12
	0x31, 0x32, 0x33,           // '123'   pos 24

	// Latin-1 supplement
	0xE0,     // a grave         pos 36
	0xE8,     // e grave         pos 40
	0xEC,     // i grave         pos 44
	0xA9,     // copyright sign  pos 48
	0XAE,     // registered sign pos 52

	// Greek and Coptic
	0x3B1,    // alfa            pos 56
	0x3B2,    // beta            pos 60
	0x3B3,    // gamma           pos 64
	0x3B4,    // delta           pos 68

	// Cyrillic
	0x424,    // capital letter EF
	0x425,    // capital letter HA
	0x426,    // capital letter TSE
	0x427,    // capital letter CHE
	0
	};

int Test7_3()
{
	char outBuff[100];
	size_t outLength;

	//////////////////////////////////////////////////////////////
	// converting to UTF8
	//////////////////////////////////////////////////////////////
	const char utf8Buff[] = {
		0x61, 0x62, 0x63, 0x41, 0x42, 0x43, 0x31, 0x32, 
		0x33, 0xc3, 0xa0, 0xc3, 0xa8, 0xc3, 0xac, 0xc2,
		0xa9, 0xc2, 0xae, 0xce, 0xb1, 0xce, 0xb2, 0xce,
		0xb3, 0xce, 0xb4, 0xd0, 0xa4, 0xd0, 0xa5, 0xd0,
		0xa6, 0xd0, 0xa7, 0x00  };

	outLength = wxConvUTF8.WC2MB( outBuff, uniBuff, 100 );
	TestCout( _T("Converting the wide-char buffer to UTF8: length="));
	TestCout( outLength, true );
	ASSERT( outLength != wxCONV_FAILED );

	int r = strcmp( utf8Buff, outBuff );
	TestCout( _T("Comparing the two buffers result="));
	TestCout( r, true );
	ASSERT( r == 0 );

	//////////////////////////////////////////////////////////////
	// converting to UCS-4BE
	//////////////////////////////////////////////////////////////
	const char ucs4beBuff[] = { 
		0x00, 0x00, 0x00, 0x61, 0x00, 0x00, 0x00, 0x62,
		0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x41,
		0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0x43,
		0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0x32,
		0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0xe0,
		0x00, 0x00, 0x00, 0xe8, 0x00, 0x00, 0x00, 0xec,
		0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00, 0xae,
		0x00, 0x00, 0x03, 0xb1, 0x00, 0x00, 0x03, 0xb2,
		0x00, 0x00, 0x03, 0xb3, 0x00, 0x00, 0x03, 0xb4,
		0x00, 0x00, 0x04, 0x24, 0x00, 0x00, 0x04, 0x25,
		0x00, 0x00, 0x04, 0x26, 0x00, 0x00, 0x04, 0x27,
		0x00, 0x00, 0x00, 0x00  };

	wxMBConvUTF32BE ucs4be;
	outLength = ucs4be.WC2MB( outBuff, uniBuff, 100 );
	TestCout( _T("\nConverting the w-char buffer to UCS-4BE length="));
	TestCout( outLength, true );
	ASSERT( outLength != wxCONV_FAILED );

	r = memcmp( ucs4beBuff, outBuff, outLength );
	TestCout( _T("Comparing the two buffers result="));
	TestCout( r, true );
	ASSERT( r == 0 );


	//////////////////////////////////////////////////////////////
	// converting to UCS-4LE
	//////////////////////////////////////////////////////////////
	const char ucs4leBuff[] = {
		0x61, 0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00,
		0x63, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00,
		0x42, 0x00, 0x00, 0x00, 0x43, 0x00, 0x00, 0x00,
		0x31, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00,
		0x33, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00,
		0xe8, 0x00, 0x00, 0x00, 0xec, 0x00, 0x00, 0x00,
		0xa9, 0x00, 0x00, 0x00, 0xae, 0x00, 0x00, 0x00,
		0xb1, 0x03, 0x00, 0x00, 0xb2, 0x03, 0x00, 0x00,
		0xb3, 0x03, 0x00, 0x00, 0xb4, 0x03, 0x00, 0x00,
		0x24, 0x04, 0x00, 0x00, 0x25, 0x04, 0x00, 0x00,
		0x26, 0x04, 0x00, 0x00, 0x27, 0x04, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	};

	wxMBConvUTF32LE ucs4le;
	outLength = ucs4le.WC2MB( outBuff, uniBuff, 100 );
	TestCout( _T("\nConverting w-char string to UCS-4LE length="));
	TestCout( outLength, true );
	ASSERT( outLength != wxCONV_FAILED );

	r = memcmp( ucs4leBuff, outBuff, outLength );
	TestCout( _T("Comparing the two buffers result="));
	TestCout( r, true );
	ASSERT( r == 0 );

	// on some platforms suche as GNU/Linux x86 wide chars are stored
	// as 32 bit integers; endianess is little endian so the UCS-4LE
	// encoding is the same as the wide char buffer
	// we compare the two buffers:
	r = memcmp( ucs4leBuff, ( const char*) uniBuff, outLength );
	TestCout( _T("\nComparing the UCS-4LE buffer with w-char string result="));
	TestCout( r, true );
	TestCout( _T("Result shuold be 0 on GNU/Linux x86\n"));


	//////////////////////////////////////////////////////////////
	// converting to UCS-2LE
	//////////////////////////////////////////////////////////////
	const char ucs2leBuff[] = { 
		0x61, 0x00, 0x62, 0x00, 0x63, 0x00, 0x41, 0x00,
		0x42, 0x00, 0x43, 0x00, 0x31, 0x00, 0x32, 0x00,
		0x33, 0x00, 0xe0, 0x00, 0xe8, 0x00, 0xec, 0x00,
		0xa9, 0x00, 0xae, 0x00, 0xb1, 0x03, 0xb2, 0x03,
		0xb3, 0x03, 0xb4, 0x03, 0x24, 0x04, 0x25, 0x04,
		0x26, 0x04, 0x27, 0x04, 0x00, 0x00   };

	wxMBConvUTF16LE ucs2le;
	outLength = ucs2le.WC2MB( outBuff, uniBuff, 100 );
	TestCout( _T("\nConverting w-char buffer to UCS-2LE length="));
	TestCout( outLength, true );
	ASSERT( outLength != wxCONV_FAILED );

	r = memcmp( ucs2leBuff, outBuff, outLength );
	TestCout( _T("Comparing the two buffers result="));
	TestCout( r, true );
	ASSERT( r == 0 );


	// on some platforms such as Win32 x86 wide chars are stored
	// as 16 bit integers; endianess is little endian so the UCS-2LE
	// encoding is the same as the wide char buffer
	// we compare the two buffers:
	r = memcmp( ucs2leBuff, ( const char*) uniBuff, outLength );
	TestCout( _T("Comparing the UCS-2LE buffer with w-char string result="));
	TestCout( r, true );
	TestCout( _T("Result shuold be 0 on Win32 x86\n"));


	//////////////////////////////////////////////////////////////
	// converting to ISO-8859-1 (Latin-1)
	//////////////////////////////////////////////////////////////
	TestCout( _T("\nConverting the w-char string to ISO-8859-1 (latin-1) length="));
	wxCSConv convLatin( _T("ISO-8859-1"));
	outLength = convLatin.WC2MB( outBuff, uniBuff, 100 );
	TestCout( outLength, true );

	// the conversion cannot be done because the unicode buffer
	// contains greek and cyrillic chars that cannot be represented
	// in the Latin-1 charset
	ASSERT( outLength == wxCONV_FAILED )


	//////////////////////////////////////////////////////////////
	// converting to ISO-8859-7 (Greek)
	//////////////////////////////////////////////////////////////
	TestCout( _T("Converting the w-char string to ISO-8859-7 (greek) length="));
	wxCSConv convGreek( _T("ISO-8859-7"));
	outLength = convGreek.WC2MB( outBuff, uniBuff, 100 );
	TestCout( outLength, true );

	// the conversion cannot be done because the unicode buffer
	// contains latin-1 and cyrillic chars that cannot be represented
	// in the Greek charset
	ASSERT( outLength == wxCONV_FAILED )


	//////////////////////////////////////////////////////////////
	// converting to ISO-8859-5 (Cyrillic)
	//////////////////////////////////////////////////////////////
	TestCout( _T("Converting the w-char string to ISO-8859-5 (cyrillic) length="));
	wxCSConv convCyril( _T("ISO-8859-5"));
	outLength = convCyril.WC2MB( outBuff, uniBuff, 100 );
	TestCout( outLength, true );

	// the conversion cannot be done because the unicode buffer
	// contains latin-1 and greek chars that cannot be represented
	// in the cyrillic charset
	ASSERT( outLength == wxCONV_FAILED )

	return 0;
}


/*******************************************************************
// Test33() converts a Unicode buffer of 14 chars to UCS-2/4,
// UTF8 and ISO-8859-1/5/7 charsets.
// Then unicode buffer contain us-ascii and latin-1 chars so the
// only conversion that can be done in ISO-8859 is the Latin-1
// charset.
*******************************************************************/
	wchar_t uniBuffLatin[] = {

		// basic latin
		0x61, 0x62, 0x63,           // 'abc'   pos  0
		0x41, 0x42, 0x43,           // 'ABC'   pos 12
		0x31, 0x32, 0x33,           // '123'   pos 24

		// Latin-1 supplement
		0xE0,     // a grave         pos 36
		0xE8,     // e grave         pos 40
		0xEC,     // i grave         pos 44
		0xA9,     // copyright sign  pos 48
		0XAE,     // registered sign pos 52
		0
	};


int Test7_4()
{
	char outBuff[100];
	size_t outLength;

	//////////////////////////////////////////////////////////////
	// converting to UTF8
	//////////////////////////////////////////////////////////////
	const char utf8Buff[] =  {
		0x61, 0x62, 0x63, 0x41, 0x42, 0x43, 0x31, 0x32,
		0x33, 0xc3, 0xa0, 0xc3, 0xa8, 0xc3, 0xac, 0xc2,
		0xa9, 0xc2, 0xae, 0x00  };

	outLength = wxConvUTF8.WC2MB( outBuff, uniBuffLatin, 100 );
	TestCout( _T("\nConverting w-char string to UTF8 length="));
	TestCout( outLength, true );
	ASSERT( outLength != wxCONV_FAILED );

	int r = memcmp( utf8Buff, outBuff, outLength );
	TestCout( _T("Comparing the two buffers result="));
	TestCout( r, true );
	ASSERT( r == 0 );


	//////////////////////////////////////////////////////////////
	// converting to UCS-4LE
	//////////////////////////////////////////////////////////////
	const char ucs4leBuff[] = {
		0x61, 0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00,
		0x63, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00,
		0x42, 0x00, 0x00, 0x00, 0x43, 0x00, 0x00, 0x00,
		0x31, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00,
		0x33, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00,
		0xe8, 0x00, 0x00, 0x00, 0xec, 0x00, 0x00, 0x00,
		0xa9, 0x00, 0x00, 0x00, 0xae, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00  };

	wxMBConvUTF32LE convUTF32le;
	outLength = convUTF32le.WC2MB( outBuff, uniBuffLatin, 100 );
	TestCout( _T("\nConverting w-char string to UCS-4LE length="));
	TestCout( outLength, true );
	ASSERT( outLength != wxCONV_FAILED );

	r = memcmp( ucs4leBuff, outBuff, outLength );
	TestCout( _T("Comparing the two buffers result="));
	TestCout( r, true );
	ASSERT( r == 0 );



	//////////////////////////////////////////////////////////////
	// converting to UCS-2BE
	//////////////////////////////////////////////////////////////
	// do not have the output of 'iconv'


	//////////////////////////////////////////////////////////////
	// converting to ISO-8859-1 (Latin-1)
	//////////////////////////////////////////////////////////////
	const char latinBuff[] = {
		0x61, 0x62, 0x63, 0x41, 0x42, 0x43, 0x31, 0x32,
		0x33, 0xe0, 0xe8, 0xec, 0xa9, 0xae, 0x00
	};

	TestCout( _T("\nConverting w-char string to ISO-8859-1 (latin-1) length="));
	wxCSConv convLatin( _T("ISO-8859-1"));
	outLength = convLatin.WC2MB( outBuff, uniBuffLatin, 100 );
	TestCout( outLength, true );

	r = memcmp( latinBuff, outBuff, outLength );
	TestCout( _T("Comparing the two buffers result="));
	TestCout( r, true );
	ASSERT( r == 0 );


	//////////////////////////////////////////////////////////////
	// converting to ISO-8859-7 (Greek)
	//////////////////////////////////////////////////////////////
	TestCout( _T("\nConverting w_char string to ISO-8859-7 (Greek) length="));
	wxCSConv convGreek( _T("ISO-8859-7"));
	outLength = convGreek.WC2MB( outBuff, uniBuffLatin, 100 );
	TestCout( outLength, true );

	// the conversion cannot be done because the unicode buffer
	// contains latin-1 characters that cannot be represente in the
	// greek charset
	ASSERT( outLength == wxCONV_FAILED )


	//////////////////////////////////////////////////////////////
	// converting to ISO-8859-5 (Cyrillic)
	//////////////////////////////////////////////////////////////
	TestCout( _T("\nConverting w-char string to ISO-8859-5 (Cyrillic) length="));
	wxCSConv convCyril( _T("ISO-8859-5"));
	outLength = convCyril.WC2MB( outBuff, uniBuffLatin, 100 );
	TestCout( outLength, true );

	// the conversion cannot be done because the unicode buffer
	// contains latin-1 characters that cannot be represente in the
	// cyrillic charset
	ASSERT( outLength == wxCONV_FAILED )

  return 0;
}



/*******************************************************************
// Test34() converts a Unicode buffer of 13 chars to UCS-2/4,
// UTF8 and ISO-8859-1/5/7 charsets.
// The unicode buffer contain us-ascii and greek chars so the
// only conversion that can be done in ISO-8859 is the Greek
// charset.
*******************************************************************/
	wchar_t uniBuffGreek[] = {

		// basic latin
		0x61, 0x62, 0x63,           // 'abc'   pos  0
		0x41, 0x42, 0x43,           // 'ABC'   pos 12
		0x31, 0x32, 0x33,           // '123'   pos 24

		// Greek and Coptic
		0x3B1,    // alfa            pos 36
		0x3B2,    // beta            pos 40
		0x3B3,    // gamma           pos 44
		0x3B4,    // delta           pos 48

		0
	};

int Test7_5()
{
	char outBuff[100];
	size_t outLength;

	//////////////////////////////////////////////////////////////
	// converting to UTF8
	//////////////////////////////////////////////////////////////
	const char utf8Buff[] = {
		0x61, 0x62, 0x63, 0x41, 0x42, 0x43, 0x31, 0x32,
		0x33, 0xce, 0xb1, 0xce, 0xb2, 0xce, 0xb3, 0xce,
		0xb4, 0x00 };

	outLength = wxConvUTF8.WC2MB( outBuff, uniBuffGreek, 100 );
	TestCout( _T("\nConverting w-char string to UTF8 length="));
	TestCout( outLength, true );
	ASSERT( outLength != wxCONV_FAILED );

	int r = memcmp( utf8Buff, outBuff, outLength );
	TestCout( _T("Comparing the buffers result="));
	TestCout( r, true );
	ASSERT( r == 0 );


	//////////////////////////////////////////////////////////////
	// converting to ISO-8859-7 (Greek)
	//////////////////////////////////////////////////////////////
	const char greekBuff[] = {
		0x61, 0x62, 0x63, 0x41, 0x42, 0x43, 0x31, 0x32,
		0x33, 0xe1, 0xe2, 0xe3, 0xe4, 0x00 };

	TestCout( _T("\nConverting w-char string to ISO-8859-7 (Greek) length="));
	wxCSConv convGreek( _T("ISO-8859-7"));
	outLength = convGreek.WC2MB( outBuff, uniBuffGreek, 100 );
	TestCout( outLength, true );

	r = memcmp( greekBuff, outBuff, outLength );
	TestCout( _T("Comparing the buffers result="));
	TestCout( r, true );
	ASSERT( r == 0 );


	//////////////////////////////////////////////////////////////
	// converting to ISO-8859-1 (Latin-1)
	//////////////////////////////////////////////////////////////
	// iconv: illegal input sequence at position 36
	TestCout( _T("\nConverting w-char string to ISO-8859-1 (Latin-1) length="));
	wxCSConv convLatin( _T("ISO-8859-1"));
	outLength = convLatin.WC2MB( outBuff, uniBuffGreek, 100 );
	TestCout( outLength, true );

	// the conversion cannot be done because the unicode buffer
	// contains greek characters that cannot be represente in the
	// latin-1 charset
	ASSERT( outLength == wxCONV_FAILED )


	//////////////////////////////////////////////////////////////
	// converting to ISO-8859-5 (Cyrillic)
	//////////////////////////////////////////////////////////////
	// iconv: illegal input sequence at position 36
	TestCout( _T("\nConverting w-char string to ISO-8859-5 (Cyrillic) length="));
	wxCSConv convCyril( _T("ISO-8859-5"));
	outLength = convCyril.WC2MB( outBuff, uniBuffGreek, 100 );
	TestCout( outLength, true );

	// the conversion cannot be done because the unicode buffer
	// contains greek characters that cannot be represente in the
	// cyrillic charset
	ASSERT( outLength == wxCONV_FAILED )

	return 0;
}




/*******************************************************************
// Test35() converts a Unicode buffer of 13 chars to UCS-2/4,
// UTF8 and ISO-8859-1/5/7 charsets.
// The unicode buffer contain us-ascii and cyrillic chars so the
// only conversion that can be done in ISO-8859 is the Cyrillic
// charset.
*******************************************************************/
	wchar_t uniBuffCyril[] = {

		// basic latin
		0x61, 0x62, 0x63,           // 'abc'   pos  0
		0x41, 0x42, 0x43,           // 'ABC'   pos 12
		0x31, 0x32, 0x33,           // '123'   pos 24

		// Cyrillic
		0x424,    // capital letter EF
		0x425,    // capital letter HA
		0x426,    // capital letter TSE
		0x427,    // capital letter CHE
		0
	};


int Test7_6()
{
	char outBuff[100];
	size_t outLength;

	//////////////////////////////////////////////////////////////
	// converting to UCS-4LE
	//////////////////////////////////////////////////////////////
	const char ucs4leBuff[] = {
		0x61, 0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00,
		0x63, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00,
		0x42, 0x00, 0x00, 0x00, 0x43, 0x00, 0x00, 0x00,
		0x31, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00,
		0x33, 0x00, 0x00, 0x00, 0x24, 0x04, 0x00, 0x00,
		0x25, 0x04, 0x00, 0x00, 0x26, 0x04, 0x00, 0x00,
		0x27, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	wxMBConvUTF32LE convUTF32le;
	outLength = convUTF32le.WC2MB( outBuff, uniBuffCyril, 100 );
	TestCout( _T("\nConverting w-char string to UCS-4LE length="));
	TestCout( outLength, true );
	ASSERT( outLength != wxCONV_FAILED );

	int r = memcmp( ucs4leBuff, outBuff, outLength );
	TestCout( _T("Comparing the buffers result="));
	TestCout( r, true );
	ASSERT( r == 0 );


	//////////////////////////////////////////////////////////////
	// converting to UTF8
	//////////////////////////////////////////////////////////////
	const char utf8Buff[] = {
		0x61, 0x62, 0x63, 0x41, 0x42, 0x43, 0x31, 0x32,
		0x33, 0xd0, 0xa4, 0xd0, 0xa5, 0xd0, 0xa6, 0xd0,
		0xa7, 0x00
	};

	outLength = wxConvUTF8.WC2MB( outBuff, uniBuffCyril, 100 );
	TestCout( _T("\nConverting w-char string to UTF8 length="));
	TestCout( outLength, true );
	ASSERT( outLength != wxCONV_FAILED );

	r = memcmp( utf8Buff, outBuff, outLength );
	TestCout( _T("Comparing the buffers result="));
	TestCout( r, true );
	ASSERT( r == 0 );


	//////////////////////////////////////////////////////////////
	// converting to ISO-8859-5 (Cyrillic)
	//////////////////////////////////////////////////////////////
	const char cyrilBuff[] = {
		0x61, 0x62, 0x63, 0x41, 0x42, 0x43, 0x31, 0x32,
		0x33, 0xc4, 0xc5, 0xc6, 0xc7, 0x00
	};

	TestCout( _T("\nConverting w-char string to ISO-8859-5 (Cyrillic) length="));
	wxCSConv convCyril( _T("ISO-8859-5"));
	outLength = convCyril.WC2MB( outBuff, uniBuffCyril, 100 );
	TestCout( outLength, true );

	r = memcmp( cyrilBuff, outBuff, outLength );
	TestCout( _T("Comparing the buffers result="));
	TestCout( r, true );
	ASSERT( r == 0 );


	//////////////////////////////////////////////////////////////
	// converting to ISO-8859-7 (Greek)
	//////////////////////////////////////////////////////////////
	// iconv: illegal input sequence at position 36
	TestCout( _T("\nConverting w-char string to ISO-8859-7 (Greek) length="));
	wxCSConv convGreek( _T("ISO-8859-7"));
	outLength = convGreek.WC2MB( outBuff, uniBuffCyril, 100 );
	TestCout( outLength, true );

	// the conversion cannot be done because the unicode buffer
	// contains cyrillic characters that cannot be represente in the
	// greek charset
	ASSERT( outLength == wxCONV_FAILED )


	//////////////////////////////////////////////////////////////
	// converting to ISO-8859-1 (Latin-1)
	//////////////////////////////////////////////////////////////
	// iconv: illegal input sequence at position 36
	TestCout( _T("\nConverting w-char string to ISO-8859-1 (Latin-1) length="));
	wxCSConv convLatin( _T("ISO-8859-1"));
	outLength = convLatin.WC2MB( outBuff, uniBuffCyril, 100 );
	TestCout( outLength, true );

	// the conversion cannot be done because the unicode buffer
	// contains cyrillic characters that cannot be represente in the
	// latin-1 charset
	ASSERT( outLength == wxCONV_FAILED )


	return 0;
}



/*
{
}
*/



