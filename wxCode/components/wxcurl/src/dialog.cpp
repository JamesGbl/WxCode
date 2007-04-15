/////////////////////////////////////////////////////////////////////////////
// Name:        dialog.cpp
// Purpose:     wxCurlDownloadDialog, wxCurlUploadDialog
// Author:      Francesco Montorsi
// Created:     2007/04/14
// RCS-ID:      $Id$
// Copyright:   (c) 2007 Francesco Montorsi
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
        #include <wx/statline.h>
        #include <wx/statbmp.h>
    #endif
#endif

#include <wx/wfstream.h>
#include <wx/filename.h>
#include "wx/curl/dialog.h"


// ----------------------------------------------------------------------------
// wxCurlDownloadDialog
// ----------------------------------------------------------------------------

enum
{
    AbortButtonId,
    PauseResumeButtonId,
    StartButtonId,

    ThreadId
};

IMPLEMENT_DYNAMIC_CLASS( wxCurlDownloadDialog, wxDialog )
BEGIN_EVENT_TABLE( wxCurlDownloadDialog, wxDialog )

    // network events
    EVT_CURL_DOWNLOAD( ThreadId, wxCurlDownloadDialog::OnDownload )
    EVT_CURL_END_PERFORM( ThreadId, wxCurlDownloadDialog::OnEndPerform )

    // user events
    EVT_BUTTON( AbortButtonId, wxCurlDownloadDialog::OnAbort )
    EVT_BUTTON( PauseResumeButtonId, wxCurlDownloadDialog::OnPauseResume )
    EVT_BUTTON( StartButtonId, wxCurlDownloadDialog::OnStart )

    // update UI
    EVT_UPDATE_UI( AbortButtonId, wxCurlDownloadDialog::OnAbortUpdateUI )
    EVT_UPDATE_UI( PauseResumeButtonId, wxCurlDownloadDialog::OnPauseResumeUpdateUI )
    EVT_UPDATE_UI( StartButtonId, wxCurlDownloadDialog::OnStartUpdateUI )

END_EVENT_TABLE()

bool wxCurlDownloadDialog::Create(const wxString &url, wxOutputStream *out,
                                  const wxString& title, const wxString& message,
                                  const wxBitmap& bitmap,
                                  wxWindow * parent, int style)
{
    if (!wxDialog::Create(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize,
                          (wxDEFAULT_DIALOG_STYLE |wxRESIZE_BORDER) | style))
        return false;

    // some of these may remain NULL...
    m_pElapsedTime = m_pEstimatedTime = m_pRemainingTime = NULL;
    m_pSpeed = m_pSize = NULL;
    m_lastEvent = NULL;
    m_pBitmap = NULL;
    m_bDownloadComplete = false;

    // set up our controls
    CreateControls(message, bitmap);
    m_pURL->SetValue(url);

    // register as the thread's event handler
    m_thread.SetEvtHandler(this, ThreadId);
    m_thread.SetURL(url);
    m_thread.SetOutputStream(out);

    return true;
}

wxDownloadReturnFlag wxCurlDownloadDialog::StartModal()
{
    if (!HasFlag(wxDLD_CAN_START))
        m_thread.Download();        // start immediately

    CenterOnScreen();

    return (wxDownloadReturnFlag)wxDialog::ShowModal();
}

#define OUTER_BORDER    12
#define BORDER          5
#define MINWIDTH        300

wxStaticText *wxCurlDownloadDialog::AddSizerRow(wxSizer *sz, const wxString &name)
{
    // the static text
    wxStaticText *st = new wxStaticText( this, wxID_STATIC, name, wxDefaultPosition, wxDefaultSize );
    st->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, _T("")));

    // the value
    wxStaticText *ret = new wxStaticText( this, wxID_STATIC, _("Not available"),
                                          wxDefaultPosition, wxDefaultSize,
                                          wxALIGN_LEFT|wxST_NO_AUTORESIZE );

    wxBoxSizer* time = new wxBoxSizer(wxHORIZONTAL);
    time->Add(st, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT, 5);
    time->Add(ret, 1, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT, 5);

    sz->Add(time, 0, wxGROW|wxLEFT|wxRIGHT, OUTER_BORDER);

    return ret;
}

void wxCurlDownloadDialog::CreateControls(const wxString &msg, const wxBitmap &bitmap)
{
    wxBoxSizer* main = new wxBoxSizer(wxVERTICAL);

    // message row
    if (!msg.IsEmpty())
    {
        wxStaticText *st = new wxStaticText( this, wxID_STATIC, msg );
        st->SetMinSize(wxSize(MINWIDTH, -1));
        main->Add(st, 0, wxLEFT|wxTOP|wxRIGHT|wxBOTTOM|wxGROW, OUTER_BORDER);
    }

    // URL row
    if (HasFlag(wxDLD_URL))
    {
        wxBoxSizer* downloading = new wxBoxSizer(wxHORIZONTAL);

        wxStaticText *st = new wxStaticText( this, wxID_STATIC, _("URL:") );
        st->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, _T("")));
        downloading->Add(st, 0, wxRIGHT|wxTOP|wxALIGN_CENTER_VERTICAL, BORDER);

        m_pURL = new wxTextCtrl( this, wxID_STATIC, _("URL"), wxDefaultPosition,
                                wxSize(MINWIDTH, -1), wxTE_CENTRE|wxTE_READONLY|wxNO_BORDER );
        m_pURL->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));

        downloading->Add(m_pURL, 1, wxALIGN_CENTER_VERTICAL|wxTOP, BORDER);
        main->Add(downloading, 0, wxGROW|wxLEFT|wxRIGHT, OUTER_BORDER);
        main->AddSpacer(5);
    }


    wxSizer *leftcolumn = new wxBoxSizer(wxVERTICAL);

    // speed & size row
    if (HasFlag(wxDLD_SPEED))
        m_pSpeed = AddSizerRow(leftcolumn, _T("Speed:"));
    if (HasFlag(wxDLD_SIZE))
        m_pSize = AddSizerRow(leftcolumn, _T("Downloaded:"));

    // a spacer
    leftcolumn->AddSpacer(5);

    // the time rows
    if (HasFlag(wxDLD_ELAPSED_TIME))
        m_pElapsedTime = AddSizerRow(leftcolumn, _T("Elapsed time:"));
    if (HasFlag(wxDLD_ESTIMATED_TIME))
        m_pEstimatedTime = AddSizerRow(leftcolumn, _T("Estimated total time:"));
    if (HasFlag(wxDLD_REMAINING_TIME))
        m_pRemainingTime = AddSizerRow(leftcolumn, _T("Estimated remaining time:"));

    if (bitmap.IsOk())
    {
        wxSizer *rightcolumn = new wxBoxSizer(wxVERTICAL);
        rightcolumn->Add( new wxStaticBitmap(this, wxID_ANY, bitmap), 
                          0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, BORDER);

        wxSizer *both = new wxBoxSizer(wxHORIZONTAL);
        both->Add(leftcolumn);
        both->Add(rightcolumn, 1, wxGROW);
        main->Add(both, 0, wxGROW);
    }
    else
        main->Add(leftcolumn);


    // the gauge
    // VERY IMPORTANT: we set as range 101 so that when, because of some approximation,
    //                 the update event handler will SetValue(100), while the transfer
    //                 is not yet complete, the gauge will not appear completely filled.
    m_pGauge = new wxGauge( this, wxID_ANY, 101 );

    main->AddSpacer(5);
    main->Add(m_pGauge, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, OUTER_BORDER);

    // an horizontal line
    main->AddStretchSpacer(1);
    main->AddSpacer(BORDER*2);
    main->Add( new wxStaticLine(this), 0, wxGROW|wxLEFT|wxRIGHT, OUTER_BORDER);

    // the button row
    wxBoxSizer *btn = new wxBoxSizer(wxHORIZONTAL);

    if (HasFlag(wxDLD_CAN_ABORT))
        btn->Add(new wxButton( this, AbortButtonId, _("Abort") ), 0);

    btn->AddStretchSpacer(1);

    if (HasFlag(wxDLD_CAN_PAUSE))
        btn->Add(new wxButton( this, PauseResumeButtonId, _("Pause") ), 0);
    if (HasFlag(wxDLD_CAN_START))
        btn->Add(new wxButton( this, StartButtonId, _("Start") ), 0, wxLEFT, BORDER);

    main->Add(btn, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP|wxBOTTOM, OUTER_BORDER);

    this->SetSizerAndFit(main);
    main->SetSizeHints(this);
}

void wxCurlDownloadDialog::EndModal(wxDownloadReturnFlag retCode)
{
    wxDialog::EndModal(retCode);

    // before dying we must be sure our thread has completed, too
    // otherwise it will try to send events to a non-existent handler
    // NB: this must be done *after* calling wxDialog::EndModal
    //     so that while we wait we are hidden
    if (m_thread.IsAlive())
        m_thread.Wait();
#ifdef __WXDEBUG__
    else
        wxASSERT(HasFlag(wxDLD_CAN_START));
            // thread is not alive: means the user has not
            // clicked on Start button yet
#endif
}

void wxCurlDownloadDialog::ForceUpdate(wxCurlDownloadEvent &ev)
{
    double fraction = ev.GetPercent();
    if (fraction != 0)
    {
        m_pGauge->SetValue((int)fraction);
    }
    else
    {
        // we don't know how much we progressed...
        m_pGauge->Pulse();
    }

    if (m_pElapsedTime)
        m_pElapsedTime->SetLabel(ev.GetElapsedTime().Format());
    if (m_pRemainingTime)
        m_pRemainingTime->SetLabel(ev.GetEstimatedRemainingTime().Format());
    if (m_pEstimatedTime)
        m_pEstimatedTime->SetLabel(ev.GetEstimatedTime().Format());

    if (m_pSize)
    {
        wxString currsize = ev.GetHumanReadableDownloadedBytes(),
                totalsize = ev.GetHumanReadableTotalBytes();
        m_pSize->SetLabel(
            wxString::Format(wxT("%s / %s  (%0.1f%%)"), 
                                currsize.c_str(), totalsize.c_str(), ev.GetPercent()));
    }

    if (m_pSpeed)
        m_pSpeed->SetLabel(ev.GetHumanReadableSpeed());
}


// ----------------------------------------------------------------------------
// wxCurlDownloadDialog - button events
// ----------------------------------------------------------------------------

void wxCurlDownloadDialog::OnAbort(wxCommandEvent &WXUNUSED(ev))
{
    wxASSERT(HasFlag(wxDLD_CAN_ABORT));

    if (m_thread.IsAlive())
    {
        m_thread.Abort();
        EndModal(wxDRF_USER_ABORTED);
    }
    else
    {
        wxASSERT(HasFlag(wxDLD_CAN_START) || !HasFlag(wxDLD_AUTO_CLOSE));
            // thread is not alive: means the user has not
            // clicked on Start button yet or the download is complete
            // and the dialog does not auto close

        EndModal(m_bDownloadComplete ? wxDRF_SUCCESS : wxDRF_USER_ABORTED);
    }
}

void wxCurlDownloadDialog::OnAbortUpdateUI(wxUpdateUIEvent &ev)
{
    ev.SetText(m_thread.IsAlive() ? wxT("Abort") : wxT("Close"));
}

void wxCurlDownloadDialog::OnPauseResume(wxCommandEvent &WXUNUSED(ev))
{
    wxASSERT(HasFlag(wxDLD_CAN_PAUSE));

    if (m_thread.IsRunning())
    {
        m_thread.Pause();
        FindWindowById(PauseResumeButtonId)->SetLabel(wxT("Resume"));
    }
    else
    {
        m_thread.Resume();
        FindWindowById(PauseResumeButtonId)->SetLabel(wxT("Pause"));
    }
}

void wxCurlDownloadDialog::OnPauseResumeUpdateUI(wxUpdateUIEvent &ev)
{
    ev.Enable(m_thread.IsAlive());
}

void wxCurlDownloadDialog::OnStart(wxCommandEvent &WXUNUSED(ev))
{
    wxASSERT(HasFlag(wxDLD_CAN_START));

    m_thread.Download();
}

void wxCurlDownloadDialog::OnStartUpdateUI(wxUpdateUIEvent &ev)
{
    ev.Enable(!m_thread.IsAlive() && !m_bDownloadComplete);
}



// ----------------------------------------------------------------------------
// wxCurlDownloadDialog - network events
// ----------------------------------------------------------------------------

void wxCurlDownloadDialog::OnEndPerform(wxCurlEndPerformEvent &ev)
{
    // in case the very last download update event was skipped because
    // of our anti-flickering label update policy, force the update with
    // that event now (otherwise the dialog may remain open showing data
    // related not to the end of the download!)
    ForceUpdate(*m_lastEvent);

    // now we're sure the transfer has completed: make the gauge
    // completely "filled"
    m_pGauge->SetValue(101);

    // this flag is used for updating labels etc:
    m_bDownloadComplete = true;

    // transfer has completed
    if (HasFlag(wxDLD_AUTO_CLOSE))
        EndModal(ev.IsSuccessful() ? wxDRF_SUCCESS : wxDRF_FAILED);
}

void wxCurlDownloadDialog::OnDownload(wxCurlDownloadEvent &ev)
{
    static wxDateTime lastLabelUpdate(0, 0, 0, 0);      // zero is to force always at least an update
    if ((wxDateTime::Now() - lastLabelUpdate).GetMilliseconds() > 200)   // avoid flickering
    {
        ForceUpdate(ev);

        lastLabelUpdate = wxDateTime::Now();
    }

    // see OnEndPerform for more info.
    if (m_lastEvent)
        delete m_lastEvent;
    m_lastEvent = wx_static_cast(wxCurlDownloadEvent*, ev.Clone());
}

