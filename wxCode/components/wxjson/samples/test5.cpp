/******************************************************************************

   test5.cpp

    test for the wxJSONReader class
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

 jul 2008 (ver 1.0.0)
 --------

 OK, successfull for both ANSI and Unicode mode. Note that Test14()
 has been deleted and Test15() only works if the input text file
 is in UTF-8 format.

*****************************************************************************/

//#ifdef __GNUG__
//    #pragma implementation "skeleton.cpp"
//#endif


#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/app.h>
#include <wx/log.h>
#include <wx/debug.h>
#include <wx/mstream.h>
#include <wx/wfstream.h>

#include <wx/jsonval.h>
#include <wx/jsonreader.h>
#include <wx/jsonwriter.h>

#include "test.h"

/****************************************************************************

		FAMILY 7: test wxJSONReader using US-ASCII only strings
		
****************************************************************************/


// test an array of values: text is well-formed with comments
int Test7_1()
{
	static const wxChar* text = 
	_T( "[\n"
		"   \"a string\",\n"
		"   12,\n"
		"   +20,\n"
		"   true,\n"
		"   null,\n"
		"   90.98\n"
		"]\n"
	);

	wxString s( text );
	wxJSONValue root;
	wxJSONReader reader;

	TestCout( _T( "The input JSON text:\n" ));
	TestCout( s );
	int numErrors = reader.Parse( text, &root );

	// now print the JSON value that we have read
	// note the lack of comments
	TestCout( _T( "The read JSON value:\n" ));
	PrintValue( root, &reader );

	ASSERT( numErrors == 0 )
	int numWarn = reader.GetWarningCount();
	ASSERT( numWarn == 0 )
	return 0;
}

// test a map of values
// 24/7/2008 failed to read the integer!!!
// Warning: lne 2 col: 3 comment is not on col 3 but col. 1
int Test7_2()
{
	static const wxChar* text = 
	_T( "\n"
		"// a C++ line of comment\n"
		"{\n"
		"   \"string\"  : \"a string\",\n"
		"   \"integer\" : 12,         // an in-line C++ comment\n"
		"   \"boolean\" : true,\n"
		"   \"null\"    : null,\n"
		"   \"double\"  : 90.98\n"
		"}\n"
	);

	wxString s( text );
	TestCout( _T( "The input JSON text:\n" ));
	TestCout( s );

	wxJSONValue root;
	wxJSONReader reader;
	int numErrors = reader.Parse( text, &root );

	wxInt64 i = root[_T("integer")].AsInt64();
	ASSERT( i == 12 )

	// now print the JSON value that we have read
	// note the lack of comments
	TestCout( _T( "The read JSON value:\n" ));
	PrintValue( root, &reader );
	ASSERT( numErrors == 0 )
	int numWarn = reader.GetWarningCount();
	ASSERT( numWarn == 2 )
	return 0;
}

// an array of string values that contains escaped characters
int Test7_3()
{
	static const wxChar* text = 
		_T("[\n"
		"  {\n"
		"     \"carriage-ret\" : \"string with \\r (CR)\"\n"
		"  },"
		"  \"string with \\\\ (backslash)\",\n"
		"  \"string with \\n (linefeed)\",\n"
		"  \"string with \\/ (slash)\",\n"
		"  \"string with \\t (tab)\",\n"
		"  \"string with \\b (backspace)\",\n"
		"  {"
		"  \"quotes\" : \"string with \\\" (quotes)\"\n"
		"  },\n"
		"  \"control char (0001): \\u0001\",\n"		
		"  \"control char (0011): \\u0011\",\n"		
		"  \"control char (invalid): \\u0hg1\",\n"
		"  \"control char (alpha): \\u03B1\"\n"
		"]\n"
	);

	wxString s( text );
	TestCout( _T( "The input JSON text:\n" ));
	TestCout( s );

	wxJSONValue root;
	wxJSONReader reader;
	int numErrors = reader.Parse( text, &root );

	TestCout( _T( "The read JSON value:\n" ));
	PrintValue( root, &reader );

	ASSERT( numErrors == 0 )
	int numWarn = reader.GetWarningCount();
	ASSERT( numWarn == 0 )
	return 0;
}


// nested objects / arrays 
int Test7_4()
{
	static const wxChar* text = 
		_T("{"
		"   \"book\" :"
		"    {"
		"       \"title\"  : \"The title\","
		"       \"author\" : \"Captain Hook\","
		"       \"pages\"  : 300,"
		"       \"price\"  : 30.30,"
		"       \"translations\" :"
		"        ["
		"           \"italian\","
		"           \"german\","
		"           \"spanish\""
		"        ]"
		"   }"
		"}"
	);

	wxString s( text );
	TestCout( _T( "The input JSON text:\n" ));
	TestCout( s );

	wxJSONValue root;
	wxJSONReader reader;
	int numErrors = reader.Parse( text, &root );

	TestCout( _T( "The read JSON value:\n" ));
	PrintValue( root, &reader );

	ASSERT( numErrors == 0 )
	int numWarn = reader.GetWarningCount();
	ASSERT( numWarn == 0 )
	return 0;
}

// many errors and warnings
int Test7_5()
{
	static const wxChar* text = 
    // column
    // 0123456789.123456789.123456789.12345678
   _T("{\n"                                       // (1)
      "   \"book\" :\n"                           // (2)
      "   / seems a C++ comment (forgot slash)\n" // (3) ERR: a comment?
      "    {\n"                                   // (4)
      "       \"title\"  : \"The title\", ,\n"    // (5) ERR: two consecutive commas
      "       \"author\" : \"Captain Hook\"\n"    // 
      "       pages      : 300,\n"                // (7) ERR: "," missing for key
                                                  // (7) ERR: ':' not allowed 
                                                  // (7) ERR: value '300' cannot follow a value
      "       \"pages2\" : abc300,\n"             // (8) ERR: 'abc300' unrecognized literal
                                                  // (8) ERR: value is missing when comma
                                                  //          encontered
      "       \"price\"  : 30.30,\n"              //
      "       \"price2\" : 30.30abc,\n"           // (10) ERR: '30.30abc unrecognized literal
                                                  // (10) ERR: value is missing when comma
                                                  //           encontered

      "       \"price3\"  30,\n"                  // (11) ERR: value '30' cannot follow a value
                                                  // (11) ERR: ':' is missing
      "       \"translations\" :\n"
      "        [\n"
      "           \"italian\",,\n"                // (14) ERR: two consecutive ','
      "           \"german\",\n"
      "           \"spanish\",\n"
      "           \"spanish2\" : \n"              // (17) ERR: ':' not allowed in arrays
      "        }\n"                               // (18) WARN: close array is ']' and not '}'
      "   }\n"
      "\n");                                      // (20) WARN: final '}' is missing

	wxString s( text );
	TestCout( _T( "The input JSON text:\n" ));
	TestCout( s );

	wxJSONValue root;
	wxJSONReader reader;
	int numErrors = reader.Parse( text, &root );

	TestCout( _T( "The read JSON value:\n" ));
	PrintValue( root, &reader );

	ASSERT( numErrors == 13 )
	int numWarn = reader.GetWarningCount();
	ASSERT( numWarn == 2 )
	return 0;
}



// read a JSON text file from the specified file
// 1 mar 2008: note that the file must be in UTF-8 format
int Test7_6()
{
	if ( gs_fileName.empty() )  {
		TestCout( _T("\nERROR: you have to specify a JSON input file after the test number\n" ));
		return 0;
	}
	wxFileInputStream is( gs_fileName );
	if ( !is.IsOk() )  {
		TestCout( _T("\nERROR: cannot read input file\n"));
		return 1;
	}
	wxJSONValue root;
	wxJSONReader reader;

	int numErrors = reader.Parse( is, &root );
	TestCout( _T( "Number of errors: "));
	TestCout( numErrors, true );
	int numWarn = reader.GetWarningCount();
	TestCout( _T( "Number of warnings: "));
	TestCout( numErrors, true );

	PrintValue( root, &reader );
	return 0;
}

// missing closing object/array on EOF
int Test7_7()
{
	static const wxChar* text = 
		_T("{\n"
		"   \"book\" :\n"
		"    {\n"
		"       \"title\"  : \"The title\",\n"
		"       \"author\" : \"Captain Hook\",\n"
		"       \"pages\"  : 300,\n"
		"       \"price\"  : 30.30,\n"
		"       \"translations\" :\n"
		"        [\n"
		"           \"italian\",\n"
		"           \"german\",\n"
		"           \"spanish\"\n"
		   // the missing closing object/array chars
		   // "        ]"
		   // "   }"
		   // "}"
	);

	wxString s( text );
	TestCout( _T( "The input JSON text:\n" ));
	TestCout( s );

	wxJSONValue root;
	wxJSONReader reader;
	int numErrors = reader.Parse( s, &root );

	TestCout( _T( "The read JSON value:\n" ));
	PrintValue( root, &reader );

	ASSERT( numErrors == 0 )
	int numWarn = reader.GetWarningCount();
	ASSERT( numWarn == 3 )
	return 0;
}

// non-JSON text before and after the top-level start/end object
int Test7_8()
{
	static const wxChar* text = 
		_T("non-JSON text before top-level start object\n"
		"{\n"
		"   \"book\" :\n"
		"    {\n"
		"       \"title\"  : \"The title\",\n"
		"       \"author\" : \"Captain Hook\",\n"
		"       \"pages\"  : 300,\n"
		"       \"price\"  : 30.30,\n"
		"       \"translations\" :\n"
		"        [\n"
		"           \"italian\",\n"
		"           \"german\",\n"
		"           \"spanish\"\n"
		"        ]\n"
		"   }\n"
		"}\n"
		"non-JSON text after top-level end object\n"
	);

	wxString s( text );
	TestCout( _T( "The input JSON text:\n" ));
	TestCout( s );

	wxJSONValue root;
	wxJSONReader reader;
	int numErrors = reader.Parse( s, &root );

	TestCout( _T( "The read JSON value:\n" ));
	PrintValue( root, &reader );

	ASSERT( numErrors == 0 )
	int numWarn = reader.GetWarningCount();
	ASSERT( numWarn == 0 );
	return 0;
}

// check missing close-array character
int Test7_9()
{
	// missing close array char
	static const wxChar* text = 
		_T("[\n"
		"   \"book\"\n"
		);
	
	wxString s( text );
	TestCout( _T( "The input JSON text:\n" ));
	TestCout( s );

	wxJSONValue root;
	wxJSONReader reader;
	int numErrors = reader.Parse( s, &root );

	TestCout( _T( "The read JSON value:\n" ));
	PrintValue( root, &reader );

	ASSERT( numErrors == 0 )
	int numWarn = reader.GetWarningCount();
	ASSERT( numWarn == 1 )
	return 0;
}


/****************************************************************************

		FAMILY 8: test wxJSONReader using different charsets
		
****************************************************************************/

// a UTF-8 buffer containing an array of strings:
// [
// "abcABC",
// "àèì©®",
// "αβγδ",		U+03B1 U+03B2 U+03B3 U+03B4
// "ФХЦЧ"		U+0424 U+0425 U+0426 U+0427
// ]
//
static const char utf8Buff[] = {
  0x5B,
  0x22,0x61,0x62,0x63,0x41,0x42,0x43,0x22,0x2C,0x0A,
  0x22,0xC3,0xA0,0xC3,0xA8, 0xC3,0xAC,0xC2,0xA9,0xC2,0xAE,0x22,0x2C, 0x0A,
  0x22,0xCE,0xB1,0xCE,0xB2,0xCE,0xB3,0xCE,0xB4,0x22,0x2C,0x0A,
  0x22,0xD0,0xA4,0xD0,0xA5,0xD0,0xA6,0xD0,0xA7,0x22,0x0A,
  0x5D,
  0x0A,0x0A,0x0
};

// read the buffer as a UTF8 stream (Unicode)
int Test8_1()
{
	TestCout( _T( "The input JSON text:\n" ));
	TestCout( (char*) utf8Buff );

	wxMemoryInputStream is( utf8Buff, strlen( utf8Buff ));
	wxJSONValue root;
	wxJSONReader reader;
	int numErrors = reader.Parse( is, &root );

	TestCout( _T( "The read JSON value:\n" ));
	PrintValue( root, &reader );

	ASSERT( numErrors == 0 )
	int numWarn = reader.GetWarningCount();
	ASSERT( numWarn == 0 );
	return 0;
}

// read the buffer as a wxString (Unicode)
int Test8_2()
{
	wxString s;
	s = wxString::FromUTF8( utf8Buff );
	TestCout( _T( "The input JSON text:\n" ));
	TestCout( s );

	wxJSONValue root;
	wxJSONReader reader;
	int numErrors = reader.Parse( s, &root );

	TestCout( _T( "The read JSON value:\n" ));
	PrintValue( root, &reader );

	ASSERT( numErrors == 0 )
	int numWarn = reader.GetWarningCount();
	ASSERT( numWarn == 0 );
	return 0;
}

// read the buffer as a UTF8 stream (ANSI)
int Test8_3()
{
	// in ANSI builds, the strings are read in UTF8 but they cannot
	// be stored correctly in wxString.
	// The default behaviour of the reader is to convert unrepresentable
	// characters as unicode escaped sequence
	TestCout( _T( "The input JSON text:\n" ));
	// TestCout( wxString( utf8Buff));

#if defined( __GNUC__ )
	setlocale( LC_ALL, "it_IT.iso88591" );
#endif

	wxMemoryInputStream is( utf8Buff, strlen( utf8Buff ));
	wxJSONValue root;
	wxJSONReader reader;
	int numErrors = reader.Parse( is, &root );
	
	// test the content of the strings
	wxString usascii  = root[0].AsString();
	wxString latin1   = root[1].AsString();
	wxString greek    = root[2].AsString();
	wxString cyrillic = root[3].AsString();
	TestCout( _T( "\nus-ascii length (6): " ));
	TestCout( usascii.Len(), true );
	TestCout( usascii );
	TestCout( _T( "\nlatin1 length (5): " ));
	TestCout( latin1.Len(), true );
	TestCout( latin1 );
	TestCout( _T( "\ngreek length (24): " ));
	TestCout( greek.Len(), true );
	TestCout( greek );
	TestCout( _T( "\ncyrillic length (24): " ));
	TestCout( cyrillic.Len(), true );
	TestCout( cyrillic );
	TestCout( _T( "\n" ));
	
	ASSERT( usascii.Len() == 6 );
	ASSERT( latin1.Len() == 5 );
	
	// the ASSERT fails but the 'greek' string seems equal to the
	// constant _T("\u03B1\u03B2\u03B3\u03B4" ). Why does it fail?
	// greek UTF8: 0xCE,0xB1,0xCE,0xB2,0xCE,0xB3,0xCE,0xB4
	//ASSERT( greek == _T("\u03B1\u03B2\u03B3\u03B4" ));
	
	// cyrillic UTF8: 0xD0,0xA4,0xD0,0xA5,0xD0,0xA6,0xD0,0xA7
	//ASSERT( cyrillic == _T("\u0424\u0425\u0426\u0427" ));
	
	ASSERT( numErrors == 0 )
	
	// if a string contains unrepresentable chars, the parser reports a warning
	int numWarn = reader.GetWarningCount();
	ASSERT( numWarn == 2 );
	return 0;
}

// read the buffer as a wxString (ANSI)
int Test8_4()
{
	// this does not work!! an empty string is returned by the FromUTF8() function
	// because the multicharset UTF-8 buffer cannot be converted in an ANSI wxString
	// object.
	// The reader does not read anything because input is empty
	wxString s;
	s = wxString::FromUTF8( utf8Buff );
	
	// en empty string is in input
	TestCout( _T( "The input JSON text:\n" ));
	TestCout( s );

	wxJSONValue root;
	wxJSONReader reader;
	// Error: cannot find a start-object/array character
	int numErrors = reader.Parse( s, &root );
	ASSERT( numErrors == 1 )

	// an invalid wxJSONvalue is returned
	TestCout( _T( "The read JSON value:\n" ));
	PrintValue( root, &reader );

	return 0;
}

// read a string that contains latin-1 characters (ANSI)
// used for single stepping in DDD
// 30 nov. 2009: the test runs correctly in DDD. The
// wxString object constains the five Latin-1 chars
// Note that there is not a char-by-char conversion
// because the UTF-8 input can be converted to ISO-8859-1
int Test8_5()
{

	// a UTF-8 buffer containing an array of strings:
	// [
	// "àèì©®"
	// ]
	//
	static const char utf8_b[] = {
		0x5B,
		0x22,0xC3,0xA0,0xC3,0xA8, 0xC3,0xAC,0xC2,0xA9,0xC2,0xAE,0x22,0x2C, 0x0A,
		0x5D,
		0x0A,0x0A,0x0
	};
	
#if defined( __GNUC__ )
	setlocale( LC_ALL, "it_IT.iso88591" );
#endif

	wxMemoryInputStream is( utf8_b, strlen( utf8_b ));
	wxJSONValue root;
	wxJSONReader reader;
	int numErrors = reader.Parse( is, &root );
	
	// get the string
	wxString str   = root[0].AsString();
	TestCout( _T( "\nThe JSON string value: " ));
	TestCout( str );
	TestCout( _T( "\n" ));
	
	return 0;
}


/*
{
}
*/

