/******************************************************************************

   test12.cpp

    test for bug fixing in version 0.4.1 (reader and writer)
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

#include <locale.h>

#include <wx/app.h>
#include <wx/string.h>
#include <wx/debug.h>
#include <wx/mstream.h>
#include <wx/file.h>
#include <wx/wfstream.h>


#include <wx/jsonval.h>
#include <wx/jsonreader.h>
#include <wx/jsonwriter.h>
#include "test.h"

static void PrintStreamResult( wxOutputStream& os )
{
  wxFileOffset len = os.GetLength();
  TestCout( _T("wxStreamBase::GetLength() = "));
  TestCout( (int) len, true );

  wxStreamError err = os.GetLastError();
  TestCout( _T("wxStreamBase::GetLastError() = "));
  TestCout( err, true );
  switch ( err )  {
    case wxSTREAM_NO_ERROR :
      TestCout( _T("wxStreamBase::GetLastError(): wxSTREAM_NO_ERROR\n"));
      break; 
    case wxSTREAM_EOF :
      TestCout( _T("wxStreamBase::GetLastError(): wxSTREAM_EOF\n"));
      break; 
    case wxSTREAM_WRITE_ERROR :
      TestCout( _T("wxStreamBase::GetLastError(): wxSTREAM_WRITE_ERROR\n"));
      break; 
    case wxSTREAM_READ_ERROR :
      TestCout( _T("wxStreamBase::GetLastError(): wxSTREAM_READ_ERROR\n"));
      break;
    default :
      TestCout( _T("wxStreamBase::GetLastError(): undefined error\n"));
      break; 
  }
}


// prints the errors and warnings array
static void PrintErrors( wxJSONReader& reader )
{
  wxString s;
  int numErrors = reader.GetErrorCount();
  s.Printf( _T( "\nERRORS: count=%d\n"), numErrors );
  TestCout( s );
  const wxArrayString& errors = reader.GetErrors();
  for ( int i = 0; i < errors.size(); i++ )  {
    TestCout( errors[i] );
    TestCout( _T( "\n" ));
  }
  int numWarn   = reader.GetWarningCount();
  const wxArrayString& warnings = reader.GetWarnings();
  s.Printf( _T("WARNINGS: count=%d\n"), numWarn );
  TestCout( s );
  for ( int i = 0; i < warnings.size(); i++ )  {
    TestCout( warnings[i] );
    TestCout( _T( "\n" ));
  }
}


static void PrintValue( wxJSONValue& val )
{
  wxJSONWriter writer;
  wxString s;
  writer.Write( val, s );
  TestCout( s );
}

//
// testing the wxJSONReader::AddWarning() function which should
// report errors when the individual wxJSON extension is turned off
// 12 apr 2008: OK, test is successfull
int Test52()
{
  static const wxChar* text = 
   _T("{\n"
	"   // comment line for \'key-1\'\n"        // (2) comment line
	"   \"key-1\" : \"value-1\",\n"             // (3)

	"   \"key-2\" : NuLL,\n"                    // (4) mixed case literal

	"   \"key-3\" : \"multi-line string\n\"\n"  // (5) multiline string
	"               \"line-2\n\"\n"             // these count as 2 warnings
	"               \"line-3\n\"\n"

	//  "}\n"                                   // (8) final '}' is missing
      "\n");
 
  wxString jsonText( text );
  int numErrors, numWarns;
  wxJSONValue root;

  // all wxJSON extensions are ON
  TestCout( _T("\nParser flag is wxJSONREADER_TOLERANT\n"));
  wxJSONReader reader1( wxJSONREADER_TOLERANT );
  numErrors = reader1.Parse( jsonText, &root );
  PrintErrors( reader1 );
  PrintValue( root );
  ASSERT( numErrors == 0 )
  numWarns  = reader1.GetWarningCount();
  ASSERT( numWarns == 5 )

  // only comments are allowed
  TestCout( _T("\nParser flag is wxJSONREADER_ALLOW_COMMENTS\n"));
  wxJSONReader reader2( wxJSONREADER_ALLOW_COMMENTS );
  numErrors = reader2.Parse( jsonText, &root );
  PrintErrors( reader2 );
  PrintValue( root );
  ASSERT( numErrors == 4 )
  numWarns  = reader2.GetWarningCount();
  ASSERT( numWarns == 1 )

  // only mixed case literals are allowed
  TestCout( _T("\nParser flag is wxJSONREADER_CASE\n"));
  wxJSONReader reader3( wxJSONREADER_CASE );
  numErrors = reader3.Parse( jsonText, &root );
  PrintErrors( reader3 );
  PrintValue( root );
  ASSERT( numErrors == 4 )
  numWarns  = reader3.GetWarningCount();
  ASSERT( numWarns == 1 )

  // only multiline strings areallowed
  TestCout( _T("\nParser flag is wxJSONREADER_MULTISTRING\n"));
  wxJSONReader reader4( wxJSONREADER_MULTISTRING );
  numErrors = reader4.Parse( jsonText, &root );
  PrintErrors( reader4 );
  PrintValue( root );
  ASSERT( numErrors == 3 )
  numWarns  = reader4.GetWarningCount();
  ASSERT( numWarns == 2 )

  // only wrong/missing close array/object char is allowed
  TestCout( _T("\nParser flag is wxJSONREADER_MISSING\n"));
  wxJSONReader reader5( wxJSONREADER_MISSING );
  numErrors = reader5.Parse( jsonText, &root );
  PrintErrors( reader5 );
  PrintValue( root );
  ASSERT( numErrors == 4 )
  numWarns  = reader5.GetWarningCount();
  ASSERT( numWarns == 1 )

  return 0;
}

//
// testing errors in the output stream
// 12 apr 2008: OK, the test is successfull
int Test53()
{
  // the output is 45 bytes long
  // {
  // "key-1" : "value-1",
  // "key-2" : "value-2"
  // }

  wxJSONValue  value;
  value[_T("key-1")] = _T("value-1");
  value[_T("key-2")] = _T("value-2");

  char buffer[128];
  wxMemoryOutputStream jsonText( buffer, 128 );
  wxJSONWriter writer( wxJSONWRITER_NONE );
  writer.Write( value, jsonText );

  // get the stream result
  TestCout( _T("\nWriting to a stream of 128 bytes\n"));
  PrintStreamResult( jsonText );

  TestCout( _T("\nWriting to a stream with no predefined length\n"));
  wxMemoryOutputStream jsonText2;
  writer.Write( value, jsonText2 );
  PrintStreamResult( jsonText2 );

  TestCout( _T("\nWriting to a stream of not enough bytes allocated on the stack\n"));
  char buffer3[20];
  wxMemoryOutputStream jsonText3( buffer3, 20 );
  // writer.Write( value, jsonText3 );    // program aborted
  TestCout( _T("The program aborts in the wxWidgets library\n"));
  PrintStreamResult( jsonText3 );

  TestCout( _T("\nWriting to a stream of not enough bytes dynamically allocated\n"));
  char* buffer4 = (char*) malloc( 20 );
  wxMemoryOutputStream jsonText4( buffer4, 20 );
  writer.Write( value, jsonText4 );
  PrintStreamResult( jsonText4 );
  // free( buffer4 );          // do not free the allocated buffer

  // use a buffer allocated on the stack but calls the wxStreamBuffer::Fixed(true)
  // so that the buffer cannt grow
  TestCout( _T("\nWriting to a stream of not enough bytes allocated on the stack (2)\n"));
  char buffer5[20];
  wxMemoryOutputStream jsonText5( buffer5, 20 );
  wxStreamBuffer* sBuffer = jsonText5.GetOutputStreamBuffer();
  ASSERT( sBuffer );
  sBuffer->Fixed( true );
  // writer.Write( value, jsonText5 );    // program aborted (realloc error)
  TestCout( _T("The program aborts ASSERTION failure (m_flushable != TRUE)\n"));
  // PrintStreamResult( jsonText5 );

  // use a buffer allocated dynamically but calls the wxStreamBuffer::Fixed(true)
  // so that the buffer cannt grow
  TestCout( _T("\nWriting to a stream of not enough bytes dinamically allocated (2)\n"));
  char* buffer6 = (char*) malloc( 20 );
  wxMemoryOutputStream jsonText6( buffer6, 20 );
  sBuffer = jsonText6.GetOutputStreamBuffer();
  ASSERT( sBuffer );
  sBuffer->Fixed( true );
  // writer.Write( value, jsonText6 );    // program aborted (realloc error)
   TestCout( _T("The program aborts ASSERTION failure (m_flushable != TRUE)\n"));
  // PrintStreamResult( jsonText6 );

  // write to a file that was opened in read-only mode
  // an error should occur - 12 apr 2008: OK, the result is wxSTREAM_WRITE_ERROR
  TestCout( _T("\nWriting to a file opened in read-only mode\n"));
  wxFile file( _T("test12.txt"), wxFile::read );
  wxFileOutputStream jsonText7( file );
  writer.Write( value, jsonText7 );
  PrintStreamResult( jsonText7 );
  return 0;
}



/*
{
}
*/





