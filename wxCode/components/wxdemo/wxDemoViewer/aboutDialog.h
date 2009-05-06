/***************************************************************
 * Name:      aboutDialog.h
 * Purpose:   Defines class for the About dialog
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <wx/wx.h>
#include <wx/dialog.h>

/*! \brief Class encapsulating the About dialog. */
class aboutDialog : public wxDialog
{
    public:
        /*!
         * \brief Default contructor.
         * \param parent Pointer to a parent window
         */
        aboutDialog(wxWindow *parent);

        /*! \brief Destructor */
        ~aboutDialog();

    private:
        /*!
         * \brief Event handler call when dialog's background needs to be repainted
         * \param event Erase event
         */
        void OnEraseBackground(wxEraseEvent &event);

        /*!
         * \brief Event handler call when dialog needs to be repainted
         * \param event Paint event
         */
        void OnPaint(wxPaintEvent &event);

        /*!
         * \brief Event handler call when dialog is clicked
         * \param event Mouse event
         */
        void OnClick(wxMouseEvent &event);

        DECLARE_EVENT_TABLE()
};

#endif // ABOUTDIALOG_H
