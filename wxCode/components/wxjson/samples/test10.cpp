/******************************************************************************

   test10.cpp

    test the int, short, long and 64-bit integer
    Copyright (C) 2009  Luciano Cattani

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
#include <wx/arrstr.h>
#include <wx/app.h>
#include <wx/log.h>
#include <wx/debug.h>
#include <wx/longlong.h>

#include <wx/jsonval.h>
#include <wx/jsonwriter.h>
#include "test.h"


static int TestX_X()
{
	return 0;
}


//
// test the wxWidgets definitions and macros
int Test13_1()
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


//
// test various return values from INT, SHORT, LONG and LONG LONG
int Test13_2()
{
	int r = 0;

	// the memberfunction that we check are:
	//
	//   GetType()
	//   IsInt()
	//   IsUInt()
	//   IsShort()
	//   IsUShort()
	//   IsInt32()     from version 1.0.0 just calls IsLong()
	//   IsUInt32()    from version 1.0.0 just calls IsULong()
	//   IsInt64()
	//   IsUInt64()
	//   AsInt()
	//   AsUInt()
	//   AsInt32()     from version 1.0.0 just calls AsLong()
	//   AsUInt32()    from version 1.0.0 just calls AsULong()
	//   AsInt64()
	//   AsUInt64()

	// we use an array of integers and check the results of the
	// memberfunctions. We set an integer value to every element
	wxJSONValue value;

	value.Append( 100 );                 // 0: signed short
	value.Append( (unsigned) 110 );      // 1: unsigned short
	value.Append( -1 );                  // 2: signed int64
	value.Append( (unsigned) -1 );       // 3: unsigned integer
	value.Append( (unsigned) 65000 );    // 4: unsigned short
	value.Append( 65000 );	       // 5. signed long (cannot be short)

	value.Append( (long) SHORT_MAX + 10 );            // 6: positive, signed long
	value.Append( (long) SHORT_MIN - 10 );            // 7. negative, sigend long
	value.Append( (unsigned long) USHORT_MAX + 10 );  // 8. unsigned long

#if defined( wxJSON_64BIT_INT )
	value.Append( (wxInt64) LONG_MAX + 10 );    // 9. positive, signed longlong
	value.Append( (wxInt64) LONG_MIN - 10 );    //10. negative, signed longlong
	value.Append( (wxUint64) ULONG_MAX + 10 );  //11. unsigned longlong
#endif
	value.Append( _T("A string"));            //12. a string value


	// the expected results for every element are stored in an array
	// of structures: for every function there is a result.
	// the first field contains the type returned by GetType().
	// Note that if the IsXxxxx() returns TRUE we can call AsXxxxx()
	// and ASSERT the return value.
	// In debug builds, the AsXxxxxx() will abort the program with an
	// ASSERTION failure unless the wxJSON_NOABORT_ASSERT is defined
	struct Result {
		wxJSONType     iType;
		bool           isInt;
		bool           isUInt;
		bool           isShort;
		bool           isUShort;
		bool           isLong;
		bool           isULong;
		bool           isInt64;
		bool           isUInt64;
		int            asInt;
		unsigned       asUInt;
		short int      asShort;
		unsigned short asUShort;
		long           asLong;
		unsigned long  asULong;
#if defined( wxJSON_64BIT_INT )
		wxInt64        asInt64;
		wxUint64       asUInt64;
#endif
	};

	Result res[] =  {
		// element 0: a positive, signed int, value=100
		{
			wxJSONTYPE_SHORT,	// iType,
			true,		// isInt,
			false,		// isUInt,
			true,		// isShort,
			false,		// isUShort,
			true,		// isLong,
			false,		// isULong,
			true,		// isInt64,
			false,		// isUInt64,
			100,		// asInt,
			100,		// asUInt,
			100,		// asShort,
			100,		// asUShort,
			100,		// asLong,
			100		// asULong
#if defined( wxJSON_64BIT_INT )
	           ,
			100,		// asInt64,
			100		// asUInt64
#endif
		},

		// element 1: a positive, unsigned int, value=110
		{
			wxJSONTYPE_USHORT,	// iType,
			false,		// isInt,
			true,		// isUInt,
			false,		// isShort,
			true,		// isUShort,
			false,		// isLong,
			true,		// isULong,
			false,		// isInt64,
			true,		// isUInt64,
			110,		// asInt,
			110,		// asUInt,
			110,		// asShort,
			110,		// asUShort,
			110,		// asLong,
			110		// asULong	
#if defined( wxJSON_64BIT_INT )
				,
			110,		// asInt64,
			110		// asUInt64
#endif
		},

		// element 2: a negative, signed int, value=-1
		{
			wxJSONTYPE_SHORT,	// iType,
			true,		// isInt,
			false,		// isUInt,
			true,		// isShort,
			false,		// isUShort,
			true,		// isLong,
			false,		// isULong,
			true,		// isInt64,
			false,		// isUInt64,
			-1,		// asInt,
			-1,		// asUInt,
			-1,		// asShort,
			-1,		// asUShort,
			-1,		// asLong,
			-1		// asULong
#if defined( wxJSON_64BIT_INT )
	           ,
			-1,		// asInt64,
			-1		// asUInt64
#endif
		},

		// element 3: a negative, unsigned int, value=-1
		{
			wxJSONTYPE_ULONG,	// iType,
			false,		// isInt,
			true,		// isUInt,
			false,		// isShort,
			false,		// isUShort,
			false,		// isLong,
			true,		// isULong,
			false,		// isInt64,
			true,		// isUInt64,
			-1,		// asInt,
			-1,		// asUInt,
			-1,		// asShort,
			-1,		// asUShort,
			-1,		// asLong,
			-1		// asULong
#if defined( wxJSON_64BIT_INT )
	           ,
			-1,		// asInt64,
			(unsigned int) -1  // asUInt64
#endif
		},


		// element 4: a positive, unsigned int, value=65000
		{
			wxJSONTYPE_USHORT,	// iType,
			false,		// isInt,
			true,		// isUInt,
			false,		// isShort,	
			true,		// isUShort,
			false,		// isLong,
			true,		// isULong,
			false,		// isInt64,
			true,		// isUInt64,
			0,		// asInt,
			65000,		// asUInt,
			0,		// asShort,
			65000,		// asUShort,
			0,		// asLong,
			65000		// asULong
#if defined( wxJSON_64BIT_INT )
	           ,
			0,		// asInt64,
			65000  		// asUInt64
#endif
		},

		// element 5: a positive, signed int, value=65000 (cannot be short)
		{
			wxJSONTYPE_LONG,	// iType,
			true,		// isInt,
			false,		// isUInt,
			false,		// isShort,
			false,		// isUShort,
			true,		// isLong,
			false,		// isULong,
			true,		// isInt64,
			false,		// isUInt64,
			65000,		// asInt,
			0,		// asUInt,
			0,		// asShort,
			0,		// asUShort,
			65000,		// asLong,
			0		// asULong
#if defined( wxJSON_64BIT_INT )
	           ,
			65000,		// asInt64,
			0		// asUInt64
#endif
		},

		// element 6: a positive, signed int, value=SHORT_MAX+10
		{
			wxJSONTYPE_LONG,	// iType,
			true,		// isInt,
			false,		// isUInt,
			false,		// isShort,
			false,		// isUShort,
			true,		// isLong,
			false,		// isULong,
			true,		// isInt64,
			false,		// isUInt64,
			SHORT_MAX + 10,	// asInt,
			0,		// asUInt,
			0,		// asShort,
			0,		// asUShort,
			SHORT_MAX + 10,	// asLong,
			0		// asULong
#if defined( wxJSON_64BIT_INT )
	           ,
			SHORT_MAX + 10,	// asInt64,
			0		// asUInt64
#endif
		},

		// element 7: a negative, signed int, value=SHORT_MIN - 10
		{
			wxJSONTYPE_LONG,	// iType,
			true,		// isInt,
			false,		// isUInt,
			false,		// isShort,
			false,		// isUShort,
			true,		// isLong,
			false,		// isULong,
			true,		// isInt64,
			false,		// isUInt64,
			SHORT_MIN - 10,	// asInt,
			0,		// asUInt,
			0,		// asShort,
			0,		// asUShort,
			SHORT_MIN - 10,	// asLong,
			0		// asULong
#if defined( wxJSON_64BIT_INT )
	           ,
			SHORT_MIN - 10,	// asInt64,
			0		// asUInt64
#endif
		},

		// element 8: a positive, unsigned INT, value=USHORT_MAX + 10
		{
			wxJSONTYPE_ULONG, // iType,
			false,		// isInt,
			true,		// isUInt,
			false,		// isShort,
			false,		// isUShort,
			false,		// isLong,
			true,		// isULong,
			false,		// isInt64,
			true,		// isUInt64,
			0,		// asInt,
			USHORT_MAX + 10, // asUInt,
			0,		// asShort,
			0,		// asUShort,
			0,		// asLong,
			USHORT_MAX + 10	 // asULong
#if defined( wxJSON_64BIT_INT )
	           ,
			0,		// asInt64,
			USHORT_MAX + 10	// asUInt64
#endif
		},

		// element 9: a positive, signed llong, value=LONG_MAX + 10
		{
			wxJSONTYPE_INT64, // iType,
			false,		// isInt,
			false,		// isUInt,
			false,		// isShort,
			false,		// isUShort,
			false,		// isLong,
			false,		// isULong,
			true,		// isInt64,
			false,		// isUInt64,
			0,		// asInt,
			0,		// asUInt,
			0,		// asShort,
			0,		// asUShort,
			0,		// asLong,
			0		// asULong
#if defined( wxJSON_64BIT_INT )
	           ,
			(wxInt64) LONG_MAX + 10, // asInt64,
			0		// asUInt64
#endif
		},

		// element 10: a negative, signed llong, value=LONG_MIN - 10
		{
			wxJSONTYPE_INT64, // iType,
			false,		// isInt,
			false,		// isUInt,
			false,		// isShort,
			false,		// isUShort,
			false,		// isLong,
			false,		// isULong,
			true,		// isInt64,
			false,		// isUInt64,
			0,		// asInt,
			0,		// asUInt,
			0,		// asShort,
			0,		// asUShort,
			0,		// asLong,
			0		// asULong
#if defined( wxJSON_64BIT_INT )
	           ,
			(wxInt64) LONG_MIN - 10, // asInt64,
			0		// asUInt64
#endif
		},


		// element 11: a positive, unsigned llong, value=ULONG_MAX + 10
		{
			wxJSONTYPE_UINT64, // iType,
			false,		// isInt,
			false,		// isUInt,
			false,		// isShort,
			false,		// isUShort,
			false,		// isLong,
			false,		// isULong,
			false,		// isInt64,
			true,		// isUInt64,
			0,		// asInt,
			0,		// asUInt,
			0,		// asShort,
			0,		// asUShort,
			0,		// asLong,
			0		// asULong
#if defined( wxJSON_64BIT_INT )
	           ,
			0,		 // asInt64,
			(wxUint64) ULONG_MAX + 10	 // asUInt64
#endif
		},

		// element 12: a string value
		{
			wxJSONTYPE_STRING, // iType,
			false,		// isInt,
			false,		// isUInt,
			false,		// isShort,
			false,		// isUShort,
			false,		// isLong,
			false,		// isULong,
			false,		// isInt64,
			false,		// isUInt64,
			0,		// asInt,
			0,		// asUInt,
			0,		// asShort,
			0,		// asUShort,
			0,		// asLong,
			0		// asULong
#if defined( wxJSON_64BIT_INT )
	           ,
			0,		 // asInt64,
			0		 // asUInt64
#endif
		}
	};		// end res[] array

	int x;       unsigned int ui;
	long int l;  unsigned long int ul;
	short int h; unsigned short int uh;

#if defined( wxJSON_64BIT_INT )
	wxInt64 i64; wxUint64 ui64; 
#endif

	int numElem = value.Size();	// the number of elements in the array

	for ( int i = 0; i < numElem; i++ )  {
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

		bool r;     // checking the IsXxxxxx() and AsXxxxxx() functions

		/****************** INT *****************************/

		TestCout( _T("Checking IsInt(): "));
		r = value[i].IsInt();
		TestCout( r, true );
		ASSERT( r == res[i].isInt );

		TestCout( _T("Checking AsInt(): "));
		if ( r )  {
			x = value[i].AsInt();
			TestCout( x, true );
			ASSERT( x == res[i].asInt )
		}
		else  {
#if defined( wxJSON_NOABORT_ASSERT )
			x = value[i].AsInt();
			TestCout( x, false );
#endif
			TestCout( _T( " - ASSERTION failure on debug\n"));
		}

		/****************** UINT *****************************/

		TestCout( _T("Checking IsUInt(): "));
		r = value[i].IsUInt();
		TestCout( r, true );
		ASSERT( r == res[i].isUInt );

		TestCout( _T("Checking AsUInt(): "));
		if ( r )  {
			ui = value[i].AsUInt();
			TestCout( ui, true );
			ASSERT( ui == res[i].asUInt )
		}
		else  {
#if defined( wxJSON_NOABORT_ASSERT )
			ui = value[i].AsUInt();
			TestCout( ui, false );
#endif
			TestCout( _T( " - ASSERTION failure on debug\n"));
		}


    /****************** SHORT *****************************/

		TestCout( _T("Checking IsShort(): "));
		r = value[i].IsShort();
		TestCout( r, true );
		ASSERT( r == res[i].isShort );

		TestCout( _T("Checking AsShort(): "));
		if ( r )  {
			h = value[i].AsShort();
			TestCout( h, true );
			ASSERT( h == res[i].asShort )
		}
		else  {
#if defined( wxJSON_NOABORT_ASSERT )
			h = value[i].AsShort();
			TestCout( h, false );
#endif
			TestCout( _T( " - ASSERTION failure on debug\n"));
		}

    /****************** USHORT *****************************/

		TestCout( _T("Checking IsUShort(): "));
		r = value[i].IsUShort();
		TestCout( r, true );
		ASSERT( r == res[i].isUShort );

		TestCout( _T("Checking AsUShort(): "));
		if ( r )  {
			uh = value[i].AsUShort();
			TestCout( uh, true );
			ASSERT( uh == res[i].asUShort )
		}
		else  {
#if defined( wxJSON_NOABORT_ASSERT )
			uh = value[i].AsUShort();
			TestCout( uh, false );
#endif
			TestCout( _T( " - ASSERTION failure on debug\n"));
		}

	/****************** LONG *****************************/

		TestCout( _T("Checking IsLong(): "));
		r = value[i].IsLong();
		TestCout( r, true );
		ASSERT( r == res[i].isLong );

		TestCout( _T("Checking AsLong(): "));
		if ( r )  {
			l = value[i].AsLong();
			TestCout( l, true );
			ASSERT( l == res[i].asLong )
		}
		else  {
#if defined( wxJSON_NOABORT_ASSERT )
			l = value[i].AsLong();
			TestCout( l, false );
#endif
			TestCout( _T( " - ASSERTION failure on debug\n"));
		}

    /****************** ULONG *****************************/

		TestCout( _T("Checking IsULong(): "));
		r = value[i].IsULong();
		TestCout( r, true );
		ASSERT( r == res[i].isULong );

		TestCout( _T("Checking AsULong(): "));
		if ( r )  {
			ul = value[i].AsULong();
			TestCout( ul, true );
			ASSERT( ul == res[i].asULong )
		}
		else  {
#if defined( wxJSON_NOABORT_ASSERT )
			ul = value[i].AsULong();
			TestCout( ul, false );
#endif
			TestCout( _T( " - ASSERTION failure on debug\n"));
		}

//
// the following are only done if and only if 64-bits INT is enables
#if defined( wxJSON_64BIT_INT )

    /****************** INT64 *****************************/

		TestCout( _T("Checking IsInt64(): "));
		r = value[i].IsInt64();
		TestCout( r, true );
		ASSERT( r == res[i].isInt64 );

		TestCout( _T("Checking AsInt64(): "));
		if ( r )  {
			i64 = value[i].AsInt64();
			TestCout( i64, true );
			ASSERT( i64 == res[i].asInt64 )
		}
		else  {
#if defined( wxJSON_NOABORT_ASSERT )
			i64 = value[i].AsInt64();
			TestCout( i64, false );
#endif
			TestCout( _T( " - ASSERTION failure on debug\n"));
		}

    /****************** UINT64 *****************************/

		TestCout( _T("Checking IsUInt64(): "));
		r = value[i].IsUInt64();
		TestCout( r, true );
		ASSERT( r == res[i].isUInt64 );

		TestCout( _T("Checking AsUInt64(): "));
		if ( r )  {
			ui64 = value[i].AsUInt64();
			TestCout( ui64, true );
			ASSERT( ui64 == res[i].asUInt64 )
		}
		else  {
#if defined( wxJSON_NOABORT_ASSERT )
			ui64 = value[i].AsUInt64();
			TestCout( ui64, false );
#endif
			TestCout( _T( " - ASSERTION failure on debug\n"));
		}

#endif    // is defined( wxJSON_64BIT_INT )

	}    // end for

	return r;
}


// there is a bug in the wxJSONReader::ReadValue() function: it seems
// that the wxString::To(U)LongLong() function always fails to convert
// numbers that are, in fact, 64-bits integers
// I test this because I have a bug in the Test13_2() function
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
// The test is successfull in ANSI mode but fails in Unicode.
// This is because the 'strtoll' function only accepts 'char *'
// and not 'wchar_t*' type.
// There is the wide-char counterpart of the function whose
// name is 'wcstoll' on my GNU/Linux system but it seems
// wxWidgets completely ignores it
// For this reason I have implemented my own ToULongLong()
// function: not a complete implementation but only the one
// that fits the wxJSON library's needs.
int Test13_3()
{
#if defined( wxJSON_64BIT_INT )
	wxString s1( _T("200"));
	wxInt64 i64;
	bool r = s1.ToLongLong( &i64);
	TestCout( _T("Converting string: 200 using wxString::ToLongLong() - result:"));
	TestCout( r, true );
	TestCout( _T("The result should be TRUE"));
	
	if ( r )	{
		ASSERT( i64 == 200 )
	}
#endif
	return 0;  
}


//  The problem:
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
//  'unsigned 'wchar_t' is not recognized as a type.
//  I tried:
//
//    int r = (unsigned int) ch;
//
//  but this does not work: the integer 'r' still remains negative
//  causing the JSON writer to stop writing.
//  This test function is a test for getting the solution.
//
// OK, the solution is:
//
//   - we can assign a 'wchar_t' to an int without the need of a cast
//   - to assign a 'char' to an int we first cast to: 'unsigned char'
//
// october 2009
// ------------
//	The test is no more usefull for ver. >= 1.1 because the reader now
//	reads UTF-8 code units (one byte) which are stored in 'unsigned char'
static int Test60()
{
	wxChar ch1 = 0x48;         // the ASCII '0' (decimal 72)
	wxChar ch2 = 0xa9;         // the copyright sign in latin1 (decimal 169)

	int r1 = ch1;
	int r2 = ch2;              // in ANSI 'r2' is negative -87

#if defined( wxJSON_USE_UNICODE )	// always OK if using wchar_t
	TestCout( _T("Unicode mode:\n"));
	TestCout( _T("   r1 (0x48) ="));
	TestCout( r1, true );
	TestCout( _T("   r2 (0xA9) ="));
	TestCout( r2, true );
#else
	int r3 = (unsigned char) ch2;  // OK, in ANSI 'r3' is 169
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


//
// Testing the wxString::To(U)Long functions.
// The problem is that the wxJSONReader::ReadValue() function
// returns unexpected results when it reads a integer that is
// too large to fit in a 32-bits integer.
// The problem only happens when 64-bits integer is disabled.
// Seems that the wxString::To(U)Long function does not behave
// correctly and returns wrong results.
// Note that the unexpected results are not to blame to the
// wxString::To(U)Long functions which just call the 'strtol'
// (or 'wcstol' functions.
// Numbers that start with a minus sign are converted by 
// ToULong() as unsigned integers.
// I had to change the wxJSONReader::ReadValue() function so
// that negative values are never converted to unsigned ints
// OK, the Test57() function is now successfull
//
// If you run the test and obtain the commented results, then
// the wxString::To(U)Long() function is still buggy
int Test13_4()
{
// results of the test are displayed in the comment lines
// the system on which the test was run is GNULinux FC6
// running wxWidgets 2.8.7 compiled un Unicode=yes debug=yes
// GCC version 4.1.1 20061011
//
	wxString str[] = {
		_T("-2147483648"),		// INT_MIN
					//  ToLong()  TRUE, -2147483648
					//  ToULong() TRUE, 2147483648

		_T("2147483647"),		// INT_MAX
					//  ToLong()  TRUE, 2147483647
					//  ToULong() TRUE, =2147483647

		_T("4294967295"),		// UINT_MAX
					//  ToLong()  FALSE, 2147483647
					//  ToULong() TRUE,  4294967295

		// 32-bits limits exceeded
		_T("2147483747"),		// INT_MAX + 100
					//  ToLong()  FALSE, 2147483647
					//  ToULong() TRUE,  2147483747

		_T("-2147483748"),		// INT_MIN - 100
					//  ToLong()  FALSE  -2147483648
					//  ToULong() TRUE   2147483548

		_T("4294967395"),		// UINT_MAX + 100
					//  ToLong()  FALSE 2147483647
					//  ToULong() FALSE 4294967295

		// small 32-bits integers
		_T("-65000"),
					//  ToLong()  TRUE, -65000
					//  ToULong() TRUE, 4294902296

		_T("65000")	
					//  ToLong()  TRUE, 65000
					//  ToULong() TRUE, 65000
	};

	// we call the ToLong() and ToULong function for every string
	for ( int i = 0; i < 8; i++ )  {
		unsigned long int ul; long int l; bool r;
		TestCout( _T("\nConverting the string: "));
		TestCout( str[i] );
		r = str[i].ToLong( &l, 10 );
		TestCout( _T("\n    ToLong() result="));
		TestCout( r, true );
		TestCout( _T("    ToLong() value="));
		TestCout( l, true );
		r = str[i].ToULong( &ul, 10 );
		TestCout( _T("    ToULong() result="));
		TestCout( r, true );
		TestCout( _T("    ToULong() value="));
		TestCout( ul, true );
	}
	return 0;
}

// test the writer class for various integer types
int Test13_5()
{
	wxJSONValue value;

	value.Append( 100 );                 // 0: signed short
	value.Append( (unsigned) 110 );      // 1: unsigned short
	value.Append( -1 );                  // 2: signed int64
	value.Append( (unsigned) -1 );       // 3: unsigned integer
	value.Append( (unsigned) 65000 );    // 4: unsigned short
	value.Append( 65000 );	       // 5. signed long (cannot be short)

	value.Append( (long) SHORT_MAX + 10 );            // 6: positive, signed long
	value.Append( (long) SHORT_MIN - 10 );            // 7. negative, sigend long
	value.Append( (unsigned long) USHORT_MAX + 10 );  // 8. unsigned long

#if defined( wxJSON_64BIT_INT )
	value.Append( (wxInt64) LONG_MAX + 10 );    // 9. positive, signed longlong
	value.Append( (wxInt64) LONG_MIN - 10 );    //10. negative, signed longlong
	value.Append( (wxUint64) ULONG_MAX + 10 );  //11. unsigned longlong
#endif
	value.Append( _T("A string"));            //12. a string value

	wxJSONWriter writer;
	wxString str;

	writer.Write( value, str );
	TestCout( _T("The JSON text document:\n"));
	TestCout( str );
	TestCout( _T("\n\n"));

	return 0;
}


// test the reader class for 64-bits integers
// this test is also done disabling 64-bit INTs (see 'json_defs.h'):
// very large INTs are stored as double types
//
// 12 may 2008: the test is successfull in both 64-bits integer support
// enabled and disabled
int Test13_6()
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
		wxJSONType results[] = {
			wxJSONTYPE_SHORT,	// -20
			wxJSONTYPE_SHORT,	// 30
			wxJSONTYPE_LONG,	// INT_MIN
			wxJSONTYPE_LONG,	// INT_MAX

#if defined( wxJSON_64BIT_INT )
			wxJSONTYPE_INT64,	// UINT_MAX
			wxJSONTYPE_INT64,	// INT_MAX + 100
			wxJSONTYPE_INT64,	// INT_MIN - 100
			wxJSONTYPE_INT64,	// UINT_MAX + 100
			wxJSONTYPE_INT64,	// LLONG_MAX
			wxJSONTYPE_INT64,	// LLONG_MIN
			wxJSONTYPE_UINT64,	// ULLONG_MAX
			wxJSONTYPE_UINT64,	// LLONG_MAX + 100
#else
			wxJSONTYPE_ULONG,	// UINT_MAX
			wxJSONTYPE_ULONG,	// INT_MAX + 100
			wxJSONTYPE_DOUBLE,	// INT_MIN - 100
			wxJSONTYPE_DOUBLE,	// UINT_MAX + 100
			wxJSONTYPE_DOUBLE,	// LLONG_MAX
			wxJSONTYPE_DOUBLE,	// LLONG_MIN
			wxJSONTYPE_DOUBLE,	// ULLONG_MAX
			wxJSONTYPE_DOUBLE,	// LLONG_MAX + 100
#endif
			wxJSONTYPE_DOUBLE,	// LLONG_MIN - 100
			wxJSONTYPE_DOUBLE,	// ULLONG_MAX + 100
		};
		
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


// testing my own strto(u)ll function
// because some systems lack the 'strtoll' function even if the system
// has native 64-bit integer support, I implemented my own 'strtoll'
// function: it is a incomplete implementation because only base 10
// conversion is implemented. This function tests the conversion 
// See the two static functions:
//
//		wxJSONReader::Strtoll()
//		wxJSONReader::Strtoull()
//
// 4 may 2008: test is sucessfull
int Test13_7()
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
#else
	TestCout( _T("The test is only usefull if 64-bit INTs is enabled\n"));
#endif        // defined wxJSON_64BIT_INT
	return 0;  
}



/*
{
}
*/


