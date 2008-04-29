/******************************************************************************

   test13.cpp

    test the 64-bit integer
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
// test the wxWidgets definitions and macros
int Test54()
{
  TestCout( _T("Test if \'wxLongLong_t\' is defined: "));
  #if defined( wxLongLong_t )
    TestCout( _T("YES\n" ));
  #else
    TestCout( _T("NO\n" ));
  #endif

  TestCout( _T("Test if \'wxLongLongFmtSpec\' is defined: "));
  #if defined( wxLongLongFmtSpec )
    TestCout( _T("YES: "));
    TestCout( wxLongLongFmtSpec);
    TestCout( _T("\n"));
  #else
    TestCout( _T("NO\n" ));
  #endif

  // wxItn64 is a typedef on 64-bit platforms, not a #define
  TestCout( _T("Test if \'wxInt64\' is defined: "));
  #if defined( wxInt64 )
    TestCout( _T("YES\n"));
  #else
    TestCout( _T("NO\n" ));
  #endif

  // the if wxJSON_64BIT_INT is defined
  TestCout( _T("Test if \'wxJSON_64BIT_INT\' is defined: "));
  #if defined( wxJSON_64BIT_INT )
    TestCout( _T("YES\n"));
  #else
    TestCout( _T("NO\n" ));
  #endif

  TestCout( _T("Test if \'wxBYTE_ORDER\' is defined: "));
  #if defined( wxBYTE_ORDER )
    TestCout( _T("YES\n"));
  #else
    TestCout( _T("NO\n" ));
  #endif

  TestCout( _T("Test the value of wxBYTE_ORDER: "));
  #if ( wxBYTE_ORDER == wxLITTLE_ENDIAN )
    TestCout( _T("wxLITTLE_ENDIAN\n"));
  #endif
  #if ( wxBYTE_ORDER == wxBIG_ENDIAN )
    TestCout( _T("wxBIG_ENDIAN\n "));
  #endif


  // test the integer constants
  TestCout( _T("Printing INT_MAX: "));
  TestCout( INT_MAX, true );
  TestCout( _T("Printing UINT_MAX: "));
  TestCout( UINT_MAX, true );
  TestCout( _T("Printing INT_MIN: "));
  TestCout( INT_MIN, true );
  // TestCout( _T("Printing UINT_MIN: "));  not defined: it is always ZERO
  // TestCout( UINT_MIN, true );

  TestCout( _T("Printing LLONG_MAX: "));
  TestCout( (wxInt64) LLONG_MAX, true );
  TestCout( _T("Printing LLONG_MIN: "));
  TestCout( (wxInt64) LLONG_MIN, true );
  TestCout( _T("Printing ULLONG_MAX: "));
  TestCout( (wxUint64) ULLONG_MAX, true );

  return 0;
}


// test the wxJSON's 64-bits related functions
// the test is only done on 64bits platforms
int Test55()
{
  // the test result
  int r = 0;

#if defined( wxJSON_64BIT_INT )
  // we use an array of integers and check the results of the
  // memberfunctions. We set an integer value to every element
  wxJSONValue value;
  value.Append( 100 ); 
  value.Append( (unsigned) 110 );
  value.Append( -1 ); 
  value.Append( (unsigned) -1 ); 
  value.Append( -65000 ); 
  value.Append( (wxInt64) INT_MAX + 10 );
  value.Append( (wxInt64) INT_MIN - 10 );
  value.Append( (wxUint64) UINT_MAX + 10 );
  value.Append( (wxInt64) UINT_MAX + 10 );

  // the memberfunction that we check are:
  //
  //   GetType()
  //   IsInt()
  //   IsUInt()
  //   IsInt32()
  //   IsUInt32()
  //   IsInt64()
  //   IsUInt64()
  //   AsInt()
  //   AsUInt()
  //   AsInt32()
  //   AsUInt32()
  //   AsInt64()
  //   AsUInt64()

  // the expected results for every element are stored in an array
  // of structures: for every function there is a result.
  // the first field contains wxJSONTYPE_INT or wxJSONTYPE_UINT
  // if the first field contains wxJSONTYPE_EMPTY then we get ab
  // ASSERTION failure if we call the function 
  struct Result {
  wxJSONType iType;
    bool     isInt;
    bool     isUInt;
    bool     isInt32;
    bool     isUInt32;
    bool     isInt64;
    bool     isUInt64;
    int      asInt;
    unsigned asUInt;
    int      asInt32;
    unsigned asUInt32;
    wxInt64  asInt64;
    wxUint64 asUInt64;
  };

  Result res[] =  {
	// element 0: a positive, signed 32-bit integer
	{
		wxJSONTYPE_INT,	// iType,
		true,		// isInt,
		false,		// isUInt,
		true,		// isInt32,
		false,		// isUInt32,
		false,		// isInt64,
		false,		// isUInt64,
		100,		// asInt,
		100,		// asUInt,
		100,		// asInt32,
		100,		// asUInt32,
		100,		// asInt64,
		100		// asUInt64
	}

  };

  for ( int i = 0; i < 1; i++ )  {
    TestCout( _T("Checking element no. "));
    TestCout( i, true );
    TestCout( _T("Value is: "));
    TestCout( value[i].AsString() );
    TestCout( _T("\n"));

    TestCout( _T("Checking GetType(): "));
    wxJSONType type = value[i].GetType();
    TestCout( wxJSONValue::TypeToString( type));
    TestCout( _T("\n"));
    ASSERT( type == res[i].iType );

    bool r;     // checking the IsXxxxxx() functions
    TestCout( _T("Checking IsInt(): "));
    r = value[i].IsInt();
    TestCout( r, true );
    ASSERT( r == res[i].isInt );
    TestCout( _T("Checking IsUInt(): "));
    r = value[i].IsUInt();
    TestCout( r, true );
    ASSERT( r == res[i].isUInt );
    TestCout( _T("Checking IsInt32(): "));
    r = value[i].IsInt32();
    TestCout( r, true );
    ASSERT( r == res[i].isInt32 );
    TestCout( _T("Checking IsUInt32(): "));
    r = value[i].IsUInt32();
    TestCout( r, true );
    ASSERT( r == res[i].isUInt32 );
    TestCout( _T("Checking IsInt64(): "));
    r = value[i].IsInt64();
    TestCout( r, true );
    ASSERT( r == res[i].isInt64 );
    TestCout( _T("Checking IsUInt64(): "));
    r = value[i].IsUInt64();
    TestCout( r, true );
    ASSERT( r == res[i].isUInt64 );

    int d; unsigned int ui;   // checking the AsXxxxxx() functions
    TestCout( _T("Checking AsInt(): "));
    d = value[i].AsInt();
    TestCout( d, true );
    ASSERT( d == res[i].asInt )
    TestCout( _T("Checking AsUInt(): "));
    ui = value[i].AsUInt();
    TestCout( ui, true );
    ASSERT( ui == res[i].asUInt )
    TestCout( _T("Checking AsInt32(): "));
    d = value[i].AsInt32();
    TestCout( d, true );
    ASSERT( d == res[i].asInt32 )
    TestCout( _T("Checking AsUInt32(): "));
    ui = value[i].AsUInt32();
    TestCout( ui, true );
    ASSERT( ui == res[i].asUInt32 )

    wxInt64 i64; wxUint64 ui64;
    TestCout( _T("Checking AsInt64(): "));
    i64 = value[i].AsInt64();
    TestCout( i64, true );
    ASSERT( i64 == res[i].asInt64 )
    TestCout( _T("Checking AsUInt64(): "));
    ui64 = value[i].AsUInt64();
    TestCout( ui64, true );
    ASSERT( ui64 == res[i].asUInt64 )
  }


#endif    // defined ( wxJSON_64BIT_INT )

  return r;
}


/*
{
}
*/





