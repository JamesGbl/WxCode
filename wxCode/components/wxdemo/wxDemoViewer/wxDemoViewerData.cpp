/***************************************************************
 * Name:      wxDemoViewerData.cpp
 * Purpose:   Implements storage class for all data loaded from XML file
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wxDemoViewerData.h"

wxDemoViewerData::wxDemoViewerData()
{
//  *** TYPES
    alias[wxT("background")] = typeBackground;
    alias[wxT("control")] = typeControl;
    alias[wxT("static")] = typeStatic;
    alias[wxT("button")] = typeButton;
    alias[wxT("other")] = typeOther;

//  *** BACKGROUNDS
    alias[wxT("mainTree")] = bgMainTree;
    alias[wxT("mainLogo")] = bgMainLogo;
    alias[wxT("mainSearch")] = bgMainSearch;
    alias[wxT("sourceTab")] = bgSourceTab;
    alias[wxT("tabsPanel")] = bgTabsPanel;

//  *** CONTROLS
    alias[wxT("mainTree")] = cMainTree;
    alias[wxT("mainSearch")] = cMainSearch;
    alias[wxT("sourceFiles")] = cSourceFiles;
    alias[wxT("sourceKeywords")] = cSourceKeywords;
    alias[wxT("sourceCodeViewer")] = cSourceCodeViewer;
    alias[wxT("sourceSearch")] = cSourceSearch;

//  *** STATICS
    alias[wxT("mainTitle")] = sMainTitle;
    alias[wxT("mainSearchTitle")] = sMainSearchTitle;
    alias[wxT("sourceFilesTitle")] = sSourceFilesTitle;
    alias[wxT("keywordsTitle")] = sKeywordsTitle;

//  *** BUTTONS
    alias[wxT("tabDescription")] = bTabDescription;
    alias[wxT("tabSource")] = bTabSource;
    alias[wxT("tabDoc")] = bTabDoc;
    alias[wxT("launchButton")] = bLaunchButton;
    alias[wxT("sourceSearchNext")] = bSourceSearchNext;
    alias[wxT("sourceSearchPrev")] = bSourceSearchPrev;
    alias[wxT("keywordsSearchCode")] = bKeywordsSearchCode;
    alias[wxT("keywordsSearchDocs")] = bKeywordsSearchDocs;

//  *** OTHERS STYLES
    alias[wxT("logoFile")] = oLogoFile;
    alias[wxT("sourceCodeViewerMore")] = oSourceCodeViewerMore;
    alias[wxT("searchIcon")] = oSearchIcon;
    alias[wxT("splitterBackColor")] = oSplitter;
    alias[wxT("descriptionColors")] = oDescriptionColors;
    alias[wxT("docToolbarBackColor")] = oDocToolbar;

//  *** OTHER
    imgList.Create(16, 16);

    stCodeViewer.backColor = *wxWHITE;
    stCodeViewer.lineNrTextColor = *wxBLACK;
    stCodeViewer.lineNrBackColor = wxColour(218, 218, 218);
    stCodeViewer.selTextColor = *wxBLACK;
    stCodeViewer.selBackColor = wxColour(218, 218, 218);
    stCodeViewer.fontSize = 10;
    splitterBackColor = wxColour(218, 218, 218);
    descriptionBackColor = wxT("#FFFFFF");
    descriptionTextColor = wxT("#000000");
    docToolbarBackColor = wxColour(218, 218, 218);
    treeRoot.imgIndex = 0;
}

wxDemoViewerData::~wxDemoViewerData()
{
    ;
}

bool wxDemoViewerData::loadData(wxString filename)
{
    wxXmlDocument doc;
    wxFileName fn(filename, wxPATH_UNIX);

    if(filename.IsEmpty()) return false;
    if(!fn.FileExists()) return false;
    if(!doc.Load(fn.GetFullPath())) return false;
    if(doc.GetRoot()->GetName() != wxT("wxDemoViewer")) return false;

    wxXmlNode *node = doc.GetRoot()->GetChildren();
    while(node)
    {
        if(node->GetName()==wxT("imageList")) loadImageList(node->GetChildren());
        else if(node->GetName()==wxT("windowStyles")) loadStyles(node->GetChildren());
        else if(node->GetName()==wxT("lexerList")) loadLexers(node->GetChildren());
        else if(node->GetName()==wxT("documentation")) loadDocumentation(node);
        else if(node->GetName()==wxT("demos"))
        {
            wxString value;
            long i;

            treeRoot.title = node->GetPropVal(wxT("title"), wxEmptyString);
            treeRoot.description = node->GetPropVal(wxT("description"), wxEmptyString);

            if(node->GetPropVal(wxT("imageIndex"), &value)) if(value.ToLong(&i)) treeRoot.imgIndex = i;

#if defined(__WXMSW__)
            if(node->GetPropVal(wxT("openWithWin"), &value) && !value.IsEmpty()) openWith = value + wxT(" ");
            if(node->GetPropVal(wxT("extWin"), &value) && !value.IsEmpty()) addedExt = wxT(".") + value;
#elif defined(__WXOSX__)
            if(node->GetPropVal(wxT("openWithMac"), &value) && !value.IsEmpty()) openWith = value + wxT(" ");
            if(node->GetPropVal(wxT("extMac"), &value) && !value.IsEmpty()) addedExt = wxT(".") + value;
#elif defined(__WXGTK__)
            if(node->GetPropVal(wxT("openWithUnix"), &value) && !value.IsEmpty()) openWith = value + wxT(" ");
            if(node->GetPropVal(wxT("extUnix"), &value) && !value.IsEmpty()) addedExt = wxT(".") + value;
#else
            if(node->GetPropVal(wxT("openWithOther"), &value) && !value.IsEmpty()) openWith = value + wxT(" ");
            if(node->GetPropVal(wxT("extOther"), &value) && !value.IsEmpty()) addedExt = wxT(".") + value;
#endif

            loadDemos(node->GetChildren());
        }

        node = node->GetNext();
    }

    return true;
}

void wxDemoViewerData::loadImageList(wxXmlNode *node)
{
    wxString value;
    wxFileName filename;
    wxBitmap tmpBitmap;

    while(node)
    {
        if(node->GetName()==wxT("image"))
        {
            if(node->GetPropVal(wxT("file"), &value))
            {
                filename.Assign(value, wxPATH_UNIX);
                if(filename.FileExists())
                {
                    if(tmpBitmap.LoadFile(filename.GetFullPath(), wxBITMAP_TYPE_ANY)) imgList.Add(tmpBitmap);
                    else wxLogWarning(wxT("File '%s' has unknown format"), filename.GetFullPath().c_str());
                } else wxLogWarning(wxT("File '%s' not found"), filename.GetFullPath().c_str());
            }
        }

        node = node->GetNext();
    }
}

void wxDemoViewerData::loadStyles(wxXmlNode *node)
{
    wxString value;

    while(node)
    {
        value = node->GetName();

        switch(alias[value])
        {
            case typeBackground:
            {
                styleBackground tmpBgStyle(node);

                if(!node->GetPropVal(wxT("name"), &value)) break;
                stBackground[alias[value]] = tmpBgStyle;
                break;
            }

            case typeControl:
            {
                styleControl tmpBorderStyle(node);

                if(!node->GetPropVal(wxT("name"), &value)) break;

                stControl[alias[value]] = tmpBorderStyle;
                break;
            }

            case typeStatic:
            {
                styleStatic tmpStaticStyle(node);

                if(!node->GetPropVal(wxT("name"), &value)) break;
                stStatic[alias[value]] = tmpStaticStyle;
                break;
            }

            case typeButton:
            {
                styleButton tmpButtonStyle(node);

                if(!node->GetPropVal(wxT("name"), &value)) break;
                stButton[alias[value]] = tmpButtonStyle;
                break;
            }

            case typeOther:
            {
                if(!node->GetPropVal(wxT("name"), &value)) break;

                switch(alias[value])
                {
                    case oLogoFile:
                        if(node->GetPropVal(wxT("file"), &value)) logoFile = value;
                        break;

                    case oSourceCodeViewerMore:
                    {
                        long i;

                        if(node->GetPropVal(wxT("lineNumberTextColor"), &value)) stCodeViewer.lineNrTextColor = wxColour(value);
                        if(node->GetPropVal(wxT("lineNumberBackColor"), &value)) stCodeViewer.lineNrBackColor = wxColour(value);
                        if(node->GetPropVal(wxT("selectionTextColor"), &value)) stCodeViewer.selTextColor = wxColour(value);
                        if(node->GetPropVal(wxT("selectionBackColor"), &value)) stCodeViewer.selBackColor = wxColour(value);
                        if(node->GetPropVal(wxT("fontSize"), &value)) if(value.ToLong(&i)) stCodeViewer.fontSize = i;
                        break;
                    }

                    case oSearchIcon:
                        if(node->GetPropVal(wxT("file"), &value)) searchIcon = value;
                        break;

                    case oSplitter:
                        if(node->GetPropVal(wxT("backColor"), &value)) splitterBackColor = wxColour(value);
                        break;

                    case oDescriptionColors:
                        if(node->GetPropVal(wxT("backColor"), &value)) descriptionBackColor = value;
                        if(node->GetPropVal(wxT("textColor"), &value)) descriptionTextColor = value;
                        break;

                    case oDocToolbar:
                        if(node->GetPropVal(wxT("backColor"), &value)) docToolbarBackColor = wxColour(value);
                        break;
                }

                break;
            }
        }

       node = node->GetNext();
    }
}

void wxDemoViewerData::loadLexers(wxXmlNode *node)
{
    wxString value;

    while(node)
    {
        if(node->GetName()==wxT("lexer"))
        {
            if(node->GetPropVal(wxT("file"), &value))
            {
                styleLexer *LS = new styleLexer();

                wxFileName filename(value);
                if(filename.FileExists())
                {
                    LS->loadLexterStyle(filename.GetFullPath());
                    lexers.Append(LS);
                } else wxLogWarning(wxT("File '%s' not found"), filename.GetFullPath().c_str());
            }
        }

       node = node->GetNext();
    }
}

void wxDemoViewerData::loadDocumentation(wxXmlNode *node)
{
    wxString value;

    if(node->GetPropVal(wxT("file"), &value))
    {
        if(!value.IsEmpty())
        {
            wxFileName filename(value, wxPATH_UNIX);
            if(filename.FileExists()) docFile = filename.GetFullPath();
            else wxLogWarning(wxT("File '%s' not found"), filename.GetFullPath().c_str());
        }
    }
}

void wxDemoViewerData::loadDemos(wxXmlNode *node)
{
    wxString value;

    while(node)
    {
        value = node->GetName();

        if(value==wxT("demo"))
        {
            wxDemoViewerDemoData *data = new wxDemoViewerDemoData(node);
            demoList.Append(data);
        }
        else if(value==wxT("demoGroup"))
        {
            long i = 0, index = 0;

            if(node->GetPropVal(wxT("imageIndex"), &value)) if(value.ToLong(&i)) index = i;

            wxDemoViewerDemoData *data = new wxDemoViewerDemoData(node->GetPropVal(wxT("title"), wxEmptyString), i);
            demoList.Append(data);

            loadDemos(node->GetChildren());
        }

        node = node->GetNext();
    }
}
