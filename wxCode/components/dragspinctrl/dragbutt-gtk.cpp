/////////////////////////////////////////////////////////////////////////////
// Name:        dragbutt-gtk.cpp
// Purpose:     Implementation of wxDragButton for wxGTK
// Author:      Aj Lavin.
// Created:     29.07.04
// RCS-ID:      $Id: dragbutt-gtk.cpp,v 1.1 2006-05-28 12:56:12 victor_gil Exp $
// Copyright:   (c) Aj Lavin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#if defined( __WXGTK__)

#include "dragbutt.h"
// Include the custom cursor.
#include "dragcur.xpm"
#include "dragmask.xpm"
#define DRAGCUR dragcur_xpm
#define DRAGMASK dragmask_xpm
#include <wx/dcscreen.h>
#include <wx/mstream.h>
#include <wx/image.h>
#include <wx/txtstrm.h>
#include <wx/tokenzr.h>


IMPLEMENT_DYNAMIC_CLASS( wxDragButton, wxDragButtonBase)


namespace
{
  wxImage LoadImageFromXpmData( char** xpm);
}


wxDragButton::wxDragButton()
{
}


wxDragButton::wxDragButton( wxWindow *parent,
			    wxWindowID id,
			    const wxPoint& pos,
			    const wxSize& size,
			    long style,
			    const wxString& name)
  : wxDragButtonBase( parent, id, pos, size, style, name)
{
    
}


void
wxDragButton::HideCursor()
{
  static wxCursor blankCursor;

  if ( ! blankCursor.Ok())
    {
      const char cursorData[] = { 0,0,0,0, 0,0,0,0 };
      blankCursor = wxCursor( cursorData, 
			      8, 8, 0, 0, 
			      cursorData, 
			      wxBLACK, 
			      wxBLACK);
    }

  SetCursor( blankCursor);
}


void
wxDragButton::DoDrawFrozenCursor( wxDC& dc)
{
  static wxBitmap dragCursorBitmap( GetDragCursorImage());

  dc.DrawBitmap( dragCursorBitmap, m_downClick.x, m_downClick.y, true);

  wxPoint screenClick = ClientToScreen( m_downClick);

  wxScreenDC screenDc;
  wxRect rect( screenClick, wxSize( dragCursorBitmap.GetWidth(),
				    dragCursorBitmap.GetHeight()));

  screenDc.StartDrawingOnTop( & rect);
  screenDc.DrawBitmap( dragCursorBitmap, screenClick.x, screenClick.y, true);
  screenDc.EndDrawingOnTop();
}


wxImage&
wxDragButton::GetDragCursorImage() const
{
  static wxImage image;

  if ( ! image.Ok())
    {
      // *** Ideally only load xpm handler, and only if it is not
      // already loaded.
      wxInitAllImageHandlers();

      image = LoadImageFromXpmData( DRAGCUR);

      wxImage mask = LoadImageFromXpmData( DRAGMASK);
      image.SetMaskFromImage( mask, 255, 255, 255);
    }

  return image;
}


namespace
{
  bool GetXpmNumRows( char** xpm, long* pNHeaderRow, long* pNImageRows);

  wxImage LoadImageFromXpmData( char** xpm)
  {
    wxMemoryOutputStream out;

    wxTextOutputStream text( out);

    text.WriteString( _T("/* XPM */\n"));
    text.WriteString( _T("static char * random_xpm[] = {\n"));

    long nImageRows;
    long nHeaderRows;
    if ( ! GetXpmNumRows( xpm, & nHeaderRows, & nImageRows))
      {
	return wxImage();
      }

    size_t nRows = nImageRows + nHeaderRows;

    for( size_t r = 0; r < nRows; ++ r)
      {
	wxString line = wxString::Format( _T("\"%s\""), xpm[ r]);
	if ( r < nRows - 1)
	  {
	    line += wxString( _T( ",\n"));
	  }
	else
	  {
	    line += wxString( _T( "};\n"));
	  }
	
	text.WriteString( line);
      }

    unsigned char* buf = new unsigned char[ 4096];
    size_t n = out.CopyTo( buf, 4096);

    wxMemoryInputStream in( buf, n);

    wxImage image;

    image.LoadFile( in, wxBITMAP_TYPE_XPM);

    delete[] buf;

    return image;
  }


  bool GetXpmNumRows( char** xpm, long* pNHeaderRow, long* pNImageRows)
  {
    wxStringTokenizer tkz( xpm[ 0]);

    if ( ! tkz.HasMoreTokens())
      {
	return false;
      }

    tkz.GetNextToken();

    if ( ! tkz.HasMoreTokens())
      {
	return false;
      }

    tkz.GetNextToken().ToLong( pNImageRows);

    if ( ! tkz.HasMoreTokens())
      {
	return false;
      }

    tkz.GetNextToken().ToLong( pNHeaderRow);
    * pNHeaderRow += 1;

    return true;
  }
}


#endif // defined( __WXGTK__)
