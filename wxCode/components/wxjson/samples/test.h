/******************************************************************************

   test.h

    macros and defines for test application
    Copyleft (C) 2007  Luciano Cattani

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

#if !defined( wxJSON_TEST_H )
#define wxJSON_TEST_H

#define ASSERT( cond )  wxASSERT( cond );


#include <wx/jsonreader.h>


// external functions: each source file 'testX.cpp is specialized
// for testing a specific feature or class.
// the following is a list of the performed tests:

//
// in test1.cpp: test the wxJSONValue class
extern int Test1();  // test ctors and member functions of wxJSONValue

//
// in test2.cpp:
// this file was deleted

//
// in test3.cpp: test the wxJSONWriter class
extern int Test2();  // a simple write test
extern int Test3();  // test escaped sequences
extern int Test4();  // print "null", "empty", "empty map" and "empty array"
extern int Test5();  // test an array of values
extern int Test6();  // a complex JSON value 
extern int Test7();  // test the simplicity of JSON format (see docs)


//
// in test4.cpp: test the wxJSONValue::IsSameAs() function
extern int Test8();  // test various JSON objects that look like the same

//
// in test5.cpp: test the wxJSONReader class (input from string)
extern int Test9();   // a well-formed array of values without comments
extern int Test10();  // a well-formed simple array of values
extern int Test11();  // an array of escaped string values
extern int Test12();  // nested objects and arrays
extern int Test13();  // many errors and warnings
extern int Test14();  // a complex JSON structure read from test5.json
extern int Test15();  // read the specified JSON text file
extern int Test16();  // missing close object/array on EOF
extern int Test17();  // non-JSON text before and after top-level start/end chars

//
// in test6.cpp: test the comment related functions (writer)
extern int Test18();  // test wxJSONValue::AddComment() function
extern int Test19();  // write comments using style= STYLED
extern int Test20();  // write comments using style= NONE
extern int Test21();  // write comments using style= WRITE_COMMENTS
extern int Test22();  // write comments using style= WRITE_COMMENTS, COMMENT_BEFORE
extern int Test23();  // write comments using style= WRITE_COMMENTS, COMMENT_AFTER
extern int Test24();  // test the MULTISTRING feature

//
// in test7.cpp: test the comment related functions (reader)
extern int Test25();  // read a JSON text with comments style=TOLERANT
extern int Test26();  // read a JSON text with comments style=TOLERANT, STORE_COMMENTS
extern int Test27();  // same as 25 but without syntax error
extern int Test28();  // read a JSON text with comments COMMENT_AFTER (not store)
extern int Test29();  // same as 28 but storing comments

//
// in test8.cpp: test the unicode stuff
extern int Test30();  // test the installed encodings in the wxCSConv classes
extern int Test31();  // test the wxJSON_USE_UNICODE macro definition
extern int Test32();  // convert wide char string to other encodings (all)
extern int Test33();  // convert wide char string to other encodings (latin-1)
extern int Test34();  // convert wide char string to other encodings (greek)
extern int Test35();  // convert wide char string to other encodings (cyrillic)

//
// in test9.cpp: the JSON writer in unicode mode
extern int Test36();   // convert a ISO-8859-1 string to UTF-8 format
extern int Test37();   // writes to a UTF-8 stream (Unicode)
extern int Test38();   // writes to a string and convert to UCS-2BE (Unicode)
extern int Test39();   // writes to a string and convert to UCS-4BE (Unicode)
extern int Test40();   // writes to a string and convert to ISO-8859-1 (Unicode)
extern int Test41();   // writes to a stream in UTF-8 (Unicode)
extern int Test42();   // writes to a stream in UTF-8 (ANSI)
extern int Test43();   // another version of Test38() (Unicode)
extern int Test44();   // test only the wxString- conversion functions (Unicode)

//
// in test10.cpp: wxJSON reader in Unicode/ANSI builds (input from streams)
extern int Test45();   // read a UTF-8 stream containing latin-1, greek and cyrillic (Uni)
extern int Test46();   // read a UTF-8 stream containing latin-1, greek and cyrillic (ANSI)

//
// in test11.cpp: wxJSON reference counting feature
extern int Test47();   // check the old DeepCopy() function (maybe there is a bug)
extern int Test48();   // check the new COW functions: nested objects
extern int Test49();   // check the new COW functions: a simple 4-items array
extern int Test50();   // check the new COW functions: several level-2 values
extern int Test51();   // check the new COW functions: read a copy of a value

//
// in test12.cpp: bug-fixing in 0.4.1 (reader and writer)
extern int Test52();   // testing the wxJSONReader::AddWarning() function
extern int Test53();   // testing errors in writing to streams

//
// in test13.cpp: test the 64-bit integer support
extern int Test54();   // testing wxWidget's macro definitions
extern int Test55();   // testing a custom version of the wxASSERT macro
extern int Test56();   // testing the wxJSON_NOABORT_ASSERT macro
extern int Test57();   // testing the wxJSONReader's integer related functions
extern int Test58();   // testing the wxString::To(U)LongLong() functions
extern int Test59();   // testing the wxJSONReader::Strtoll() functions
extern int Test60();   // testing the wxJSONWriter::WriteChar() function
extern int Test61();   // testing the wxString::To(U)Long functions
extern int Test62();   // testing the wxJSONValue::AsString() function for 64-bits int
extern int Test63();   // testing the wxJSONWriter for various integer types
extern int Test64();   // testing the wxJSONReader for various integer types

//
// in test14.cpp: test the wxJSONWriter class with various flags
extern int Test65();   // writing an array of values using wxJSONWRITER_NONE
extern int Test66();   // writing an array of values using wxJSONWRITER_STYLED
extern int Test67();   // writing an array of values using wxJSONWRITER_WRITE_COMMENTS
extern int Test68();   // writing an array of values using wxJSONWRITER_NO_LINEFEEDS
extern int Test69();   // writing an array of values using wxJSONWRITER_SPLIT_STRING
extern int Test70();   // writing an array of values using wxJSONWRITER_MULTILINE_STRING
extern int Test71();   // checking the wxJSONWRITER_SPLIT_STRING
extern int Test72();   // checking the wxJSONWRITER_SPLIT_STRING
extern int Test73();   // checking the wxJSONWRITER_TAB_INDENT


// global variables in 'main.cpp'
extern int gs_tolerant;
extern int gs_indentation;
extern wxString gs_fileName;

// printf functions in 'main.cpp'
extern void TestCout( const wxChar* str );
extern void TestCout( wxChar ch, bool lf = true );
extern void TestCout( const wxString& str );
extern void TestCout( int i, bool lf = false );
extern void TestCout( bool b, bool lf = false );
extern void TestCout( unsigned ui, bool lf = false );
extern void TestCout( long int l, bool lf = false );
extern void TestCout( unsigned long int ul, bool lf = false );
extern void TestCout( double d, bool lf = false );

// functions for printing values and parser error array
extern void PrintErrors( wxJSONReader& reader );
extern void PrintValue( wxJSONValue& val, wxJSONReader* reader = 0 );


#if defined( wxJSON_64BIT_INT )
extern void TestCout( wxInt64 i64, bool lf = false );
extern void TestCout( wxUint64 ui64, bool lf = false );
#endif

#endif            // not defined wxJSON_TEST_H


/*
{
}
*/



