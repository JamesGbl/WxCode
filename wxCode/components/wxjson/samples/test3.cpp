/******************************************************************************

   test3.cpp

    test for the wxJSONWriter class
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


#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/app.h>
#include <wx/log.h>
#include <wx/debug.h>
#include <wx/wfstream.h>

#include <wx/jsonval.h>
#include <wx/jsonwriter.h>
#include "test.h"

// a simple write test
int Test2()
{
  wxJSONValue root;
  root[_T("integer")]      = 20;
  root[_T("unsigned int")] = (unsigned int) 30;
  root[_T("bool")]         = true;
  root[_T("double")]       = 20.30;
  root[_T("C string")]     = _T("static string 3");
  root[_T("wxString")]     = _T("wxString 3");

  root[_T("font")][_T("face")] = _T("Arial");
  root[_T("font")][_T("size")] = 12;
  root[_T("font")][_T("bold")] = true;
  root[_T("font")][_T("double")] = 90.67;
  root[_T("font")][_T("encoding")] = wxJSONValue( wxJSONTYPE_NULL);

  wxJSONValue lang;
  lang.Append( _T("it") );
  lang.Append( _T("en") );
  lang.Append( _T("de") );
  lang.Append( _T("is") );
  root[_T("font")][_T("locale")] = lang;

  // this is the same as above
  //root[_T("font")][_T("locale")].Append( "it" );
  //root[_T("font")][_T("locale")].Append( "en" );
  //root[_T("font")][_T("locale")].Append( "de" );
  //root[_T("font")][_T("locale")].Append( "is" );

  // now write the JSON object to a string object
  wxString str;
  wxJSONWriter wrt( gs_indentation );
  wrt.Write( root, str );
  TestCout( str );
  TestCout( _T( "<-----------------\n"));
  return 0;
}

// test escaped chars
int Test3()
{
  wxJSONValue root;
  root[_T("backslash")]    = _T("string with \\ (backslash)");
  root[_T("linefeed")]     = _T("string with \n (linefeed)");
  root[_T("slash")]        = _T("string with / (slash)");
  root[_T("carriage-ret")] = _T("string with \r (CR)");
  root[_T("tab")]          = _T("string with \t (tab)");
  root[_T("backspace")]    = _T("string with \b (backspace)");
  root[_T("quotes")]       = _T("string with \" (quotes)");
  root[_T("3 CR string")]  = _T("string with 3 CR\r1-\r 2-\r");

  wxString str;
  wxJSONWriter wrt(  gs_indentation );
  wrt.Write( root, str );
  TestCout( str );
  TestCout( _T( "<-----------------\n"));
  return 0;
}

// prints the 'null', 'empty' and 'empty map' empty array' objects
int Test4()
{
  wxJSONValue valueNull;
  wxJSONValue valueEmpty( wxJSONTYPE_INVALID );
  wxJSONValue valueEmptyMap( wxJSONTYPE_OBJECT );
  wxJSONValue valueEmptyArray( wxJSONTYPE_ARRAY );

  wxJSONValue root;
  root.Append( valueNull );

  // root.Append( valueEmpty );   // 24/7/08 ASSERTION failure in debug builds
  root.Append( valueEmptyMap );
  root.Append( valueEmptyArray );

  wxString str;
  wxJSONWriter wrt( gs_indentation );
  wrt.Write( root, str );
  TestCout( str );
  TestCout( _T( "<-----------------\n"));
  return 0;
}

// an array of objects as root
int Test5()
{
  wxJSONValue root;
  root.Append( _T("a string") );
  root.Append( 12 );
  root.Append( true );
  root.Append( wxJSONValue());   // this is a 'null' value

  root.Append( 90e+12 );

  wxString str;
  wxJSONWriter wrt( gs_indentation );
  wrt.Write( root, str );
  TestCout( str );
  TestCout( _T( "<-----------------\n"));
  return 0;
}

// a complex JSON object which will be used to
// test the JSON reader (this output lacks comments
// which will be added by hand)
int Test6()
{
  wxJSONValue root;

  // the 'object' object contains all primitive types
  root[_T("object")][_T("integer")]      = 20;
  root[_T("object")][_T("unsigned int")] = (unsigned int) 30;
  root[_T("object")][_T("bool-t")]       = true;
  root[_T("object")][_T("bool-f")]       = false;
  root[_T("object")][_T("double")]       = 20.30;
  root[_T("object")][_T("C string")]     = _T("static string 1");
  root[_T("object")][_T("wxString")]     = _T("wxString 1");
  root[_T("object")][_T("null")]         = wxJSONValue( wxJSONTYPE_NULL);


  // the 'array' object contains an array of primitive types
  root[_T("array")].Append( 30 );
  root[_T("array")].Append( (unsigned int ) 40);
  root[_T("array")].Append( true );
  root[_T("array")].Append( false );
  root[_T("array")].Append( 30.40 );
  root[_T("array")].Append( _T("static string 2") );
  root[_T("array")].Append( wxString( _T("wxString 2")));
  wxJSONValue obj;
  obj[_T("key")] = _T("value");
  root[_T("array")].Append( obj );

  // the void-object' object contains an empty object
  root[_T("void-object")] = wxJSONValue( wxJSONTYPE_OBJECT );

  // the 'void-array' array contains an empty array
  root[_T("void-array")] = wxJSONValue( wxJSONTYPE_ARRAY );

  // an array of objects
  root[_T("array2")][0][_T("item")] = 1;
  root[_T("array2")][1][_T("item")] = 2;
  root[_T("array2")][2][_T("item")] = 3;
  root[_T("array2")][3][_T("item")] = 4;
  root[_T("array2")][4][_T("item")] = 5;

  // an object that contains escaped chars strings
  root[_T("escaped")][_T("backslash")]    = _T("string with \\ (backslash)");
  root[_T("escaped")][_T("linefeed")]     = _T("string with \n (linefeed)");
  root[_T("escaped")][_T("slash")]        = _T("string with / (slash)");
  root[_T("escaped")][_T("carriage-ret")] = _T("string with \r (CR)");
  root[_T("escaped")][_T("tab")]          = _T("string with \t (tab)");
  root[_T("escaped")][_T("backspace")]    = _T("string with \b (backspace)");
  root[_T("escaped")][_T("quotes")]       = _T("string with \" (quotes)");
  root[_T("escaped")][_T("3 CR string")]  = _T("string with 3 CR\r1-\r 2-\r");

  // and finally an array that contains escaped chars strings
  root[_T("escaped-array")].Append( _T("string with \\ (backslash)"));
  root[_T("escaped-array")].Append( _T("string with \n (linefeed)"));
  root[_T("escaped-array")].Append( _T("string with / (slash)"));
  root[_T("escaped-array")].Append( _T("string with \r (CR)"));
  root[_T("escaped-array")].Append( _T("string with \t (tab)"));
  root[_T("escaped-array")].Append( _T("string with \b (backspace)"));
  root[_T("escaped-array")].Append( _T("string with \" (quotes)"));
  root[_T("escaped-array")].Append( _T("string with 3 CR\r1-\r 2-\r"));

  wxString str;
  wxJSONWriter wrt( gs_indentation );
  wrt.Write( root, str );
  TestCout( str );
  TestCout( _T( "<-----------------\n"));
  return 0;
}

// check that what I wrote in the docs in really true...
// the output should be:
//  {
//    "key-1" :  {
//       "key-2" :  {
//          "key-3" : [
//             null,
//             null,
//             null,
//             null,
//             12
//          ]
//        }
//     }
//  }
int Test7()
{
  wxJSONValue root;
  root[_T("key-1")][_T("key-2")][_T("key-3")][4] = 12;
  wxString str;

  wxJSONWriter wrt( gs_indentation );
  wrt.Write( root, str );
  TestCout( str );
  TestCout( _T( "<-----------------\n"));
  return 0;
}



/*
{
}
*/

