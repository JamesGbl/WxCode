/***************************************************************
 * Name:      ownerDrawnPanel.h
 * Purpose:   Defines class for panel with custom background colors and a ability to draw borders around selected controls
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef OWNERDRAWNPANEL_H
#define OWNERDRAWNPANEL_H

#include "ownerDrawnWindow.h"
#include "styleControl.h"

/*! \brief Class encapsulating the control border style. */
class cWindowListElement
{
    public:
        /*! \brief Pointer to a window. */
        wxWindow *window;

        /*! \brief Border style. */
        styleControl bs;

        /*!
         * \brief Default contructor.
         * \param window Pointer to a parent window
         * \param style Control style
         */
        cWindowListElement(wxWindow *window, styleControl style);

        /*! \brief Destructor */
        ~cWindowListElement();
};

WX_DECLARE_LIST(cWindowListElement, cWindowList);

/*!
 *  \brief Class represents panel with custom background colors and a ability to draw borders around selected controls.
 *  \sa ownerDrawnWindow
*/
class ownerDrawnPanel : public ownerDrawnWindow
{
    public:
        /*!
         * \brief Default contructor.
         * \param parent Pointer to a parent window
         * \param style Background style
         */
        ownerDrawnPanel(wxWindow *parent, styleBackground style, wxSize size = wxDefaultSize);

        /*! \brief Destructor */
        ~ownerDrawnPanel();

        /*!
         * \brief Tells panel to draw a border around window pointed by first parameter.
         * \param window Pointer to window
         * \param style Border style
         */
        void AppendBorder(wxWindow *window, styleControl style);

    private:
        /*! \brief List of windows */
        cWindowList m_windowList;

        /*!
         * \brief Custom drawing function. Draws borders.
         * \param dc Pointer to a device context used for drawing
         */
        virtual void drawCustom(wxPaintDC *dc);

        /*!
         * \brief Event handler call when panel has been resized
         * \param event Size event
         */
        void OnSize(wxSizeEvent& event);

        WX_DECLARE_CONTROL_CONTAINER();
        DECLARE_EVENT_TABLE()
};

#endif // OWNERDRAWNPANEL_H
