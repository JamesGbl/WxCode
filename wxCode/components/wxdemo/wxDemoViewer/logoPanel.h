/***************************************************************
 * Name:      logoPanel.h
 * Purpose:   Defines class for panel with custom background colors and image
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef LOGOPANEL_H
#define LOGOPANEL_H

#include <wx/filename.h>

#include "ownerDrawnPanel.h"

/*!
 *  \brief Class represents panel with custom background colors and image.
 *  \sa ownerDrawnWindow
 */
class logoPanel : public ownerDrawnWindow
{
    public:
        /*!
         * \brief Default contructor.
         * \param parent Pointer to a parent window
         * \param style Background style
         * \param filename Image filename
         */
        logoPanel(wxWindow *parent, styleBackground style, wxString filename);

        /*! \brief Destructor */
        ~logoPanel();

    private:
        /*! \brief Custom image. */
        wxBitmap m_img;

        /*! \brief True if image is successfully loaded. */
        bool m_hasImage;

        /*! \brief Image width and height. */
        int m_imgWidth, m_imgHeight;

        /*!
         * \brief Custom drawing function. Draws image.
         * \param dc Pointer to a device context used for drawing
         */
        virtual void drawCustom(wxPaintDC *dc);
};

#endif // LOGOPANEL_H
