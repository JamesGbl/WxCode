/******************************************************************************

   test8.cpp

    test the comment related functions (reader)
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


static int TestX_X()
{
	return 0;
}

// the first JSON text input:
//
//		comments before
//		contains a syntax error on line 42
//
// it is read by:
//	Test11_1 (without storing comments
//	Test11_2 (storing comments)
//
static const wxChar* test_11_1 = _T("// comment for root (1)\n"
/*  2 */  "// comment for root (2)\n"
/*  3 */  "{\n"
/*  4 */  "  \"key-1\" : \"value1\",\n"
/*  5 */  "\n"
/*  6 */  "  // comment before \'key2\'\n"
/*  7 */  "  \"key-2\" : \"value2\",\n"
/*  8 */  "  // comment before \'key3\' (1)\n"
/*  9 */  "  // comment before \'key3\' (2)\n"
/* 10 */  "\n"
/* 11 */  "  \"key-3\" : {\n"
/* 12 */  "     \"key3-1\" : \"value3-1\",\n"
/* 13 */  "\n"
/* 14 */  "      // comment before key3-2\n"
/* 15 */  "      \"key3-2\" : \"value3-2\"\n"
/* 16 */  "   },\n"
/* 17 */  "\n"
/* 18 */  "   \"key-4\" : {   // comment inline key4\n"
/* 19 */  "      // this comment does not refer to anything (19)\n"
/* 20 */  "   },\n"
/* 21 */  "\n"
/* 22 */  "   \"key-5\" : [ // comment inline key5\n"
/* 23 */  "\n"
/* 24 */  "      // comment before item 5-1\n"
/* 25 */  "      \"item5-1\",\n"
/* 26 */  "      \"item5-2\", // comment inline 5-2\n"
/* 27 */  "      \"item5-3\"  // comment inline 5-3\n"
/* 28 */  "\n"
/* 29 */  "      // this comment does not refer to anything (29)\n"
/* 30 */  "   ],\n"
/* 31 */  "\n"
/* 32 */  "   \"key-6\"\n"
/* 33 */  "      :        // comment inline key-6\n"
/* 34 */  "        \"value\",\n"
/* 35 */  "\n"
/* 36 */  "   \"key-7\" : {\n"
/* 37 */  "      \"key-7-1\" : \"value-7-1\"\n"
/* 38 */  "   },        // comment inline key-7\n"
/* 39 */  "\n"
/* 40 */  "   \"key-8\"     // comment inline key-8(1)\n"
/* 41 */  "      :        // comment inline key-8(2)\n"
/* 42 */  "       value,  // comment inline key-8(3)\n"     // ERROR: value not quoted
/* 43 */  "\n"
/* 44 */  "   \"key-8\"     // comment inline key-8(1-1)\n"
/* 45 */  "      :        // comment inline key-8(1-2)\n"
/* 46 */  "      \"value-8\", // comment inline key-8(1-3)\n"
/* 47 */  "\n"
/* 48 */  "   \"key-9\" : {\n"
/* 49 */  "      \"key9-1\" : 91,\n"
/* 50 */  "      \"key9-2\" : 92\n"
/* 51 */  "   },\n"
/* 52 */  "\n"
/* 53 */  "\n"
/* 54 */  "   \"key-10\" : [\n"
/* 55 */  "   ]            // comment inline key-10\n"
/* 56 */  "\n"
/* 57 */  "   // this comment does not refer to anything (57)\n"
/* 58 */  "}\n"
/* 59 */  " // this comment is not stored in the root value\n"
/* 60 */  " // if COMMENT_BEFORE. If should be if COMMENT_AFTER.\n"
/* 61 */ "\n"
/* 62 */ " This non-JSON text is ignored by the parser because\n"
/* 63 */ " it apears after the top-level close-object character\n"
/* 64 */ "\n");


// this function constructs a wxJSONValue object that contains
// the expected value when reading 'test_11_before' JSON text
// if 'comments' is TRUE, the function also stores the comments
// in the value objects
static void ExpectedValue_11_1( wxJSONValue& val, bool comments )
{
	val[_T("key-1")] = _T("value1");
	val[_T("key-2")] = _T("value2");
	if ( comments ) {
		val[_T("key-2")].AddComment( _T("// comment before \'key2\'"),
						wxJSONVALUE_COMMENT_BEFORE );

		val[_T("key-3")].AddComment( _T("// comment before \'key3\' (1)"),
						wxJSONVALUE_COMMENT_BEFORE );
		val[_T("key-3")].AddComment( _T("// comment before \'key3\' (2)"),
						wxJSONVALUE_COMMENT_BEFORE );
	}
	val[_T("key-3")][_T("key3-1")] = _T("value3-1");
	val[_T("key-3")][_T("key3-2")] = _T("value3-2");

	if ( comments )  {
		val[_T("key-3")][_T("key3-2")].AddComment( _T("// comment before key3-2\n"), 
						wxJSONVALUE_COMMENT_BEFORE );
	}

	wxJSONValue key4( wxJSONTYPE_OBJECT );
	if ( comments )  {
		key4.AddComment( _T("// comment inline key4"), 
						wxJSONVALUE_COMMENT_INLINE );
	}
	val[_T("key-4")] = key4;
	val[_T("key-5")].Append( _T("item5-1"));
	val[_T("key-5")].Append( _T("item5-2"));
	val[_T("key-5")].Append( _T("item5-3"));
	if ( comments )  {
		val[_T("key-5")].AddComment( _T("// comment inline key5\n"),
						wxJSONVALUE_COMMENT_INLINE );
		val[_T("key-5")][0].AddComment( _T("// comment before item 5-1\n"),
						wxJSONVALUE_COMMENT_BEFORE );
		val[_T("key-5")][1].AddComment( _T("// comment inline item 5-2\n"),
						wxJSONVALUE_COMMENT_INLINE );
		val[_T("key-5")][2].AddComment( _T("// comment inline item 5-3"),
						wxJSONVALUE_COMMENT_INLINE );
	}

	val[_T("key-6")] = _T("value");
	val[_T("key-7")][_T("key-7-1")] = _T("value-7-1");

	if ( comments )  {
		val[_T("key-6")].AddComment( _T("// comment inline key-6\n"),
						wxJSONVALUE_COMMENT_INLINE );
		val[_T("key-7")].AddComment( _T("// comment inline key-7\n"),
						wxJSONVALUE_COMMENT_INLINE );
	}

	// the first 'key-8' is wrong: a syntax error is detected
	val[_T("key-8")] = _T("value-8");
	if ( comments )  {
		val[_T("key-8")].AddComment( _T("// comment inline key-8(1)\n"),
						wxJSONVALUE_COMMENT_INLINE );
		val[_T("key-8")].AddComment( _T("// comment inline key-8(2)\n"),
						wxJSONVALUE_COMMENT_INLINE );
		val[_T("key-8")].AddComment( _T("// comment inline key-8(3)\n"),
						wxJSONVALUE_COMMENT_INLINE );
	}

	val[_T("key-9")][_T("key9-1")] = 91;
	val[_T("key-9")][_T("key9-2")] = 92;

	wxJSONValue key10( wxJSONTYPE_ARRAY );
	val[_T("key-10")] = key10;
}



// first test without comments 6 dec 2007: OK, successfull
int Test11_1()
{
	wxString s( test_11_1 );    // the JSON document
	wxJSONValue root;

	// do not store comments
	wxJSONReader reader( wxJSONREADER_TOLERANT );
	int numErrors = reader.Parse( s, &root );
	PrintValue( root, &reader );
	ASSERT( numErrors == 2 )

	wxJSONValue result;
	ExpectedValue_11_1( result, true );
	bool r = result.IsSameAs( root );

	TestCout( _T("\nEXPECTED VALUE:\n") );
	wxJSONWriter writer( wxJSONWRITER_STYLED );
	wxString text;
	writer.Write( result, text );
	TestCout( text );

	ASSERT( r == true );
	return 0;
}

// second test storing comments (COMMENTS_BEFORE) 
int Test11_2()
{
	wxString s( test_11_1 );    // the JSON document
	wxJSONValue root;

	// do not store comments
	wxJSONReader reader( wxJSONREADER_TOLERANT | wxJSONREADER_STORE_COMMENTS );
	int numErrors = reader.Parse( s, &root );
	PrintValue( root, &reader );
	ASSERT( numErrors == 4 )

	// check if the read value is 'the same' as the expected value
	wxJSONValue result;
	ExpectedValue_11_1( result, true );
	bool r = result.IsSameAs( root );
	ASSERT( r == true );

	// now assert comment related results
	int pos, count;
	pos   = root.GetCommentPos();
	count = root.GetCommentCount();
	ASSERT( count == 2 )
	ASSERT( pos == wxJSONVALUE_COMMENT_BEFORE )

	pos   = root[_T("key-1")].GetCommentPos();
	count = root[_T("key-1")].GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_BEFORE )

	pos   = root[_T("key-2")].GetCommentPos();
	count = root[_T("key-2")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_BEFORE )

	pos   = root[_T("key-3")].GetCommentPos();
	count = root[_T("key-3")].GetCommentCount();
	ASSERT( count == 2 )
	ASSERT( pos == wxJSONVALUE_COMMENT_BEFORE )

	pos   = root[_T("key-3")][_T("key3-1")].GetCommentPos();
	count = root[_T("key-3")][_T("key3-1")].GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_BEFORE )

	pos   = root[_T("key-3")][_T("key3-2")].GetCommentPos();
	count = root[_T("key-3")][_T("key3-2")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_BEFORE )

	pos   = root[_T("key-4")].GetCommentPos();
	count = root[_T("key-4")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-5")].GetCommentPos();
	count = root[_T("key-5")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-5")][0].GetCommentPos();
	count = root[_T("key-5")][0].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_BEFORE )

	pos   = root[_T("key-5")][1].GetCommentPos();
	count = root[_T("key-5")][1].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-5")][2].GetCommentPos();
	count = root[_T("key-5")][2].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-6")].GetCommentPos();
	count = root[_T("key-6")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-7")].GetCommentPos();
	count = root[_T("key-7")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-7")][_T("key-7-1")].GetCommentPos();
	count = root[_T("key-7")][_T("key-7-1")].GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-8")].GetCommentPos();
	count = root[_T("key-8")].GetCommentCount();

	// BUG for a bug in the StoreComment, the number of comments in key-8
	// is 4 instead of 3 In fact there are only 3 comment lines
	ASSERT( count == 4 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-9")].GetCommentPos();
	count = root[_T("key-9")].GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-10")].GetCommentPos();
	count = root[_T("key-10")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	return 0;
}

// this is the same as test_11_2 but without the syntax error
// in 'key-8'
int Test11_3()
{
	static const wxChar* text_11_3 = _T("// comment for root (1)\n"
/*  2 */  "// comment for root (2)\n"
/*  3 */  "{\n"
/*  4 */  "  \"key-1\" : \"value1\",\n"
/*  5 */  "\n"
/*  6 */  "  // comment before \'key2\'\n"
/*  7 */  "  \"key-2\" : \"value2\",\n"
/*  8 */  "  // comment before \'key3\' (1)\n"
/*  9 */  "  // comment before \'key3\' (2)\n"
/* 10 */  "\n"
/* 11 */  "  \"key-3\" : {\n"
/* 12 */  "     \"key3-1\" : \"value3-1\",\n"
/* 13 */  "\n"
/* 14 */  "      // comment before key3-2\n"
/* 15 */  "      \"key3-2\" : \"value3-2\"\n"
/* 16 */  "   },\n"
/* 17 */  "\n"
/* 18 */  "   \"key-4\" : {   // comment inline key4\n"
/* 19 */  "      // this comment does not refer to anything (19)\n"
/* 20 */  "   },\n"
/* 21 */  "\n"
/* 22 */  "   \"key-5\" : [ // comment inline key5\n"
/* 23 */  "\n"
/* 24 */  "      // comment before item 5-1\n"
/* 25 */  "      \"item5-1\",\n"
/* 26 */  "      \"item5-2\", // comment inline 5-2\n"
/* 27 */  "      \"item5-3\"  // comment inline 5-3\n"
/* 28 */  "\n"
/* 29 */  "      // this comment does not refer to anything (29)\n"
/* 30 */  "   ],\n"
/* 31 */  "\n"
/* 32 */  "   \"key-6\"\n"
/* 33 */  "      :        // comment inline key-6\n"
/* 34 */  "        \"value\",\n"
/* 35 */  "\n"
/* 36 */  "   \"key-7\" : {\n"
/* 37 */  "      \"key-7-1\" : \"value-7-1\"\n"
/* 38 */  "   },        // comment inline key-7\n"
/* 39 */  "\n"
/* 40 */  "\n"  // these lines contained the erroneous
/* 41 */  "\n"  // 'key-8' value
/* 42 */  "\n"
/* 43 */  "\n"
/* 44 */  "   \"key-8\"     // comment inline key-8(1-1)\n"
/* 45 */  "      :        // comment inline key-8(1-2)\n"
/* 46 */  "      \"value-8\", // comment inline key-8(1-3)\n"
/* 47 */  "\n"
/* 48 */  "   \"key-9\" : {\n"
/* 49 */  "      \"key9-1\" : 91,\n"
/* 50 */  "      \"key9-2\" : 92\n"
/* 51 */  "   },\n"
/* 52 */  "\n"
/* 53 */  "\n"
/* 54 */  "   \"key-10\" : [\n"
/* 55 */  "   ]            // comment inline key-10\n"
/* 56 */  "\n"
/* 57 */  "   // this comment does not refer to anything (57)\n"
/* 58 */  "}\n"
/* 59 */  " // this comment is not stored in the root value\n"
/* 60 */  " // if COMMENT_BEFORE. If should be if COMMENT_AFTER.\n"
/* 61 */ "\n"
/* 62 */ " This non-JSON text is ignored by the parser because\n"
/* 63 */ " it apears after the top-level close-object character\n"
/* 64 */ "\n");

	wxString s( text_11_3 );    // the JSON document
	wxJSONValue root;

	// do store comments
	wxJSONReader reader( wxJSONREADER_TOLERANT | wxJSONREADER_STORE_COMMENTS );
	int numErrors = reader.Parse( s, &root );
	PrintValue( root, &reader );
	ASSERT( numErrors == 2 )

	// check if the read value is 'the same' as the expected value
	wxJSONValue result;
	ExpectedValue_11_1( result, true );
	bool r = result.IsSameAs( root );
	ASSERT( r == true );

	// now assert comment related results
	int pos, count;
	pos   = root.GetCommentPos();
	 count = root.GetCommentCount();
	ASSERT( count == 2 )
	ASSERT( pos == wxJSONVALUE_COMMENT_BEFORE )

	pos   = root[_T("key-1")].GetCommentPos();
	count = root[_T("key-1")].GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_BEFORE )

	pos   = root[_T("key-2")].GetCommentPos();
	count = root[_T("key-2")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_BEFORE )

	pos   = root[_T("key-3")].GetCommentPos();
	count = root[_T("key-3")].GetCommentCount();
	ASSERT( count == 2 )
	ASSERT( pos == wxJSONVALUE_COMMENT_BEFORE )

	pos   = root[_T("key-3")][_T("key3-1")].GetCommentPos();
	count = root[_T("key-3")][_T("key3-1")].GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_BEFORE )

	pos   = root[_T("key-3")][_T("key3-2")].GetCommentPos();
	count = root[_T("key-3")][_T("key3-2")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_BEFORE )

	pos   = root[_T("key-4")].GetCommentPos();
	count = root[_T("key-4")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-5")].GetCommentPos();
	count = root[_T("key-5")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-5")][0].GetCommentPos();
	count = root[_T("key-5")][0].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_BEFORE )

	pos   = root[_T("key-5")][1].GetCommentPos();
	count = root[_T("key-5")][1].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-5")][2].GetCommentPos();
	count = root[_T("key-5")][2].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-6")].GetCommentPos();
	count = root[_T("key-6")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-7")].GetCommentPos();
	count = root[_T("key-7")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-7")][_T("key-7-1")].GetCommentPos();
	count = root[_T("key-7")][_T("key-7-1")].GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-8")].GetCommentPos();
	count = root[_T("key-8")].GetCommentCount();
	ASSERT( count == 3 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-9")].GetCommentPos();
	count = root[_T("key-9")].GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-10")].GetCommentPos();
	count = root[_T("key-10")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	return 0;
}


/****************************************************************
		test COMMENTS_AFTER
****************************************************************/

static const wxChar* text_11_4 = _T("// this comment does not refer to any value (1)\n"
/*  2 */ "// this comment does not refer to any value (2)\n"
/*  3 */ "{\n"
/*  4 */ "  // this comment does not refer to anything (4)\n"
/*  5 */ "  \"key-1\" : \"value1\",\n"
/*  6 */ "  \"key-2\" : \"value2\",\n"
/*  7 */ "  // comment after \'key2\'\n"
/*  8 */ "\n"
/*  9 */ "  \"key-3\" : {\n"
/* 10 */ "     \"key3-1\" : \"value3-1\",\n"
/* 11 */ "\n"
/* 12 */ "    \"key3-2\" : \"value3-2\"\n"
/* 13 */ "     // comment after key3-2\n"
/* 14 */ "  },\n"
/* 15 */ "  // comment after 'key3' (1)\n"
/* 16 */ "  // comment after 'key3' (2)\n"
/* 17 */ "\n"
/* 18 */ "  \"key-4\" : {   // comment inline key4\n"
/* 19 */ "     // this comment does not refer to anything (19)\n"
/* 20 */ "  },\n"
/* 21 */ "\n"
/* 22 */ "  \"key-5\" : [ // comment inline key5\n"
/* 23 */ "     // this comment does not refer to anything (23)\n"
/* 24 */ "     \"item5-1\",\n"
/* 25 */ "\n"
/* 26 */ "     // comment after item 5-1\n"
/* 27 */ "     \"item5-2\", // comment inline 5-2\n"
/* 28 */ "     \"item5-3\", // comment inline 5-3\n"
/* 29 */ "     \"item5-4\"\n"
/* 30 */ "     // comment after item 5-4\n"
/* 31 */ "  ],\n"
/* 32 */ "  \"key-6\"\n"
/* 33 */ "     :        // comment inline key-6\n"
/* 34 */ "       \"value\",\n"
/* 35 */ "\n"
/* 36 */ "  \"key-7\" : {\n"
/* 37 */ "     \"key-7-1\" : \"value-7-1\"\n"
/* 38 */ "  },        // comment inline key-7\n"
/* 39 */ "\n"
/* 40 */ "\n"    // \"key-8\"     // comment inline key-8(1)\n"
/* 41 */ "\n"    //    :        // comment inline key-8(2)\n"
/* 42 */ "\n"    //    value,  // comment inline key-8(3)\n"  // ERROR: value not quoted
/* 43 */ "\n"
/* 44 */ "  \"key-8\"     // comment inline key-8(1)\n"
/* 45 */ "     :        // comment inline key-8(2)\n"
/* 46 */ "     \"value-8\", // comment inline key-8(3)\n"
/* 47 */ "\n"
/* 48 */ "  \"key-9\" : {\n"
/* 49 */ "     \"key9-1\" :\n"
/* 50 */ "     // this comment line is not BEFORE nor AFTER nor INLINE (50)\n"
/* 51 */ "                91,\n"
/* 52 */ "     \"key9-2\" : 92\n"
/* 53 */ "  },\n"
/* 54 */ "\n"
/* 55 */ "\n"
/* 56 */ "  \"key-10\" : [\n"
/* 57 */ "  ],            // comment inline key-10\n"
/* 58 */ "\n"
/* 59 */ "  // comment after key-10\n"
/* 60 */ "\n"
/* 61 */ "  \"key-20\" : [ \"20-1\", \"20-2\"   ], // comment inline key-20\n"
/* 62 */ "\n"
/* 63 */ "  \"key-21\" : [ \"21-1\",            // comment inline 21-1\n"
/* 64 */ "               \"21-2\"   ],\n" 
/* 65 */ "\n"
/* 66 */ "  \"key-22\" : [ \"22-1\",\n"
/* 67 */ "               \"22-2\"   ]    // comment inline key-22\n"
/* 68 */ "\n"
/* 69 */ "}\n"
/* 70 */ "// comment for root (1)\n"
/* 71 */ "// comment for root (2)\n"
/* 72 */ "\n"
/* 73 */ "This non-JSON text is ignored by the parser because\n"
/* 74 */ "it apears after the top-level close-object character\n"
/* 75 */ "\n");

static void ExpectedValue11_4( wxJSONValue& val, bool comments )
{
	val[_T("key-1")] = _T("value1");
	val[_T("key-2")] = _T("value2");
	if ( comments ) {
		val[_T("key-2")].AddComment( _T("// comment after \'key2\'"),
						wxJSONVALUE_COMMENT_AFTER );

		val[_T("key-3")].AddComment( _T("// comment after \'key3\' (1)"),
						wxJSONVALUE_COMMENT_AFTER );
		val[_T("key-3")].AddComment( _T("// comment after \'key3\' (2)"),
						wxJSONVALUE_COMMENT_AFTER );
	}
	val[_T("key-3")][_T("key3-1")] = _T("value3-1");
	val[_T("key-3")][_T("key3-2")] = _T("value3-2");

	if ( comments )  {
		val[_T("key-3")][_T("key3-2")].AddComment( _T("// comment after key3-2\n"), 
						wxJSONVALUE_COMMENT_AFTER );
	}

	wxJSONValue key4( wxJSONTYPE_OBJECT );
	if ( comments )  {
		key4.AddComment( _T("// comment inline key4"), 
						wxJSONVALUE_COMMENT_INLINE );
	}
	val[_T("key-4")] = key4;
	val[_T("key-5")].Append( _T("item5-1"));
	val[_T("key-5")].Append( _T("item5-2"));
	val[_T("key-5")].Append( _T("item5-3"));
	val[_T("key-5")].Append( _T("item5-4"));
	if ( comments )  {
		val[_T("key-5")].AddComment( _T("// comment inline key5\n"),
						wxJSONVALUE_COMMENT_INLINE );
		val[_T("key-5")][0].AddComment( _T("// comment after item 5-1\n"),
						wxJSONVALUE_COMMENT_AFTER );
		val[_T("key-5")][1].AddComment( _T("// comment inline item 5-2\n"),
						wxJSONVALUE_COMMENT_INLINE );
		val[_T("key-5")][2].AddComment( _T("// comment inline item 5-3"),
						wxJSONVALUE_COMMENT_INLINE );
		val[_T("key-5")][3].AddComment( _T("// comment after item 5-4\n"),
						wxJSONVALUE_COMMENT_AFTER );
	}

	val[_T("key-6")] = _T("value");
	val[_T("key-7")][_T("key-7-1")] = _T("value-7-1");

	if ( comments )  {
		val[_T("key-6")].AddComment( _T("// comment inline key-6\n"),
						wxJSONVALUE_COMMENT_INLINE );
		val[_T("key-7")].AddComment( _T("// comment inline key-7\n"),
						wxJSONVALUE_COMMENT_INLINE );
	}

	// the first 'key-8' is wrong: a syntax error is detected
	val[_T("key-8")] = _T("value-8");
	if ( comments )  {
		val[_T("key-8")].AddComment( _T("// comment inline key-8(1)\n"),
						wxJSONVALUE_COMMENT_INLINE );
		val[_T("key-8")].AddComment( _T("// comment inline key-8(2)\n"),
						wxJSONVALUE_COMMENT_INLINE );
		val[_T("key-8")].AddComment( _T("// comment inline key-8(3)\n"),
						wxJSONVALUE_COMMENT_INLINE );
	}

	val[_T("key-9")][_T("key9-1")] = 91;
	val[_T("key-9")][_T("key9-2")] = 92;

	wxJSONValue key10( wxJSONTYPE_ARRAY );
	if ( comments )  {
		key10.AddComment( _T("// comment inline key-10\n"),
						wxJSONVALUE_COMMENT_INLINE );
		key10.AddComment( _T("// comment after key-10\n"),
						wxJSONVALUE_COMMENT_AFTER );
	}
	val[_T("key-10")] = key10;

	val[_T("key-20")].Append( _T("20-1"));
	val[_T("key-20")].Append( _T("20-2"));
	if ( comments )  {
		val[_T("key-20")].AddComment( _T("// comment inline key-20\n"),
						wxJSONVALUE_COMMENT_INLINE );
	}

	val[_T("key-21")].Append( _T("21-1"));
	val[_T("key-21")].Append( _T("21-2"));
	if ( comments )  {
		val[_T("key-21")][0].AddComment( _T("// comment inline key-21/21-1\n"),
						wxJSONVALUE_COMMENT_INLINE );
	}

	val[_T("key-22")].Append( _T("22-1"));
	val[_T("key-22")].Append( _T("22-2"));
	if ( comments )  {
		val[_T("key-22")].AddComment( _T("// comment inline key-22\n"),
						wxJSONVALUE_COMMENT_INLINE );
	}
}


// first test without storing comments (COMMENTS_AFTER)
int Test11_4()
{
	wxString s( text_11_4 );    // the JSON document
	wxJSONValue root;

	// do not store comments
	wxJSONReader reader( wxJSONREADER_TOLERANT );
	int numErrors = reader.Parse( s, &root );
	PrintValue( root, &reader );
	ASSERT( numErrors == 0 )

	wxJSONValue result;
	ExpectedValue11_4( result, true );
	bool r = result.IsSameAs( root );

	TestCout( _T("\nEXPECTED VALUE:\n") );
	wxJSONWriter writer( wxJSONWRITER_STYLED );
	wxString json;
	writer.Write( result, json );
	TestCout( json );

	ASSERT( r == true );
	return 0;
}

// second test storing comments (COMMENTS_AFTER) 
int Test11_5()
{
	wxString s( text_11_4 );    // the JSON document
	wxJSONValue root;

	wxJSONReader reader( wxJSONREADER_TOLERANT
			| wxJSONREADER_STORE_COMMENTS
			| wxJSONREADER_COMMENTS_AFTER );
	int numErrors = reader.Parse( s, &root );
	PrintValue( root, &reader );
	ASSERT( numErrors == 6 )

	// check if the read value is 'the same' as the expected value
	wxJSONValue result;
	ExpectedValue11_4( result, true );
	bool r = result.IsSameAs( root );
	ASSERT( r == true );

	// now assert comment related results
	int pos, count;
	pos   = root.GetCommentPos();
	count = root.GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_BEFORE )

	pos   = root[_T("key-1")].GetCommentPos();
	count = root[_T("key-1")].GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_BEFORE )

	pos   = root[_T("key-2")].GetCommentPos();
	count = root[_T("key-2")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_AFTER )

	pos   = root[_T("key-3")].GetCommentPos();
	count = root[_T("key-3")].GetCommentCount();
	ASSERT( count == 2 )
	ASSERT( pos == wxJSONVALUE_COMMENT_AFTER )

	pos   = root[_T("key-3")][_T("key3-1")].GetCommentPos();
	count = root[_T("key-3")][_T("key3-1")].GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_BEFORE )

	pos   = root[_T("key-3")][_T("key3-2")].GetCommentPos();
	count = root[_T("key-3")][_T("key3-2")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_AFTER )

	pos   = root[_T("key-4")].GetCommentPos();
	count = root[_T("key-4")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-5")].GetCommentPos();
	count = root[_T("key-5")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-5")][0].GetCommentPos();
	count = root[_T("key-5")][0].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_AFTER )

	pos   = root[_T("key-5")][1].GetCommentPos();
	count = root[_T("key-5")][1].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-5")][2].GetCommentPos();
	count = root[_T("key-5")][2].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-5")][3].GetCommentPos();
	count = root[_T("key-5")][3].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_AFTER )

	pos   = root[_T("key-6")].GetCommentPos();
	count = root[_T("key-6")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-7")].GetCommentPos();
	count = root[_T("key-7")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-7")][_T("key-7-1")].GetCommentPos();
	count = root[_T("key-7")][_T("key-7-1")].GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-8")].GetCommentPos();
	count = root[_T("key-8")].GetCommentCount();
	ASSERT( count == 3 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-9")].GetCommentPos();
	count = root[_T("key-9")].GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-9")][_T("key9-1")].GetCommentPos();
	count = root[_T("key-9")][_T("key9-1")].GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-9")][_T("key9-2")].GetCommentPos();
	count = root[_T("key-9")][_T("key9-2")].GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-10")].GetCommentPos();
	count = root[_T("key-10")].GetCommentCount();
	ASSERT( count == 2 )
	// the input file contains one inline comment and one
	// AFTER comment. The AFTER is added after the inline
	// so both will get the AFTER position
	ASSERT( pos == wxJSONVALUE_COMMENT_AFTER )

	pos   = root[_T("key-20")].GetCommentPos();
	count = root[_T("key-20")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-20")][0].GetCommentPos();
	count = root[_T("key-20")][0].GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-20")][1].GetCommentPos();
	count = root[_T("key-20")][1].GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-21")].GetCommentPos();
	count = root[_T("key-21")].GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-21")][0].GetCommentPos();
	count = root[_T("key-21")][0].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-21")][1].GetCommentPos();
	count = root[_T("key-21")][1].GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-22")].GetCommentPos();
	count = root[_T("key-22")].GetCommentCount();
	ASSERT( count == 1 )
	ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-22")][0].GetCommentPos();
	count = root[_T("key-22")][0].GetCommentCount();
	ASSERT( count == 0 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

	pos   = root[_T("key-22")][1].GetCommentPos();
	count = root[_T("key-22")][1].GetCommentCount();
	// BUG: the comment after the top level close-object
	// character are not stored because the DoRead()
	// function returns
	// ASSERT( count == 2 )
	// ASSERT( pos == wxJSONVALUE_COMMENT_INLINE )

  return 0;
}

/*
{
}
*/


