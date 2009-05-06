/***************************************************************
 * Name:      wxDemoViewerDemoData.cpp
 * Purpose:   Implements storage class for single demo and list of demos
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wxDemoViewerDemoData.h"
#include <wx/listimpl.cpp>

WX_DEFINE_LIST(wxDemoViewerDemoDataList);

wxDemoViewerDemoData::wxDemoViewerDemoData()
{
    imgIndex = 0;
    isDemo = false;
}

wxDemoViewerDemoData::wxDemoViewerDemoData(wxXmlNode *node)
{
    wxString value;
    long i;

    imgIndex = 0;
    isDemo = true;

    title = node->GetPropVal(wxT("title"), wxEmptyString);
    description = node->GetPropVal(wxT("description"), wxEmptyString);

    if(node->GetPropVal(wxT("imageIndex"), &value)) if(value.ToLong(&i)) imgIndex = i;

    node = node->GetChildren();

    while(node)
    {
        if(node->GetName()==wxT("sourceFiles")) loadSourceFiles(node->GetChildren());
        else if(node->GetName()==wxT("keywords")) loadKeywords(node->GetChildren());
        else if(node->GetName()==wxT("executable")) exeFile = node->GetPropVal(wxT("file"), wxEmptyString);

        node = node->GetNext();
    }
}

wxDemoViewerDemoData::wxDemoViewerDemoData(wxString title, int imgIndex)
{
    isDemo = false;
    this->title = title;
    this->imgIndex = imgIndex;
}

void wxDemoViewerDemoData::loadSourceFiles(wxXmlNode *node)
{
    wxString value;
    wxFileName filename;
    long i;

    while(node)
    {
        if(node->GetName()==wxT("sourceFile"))
        {
            long index = 0, lexer = 0;
            wxString f = node->GetPropVal(wxT("file"), wxEmptyString);
            filename.Assign(f, wxPATH_UNIX);

            if(node->GetPropVal(wxT("imageIndex"), &value)) if(value.ToLong(&i)) index = i;
            if(node->GetPropVal(wxT("lexer"), &value)) if(value.ToLong(&i)) lexer = i;

            ownerDrawnComboBoxData *data = new ownerDrawnComboBoxData(filename.GetFullName(), f, index, lexer);
            sourceFiles.Append(data);
        }

       node = node->GetNext();
    }
}

void wxDemoViewerDemoData::loadKeywords(wxXmlNode *node)
{
    wxString value;
    long i;

    while(node)
    {
        if(node->GetName()==wxT("keyword"))
        {
            long index = 0;
            wxString t = node->GetPropVal(wxT("title"), wxEmptyString);

            if(node->GetPropVal(wxT("imageIndex"), &value)) if(value.ToLong(&i)) index = i;

            ownerDrawnComboBoxData *data = new ownerDrawnComboBoxData(t, wxEmptyString, index);
            keywords.Append(data);
        }

       node = node->GetNext();
    }
}
