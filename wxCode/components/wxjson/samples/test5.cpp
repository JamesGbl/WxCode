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


 This is a console application used to test the wxJSONReader class
 The app is divided in various tests in order to test various
 JSON inputs.
 Comments precede all test functions so that you know what is
 the expected results.
 Also, the test ASSERTs that the number of errors and warnings
 is what we expect.

 Syntax:

   test5.bin [-w] [NUMTEST]

 where NUMTEST is the number of the test to do (all is not specified
 and the '-w' option tells that the parser is contructed with the
 strict flag. In other words, warnings are considered errors.

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
#include <wx/wfstream.h>

#include <wx/jsonval.h>
#include <wx/jsonreader.h>
#include <wx/jsonwriter.h>

#include "test.h"



// test an array of values: text is well-formed with comments
int Test4_1()
{
  static const wxChar* text = 
    _T( "[\n"
	"   \"a string\",\n"
	"   12,\n"
	"   +20,\n"
	"   true,\n"
	"   null,\n"
	"   90.98\n"
	"]\n");

  wxString s( text );
  wxJSONValue root;
  wxJSONReader reader( gs_tolerant );

  int numErrors = reader.Parse( text, &root );
  // now print the JSON value that we have read
  // note the lack of comments
  PrintValue( root, &reader );

  ASSERT( numErrors == 0 )
  int numWarn = reader.GetWarningCount();
  ASSERT( numWarn == 0 )
  return 0;
}

// test a map of values
// 24/7/2008 failed to read the integer!!!
// Warning: lne 2 col: 3 comment is not on col 3 but col. 1
int Test4_2()
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
	"}\n");

  wxString s( text );
  wxJSONValue root;
  wxJSONReader reader( gs_tolerant );

  int numErrors = reader.Parse( text, &root );

  wxInt64 i = root[_T("integer")].AsInt64();
  ASSERT( i == 12 )

  // now print the JSON value that we have read
  // note the lack of comments
  PrintValue( root, &reader );
  ASSERT( numErrors == 0 )
  int numWarn = reader.GetWarningCount();
  ASSERT( numWarn == 2 )
  return 0;
}

// an array of escaped strings and objects with escaped strings
int Test4_3()
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
	"  }\n"
	"]\n");

  wxString s( text );
  wxJSONValue root;

  wxJSONReader reader( gs_tolerant );
  int numErrors = reader.Parse( text, &root );
  PrintValue( root, &reader );

  ASSERT( numErrors == 0 )
  int numWarn = reader.GetWarningCount();
  ASSERT( numWarn == 0 )
  return 0;
}


// nested objects / arrays 
int Test4_4()
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
      "}");

  wxString s( text );
  wxJSONValue root;

  wxJSONReader reader( gs_tolerant );
  int numErrors = reader.Parse( text, &root );
  PrintValue( root, &reader );

  ASSERT( numErrors == 0 )
  int numWarn = reader.GetWarningCount();
  ASSERT( numWarn == 0 )
  return 0;
}

// many errors and warnings
int Test4_5()
{
  static const wxChar* text = 
    // column
    // 0123456789.
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
  wxJSONValue root;

  wxJSONReader reader( gs_tolerant );
  int numErrors = reader.Parse( text, &root );
  PrintValue( root, &reader );

  ASSERT( numErrors == 13 )
  int numWarn = reader.GetWarningCount();
  ASSERT( numWarn == 2 )
  return 0;
}

// a complex JSON structure read from file 'test5.json'
// 1 mar 2008: this test always fails if the JSON text file is
// not in UTF-8 format. The test is deleted because UTF-8
// encoded streams are tested in the 'test10.cpp' file
#if defined( NOT_DEFINED )
int Test4_6()
{
  TestCout( _T( "\nTest #14 was deleted: UTF-8 streams are tested in \'test10.cpp\'\n" ));
  wxString fileName = _T("samples/test5.json");
  wxFileInputStream is( fileName );
  if ( !is.IsOk() )  {
   printf("\nERROR: cannot read input file \'%s\'\n", fileName.c_str() );
   return 1;
  }
  wxJSONValue root;

  wxJSONReader reader( gs_tolerant );
  int numErrors = reader.Parse( is, &root );
  PrintValue( root, &reader );
  ASSERT( numErrors == 0 )

  // there are 17 warnings that are the comment line
  // note that the first C-style comment counts as 1 warning
  // while C++ comments count 1 warning every line even when
  // a comment line immediatly follows another comment line
  int numWarn = reader.GetWarningCount();
  ASSERT( numWarn == 17 )
  return 0;
}
#endif


// read a JSON text file from the specified file
// 1 mar 2008: note that the file must be in UTF-8 format
int Test4_6()
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
  wxJSONReader reader( gs_tolerant );

  int numErrors = reader.Parse( is, &root );
  ASSERT( numErrors == 0 )
  int numWarn = reader.GetWarningCount();
  ASSERT( numWarn == 0 )

  PrintValue( root, &reader );
  return 0;
}

// missing closing object/array on EOF
int Test4_7()
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
  wxJSONValue root;

  wxJSONReader reader( gs_tolerant );
  int numErrors = reader.Parse( s, &root );
  PrintValue( root, &reader );

  ASSERT( numErrors == 0 )
  int numWarn = reader.GetWarningCount();
  ASSERT( numWarn == 3 )
  return 0;
}

// non-JSON text before and after the top-level start/end object
int Test4_8()
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
  wxJSONValue root;

  wxJSONReader reader( gs_tolerant );
  int numErrors = reader.Parse( s, &root );
  PrintValue( root, &reader );

  ASSERT( numErrors == 0 )
  int numWarn = reader.GetWarningCount();
  ASSERT( numWarn == 0 )
  return 0;
}



/*
{
}
*/

