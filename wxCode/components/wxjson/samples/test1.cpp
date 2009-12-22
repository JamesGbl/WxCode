/******************************************************************************

   test1.cpp

    test for the wxJSONValue class: ctors, dtor and memberfunctions
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


*****************************************************************************/

//#ifdef __GNUG__
//    #pragma implementation "skeleton.cpp"
//#endif

#include <wx/app.h>
#include <wx/ffile.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/log.h>
#include <wx/debug.h>

#include <wx/jsonval.h>
#include "test.h"


// creates an array of all possible value types
static void CreateValue( wxJSONValue& arr )
{
	arr.Append( wxJSONValue((wxJSONType) wxJSONTYPE_INVALID ));	// arr[0] INVALID
	arr.Append( wxJSONValue());					// arr[1] NULL
	arr.Append( wxJSONValue( wxJSONTYPE_INVALID ));			// arr[2] INVALID without cast
	arr.Append( wxJSONValue( 10 ));					// arr[3] SHORT
	arr.Append( wxJSONValue( (unsigned int) 20 ));			// arr[4] USHORT
	arr.Append( wxJSONValue( true ));				// arr[5] BOOL
	arr.Append( wxJSONValue( 10.20 ));				// arr[6] DOUBLE
	arr.Append( wxJSONValue( _T("This is a static C string")));	// arr[7] CSTRING
	arr.Append( wxJSONValue ( wxString( _T("wxString 1"))));	// arr[8] STRING

	// long integers
	arr.Append( (long) SHORT_MAX + 10 );				// arr[9] LONG
	arr.Append( (long) SHORT_MIN - 10 );				// arr[10] LONG
	arr.Append( (unsigned long) USHORT_MAX + 10 );			// arr[11] ULONG

	// cast long but the actual value is short the type returned is SHORT
	// NOTE: USHORT_MAX can be represented either as a 'unsigned short' or
	// as a 'signed long'
	// the returned type is 'signed long' if it is not constructed as 'unsigned'

	arr.Append( (long) SHORT_MIN );				// arr[12] SHORT
	arr.Append( (long) SHORT_MAX );				// arr[13] SHORT
	arr.Append( (long) USHORT_MAX );			// arr[14] LONG not USHORT
	wxJSONValue v;
	v[ _T( "key" )] = _T( "value" );
	arr.Append( v );					// arr[15] OBJECT
#if defined( wxJSON_64BIT_INT )
	arr.Append( (wxInt64) LONG_MAX + 10 );			// arr[16] INT64
	arr.Append( (wxInt64) LONG_MIN - 10 );			// arr[17] INT64
	arr.Append( (wxUint64) ULONG_MAX + 10 );		// arr[18] UINT64
#endif
}

// test 64-bit integer support in wxWidgets
int Test1_1()
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

	TestCout( _T("Printing LONG_MAX: "));
	TestCout( (long int) LONG_MAX, true );
	TestCout( _T("Printing ULONG_MAX: "));
	TestCout( (unsigned long) ULONG_MAX, true );
	TestCout( _T("Printing LONG_MIN: "));
	TestCout( (long int) LONG_MIN, true );

	TestCout( _T("Printing SHORT_MAX: "));
	TestCout( SHORT_MAX, true );
	TestCout( _T("Printing USHORT_MAX: "));
	TestCout( (unsigned) USHORT_MAX, true );
	TestCout( _T("Printing SHORT_MIN: "));
	TestCout( SHORT_MIN, true );

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

// creates a wxJSONValue array of all types and checks the IsXxxxxxx() function
int Test1_2() 
{
	wxJSONValue v;
	CreateValue( v );
	ASSERT( v.IsArray());
	
	ASSERT( !v[0].IsValid() );
	ASSERT( v[1].IsNull() );
	ASSERT( !v[2].IsValid() );
	ASSERT( v[3].IsShort() );
	ASSERT( v[4].IsUShort() );
	ASSERT( v[5].IsBool() );
	ASSERT( v[6].IsDouble() );
#if defined( wxJSON_USE_CSTRING )
	ASSERT( v[7].IsCString() );
#else
	ASSERT( v[7].IsString() );
#endif
	ASSERT( v[8].IsString() );
	ASSERT( v[9].IsLong() );
	ASSERT( v[10].IsLong() );
	ASSERT( v[11].IsULong() );
	
	ASSERT( v[12].IsShort() );
	ASSERT( v[13].IsShort() );
	ASSERT( v[14].IsLong() );
	ASSERT( v[15].IsObject() );
	
#if defined( wxJSON_64BIT_INT )
	ASSERT( v[16].IsInt64() );
	ASSERT( v[17].IsInt64() );
	ASSERT( v[18].IsUInt64() );
#endif

	// SHORTs and LONGs (on 32-bits platforms) are also INTs 
	ASSERT( v[3].IsInt() );		// SHORT
	ASSERT( v[4].IsUInt() );	// USHORT
	ASSERT( v[9].IsInt() );		// LONG
	ASSERT( v[10].IsInt() );	// LONG
	ASSERT( v[11].IsUInt() );	// ULONG
	ASSERT( v[12].IsInt() );	// SHORT
	ASSERT( v[13].IsInt() );	// SHORT
	ASSERT( v[14].IsInt() );	// LONG

	bool r = v[10].IsShort();
	ASSERT( !r );
	// ASSERT( v[10].IsShort() );	// LONG		10/09/2009 this fails!! OK
	
	// all integers types are also INT64 because they CAN be represented as int64
	// but they are NOT uint64
	ASSERT( v[3].IsInt64() );		// SHORT
	ASSERT( v[9].IsInt64() );		// LONG

	r = v[3].IsUInt64();		// SHORT
	ASSERT( !r );
	r = v[9].IsUInt64();		// LONG
	ASSERT( !r );
	
	return 0;
}

// test the AsString() function
int Test1_3() 
{
	static const wxChar* results[]	= 
	{
		_T( "<invalid>" ),	// arr[0]
		_T( "null" ),		// arr[1]
		_T( "<invalid>" ),	// arr[2]
		_T( "10" ),		// arr[3]
		_T( "20" ),		// arr[4]
		_T( "true" ),		// arr[5]
		_T( "10.2" ),	// arr[6]
		_T( "This is a static C string" ),	// arr[7]
		_T( "wxString 1" ),			// arr[8]
		_T( "32777" ),				// arr[9]
		_T( "-32778" ),				// arr[10]
		_T( "65545" ),				// arr[11]
		_T( "-32768" ),				// arr[12]
		_T( "32767" ),				// arr[13]
		_T( "65535" ),				// arr[14]
		_T( "{1}" ),				// arr[15]
#if defined( wxJSON_64BIT_INT )
		_T( "2147483657" ),			// arr[16] 
		_T( "-2147483658" ),			// arr[17] 
		_T( "4294967305" ),			// arr[18] 
#endif
	};
	
	// the number of elements
	int num = sizeof( results) / sizeof( wxChar*);

	wxJSONValue v;
	CreateValue( v );
	TestCout( _T("Checking ROOT value\n" ));
	TestCout( _T( "       expected result: [19]\n" ));
	wxString s = v.AsString();
	TestCout( _T( "       actual result:" ));
	TestCout( s );
	TestCout( (wxChar) '\n', false );
	ASSERT( s == _T( "[19]" ));
	
	for ( int i = 0; i < num; i++ )	{
		TestCout( _T("Checking element no. " ));
		TestCout( i, true );
		TestCout( _T( "       expected result:" ));
		TestCout( results[i] );
		s = v[i].AsString();
		TestCout( _T( "       actual result:" ));
		TestCout( s );
		TestCout( (wxChar) '\n', false );
		ASSERT( s == results[i] );
	}

	return 0;
}

// try the GetInfo() and Dump() function
int Test1_4() 
{
	wxJSONValue v;
	CreateValue( v );
	
	wxString info = v.GetInfo();
	TestCout( _T("Info about \'v\'\n"));
	TestCout( info );
	TestCout( _T( "\n"));
	
	wxString dump = v.Dump();
	TestCout( _T("Dump for value \'v\'\n"));
	TestCout( dump );
	TestCout( _T( "\n"));
	
	return 0;
}

// test access functions
int Test1_5() 
{
	// arr[0] INVALID
	// arr[1] NULL
	// arr[2] INVALID without cast
	// arr[3] SHORT		(10)
	// arr[4] USHORT	(20)
	// arr[5] BOOL		(true)
	
	// arr[6] DOUBLE	(10.20)
	// arr[7] CSTRING	("This is a static string")
	// arr[8] STRING	("wxString 1")

	// arr[9] LONG		(SHORT_MAX + 10)
	// arr[10] LONG		(SHORT_MIN - 10)
	// arr[11] ULONG	(USHORT_MAX + 10 )

	// arr[12] SHORT	(SHORT_MIN)
	// arr[13] SHORT	(SHORT_MAX)
	// arr[14] LONG not USHORT	(USHORT_MAX)
	// arr[15] OBJECT		(v["key"] = "value")
	
	// arr[16] INT64	(LONG_MAX + 10)
	// arr[17] INT64	(LONG_MIN - 10)
	// arr[18] UINT64	(ULONG_MAX + 10)
	
	wxJSONValue v;
	CreateValue( v );
	int num = v.Size();

	int i = v[3].AsInt();	// does not work??
	i = v[3].AsShort();
	ASSERT( i == 10 );
	
	unsigned int ui = v[4].AsUInt();
	ASSERT( ui == 20 );
	
	// now v[3] as short (OK)
	short si = v[3].AsShort();
	ASSERT( si == 10 );
	
	// as long does not work but... you can do it in other way
	long li = v[3].AsLong();
	ASSERT( li == 10 );
	li = (long) v[3].AsShort();
	
	if ( num > 16 )	{
		i = v[16].AsInt();		// does not work
		i = v[16].AsInt64();		// compiler warning
		i = (int) v[16].AsInt64();	// explicit cast if you know what you're doing
		wxInt64 i64 = v[16].AsInt64();
		ASSERT( i64 == (wxInt64) LONG_MAX + 10 );
	}
	
	wxString str = v[8].AsString();
	ASSERT( str == _T( "wxString 1"));		
	
	return 0;
}

	
// test access operators and functions
int Test1_6() 
{
	wxJSONValue v;
	v[_T("wxWidgets")][_T("Description")] = _T("multiplatform GUI framework");
	v[_T("wxWidgets")][_T("License")] = _T("wxWidgets");
	
	// creates the node, if not exists
	wxJSONValue version = v[_T("wxWidgets")][_T("Version")];
	version[_T("Major")] = 2;
	version[_T("Minor")] = 8;
	version[_T("Stable")] = true;

	v[_T("wxWidgets")][_T("Languages")].Append( _T("C/C++"));
	v[_T("wxWidgets")][_T("Languages")].Append( _T("Java"));
	v[_T("wxWidgets")][_T("Languages")].Append( _T("Phyton"));
	v[_T("wxWidgets")][_T("Languages")].Append( _T("Perl"));
	
	wxString str = 	v[_T("wxWidgets")][_T("Languages")][1].AsString();
	ASSERT( str == _T( "Java"));
	
	int minorVer = v[_T("wxWidgets")][_T("Version")][_T("Minor")].AsInt();
	ASSERT( minorVer == 8 );

	// try to print the member names of the root value
	TestCout( _T("Printing the member names (root node) ... \n"));
	wxArrayString strArray = v.GetMemberNames();
	for ( int i = 0; i < strArray.size(); i++ )  {
		TestCout( _T("....."));
		TestCout( strArray[i]);
		TestCout( _T("\n"));
	}
	TestCout( _T("..................OK\n" ));

	// try the HasMember() function
	bool b = v[_T("wxWidgets")].HasMember( _T( "Version" ));
	ASSERT( b )
	b = v[_T("wxWidgets")].HasMember( _T( "Not exists" ));
	ASSERT( !b )
	b = v[_T("wxWidgets")][_T("Version")].HasMember( _T( "Major" ));
	ASSERT( b )
	b = v[_T("wxWidgets")].HasMember( _T( "Major" ));
	ASSERT( !b )

	return 0;
}


/************************************************************************

			testting wxJSONValue::IsSameAs()

************************************************************************/

//  comparing fundamental types
int Test2_1() 
{
	wxString aStr( _T("string 1"));
	wxString bStr( _T("string 1"));
	wxString cStr( _T("string 2"));
	bool b = aStr != bStr;
	ASSERT( !b )
	b = aStr != cStr;
	ASSERT( b )

	wxJSONValue int1( 10 );
	wxJSONValue int2( 20 );
	wxJSONValue int3( 10 );
	bool r = int1.IsSameAs( int2 );
	ASSERT( r == false );
	r = int1.IsSameAs( int3 );
	ASSERT( r == true );

	wxJSONValue str1( _T("string 1") );
	wxJSONValue str2( _T("string 2") );
	wxJSONValue str3( _T("string 1") );
	r = str1.IsSameAs( str2 );
	ASSERT( r == false );
	r = str1.IsSameAs( str3 );
	ASSERT( r == true );

	wxJSONValue double1( 100.90 );
	wxJSONValue double2( 1000.90 );
	wxJSONValue double3( 100.90 );
	r = double1.IsSameAs( double2 );
	ASSERT( r == false );
	r = double1.IsSameAs( double3 );
	ASSERT( r == true );

	wxJSONValue bool1( true );
	wxJSONValue bool2( false );
	wxJSONValue bool3( true );
	r = bool1.IsSameAs( bool2 );
	ASSERT( r == false );
	r = bool1.IsSameAs( bool3 );
	ASSERT( r == true );

	// this is a special case: TypeCString and TypeString:
	// WXJSON_USE_CSTRING macro must be is defined
	//
	wxJSONValue cStr1( _T("C-string") );
	wxJSONValue cStr2( wxString( _T("C-string") ));
	wxJSONValue cStr3( wxString( _T("C-string 1") ));
	r = cStr1.IsSameAs( cStr2 );
	ASSERT( r == true );
	r = cStr2.IsSameAs( cStr3 );
	ASSERT( r == false );

	return 0;
}

// comparing short, lomg and long-long
int Test2_2() 
{
	// the following are the same
	wxJSONValue s1( (short) 100 );
	wxJSONValue l1( (long) 100 );
	wxJSONValue i1( (wxInt64) 100 );
	bool r;
	r = s1.IsSameAs( l1 );
	ASSERT( r == true );
	r = s1.IsSameAs( i1 );
	ASSERT( r == true );
	
	// comparing very large INT64: they are the same
	wxJSONValue i64_1( (wxInt64) 9223372036854775707LL );	// LLONG_MAX - 100
	wxJSONValue i64_2( (wxInt64) 9223372036854775707LL );	// LLONG_MAX - 100
	r = i64_1.IsSameAs( i64_2 );
	ASSERT( r == true );

	// comparing unsigned and signed
	// if value is positive and between LLONG_MAX they are the same, otherwise not the same
	wxJSONValue si( 1000 );
	wxJSONValue ui( (unsigned) 1000 );	
	r = si.IsSameAs( ui );
	ASSERT( r == true );

	// comparing unsigned and signed: negative values are NOT the same
	si = -1000;
	ui = (unsigned) -1000;	
	r = si.IsSameAs( ui );
	ASSERT( r == false );
	
	return 0;
}

// comparing INTs and doubles: when comparing INTs and doubles, the values are the same if
// the double has enough precision to hold the whole number
int Test2_3() 
{
	// the following are the same
	wxJSONValue i1( 100 );
	wxJSONValue d1( (double) 100.00 );
	bool r;
	r = i1.IsSameAs( d1 );
	ASSERT( r == true );
	
	// comparing very large INT64 and a double: 
	wxJSONValue i2( (wxInt64) 9223372036854775707LL );	// LLONG_MAX - 100
	wxJSONValue d2( (double) 9223372036854775707LL );		// LLONG_MAX - 100
	r = i2.IsSameAs( d2 );
	ASSERT( r == true );

	// comparing very large INT64: 
	wxJSONValue i4( (wxInt64) 9223372036854775706LL );	// LLONG_MAX - 101
	wxJSONValue d4( (wxInt64) 9223372036854775707LL );	// LLONG_MAX - 100
	r = i4.IsSameAs( d4 );
	ASSERT( r == false );		// BUG: test returns TRUE: fixed in vers. 1.1

	// comparing unsigned and signed
	wxJSONValue i3( 1000 );
	wxJSONValue ui3( (unsigned) 1000 );	
	wxJSONValue d3( (double) 1000 );
	r = i3.IsSameAs( ui3 );
	ASSERT( r == true );
	r = i3.IsSameAs( d3 );
	ASSERT( r == true );
	
	return 0;
}

// comparing objects
int Test2_4() 
{
	// 'map1' and 'map2 are the same
	wxJSONValue map1;
	map1[_T("first")] = 10;
	map1[_T("second")] = 20.1;
	map1[_T("third")] = _T("A string");
	map1[_T("fourth")] = true;
	wxJSONValue map2;
	map2[_T("first")] = 10;
	map2[_T("second")] = 20.1;
	map2[_T("third")] = _T("A string");
	map2[_T("fourth")] = true;
	bool r = map1.IsSameAs( map2 );
	ASSERT( r == true );

	// 'map1' and 'map3' are NOT the same because 'third' key is different
	wxJSONValue map3;
	map3[_T("first")] = 10;
	map3[_T("second")] = 20.1;
	map3[_T("third")] = _T("Another string");
	map3[_T("fourth")] = true;
	r = map1.IsSameAs( map3 );
	ASSERT( r == false );

	// 'map1' and 'map4 are NOT the same: values are equal but keys are different
	wxJSONValue map4;
	map4[_T("first")] = 10;
	map4[_T("second")] = 20.1;
	map4[_T("third")] = _T("A string");
	map4[_T("fifth")] = true;
	r = map1.IsSameAs( map4 );
	ASSERT( r == false );

	// 'map1' and 'map5 are NOT the same: the 'third' KEY does not exists
	wxJSONValue map5;
	map5[_T("first")] = 10;
	map5[_T("second")] = 20.1;
	// map5[_T("third")] = _T("A string");
	map5[_T("fourth")] = true;
	r = map1.IsSameAs( map5 );
	ASSERT( r == false );
	return 0;
}


// comparing array types
int Test2_5() 
{
	wxJSONValue arr1;
	arr1.Append( 10 );
	arr1.Append( 20.1 );
	arr1.Append( true );
	arr1.Append( _T("A string" ));

	// the same values in the same order
	wxJSONValue arr4;
	arr4.Append( 10 );
	arr4.Append( 20.1 );
	arr4.Append( true );
	arr4.Append( _T("A string" ));
	bool r = arr1.IsSameAs( arr4 );
	ASSERT( r == true );

	// same values in different order
	wxJSONValue arr2;
	arr2.Append( _T("A string") );
	arr2.Append( 20.1 );
	arr2.Append( true );
	arr2.Append( 10 );
	r = arr1.IsSameAs( arr2 );
	ASSERT( r == false );		// BUG: function returns TRUE

	// different values
	wxJSONValue arr3;
	arr3.Append( _T("A string") );
	arr3.Append( 20.1 );
	arr3.Append( false );
	arr3.Append( 10 );
	r = arr1.IsSameAs( arr3 );
	ASSERT( r == false );

	return 0;
}

// comparing map types, special case NULL values
int Test2_6() 
{
	// 'map1' contains a item of value NULL but 'map2' does not contains it.
	// if a key is not present, the default value assigned to it depends on the application
	// (see the wxJSONValue::Get() function and does not necessarly default to NULL
	// so comparing such objects should return FALSE
	wxJSONValue map1;
	map1[_T("first")] = 10;
	map1[_T("second")] = 20.1;
	map1[_T("third")] = _T("string");
	map1[_T("fourth")] = wxJSONValue( (wxJSONType) wxJSONTYPE_NULL );
	wxJSONValue map2;
	map2[_T("first")] = 10;
	map2[_T("second")] = 20.1;
	map2[_T("third")] = _T("string");
	// map1[_T("fourth")] = wxJSONValue( (wxJSONType) wxJSONTYPE_NULL ));
	bool r = map1.IsSameAs( map2 );
	ASSERT( r == false );
	return 0;
}

// comparing incompatible types that may express the same quantity for example string containing "10"
// and a INT containing 10, returns FALSE
int Test2_7() 
{
	bool r;
	wxJSONValue int10( 10 );
	wxJSONValue uint10( (unsigned) 10 );
	wxJSONValue double10( 10.0 );

	// strings that contain the same numeric value is not
	// the same as a numeric value object
	wxJSONValue string10( _T("10"));
	r = string10.IsSameAs( int10 );
	ASSERT( r == false );
	r = string10.IsSameAs( uint10 );
	ASSERT( r == false );
	r = string10.IsSameAs( double10 );
	ASSERT( r == false );

	// numbers that are stored in the same way are not the same
	// if the number they express are different
	// for example -1.
	wxJSONValue menoUno1( -1 );            // this is the INT -1
	wxJSONValue menoUno2( (unsigned) -1 ); // this is UINT 4.294.967.296
	r = menoUno1.IsSameAs( menoUno2 );
	// ASSERT( r == false );	BUG: the test returns TRUE

	return 0;
}

// a test for searching for the BUG in Test2_2()
int Test2_8() 
{
	// this is the test failure
	wxJSONValue si( 1000 );
	wxJSONValue ui( (unsigned) 1000 );	
	// bool r = s1.IsSameAs( ui );
	// ASSERT( r == true );   // BUG: the test returns FALSE

	// get the referenced data
	wxJSONRefData* siData = si.GetRefData();
	wxJSONRefData* uiData = ui.GetRefData();
	
	// compare the m_value->m_valUint64 data member
	TestCout( _T("Comparing valUInt64 values: "));
	bool r = siData->m_value.m_valUInt64 == uiData->m_value.m_valUInt64;
	TestCout( r ? _T("TRUE") : _("FALSE"));
	TestCout( _T("\n"));

	TestCout( _T("Comparing VAL_UINT values: "));
	bool r2 = siData->m_value.VAL_UINT == uiData->m_value.VAL_UINT;
	TestCout( r2 ? _T("TRUE") : _("FALSE"));
	TestCout( _T("\n"));

	
	return 0;
}


/*
{
}
*/


