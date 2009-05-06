/***************************************************************
 * Name:      styleControl.h
 * Purpose:   Defines class encapsulating style for a control
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef STYLECONTROL_H
#define STYLECONTROL_H

#include <wx/wx.h>
#include <wx/xml/xml.h>

/*! \brief Class encapsulating style for a control. */
class styleControl
{
    public:
        /*! \brief Background color. */
        wxColor backColor;

        /*! \brief Border color. */
        wxColor borderColor;

        /*! \brief Text color. */
        wxColor textColor;

        /*! \brief True if control has border. */
        bool hasBorder;

        /*! \brief True if border is rounded. */
        bool rounded;

        /*! \brief Default contructor. */
        styleControl();

        /*!
         * \brief Contstructs object from XML node.
         * \param node Pointer to xml node
         */
        styleControl(wxXmlNode *node);

        /*! \brief Destructor */
        ~styleControl();
};

#endif // STYLECONTROL_H
