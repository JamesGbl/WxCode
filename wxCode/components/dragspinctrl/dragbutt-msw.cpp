/////////////////////////////////////////////////////////////////////////////
// Name:        dragbutt-msw.cpp
// Purpose:     Implementation of wxDragButton for wxMSW
// Author:      Aj Lavin.
// Created:     29.07.04
// RCS-ID:      $Id: dragbutt-msw.cpp,v 1.1 2006-05-28 12:56:12 victor_gil Exp $
// Copyright:   (c) Aj Lavin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __WXMSW__

#include "dragbutt.h"
#include <windows.h>
#include <wx/msw/winundef.h>
#include <wx/dcscreen.h>


#define DEBUG_DRAGBUTT 0

namespace
{
  WXHCURSOR GetCurrentCursor();
  wxPoint GetHotSpot( WXHCURSOR cursor);
}


IMPLEMENT_DYNAMIC_CLASS( wxDragButton, wxDragButtonBase)


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
                         
  m_cursor = GetCurrentCursor();

  m_hotSpot = GetHotSpot( m_cursor);

  ::SetCursor(NULL);
}


void
wxDragButton::DoDrawFrozenCursor( wxDC& dc)
{
  wxPoint p = ClientToScreen( m_downClick);

  wxScreenDC screenDc;

  if ( ! ::DrawIconEx( reinterpret_cast< HDC>( screenDc.GetHDC()),
		       p.x - m_hotSpot.x,
		       p.y - m_hotSpot.y,
		       reinterpret_cast< HCURSOR>( m_cursor),
		       0, // Choose default width, height.
		       0,
		       0, // Animation step.
		       0, // Handle for brush for flicker free drawing
		       DI_NORMAL | DI_DEFAULTSIZE))
    {
#if DEBUG_DRAGBUTT
	fprintf( stderr, 
		 "DrawIconEx failed with code: '%d\n",
		 GetLastError() );		 
#endif
	return;
    }
}


namespace
{
  WXHCURSOR GetCurrentCursor()
  {
    CURSORINFO info;
    info.cbSize = sizeof( CURSORINFO);

    if ( ! ::GetCursorInfo( & info))
      {
#if DEBUG_DRAGBUTT
	fprintf( stderr, 
		 "GetCursorInfo failed with code: '%d\n",
		 GetLastError() );		 
#endif
	return 0;
      }

    return reinterpret_cast< WXHCURSOR>( info.hCursor);
  }

  wxPoint GetHotSpot( WXHCURSOR cursor)
  {
    // Get the cursor's hostpot.
    ICONINFO info;
    memset( & info, 0, sizeof( ICONINFO));
    if ( ! GetIconInfo( reinterpret_cast< HCURSOR>( cursor), & info))
      {
	return wxPoint( 0, 0);
      }
    else
      {
	return wxPoint( info.xHotspot, info.yHotspot);
      }
  }
}

#endif
