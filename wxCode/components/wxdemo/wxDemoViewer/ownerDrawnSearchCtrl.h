/***************************************************************
 * Name:      ownerDrawnSearchCtrl.h
 * Purpose:   Defines class for search control with custom background and text color and image
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef OWNERDRAWNSEARCHCTRL_H
#define OWNERDRAWNSEARCHCTRL_H

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/filename.h>
#include <wx/textctrl.h>

/*! \brief Class represents search control with custom background and text color and image. */
class ownerDrawnSearchCtrl : public wxPanel
{
    public:
        /*!
         * \brief Default contructor.
         * \param parent Pointer to a parent window
         * \param id Window identifier
         * \param textColor Text color
         * \param backColor Background color
         * \param filename Image filename
         */
        ownerDrawnSearchCtrl(wxWindow *parent, wxWindowID id, wxColour textColor, wxColour backColor, wxString filename);

        /*! \brief Destructor */
        ~ownerDrawnSearchCtrl();

        /*! \brief Returns current text value */
        wxString GetValue();

        /*!
         * \brief Sets the text value.
         * \param value New value
         */
        void SetValue(wxString value);

        /*! \brief Sets the window to receive keyboard input */
        void Focus();

    private:
        /*! \brief Background color. */
        wxColour m_backColor;

        /*! \brief Pointer to text control. */
        wxTextCtrl *m_pTextCtrl;

        /*! \brief Custom image. */
        wxBitmap m_img;

        /*! \brief True if image is successfully loaded. */
        bool m_hasImage;

        /*! \brief Image width and height. */
        int m_imgWidth, m_imgHeight;

        /*! \brief Returns the best size for control. */
        virtual wxSize DoGetBestSize() const;

        /*!
         * \brief Event handler call when control's background needs to be repainted
         * \param event Erase event
         */
        void onEraseBackground(wxEraseEvent &event);

        /*!
         * \brief Event handler call when control needs to be repainted
         * \param event Paint event
         */
        void onPaint(wxPaintEvent &event);

        /*!
         * \brief Event handler call when control has been resized
         * \param event Size event
         */
        void onResize(wxSizeEvent &event);

        DECLARE_EVENT_TABLE()
};

#endif // OWNERDRAWNSEARCHCTRL_H
