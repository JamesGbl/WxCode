/******************************************************************************

   test1.cpp

    test for the wxJSONValue class
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


int Test1() 
{
  TestCout( _T("TEST #1 - Constructing JSON values ... \n"));

  // creates an instance of all value types
  wxJSONValue valueEmpty((wxJSONType) wxJSONTYPE_EMPTY );
  wxJSONValue valueNull;

  // the compiler should call the wxJSONValue( wxJSONType) ctor
  // even if the cast to wxJSONType is not used
  // if your compiler fails this test, you always have to cast
  // to wxJSONType 
  wxJSONValue valueNull2;
  wxJSONValue valueInt( 10 );
  wxJSONValue valueUInt( (unsigned int) 20 );
  wxJSONValue valueBool( true );
  wxJSONValue valueDouble( 10.20 );
  static const wxChar* buff1 = _T("This is a static C string");
  wxJSONValue valueCString( buff1 );
  wxJSONValue valueString( wxString( _T("wxString 1")));

  // default ctor: the following values will become an array and a map
  // but by now there are of type wxJSONTYPE_EMPTY.
  wxJSONValue valueArray;
  wxJSONValue valueMap;

  TestCout( _T("..................OK\n" ));

  TestCout( _T("TEST #1: testing AsString() on all types ... \n"));
  wxString asString;
  asString = valueEmpty.AsString();
  ASSERT( asString == _T("<empty>"));
  asString = valueInt.AsString();
  ASSERT( asString == _T("10"));
  asString = valueUInt.AsString();
  ASSERT( asString == _T("20"));
  asString = valueDouble.AsString();
  ASSERT( asString == _T("10.200000"));
  asString = valueNull.AsString();
  ASSERT( asString == _T("null"));

  wxString info = valueInt.GetInfo();
  TestCout( _T("Info about \'valueInt\'\n"));
  TestCout( info );
  TestCout( _T( "\n"));

  // now fill the array and the map with some values
  TestCout( _T("TEST #1 - filling JSON array and object types ... \n"));

  valueArray.Append( 20 );
  valueArray.Append( (unsigned int) 30 );
  valueArray.Append( false );
  valueArray.Append( 20.30 );
  valueArray.Append( _T("Another static C string") );
  valueArray.Append( wxString( _T("wxString 2")));
  asString = valueArray.AsString();
  ASSERT( asString == _T(""));

  valueMap[_T("integer")]      = 20;
  valueMap[_T("unsigned int")] =  (unsigned int) 30;
  valueMap[_T("bool")]         = true;
  valueMap[_T("double")]       = 20.30;
  valueMap[_T("C string")]     = _T("static string 3");
  valueMap[_T("wxString")]     = _T("wxString 3");
  asString = valueMap.AsString();
  ASSERT( asString == _T(""));

  TestCout( _T("..................OK\n" ));

  info = valueMap.GetInfo();
  TestCout( _T("Info about \'valueMap\'\n"));
  TestCout( info );

  TestCout( _T("\nTEST # 1 - checking returned types ... \n"));
  int r;
  r = valueEmpty.IsEmpty();
  ASSERT( r == true );
  r = valueNull.IsNull();
  ASSERT( r == true );
  r = valueInt.IsInt();
  ASSERT( r == true );
  r = valueUInt.IsUInt();
  ASSERT( r == true );
  r = valueBool.IsBool();
  ASSERT( r == true );
  r = valueDouble.IsDouble();
  ASSERT( r == true );
  r = valueCString.IsCString();
  ASSERT( r == false );
  // ASSERT( r == true )        // only if WXJSON_USECSTRING is defined
  r = valueString.IsString();
  ASSERT( r == true );
  r = valueArray.IsArray();
  ASSERT( r == true );
  r = valueMap.IsObject();
  ASSERT( r == true );

 // check this test: should be TRUE
  r = valueNull2.IsNull();
  ASSERT( r == true );

  TestCout( _T("..................OK\n" ));

  TestCout( _T( "TEST #1 - checking the returned values ... \n"));

  // we cannot check the return value of a NULL value: it is garbage
  // valueNull.AsInt();

  int i = valueInt.AsInt();
  ASSERT( i == 10 );
  unsigned int ui = valueUInt.AsUInt();
  ASSERT( ui == 20 );

  // checking int and unsigned int as other types
  ui = valueInt.AsUInt();
  i  = valueUInt.AsInt();
  ASSERT( ui == 10 );
  ASSERT( i == 20 );

  bool b = valueBool.AsBool();
  ASSERT( b == true );
  double d = valueDouble.AsDouble();
  ASSERT( d == 10.20 );

  // checking int and uint AsDouble()
  d = valueInt.AsDouble();
  ASSERT( d == 10 );
  d = valueUInt.AsDouble();
  ASSERT( d == 20 );

#if defined( NOT_DEFINED )
  //  only if WXJSON_USECSTRING is defined
  const wxChar* cStr = valueCString.AsCString();
  i = strcmp( cStr, buff1 );
  ASSERT( i == 0 )
#endif

  wxString wxStr1 = valueCString.AsString();
  ASSERT( wxStr1 == _T("This is a static C string" ));
  wxString wxStr2 = valueString.AsString();
  ASSERT( wxStr2 == _T("wxString 1" ));

  // checking the string concatention
  valueString.Cat( _T("\nappended string" ));
  wxStr2 = valueString.AsString();
  ASSERT( wxStr2 == _T("wxString 1\nappended string" ));
  TestCout( _T("..................OK\n" ));

#if defined( NOT_DEFINED )
  //
  // ASSERTION failures in debug builds: try to get the wrong type
  //
  TestCout( _T("ASSERTION failures: should not print OK ... \n"));
  b    = valueDouble.AsBool();
  i    = valueString.AsInt();
  d    = valueArray.AsDouble();
  // althrough the 'valueCString' was constructed using a static
  // C string, the definition of the USE_WXSTRING macro at the
  // beginning of the source file force the wxJSONValue class
  // to make  copy of the string and to store it as a wxString
  // object so we cannot get the AsCString() pointer. 
  const wxChar* cStr = valueCString.AsCString();
  TestCout( _T("..................OK\n" ));
#endif

  TestCout( _T("Constructing a JSON value tree ... \n"));

  // the 'valueMap' value is the root node
  // and another item is added which contains the array
  valueMap[_T("object")] = valueArray;

  // another object is created and this object is a object itself
  valueMap[_T("font")][_T("face")] = _T("Arial");
  valueMap[_T("font")][_T("size")] = 12;
  valueMap[_T("font")][_T("bold")] = true;
  valueMap[_T("font")][_T("coordinate")] = 90.67;
  valueMap[_T("font")][_T("encoding")] = wxJSONValue( wxJSONTYPE_NULL);

  wxJSONValue lang;
  lang.Append( _T("it"));
  lang.Append( _T("en"));
  lang.Append( _T("de"));
  lang.Append( _T("is"));
  valueMap[_T("font")][_T("lang")] = lang;

  // try to print the member names of the root value
  TestCout( _T("Printing the member names (root node) ... \n"));
  wxArrayString strArray = valueMap.GetMemberNames();
  for ( int i = 0; i < strArray.size(); i++ )  {
    TestCout( _T("....."));
    TestCout( strArray[i]);
    TestCout( _T("\n"));
  }
  TestCout( _T("..................OK\n" ));

  TestCout( _T("Accessing values in the JSON object ... \n"));

  i = valueMap[_T("integer")].AsInt();
  ASSERT( i == 20 );
  i = valueMap[_T("font")][_T("size")].AsInt();
  ASSERT( i == 12 );
  b = valueMap[_T("font")][_T("bold")].AsBool();
  ASSERT( b == true );
  r = valueMap[_T("font")][_T("encoding")].IsNull();
  ASSERT( r == true );
  wxStr1 = valueMap[_T("font")][_T("lang")][1].AsString();  // "it" = 0, "en", "de", "is"
  ASSERT( wxStr1 == _T("en"));
  TestCout( _T("..................OK\n" ));


  TestCout( _T("Checking the HasMember() function ... \n"));

  r = valueMap.HasMember( _T("object"));
  ASSERT( r == true );
  r = valueMap[_T("font")][_T("lang")].HasMember( _T("bold"));  // false: is an array
  ASSERT( r == false );
  r = valueMap[_T("font")][_T("lang")].HasMember( 3 );  // 4 elements in array
  ASSERT( r == true );
  r = valueMap[_T("font")][_T("lang")].HasMember( 4 );  // 4 elements in array
  ASSERT( r == false );

  r = valueMap[_T("font")].HasMember( _T("bold") );
  ASSERT( r == true );
  r = valueMap[_T("font")].HasMember( _T("no_member") );
  ASSERT( r == false );
  r = valueMap[_T("font")].HasMember( 2 );  // 6 members
  ASSERT( r == true );
  r = valueMap[_T("font")].HasMember( 7 );  // 6 members
  ASSERT( r == false );
  TestCout( _T("..................OK\n" ));

  TestCout( _T( "\'TEST #1\' successfully completed\n\n"));
  return 0;
}


/*
{
}
*/


