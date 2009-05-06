/***************************************************************
 * Name:      wxDemoViewerDemoData.h
 * Purpose:   Defines storage class for single demo and list of demos
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef WXDEMOVIEWERDEMODATA_H
#define WXDEMOVIEWERDEMODATA_H

#include <wx/xml/xml.h>
#include <wx/filename.h>
#include <wx/treectrl.h>

#include "ownerDrawnComboBox.h"

/*! \brief Class represents single demo... */
class wxDemoViewerDemoData
{
    public:
        /*! \brief Demo title. */
        wxString title;

        /*! \brief Demo description text. */
        wxString description;

        /*! \brief Demo executable file. */
        wxString exeFile;

        /*! \brief Demo image index used in tree control. */
        int imgIndex;

        /*! \brief List of source files. */
        ownerDrawnComboBoxDataList sourceFiles;

        /*! \brief List of keywords. */
        ownerDrawnComboBoxDataList keywords;

        /*! \brief True if object represents demo or node in tree control. */
        bool isDemo;

        /*! \brief Default contructor. */
        wxDemoViewerDemoData();

        /*!
         * \brief Contstructs object from XML node.
         * \param node Pointer to xml node
         */
        wxDemoViewerDemoData(wxXmlNode *node);

        /*!
         * \brief Set demo title and image index.
         * \param title Demo title
         * \param imgIndex Demo image index used in tree control
         */
        wxDemoViewerDemoData(wxString title, int imgIndex);

        /*!
         * \brief Loads source files from XML node.
         * \param node Pointer to xml node
         */
        void loadSourceFiles(wxXmlNode *node);

        /*!
         * \brief Loads keywords from XML node.
         * \param node Pointer to xml node
         */
        void loadKeywords(wxXmlNode *node);
};

WX_DECLARE_LIST(wxDemoViewerDemoData, wxDemoViewerDemoDataList);

/*! \brief Class encapsulating the tree nodes. */
class wxDemoViewerTreeItemData : public wxTreeItemData
{
    public:
        /*! \brief Destructor */
        wxDemoViewerDemoData *data;

        /*!
         * \brief Default contructor.
         * \param data Pointer to a wxDemoViewerDemoData object
         */
        wxDemoViewerTreeItemData(wxDemoViewerDemoData *data)
        {
            this->data = data;
        }

        /*! \brief Destructor */
        virtual ~wxDemoViewerTreeItemData()
        {
            ;
        }
};

#endif // WXDEMOVIEWERDEMODATA_H
