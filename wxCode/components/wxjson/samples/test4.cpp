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
#include <wx/wfstream.h>

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

		now starts the test functions
		
****************************************************************************/


int TestX_Y() 
{
	return 0;
}


/*
{
}
*/

