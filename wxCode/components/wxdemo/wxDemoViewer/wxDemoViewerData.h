/***************************************************************
 * Name:      wxDemoViewerData.h
 * Purpose:   Defines storage class for all data loaded from XML file
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef WXDEMOVIEWERDATA_H
#define WXDEMOVIEWERDATA_H

#include <wx/xml/xml.h>
#include <wx/imaglist.h>
#include <wx/filename.h>

#include "styleBackground.h"
#include "styleControl.h"
#include "styleStatic.h"
#include "styleButton.h"
#include "styleLexer.h"
#include "wxSTC.h"
#include "wxDemoViewerDemoData.h"

#define DV_WINDOWTYPE_BACKGROUND 1
#define DV_WINDOWTYPE_CONTROL 2
#define DV_WINDOWTYPE_STATIC 3

WX_DECLARE_STRING_HASH_MAP(int, hashTable);

/*! \brief Class represents all data loaded from demo xml file like documentation, skin, texts, images, demos... */
class wxDemoViewerData
{
    public:
        enum eWindowTypes { typeBackground, typeControl, typeStatic, typeButton, typeOther };
        enum eBackground { bgMainTree, bgMainLogo, bgMainSearch, bgSourceTab, bgTabsPanel };
        enum eConltol { cMainTree, cSourceFiles, cMainSearch, cSourceKeywords, cSourceCodeViewer, cSourceSearch };
        enum eStatic { sMainTitle, sMainSearchTitle, sSourceFilesTitle, sKeywordsTitle };
        enum eButton { bTabDescription, bTabSource, bTabDoc, bLaunchButton, bSourceSearchNext, bSourceSearchPrev, bKeywordsSearchCode, bKeywordsSearchDocs };
        enum eOther { oLogoFile, oSourceCodeViewerMore, oSearchIcon, oSplitter, oDescriptionColors, oDocToolbar };

        /*! \brief ownerDrawnPanel styles. */
        styleBackground stBackground[5];

        /*! \brief Control styles. */
        styleControl stControl[6];

        /*! \brief Static text styles. */
        styleStatic stStatic[4];

        /*! \brief Button styles. */
        styleButton stButton[8];

        /*! \brief Path to the logo file. */
        wxString logoFile;

        /*! \brief Path to search icon image. */
        wxString searchIcon;

        /*! \brief Description background color. */
        wxString descriptionBackColor;

        /*! \brief Description text color. */
        wxString descriptionTextColor;

        /*! \brief Path to the documentation file. */
        wxString docFile;

        /*! \brief Extension which will be added to demo executable file. */
        wxString addedExt;

        /*! \brief Application with which the demo will be opened. */
        wxString openWith;

        /*! \brief Main image list. */
        wxImageList imgList;

        /*! \brief List of text highlighters - lexers. */
        styleLexerList lexers;

        /*! \brief wxStyledTextControl style. */
        wxSTCStyle stCodeViewer;

        /*! \brief wxSplitterWindow background color. */
        wxColour splitterBackColor;

        /*! \brief Documentation toolbar background color. */
        wxColour docToolbarBackColor;

        /*! \brief List of all demos. */
        wxDemoViewerDemoDataList demoList;

        /*! \brief Root for the tree control. */
        wxDemoViewerDemoData treeRoot;

        /*! \brief Default contructor. */
        wxDemoViewerData();

        /*! \brief Destructor */
        ~wxDemoViewerData();

        /*!
         * \brief Loads all data from xml file.
         * \param filename Path to xml file
         */
        bool loadData(wxString filename);

    private:
        /*! \brief String Hash table used to speed up the string recognition process used in xml nodes. */
        hashTable alias;

        /*!
         * \brief Creates image list from xml node.
         * \param node Pointer to xml node
         */
        void loadImageList(wxXmlNode *node);

        /*!
         * \brief Loads styles from xml node.
         * \param node Pointer to xml node
         */
        void loadStyles(wxXmlNode *node);

        /*!
         * \brief Loads lexers from xml node.
         * \param node Pointer to xml node
         */
        void loadLexers(wxXmlNode *node);

        /*!
         * \brief Loads documentation from xml node.
         * \param node Pointer to xml node
         */
        void loadDocumentation(wxXmlNode *node);

        /*!
         * \brief Loads demos from xml node.
         * \param node Pointer to xml node
         */
        void loadDemos(wxXmlNode *node);
};

#endif // WXDEMOVIEWERDATA_H
