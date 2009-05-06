/***************************************************************
 * Name:      ownerDrawnStatic.h
 * Purpose:   Implements class for a window with custom background colors
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "ownerDrawnWindow.h"

BEGIN_EVENT_TABLE(ownerDrawnWindow, wxWindow)
    EVT_SIZE(ownerDrawnWindow::OnResize)
    EVT_PAINT(ownerDrawnWindow::OnPaint)
    EVT_ERASE_BACKGROUND(ownerDrawnWindow::OnEraseBackground)
END_EVENT_TABLE()

ownerDrawnWindow::ownerDrawnWindow(wxWindow *parent, wxWindowID id, styleBackground style, wxSize size) : wxWindow(parent, id, wxDefaultPosition, size, wxBORDER_NONE | wxTAB_TRAVERSAL | wxCLIP_CHILDREN)
{
    m_bgStyle = style;
}

ownerDrawnWindow::~ownerDrawnWindow()
{
    ;
}

void ownerDrawnWindow::OnResize(wxSizeEvent &event)
{
    Refresh();
    event.Skip();
}

void ownerDrawnWindow::OnEraseBackground(wxEraseEvent &event)
{
    ;
}

void ownerDrawnWindow::drawCustom(wxPaintDC *dc)
{
    ;
}

void ownerDrawnWindow::changeStyle(styleBackground bgStyle)
{
    m_bgStyle = bgStyle;
    Refresh();
}

void ownerDrawnWindow::OnPaint(wxPaintEvent &event)
{
    wxPaintDC dc(this);
    wxRect rect = GetClientRect(), halfRect = rect;
    wxSize size;
    wxBrush brush;

    if(m_bgStyle.borders!=0)
    {
        if(m_bgStyle.borders & wxTOP) rect.y = 1;
        if(m_bgStyle.borders & wxLEFT) rect.x = 1;

        if(m_bgStyle.borders & wxBOTTOM) rect.height -= 1;
        if(m_bgStyle.borders & wxRIGHT) rect.width -= 1;

        if((m_bgStyle.borders & wxTOP) && (m_bgStyle.borders & wxBOTTOM)) rect.height -= 1;
        if((m_bgStyle.borders & wxLEFT) && (m_bgStyle.borders & wxRIGHT)) rect.width -= 1;

        wxPen pen(m_bgStyle.borderColor, 1);
        dc.SetPen(pen);
        dc.DrawRectangle(0, 0, halfRect.width, halfRect.height);
        dc.SetPen(wxNullPen);
    }

    switch(m_bgStyle.colorCount)
    {
        case 1:
        default:
            dc.GradientFillLinear(rect, m_bgStyle.colors[CQ_COLOR_A], m_bgStyle.colors[CQ_COLOR_A], wxSOUTH);
            break;

        case 2:
            dc.GradientFillLinear(rect, m_bgStyle.colors[CQ_COLOR_A], m_bgStyle.colors[CQ_COLOR_B], wxSOUTH);
            break;

        case 3:
            halfRect = rect;
            halfRect.SetHeight(halfRect.height / 2);
            dc.GradientFillLinear(halfRect, m_bgStyle.colors[CQ_COLOR_A], m_bgStyle.colors[CQ_COLOR_B], wxSOUTH);

            halfRect.Offset(0, rect.height - halfRect.height);
            dc.GradientFillLinear(halfRect, m_bgStyle.colors[CQ_COLOR_B], m_bgStyle.colors[CQ_COLOR_C], wxSOUTH);
            break;

        case 4:
            halfRect = rect;
            halfRect.SetHeight(halfRect.height / 2);
            dc.GradientFillLinear(halfRect, m_bgStyle.colors[CQ_COLOR_A], m_bgStyle.colors[CQ_COLOR_B], wxSOUTH);

            halfRect.Offset(0, halfRect.height);
            halfRect.SetHeight(rect.height - halfRect.height);

            dc.GradientFillLinear(halfRect, m_bgStyle.colors[CQ_COLOR_C], m_bgStyle.colors[CQ_COLOR_D], wxSOUTH);
            break;
    }

    drawCustom(&dc);
}
