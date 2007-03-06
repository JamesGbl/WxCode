/////////////////////////////////////////////////////////////////////////////
// Name:        download.cpp
// Purpose:     wxDownloadThread, wxSizeCacherThread, wxDownloadDialog
// Author:      Francesco Montorsi
// Created:     2005/06/23
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#ifndef WX_PRECOMP
    #include <wx/log.h>
    #include <wx/intl.h>        // for _() support
    #include <wx/utils.h>       // for wxMilliSleep

    #if wxUSE_GUI
        #include <wx/textctrl.h>
        #include <wx/gauge.h>
        #include <wx/stattext.h>
        #include <wx/sizer.h>
        #include <wx/msgdlg.h>
        #include <wx/settings.h>
        #include <wx/button.h>
    #endif
#endif

#include <wx/wfstream.h>
#include <wx/filename.h>
#include "wx/download.h"



// ---------------------
// wxDownloadThread
// ---------------------

wxDownloadThreadFlag wxDownloadThread::GetFlag() const
{
    if (m_pFlag)
        return *m_pFlag;
    return wxDTF_CONTINUE;
}

void wxDownloadThread::PostEvent(WXTYPE eventType)
{
#if wxUSE_GUI
    if (!m_pHandler)
        return;

    wxDownloadEvent event(eventType, m_nId, m_dtStart,
                          m_url.GetURL(), m_nCurrentSize, m_nFinalSize);
    wxPostEvent(m_pHandler, event);
#else
    // in console mode (wxUSE_GUI == 0) events have no sense
    wxUnusedVar(eventType);
#endif
}

wxThreadError wxDownloadThread::Download(const wxString &url, wxOutputStream *out)
{
    wxThreadError ret;

    // set url
    m_url.SetURL(url);
    wxASSERT_MSG(m_url.IsOk(), wxT("Invalid URL!"));

    // set a lower timeout than 10 minutes default!
    m_url.GetProtocol().SetTimeout(30); // 30 secs

    // set output stream
    m_output = out;
    if (!m_output)
    {
        // try to create a temporary file output stream
        m_output = new wxFileOutputStream(wxFileName::CreateTempFileName(wxT("download")));
        if (!m_output || !m_output->IsOk())
            return wxTHREAD_NO_RESOURCE;
    }
    else
    {
        // the user-provided output stream must be valid
        wxASSERT_MSG(m_output->IsOk(), wxT("Invalid output stream!"));
    }

    // create & run this thread
    if ((ret=Create(wxDT_BUF_TEMP_SIZE)) != wxTHREAD_NO_ERROR)
        return ret;
    if ((ret=Run()) != wxTHREAD_NO_ERROR)
        return ret;

    return wxTHREAD_NO_ERROR;
}

void *wxDownloadThread::Entry()
{
    wxInputStream *in = m_url.GetInputStream();

    // check the stream
    if (in == NULL)
    {
        // something is wrong with the input URL...
        OnAbort();
        return NULL;
    }
    if (!in->IsOk())
    {
        delete in;

        // something is wrong with the input URL...
        OnAbort();
        return NULL;
    }

    // we successfully connected with the server
    OnConnect();

    // we are starting the download of a file; update our datetime field
    wxLogDebug(_("wxDownloadThread::Entry - downloading [%s]"),
               m_url.GetURL().c_str());
    m_dtStart = wxDateTime::UNow();

    wxASSERT(m_output->IsOk());

    // get size of download, if available
    size_t sz = in->GetSize();
    m_nFinalSize = (sz == (size_t)-1) ? wxInvalidSize : sz;

    // begin the download
    char buf[wxDT_BUF_TEMP_SIZE];
    bool paused = false;
    while (!TestDestroy())
    {
        if (GetFlag() == wxDTF_ABORT)
        {
            wxLogDebug(wxT("wxDownloadThread::Entry - user-aborting"));
            delete in;
            OnUserAbort();
            return NULL;
        }
        else if (GetFlag() == wxDTF_PAUSE)
        {
            wxLogDebug(wxT("wxDownloadThread::Entry - sleeping"));

            // did we warn our event handler that the download was paused?
            if (!paused)
            {
                paused = true;
                OnPause();
            }

            // sleep 100 msec and then test again our flag to see
            // if it has changed to wxDTF_CONTINUE or to wxDTF_ABORT
            wxMilliSleep(100);
            continue;
        }

        paused = false;

        // write the downloaded stuff in the output file
        // without using the
        //      out.Write(*in);
        // command; that would be easier but would not allow
        // the program to stop this thread while downloading
        // the file since the TestDestroy() function would not
        // be called in that way...
        size_t bytes_read = in->Read(buf, WXSIZEOF(buf)).LastRead();
        if ( !bytes_read )
            break;      // no more data to read

        if ( m_output->Write(buf, bytes_read).LastWrite() != bytes_read )
        {
            // something wrong with saving downloaded data!
            OnAbort();
            return NULL;
        }

        // update our downloaded bytes var
        m_nCurrentSize = m_output->GetSize();

        // notify our even handler that we made progress
        OnUpdate();
    }

    // we don't need the INPUT stream anymore...
    delete in;

    wxASSERT_MSG(m_nCurrentSize == m_nFinalSize || m_nFinalSize == wxInvalidSize,
                 wxT("All errors should have already been catched!"));

    wxLogDebug(_("wxDownloadThread::Entry - completed download of %lu bytes"),
               m_nCurrentSize.ToULong());

    // download is complete
    OnComplete();
    wxLogDebug(wxT("sent complete event"));
    return NULL;
}

void wxDownloadThread::OnExit()
{
    wxLogDebug(wxT("wxDownloadThread - exiting"));
}


#if wxUSE_GUI

// ----------------------------------------------------------------------------
// wxDownloadDialog
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS( wxDownloadDialog, wxDialog )
BEGIN_EVENT_TABLE( wxDownloadDialog, wxDialog )

    // network events
    EVT_DOWNLOAD_UPDATE( wxID_ANY, wxDownloadDialog::OnDownloadUpdate )
    EVT_DOWNLOAD_COMPLETE( wxID_ANY, wxDownloadDialog::OnDownloadComplete )
    EVT_DOWNLOAD_ABORTED( wxID_ANY, wxDownloadDialog::OnDownloadAbort )
    EVT_DOWNLOAD_USER_ABORTED( wxID_ANY, wxDownloadDialog::OnDownloadUserAbort )

    // user events
    EVT_BUTTON( wxID_CANCEL, wxDownloadDialog::OnCancel )
    EVT_IDLE( wxDownloadDialog::OnIdle )

END_EVENT_TABLE()

bool wxDownloadDialog::Create(const wxString &url, wxOutputStream *out,
                              const wxString& title, const wxString& message,
                              wxWindow * parent, int style)
{
    if (!wxDialog::Create(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize,
                          (wxDEFAULT_DIALOG_STYLE & ~wxRESIZE_BORDER) | style))
        return false;

    CreateControls(message);
    m_pURL->SetValue(url);
    CenterOnScreen();

    m_flag = wxDTF_CONTINUE;
    m_bDisableEvents = false;
    m_bDelayEndModal = false;
    m_bConnecting = true;

    // run the thread which does the download
    m_bThreadExist = true;
    new wxDownloadThread(this, wxID_ANY, &m_flag, url, out);

    return true;
}

void wxDownloadDialog::EndModal(wxDownloadReturnFlag retCode)
{
    wxDialog::EndModal(retCode);

    // before dying we must be sure our thread has completed, too
    // otherwise it will try to access an invalid m_flag!
    // To do this we wait until one of our event handlers sets
    // the m_bThreadExist flag to false.
    // NB: this must be done *after* calling wxDialog::EndModal
    //     so that while we wait we are hidden
    while (m_bThreadExist)
    {
        // process events
        wxYield();
    }
}

wxStaticText *wxDownloadDialog::AddTimeSizer(wxSizer *sz, const wxString &name)
{
    wxBoxSizer* time = new wxBoxSizer(wxHORIZONTAL);

    // the explicit size is to make all labels with the same width
    time->Add(new wxStaticText( this, wxID_STATIC, name, wxDefaultPosition, wxSize(100,-1) ),
              0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText *ret = new wxStaticText( this, wxID_STATIC, _("Not available"),
                                          wxDefaultPosition, wxDefaultSize,
                                          wxALIGN_CENTRE|wxST_NO_AUTORESIZE );
    time->Add(ret, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    sz->Add(time, 0, wxGROW|wxLEFT, 5);

    return ret;
}

void wxDownloadDialog::CreateControls(const wxString &msg)
{
    wxBoxSizer* main = new wxBoxSizer(wxVERTICAL);

    // message row
    if (!msg.IsEmpty())
        main->Add(new wxStaticText( this, wxID_STATIC, msg ), 0, wxALL, 5);

    // downloading row
    wxBoxSizer* downloading = new wxBoxSizer(wxHORIZONTAL);
    downloading->Add(new wxStaticText( this, wxID_STATIC, _("URL:") ),
                     0, wxALL, 5);
    m_pURL = new wxTextCtrl( this, wxID_STATIC, _("URL"), wxDefaultPosition,
                             wxSize(300, -1), wxTE_CENTRE|wxTE_READONLY|wxNO_BORDER );
    m_pURL->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
    m_pURL->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, _T("")));
    downloading->Add(m_pURL, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    main->Add(downloading, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    // speed & size row
    wxBoxSizer* speedsize = new wxBoxSizer(wxHORIZONTAL);
    speedsize->Add(new wxStaticText( this, wxID_STATIC, _("Speed:") ),
                   0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);
    m_pSpeed = new wxStaticText( this, wxID_STATIC, _("Not available") );
    speedsize->Add(m_pSpeed, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    speedsize->Add(new wxStaticText( this, wxID_STATIC, _("Size:") ),
                   0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);
    m_pSize = new wxStaticText( this, wxID_STATIC, _("Not available") );
    speedsize->Add(m_pSize, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    main->Add(speedsize, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    // a spacer
    main->Add(5, 5, 0);

    // the time rows
    m_pElapsedTime = m_pEstimatedTime = m_pRemainingTime = NULL;

    if (HasFlag(wxDLD_ELAPSED_TIME))
        m_pElapsedTime = AddTimeSizer(main, _T("Elapsed time:"));
    if (HasFlag(wxDLD_ESTIMATED_TIME))
        m_pEstimatedTime = AddTimeSizer(main, _T("Estimated time:"));
    if (HasFlag(wxDLD_REMAINING_TIME))
        m_pRemainingTime = AddTimeSizer(main, _T("Remaining time:"));

    // the gauge
    m_pGauge = new wxGauge( this, wxID_ANY, 101 );

    main->Add(5, 5, 0);
    main->Add(m_pGauge, 0, wxGROW|wxALL, 5);
    main->Add(5, 5, 0);

    // the abort button
    if (HasFlag(wxDLD_CAN_ABORT))
        main->Add(new wxButton( this, wxID_CANCEL, _("Abort") ),
                  0, wxALIGN_RIGHT|wxALL, 5);

    this->SetSizer(main);
    main->SetSizeHints(this);
}


// ----------------------------------------------------------------------------
// wxDownloadDialog - event handlers
// ----------------------------------------------------------------------------

void wxDownloadDialog::OnDownloadAbort(wxDownloadEvent &WXUNUSED(ev))
{
    m_exitCode = wxDRF_FAILED;      // network error
    m_bThreadExist = false;

    // if we are waiting for the user's reply in OnDownloadUpdate()
    // do not call EndModal() even if meanwhile the download aborted
    // for other reasons
    if (m_bDisableEvents)
        m_bDelayEndModal = true;
    else
        EndModal(m_exitCode);
}

void wxDownloadDialog::OnDownloadComplete(wxDownloadEvent &WXUNUSED(ev))
{
    m_exitCode = wxDRF_SUCCESS;
    m_bThreadExist = false;

    // if we are waiting for the user's reply in OnDownloadUpdate()
    // do not call EndModal() even if meanwhile the download aborted
    // for other reasons
    if (m_bDisableEvents)
        m_bDelayEndModal = true;
    else
        EndModal(m_exitCode);
}

void wxDownloadDialog::OnDownloadUserAbort(wxDownloadEvent &WXUNUSED(ev))
{
    m_bThreadExist = false;

    // when this event handler is called, we are already inside EndModal()
    // waiting for the thread to exit after setting m_flag to wxDTF_ABORT.
    // Thus we don't need to call EndModal() here
}

void wxDownloadDialog::OnDownloadUpdate(wxDownloadEvent &ev)
{
    // the input stream has been created - now we've started to download data
    m_bConnecting = false;

    if (m_bDisableEvents)
        return;

    static wxDateTime lastLabelUpdate = wxDateTime::Now();
    wxLogDebug(wxT("wxDownloadDialog::OnDownloadUpdate"));

    if ((wxDateTime::Now() - lastLabelUpdate).GetMilliseconds() > 200)   // avoid flickering
    {
        double fraction = ev.GetDownloadProgress();
        if (fraction != 0)
        {
            m_pGauge->SetValue((int)(fraction * 100));
        }
        else
        {
            // we don't know how much we progressed
            m_pGauge->Pulse();
        }

        if (m_pElapsedTime)
            m_pElapsedTime->SetLabel(ev.GetElapsedTime().Format());
        if (m_pRemainingTime)
            m_pRemainingTime->SetLabel(ev.GetEstimatedRemainingTime().Format());
        if (m_pEstimatedTime)
            m_pEstimatedTime->SetLabel(ev.GetEstimatedTime().Format());

        wxString currsize = wxFileName::GetHumanReadableSize(ev.GetCurrentSize()),
                 totalsize = wxFileName::GetHumanReadableSize(ev.GetTotalSize());
        m_pSize->SetLabel(currsize + wxT(" / ") + totalsize);
        m_pSpeed->SetLabel(ev.GetHumanReadableDownloadSpeed());

        lastLabelUpdate = wxDateTime::Now();
    }

    // we have to yield because not only we want to update the display but
    // also to process the clicks on the cancel and skip buttons
    wxYieldIfNeeded();
}

void wxDownloadDialog::OnIdle(wxIdleEvent &ev)
{
    if (m_bDisableEvents || !m_bConnecting)
        return;

    static wxDateTime lastLabelUpdate = wxDateTime::Now();
    if ((wxDateTime::Now() - lastLabelUpdate).GetMilliseconds() > 200)   // avoid flickering
    {
        // we don't know how much we progressed
        m_pGauge->Pulse();

        lastLabelUpdate = wxDateTime::Now();
    }

    // we have to yield because not only we want to update the display but
    // also to process the clicks on the cancel and skip buttons
    wxYieldIfNeeded();
}

void wxDownloadDialog::OnCancel(wxCommandEvent &)
{
    // the user pressed the "cancel" button
    m_flag = wxDTF_PAUSE;
    m_bDisableEvents = true;
    int reply = wxMessageBox(wxT("Are you sure you want to cancel the download?"),
                                wxT("Question"), wxYES_NO|wxICON_QUESTION);
    if (reply == wxYES)
    {
        m_flag = wxDTF_ABORT;
        EndModal(wxDRF_USER_ABORTED);
    }
    else
    {
        m_flag = wxDTF_CONTINUE;

        // while we waited for user's input, a network event could have occurred:
        // e.g. the download may have been completed or aborted (not by the user
        // but because e.g. network failure). In that case our m_bDelayEndModal
        // will have been set to true by one of our event handlers...
        if (m_bDelayEndModal)
        {
            // we can immediately exit as the thread is not running anymore here
            // since the download has been completed or aborted (not by the user)
            EndModal(m_exitCode);
        }
        else
            m_bDisableEvents = false;
    }
}


// ----------------------------------------------------------------------------
// wxDownloadEvent
// ----------------------------------------------------------------------------

IMPLEMENT_CLASS(wxDownloadEvent, wxCommandEvent)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_DOWNLOAD_CONNECTED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_DOWNLOAD_UPDATE)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_DOWNLOAD_ABORTED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_DOWNLOAD_USER_ABORTED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_DOWNLOAD_PAUSED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_DOWNLOAD_COMPLETE)

wxULongLong wxDownloadEvent::GetDownloadSpeed() const
{
    wxTimeSpan sec = GetElapsedTime();
    if (sec.GetSeconds() == 0)
        return 0;       // avoid division by zero

    // returned value is in bytes per second
    return (wxULongLong_t)(m_curr.ToDouble() / sec.GetSeconds().ToDouble());
}

wxString wxDownloadEvent::GetHumanReadableDownloadSpeed() const
{
    wxULongLong speed = GetDownloadSpeed();
    if (speed == 0)
        return _("Not available");

    return wxFileName::GetHumanReadableSize(speed) + wxT("/s");
}

wxTimeSpan wxDownloadEvent::GetElapsedTime() const
{
    return m_timeEvent - m_timeStart;
}

wxTimeSpan wxDownloadEvent::GetEstimatedTime() const
{
    wxULongLong nBytesPerSec = GetDownloadSpeed();
    if (nBytesPerSec == 0)
        return 0;       // avoid division by zero

    // compute remaining seconds; here we assume that the current
    // download speed will be constant also in future
    double secs = m_total.ToDouble() / nBytesPerSec.ToDouble();

    return wxTimeSpan(0,        // hours
                      0,        // minutes
                      (wxLongLong_t)secs,     // seconds
                      0);       // milliseconds
}

wxTimeSpan wxDownloadEvent::GetEstimatedRemainingTime() const
{
    return GetEstimatedTime() - GetElapsedTime();
}

double wxDownloadEvent::GetDownloadProgress() const
{
    if (m_total == 0 || m_curr == 0)
        return 0.0;

    // VERY IMPORTANT: the ToDouble() function must be called *before*
    //                 doing the division!
    return m_curr.ToDouble() / m_total.ToDouble();
}

#endif      // wxUSE_GUI


// ---------------------
// wxSIZECACHERTHREAD
// ---------------------

DEFINE_EVENT_TYPE(wxEVT_COMMAND_CACHESIZE_COMPLETE)

void *wxSizeCacherThread::Entry()
{
    wxLogDebug(wxT("wxSizeCacherThread::Entry - caching file sizes"));
    bool allok = TRUE;

    if (m_urls.GetCount() == 0)
        return (void*)FALSE;     // no urls whose size must be cached ?

    // be sure to have n null entries in our cache array, where
    // 'n' is the number of URLs whose size must be cached
    m_urlSizes = new wxArrayLong();
    m_urlSizes->Add((long)0, m_urls.GetCount());

    // begin our loop
    for (int i=0; i<(int)m_urls.GetCount() && !TestDestroy(); i++) {

        // get the input stream
        wxURL u(m_urls[i]);
        wxInputStream *is = u.GetInputStream();
        if (is == NULL) {
            wxLogDebug(wxT("wxGetSizeOfURI - aborting; invalid URL !"));
            return 0;
        }

        if (!is->IsOk()) {
            wxLogDebug(wxT("wxGetSizeOfURI - aborting; invalid URL !"));
            delete is;          // be sure to avoid leaks
            return 0;
        }

        size_t sz = is->GetSize();
        delete is;

        // see wxHTTP::GetInputStream docs
        if (sz == (size_t)-1)
            sz = 0;

        // getting the input stream for the url is the only way
        // to get the size of the resource pointed by that url...
        m_urlSizes->Item(i) = sz;
        allok &= (m_urlSizes->Item(i) != 0);
    }

    wxLogDebug(wxT("wxSizeCacherThread::Entry - caching of file sizes completed"));
    return (void *)allok;
}

void wxSizeCacherThread::OnExit()
{
#if wxUSE_GUI
    // we'll use wxPostEvent to post this event since this is the
    // only thread-safe way to post events !
    wxCommandEvent updatevent(wxEVT_COMMAND_CACHESIZE_COMPLETE);

    // the event handler must delete the wxArrayLong which we pass to it in the event !
    updatevent.SetClientData(m_urlSizes);
    if (m_pHandler)
		wxPostEvent(m_pHandler, updatevent);
#endif
}

