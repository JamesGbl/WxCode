/******************************************************************************

   test13.cpp

    test the 64-bit integer
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


*****************************************************************************/

#include <wx/string.h>
#include <wx/debug.h>
#include <wx/mstream.h>
#include <wx/file.h>
#include <wx/longlong.h>

#include <wx/jsonval.h>
#include <wx/jsonreader.h>
#include <wx/jsonwriter.h>

#include "test.h"


//
// test the definitions
int Test54()
{
  TestCout( _T("Test if \'wxLongLong_t\' is defined: "));
  #if defined( wxLongLong_t )
    TestCout( _T("YES\n" ));
  #else
    TestCout( _T("NO\n" ));
  #endif

  TestCout( _T("Test if \'wxLongLongFmtSpec\' is defined: "));
  #if defined( wxLongLongFmtSpec )
    TestCout( _T("YES: "));
    TestCout( wxLongLongFmtSpec);
    TestCout( _T("\n"));
  #else
    TestCout( _T("NO\n" ));
  #endif

  // wxItn64 is a typedef on 64-bit platforms, not a #define
  TestCout( _T("Test if \'wxInt64\' is defined: "));
  #if defined( wxInt64 )
    TestCout( _T("YES\n "));
  #else
    TestCout( _T("NO\n" ));
  #endif

  // the if wxJSON_64BIT_INT is defined
  TestCout( _T("Test if \'wxJSON_64BIT_INT\' is defined: "));
  #if defined( wxJSON_64BIT_INT )
    TestCout( _T("YES\n "));
  #else
    TestCout( _T("NO\n" ));
  #endif

  return 0;
}




/*
{
}
*/





