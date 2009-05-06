/***************************************************************
 * Name:      styleStatic.h
 * Purpose:   Defines class encapsulating style for a static text control
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef STYLESTATIC_H
#define STYLESTATIC_H

#include <wx/xml/xml.h>

#include "styleBackground.h"

/*! \brief Class encapsulating style for a static text control. */
class styleStatic
{
    public:
        /*! \brief Background style. */
        styleBackground bgStyle;

        /*! \brief Text color. */
        wxColor textColor;

        /*! \brief Displayed text. */
        wxString title;

        /*! \brief Font size. */
        int fontSize;

        /*! \brief Text align. */
        int textAlign;

        /*! \brief Control height. */
        int height;

        /*! \brief Tells if title will be drawned using bold font. */
        bool bold;

        /*! \brief Default contructor. */
        styleStatic();

        /*!
         * \brief Contstructs object from XML node.
         * \param node Pointer to xml node
         */
        styleStatic(wxXmlNode *node);

        /*! \brief Destructor */
        ~styleStatic();
};

#endif // STYLESTATIC_H
