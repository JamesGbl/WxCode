/******************************************************************************

   test7.cpp

    comment related functions: memberfunctions, writer and reader
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


// the function fills a wxJSONValue array as explained
// in the function's description comments
// the stored value is used in various test functions
static void StoreValue( wxJSONValue& root )
{

// the following lines shows what are the values stored in 'root'
//
#if defined( NOT_DEFINED )
// C++ comment for root (first line) 
// C++ comment for root (second line)
// C++ comment for root (third line)
[
   // comment for value-0 (line-1)
   // comment for value-0 (line-2)
   "value 0",
   [
      "value 1.0", // 'value 1.0' inline C++ comment
      "value 1.1", // 'value 1.1' inline C++ comment
      "value 1.2", /* 'value 1.2' inline C comment  */
      "value 1.3"
   ],
   
   /*****************************
     C style comment for value-2
   *****************************/
   "value 2",       
   
   "value 3" : [ // inline comment for value 3 (an empty array)
   ],
   "value 4" : [ // inline comment for value 4
      1,
      2,
      3
   ],
   "value 5"
   // comment after value 5
]
#endif    // defined( NOT_DEFINED)

  root.AddComment( _T("// C++ comment for root (first line)\n"));
  root.AddComment( _T("// C++ comment for root (second line)\n"));
  root.AddComment( _T("// C++ comment for root (third line)\n"), wxJSONVALUE_COMMENT_BEFORE );

  // value 0
  root.Append( _T("value 0"));
  root[0].AddComment( _T( "// comment for value-0 (line-1)"), wxJSONVALUE_COMMENT_BEFORE);
  root[0].AddComment( _T( "// comment for value-0 (line-2)"));

  // value 1
  root[1][0] = _T( "value 1.0");
  root[1][1] = _T( "value 1.1");
  root[1][2] = _T( "value 1.2");
  root[1][3] = _T( "value 1.3");
  root[1][0].AddComment( _T( "// 'value 1.0' inline C++ comment"), wxJSONVALUE_COMMENT_INLINE);
  root[1][1].AddComment( _T( "// 'value 1.1' inline C++ comment"), wxJSONVALUE_COMMENT_INLINE);
  root[1][2].AddComment( _T( "/* 'value 1.2' inline C comment  */"), wxJSONVALUE_COMMENT_INLINE);

  // value 2
  root.Append( _T( "value 2" ));
  root[2].AddComment( _T("/*****************************\n"
				"multiline C style comment for value 2\n"
				"   *****************************/"));

  // value 3 (an empty subarray)
  wxJSONValue a( wxJSONTYPE_ARRAY );
  a.AddComment( _T("// inline comment for value 3 (an empty array)"),
						wxJSONVALUE_COMMENT_INLINE );

  root.Append( a );

  // value 4 (a subarray of 3 items
  wxJSONValue a2;
  a2.AddComment( _T("// inline comment for value 4"), wxJSONVALUE_COMMENT_INLINE );
  a2.Append( 1 );
  a2.Append( 2 );
  a2.Append( 3 );
  root.Append( a2 );
  
  // value 5 (comment after)
  root.Append( _T("value 5"));
  root[5].AddComment( _T( "// comment after value 5"), wxJSONVALUE_COMMENT_AFTER );
  
  // another multiline C-style comment before
  // value 6
  root.Append( _T( "value 6" ));
  root[6].AddComment( _T("/*****************************************/\n" ));
  root[6].AddComment( _T("/* multiline C style comment for value 6 */\n" ));
  root[6].AddComment( _T("/*****************************************/\n" ));
  
  
}



// test the wxJSONValue's memberfunctions
int Test10_1()
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


// write the root JSON value to a string using style wxJSONWRITER_STYLED
// which will write indentation but no comments
int Test10_2()
{
	static const wxChar* expectedResult = _T( "[\n"
		"   \"value 0\",\n"
		"   [\n"
		"      \"value 1.0\",\n"
		"      \"value 1.1\",\n"
		"      \"value 1.2\",\n"
		"      \"value 1.3\"\n"
		"   ],\n"
		"   \"value 2\",\n"
		"   [\n"
		"   ],\n"
		"   [\n"
		"      1,\n"
		"      2,\n"
		"      3\n"
		"   ],\n"
		"   \"value 5\",\n"
		"   \"value 6\"\n"
		"]\n" );


	wxJSONValue root;
	StoreValue( root );

	wxJSONWriter writer;
	wxString s;
	writer.Write( root, s );
	TestCout( s );
	
	ASSERT( s == expectedResult );
	return 0;
}


// write the root JSON value to a string using style wxJSONWRITER_NONE
// which will write without indentation and without comments
int Test10_3()
{
	static const wxChar* expectedResult = _T(
		"[\"value 0\",[\"value 1.0\",\"value 1.1\",\"value 1.2\",\"value 1.3\"],"
		"\"value 2\",[],[1,2,3],\"value 5\",\"value 6\"]" );

	wxJSONValue root;
	StoreValue( root );

	wxJSONWriter writer(wxJSONWRITER_NONE);
	wxString s;
	writer.Write( root, s );
	TestCout( s );

	ASSERT( s == expectedResult );
	return 0;
}


// write the root JSON value to a string using style wxJSONWRITER_WRITE_COMMENTS
// which will write indentation and comments. Comments are written in the
// position they were added to the value they refer to.
int Test10_4()
{
	static const wxChar* expectedResult = _T(
		"// C++ comment for root (first line)\n"
		"// C++ comment for root (second line)\n"
		"// C++ comment for root (third line)\n"
		"[\n"
		"   // comment for value-0 (line-1)\n"
		"   // comment for value-0 (line-2)\n"
		"   \"value 0\",\n"
		"   [\n"
		"      \"value 1.0\",\t// 'value 1.0' inline C++ comment\n"
		"      \"value 1.1\",\t// 'value 1.1' inline C++ comment\n"
		"      \"value 1.2\",\t/* 'value 1.2' inline C comment  */\n"
		"      \"value 1.3\"\n"
		"   ],\n"
		"   /*****************************\n"
		"multiline C style comment for value 2\n"
		"   *****************************/\n"
		"   \"value 2\",\n"
		"   [	// inline comment for value 3 (an empty array)\n"
		"   ],\n"
		"   [	// inline comment for value 4\n"
		"      1,\n"
		"      2,\n"
		"      3\n"
		"   ],\n"
		"   \"value 5\",\n"
		"   // comment after value 5\n"
		"   /*****************************************/\n"
		"   /* multiline C style comment for value 6 */\n"
		"   /*****************************************/\n"
		"   \"value 6\"\n"
		"]\n" );

    wxJSONValue root;
    StoreValue( root );

    wxJSONWriter writer( wxJSONWRITER_STYLED | wxJSONWRITER_WRITE_COMMENTS );
    wxString s;

	int r = CheckResult( s, expectedResult );
    return r;
}

// write the root JSON value to a string using style wxJSONWRITER_COMMENTS_BEFORE
int Test10_5()
{
	static const wxChar* expectedResult = _T( 
		"// C++ comment for root (first line)\n"
		"// C++ comment for root (second line)\n"
		"// C++ comment for root (third line)\n"
		"[\n"
		"   // comment for value-0 (line-1)\n"
		"   // comment for value-0 (line-2)\n"
		"   \"value 0\",\n"
		"   [\n"
		"      // 'value 1.0' inline C++ comment\n"
		"      \"value 1.0\",\n"
		"      // 'value 1.1' inline C++ comment\n"
		"      \"value 1.1\",\n"
		"      /* 'value 1.2' inline C comment  */\n"
		"      \"value 1.2\",\n"
		"      \"value 1.3\"\n"
		"   ],\n"
		"   /*****************************\n"
		"multiline C style comment for value 2\n"
		"   *****************************/\n"
		"   \"value 2\",\n"
		"   // inline comment for value 3 (an empty array)\n"
		"   [\n"
		"   ],\n"
		"   // inline comment for value 4\n"
		"   [\n"
		"      1,\n"
		"      2,\n"
		"      3\n"
		"   ],\n"
		"   // comment after value 5\n"
		"   \"value 5\",\n"
		"   /*****************************************/\n"
		"   /* multiline C style comment for value 6 */\n"
		"   /*****************************************/\n"
		"   \"value 6\"\n"
		"]\n" );


	wxJSONValue root;
	StoreValue( root );

	wxJSONWriter writer( wxJSONWRITER_STYLED
				| wxJSONWRITER_WRITE_COMMENTS
				| wxJSONWRITER_COMMENTS_BEFORE);
	wxString s;
	writer.Write( root, s );

	int r = CheckResult( s, expectedResult );
    return r;
}

// COMMENTS_AFTER
int Test10_6()
{
	static const wxChar* expectedResult = _T(
		"[\n"
		"   \"value 0\",\n"
		"   // comment for value-0 (line-1)\n"
		"   // comment for value-0 (line-2)\n"
		"   [\n"
		"      \"value 1.0\",\n"
		"      // 'value 1.0' inline C++ comment\n"
		"      \"value 1.1\",\n"
		"      // 'value 1.1' inline C++ comment\n"
		"      \"value 1.2\",\n"
		"      /* 'value 1.2' inline C comment  */\n"
		"      \"value 1.3\"\n"
		"   ],\n"
		"   \"value 2\",\n"
		"   /*****************************\n"
		"multiline C style comment for value 2\n"
		"   *****************************/\n"
		"   [\n"
		"   ],\n"
		"   // inline comment for value 3 (an empty array)\n"
		"   [\n"
		"      1,\n"
		"      2,\n"
		"      3\n"
		"   ],\n"
		"   // inline comment for value 4\n"
		"   \"value 5\",\n"
		"   // comment after value 5\n"
		"   \"value 6\"\n"
		"   /*****************************************/\n"
		"   /* multiline C style comment for value 6 */\n"
		"   /*****************************************/\n"
		"]\n"
		"// C++ comment for root (first line)\n"
		"// C++ comment for root (second line)\n"
		"// C++ comment for root (third line)\n"
	);

	wxJSONValue root;
	StoreValue( root );

	wxJSONWriter writer( wxJSONWRITER_STYLED
				| wxJSONWRITER_WRITE_COMMENTS
				| wxJSONWRITER_COMMENTS_AFTER);
	wxString s;
	writer.Write( root, s );
	
	int r = CheckResult( s, expectedResult );
    return r;
}



/*
{
}
*/


