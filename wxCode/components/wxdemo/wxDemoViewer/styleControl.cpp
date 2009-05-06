/***************************************************************
 * Name:      styleControl.cpp
 * Purpose:   Implements class encapsulating style for a control
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "styleControl.h"

styleControl::styleControl()
{
    backColor = *wxWHITE;
    borderColor = *wxBLACK;
    textColor = *wxBLACK;
    hasBorder = false;
    rounded = false;
}

styleControl::styleControl(wxXmlNode *node)
{
    wxString value;

    backColor = *wxWHITE;
    borderColor = *wxBLACK;
    textColor = *wxBLACK;
    hasBorder = false;
    rounded = false;

    if(node->GetPropVal(wxT("backColor"), &value)) backColor = wxColour(value);
    if(node->GetPropVal(wxT("borderColor"), &value)) borderColor = wxColour(value);
    if(node->GetPropVal(wxT("textColor"), &value)) textColor = wxColour(value);
    if(node->GetPropVal(wxT("hasBorder"), &value)) hasBorder = value==wxT("true");
    if(node->GetPropVal(wxT("rounded"), &value)) rounded = value==wxT("true");
}

styleControl::~styleControl()
{
    ;
}
