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
//		*********** test1.cpp ****************
//
// family 1: test the wxJSONValue class, ctors,dtor and memberfunctions
extern int Test1_1();	// test 64-bits support in wxWidgets
extern int Test1_2();	// test wxJSONValue ctors and IsXxxxxx() function
extern int Test1_3();	// test wxJSONValue::AsString()
extern int Test1_4();	// test wxJSONValue::GetInfo() and ::Dump() functions
extern int Test1_5();	// test access methods and other functions

// family 2: test the wxJSONValue::IsSameAs() function
extern int Test2_1();  // comparing fundamental types
extern int Test2_2();  // comparing short and long and Int64
extern int Test2_3();  // comparing INTs and DOUBLEs
extern int Test2_4();  // comparing objects
extern int Test2_5();  // comparing arrays
extern int Test2_6();  // comparing maps, special case: the NULL value
extern int Test2_7();  // comparing incompatible types
extern int Test2_7();  // comparing arrays

//
//		*********** test3.cpp ****************
//
// family #3: test the UTF-8 conversion from / to wxString
extern int Test3_1();  // converting US-ASCII UTF-8 buffer to wxString
extern int Test3_2();  // converting latin, greek and cyrillic UTF-8 to wxString
extern int Test3_3();  // converting a US-ASCII wxString to UTF-8
extern int Test3_4();  // converting a latin-1, greek, cyrillic wxString to UTF-8
extern int Test3_5();  // converting from UTF-8 to wchar_t and to wxString

// family #4: test the wxJSONWriter class
extern int Test4_1();  // wxJSONWriter: a simple write test
extern int Test4_2();  // wxJSONWriter: test escaped characters
extern int Test4_3();  // wxJSONWriter: writes empty, invalid and null objects
extern int Test4_4();  // wxJSONWriter: an array of objects as root
extern int Test4_5();  // wxJSONWriter: how much simple is wxJSON
extern int Test4_6();  // wxJSONWriter: test control characters

//
//		*********** test4.cpp ****************
//
// family #5: test the wxJSONWriter class test various writer's flags
extern int Test5_1();  // writing an array of values using wxJSONWRITER_NONEextern int Test5_2();  // writing an array of values using wxJSONWRITER_STYLED
extern int Test5_3();  // writing an array of values using wxJSONWRITER_WRITE_COMMENTS
extern int Test5_4();  // writing an array of values using wxJSONWRITER_NO_LINEFEEDSextern int Test5_5();  // writing an array of values using wxJSONWRITER_SPLIT_STRING
extern int Test5_6();  // writing an array of values using wxJSONWRITER_MULTILINE_STRING
extern int Test5_7();  // checking the wxJSONWRITER_SPLIT_STRING (very long string)
extern int Test5_8();  // checking the wxJSONWRITER_SPLIT_STRING (value > column 50)extern int Test5_9();   // checking the wxJSONWRITER_TAB_INDENT

// family #6: test the wxJSONWriter class: unicode strings
extern int Test6_1();  // write to wxString  a JSON array using various european charsets
extern int Test6_2();  // write to stream a JSON array using various european charsets

//
//		*********** test5.cpp ****************
//
// family #7: test the wxJSONReader class: input is US-ASCII strings
extern int Test7_1();  // a well-formed array of values without comments
extern int Test7_2();  // a well-formed simple array of values
extern int Test7_3();  // an array of escaped string values
extern int Test7_4();  // nested objects and arrays
extern int Test7_5();  // many errors and warnings
extern int Test7_6();  // read the specified JSON text file
extern int Test7_7();  // missing close object/array on EOF
extern int Test7_8();  // non-JSON text before and after top-level start/end chars
extern int Test7_9();  // missing close array on EOF
//
// family #8: test the wxJSONReader class: strings contains different charsets
extern int Test8_1();  // (Unicode) input from a UTF-8 stream
extern int Test8_2();  // (Unicode) input from a wxString object
extern int Test8_3();  // (ANSI) input from a UTF-8 stream
extern int Test8_4();  // (ANSI) input from a wxString object


//
//		*********** test6.cpp ****************
//
// family #9: test the wxJSON reader and writer speed



//
//		*********** test7.cpp ****************
//
// family #


//
//		*********** test8.cpp ****************
//
// family #

//
//		*********** test9.cpp ****************
//
// family #


//
//		*********** test10.cpp ****************
//
// family #


//
//		*********** test11.cpp ****************
//
// family #


//
//		*********** test12.cpp ****************
//
// family #

//
//		*********** test13.cpp ****************
//
// family #
//
//		*********** test14.cpp ****************
//
// family #

/*

//
// in test6.cpp: test the comment related functions (writer)
extern int Test5_1();
extern int Test5_2();
extern int Test5_3();
extern int Test5_4();
extern int Test5_5();
extern int Test5_6();
extern int Test5_7();

//
// in test7.cpp: test the comment related functions (reader)
extern int Test6_1();
extern int Test6_2();
extern int Test6_3();
extern int Test6_4();
extern int Test6_5();

//
// in test8.cpp: test the unicode stuff
extern int Test7_1();
extern int Test7_2();
extern int Test7_3();
extern int Test7_4();
extern int Test7_5();
extern int Test7_6();



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

//
// in test15.cpp: test the speed of wxJSON reader and writer
extern int Test74();   // writes 10000 values to a wxString object 
extern int Test75();   // writes 10000 values to a stream
extern int Test76();   // read 10000 values from a wxString object
extern int Test77();   // read 10000 values from a stream

*/

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



