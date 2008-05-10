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
  TestCout( (int) INT_MAX, true );
  TestCout( _T("Printing UINT_MAX: "));
  TestCout( (unsigned int) UINT_MAX, true );
  TestCout( _T("Printing INT_MIN: "));
  TestCout( (int) INT_MIN, true );
  // TestCout( _T("Printing UINT_MIN: "));  not defined: it is always ZERO
  // TestCout( UINT_MIN, true );

  #if defined( wxJSON_64BIT_INT )
    TestCout( _T("Printing LLONG_MAX: "));
    TestCout( (wxInt64) LLONG_MAX, true );
    TestCout( _T("Printing LLONG_MIN: "));
    TestCout( (wxInt64) LLONG_MIN, true );
    TestCout( _T("Printing ULLONG_MAX: "));
    TestCout( (wxUint64) ULLONG_MAX, true );
  #endif
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
  value.Append( _T("A string"));

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
	// element 0: a positive, signed 32-bit integer, value=100
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
	},

	// element 1: a positive, unsigned 32-bit integer, value=110
	{
		wxJSONTYPE_UINT,	// iType,
		false,			// isInt,
		true,			// isUInt,
		false,			// isInt32,
		true,			// isUInt32,
		false,			// isInt64,
		false,			// isUInt64,
		110,			// asInt,
		110,			// asUInt,
		110,			// asInt32,
		110,			// asUInt32,
		110,			// asInt64,
		110			// asUInt64
	},

	// element 2: a negative, signed 32-bit integer, value=-1
	{
		wxJSONTYPE_INT,		// iType,
		true,			// isInt,
		false,			// isUInt,
		true,			// isInt32,
		false,			// isUInt32,
		false,			// isInt64,
		false,			// isUInt64,
		-1,			// asInt,
		0,			// asUInt,  ASSERTION failure: signed value
		-1,			// asInt32,
		UINT_MAX,		// asUInt32,
		-1,			// asInt64,
		ULLONG_MAX		// asUInt64
	},

	// element 3: a unsigned integer, value= (unsigned) -1
	{
		wxJSONTYPE_UINT,	// iType,
		false,			// isInt,
		true,			// isUInt,
		false,			// isInt32,
		true,			// isUInt32,
		false,			// isInt64,
		false,			// isUInt64,
		0,			// asInt,   ASSERTION failure: too large for signed 32-bit
		UINT_MAX,		// asUInt, 
		-1,			// asInt32,
		UINT_MAX,		// asUInt32,
		UINT_MAX,		// asInt64,
		UINT_MAX		// asUInt64
	},

	// element 4: a negative, signed integer, value=-65000
	{
		wxJSONTYPE_INT,		// iType,
		true,			// isInt,
		false,			// isUInt,
		true,			// isInt32,
		false,			// isUInt32,
		false,			// isInt64,
		false,			// isUInt64,
		-65000,			// asInt,
		0,			// asUInt,   ASSERTION failure: signed int. 
		-65000,			// asInt32,
		-65000,			// asUInt32, WRONG RESULT
		-65000,			// asInt64,
		-65000			// asUInt64  WRONG RESULT
	},

	// element 5: a positive, signed integer, value=INT_MAX + 10
	{
		wxJSONTYPE_INT64,	// iType,
		true,			// isInt,
		false,			// isUInt,
		false,			// isInt32,
		false,			// isUInt32,
		true,			// isInt64,
		false,			// isUInt64,
		0, 			// asInt,   ASSERTION failure: value too large
		INT_MAX + 10,		// asUInt 
		-2147483639,		// asInt32, WRONG RESULT: too big for 'int32'
		INT_MAX + 10,		// asUInt32
		(wxInt64) INT_MAX + 10,	// asInt64,
		(wxUint64)INT_MAX + 10	// asUInt64
	},

	// element 6: a negative, signed integer, value=INT_MIN - 10 (-2147483658)
	{
		wxJSONTYPE_INT64,	// iType,
		true,			// isInt,
		false,			// isUInt,
		false,			// isInt32,
		false,			// isUInt32,
		true,			// isInt64,
		false,			// isUInt64,
		0,			// asInt,  ASSERTION failure: value cannot fit( too large)
		0,			// asUInt  ASSERTION failure: value cannot fit (negative)
		2147483638,		// asInt32, WRONG RESULT:
		2147483638,		// asUInt32 WRONG RESULT
		(wxInt64) INT_MIN - 10,	// asInt64,
		(wxUint64)INT_MIN - 10	// asUInt64 WRONG RESULT: returned as a unsigned int64
	},

	// element 7: a positive, unsigned integer, value=UINT_MAX + 10 (4294967305)
	{
		wxJSONTYPE_UINT64,	// iType,
		false,			// isInt,
		true,			// isUInt,
		false,			// isInt32,
		false,			// isUInt32,
		false,			// isInt64,
		true,			// isUInt64,
		0,			// asInt,  ASSERTION failure: value cannot fit (too large)
		0,			// asUInt  ASSERTION failure: value cannot fit (too large)
		9,			// asInt32, WRONG RESULT:
		9,			// asUInt32 WRONG RESULT
		(wxInt64) UINT_MAX + 10,  // asInt64,
		(wxUint64)UINT_MAX + 10	  // asUInt64
	},

	// element 8: a positive, signed integer, value=UINT_MAX + 10 (4294967305)
	{
		wxJSONTYPE_INT64,	// iType,
		true,			// isInt,
		false,			// isUInt,
		false,			// isInt32,
		false,			// isUInt32,
		true,			// isInt64,
		false,			// isUInt64,
		0,			// asInt,  ASSERTION failure: value cannot fit (too large)
		0,			// asUInt  ASSERTION failure: value cannot fit (too large)
		9,			// asInt32, WRONG RESULT:
		9,			// asUInt32 WRONG RESULT
		(wxInt64) UINT_MAX + 10,  // asInt64,
		(wxUint64)UINT_MAX + 10	  // asUInt64
	},

	// element 9: a string, value="A string"
	{
		wxJSONTYPE_STRING,	// iType,
		false,			// isInt,
		false,			// isUInt,
		false,			// isInt32,
		false,			// isUInt32,
		false,			// isInt64,
		false,			// isUInt64,
		0,			// asInt,   ASSERTION failure: type not compatible
		0,			// asUInt   ASSERTION failure: type not compatible
		0,			// asInt32, ASSERTION failure: type not compatible
		0,			// asUInt32 ASSERTION failure: type not compatible
		0,			// asInt64, ASSERTION failure: type not compatible
		0			// asUInt64 ASSERTION failure: type not compatible
	}

  };

  for ( int i = 0; i < 10; i++ )  {
    TestCout( _T("\nChecking element no. "));
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
    if ( res[i].asInt != 0 )  {
      d = value[i].AsInt();
      TestCout( d, true );
      ASSERT( d == res[i].asInt )
    }
    else  {
      TestCout( _T("ASSERTION failure\n"));
    }
    TestCout( _T("Checking AsUInt(): "));
    if ( res[i].asUInt != 0 )  {
      ui = value[i].AsUInt();
      TestCout( ui, true );
      ASSERT( ui == res[i].asUInt )
    }
    else  {
      TestCout( _T("ASSERTION failure\n"));
    }
    TestCout( _T("Checking AsInt32(): "));
    if ( res[i].asInt32 != 0 )  {
      d = value[i].AsInt32();
      TestCout( d, true );
      ASSERT( d == res[i].asInt32 )
    }
    else  {
      TestCout( _T("ASSERTION failure\n"));
    }
    TestCout( _T("Checking AsUInt32(): "));
    if ( res[i].asUInt32 != 0 )  {
      ui = value[i].AsUInt32();
      TestCout( ui, true );
      ASSERT( ui == res[i].asUInt32 )
    }
    else  {
      TestCout( _T("ASSERTION failure\n"));
    }

    wxInt64 i64; wxUint64 ui64;

    TestCout( _T("Checking AsInt64(): "));
    if ( res[i].asInt64 != 0 )  {
      i64 = value[i].AsInt64();
      TestCout( i64, true );
      ASSERT( i64 == res[i].asInt64 )
    }
    else  {
      TestCout( _T("ASSERTION failure\n"));
    }
    TestCout( _T("Checking AsUInt64(): "));
    if ( res[i].asUInt64 != 0 )  {
      ui64 = value[i].AsUInt64();
      TestCout( ui64, true );
      ASSERT( ui64 == res[i].asUInt64 )
    }
    else  {
      TestCout( _T("ASSERTION failure\n"));
    }

  }    // end for

#endif    // defined ( wxJSON_64BIT_INT )
  return r;
}

// test the writer class for 64-bits integers
int Test56()
{
#if defined( wxJSON_64BIT_INT )
  wxJSONValue root;

  // the 'object' object contains all primitive types
  root[_T("object")][_T("integer")]      = -20;
  root[_T("object")][_T("unsigned int")] = (unsigned int) 30;
  root[_T("object")][_T("int64")] = (wxInt64) INT_MIN - 100;
  root[_T("object")][_T("uint64")] = (wxUint64) INT_MAX + 100;
  root[_T("object")][_T("int64-2")] = (wxInt64) LLONG_MIN;
  root[_T("object")][_T("uint64-2")] = (wxUint64) LLONG_MAX;
  root[_T("object")][_T("bool-t")]       = true;
  root[_T("object")][_T("bool-f")]       = false;
  root[_T("object")][_T("double")]       = 20.30;
  root[_T("object")][_T("C string")]     = _T("static string 1");
  root[_T("object")][_T("wxString")]     = _T("wxString 1");
  root[_T("object")][_T("null")]         = wxJSONValue();

  wxJSONWriter writer;
  wxString str;

  writer.Write( root, str );
  TestCout( _T("The JSON text document:\n"));
  TestCout( str );
  TestCout( _T("\n\n"));

#endif    // defined ( wxJSON_64BIT_INT )
  return 0;
}



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

// prints a JSON value object and the reader's errors
static void PrintValue( wxJSONValue& val, wxJSONReader* reader = 0 )
{
  wxJSONWriter writer( wxJSONWRITER_STYLED | wxJSONWRITER_WRITE_COMMENTS );
  wxString s;
  writer.Write( val, s );
  TestCout( s );
  if ( reader )  {
    PrintErrors( *reader );
  }
}

// test the reader class for 64-bits integers
// this test is also done in 32-bits mode: the 64-bits integers will
// be stored as double types
int Test57()
{
  static const wxChar* buff = _T("\n")
	_T("[\n")
	_T("   -20,\n")
	_T("   30,\n")

	// 32-bits limits
	_T("   -2147483648,\n")	// INT_MIN
	_T("   2147483647,\n")	// INT_MAX
	_T("   4294967295,\n")	// UINT_MAX

	// 32-bits limits exceeded
	_T("   2147483747,\n")	// INT_MAX + 100
	_T("   -2147483748,\n")	// INT_MIN - 100
	_T("   4294967395,\n")	// UINT_MAX + 100

	// 64-bits limits
	_T("   9223372036854775807,\n")		// LLONG_MAX
	_T("   -9223372036854775808,\n")	// LLONG_MIN
	_T("   18446744073709551615,\n")	// ULLONG_MAX

	// 64-bits limits exceeded
	_T("   9223372036854775907,\n")		// LLONG_MAX + 100
	_T("   -9223372036854775908,\n")	// LLONG_MIN - 100
	_T("   18446744073709551715\n")		// ULLONG_MAX + 100

	_T("]\n");

  int r = 0;                // return status code ZERO=Ok

  wxString str( buff );
  TestCout( _T("The JSON text document:\n"));
  TestCout( str );

  wxJSONValue  root;
  wxJSONReader reader;
  int numErrors = reader.Parse( str, &root );
  TestCout( _T("\n\nErrors reading the document: "));
  TestCout( numErrors, true );

  if ( numErrors > 0 )  {
    PrintErrors( reader );
    r = 1;
  }
  else  {
    // now check the results by getting the type of the values
    // note that the results are different if the platform does
    // support 64-bits integer or not
#if defined( wxJSON_64BIT_INT )
    wxJSONType results[] = {
	wxJSONTYPE_INT,		// -20
	wxJSONTYPE_INT,		// 30
	wxJSONTYPE_INT,		// INT_MIN
	wxJSONTYPE_INT,		// INT_MAX

	wxJSONTYPE_INT64,	// UINT_MAX
	wxJSONTYPE_INT64,	// INT_MAX + 100
	wxJSONTYPE_INT64,	// INT_MIN - 100
	wxJSONTYPE_INT64,	// UINT_MAX + 100

	wxJSONTYPE_INT64,	// LLONG_MAX
	wxJSONTYPE_INT64,	// LLONG_MIN
	wxJSONTYPE_UINT64,	// ULLONG_MAX

	wxJSONTYPE_UINT64,	// LLONG_MAX + 100
	wxJSONTYPE_DOUBLE,	// LLONG_MIN - 100
	wxJSONTYPE_DOUBLE,	// ULLONG_MAX + 100
    };
#else
    wxJSONType results[] = {
	wxJSONTYPE_INT,		// -20
	wxJSONTYPE_INT,		// 30
	wxJSONTYPE_INT,		// INT_MIN
	wxJSONTYPE_INT,		// INT_MAX

	wxJSONTYPE_UINT,	// UINT_MAX
	wxJSONTYPE_DOUBLE,	// INT_MAX + 100
	wxJSONTYPE_DOUBLE,	// INT_MIN - 100
	wxJSONTYPE_DOUBLE,	// UINT_MAX + 100

	wxJSONTYPE_DOUBLE,	// LLONG_MAX
	wxJSONTYPE_DOUBLE,	// LLONG_MIN
	wxJSONTYPE_DOUBLE,	// ULLONG_MAX

	wxJSONTYPE_DOUBLE,	// LLONG_MAX + 100
	wxJSONTYPE_DOUBLE,	// LLONG_MIN - 100
	wxJSONTYPE_DOUBLE,	// ULLONG_MAX + 100
    };
#endif

    for ( int i = 0; i < 14; i++ )  {
      wxJSONType type;
      type =  root[i].GetType();
      TestCout( _T("Element no. "));
      TestCout( i );
      TestCout( _T(" type is: "));
      TestCout( wxJSONValue::TypeToString( type));
      TestCout( _T("\n"));
      ASSERT( type == results[i] );
    }

    r = 0;
  }

  return r;
}

// there is a bug in the wxJSONReader::ReadValue() function: it seems
// that the wxString::To(U)LongLong() function always fails to convert
// numbers that are, in fact, 64-bits integers
// I test this because I have a bug in the Test57() function
//
// 3 may 2008: the test failed: the wxString::ToLongLong() function
// always returns FALSE because the 'wxHAS_STRTOLL' macro is not
// defined on my system although the 'strtoll' function does exist.
//
// This is the source of the wxWidgets 2.8.7 function:
//
//   bool wxString::ToLongLong(wxLongLong_t *val, int base) const
//   {
//   #ifdef wxHAS_STRTOLL
//       return wxStringToIntType(c_str(), val, base, wxStrtoll);
//   #else
//      // TODO: implement this ourselves
//      wxUnusedVar(val);
//      wxUnusedVar(base);
//      return false;
//   #endif // wxHAS_STRTOLL
//   }
//
int Test58()
{
#if defined( wxJSON_64BIT_INT )
  wxString s1( _T("200"));
  wxInt64 i64;
  bool r = s1.ToLongLong( &i64);
  TestCout( _T("Converting string: 200 - result:"));
  TestCout( r, true );
  // ASSERT( r )         // the test app. fails (see above)
#endif
  return 0;  
}


//
// because some systems lack the 'strtoll' function even if the system
// has native 64-bit integer support, I implemented my own 'strtoll'
// function: it is a incomplete implementation because only base 10
// conversion is implemented. This function tests the conversion 
//
// 4 may 2008: test is sucessfull
int Test59()
{
  // The Borland BCC 5.5 compiler fails to compile this test
  // function because:
  //   'Undefined call to function wxULL() in Test59()'
  //
#if defined( wxJSON_64BIT_INT ) && !defined( __BORLANDC__ )
  wxString s1( _T("200"));
  wxUint64 ui64;
  bool r = wxJSONReader::Strtoull( s1, &ui64);
  TestCout( _T("Converting string: 200 - result:"));
  TestCout( r, true );
  TestCout( _T("Printing the converted value: "));
  TestCout( ui64, true );
  ASSERT( r )
  ASSERT( ui64 == 200 )

  wxString s2( _T("-200"));
  wxInt64 i64;
  r = wxJSONReader::Strtoll( s2, &i64);
  TestCout( _T("Converting string: -200 - result:"));
  TestCout( r, true );
  TestCout( _T("Printing the converted value: "));
  TestCout( i64, true );
  ASSERT( r )
  ASSERT( i64 == -200 )

  // check many values

  struct Result {
    wxChar*  str;       // the string that has to be converted
    bool     res;	// result
    bool     uRes;	// result for unsigned
    wxInt64  value;	// value
    wxUint64 uValue;	// value for unsigned
  }; 

  Result results[] = {
	{
		_T("1000000" ),
		true, true,
		wxLL(1000000), wxULL(1000000)
	},

	{
		_T("1007463540000" ),
		true, true,
		wxLL(1007463540000), wxULL(1007463540000)
	},

	{
		_T("-465342984756" ),
		true, false,
		wxLL(-465342984756), wxULL(0)
	},

	{
		_T("-1" ),
		true, false,
		wxLL(-1), wxULL(0)
	},

	{
		_T("a89" ),
		false, false,
		wxLL(-465342984756), wxULL(0)
	},

	{
		_T("980a89" ),
		false, false,
		wxLL(-465342984756), wxULL(0)
	},

	{
		_T("9223372036854775807" ),	// LLONG_MAX
		true, true,
		wxLL( 9223372036854775807), wxULL(9223372036854775807)
	},

	{
		_T("-9223372036854775808" ),	// LLONG_MIN
		true, false,
		wxLL(-9223372036854775808), wxULL(0)
	},

	{
		_T("18446744073709551615" ),	// ULLONG_MAX
		false, true,
		wxLL(0), wxULL(18446744073709551615)
	},

	{
		_T("18446744073709551625" ),	// ULLONG_MAX + 10 (overflow)
		false, false,
		wxLL(0), wxULL(0)
	},

	{
		_T("-9223372036854775809" ),	// LLONG_MIN - 1 (overflow)
		false, false,
		wxLL(0), wxULL(0)
	},

	{
		_T("9223372036854775808" ),	// LLONG_MAX + 1
		false, true,
		wxLL(0), wxULL(9223372036854775808)
	},

	{
		_T("218446744073709551615" ),	// string is too long (21 digits)
		false, false,
		wxLL(-9223372036854775808), wxULL(0)
	}

  };

  for ( int i = 0; i < 13; i++ )  {
    bool r1, r2;
    TestCout( _T("Converting string: "));
    TestCout( results[i].str );
    TestCout( _T("\n    To wxInt64 - result is: "));
    wxString strStr( results[i].str );
    r1 = wxJSONReader::Strtoll( strStr, &i64);
    TestCout( r1, true );

    TestCout( _T("    To wxUint64 - result is: "));
    r2 = wxJSONReader::Strtoull( strStr, &ui64);
    TestCout( r2, true );
    ASSERT( r1 == results[i].res )
    ASSERT( r2 == results[i].uRes )

    if ( r1 ) {
      TestCout( _T("    Printing the converted INT value: "));
      TestCout( i64, true );
      ASSERT( i64 == results[i].value )
    }
    if ( r2 ) {
      TestCout( _T("    Printing the converted UINT value: "));
      TestCout( ui64, true );
      ASSERT( ui64 == results[i].uValue )
    }
  }
#endif        // defined wxJSON_64BIT_INT
  return 0;  
}

// testing the wxJSONWriter::WriteChar() function
int Test60()
// the problem:
//  the wxJSONWriter::WriteChar( wxChar ch ) function returns -1 if
//  an error occur; otherwise the character itself is returned that is
//  'ch' itself.
//  The problem is that in Unicode builds the character is never
//  negative because 'wxChar' is a 'wchar_t' type but in ANSI builds
//  the character 'ch' may be greater than 0x7F and it becomes
//  negative when it is assigned to the 'int' return value.
//  I tried a cast to unsigned character:
// 
//    int r = (unsigned wxChar) ch;   // fails to compile on BCC 5.5
//
//  but this does not compile on BCC 5.5 in Unicode mode because
//  'unsigned 'wchar_t' is not a recognized as a type.
//  I tried:
//
//    int r = (unsigned int) ch;
//
//  but this does not work: the integer 'r' still remains negative
//  causing the JSON writer to stop writing.
//  This test function is a test for getting the solution.
{
  wxChar ch1 = 0x48;         // the ASCII '0' (decimal 72)
  wxChar ch2 = 0xa9;         // the copyright sign in latin1 (decimal 169)

  int r1 = ch1;
  int r2 = ch2;              // in ANSI 'r2' is negative -87

  int r3 = (unsigned char) ch2;  // OK, in ANSI 'r2' is 169


#if defined( wxJSON_USE_UNICODE )
  TestCout( _T("Unicode mode:\n"));
  TestCout( _T("   r1 (0x48) ="));
  TestCout( r1, true );
  TestCout( _T("   r2 (0xA9) ="));
  TestCout( r2, true );
#else
  TestCout( _T("ANSI mode:\n"));
  TestCout( _T("   r1 (0x48) ="));
  TestCout( r1, true );
  TestCout( _T("   r2 (0xA9) ="));
  TestCout( r2, true );
  TestCout( _T("   r3 (0xA9) ="));
  TestCout( r3, true );
#endif

  return 0;
}


/*
{
}
*/





