/***************************************************************
 * Name:      styleBackground.cpp
 * Purpose:   Implements class encapsulating style for a ownerdrawn background
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "styleBackground.h"

styleBackground::styleBackground()
{
    colors[CQ_COLOR_A] = wxColour(208, 208, 208);
    colorCount = 1;

    borderColor = wxColour(0, 0, 0);
    borders = 0;
}

styleBackground::styleBackground(wxXmlNode *node)
{
    wxString value;

    colors[CQ_COLOR_A] = wxColour(208, 208, 208);
    colorCount = 1;

    borderColor = wxColour(0, 0, 0);
    borders = 0;

    if(node->GetPropVal(wxT("borderColor"), &value)) borderColor = wxColour(value);

    if(node->GetPropVal(wxT("borders"), &value))
    {
        value = value.MakeUpper();
        if(value.Find(wxT('T'))!=wxNOT_FOUND) borders |= wxTOP;
        if(value.Find(wxT('R'))!=wxNOT_FOUND) borders |= wxRIGHT;
        if(value.Find(wxT('B'))!=wxNOT_FOUND) borders |= wxBOTTOM;
        if(value.Find(wxT('L'))!=wxNOT_FOUND) borders |= wxLEFT;
    }

    if(node->GetPropVal(wxT("colorA"), &value)) colors[CQ_COLOR_A] = wxColour(value);
    else return;

    if(node->GetPropVal(wxT("colorB"), &value)) { colors[CQ_COLOR_B] = wxColour(value); colorCount = 2; }
    else return;

    if(node->GetPropVal(wxT("colorC"), &value)) { colors[CQ_COLOR_C] = wxColour(value); colorCount = 3; }
    else return;

    if(node->GetPropVal(wxT("colorD"), &value)) { colors[CQ_COLOR_D] = wxColour(value); colorCount = 4; }
    else return;
}

styleBackground::~styleBackground()
{
    ;
}
