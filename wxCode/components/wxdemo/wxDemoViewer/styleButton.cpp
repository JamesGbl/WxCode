/***************************************************************
 * Name:      styleButton.cpp
 * Purpose:   Implements class encapsulating style for a ownerdrawn button
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "styleButton.h"

styleButton::styleButton()
{
    textColor[0] = textColor[1] = textColor[2] = *wxBLACK;
    fontSize = 9;
    textAlign = wxALIGN_RIGHT;
    bold[0] = bold[1] =  false;
    bold[2] = true;
    imgPosition = wxLEFT;
}

styleButton::styleButton(wxXmlNode *node)
{
    wxString value;
    long i;
    wxXmlNode *subNode;

    textColor[0] = textColor[1] = textColor[2] = *wxBLACK;
    fontSize = 9;
    textAlign = wxALIGN_LEFT;
    bold[0] = bold[1] =  false;
    bold[2] = true;
    imgPosition = wxLEFT;

    if(node->GetPropVal(wxT("title"), &value)) title = value;
    if(node->GetPropVal(wxT("icon"), &value)) imgFile = value;

    if(node->GetPropVal(wxT("iconPosition"), &value))
    {
        value = value.MakeLower();
        if(value==wxT("top")) imgPosition = wxTOP;
        else if(value==wxT("right")) imgPosition = wxRIGHT;
        else if(value==wxT("bottom")) imgPosition = wxBOTTOM;
        else if(value==wxT("left")) imgPosition = wxLEFT;
    }

    if(node->GetPropVal(wxT("fontSize"), &value)) if(value.ToLong(&i)) fontSize = i;

    if(node->GetPropVal(wxT("textAlign"), &value))
    {
        if(value==wxT("center")) textAlign = wxALIGN_CENTER;
        else if(value==wxT("right")) textAlign = wxALIGN_RIGHT;
    }

    subNode = node->GetChildren();
    while(subNode)
    {
       if(subNode->GetName()==wxT("stateNormal")) i = 0;
       else if(subNode->GetName()==wxT("stateHover")) i = 1;
       else if(subNode->GetName()==wxT("stateSelected")) i = 2;
       else { subNode = subNode->GetNext(); continue; }

       bgStyle[i] = styleBackground(subNode);
       if(subNode->GetPropVal(wxT("textColor"), &value)) textColor[i] = wxColour(value);
       if(subNode->GetPropVal(wxT("bold"), &value)) bold[i] = value==wxT("true");

       subNode = subNode->GetNext();
    }
}

styleButton::~styleButton()
{
    ;
}
