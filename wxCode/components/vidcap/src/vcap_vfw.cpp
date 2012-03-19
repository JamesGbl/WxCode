//TODO:
//
// What should the MSW capture window's ID be? it's 125 now...
// What the heck is an MCI device? vcr/videodisc? do/did they really exist?
// Audio volume and maybe device? mine's on the mic input :(
// Allow for centering of hWnd, when no scrollbars on the window, maybe
// RegisterHotKey to use SetAbortKey
// Pinnacle Studio PCTV flashes GUI when device is disconnected and reconnected
//      and then preview is set on, all video programs do it, if Overlay
//      first then preview it's OK, hummm... see DeviceConnect
// Deal with status callback in a more sensible way
// Deal with the Yield callback, NOT TESTED
// Allow for image processing of the captured frames before saving
// Need to set Preview(false) in program's OnClose or wait for a while?


/////////////////////////////////////////////////////////////////////////////
// Name:        vcap_vfw.cpp, VFW 1.1 video capture/playback for wxWidgets
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

#if defined(__WXMSW__)

#include <wx/clipbrd.h>
#include <wx/file.h>
#include <wx/numdlg.h>

#include "wx/vidcap/vcapwin.h"
#include "vcapdlgs.h"

#include <windows.h>

//----------------------------------------------------------------------------
// wxVideoCaptureWindow #defines and globals
//----------------------------------------------------------------------------
// id for the timer used by previewing w/ wxImages
#define IDD_wxVIDCAP_PREVIEW_WXIMAGE_TIMER 511

// id's for the dialogs, only one at a time please
enum
{
    IDD_wxVIDCAP_CAPSNGFRAMESDLG = 10,
    IDD_wxVIDCAP_CAPPREFDLG,
    IDD_wxVIDCAP_VIDEOFORMATDLG,
    IDD_wxVIDCAP_AUDIOFORMATDLG
};

//----------------------------------------------------------------------------
// wxList of all wxVideoCaptureWindows, for MSW callbacks (copied from msw/window.cpp)
//      you can actually make more then one, but only "gently" then crash :)
//----------------------------------------------------------------------------

// hash containing all our windows, it uses HWND keys and wxWindow* values
WX_DECLARE_HASH_MAP(WXHWND, wxWindow *,
                    wxPointerHash, wxPointerEqual,
                    wxVideoCaptureWindowHandles);

wxVideoCaptureWindowHandles gs_vidCapWinHandles;

// find the wxVideoCaptureWindow that has this hWnd
wxVideoCaptureWindowVFW *wxFindVidCapWinFromHandle( WXHWND hWnd )
{
    wxVideoCaptureWindowHandles::const_iterator iter = gs_vidCapWinHandles.find(hWnd);

    if ( iter == gs_vidCapWinHandles.end() )
        return (wxVideoCaptureWindowVFW*)NULL;

    return (wxVideoCaptureWindowVFW*)iter->second;
}

// call in constructor, when a wxVideoCaptureWindow is created
// associate the m_hWndC w/ wxVideoCaptureWindow
void wxAssociateVidCapWinWithHandle(HWND hWnd, wxWindow *win)
{
    // adding NULL hWnd is (first) surely a result of an error and
    // (secondly) breaks menu command processing
    wxCHECK_RET( hWnd != (HWND)NULL,
                 wxT("attempt to add a NULL hWnd to window list ignored") );

    wxWindow *oldWin = wxFindVidCapWinFromHandle((WXHWND) hWnd);
    if ( oldWin && (oldWin != win) )
    {
        wxString str(win->GetClassInfo()->GetClassName());
        wxLogError(wxT("Bug! Found existing HWND %p for new window of class %s"),
                    hWnd, str.c_str());
    }
    else if (!oldWin)
    {
        gs_vidCapWinHandles[hWnd] = win;
    }
}

// call on destructor
// remove the association between the m_hWndC and the wxVideoCaptureWindow
void wxRemoveVidCapWinHandleAssociation(wxWindow *win)
{
    gs_vidCapWinHandles.erase(GetHwndOf(win));
}

//----------------------------------------------------------------------------
// prototypes for MSW Callbacks for wxVideoCaptureWindow, see bottom of file
//----------------------------------------------------------------------------
LONG VFW_VideoExceptionHandler( LPEXCEPTION_POINTERS arg );
// note: callbacks return LRESULT FAR PASCAL, can use CALLBACK = FAR PASCAL

#if UNICODE
    static LRESULT CALLBACK VFW_VideoErrorCallbackProc(HWND hWnd, int nErrID, LPCWSTR lpErrorText);
    static LRESULT CALLBACK VFW_VideoStatusCallbackProc(HWND hWnd, int nID, LPCWSTR lpStatusText);
#else
    static LRESULT CALLBACK VFW_VideoErrorCallbackProc(HWND hWnd, int nErrID, LPSTR lpErrorText);
    static LRESULT CALLBACK VFW_VideoStatusCallbackProc(HWND hWnd, int nID, LPSTR lpStatusText);
#endif

static LRESULT CALLBACK VFW_VideoFrameCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr);
static LRESULT CALLBACK VFW_VideoStreamCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr);
static LRESULT CALLBACK VFW_CaptureYieldCallbackProc(HWND hwnd);
static LRESULT CALLBACK VFW_CaptureControlCallbackProc(HWND hWnd, int nState);
static LRESULT CALLBACK VFW_WaveStreamCallbackProc(HWND hWnd, LPWAVEHDR lpWHdr);

//------------------------------------------------------------------------------
// wxVideoCaptureWindow
//------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(wxVideoCaptureWindowVFW, wxVideoCaptureWindowBase)

BEGIN_EVENT_TABLE(wxVideoCaptureWindowVFW, wxVideoCaptureWindowBase)
    EVT_MOVE(wxVideoCaptureWindowVFW::OnMove)
    EVT_PAINT(wxVideoCaptureWindowVFW::OnDraw)
    EVT_SCROLLWIN(wxVideoCaptureWindowVFW::OnScrollWin)
    EVT_TIMER(IDD_wxVIDCAP_PREVIEW_WXIMAGE_TIMER, wxVideoCaptureWindowVFW::OnPreviewwxImageTimer)
    EVT_IDLE(wxVideoCaptureWindowVFW::OnIdle)
    EVT_CLOSE(wxVideoCaptureWindowVFW::OnCloseWindow)
END_EVENT_TABLE()

void wxVideoCaptureWindowVFW::Init()
{
    m_hWndC = (HWND)NULL;

    // initialize the variables
    m_lpBmpInfo24bpp = (BITMAPINFO*)NULL;   // our 24bpp header for a MSW DIB
    m_lpBmpInfo = (BITMAPINFO*)NULL;        // the bitmap header from the video
    m_lpBmpInfoLast = (BITMAPINFO*)NULL;    // the last frame's bitmap header
    m_hic_compressor = (HIC)NULL;           // MSW video (de)compressor handle
    m_getting_wximage = false;              // not filling the wximage now
    m_grab_wximage = false;                 // don't grab 1st frame

    m_bmpdata = (unsigned char*)NULL;       // holder for some data

    m_capsingleframecount = -1;
}

wxVideoCaptureWindowVFW::wxVideoCaptureWindowVFW( wxWindow *parent, wxWindowID id,
                                                  const wxPoint &pos, const wxSize &size,
                                                  long style, const wxString &name)
                        :wxVideoCaptureWindowBase()
{
    Init();
    if (!Create( parent, id, pos, size, style, name ))
        wxMessageBox(wxT("Can't create capture window"), wxT("wxVideoCaptureWindow Error"));
}

bool wxVideoCaptureWindowVFW::Create( wxWindow *parent, wxWindowID id,
                                      const wxPoint &pos, const wxSize &size,
                                      long style, const wxString &name )
{
    wxCHECK_MSG(!m_hWndC, false, wxT("Called wxVideoCaptureWindowVFW::Create twice, DO NOT CONTINUE!"));

    if (!wxVideoCaptureWindowBase::Create(parent, id, pos, size, style, name))
        return false;

    m_clientSize = GetClientSize();

    // this is the MSW VFW vidcap window
    m_hWndC = capCreateCaptureWindow(wxString(wxT("Capture Window")).fn_str(),
                                    WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,
                                    pos.x, pos.y,
                                    m_clientSize.GetWidth(),
                                    m_clientSize.GetHeight(),
                                    (HWND)GetHWND(),      // parent
                                    id);                  // capwindow ID?

    wxCHECK_MSG(m_hWndC, false, wxT("Failed to create the capture window, DO NOT CONTINUE!"));

    // causes segfault when used in conjunction with VFW callbacks
    //SubclassWin( (WXHWND)m_hWndC );

    // add to private list of video windows
    wxAssociateVidCapWinWithHandle( (HWND)m_hWndC, this );

    // disconnect devices if *anything* strange happens
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)(VFW_VideoExceptionHandler));

    // get default setup of AVIcap window
    VFW_GetCAPTUREPARMS();
    VFW_GetCAPSTATUS();
    EnumerateDevices();

    // setup the timer when previewing w/ wxImages
    m_preview_wximage_timer.SetOwner(this, IDD_wxVIDCAP_PREVIEW_WXIMAGE_TIMER);

    // set up some generic useful parameters
    SetPreviewRateMS(m_previewmsperframe);
    SetDialogInitiatedCapture(true);    // popup dlg, no surprises
    SetAbortRightMouse(true);
    SetAbortLeftMouse(true);
    SetAudioStreamMaster(true);
    SetMaxAllowedFramesDropped(100);    // sure why not

    return true;
}

// yes this has the same stuff as OnCloseWindow, erm... which is better?
wxVideoCaptureWindowVFW::~wxVideoCaptureWindowVFW()
{
    if (m_capsingleframecount != -1) CaptureSingleFramesToFileClose();

    DeviceDisconnect();
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)NULL);

    // delete from list of wxVideoCaptureWindows
    wxRemoveVidCapWinHandleAssociation(this);

    if (m_hWndC) DestroyWindow(m_hWndC);

    // "delete" any other data structures
    if (m_bmpdata) delete []m_bmpdata;
    if (m_wximage.Ok()) m_wximage.Destroy();
    if (m_hic_compressor) ICClose(m_hic_compressor);
    if (m_lpBmpInfo24bpp) delete []m_lpBmpInfo24bpp;
    if (m_lpBmpInfo) delete []m_lpBmpInfo;
    if (m_lpBmpInfoLast) delete []m_lpBmpInfoLast;
}

// yes this has the same stuff as ~wxVideoCaptureWindow, erm... which is better?
// I think that makes the closing more responsive when previewing w/ wxImages
void wxVideoCaptureWindowVFW::OnCloseWindow(wxCloseEvent &)
{
    if (m_capsingleframecount != -1) CaptureSingleFramesToFileClose();

    DeviceDisconnect();
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)NULL);

    // delete from list of wxVideoCaptureWindows
    wxRemoveVidCapWinHandleAssociation(this);

    if (m_hWndC)
    {
        DestroyWindow(m_hWndC);
        m_hWndC = NULL;
    }

    // "delete" any other data structures
    if (m_bmpdata)
    {
        delete []m_bmpdata;
        m_bmpdata = NULL;
    }
    if (m_wximage.Ok()) m_wximage.Destroy();
    if (m_hic_compressor)
    {
        ICClose(m_hic_compressor);
        m_hic_compressor = NULL;
    }
    if (m_lpBmpInfo24bpp)
    {
        delete []m_lpBmpInfo24bpp;
        m_lpBmpInfo24bpp = NULL;
    }
    if (m_lpBmpInfo)
    {
        delete []m_lpBmpInfo;
        m_lpBmpInfo = NULL;
    }
    if (m_lpBmpInfoLast)
    {
        delete []m_lpBmpInfoLast;
        m_lpBmpInfoLast = NULL;
    }
}

void wxVideoCaptureWindowVFW::OnIdle( wxIdleEvent &event )
{
#if USE_PREVIEW_wxIMAGE_TIMER
    static unsigned long int last_frame = -1;
    static unsigned int delaytime = 0;
    static wxLongLong timenow = wxGetLocalTimeMillis();
    static wxLongLong lasttime = wxGetLocalTimeMillis();

    if (m_preview_wximage)
    {
        // last message has to be processed before we call again
        if (!m_getting_wximage && (last_frame != m_framenumber) && IsDeviceConnected())
        {
            last_frame = m_framenumber; // at least wait for the frame to process

            timenow = wxGetLocalTimeMillis();
            delaytime = (unsigned int)(timenow.GetLo() - lasttime.GetLo());
            lasttime = timenow;

            delaytime = (delaytime > m_previewmsperframe) ? 0 : m_previewmsperframe - delaytime;

            m_preview_wximage_timer.Start(delaytime, true); // one shot
        }
    }
#endif

    event.Skip();
}

void wxVideoCaptureWindowVFW::DoSetSize(int x, int y, int width, int height,
                                        int sizeFlags)
{
    wxVideoCaptureWindowBase::DoSetSize(x, y, width, height, sizeFlags);
    // the m_hWndC *must* be reset to 0,0 so it doesn't get lost if scrolled
    DoSizeWindow();
}

void wxVideoCaptureWindowVFW::DoSizeWindow()
{
    m_clientSize = GetClientSize();

    int virtual_width, virtual_height;
    GetVirtualSize( &virtual_width, &virtual_height );

    // clear up backgound if necessary
    if ((GetImageWidth() < virtual_width) || (GetImageHeight() < virtual_height))
    {
        //Refresh(true); // this doesn't cut it when using wxImages...
        wxClientDC dc(this);
        dc.Clear();
    }

    if (IsPreviewScaled())
    {
        // the video will fill the extent of the window
        SetScrollbars(1, 1, 2, 2, 0, 0, true); // make them go away
        MoveWindow(m_hWndC, 0, 0, m_clientSize.x, m_clientSize.y, true);
    }
    // use scrollbars when video is larger than window size
    else
    {
        SetScrollbars(1, 1, GetImageWidth(), GetImageHeight(), 0, 0, true);
        VFW_ScrollHWND( 0, 0 );
        MoveWindow(m_hWndC, 0, 0, GetImageWidth(), GetImageHeight(), false);
    }
}

void wxVideoCaptureWindowVFW::OnMove( wxMoveEvent &event )
{
    // if we're Overlaying the window, make it follow by resetting
    // otherwise, we don't care
    if (IsOverlaying()) capOverlay(m_hWndC, true);
    event.Skip();
}

bool wxVideoCaptureWindowVFW::VFW_ScrollHWND(int x, int y)
{
    POINT orig;
    orig.x = x;
    orig.y = y;
    return 0!=capSetScrollPos(m_hWndC, &orig);
}

wxPoint wxVideoCaptureWindowVFW::VFW_GetHWNDViewStart()
{
    VFW_GetCAPSTATUS();
    return wxPoint(m_CAPSTATUS.ptScroll.x, m_CAPSTATUS.ptScroll.y);
}

void wxVideoCaptureWindowVFW::OnScrollWin( wxScrollWinEvent &event )
{
    // if we're Overlaying the window, try to scroll (scanline garbage usually)
    // otherwise, we don't care
    if (IsOverlaying())
    {
        int x0, y0;
        GetViewStart( &x0, &y0 );
        VFW_ScrollHWND( x0, y0 );
    }
    event.Skip();
}

void wxVideoCaptureWindowVFW::OnDraw( wxPaintEvent &event )
{
    wxPaintDC dc(this);
    PrepareDC( dc );

    if (m_preview_wximage && !m_getting_wximage && m_wximage.Ok())
    {
        if (m_previewscaled)
            dc.DrawBitmap( wxBitmap(m_wximage.Scale(m_clientSize.x,m_clientSize.y)), 0, 0 );
        else
            dc.DrawBitmap(wxBitmap(m_wximage), 0, 0);
    }

    event.Skip();
}

// ----------------------------------------------------------------------
// Device Descriptions, get and enumerate
// ----------------------------------------------------------------------
void wxVideoCaptureWindowVFW::EnumerateDevices()
{
    unsigned int index;
    wxString wxstr;
    TCHAR devicename[80];
    TCHAR deviceversion[80];

    m_deviceNames.Clear();
    m_deviceVersions.Clear();

    for (index=0; index<10; index++) // MAX allowed in VFW is 10
    {
        bool ok = 0!=capGetDriverDescription( index,
                                            devicename,
                                            sizeof(devicename),
                                            deviceversion,
                                            sizeof(deviceversion));
        if (ok)
        {
            m_deviceNames.Add(wxString(devicename));
            m_deviceVersions.Add(wxString(deviceversion));
        }
        else
            break; // no more devices
    }
}

// ----------------------------------------------------------------------
// Connect and Disconnect to device
// ----------------------------------------------------------------------
bool wxVideoCaptureWindowVFW::IsDeviceInitialized()
{
    VFW_GetCAPDRIVERCAPS();
    return 0!=m_CAPDRIVERCAPS.fCaptureInitialized;
}

bool wxVideoCaptureWindowVFW::DeviceConnect(int index)
{
    wxCHECK_MSG((index>=0) && (index < GetDeviceCount()), false, wxT("invalid device index"));

    if (IsDeviceConnected()) DeviceDisconnect();

    VFW_SetCallbackOnError(true);    // set error handling callback function
    VFW_SetCallbackOnStatus(true);   // set status callback function

    // TODO??
    // "Debug Assertion Failed! dbgdel.cpp line 47, _BLOCK_TYPE_IS_VALID(pHead, nBlockUse)"
    // if device doesn't initialize correctly on boot-up
    // eg. Kensington device (others?) and Compaq w/ VIA USB chipset (culprit?)
    // device appears in "Device Manager" as OK, but nothing can use it
    // solution: CLOSE capture program and unplug device, plug it back in
    // probably no real solution
    m_deviceIndex = (0!=capDriverConnect(m_hWndC, index))? index : -1;

    VFW_GetCAPDRIVERCAPS();
    VFW_GetCAPSTATUS();
    VFW_GetCAPTUREPARMS();
    DoSizeWindow();

    // NASTY HACK, Pinnacle TV cards flash GUI once/second if connected w/ preview
    // run Overlay first and all is good
    if (IsDeviceConnected())
    {
        if (HasOverlay() && !IsOverlaying())
        {
            Overlay(true);
            Overlay(false);
        }
    }
    else
    {
        wxMessageBox(wxString::Format(wxT("Can't Connect to Device %d\n%s"),
                                        index, GetDeviceName(index).c_str()),
                                        wxT("wxVideoCaptureWindowVFW Error"),
                                        wxOK|wxICON_EXCLAMATION|wxCENTRE);
    }

    return IsDeviceConnected();
}

bool wxVideoCaptureWindowVFW::DeviceDisconnect()
{
    if (m_capsingleframecount != -1) CaptureSingleFramesToFileClose();

    // turn OFF all callbacks first
    VFW_SetCallbackOnError(false);
    VFW_SetCallbackOnStatus(false);
    VFW_SetCallbackFrame(false);
    VFW_SetCallbackOnCaptureYield(false);
    VFW_SetCallbackOnCaptureControl(false);
    VFW_SetCallbackOnVideoStream(false);
    VFW_SetCallbackOnWaveStream(false);

    bool ok = 0!=capDriverDisconnect(m_hWndC);
    VFW_GetCAPSTATUS();

    Reset();

    return ok;
}

// ----------------------------------------------------------------------
// Display device's dialogs to set video characteristics
// ----------------------------------------------------------------------

bool wxVideoCaptureWindowVFW::HasVideoSourceDialog()
{
    if (IsDeviceConnected())
    {
        return 0!=m_CAPDRIVERCAPS.fHasDlgVideoSource;
    }
    return false;
}

void wxVideoCaptureWindowVFW::VideoSourceDialog()
{
    if (HasVideoSourceDialog())
    {
        capDlgVideoSource(m_hWndC);
        // you can change the device w/ this dialog so update everything
        VFW_GetCAPSTATUS();
        VFW_GetCAPDRIVERCAPS();
        VFW_GetCAPTUREPARMS();
        DoSizeWindow();
    }
}

bool wxVideoCaptureWindowVFW::HasVideoFormatDialog()
{
    if (IsDeviceConnected())
    {
        return 0!=m_CAPDRIVERCAPS.fHasDlgVideoFormat;
    }
    return false;
}

void wxVideoCaptureWindowVFW::VideoFormatDialog()
{
    if (HasVideoFormatDialog())
    {
        capDlgVideoFormat(m_hWndC);
        VFW_GetCAPSTATUS();              // update device status
        VFW_GetCAPDRIVERCAPS();
        VFW_GetCAPTUREPARMS();
        DoSizeWindow();
    }
}

void wxVideoCaptureWindowVFW::VideoCustomFormatDialog()
{
    if (IsDeviceConnected())
    {
        if (!FindWindow(IDD_wxVIDCAP_VIDEOFORMATDLG))
        {
            wxVideoCaptureWindowCustomVideoFormatDialog* dlg = new wxVideoCaptureWindowCustomVideoFormatDialog(this, IDD_wxVIDCAP_VIDEOFORMATDLG);
            dlg->Show();
        }
    }
}

bool wxVideoCaptureWindowVFW::HasVideoDisplayDialog()
{
    if (IsDeviceConnected())
    {
        return 0!=m_CAPDRIVERCAPS.fHasDlgVideoDisplay;
    }
    return false;
}

void wxVideoCaptureWindowVFW::VideoDisplayDialog()
{
    VFW_GetCAPDRIVERCAPS();
    if (HasVideoDisplayDialog())
    {
        capDlgVideoDisplay(m_hWndC);
        VFW_GetCAPSTATUS();              // update device status
        VFW_GetCAPDRIVERCAPS();
        VFW_GetCAPTUREPARMS();
        DoSizeWindow();
    }
}

void wxVideoCaptureWindowVFW::VideoCompressionDialog()
{
    capDlgVideoCompression(m_hWndC);
}

void wxVideoCaptureWindowVFW::CapturePreferencesDialog()
{
    if (!FindWindow(IDD_wxVIDCAP_CAPPREFDLG))
    {
        wxVideoCaptureWindowCapturePreferencesDialog dlg(this, IDD_wxVIDCAP_CAPPREFDLG);
        dlg.ShowModal();
    }
}

void wxVideoCaptureWindowVFW::AudioFormatDialog()
{
    if (HasAudioHardware())
    {
        if (!FindWindow(IDD_wxVIDCAP_AUDIOFORMATDLG))
        {
            wxVideoCaptureWindowAudioFormatDialog* dlg = new wxVideoCaptureWindowAudioFormatDialog(this, IDD_wxVIDCAP_AUDIOFORMATDLG);
            dlg->Show();
        }
    }
    else if (!HasAudioHardware())
        wxMessageBox(wxT("No audio hardware detected"), wxT("wxVideoCaptureWindow Error"),
                        wxOK|wxICON_EXCLAMATION|wxCENTRE);
}

void wxVideoCaptureWindowVFW::PropertiesDialog()
{
    wxDialog *dialog = new wxDialog(this, -1, wxT("wxVideoCaptureWindow Properties"),
                                    wxPoint(0,0), wxDefaultSize,
                                    wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);

    wxBoxSizer *dialogsizer = new wxBoxSizer( wxVERTICAL );
    wxTextCtrl *textctrl = new wxTextCtrl( dialog, -1, wxT(""),
                                           wxDefaultPosition, wxSize(400,400),
                                           wxTE_MULTILINE|wxTE_READONLY|wxHSCROLL);
    textctrl->SetFont(wxFont(10, wxMODERN, wxNORMAL, wxNORMAL));

    dialogsizer->Add( textctrl, 1, wxEXPAND);

    dialog->SetAutoLayout(true);
    dialog->SetSizer(dialogsizer);
    dialogsizer->Fit(dialog);
    dialogsizer->SetSizeHints(dialog);

    textctrl->AppendText(GetPropertiesString());

    dialog->Show(true);
}

wxString wxVideoCaptureWindowVFW::GetPropertiesString()
{
    wxString s;
    int i;

    s += wxT("Detected Devices:\n");
    s += wxString::Format(wxT("GetDeviceCount() : %d\n\n"), GetDeviceCount());
    for (i=0; i<GetDeviceCount(); i++)
    {
        s += GetDeviceName(i)+wxT("\n");
        s += GetDeviceVersion(i)+wxT("\n\n");
    }

    s += wxT("Currently connected device:\n");
    s += wxString::Format(wxT("GetDeviceIndex(): %d\n\n"), GetDeviceIndex());
    s += GetDeviceName() + wxT("\n");
    s += GetDeviceVersion()+wxT("\n\n");

    s += wxT("Video device properties:\n\n");

    s += wxString::Format(wxT("IsDeviceConnected()   : %d\n"), IsDeviceConnected());
    s += wxString::Format(wxT("IsDeviceInitialized() : %d\n\n"), IsDeviceInitialized());

    s += wxString::Format(wxT("HasVideoSourceDialog()  : %d\n"), HasVideoSourceDialog());
    s += wxString::Format(wxT("HasVideoFormatDialog()  : %d\n"), HasVideoFormatDialog());
    s += wxString::Format(wxT("HasVideoDisplayDialog() : %d\n\n"), HasVideoDisplayDialog());

    s += wxT("Video properties:\n\n");

    s += wxString::Format(wxT("GetImageWidth() x GetImageHeight() : %d x %d\n\n"), GetImageWidth(), GetImageHeight());

    int width, height, bpp;
    FOURCC fourcc;
    GetVideoFormat( &width, &height, &bpp, &fourcc );
    wxString fourccStr(FOURCCTowxString(fourcc));

    s += wxString::Format(wxT("GetVideoFormat() -> width x height : %d x %d\n"), width, height);
    s += wxString::Format(wxT("GetVideoFormat() -> bits per pixel : %d \n"), bpp);
    s += wxString::Format(wxT("GetVideoFormat() -> compression    : %s \n\n"), fourccStr.c_str());

    s += wxT("=============================================\n");

    s += wxString::Format(wxT("Video preview properties:\n\n"));

    s += wxString::Format(wxT("IsPreviewing()     : %d\n"), IsPreviewing());
    s += wxString::Format(wxT("IsPreviewScaled()  : %d\n"), IsPreviewScaled());
    s += wxString::Format(wxT("GetPreviewRateMS() : %d\n"), GetPreviewRateMS());

    s += wxString::Format(wxT("HasOverlay()   : %d\n"), HasOverlay());
    s += wxString::Format(wxT("IsOverlaying() : %d\n\n"), IsOverlaying());

    s += wxT("=============================================\n");
    s += wxString::Format(wxT("Video capture properties:\n\n"));

    s += wxString::Format(wxT("GetMicroSecPerFrameRequested() : %ld\n\n"), GetMicroSecPerFrameRequested());

    s += wxString::Format(wxT("GetAbortKey()        : %d\n"), GetAbortKey());
    s += wxString::Format(wxT("GetAbortLeftMouse()  : %d\n"), GetAbortLeftMouse());
    s += wxString::Format(wxT("GetAbortRightMouse() : %d\n"), GetAbortRightMouse());
    s += wxString::Format(wxT("GetTimeLimitedCapture()      : %d\n"), GetTimeLimitedCapture());
    s += wxString::Format(wxT("GetCaptureTimeLimit()        : %d\n"), GetCaptureTimeLimit());
    s += wxString::Format(wxT("GetDialogInitiatedCapture()  : %d\n"), GetDialogInitiatedCapture());
    s += wxString::Format(wxT("GetMaxAllowedFramesDropped() : %d\n\n"), GetMaxAllowedFramesDropped());

    s += wxString::Format(wxT("GetNumVideoBuffersAllocated()   : %d\n"), GetNumVideoBuffersAllocated());
    s += wxString::Format(wxT("GetNumVideoBuffers()            : %d\n"), GetNumVideoBuffers());
    s += wxString::Format(wxT("GetUseThreadToCapture()         : %d\n"), GetUseThreadToCapture());
    s += wxString::Format(wxT("GetStepCaptureAt2x()            : %d\n"), GetStepCaptureAt2x());
    s += wxString::Format(wxT("GetStepCaptureAverageFrames()   : %d\n"), GetStepCaptureAverageFrames());
    s += wxString::Format(wxT("GetAviMaxIndexEntries()         : %d\n"), GetAviMaxIndexEntries());
    s += wxString::Format(wxT("GetChunkGranularity()           : %d\n"), GetChunkGranularity());
    s += wxString::Format(wxT("GetCaptureTimeElapsedMS()       : %d\n"), GetCaptureTimeElapsedMS());
    s += wxString::Format(wxT("IsCapturingNow()                : %d\n"), IsCapturingNow());
    s += wxString::Format(wxT("GetCapturedVideoFramesCount()   : %d\n"), GetCapturedVideoFramesCount());
    s += wxString::Format(wxT("GetCapturedVideoFramesDropped() : %d\n\n"), GetCapturedVideoFramesDropped());

    s += wxString::Format(wxT("CaptureFileExists()  : %d\n"), CaptureFileExists());
    s += wxString::Format(wxT("GetCaptureFilename() : %s\n\n"), GetCaptureFilename().c_str());
//  s += wxString::Format(wxT("GetFreeDiskSpaceInKB() : %ld\n", GetFreeDiskSpaceInKB());
//  s += wxString::Format(wxT("GetFileSizeInKB() : %d\n\n", GetFileSizeInKB());

    s += wxT("=============================================\n");
    s += wxString::Format(wxT("Audio capture properties:\n\n"));

    s += wxString::Format(wxT("HasAudioHardware()            : %d\n"), HasAudioHardware());
    s += wxString::Format(wxT("GetCaptureAudio()             : %d\n"), GetCaptureAudio());
    s += wxString::Format(wxT("GetNumAudioBuffers()          : %d\n"), GetNumAudioBuffers());
    s += wxString::Format(wxT("GetNumAudioBuffersAllocated() : %d\n"), GetNumAudioBuffersAllocated());
    s += wxString::Format(wxT("GetAudioBufferSize()          : %ld\n"), GetAudioBufferSize());
    s += wxString::Format(wxT("GetAudioStreamMaster()        : %d\n"), GetAudioStreamMaster());
    s += wxString::Format(wxT("GetCapturedWaveSamplesCount() : %ld\n\n"), GetCapturedWaveSamplesCount());

    s += wxT("=============================================\n");
    s += wxString::Format(wxT("Palette properties:\n\n"));

    s += wxString::Format(wxT("DriverSuppliesPalettes() : %d\n"), DriverSuppliesPalettes());
    s += wxString::Format(wxT("IsUsingDefaultPalette()  : %d\n"), IsUsingDefaultPalette());

    s += wxT("\n");

    s += wxT("====================================================\n");
    s += wxT("Video For Windows structs - garbage if not connected\n");
    s += wxT("====================================================\n");

    s += wxString::Format(wxT("CAPDRIVERCAPS.wDeviceIndex            : %d\n"), (unsigned int)m_CAPDRIVERCAPS.wDeviceIndex);
    s += wxString::Format(wxT("CAPDRIVERCAPS.fHasOverlay             : %d\n"), (int)m_CAPDRIVERCAPS.fHasOverlay);
    s += wxString::Format(wxT("CAPDRIVERCAPS.fHasDlgVideoSource      : %d\n"), (int)m_CAPDRIVERCAPS.fHasDlgVideoSource);
    s += wxString::Format(wxT("CAPDRIVERCAPS.fHasDlgVideoFormat      : %d\n"), (int)m_CAPDRIVERCAPS.fHasDlgVideoFormat);
    s += wxString::Format(wxT("CAPDRIVERCAPS.fHasDlgVideoDisplay     : %d\n"), (int)m_CAPDRIVERCAPS.fHasDlgVideoDisplay);
    s += wxString::Format(wxT("CAPDRIVERCAPS.fCaptureInitialized     : %d\n"), (int)m_CAPDRIVERCAPS.fCaptureInitialized);
    s += wxString::Format(wxT("CAPDRIVERCAPS.fDriverSuppliesPalettes : %d\n"), (int)m_CAPDRIVERCAPS.fDriverSuppliesPalettes);
    s += wxString::Format(wxT("CAPDRIVERCAPS.hVideoIn         !Win32 : %d\n"), (int)m_CAPDRIVERCAPS.hVideoIn);
    s += wxString::Format(wxT("CAPDRIVERCAPS.hVideoOut        !Win32 : %d\n"), (int)m_CAPDRIVERCAPS.hVideoOut);
    s += wxString::Format(wxT("CAPDRIVERCAPS.hVideoExtIn      !Win32 : %d\n"), (int)m_CAPDRIVERCAPS.hVideoExtIn);
    s += wxString::Format(wxT("CAPDRIVERCAPS.hVideoExtOut     !Win32 : %d\n"), (int)m_CAPDRIVERCAPS.hVideoExtOut);

    s += wxT("\n");

    s += wxString::Format(wxT("CAPSTATUS.uiImageWidth                : %d\n"), (unsigned int)m_CAPSTATUS.uiImageWidth);
    s += wxString::Format(wxT("CAPSTATUS.uiImageHeight               : %d\n"), (unsigned int)m_CAPSTATUS.uiImageHeight);
    s += wxString::Format(wxT("CAPSTATUS.fLiveWindow                 : %d\n"), (int)m_CAPSTATUS.fLiveWindow);
    s += wxString::Format(wxT("CAPSTATUS.fOverlayWindow              : %d\n"), (int)m_CAPSTATUS.fOverlayWindow);
    s += wxString::Format(wxT("CAPSTATUS.fScale                      : %d\n"), (int)m_CAPSTATUS.fScale);
    s += wxString::Format(wxT("CAPSTATUS.ptScroll                    : %d %d\n"), (int)m_CAPSTATUS.ptScroll.x, (int)m_CAPSTATUS.ptScroll.y);
    s += wxString::Format(wxT("CAPSTATUS.fUsingDefaultPalette        : %d\n"), (int)m_CAPSTATUS.fUsingDefaultPalette);
    s += wxString::Format(wxT("CAPSTATUS.fAudioHardware              : %d\n"), (int)m_CAPSTATUS.fAudioHardware);
    s += wxString::Format(wxT("CAPSTATUS.fCapFileExists              : %d\n"), (int)m_CAPSTATUS.fCapFileExists);
    s += wxString::Format(wxT("CAPSTATUS.dwCurrentVideoFrame         : %ld\n"), (long)m_CAPSTATUS.dwCurrentVideoFrame);
    s += wxString::Format(wxT("CAPSTATUS.dwCurrentVideoFramesDropped : %ld\n"), (long)m_CAPSTATUS.dwCurrentVideoFramesDropped);
    s += wxString::Format(wxT("CAPSTATUS.dwCurrentWaveSamples        : %ld\n"), (long)m_CAPSTATUS.dwCurrentWaveSamples);
    s += wxString::Format(wxT("CAPSTATUS.dwCurrentTimeElapsedMS      : %ld\n"), (long)m_CAPSTATUS.dwCurrentTimeElapsedMS);
    s += wxString::Format(wxT("CAPSTATUS.hPalCurrent                 : Not printed\n"));
    s += wxString::Format(wxT("CAPSTATUS.fCapturingNow               : %d\n"), (int)m_CAPSTATUS.fCapturingNow);
    s += wxString::Format(wxT("CAPSTATUS.dwReturn                    : %ld\n"), (long)m_CAPSTATUS.dwReturn);
    s += wxString::Format(wxT("CAPSTATUS.wNumVideoAllocated          : %d\n"), (unsigned int)m_CAPSTATUS.wNumVideoAllocated);
    s += wxString::Format(wxT("CAPSTATUS.wNumAudioAllocated          : %d\n"), (unsigned int)m_CAPSTATUS.wNumAudioAllocated);

    s += wxT("\n");

    s += wxString::Format(wxT("CAPTUREPARMS.dwRequestMicroSecPerFrame : %ld\n"), (long)m_CAPTUREPARMS.dwRequestMicroSecPerFrame);
    s += wxString::Format(wxT("CAPTUREPARMS.fMakeUserHitOKToCapture   : %d\n"), (int)m_CAPTUREPARMS.fMakeUserHitOKToCapture);
    s += wxString::Format(wxT("CAPTUREPARMS.wPercentDropForError      : %d\n"), (unsigned int)m_CAPTUREPARMS.wPercentDropForError);
    s += wxString::Format(wxT("CAPTUREPARMS.fYield                    : %d\n"), (int)m_CAPTUREPARMS.fYield);
    s += wxString::Format(wxT("CAPTUREPARMS.dwIndexSize               : %ld\n"), (long)m_CAPTUREPARMS.dwIndexSize);
    s += wxString::Format(wxT("CAPTUREPARMS.wChunkGranularity         : %d\n"), (unsigned int)m_CAPTUREPARMS.wChunkGranularity);
    s += wxString::Format(wxT("CAPTUREPARMS.fUsingDOSMemory    !Win32 : %d\n"), (int)m_CAPTUREPARMS.fUsingDOSMemory);
    s += wxString::Format(wxT("CAPTUREPARMS.wNumVideoRequested        : %d\n"), (unsigned int)m_CAPTUREPARMS.wNumVideoRequested);
    s += wxString::Format(wxT("CAPTUREPARMS.fCaptureAudio             : %d\n"), (int)m_CAPTUREPARMS.fCaptureAudio);
    s += wxString::Format(wxT("CAPTUREPARMS.wNumAudioRequested        : %d\n"), (unsigned int)m_CAPTUREPARMS.wNumAudioRequested);
    s += wxString::Format(wxT("CAPTUREPARMS.vKeyAbort                 : %d\n"), (unsigned int)m_CAPTUREPARMS.vKeyAbort);
    s += wxString::Format(wxT("CAPTUREPARMS.fAbortLeftMouse           : %d\n"), (int)m_CAPTUREPARMS.fAbortLeftMouse);
    s += wxString::Format(wxT("CAPTUREPARMS.fAbortRightMouse          : %d\n"), (int)m_CAPTUREPARMS.fAbortRightMouse);
    s += wxString::Format(wxT("CAPTUREPARMS.fLimitEnabled             : %d\n"), (int)m_CAPTUREPARMS.fLimitEnabled);
    s += wxString::Format(wxT("CAPTUREPARMS.wTimeLimit                : %d\n"), (unsigned int)m_CAPTUREPARMS.wTimeLimit);
    s += wxString::Format(wxT("CAPTUREPARMS.fMCIControl               : %d\n"), (int)m_CAPTUREPARMS.fMCIControl);
    s += wxString::Format(wxT("CAPTUREPARMS.fStepMCIDevice            : %d\n"), (int)m_CAPTUREPARMS.fStepMCIDevice);
    s += wxString::Format(wxT("CAPTUREPARMS.dwMCIStartTime            : %ld\n"), (long)m_CAPTUREPARMS.dwMCIStartTime);
    s += wxString::Format(wxT("CAPTUREPARMS.dwMCIStopTime             : %ld\n"), (long)m_CAPTUREPARMS.dwMCIStopTime);
    s += wxString::Format(wxT("CAPTUREPARMS.fStepCaptureAt2x          : %d\n"), (int)m_CAPTUREPARMS.fStepCaptureAt2x);
    s += wxString::Format(wxT("CAPTUREPARMS.wStepCaptureAverageFrames : %d\n"), (unsigned int)m_CAPTUREPARMS.wStepCaptureAverageFrames);
    s += wxString::Format(wxT("CAPTUREPARMS.dwAudioBufferSize         : %ld\n"), (long)m_CAPTUREPARMS.dwAudioBufferSize);
    s += wxString::Format(wxT("CAPTUREPARMS.fDisableWriteCache !Win32 : %d\n"), (int)m_CAPTUREPARMS.fDisableWriteCache);
    s += wxString::Format(wxT("CAPTUREPARMS.AVStreamMaster            : %d\n"), (unsigned int)m_CAPTUREPARMS.AVStreamMaster);

    return s;
}

// ----------------------------------------------------------------------
// Video characteristics and manipulation
// ----------------------------------------------------------------------

bool wxVideoCaptureWindowVFW::GetVideoFormat(int *width, int *height, int *bpp, FOURCC *fourcc)
{
    if (IsDeviceConnected())
    {
        int bmpformatsize = capGetVideoFormatSize(m_hWndC);
        if (bmpformatsize)
        {
            BITMAPINFO *lpbmpinfo = (BITMAPINFO*)(new char[bmpformatsize]);
            bmpformatsize = capGetVideoFormat(m_hWndC, lpbmpinfo, (WORD)bmpformatsize);

            if (width)  *width = lpbmpinfo->bmiHeader.biWidth;
            if (height) *height = lpbmpinfo->bmiHeader.biHeight;
            if (bpp)    *bpp = lpbmpinfo->bmiHeader.biBitCount;
            if (fourcc) *fourcc = lpbmpinfo->bmiHeader.biCompression;

            delete []lpbmpinfo;

            return true;
        }
    }

    if (width)  *width = 0;
    if (height) *height = 0;
    if (bpp)    *bpp = 0;
    if (fourcc) *fourcc = wxNullFOURCC;
    return false;
}

bool wxVideoCaptureWindowVFW::SetVideoFormat( int width, int height,
                                              int bpp, FOURCC format )
{
    // check for valid input, some heights can be negative, MS is
    //    wishy-washy on it, lets just say they're invalid
    if ((width < -1) || (height < -1) || (bpp < -1)) return false;

    // get current preview/overlay state so we can stop it for now
    // this really helps avoid crashes!
    bool preview = IsPreviewing() && !m_preview_wximage;
    bool preview_wximage = m_preview_wximage;
    bool overlay = IsOverlaying();
    Preview(false);
    Overlay(false);

    // get the current setting, also serves as a sanity check
    int c_bmpformatsize = capGetVideoFormatSize(m_hWndC);
    if (c_bmpformatsize == 0) return false;

    // info and values for current BITMAPINFO structure
    int c_width, c_height, c_bpp;   // current values
    FOURCC c_fourcc = 0;
    GetVideoFormat( &c_width, &c_height, &c_bpp, &c_fourcc );
    wxString c_fourcc_str(FOURCCTowxString(c_fourcc));

    BITMAPINFO *c_bmpinfo = (BITMAPINFO*)malloc(c_bmpformatsize);
    c_bmpformatsize = capGetVideoFormat(m_hWndC, c_bmpinfo, c_bmpformatsize);

    // data structures for a whole new BITMAPINFO strucure
    BITMAPINFO *new_bmpinfo = NULL;     // new structure to create if needed
    int new_rgbquad = 0;                // # of palette entries, NOT USED

    // pointers and values of which of the two structures we'll actually use
    BITMAPINFO *lpbmpinfo = NULL;     // pointer to bmpinfo structure to use
    int lpbmpinfosize;                // size of used structure
    bool usecurrent_bmpheader = true; // use parts of the current header

    // if just changing the size then preserve the palette (if any)...
    if (((c_fourcc == format)&&(c_bpp == bpp))||((format == -1)&&(bpp == -1)))
    {
        lpbmpinfo = c_bmpinfo;      // point to current header
        lpbmpinfosize = c_bmpformatsize;
        usecurrent_bmpheader = true;
    }
    else
    {
        new_bmpinfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER) +
                        new_rgbquad*sizeof(RGBQUAD));
        lpbmpinfo = new_bmpinfo;
        lpbmpinfosize = sizeof(BITMAPINFOHEADER) + new_rgbquad*sizeof(RGBQUAD);
        usecurrent_bmpheader = false;
    }

    lpbmpinfo->bmiHeader.biSize = lpbmpinfosize;

    if (width == -1) width = c_width;
    lpbmpinfo->bmiHeader.biWidth = width;

    if (height == -1) height = c_height;
    lpbmpinfo->bmiHeader.biHeight = height;

    // this needs to be set for Pinnacle TV cards, sometimes
    lpbmpinfo->bmiHeader.biPlanes = 1;  // always 1
    // these might as well be set in case junk comes back
    lpbmpinfo->bmiHeader.biClrUsed = 0; // all colors used
    lpbmpinfo->bmiHeader.biClrImportant = 0; // all colors
    lpbmpinfo->bmiHeader.biXPelsPerMeter = 72;
    lpbmpinfo->bmiHeader.biYPelsPerMeter = 72;

    if (!usecurrent_bmpheader)
    {
        lpbmpinfo->bmiHeader.biBitCount = bpp;
        lpbmpinfo->bmiHeader.biCompression = format;
    }

    lpbmpinfo->bmiHeader.biSizeImage = lpbmpinfo->bmiHeader.biHeight *
        ((lpbmpinfo->bmiHeader.biWidth * lpbmpinfo->bmiHeader.biBitCount + 31) / 32) *
        4 * lpbmpinfo->bmiHeader.biPlanes;

    // THIS IS NOT USED/FINISHED needed?
/*  if (!usecurrent_bmpheader && new_rgbquad)
    {
        int i;
        for (i=0; i<new_rgbquad; i++)
        {
            lpbmpinfo->bmiColors[i].rgbRed = i;
            lpbmpinfo->bmiColors[i].rgbGreen = i;
            lpbmpinfo->bmiColors[i].rgbBlue = i;
        }
    }   */

    bool formatset = 0!=capSetVideoFormat(m_hWndC, (LPBITMAPINFO)lpbmpinfo,
                                             (WORD)lpbmpinfosize);

    if (c_bmpinfo) free(c_bmpinfo);
    if (new_bmpinfo) free(new_bmpinfo);

    if (formatset) DoSizeWindow();

    // return to previous state
    if (preview_wximage) Preview( true, true );
    else if (preview) Preview( true, false );
    else if (overlay) Overlay( true );

    return formatset;
}

// true if driver can create palettes
bool wxVideoCaptureWindowVFW::DriverSuppliesPalettes()
{
    return 0!=m_CAPDRIVERCAPS.fDriverSuppliesPalettes;
}


// convert the DDB "bitmap" to a 24bpp bitmap that we can deal with
// stick it into the m_wximage
bool wxVideoCaptureWindowVFW::VFW_DDBtoDIB(LPVIDEOHDR lpVHdr)
{
    // a video device can pump out whatever it feels like
    // YUVxyz, JBYR, ABYR, etc etc... 20 bit formats... :(

    // the absolute last resort to get the data in a useable form
    // copy the pixels from the screen: HDC hdc; hdc = GetDC(m_hWndC);
    // for x to width for y to height; array[x][y] = GetPixel(hdc,x,y);

    // an equally lame way to get video, copy it to the clipboard
    //  if (capEditCopy(m_hWndC))
    //      if (wxTheClipboard->Open()) {
    //          if (wxTheClipboard->IsSupported(wxDF_BITMAP)) {
    //              wxBitmapDataObject bmpdataobj;
    //              wxTheClipboard->GetData( bmpdataobj );
    //              wximage.Destroy();
    //              wximage = wxImage(bmpdataobj.GetBitmap());
    //          }
    //          wxTheClipboard->Close();
    //      }

    static int bmpwidth, bmpheight;     // the size from the BITMAPINFO header
    static long int bmpdatasize;        // bytes of image data from device
    static int bmpformatsize = 0, bmpformatsize_last = 1; // BITMAPINFO size
    static int bmppadding24bpp;         // # bytes to pad to 4byte boundary

    // I don't know is this is necessary, but without it I get crashes when
    // the format dialog is pulled up and wximage previewing is active,
    // it could merely be driver problems :) is this multithreaded? just dunno
    m_getting_wximage = true; // don't draw wximage when filling it

    // Start by checking to see if the format has changed
    // note: some drivers will correctly call the status callback,
    // at least one (mine) doesn't in win95, so always compare, else crash
    bmpformatsize = capGetVideoFormatSize(m_hWndC);

    // this is probably always true as the bmp header is 40 bytes
    // but, we need to set it up at least once
    if (bmpformatsize != bmpformatsize_last)
    {
        // recreate the current bitmap header for this video
        if (m_lpBmpInfo) delete []m_lpBmpInfo;
        m_lpBmpInfo = (BITMAPINFO*)(new char[bmpformatsize]);
        // recreate the last bitmap header for this video
        if (m_lpBmpInfoLast) delete []m_lpBmpInfoLast;
        m_lpBmpInfoLast = (BITMAPINFO*)(new char[bmpformatsize]);
        // now make 'em different so we'll recreate the rest
        ((LPBITMAPINFOHEADER)m_lpBmpInfoLast)->biWidth = 9;

        bmpformatsize_last = bmpformatsize;
    }

    // get the header for the video frame's BITMAP data
    // again, unfortunately always need to do this, see above...
    bmpformatsize = capGetVideoFormat(m_hWndC, m_lpBmpInfo, bmpformatsize);

    bmpwidth = m_lpBmpInfo->bmiHeader.biWidth;
    bmpheight = m_lpBmpInfo->bmiHeader.biHeight;
    // bmpdatasize = lpBmpInfo->bmiHeader.biSize;
    bmpdatasize = lpVHdr->dwBytesUsed;  // lets trust this size :)
    // extra bytes (per row) needed to pad each row to a 4 byte boundary
    bmppadding24bpp = (bmpwidth*3) % 4;

    // does the last bmpheader == this one? if not size/compresson has changed
    if ( memcmp(m_lpBmpInfo, m_lpBmpInfoLast, bmpformatsize) != 0 )
    {
        // set up the 24bpp bitmap header (the output format we want)
        if (m_lpBmpInfo24bpp) delete []m_lpBmpInfo24bpp;
        m_lpBmpInfo24bpp = (BITMAPINFO*)(new char[bmpformatsize]);
        bmpformatsize = capGetVideoFormat(m_hWndC, m_lpBmpInfo24bpp, bmpformatsize);
        m_lpBmpInfo24bpp->bmiHeader.biBitCount = 24;        // we like 24 bpp
        m_lpBmpInfo24bpp->bmiHeader.biClrImportant = 0;     // all important
        m_lpBmpInfo24bpp->bmiHeader.biClrUsed = 0;          // no colormap
        m_lpBmpInfo24bpp->bmiHeader.biCompression = BI_RGB; // = 0
        m_lpBmpInfo24bpp->bmiHeader.biPlanes = 1;           // always 1
        m_lpBmpInfo24bpp->bmiHeader.biSizeImage = bmpwidth*bmpheight*3 +
                                                  bmppadding24bpp*bmpheight;

        // recreate the temp storage variable, tack on extra? sure...
        if (m_bmpdata) delete []m_bmpdata;
        m_bmpdata = new unsigned char[bmpwidth*bmpheight*3 +
                                      bmppadding24bpp*bmpheight + 1024];

        // setup the output structure
        if (!m_wximage.Ok() || (m_wximage.GetWidth() != bmpwidth) ||
                               (m_wximage.GetHeight() != bmpheight))
        {
            m_wximage.Create(bmpwidth, bmpheight);
        }

        // get a handle to an appropriate video (de)compressor
        if (m_hic_compressor) ICClose(m_hic_compressor);
        // first param of ICLocate is ICTYPE_VIDEO or ('VIDC' in docs)
        m_hic_compressor = ICLocate(ICTYPE_VIDEO, NULL,
                                    (LPBITMAPINFOHEADER)m_lpBmpInfo,
                                    (LPBITMAPINFOHEADER)m_lpBmpInfo24bpp,
                                    ICMODE_DECOMPRESS );

        memcpy(m_lpBmpInfoLast, m_lpBmpInfo, bmpformatsize);
    }

    static LRESULT icdecomp;            // result of the (de)compressor
    static LRESULT icdresult;

    // uncompress the video image and fill a temp buffer w/ 24bpp bitmap data
    if (m_hic_compressor && m_lpBmpInfo24bpp && m_bmpdata )
    {
        // apparently this is necessary, but the docs don't say much about it
        icdresult = ICDecompressBegin( m_hic_compressor,
                                      (LPBITMAPINFOHEADER)m_lpBmpInfo,
                                      (LPBITMAPINFOHEADER)m_lpBmpInfo24bpp);
        if (icdresult == ICERR_OK)
            icdecomp = ICDecompress( m_hic_compressor,
                                    //ICDECOMPRESS_HURRYUP,
                                    (lpVHdr->dwFlags & VHDR_KEYFRAME ? AVIIF_KEYFRAME : 0),
                                    (LPBITMAPINFOHEADER)m_lpBmpInfo, lpVHdr->lpData,
                                    (LPBITMAPINFOHEADER)m_lpBmpInfo24bpp, m_bmpdata);

        icdresult = ICDecompressEnd( m_hic_compressor );
    }

    // write 24bpp bitmap data to a wximage
    if (m_bmpdata && m_wximage.Ok())
    {
        int rowpos, rowsize = 3*bmpwidth+bmppadding24bpp;
        register int i, j;
        unsigned char *imgptr = m_wximage.GetData();

        // this format is not upside down
        if (m_lpBmpInfo->bmiHeader.biCompression == FOURCC(STRING_TO_FOURCC("Y41P")))
        {
            for (j=0; j<bmpheight; j++)
            {
                rowpos = j*rowsize;
                for (i=0; i<bmpwidth*3; i+=3)
                {
                    *imgptr++ = m_bmpdata[rowpos + i+2];
                    *imgptr++ = m_bmpdata[rowpos + i+1];
                    *imgptr++ = m_bmpdata[rowpos + i];
                }
            }
        }
        else
        {
            for (j=bmpheight-1; j>=0; j--)
            {
                rowpos = j*rowsize;
                for (i=0; i<bmpwidth*3; i+=3)
                {
                    *imgptr++ = m_bmpdata[rowpos + i+2];
                    *imgptr++ = m_bmpdata[rowpos + i+1];
                    *imgptr++ = m_bmpdata[rowpos + i];
                }
            }
        }
    }

    m_getting_wximage = false;          // done filling wximage

    return true;
}

// ----------------------------------------------------------------------
// Capture Preview and Overlay
// ----------------------------------------------------------------------

bool wxVideoCaptureWindowVFW::Preview(bool on, bool wxpreview)
{
    bool previewingOK = false;

    VFW_GetCAPSTATUS();

    // note: VFW automatically turns off Overlaying, do it anyway
    if (on && IsOverlaying()) Overlay(false);

    // turn any type of previewing off
    if (!on)
    {
        m_preview_wximage_timer.Stop();
        VFW_SetCallbackFrame(false);
        previewingOK = (0 != capPreview(m_hWndC, (BOOL)false)); // true if set correctly
        m_preview_wximage = false;
        m_previewing = false;
        ShowWindow(m_hWndC, SW_SHOW); // for snapshot to window
        return previewingOK;
    }

    VFW_SetCallbackFrame(true);

    // use the VFW preview capabilities
    if (wxpreview == false)
    {
        m_preview_wximage_timer.Stop();
        m_preview_wximage = false;
        ShowWindow(m_hWndC, SW_SHOW);
        DoSizeWindow();
        previewingOK = (0 != capPreview(m_hWndC, (BOOL)on)); // true if set correctly
        if (!previewingOK)
        {
            VFW_SetCallbackFrame(false);
            wxMessageBox(wxT("Can't Set Preview"), wxT("wxVideoCaptureWindow Error"), wxOK|wxICON_EXCLAMATION|wxCENTRE);
        }
    }
    // use the callback and OnIdle/OnPreviewxImageTimer to preview using wxImages
    else
    {
        m_preview_wximage = true;
        ShowWindow(m_hWndC, SW_HIDE);
        DoSizeWindow();
        previewingOK = true; // sure why wouldn't it :)
#if !USE_PREVIEW_wxIMAGE_TIMER
        m_preview_wximage_timer.Start(m_previewmsperframe);
#endif
    }

    VFW_GetCAPSTATUS();

    return previewingOK;
}

void wxVideoCaptureWindowVFW::OnPreviewwxImageTimer(wxTimerEvent& event)
{
    static unsigned long int last_frame = (unsigned long int)(-1);

    if (m_preview_wximage)
    {
        // last message has to be processed before we call again
        if (!m_getting_wximage && (last_frame != m_framenumber) && IsDeviceConnected())
        {
            last_frame = m_framenumber;
            capGrabFrameNoStop(m_hWndC);
        }
    }
    event.Skip();
}

bool wxVideoCaptureWindowVFW::PreviewScaled(bool scale)
{
    bool scalingOK;
    VFW_GetCAPSTATUS();

    // set the window up the way we want it first
    if (scale)
    {
        SetScrollbars(1, 1, 2, 2, 0, 0, false); // make them go away
        m_clientSize = GetClientSize();
        MoveWindow(m_hWndC, 0, 0, m_clientSize.x, m_clientSize.y, false);
    }

    scalingOK = (0 != capPreviewScale(m_hWndC, scale));
    m_previewscaled = scale;

    VFW_GetCAPSTATUS();
    DoSizeWindow();     // clean it up, readjust the scollbars

    return scalingOK;
}

bool wxVideoCaptureWindowVFW::SetPreviewRateMS( unsigned int msperframe )
{
    wxVideoCaptureWindowBase::DoSetPreviewRateMS(msperframe);

    bool setrateOK = (0 != capPreviewRate(m_hWndC, msperframe));
    if (setrateOK) m_previewmsperframe = msperframe;

#if !USE_PREVIEW_wxIMAGE_TIMER
    if (m_preview_wximage)
    {
        m_preview_wximage_timer.Stop();
        m_preview_wximage_timer.Start(m_previewmsperframe);
    }
#endif

    VFW_GetCAPSTATUS();
    return setrateOK;
}

bool wxVideoCaptureWindowVFW::Overlay(bool on)
{
    VFW_GetCAPDRIVERCAPS();
    if (!HasOverlay()) return false;

    // note: VFW automatically turns off it's software preview, but...
    if (on && IsPreviewing()) Preview(false); // turn off software preview

    bool overlayingOK = (0 != capOverlay(m_hWndC, on));
    VFW_GetCAPSTATUS();
    return overlayingOK;
}

// ----------------------------------------------------------------------
// Capture single frames, take snapshots of streaming video
// ----------------------------------------------------------------------

bool wxVideoCaptureWindowVFW::SnapshotToWindow()
{
    Preview(false);

    bool grabbedOK = (0 != capGrabFrame(m_hWndC));
    return (grabbedOK);
}

bool wxVideoCaptureWindowVFW::SnapshotToClipboard()
{
    bool previewing = IsPreviewing();
    bool overlaying = IsOverlaying();

    bool grabbedOK;
    if (previewing || overlaying)
        grabbedOK = (0 != capGrabFrameNoStop(m_hWndC));
    else
        grabbedOK = (0 != capGrabFrame(m_hWndC));

    bool editcopyOK = (0 != capEditCopy(m_hWndC));

    return (grabbedOK && editcopyOK);
}

bool wxVideoCaptureWindowVFW::SnapshotToBMP( const wxString &filename )
{
    return (0 != capFileSaveDIB( m_hWndC, filename.fn_str() ));
}

bool wxVideoCaptureWindowVFW::SnapshotTowxImage( wxImage &image )
{
    bool grabbedOK = SnapshotTowxImage();

    if (m_wximage.Ok() && !m_grab_wximage && !m_getting_wximage && grabbedOK)
    {
        // maybe just ref it ?
        image = m_wximage;

/*
        int width = m_wximage.GetWidth();
        int height = m_wximage.GetHeight();

        if (!image.Ok() || (image.GetWidth() != width) ||
                           (image.GetHeight() != height))
        {
            image.Create( width, height );
        }

        unsigned char *m_wximgptr = m_wximage.GetData();
        unsigned char *imgptr = image.GetData();

        memcpy(imgptr, m_wximgptr, width*height*3);
*/
    }

    return grabbedOK;
}

bool wxVideoCaptureWindowVFW::SnapshotTowxImage()
{
    bool previewing = IsPreviewing();

    bool grabbedOK = false;

    m_grab_wximage = true; // fill the m_wximage in callback

    // don't want to mess up callback for preview
    if (previewing)
    {
        //grabbedOK = (bool)capGrabFrameNoStop(m_hWndC);
        // wait for message to be processed first, max 4000 milliseconds
        // I think this is necessary, but could just be driver problems
        grabbedOK = 0!=SendMessageTimeoutA(m_hWndC, WM_CAP_GRAB_FRAME_NOSTOP,
                                        (WPARAM)0, (LPARAM)0L,
                                        SMTO_BLOCK, 4000,
#if _MSC_VER > 1200 // works in 2008, but I'm guessing it was around earlier
                                        (PDWORD_PTR)0 ); // (LPDWORD)0 );
#else
                                        (LPDWORD)0 );
#endif
    }
    else
    {
        VFW_SetCallbackFrame(true);
        //grabbedOK = (bool)capGrabFrame(m_hWndC);
        grabbedOK = 0!=SendMessageTimeoutA(m_hWndC, WM_CAP_GRAB_FRAME,
                                        (WPARAM)0, (LPARAM)0L,
                                        SMTO_BLOCK, 4000,
#if _MSC_VER > 1200 // works in 2008, but I'm guessing it was around earlier
                                        (PDWORD_PTR)0 ); // (LPDWORD)0 );
#else
                                        (LPDWORD)0 );
#endif

        VFW_SetCallbackFrame(false);
    }

    return grabbedOK;
}

// ----------------------------------------------------------------------
// Capture (append) single video frames to an AVI file
// ----------------------------------------------------------------------

bool wxVideoCaptureWindowVFW::CaptureSingleFramesToFileOpen()
{
    wxCHECK_MSG(m_capsingleframecount == -1, false, wxT("Currently capturing frames, can't restart."));

    bool openedOK = (0 != capCaptureSingleFrameOpen(m_hWndC));
    m_capsingleframecount = 0;
    return openedOK;
}

bool wxVideoCaptureWindowVFW::CaptureSingleFramesToFile()
{
    wxCHECK_MSG(m_capsingleframecount != -1, false, wxT("Can't capture frames, open AVI file first."));

    bool capturedOK = (0 != capCaptureSingleFrame(m_hWndC));
    if (capturedOK) m_capsingleframecount++;
    return  capturedOK;
}

bool wxVideoCaptureWindowVFW::CaptureSingleFramesToFileClose()
{
    if (m_capsingleframecount == -1) return false;

    capCaptureSingleFrameClose(m_hWndC);
    m_capsingleframecount = -1;
    return false;
}

void wxVideoCaptureWindowVFW::CaptureSingleFramesToFileDialog()
{
    if (IsDeviceConnected() && !FindWindow(IDD_wxVIDCAP_CAPSNGFRAMESDLG))
    {
        new wxVideoCaptureWindowCaptureSingleFramesDialog(this, IDD_wxVIDCAP_CAPSNGFRAMESDLG);
    }
}

// ----------------------------------------------------------------------
// Capture streaming video to a file
// ----------------------------------------------------------------------

unsigned long int wxVideoCaptureWindowVFW::GetMicroSecPerFrameRequested()
{
    VFW_GetCAPTUREPARMS();
    return (unsigned long)(m_CAPTUREPARMS.dwRequestMicroSecPerFrame);
}
void wxVideoCaptureWindowVFW::SetMicroSecPerFrameRequested( unsigned long int framespersec)
{
    m_CAPTUREPARMS.dwRequestMicroSecPerFrame = (DWORD)(framespersec);
    VFW_SetCAPTUREPARMS();
}

// keycode to terminate streaming video, default = VK_ESCAPE
// NOTE use RegisterHotKey() for system wide key, NOT IMPLEMENTED, NOT TESTED
unsigned int wxVideoCaptureWindowVFW::GetAbortKey()
{
    VFW_GetCAPTUREPARMS();
    return m_CAPTUREPARMS.vKeyAbort;
}
void wxVideoCaptureWindowVFW::SetAbortKey( unsigned int key )
{
    m_CAPTUREPARMS.vKeyAbort = (UINT)key; VFW_SetCAPTUREPARMS();
    // TODO RegisterHotKey(m_hWndC,
}
// abort video capture on left mouse button, default = true
bool wxVideoCaptureWindowVFW::GetAbortLeftMouse()
{
    VFW_GetCAPTUREPARMS();
    return (0 != m_CAPTUREPARMS.fAbortLeftMouse);
}
void wxVideoCaptureWindowVFW::SetAbortLeftMouse( bool leftmouse )
{
    m_CAPTUREPARMS.fAbortLeftMouse = (BOOL)leftmouse;
    VFW_SetCAPTUREPARMS();
}
// abort video capture on right mouse button, default = true
bool wxVideoCaptureWindowVFW::GetAbortRightMouse()
{
    VFW_GetCAPTUREPARMS();
    return (0 != m_CAPTUREPARMS.fAbortRightMouse);
}
void wxVideoCaptureWindowVFW::SetAbortRightMouse( bool rightmouse )
{
    m_CAPTUREPARMS.fAbortRightMouse = (BOOL)rightmouse;
    VFW_SetCAPTUREPARMS();
}

// only capture for a fixed number of seconds
bool wxVideoCaptureWindowVFW::GetTimeLimitedCapture()
{
    VFW_GetCAPTUREPARMS();
    return (0 != m_CAPTUREPARMS.fLimitEnabled);
}
void wxVideoCaptureWindowVFW::SetTimeLimitedCapture( bool usetimelimit )
{
    m_CAPTUREPARMS.fLimitEnabled = (BOOL)usetimelimit;
    VFW_SetCAPTUREPARMS();
}
// number of seconds to capture video for
unsigned int wxVideoCaptureWindowVFW::GetCaptureTimeLimit()
{
    VFW_GetCAPTUREPARMS();
    return m_CAPTUREPARMS.wTimeLimit;
}
void wxVideoCaptureWindowVFW::SetCaptureTimeLimit( unsigned int timelimit )
{
    m_CAPTUREPARMS.wTimeLimit = (UINT)timelimit;
    VFW_SetCAPTUREPARMS();
}

// popup "OK" dialog to initiate capture, default = false
bool wxVideoCaptureWindowVFW::GetDialogInitiatedCapture()
{
    VFW_GetCAPTUREPARMS();
    return (0 != m_CAPTUREPARMS.fMakeUserHitOKToCapture);
}
void wxVideoCaptureWindowVFW::SetDialogInitiatedCapture( bool usedialog )
{
    m_CAPTUREPARMS.fMakeUserHitOKToCapture = (BOOL)usedialog;
    VFW_SetCAPTUREPARMS();
}

// max allowed % of frames dropped, default=10 [0..100], popup error dialog
// note: dropped frames are replaced by the last grabbed frame in AVI file
unsigned int wxVideoCaptureWindowVFW::GetMaxAllowedFramesDropped()
{
    VFW_GetCAPTUREPARMS();
    return m_CAPTUREPARMS.wPercentDropForError;
}
void wxVideoCaptureWindowVFW::SetMaxAllowedFramesDropped(unsigned int maxdrop )
{
    m_CAPTUREPARMS.wPercentDropForError = (UINT)maxdrop;
    VFW_SetCAPTUREPARMS();
}

// The default values are probably fine, but who knows?
// actual number of video buffers allocated in the memory heap
unsigned int wxVideoCaptureWindowVFW::GetNumVideoBuffersAllocated()
{
    VFW_GetCAPSTATUS();
    return m_CAPSTATUS.wNumVideoAllocated;
}
// max number of video buffers to allocate, if 0 then autocalc, dunno?
unsigned int wxVideoCaptureWindowVFW::GetNumVideoBuffers()
{
    VFW_GetCAPTUREPARMS();
    return m_CAPTUREPARMS.wNumVideoRequested;
}
void wxVideoCaptureWindowVFW::SetNumVideoBuffers( unsigned int vidbufs )
{
    m_CAPTUREPARMS.wNumVideoRequested = (UINT)vidbufs;
    VFW_SetCAPTUREPARMS();
}

// use background thread to capture, controls NOT disabled, default=false
// see VFW_CallbackOnCaptureYield also VFW's capSetCallbackOnYield...
// NOT TESTED
bool wxVideoCaptureWindowVFW::GetUseThreadToCapture()
{
    VFW_GetCAPTUREPARMS();
    return (0 != m_CAPTUREPARMS.fYield);
}
void wxVideoCaptureWindowVFW::SetUseThreadToCapture( bool usethread )
{
    m_CAPTUREPARMS.fYield = (BOOL)usethread;
    VFW_SetCAPTUREPARMS();
}

// ??? does this work? and for what device?
// double resolution capture, (height&width*2)
//      "enable if hardware doesn't support decimation & using RGB" huh?
bool wxVideoCaptureWindowVFW::GetStepCaptureAt2x()
{
    VFW_GetCAPTUREPARMS();
    return (0 != m_CAPTUREPARMS.fStepCaptureAt2x);
}
void wxVideoCaptureWindowVFW::SetStepCaptureAt2x( bool cap2x)
{
    m_CAPTUREPARMS.fStepCaptureAt2x = (BOOL)cap2x;
    VFW_SetCAPTUREPARMS();
}
// ??? does this work? and for what device?
// number of times a frame is sampled for average frame, typically 5
unsigned int wxVideoCaptureWindowVFW::GetStepCaptureAverageFrames()
{
    VFW_GetCAPTUREPARMS();
    return m_CAPTUREPARMS.wStepCaptureAverageFrames;
}
void wxVideoCaptureWindowVFW::SetStepCaptureAverageFrames( unsigned int aveframes )
{
    m_CAPTUREPARMS.wStepCaptureAverageFrames = (UINT)aveframes;
    VFW_SetCAPTUREPARMS();
}

// max number of index entries in AVI file [1800..324,000]
//      each frame or audio buffer uses 1 index, sets max num of frames
//      default = 34,952 (32K frames + proportional num of audio buffers)
unsigned long int wxVideoCaptureWindowVFW::GetAviMaxIndexEntries()
{
    VFW_GetCAPTUREPARMS();
    return m_CAPTUREPARMS.dwIndexSize;
}
void wxVideoCaptureWindowVFW::SetAviMaxIndexEntries( unsigned long int maxindex)
{
    m_CAPTUREPARMS.dwIndexSize = (DWORD)maxindex;
    VFW_SetCAPTUREPARMS();
}
// Logical block size in an AVI file in bytes
//  0 = current sector size, recommended default=2K
//  if larger than the data structure, the rest is filled w/ RIFF "JUNK"
//  64,128,256,512,1K,2K,4K,8K,16K,32K are valid... I think?
unsigned int wxVideoCaptureWindowVFW::GetChunkGranularity()
{
    VFW_GetCAPTUREPARMS();
    return m_CAPTUREPARMS.wChunkGranularity;
}
void wxVideoCaptureWindowVFW::SetChunkGranularity( unsigned int chunkbytes )
{
    m_CAPTUREPARMS.wChunkGranularity = (UINT)chunkbytes;
    VFW_SetCAPTUREPARMS();
}


// record a video segment, exit by setting up capture parameters, returns success
bool wxVideoCaptureWindowVFW::CaptureVideoToFile()
{
    if (!HasAudioHardware()) SetCaptureAudio(false);

    VFW_SetCAPTUREPARMS();  // flush any parameters set by the user behind our backs
    return (0 != capCaptureSequence(m_hWndC));
}

// I think need Yield for this?
// stop capturing video & audio to AVI file, close file and end
bool wxVideoCaptureWindowVFW::CaptureVideoToFileStop()
{
    return (0 != capCaptureStop(m_hWndC));
}

// abort capturing video to AVI file, "discard" data
// notes: capture operation must yield to use this macro
//        image data is retained, audio is not
bool wxVideoCaptureWindowVFW::CaptureVideoToFileAbort()
{
    return (0 != capCaptureAbort(m_hWndC));
}

// time elapsed since the start of current/recent capture
unsigned long int wxVideoCaptureWindowVFW::GetCaptureTimeElapsedMS()
{
    VFW_GetCAPSTATUS();
    return (unsigned int)m_CAPSTATUS.dwCurrentTimeElapsedMS;
}
// capture in progress? then true
bool wxVideoCaptureWindowVFW::IsCapturingNow()
{
    VFW_GetCAPSTATUS();
    return (0 != m_CAPSTATUS.fCapturingNow);
}

// number of frames processed during current/recent capture
//   includes dropped frames
unsigned long int wxVideoCaptureWindowVFW::GetCapturedVideoFramesCount()
{
    VFW_GetCAPSTATUS();
    return (unsigned int)m_CAPSTATUS.dwCurrentVideoFrame;
}
// number of frames dropped during current/recent capture
//      when frame is dropped the previous frame is put into the AVI file
//      therefore does not affect syncronization, w/ audio & timing
unsigned long int wxVideoCaptureWindowVFW::GetCapturedVideoFramesDropped()
{
    VFW_GetCAPSTATUS();
    return (unsigned int)m_CAPSTATUS.dwCurrentVideoFramesDropped;
}

// ----------------------------------------------------------------------
// Capture file settings, filename to capture video to
// ----------------------------------------------------------------------
// valid capture file exists, then true
bool wxVideoCaptureWindowVFW::CaptureFileExists()
{
    VFW_GetCAPSTATUS();
    return (0 != m_CAPSTATUS.fCapFileExists);
}
// returns the name of the file to capture to AVI file
wxString wxVideoCaptureWindowVFW::GetCaptureFilename()
{
    VFW_GetCaptureFilename();
    return m_capturefilename;
}

// set name of the file to capture AVI file to, doesn't create/open/allocate
bool wxVideoCaptureWindowVFW::SetCaptureFilename(const wxString &capfilename)
{
    wxCHECK_MSG(capfilename != wxEmptyString, false, wxT("Invalid filename"));
    return (0 != capFileSetCaptureFile(m_hWndC, capfilename.fn_str()));
}

// Get the capture filename from MSW AVIcap window, returns success
// internal use only
bool wxVideoCaptureWindowVFW::VFW_GetCaptureFilename()
{
    LPTSTR filename[_MAX_PATH]; // must be >= _MAX_PATH = 256
    bool filenameOK = (0 != capFileGetCaptureFile(m_hWndC, filename, sizeof(filename)));
    if (filenameOK)
        m_capturefilename = wxString((wxChar*)filename);
    return filenameOK;
}

// dialog to ask the user what capture filename to use
bool wxVideoCaptureWindowVFW::SetCaptureFilenameDialog()
{
    VFW_GetCaptureFilename();
    m_capturefilename = wxFileSelector(wxT("Set capture filename"),
                                        wxPathOnly(m_capturefilename),
                                        wxFileNameFromPath(m_capturefilename),
                                        wxT("*.*"),
                                        wxT("AVI files (*.avi)|*.avi"),
                                        wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (!m_capturefilename.IsEmpty())
    {
        return SetCaptureFilename(m_capturefilename);
    }
    return false;
}

// preallocate space for the capture file
bool wxVideoCaptureWindowVFW::SetCaptureFilesizeMB(unsigned int filesizeMB )
{
    // the the total amount of free disk space
    unsigned int freediskspaceMB = GetFreeDiskSpaceInKB(m_capturefilename)/1024L;
    // if the capture file already exists, this is more "free space"
    unsigned int capfilesizeMB = GetFileSizeInKB(m_capturefilename)/1024L;
    if (capfilesizeMB < 0) capfilesizeMB = 0; // if -1, it doesn't exist so it's 0

    if (m_hWndC && (filesizeMB < 2048) && (freediskspaceMB + capfilesizeMB > filesizeMB))
    {
        return (0 != capFileAlloc(m_hWndC, (1024L*1024L*filesizeMB)));
    }
    return false;
}

// dialog to preallocate space for video capture
bool wxVideoCaptureWindowVFW::SetCaptureFileSizeDialog()
{
    wxCHECK_MSG(!m_capturefilename.IsNull(), false, wxT("Set capture filename before setting filesize"));

    VFW_GetCaptureFilename();
    // find the the total amount of free disk space, good only for < 2048MB, sigh...
    int freediskspaceMB = GetFreeDiskSpaceInKB(m_capturefilename)/1024L;
    // if the capture file already exists, this is more "free space"
    int capfilesizeMB = GetFileSizeInKB(m_capturefilename)/1024L;
    if (capfilesizeMB < 0) capfilesizeMB = 0; // if -1, it doesn't exist so it's really 0

    // 2GB limit or the available free space, whatever is less
    unsigned long int maxsize = wxMin(freediskspaceMB+capfilesizeMB, 2047L);

    wxString dialogtext;
    dialogtext.Printf(wxT("Filename: %s\nCurrent file size %dMB\nMax size %ldMB"),
                        m_capturefilename.c_str(), capfilesizeMB, maxsize);

    long filesize = wxGetNumberFromUser(dialogtext,
                                        wxT("MB"),
                                        wxT("Allocate space for capture file"),
                                        1, 0, maxsize);
    return SetCaptureFilesizeMB((unsigned int)filesize);
}

// extract the actual data from the SetCaptureFilename and save it to a new file
// this is used to create a file that is correctly sized to the video data size
// if not enough space for this file then it's deleted
bool wxVideoCaptureWindowVFW::SaveCapturedFileAs( const wxString &filename )
{
    return (0 != capFileSaveAs( m_hWndC, filename.fn_str() ));
}

// get the amount of free disk space in KB
long int wxVideoCaptureWindowVFW::GetFreeDiskSpaceInKB( const wxString &filepath )
{
    DWORD freeclusters, bytespersector, sectorspercluster, clusters;
    TCHAR rootdir[MAX_PATH+1];
    TCHAR* tmp_str = 0;    //required arg, a temp variable

    // need to find path for root directory on drive containing this file.

    GetFullPathName(filepath.fn_str(), sizeof(rootdir)/sizeof(rootdir[0]), rootdir, NULL);

    // truncate this to the name of the root directory
    if ((rootdir[0] == '\\') && (rootdir[1] == '\\'))
    {
        // path begins with  \\server\share\path so skip the first
        // three backslashes
        tmp_str = &rootdir[2];
        while (*tmp_str && (*tmp_str != '\\'))
        {
           tmp_str++;
        }
        if (*tmp_str)
        {
            // advance past the third backslash
            tmp_str++;
        }
    }
    else
    {
        // path must be drv:\path
        tmp_str = rootdir;
    }

    // find next backslash and put a null after it
    while (*tmp_str && (*tmp_str != '\\'))
    {
        tmp_str++;
    }
    // found a backslash ?
    if (*tmp_str)
    {
        // skip it and insert null
        tmp_str++;
        *tmp_str = '\0';
    }

    if (!GetDiskFreeSpace(rootdir,
                            &sectorspercluster,
                            &bytespersector,
                            &freeclusters,
                            &clusters))
    {
            wxMessageBox(wxT("Error determining free disk space"),
                            wxT("wxVideoCaptureWindow Error"),
                            wxOK|wxICON_EXCLAMATION|wxCENTRE);
            return (-1);
    }
    return MulDiv(sectorspercluster*bytespersector, freeclusters, 1024);
}

// ----------------------------------------------------------------------
// Audio Setup
// ----------------------------------------------------------------------
bool wxVideoCaptureWindowVFW::HasAudioHardware()
{
    return (0 != m_CAPSTATUS.fAudioHardware);
}

// set the audio recording properties, see #defines in header
// channels = 1,2 (mono,stereo)
// bitspersample = 8, 16
// samplespersecond = 8000,11025,16000,22050,24000,32000,44100,48000 (freq)
bool wxVideoCaptureWindowVFW::SetAudioFormat( int channels, int bitspersample, long int samplespersecond )
{
    if (!HasAudioHardware()) return false;

    DWORD audioformatsize = capGetAudioFormatSize(m_hWndC);
    if (audioformatsize == 0) return false;
    LPWAVEFORMATEX lpwfex = (LPWAVEFORMATEX) GlobalAllocPtr(GHND, audioformatsize);
    if (lpwfex == NULL) return false;

    // set channels 1=mono, 2=stereo
    if (channels == 2)
        lpwfex->nChannels = 2;
    else
        lpwfex->nChannels = 1;

    if (bitspersample == 16)
        lpwfex->wBitsPerSample = 16;
    else
        lpwfex->wBitsPerSample = 8;

    int i;
    bool samplespersecondOk = false;

    for (i = 0; i < wxVIDCAP_AUDIO_SAMPLESPERSEC_COUNT; i++ )
        if (samplespersecond == wxVIDCAP_AUDIO_SAMPLESPERSEC[i])
            samplespersecondOk = true;

    if (samplespersecondOk == true)
        lpwfex->nSamplesPerSec = samplespersecond;
    else
        lpwfex->nSamplesPerSec = 44100;

    // nBlockAlign = # of bytes per sample
    lpwfex->nBlockAlign = lpwfex->nChannels * (lpwfex->wBitsPerSample / 8);
    // nAvgBytesPerSec = for buffer estimation
    lpwfex->nAvgBytesPerSec = (long)lpwfex->nSamplesPerSec * lpwfex->nBlockAlign;
    // counts in bytes of extra info after cbSize, always 0 for WAVE_FORMAT_PCM
    //      can be nonzero for other formats
    lpwfex->cbSize = 0;
    // format type, are there any others that we can use?
    lpwfex->wFormatTag = WAVE_FORMAT_PCM;

    bool setOk = (0 != capSetAudioFormat(m_hWndC, lpwfex, (WORD)audioformatsize));

    GlobalFreePtr( lpwfex );

    return setOk;
}

// get the audio format, see SetAudioFormat, see #defines in header
bool wxVideoCaptureWindowVFW::GetAudioFormat( int *channels, int *bitspersample, long int *samplespersecond )
{
    if (!HasAudioHardware()) return false;

    DWORD audioformatsize = capGetAudioFormatSize(m_hWndC);
    if (audioformatsize == 0) return false;
    LPWAVEFORMATEX lpwfex = (LPWAVEFORMATEX) GlobalAllocPtr(GHND, audioformatsize);
    if (lpwfex == NULL) return false;

    if (!capGetAudioFormat(m_hWndC, lpwfex, (WORD)audioformatsize)) return false;

    if (channels)
        *channels = lpwfex->nChannels;
    if (bitspersample)
        *bitspersample = lpwfex->wBitsPerSample;
    if (samplespersecond)
        *samplespersecond = lpwfex->nSamplesPerSec;

    // for reference
    //int bytespersample = lpwfex->nBlockAlign;
    //long int avgbytespersecond = lpwfex->nAvgBytesPerSec;
    //int format = lpwfex->wFormatTag; // = WAVE_FORMAT_PCM;

    GlobalFreePtr( lpwfex );

    return true;
}

// capture audio too, default = true if device has audio capability
// actually the device is your sound card, I guess?
bool wxVideoCaptureWindowVFW::GetCaptureAudio()
{
    VFW_GetCAPTUREPARMS();
    return 0!=m_CAPTUREPARMS.fCaptureAudio;
}
void wxVideoCaptureWindowVFW::SetCaptureAudio( bool capaudio )
{
    m_CAPTUREPARMS.fCaptureAudio = (BOOL)capaudio;
    VFW_SetCAPTUREPARMS();
}

// max number of audio buffers, (max = 10), if 0 then autocalc, default = 4
unsigned int wxVideoCaptureWindowVFW::GetNumAudioBuffers()
{
    VFW_GetCAPTUREPARMS();
    return m_CAPTUREPARMS.wNumAudioRequested;
}
void wxVideoCaptureWindowVFW::SetNumAudioBuffers( unsigned int audiobufs )
{
    m_CAPTUREPARMS.wNumAudioRequested = (UINT)audiobufs;
    VFW_SetCAPTUREPARMS();
}

// Number of audio buffers allocated
unsigned int wxVideoCaptureWindowVFW::GetNumAudioBuffersAllocated()
{
    VFW_GetCAPSTATUS();
    return m_CAPSTATUS.wNumAudioAllocated;
}
// audio buffer size, default = 0 = max of either .5sec or 10Kb
unsigned long int wxVideoCaptureWindowVFW::GetAudioBufferSize()
{
    VFW_GetCAPTUREPARMS();
    return m_CAPTUREPARMS.dwAudioBufferSize;
}
void wxVideoCaptureWindowVFW::SetAudioBufferSize( unsigned long int audiobufsize )
{
    m_CAPTUREPARMS.dwAudioBufferSize = (DWORD)audiobufsize;
    VFW_SetCAPTUREPARMS();
}

// audio stream controls clock for AVI file
//      video duration is forced to match audio duration
//      if false they can differ in length
bool wxVideoCaptureWindowVFW::GetAudioStreamMaster()
{
    VFW_GetCAPTUREPARMS();
    return AVSTREAMMASTER_AUDIO & m_CAPTUREPARMS.AVStreamMaster;
}
void wxVideoCaptureWindowVFW::SetAudioStreamMaster( bool audiomaster)
{
    if (audiomaster)
        m_CAPTUREPARMS.AVStreamMaster = AVSTREAMMASTER_AUDIO;
    else
        m_CAPTUREPARMS.AVStreamMaster = AVSTREAMMASTER_NONE;

    VFW_SetCAPTUREPARMS();
}

// number of waveform-audio samples processed during current/recent capture
unsigned long int wxVideoCaptureWindowVFW::GetCapturedWaveSamplesCount()
{
    VFW_GetCAPSTATUS();
    return (unsigned long int)m_CAPSTATUS.dwCurrentWaveSamples;
}

// --------------------------------------------------------------------------
// MSW VFW Callbacks call these wxVideoCaptureWindow functions that YOU can override
//      check out vfw.h, search IDS_CAP_BEGIN and others to get Id definitions
// --------------------------------------------------------------------------

bool wxVideoCaptureWindowVFW::VFW_SetCallbackOnError(bool on)
{
    if (on)
        return (0 != capSetCallbackOnError(m_hWndC, VFW_VideoErrorCallbackProc));

    return (0 != capSetCallbackOnError(m_hWndC, NULL));
}
// deal with non-fatal errors from the capture hWnd
// reports: no disk space, read-only file, can't access hardware,
//          too many frames dropped, and others?
bool wxVideoCaptureWindowVFW::VFW_CallbackOnError(const wxString &errortext, int errorId)
{
    // starting new major function, clear out old errors, this is NOT an error
    if (errorId == 0) return true;

    m_errortext = errortext;
    wxVideoCaptureEvent event( wxEVT_VIDEO_ERROR, this, GetId() );
    event.SetErrorText(m_errortext);
    GetEventHandler()->ProcessEvent(event);

    // show error id and text
    wxString errormessage;
    errormessage.Printf(wxT("wxVideoCaptureWindow Error# %d\n %s"), errorId, errortext.c_str());
    wxMessageBox(errormessage, wxT("wxVideoCaptureWindow Error"), wxOK|wxICON_EXCLAMATION|wxCENTRE);

    return true;
}


bool wxVideoCaptureWindowVFW::VFW_SetCallbackOnStatus(bool on)
{
    if (on)
        return (0 != capSetCallbackOnStatus(m_hWndC, VFW_VideoStatusCallbackProc));

    return (0 != capSetCallbackOnStatus(m_hWndC, NULL));
}
// changes in the status for the video hWnd
bool wxVideoCaptureWindowVFW::VFW_CallbackOnStatus(const wxString &statustext, int statusId)
{
    static int lastId = -12345;

    // IDS_CAP_END message overwrites useful statistics message,
    // such as how many frames captured/dropped/how long
    if (statusId == IDS_CAP_END)
    {
        if ((lastId == IDS_CAP_STAT_VIDEOAUDIO) ||
            (lastId == IDS_CAP_STAT_VIDEOONLY) )
        {
            return true;
        }

    }
    lastId = statusId;

    m_statustext = statustext;
    wxVideoCaptureEvent event( wxEVT_VIDEO_STATUS, this, GetId() );
    event.SetStatusText(m_statustext);
    GetEventHandler()->ProcessEvent(event);

    return true;
}


bool wxVideoCaptureWindowVFW::VFW_SetCallbackFrame(bool on)
{
    if (on)
        return (0 != capSetCallbackOnFrame(m_hWndC, VFW_VideoFrameCallbackProc));

    return (0 != capSetCallbackOnFrame(m_hWndC, NULL));
}
// frame callback, during software previewing
// called when a video frame is available, when preview rate is > 0 and
// streaming capture is not in progress
bool wxVideoCaptureWindowVFW::VFW_CallbackOnFrame(LPVIDEOHDR lpVHdr)
{
    static wxLongLong timenow = wxGetLocalTimeMillis();
    static wxLongLong lasttime = wxGetLocalTimeMillis();

    m_framenumber++;

    if (m_preview_wximage || m_grab_wximage)
    {
        VFW_DDBtoDIB(lpVHdr);           // fill m_wximage
        if(ProcesswxImageFrame())       // Call the stub in case something is done here
            Refresh(false);             // draw image
        m_grab_wximage = false;         // got the frame
    }

    unsigned int timeave = (m_previewmsperframe > 500) ? 1 : 4; // get a nicer frame rate
    if (m_framenumber%timeave == 0)
    {
        timenow = wxGetLocalTimeMillis();
        m_actualpreviewmsperframe = (unsigned int)((timenow.GetLo() - lasttime.GetLo())/timeave);
        lasttime = timenow;
    }

    wxVideoCaptureEvent event( wxEVT_VIDEO_FRAME, this, GetId() );
    event.SetFrameNumber( m_framenumber );
    event.SetFrameRateMS( m_actualpreviewmsperframe );
    GetEventHandler()->ProcessEvent(event);

    return true;
}


bool wxVideoCaptureWindowVFW::VFW_SetCallbackOnCaptureYield(bool on)
{
    if (on)
        return (0 != capSetCallbackOnYield(m_hWndC, VFW_CaptureYieldCallbackProc));

    return (0 != capSetCallbackOnYield(m_hWndC, NULL));
}
// yield callback, called during capturing to allow for updating the gui
bool wxVideoCaptureWindowVFW::VFW_CallbackOnCaptureYield()
{
    return true;
}


bool wxVideoCaptureWindowVFW::VFW_SetCallbackOnCaptureControl(bool on)
{
    if (on)
        return (0 != capSetCallbackOnCapControl(m_hWndC, VFW_CaptureControlCallbackProc));

    return (0 != capSetCallbackOnCapControl(m_hWndC, NULL));
}
// capture start/stop control, one message when ready to begin
//  and then between frames to end
// if (nState == CONTROLCALLBACK_PREROLL) buffers allocated ready to start capture
// if (nState == CONTROLCALLBACK_CAPTURING) now capturing, end capture by returning false
bool wxVideoCaptureWindowVFW::VFW_CallbackOnCaptureControl(int WXUNUSED(nState))
{
    return true;
}


bool wxVideoCaptureWindowVFW::VFW_SetCallbackOnVideoStream(bool on)
{
    if (on)
        return (0 != capSetCallbackOnVideoStream(m_hWndC, VFW_VideoStreamCallbackProc));

    return (0 != capSetCallbackOnVideoStream(m_hWndC, NULL));
}
// stream callback, called just before saving to disk when recording
bool wxVideoCaptureWindowVFW::VFW_CallbackOnVideoStream(LPVIDEOHDR WXUNUSED(lpVHdr))
{
    wxVideoCaptureEvent event( wxEVT_VIDEO_STREAM, this, GetId() );
    event.SetStatusText( wxT("Video recording stream start") );
    GetEventHandler()->ProcessEvent(event);

    return true;
}


bool wxVideoCaptureWindowVFW::VFW_SetCallbackOnWaveStream(bool on)
{
    if (on)
        return (0 != capSetCallbackOnWaveStream(m_hWndC, VFW_WaveStreamCallbackProc));

    return (0 != capSetCallbackOnWaveStream(m_hWndC, NULL));
}
// called before writing audio buffers to disk, they can be modified here
bool wxVideoCaptureWindowVFW::VFW_CallbackOnWaveStream(LPWAVEHDR WXUNUSED(lpWHdr))
{
    return true;
}

// ----------------------------------------------------------------------
// MSW VFW Capture Driver Capabilities, updated when driver is changed
// do not need to call this, unless explicitly changing m_CAPDRIVERCAPS
// ----------------------------------------------------------------------
bool wxVideoCaptureWindowVFW::VFW_GetCAPDRIVERCAPS()
{
    bool ok = (0 != capDriverGetCaps(m_hWndC, &m_CAPDRIVERCAPS, sizeof(CAPDRIVERCAPS)));

    // or SendMessage(m_hWndC,WM_CAP_DRIVER_GET_CAPS,sizeof(CAPDRIVERCAPS),
    //                  (LONG)(LPVOID)&m_CAPDRIVERCAPS);

    if (ok)
    {
        m_has_overlay = (0 != m_CAPDRIVERCAPS.fHasOverlay);

    }
    else
    {
        Reset(false);
    }

    return ok;
}

// ----------------------------------------------------------------------
// MSW VFW Current status of the capture driver
// you do not need to call this, unless explicitly changing m_CAPSTATUS
// ----------------------------------------------------------------------
bool wxVideoCaptureWindowVFW::VFW_GetCAPSTATUS()
{
    bool ok = (0 != capGetStatus(m_hWndC, &m_CAPSTATUS, sizeof(CAPSTATUS)));

    if (ok)
    {
        m_deviceIndex = (int)m_CAPDRIVERCAPS.wDeviceIndex;
        if (m_deviceIndex >= 0)
        {
            m_previewing = (0 != m_CAPSTATUS.fLiveWindow);
            m_overlaying = (0 != m_CAPSTATUS.fOverlayWindow);
            if (!m_preview_wximage) m_previewscaled = (0 != m_CAPSTATUS.fScale);
            m_imageSize.x = m_CAPSTATUS.uiImageWidth;
            m_imageSize.y = m_CAPSTATUS.uiImageHeight;
        }
    }
    // reset the vars since when you're not connected you get junk
    //    ie. image size is -124512512x235121221 and what not
    if (!ok || (m_deviceIndex < 0))
    {
        Reset(false);
    }

    return ok;
}

// default driver palette being used, then true
bool wxVideoCaptureWindowVFW::IsUsingDefaultPalette()
{
    VFW_GetCAPSTATUS();
    return (0 != m_CAPSTATUS.fUsingDefaultPalette);
}

// ----------------------------------------------------------------------
// MSW VFW Capture Parameters
// you do not need to call this, unless explicitly changing m_CAPTUREPARMS
// ----------------------------------------------------------------------
bool wxVideoCaptureWindowVFW::VFW_GetCAPTUREPARMS()
{
    bool ok = (0 != capCaptureGetSetup(m_hWndC, &m_CAPTUREPARMS, sizeof(CAPTUREPARMS)));
    if (ok)
    {
        // load local vars here
    }
    else
    {
        Reset(false);
    }

    return ok;
}

bool wxVideoCaptureWindowVFW::VFW_SetCAPTUREPARMS()
{
    bool ok = (0 != capCaptureSetSetup(m_hWndC, &m_CAPTUREPARMS, sizeof(CAPTUREPARMS)));

    if (ok)
    {
        // load local vars here
    }
    else
    {
        Reset(false);
    }

    return ok;
}



//------------------------------------------------------------------------------
// MSW VFW Callbacks for wxVideoCaptureWindow
//------------------------------------------------------------------------------

// deal with (usually) fatal exceptions
// For any unhandled exceptions, DISCONNECT before crashing, or at least die trying
// called by SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)(VFW_VideoExceptionHandler));
LONG VFW_VideoExceptionHandler( LPEXCEPTION_POINTERS WXUNUSED(arg) )
{
    wxVideoCaptureWindowHandles::const_iterator iter = gs_vidCapWinHandles.begin();

    for( ; iter != gs_vidCapWinHandles.end(); iter++)
    {
        wxVideoCaptureWindowVFW *win = (wxVideoCaptureWindowVFW*)iter->second;

        // seems to avoid recursion this way
        if ((win != NULL) && win->IsDeviceConnected())
            win->DeviceDisconnect();
    }

    return EXCEPTION_CONTINUE_SEARCH; // anyone else want to clean up this mess?
}

// deal with non-fatal errors from the capture hWnd
// implemented by capSetCallbackOnError(m_hWndC, VFW_VideoErrorCallbackProc);
// reports: no disk space, read-only file, can't access hardware,
//          too many frames dropped, and others?
#if UNICODE
    static LRESULT CALLBACK VFW_VideoErrorCallbackProc(HWND hWnd, int nErrID, LPCWSTR lpErrorText)
#else
    static LRESULT CALLBACK VFW_VideoErrorCallbackProc(HWND hWnd, int nErrID, LPSTR lpErrorText)
#endif
{
    if (!hWnd) return (LRESULT)false;

    wxVideoCaptureWindowVFW *vidCapWin = wxFindVidCapWinFromHandle( (WXHWND)hWnd );
    if (vidCapWin) vidCapWin->VFW_CallbackOnError(wxString(lpErrorText), nErrID);

    return (LRESULT)true;
}

// changes in the status for the video hWnd
// implemented by capSetCallbackOnStatus(m_hWndC, VFW_VideoStatusCallbackProc);
#if UNICODE
    static LRESULT CALLBACK VFW_VideoStatusCallbackProc(HWND hWnd, int nID, LPCWSTR lpStatusText)
#else
    static LRESULT CALLBACK VFW_VideoStatusCallbackProc(HWND hWnd, int nID, LPSTR lpStatusText)
#endif
{
    if (!hWnd) return (LRESULT)false;

    wxVideoCaptureWindowVFW *vidCapWin = wxFindVidCapWinFromHandle( (WXHWND)hWnd );
    if (vidCapWin) vidCapWin->VFW_CallbackOnStatus(wxString(lpStatusText), nID);

    return (LRESULT)true;
}

// frame callback, during software previewing
// called when a video frame is available, when preview rate is > 0 and
// streaming capture is not in progress
// implemented with capSetCallbackOnFrame(m_hWndC, VFW_VideoFrameCallbackProc);
static LRESULT CALLBACK VFW_VideoFrameCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr)
{
    static wxVideoCaptureWindowVFW *vidCapWin = (wxVideoCaptureWindowVFW*)NULL; // quicker

    if (!hWnd) return (LRESULT)false;

    vidCapWin = wxFindVidCapWinFromHandle( (WXHWND)hWnd );
    if (vidCapWin) vidCapWin->VFW_CallbackOnFrame(lpVHdr);

    return (LRESULT)true;
}

// yield callback, called during capturing to allow for updating the gui
// implemented with capSetCallbackOnYield(m_hWndC, VFW_CaptureYieldCallbackProc);
static LRESULT CALLBACK VFW_CaptureYieldCallbackProc(HWND WXUNUSED(hWnd))
{
    //"If a yield callback function is installed, it will be called regardless
    //     of the state of the fYield member of the CAPTUREPARMS structure"

    MSG msg;

    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (LRESULT)true; // true to continue capturing, false to stop
}

// capture start/stop control, one message when ready to begin
//  and then between frames to end
// if (nState == CONTROLCALLBACK_PREROLL) buffers allocated ready to start capture
// if (nState == CONTROLCALLBACK_CAPTURING) now capturing, end capture by returning false
// implemented with capSetCallbackOnCapControl(m_hWndC, VFW_CaptureControlCallbackProc)
static LRESULT CALLBACK VFW_CaptureControlCallbackProc(HWND WXUNUSED(hWnd), int WXUNUSED(nState))
{
    // humm ??
    return (LRESULT)true;
}

// stream callback, called just before saving to disk when recording
// implemented with capSetCallbackOnVideoStream(m_hWndC, VFW_VideoStreamCallbackProc);
static LRESULT CALLBACK VFW_VideoStreamCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr)
{
    static wxVideoCaptureWindowVFW *vidCapWin = (wxVideoCaptureWindowVFW*)NULL;

    if (!hWnd) return (LRESULT)false;

    vidCapWin = wxFindVidCapWinFromHandle( (WXHWND)hWnd );
    if (vidCapWin) vidCapWin->VFW_CallbackOnVideoStream(lpVHdr);

    return (LRESULT)true;
}

// called before writing audio buffers to disk, they can be modified here
// implemented with capSetCallbackOnWaveStream(hWndCapture, VFW_WaveStreamCallbackProc);
static LRESULT CALLBACK VFW_WaveStreamCallbackProc(HWND WXUNUSED(hWnd), LPWAVEHDR WXUNUSED(lpWHdr))
{
    // humm ??
    return (LRESULT)true;
}

#endif // defined(__WXMSW__)


//=============================================================================
//=============================================================================
//  Everything below this point is for reference only
//=============================================================================
//=============================================================================
/*
typedef struct {
    UINT   wDeviceIndex;
!    BOOL   fHasOverlay;
    BOOL   fHasDlgVideoSource;
    BOOL   fHasDlgVideoFormat;
    BOOL   fHasDlgVideoDisplay;
    BOOL   fCaptureInitialized;
    BOOL   fDriverSuppliesPalettes;
    HANDLE hVideoIn;
    HANDLE hVideoOut;
    HANDLE hVideoExtIn;
    HANDLE hVideoExtOut;
} CAPDRIVERCAPS;

Members

wDeviceIndex
    Index of the capture driver. An index value can range from 0 to 9.
fHasOverlay
    Video-overlay flag. The value of this member is true if the device supports video overlay.
fHasDlgVideoSource
    Video source dialog flag. The value of this member is true if the device
    supports a dialog box for selecting and controlling the video source.
fHasDlgVideoFormat
    Video format dialog flag. The value of this member is true if the device
    supports a dialog box for selecting the video format.
fHasDlgVideoDisplay
    Video display dialog flag. The value of this member is true if the device
    supports a dialog box for controlling the redisplay of video from the capture frame buffer.
fCaptureInitialized
    Capture initialization flag. The value of this member is true if a capture
    device has been successfully connected.
fDriverSuppliesPalettes
    Driver palette flag. The value of this member is true if the driver can create palettes.
hVideoIn
    Not used in Win32 applications.
hVideoOut
    Not used in Win32 applications.
hVideoExtIn
    Not used in Win32 applications.
hVideoExtOut
    Not used in Win32 applications.

*/


/*
typedef struct {
!    UINT     uiImageWidth;
!    UINT     uiImageHeight;
!    BOOL     fLiveWindow;
!    BOOL     fOverlayWindow;
!    BOOL     fScale;
    POINT    ptScroll;
    BOOL     fUsingDefaultPalette;
    BOOL     fAudioHardware;
    BOOL     fCapFileExists;
    DWORD    dwCurrentVideoFrame;
    DWORD    dwCurrentVideoFramesDropped;
    DWORD    dwCurrentWaveSamples;
    DWORD    dwCurrentTimeElapsedMS;
    HPALETTE hPalCurrent;
    BOOL     fCapturingNow;
    DWORD    dwReturn;
    UINT     wNumVideoAllocated;
    UINT     wNumAudioAllocated;
} CAPSTATUS;

Members

uiImageWidth
    Image width, in pixels.
uiImageHeight
    Image height, in pixels
fLiveWindow
    Live window flag. The value of this member is true if the window is
    displaying video using the preview method.
fOverlayWindow
    Overlay window flag. The value of this member is true if the window is
    displaying video using hardware overlay.
fScale
    Input scaling flag. The value of this member is true if the window is
    scaling the input video to the client area when displaying video using
    preview. This parameter has no effect when displaying video using overlay.
ptScroll
    The x- and y-offset of the pixel displayed in the upper left corner of the
    client area of the window.
fUsingDefaultPalette
    Default palette flag. The value of this member is true if the capture driver
    is using its default palette.
fAudioHardware
    Audio hardware flag. The value of this member is true if the system has
    waveform-audio hardware installed.
fCapFileExists
    Capture file flag. The value of this member is true if a valid capture file
    has been generated.
dwCurrentVideoFrame
    Number of frames processed during the current (or most recent) streaming
    capture. This count includes dropped frames.
dwCurrentVideoFramesDropped
    Number of frames dropped during the current (or most recent) streaming
    capture. Dropped frames occur when the capture rate exceeds the rate at
    which frames can be saved to file. In this case, the capture driver has no
    buffers available for storing data. Dropping frames does not affect
    synchronization because the previous frame is displayed in place of the dropped frame.
dwCurrentWaveSamples
    Number of waveform-audio samples processed during the current (or most recent) streaming capture.
dwCurrentTimeElapsedMS
    Time, in milliseconds, since the start of the current (or most recent) streaming capture.
hPalCurrent
    Handle to current palette.
fCapturingNow
    Capturing flag. The value of this member is true when capturing is in progress.
dwReturn
    Error return values. Use this member if your application does not support an error callback function.
wNumVideoAllocated
    Number of video buffers allocated. This value might be less than the number
    specified in the wNumVideoRequested member of the CAPTUREPARMS structure.
wNumAudioAllocated
    Number of audio buffers allocated. This value might be less than the number
    specified in the wNumAudioRequested member of the CAPTUREPARMS structure.

*/

/*

typedef struct {
    DWORD dwRequestMicroSecPerFrame;
    BOOL  fMakeUserHitOKToCapture;
    UINT  wPercentDropForError;
    BOOL  fYield;
    DWORD dwIndexSize;
    UINT  wChunkGranularity;
    BOOL  fUsingDOSMemory;
    UINT  wNumVideoRequested;
    BOOL  fCaptureAudio;
    UINT  wNumAudioRequested;
    UINT  vKeyAbort;
    BOOL  fAbortLeftMouse;
    BOOL  fAbortRightMouse;
    BOOL  fLimitEnabled;
    UINT  wTimeLimit;
    BOOL  fMCIControl;
    BOOL  fStepMCIDevice;
    DWORD dwMCIStartTime;
    DWORD dwMCIStopTime;
    BOOL  fStepCaptureAt2x;
    UINT  wStepCaptureAverageFrames;
    DWORD dwAudioBufferSize;
    BOOL  fDisableWriteCache;
    UINT  AVStreamMaster;
} CAPTUREPARMS;


Members

dwRequestMicroSecPerFrame
    Requested frame rate, in microseconds. The default value is 66667, which
    corresponds to 15 frames per second.
fMakeUserHitOKToCapture
    User-initiated capture flag. If this member is true, AVICap displays a
    dialog box prompting the user to initiate capture. The default value is false.
wPercentDropForError
    Maximum allowable percentage of dropped frames during capture. Values range
    from 0 to 100. The default value is 10.
fYield
    Yield flag. If this member is true, the capture window spawns a separate
    background thread to perform step and streaming capture. The default value is false.
    Applications that set this flag must handle potential reentry issues because
    the controls in the application are not disabled while capture is in progress.
dwIndexSize
    Maximum number of index entries in an AVI file. Values range from 1800 to
    324,000. If set to 0, a default value of 34,952 (32K frames plus a
    proportional number of audio buffers) is used.
    Each video frame or buffer of waveform-audio data uses one index entry. The value of this entry establishes a limit for the number of frames or audio buffers that can be captured.
wChunkGranularity
    Logical block size, in bytes, of an AVI file. The value 0 indicates the
    current sector size is used as the granularity.
fUsingDOSMemory
    Not used in Win32 applications.
wNumVideoRequested
    Maximum number of video buffers to allocate. The memory area to place the
    buffers is specified with fUsingDOSMemory. The actual number of buffers
    allocated might be lower if memory is unavailable.
fCaptureAudio
    Capture audio flag. If this member is true, audio is captured during
    streaming capture. This is the default value if audio hardware is installed.
wNumAudioRequested
    Maximum number of audio buffers to allocate. The maximum number of buffers is 10.
vKeyAbort
    Virtual keycode used to terminate streaming capture. The default value is
    VK_ESCAPE. You must call the RegisterHotKey function before specifying a
    keystroke that can abort a capture session.

    You can combine keycodes that include CTRL and SHIFT keystrokes by using
    the logical OR operator with the keycodes for CTRL (0x8000) and SHIFT (0x4000).
fAbortLeftMouse
    Abort flag for left mouse button. If this member is true, streaming capture
    stops if the left mouse button is pressed. The default value is true.
fAbortRightMouse
    Abort flag for right mouse button. If this member is true, streaming capture
    stops if the right mouse button is pressed. The default value is true.
fLimitEnabled
    Time limit enabled flag. If this member is true, streaming capture stops
    after the number of seconds in wTimeLimit has elapsed. The default value is false.
wTimeLimit
    Time limit for capture, in seconds. This parameter is used only if fLimitEnabled is true.
fMCIControl
    MCI device capture flag. If this member is true, AVICap controls an
    MCI-compatible video source during streaming capture. MCI-compatible video
    sources include VCRs and laserdiscs.
fStepMCIDevice
    MCI device step capture flag. If this member is true, step capture using an
    MCI device as a video source is enabled. If it is false, real-time capture
    using an MCI device is enabled. (If fMCIControl is false, this member is ignored.)
dwMCIStartTime
    Starting position, in milliseconds, of the MCI device for the capture sequence.
    (If fMCIControl is false, this member is ignored.)
dwMCIStopTime
    Stopping position, in milliseconds, of the MCI device for the capture
    sequence. When this position in the content is reached, capture ends and the
    MCI device stops. (If fMCIControl is false, this member is ignored.)
fStepCaptureAt2x
    Double-resolution step capture flag. If this member is true, the capture
    hardware captures at twice the specified resolution. (The resolution for the
    height and width is doubled.)

    Enable this option if the hardware does not support hardware-based decimation and you are capturing in the RGB format.
wStepCaptureAverageFrames
    Number of times a frame is sampled when creating a frame based on the average
    sample. A typical value for the number of averages is 5.
dwAudioBufferSize
    Audio buffer size. If the default value of zero is used, the size of each
    buffer will be the maximum of 0.5 seconds of audio or 10K bytes.
fDisableWriteCache
    Not used in Win32 applications.
AVStreamMaster
    Indicates whether the audio stream controls the clock when writing an AVI
    file. If this member is set to AVSTREAMMASTER_AUDIO, the audio stream is
    considered the master stream and the video stream duration is forced to
    match the audio duration. If this member is set to AVSTREAMMASTER_NONE, the
    durations of audio and video streams can differ.

*/
