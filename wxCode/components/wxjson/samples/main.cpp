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

   test.bin [OPTIONS] NUMTEST

 where NUMTEST is the test number that has to be perfomed (ZERO=all)

 OPTIONS:

   -t     the wxJSONReader is not tolerant
   -s     suppress indentation for wxJSONWriter
   -f FILENAME the filename to read by the wxJSONReader (only test #15)


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


int      gs_tolerant = wxJSONREADER_TOLERANT;
int      gs_indentation = wxJSONWRITER_STYLED;
wxString gs_fileName;

static wxFFile* gs_cout = 0;

typedef int (*TestFunc)();


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

  wxCmdLineEntryDesc cmdLineDesc[] =  {
	{ wxCMD_LINE_SWITCH, _T("t"), _T("not-tolerant"), _T("the parser is not tolerant") },
	{ wxCMD_LINE_SWITCH, _T("s"), _T("no-indentation"), 
					_T("suppress indentation when writing") },
	{ wxCMD_LINE_OPTION, _T("f"), _T("file"), _T("the filename to be read (test #15)"),
						wxCMD_LINE_VAL_STRING },
	
	{ wxCMD_LINE_PARAM, NULL, NULL, _T("test number (1-17)"), wxCMD_LINE_VAL_NUMBER,
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

  if ( cmdLine.Found( _T("not-tolerant" )))  {
    gs_tolerant = wxJSONREADER_STRICT;
  }
  if ( cmdLine.Found( _T("no-indentation" )))  {
    gs_indentation = wxJSONWRITER_NONE;
  }

  // get the filename is -f is specified
  cmdLine.Found( _T("file"), &gs_fileName );

  TestFunc testFunc[] =  {
	Test1,
	Test2, Test3, Test4, Test5, Test6, Test7, 
	Test8,
	Test9, Test10, Test11, Test12, Test13, Test14, Test15, Test16, Test17,
	Test18, Test19, Test20, Test21, Test22, Test23, Test24,
	Test25, Test26, Test27, Test28, Test29,
	Test30, Test31, Test32, Test33, Test34, Test35, 
	Test36, Test37, Test38, Test39, Test40, Test41, Test42, Test43, Test44,
	Test45, Test46,
	Test47, Test48, Test49, Test50, Test51,
	Test52, Test53,
	Test54, Test55, Test56, Test57,
	0
  };

#define TOTAL_TESTS 57

  int numParams = cmdLine.GetParamCount();
  if ( numParams == 0 )  {
    for ( int i = 0; i < TOTAL_TESTS; i++ )  {
      // test #15 is not performed because it needs the -f option
      if ( i == 14 && gs_fileName.empty() )  {
        continue;
      }
      TestCout( _T("----------------------------\nPerforming test number: " ));
      TestCout( (int) i + 1 );
      TestCout( _T("\n----------------------------\n" ));
      result = testFunc[i]();
      TestCout( _T("----------------------------\nEND TEST number: " ));
      TestCout( (int) i + 1 );
      TestCout( _T("\n----------------------------\n" ));
      if ( result > 0 )  {
        return result;
      }
    }
  }

  else {
    for ( int i = 0; i < numParams; i++ )  {
      wxString numTestStr = cmdLine.GetParam( i );
      long int numTest;
      r = numTestStr.ToLong( &numTest );
      if ( !r )  {
        TestCout( _T("Invalid test number: " ));
        TestCout( numTestStr );
        TestCout( _T("\n" ));
      }
      else if ( numTest <= 0 || numTest > TOTAL_TESTS ) {
        TestCout( _T("Invalid test number: " ));
        TestCout( numTestStr );
        TestCout( _T("\n" ));
      }
      else {
        TestCout( _T("Performing test number: " ));
        TestCout( (int) numTest, true );
        result = testFunc[numTest - 1]();
        if ( result > 0 )  {
          TestCout( _T( "\nTEST application failed\n\n" ));
          return result;
        }
      }
    }
  }

  TestCout( _T( "\nTEST application successfully completed\n\n" ));
  ::wxUninitialize();
  return 0;
}


/*
{
}
*/

