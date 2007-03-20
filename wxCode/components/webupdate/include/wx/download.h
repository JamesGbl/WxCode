/////////////////////////////////////////////////////////////////////////////
// Name:        download.h
// Purpose:     wxDownloadThread, wxSizeCacherThread, wxDownloadDialog
// Author:      Francesco Montorsi
// Created:     2005/07/14
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_DOWNLOAD_H_
#define _WX_DOWNLOAD_H_

// wxWidgets headers
#include "wx/defs.h"
#include "wx/url.h"
#include "wx/datetime.h"
#include "wx/filename.h"        // for wxInvalidSize
#include "wx/thread.h"

#if wxUSE_GUI
#include "wx/event.h"
#include "wx/progdlg.h"
#include "wx/textctrl.h"
#endif

class WXDLLIMPEXP_NET wxDownloadEvent;

enum wxDownloadThreadFlag
{
    wxDTF_CONTINUE,
    wxDTF_PAUSE,
    wxDTF_ABORT
};

// The size of the temporary buffer used by wxDownloadThread to store the
// downloaded data.
#define wxDT_BUF_TEMP_SIZE              2048

#if wxUSE_GUI

// wxDownloadEvent uses the following event types:
BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_NET, wxEVT_COMMAND_DOWNLOAD_CONNECTED, 6891)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_NET, wxEVT_COMMAND_DOWNLOAD_UPDATE, 6892)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_NET, wxEVT_COMMAND_DOWNLOAD_PAUSED, 6893)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_NET, wxEVT_COMMAND_DOWNLOAD_ABORTED, 6894)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_NET, wxEVT_COMMAND_DOWNLOAD_USER_ABORTED, 6895)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_NET, wxEVT_COMMAND_DOWNLOAD_COMPLETE, 6896)
END_DECLARE_EVENT_TYPES()

#endif      // wxUSE_GUI


// ----------------------------------------------------------------------------
// wxDownloadThread
// ----------------------------------------------------------------------------

// This class is generic and can be used to download anything you need
// from any URL.
// It already provides some useful functions like #GetDownloadSpeed() and
// #GetRemainingTime() which helps you to create dialogs with progress bars
// and/or time indicators.
class WXDLLIMPEXP_NET wxDownloadThread : public wxThread
{
protected:		// these are written by this thread and they must be only read
                // by other classes; to ensure that, user can access these only
                // through the events posted by wxDownloadThread or using getters

    // The moment in the time where the download of the current file started.
    wxDateTime m_dtStart;

    // How much of the file has been currently downloaded.
    wxULongLong m_nCurrentSize;

    // The size of the download.
    wxULongLong m_nFinalSize;

    // The output stream of downloaded data.
    wxOutputStream *m_output;

protected:            // (these vars are only read by this thread; so that, even
                    //  if user is allowed to modify them through setters,
                    //  they do not need to be protected with mutexes).

    // The wxEvtHandler which will receive our notification events.
    wxEvtHandler *m_pHandler;

    // The ID of our events
    int m_nId;

    // The URL of the resource this thread will download.
    wxURL m_url;

    // The download will proceed until this variable is NULL or its value is
    // wxDTF_CONTINUE; the download will be paused when its value is wxDTF_PAUSE
    // and aborted when it's wxDTF_ABORT.
    wxDownloadThreadFlag *m_pFlag;

public:
    wxDownloadThread(wxEvtHandler *handler = NULL,
                    int id = wxID_ANY,
                    wxDownloadThreadFlag *flag = NULL,
                    const wxString &url = wxEmptyString,
                    wxOutputStream *out = NULL)
        : wxThread(wxTHREAD_DETACHED)
    { 
        m_nId=id;
        m_pHandler = handler;

        m_nFinalSize = 0; 
        m_nCurrentSize = 0;
        m_pFlag = flag;
        m_output = NULL;

        if (!url.IsEmpty())
            Download(url, out);
    }

public:     // getters; identic to the ones in wxDownloadEvent but these
            // can be accessed only when you're sure the thread is still running

    wxString GetHumanReadableDownloadSpeed() const;
    wxTimeSpan GetElapsedTime() const;
    wxTimeSpan GetEstimatedRemainingTime() const;

    wxDateTime GetDownloadStartTime() const
        { return m_dtStart; }
    wxULongLong GetCurrentSize() const
        { return m_nCurrentSize; }
    wxULongLong GetTotalSize() const
        { return m_nFinalSize; }

public:     // public API

    void SetOwner(wxEvtHandler *handler, int id = wxID_ANY)
        { m_pHandler=handler; m_nId=id; }
    void SetFlag(wxDownloadThreadFlag *flag)
        { m_pFlag=flag; }

    wxThreadError Download(const wxString &url, wxOutputStream *out = NULL);


protected:

    // overrideables; default action is to send events
    // console applications (with wxUSE_GUI=0) may override these
    // to be able to use wxDownloadThread

#if wxUSE_GUI

    virtual void OnConnect()
        { PostEvent(wxEVT_COMMAND_DOWNLOAD_CONNECTED); }
    virtual void OnUpdate()
        { PostEvent(wxEVT_COMMAND_DOWNLOAD_UPDATE); }
    virtual void OnPause()
        { PostEvent(wxEVT_COMMAND_DOWNLOAD_PAUSED); }
    virtual void OnAbort()
        { PostEvent(wxEVT_COMMAND_DOWNLOAD_ABORTED); }
    virtual void OnUserAbort()
        { PostEvent(wxEVT_COMMAND_DOWNLOAD_USER_ABORTED); }
    virtual void OnComplete()
        { PostEvent(wxEVT_COMMAND_DOWNLOAD_COMPLETE); }

#else

    virtual void OnConnect() {}
    virtual void OnUpdate() {}
    virtual void OnPause() {}
    virtual void OnAbort() {}
    virtual void OnUserAbort() {}
    virtual void OnComplete() {}

#endif

    wxDownloadThreadFlag GetFlag() const; 
    void PostEvent(WXTYPE eventType);

    virtual void *Entry();
    virtual void OnExit();
};



// ----------------------------------------------------------------------------
// wxSizeCacherThread
// ----------------------------------------------------------------------------

// This threads just retrieve the sizes of the given files and cache them.
// All remote operations can take various time to be completed and thus
// they should all be performed by a secondary thread.
class WXDLLIMPEXP_NET wxSizeCacherThread : public wxThread
{
protected:

    // The array with our precomputed sizes.
    // It must be allocated on the heap since it will be returned as
    // result by the wxEVT_COMMAND_CACHESIZE_COMPLETE event and when the
    // event handler will process this event, this thread could have been
    // already destroyed (this is a detached thread).
    wxArrayLong *m_urlSizes;

public:		// to avoid setters/getters (these vars are only read by this thread;
            // they are never written and so they are not protected with mutexes).

    // The wxEvtHandler which will receive our wxDT_NOTIFICATION events.
    wxEvtHandler *m_pHandler;

    // The list of the file paths (as URLs) whose sizes must be cached.
    wxArrayString m_urls;

public:
    wxSizeCacherThread(wxEvtHandler *dlg = NULL) 
        : wxThread(wxTHREAD_DETACHED), m_pHandler(dlg) {}
    virtual ~wxSizeCacherThread() {}

    // Cache the file sizes and then exits (this raises a call to #OnExit)
    virtual void *Entry();
    
    // Sends the wxEVT_COMMAND_CACHESIZE_COMPLETE event
    // to the #m_pHandler event handler.
    virtual void OnExit();
};

DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_NET, wxEVT_COMMAND_CACHESIZE_COMPLETE, -1);

#define EVT_CACHESIZE_COMPLETE(id, func)		\
    EVT_COMMAND(id, wxEVT_COMMAND_CACHESIZE_COMPLETE, func)




#if wxUSE_GUI

// ----------------------------------------------------------------------------
// wxDownloadDialog
// ----------------------------------------------------------------------------

enum
{
    wxDLD_ELAPSED_TIME = 0x0100,
    wxDLD_ESTIMATED_TIME = 0x0200,
    wxDLD_REMAINING_TIME = 0x0400,
    wxDLD_CAN_ABORT = 0x0800,

    wxDLD_DEFAULT_STYLE = wxDLD_CAN_ABORT|wxDLD_ELAPSED_TIME|
                          wxDLD_ESTIMATED_TIME|wxDLD_REMAINING_TIME
};

enum wxDownloadReturnFlag
{
    wxDRF_SUCCESS,        // download successfully completed; output stream
                        // contains all the data
    wxDRF_USER_ABORTED,   // user aborted the download
    wxDRF_FAILED          // invalid URL / other networking problem happened
};

class WXDLLIMPEXP_NET wxDownloadDialog : public wxDialog
{
public:
    wxDownloadDialog() {}

    wxDownloadDialog(const wxString &url,
                    wxOutputStream *out,
                    const wxString& title = wxT("Downloading..."), 
                    const wxString& message = wxEmptyString, 
                    wxWindow * parent = NULL, 
                    int style = wxDLD_DEFAULT_STYLE)
        { Create(url, out, title, message, parent, style); }

    bool Create(const wxString &url,
                wxOutputStream *out,
                const wxString& title = wxT("Downloading..."), 
                const wxString& message = wxEmptyString, 
                wxWindow * parent = NULL, 
                int style = wxDLD_DEFAULT_STYLE);

protected:     // internal utils

    virtual void EndModal(wxDownloadReturnFlag retCode);

    wxStaticText *AddTimeSizer(wxSizer *sz, const wxString &name);
    void CreateControls(const wxString &msg);

public:     // event handlers

    void OnDownloadComplete(wxDownloadEvent &);
    void OnDownloadUpdate(wxDownloadEvent &);
    void OnDownloadAbort(wxDownloadEvent &);
    void OnDownloadUserAbort(wxDownloadEvent &);

    void OnCancel(wxCommandEvent &);
    void OnIdle(wxIdleEvent &ev);

protected:      // thread controller stuff

    wxDownloadThreadFlag m_flag;
    wxDownloadReturnFlag m_exitCode;

    // this flag is updated by wxDownloadDialog in its
    // event handlers in response to thread creation/destruction.
    // the "volatile" attribute is important to avoid unwanted compiler
    // optimizations (see e.g. http://www.ddj.com/dept/cpp/184403766)
    volatile bool m_bThreadExist;

    // true until the first DownloadUpdate() arrives
    bool m_bConnecting;

    // required for the "are you sure?" stuff
    bool m_bDisableEvents;
    bool m_bDelayEndModal;

protected:      // controls

    wxTextCtrl* m_pURL;
    wxStaticText* m_pSpeed;
    wxStaticText* m_pSize;
    wxGauge* m_pGauge;

    wxStaticText* m_pElapsedTime;
    wxStaticText* m_pRemainingTime;
    wxStaticText* m_pEstimatedTime;

private:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxDownloadDialog)
};



// ----------------------------------------------------------------------------
// wxDownloadEvent
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_NET wxDownloadEvent : public wxCommandEvent
{
public:
    wxDownloadEvent() {}
    wxDownloadEvent(WXTYPE eventType, int id, 
                    const wxDateTime &start,
                    const wxString &url = wxEmptyString, 
                    const wxULongLong &curr = wxInvalidSize, 
                    const wxULongLong &total = wxInvalidSize)
        : wxCommandEvent(eventType, id)
    {
        SetString(url);
        m_curr = curr;
        m_total = total;

        m_timeStart = start;
        m_timeEvent = wxDateTime::Now();
    }

public:

    wxULongLong GetDownloadSpeed() const;
    double GetDownloadProgress() const;

    wxString GetHumanReadableDownloadSpeed(const wxString &invalid = _("Not available")) const;

    wxString GetURL() const
        { return GetString(); }

    wxTimeSpan GetElapsedTime() const;
    wxTimeSpan GetEstimatedTime() const;
    wxTimeSpan GetEstimatedRemainingTime() const;

    wxDateTime GetDownloadStartTime() const
        { return m_timeStart; }
    wxULongLong GetCurrentSize() const
        { return m_curr; }
    wxULongLong GetTotalSize() const
        { return m_total; }

    wxDateTime GetEventCreationTime() const
        { return m_timeEvent; }

    // default copy ctor, assignment operator and dtor are ok
    virtual wxEvent *Clone() const { return new wxDownloadEvent(*this); }

protected:

    wxULongLong m_curr, m_total;

    // wxEvent's timestamp is not so easy to use as a wxDateTime object
    wxDateTime m_timeEvent;

    // the start time of this download
    wxDateTime m_timeStart;

private:
    DECLARE_DYNAMIC_CLASS_NO_ASSIGN(wxDownloadEvent)
};

typedef void (wxEvtHandler::*wxDownloadEventFunction)(wxDownloadEvent&);

#define wxDownloadEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxDownloadEventFunction, &func)

#define EVT_DOWNLOAD_CONNECTED(id, fn) \
    wx__DECLARE_EVT1(wxEVT_COMMAND_DOWNLOAD_CONNECTED, id, wxDownloadEventHandler(fn))
#define EVT_DOWNLOAD_UPDATE(id, fn) \
    wx__DECLARE_EVT1(wxEVT_COMMAND_DOWNLOAD_UPDATE, id, wxDownloadEventHandler(fn))
#define EVT_DOWNLOAD_PAUSED(id, fn) \
    wx__DECLARE_EVT1(wxEVT_COMMAND_DOWNLOAD_PAUSED, id, wxDownloadEventHandler(fn))
#define EVT_DOWNLOAD_ABORTED(id, fn) \
    wx__DECLARE_EVT1(wxEVT_COMMAND_DOWNLOAD_ABORTED, id, wxDownloadEventHandler(fn))
#define EVT_DOWNLOAD_USER_ABORTED(id, fn) \
    wx__DECLARE_EVT1(wxEVT_COMMAND_DOWNLOAD_USER_ABORTED, id, wxDownloadEventHandler(fn))
#define EVT_DOWNLOAD_COMPLETE(id, fn) \
    wx__DECLARE_EVT1(wxEVT_COMMAND_DOWNLOAD_COMPLETE, id, wxDownloadEventHandler(fn))

#endif      // wxUSE_GUI

#endif // _WX_DOWNLOAD_H_

