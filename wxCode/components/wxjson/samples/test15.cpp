/******************************************************************************

   test15.cpp

    test the speed of wxJSON writer and reader 
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

 may 20, 2009:
-------------
 I want to test the speed of wxJSON reader and writer. I think
 that I oculd achieve significant speed improvements by not converting each
 char to / from UTF-8.
 The tests were done on my notebook: Pentium Core 2 Duo T5670 1.8 GHz,
 2 GB DDR2 RAM, HDD sata 160 GB.
 
 Note that this '.cpp' file is only linked in the 'makefile.unx' build
 system: it is not linked in the bakefile-generated makefiles

 The results writing an array of 10,000 elements of type OBJECT
 These results were achieved using version 1.0.1 version of the library
 which does a conversion char-by-char.

	TEST				ANSI		UNICODE
	----				----		-------
	write to a wxString		 1443ms		 1611ms
	write to a stream		 2440ms		 1968ms
	read from a wxString		 3522ms		 3693ms
	read from a stream		 5563ms		 4767ms

 As expected, writing to a stream is slower because of the converison of
 each character. Note that the stream length is about 1.5 megabytes.
 Also note that, writing to a wxString object, that is without UTF-8 
 conversion is slower in Unicode mode: this is probably due to the
 fact that Unicode builds store characters in 4 bytes instead of only one.

 You have to note that although wxJSON in not fast, it is not so
 terrible: reading a 1.5MB stream which contains 10.000 items each of
 one is made of 4 subkeys (that is 40.000 values) in five seconds is
 not so drammatic.

 may xx, 2009
 ------------
 I run the test with the new organization of the library in version 1.1.0
 which does not convert each character but only string values.
 The conversion is done by converting the whole string value and not
 char-by-char.


*****************************************************************************/

#include <wx/string.h>
#include <wx/debug.h>
#include <wx/mstream.h>
#include <wx/file.h>
#include <wx/longlong.h>
#include <wx/datetime.h>

#include <wx/jsonval.h>
#include <wx/jsonreader.h>
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
int Test74()
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
	return 0;
}

//
// same as above but using a wxMemoryOutputStream for the JSON text
// this will use char-by-char conversion
// may 20, 2009
// tried with 10000 items: unicode= 1839ms, ansi 2440ms
int Test75()
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

//
// read 10000 items from a wxString object
// note that you have to subtract the time needed for
// constructing the JSON text (the Test74() function)
//
// tried with 10000 items: unicode= 3693ms, ansi 3522ms
int Test76()
{
	// store the JSON test in the jsonText pointer
	Test74();

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

//
// read 10000 items from a wxString object
// note that you have to subtract the time needed for
// constructing the JSON text (the Test74() function)
//
// tried with 10000 items: unicode= 4767ms, ansi= 5563ms
int Test77()
{
	// store the JSON test in the 'mem' pointer
	Test75();

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


