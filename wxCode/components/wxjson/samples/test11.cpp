/******************************************************************************

   test11.cpp

    test the wxJSON reference counting stuff (vers. 9.4)
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




static void PrintValue( wxJSONValue& val )
{
  wxJSONWriter writer;
  wxString s;
  writer.Write( val, s );
  TestCout( s );
}

//
// testing the old wxJSONValue::DeepCopy() function which was never
// tested before. This test is done in a version before 0.4 when COW
// was not yet implemented
// 11 mar 2008: OK, the test is successfull.
//
// later I run the test for COW
int Test47()
{
  // test nested object/array values
  wxJSONValue v1;
  v1[_T("key1")][_T("key2")][_T("key3")][4] = 12;

  // copy the value object from 'v1' to 'v2'
  wxJSONValue v2( v1 );

  // change v2's data
  v2[_T("key1")][_T("key2")] = _T("value changed key1-key2");

  TestCout( _T("\nPrinting v1\n" ));
  PrintValue( v1 );
  TestCout( _T("\nPrinting v2\n" ));
  PrintValue( v2 );

  // we changed v2's array's fourth element;
  // check that v1's element is still NULL
  bool b =    v1[_T("key1")][_T("key2")][_T("key3")][3].IsNull();
  ASSERT( b == true )
 
  return 0;
}

// test a copy of JSON values
int Test48()
{
  // test nested object/array values
  wxJSONValue v1;
  v1[_T("key1")][_T("key2")][3] = 12;

  // dump the two values
  wxString s1 = v1.Dump( true );
  TestCout( _T("\nDumping v1\n" ));
  TestCout( s1 );

  // copy the value object from 'v1' to 'v2'
  TestCout( _T("\nCopying \'v1\' to \'v2\'\n" ));
  wxJSONValue v2( v1 );

  TestCout( _T("\nPrinting v1\n" ));
  PrintValue( v1 );
  TestCout( _T("\nPrinting v2\n" ));
  PrintValue( v2 );

  // dump the two values
  s1 = v1.Dump( true );
  wxString s2 = v2.Dump( true );

  TestCout( _T("\nDumping v1\n" ));
  TestCout( s1 );
  TestCout( _T("\nDumping v2\n" ));
  TestCout( s2 );

  // get the referenced data pointer of v1[key1][key2][0]
  // NOTE: we cannot use this construct because the operat []
  // cause a non-const function to be called for the parent
  // value objects and a real copy to be performed
  // wxJSONRefData* data = v1[_T("key1")][_T("key2")][0].GetRefData();
  // wxString s;
  // s.Printf( _T("\nData pointer: %p\n" ), data);
  // TestCout( s );


  TestCout( _T("\nChanging v2\' third array element\n" ));
  v2[_T("key1")][_T("key2")][2] = 2;

  TestCout( _T("\nPrinting v1\n" ));
  PrintValue( v1 );
  TestCout( _T("\nPrinting v2\n" ));
  PrintValue( v2 );

  s1 = v1.Dump( true );
  s2 = v2.Dump( true );

  TestCout( _T("\nDumping v1\n" ));
  TestCout( s1 );
  TestCout( _T("\nDumping v2\n" ));
  TestCout( s2 );

  // make a new copy of 'v1' and change 'key1-key2'
  wxJSONValue v3( v1 );
  v3[_T("key1")] = _T("new key1 value");

  TestCout( _T("\nPrinting v1\n" ));
  PrintValue( v1 );
  TestCout( _T("\nPrinting v2\n" ));
  PrintValue( v2 );
  TestCout( _T("\nPrinting v3\n" ));
  PrintValue( v3 );

  wxString s3;
  s3 = v3.Dump( true );
  TestCout( _T("\nDumping v3\n" ));
  TestCout( s3 );

  return 0;
}

// for debugging purposes
static wxJSONValue *vp1, *vp2;


// this tests are for using with DDD and doing the hardcopy
// of the values used in the wxJSON internals documentation
int Test49()
{
  wxString s1, s2;  // used for dumping the values

  // creates a INT value
  wxJSONValue v1( -1 );
  s1 = v1.Dump( true );

  // now display the 'v1' value in DDD and save an hardcopy

  wxJSONValue v2( v1 );

  // now display the 'v2' value in DDD and save an hardcopy
  // notice that the referenced data points to the same ref-data

  s2 = v2.Dump( true );
  TestCout( _T("\nDumping v1\n" ));
  TestCout( s1 );
  TestCout( _T("\nDumping v2\n" ));
  TestCout( s2 );

  v1[3] = 12;

  // for displaying the 4 array's elements in DDD we need an array of
  // wxJSONValues: we store the pointers to the values in the wxJSONValue array
  const wxJSONValue* v1ArrayElem[4];
  const wxJSONInternalArray* v1Array = v1.AsArray();
  for ( int i = 0; i < 4; i++ )  {
    v1ArrayElem[i] = &(v1Array->Item( i ));
  }

  // now display the 'v1ArrayElem' pointer's array in DDD and save an hardcopy


  // dump 'v1'
  s1 = v1.Dump( true );
  TestCout( _T("\nDumping v1\n" ));
  TestCout( s1 );

  // copy the value object from 'v1' to 'v3'
  TestCout( _T("\nCopying \'v1\' to \'v3\'\n" ));
  wxJSONValue v3( v1 );

  TestCout( _T("\nPrinting v1\n" ));
  PrintValue( v1 );
  TestCout( _T("\nPrinting v3\n" ));
  PrintValue( v3 );

  // dump 'v1' and 'v3'
  s1 = v1.Dump( true );
  s2 = v3.Dump( true );

  TestCout( _T("\nDumping v1\n" ));
  TestCout( s1 );
  TestCout( _T("\nDumping v3\n" ));
  TestCout( s2 );

  // we can use here the Dump() function in the docs
  // but this code is used to get the array of pointers to
  // the 'v3' JSON array
  const wxJSONValue* v3ArrayElem[4];
  const wxJSONInternalArray* v3Array = v3.AsArray();
  for ( int i = 0; i < 4; i++ )  {
    v3ArrayElem[i] = &(v3Array->Item( i ));
  }

  TestCout( _T("\nChanging v2\'s second array element\n" ));
  v3[1] = 2;

  // and now display the new 'v3' array
  v3Array = v3.AsArray();
  for ( int i = 0; i < 4; i++ )  {
    v3ArrayElem[i] = &(v3Array->Item( i ));
  }

  TestCout( _T("\nPrinting v1\n" ));
  PrintValue( v1 );
  TestCout( _T("\nPrinting v3\n" ));
  PrintValue( v3 );

  s1 = v1.Dump( true );
  s2 = v3.Dump( true );

  TestCout( _T("\nDumping v1\n" ));
  TestCout( s1 );
  TestCout( _T("\nDumping v3\n" ));
  TestCout( s2 );




  return 0;
}

// test a copy of JSON values which holds more level-1 keys
// the test is done by copying the root value and then
// changing the value of a level-2 JSON value:
// note that level-1 value will be copied but only the
// changed value should be copied.
int Test50()
{
  // the value
  // {
  //    "book" :
  //    {
  //       "title"  : "How to catch Peter Pan",
  //       "author" : "Captain Hook\",
  //       "pages"  : 300,
  //       "price"  : 30.30,
  //       "translations" :
  //       [
  //          "italian",
  //          "german",
  //          "spanish"
  //       ]
  //    }
  // }

  wxJSONValue v1;
  v1[_T("book")][_T("title" )] = _T("How to catch Peter Pan");

  v1[_T("book")][_T("author" )] = _T("Captain Hook");
  v1[_T("book")][_T("pages" )] = 300;
  v1[_T("book")][_T("price" )] = 30.30;
  v1[_T("book")][_T("translations" )].Append( _T("italian"));
  v1[_T("book")][_T("translations" )].Append( _T("german"));
  v1[_T("book")][_T("translations" )].Append( _T("spanish"));

  TestCout( _T("\nPrinting v1\n" ));
  PrintValue( v1 );

  wxString s1, s2;
  s1 = v1.Dump( true );
  TestCout( _T("\nDumping v1\n" ));
  TestCout( s1 );

  TestCout( _T("\nCopying v1 to v2\n" ));
  wxJSONValue v2( v1 );

  s2 = v2.Dump( true );
  TestCout( _T("\nDumping v2\n" ));
  TestCout( s2 );

  TestCout( _T("\nChanging v2\'s price item\n" ));
  v2[_T("book")][_T("price" )] = 40.40;
  s2 = v2.Dump( true );
  TestCout( _T("\nDumping v2\n" ));
  TestCout( s2 );

  TestCout( _T("\nPrinting v2\n" ));
  PrintValue( v2 );

  return 0;
}


// test a copy of JSON values and a read operation
int Test51()
{
  wxJSONValue v1;
  v1[3] = 12;
  TestCout( _T("\nPrinting v1\n" ));
  PrintValue( v1 );

  // copy the value and read the fourth element
  TestCout( _T("\nCopying \'v1\' to \'v2\' and read using subscript\n" ));
  wxJSONValue v2( v1 );
  int i = v2[3].AsInt();
  ASSERT( i == 12 )

  // dumping 'v1' and 'v2' they are not shared
  wxString s1, s2;
  s1 = v1.Dump( true );
  TestCout( _T("\nDumping v1\n" ));
  TestCout( s1 );

  s2 = v2.Dump( true );
  TestCout( _T("\nDumping v2\n" ));
  TestCout( s2 );

  wxJSONValue v3;
  v3[3] = 12;
  TestCout( _T("\nPrinting v3\n" ));
  PrintValue( v3 );

  // copy the value
  TestCout( _T("\nCopying \'v3\' to \'v4\'\n" ));
  wxJSONValue v4( v3 );

  // read the fourth element using ItemAt()
  TestCout( _T("\nRead using ItemAt() const\n" ));
  i = v4.ItemAt( 3 ).AsInt();
  ASSERT( i == 12 )

  // dumping 'v1' and 'v3' they are shared
  wxString s3, s4;
  s3 = v3.Dump( true );
  TestCout( _T("\nDumping v3\n" ));
  TestCout( s3 );

  s4 = v4.Dump( true );
  TestCout( _T("\nDumping v4\n" ));
  TestCout( s4 );

  return 0;
}

/*
{
}
*/





