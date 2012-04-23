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
#include <wx/artprov.h>

#include "wxvidcap.h"

IMPLEMENT_APP(MyApp)

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------
bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    // create the main application window
    MyFrame *frame = new MyFrame(NULL, wxID_ANY, wxT("wxVidCap"),
                                 wxDefaultPosition, wxSize(550, 400));

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

    EVT_MENU_RANGE(ID_DEVICE_ENUMERATE, ID_DEVICE9, MyFrame::OnChangeDevice)

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
    EVT_MENU(ID_SETCAPFILENAME,     MyFrame::OnSetCaptureFilename)
    EVT_MENU(ID_SETCAPFILESIZE,     MyFrame::OnSetCaptureFilesize)

    EVT_MENU(ID_DLGCOMPRESSION,     MyFrame::OnVideoDialogs)
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

    SetMinSize(wxSize(200, 200));

    // set-up the menu, toolbar, statusbar so we know what the client size will be
    m_menubar = new wxMenuBar();
    SetMenuBar(m_menubar);
    m_toolBar = CreateToolBar();

#if wxUSE_STATUSBAR
    CreateStatusBar(2);
#endif // wxUSE_STATUSBAR

    m_splitterWin = new wxSplitterWindow(this, wxID_ANY);
    m_splitterWin->SetMinimumPaneSize(20);

    m_logPanel = new wxPanel( m_splitterWin, wxID_ANY );
    wxBoxSizer *logpanelsizer = new wxBoxSizer(wxVERTICAL);
    m_logTextCtrl = new wxTextCtrl(m_logPanel, wxID_ANY, 
                                   wxT("Event Log for wxVideoCaptureWindow : \n"), 
                                   wxDefaultPosition, wxDefaultSize, 
                                   wxTE_MULTILINE|wxTE_RICH2|wxTE_READONLY );
    logpanelsizer->Add(m_logTextCtrl, 1, wxEXPAND);
    m_logPanel->SetAutoLayout( true );
    m_logPanel->SetSizer(logpanelsizer);
    logpanelsizer->Fit( m_logPanel );
    logpanelsizer->SetSizeHints( m_logPanel );

    m_vidCapWin = new MyVideoCaptureWindow( m_splitterWin, ID_VIDEOWIN, 
                                            wxDefaultPosition, wxDefaultSize);

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

void MyFrame::EnumerateDevices()
{
    // remember what was connected
    int old_device_index = m_vidCapWin->GetDeviceIndex();
    wxString oldDeviceName("X");
    wxString oldDeviceVersion("X");

    if (old_device_index >= 0)
    {
        oldDeviceName    = m_vidCapWin->GetDeviceName(old_device_index);
        oldDeviceVersion = m_vidCapWin->GetDeviceVersion(old_device_index);
    }

    bool was_connected = m_vidCapWin->IsDeviceConnected();
    if (was_connected)
        m_vidCapWin->DeviceDisconnect();


    m_vidCapWin->EnumerateDevices();

    unsigned int i, count = m_vidCapWin->GetDeviceCount();

    // Clear all the old menu items
    for (i = ID_DEVICE0; i < ID_DEVICE__MAX; ++i)
    {
        wxMenuItem* menuItem = m_videoMenu->FindItem(i);
        if (menuItem != NULL)
            m_videoMenu->Delete(menuItem);
    }

    const size_t device_insert_pos = 3; // pos in the menu to insert ID_DEVICEX at
    int new_device_index = -1;

    // Add back the menu items for each device
    if (count > 0)
    {
        // we only allow 10 devices
        count = wxMin(count, ID_DEVICE__MAX-ID_DEVICE0);

        for (i = 0; i < count; i++)
        {
            m_videoMenu->Insert(device_insert_pos+i, ID_DEVICE0+i, m_vidCapWin->GetDeviceName(i), wxString::Format(wxT("Video device #%d"),i), wxITEM_CHECK);

            // the numbers may have changed, find the same device
            if ((oldDeviceName    == m_vidCapWin->GetDeviceName(i)) &&
                (oldDeviceVersion == m_vidCapWin->GetDeviceVersion(i)))
            {
                new_device_index = i;
            }
        }
    }
    else
    {
        m_videoMenu->Insert(device_insert_pos, ID_DEVICE0, wxT("No Video Devices found"), wxT("No compatible video devices found"));
        m_videoMenu->Enable(ID_DEVICE0, false);
    }

    // try to reconnect back to the old device if it's still there
    if (new_device_index >= 0)
        m_vidCapWin->DeviceConnect(new_device_index);
}

void MyFrame::CreateMenus()
{
    //---- File menu --------------------------------------------------------
    m_fileMenu = new wxMenu;
#ifdef WXVIDCAP_AVI_SUPPORT
    m_fileMenu->Append(ID_SETCAPFILENAME,      wxT("Set Capture File&name..."), wxT("Set capture filename"));
    m_fileMenu->Append(ID_SETCAPFILESIZE,      wxT("Set Capture File&size..."), wxT("Set capture filesize"));
    m_fileMenu->Append(ID_SAVECAPTUREDVIDEOAS, wxT("&Extract video to file"),   wxT("After capture save video to new file"), wxITEM_CHECK);

    m_fileMenu->AppendSeparator();
#endif // WXVIDCAP_AVI_SUPPORT

    m_fileMenu->Append(ID_QUIT, wxT("E&xit\tAlt-X"), wxT("Quit this program"));

    //---- Image Processing menu ---------------------------------------------
    m_processMenu = new wxMenu;

    m_processMenu->Append(ID_IMGPROCESS_NEGATIVE, wxT("&Negative"), wxT("Invert intensities"), wxITEM_CHECK);
    m_processMenu->Append(ID_IMGPROCESS_EDGE,     wxT("&Edge detector"), wxT("Very simple edge detector"), wxITEM_CHECK);
    m_processMenu->Append(ID_IMGPROCESS_MOTION,   wxT("&Motion"), wxT("Simple motion detector"), wxITEM_CHECK);

    //---- Video menu -------------------------------------------------------
    m_videoMenu = new wxMenu;

    m_videoMenu->Append(ID_DEVICE_ENUMERATE, wxT("Sca&n for devices"), wxT("Enumerate devices found on the system"));
    m_videoMenu->AppendSeparator();

    m_videoMenu->Append(ID_DEVICENONE,       wxT("&Disconnect"),       wxT("Disconnect from all devices"), wxITEM_CHECK );

    EnumerateDevices();

    m_videoMenu->AppendSeparator();
    m_videoMenu->Append(ID_PREVIEW,         wxT("&Preview\tCtrl-P"),         wxT("Preview video"), wxITEM_CHECK);
    m_videoMenu->Append(ID_PREVIEWIMAGE,    wxT("Preview wx&Image\tCtrl-I"), wxT("Preview using wxImages"), wxITEM_CHECK);
    m_videoMenu->Append(ID_IMGPROCESS_MENU, wxT("P&rocess wxImage"), m_processMenu, wxT("Simple image processing of wxImages"));
    m_videoMenu->Append(ID_PREVIEWSCALED,   wxT("Preview &Scaling\tCtrl-U"), wxT("Fit video size to window"), wxITEM_CHECK);
    m_videoMenu->Append(ID_PREVIEWRATE,     wxT("Preview &Rate...\tCtrl-R"), wxT("Set preview rate"));

    m_videoMenu->Append(ID_OVERLAY,         wxT("&Overlay\tCtrl-O"),  wxT("Preview using hardware overlay"), wxITEM_CHECK);

    m_videoMenu->AppendSeparator();
    m_videoMenu->Append(ID_DLGSOURCE,       wxT("So&urce..."),        wxT("Set video source"));
    m_videoMenu->Append(ID_DLGFORMAT,       wxT("&Format..."),        wxT("Set video format"));
    m_videoMenu->Append(ID_DLGCUSTOMFORMAT, wxT("&Custom format..."), wxT("Set custom video format"));
    m_videoMenu->Append(ID_DLGDISPLAY,      wxT("&Display..."),       wxT("Set video display"));
    m_videoMenu->Append(ID_DLGPROPERTIES,   wxT("P&roperties..."),    wxT("Video properties"));

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

    m_captureMenu->Append(ID_NOPREVIEWONCAP, wxT("&Stop preview on capture"), wxT("Stop previewing for any capture"), wxITEM_CHECK);
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
    m_helpMenu->Append(ID_SHOWLOG, wxT("&Show log\tCtrl-L"), wxT("Show log window"), wxITEM_CHECK);

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

    bool connected          = m_vidCapWin->IsDeviceConnected();
    bool previewing         = m_vidCapWin->IsPreviewing();
    bool previewing_wximage = m_vidCapWin->IsPreviewingwxImage();
    bool previewing_scaled  = m_vidCapWin->IsPreviewScaled();

    // uncheck the other device
    for (i = 0; i < m_vidCapWin->GetDeviceCount(); i++)
        m_videoMenu->Check(ID_DEVICE0+i, false);

    if (connected)
    {
        m_videoMenu->Check(ID_DEVICE0+m_vidCapWin->GetDeviceIndex(), true);

        m_toolBar->ToggleTool(ID_PREVIEWBUTTON, previewing || m_vidCapWin->IsOverlaying());
        m_videoMenu->Check(ID_PREVIEW,          previewing && !previewing_wximage);
        m_videoMenu->Check(ID_PREVIEWIMAGE,     previewing_wximage);
        m_videoMenu->Check(ID_PREVIEWSCALED,    previewing_scaled);
        m_toolBar->ToggleTool(ID_PREVIEWSCALED, previewing_scaled);

        m_videoMenu->Check( ID_OVERLAY, m_vidCapWin->IsOverlaying());
        m_videoMenu->Enable(ID_OVERLAY, m_vidCapWin->HasOverlay());

        m_videoMenu->Enable(ID_DLGSOURCE,  m_vidCapWin->HasVideoSourceDialog());
        m_videoMenu->Enable(ID_DLGFORMAT,  m_vidCapWin->HasVideoFormatDialog());
        m_videoMenu->Enable(ID_DLGDISPLAY, m_vidCapWin->HasVideoDisplayDialog());

#ifdef WXVIDCAP_AUDIO_SUPPORT
        m_captureMenu->Enable(ID_DLGAUDIO, m_vidCapWin->HasAudioHardware());
#endif // WXVIDCAP_AUDIO_SUPPORT
    }
    else
    {
        m_videoMenu->Enable(ID_OVERLAY,               false);
        m_videoMenu->Enable(ID_DLGSOURCE,             false);
        m_videoMenu->Enable(ID_DLGFORMAT,             false);
        m_videoMenu->Enable(ID_DLGDISPLAY,            false);

#ifdef WXVIDCAP_AUDIO_SUPPORT
        m_captureMenu->Enable(ID_DLGAUDIO, false);
#endif // WXVIDCAP_AUDIO_SUPPORT
    }

    m_videoMenu->Check(ID_DEVICENONE,           !connected);

    m_videoMenu->Enable(  ID_IMGPROCESS_MENU,     previewing_wximage);
    m_processMenu->Enable(ID_IMGPROCESS_NEGATIVE, previewing_wximage);
    m_processMenu->Enable(ID_IMGPROCESS_EDGE,     previewing_wximage);
    m_processMenu->Enable(ID_IMGPROCESS_MOTION,   previewing_wximage);

    // enable/disable the rest of the stuff
#ifdef WXVIDCAP_AUDIO_SUPPORT
    m_fileMenu->Enable(ID_SETCAPFILENAME,       connected);
    m_fileMenu->Enable(ID_SETCAPFILESIZE,       connected);
    m_fileMenu->Enable(ID_SAVECAPTUREDVIDEOAS,  connected);
#endif // WXVIDCAP_AUDIO_SUPPORT

    m_videoMenu->Enable(  ID_PREVIEW,           connected);
    m_toolBar->EnableTool(ID_PREVIEWBUTTON,     connected);
    m_videoMenu->Enable(  ID_PREVIEWIMAGE,      connected);
    m_videoMenu->Enable(  ID_PREVIEWSCALED,     connected);
    m_toolBar->EnableTool(ID_PREVIEWSCALED,     connected);
    m_videoMenu->Enable(  ID_PREVIEWRATE,       connected);

    m_videoMenu->Enable(  ID_DLGCUSTOMFORMAT,   connected);

    m_captureMenu->Enable(ID_SNAPTOWINDOW,      connected);
    m_captureMenu->Enable(ID_SNAPTOCLIPBOARD,   connected);
    m_toolBar->EnableTool(ID_SNAPTOCLIPBOARD,   connected);
    m_captureMenu->Enable(ID_SNAPTOFILE,        connected);
    m_toolBar->EnableTool(ID_SNAPTOFILE,        connected);
    m_captureMenu->Enable(ID_SNAPTOBMP,         connected);

#ifdef WXVIDCAP_AVI_SUPPORT
    m_captureMenu->Enable(ID_CAPVIDEO,          connected);
    m_toolBar->EnableTool(ID_CAPVIDEO,          connected);
    m_captureMenu->Enable(ID_CAPSINGLEFRAMES,   connected);
    m_toolBar->EnableTool(ID_CAPSINGLEFRAMES,   connected);
    m_captureMenu->Enable(ID_DLGCAPPREFERENCES, connected);
    m_toolBar->EnableTool(ID_DLGCAPPREFERENCES, connected);
    m_captureMenu->Enable(ID_DLGCOMPRESSION,    connected);

    m_captureMenu->Enable(ID_NOPREVIEWONCAP,    connected);
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

    int win_id = event.GetId();

    switch (win_id)
    {
        case ID_DEVICE_ENUMERATE :
        {
            EnumerateDevices();
            break;
        }
        case ID_DEVICENONE :
        {
            m_vidCapWin->DeviceDisconnect();
            m_videoMenu->Check(ID_DEVICENONE, true);
            break;
        }
        default :
        {
            m_vidCapWin->DeviceConnect(devicenumber);
            break;
        }
    }

    SetupVideoMenus();
}

// toolbar button to preview/overlay video from menu selection
void MyFrame::OnPreviewButton(wxCommandEvent &event)
{
    bool preview = event.IsChecked();

    switch (m_preview_state)
    {
        case ID_PREVIEW      : m_vidCapWin->Preview(preview, false); break;
        case ID_PREVIEWIMAGE : m_vidCapWin->Preview(preview, true);  break;
        case ID_OVERLAY      : m_vidCapWin->Overlay(preview);        break;
        default              : break;
    }

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
        case ID_DLGSOURCE         : m_vidCapWin->ShowVideoSourceDialog(); break;
        case ID_DLGFORMAT         : m_vidCapWin->ShowVideoFormatDialog(); break;
        case ID_DLGCUSTOMFORMAT   : m_vidCapWin->ShowVideoCustomFormatDialog(); break;
        case ID_DLGDISPLAY        : m_vidCapWin->ShowVideoDisplayDialog(); break;
#ifdef WXVIDCAP_AVI_SUPPORT
        case ID_DLGCOMPRESSION    : m_vidCapWin->ShowVideoCompressionDialog(); break;
        case ID_DLGCAPPREFERENCES : m_vidCapWin->ShowCapturePreferencesDialog(); break;
#endif // WXVIDCAP_AVI_SUPPORT
#ifdef WXVIDCAP_AUDIO_SUPPORT
        case ID_DLGAUDIO          : m_vidCapWin->ShowAudioFormatDialog(); break;
#endif // WXVIDCAP_AUDIO_SUPPORT

        case ID_DLGPROPERTIES     : m_vidCapWin->ShowPropertiesDialog(); break;
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
                     wxOK|wxCENTRE, this);
        return;
    }

    wxString filename = wxFileSelector( wxT("Save Image"),
                                     wxPathOnly(m_image_filename),
                                     wxFileNameFromPath(m_image_filename),
                                     wxT("*.*"),
                                     wxT("BMP files (*.bmp)|*.bmp|PNG files (*.png)|*.png|JPEG files (*.jpg)|*.jpg|GIF files (*.gif)|*.gif|TIFF files (*.tif)|*.tif|PCX files (*.pcx)|*.pcx"),
                                     wxFD_SAVE|wxFD_OVERWRITE_PROMPT, this);

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
                      wxOK|wxCENTRE, this);

    if (saved) m_image_filename = filename;
}

void MyFrame::OnSnapshotToBMP(wxCommandEvent &)
{
    wxString filename = wxFileSelector( wxT("Save Image"),
                                        wxPathOnly(m_bmp_filename),
                                        wxFileNameFromPath(m_bmp_filename),
                                        wxT("*.bmp"),
                                        wxT("BMP files (*.bmp)|*.bmp"),
                                        wxFD_SAVE|wxFD_OVERWRITE_PROMPT, this);

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
                                                wxFD_SAVE|wxFD_OVERWRITE_PROMPT, this);

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
    EVT_VIDEO_FRAME ( ID_VIDEOWIN,  MyVideoCaptureWindow::OnVideoFrameEvent )
    EVT_VIDEO_STREAM( ID_VIDEOWIN,  MyVideoCaptureWindow::OnVideoStreamEvent )
    EVT_VIDEO_ERROR ( ID_VIDEOWIN,  MyVideoCaptureWindow::OnVideoErrorEvent )
    EVT_VIDEO       ( ID_VIDEOWIN,  MyVideoCaptureWindow::OnVideoEvent )
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

void MyVideoCaptureWindow::DoPaint( wxPaintDC& dc )
{
    wxVideoCaptureWindowBase::DoPaint(dc);   

    if (!IsDeviceConnected())
    {
        // The window is blank when disconnected so we might as well
        // give a hint to people that they need to connect first.
        wxBitmap bmp = wxArtProvider::GetBitmap(wxART_MISSING_IMAGE, wxART_OTHER, wxSize(32, 32));
        wxSize clientSize(GetClientSize());
        
        wxString txt(wxT("Please select a capture device"));
        wxSize txtSize = dc.GetTextExtent(txt);
        
        dc.DrawText(txt,   clientSize.GetWidth()/2  - txtSize.GetWidth()/2,
                           clientSize.GetHeight()/2 - txtSize.GetHeight());
        dc.DrawBitmap(bmp, clientSize.GetWidth()/2  - bmp.GetWidth()/2,
                           clientSize.GetHeight()/2 + 4);
    }
}

void MyVideoCaptureWindow::OnVideoEvent( wxVideoCaptureEvent &event )
{
    event.Skip();
}

void MyVideoCaptureWindow::OnVideoStatusEvent( wxVideoCaptureEvent &event )
{
    wxString wxstr;
    wxstr.Printf(wxT("wxEVT_VIDEO_STATUS: %s \n"), event.GetStatusText().c_str());
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
    wxstr.Printf(wxT("wxEVT_VIDEO_ERROR: %s \n"), event.GetErrorText().c_str());

    m_frame->m_logTextCtrl->AppendText(wxstr);
    event.Skip();
}

bool MyVideoCaptureWindow::ProcesswxImageFrame()
{
    // This function is only called with a valid wxImage

    bool refresh = true; // return value
    int i, j, jj;
    const int width        = m_wximage.GetWidth();
    const int height       = m_wximage.GetHeight();
    const int width_x3     = width*3;
    const int imgdata_size = width*height*3;
    unsigned char *imgdata = m_wximage.GetData();

    static wxImage lastimage(width, height); // for motion detector

    // Invert the image - negative
    if (m_frame->m_processMenu->IsChecked(ID_IMGPROCESS_NEGATIVE))
    {
        for (i = 0; i < imgdata_size; ++i) imgdata[i] = 255 - imgdata[i];
    }

    // Very basic edge detector
    if (m_frame->m_processMenu->IsChecked(ID_IMGPROCESS_EDGE))
    {
        unsigned char *imgrow = new unsigned char[width_x3];

        if (imgrow)
        {
            unsigned char *rowptr = m_wximage.GetData();

            for (j = 0; j < height; ++j)
            {
                int jj = j*width_x3;
                memcpy(imgrow, rowptr, width_x3);

                for (i = 3; i < width_x3; i += 3)
                {
                    imgdata[i   + jj] = abs((int)imgrow[i  ] - imgrow[i-3])*4;
                    imgdata[i+1 + jj] = abs((int)imgrow[i+1] - imgrow[i-2])*4;
                    imgdata[i+2 + jj] = abs((int)imgrow[i+2] - imgrow[i-1])*4;
                }

                rowptr += width_x3;
            }

            delete []imgrow;
        }
    }

    // Very basic motion detector,
    //    tweak pixel_threshold, pixels_changed_threshold
    if (m_frame->m_processMenu->IsChecked(ID_IMGPROCESS_MOTION))
    {
        // is the last image still good?
        if (!lastimage.Ok() || 
            (lastimage.GetWidth() != width) || 
            (lastimage.GetHeight() != height))
        {
            lastimage.Create(width,height);
        }

        unsigned char *lastdata = lastimage.GetData();

        int pixel_threshold          = 64; // each pixel checked has to differ by this
        int pixels_changed_threshold = 10; // this many pixels must change

        int pixels_changed = 0; // # of pixels changed by threshold

        int skip_rows = 3;      // horiz rows to skip
        int skip_cols = 13;     // vert cols to skip

        for (j = 0; j < height; j += skip_rows)
        {
            jj = j*width_x3;

            for (i = 0; i < width_x3; i += skip_cols)
            {
                if(abs((int)imgdata[i+jj] - lastdata[i+jj]) > pixel_threshold)
                    pixels_changed++;
            }
        }

        if (pixels_changed < pixels_changed_threshold) refresh = false;

        memcpy(lastdata, imgdata, sizeof(unsigned char)*imgdata_size);
    }

    return refresh;
}
