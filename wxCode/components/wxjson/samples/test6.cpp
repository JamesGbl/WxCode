/******************************************************************************

   test6.cpp

    test the wxJSON reader and writer speed
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
    
 may 20, 2009:
-------------
 I want to test the speed of wxJSON reader and writer. I think
 that I oculd achieve significant speed improvements by not converting each
 char to / from UTF-8.
 The tests were done on my notebook: Pentium Core 2 Duo T5670 1.8 GHz,
 2 GB DDR2 RAM, HDD sata 160 GB.
 The system is Fedora Core 9, GCC 4.3.0, wxWidgets 2.8.10
 
 The test consists of reading and writing an array of 10,000 elements of type OBJECT
 which contains 4 key/value pair: total number of JSON values is 40,000
 
 These results were achieved using version 1.0.1 version of the library
 which does a conversion char-by-char.

	TEST                   ANSI        UNICODE
	----                   ----        -------
	write to a wxString    1443ms      1611ms
	write to a stream      2440ms      1968ms
	read from a wxString   3522ms      3693ms
	read from a stream     5563ms      4767ms

 As expected, writing to a stream is slower because of the converison of
 each character. Note that the stream length is about 1.5 megabytes.
 Also note that, writing to a wxString object, that is without UTF-8 
 conversion is slower in Unicode mode: this is probably due to the
 fact that Unicode builds store characters in 4 bytes instead of only one.

 You have to note that although wxJSON in not fast, it is not so
 terrible: reading a 1.5MB stream which contains 10.000 items each of
 one is made of 4 subkeys (that is 40.000 values) in five seconds is
 not so drammatic.

 october, 2009
 -------------
 Version 1.1 is a complete reorganizations of the reader and the parser:
 now both classes only works with UTF-8 streams.
 I run the same tests on the same machine and environment. These the
 results:

	TEST                   ANSI        UNICODE
	----                   ----        -------
	write to a wxString    1173ms      1270ms
	write to a stream      1044ms      1226ms
	read from a wxString   3573ms      3858ms
	read from a stream     3344ms      3772ms

 As expected, there is a general improvment of performances. Now reading /
 writing from / to streams is much more faster than from / to wxString because
 wxString objects need to be converted to a temporary UTF-8 stream.
 
 In conclusion:
 
 - writing to a wxString is 30% faster
 - writing to a stream is about 50% faster
 - reading from a wxString is not much faster
 - reading from a stream is 30% - 40% faster
 

*****************************************************************************/


#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/app.h>
#include <wx/log.h>
#include <wx/debug.h>
#include <wx/mstream.h>
#include <wx/longlong.h>
#include <wx/datetime.h>

#include <wx/jsonval.h>
#include <wx/jsonwriter.h>
#include "test.h"

static wxString			*jsonText = 0;
static wxMemoryOutputStream	*mem = 0;

static void CreateJSONArray( wxJSONValue& root, int numItems )
{
	// items are in this format:
	//
	//	"index" : <number>,
	//	"name" : "item number <number>,
	//	"float" : <float>,
	//	"description" : "this is a description of the item"
	for (int i = 0; i < numItems; i++ )	{
		root[i][_T("index")] = i;
		wxString s;
		s.Printf( _T("item number %d"), i );
		root[i][_T("name")] = s;
		root[i][_T("float")] = (double) i;
		root[i][_T("description")] = _T("this is a description of the item");
	}
}


//
// the function creates an array of many JSON value objects
// and writes to a wxString object. Writing to a wxString does
// not involve conversions.
//
// may 20, 2009
// tried with 1000 items:  unicode=165ms, ansi= 134ms
// tried with 10000 items: unicode= 1611ms, ansi 1443ms
//
// oct 18, 2009
// tried with 1000 items:  unicode=151ms, ansi= not tried
// tried with 10000 items: unicode= 1270ms, ansi 1173ms
//
int Test9_1()
{
	wxJSONValue root;
	CreateJSONArray( root, 10000 );

	// now write the JSON value
	jsonText = new wxString();
	wxJSONWriter writer;
	wxDateTime now = wxDateTime::Now();
	writer.Write( root, *jsonText );
	wxDateTime now2 = wxDateTime::Now();

	// how much time?
	wxTimeSpan t = now2.Subtract( now );
	TestCout( t.Format( _T("milliseconds: %l")));

	// now write the jsonText
	// TestCout( *jsonText );		
	TestCout( _T("\nTest 9.1 done\n") );		
	return 0;
}


// the same as above but writing to a stream
int Test9_2()
{
	wxJSONValue root;
	CreateJSONArray( root, 10000 );

	// now write the JSON value
	mem = new wxMemoryOutputStream();
	wxJSONWriter writer;
	wxDateTime now = wxDateTime::Now();
	writer.Write( root, *mem );
	wxDateTime now2 = wxDateTime::Now();

	// how much time?
	wxTimeSpan t = now2.Subtract( now );
	TestCout( t.Format( _T("milliseconds: %l\n")));

	// how many bytes we have stored?
	int len = mem->GetSize();
	TestCout( _T("bytes written="));
	TestCout( len, true );

  return 0;
}

// read from a wxString object
int Test9_3()
{
	// store the JSON test in the jsonText pointer
	Test9_1();

	wxJSONReader reader;
	wxJSONValue  root;
	int numErrors = reader.Parse( *jsonText, &root );
	TestCout( _T( "num errors="));
	TestCout( numErrors, true );

	// print the size of the array (10.000)
	int size = root.Size();
	TestCout( _T( "array size="));
	TestCout( size, true );

	return 0;
}

// same as above but read from a stream object
int Test9_4()
{
	// store the JSON test in the 'mem' pointer
	Test9_2();

	wxJSONReader reader;
	wxJSONValue  root;

	// we have to copy the memory output stream to a memory input stream

	int   len = mem->GetSize();
	void* buff = mem->GetOutputStreamBuffer()->GetBufferStart();

	wxMemoryInputStream in( buff, len );

	int numErrors = reader.Parse( in, &root );
	TestCout( _T( "num errors="));
	TestCout( numErrors, true );

	// print the size of the array (10.000)
	int size = root.Size();
	TestCout( _T( "array size="));
	TestCout( size, true );

  return 0;
}

/*
{
}
*/


