/////////////////////////////////////////////////////////////////////////////
// Name:        vcapdlgs.cpp, generic dialogs for wxVideoCaptureWindow
// Author:      John Labenski
// Created:     07/06/01
// Modified:    01/14/03
// Copyright:   John Labenski
// License:     wxWidgets V2.0
/////////////////////////////////////////////////////////////////////////////

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "precomp.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "wx/vidcap/vcapwin.h"
#include "vcapdlgs.h"
#include <wx/spinctrl.h>

#ifdef WXVIDCAP_AVI_SUPPORT
// --------------------------------------------------------------------------
// wxVideoCaptureWindowCaptureSingleFramesDialog
//      NEVER call this, use wxVideoCaptureWindow::CaptureSingleFramesToFileDialog
// --------------------------------------------------------------------------

#define IDD_wxVIDCAP_CAPSNGFRAMESDLG_CAPTURE    100
#define IDD_wxVIDCAP_CAPSNGFRAMESDLG_DONE       101

BEGIN_EVENT_TABLE(wxVideoCaptureWindowCaptureSingleFramesDialog, wxDialog)
    EVT_BUTTON( IDD_wxVIDCAP_CAPSNGFRAMESDLG_CAPTURE, wxVideoCaptureWindowCaptureSingleFramesDialog::OnCapture )
    EVT_BUTTON( IDD_wxVIDCAP_CAPSNGFRAMESDLG_DONE, wxVideoCaptureWindowCaptureSingleFramesDialog::OnDone )
    EVT_CLOSE(wxVideoCaptureWindowCaptureSingleFramesDialog::OnCloseDialog)
END_EVENT_TABLE()

wxVideoCaptureWindowCaptureSingleFramesDialog::wxVideoCaptureWindowCaptureSingleFramesDialog(
                        wxVideoCaptureWindowVFW *parent, wxWindowID id, const wxString &title,
                        const wxPoint &position, const wxSize &size, long style)
                        :wxDialog( parent, id, title, position, size, style )
{
    wxASSERT(parent);
    m_parent = parent;

    // this shouldn't be run on its own, create wxVideoCaptureWindow
    //  only called using wxVideoCaptureWindow::AudioFormatDialog
/*
    if (!m_parent->m_hWndC)
    {
        wxStaticText errormsg(this, -1, wxT("This is WRONG!"));
        Show(true);
        return;
    }
*/

    bool ok = m_parent->CaptureSingleFramesToFileOpen();
    m_fileName = m_parent->GetCaptureFilename();

    if (ok)
        m_captureString = wxT("Begin recording frames to ") + m_fileName;
    else
        m_captureString = wxT("Error initiating capture");

    wxBoxSizer *mainsizer = new wxBoxSizer( wxVERTICAL );

    m_captureText = new wxStaticText( this, -1, m_captureString, wxDefaultPosition, wxDefaultSize, 0 );
    mainsizer->Add( m_captureText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxSizer *buttonsizer = new wxBoxSizer( wxHORIZONTAL );

    m_captureButton = new wxButton( this, IDD_wxVIDCAP_CAPSNGFRAMESDLG_CAPTURE, wxT("Capture"), wxDefaultPosition, wxSize(60,-1));
    m_captureButton->SetDefault();
    buttonsizer->Add( m_captureButton, 0, wxALIGN_CENTRE|wxALL, 5 );

    if (!ok) m_captureButton->Enable(false);

    m_doneButton = new wxButton( this, IDD_wxVIDCAP_CAPSNGFRAMESDLG_DONE, wxT("Done"), wxDefaultPosition, wxSize(60,-1));
    buttonsizer->Add( m_doneButton, 0, wxALIGN_CENTRE|wxALL, 5 );

    mainsizer->Add( buttonsizer, 0, wxALIGN_CENTRE|wxALL, 5 );

    SetAutoLayout( true );
    SetSizer( mainsizer );
    mainsizer->Fit( this );
    mainsizer->SetSizeHints( this );
    Center();
    Show(true);
}

void wxVideoCaptureWindowCaptureSingleFramesDialog::OnCloseDialog(wxCloseEvent &event)
{
    if (m_parent->GetCapturedFramesToFileCount() != -1)
        m_parent->CaptureSingleFramesToFileClose();
    event.Skip();
}

void wxVideoCaptureWindowCaptureSingleFramesDialog::OnCapture(wxCommandEvent &)
{
    bool ok = m_parent->CaptureSingleFramesToFile();

    if (ok)
    {
        m_captureString.Printf(wxT("%d frames saved to %s"),
                    m_parent->GetCapturedFramesToFileCount(), m_fileName.c_str());
    }
    else
    {
        m_captureString = wxT("Error recording frame to ") + m_fileName;
        m_captureButton->Enable(false);
    }

    m_captureText->SetLabel(m_captureString);
}

void wxVideoCaptureWindowCaptureSingleFramesDialog::OnDone(wxCommandEvent &)
{
    Show(false);
    if (m_parent->GetCapturedFramesToFileCount() != -1)
        m_parent->CaptureSingleFramesToFileClose();
    Destroy();
}

// --------------------------------------------------------------------------
// wxVideoCaptureWindowCapturePreferencesDialog
//      NEVER call this, use wxVideoCaptureWindow::CapturePreferencesDialog()
// --------------------------------------------------------------------------
#define IDD_wxVIDCAP_CAPPREFDLG_MSFRAME_SPIN    200
#define IDD_wxVIDCAP_CAPPREFDLG_APPLY           201
#define IDD_wxVIDCAP_CAPPREFDLG_OK              202
#define IDD_wxVIDCAP_CAPPREFDLG_CANCEL          203

BEGIN_EVENT_TABLE(wxVideoCaptureWindowCapturePreferencesDialog, wxDialog)
    EVT_SPINCTRL( IDD_wxVIDCAP_CAPPREFDLG_MSFRAME_SPIN, wxVideoCaptureWindowCapturePreferencesDialog::OnMSFrameSpin )
    EVT_BUTTON( IDD_wxVIDCAP_CAPPREFDLG_APPLY,  wxVideoCaptureWindowCapturePreferencesDialog::OnApply )
    EVT_BUTTON( IDD_wxVIDCAP_CAPPREFDLG_OK,     wxVideoCaptureWindowCapturePreferencesDialog::OnOk )
    EVT_BUTTON( IDD_wxVIDCAP_CAPPREFDLG_CANCEL, wxVideoCaptureWindowCapturePreferencesDialog::OnCancel )
END_EVENT_TABLE()

wxVideoCaptureWindowCapturePreferencesDialog::wxVideoCaptureWindowCapturePreferencesDialog(
                                        wxVideoCaptureWindowVFW *parent, wxWindowID id,
                                        const wxString &title,
                                        const wxPoint &position,
                                        const wxSize &size, long style)
                                        :wxDialog( parent, id, title, position, size, style )
{
    m_parent = parent;

    // this shouldn't be run on its own, create wxVideoCaptureWindow
    //  only called using wxVideoCaptureWindow::CapturePreferencesDialog
    if (!m_parent->IsDeviceConnected()) //!m_parent->m_hWndC ||
    {
        wxStaticText errormsg(this, -1, wxT("This is WRONG!"));
        Show(true);
        return;
    }

    wxBoxSizer *mainsizer = new wxBoxSizer( wxVERTICAL );

    // ---------- Capture Settings -------------------------------------------
    wxStaticBox *capture_staticbox = new wxStaticBox( this, -1, wxT("Capture settings") );
    wxStaticBoxSizer *capture_staticboxsizer = new wxStaticBoxSizer( capture_staticbox, wxVERTICAL );

    wxFlexGridSizer *capture_flexgridsizer = new wxFlexGridSizer( 2, 0, 0 );

    m_msframeSpinCtrl = new wxSpinCtrl( this, IDD_wxVIDCAP_CAPPREFDLG_MSFRAME_SPIN, wxT("0"), wxDefaultPosition, wxSize(100,-1), 0, 0, 32000, 0 );
    capture_flexgridsizer->Add( m_msframeSpinCtrl, 0, wxALIGN_CENTER_VERTICAL, 0 );

    m_msframeText = new wxStaticText( this, -1, wxT("ms/frame = 0 fps"), wxDefaultPosition, wxDefaultSize, 0 );
    m_msframeText->SetToolTip( wxT("Number of milliseconds between recorded frames") );
    capture_flexgridsizer->Add( m_msframeText, 0, wxALIGN_CENTER_VERTICAL, 0 );

    m_dropSpinCtrl = new wxSpinCtrl( this, -1, wxT("0"), wxDefaultPosition, wxSize(100,-1), 0, 0, 100, 0 );
    capture_flexgridsizer->Add( m_dropSpinCtrl, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5 );

    wxStaticText *drop_text = new wxStaticText( this, -1, wxT("% dropped frame warning"), wxDefaultPosition, wxDefaultSize, 0 );
    drop_text->SetToolTip( wxT("% allowed frames dropped before error message") );
    capture_flexgridsizer->Add( drop_text, 0, wxALIGN_CENTER_VERTICAL, 0 );

    m_timelimitCheckBox = new wxCheckBox( this, -1, wxT("Time limit"), wxDefaultPosition, wxDefaultSize, 0 );
    m_timelimitCheckBox->SetToolTip( wxT("Capture for preset # of seconds") );
    capture_flexgridsizer->Add( m_timelimitCheckBox, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_timelimitSpinCtrl = new wxSpinCtrl( this, -1, wxT("0"), wxDefaultPosition, wxSize(100,-1), 0, 0, 32000, 0 );
    m_timelimitSpinCtrl->SetToolTip( wxT("Number of seconds to capture for") );
    capture_flexgridsizer->Add( m_timelimitSpinCtrl, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5 );

    m_recordaudioCheckBox = new wxCheckBox( this, -1, wxT("Record Audio"), wxDefaultPosition, wxDefaultSize, 0 );
    capture_flexgridsizer->Add( m_recordaudioCheckBox, 0, wxALIGN_CENTER_VERTICAL, 0 );

    m_audiomasterCheckBox = new wxCheckBox( this, -1, wxT("Audio Master"), wxDefaultPosition, wxDefaultSize, 0 );
    m_audiomasterCheckBox->SetToolTip( wxT("Sync video to audio stream") );
    capture_flexgridsizer->Add( m_audiomasterCheckBox, 0, wxALIGN_CENTER_VERTICAL, 0 );

    capture_staticboxsizer->Add( capture_flexgridsizer, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    mainsizer->Add( capture_staticboxsizer, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    // ---------- Init / Abort Settings --------------------------------------
    wxStaticBox *initabort_staticbox = new wxStaticBox( this, -1, wxT("Initiate / Abort options") );
    wxStaticBoxSizer *initabort_staticboxsizer = new wxStaticBoxSizer( initabort_staticbox, wxVERTICAL );

    wxFlexGridSizer *initabort_flexgridsizer = new wxFlexGridSizer( 2, 0, 0 );

    m_dialogtocaptureCheckBox = new wxCheckBox( this, -1, wxT("Dialog initiated capture"), wxDefaultPosition, wxDefaultSize, 0 );
    initabort_flexgridsizer->Add( m_dialogtocaptureCheckBox, 0, wxALIGN_CENTER_VERTICAL, 0 );

    initabort_flexgridsizer->Add( 20, 20, 0, wxALIGN_CENTRE|wxALL, 5 );

    m_abortLmouseCheckBox = new wxCheckBox( this, -1, wxT("Abort Left Mouse"), wxDefaultPosition, wxDefaultSize, 0 );
    m_abortLmouseCheckBox->SetToolTip( wxT("Stop capturing when left mouse button pressed") );
    initabort_flexgridsizer->Add( m_abortLmouseCheckBox, 0, wxALIGN_CENTER_VERTICAL, 0 );

    m_abortRmouseCheckBox = new wxCheckBox( this, -1, wxT("Abort Right Mouse"), wxDefaultPosition, wxDefaultSize, 0 );
    m_abortRmouseCheckBox->SetToolTip( wxT("Stop capturing when right mouse button pressed") );
    initabort_flexgridsizer->Add( m_abortRmouseCheckBox, 0, wxALIGN_CENTER_VERTICAL, 0 );

    initabort_staticboxsizer->Add( initabort_flexgridsizer, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5 );

    mainsizer->Add( initabort_staticboxsizer, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    // ---------- Advanced Settings -----------------------------------------

    wxStaticBox *advanced_staticbox = new wxStaticBox( this, -1, wxT("Advanced") );
    wxStaticBoxSizer *advanced_staticboxsizer = new wxStaticBoxSizer( advanced_staticbox, wxVERTICAL );

    wxFlexGridSizer *advanced_flexgridsizer = new wxFlexGridSizer( 2, 0, 0 );

    m_step2xCheckBox = new wxCheckBox( this, -1, wxT("Step Capture 2x"), wxDefaultPosition, wxDefaultSize, 0 );
    m_step2xCheckBox->SetToolTip( wxT("Double resolution capture") );
    advanced_flexgridsizer->Add( m_step2xCheckBox, 0, wxALIGN_CENTER_VERTICAL, 0 );

    advanced_flexgridsizer->Add( 20, 20, 0, wxALIGN_CENTRE|wxALL, 5 );

    m_avgframesSpinCtrl = new wxSpinCtrl( this, -1, wxT("0"), wxDefaultPosition, wxSize(100,-1), 0, 0, 100, 0 );
    advanced_flexgridsizer->Add( m_avgframesSpinCtrl, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxBOTTOM, 5 );

    wxStaticText *avgframes_text = new wxStaticText( this, -1, wxT("Average frames"), wxDefaultPosition, wxDefaultSize, 0 );
    avgframes_text->SetToolTip( wxT("Number of times a frame is sampled before saving") );
    advanced_flexgridsizer->Add( avgframes_text, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_videobuffersSpinCtrl = new wxSpinCtrl( this, -1, wxT("0"), wxDefaultPosition, wxSize(100,-1), 0, 0, 100, 0 );
    advanced_flexgridsizer->Add( m_videobuffersSpinCtrl, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM, 5 );

    wxStaticText *videobuffers_text = new wxStaticText( this, -1, wxT("Number of video buffers"), wxDefaultPosition, wxDefaultSize, 0 );
    videobuffers_text->SetToolTip( wxT("Number of video buffers allocated, 0 = autocalc") );
    advanced_flexgridsizer->Add( videobuffers_text, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_audiobuffersSpinCtrl = new wxSpinCtrl( this, -1, wxT("0"), wxDefaultPosition, wxSize(100,-1), 0, 0, 100, 0 );
    advanced_flexgridsizer->Add( m_audiobuffersSpinCtrl, 0, wxALIGN_CENTRE_VERTICAL|wxBOTTOM, 5 );

    wxStaticText *audiobuffers_text = new wxStaticText( this, -1, wxT("Number of audio buffers"), wxDefaultPosition, wxDefaultSize, 0 );
    audiobuffers_text->SetToolTip( wxT("Number of audio buffers allocated, 0 = max of .5sec or 10Kb") );
    advanced_flexgridsizer->Add( audiobuffers_text, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

    m_maxaudiobuffersSpinCtrl = new wxSpinCtrl( this, -1, wxT("0"), wxDefaultPosition, wxSize(100,-1), 0, 0, 10, 0 );
    advanced_flexgridsizer->Add( m_maxaudiobuffersSpinCtrl, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM, 5 );

    wxStaticText *maxaudiobuffers_text = new wxStaticText( this, -1, wxT("Max audio buffers "), wxDefaultPosition, wxDefaultSize, 0 );
    maxaudiobuffers_text->SetToolTip( wxT("Max number of audio buffers to allocate, 0 = autocalc") );
    advanced_flexgridsizer->Add( maxaudiobuffers_text, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_avientriesString.Printf(wxT("%d"),1234);
    m_avientriesTextCtrl = new wxTextCtrl( this, -1, wxT("123"), wxDefaultPosition, wxSize(80,-1), wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_NUMERIC, &m_avientriesString));
    advanced_flexgridsizer->Add( m_avientriesTextCtrl, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM, 5 );

    wxStaticText *avientries_text = new wxStaticText( this, -1, wxT("Max AVI entries"), wxDefaultPosition, wxDefaultSize, 0 );
    avientries_text->SetToolTip( wxT("Max number of frames in AVI file [1800..324000]") );
    advanced_flexgridsizer->Add( avientries_text, 0, wxALIGN_CENTER_VERTICAL, 5 );

    m_chunkSpinCtrl = new wxSpinCtrl( this, -1, wxT("0"), wxDefaultPosition, wxSize(100,-1), 0, 0, 100, 0 );
    advanced_flexgridsizer->Add( m_chunkSpinCtrl, 0, wxALIGN_CENTER_VERTICAL, 0 );

    wxStaticText *chunk_text = new wxStaticText( this, -1, wxT("Chunk granularity"), wxDefaultPosition, wxDefaultSize, 0 );
    chunk_text->SetToolTip(wxT("Logical block size in AVI file, 0 = sector size, default = 2k") );
    advanced_flexgridsizer->Add( chunk_text, 0, wxALIGN_CENTER_VERTICAL, 5 );

    advanced_staticboxsizer->Add( advanced_flexgridsizer, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5 );

    mainsizer->Add( advanced_staticboxsizer, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxGridSizer *button_gridsizer = new wxGridSizer( 3, 0, 0 );

    m_applyButton = new wxButton( this, IDD_wxVIDCAP_CAPPREFDLG_APPLY, wxT("Apply"), wxDefaultPosition, wxSize(60,-1));
    m_applyButton->SetToolTip(wxT("Apply changes, controls may take only allowed values"));
    button_gridsizer->Add( m_applyButton, 0, wxALIGN_CENTRE|wxALL, 5 );

    m_okButton = new wxButton( this, IDD_wxVIDCAP_CAPPREFDLG_OK, wxT("Ok"), wxDefaultPosition, wxSize(60,-1));
    m_okButton->SetToolTip(wxT("Use apply changes to verify allowed values"));
    m_okButton->SetDefault();
    button_gridsizer->Add( m_okButton, 0, wxALIGN_CENTRE|wxALL, 5 );

    m_cancelButton = new wxButton( this, IDD_wxVIDCAP_CAPPREFDLG_CANCEL, wxT("Cancel"), wxDefaultPosition, wxSize(60,-1));
    button_gridsizer->Add( m_cancelButton, 0, wxALIGN_CENTRE|wxALL, 5 );

    mainsizer->Add( button_gridsizer, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    SetAutoLayout( true );
    SetSizer( mainsizer );
    mainsizer->Fit( this );
    mainsizer->SetSizeHints( this );

    GetSetup();

    Center();
    Show(true);
}

void wxVideoCaptureWindowCapturePreferencesDialog::OnMSFrameSpin(wxSpinEvent &event)
{
    m_msframeText->SetLabel(wxString::Format(wxT("ms/frame = %3.1f fps"), 1000.0/event.GetPosition()));
}

void wxVideoCaptureWindowCapturePreferencesDialog::OnApply(wxCommandEvent &)
{
    SetSetup();
    GetSetup();
}

void wxVideoCaptureWindowCapturePreferencesDialog::OnOk(wxCommandEvent &)
{
    Show(false);
    SetSetup();
    Destroy();
}

void wxVideoCaptureWindowCapturePreferencesDialog::OnCancel(wxCommandEvent &)
{
    Show(false);
    Destroy();
}

void wxVideoCaptureWindowCapturePreferencesDialog::GetSetup()
{
    long int microsecperframe = m_parent->GetMicroSecPerFrameRequested();
    m_msframeSpinCtrl->SetValue(microsecperframe/1000);
    m_msframeText->SetLabel(wxString::Format(wxT("ms/frame = %3.1f fps"), 1000000.0/microsecperframe));

    m_dropSpinCtrl->SetValue(m_parent->GetMaxAllowedFramesDropped());

    m_timelimitCheckBox->SetValue(m_parent->GetTimeLimitedCapture());
    m_timelimitSpinCtrl->SetValue(m_parent->GetCaptureTimeLimit());

    if (m_parent->HasAudioHardware())
    {
        m_recordaudioCheckBox->SetValue(m_parent->GetCaptureAudio());
        m_audiomasterCheckBox->SetValue(m_parent->GetAudioStreamMaster());
    }
    else
    {
        m_recordaudioCheckBox->Enable(false);
        m_audiomasterCheckBox->Enable(false);
    }

    m_dialogtocaptureCheckBox->SetValue(m_parent->GetDialogInitiatedCapture());
    m_abortLmouseCheckBox->SetValue(m_parent->GetAbortLeftMouse());
    m_abortRmouseCheckBox->SetValue(m_parent->GetAbortRightMouse());

    m_step2xCheckBox->SetValue(m_parent->GetStepCaptureAt2x());

    m_avgframesSpinCtrl->SetValue(m_parent->GetStepCaptureAverageFrames());

    m_videobuffersSpinCtrl->SetValue(m_parent->GetNumVideoBuffersAllocated());

    if (m_parent->HasAudioHardware())
    {
        m_audiobuffersSpinCtrl->SetValue(m_parent->GetAudioBufferSize());
        m_maxaudiobuffersSpinCtrl->SetValue(m_parent->GetNumAudioBuffers());
    }
    else
    {
        m_audiobuffersSpinCtrl->Enable(false);
        m_maxaudiobuffersSpinCtrl->Enable(false);
    }

    m_avientriesString.Printf(wxT("%ld"),m_parent->GetAviMaxIndexEntries());
    //m_avientriesTextCtrl->SetValue(wxString::Format(wxT("%ld"), m_parent->GetAviMaxIndexEntries()));

    m_chunkSpinCtrl->SetValue(m_parent->GetChunkGranularity());
}

void wxVideoCaptureWindowCapturePreferencesDialog::SetSetup()
{
    m_parent->SetMicroSecPerFrameRequested((long)m_msframeSpinCtrl->GetValue()*1000);

    m_parent->SetMaxAllowedFramesDropped((unsigned int)m_dropSpinCtrl->GetValue());

    m_parent->SetTimeLimitedCapture(m_timelimitCheckBox->GetValue());
    m_parent->SetCaptureTimeLimit((unsigned int)m_timelimitSpinCtrl->GetValue());

    if (m_parent->HasAudioHardware())
    {
        m_parent->SetCaptureAudio(m_recordaudioCheckBox->GetValue());
        m_parent->SetAudioStreamMaster(m_audiomasterCheckBox->GetValue());
    }

    m_parent->SetDialogInitiatedCapture(m_dialogtocaptureCheckBox->GetValue());
    m_parent->SetAbortLeftMouse(m_abortLmouseCheckBox->GetValue());
    m_parent->SetAbortRightMouse(m_abortRmouseCheckBox->GetValue());

    m_parent->SetStepCaptureAt2x(m_step2xCheckBox->GetValue());

    m_parent->SetStepCaptureAverageFrames((unsigned int)m_avgframesSpinCtrl->GetValue());

    m_parent->SetNumVideoBuffers((unsigned int)m_videobuffersSpinCtrl->GetValue());

    if (m_parent->HasAudioHardware())
    {
        m_parent->SetAudioBufferSize(m_audiobuffersSpinCtrl->GetValue());
        m_parent->SetNumAudioBuffers(m_maxaudiobuffersSpinCtrl->GetValue());
    }

    unsigned long int avimaxentries = 34952;
    bool avientriesOk = m_avientriesTextCtrl->GetValue().ToULong(&avimaxentries);
    if ((avimaxentries < 1800) || (avimaxentries > 324000) || !avientriesOk) avimaxentries = 34952;
    m_parent->SetAviMaxIndexEntries(avimaxentries);

    m_parent->SetChunkGranularity(m_chunkSpinCtrl->GetValue());
}

#endif // WXVIDCAP_AVI_SUPPORT

// --------------------------------------------------------------------------
// wxVideoCaptureWindowCustomVideoFormatDialog
//      NEVER call this, use wxVideoCaptureWindow::VideoCustomFormatDialog
// --------------------------------------------------------------------------

#define IDD_wxVIDCAP_VIDEOFORMATDLG_APPLY   301
#define IDD_wxVIDCAP_VIDEOFORMATDLG_OK      302
#define IDD_wxVIDCAP_VIDEOFORMATDLG_CANCEL  303

BEGIN_EVENT_TABLE(wxVideoCaptureWindowCustomVideoFormatDialog, wxDialog)
    EVT_BUTTON( IDD_wxVIDCAP_VIDEOFORMATDLG_APPLY, wxVideoCaptureWindowCustomVideoFormatDialog::OnApply )
    EVT_BUTTON( IDD_wxVIDCAP_VIDEOFORMATDLG_OK, wxVideoCaptureWindowCustomVideoFormatDialog::OnOk )
    EVT_BUTTON( IDD_wxVIDCAP_VIDEOFORMATDLG_CANCEL, wxVideoCaptureWindowCustomVideoFormatDialog::OnCancel )
END_EVENT_TABLE()

wxVideoCaptureWindowCustomVideoFormatDialog::wxVideoCaptureWindowCustomVideoFormatDialog(
                                   wxVideoCaptureWindowBase *parent,
                                   wxWindowID id, const wxString &title,
                                   const wxPoint &position, const wxSize &size, long style)
                                   :wxDialog( parent, id, title, position, size, style )
{
    m_parent = parent;

    // this shouldn't be run on its own, create wxVideoCaptureWindow
    //  only called using wxVideoCaptureWindow::VideoFormatDialog
/*
    if (!m_parent->m_hWndC)
    {
        wxStaticText errormsg(this, -1, wxT("This is WRONG!"));
        Show(true);
        return;
    }
*/
    int i;

    // create strings to fill the control's selections
    int video_format_count = m_parent->GetVideoCaptureFormatCount();
    wxString *formats_str = new wxString[video_format_count];
    for (i=0; i<video_format_count; i++)
        formats_str[i] = m_parent->GetVideoCaptureFormat(i).m_description;

    wxString widths_str[wxVIDCAP_VIDEO_WIDTHS_COUNT];
    for (i=0; i<wxVIDCAP_VIDEO_WIDTHS_COUNT; i++)
        widths_str[i].Printf(wxT("%d"), wxVIDCAP_VIDEO_WIDTHS[i]);

    wxString heights_str[wxVIDCAP_VIDEO_HEIGHTS_COUNT];
    for (i=0; i<wxVIDCAP_VIDEO_HEIGHTS_COUNT; i++)
        heights_str[i].Printf(wxT("%d"), wxVIDCAP_VIDEO_HEIGHTS[i]);


    wxSizer *mainsizer = new wxBoxSizer( wxVERTICAL );

    mainsizer->Add(new wxStaticText(this, -1, wxT("Caution: This could crash your system!")), 0, wxALL, 5);

    //------- Current setting display ---------------------------------------
    wxStaticBox *currentstaticbox = new wxStaticBox( this, -1, wxT("Current Format"));
    wxStaticBoxSizer *currentstaticboxsizer = new wxStaticBoxSizer( currentstaticbox, wxVERTICAL );

    m_formatText = new wxStaticText( this, -1, wxT("current format"));
    currentstaticboxsizer->Add( m_formatText, 0, wxGROW|wxALL, 5 );

    m_fourccText = new wxStaticText( this, -1, wxT("current format"));
    currentstaticboxsizer->Add( m_fourccText, 0, wxGROW|wxALL, 5 );

    mainsizer->Add( currentstaticboxsizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 5 );

    //------- Standard settings display --------------------------------------
    wxStaticBox *standardstaticbox = new wxStaticBox( this, -1, wxT("Standard Formats"));
    wxStaticBoxSizer *standardstaticboxsizer = new wxStaticBoxSizer( standardstaticbox, wxVERTICAL );


    wxBoxSizer *sizeboxsizer = new wxBoxSizer( wxHORIZONTAL );

    sizeboxsizer->Add( new wxStaticText( this, -1, wxT("Format Size: ")), 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

    m_widthChoice = new wxChoice( this, -1, wxDefaultPosition, wxDefaultSize, wxVIDCAP_VIDEO_WIDTHS_COUNT, widths_str);
    sizeboxsizer->Add( m_widthChoice, 0, wxGROW|wxALL, 5 );

    sizeboxsizer->Add( new wxStaticText( this, -1, wxT("x")), 0,
        wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

    m_heightChoice = new wxChoice( this, -1, wxDefaultPosition, wxDefaultSize, wxVIDCAP_VIDEO_HEIGHTS_COUNT, heights_str);
    sizeboxsizer->Add( m_heightChoice, 0, wxGROW|wxALL, 5 );

    standardstaticboxsizer->Add( sizeboxsizer );

    standardstaticboxsizer->Add( new wxStaticText( this, -1, wxT("Compression Format")), 0, wxGROW|wxLEFT|wxRIGHT, 5 );

    m_formatChoice = new wxChoice(this, -1, wxDefaultPosition, wxDefaultSize, video_format_count, formats_str);
    standardstaticboxsizer->Add( m_formatChoice, 0, wxGROW|wxALL, 5  );

    mainsizer->Add( standardstaticboxsizer, 0, wxGROW|wxALIGN_CENTRE|wxALL, 5 );

    //------- Buttons -------------------------------------------------------
    wxSizer *buttonsizer = new wxBoxSizer( wxHORIZONTAL );

    wxButton *m_applyButton = new wxButton( this, IDD_wxVIDCAP_VIDEOFORMATDLG_APPLY, wxT("Apply"), wxDefaultPosition, wxSize(-1,-1));
    m_applyButton->SetToolTip(wxT("Apply changes, controls may take only allowed values"));
    m_applyButton->SetDefault();
    buttonsizer->Add( m_applyButton, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxButton *m_okButton = new wxButton( this, IDD_wxVIDCAP_VIDEOFORMATDLG_OK, wxT("OK"), wxDefaultPosition, wxSize(-1,-1));
    m_okButton->SetToolTip(wxT("Use apply changes to verify allowed values"));
    buttonsizer->Add( m_okButton, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxButton *m_cancelButton = new wxButton( this, IDD_wxVIDCAP_VIDEOFORMATDLG_CANCEL, wxT("Cancel"), wxDefaultPosition, wxSize(-1,-1));
    buttonsizer->Add( m_cancelButton, 0, wxALIGN_CENTRE|wxALL, 5 );

    mainsizer->Add( buttonsizer, 0, wxALIGN_CENTRE|wxALL, 5 );

    delete []formats_str;

    GetSetup();

    SetAutoLayout( true );
    SetSizer( mainsizer );
    mainsizer->Fit( this );
    mainsizer->SetSizeHints( this );
    Center();
    Show(true);
}

void wxVideoCaptureWindowCustomVideoFormatDialog::GetSetup()
{
    int c_width, c_height, c_bpp;   // current settings from GetSetup()

    FOURCC fourcc;
    m_parent->GetVideoFormat( &c_width, &c_height, &c_bpp, &fourcc );
    wxString fourccStr(FOURCCTowxString(fourcc));

    m_formatText->SetLabel(wxString::Format(wxT("Size: %dx%d, bpp: %d"), c_width, c_height, c_bpp));
    m_fourccText->SetLabel(wxString::Format(wxT("FOURCC: '%s' = 0x%lx"), fourccStr.c_str(), (long unsigned int)fourcc));

    // find the current width and select the correct item
    int width_selection = m_widthChoice->FindString(wxString::Format(wxT("%d"),c_width));
    if (width_selection == -1)
    {
        m_widthChoice->Append(wxString::Format(wxT("%d"),c_width));
        width_selection = m_widthChoice->GetCount() - 1;
    }
    m_widthChoice->SetSelection(width_selection);

    // find the current width and select the correct item
    int height_selection = m_heightChoice->FindString(wxString::Format(wxT("%d"),c_height));
    if (height_selection == -1)
    {
        m_heightChoice->Append(wxString::Format(wxT("%d"),c_height));
        height_selection = m_heightChoice->GetCount() - 1;
    }
    m_heightChoice->SetSelection(height_selection);

    // find what format is being used, first check predefined formats
    int format_selection = m_parent->FindVideoCaptureFormatFOURCC(fourcc);
    // if this is an unknown format
    if (format_selection == -1)
    {
        wxString unknownDescription = wxString::Format(wxT("%s %d-bits Unknown"), fourccStr.c_str(), c_bpp);

        m_parent->RegisterVideoCaptureFormat(
                      new wxVideoCaptureFormat(unknownDescription, fourcc, c_bpp, -1));

        m_formatChoice->Append(unknownDescription);
        format_selection = m_formatChoice->GetCount() - 1;
    }

    m_formatChoice->SetSelection(format_selection);
}

void wxVideoCaptureWindowCustomVideoFormatDialog::SetSetup()
{
    long int width, height;
    int format_selection;

    bool ok = (m_widthChoice->GetStringSelection()).ToLong(&width);
    if (!ok) width = -1; // lets say that it's valid as is

    ok = (m_heightChoice->GetStringSelection()).ToLong(&height);
    if (!ok) height = -1; // good as is

    format_selection = m_formatChoice->GetSelection();

    int bpp = m_parent->GetVideoCaptureFormat(format_selection).m_bpp;
    FOURCC fourcc = m_parent->GetVideoCaptureFormat(format_selection).m_fourcc;

    m_parent->SetVideoFormat( (int)width, (int)height, bpp, fourcc);
}

void wxVideoCaptureWindowCustomVideoFormatDialog::OnApply(wxCommandEvent &)
{
    SetSetup();
    GetSetup();
}

void wxVideoCaptureWindowCustomVideoFormatDialog::OnOk(wxCommandEvent &)
{
    Show(false);
    SetSetup();
    Destroy();
}

void wxVideoCaptureWindowCustomVideoFormatDialog::OnCancel(wxCommandEvent &)
{
    Show(false);
    Destroy();
}

#ifdef WXVIDCAP_AUDIO_SUPPORT

// --------------------------------------------------------------------------
// wxVideoCaptureWindowAudioFormatDialog
//      NEVER call this, use wxVideoCaptureWindow::AudioFormatDialog
// --------------------------------------------------------------------------

#define IDD_wxVIDCAP_AUDIOFORMATDLG_SAMPLES     400
#define IDD_wxVIDCAP_AUDIOFORMATDLG_BITS        401
#define IDD_wxVIDCAP_AUDIOFORMATDLG_CHANNELS    402
#define IDD_wxVIDCAP_AUDIOFORMATDLG_APPLY       403
#define IDD_wxVIDCAP_AUDIOFORMATDLG_OK          404
#define IDD_wxVIDCAP_AUDIOFORMATDLG_CANCEL      405

BEGIN_EVENT_TABLE(wxVideoCaptureWindowAudioFormatDialog, wxDialog)
    EVT_BUTTON( IDD_wxVIDCAP_AUDIOFORMATDLG_APPLY,  wxVideoCaptureWindowAudioFormatDialog::OnApply )
    EVT_BUTTON( IDD_wxVIDCAP_AUDIOFORMATDLG_OK,     wxVideoCaptureWindowAudioFormatDialog::OnOk )
    EVT_BUTTON( IDD_wxVIDCAP_AUDIOFORMATDLG_CANCEL, wxVideoCaptureWindowAudioFormatDialog::OnCancel )
    EVT_CHOICE( IDD_wxVIDCAP_AUDIOFORMATDLG_SAMPLES, wxVideoCaptureWindowAudioFormatDialog::OnChoice )
    EVT_CHOICE( IDD_wxVIDCAP_AUDIOFORMATDLG_BITS, wxVideoCaptureWindowAudioFormatDialog::OnChoice )
    EVT_CHOICE( IDD_wxVIDCAP_AUDIOFORMATDLG_CHANNELS, wxVideoCaptureWindowAudioFormatDialog::OnChoice )
END_EVENT_TABLE()

wxVideoCaptureWindowAudioFormatDialog::wxVideoCaptureWindowAudioFormatDialog(
                                   wxVideoCaptureWindowVFW *parent,
                                   wxWindowID id, const wxString &title,
                                   const wxPoint &position, const wxSize &size, long style)
                                   :wxDialog( parent, id, title, position, size, style )
{
    int i;
    m_parent = parent;

    // this shouldn't be run on its own, create wxVideoCaptureWindow
    //  only called using wxVideoCaptureWindow::AudioFormatDialog
    if (!m_parent->HasAudioHardware()) //!m_parent->m_hWndC ||
    {
        wxStaticText errormsg(this, -1, wxT("This is WRONG!"));
        Show(true);
        return;
    }

    // -------- Format settings --------------------------------------------

    wxSizer *mainsizer = new wxBoxSizer( wxVERTICAL );

    wxStaticBox *formatstaticbox = new wxStaticBox( this, -1, wxT("Audio Format") );
    wxStaticBoxSizer *formatstaticsizer = new wxStaticBoxSizer( formatstaticbox, wxVERTICAL );

    m_datasizeText = new wxStaticText( this, -1, wxT("Data size: 123456 bytes/second") );
    formatstaticsizer->Add( m_datasizeText, 0, wxGROW|wxALL, 5 );

    wxFlexGridSizer *formatsizer = new wxFlexGridSizer( 2 );

    //wxString format_str[] = { wxT("Unknown"), wxT("CD quality"), wxT("Radio quality"), wxT("Telephone quality") };
    //formatsizer->Add( new wxStaticText( this, -1, wxT("Format:")), 0, wxGROW|wxTOP|wxLEFT|wxRIGHT, 5 );
    //m_formatChoice = new wxChoice( this, -1, wxDefaultPosition, wxDefaultSize, 4, format_str);
    //m_formatChoice->SetSelection(0);
    //formatsizer->Add( m_formatChoice, 0, wxGROW|wxALL, 5 );

    wxString samples_str[wxVIDCAP_AUDIO_SAMPLESPERSEC_COUNT];
    for (i=0; i<wxVIDCAP_AUDIO_SAMPLESPERSEC_COUNT; i++)
        samples_str[i].Printf(wxT("%ld"), wxVIDCAP_AUDIO_SAMPLESPERSEC[i]);

    formatsizer->Add( new wxStaticText( this, -1, wxT("Samples per second (Hz):")), 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    m_samplespersecondChoice = new wxChoice( this, IDD_wxVIDCAP_AUDIOFORMATDLG_SAMPLES, wxDefaultPosition, wxDefaultSize, wxVIDCAP_AUDIO_SAMPLESPERSEC_COUNT, samples_str);
    formatsizer->Add( m_samplespersecondChoice, 0, wxGROW|wxALL, 5 );

    wxString bits_str[] = { wxT("8"), wxT("16") };
    formatsizer->Add( new wxStaticText( this, -1, wxT("Bits per sample:")), 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    m_bitspersampleChoice = new wxChoice( this, IDD_wxVIDCAP_AUDIOFORMATDLG_BITS, wxDefaultPosition, wxDefaultSize, 2, bits_str);
    formatsizer->Add( m_bitspersampleChoice, 0, wxGROW|wxALL, 5 );

    wxString channels_str[] = { wxT("Mono"), wxT("Stereo") };
    formatsizer->Add( new wxStaticText( this, -1, wxT("Channels:")), 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    m_channelsChoice = new wxChoice( this, IDD_wxVIDCAP_AUDIOFORMATDLG_CHANNELS, wxDefaultPosition, wxDefaultSize, 2, channels_str);
    formatsizer->Add( m_channelsChoice, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    formatstaticsizer->Add( formatsizer, 0, wxGROW|wxALL, 0 );
    mainsizer->Add( formatstaticsizer, 0, wxGROW|wxALL, 5 );

    // -------- volume setting ----------------------------------------------
/*
    wxStaticBox *volumebox = new wxStaticBox( this, -1, wxT("Volume") );
    wxStaticBoxSizer *volumeboxsizer = new wxStaticBoxSizer( volumebox, wxVERTICAL );

    wxFlexGridSizer *volumesizer = new wxFlexGridSizer( 2, 0, 0 );
    volumeboxsizer->Add( volumesizer, 1, wxGROW, 0 );

    volumesizer->Add( new wxStaticText( this, -1, wxT("L")), 0, wxLEFT|wxTOP, 5 );
    m_leftvolumeSlider = new wxSlider( this, -1, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    volumesizer->Add( m_leftvolumeSlider, 1, wxGROW|wxRIGHT|wxTOP, 5 );

    volumesizer->Add( new wxStaticText( this, -1, wxT("R")), 0, wxLEFT, 5 );
    m_rightvolumeSlider = new wxSlider( this, -1, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    volumesizer->Add( m_rightvolumeSlider, 1, wxGROW|wxRIGHT, 5 );

    mainsizer->Add( volumeboxsizer, 0, wxGROW|wxALL, 5 );
*/
    // -------- buttons -----------------------------------------------------

    wxSizer *buttonsizer = new wxBoxSizer( wxHORIZONTAL );

    wxButton *m_applyButton = new wxButton( this, IDD_wxVIDCAP_AUDIOFORMATDLG_APPLY, wxT("Apply"), wxDefaultPosition, wxSize(60,-1));
    m_applyButton->SetToolTip(wxT("Apply changes, controls may take only allowed values"));
    m_applyButton->SetDefault();
    buttonsizer->Add( m_applyButton, 0, wxALIGN_CENTRE|wxALL, 5 );


    wxButton *m_okButton = new wxButton( this, IDD_wxVIDCAP_AUDIOFORMATDLG_OK, wxT("Ok"), wxDefaultPosition, wxSize(60,-1));
    m_okButton->SetToolTip(wxT("Use apply changes to verify allowed values"));
    buttonsizer->Add( m_okButton, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxButton *m_cancelButton = new wxButton( this, IDD_wxVIDCAP_AUDIOFORMATDLG_CANCEL, wxT("Cancel"), wxDefaultPosition, wxSize(60,-1));
    buttonsizer->Add( m_cancelButton, 0, wxALIGN_CENTRE|wxALL, 5 );

    mainsizer->Add( buttonsizer, 0, wxALIGN_CENTRE|wxALL, 5 );

    GetSetup();

    SetAutoLayout( true );
    SetSizer( mainsizer );
    mainsizer->Fit( this );
    mainsizer->SetSizeHints( this );
    Center();
    Show(true);
}

void wxVideoCaptureWindowAudioFormatDialog::OnChoice(wxCommandEvent &)
{
    long int samplespersecond=8, channels=8, bitspersample=8;

    (m_samplespersecondChoice->GetStringSelection()).ToLong(&samplespersecond);
    (m_bitspersampleChoice->GetStringSelection()).ToLong(&bitspersample);
    channels = m_channelsChoice->GetSelection() + 1;

    long int datasize = samplespersecond * bitspersample * channels / 8;

    m_datasizeText->SetLabel(wxString::Format(wxT("Data size: %ld bytes/second"), datasize));
}

void wxVideoCaptureWindowAudioFormatDialog::GetSetup()
{
    int channels, bitspersample, i;
    long int samplespersecond;

    m_parent->GetAudioFormat( &channels, &bitspersample, &samplespersecond );

    int samples_selection = 0;
    for (i=0; i<wxVIDCAP_AUDIO_SAMPLESPERSEC_COUNT; i++)
        if (samplespersecond == wxVIDCAP_AUDIO_SAMPLESPERSEC[i]) samples_selection = i;
    m_samplespersecondChoice->SetSelection(samples_selection);

    int bits_selection = ((bitspersample == 8) ? 0 : 1);
    m_bitspersampleChoice->SetSelection(bits_selection);

    m_channelsChoice->SetSelection(channels - 1);

    wxCommandEvent event; // throwaway
    OnChoice(event);
}

void wxVideoCaptureWindowAudioFormatDialog::SetSetup()
{
    int channels = 1;
    long int bitspersample = 8;
    long int samplespersecond = 11025;

    (m_samplespersecondChoice->GetStringSelection()).ToLong(&samplespersecond);

    (m_bitspersampleChoice->GetStringSelection()).ToLong(&bitspersample);

    channels = m_channelsChoice->GetSelection() + 1; // mono = 1, stereo = 2

    m_parent->SetAudioFormat( channels, (int)bitspersample, samplespersecond );
}

void wxVideoCaptureWindowAudioFormatDialog::OnApply(wxCommandEvent &)
{
    SetSetup();
    GetSetup();
}

void wxVideoCaptureWindowAudioFormatDialog::OnOk(wxCommandEvent &event)
{
    Show(false);
    OnApply(event);
    Destroy();
}

void wxVideoCaptureWindowAudioFormatDialog::OnCancel(wxCommandEvent &)
{
    Show(false);
    Destroy();
}

#endif // WXVIDCAP_AUDIO_SUPPORT

