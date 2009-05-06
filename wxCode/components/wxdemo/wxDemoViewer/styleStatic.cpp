/***************************************************************
 * Name:      styleStatic.cpp
 * Purpose:   Implements class encapsulating style for a static text control
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "styleStatic.h"

styleStatic::styleStatic()
{
    textColor = *wxBLACK;
    fontSize = 9;
    textAlign = wxALIGN_LEFT;
    height = 20;
    bold = true;
}

styleStatic::styleStatic(wxXmlNode *node)
{
    wxString value;
    long i;

    textColor = *wxBLACK;
    fontSize = 9;
    textAlign = wxALIGN_LEFT;
    height = 20;
    bold = true;

    bgStyle = styleBackground(node);

    if(node->GetPropVal(wxT("title"), &value)) title = value;
    if(node->GetPropVal(wxT("textColor"), &value)) textColor = wxColour(value);

    if(node->GetPropVal(wxT("fontSize"), &value)) if(value.ToLong(&i)) fontSize = i;

    if(node->GetPropVal(wxT("textAlign"), &value))
    {
        if(value==wxT("center")) textAlign = wxALIGN_CENTER;
        else if(value==wxT("right")) textAlign = wxALIGN_RIGHT;
    }

    if(node->GetPropVal(wxT("height"), &value)) if(value.ToLong(&i)) height = i;

    if(node->GetPropVal(wxT("bold"), &value)) bold = value==wxT("true");
}

styleStatic::~styleStatic()
{
    ;
}
