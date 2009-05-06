/***************************************************************
 * Name:      ownerDrawnSplitter.h
 * Purpose:   Defines class for wxSplitterWindow with custom background color
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef OWNERDRAWNSPLITTER_H
#define OWNERDRAWNSPLITTER_H

#include <wx/wx.h>
#include <wx/splitter.h>

/*! \brief Class represents wxSplitterWindow with custom background color. */
class ownerDrawnSplitter : public wxSplitterWindow
{
    public:
        /*!
         * \brief Default contructor.
         * \param parent Pointer to a parent window
         * \param id Window identifier
         * \param backColor Background color
         */
        ownerDrawnSplitter(wxWindow *parent, wxWindowID id, wxColour backColor);

        /*! \brief Destructor */
        ~ownerDrawnSplitter();

    private:
        /*! \brief Bacground color. */
        wxColor m_backColor;

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

#endif // OWNERDRAWNSPLITTER_H
