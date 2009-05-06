/***************************************************************
 * Name:      styleBackground.h
 * Purpose:   Defines class encapsulating style for a ownerdrawn background
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef STYLEBACKGROUND_H
#define STYLEBACKGROUND_H

#include <wx/wx.h>
#include <wx/xml/xml.h>

#define CQ_COLOR_A 0
#define CQ_COLOR_B 1
#define CQ_COLOR_C 2
#define CQ_COLOR_D 3

/*! \brief Class encapsulating style for a ownerdrawn window. */
class styleBackground
{
    public:
        /*! \brief Background colors. */
        wxColour colors[4];

        /*! \brief Border color. */
        wxColour borderColor;

        /*! \brief Color count. */
        int colorCount;

        /*! \brief Tells which borders will be drawned. */
        int borders;

        /*! \brief Default contructor. */
        styleBackground();

        /*!
         * \brief Contstructs object from XML node.
         * \param node Pointer to xml node
         */
        styleBackground(wxXmlNode *node);

        /*! \brief Destructor */
        ~styleBackground();
};

#endif // STYLEBACKGROUND_H
