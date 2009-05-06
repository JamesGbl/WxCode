/***************************************************************
 * Name:      aboutDialog.cpp
 * Purpose:   Implements class for the About dialog
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "aboutDialog.h"
#include "resource/about.xpm"

BEGIN_EVENT_TABLE(aboutDialog, wxDialog)
    EVT_PAINT(aboutDialog::OnPaint)
    EVT_ERASE_BACKGROUND(aboutDialog::OnEraseBackground)
    EVT_LEFT_DOWN(aboutDialog::OnClick)
END_EVENT_TABLE()

aboutDialog::aboutDialog(wxWindow *parent) : wxDialog(parent, wxID_ANY, wxT("About wxDemoViewer"), wxDefaultPosition, wxSize(250, 150), wxSTAY_ON_TOP | wxBORDER_NONE)
{
    ;
}

aboutDialog::~aboutDialog()
{
    ;
}

void aboutDialog::OnClick(wxMouseEvent &event)
{
    EndModal(0);
}

void aboutDialog::OnEraseBackground(wxEraseEvent &event)
{
    ;
}

void aboutDialog::OnPaint(wxPaintEvent &event)
{
    wxPaintDC dc(this);
    wxRect rect = GetClientRect();
    wxBrush brush(wxColour(212, 225, 241));
    wxPen pen(wxColour(153, 187, 232));
    wxFont font(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    wxBitmap img(aboutLogo);

    dc.SetBrush(brush);
    dc.SetPen(pen);
    dc.SetFont(font);

    dc.DrawRectangle(rect);

    dc.DrawBitmap(img, 15, 10, true);


    rect.Deflate(15, 80);

    dc.DrawLabel(wxT("wxDemoViewer 1.0\n\nJan Chudy\n15. 03. 2009"), rect, wxALIGN_CENTER_HORIZONTAL);

    dc.SetBrush(wxNullBrush);
    dc.SetPen(wxNullPen);
}
