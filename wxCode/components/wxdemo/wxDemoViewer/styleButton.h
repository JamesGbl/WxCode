/***************************************************************
 * Name:      styleButton.h
 * Purpose:   Defines class encapsulating style for a ownerdrawn button
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef STYLEBUTTON_H
#define STYLEBUTTON_H

#include <wx/xml/xml.h>

#include "styleBackground.h"

/*! \brief Class encapsulating style for a ownerdrawn button. */
class styleButton
{
    public:
        /*! \brief Background styles for different button states. */
        styleBackground bgStyle[3];

        /*! \brief Button title. */
        wxString title;

        /*! \brief Image filename. */
        wxString imgFile;

        /*! \brief Text colors for different button states. */
        wxColor textColor[3];

        /*! \brief Font size. */
        int fontSize;

        /*! \brief Text align. */
        int textAlign;

        /*! \brief Tells if title will be drawned using bold font. */
        bool bold[3];

        /*! \brief Position of a image. Choices are wxTOP, wxRIGHT, wxBOTTOM, wxLEFT */
        int imgPosition;

        /*! \brief Default contructor. */
        styleButton();

        /*!
         * \brief Contstructs object from XML node.
         * \param node Pointer to xml node
         */
        styleButton(wxXmlNode *node);

        /*! \brief Destructor */
        ~styleButton();
};

#endif // STYLEBUTTON_H
