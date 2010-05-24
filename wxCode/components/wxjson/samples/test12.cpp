/******************************************************************************

   test12.cpp

    test suite for the new JSON type: memory buffer
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


// just test some wxMemoryBuffer's copy ctor and assignment op 
int Test15_1()
{
    // two tiny memory buffer that contains ten bytes from 0 to 9
    unsigned char mem1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    unsigned char mem2[] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

    wxMemoryBuffer buff1;
    buff1.AppendData( mem1, 10 );

    wxMemoryBuffer buff2;
    buff2.AppendData( mem2, 10 );

    // copy buff1 to buff3
    wxMemoryBuffer buff3( buff1 );

    // creates buff4 which contains the same data as buff1
    wxMemoryBuffer buff4;
    buff4.AppendData( mem1, 10 );

    // assings buff2 to buff5
    wxMemoryBuffer buff5;
    buff5 = buff2;

    // prints the buffers as strings
    wxString s;
    s = wxJSONValue::MemoryBuffToString( buff1 );
    TestCout( _T("buffer to string: \'buff1'\n"));
    TestCout( s );
    TestCout( _T("\n"));
    s = wxJSONValue::MemoryBuffToString( buff2 );
    TestCout( _T("buffer to string: \'buff2'\n"));
    TestCout( s );
    TestCout( _T("\n"));
    s = wxJSONValue::MemoryBuffToString( buff3 );
    TestCout( _T("buffer to string: \'buff3'\n"));
    TestCout( s );
    TestCout( _T("\n"));
    s = wxJSONValue::MemoryBuffToString( buff4 );
    TestCout( _T("buffer to string: \'buff4'\n"));
    TestCout( s );
    TestCout( _T("\n"));
    s = wxJSONValue::MemoryBuffToString( buff5 );
    TestCout( _T("buffer to string: \'buff5'\n"));
    TestCout( s );
    TestCout( _T("\n"));

    // test comparison operator

    bool r = buff1 == buff2;
    TestCout( _T( "buff1 and buff2 operator == :" ));   // not equal
    TestCout( r ? _T("equal") : _T("not equal"));
    TestCout( _T("\n"));
    r = buff1 == buff3;
    TestCout( _T( "buff1 and buff3 operator == :" ));   // equal (copied)
    TestCout( r ? _T("equal") : _T("not equal"));
    TestCout( _T("\n"));
    r = buff1 == buff4;
    TestCout( _T( "buff1 and buff4 operator == :" ));   // not equal, not copied
    TestCout( r ? _T("equal") : _T("not equal"));
    TestCout( _T("\n"));
    r = buff2 == buff5;
    TestCout( _T( "buff2 and buff5 operator == :" ));   // equal, assigned
    TestCout( r ? _T("equal") : _T("not equal"));
    TestCout( _T("\n"));

    // changes 'buff5', note that 'buff2' will also be changed
    buff5.SetDataLen( 0 );
    s = wxJSONValue::MemoryBuffToString( buff5 );
    TestCout( _T("buffer to string: \'buff5'\n"));
    TestCout( s );
    TestCout( _T("\n"));
    s = wxJSONValue::MemoryBuffToString( buff2 );
    TestCout( _T("buffer to string: \'buff2'\n"));
    TestCout( s );
    TestCout( _T("\n"));

	return 0;
}


// test new wxJSONTYPE_MEMORYBUFF ctors, assignment, copy ctors, etc.
int Test15_2()
{
    unsigned char mem1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };            // 10 bytes
    unsigned char mem2[] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };  // 10 bytes
    unsigned char mem3[] = { 20, 21, 22, 23, 24 };                      //  5 bytes
    unsigned char mem4[] = { 25, 26, 27, 28, 29 };                      //  5 bytes

    bool r; int  i; wxString s;

    TestCout( _T("\nCreating two different memory buffers\n"));
    // 1.
    // JSON value ctor from wxMemoryBuffer
    wxMemoryBuffer b1;
    b1.AppendData( mem1, 10 );
    wxJSONValue v1( b1 );

    // 2.
    // JSON value ctor from memory: they are the same
    wxJSONValue v2( mem1, 10 );

    // printing the two JSON values as strings
    s = v1.AsString();
    TestCout( _T("buffer to string: \'v1'\n"));
    TestCout( s );
    TestCout( _T("\n"));
    s = v2.AsString();
    TestCout( _T("buffer to string: \'v2'\n"));
    TestCout( s );
    TestCout( _T("\n"));

    // testing comparison operator: this makes a deep copy of the memory buffers
    r = v1.IsSameAs( v2 );
    ASSERT( r );

    TestCout( _T("\nCopying v1 to v3\n"));

    // copy v1 to v3
    wxJSONValue v3( v1 );
    // print the values of v1 and v3
    s = v1.AsString();
    TestCout( _T("buffer to string: \'v1'\n"));
    TestCout( s );
    TestCout( _T("\n"));
    s = v3.AsString();
    TestCout( _T("buffer to string: \'v3'\n"));
    TestCout( s );
    TestCout( _T("\n"));

    TestCout( _T("\nChanging v3, v1 should not change\n"));
    // now change 'v3', 'v1' should not change
    wxMemoryBuffer b3;
    b3.AppendData( mem3, 5 );
    v3 = b3;

    // print the new values of the two JSON values as strings
    s = v1.AsString();
    TestCout( _T("buffer to string: \'v1'\n"));
    TestCout( s );
    TestCout( _T("\n"));
    s = v3.AsString();
    TestCout( _T("buffer to string: \'v3'\n"));
    TestCout( s );
    TestCout( _T("\n"));

    // now we use the assignment operator. create v4 by assigning v2 and then
    // change v4: v2 should not change
    TestCout( _T("\nAssigning v2 to v4\n"));
    wxJSONValue v4;
    v4 = v2;

    TestCout( _T("\nChanging the value of v4 (v2 should not change)\n"));
    wxMemoryBuffer b4;
    b4.AppendData( mem4, 5 );
    v4 = b4;
    s = v4.AsString();
    TestCout( _T("buffer to string: \'v4'\n"));
    TestCout( s );
    TestCout( _T("\n"));
    s = v2.AsString();
    TestCout( _T("buffer to string: \'v2'\n"));
    TestCout( s );
    TestCout( _T("\n"));

    return 0;
}


// test the conversion to string (wxJSONValue::MemoryBuffToString) static function
int Test15_3()
{
    unsigned char mem1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa0, 0xb1, 0xc3, 0xe4, 0xf5 };

    wxMemoryBuffer b1;
    b1.AppendData( mem1, 15 );

    wxString s = wxJSONValue::MemoryBuffToString( b1 );
    TestCout( _T("Printing the memory buffer \'mem1\'\n"));
    TestCout( s );
    TestCout( _T("\n"));

    s = wxJSONValue::MemoryBuffToString( b1, 5 );
    TestCout( _T("Printing the first 5 bytes of memory buffer object'\n"));
    TestCout( s );
    TestCout( _T("\n"));

    s = wxJSONValue::MemoryBuffToString( b1, 20 );
    TestCout( _T("Try to print 20 bytes of a 15-bytes memory buffer object\n"));
    TestCout( s );
    TestCout( _T("\n"));

    s = wxJSONValue::MemoryBuffToString( mem1, 10 );
    TestCout( _T("Print 10 bytes from a memory buffer\n"));
    TestCout( s );
    TestCout( _T("\n"));

    return 0;    
}



// test the comparison static function
int Test15_4()
{
    unsigned char mem1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa0, 0xb1, 0xc3, 0xe4, 0xf5 };
    unsigned char mem2[] = { 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa0, 0xb1, 0xc3, 0xe4, 0xf5 };

    TestCout( _T("creating \'b1\' and \'b2\' (the same) \'b3\' is different'\n"));
    wxMemoryBuffer b1;
    b1.AppendData( mem1, 15 );
    wxMemoryBuffer b2;
    b2.AppendData( mem1, 15 );

    void* ptr1 = b1.GetData();
    void* ptr2 = b2.GetData();
    wxString s;
    TestCout( _T("b1 pointer: "));
    s.Printf( _T("%p\n"), ptr1 );
    TestCout( s );
    TestCout( _T("b2 pointer: "));
    s.Printf( _T("%p\n"), ptr2 );
    TestCout( s );
    ASSERT( ptr1 != ptr2 );    
    TestCout( _T("ASSERTing \'b1\' is same as \'b2\'\n"));
    int r = wxJSONValue::CompareMemoryBuff( b1, b2 );
    ASSERT( r == 0 );

    wxMemoryBuffer b3;
    b3.AppendData( mem2, 15 );  // this is different
    r = wxJSONValue::CompareMemoryBuff( b1, b3 );
    TestCout( _T("ASSERTing \'b1\' is different as \'b3\'\n"));
    ASSERT( r != 0 );

    r = wxJSONValue::CompareMemoryBuff( b1, (const void*) mem1 );
    TestCout( _T("ASSERTing \'b1\' is same as \'mem1\'\n"));
    ASSERT( r == 0 );

    return 0;    
}

// test the WRITER - we try to write an array of memory buffer object of various sizes:
//	- 5 bytes
//	- 19 bytes
//	- 20 bytes
//	- 50 bytes
//	- 40 bytes
//	- 200 bytes
//	- empty
// using variuos flags:
//	STYLED
//	STYLED & SPLIT_STRING
//	NONE
//	MEMORYBUFF
//	MEMORYBUFF & STYLED
//	MEMORYBUFF & STYLED & SPLIT_STRING

static const char mem[] = {
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,
		29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,
		54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,
		89,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,
		100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,
		119,120,121,122,123,124,125,126,127,128,
		129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,
		148,149,150,151,152,153,
		154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,
		173,174,175,176,177,178,
		189,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,
		198,199,200
    };

int Test15_5()
{

	wxJSONValue root;
	root.Append( mem, 5 );
	root.Append( mem, 19 );
	root.Append( mem, 20 );
	root.Append( mem, 21 );
	root.Append( mem, 45 );
	root.Append( mem, 200 );
	root.Append( wxMemoryBuffer() );	// an empty buffer

	// creates the writer's objects
	wxJSONWriter wrt1( wxJSONWRITER_STYLED );
	wxJSONWriter wrt2( wxJSONWRITER_STYLED | wxJSONWRITER_SPLIT_STRING );
	wxJSONWriter wrt3( wxJSONWRITER_NONE );
	wxJSONWriter wrt4( wxJSONWRITER_STYLED | wxJSONWRITER_MEMORYBUFF );
	wxJSONWriter wrt5( wxJSONWRITER_STYLED
			| wxJSONWRITER_SPLIT_STRING | wxJSONWRITER_MEMORYBUFF );
	wxJSONWriter wrt6( wxJSONWRITER_MEMORYBUFF );

	// the JSON text output
	wxString s;
	wrt1.Write( root, s );
    TestCout( _T("\nstyle=STYLED\n"));
	TestCout( s );

	wrt2.Write( root, s );
    TestCout( _T("\nstyle=STYLED | SPLIT_STRING\n"));
	TestCout( s );

	wrt3.Write( root, s );
    TestCout( _T("\nstyle=NONE\n"));
	TestCout( s );

	wrt4.Write( root, s );
    TestCout( _T("\nstyle=STYLED | MEMORYBUFF \n"));
	TestCout( s );

	wrt5.Write( root, s );
    TestCout( _T("\nstyle=STYLED | MEMORYBUFF | SPLT_STRING\n"));
	TestCout( s );

	wrt6.Write( root, s );
    TestCout( _T("\nstyle=MEMORYBUFF\n"));
	TestCout( s );

	return 0;
}


// test the READER - we try to read various memory buffers from JSON text
// jan 27, 2010 BUG=memory buffers are all ZERO length
int Test15_6()
{
    // the following text contains an arrat of:
    //
    //  0: an array of 5 INTs
    //  1: an array of 19 INTs
    //  2: an array of 20 INTs
    //  3: an array of 200 INTs
    //  4. an empty array
    //  5. a memory buffer of 200 bytes
    //  6. a memory buffer of 20 bytes
    //  7. a memory buffer of 45 bytes
    //  8. a memory buffer of ZERO bytes (empty buffer))

    static const wxChar* json = _T("\n"
        "[\n"
        "   [0,1,2,3,4],      // 0: 5-bytes array of INTs\n"
        "   [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18],       // 1: 19 bytes array of INTs\n"
        "   [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19],    // 2: 20 bytes array of INTs\n"
        "   // 3: a 200 bytes array of INTs\n"
        "   [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,"
        "21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,"
        "46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,"
        "71,72,73,74,75,76,77,78,89,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,"
        "96,97,98,99,100,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,"
        "115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,"
        "134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,"
        "153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,"
        "172,173,174,175,176,177,178,189,180,181,182,183,184,185,186,187,188,189,190,"
        "191,192,193,194,195,196,197,198,199],\n"
        "   [],      // 4: and empty array of INTs\n"

        "   // 5: a 200 bytes memory buffer\n"
        "   \'000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F2021222324252627\',\n"
        "   \'000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F20"
        "2122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F404142434445"
        "464748494A4B4C4D4E59505152535455565758595A5B5C5D5E5F60616263646465666768696A6B6C6D6E6F"
        "707172737475767778797A7B7C7D7E7F808182838485868788898A8B8C8D8E8F90"
        "9192939495969798999A9B9C9D9E9FA0A1A2A3A4A5A6A7A8A9AAABACADAEAFB0"
        "B1B2BDB4B5B6B7B8B9BABBBCBDBEBFC0C1C2C3C4C5C6\',\n"

        "   \'000102030405060708090A0B0C0D0E0F1011121314\',  // 6. 20 bytes\n"
        "   \'000102030405060708090A0B0C0D0E0F10111213\'\n"
        "         \'1415161718191A1B1C1D1E1F2021222324252627\'\n"
        "         \'28292A2B2C\',       // 7. 45 bytes\n"
        "   ''  // 8: an empty memory buffer\n"
    "]\n" );

    wxJSONReader reader( wxJSONREADER_TOLERANT | wxJSONREADER_MEMORYBUFF );
    wxString text( json );
    wxJSONValue root( wxJSONTYPE_INVALID );
    int numErrors = reader.Parse( text, &root );

    // now construct the 9 wxMemoryBuffer objects that are the expected results
    size_t sizes[] = { 5, 19, 20, 200, 0, 200, 20, 45, 0 };

    // expected result mem buffers
    wxMemoryBuffer mb[9];
    for ( int i = 0; i < 9; i++ )   {
        if ( sizes[i] > 0 ) {
            mb[i].AppendData( mem, sizes[i] );
        }
    }

    // now checks that wxMemoryBuffers in the JSON array items 4-8
    // are equal to expected results memory buffers
    for ( int i = 5; i < 9; i++ )   {
        TestCout( _T("\nTesting item no. "));
        TestCout( i, true );
        wxString s = root[i].AsString();
        TestCout( s );
        TestCout( _T("\n"));
        wxMemoryBuffer b;
        bool r = root[i].AsMemoryBuff( b );
        ASSERT( r );
        int r2 = wxJSONValue::CompareMemoryBuff( b, mb[i] );
        // ASSERT( r2 == 0 );
    }

    // now converts array of INTs items 0-4 in wxMemoryBuffer objects
    // and checks that they are the same as expected results memory buffers
    
    return 0;
}





/*
{
}
*/


