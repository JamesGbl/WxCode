/////////////////////////////////////////////////////////////////////////////
// Name:        dialog.h
// Purpose:     wxCurlDownloadDialog, wxCurlUploadDialog
// Author:      Francesco Montorsi
// Created:     2007/04/14
// RCS-ID:      $Id$
// Copyright:   (c) 2007 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXCURL_DIALOG_H_
#define _WXCURL_DIALOG_H_

#include "wx/dialog.h"
#include "wx/curl/thread.h"

// forward declarations
class WXDLLIMPEXP_CORE wxStaticText;
class WXDLLIMPEXP_CORE wxTextCtrl;
class WXDLLIMPEXP_CORE wxGauge;
class WXDLLIMPEXP_CORE wxStaticBitmap;


// ----------------------------------------------------------------------------
// wxCurlDownloadDialog
// ----------------------------------------------------------------------------

//! Possible wxCurlDownloadDialog styles.
enum wxDownloadDialogStyles
{
    wxDLD_ELAPSED_TIME = 0x0001,       //!< The dialog shows the elapsed time.
    wxDLD_ESTIMATED_TIME = 0x0002,     //!< The dialog shows the estimated total time.
    wxDLD_REMAINING_TIME = 0x0004,     //!< The dialog shows the remaining time.
    wxDLD_SPEED = 0x0008,              //!< The dialog shows the transfer speed.
    wxDLD_SIZE = 0x0010,               //!< The dialog shows the size of the resource to download/upload.
    wxDLD_URL = 0x0020,                //!< The dialog shows the URL involved in the transfer.

    wxDLD_SHOW_ALL = wxDLD_ELAPSED_TIME|wxDLD_ESTIMATED_TIME|wxDLD_REMAINING_TIME|
                     wxDLD_SPEED|wxDLD_SIZE|wxDLD_URL,

    wxDLD_CAN_ABORT = 0x0040,          //!< The transfer can be aborted by the user.
    wxDLD_CAN_START = 0x0080,          //!< The transfer won't start automatically. The user needs to start it.
    wxDLD_CAN_PAUSE = 0x0100,          //!< The transfer can be paused.

    wxDLD_AUTO_CLOSE = 0x0200,         //!< The dialog auto closes when transfer is complete.

    // by default all available features are enabled:
    wxDLD_DEFAULT_STYLE = wxDLD_CAN_START|wxDLD_CAN_PAUSE|wxDLD_CAN_ABORT|wxDLD_SHOW_ALL|wxDLD_AUTO_CLOSE
};

enum wxDownloadReturnFlag
{
    wxDRF_SUCCESS,        //!< Download successfully completed.
    wxDRF_USER_ABORTED,   //!< User aborted the download.
    wxDRF_FAILED          //!< Invalid URL / other networking problem happened.
};


//! An highly-configurable dialog which shows progress about a download.
class WXDLLIMPEXP_CURL wxCurlDownloadDialog : public wxDialog
{
public:
    wxCurlDownloadDialog() {}

    wxCurlDownloadDialog(const wxString &url,
                        wxOutputStream *out,
                        const wxString& title = wxT("Downloading..."),
                        const wxString& message = wxEmptyString,
                        const wxBitmap& bitmap = wxNullBitmap,
                        wxWindow *parent = NULL,
                        int style = wxDLD_DEFAULT_STYLE)
        { Create(url, out, title, message, bitmap, parent, style); }

    bool Create(const wxString &url,
                wxOutputStream *out,
                const wxString& title = wxT("Downloading..."),
                const wxString& message = wxEmptyString,
                const wxBitmap& bitmap = wxNullBitmap,
                wxWindow *parent = NULL,
                int style = wxDLD_DEFAULT_STYLE);

    ~wxCurlDownloadDialog()
        { wxDELETE(m_lastEvent); }


    //! Starts the download of the url and in the output stream given at creation time.
    //! Note that you should use this function instead of wxDialog::ShowModal().
    wxDownloadReturnFlag StartModal();


protected:     // internal utils

    virtual void EndModal(wxDownloadReturnFlag retCode);

    wxStaticText *AddSizerRow(wxSizer *sz, const wxString &name);
    void CreateControls(const wxString &msg, const wxBitmap &bitmap);

    void ForceUpdate(wxCurlDownloadEvent &ev);

    // change access policy:

    virtual int ShowModal()
        { return wxDialog::ShowModal(); }

public:     // event handlers

    void OnEndPerform(wxCurlEndPerformEvent &);
    void OnDownload(wxCurlDownloadEvent &);

    void OnAbort(wxCommandEvent &);
    void OnPauseResume(wxCommandEvent &);
    void OnStart(wxCommandEvent &);

    void OnAbortUpdateUI(wxUpdateUIEvent &);
    void OnStartUpdateUI(wxUpdateUIEvent &);
    void OnPauseResumeUpdateUI(wxUpdateUIEvent &);

protected:

    wxCurlDownloadThread m_thread;
    wxCurlDownloadEvent *m_lastEvent;

    bool m_bDownloadComplete;

protected:      // controls

    wxTextCtrl* m_pURL;
    wxStaticText* m_pSpeed;
    wxStaticText* m_pSize;
    wxGauge* m_pGauge;
    wxStaticBitmap* m_pBitmap;

    wxStaticText* m_pElapsedTime;
    wxStaticText* m_pRemainingTime;
    wxStaticText* m_pEstimatedTime;

private:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxCurlDownloadDialog)
};

#endif // _WXCURL_DIALOG_H_

