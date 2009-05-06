/***************************************************************
 * Name:      ownerDrawnStatic.cpp
 * Purpose:   Implements class for static text control with custom background and text colors
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "ownerDrawnStatic.h"


ownerDrawnStatic::ownerDrawnStatic(wxWindow *parent, styleStatic style) : ownerDrawnWindow(parent, wxID_ANY, style.bgStyle, wxSize(-1, style.height))
{
    int b;
    m_style = style;

    if(style.bold) b = wxFONTWEIGHT_BOLD;
    else b = wxFONTWEIGHT_NORMAL;

    m_font = wxFont(style.fontSize, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, b);
}

ownerDrawnStatic::~ownerDrawnStatic()
{
    ;
}

void ownerDrawnStatic::drawCustom(wxPaintDC *dc)
{
    wxRect rect = GetClientRect();

    dc->SetFont(m_font);
    dc->SetTextForeground(m_style.textColor);
    rect.Deflate(3);
    dc->DrawLabel(m_style.title, rect, m_style.textAlign | wxALIGN_CENTER_VERTICAL);
}
