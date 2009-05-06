/***************************************************************
 * Name:      styleLexer.cpp
 * Purpose:   Implements class encapsulating a text highlighter - lexer for wxSTC control
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "styleLexer.h"
#include <wx/listimpl.cpp>

WX_DEFINE_LIST(styleLexerList);

styleLexer::styleLexer()
{
    clearAllStyles();
}

styleLexer::~styleLexer()
{
    ;
}

void styleLexer::clearAllStyles()
{
    lexerID = 1;
    styleCount = 32;

    for(int n=0; n<32; n++)
    {
        styles[n].backColor = wxColour(*wxWHITE);
        styles[n].textColor = wxColour(*wxBLACK);
        styles[n].bold = styles[n].underline = styles[n].italic = false;
        styles[n].keywords = wxEmptyString;
    }
}

bool styleLexer::loadLexterStyle(wxString filename)
{
    wxXmlDocument doc;
    unsigned int counter = 0;
    wxColour colorVal, defTextColor = *wxBLACK, defBackColor = *wxWHITE;
    long i;
    wxString value;
    wxXmlNode *topNode, *child;
    wxFileName fn(filename, wxPATH_UNIX);

    clearAllStyles();

    if(filename.IsEmpty()) return false;
    if(!fn.FileExists()) return false;
    if(!doc.Load(fn.GetFullPath())) return false;

    topNode = doc.GetRoot();

    if(topNode==NULL) return false;
    if(topNode->GetName()!=wxT("styleLexer")) return false;

    value = topNode->GetPropVal(wxT("lexerID"), wxT("1"));
    if(value.ToLong(&i)) lexerID = i;
    else lexerID = 1;

    if(topNode->GetPropVal(wxT("defaultTextColor"), &value)) defTextColor = wxColour(value);
    if(topNode->GetPropVal(wxT("defaultBackColor"), &value)) defBackColor = wxColour(value);

    child = topNode->GetChildren();
    while(child)
    {
        if(child->GetName()==wxT("style") && counter<32)
        {
            if(child->GetPropVal(wxT("backColor"), &value)) styles[counter].backColor = wxColour(value);
            else styles[counter].backColor = defBackColor;

            if(child->GetPropVal(wxT("textColor"), &value)) styles[counter].textColor = wxColour(value);
            else styles[counter].textColor = defTextColor;

            wxString bold = child->GetPropVal(wxT("bold"), wxT("false"));
            wxString underline = child->GetPropVal(wxT("underline"), wxT("false"));
            wxString italic = child->GetPropVal(wxT("italic"), wxT("false"));
            wxString keywords = child->GetPropVal(wxT("keywords"), wxEmptyString);

            if(bold==wxT("true")) styles[counter].bold = true;
            if(underline==wxT("true")) styles[counter].underline = true;
            if(italic==wxT("true")) styles[counter].italic = true;

            if(!keywords.IsEmpty()) styles[counter].keywords = keywords;

            counter++;
        }

        child = child->GetNext();
    }

    styleCount = counter;

    return true;
}
