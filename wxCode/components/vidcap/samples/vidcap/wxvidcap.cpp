/////////////////////////////////////////////////////////////////////////////
// Name:        wxvidcap.cpp
// Author:      John Labenski
// Created:     07/06/01
// Modified:    01/14/03
// Copyright:   John Labenski
// License:     wxWidgets V2.0
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include <wx/numdlg.h> // for wxGetNumberFromUser
#include "wxvidcap.h"

IMPLEMENT_APP(MyApp)

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------
bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    // create the main application window
    MyFrame *frame = new MyFrame(NULL, -1, wxT("wxVidCap"),
                                 wxPoint(50, 50), wxSize(550, 400));

    frame->Show(true);

    return true;
}

//----------------------------------------------------------------------------
// MyFrame
//----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyFrame,wxFrame)
    //EVT_SIZE( MyFrame::OnSize )
    //EVT_IDLE( MyFrame::OnIdle )
    EVT_CLOSE(MyFrame::OnCloseWindow )

    EVT_MENU(ID_QUIT,  MyFrame::OnQuit)

    EVT_MENU_RANGE(ID_DEVICENONE, ID_DEVICE9, MyFrame::OnChangeDevice)

    EVT_MENU(ID_PREVIEWBUTTON,  MyFrame::OnPreviewButton)
    EVT_MENU(ID_PREVIEW,        MyFrame::OnPreview)
    EVT_MENU(ID_PREVIEWIMAGE,   MyFrame::OnPreviewImage)
    EVT_MENU(ID_PREVIEWSCALED,  MyFrame::OnPreviewScaled)
    EVT_MENU(ID_PREVIEWRATE,    MyFrame::OnPreviewRate)
    EVT_MENU(ID_OVERLAY,        MyFrame::OnOverlay)

    EVT_MENU(ID_DLGSOURCE,      MyFrame::OnVideoDialogs)
    EVT_MENU(ID_DLGFORMAT,      MyFrame::OnVideoDialogs)
    EVT_MENU(ID_DLGCUSTOMFORMAT,MyFrame::OnVideoDialogs)
    EVT_MENU(ID_DLGDISPLAY,     MyFrame::OnVideoDialogs)
    EVT_MENU(ID_DLGPROPERTIES,  MyFrame::OnVideoDialogs)

#ifdef WXVIDCAP_AUDIO_SUPPORT
    EVT_MENU(ID_DLGAUDIO,       MyFrame::OnVideoDialogs)
#endif //WXVIDCAP_AUDIO_SUPPORT

    EVT_MENU(ID_SNAPTOCLIPBOARD,    MyFrame::OnSnapshotToClipboard)
    EVT_MENU(ID_SNAPTOWINDOW,       MyFrame::OnSnapshotToWindow)
    EVT_MENU(ID_SNAPTOFILE,         MyFrame::OnSnapshotToFile)
    EVT_MENU(ID_SNAPTOBMP,          MyFrame::OnSnapshotToBMP)

#ifdef WXVIDCAP_AVI_SUPPORT
    EVT_MENU(ID_SETCAPFILENAME, MyFrame::OnSetCaptureFilename)
    EVT_MENU(ID_SETCAPFILESIZE, MyFrame::OnSetCaptureFilesize)

    EVT_MENU(ID_DLGCOMPRESSION, MyFrame::OnVideoDialogs)
    EVT_MENU(ID_CAPVIDEO,           MyFrame::OnCaptureVideo)
    EVT_MENU(ID_CAPSINGLEFRAMES,    MyFrame::OnCaptureSingleFrames)

    EVT_MENU(ID_DLGCAPPREFERENCES,  MyFrame::OnVideoDialogs)
#endif // WXVIDCAP_AVI_SUPPORT

    EVT_MENU(ID_ABOUT, MyFrame::OnAbout)
    EVT_MENU(ID_SHOWLOG, MyFrame::OnShowLog)
END_EVENT_TABLE()

MyFrame::MyFrame( wxWindow *parent, wxWindowID id, const wxString &title,
                  const wxPoint &position, const wxSize& size, long style )
        :wxFrame( parent, id, title, position, size, style )
{
    m_preview_state = ID_PREVIEW;

    // set the frame icon
    SetIcon(wxICON(mondrian));

    // set-up the menu, toolbar, statusbar so we know what the client size will be
    m_menubar = new wxMenuBar();
    SetMenuBar(m_menubar);
    m_toolBar = CreateToolBar();

#if wxUSE_STATUSBAR
    CreateStatusBar(2);
    SetStatusText(wxT("wxWidgets Video Capture App - please select a device."));
#endif // wxUSE_STATUSBAR

    m_splitterWin = new wxSplitterWindow(this, -1);

    m_logPanel = new wxPanel( m_splitterWin, -1 );
    wxBoxSizer *logpanelsizer = new wxBoxSizer(wxVERTICAL);
    m_logTextCtrl = new wxTextCtrl(m_logPanel, -1, wxT("Event Log for wxVideoCaptureWindow \n"), wxPoint(0,0), wxSize(500,100), wxTE_MULTILINE );
    logpanelsizer->Add(m_logTextCtrl, 1, wxEXPAND);
    m_logPanel->SetAutoLayout( true );
    m_logPanel->SetSizer(logpanelsizer);
    logpanelsizer->Fit( m_logPanel );
    logpanelsizer->SetSizeHints( m_logPanel );

    m_vidCapWin = new MyVideoCaptureWindow( m_splitterWin, ID_VIDEOWIN, wxPoint(0,0), wxSize(400,300));

#ifdef WXVIDCAP_AVI_SUPPORT
    SetTitle(wxT("wxVidCap - ") + m_vidCapWin->GetCaptureFilename());
#else
    SetTitle(wxT("wxVidCap"));
#endif // WXVIDCAP_AVI_SUPPORT

    m_logPanel->Show(false);
    m_vidCapWin->Show(true);
    m_splitterWin->Initialize(m_vidCapWin);

    CreateMenus();
    CreateToolbar();
    SetupVideoMenus();
}

MyFrame::~MyFrame()
{
}

void MyFrame::OnCloseWindow( wxCloseEvent &event)
{
    m_vidCapWin->Preview(false);    // allows for quicker closing
    event.Skip();
}

void MyFrame::OnSize(wxSizeEvent &event)
{
    event.Skip();
}

void MyFrame::OnIdle(wxIdleEvent &event)
{
    event.Skip();
}

void MyFrame::CreateMenus()
{
    //---- File menu --------------------------------------------------------
    m_fileMenu = new wxMenu;
#ifdef WXVIDCAP_AVI_SUPPORT
    m_fileMenu->Append(ID_SETCAPFILENAME,      wxT("Set Capture File&name..."), wxT("Set capture filename"));
    m_fileMenu->Append(ID_SETCAPFILESIZE,      wxT("Set Capture File&size..."), wxT("Set capture filesize"));
    m_fileMenu->Append(ID_SAVECAPTUREDVIDEOAS, wxT("&Extract video to file"),   wxT("After capture save video to new file"), true);

    m_fileMenu->AppendSeparator();
#endif // WXVIDCAP_AVI_SUPPORT

    m_fileMenu->Append(ID_QUIT, wxT("E&xit\tAlt-X"), wxT("Quit this program"));

    //---- Image Processing menu ---------------------------------------------
    m_processMenu = new wxMenu;

    m_processMenu->Append(ID_IMGPROCESS_NEGATIVE, wxT("&Negative"), wxT("Invert intensities"), true);
    m_processMenu->Append(ID_IMGPROCESS_EDGE,     wxT("&Edge detector"), wxT("Very simple edge detector"), true);
    m_processMenu->Append(ID_IMGPROCESS_MOTION,   wxT("&Motion"), wxT("Simple motion detector"), true);

    //---- Video menu -------------------------------------------------------
    m_videoMenu = new wxMenu;

    m_videoMenu->Append(ID_DEVICENONE, wxT("&Disconnect"), wxT("Disconnect from all devices"), true );

    unsigned int i, count = m_vidCapWin->GetDeviceCount();
    if (count > 0)
    {
        for (i=0; i<count; i++)
            m_videoMenu->Append(ID_DEVICE0+i, m_vidCapWin->GetDeviceName(i), wxString::Format(wxT("Video device #%d"),i), true);
    }
    else
    {
        m_videoMenu->Append(ID_DEVICE0, wxT("No Video Devices found"), wxT("No VFW compatible devices found"));
        m_videoMenu->Enable(ID_DEVICE0, false);
    }

    m_videoMenu->AppendSeparator();
    m_videoMenu->Append(ID_PREVIEW,         wxT("&Preview\tCtrl-P"), wxT("Preview video"), true);
    m_videoMenu->Append(ID_PREVIEWIMAGE,    wxT("Preview wx&Image\tCtrl-I"), wxT("Preview using wxImages"), true);
    m_videoMenu->Append(ID_IMGPROCESS_MENU, wxT("P&rocess wxImage"), m_processMenu, wxT("Simple image processing of wxImages"));
    m_videoMenu->Append(ID_PREVIEWSCALED,   wxT("Preview &Scaling\tCtrl-U"), wxT("Fit video size to window"), true);
    m_videoMenu->Append(ID_PREVIEWRATE,     wxT("Preview &Rate...\tCtrl-R"), wxT("Set preview rate"));

    m_videoMenu->Append(ID_OVERLAY, wxT("&Overlay\tCtrl-O"), wxT("Preview using hardware overlay"), true);

    m_videoMenu->AppendSeparator();
    m_videoMenu->Append(ID_DLGSOURCE,       wxT("So&urce..."), wxT("Set video source"));
    m_videoMenu->Append(ID_DLGFORMAT,       wxT("&Format..."), wxT("Set video format"));
    m_videoMenu->Append(ID_DLGCUSTOMFORMAT, wxT("&Custom format..."), wxT("Set custom video format"));
    m_videoMenu->Append(ID_DLGDISPLAY,      wxT("&Display..."), wxT("Set video display"));
    m_videoMenu->Append(ID_DLGPROPERTIES,   wxT("Properties..."), wxT("Video properties"));

    //---- Capture menu ------------------------------------------------------
    m_captureMenu = new wxMenu;

    m_captureMenu->Append(ID_SNAPTOWINDOW,    wxT("Snapshot to &window\tCtrl-W"), wxT("View the current frame"));
    m_captureMenu->Append(ID_SNAPTOCLIPBOARD, wxT("Snapshot to &clipboard\tCtrl-C"), wxT("Capture a frame to the clipboard"));
    m_captureMenu->Append(ID_SNAPTOFILE,      wxT("Snapshot to &file...\tCtrl-F"), wxT("Capture and save a frame to an image file"));
    m_captureMenu->Append(ID_SNAPTOBMP,       wxT("Snapshot to &BMP file...\tCtrl-B"), wxT("Capture and save a frame as a BMP file"));

#ifdef WXVIDCAP_AVI_SUPPORT
    m_captureMenu->Append(ID_CAPVIDEO,        wxT("Capture &video"), wxT("Capture video"));
    m_captureMenu->Append(ID_CAPSINGLEFRAMES, wxT("Capture &single frames..."), wxT("Capture a series of single frames as video"));
#endif // WXVIDCAP_AVI_SUPPORT

    m_captureMenu->AppendSeparator();

    m_captureMenu->Append(ID_NOPREVIEWONCAP, wxT("&Stop preview on capture"), wxT("Stop previewing for any capture"), true);
    m_captureMenu->Check(ID_NOPREVIEWONCAP, true);

#ifdef WXVIDCAP_AVI_SUPPORT
    m_captureMenu->Append(ID_DLGCAPPREFERENCES, wxT("Capture &preferences..."), wxT("Capture preferences dialog"));
    m_captureMenu->Append(ID_DLGCOMPRESSION,    wxT("&Compression..."), wxT("Set recorded video compression"));
#endif // WXVIDCAP_AVI_SUPPORT

#ifdef WXVIDCAP_AUDIO_SUPPORT
    m_captureMenu->Append(ID_DLGAUDIO, wxT("&Audio Format..."), wxT("Set recorded audio format"));
#endif // WXVIDCAP_AUDIO_SUPPORT

    //---- Help menu --------------------------------------------------------
    m_helpMenu = new wxMenu;
    m_helpMenu->Append(ID_ABOUT,   wxT("&About...\tCtrl-A"), wxT("Show about dialog"));
    m_helpMenu->Append(ID_SHOWLOG, wxT("&Show log\tCtrl-L"), wxT("Show log window"), true);

    // now append the freshly created menu to the menu bar...
    m_menubar->Append(m_fileMenu,    wxT("&File"));
    m_menubar->Append(m_videoMenu,   wxT("&Video"));
    m_menubar->Append(m_captureMenu, wxT("&Capture"));
    m_menubar->Append(m_helpMenu,    wxT("&Help"));
}

void MyFrame::CreateToolbar()
{
    m_toolBar->AddCheckTool(ID_PREVIEWBUTTON, wxT("Preview"),        ToolBarBitmaps(0), wxNullBitmap, wxT("Preview"));
    m_toolBar->AddCheckTool(ID_PREVIEWSCALED, wxT("Preview scaled"), ToolBarBitmaps(1), ToolBarBitmaps(2), wxT("Preview scaled"));

    m_toolBar->AddSeparator();

    m_toolBar->AddTool(ID_SNAPTOCLIPBOARD, wxT("Snap to clipboard"), ToolBarBitmaps(3), wxT("Capture image to clipboard"));
    m_toolBar->AddTool(ID_SNAPTOFILE,      wxT("Snap to file"),      ToolBarBitmaps(4), wxT("Capture single image to file"));
#ifdef WXVIDCAP_AVI_SUPPORT
    m_toolBar->AddTool(ID_CAPSINGLEFRAMES, wxT("Cap Frame"), ToolBarBitmaps(5), wxT("Capture single frames to AVI file"));
    m_toolBar->AddTool(ID_CAPVIDEO,        wxT("Cap Video"), ToolBarBitmaps(6), wxT("Capture video to AVI file"));

    m_toolBar->AddSeparator();

    m_toolBar->AddTool(ID_SETCAPFILENAME,    wxT("Set filename"), ToolBarBitmaps(7), wxT("Set capture filename"));
    m_toolBar->AddTool(ID_DLGCAPPREFERENCES, wxT("Preferences"),  ToolBarBitmaps(8), wxT("Capture preferences..."));
#endif // WXVIDCAP_AVI_SUPPORT

    m_toolBar->Realize();
}

void MyFrame::SetupVideoMenus()
{
    int i;
    bool enable = false;    // enable/disable when (dis)connected

    // uncheck the other device
    for (i=0; i<m_vidCapWin->GetDeviceCount(); i++)
        m_videoMenu->Check(ID_DEVICE0+i, false);

    if (m_vidCapWin->IsDeviceConnected())
    {
        enable = true;

        m_videoMenu->Check(ID_DEVICE0+m_vidCapWin->GetDeviceIndex(), true);

        m_toolBar->ToggleTool(ID_PREVIEWBUTTON, m_vidCapWin->IsPreviewing() || m_vidCapWin->IsOverlaying());
        m_videoMenu->Check(ID_PREVIEW, m_vidCapWin->IsPreviewing() && !m_vidCapWin->IsPreviewingwxImage());
        m_videoMenu->Check(ID_PREVIEWIMAGE, m_vidCapWin->IsPreviewingwxImage());
        m_videoMenu->Check(ID_PREVIEWSCALED, m_vidCapWin->IsPreviewScaled());
        m_toolBar->ToggleTool(ID_PREVIEWSCALED, m_vidCapWin->IsPreviewScaled());

        m_videoMenu->Enable(ID_IMGPROCESS_MENU, m_vidCapWin->IsPreviewingwxImage());
        m_processMenu->Enable(ID_IMGPROCESS_NEGATIVE, m_vidCapWin->IsPreviewingwxImage());
        m_processMenu->Enable(ID_IMGPROCESS_EDGE, m_vidCapWin->IsPreviewingwxImage());
        m_processMenu->Enable(ID_IMGPROCESS_MOTION, m_vidCapWin->IsPreviewingwxImage());

        m_videoMenu->Check(ID_OVERLAY, m_vidCapWin->IsOverlaying());
        m_videoMenu->Enable(ID_OVERLAY, m_vidCapWin->HasOverlay());

        m_videoMenu->Enable(ID_DLGSOURCE, m_vidCapWin->HasVideoSourceDialog());
        m_videoMenu->Enable(ID_DLGFORMAT, m_vidCapWin->HasVideoFormatDialog());
        m_videoMenu->Enable(ID_DLGDISPLAY, m_vidCapWin->HasVideoDisplayDialog());

#ifdef WXVIDCAP_AUDIO_SUPPORT
        m_captureMenu->Enable(ID_DLGAUDIO, m_vidCapWin->HasAudioHardware());
#endif // WXVIDCAP_AUDIO_SUPPORT
    }
    else
    {
        m_videoMenu->Enable(ID_OVERLAY, false);

        m_videoMenu->Enable(ID_DLGSOURCE, false);
        m_videoMenu->Enable(ID_DLGFORMAT, false);
        m_videoMenu->Enable(ID_DLGDISPLAY, false);

        m_videoMenu->Enable(ID_IMGPROCESS_MENU, false);
        m_processMenu->Enable(ID_IMGPROCESS_NEGATIVE, false);
        m_processMenu->Enable(ID_IMGPROCESS_EDGE, false);
        m_processMenu->Enable(ID_IMGPROCESS_MOTION, false);

#ifdef WXVIDCAP_AUDIO_SUPPORT
        m_captureMenu->Enable(ID_DLGAUDIO, false);
#endif // WXVIDCAP_AUDIO_SUPPORT
    }

    m_videoMenu->Check(ID_DEVICENONE, !enable);

    // enable/disable the rest of the stuff
#ifdef WXVIDCAP_AUDIO_SUPPORT
    m_fileMenu->Enable(ID_SETCAPFILENAME, enable);
    m_fileMenu->Enable(ID_SETCAPFILESIZE, enable);
    m_fileMenu->Enable(ID_SAVECAPTUREDVIDEOAS, enable);
#endif // WXVIDCAP_AUDIO_SUPPORT

    m_videoMenu->Enable(ID_PREVIEW, enable);
    m_toolBar->EnableTool(ID_PREVIEWBUTTON, enable);
    m_videoMenu->Enable(ID_PREVIEWIMAGE, enable);
    m_videoMenu->Enable(ID_PREVIEWSCALED, enable);
    m_toolBar->EnableTool(ID_PREVIEWSCALED, enable);
    m_videoMenu->Enable(ID_PREVIEWRATE, enable);

    m_videoMenu->Enable(ID_DLGCUSTOMFORMAT, enable);

    m_captureMenu->Enable(ID_SNAPTOWINDOW, enable);
    m_captureMenu->Enable(ID_SNAPTOCLIPBOARD, enable);
    m_toolBar->EnableTool(ID_SNAPTOCLIPBOARD, enable);
    m_captureMenu->Enable(ID_SNAPTOFILE, enable);
    m_toolBar->EnableTool(ID_SNAPTOFILE, enable);
    m_captureMenu->Enable(ID_SNAPTOBMP, enable);

#ifdef WXVIDCAP_AVI_SUPPORT
    m_captureMenu->Enable(ID_CAPVIDEO, enable);
    m_toolBar->EnableTool(ID_CAPVIDEO, enable);
    m_captureMenu->Enable(ID_CAPSINGLEFRAMES, enable);
    m_toolBar->EnableTool(ID_CAPSINGLEFRAMES, enable);
    m_captureMenu->Enable(ID_DLGCAPPREFERENCES, enable);
    m_toolBar->EnableTool(ID_DLGCAPPREFERENCES, enable);
    m_captureMenu->Enable(ID_DLGCOMPRESSION, enable);

    m_captureMenu->Enable(ID_NOPREVIEWONCAP, enable);
#endif // WXVIDCAP_AVI_SUPPORT
}

// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    m_vidCapWin->Preview(false);
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf( _T("wxVideoCaptureWindow capture class demo.\n")
                _T("Written by John Labenski.\n")
                _T("Welcome to %s"), wxVERSION_STRING);

    wxMessageBox(msg, wxT("About wxVidCap"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnShowLog(wxCommandEvent& event)
{
    if (event.IsChecked() && !m_splitterWin->IsSplit())
    {
        m_logPanel->Show(true);
        m_vidCapWin->Show(true);
        m_splitterWin->SplitHorizontally(m_vidCapWin, m_logPanel, 200);
    }
    else if (m_splitterWin->IsSplit())
        m_splitterWin->Unsplit();
}

void MyFrame::OnChangeDevice(wxCommandEvent &event)
{
    int devicenumber = event.GetId() - ID_DEVICE0;

    if (event.GetId() == ID_DEVICENONE)
    {
        m_vidCapWin->DeviceDisconnect();
        m_videoMenu->Check(ID_DEVICENONE, true);
    }
    else
        m_vidCapWin->DeviceConnect(devicenumber);

    SetupVideoMenus();
}

// toolbar button to preview/overlay video from menu selection
void MyFrame::OnPreviewButton(wxCommandEvent &event)
{
    bool preview = 0!=event.GetExtraLong();

    if (m_preview_state == ID_PREVIEW)
        m_vidCapWin->Preview(preview, false);
    else if (m_preview_state == ID_PREVIEWIMAGE)
        m_vidCapWin->Preview(preview, true);
    else if (m_preview_state == ID_OVERLAY)
        m_vidCapWin->Overlay(preview);

    SetupVideoMenus();
}

void MyFrame::OnPreview(wxCommandEvent &event)
{
    if (m_vidCapWin->IsPreviewing() && !event.IsChecked())
        m_vidCapWin->Preview(false);
    else
        m_vidCapWin->Preview(true);

    m_preview_state = ID_PREVIEW;
    SetupVideoMenus();
}

void MyFrame::OnPreviewImage(wxCommandEvent &event)
{
    if (m_vidCapWin->IsPreviewing() && !event.IsChecked())
        m_vidCapWin->Preview(false);
    else
        m_vidCapWin->Preview(true, true);

    m_preview_state = ID_PREVIEWIMAGE;
    SetupVideoMenus();
}

void MyFrame::OnPreviewScaled(wxCommandEvent &)
{
    m_vidCapWin->PreviewScaled(!m_vidCapWin->IsPreviewScaled());
    SetupVideoMenus();
}

void MyFrame::OnPreviewRate(wxCommandEvent &)
{
    unsigned int currentpreviewrate = m_vidCapWin->GetPreviewRateMS();
    long int previewrate;

    previewrate = wxGetNumberFromUser(wxT("Set minimum time between preview frames"),
                                      wxT("milliseconds"),
                                      wxT("Preview Rate"),
                                      currentpreviewrate, 0, 32000,
                                      this );
    // if cancel pressed
    if (previewrate == -1) return;

    m_vidCapWin->SetPreviewRateMS(previewrate);
}

void MyFrame::OnOverlay(wxCommandEvent &event)
{
    if (m_vidCapWin->IsOverlaying() && !event.IsChecked())
        m_vidCapWin->Overlay(false);
    else
        m_vidCapWin->Overlay(true);

    m_preview_state = ID_OVERLAY;

    SetupVideoMenus();
}

void MyFrame::OnVideoDialogs(wxCommandEvent &event)
{
    switch (event.GetId())
    {
        case ID_DLGSOURCE       : m_vidCapWin->VideoSourceDialog(); break;
        case ID_DLGFORMAT       : m_vidCapWin->VideoFormatDialog(); break;
        case ID_DLGCUSTOMFORMAT : m_vidCapWin->VideoCustomFormatDialog(); break;
        case ID_DLGDISPLAY      : m_vidCapWin->VideoDisplayDialog(); break;
#ifdef WXVIDCAP_AVI_SUPPORT
        case ID_DLGCOMPRESSION    : m_vidCapWin->VideoCompressionDialog(); break;
        case ID_DLGCAPPREFERENCES : m_vidCapWin->CapturePreferencesDialog(); break;
#endif // WXVIDCAP_AVI_SUPPORT
#ifdef WXVIDCAP_AUDIO_SUPPORT
        case ID_DLGAUDIO      : m_vidCapWin->AudioFormatDialog(); break;
#endif // WXVIDCAP_AUDIO_SUPPORT

        case ID_DLGPROPERTIES : m_vidCapWin->PropertiesDialog(); break;
        default : break;
    }

    SetupVideoMenus();
}

void MyFrame::OnSnapshotToClipboard(wxCommandEvent &)
{
    if (m_captureMenu->IsChecked(ID_NOPREVIEWONCAP))
    {
        m_vidCapWin->Preview(false);
        m_vidCapWin->Overlay(false);
    }
    m_vidCapWin->SnapshotToClipboard();
    SetupVideoMenus();
}

void MyFrame::OnSnapshotToWindow(wxCommandEvent &)
{
    m_vidCapWin->SnapshotToWindow();
    SetupVideoMenus();
}

void MyFrame::OnSnapshotToFile(wxCommandEvent &)
{
    if (m_captureMenu->IsChecked(ID_NOPREVIEWONCAP))
    {
        m_vidCapWin->Preview(false);
        m_vidCapWin->Overlay(false);
    }

    wxImage image;
    m_vidCapWin->SnapshotTowxImage( image );
    //image = m_vidCapWin->GetwxImage(); // could do this too

    if (!image.Ok())
    {
        wxMessageBox(wxT("Error grabbing frame"),
                     wxT("wxVideoCaptureWindow Error"),
                      wxOK|wxCENTRE);
        return;
    }

    wxString filename = wxFileSelector( wxT("Save Image"),
                                     wxPathOnly(m_image_filename),
                                     wxFileNameFromPath(m_image_filename),
                                     wxT("*.*"),
                                     wxT("BMP files (*.bmp)|*.bmp|PNG files (*.png)|*.png|JPEG files (*.jpg)|*.jpg|GIF files (*.gif)|*.gif|TIFF files (*.tif)|*.tif|PCX files (*.pcx)|*.pcx"),
                                     wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    if (filename == wxEmptyString)  return;

    bool saved = false;

    wxString extension = filename.AfterLast('.').Lower();

    if (extension == wxT("bmp"))
        saved=image.SaveFile(filename, wxBITMAP_TYPE_BMP);
    else if (extension == wxT("png"))
        saved=image.SaveFile(filename, wxBITMAP_TYPE_PNG);
    else if (extension == wxT("pcx"))
        saved=image.SaveFile(filename, wxBITMAP_TYPE_PCX);
    else if ((extension == wxT("tif")) || (extension == wxT("tiff")))
        saved=image.SaveFile(filename, wxBITMAP_TYPE_TIF);
    else if (extension == wxT("jpg"))
        saved=image.SaveFile(filename, wxBITMAP_TYPE_JPEG);
    else if (extension == wxT("pnm"))
        saved=image.SaveFile(filename, wxBITMAP_TYPE_PNM);
    else
        wxMessageBox(wxT("Unknown file type, see options in file selector."),
                     wxT("Unknown file type"),
                      wxOK|wxCENTRE);

    if (saved) m_image_filename = filename;
}

void MyFrame::OnSnapshotToBMP(wxCommandEvent &)
{
    wxString filename = wxFileSelector( wxT("Save Image"),
                                        wxPathOnly(m_bmp_filename),
                                        wxFileNameFromPath(m_bmp_filename),
                                        wxT("*.bmp"),
                                        wxT("BMP files (*.bmp)|*.bmp"),
                                        wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    if (filename == wxEmptyString)  return;

    wxString extension = filename.AfterLast('.').Lower();

    if (m_captureMenu->IsChecked(ID_NOPREVIEWONCAP))
    {
        m_vidCapWin->Preview(false);
        m_vidCapWin->Overlay(false);
    }

    bool saved = false;

    if (extension == wxT("bmp"))
        saved = m_vidCapWin->SnapshotToBMP( filename );

    if (saved) m_bmp_filename = filename;
}

#ifdef WXVIDCAP_AVI_SUPPORT

void MyFrame::OnSetCaptureFilename(wxCommandEvent &)
{
    m_vidCapWin->SetCaptureFilenameDialog();
    SetTitle(wxT("wxVidCap - ") + m_vidCapWin->GetCaptureFilename());
}

void MyFrame::OnSetCaptureFilesize(wxCommandEvent &)
{
    m_vidCapWin->SetCaptureFileSizeDialog();
}

void MyFrame::OnCaptureVideo(wxCommandEvent &)
{
    if (m_captureMenu->IsChecked(ID_NOPREVIEWONCAP))
    {
        m_vidCapWin->Preview(false);
        m_vidCapWin->Overlay(false);
    }
    m_vidCapWin->CaptureVideoToFile();

    float cap_time = m_vidCapWin->GetCaptureTimeElapsedMS()/1000.0;
    unsigned long int frames = m_vidCapWin->GetCapturedVideoFramesCount();
    unsigned long int dropped = m_vidCapWin->GetCapturedVideoFramesDropped();
    float fps = (frames - dropped)/cap_time;

    SetStatusText(wxString::Format(wxT("Captured for: %4.3fs, %ld frames, %ld dropped, (%2.3f f/s)"),
                                    cap_time, frames, dropped, fps), 0);

    if (m_fileMenu->IsChecked(ID_SAVECAPTUREDVIDEOAS))
    {
        wxString savefilename = wxFileSelector( wxT("Save captured video to"),
                                                wxT(""),
                                                wxT(""),
                                                wxT("avi"),
                                                wxT("AVI files (*.avi)|*.avi"),
                                                wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

        if (!savefilename.IsNull()) m_vidCapWin->SaveCapturedFileAs(savefilename);
    }

    SetupVideoMenus();
}

void MyFrame::OnCaptureSingleFrames(wxCommandEvent &)
{
    m_vidCapWin->CaptureSingleFramesToFileDialog();
}

#endif // WXVIDCAP_AVI_SUPPORT

//---------------------------------------------------------------------------
// MyVideoCaptureWindow
//---------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyVideoCaptureWindow,wxVideoCaptureWindow)
    EVT_VIDEO_STATUS( ID_VIDEOWIN,  MyVideoCaptureWindow::OnVideoStatusEvent )
    EVT_VIDEO_FRAME(  ID_VIDEOWIN,  MyVideoCaptureWindow::OnVideoFrameEvent )
    EVT_VIDEO_STREAM( ID_VIDEOWIN,  MyVideoCaptureWindow::OnVideoStreamEvent )
    EVT_VIDEO_ERROR(  ID_VIDEOWIN,  MyVideoCaptureWindow::OnVideoErrorEvent )
    EVT_VIDEO( ID_VIDEOWIN,         MyVideoCaptureWindow::OnVideoEvent )
END_EVENT_TABLE()

MyVideoCaptureWindow::MyVideoCaptureWindow(wxWindow *parent, wxWindowID id,
                             const wxPoint &pos, const wxSize &size,
                             long style, const wxString &name)
                             :wxVideoCaptureWindow(parent, id, pos, size, style, name)
{
    m_frame = (MyFrame*)parent->GetParent();
}

MyVideoCaptureWindow::~MyVideoCaptureWindow()
{
}

void MyVideoCaptureWindow::OnVideoEvent( wxVideoCaptureEvent &event )
{
    event.Skip();
}

void MyVideoCaptureWindow::OnVideoStatusEvent( wxVideoCaptureEvent &event )
{
    wxString wxstr;
    wxstr.Printf(wxT("EVT_VIDEO_STATUS: %s \n"), event.GetStatusText().c_str());
    m_frame->m_logTextCtrl->AppendText(wxstr);

    wxstr.Printf(wxT("%s"), event.GetStatusText().c_str());
    m_frame->SetStatusText(wxstr, 1);

    event.Skip();
}

void MyVideoCaptureWindow::OnVideoFrameEvent( wxVideoCaptureEvent &event )
{
    wxString wxstr;
    unsigned int msperframe = event.GetFramerateMS();
    if (msperframe == 0) msperframe = 1000;

    wxstr.Printf(wxT("Frame #: %ld, ms/frame: %d, fps: %2.2f"),
                    event.GetFrameNumber(), msperframe, 1000.0/msperframe);

    m_frame->SetStatusText(wxstr, 1);
    event.Skip();
}

void MyVideoCaptureWindow::OnVideoStreamEvent( wxVideoCaptureEvent &event )
{
    m_frame->m_logTextCtrl->AppendText(wxString::Format(wxT("StreamEvent \x0D\x0A")));
    event.Skip();
}

void MyVideoCaptureWindow::OnVideoErrorEvent( wxVideoCaptureEvent &event )
{
    wxString wxstr;
    wxstr.Printf(wxT("EVT_VIDEO_ERROR: %s \n"), event.GetErrorText().c_str());

    m_frame->m_logTextCtrl->AppendText(wxstr);
    event.Skip();
}

bool MyVideoCaptureWindow::ProcesswxImageFrame()
{
    bool refresh = true; // return value
    long int i, jj;
    int j;
    const int width = m_wximage.GetWidth();
    const int height = m_wximage.GetHeight();
    unsigned char *imgdata = m_wximage.GetData();

    static wxImage lastimage(width,height); // for motion detector

    // Invert the image - negative
    if (m_frame->m_processMenu->IsChecked(ID_IMGPROCESS_NEGATIVE))
    {
        for (i=0; i<width*height*3; i++) imgdata[i] = 255 - imgdata[i];
    }

    // Very basic edge detector
    if (m_frame->m_processMenu->IsChecked(ID_IMGPROCESS_EDGE))
    {
        unsigned char *imgrow = new unsigned char[width*3];

        if (imgrow)
        {
            unsigned char *rowptr = m_wximage.GetData();

            for (j=0; j<height; j++)
            {
                jj = j*width*3;
                memcpy(imgrow, rowptr, width*3);

                for (i=3; i<width*3; i+=3)
                {
                    imgdata[i   + jj] = abs((int)imgrow[i  ] - imgrow[i-3])*4;
                    imgdata[i+1 + jj] = abs((int)imgrow[i+1] - imgrow[i-2])*4;
                    imgdata[i+2 + jj] = abs((int)imgrow[i+2] - imgrow[i-1])*4;
                }

                rowptr += width*3;
            }

            delete []imgrow;
        }
    }

    // Very basic motion detector,
    //    tweak pixel_threshold, pixels_changed_threshold
    if (m_frame->m_processMenu->IsChecked(ID_IMGPROCESS_MOTION))
    {
        // is the last image still good?
        if (!lastimage.Ok() || (lastimage.GetWidth() != width) || (lastimage.GetHeight() != height))
            lastimage.Create(width,height);

        unsigned char *lastdata = lastimage.GetData();

        int pixel_threshold = 64;  // each pixel checked has to differ by this
        int pixels_changed_threshold = 10; // this many pixels must change

        long int pixels_changed = 0; // # of pixels changed by threshold

        int skip_rows = 3;      // horiz rows to skip
        int skip_cols = 13;     // vert cols to skip

        for (j=0; j<height; j+=skip_rows)
        {
            jj = j*width*3;
            for (i=0; i<width*3; i+=skip_cols)
            {
                if(abs((int)imgdata[i+jj] - lastdata[i+jj]) > pixel_threshold)
                    pixels_changed++;
            }
        }

        if (pixels_changed < pixels_changed_threshold) refresh = false;

        memcpy(lastdata, imgdata, sizeof(unsigned char)*width*height*3);
    }

    return refresh;
}
