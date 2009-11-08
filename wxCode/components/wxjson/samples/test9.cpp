/******************************************************************************

   test9.cpp

    test the wxJSON reference counting stuff (vers >= 0.4)
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

#include <wx/jsonval.h>
#include <wx/jsonwriter.h>
#include "test.h"


static int TestX_X()
{
	return 0;
}

//
// makes some copies of the wxJSONValues and asserts that the referenced
// data structure is the expected value
int Test12_1()
{
	TestCout( _T("\nMaking a copy of \'v1\': v2 and v3\n" ));

	wxJSONValue* v1 = new wxJSONValue( _T( "100" ));	// an integer
	wxJSONValue* v2 = new wxJSONValue( *v1 );
	wxJSONValue* v3 = new wxJSONValue( *v2 );

	// dump the values
	wxString dump1 = v1->Dump( false );
	wxString dump2 = v2->Dump( false );
	wxString dump3 = v3->Dump( false );

	TestCout( _T("\nDumping v1\n" ));
	TestCout( dump1 );
	TestCout( _T("\nDumping v2\n" ));
	TestCout( dump2 );
	TestCout( _T("\nDumping v3\n" ));
	TestCout( dump3 );

	wxJSONRefData* pV1 = v1->GetRefData();
	wxJSONRefData* pV2 = v2->GetRefData();
	wxJSONRefData* pV3 = v3->GetRefData();
	ASSERT( pV1 == pV2 );
	ASSERT( pV1 == pV3 );
	
	int refCount = pV1->GetRefCount();
	TestCout( _T("\nNumber of shared data structure (3): " ));
	TestCout( refCount, true );
	ASSERT( refCount == 3 );

	// change v2: v1 and v3 share the same data
	TestCout( _T("\nChange the value of \'v2\'\n" ));
	*(v2) = _T( "200" );
	pV1 = v1->GetRefData();
	pV2 = v2->GetRefData();
	pV3 = v3->GetRefData();
	ASSERT( pV1 != pV2 );
	ASSERT( pV1 == pV3 );
	refCount = pV1->GetRefCount();
	ASSERT( refCount == 2 );

	// now delete 'v3'
	delete v3;
	refCount = pV1->GetRefCount();
	ASSERT( refCount == 1 );
	
	// get the values
	wxString s1 = v1->AsString();
	ASSERT( s1 == _T("100" ));
	wxString s2 = v2->AsString();
	ASSERT( s2 == _T("200"));

	// dump the values
	dump1 = v1->Dump( false );
	dump2 = v2->Dump( false );
	// wxString dump3 = v3->Dump( false );

	TestCout( _T("\nDumping v1\n" ));
	TestCout( dump1 );
	TestCout( _T("\nDumping v2\n" ));
	TestCout( dump2 );
	// TestCout( _T("\nDumping v3\n" ));
	// TestCout( dump3 );
	
	return 0;
}

// this test is for using with DDD in order of doing the hardcopy
// of the values used in the wxJSON internals documentation
int Test12_2()
{
	wxString s1, s2;  // used for dumping the values

	// creates a INT value
	wxJSONValue v1( -1 );
	s1 = v1.Dump( true );

	// now display the 'v1' value in DDD and save an hardcopy
	wxJSONValue v2( v1 );

	// now display the 'v2' value in DDD and save an hardcopy
	// notice that the referenced data points to the same ref-data
	s2 = v2.Dump( true );
	TestCout( _T("\nDumping v1\n" ));
	TestCout( s1 );
	TestCout( _T("\nDumping v2\n" ));
	TestCout( s2 );

	v1[3] = 12;

	// for displaying the 4 array's elements in DDD we need an array of
	// wxJSONValues: we store the pointers to the values in the wxJSONValue array
	const wxJSONValue* v1ArrayElem[4];
	const wxJSONInternalArray* v1Array = v1.AsArray();
	for ( int i = 0; i < 4; i++ )  {
		v1ArrayElem[i] = &(v1Array->Item( i ));
	}

	// now display the 'v1ArrayElem' pointer's array in DDD and save an hardcopy

	// dump 'v1'
	s1 = v1.Dump( true );
	TestCout( _T("\nDumping v1\n" ));
	TestCout( s1 );

	// copy the value object from 'v1' to 'v3'
	TestCout( _T("\nCopying \'v1\' to \'v3\'\n" ));
	wxJSONValue v3( v1 );

	TestCout( _T("\nPrinting v1\n" ));
	PrintValue( v1 );
	TestCout( _T("\nPrinting v3\n" ));
	PrintValue( v3 );

	// dump 'v1' and 'v3'
	s1 = v1.Dump( true );
	s2 = v3.Dump( true );

	TestCout( _T("\nDumping v1\n" ));
	TestCout( s1 );
	TestCout( _T("\nDumping v3\n" ));
	TestCout( s2 );

	// we can use here the Dump() function in the docs
	// but this code is used to get the array of pointers to
	// the 'v3' JSON array
	const wxJSONValue* v3ArrayElem[4];
	const wxJSONInternalArray* v3Array = v3.AsArray();
	for ( int i = 0; i < 4; i++ )  {
		v3ArrayElem[i] = &(v3Array->Item( i ));
	}

	TestCout( _T("\nChanging v2\'s second array element\n" ));
	v3[1] = 2;

	// and now display the new 'v3' array
	v3Array = v3.AsArray();
	for ( int i = 0; i < 4; i++ )  {
		v3ArrayElem[i] = &(v3Array->Item( i ));
	}

	TestCout( _T("\nPrinting v1\n" ));
	PrintValue( v1 );
	TestCout( _T("\nPrinting v3\n" ));
	PrintValue( v3 );

	s1 = v1.Dump( true );
	s2 = v3.Dump( true );

	TestCout( _T("\nDumping v1\n" ));
	TestCout( s1 );
	TestCout( _T("\nDumping v3\n" ));
	TestCout( s2 );

	return 0;
}


// an array of values copied and accessed an item of level-3
// note that level-1 is also copied
int Test12_3()
{
	wxJSONValue v1;
	v1[1][2][3] = 100;
	
	wxString s1;
	s1 = v1.Dump( true );

	TestCout( _T("Dumping v1\n" ));
	TestCout( s1 );

	wxJSONValue v2 = v1;
	TestCout( _T("Copied v1 to v2\n" ));

	wxJSONRefData* pV1 = v1.GetRefData();
	wxJSONRefData* pV2 = v2.GetRefData();
	TestCout( _T("Checking refData of v1 and v2 (equal)\n" ));
	ASSERT( pV1 == pV2 );

	// now change v2's last level item: note that also the
	// level-1 is unshared
	v2[1][2][3] = 200;
	pV1 = v1.GetRefData();
	pV2 = v2.GetRefData();
	TestCout( _T("Dumping v1\n" ));
	s1 = v1.Dump( true );
	TestCout( s1 );
	s1 = v2.Dump( true );
	TestCout( _T("Dumping v2\n" ));
	TestCout( s1 );

	ASSERT( pV1 != pV2 );

	// but v1[0] and v2[0] shares the same address
	pV1 = v1[0].GetRefData();
	pV2 = v2[0].GetRefData();
	ASSERT( pV1 == pV2 );
	
	return 0;
}


/*
{
}
*/


