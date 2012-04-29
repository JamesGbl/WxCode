/////////////////////////////////////////////////////////////////////////////
// Name:        vcapdlgs.h, dialogs for wxVideoCaptureWindow
// Author:      John Labenski
// Created:     7/06/2001
// Modified:    01/14/03
// Copyright:   John Labenski
// License:     wxWidgets V2.0
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_VCAPDLGS_H__
#define __WX_VCAPDLGS_H__

class WXDLLIMPEXP_FWD_CORE wxSpinCtrl;
class WXDLLIMPEXP_FWD_CORE wxSpinEvent;

#ifdef WXVIDCAP_AVI_SUPPORT
//----------------------------------------------------------------------------
// wxVideoCaptureWindowCaptureSingleFramesDialog - a dialog to capture singles frames
//      NEVER call this, use wxVideoCaptureWindow::CaptureSingleFramesToFileDialog
//----------------------------------------------------------------------------
class WXDLLIMPEXP_VIDCAP wxVideoCaptureWindowCaptureSingleFramesDialog: public wxDialog
{
public:
    wxVideoCaptureWindowCaptureSingleFramesDialog( wxVideoCaptureWindowVFW *parent = NULL,
                                                   wxWindowID id = wxID_ANY,
                                                   const wxString &title = wxT("Capture single frames"),
                                                   const wxPoint& pos = wxDefaultPosition,
                                                   const wxSize& size = wxDefaultSize,
                                                   long style = wxDEFAULT_DIALOG_STYLE );

    void OnCloseDialog(wxCloseEvent &event);
    void OnCapture(wxCommandEvent &event);
    void OnDone(wxCommandEvent &event);

private:
    wxVideoCaptureWindowVFW *m_parent;

    wxButton *m_captureButton;
    wxButton *m_doneButton;

    wxStaticText *m_captureText;
    wxString m_captureString;

    wxString m_fileName;

    DECLARE_EVENT_TABLE()
};
#endif // WXVIDCAP_AVI_SUPPORT

#ifdef WXVIDCAP_AVI_SUPPORT
//----------------------------------------------------------------------------
// wxVideoCaptureWindowCapturePreferencesDialog - a dialog to adjust the capture preferences
//      NEVER call this, use wxVideoCaptureWindow::CapturePreferencesDialog()
//----------------------------------------------------------------------------
class WXDLLIMPEXP_VIDCAP wxVideoCaptureWindowCapturePreferencesDialog: public wxDialog
{
public:
    wxVideoCaptureWindowCapturePreferencesDialog(wxVideoCaptureWindowVFW *parent = NULL,
                                                 wxWindowID id = wxID_ANY,
                                                 const wxString &title = wxT("Capture Preferences"),
                                                 const wxPoint& pos = wxDefaultPosition,
                                                 const wxSize& size = wxDefaultSize,
                                                 long style = wxDEFAULT_DIALOG_STYLE );

    void GetSetup();    // get the current wxVideoCaptureWindow settings
    void SetSetup();    // set capture settings to wxVideoCaptureWindow

    void OnMSFrameSpin(wxSpinEvent &event); // update ms/frame to frames/sec

    void OnApply(wxCommandEvent &event);
    void OnOk(wxCommandEvent &event);
    void OnCancel(wxCommandEvent &event);

    wxVideoCaptureWindowVFW *m_parent;

    // capture controls
    wxSpinCtrl *m_msframeSpinCtrl;
    wxStaticText *m_msframeText;

    wxSpinCtrl *m_dropSpinCtrl;

    wxCheckBox *m_timelimitCheckBox;
    wxSpinCtrl *m_timelimitSpinCtrl;

    wxCheckBox *m_recordaudioCheckBox;
    wxCheckBox *m_audiomasterCheckBox;

    // init/abort capture
    wxCheckBox *m_dialogtocaptureCheckBox;
    wxCheckBox *m_abortLmouseCheckBox;
    wxCheckBox *m_abortRmouseCheckBox;

    // advanced
    wxCheckBox *m_step2xCheckBox;
    wxSpinCtrl *m_avgframesSpinCtrl;

    wxSpinCtrl *m_videobuffersSpinCtrl;
    wxSpinCtrl *m_audiobuffersSpinCtrl;
    wxSpinCtrl *m_maxaudiobuffersSpinCtrl;
    wxString    m_avientriesString; // makes the validator happy
    wxTextCtrl *m_avientriesTextCtrl;
    wxSpinCtrl *m_chunkSpinCtrl;

    // buttons
    wxButton *m_applyButton;
    wxButton *m_okButton;
    wxButton *m_cancelButton;

private:
    DECLARE_EVENT_TABLE()
};
#endif // WXVIDCAP_AVI_SUPPORT

//----------------------------------------------------------------------------
// wxVideoCaptureWindowCustomVideoFormatDialog - a dialog to adjust the video format
//      NEVER call this, use wxVideoCaptureWindow::VideoCustomFormatDialog
//----------------------------------------------------------------------------
class WXDLLIMPEXP_VIDCAP wxVideoCaptureWindowCustomVideoFormatDialog: public wxDialog
{
public:
    wxVideoCaptureWindowCustomVideoFormatDialog(wxVideoCaptureWindowBase *parent = NULL,
                                                wxWindowID id = wxID_ANY,
                                                const wxString &title = wxT("Video Format Setup"),
                                                const wxPoint& pos = wxDefaultPosition,
                                                const wxSize& size = wxDefaultSize,
                                                long style = wxDEFAULT_DIALOG_STYLE );
    void GetSetup();
    void SetSetup();

    void OnApply(wxCommandEvent &event);
    void OnOk(wxCommandEvent &event);
    void OnCancel(wxCommandEvent &event);

private:
    wxVideoCaptureWindowBase *m_parent;

    wxStaticText *m_formatText;
    wxStaticText *m_fourccText;

    wxChoice *m_widthChoice;
    wxChoice *m_heightChoice;
    wxChoice *m_formatChoice;

    DECLARE_EVENT_TABLE()
};

#ifdef WXVIDCAP_AUDIO_SUPPORT
//----------------------------------------------------------------------------
// wxVideoCaptureWindowAudioFormatDialog - a dialog to easily adjust the audio properties
//      NEVER call this, use wxVideoCaptureWindow::AudioFormatDialog
//----------------------------------------------------------------------------

class WXDLLIMPEXP_VIDCAP wxVideoCaptureWindowAudioFormatDialog: public wxDialog
{
public:
    wxVideoCaptureWindowAudioFormatDialog( wxVideoCaptureWindowVFW *parent = NULL,
                                           wxWindowID id = wxID_ANY, const
                                           wxString &title = wxT("Sound Setup"),
                                           const wxPoint& pos = wxDefaultPosition,
                                           const wxSize& size = wxDefaultSize,
                                           long style = wxDEFAULT_DIALOG_STYLE );
    void GetSetup();
    void SetSetup();

    void OnChoice(wxCommandEvent &event);
    void OnApply(wxCommandEvent &event);
    void OnOk(wxCommandEvent &event);
    void OnCancel(wxCommandEvent &event);

private:
    wxVideoCaptureWindowVFW *m_parent;

    wxChoice *m_formatChoice;
    wxChoice *m_samplespersecondChoice;
    wxChoice *m_bitspersampleChoice;
    wxChoice *m_channelsChoice;

    wxStaticText *m_datasizeText;

//  wxSlider *m_leftvolumeSlider;
//  wxSlider *m_rightvolumeSlider;

    DECLARE_EVENT_TABLE()
};
#endif // WXVIDCAP_AUDIO_SUPPORT

#endif //__WX_VCAPDLGS_H__
