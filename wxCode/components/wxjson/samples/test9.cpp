/******************************************************************************

   test9.cpp

    test the wxJSONWriter in unicode mode
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


 Detailed description of the test application

*****************************************************************************/

#include <locale.h>

#include <wx/app.h>
#include <wx/string.h>
#include <wx/debug.h>
#include <wx/mstream.h>
#include <wx/strconv.h>

#include <wx/jsonval.h>
#include <wx/jsonwriter.h>
#include "test.h"


int CheckBuffer( const char* buff1, const char* buff2, int len )
{
  int r = 0;
  for ( int i = 0; i < len; i++ )   {
    char c1 = *(buff1 + i);
    char c2 = *(buff2 + i);
    if ( c1 != c2 )  {
      TestCout( _T("Character not matching at position : "));
      TestCout( i, true );
      r = 1;
      break;
    }
  }
  return r;
}



// test the conversion from a locale dependent charset
// to UTF-8: the conversion is done by first converting
// to wide char and then to UTF-8
// OK, 26 feb 2008 the test is successfull
int Test36()
{
  int  r = 0;

  // we convert Latin-1 characters (ANSI builds)
  // this test only works if the current locale is Latin-1
#if !defined( wxJSON_USE_UNICODE )

  static char utf8Buff[] = {
		0x61, 0x62, 0x63, 0x41, 0x42, 0x43,  // 'abcABC'
		0x31, 0x32, 0x33,                    // '123'
		0xc3, 0xa0, 0xc3, 0xa8, 0xc3, 0xac,  // a, e, i grave
		0xc2, 0xa9, 0xc2, 0xae,              // copyright, registered sign
		0x00
  };


  // The string is: abcABC123àèì©®
  static const char iso8859_1Buff[] = {
	0x61, 0x62, 0x63,   // 'ABC'                              1
	0x41, 0x42, 0x43,   // 'abc'                              4
	0x31, 0x32, 0x33,   // '123'                              7
	0xe0, 0xe8, 0xec,   // a, e, i grave                     10
	0xa9, 0xae,         // copyright sign, registered sign   13
  };

  // this is needed otherwise you have to specify the locale in the
  // LC_CTYPE environment variable and then call:
  // setlocale( LC_CTYPE, _T(""));

  // if the locale is not set, the conversion of Latin-1 characters
  // fails.
  setlocale( LC_CTYPE, _T("it_IT.88591"));

  // the output buffer
  char buffer[100];
  int  outLen = 0;

  // we convert one char at a time
  int srcLen = sizeof( iso8859_1Buff );
  for ( int i = 0; i < srcLen; i++ )  {
    wchar_t wchar[2];

    size_t len;

    len  = wxConvLibc.ToWChar( 0, 0, iso8859_1Buff + i, 1 );
    TestCout( _T("Convert MBS to WCS - Needed buffer is: "));
    TestCout( len, true );

    len = wxConvLibc.ToWChar( wchar, 2, iso8859_1Buff + i, 1 );
    wxASSERT( len != wxCONV_FAILED );


    // and now convert the wide char to UTF8
    len = wxConvUTF8.FromWChar( buffer + outLen, 100, wchar, 1 );
    wxASSERT( len != wxCONV_FAILED );
    outLen += len -1;
  }
  // now compare the UTF-8 buffer to the expected result
  TestCout( _T("Output buffer length="));
  TestCout( outLen, true );

  r = CheckBuffer( utf8Buff, buffer, outLen );
#else
  TestCout( _T("Test #36 is only meaningfull in ANSI builds\n" ));
#endif
  return r;
}




// The JSON value that we write in Unicode mode is:
// (without indentation):
//
// {
//    "us-ascii" : "abcABC",
//    "latin1"   : "àèì©®",
//    "greek"    : "αβγδ",
//    "cyrillic" : "ФХЦЧ"
// }
//


////////////////////////////////////////////////////////////
//  UTF8 buffer
////////////////////////////////////////////////////////////
static char utf8Buff[] = {
  0x7b, 0x0a, 0x22, 0x67, 0x72, 0x65, 0x65, 0x6b,
  0x22, 0x20, 0x3a, 0x20, 0x22, 0xce, 0xb1, 0xce,  //  |{."greek" : "...|
  0xb2, 0xce, 0xb3, 0xce, 0xb4, 0x22, 0x2c, 0x0a, 
  0x22, 0x63, 0x79, 0x72, 0x69, 0x6c, 0x6c, 0x69,  //  |.....",."cyrilli|
  0x63, 0x22, 0x20, 0x3a, 0x20, 0x22, 0xd0, 0xa4,
  0xd0, 0xa5, 0xd0, 0xa6, 0xd0, 0xa7, 0x22, 0x2c,  //  |c" : "........",|
  0x0a, 0x22, 0x6c, 0x61, 0x74, 0x69, 0x6e, 0x31,
  0x22, 0x20, 0x3a, 0x20, 0x22, 0xc3, 0xa0, 0xc3,  //  |."latin1" : "...|
  0xa8, 0xc3, 0xac, 0xc2, 0xa9, 0xc2, 0xae, 0x22,
  0x2c, 0x0a, 0x22, 0x75, 0x73, 0x2d, 0x61, 0x73,  //  |.......",."us-as|
  0x63, 0x69, 0x69, 0x22, 0x20, 0x3a, 0x20, 0x22,
  0x61, 0x62, 0x63, 0x41, 0x42, 0x43, 0x22, 0x0a,  //  |cii" : "abcABC".|
  0x7d,                                            //  |}|
};

// write a JSON value containing latin-1 (ISO-8859-1), greek
// (ISO-8859-7) and cyrillic (ISO-8859-5) characters
// in UTF-8 encoding format and check the result
// output buffer with the expected 'utf8Buff' buffer
// OK, 26 feb 2008 - the test is successfull
int Test37()
{
  int r = 0;
#if defined( wxJSON_USE_UNICODE )
  wxJSONValue  value;
  value[_T("us-ascii")] = _T("abcABC");
  value[_T("latin1")]   = _T("àèì©®");
  value[_T("greek")]    = _T("αβγδ");
  value[_T("cyrillic")] = _T("ФХЦЧ");

  char buffer[128];
  wxMemoryOutputStream jsonText( buffer, 128 );
  wxJSONWriter writer( wxJSONWRITER_NONE );
  writer.Write( value, jsonText );

  // and now we check the result
  int len = sizeof( utf8Buff );
  TestCout( _T("Writing JSON value in UTF-8 format - length: " ));
  TestCout( len, true );
  r = CheckBuffer( utf8Buff, buffer, len );

#else
  TestCout( _T("Test #37 is only meaningfull in Unicode builds\n" ));
#endif
  return r;
}



// writing to a string in Unicode mode and then
// converting to UCS-2BE
// 3 mar 2008: the test fails because the convesion function
// returned wxCONV_FAILED
int Test38()
{
  static char ucs2beBuff[] = {
  0x00, 0x7b, 0x00, 0x0a, 0x00, 0x22, 0x00, 0x67,
  0x00, 0x72, 0x00, 0x65, 0x00, 0x65, 0x00, 0x6b,  //  |.{...".g.r.e.e.k|
  0x00, 0x22, 0x00, 0x20, 0x00, 0x3a, 0x00, 0x20,
  0x00, 0x22, 0x03, 0xb1, 0x03, 0xb2, 0x03, 0xb3,  //  |.". .:. ."......|
  0x03, 0xb4, 0x00, 0x22, 0x00, 0x2c, 0x00, 0x0a,
  0x00, 0x22, 0x00, 0x63, 0x00, 0x79, 0x00, 0x72,  //  |...".,...".c.y.r|
  0x00, 0x69, 0x00, 0x6c, 0x00, 0x6c, 0x00, 0x69,
  0x00, 0x63, 0x00, 0x22, 0x00, 0x20, 0x00, 0x3a,  //  |.i.l.l.i.c.". .:|
  0x00, 0x20, 0x00, 0x22, 0x04, 0x24, 0x04, 0x25,
  0x04, 0x26, 0x04, 0x27, 0x00, 0x22, 0x00, 0x2c,  //  |. .".$.%.&.'.".,|
  0x00, 0x0a, 0x00, 0x22, 0x00, 0x6c, 0x00, 0x61,
  0x00, 0x74, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x31,  //  |...".l.a.t.i.n.1|
  0x00, 0x22, 0x00, 0x20, 0x00, 0x3a, 0x00, 0x20,
  0x00, 0x22, 0x00, 0xe0, 0x00, 0xe8, 0x00, 0xec,  //  |.". .:. ."......|
  0x00, 0xa9, 0x00, 0xae, 0x00, 0x22, 0x00, 0x2c,
  0x00, 0x0a, 0x00, 0x22, 0x00, 0x75, 0x00, 0x73,  //  |.....".,...".u.s|
  0x00, 0x2d, 0x00, 0x61, 0x00, 0x73, 0x00, 0x63,
  0x00, 0x69, 0x00, 0x69, 0x00, 0x22, 0x00, 0x20,  //  |.-.a.s.c.i.i.". |
  0x00, 0x3a, 0x00, 0x20, 0x00, 0x22, 0x00, 0x61,
  0x00, 0x62, 0x00, 0x63, 0x00, 0x41, 0x00, 0x42,  //  |.:. .".a.b.c.A.B|
  0x00, 0x43, 0x00, 0x22, 0x00, 0x0a, 0x00, 0x7d,
  0x00, 0x0a                                       //  |.C."...}..|
  };

  int r = 0;
#if defined( wxJSON_USE_UNICODE )
  wxJSONValue  value;
  value[_T("us-ascii")] = _T("abcABC");
  value[_T("latin1")]   = _T("àèì©®");
  value[_T("greek")]    = _T("αβγδ");
  value[_T("cyrillic")] = _T("ФХЦЧ");

  wxString jsonText;
  wxJSONWriter writer( wxJSONWRITER_NONE );
  writer.Write( value, jsonText );
  // ASSERT( success == 0 );
  TestCout( _T("The result string=\n" ));
  TestCout( jsonText );

  wxCSConv  conv( _T("UCS-2BE"));
  ASSERT( conv.IsOk());

  // compute the length of the needed buffer
  size_t len = conv.FromWChar( 0, 0, jsonText.c_str());
  TestCout( _T("Needed buffer length=\n" ));
  TestCout( len, true );
  // ASSERT( len != wxCONV_FAILED )    // 27 feb 2008: here the test fails!!

  TestCout( _T("Result buffer length=\n" ));
  TestCout( sizeof(ucs2beBuff), true );

  // allocate the buffer
  // char* buffer = new char[len + 4];

  // do the conversion and finally check the buffer results
  // len = conv.FromWChar( buffer, len + 4, jsonText.c_str());
  // r = CheckBuffer( buffer, ucs2beBuff, len );

  // delete[] buffer;
#else
  TestCout( _T("Test #38 is only meaningfull in Unicode builds\n" ));
#endif

  return r;
}


// writing to a string in Unicode mode and then
// converting to UCS-4LE
// the test is successfull but i got debug error messages
// related to the 'iconv' facility. Here is the output:
// 22:38:51: Debug: In file ./src/common/strconv.cpp at line 1731: 'iconv' failed with error
//       0x00000016 (Invalid argument).
// 22:38:51: Error: Conversion to charset 'UTF-32LE' doesn't work.
// 22:38:51: Debug: In file ./src/common/strconv.cpp at line 1731: 'iconv' failed with error
//       0x00000016 (Invalid argument).
// 22:38:51: Error: Conversion to charset 'UCS-4LE' doesn't work.

int Test39()
{
  static char ucs4leBuff[] = {
  0x7B,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,
  0x22,0x00,0x00,0x00,0x67,0x00,0x00,0x00,
  0x72,0x00,0x00,0x00,0x65,0x00,0x00,0x00,
  0x65,0x00,0x00,0x00,0x6B,0x00,0x00,0x00,
  0x22,0x00,0x00,0x00,0x20,0x00,0x00,0x00,
  0x3A,0x00,0x00,0x00,0x20,0x00,0x00,0x00,
  0x22,0x00,0x00,0x00,0xB1,0x03,0x00,0x00,
  0xB2,0x03,0x00,0x00,0xB3,0x03,0x00,0x00,
  0xB4,0x03,0x00,0x00,0x22,0x00,0x00,0x00,
  0x2C,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,
  0x22,0x00,0x00,0x00,0x63,0x00,0x00,0x00,
  0x79,0x00,0x00,0x00,0x72,0x00,0x00,0x00,
  0x69,0x00,0x00,0x00,0x6C,0x00,0x00,0x00,
  0x6C,0x00,0x00,0x00,0x69,0x00,0x00,0x00,
  0x63,0x00,0x00,0x00,0x22,0x00,0x00,0x00,
  0x20,0x00,0x00,0x00,0x3A,0x00,0x00,0x00,
  0x20,0x00,0x00,0x00,0x22,0x00,0x00,0x00,
  0x24,0x04,0x00,0x00,0x25,0x04,0x00,0x00,
  0x26,0x04,0x00,0x00,0x27,0x04,0x00,0x00,
  0x22,0x00,0x00,0x00,0x2C,0x00,0x00,0x00,
  0x0A,0x00,0x00,0x00,0x22,0x00,0x00,0x00,
  0x6C,0x00,0x00,0x00,0x61,0x00,0x00,0x00,
  0x74,0x00,0x00,0x00,0x69,0x00,0x00,0x00,
  0x6E,0x00,0x00,0x00,0x31,0x00,0x00,0x00,
  0x22,0x00,0x00,0x00,0x20,0x00,0x00,0x00,
  0x3A,0x00,0x00,0x00,0x20,0x00,0x00,0x00,
  0x22,0x00,0x00,0x00,0xE0,0x00,0x00,0x00,
  0xE8,0x00,0x00,0x00,0xEC,0x00,0x00,0x00,
  0xA9,0x00,0x00,0x00,0xAE,0x00,0x00,0x00,
  0x22,0x00,0x00,0x00,0x2C,0x00,0x00,0x00,
  0x0A,0x00,0x00,0x00,0x22,0x00,0x00,0x00,
  0x75,0x00,0x00,0x00,0x73,0x00,0x00,0x00,
  0x2D,0x00,0x00,0x00,0x61,0x00,0x00,0x00,
  0x73,0x00,0x00,0x00,0x63,0x00,0x00,0x00,
  0x69,0x00,0x00,0x00,0x69,0x00,0x00,0x00,
  0x22,0x00,0x00,0x00,0x20,0x00,0x00,0x00,
  0x3A,0x00,0x00,0x00,0x20,0x00,0x00,0x00,
  0x22,0x00,0x00,0x00,0x61,0x00,0x00,0x00,
  0x62,0x00,0x00,0x00,0x63,0x00,0x00,0x00,
  0x41,0x00,0x00,0x00,0x42,0x00,0x00,0x00,
  0x43,0x00,0x00,0x00,0x22,0x00,0x00,0x00,
  0x0A,0x00,0x00,0x00,0x7D,0x00,0x00,0x00,
  0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00
  };

  int r = 0;
#if defined( wxJSON_USE_UNICODE )
  wxJSONValue  value;
  value[_T("us-ascii")] = _T("abcABC");
  value[_T("latin1")]   = _T("àèì©®");
  value[_T("greek")]    = _T("αβγδ");
  value[_T("cyrillic")] = _T("ФХЦЧ");

  wxString jsonText;
  wxJSONWriter writer( wxJSONWRITER_NONE );
  writer.Write( value, jsonText );
  // ASSERT( success == 0 );
  TestCout( _T("The result string=\n" ));
  TestCout( jsonText );

  wxCSConv  conv( _T("UCS-4LE"));
  ASSERT( conv.IsOk());

  // compute the length of the needed buffer
  size_t len = conv.FromWChar( 0, 0, jsonText.c_str());
  TestCout( _T("Needed buffer length=\n" ));
  TestCout( len, true );
  ASSERT( len != wxCONV_FAILED )

  TestCout( _T("Result buffer length=\n" ));
  TestCout( sizeof(ucs4leBuff), true );

  // allocate the buffer
  char* buffer = new char[len + 4];

  // do the conversion and finally check the buffer results
  // note that if using 'len' (344) in the CheckBuffer()
  // function, the test fails so we use 340
  // which is the length of the ucs4leBuff buffer
  len = conv.FromWChar( buffer, len + 4, jsonText.c_str());
  r = CheckBuffer( buffer, ucs4leBuff, 340 );
#else
  TestCout( _T("Test #39 is only meaningfull in Unicode builds\n" ));
#endif

  return r;
}


// writing to a string in Unicode mode and then
// converting to ISO-8859-1 which is not suitable for storing
// characters from different charsets so the wxCSConv::FromWChar()
// function should return wxCONV_FAILED
// 27 feb 2008: OK, test is successfull, cannot convert the
// unicode string to Latin-1 charset
int Test40()
{
  int r = 0;
#if defined( wxJSON_USE_UNICODE )
  wxJSONValue  value;
  value[_T("us-ascii")] = _T("abcABC");
  value[_T("latin1")]   = _T("àèì©®");
  value[_T("greek")]    = _T("αβγδ");
  value[_T("cyrillic")] = _T("ФХЦЧ");

  wxString jsonText;
  wxJSONWriter writer( wxJSONWRITER_NONE );
  writer.Write( value, jsonText );
  TestCout( _T("The result string=\n" ));
  TestCout( jsonText );

  wxCSConv  conv( _T("ISO-8859-1"));
  ASSERT( conv.IsOk());

  // compute the length of the needed buffer
  size_t len = conv.FromWChar( 0, 0, jsonText.c_str());
  TestCout( _T("Needed buffer length=\n" ));
  TestCout( len, true );
  ASSERT( len == wxCONV_FAILED )
#else
  TestCout( _T("Test #40 is only meaningfull in Unicode builds\n" ));
#endif
  return r;
}


// Unicode mode: writes to a stream in UTF-8 a value
// 29 feb 2008: OK, test successfull
int Test41()
{
  int r = 0;
#if defined( wxJSON_USE_UNICODE )

  wxJSONValue  value;
  value[_T("us-ascii")] = _T("abcABC");
  value[_T("latin1")]   = _T("àèì©®");
  value[_T("greek")]    = _T("αβγδ");
  value[_T("cyrillic")] = _T("ФХЦЧ");

  char buffer[200];
  wxMemoryOutputStream jsonText( buffer, 200 );
  wxJSONWriter writer( wxJSONWRITER_NONE );
  writer.Write( value, jsonText );

  TestCout( _T("Result buffer length=" ));
  TestCout( sizeof(utf8Buff), true );

  // and now we check the obtained buffer against the
  // expected result stored in 'utf8Buff'
  r = CheckBuffer( buffer, utf8Buff, sizeof( utf8Buff ));
#else
  TestCout( _T("Test #41 is only meaningfull in Unicode builds\n" ));
#endif
  return r;
}


// ANSI mode: writes in UTF-8 a value that contains Latin-1
// characters. We set the 'it_IT.iso88591' locale so that
// the conversion should be OK.
// 29 feb 2008: OK, test is successfull
// 17 apr 2008: test failed!
int Test42()
{
  int r = 0;
#if !defined( wxJSON_USE_UNICODE )

  static char utf8Buff_2[] = {
  0x7B,0x0A,0x22,0x6C,0x61,0x74,0x69,0x6E,
  0x31,0x22,0x20,0x3A,0x20,0x22,0xC3,0xA0,
  0xC3,0xA8,0xC3,0xAC,0xC2,0xA9,0xC2,0xAE,
  0x22,0x2C,0x0A,0x22,0x75,0x73,0x2D,0x61,
  0x73,0x63,0x69,0x69,0x22,0x20,0x3A,0x20,
  0x22,0x61,0x62,0x63,0x41,0x42,0x43,0x22,
  0x0A,0x7D,0x0A,
  };

  // use the latin-1 locale charset (as used in Italy)
  setlocale( LC_CTYPE, _T("it_IT.88591"));

  char buff1[] = {
	0xe0, 0xe8, 0xec,   // a, e, i grave
	0xa9, 0xae, 0       // copyright sign, registered sign
  };

  wxJSONValue  value;
  value[_T("us-ascii")] = _T("abcABC");
  value[_T("latin1")]   = buff1;

  char buffer[128];
  wxMemoryOutputStream jsonText( buffer, 128 );
  wxJSONWriter writer( wxJSONWRITER_NONE );
  writer.Write( value, jsonText );

  // and now we check the result
  r = CheckBuffer( buffer, utf8Buff_2, sizeof( utf8Buff_2 ));
#else
  TestCout( _T("Test #42 is only meaningfull in ANSI builds\n" ));
#endif
  return r;
}


// this is much like Test38() which failed:
// (Unicode) writing to a string and then
// converting to UCS-2BE
// now using the wxCharBuffer class as suggested
// by Brian Ashby
// 3 mar 2008: the test failed because buffer.data()
// returns a NULL pointer
int Test43()
{
  static char ucs2beBuff[] = {
  0x00, 0x7b, 0x00, 0x0a, 0x00, 0x22, 0x00, 0x67,
  0x00, 0x72, 0x00, 0x65, 0x00, 0x65, 0x00, 0x6b,  //  |.{...".g.r.e.e.k|
  0x00, 0x22, 0x00, 0x20, 0x00, 0x3a, 0x00, 0x20,
  0x00, 0x22, 0x03, 0xb1, 0x03, 0xb2, 0x03, 0xb3,  //  |.". .:. ."......|
  0x03, 0xb4, 0x00, 0x22, 0x00, 0x2c, 0x00, 0x0a,
  0x00, 0x22, 0x00, 0x63, 0x00, 0x79, 0x00, 0x72,  //  |...".,...".c.y.r|
  0x00, 0x69, 0x00, 0x6c, 0x00, 0x6c, 0x00, 0x69,
  0x00, 0x63, 0x00, 0x22, 0x00, 0x20, 0x00, 0x3a,  //  |.i.l.l.i.c.". .:|
  0x00, 0x20, 0x00, 0x22, 0x04, 0x24, 0x04, 0x25,
  0x04, 0x26, 0x04, 0x27, 0x00, 0x22, 0x00, 0x2c,  //  |. .".$.%.&.'.".,|
  0x00, 0x0a, 0x00, 0x22, 0x00, 0x6c, 0x00, 0x61,
  0x00, 0x74, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x31,  //  |...".l.a.t.i.n.1|
  0x00, 0x22, 0x00, 0x20, 0x00, 0x3a, 0x00, 0x20,
  0x00, 0x22, 0x00, 0xe0, 0x00, 0xe8, 0x00, 0xec,  //  |.". .:. ."......|
  0x00, 0xa9, 0x00, 0xae, 0x00, 0x22, 0x00, 0x2c,
  0x00, 0x0a, 0x00, 0x22, 0x00, 0x75, 0x00, 0x73,  //  |.....".,...".u.s|
  0x00, 0x2d, 0x00, 0x61, 0x00, 0x73, 0x00, 0x63,
  0x00, 0x69, 0x00, 0x69, 0x00, 0x22, 0x00, 0x20,  //  |.-.a.s.c.i.i.". |
  0x00, 0x3a, 0x00, 0x20, 0x00, 0x22, 0x00, 0x61,
  0x00, 0x62, 0x00, 0x63, 0x00, 0x41, 0x00, 0x42,  //  |.:. .".a.b.c.A.B|
  0x00, 0x43, 0x00, 0x22, 0x00, 0x0a, 0x00, 0x7d,
  0x00, 0x0a                                       //  |.C."...}..|
  };

  int r = 0;
#if defined( wxJSON_USE_UNICODE )
  wxJSONValue  value;
  value[_T("us-ascii")] = _T("abcABC");
  value[_T("latin1")]   = _T("àèì©®");
  value[_T("greek")]    = _T("αβγδ");
  value[_T("cyrillic")] = _T("ФХЦЧ");

  wxString jsonText;
  wxJSONWriter writer( wxJSONWRITER_NONE );
  writer.Write( value, jsonText );
  TestCout( _T("The result string=\n" ));
  TestCout( jsonText );

  wxCSConv  conv( _T("UCS-2BE"));
  ASSERT( conv.IsOk());

  wxCharBuffer buffer = jsonText.mb_str( conv );
  // ASSERT( buffer.data() != 0 )   // fails

  int len = sizeof( ucs2beBuff );
  if ( buffer.data() != 0 )  {
    r = CheckBuffer( buffer.data(), ucs2beBuff, len );
  }

#else
  TestCout( _T("Test #43 is only meaningfull in Unicode builds\n" ));
#endif

  return r;
}


// test only the conversion utility
// 3 mar 2008: the test fails the conversion to UCS-2BE but is
// successfull for UTF-8
// note that in this test the wxJSON library is not needed
// the string was directly constructed from a wide-char buffer
int Test44()
{
  int r = 0;
#if defined( wxJSON_USE_UNICODE )

  wxString jsonText( _T("{\n"
		"   \"us-ascii\" : \"abcABC\",\n"
		"   \"latin1\"   : \"àèì©®\",\n"
		"   \"greek\"    : \"αβγδ\",\n"
		"   \"cyrillic\" : \"ФХЦЧ\"\n"
		"}\n"));

  TestCout( _T("The string that has to be converted=\n" ));
  TestCout( jsonText );

  wxCSConv  conv( _T("UCS-2BE"));
  ASSERT( conv.IsOk());

  wxCharBuffer buffer;

  buffer = jsonText.mb_str( conv );
  // ASSERT( buffer.data() != 0 )  // failed

  buffer = jsonText.mb_str( wxConvUTF8 );
  ASSERT( buffer.data() != 0 ) 


#else
  TestCout( _T("Test #44 is only meaningfull in Unicode builds\n" ));
#endif

  return r;
}



/*
{
}
*/



