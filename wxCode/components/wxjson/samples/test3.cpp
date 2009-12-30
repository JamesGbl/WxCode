/******************************************************************************

   test3.cpp

    test for the wxJSONWriter class
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

 jul 2008 (vers. 1.0.0=
 --------
 Test is successfull

*****************************************************************************/


#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/app.h>
#include <wx/log.h>
#include <wx/debug.h>
#include <wx/mstream.h>

#include <wx/jsonval.h>
#include <wx/jsonwriter.h>
#include "test.h"

//
// The function prints the jsonText output and checks if it is
// equal to the expected result: if not it prints the expected
// result and a message.
// Returns ZERO if successfull, 1 otherwaise.
static int
CheckResult( const wxString& jsonText, const wxString& result )
{
	TestCout( _T( "The JSON text output:\n"));
	TestCout( jsonText );
	TestCout( _T( "\nThe expected result:\n"));
	TestCout( result );

	TestCout( _T( "\nChecking the result:\n"));
	size_t len = result.length();
	for ( size_t i = 0; i < len; i++ )  {
		wxChar ch = jsonText.at( i );
		wxChar ex = result.at( i );
		TestCout( ch, false );
		if ( ch != ex)  {
			TestCout( _T( "\n<---- Unexpected character! Should be: "));
			TestCout( ex, true );
		return 1;
		}
	}
	return 0;
}

/************************************************************************

			FAMILY #3: test conversion wxString <-> UTF-8

************************************************************************/


// write to a memory stream some US-ASCII chars and convert to wxString
int Test3_1()
{
	wxMemoryOutputStream os;
	os.Write( "ABCDEFGabcdefg1234567,[]{}", 26 );	// 26 chars
	
	wxFileOffset len = os.GetLength();
	TestCout( _T("buffer length (26): " ));
	TestCout( len, true );
	
	// get the address of the buffer
	wxStreamBuffer* osBuff = os.GetOutputStreamBuffer();
	void* buffStart = osBuff->GetBufferStart();
	
	// convert the UTF-8 buffer into a string
	wxString s;
	s = wxString::FromUTF8( (const char*) buffStart, len );
	
	TestCout( _T("The converted string: "));
	TestCout( s );
	TestCout( _T("\n" ));
	
	wxString expected( _T("ABCDEFGabcdefg1234567,[]{}" ));
	ASSERT( s == expected );
	return 0;
}

static const char utf8Buff[] = {
		// basic latin
		0x61, 0x62, 0x63,		// ABC
		0x41, 0x42, 0x43,		// abc
		0x31, 0x32, 0x33,		// 123
		
		// latin-1 
		0xc3, 0xa0,		// 0xE0 a-grave
		0xc3, 0xa8,		// 0xE8 e-grave
		0xc3, 0xac,		// 0xEC i-grave
		0xc2, 0xa9,		// 0xA9 copyright
		0xc2, 0xae,		// 0xAE registered
		
		// greek and coptic
		0xce, 0xb1,		// 0x3B1 alfa
		0xce, 0xb2,		// 0x3B2 beta
		0xce, 0xb3,		// 0x3B3 gamma
		0xce, 0xb4,		// 0x3B4 delta

		// cyrillic
		0xd0, 0xa4,		// 0x424 capital letter EF
		0xd0, 0xa5,		// 0x425 capital letter HA
		0xd0, 0xa6,		// 0x426 capital letter TSE
		0xd0, 0xa7,		// 0x427 capital letter CHE
		0			// total length (included NULL) = 36
		};
		
static const wchar_t uniBuff[] = {

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
	0	  // total length, NULL included = 23
	};


// write to a memory stream some US-ASCII, latin, greek and cyrillic
// chars and convert to wxString
int Test3_2()
{
	wxMemoryOutputStream os;
	os.Write( utf8Buff, 35 );
	
	wxFileOffset len = os.GetLength();
	TestCout( _T("buffer length (35): " ));
	TestCout( len, true );
	
	// get the address of the buffer
	wxStreamBuffer* osBuff = os.GetOutputStreamBuffer();
	void* buffStart = osBuff->GetBufferStart();
	
	// convert the UTF-8 buffer into a string
	wxString s;
	s = wxString::FromUTF8( (const char*) buffStart, len );
	
	TestCout( _T("The converted string: "));
	TestCout( s );
	TestCout( _T("\n" ));
	
	wxString expected( _T("abcABC123àèì©®αβγδФХЦЧ" ));
	ASSERT( s == expected );	// always fails in ANSI builds
	return 0;
}

// convert a string to UTF-8
int Test3_3()
{
	wxString s1( _T("ABCDEFGabcdefg1234567,[]{}"));
	
	// now convert the string to UTF8 NOTE: a wxCharBuffer is returned
	wxCharBuffer cb = s1.ToUTF8();
	const char* s1Buff = cb.data();	// get the char data. release() can be used to get ownership
	
	// check the length of the buffer
	size_t len = strlen( s1Buff );
	TestCout( _T("Buffer length is (26): " ));
	TestCout( len, true );
	ASSERT( len == 26 );
		
	const char* expected = "ABCDEFGabcdefg1234567,[]{}";
	int r = memcmp( s1Buff, expected, len );
	ASSERT( r == 0 );
	return 0;
}

// convert a latin-1 greek, cyrillic wxString to UTF-8
int Test3_4()
{
	// 's1' is constructed using Unicode UCS-2/4 code units (wchar_t)
	// does not work in ANSI builds
	wxString s1( uniBuff );

	
	// now convert the string to UTF8 NOTE: a wxCharBuffer is returned
	wxCharBuffer cb = s1.ToUTF8();
	const char* s1Buff = cb.data();	// get the char data. release() can be used to get ownership
	
	// check the length of the buffer
	size_t len = strlen( s1Buff );
	TestCout( _T("Buffer length is (35): " ));
	TestCout( len, true );
	ASSERT( len == 35 );
		
	int r = memcmp( s1Buff, utf8Buff, len );
	ASSERT( r == 0 );
	return 0;
}

// convert a UTF-8 buffer to wchar_t and then to a wxString
// oct 2009: the test can be run in either ANSI and Unicode.
// In both modes, the conversion to wchar_t succeeds but the
// conversion to wxString is successfull in Unicode and in
// ANSI an empty string is returned because the presence of
// unrepresentable chars
int Test3_5()
{

	wchar_t destBuffer[42];
	
	size_t convLen = wxConvUTF8.ToWChar( destBuffer,	// wchar_t destination
						42,		// size_t  destLenght 
						utf8Buff,	// char_t  source
						36 );		// size_t  sourceLenght
						
	// convLen should be 23, NULL included 
	TestCout( _T("Conv length: " ));
	TestCout( convLen, true );
	ASSERT( convLen == 23 );
	
	// now convert to a wxString
	wxString s;
	s = wxString::FromUTF8( utf8Buff, 36 );
	TestCout( _T("String length: " ));
	int strLen = s.Len();
	TestCout( strLen, true );
	TestCout( _T("The converted string: " ));
	TestCout( s );

	return 0;


}


/************************************************************************

			FAMILY #3: test the wxJSONWriter

************************************************************************/

// a simple write test
int Test4_1()
{
	wxJSONValue root;
	root[_T("integer")]      = 20;
	root[_T("unsigned int")] = (unsigned int) 30;
	root[_T("bool")]         = true;
	root[_T("double")]       = 20.30;
	root[_T("C string")]     = _T("static string 3");
	root[_T("wxString")]     = _T("wxString 3");

	root[_T("font")][_T("face")] = _T("Arial");
	root[_T("font")][_T("size")] = 12;
	root[_T("font")][_T("bold")] = true;
	root[_T("font")][_T("double")] = 90.67;
	root[_T("font")][_T("encoding")] = wxJSONValue( wxJSONTYPE_NULL);

	wxJSONValue lang;
	lang.Append( _T("it") );
	lang.Append( _T("en") );
	lang.Append( _T("de") );
	lang.Append( _T("is") );
	root[_T("font")][_T("locale")] = lang;

	// this is the same as above
	//root[_T("font")][_T("locale")].Append( "it" );
	//root[_T("font")][_T("locale")].Append( "en" );
	//root[_T("font")][_T("locale")].Append( "de" );
	//root[_T("font")][_T("locale")].Append( "is" );

	// now write the JSON object to a string object
	wxString str;
	wxJSONWriter writer;
	writer.Write( root, str );
	TestCout( str );
	TestCout( _T( "<-----------------\n"));
	return 0;
}

// test escaped chars and  some writer flags on strings
int Test4_2()
{
	wxJSONValue root;
	root.Append( _T("string with \\ (backslash)"));
	root.Append( _T("string with \n (linefeed)"));
	root.Append( _T("string with / (slash)"));
	root.Append( _T("string with \r (CR)"));
	root.Append( _T("string with \t (tab)"));
	root.Append( _T("string with \b (backspace)"));
	root.Append( _T("string with \" (quotes)"));
	root.Append( _T("string with 3 CR\r1-\r 2-\r"));
	root.Append( _T("This is line number 1\nThis is line number 2\nThis is line number 3"));
	root.Append( _T("This is a very long string that cause the user"
					" to scroll orizontally in order to view the whole string"));
	wxString s( _T("string that contains 3 control characters (0x01, 0x03, 0x05):")); 
	s.Append( (wxChar) 1 );
	s.Append( (wxChar) 3 );
	s.Append( (wxChar) 5 );
	root.Append( s );

	// default ctor for the writer
	TestCout( _T( "Using default ctor----------------->\n"));
	wxString str;
	wxJSONWriter wrt;
	wrt.Write( root, str );

	// the expected result
	wxString r1( _T("[\n"
			"   \"string with \\\\ (backslash)\",\n"
			"   \"string with \\n (linefeed)\",\n"
			"   \"string with / (slash)\",\n"
			"   \"string with \\r (CR)\",\n"
			"   \"string with \\t (tab)\",\n"
			"   \"string with \\b (backspace)\",\n"
			"   \"string with \\\" (quotes)\",\n"
			"   \"string with 3 CR\\r1-\\r 2-\\r\",\n"
			"   \"This is line number 1\\nThis is line number 2\\nThis is line number 3\",\n"
			"   \"This is a very long string that cause the user to scroll "
				"orizontally in order to view the whole string\",\n"
			"   \"string that contains 3 control characters (0x01, 0x03, 0x05):"
				"\\u0001\\u0003\\u0005\"\n"
			"]\n"));
			
	int r = CheckResult( str, r1 );
	if ( r != 0 )	{
		return r;
	}

	// strict JSON
	str.Clear();
	TestCout( _T( "Using wxJSONWRITER_NONE----------------->\n"));
	wxJSONWriter wrt2( wxJSONWRITER_NONE );
	wrt2.Write( root, str );
	TestCout( str );
	TestCout( _T( "<-----------------\n"));

	// expected result
	wxString r2( _T(	
		"["
		"\"string with \\\\ (backslash)\""
		",\"string with \\n (linefeed)\""
		",\"string with / (slash)\""
		",\"string with \\r (CR)\""
		",\"string with \\t (tab)\""
		",\"string with \\b (backspace)\""
		",\"string with \\\" (quotes)\""
		",\"string with 3 CR\\r1-\\r 2-\\r\""
		",\"This is line number 1\\nThis is line number 2\\nThis is line number 3\""
		",\"This is a very long string that cause the user to scroll orizontally in"
			" order to view the whole string\""
		",\"string that contains 3 control characters (0x01, 0x03, 0x05):\\u0001\\u0003\\u0005\""
			"]"));
	
	r = CheckResult( str, r2 );
	if ( r != 0 )	{
		return r;
	}

	// split strings: NOTE: very long strings are no more splitted as in 1.0
	str.Clear();
	TestCout( _T( "Using wxJSONWRITER_SPLIT_STRING----------------->\n"));
	wxJSONWriter wrt3( wxJSONWRITER_STYLED | wxJSONWRITER_SPLIT_STRING );
	wrt3.Write( root, str );
	TestCout( str );
	TestCout( _T( "<-----------------\n"));

	// multiline strings
	str.Clear();
	TestCout( _T( "Using wxJSONWRITER_MULTILINE_STRING----------------->\n"));
	wxJSONWriter wrt4( wxJSONWRITER_STYLED | wxJSONWRITER_MULTILINE_STRING );
	wrt4.Write( root, str );
	TestCout( str );
	TestCout( _T( "<-----------------\n"));

	return 0;
}

// prints the 'null', 'invalid', 'empty map' and 'empty array' objects
int Test4_3()
{
	wxJSONValue valueNull;
	wxJSONValue valueIvalid( wxJSONTYPE_INVALID );
	wxJSONValue valueEmptyMap( wxJSONTYPE_OBJECT );
	wxJSONValue valueEmptyArray( wxJSONTYPE_ARRAY );

	wxJSONValue root;
	root.Append( valueNull );

	// root.Append( valueInvalid );		// 6/10/2009 ASSERTION failure in debug builds cannot write
	root.Append( valueEmptyMap );		// an invalid JSON object
	root.Append( valueEmptyArray );

	wxString str;
	wxJSONWriter wrt;
	wrt.Write( root, str );
	TestCout( str );
	TestCout( _T( "<-----------------\n"));
	
	return 0;
}

// an array of objects as root
int Test4_4()
{
	wxJSONValue root;
	root.Append( _T("a string") );
	root.Append( 12 );
	root.Append( true );
	root.Append( wxJSONValue());   // this is a 'null' value

	root.Append( 90e+12 );

	wxString str;
	wxJSONWriter wrt;
	wrt.Write( root, str );
	TestCout( str );
	TestCout( _T( "<-----------------\n"));

	wxString expectedResult = _T("[\n")
		_T("   \"a string\",\n")
		"   12,\n"
		"   true,\n"
		"   null,\n"
		"   9e+13\n"
		"]\n";

	int r = CheckResult( str, expectedResult );
	if ( r != 0 )	{
		return r;
	}
	return 0;
}

// test how much simple is wxJSON
int Test4_5()
{
	wxJSONValue root;
	root[_T("key-1")][_T("key-2")][_T("key-3")][4] = 12;
	wxString str;

	wxJSONWriter wrt;
	wrt.Write( root, str );
	TestCout( str );
	TestCout( _T( "<-----------------\n"));

	return 0;
}

// test control characters
// [
//    "\u0001\u0003\u0005"
// ]
int Test4_6()
{
	wxJSONValue root;
	wxString s;
	s.Append( (wxChar) 1 );
	s.Append( (wxChar) 3 );
	s.Append( (wxChar) 5 );
	root.Append( s );
	
	wxString str;

	wxJSONWriter wrt;
	wrt.Write( root, str );
	TestCout( str );
	TestCout( _T( "<-----------------\n"));

	static const wxChar* expectedResult =
	_T("[\n"
	"   \"\\u0001\\u0003\\u0005\"\n"
	"]\n" );

	ASSERT( str == expectedResult );

	return 0;
}

// test doubles
int Test4_7()
{
	wxJSONValue root;
	root.Append( 90.3 );
	root.Append( 9.123456 );
	root.Append( 10.123456789 );
	root.Append( 0.1234 );
	
	wxString str;

	wxJSONWriter wrt;
	wrt.Write( root, str );
	TestCout( str );
	TestCout( _T( "<-----------------\n"));

	static const wxChar* expectedResult =
	_T("[\n"
	"   90.3,\n"
	"   9.123456,\n"
	"   10.12345679,\n"
	"   0.1234\n"
	"]\n" );

	ASSERT( str == expectedResult );

	return 0;
}

// test the printf function
int Test4_8()
{
	printf( "%f\n", 10.123456789 );
	printf( "%.0f\n", 10.123456789 );
	printf( "%.f\n", 0.123456789 );
	printf( "%.10f\n", 10.123456789 );
	printf( "%.10f\n", 10.123 );

	printf( "\n" );
	printf( "%g\n", 10.123456789 );
	printf( "%.0g\n", 10.123456789 );
	printf( "%.g\n", 0.123456789 );
	printf( "%.20g\n", 10.123456789 );
	printf( "%.10g\n", 10.123 );

	printf( "\n" );
	printf( "fmt=f: %f\n", 0.1 );
	printf( "fmt=g: %g\n", 0.1 );
	printf( "fmt=.0g: %.0g\n", 0.1 );
	printf( "fmt=.g: %.g\n", 0.1 );
	printf( "fmt=.20g: %.20g\n", 0.1 );
	printf( "fmt=.10g: %.10g\n", 0.1 );

	wxString s;
	s.Printf( _T("%f"), 0.1 );
	TestCout( _T("wxString::Printf(%f); "));
	TestCout( s );

	s.Printf( _T("%g"), 0.1 );
	TestCout( _T("\nwxString::Printf(%g); "));
	TestCout( s );

	s.Printf( _T("%.g"), 0.1 );
	TestCout( _T("\nwxString::Printf(%.g); "));
	TestCout( s );

	s.Printf( _T("%.20g"), 0.1 );
	TestCout( _T("\nwxString::Printf(%.20g); "));
	TestCout( s );

	s.Printf( _T("%.10g"), 0.1 );
	TestCout( _T("\nwxString::Printf(%.10g); "));
	TestCout( s );
	
	return 0;
}


/*
{
}
*/

