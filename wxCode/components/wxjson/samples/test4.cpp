/******************************************************************************

   test4.cpp

    test for the wxJSONValue::IsSameAs() function
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



int Test8() 
{
  TestCout( _T("TEST #8: comparing strings ... \n"));

  wxString aStr( _T("string 1"));
  wxString bStr( _T("string 1"));
  wxString cStr( _T("string 2"));
  bool b = aStr != bStr;
  ASSERT( !b )
  b = aStr != cStr;
  ASSERT( b )

  TestCout( _T("TEST #8: comparing JSON fundamental types ... \n"));
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

  // this is a special case: different types always return FALSE
  // with the excpetion of TypeCString and TypeString if the
  // WXJSON_USE_CSTRING macro is defined
  //
  wxJSONValue cStr1( _T("C-string") );
  wxJSONValue cStr2( wxString( _T("C-string") ));
  wxJSONValue cStr3( wxString( _T("C-string 1") ));
  r = cStr1.IsSameAs( cStr2 );
  ASSERT( r == true );
  r = cStr2.IsSameAs( cStr3 );
  ASSERT( r == false );

  TestCout( _T("TEST #8: comparing ints and doubles ... \n"));

  // test a int vs. double with the same value
  // althrough the values cannot be considered 'equal' they are 'the same'
  wxJSONValue double4( 100.0 );
  wxJSONValue int4( 100 );
  r = double4.IsSameAs( int4 );
  // ASSERT( r == false );           // BUG: fixed in rel 0.2.2

  TestCout( _T("TEST #8: comparing array types ... \n"));

  wxJSONValue arr1;
  arr1.Append( 10 );
  arr1.Append( 20.1 );
  arr1.Append( true );
  arr1.Append( _T("A string" ));

  wxJSONValue arr2;   // this is the same, in a different order
  arr2.Append( _T("A string") );
  arr2.Append( 20.1 );
  arr2.Append( true );
  arr2.Append( 10 );
  r = arr1.IsSameAs( arr2 );
  ASSERT( r == true );

  wxJSONValue arr3;   // arr3 is not the same as arr1
  arr3.Append( _T("A string") );
  arr3.Append( 20.1 );
  arr3.Append( false );
  arr3.Append( 10 );
  r = arr1.IsSameAs( arr3 );
  ASSERT( r == false );

  TestCout( _T("TEST #8: comparing array types (2) ... \n"));

  wxJSONValue arr4;   // 
  arr4.Append( _T("A string"));   // arr1:  'A String'
  arr4.Append( 20.1 );         // arr1:  20.1
  arr4.Append( 10 );           // arr1:  false
  arr4.Append( 10 );           // arr1:  10

  // this works because 'arr1' is compared to 'arr4' and the 'false'
  // element (no. 2) is not found
  r = arr1.IsSameAs( arr4 );
  ASSERT( r == false );

  // this will discover the bug in the 'array' type
  // this is because the array comparison does not mark an element
  // as 'already used' so the third and forth element in arr4 are
  // both compared against the forth element in arr1 
  // OK, bug fixed on 19 oct 2007 introducing the 'already used'
  // array.
  r = arr4.IsSameAs( arr1 );
  ASSERT( r == false );

  TestCout( _T("TEST #8: comparing object types ... \n"));

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
  r = map1.IsSameAs( map2 );
  ASSERT( r == true );

  // 'map1' and 'map3 are NOT the same because 'third' key is different
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

  TestCout( _T("TEST #8: comparing arrays of objects ... \n"));

  // comparing two arrays of objects
  wxJSONValue arr5;
  arr5[0][_T("item")] = 1;
  arr5[1][_T("item")] = 2;
  arr5[2][_T("item")] = 3;
  arr5[3][_T("item")] = 4;
  arr5[4][_T("item")] = 5;

  wxJSONValue arr6;
  arr6[0][_T("item")] = 4;
  arr6[1][_T("item")] = 5;
  arr6[2][_T("item")] = 3;
  arr6[3][_T("item")] = 1;
  arr6[4][_T("item")] = 2;
  r = arr5.IsSameAs( arr6 );
  ASSERT( r == true );

  wxJSONValue arr7;
  arr7[0][_T("item")] = 4;
  arr7[1][_T("item")] = 5;
  arr7[2][_T("item")] = 4;
  arr7[3][_T("item")] = 1;
  arr7[4][_T("item")] = 2;
  r = arr7.IsSameAs( arr6 );
  ASSERT( r == false );

  TestCout( _T("TEST #8: comparing compatible types\n"));
  wxJSONValue int10( 10 );
  wxJSONValue uint10( (unsigned) 10 );
  wxJSONValue double10( 10.0 );

  r = int10.IsSameAs( uint10 );
  ASSERT( r == true );
  r = int10.IsSameAs( double10 );
  ASSERT( r == true );
  r = double10.IsSameAs( uint10 );
  ASSERT( r == true );

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
  ASSERT( r == false );

  // writing the values
  wxString s;
  wxJSONWriter writer;
  writer.Write( menoUno1, s );
  TestCout( s );

  s.clear();
  writer.Write( menoUno2, s );
  TestCout( s );

  TestCout( _T("TEST #8: successfully completed\n"));
  return 0;
}


/*
{
}
*/

