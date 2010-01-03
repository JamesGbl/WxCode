/******************************************************************************

   hexd.cpp

    a modified version of 'hexdump' which produces C-style output
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


 This small application reads a file and produces an output similar to
 'hexdump' but that can be copied-and-pasted into a C/C++ source file
 for creating a buffer of the same content of the file.
 For example, if the file contains:

	12345\n

 the application's output is:

	0x31,0x32,0x33,0x34,0x35,0x0A


 Syntax:

   hexd.bin FILENAME

 NOTE: this app is not built using wxWidgets so it would only run
 (or even compile) in a Unix-like system.


*****************************************************************************/

using namespace std;

#define BYTES_PER_ROW     8

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void PrintHexRow( int len, const char* buffer )
{
  printf( "  " );
  for ( int i = 0; i < len; i++ )  {
    unsigned char c = buffer[i];
    char hex[8];
    snprintf( hex, 8, "%02X", (int) c );
    printf( "0x%s,", hex );
  }
  printf( "\n" );
}

int main( int argc, char* argv[] ) 
{
  int opt;

  do {
    opt = getopt( argc, argv, "h" );
    switch ( opt )  {
      case '?' :
        return 1;
        break;
      case 'h' :
        printf( "Syntax:\n  hexd.bin FILENAME\n\n");
        return 0;
        break;
      default :
        break;
    }

  } while ( opt != -1 );

  // for every FILENAME argument prints the C-style hexdump
  for ( int i = optind; i < argc; i++ )  {
    const char* file = argv[i];
    printf( "Processing file: %s\n", file );

    int fd = open( file, O_RDONLY );
    if ( fd < 0 )  {
      perror( 0 );
      return 1;
    }

    while (1)  {
      // we read 8 bytes at a time
      char buffer[BYTES_PER_ROW + 1];
      size_t len = read( fd, buffer, BYTES_PER_ROW );
      if ( len == 0 ) {  // EOF
        break;
      }
      if ( len < 1 ) {  // errors?
        perror( 0 );
        return 1;
      }
      PrintHexRow( len, buffer );
    }
    close( fd );
  }  // continue for all files

  return 0;
}


/*
{
}
*/

