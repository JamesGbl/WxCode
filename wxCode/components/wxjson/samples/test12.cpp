/******************************************************************************

   test12.cpp

    test for bug fixing in version 0.4.1 (reader and writer)
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

#include <locale.h>

#include <wx/app.h>
#include <wx/string.h>
#include <wx/debug.h>

#include <wx/jsonval.h>
#include <wx/jsonreader.h>
#include <wx/jsonwriter.h>
#include "test.h"



// prints the errors and warnings array
static void PrintErrors( wxJSONReader& reader )
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


static void PrintValue( wxJSONValue& val )
{
  wxJSONWriter writer;
  wxString s;
  writer.Write( val, s );
  TestCout( s );
}

//
// testing the wxJSONReader::AddWarning() function which should
// report errors when the individual wxJSON extension is turned off
// 12 apr 2008: OK, test is successfull
int Test52()
{
  static const wxChar* text = 
   _T("{\n"
	"   // comment line for \'key-1\'\n"        // (2) comment line
	"   \"key-1\" : \"value-1\",\n"             // (3)

	"   \"key-2\" : NuLL,\n"                    // (4) mixed case literal

	"   \"key-3\" : \"multi-line string\n\"\n"  // (5) multiline string
	"               \"line-2\n\"\n"             // these count as 2 warnings
	"               \"line-3\n\"\n"

	//  "}\n"                                   // (8) final '}' is missing
      "\n");
 
  wxString jsonText( text );
  int numErrors, numWarns;
  wxJSONValue root;

  // all wxJSON extensions are ON
  TestCout( _T("\nParser flag is wxJSONREADER_TOLERANT\n"));
  wxJSONReader reader1( wxJSONREADER_TOLERANT );
  numErrors = reader1.Parse( jsonText, &root );
  PrintErrors( reader1 );
  PrintValue( root );
  ASSERT( numErrors == 0 )
  numWarns  = reader1.GetWarningCount();
  ASSERT( numWarns == 5 )

  // only comments are allowed
  TestCout( _T("\nParser flag is wxJSONREADER_ALLOW_COMMENTS\n"));
  wxJSONReader reader2( wxJSONREADER_ALLOW_COMMENTS );
  numErrors = reader2.Parse( jsonText, &root );
  PrintErrors( reader2 );
  PrintValue( root );
  ASSERT( numErrors == 4 )
  numWarns  = reader2.GetWarningCount();
  ASSERT( numWarns == 1 )

  // only mixed case literals are allowed
  TestCout( _T("\nParser flag is wxJSONREADER_CASE\n"));
  wxJSONReader reader3( wxJSONREADER_CASE );
  numErrors = reader3.Parse( jsonText, &root );
  PrintErrors( reader3 );
  PrintValue( root );
  ASSERT( numErrors == 4 )
  numWarns  = reader3.GetWarningCount();
  ASSERT( numWarns == 1 )

  // only multiline strings areallowed
  TestCout( _T("\nParser flag is wxJSONREADER_MULTISTRING\n"));
  wxJSONReader reader4( wxJSONREADER_MULTISTRING );
  numErrors = reader4.Parse( jsonText, &root );
  PrintErrors( reader4 );
  PrintValue( root );
  ASSERT( numErrors == 3 )
  numWarns  = reader4.GetWarningCount();
  ASSERT( numWarns == 2 )

  // only wrong/missing close array/object char is allowed
  TestCout( _T("\nParser flag is wxJSONREADER_MISSING\n"));
  wxJSONReader reader5( wxJSONREADER_MISSING );
  numErrors = reader5.Parse( jsonText, &root );
  PrintErrors( reader5 );
  PrintValue( root );
  ASSERT( numErrors == 4 )
  numWarns  = reader5.GetWarningCount();
  ASSERT( numWarns == 1 )

  return 0;
}


/*
{
}
*/





