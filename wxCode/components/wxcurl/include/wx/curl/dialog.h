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


//! Possible wxCurlDownloadDialog styles.
enum wxCurlDialogStyle
{
    wxCDS_ELAPSED_TIME = 0x0001,       //!< The dialog shows the elapsed time.
    wxCDS_ESTIMATED_TIME = 0x0002,     //!< The dialog shows the estimated total time.
    wxCDS_REMAINING_TIME = 0x0004,     //!< The dialog shows the remaining time.
    wxCDS_SPEED = 0x0008,              //!< The dialog shows the transfer speed.
    wxCDS_SIZE = 0x0010,               //!< The dialog shows the size of the resource to download/upload.
    wxCDS_URL = 0x0020,                //!< The dialog shows the URL involved in the transfer.

    wxCDS_SHOW_ALL = wxCDS_ELAPSED_TIME|wxCDS_ESTIMATED_TIME|wxCDS_REMAINING_TIME|
                     wxCDS_SPEED|wxCDS_SIZE|wxCDS_URL,

    wxCDS_CAN_ABORT = 0x0040,          //!< The transfer can be aborted by the user.
    wxCDS_CAN_START = 0x0080,          //!< The transfer won't start automatically. The user needs to start it.
    wxCDS_CAN_PAUSE = 0x0100,          //!< The transfer can be paused.

    wxCDS_AUTO_CLOSE = 0x0200,         //!< The dialog auto closes when transfer is complete.

    // by default all available features are enabled:
    wxCDS_DEFAULT_STYLE = wxCDS_CAN_START|wxCDS_CAN_PAUSE|wxCDS_CAN_ABORT|wxCDS_SHOW_ALL|wxCDS_AUTO_CLOSE
};

//! The return flag of .
enum wxCurlDialogReturnFlag
{
    wxCDRF_SUCCESS,        //!< Download successfully completed.
    wxCDRF_USER_ABORTED,   //!< User aborted the download.
    wxCDRF_FAILED          //!< Invalid URL / other networking problem happened.
};


// ----------------------------------------------------------------------------
// wxCurlBaseDialog
// ----------------------------------------------------------------------------

//! The base class for wxCurlDownloadDialog and wxCurlUploadDialog.
class WXDLLIMPEXP_CURL wxCurlBaseDialog : public wxDialog
{
public:
    wxCurlBaseDialog() 
    {
        m_bTransferComplete = false;

        // some of these may remain NULL:
        m_pElapsedTime = m_pEstimatedTime = m_pRemainingTime = NULL;
        m_pSpeed = m_pSize = NULL;
        m_pBitmap = NULL;
        m_pURL = NULL;
        m_pGauge = NULL;
        m_pLastEvent = NULL;
    }

    bool Create(const wxString &url,
                const wxString& title,
                const wxString& message = wxEmptyString,
                const wxString& sizeLabel = _("Transferred:"),
                const wxBitmap& bitmap = wxNullBitmap,
                wxWindow *parent = NULL,
                int style = wxCDS_DEFAULT_STYLE);

    ~wxCurlBaseDialog()
        { wxDELETE(m_pLastEvent); }


    //! Shows the dialog as modal. If the wxCDS_CAN_START flag was not given,
    //! then the transfer starts automatically.
    //! Note that you should use this function instead of wxDialog::ShowModal().
    wxCurlDialogReturnFlag StartModal();

    //! Shows the dialog as modeless.
    bool Show(const bool show);


protected:     // internal utils

    virtual void EndModal(wxCurlDialogReturnFlag retCode);

    wxStaticText *AddSizerRow(wxSizer *sz, const wxString &name);
    void CreateControls(const wxString &msg, const wxString& sizeLabel, const wxBitmap &bitmap);
    void UpdateLabels(wxCurlProgressBaseEvent *ev);

    // change access policy:

    virtual int ShowModal()
        { return wxDialog::ShowModal(); }

public:     // event handlers

    void OnEndPerform(wxCurlEndPerformEvent &);

    void OnAbort(wxCommandEvent &);
    void OnPauseResume(wxCommandEvent &);
    void OnStart(wxCommandEvent &);

    void OnAbortUpdateUI(wxUpdateUIEvent &);
    void OnStartUpdateUI(wxUpdateUIEvent &);
    void OnPauseResumeUpdateUI(wxUpdateUIEvent &);

protected:

    wxCurlBaseThread *m_pThread;
    wxCurlProgressBaseEvent *m_pLastEvent;
    bool m_bTransferComplete;

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
};


// ----------------------------------------------------------------------------
// wxCurlDownloadDialog
// ----------------------------------------------------------------------------

//! An highly-configurable dialog which shows progress about a download.
class WXDLLIMPEXP_CURL wxCurlDownloadDialog : public wxCurlBaseDialog
{
public:
    wxCurlDownloadDialog() { }

    wxCurlDownloadDialog(const wxString &url,
                        wxOutputStream *out,
                        const wxString& title = wxT("Downloading..."),
                        const wxString& message = wxEmptyString,
                        const wxBitmap& bitmap = wxNullBitmap,
                        wxWindow *parent = NULL,
                        int style = wxCDS_DEFAULT_STYLE)
        { Create(url, out, title, message, bitmap, parent, style); }

    bool Create(const wxString &url,
                wxOutputStream *out,
                const wxString& title = wxT("Downloading..."),
                const wxString& message = wxEmptyString,
                const wxBitmap& bitmap = wxNullBitmap,
                wxWindow *parent = NULL,
                int style = wxCDS_DEFAULT_STYLE);


public:     // event handlers

    void OnDownload(wxCurlDownloadEvent &);

private:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxCurlDownloadDialog)
};


// ----------------------------------------------------------------------------
// wxCurlUploadDialog
// ----------------------------------------------------------------------------

//! An highly-configurable dialog which shows progress about an upload.
class WXDLLIMPEXP_CURL wxCurlUploadDialog : public wxCurlBaseDialog
{
public:
    wxCurlUploadDialog() { }

    wxCurlUploadDialog(const wxString &url,
                        wxInputStream *in,
                        const wxString& title = wxT("Uploading..."),
                        const wxString& message = wxEmptyString,
                        const wxBitmap& bitmap = wxNullBitmap,
                        wxWindow *parent = NULL,
                        int style = wxCDS_DEFAULT_STYLE)
        { Create(url, in, title, message, bitmap, parent, style); }

    bool Create(const wxString &url,
                wxInputStream *in,
                const wxString& title = wxT("Uploading..."),
                const wxString& message = wxEmptyString,
                const wxBitmap& bitmap = wxNullBitmap,
                wxWindow *parent = NULL,
                int style = wxCDS_DEFAULT_STYLE);


public:     // event handlers

    void OnUpload(wxCurlUploadEvent &);

private:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxCurlUploadDialog)
};


#endif // _WXCURL_DIALOG_H_

