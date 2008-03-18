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
//extern int Test47();   // check the old DeepCopy() function (maybe there is a bug)



// global variables in 'main.cpp'
extern int gs_tolerant;
extern int gs_indentation;
extern wxString gs_fileName;

// printf functions in 'main.cpp'
extern void TestCout( const wxChar* str );
extern void TestCout( const wxString& str );
extern void TestCout( int i, bool lf = false );
extern void TestCout( unsigned ui, bool lf = false );
extern void TestCout( double d, bool lf = false );


#endif            // not defined wxJSON_TEST_H


/*
{
}
*/

