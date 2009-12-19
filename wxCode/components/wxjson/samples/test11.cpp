/******************************************************************************

   test11.cpp

    test the use of STL

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
#include <wx/mstream.h>

#include <wx/jsonval.h>
#include <wx/jsonwriter.h>
#include "test.h"

//
// test if wxJSON_USE_STL is defined
int Test14_1()
{
	TestCout( _T("Test if wxUSE_STL is defined: " ));
	bool r = false;
	#if defined( wxUSE_STL )
		r = wxUSE_STL;
	#endif
	TestCout( r ? _T("TRUE") : _T("FALSE"));
	TestCout( _T("\n"));
	
	r = false;
	TestCout( _T("Test if wxJSON_USE_STL is defined: " ));
	#if defined( wxJSON_USE_STL )
		r = true;
	#endif
	TestCout( r ? _T("TRUE") : _T("FALSE"));
	TestCout( _T("\n"));
	
	return 0;
}


// check the use of iterators (array)
int Test14_2()
{
	wxJSONValue root;
	root.Append( _T("first"));
	root.Append( _T("second"));
	root.Append( _T("third"));
	root.Append( _T("fourth"));

	const wxJSONInternalArray* array = root.AsArray();
	ASSERT( array != 0 );
	
	/* the following does not work either if STL is or isn't enabled
	wxJSONInternalArray::const_iterator iter;
	for ( iter = array->begin(); iter != array->end(); iter++ )	{
		const wxJSONValue& v = *iter;
		TestCout( _T("value="));
		TestCout( v.AsString() );
		TestCout( _T("\n"));
	}
	*/
	
	return 0;
}

// check the use of iterators (key/value)
int Test14_3()
{
	wxJSONValue root;
	root[_T("first")] = 1;
	root[_T("second")] = 2;
	root[_T("third")] = 3;
	root[_T("fourth")] = 4;
	
	// this works in both enabled and disabled STL
	const wxJSONInternalMap* map = root.AsMap();
	wxJSONInternalMap::const_iterator iter;
	for ( iter = map->begin(); iter != map->end(); iter++ )	{
		const wxJSONInternalMap::value_type& v = *iter;
		TestCout( _T("key="));
		TestCout( v.first );
		TestCout( _T("\n"));
	}
	
	return 0;
}




