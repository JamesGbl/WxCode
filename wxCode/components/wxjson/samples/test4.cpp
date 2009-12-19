/******************************************************************************

   test4.cpp

    test for the wxJSONwriter: testing writer's flag and characters other than US-ASCII
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

 jul 2008 (vers. 1.0.0.)
 --------
 Test is successfull in both Unicode and ANSI mode

*****************************************************************************/

// using namespace std;

//#ifdef __GNUG__
//    #pragma implementation "skeleton.cpp"
//#endif


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
	size_t len = jsonText.length();
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

//
// The function prints the jsonText output and checks if it is
// equal to the expected result: if not it prints the expected
// result and a message.
// Returns ZERO if successfull, 1 otherwaise.
static int
CheckResult( const wxMemoryOutputStream& jsonText, const char* result )
{
	// get the address of the buffer
	wxStreamBuffer* osBuff = jsonText.GetOutputStreamBuffer();
	char* buffStart = (char*) osBuff->GetBufferStart();
	size_t len = jsonText.GetLength();

	TestCout( _T( "\nThe expected result:\n"));
	TestCout( result );

	TestCout( _T( "\nChecking the result:\n"));
	for ( size_t i = 0; i < len; i++ )  {
		char ch = buffStart[i];
		char ex = result[i];
		TestCout( ch, false );
		if ( ch != ex)  {
			TestCout( _T( "\n<---- Unexpected character! Should be: "));
			TestCout( ex, true );
		return 1;
		}
	}
	return 0;
}



//
// the function stores some values (strings, booleans, integers etc.
// int a wxJSONValue array and adds comments to each element
// comments are in both C-style and C++ style
static void
StoreValue1( wxJSONValue& value, int commentPos = wxJSONVALUE_COMMENT_INLINE )
{
	value.Append( _T( "a string"));
	value[0].AddComment( _T( "// a string object"), commentPos );

	value.Append( _T( "C string"));
	value[1].AddComment( _T( "/* a pointer to static string (C-style)*/" ), commentPos);

	value.Append( 100 );
	value[2].AddComment( _T( "// short integer value=100\n" ), commentPos );

	value.Append( -100 );
	value[3].AddComment( _T( "/* negative short value=-100 (C-style)*/\n" ), commentPos );

	value.Append( 110 );
	value[4].AddComment( _T( "// unsigned short value=110\n\n" ), commentPos );

	value.Append( 100000 );
	value[5].AddComment( _T( "/* long integer value=100.000*/\n\n" ), commentPos );

	value.Append( -100000 );
	value[6].AddComment( _T( "// negative long value=-100.000" ), commentPos );

	value.Append( 110000 );
	value[7].AddComment( _T( "// unsigned long value=110.000" ), commentPos );

	// 2147483647 + 10
	value.Append( (wxInt64) LONG_MAX + 10 );
	value[8].AddComment( _T( "// wxInt64 value=2147483657" ), commentPos );

	// -2147483648 - 10
	value.Append( (wxInt64) LONG_MIN - 10 );
	value[9].AddComment( _T( "// wxInt64 value=-2147483658" ), commentPos );

	// 4294967295 + 110
	value.Append( (wxUint64) ULONG_MAX + 110 );
	value[10].AddComment( _T( "// wxUint64 value=42944867405" ), commentPos );

	value.Append( 90.30 );
	value[11].AddComment( _T( "// double value=90.30" ), commentPos );

	value.Append( true );
	value[12].AddComment( _T( "// boolean value=TRUE" ), commentPos );

	value.Append( false );
	value[13].AddComment( _T( "// boolean value=FALSE" ), commentPos );

	value.Append( wxJSONValue() );
	value[14].AddComment( _T( "// null value=NULL" ), commentPos );

	value.Append( _T( "A\n\tmultiline\n\tstring"));
	value[15].AddComment( _T( "// multiline string LF and TAB" ), commentPos );
}


/****************************************************************************

		FAMILY 5: test wxJSONWriter's flags
		
****************************************************************************/

//
// style wxJSONWRITER_NONE (strict JSON)
// oct: test is succesfull in both ANSI and Unicode 
//
int Test5_1()
{
	// expected result
	static const wxChar* result = _T( "["
		"\"a string\","  // a string object
		"\"C string\","  /* a pointer to static string (C-style)*/
		"100,"           // short integer value=100
		"-100,"          /* negative short value=-100 (C-style)*/
		"110,"           // unsigned short value=110
		"100000,"        // long integer value=100.000
		"-100000,"       // negative long value=-100.000
		"110000,"        // unsigned long value=110.000
		"2147483657,"    // wxInt64 value=2147483657
		"-2147483658,"   // wxInt64 value=-2147483658
		"4294967405,"    // wxUint64 value=42944867405
		"90.3,"          // double value=90.30
		"true,"          // boolean value=TRUE
		"false,"         // boolean value=FALSE
		"null,"          // null value=NULL
		"\"A\\n\\tmultiline\\n\\tstring\""
					// multiline string LF and TAB
		"]");

	int r;
	wxJSONValue v;
	 StoreValue1( v );

	wxString jsonText;
	wxJSONWriter writer( wxJSONWRITER_NONE );
	writer.Write( v, jsonText );

	// now check the difference between the JSON text output and the
	// expected result
	r = CheckResult( jsonText, result );
	return r;
}

//
// style wxJSONWRITER_STYLED (strict JSON using indentation and LF between values)
// oct 2009:
//
int Test5_2()
{
	// expected result
	static const wxChar* result = _T( "[\n"
		"   \"a string\",\n"    // a string object
		"   \"C string\",\n"    /* a pointer to static string (C-style)*/
		"   100,\n"	            // short integer value=100
		"   -100,\n"            /* negative short value=-100 (C-style)*/
		"   110,\n"	            // unsigned short value=110
		"   100000,\n"		// long integer value=100.000
		"   -100000,\n"		// negative long value=-100.000
		"   110000,\n"		// unsigned long value=110.000
		"   2147483657,\n"	// wxInt64 value=2147483657
		"   -2147483658,\n" 	// wxInt64 value=-2147483658
		"   4294967405,\n"	// wxUint64 value=42944867405
		"   90.3,\n"	    // double value=90.30
		"   true,\n"		// boolean value=TRUE
		"   false,\n"		// boolean value=FALSE
		"   null,\n"		// null value=NULL
		"   \"A\\n\\tmultiline\\n\\tstring\"\n"
					// multiline string LF and TAB
		"]\n");

	int r;
	wxJSONValue v;
	StoreValue1( v );

	wxString jsonText;
	wxJSONWriter writer( wxJSONWRITER_STYLED );
	writer.Write( v, jsonText );

	// now check the difference between the JSON text output and the
	// expected result
	r = CheckResult( jsonText, result );
	return r;
}

//
// style wxJSONWRITER_WRITE_COMMENTS (not strict JSON)
// comment are written in the position they apear (INLINE)
// oct 2009: 
//
int Test5_3()
{
  // expected result
	static const wxChar* result = _T( "[\n"
		"   \"a string\",\t// a string object\n"
		"   \"C string\",\t/* a pointer to static string (C-style)*/\n"
		"   100,\t// short integer value=100\n"
		"   -100,\t/* negative short value=-100 (C-style)*/\n"
		"   110,\t// unsigned short value=110\n\n"
		"   100000,\t/* long integer value=100.000*/\n\n"
		"   -100000,\t// negative long value=-100.000\n"
		"   110000,\t// unsigned long value=110.000\n"
		"   2147483657,\t// wxInt64 value=2147483657\n"
		"   -2147483658,\t// wxInt64 value=-2147483658\n"
		"   4294967405,\t// wxUint64 value=42944867405\n"
		"   90.3,\t// double value=90.30\n"
		"   true,\t// boolean value=TRUE\n"
		"   false,\t// boolean value=FALSE\n"
		"   null,\t// null value=NULL\n"
		"   \"A\\n\\tmultiline\\n\\tstring\""
				"\t// multiline string LF and TAB\n"
		"]\n");

	int r;
	wxJSONValue v;
	StoreValue1( v );

	wxString jsonText;
	wxJSONWriter writer( wxJSONWRITER_STYLED | wxJSONWRITER_WRITE_COMMENTS );
	writer.Write( v, jsonText );

	// now check the difference between the JSON text output and the
	// expected result
	r = CheckResult( jsonText, result );
	return r;
}


//
// style wxJSONWRITER_NO_LINEFEEDS (strict JSON)
// note that the wxJSONWIRTER_STYLED flag must also be set
// oct 2009:
//
int Test5_4()
{
	// expected result: LF are suppressed between values but the indentation remains
	static const wxChar* result = _T( "["
		"   \"a string\","  // a string object
		"   \"C string\","  /* a pointer to static string (C-style)*/
		"   100,"           // short integer value=100
		"   -100,"          /* negative short value=-100 (C-style) */
		"   110,"           // unsigned short value=110
		"   100000,"        // long integer value=100.000
		"   -100000,"       // negative long value=-100.000
		"   110000,"        // unsigned long value=110.000
		"   2147483657,"    // wxInt64 value=2147483657
		"   -2147483658,"   // wxInt64 value=-2147483658
		"   4294967405,"    // wxUint64 value=42944867405
		"   90.3,"     // double value=90.30
		"   true,"          // boolean value=TRUE
		"   false,"         // boolean value=FALSE
		"   null,"          // null value=NULL
		"   \"A\\n\\tmultiline\\n\\tstring\""
					// multiline string LF and TAB
		"]");

	int r;
	wxJSONValue v;
	StoreValue1( v );

	wxString jsonText;

	// if you do not specify wxJSONWRITER_STYLED the flag has no effect
	// wxJSONWriter writer( wxJSONWRITER_NO_LINEFEEDS );

	wxJSONWriter writer( wxJSONWRITER_STYLED | wxJSONWRITER_NO_LINEFEEDS );
	writer.Write( v, jsonText );

	r = CheckResult( jsonText, result );
	return r;
}

//
// style wxJSONWRITER_SPLIT_STRING (not strict JSON)
// strings that contains LF are splitted into two or more lines
// oct 2009: test successfull in both ANSI and Unicode
int Test5_5()
{
	// expected result
	static const wxChar* result = _T( "[\n"
		"   \"a string\",\n"	// a string object
		"   \"C string\",\n"	// a pointer to static string
		"   100,\n"		// short integer value=100
		"   -100,\n"		// negative short value=-100
		"   110,\n"		// unsigned short value=110
		"   100000,\n"		// long integer value=100.000
		"   -100000,\n"		// negative long value=-100.000
		"   110000,\n"		// unsigned long value=110.000
		"   2147483657,\n"	// wxInt64 value=2147483657
		"   -2147483658,\n" 	// wxInt64 value=-2147483658
		"   4294967405,\n"	// wxUint64 value=42944867405
		"   90.3,\n"	// double value=90.30
		"   true,\n"		// boolean value=TRUE
		"   false,\n"		// boolean value=FALSE
		"   null,\n"		// null value=NULL
		"   \"A\\n\"\n"
		"         \"\\tmultiline\\n\"\n"
		"         \"\\tstring\"\n"		// multiline string LF and TAB
		"]\n");

	int r;
	wxJSONValue v;
	StoreValue1( v );

	wxString jsonText;
	wxJSONWriter writer( wxJSONWRITER_STYLED | wxJSONWRITER_SPLIT_STRING );
	writer.Write( v, jsonText );

	r = CheckResult( jsonText, result );
	return r;
}


//
// style wxJSONWRITER_MULTILINE_STRING (not strict JSON)
// LFs and TABs are not escaped in string values
// oct 2009: test is succesfull in both ANSI and Unicode 
int Test5_6()
{
	// expected result
	static const wxChar* result = _T( "[\n"
		"   \"a string\",\n"	// a string object
		"   \"C string\",\n"	// a pointer to static string
		"   100,\n"		// short integer value=100
		"   -100,\n"		// negative short value=-100
		"   110,\n"		// unsigned short value=110
		"   100000,\n"		// long integer value=100.000
		"   -100000,\n"		// negative long value=-100.000
		"   110000,\n"		// unsigned long value=110.000
		"   2147483657,\n"	// wxInt64 value=2147483657
		"   -2147483658,\n" 	// wxInt64 value=-2147483658
		"   4294967405,\n"	// wxUint64 value=42944867405
		"   90.3,\n"	// double value=90.30
		"   true,\n"		// boolean value=TRUE
		"   false,\n"		// boolean value=FALSE
		"   null,\n"		// null value=NULL
		"   \"A\n\tmultiline\n\tstring\"\n"
					// multiline string LF and TAB
		"]\n");
	int r;
	wxJSONValue v;
	StoreValue1( v );

	wxString jsonText;
	wxJSONWriter writer( wxJSONWRITER_STYLED | wxJSONWRITER_MULTILINE_STRING );
	writer.Write( v, jsonText );

	r = CheckResult( jsonText, result );
	return r;
}


// check the SPLIT_STRING flag
// string values are splitted in two or more lines if they exceed column 75
// provided that:
//
// - they start at column 50 or less
// - there at least 15 characters left to be written
//
// BUG: the test fails because the writer does no more split long string values.
// This is because in the new 1.1 organization, the column number at which the
// string value is printed is no more counted
int Test5_7()
{
	// expected result
	static const wxChar* result = _T( "[\n"

	"   \"A very long string that cause the wxJSONWriter to split it into several \"\n"
		"         \"lines if wxJSONWRITER_SPLIT_STRING flag is set\",\n"

	"   \"A very long string that contains LF\\n\"\n"
		"         \"so that it will not be splitted because column 75\\n\"\n"
		"         \"is not exceeded\",\n"
//	           1         2         3         4         5         6         7
//       1234 56789.123456789.123456789.123456789.123456789.123456789.123456789.12345
        "   \"A very long string that cause the wxJSONWriter to split it into several lines\",\n"

        "   \"A very long string that cause the wxJSONWriter to split at column 79 where \"\n"
		"         \"a space character is encontered\"\n"
	"]\n" );


	wxJSONValue value;

	// a long line that does not have LF and will be splitted
	value.Append( 
	//    123456789.123456789.123456789.123456789.123456789.231456789.231456789.23145
       	_T("A very long string that cause the wxJSONWriter to split it into several "
		"lines if wxJSONWRITER_SPLIT_STRING flag is set" ));

	// a long line that contains LF and will not be splitted
	value.Append(
	 _T("A very long string that contains LF\n"
		"so that it will not be splitted because column 75\n"
		"is not exceeded"));

	// this line is not splitted because the ramaining length is less than 15
	value.Append(
	//  123456789.123456789.123456789.123456789.123456789.231456789.231456789.23145
	_T("A very long string that cause the wxJSONWriter to split it into several "
		"lines"));

	// a string that will be splitted at column 79 where a space is found
	value.Append(
	//  123456789.123456789.123456789.123456789.123456789.231456789.231456789.231456789
	_T("A very long string that cause the wxJSONWriter to split at column 79 where "
		"a space character is encontered"));

	int r = 0;
	wxJSONWriter writer( wxJSONWRITER_STYLED | wxJSONWRITER_SPLIT_STRING );
	wxString jsonText;
	writer.Write( value, jsonText );

	// now check the difference between the JSON text output and the
	// expected result
	r = CheckResult( jsonText, result );
	return r;
}


//
// test the SPLIT_STRING: the string value is printed at column 51
// so no split string is performed
//
// in order to have the string value at column 51 we store a string
// value at sub-level 4 and set an indentation step of 10 spaces
int Test5_8()
{
	// expected result
	static const wxChar* result = _T( "{\n"
  //          1         2         3         4         5         6         7
  // 123456789.123456789.123456789.123456789.123456789.231456789.231456789.23145
    "          \"key1\" : {\n"
    "                    \"key2\" : {\n"
    "                              \"key3\" : {\n"
    "                                        \"key4-long\" : \"A long string that exceed"
									" column 75\"\n"
    "                              }\n"
    "                    }\n"
    "          }\n"
    "}\n" );

	wxJSONValue value;
	value[_T("key1")][_T("key2")][_T("key3")][_T("key4-long")] =
		_T("A long string that exceed column 75");


	int r = 0;
	wxJSONWriter writer( wxJSONWRITER_STYLED | wxJSONWRITER_SPLIT_STRING, 0, 10 );
	wxString jsonText;
	writer.Write( value, jsonText );

	r = CheckResult( jsonText, result );
	return r;
}

//
// style wxJSONWRITER_STYLED and wxJSONWRITER_TAB_INDENT
// produces strict JSON using indentation using TABs and LF between values
// aug 2008:  
//
int Test5_9()
{
	// expected result
	static const wxChar* result = _T( "[\n"
		"\t\"a string\",\n"    // a string object
		"\t\"C string\",\n"    /* a pointer to static string (C-style)*/
		"\t100,\n"	            // short integer value=100
		"\t-100,\n"            /* negative short value=-100 (C-style)*/
		"\t110,\n"	            // unsigned short value=110
		"\t100000,\n"		// long integer value=100.000
		"\t-100000,\n"		// negative long value=-100.000
		"\t110000,\n"		// unsigned long value=110.000
		"\t2147483657,\n"	// wxInt64 value=2147483657
		"\t-2147483658,\n" 	// wxInt64 value=-2147483658
		"\t4294967405,\n"	// wxUint64 value=42944867405
		"\t90.3,\n"	// double value=90.30
		"\ttrue,\n"		// boolean value=TRUE
		"\tfalse,\n"		// boolean value=FALSE
		"\tnull,\n"		// null value=NULL
		"\t\"A\\n\\tmultiline\\n\\tstring\"\n"
					// multiline string LF and TAB
		"]\n");

	int r;
	wxJSONValue v;
	StoreValue1( v );

	wxString jsonText;
	wxJSONWriter writer( wxJSONWRITER_STYLED | wxJSONWRITER_TAB_INDENT );
	writer.Write( v, jsonText );

	r = CheckResult( jsonText, result );
	return r;
}


/****************************************************************************

		FAMILY 6: test wxJSONWriter: unicode strings
		
****************************************************************************/

// we write an array of values that contains 4 strings:
// [
//	"abcABC",	// us-ascii
//	"àèì©®",	// latin-1
//	"αβγδ",		// greek
//	"ФХЦЧ"		// cyrillic
// ]


// the following is the UTF-8 buffer produced by the JSON writer
static const char utf8Buff[] = {
  0x5B,0x22,0x61,0x62,0x63,0x41,0x42,0x43,
  0x22,0x2C,0x22,0xC3,0xA0,0xC3,0xA8,0xC3,
  0xAC,0xC2,0xA9,0xC2,0xAE,0x22,0x2C,0x22,
  0xCE,0xB1,0xCE,0xB2,0xCE,0xB3,0xCE,0xB4,
  0x22,0x2C,0x22,0xD0,0xA4,0xD0,0xA5,0xD0,
  0xA6,0xD0,0xA7,0x22,0x5D,0x0A
};

/*
  0x22,0x61,0x62,0x63,0x41,0x42,0x43, 0x22,
  0x22,0xC3,0xA0,0xC3,0xA8,0xC3,0xAC,0xC2,0xA9,0xC2,0xAE,0x22
  0x22,0xCE,0xB1,0xCE,0xB2,0xCE,0xB3,0xCE,0xB4,0x22,
  0x22,0xD0,0xA4,0xD0,0xA5,0xD0,0xA6,0xD0,0xA7,0x22,
*/

int Test6_1()
{
	int r;
	wxJSONValue v;
	v.Append( _T("abcABC"));
	v.Append( _T("àèì©®"));
	v.Append( _T("αβγδ"));
	v.Append( _T("ФХЦЧ"));

	wxString jsonText;
	wxJSONWriter writer( wxJSONWRITER_NONE );
	writer.Write( v, jsonText );

	wxString result;
	result = wxString::FromUTF8( utf8Buff );
	r = CheckResult( jsonText, result );
	return r;
}

// same as above but writes to a stream
int Test6_2()
{
	int r;
	wxJSONValue v;
	v.Append( _T("abcABC"));
	v.Append( _T("àèì©®"));
	v.Append( _T("αβγδ"));
	v.Append( _T("ФХЦЧ"));

	wxMemoryOutputStream jsonText;
	wxJSONWriter writer( wxJSONWRITER_NONE );
	writer.Write( v, jsonText );
	

	r = CheckResult( jsonText, utf8Buff );
	return r;
}



/*
{
}
*/

