/***************************************************************
 * Name:      ownerDrawnButton.h
 * Purpose:   Defines class for button with custom background colors and image
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef OWNERDRAWNBUTTON_H
#define OWNERDRAWNBUTTON_H

#include <wx/filename.h>

#include "ownerDrawnPanel.h"
#include "styleButton.h"

#define BUTTON_STATE_NORMAL 0
#define BUTTON_STATE_HOVER 1
#define BUTTON_STATE_SELECTED 2

/*!
 *  \brief Class represents button with custom background colors and image.
 *  \sa ownerDrawnWindow
 */
class ownerDrawnButton : public ownerDrawnWindow
{
    public:
        /*!
         * \brief Default contructor.
         * \param parent Pointer to a parent window
         * \param id Window identifier
         * \param style Background style
         * \param toggle True if button stays pressed when clicked by the user
         */
        ownerDrawnButton(wxWindow *parent, wxWindowID id, styleButton style, bool toggle = false);

        /*! \brief Destructor */
        ~ownerDrawnButton();

        /*! \brief Restores the default style */
        void resetStyle();

        /*! \brief Simulates click on the button */
        void select();

    private:
        /*! \brief Background style. */
        styleButton m_style;

        /*! \brief Font for different states. */
        wxFont m_font[3];

        /*! \brief Custom image. */
        wxBitmap m_img;

        /*! \brief True if button is pressed. */
        bool m_selected;

        /*! \brief  True if image is successfully loaded. */
        bool m_hasImage;

        /*! \brief  True if button is Toggle button. */
        bool m_toggle;

        /*! \brief Current button state. */
        int m_currentState;

        /*! \brief Image width and height. */
        int m_imgWidth, m_imgHeight;

        /*!
         * \brief Custom drawing function.
         * \param dc Pointer to a device context used for drawing
         */
        virtual void drawCustom(wxPaintDC *dc);

        /*! \brief Returns the best size for the button. */
        virtual wxSize DoGetBestSize() const;

        /*!
         * \brief Event handler call when button is clicked
         * \param event Mouse event
         */
        void OnLeftClick(wxMouseEvent &event);

        /*!
         * \brief Event handler call when mouse button is released
         * \param event Mouse event
         */
        void OnLeftRelease(wxMouseEvent &event);

        /*!
         * \brief Event handler call when mouse enters the button
         * \param event Mouse event
         */
        void OnEnterWindow(wxMouseEvent &event);

        /*!
         * \brief Event handler mouse leaves the button
         * \param event Mouse event
         */
        void OnLeaveWindow(wxMouseEvent &event);

        DECLARE_EVENT_TABLE()
};

#endif // OWNERDRAWNBUTTON_H
