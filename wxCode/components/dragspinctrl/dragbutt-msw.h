/////////////////////////////////////////////////////////////////////////////
// Name:        dragbutt-msw.h
// Purpose:     Implementation of wxDragButton for wxMSW
// Author:      Aj Lavin.
// Created:     29.07.04
// RCS-ID:      $Id: dragbutt-msw.h,v 1.1 2006-05-28 12:56:12 victor_gil Exp $
// Copyright:   (c) Aj Lavin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef DRAGBUTT_MSW_H
#define DRAGBUTT_MSW_H

#ifndef __WXMSW__
#error
#endif


class WXDLLEXPORT wxDragButton : public wxDragButtonBase
{
public:

  wxDragButton();
  wxDragButton( wxWindow *parent,
		wxWindowID id = -1,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDB_DEFAULT_FLAGS,
		const wxString& name = wxDRAGBUTTON_NAME);

 protected:

  void HideCursor();
  void DoDrawFrozenCursor( wxDC& dc);

private:

  DECLARE_DYNAMIC_CLASS(wxDragButton)

  wxPoint m_hotSpot;

  WXHCURSOR m_cursor;
};


#endif
