/***************************************************************
 * Name:      ownerDrawnStatic.h
 * Purpose:   Defines class for static text control with custom background and text colors
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef OWNERDRAWNSTATIC_H
#define OWNERDRAWNSTATIC_H

#include "ownerDrawnPanel.h"
#include "styleStatic.h"
#include "styleBackground.h"

/*!
 *  \brief Class represents static text control with custom background and text colors.
 *  \sa ownerDrawnWindow
 */
class ownerDrawnStatic : public ownerDrawnWindow
{
    public:
        /*!
         * \brief Default contructor.
         * \param parent Pointer to a parent window
         * \param style Background and text style
         */
        ownerDrawnStatic(wxWindow *parent, styleStatic style);

        /*! \brief Destructor */
        ~ownerDrawnStatic();

    private:
        /*! \brief Background and text style. */
        styleStatic m_style;

        /*! \brief Custom font. */
        wxFont m_font;

        /*!
         * \brief Custom drawing function.
         * \param dc Pointer to a device context used for drawing
         */
        virtual void drawCustom(wxPaintDC *dc);
};

#endif // OWNERDRAWNSTATIC_H
