/******************************************************************************

   test6.cpp

    test for the comment-related features
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

 Test18(): calls the AddComment() function in various form and ASSERTs
           the results
 Test19(): use TestValue() to fill a JSON object with values and comments
           then writes to a string using style wxJSONWRITER_STYLED
 Test20(): use TestValue() to fill a JSON object with values and comments
           then writes to a string using style wxJSONWRITER_NONE
 Test21(): use TestValue() to fill a JSON object with values and comments
           then writes to a string using style wxJSONWRITER_WRITE_COMMENTS
 Test22(): use TestValue() to fill a JSON object with values and comments
           then writes to a string using style wxJSONWRITER_COMMENTS_BEFORE
 Test23(): use TestValue() to fill a JSON object with values and comments
           then writes to a string using style wxJSONWRITER_COMMENTS_AFTER
 Test24(): test the wxJSONWRITER_MULTISTRING style: writing multiple-line
           strings

 jul 2008 (vers. 1.0.0)
 --------
 Tests were successfull

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


// test the wxJSONValue's memberfunctions
int Test18()
{
  // two comment lines
  static const wxChar* cp1 = _T("// C++ comment: one line LF terminated\n");
  static const wxChar* cp2 = _T("// C++ comment: one line not LF terminated");

  // a multiline comment
  static const wxChar* mc1 = _T("// C++ comment: first line\n");
  static const wxChar* mc2 = _T("// C++ comment: second line");
  static const wxChar* mc3 = _T("// C++ comment: third line\n");

  // also a multiline comment
  static const wxChar* mc4 = _T("// C++ comment: first line\n"
				"// C++ comment: second line\n"
				"// C++ comment: third line\n");

  // a C-style comment
  static const wxChar* c1  = _T("/* C-style comment: one line not LF terminated*/");
  static const wxChar* c2  = _T("/* C-style comment: one line LF terminated*/\n");
  static const wxChar* c3  = _T("/* C-style comment: LF terminated    */     \n");

  static const wxChar* c4  = _T("/* multi-line C-style comment (line 1)\n"
 				"multi-line C-style comment (line 2)\n"
  				"multi-line C-style comment (line 3)*/\n");

  // not valid comments
  static const wxChar* c10 = _T("/ not valid comment");
  static const wxChar* c11 = _T("-- not valid comment");
  static const wxChar* c12 = _T("/* not valid comment*");
  static const wxChar* c13 = _T("/* not valid comment/");

  // bugs: the following are not-valid comments but the AddComment()
  // function is not able to catch them and it returns OK
  static const wxChar* c14 = _T("/* not valid comment*/  comment */");
  static const wxChar* c15 = _T("// this is OK C++ comment\nthis is not a valid line\n");


  int r; wxString cmt; int cmtCount;

  wxJSONValue v1( 10 );
  r = v1.AddComment( cp1 );
  ASSERT( r >= 0 )
  // the first comment is as we added it
  cmt = v1.GetComment( 0 );
  ASSERT( cmt == cp1 )

  wxJSONValue v2( _T("value v2"));
  r = v2.AddComment( cp2 );
  ASSERT( r >= 0 )
  // the second comment will have a trailing LF
  cmt = v2.GetComment( 0 );
  wxString v2cmt( cp2 );
  v2cmt.append( 1, '\n' ); 
  ASSERT( cmt == v2cmt )

  // add a multiline comment
  wxJSONValue v3( 20.10 );
  r = v3.AddComment( mc1 );
  ASSERT( r >= 0 )
  r = v3.AddComment( mc2 );
  ASSERT( r >= 0 )
  r = v3.AddComment( mc3 );
  ASSERT( r >= 0 )
  cmtCount = v3.GetCommentCount();
  ASSERT( cmtCount == 3 )
  // get the comment as a unique string
  cmt = v3.GetComment( -1 );
  // construct the result
  wxString v3cmt( mc1 );
  v3cmt.append( mc2 ); 
  v3cmt.append( 1, '\n' ); 
  v3cmt.append( mc3 ); 
  ASSERT( cmt == v3cmt )

  // another multiline comment in a unique string
  wxJSONValue v9( 9 );
  r = v9.AddComment( mc4 );
  cmtCount = v9.GetCommentCount();
  ASSERT( cmtCount == 1 )
  // get the comment as a unique string
  cmt = v3.GetComment( -1 );
  ASSERT( cmt == mc4 )

  // add C-style comments
  wxJSONValue v5( 5 );
  r = v5.AddComment( c1 );
  ASSERT( r >= 0 )
  cmt = v5.GetComment();
  ASSERT( cmt == c1 )

  wxJSONValue v6( 6 );
  r = v6.AddComment( c2 );
  ASSERT( r >= 0 )
  cmt = v6.GetComment();
  ASSERT( cmt == c2 )

  wxJSONValue v7( 7 );
  r = v7.AddComment( c3 );
  ASSERT( r >= 0 )
  cmt = v7.GetComment();
  ASSERT( cmt == c3 )

  // multi-line C-style comment
  wxJSONValue v8( 8 );
  r = v8.AddComment( c4 );
  ASSERT( r >= 0 )
  cmt = v8.GetComment();
  ASSERT( cmt == c4 )


  // try to add not valid comment's strings
  wxJSONValue v4( true );
  r = v4.AddComment( c10 );
  ASSERT( r < 0 )
  r = v4.AddComment( c11 );
  ASSERT( r < 0 )
  r = v4.AddComment( c12 );
  ASSERT( r < 0 )
  r = v4.AddComment( c13 );
  ASSERT( r < 0 )
  r = v4.AddComment( c14 );
  // ASSERT( r < 0 )             // BUG
  r = v4.AddComment( c15 );
  // ASSERT( r < 0 )             // BUG

  return 0;
}


// the function fills a wxJSONValue object as explained
// in the function's description comments
// the function is used in Test19, Test20, Test21
void TestValue( wxJSONValue& root )
{

// the following lines shows the expected result
//
#if defined( NOT_DEFINED )
// C++ comment (first line) 
// C++ comment (second line)
// C++ comment (third line)
{
   // comment for the 'obj1' value (line-1)
   // comment for the 'obj1' value (line-2)
   "obj1" : {
      "1value-1" : 10, // 'value-1' inline C++ comment
      "1value-2" : 20, // 'value-2' inline C++ comment
      "1value-3" : 30, /* 'value-3' inline C comment  */
      "1value-4" : 40,
      /*****************************
         C style comment for 2array
      *****************************/
      "2array" : [
        // item-1 comment
        "item-1",
        // item-2 comment // item-2 comment // item-2 comment
        "item-2",
        "item-3",
        "item-4",
        // item-5 is an object composed of four name/value pairs
        {
           "key-1" : 1001,
           "key-2" : 1002, // comment for key-2 (inline)
           // comment for key-3 (before)
           "key-3" : 1003,
           "key-4" : 1004
           // comment for key-4 (after)
        }
      ]
   },
   "obj2" : { // an in-line comment for an object value
      "2value-1" : 10,
      "2value-2" : 20,
      // comment AFTER for 2-value2
      "2value-3" : 30,
      "2value-4" : {
         "2value-4-1" : "4-1", // inline comment for 4-1
         "2value-4-2" : "4-2", /* inline comment for 4-2   */
         /* C-style comment before 2value-4-3  */
         "2value-4-3" : "4-3",
         "2value-4-4" : "4-4" // inline comment for 4-4
      }
   },
   "array2" : [ // inline comment for array2 (an empty array)
   ],
   "array3" : [ // inline comment for array3
      1,
      2,
      3
   ]
}
#endif    // defined( NOT_DEFINED)

  root.AddComment( _T("// C++ comment (first line)\n"));
  root.AddComment( _T("// C++ comment (second line)\n"));
  root.AddComment( _T("// C++ comment (third line)\n"), wxJSONVALUE_COMMENT_BEFORE );

  wxJSONValue& obj1 = root[_T("obj1")];
  obj1.AddComment( _T("// comment for the 'obj1' value (line-1)\n"));
  obj1.AddComment( _T("// comment for the 'obj1' value (line-2)\n"));

  root[_T("obj1")][_T("1value-1")] = 10;
  root[_T("obj1")][_T("1value-1")].AddComment( _T("// \'value-1\' inline C++ comment\n"),
							 wxJSONVALUE_COMMENT_INLINE );

  root[_T("obj1")][_T("1value-2")] = 20;
  root[_T("obj1")][_T("1value-2")].AddComment( _T("// \'value-2\' inline C++ comment\n"),
							 wxJSONVALUE_COMMENT_INLINE );

  root[_T("obj1")][_T("1value-3")] = 30;
  root[_T("obj1")][_T("1value-3")].AddComment( _T("/* \'value-3\' inline C comment   */\n"),
							 wxJSONVALUE_COMMENT_INLINE );

  root[_T("obj1")][_T("1value-4")] = 40;

  wxJSONValue array;
  array.AddComment( _T("/*****************************\n"
				"         C style comment for 2array\n"
				"      *****************************/"));

  array[0] = _T("item-1");
  array[0].AddComment( _T("// item-1 comment"));
  array[1] = _T("item-2");
  array[2] = _T("item-3");
  array[3] = _T("item-4");

  // the fifth item is an object
  array[4].AddComment( _T("// item-5 is an object composed of four name/value pairs"));

  array[4][_T("key-1")] = 1001;
  array[4][_T("key-2")] = 1002;
  array[4][_T("key-2")].AddComment( _T("// comment for key-2 (inline)\n"),
								 wxJSONVALUE_COMMENT_INLINE );

  array[4][_T("key-3")] = 1003;
  array[4][_T("key-3")].AddComment( _T("// comment for key-3 (before)"));

  array[4][_T("key-4")] = 1004;
  array[4][_T("key-4")].AddComment( _T("// comment for key-4 (after)"),
								 wxJSONVALUE_COMMENT_AFTER );

  root[_T("obj1")][_T("2array")] = array;

  wxJSONValue obj2;
  obj2.AddComment( _T("// an in-line comment for obj2 value"),
						wxJSONVALUE_COMMENT_INLINE );
  obj2[_T("2value-1")] = 10;
  obj2[_T("2value-2")] = 20;
  obj2[_T("2value-3")] = 30;
  obj2[_T("2value-4")] = 40;

  obj2[_T("2value-2")].AddComment( _T("//comment AFTER for 2-value2"), wxJSONVALUE_COMMENT_AFTER );

  obj2[_T("2value-4")][_T("2value-4-1")] = _T("4-1");
  obj2[_T("2value-4")][_T("2value-4-2")] = _T("4-2");
  obj2[_T("2value-4")][_T("2value-4-3")] = _T("4-3");
  obj2[_T("2value-4")][_T("2value-4-4")] = _T("4-4");

  obj2[_T("2value-4")][_T("2value-4-1")].AddComment( _T("// inline comment for 4-1"),
						wxJSONVALUE_COMMENT_INLINE );
  obj2[_T("2value-4")][_T("2value-4-2")].AddComment( _T("// inline comment for 4-2"),
						wxJSONVALUE_COMMENT_INLINE );
  obj2[_T("2value-4")][_T("2value-4-3")].AddComment( _T("/* C-style comment before 2value-4-3 */"),
						wxJSONVALUE_COMMENT_BEFORE );
  obj2[_T("2value-4")][_T("2value-4-4")].AddComment( _T("// inline comment for 4-4"),
						wxJSONVALUE_COMMENT_INLINE );
  root[_T("obj2")] = obj2;

  // the empty 'array2' array has INLINE comment
  root[_T("array2")].SetType( wxJSONTYPE_ARRAY );
  root[_T("array2")].AddComment( _T("// inline comment for array2 (empty)"),
						wxJSONVALUE_COMMENT_INLINE );

  // the 'array3' array has INLINE comment
  root[_T("array3")][0] = 1;
  root[_T("array3")][1] = 2;
  root[_T("array3")][2] = 3;
  root[_T("array3")].AddComment( _T("// inline comment for array3"),
						wxJSONVALUE_COMMENT_INLINE );

}


// write the root JSON value to a string using style wxJSONWRITER_STYLED
// which will write indentation but no comments
// 23/11/2007 OK, seems all right
int Test19()
{
  TestCout( _T("Testing STYLE=STYLED\n"));
  wxJSONValue root;
  TestValue( root );

  wxJSONWriter writer;
  wxString s;
  writer.Write( root, s );
  TestCout( s );

  return 0;
}

// write the root JSON value to a string using style wxJSONWRITER_NONE
// which will write without indentation and without comments
int Test20()
{
  TestCout( _T("\n\nTesting STYLE=NONE\n"));
  wxJSONValue root;
  TestValue( root );

  wxJSONWriter writer(wxJSONWRITER_NONE);
  wxString s;
  writer.Write( root, s );
  TestCout( s );

  return 0;
}

// write the root JSON value to a string using style wxJSONWRITER_WRITE_COMMENTS
// which will write indentation and comments. Comments are written in the
// position they were added to the value they refer to.
int Test21()
{
  TestCout( _T("\n\nTesting STYLE=WRITE_COMMENT\n"));
  wxJSONValue root;
  TestValue( root );

  wxJSONWriter writer( wxJSONWRITER_STYLED | wxJSONWRITER_WRITE_COMMENTS );
  wxString s;
  writer.Write( root, s );
  TestCout( s );
  return 0;
}


// write the root JSON value to a string using style wxJSONWRITER_COMMENTS_BEFORE
int Test22()
{
  TestCout( _T("\n\nTesting STYLE=WRITE_COMMENT COMMENT_BEFORE\n"));
  wxJSONValue root;
  TestValue( root );

  wxJSONWriter writer( wxJSONWRITER_STYLED
                     | wxJSONWRITER_WRITE_COMMENTS
                     | wxJSONWRITER_COMMENTS_BEFORE);
  wxString s;
  writer.Write( root, s );
  TestCout( s );

  return 0;
}

int Test23()
{
  TestCout( _T("\n\nTesting STYLE=WRITE_COMMENT COMMENT_AFTER\n"));
  wxJSONValue root;
  TestValue( root );

  wxJSONWriter writer( wxJSONWRITER_STYLED
                     | wxJSONWRITER_WRITE_COMMENTS
                     | wxJSONWRITER_COMMENTS_AFTER);
  wxString s;
  writer.Write( root, s );
  TestCout( s );

  return 0;
}

// testing the MULTISTRING style
int Test24()
{
  wxJSONValue v = _T("This is a multiline string\nthis is line #2\nthis is line #3");
  wxJSONValue root;
  root[_T("string")] = v;
  wxJSONWriter writer( wxJSONWRITER_STYLED
                       | wxJSONWRITER_SPLIT_STRING );

  TestCout( _T("\nTest24(): STYLE=SPLIT_STRING\n"));
  wxString s;
  writer.Write( root, s );
  TestCout( s );

  TestCout( _T("\nTest24(): STYLE=STYLED\n"));
  wxJSONWriter wrt( wxJSONWRITER_STYLED );
  wxString s2;
  wrt.Write( root, s2 );
  TestCout( s2 );

  return 0;
}



/*
{
}
*/


