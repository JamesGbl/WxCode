/***************************************************************
 * Name:      ownerDrawnStatic.h
 * Purpose:   Defines class for a window with custom background colors
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef OWNERDRAWNWINDOW_H
#define OWNERDRAWNWINDOW_H

#include <wx/window.h>

#include "styleBackground.h"

/*! \brief Class represents panel with custom background colors. */
class ownerDrawnWindow : public wxWindow
{
    public:
        /*!
         * \brief Default contructor.
         * \param parent Pointer to a parent window
         * \param id Window identifier
         * \param style Background style
         * \param size Window size
         */
        ownerDrawnWindow(wxWindow *parent, wxWindowID id, styleBackground style, wxSize size = wxDefaultSize);

        /*! \brief Destructor */
        virtual ~ownerDrawnWindow();

        /*!
         * \brief Changes background style to new provided style.
         * \param style New background style
         */
        void changeStyle(styleBackground style);

    private:
        /*! \brief Current bacground style. */
        styleBackground m_bgStyle;

        /*!
         * \brief Custom drawing function
         * \param dc Pointer to a device context used for drawing
         */
        virtual void drawCustom(wxPaintDC *dc);

        /*!
         * \brief Event handler call when panel's background needs to be repainted
         * \param event Erase event
         */
        void OnEraseBackground(wxEraseEvent &event);

        /*!
         * \brief Event handler call when panel needs to be repainted
         * \param event Paint event
         */
        void OnPaint(wxPaintEvent &event);

        /*!
         * \brief Event handler call when panel has been resized
         * \param event Size event
         */
        void OnResize(wxSizeEvent &event);

        DECLARE_EVENT_TABLE()
};

#endif // OWNERDRAWNWINDOW_H
