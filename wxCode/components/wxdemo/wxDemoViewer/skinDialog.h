/***************************************************************
 * Name:      skinDialog.h
 * Purpose:   Defines class encapsulating the Skin selection dialog
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef SKINDIALOG_H
#define SKINDIALOG_H

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/combobox.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>

/*! \brief Class encapsulating the Skin selection dialog. */
class skinDialog : public wxDialog
{
    public:
        /*!
         * \brief Default contructor.
         * \param parent Pointer to a parent window
         * \param demoDir Path to a directory where demo files are located
         * \param exeDir Path to a directory where the main executable is located
         */
        skinDialog(wxWindow *parent, wxString demoDir, wxString exeDir);

        /*! \brief Destructor */
        ~skinDialog();

    private:
        enum
        {
            /*! \brief Window identifier for the Combo Box. */
            idCombo = 2000,

            /*! \brief Window identifier for the OK button. */
            idOkButton
        };

        /*! \brief Path to a directory where demo files are located. */
        wxString m_sDemoDir;

        /*! \brief Path to a directory where the main executable is located. */
        wxString m_sExeDir;

        /*! \brief Pointer to the Combo Box control. */
        wxComboBox *m_pCombo;

        /*! \brief Pointer to the OK button. */
        wxButton *m_pOkButton;

        /*! \brief Used to fill the Combo Box with data. */
        void FindFiles();

        /*!
         * \brief Event handler call when OK button is clicked
         * \param event Command event
         */
        void OnOkClick(wxCommandEvent &event);

        DECLARE_EVENT_TABLE()
};

#endif // SKINDIALOG_H
