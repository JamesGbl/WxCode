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


// just test some wxMemoryBuffer memberfunctions
int Test15_1()
{
    // create a tiny memory buffer that contains ten bytes from 0 to 9
    unsigned char mem1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    wxMemoryBuffer buff1;
    buff1.AppendData( mem1, 10 );

    // create a tiny memory buffer that contains ten bytes from 10 to 19
    unsigned char mem2[] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
    wxMemoryBuffer buff2;
    buff2.AppendData( mem2, 10 );
    
    // checks is the two buffers are equal
    bool r = buff1 == buff2;
    TestCout( _T( "buff1 and buff2 operator == :" ));
    TestCout( r ? _T("equal") : _T("not equal"));
    TestCout( _T("\n"));
    ASSERT( !r );

    // creates a copy of mem1 and checks if the data points to same memory
    wxMemoryBuffer buff3( buff1 );
    void* ptr1 = buff1.GetData();
    void* ptr2 = buff2.GetData();
    void* ptr3 = buff3.GetData();
    wxString s;
    TestCout( _T("buff1 pointer: "));
    s.Printf( _T("%p\n"), ptr1 );
    TestCout( s );
    TestCout( _T("buff2 pointer: "));
    s.Printf( _T("%p\n"), ptr2 );
    TestCout( s );
    TestCout( _T("buff3 pointer: "));
    s.Printf( _T("%p\n"), ptr3 );
    TestCout( s );
    ASSERT( ptr1 == ptr3 );

    r = buff1 == buff3;
    TestCout( _T( "buff1 and buff3 operator == :" ));
    TestCout( r ? _T("equal") : _T("not equal"));
    TestCout( _T("\n"));
    ASSERT( r );

    // testing data len and buffer len: data len==10, buffer size may be greater
    size_t len;
    len = buff1.GetBufSize();
    TestCout( _T( "buff1.GetBufSize(): " ));
    TestCout( len, true );
    len = buff1.GetDataLen();
    TestCout( _T( "buff1.GetDataLen(): " ));
    TestCout( len, true );
    ASSERT( len == 10 );

    // append mem2 (10..19) to buff3: buff1 is also modified
    buff3.AppendData( mem2, 10 );
    len = buff3.GetDataLen();
    TestCout( _T( "buff3.GetDataLen(): " ));
    TestCout( len, true );
    ASSERT( len == 20 );
    len = buff1.GetDataLen();
    TestCout( _T( "buff1.GetDataLen(): " ));
    TestCout( len, true );
    ASSERT( len == 20 );

    // testing the comparison operator of wxMemoryBuffer
    // creates two memory buffer objects that points to the same content
    // 3 jan 2010: as of wxW 2.8 and 2.9 the wxMemoryBuffer class does not
    // define a operator == function so the default, swallow comparison is
    // used. The result is that two memory buffer object that contains
    // the same data and of the same length are not 'equal' unless one of
    // two object is a copy of the other (they share the data at the
    // same address)
    TestCout( _T( "Testing wxMemoryBuffer\'s comparison operator\n" ));
    wxMemoryBuffer buff5;
    buff5.AppendData( mem1, 10 );
    wxMemoryBuffer buff6;
    buff6.AppendData( mem1, 10 );
    ptr1 = buff5.GetData();
    ptr2 = buff6.GetData();
    TestCout( _T("buff5 pointer: "));
    s.Printf( _T("%p\n"), ptr1 );
    TestCout( s );
    TestCout( _T("buff6 pointer: "));
    s.Printf( _T("%p\n"), ptr2 );
    TestCout( s );
    ASSERT( ptr1 != ptr2 );
    r = buff5 == buff6;
    TestCout( _T("buff5 == buff6 (should be TRUE): ")); // the actual result is FALSE
    TestCout( r ? _T("TRUE\n") : _T("FALSE\n"));

	return 0;
}


// test new wxJSONTYPE_MEMORYBUFF ctors, asignment, copy ctors etc.
int Test15_2()
{
    unsigned char mem1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    unsigned char mem2[] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
    unsigned char mem3[] = { 20, 21, 22, 23, 24 };

    // creates the wxMemoryBuffer and then the JSON value
    wxMemoryBuffer b1;
    b1.AppendData( mem1, 10 );
    wxJSONValue v1( b1 );

    wxJSONValue v2( mem2, 10 );
    wxJSONValue v3( mem3, 5 );
    wxJSONValue v4( _T( "is not a memory buffer object" ));

    // check the type of the JSON values
    TestCout( _T("Testing the wxJSON value types\n"));
    bool r;
    r = v1.IsMemoryBuff();
    ASSERT( r );
    r = v2.IsMemoryBuff();
    ASSERT( r );
    r = v3.IsMemoryBuff();
    ASSERT( r );
    r = v4.IsMemoryBuff();
    ASSERT( !r );

    // check the returned type
    wxMemoryBuffer vb1 = v1.AsMemoryBuff();
    TestCout( _T("ASSERTing \'b1\' is equal to \'vb1\'\n"));
    ASSERT( vb1 == b1 );
    // in the wxJSONValue ctor, memory buffers are not copied using the
    // copy ctor but the assignment op.
    // check the referenced data of the two memory buffer objects
    // 2 jan 2010: the two pointers are equal so there is a swallow copy
    // in the wxMemoryBuffer assignment operator
    void* ptr1 = b1.GetData();
    void* ptr2 = vb1.GetData();
    wxString s;
    TestCout( _T("b1 pointer: "));
    s.Printf( _T("%p\n"), ptr1 );
    TestCout( s );
    TestCout( _T("vb1 pointer: "));
    s.Printf( _T("%p\n"), ptr2 );
    TestCout( s );

    // check the returned data of v2, v3 and v4 (v4 is not a membuffer)
    TestCout( _T("checking returned values of the three JSON values\n"));
    wxMemoryBuffer vb2 = v2.AsMemoryBuff();
    wxMemoryBuffer vb3 = v3.AsMemoryBuff();
    // wxMemoryBuffer vb4 = v4.AsMemoryBuff(); ASSERT failure in debug builds

    // check the content of 'vb2' and 'vb3'
    size_t len;
    len = vb2.GetDataLen();
    ASSERT( len == 10 );
    len = vb3.GetDataLen();
    ASSERT( len == 5 );

    // creates a copy of v1 to v4
    TestCout( _T("Assigns wxJSON value \'v1\' to \'v4'"));
    v4 = v1;
    r = v4.IsMemoryBuff();
    ASSERT( r );
    wxMemoryBuffer vb4 = v4.AsMemoryBuff();
    
    TestCout( _T("Printing the pointers of b1, vb1 and vb4 (all the same?)\n"));
    // 2 jan 2010: the three data pointers are all the same
    ptr1 = vb1.GetData();
    ptr2 = vb4.GetData();
    void* ptr3 = b1.GetData();
    //wxString s;
    TestCout( _T("vb1 pointer: "));
    s.Printf( _T("%p\n"), ptr1 );
    TestCout( s );
    TestCout( _T("vb4 pointer: "));
    s.Printf( _T("%p\n"), ptr2 );
    TestCout( s );
    TestCout( _T("b1 pointer: "));
    s.Printf( _T("%p\n"), ptr3 );
    TestCout( s );

    // creates two memory buffer objects from the same memory are
    // the pointers are different but they are the same
    wxJSONValue v8( mem1, 10 );
    wxJSONValue v9( mem1, 10 );
    ptr1 = v8.AsMemoryBuff().GetData();
    ptr2 = v9.AsMemoryBuff().GetData();
    TestCout( _T("v8 pointer: "));
    s.Printf( _T("%p\n"), ptr1 );
    TestCout( s );
    TestCout( _T("v9 pointer: "));
    s.Printf( _T("%p\n"), ptr2 );
    TestCout( s );
    ASSERT( ptr1 != ptr2 );
    TestCout( _T("ASSERTing \'v8\' is same as \'v9\'\n"));
    r = v8.IsSameAs( v9 );
    ASSERT( r );

    TestCout( _T("Assign \'mem2\' to \'v1\'\n"));
    // mem2 is also contained in 'v2': 'v1' and 'v2' contain the same memory
    // buffer but in two different memory areas because 'v2' is not copied to 'v1'
    wxMemoryBuffer b2;
    b2.AppendData( mem2, 10 );
    v1 = b2;
    ptr1 = v1.AsMemoryBuff().GetData();
    ptr2 = v2.AsMemoryBuff().GetData();
    TestCout( _T("v1 pointer: "));
    s.Printf( _T("%p\n"), ptr1 );
    TestCout( s );
    TestCout( _T("v2 pointer: "));
    s.Printf( _T("%p\n"), ptr2 );
    TestCout( s );
    ASSERT( ptr1 != ptr2 );    
    TestCout( _T("ASSERTing \'v1\' is same as \'v2\'\n"));
    r = v1.IsSameAs( v2 );
    ASSERT( r );

    return 0;
}


// test the conversion static function
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
    TestCout( _T("Try to print 20 bytes of a 10-bytes memory buffer object\n"));
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





/*
{
}
*/


