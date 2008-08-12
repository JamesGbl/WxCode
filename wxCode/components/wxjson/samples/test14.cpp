/******************************************************************************

   test14.cpp

    test the wxJSONWriter class with several style flags
    Copyright (C) 2008  Luciano Cattani

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


*****************************************************************************/

#include <wx/string.h>
#include <wx/debug.h>
#include <wx/mstream.h>
#include <wx/file.h>
#include <wx/longlong.h>

#include <wx/jsonval.h>
#include <wx/jsonreader.h>
#include <wx/jsonwriter.h>

#include "test.h"



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
  value[2].AddComment( _T( "// short integer value=100" ), commentPos );

  value.Append( -100 );
  value[3].AddComment( _T( "/* negative short value=-100 (C-style)*/" ), commentPos );

  value.Append( 110 );
  value[4].AddComment( _T( "// unsigned short value=110" ), commentPos );

  value.Append( 100000 );
  value[5].AddComment( _T( "// long integer value=100.000" ), commentPos );

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

//
// The function prints the jsonText output and checks if it is
// equal to the expected result: if not it prints the expected
// result and a message.
// Returns ZERO if successfull, 1 otherwaise.
static int
CheckResult( const wxString& jsonText, const wxChar* result )
{
  TestCout( _T( "The JSON text output:\n"));
  TestCout( jsonText );

  TestCout( _T( "\nChecking the result:\n"));
  size_t len = jsonText.length();
  for ( size_t i = 0; i < len; i++ )  {
    wxChar ch = jsonText.at( i );
    TestCout( ch, false );
    if ( ch != result[i] )  {
      TestCout( _T( "\n<---- Unexpected character! Should be: "));
      TestCout( result[i], true );
      return 1;
    }
  }
  return 0;
}



//
// style wxJSONWRITER_NONE (strict JSON)
// aug 2008: test is succesfull in both ANSI and Unicode 
//
int Test65()
{
  // expected result
  static const wxChar* result = _T( "[\n"
		"\"a string\",\n"  // a string object
		"\"C string\",\n"  /* a pointer to static string (C-style)*/
		"100,\n"           // short integer value=100
		"-100,\n"          /* negative short value=-100 (C-style)*/
		"110,\n"           // unsigned short value=110
		"100000,\n"        // long integer value=100.000
		"-100000,\n"       // negative long value=-100.000
		"110000,\n"        // unsigned long value=110.000
		"2147483657,\n"    // wxInt64 value=2147483657
		"-2147483658,\n"   // wxInt64 value=-2147483658
		"4294967405,\n"    // wxUint64 value=42944867405
		"90.300000,\n"     // double value=90.30
		"true,\n"          // boolean value=TRUE
		"false,\n"         // boolean value=FALSE
		"null,\n"          // null value=NULL
		"\"A\\\n\\\tmultiline\\\n\\\tstring\"\n"
					// multiline string LF and TAB
		"]\n");

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
// style wxJSONWRITER_STYLED (strict JSON using indentation)
// aug 2008: test is succesfull in both ANSI and Unicode 
//
int Test66()
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
		"   90.300000,\n"	// double value=90.30
		"   true,\n"		// boolean value=TRUE
		"   false,\n"		// boolean value=FALSE
		"   null,\n"		// null value=NULL
		"   \"A\\\n\\\tmultiline\\\n\\\tstring\"\n"
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
// aug 2008: test is succesfull in both ANSI and Unicode 
//
int Test67()
{
  // expected result
  static const wxChar* result = _T( "[\n"
		"   \"a string\", // a string object\n"
		"   \"C string\", /* a pointer to static string (C-style)*/\n"
		"   100, // short integer value=100\n"
		"   -100, /* negative short value=-100 (C-style)*/\n"
		"   110, // unsigned short value=110\n"
		"   100000, // long integer value=100.000\n"
		"   -100000, // negative long value=-100.000\n"
		"   110000, // unsigned long value=110.000\n"
		"   2147483657, // wxInt64 value=2147483657\n"
		"   -2147483658, // wxInt64 value=-2147483658\n"
		"   4294967405, // wxUint64 value=42944867405\n"
		"   90.300000, // double value=90.30\n"
		"   true, // boolean value=TRUE\n"
		"   false, // boolean value=FALSE\n"
		"   null, // null value=NULL\n"
		"   \"A\\\n\\\tmultiline\\\n\\\tstring\""
				" // multiline string LF and TAB\n"
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
// test failed: the LF in the 'multiline string' is not printed
//
int Test68()
{
  // expected result
  static const wxChar* result = _T( "["
		"\"a string\","  // a string object
		"\"C string\","  /* a pointer to static string (C-style)*/
		"100,"           // short integer value=100
		"-100,"          /* negative short value=-100 (C-style) */
		"110,"           // unsigned short value=110
		"100000,"        // long integer value=100.000
		"-100000,"       // negative long value=-100.000
		"110000,"        // unsigned long value=110.000
		"2147483657,"    // wxInt64 value=2147483657
		"-2147483658,"   // wxInt64 value=-2147483658
		"4294967405,"    // wxUint64 value=42944867405
		"90.300000,"     // double value=90.30
		"true,"          // boolean value=TRUE
		"false,"         // boolean value=FALSE
		"null,"          // null value=NULL
		"\"A\\\n\\\tmultiline\\\n\\\tstring\""
					// multiline string LF and TAB
		"]");

  int r;
  wxJSONValue v;
  StoreValue1( v );

  wxString jsonText;
  wxJSONWriter writer( wxJSONWRITER_NO_LINEFEEDS );
  writer.Write( v, jsonText );

  // now check the difference between the JSON text output and the
  // expected result
  r = CheckResult( jsonText, result );
  return r;
}


//
// style wxJSONWRITER_SPLIT_STRING (not strict JSON)
// strings that contain LF are splitted into two or more lines
// aug 2008: test failed!
int Test69()
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
		"   90.300000,\n"	// double value=90.30
		"   true,\n"		// boolean value=TRUE
		"   false,\n"		// boolean value=FALSE
		"   null,\n"		// null value=NULL
		"   \"A\\\n\\\"\n"
		"         \"\\\tmultiline\\\n\"\n"
		"         \"\\\tstring\"\n"      // multiline string LF and TAB
		"]\n");

  int r;
  wxJSONValue v;
  StoreValue1( v );

  wxString jsonText;
  wxJSONWriter writer( wxJSONWRITER_STYLED | wxJSONWRITER_SPLIT_STRING );
  writer.Write( v, jsonText );

  // now check the difference between the JSON text output and the
  // expected result
  r = CheckResult( jsonText, result );
  return r;
}



//
// style wxJSONWRITER_MULTILINE_STRING (not strict JSON)
// LFs and TABs are not escaped
// aug 2008: test is succesfull in both ANSI and Unicode 
int Test70()
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
		"   90.300000,\n"	// double value=90.30
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

  // now check the difference between the JSON text output and the
  // expected result
  r = CheckResult( jsonText, result );
  return r;
}

/*
{
}
*/


