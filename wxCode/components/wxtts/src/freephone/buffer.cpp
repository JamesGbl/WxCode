/////////////////////////////////////////////////////////////////////////////
// Name:        buffer.cpp
// Purpose:     Buffer operations
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: buffer.cpp,v 1.1.1.1 2003-12-28 21:08:19 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#include "t2s.h"

char *tsFreePhoneImplementation::buffer_text(BUFFER *buffer)
{
  return buffer->text;
}

void tsFreePhoneImplementation::buffer_init(BUFFER *buffer)
{
  buffer->size = 100;
  buffer->fill = 0;
  buffer->text = (char *)malloc(buffer->size + 1);
  buffer->text[0] = 0;
}

void tsFreePhoneImplementation::buffer_require(BUFFER *buffer, int space)
{
  if (space <= buffer->size) return;
  space = ((space*3)/2) + 100;
  buffer->text = (char *)realloc(buffer->text, space+1);
  buffer->size = space;
  buffer->text[space] = 0;
}

void tsFreePhoneImplementation::buffer_add_char(BUFFER *buffer, int c)
{
  int fill = buffer->fill;
  buffer_require(buffer, fill + 1);
  buffer->text[fill] = c;
  buffer->text[fill+1] = 0;
  buffer->fill = fill+1;
}

void tsFreePhoneImplementation::buffer_add_str(BUFFER *buffer, char *str)
{
  int len = strlen(str);
  int fill = buffer->fill;
  buffer_require(buffer, fill+len);
  strcpy(buffer->text + fill, str);
  buffer->fill = fill + len;
}

char tsFreePhoneImplementation::buffer_last_char(BUFFER *buffer)
{
  if (buffer->fill==0) return 0;
  return buffer->text[buffer->fill-1];
}

void tsFreePhoneImplementation::buffer_clear(BUFFER *buffer)
{
  buffer->fill = 0;
  buffer->text[0] = 0;
}

void tsFreePhoneImplementation::buffer_free(BUFFER *buffer)
{
  free(buffer->text);
}


