/***************************************************************
 * Name:      ownerDrawnSplitter.cpp
 * Purpose:   Implements class for wxSplitterWindow with custom background color
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "ownerDrawnSplitter.h"

BEGIN_EVENT_TABLE(ownerDrawnSplitter, wxSplitterWindow)
    EVT_SIZE(ownerDrawnSplitter::onResize)
    EVT_PAINT(ownerDrawnSplitter::onPaint)
    EVT_ERASE_BACKGROUND(ownerDrawnSplitter::onEraseBackground)
END_EVENT_TABLE()

ownerDrawnSplitter::ownerDrawnSplitter(wxWindow *parent, wxWindowID id, wxColour backColor) : wxSplitterWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER | wxSP_NO_XP_THEME | wxSP_LIVE_UPDATE)
{
    m_backColor = backColor;
}

ownerDrawnSplitter::~ownerDrawnSplitter()
{
    ;
}

void ownerDrawnSplitter::onResize(wxSizeEvent &event)
{
    Refresh();
    event.Skip();
}

void ownerDrawnSplitter::onEraseBackground(wxEraseEvent &event)
{
    ;
}

void ownerDrawnSplitter::onPaint(wxPaintEvent &event)
{
    wxPaintDC dc(this);
    wxBrush brush(m_backColor);

    dc.SetBackground(brush);
    dc.Clear();
    dc.SetBackground(wxNullBrush);
}
