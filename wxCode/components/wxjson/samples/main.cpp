/******************************************************************************

   main.cpp

    the 'main' function for the tests
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


 This is a console application used to test the wxJSONValue, wxJSONReader
 and wxJSONWriter classes.

 The results of the tests are ASSERTed in the program flow using wxASSERT
 so you have to link against a debug build of wxWidgets in order to
 actually do the tests.
 A normal termination of this  test application means that the tests were
 successfull.

 Syntax:

   test.bin [OPTIONS] [NUMTEST] [NUMSUBTEST]


 OPTIONS:
	-s		do not stop execution on test's failure
	-l		list test's descriptions
	-f FILENAME	JSON text output will be also written to FILENAME and displayed
	-x FLAG,FLAG..	use the specified flags for the reader and writer
			(not yet implemented)

 Please note that some compilers may fail to compile and link this test
 application, especially on Windows. The test app was written for unix-like
 systems so if you get errors on compiling it does not mean that you
 cannot use the library.
 The wxJSON library is compiled first, so an error in the building process
 of the test application means that the library was corretly complied.


 apr 2008
 --------
 The test application was successfull

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
#include <wx/ffile.h>
#include <wx/cmdline.h>

#include <wx/jsonval.h>
#include <wx/jsonreader.h>
#include <wx/jsonwriter.h>

// includes some defines and macros
#include "test.h"

static bool	gs_nostop = false;
wxString	gs_fileName;
int		gs_flags;
int		gs_list  = false;


static wxFFile* gs_cout = 0;

typedef int (*TestFunc)();

// max 10 tests for each family
#define NUM_SUBTESTS  10
#define NUM_TESTS     15


// the test function's structure
// each test family has MAX_FAMILY_TEST entries of this structure
// if the structure has a NULL pointer has the 'functPtr' member than that
// test's number is not defined
// the FIRST entry of the family has a PTR = ZERO and the 'testDesc' is the
// description of the family.
// This is TEST #0 of the family which does not exist
struct TestStruc
{
	TestFunc	m_funcPtr;	// the pointer to the test function
	int		m_auto;		// 0=no, 1=unicode, 2=ansi, 3=both
	const wxChar*	m_testDesc;	// description of the test
};



int main( int argc, char* argv[] ) 
{
	// the following is mandatory for a console app.
	bool r = ::wxInitialize();
	if ( !r )   {
		printf( "FATAL ERROR: cannot init wxBase library\n");
	return 1;
	}

	// write output to STDOUT
	gs_cout = new wxFFile( stdout );

#if wxCHECK_VERSION( 2, 9, 0 )
    // I got compile errors in wxW 2.9 if the string constants of the wxCmdLineDescEntry
    // class were enclosed in the _T() macro but it seems that wxW 2.8 needs that macro
    // so I conditionally compile this piece of code
	wxCmdLineEntryDesc cmdLineDesc[] =  {
		{ wxCMD_LINE_SWITCH, "s", "nostop", "do not stop on test\'s failure" },
		{ wxCMD_LINE_SWITCH, "l", "list", "list test\'s descriptions" },
		{ wxCMD_LINE_OPTION, "r", "reader", 
			"use the specified flags for the reader", wxCMD_LINE_VAL_STRING },
		{ wxCMD_LINE_OPTION, "w", "writer", 
			"use the specified flags for the writer", wxCMD_LINE_VAL_STRING },
		{ wxCMD_LINE_OPTION, "f", "file", "the filename to be read (test #15)",
						wxCMD_LINE_VAL_STRING },
		{ wxCMD_LINE_PARAM, NULL, NULL, "test [sub-test]", wxCMD_LINE_VAL_NUMBER,
				wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE },
		{ wxCMD_LINE_NONE },
	};
	wxCmdLineParser cmdLine( cmdLineDesc, argc, argv );
	int result = cmdLine.Parse();
	if ( result > 0 )  {
		return 1;        // error
	}
	else if ( result < 0 )  {
		return 1;        // '-h'
	}
	if ( cmdLine.Found( _T("nostop" )))  {
		gs_nostop = true;
	}
	if ( cmdLine.Found( _T("list" )))  {
		gs_list = true;
	}

	wxString flags;
	
	// get a option's argument and store in static variables. This arguments are only usefull for test
	// nr. 2.7 (write to the specified file) and 4.7 (read from the specified file)
	cmdLine.Found( _T("file"), &gs_fileName );
	cmdLine.Found( _T("reader"), &flags );
	cmdLine.Found( _T("writer"), &flags );
#else
	wxCmdLineEntryDesc cmdLineDesc[] =  {
		{ wxCMD_LINE_SWITCH, _T("s"), _T("nostop"), _T("do not stop on test\'s failure") },
		{ wxCMD_LINE_SWITCH, _T("l"), _T("list"), _T("list test\'s descriptions") },
		{ wxCMD_LINE_OPTION, _T("r"), _T("reader"), 
			_T("use the specified flags for the reader"), wxCMD_LINE_VAL_STRING },
		{ wxCMD_LINE_OPTION, _T("w"), _T("writer"), 
			_T("use the specified flags for the writer"), wxCMD_LINE_VAL_STRING },
		{ wxCMD_LINE_OPTION, _T("f"), _T("file"), _T("the filename to be read (test #15)"),
						wxCMD_LINE_VAL_STRING },
		{ wxCMD_LINE_PARAM, NULL, NULL, _T("test [sub-test]"), wxCMD_LINE_VAL_NUMBER,
				wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE },
		{ wxCMD_LINE_NONE },
	};
	wxCmdLineParser cmdLine( cmdLineDesc, argc, argv );
	int result = cmdLine.Parse();
	if ( result > 0 )  {
		return 1;        // error
	}
	else if ( result < 0 )  {
		return 1;        // '-h'
	}
	if ( cmdLine.Found( _T("nostop" )))  {
		gs_nostop = true;
	}
	if ( cmdLine.Found( _T("list" )))  {
		gs_list = true;
	}

	wxString flags;
	
	// get a option's argument and store in static variables. This arguments are only usefull for test
	// nr. 2.7 (write to the specified file) and 4.7 (read from the specified file)
	cmdLine.Found( _T("file"), &gs_fileName );
	cmdLine.Found( _T("reader"), &flags );
	cmdLine.Found( _T("writer"), &flags );
#endif

	TestStruc testArray[] =  {
		// family #0 does not exist
		{
			0,	// m_funcPtr
			false,	// m_auto
			_T( "family #0 does not exist" )	// m_testDesc
		},
		{ 0 },{ 0 },{ 0	},{ 0 },{ 0 },{ 0 },{ 0 },{ 0 },{ 0 },

		// family #1		(test1.cpp)
		{
			0,		// test #0: description of the family
			3,		// m_auto
			_T( "test ctors, dtor and member functions" )
		},
		{
			Test1_1, 3, _T( "test if 64-bits INTs is supported" )
		},
		{
			Test1_2, 3, _T( "test wxJSONValue ctors and JSON value's types" )
		},
		{
			Test1_3, 3, _T( "test the wxJSONValue::AsString() function" )
		},
		{
			Test1_4, 3, _T( "test wxJSONValue::GetInfo() and ::Dump() functions" )
		},
		{
			Test1_5, 3, _T( "test access methods and other functions" )
		},
		{ 0 },{ 0 },{ 0 },{ 0 },
		
		// family #2		(test1.cpp)
		{
			0,		// test #0: description of the family
			3,		// m_auto
			_T( "test the wxJSONValue::ISSameAs() function" )
		},
		{
			Test2_1, 3, _T( "comparing fundamental types" )
		},
		{
			Test2_2, 3, _T( "comparing short, long and long-long" )
		},
		{
			Test2_3, 3, _T( "comparing ints and doubles" )
		},
		{
			Test2_4, 3, _T( "comparing objects" )
		},
		{
			Test2_5, 3, _T( "comparing arrays" )
		},
		{
			Test2_6, 3, _T( "comparing objects: special case of NULL values" )
		},
		{
			Test2_7, 3, _T( "comparing incompatible types" )
		},
		{
			Test2_8, 3, _T( "comparing data structure of positive INT and UINT" )
		},
		{ 0 },
		
		// family #3		(test3.cpp)
		{
			0,		// test #0: description of the family
			1,		// Unicode
			_T( "testing wxString::ToUTF8() and wxString::FromUTF8()" )
		},
		{
			Test3_1, 1, _T( "converting a US-ASCII UTF-8 buffer to wxString" )
		},
		{
			Test3_2, 1, _T( "converting a latin,greek,cyrillic UTF-8 buffer to wxString" )
		},
		{
			Test3_3, 1, _T( "converting a US-ASCII wxString to UTF-8" )
		},
		{
			Test3_4, 1, _T( "converting a latin1.greek,cyrillic wxString to UTF-8" )
		},
		{
			Test3_5, 3, _T( "converting UTF-8 buffer to wchar_t and wxString" )
		},
		{ 0 },{ 0 },{ 0 },{ 0 },

		// family #4		(test3.cpp)
		{
			0,		// test #0: description of the family
			3,		// m_auto
			_T( "testing the wxJSONWriter class" )
		},
		{
			Test4_1, 3, _T( "wxJSONWriter: a simple write test" )
		},
		{
			Test4_2, 3, _T( "wxJSONWriter: test escaped characters" )
		},
		{
			Test4_3, 3, _T( "wxJSONWriter: writes empty, invalid and null objects" )
		},
		{
			Test4_4, 3, _T( "wxJSONWriter: an array of objects as root" )
		},
		{
			Test4_5, 3, _T( "wxJSONWriter: how much simple is wxJSON" )
		},
		{
			Test4_6, 3, _T( "wxJSONWriter: test control characters" )
		},
		{
			Test4_7, 3, _T( "wxJSONWriter: test an array of doubles" )
		},
		{
			Test4_8, 3, _T( "test the printf function with doubles" )
		},
		{ 0 },
		
		// family #5		(test4.cpp)
		{
			0,		// test #0: description of the family
			3,		// m_auto
			_T( "testing the wxJSONWriter class with various flags" )
		},
		{
			Test5_1, 3, _T( "writing an array of values using wxJSONWRITER_NONE" )
		},
		{
			Test5_2, 3, _T( "writing an array of values using wxJSONWRITER_STYLED" )
		},
		{
			Test5_3, 3, _T( "writing an array of values using wxJSONWRITER_WRITE_COMMENTS" )
		},
		{
			Test5_4, 3, _T( "writing an array of values using wxJSONWRITER_NO_LINEFEEDS" )
		},
		{
			Test5_5, 3, _T( "writing an array of values using wxJSONWRITER_SPLIT_STRING" )
		},
		{
			Test5_6, 3, _T( "writing an array of values using wxJSONWRITER_MULTILINE_STRING" )
		},
		{
			// this test fails: see the source code for details
			Test5_7, 0, _T( "checking the wxJSONWRITER_SPLIT_STRING (long strings)" )
		},
		{
			Test5_8, 3, _T( "checking the wxJSONWRITER_SPLIT_STRING (value > column 50" )
		},
		{
			Test5_9, 3, _T( "checking the wxJSONWRITER_TAB_INDENT" )
		},

		// family #6		(test4.cpp)
		{
			0,		// test #0: description of the family
			1,		// m_auto 1=unicode, not applicable in ANSI builds
			_T( "testing the wxJSONWriter class with Unicode" )
		},
		{
			Test6_1, 1, _T( "write to wxString an array of strings from different charsets" )
		},
		{
			Test6_2, 1, _T( "writing to stream an array of strings from different charsets" )
		},
		{ 0 },{ 0 },{ 0 },{ 0 },{ 0 },{ 0 },{ 0 },
		

		// family #7		(test5.cpp)
		{
			0,		// test #0: description of the family
			1,		// m_auto 1=unicode, not applicable in ANSI builds
			_T( "testing the wxJSONReader class (only US-ASCII input)" )
		},
		{
			Test7_1, 3, _T( "a well-formed array of values without comments" )
		},
		{
			Test7_2, 3, _T( "a well-formed simple key/value pairs of values" )
		},
		{
			Test7_3, 3, _T( "an array of escaped string values" )
		},
		{
			Test7_4, 3, _T( "nested objects and arrays" )
		},
		{
			Test7_5, 3, _T( "many errors and warnings" )
		},
		{
			Test7_6, 0, _T( "read the specified JSON text file (need -f option)" )
		},
		{
			Test7_7, 3, _T( "missing close object/array on EOF" )
		},
		{
			Test7_8, 3, _T( "non-JSON text before and after top-level start/end chars" )
		},
		{
			Test7_9, 3, _T( "missing close array character" )
		},


		// family #8		(test5.cpp)
		{
			0,		// test #0: description of the family
			3,		// m_auto
			_T( "testing the wxJSONReader class (strings use different charsets)" )
		},
		{
			Test8_1, 1, _T( "(Unicode) an array of strings read from a UTF-8 stream" )
		},
		{
			Test8_2, 1, _T( "(Unicode) an array of strings read from a wxString" )
		},
		{
			Test8_3, 2, _T( "(ANSI) an array of strings read from a UTF-8 stream" )
		},
		{
			Test8_4, 2, _T( "(ANSI) an array of strings read from a wxString" )
		},
		{
			Test8_5, 2, _T( "(ANSI) read a latin-1 string from UTF-8 buffer" )
		},
		{ 0 },{ 0 },{ 0 },{ 0 },


		// family #9		(test6.cpp)
		{
			0,		// test #0: description of the family
			0,		// m_auto (none)
			_T( "testing the wxJSON reader and writer speed" )
		},
		{
			Test9_1, 3, _T( "writing 40,000 values to a string object" )
		},
		{
			Test9_2, 3, _T( "writing 40,000 values to a string object" )
		},
		{
			Test9_3, 3, _T( "writing 40,000 values to a string object" )
		},
		{
			Test9_4, 3, _T( "writing 40,000 values to a string object" )
		},
		{ 0 },{ 0 },{ 0 },{ 0 },{ 0 },
		
		// family #10		(test7.cpp)
		{
			0,		// test #0: description of the family
			3,		// m_auto (all)
			_T( "test the comment related functions (writer)" )
		},
		{
			Test10_1, 3, _T( "test the wxJSONValue's memberfunctions" )
		},
		{
			Test10_2, 3, _T( "write a commented value using style wxJSONWRITER_STYLED" )
		},
		{
			Test10_3, 3, _T( "write a commented value using style wxJSONWRITER_NONE" )
		},
		{
			Test10_4, 3, _T( "write a commented value using style wxJSONWRITER_WRITE_COMMENTS" )
		},
		{
			Test10_5, 3, _T( "write a commented value using style wxJSONWRITER_COMMENTS_BEFORE" )
		},
		{
			Test10_6, 3, _T( "write a commented value using style wxJSONWRITER_COMMENTS_AFTER" )
		},
		{ 0 },{ 0 },{ 0 },
		

		// family #11		(test8.cpp)
		{
			0,		// test #0: description of the family
			3,		// m_auto (all)
			_T( "test the comment related functions (reader)" )
		},
		{
			Test11_1, 3, _T( "first test without storing comments" )
		},
		{
			Test11_2, 3, _T( "second test storing comments (COMMENTS_BEFORE)" )
		},
		{
			Test11_3, 3, _T( "third test storing comments (COMMENTS_BEFORE)" )
		},
		{
			Test11_4, 3, _T( "first test without comments (COMMENTS_AFTER)" )
		},
		{
			Test11_5, 3, _T( "second test storing comments (COMMENTS_AFTER)" )
		},
		{ 0 },{ 0 },{ 0 },{ 0 },
		
		// family #12		(test9.cpp)
		{
			0,		// test #0: description of the family
			3,		// m_auto (all)
			_T( "test the copy-on-write implementation" )
		},
		{
			Test12_1, 3, _T( "test some copies of the value and unref" )
		},
		{
			Test12_2, 3, _T( "a test used in DDD for getting hardcopy" )
		},
		{
			Test12_3, 3, _T( "copy, write and test of sub-levels" )
		},
		{ 0 },{ 0 },{ 0 },{ 0 },{ 0 },{ 0 },

		
		// family #13		(test10.cpp)
		{
			0,		// test #0: description of the family
			3,		// m_auto (all)
			_T( "test the 64-bit integer support" )
		},
		{
			Test13_1, 3, _T( "test the wxWidgets definitions and macros" )
		},
		{
			Test13_2, 3, _T( "test return values from INT, SHORT, LONG and LLONG" )
		},
		{
			Test13_3, 3, _T( "test the wxString::To(U)LongLong() function" )
		},
		{
			Test13_4, 3, _T( "test the wxString::To(U)Long() function" )
		},
		{
			Test13_5, 3, _T( "test the writer class for various integer types" )
		},
		{
			Test13_6, 3, _T( "test the reader class for 64-bits integers" )
		},
		{
			Test13_7, 3, _T( "test the wxJSONReader::Strto(u)ll function" )
		},
		{ 0 },{ 0 },
		

		// family #14		(test11.cpp)
		{
			0,		// test #0: description of the family
			3,		// m_auto (all)
			_T( "test the use of STL for array and object types" )
		},
		{
			Test14_1, 3, _T( "test the wxWidgets definitions and macros" )
		},
		{
			Test14_2, 3, _T( "test a JSON array type" )
		},
		{
			Test14_3, 3, _T( "test a JSON key/value type" )
		},
		{ 0 },{ 0 },{ 0 },{ 0 },{ 0 },{ 0 },


		// family #15		(test12.cpp)
		{
			0,		// test #0: description of the family
			3,		// m_auto (all)
			_T( "test the new wxJSON type: memory buffer" )
		},
		{
			Test15_1, 3, _T( "testing some wxMemoryBuffer member functions" )
		},
		{
			Test15_2, 3, _T( "testing ctors, copy ctors, assignment op etc" )
		},
		{
			Test15_3, 3, _T( "test the conversion static functions" )
		},
		{
			Test15_4, 3, _T( "test the comparison static functions" )
		},
		{
			Test15_5, 3, _T( "test the writer for memory buffer types" )
		},
		{
			Test15_6, 3, _T( "test the reader for memory buffer types" )
		},
		{ 0 },{ 0 },{ 0 },

		// END OF TABLE
		{0}
		
	};

	int numParams = cmdLine.GetParamCount();
	int numTestStart    = 1;
	int numTestEnd      = NUM_TESTS;
	int numSubTestStart = 1;
	int numSubTestEnd   = NUM_SUBTESTS - 1;
	
	// sets the starting test and sub-test numbers for every parameter
	// parameters are in the form XX [YY]
	wxString p1, p2;
	if ( numParams > 0 )	{
		p1 = cmdLine.GetParam( 0 );	// p1 = test number (the family)
		long l;
		bool r = p1.ToLong( &l );
		if ( r )	{
			numTestStart = numTestEnd = (int) l;
		}
	}
	if ( numParams > 1 )	{
		p2 = cmdLine.GetParam( 1 );	// p2 = subtest numero
		long l;
		bool r = p2.ToLong( &l );
		if ( r )	{
			numSubTestStart = numSubTestEnd = (int) l;
		}
	}
	
	for ( int x = numTestStart; x <= numTestEnd; x++ )  {
		// compute the family index
		int idx = x * NUM_SUBTESTS;
		if ( gs_list )	{
			TestCout( _T("Test number: " ));
		}
		else	{
			TestCout( _T("\nPerforming test number: " ));
		}
		TestCout( x );
		// print the description of the TEST
		TestCout( _T( " - "));
		TestCout( testArray[idx].m_testDesc );
		TestCout( (wxChar) '\n', false );
		
		
		bool runFamily = false;
		#if defined( wxJSON_USE_UNICODE )
				if ( ( testArray[idx].m_auto & 1 ) || !p1.empty() )	{
					runFamily = true;
				}
		#else
				if ( ( testArray[idx].m_auto & 2 ) || !p1.empty() )	{
					runFamily = true;
				}
		#endif

		if ( !runFamily )	{
			continue;
		}
		// subtests	
		for ( int y = numSubTestStart; y <= numSubTestEnd; y++ )  {
			int idx = ( x * NUM_SUBTESTS ) + y;
			TestFunc funcPtr = testArray[idx].m_funcPtr; 
			if ( funcPtr )	{
				if ( gs_list )	{
					TestCout( _T("Subtest #" ));
					TestCout( y );
					// print the description of the TEST
					TestCout( _T( " - "));
					TestCout( testArray[idx].m_testDesc );
					//TestCout( _T( " AUTO=" ));
					//TestCout( ( testArray[idx].m_auto ? _T( "YES" ) : _T( "NO" )));
					TestCout( (wxChar) '\n', false );
					continue;
				}
				else	{
					TestCout( _T("\nPerforming Subtest #" ));
				}
				TestCout( x );
				TestCout( (wxChar) '.', false );
				TestCout( y );
				// print the description of the TEST
				TestCout( _T( "\nDescription: "));
				TestCout( testArray[idx].m_testDesc );
				TestCout( (wxChar) '\n', false );

			
				// the test is actually done if m_auto is TRUE or if 'p2' is not empty
				// depending the m_auto value and the build mode (Unicode,ANSI)
				bool runTest = false;
		#if defined( wxJSON_USE_UNICODE )
				if ( ( testArray[idx].m_auto & 1 ) || !p2.empty() )	{
					runTest = true;
				}
		#else
				if ( ( testArray[idx].m_auto & 2 ) || !p2.empty() )	{
					runTest = true;
				}
		#endif
				if ( runTest )	{
					result = funcPtr();
					TestCout( _T("<-- END TEST: result=" ));
					TestCout( result );
					TestCout( _T(" -->\n" ));
				}
				// if the test fails, stop the program
				if ( result > 0 && !gs_nostop )  {
					return result;
				}
			}
		}
	}
	
	TestCout( _T( "\nTEST application successfully completed\n\n" ));
	::wxUninitialize();
	return 0;
}


// prints the errors and warnings array of the 
void PrintErrors( wxJSONReader& reader )
{
	wxString s;
	int numErrors = reader.GetErrorCount();
	s.Printf( _T( "\nERRORS: count=%d\n"), numErrors );
	TestCout( s );
	const wxArrayString& errors = reader.GetErrors();
	for ( int i = 0; i < errors.size(); i++ )  {
		TestCout( errors[i] );
		TestCout( _T( "\n" ));
	}
	 int numWarn   = reader.GetWarningCount();
	const wxArrayString& warnings = reader.GetWarnings();
	s.Printf( _T("WARNINGS: count=%d\n"), numWarn );
	TestCout( s );
	for ( int i = 0; i < warnings.size(); i++ )  {
		TestCout( warnings[i] );
		TestCout( _T( "\n" ));
	}
}

// prints a JSON value object and the reader's errors
void PrintValue( wxJSONValue& val, wxJSONReader* reader )
{
	wxJSONWriter writer( wxJSONWRITER_STYLED | wxJSONWRITER_WRITE_COMMENTS );
	wxString s;
	writer.Write( val, s );
	TestCout( s );
	if ( reader )  {
		PrintErrors( *reader );
	}
}


void TestCout( wxChar ch, bool lf )
{
  wxString s;
  if ( lf ) {
    s.Printf( _T("%c\n" ), ch);
  }
  else  {
    s.Printf( _T("%c" ), ch);
  }
  TestCout( s );
}

void TestCout( const wxChar* str )
{
  wxString s( str );
  TestCout( s );
}

void TestCout( const wxString& str )
{
  ASSERT( gs_cout );
  gs_cout->Write( str );
}

void TestCout( int i, bool lf )
{
  wxString s;
  if ( lf ) {
    s.Printf( _T("%d\n" ), i);
  }
  else  {
    s.Printf( _T("%d" ), i);
  }
  TestCout( s );
}

void TestCout( unsigned ui, bool lf )
{
  wxString s;
  if ( lf ) {
    s.Printf( _T("%u\n" ), ui);
  }
  else  {
    s.Printf( _T("%u" ), ui);
  }
  TestCout( s );
}

void TestCout( long int l, bool lf )
{
  wxString s;
  if ( lf ) {
    s.Printf( _T("%ld\n" ), l );
  }
  else  {
    s.Printf( _T("%ld" ), l);
  }
  TestCout( s );
}

void TestCout( unsigned long int ul, bool lf )
{
  wxString s;
  if ( lf ) {
    s.Printf( _T("%lu\n" ), ul );
  }
  else  {
    s.Printf( _T("%lu" ), ul);
  }
  TestCout( s );
}

void TestCout( double d, bool lf )
{
  wxString s;
  if ( lf ) {
    s.Printf( _T("%f\n" ), d);
  }
  else  {
    s.Printf( _T("%f" ), d);
  }
  TestCout( s );
}

void TestCout( bool b, bool lf )
{
  wxString s( ( b ? _T("TRUE") : _T("FALSE")));
  if ( lf ) {
    s.Append( '\n', 1 );
  }
  TestCout( s );
}

#if defined( wxJSON_64BIT_INT )
void TestCout( wxInt64 i64, bool lf )
{
  wxString s;
  if ( lf ) {
    s.Printf( _T("%lld\n" ), i64);
  }
  else  {
    s.Printf( _T("%lld" ), i64);
  }
  TestCout( s );
}

void TestCout( wxUint64 ui64, bool lf )
{
  wxString s;
  if ( lf ) {
    s.Printf( _T("%llu\n" ), ui64);
  }
  else  {
    s.Printf( _T("%llu" ), ui64);
  }
  TestCout( s );
}
#endif            // defined( wxJSON_64BIT_INT)


/*
{
}
*/

