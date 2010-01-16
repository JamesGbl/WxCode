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
extern int Test2_8();  // comparing arrays

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
extern int Test4_7();  // wxJSONWriter: test writing double values
extern int Test4_8();  // test the printf function


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
extern int Test8_5();  // (ANSI) read a latin-1 string


//
//		*********** test6.cpp ****************
//
// family #9: test the wxJSON reader and writer speed
extern int Test9_1();  // write 40.000 values to a wxString object
extern int Test9_2();  // write 40.000 values to a stream object
extern int Test9_3();  // read 40.000 values from a wxString object
extern int Test9_4();  // read 40.000 values from a stream object



//
//		*********** test7.cpp ****************
//
// family #10: test the comment related functions (writer)
extern int Test10_1();  // test the wxJSONValue's memberfunctions
extern int Test10_2();  // write the commented value using style wxJSONWRITER_STYLED
extern int Test10_3();  // write the commented value using style wxJSONWRITER_NONE
extern int Test10_4();  // write the commented value using style wxJSONWRITER_WRITE_COMMENTS
extern int Test10_5();  // write the commented value using style wxJSONWRITER_COMMENTS_BEFORE
extern int Test10_6();  // write the commented value using style wxJSONWRITER_COMMENTS_AFTER


//
//		*********** test8.cpp ****************
//
// family #11: test the comment related functions (reader)
extern int Test11_1();  // first test without storing comments
extern int Test11_2();  // second test storing comments (COMMENTS_BEFORE) 
extern int Test11_3();  // third test storing comments (COMMENTS_BEFORE) 
extern int Test11_4();  // first test without comments (COMMENTS_AFTER)
extern int Test11_5();  // second test storing comments (COMMENTS_AFTER)


//
//		*********** test9.cpp ****************
//
// family #12: wxJSON reference counting feature (was test11.cpp)
extern int Test12_1();  // test some copies of the value and unref
extern int Test12_2();  // a test used in DDD for getting hardcopy
extern int Test12_3();  // copy, write and test of sub-levels



//
//		*********** test10.cpp ****************
//
// family #13: test the 64-bit integer support (was test13.cpp)
extern int Test13_1();  // test the wxWidgets definitions and macros
extern int Test13_2();  // test various return values from INT, SHORT, LONG and LONG LONG
extern int Test13_3();  // test the wxString::To(U)LongLong() function
extern int Test13_4();  // test the wxString::To(U)Long() function
extern int Test13_5();  // test the writer class for various integer types
extern int Test13_6();  // test the reader class for 64-bits integers
extern int Test13_7();  // test the wxJSONReader::Strto(u)ll function



//
//		*********** test11.cpp ****************
//
// family #14: test the use of STL 
extern int Test14_1();  //
extern int Test14_2();  //
extern int Test14_3();  //


//
//		*********** test12.cpp ****************
//
// family #15: test the new JSON type: memory buffer
extern int Test15_1();  // test equal operator of wxMemoryBuffer
extern int Test15_2();  // test ctors, copy ctors, assignment op etc.
extern int Test15_3();  // test the conversion static functions
extern int Test15_4();  // test the comparison static functions
extern int Test15_5();  // test the writer for memory buffer types
extern int Test15_6();  // test the reader for memory buffer types


//
//		*********** test13.cpp ****************
//
// family #


//
//		*********** test14.cpp ****************
//
// family #


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



