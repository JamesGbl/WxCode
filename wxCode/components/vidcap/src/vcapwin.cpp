//TODO:
//
// What is the MSW capture window's ID? it's 125 now...
// EVT_USER + 101? set to a real value...
// What the heck is an MCI device? vcr/videodisc? do/did they really exist?
// Audio volume and maybe device? mine's on the mic input.
// Allow for centering of hWnd, when no scrollbars on the window, maybe
// RegisterHotKey to use SetAbortKey
// Pinnacle Studio PCTV flashes GUI when device is disconnected and reconnected
//      and then preview is set on, all video programs do it, if Overlay
//      first then preview it's OK, hummm... see DeviceConnect()
// Deal with status callback in a more sensible way
// Deal with the Yield callback, NOT TESTED
// Allow for image processing of the captured frames before saving
// Need to set Preview(false) in program's OnClose or wait for a while?

/////////////////////////////////////////////////////////////////////////////
// Name:        vcapwin.cpp - wxVideoCaptureWindowBase
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
//#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    //#include <wx/wx.h>
#endif

#include "wx/vidcap/vcapwin.h"
#include <wx/file.h>
#include <wx/arrimpl.cpp> // for wxArrayVideoCaptureFormat

#if !wxCHECK_VERSION(2,9,0)
    #define wxPENSTYLE_SOLID   wxSOLID
    #define wxBRUSHSTYLE_SOLID wxSOLID
#endif

//----------------------------------------------------------------------------
// wxVideoCaptureWindow #defines and globals
//----------------------------------------------------------------------------

// array of some predefined "standard" video widths
const unsigned int wxVIDCAP_VIDEO_WIDTHS[wxVIDCAP_VIDEO_WIDTHS_COUNT] =
    {80,128,160,176,180,192,240,320,352,360,384,400,480,640,704,720,768,800,1024};
// array of some predefined "standard" video heights
const unsigned int wxVIDCAP_VIDEO_HEIGHTS[wxVIDCAP_VIDEO_HEIGHTS_COUNT] =
    {60, 96, 120, 144, 180, 240, 288, 300, 360, 480, 576, 600, 768 };

// notes: in MSW can use these if you really want
// FOURCC mmioStringToFOURCC("CYUV",0) converts strings
// MAKEFOURCC('C','Y','U','V') converts 4 chars
// some checks: "Y8"="Y8  "=0x20203859 & Y41P=0x50313459

FOURCC wxStringToFOURCC(const wxString &string)
    { return StringToFOURCC(wxConvUTF8.cWX2MB(string.c_str())); }

wxString FOURCCTowxString(FOURCC fourcc)
    { return wxString(wxConvUTF8.cMB2WX(FOURCCToString(fourcc))); }

const char* FOURCCToString(FOURCC fourcc)
{
//  union { char fccstring[5]; FOURCC fcc; };
//  fcc = fourcc;
//  fccstring[4] = 0;
//  return fccstring;

    static char s4[5] = "    ";
    char *s = (char*)(&fourcc);
    for (unsigned int i = 0; i < 4; ++i) s4[i] = s[i];
    s4[4] = 0;
    return s4;
}

FOURCC StringToFOURCC( const char *s )
{
    char s4[5] = "    "; // when the FOURCC is not 4 chars like "Y8"

    unsigned int len = strlen(s);
    if (len < 1) return FOURCC(0); // for BI_RGB = 0

    for (unsigned int i = 0; i < len; ++i) s4[i] = s[i];
    return FOURCC((s4[0])|(s4[1]<<8)|(s4[2]<<16)|(s4[3]<<24));
}

//---------------------------------------------------------------------------
// Audio
//---------------------------------------------------------------------------

// array of "standard" audio rates
const unsigned int wxVIDCAP_AUDIO_SAMPLESPERSEC[wxVIDCAP_AUDIO_SAMPLESPERSEC_COUNT]=
    { 8000, 11025, 16000, 22050, 24000, 32000, 44100, 48000 };

//------------------------------------------------------------------------------
// wxVideoCaptureFormat
//------------------------------------------------------------------------------

const wxVideoCaptureFormat wxNullVideoCaptureFormat(wxT("Invalid Video Format"), wxNullFOURCC, wxNullFOURCC, 0);

WX_DEFINE_OBJARRAY(wxArrayVideoCaptureFormat);

wxVideoCaptureFormat::wxVideoCaptureFormat(const wxString &description, FOURCC fourcc,
                                           int v4l2_pixelformat, int bpp)
                     : m_description(description),
                       m_fourcc(fourcc),
                       m_v4l2_pixelformat(v4l2_pixelformat),
                       m_bpp(bpp)
{
}

//------------------------------------------------------------------------------
// wxVideoCaptureEvent, events for the wxVideoCaptureWindow
//------------------------------------------------------------------------------
DEFINE_EVENT_TYPE(wxEVT_VIDEO_STATUS)
DEFINE_EVENT_TYPE(wxEVT_VIDEO_FRAME)
DEFINE_EVENT_TYPE(wxEVT_VIDEO_STREAM)
DEFINE_EVENT_TYPE(wxEVT_VIDEO_ERROR)
DEFINE_EVENT_TYPE(wxEVT_VIDEO_FRAME_REQ)

IMPLEMENT_DYNAMIC_CLASS(wxVideoCaptureEvent, wxEvent)

wxVideoCaptureEvent::wxVideoCaptureEvent(const wxVideoCaptureEvent& event)
                    :wxEvent(event)
{
    m_statusMessage = event.m_statusMessage;
    m_errorMessage  = event.m_errorMessage;
    m_framenumber   = event.m_framenumber;
    m_frameratems   = event.m_frameratems;
}

wxVideoCaptureEvent::wxVideoCaptureEvent(wxEventType commandType,
                                         wxVideoCaptureWindowBase *vidCapWin,
                                         int win_id)
                    :wxEvent(win_id, commandType)
{
    SetEventObject( vidCapWin );
}

//----------------------------------------------------------------------------
// wxVideoCaptureWindowBase : window for viewing/recording streaming video or snapshots
//----------------------------------------------------------------------------
IMPLEMENT_ABSTRACT_CLASS(wxVideoCaptureWindowBase, wxScrolledWindow);

// static
wxArrayVideoCaptureFormat wxVideoCaptureWindowBase::sm_wxVideoCaptureFormats;

wxVideoCaptureWindowBase::wxVideoCaptureWindowBase( wxWindow *parent, wxWindowID id,
                                                    const wxPoint &pos, const wxSize &size,
                                                    long style, const wxString &name)
                         :wxScrolledWindow()
{
    Reset(true);
    Create(parent, id, pos, size, style, name);
}

bool wxVideoCaptureWindowBase::Create( wxWindow *parent, wxWindowID id,
                                       const wxPoint &pos, const wxSize &size,
                                       long style, const wxString &name)
{
    // setup the timer for previewing w/ wxImages
    m_previewTimer.SetOwner(this, ID_wxVIDCAP_PREVIEW_WXIMAGE_TIMER);

    return wxScrolledWindow::Create(parent, id, pos, size, style, name);
}

wxVideoCaptureWindowBase::~wxVideoCaptureWindowBase()
{
    m_previewTimer.Stop(); // just to be sure
}

void wxVideoCaptureWindowBase::Reset(bool full)
{
    if (full) m_deviceNames.Clear();
    if (full) m_deviceVersions.Clear();

    m_deviceIndex                 = -1;

    m_previewing                  = false;
    m_preview_wximage             = false;
    if (full) m_previewscaled     = false;
    if (full) m_previewmsperframe = 66;             // 15fps
    m_actualpreviewmsperframe     = 0;

    m_has_overlay                 = false;
    m_overlaying                  = false;

    m_getting_wximage             = false;          // not filling the wximage now

    if (full) m_framenumber         = 0;
    if (full) m_lastframetimemillis = 0; // wxGetLocalTimeMillis();
    if (full) m_actualpreviewtimemillis = 0;

    m_imageSize                   = wxSize(0, 0);
    m_minImageSize                = wxSize(32, 32);    // just a guess, VFW doesn't support this so...
    m_maxImageSize                = wxSize(1024, 768); // just a guess, VFW doesn't support this so...

    m_last_error_num              = 0;
}

// ----------------------------------------------------------------------
// Device descriptions & versions, get and enumerate
// ----------------------------------------------------------------------

wxString wxVideoCaptureWindowBase::GetDeviceName(int index) const
{
    wxCHECK_MSG((index >= 0) && (index < (int)m_deviceNames.GetCount()), wxEmptyString, wxT("Invalid video device index"));

    return m_deviceNames.Item(index);
}

wxString wxVideoCaptureWindowBase::GetDeviceVersion(int index) const
{
    wxCHECK_MSG((index >= 0) && (index < (int)m_deviceNames.GetCount()), wxEmptyString, wxT("Invalid video device index"));

    return m_deviceVersions.Item(index);
}

// ----------------------------------------------------------------------
// Connect or Disconnect to device
// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
// Display dialogs to set/get video characteristics
// ----------------------------------------------------------------------

void wxVideoCaptureWindowBase::ShowPropertiesDialog()
{
    wxFrame *dialog = new wxFrame(this, wxID_ANY,
                                  wxT("wxVideoCaptureWindow Properties"),
                                  wxDefaultPosition, wxDefaultSize,
                                  wxDEFAULT_FRAME_STYLE);

    wxBoxSizer *dialogsizer = new wxBoxSizer( wxVERTICAL );
    wxTextCtrl *textctrl = new wxTextCtrl( dialog, wxID_ANY,
                                           wxT(""),
                                           wxDefaultPosition, wxSize(600,600),
                                           wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH2|wxHSCROLL);

    textctrl->SetFont(wxFont(10, wxMODERN, wxNORMAL, wxNORMAL)); // want monospace
    textctrl->AppendText(GetPropertiesString());
    textctrl->SetInsertionPoint(0);

    dialogsizer->Add( textctrl, 1, wxEXPAND);

    dialog->SetAutoLayout(true);
    dialog->SetSizer(dialogsizer);
    dialogsizer->Fit(dialog);
    dialogsizer->SetSizeHints(dialog);

    dialog->Show(true);
}

wxString wxVideoCaptureWindowBase::GetPropertiesString()
{
    wxString s;
    int i;

    s += wxT("---------------------------------------------\n");
    s += wxT("Video Devices\n\n");
    s += wxString::Format(wxT("Detected Number of Devices  : %d\n\n"), GetDeviceCount());
    for (i = 0; i < GetDeviceCount(); i++)
    {
        s += wxString::Format(wxT("%2d : Device Name    '%s'\n"), i, GetDeviceName(i).wx_str());
        s += wxString::Format(wxT("     Device Version '%s'\n"), GetDeviceVersion(i).wx_str());
    }
    s += wxT("\n");
    s += wxT("---------------------------------------------\n");
    s += wxT("Video Device Properties\n\n");
    s += wxString::Format(wxT("Currently Connected Device  : %d\n"), GetDeviceIndex());
    s += wxString::Format(wxT("IsDeviceConnected           : %d\n"), (int)IsDeviceConnected());
    s += wxString::Format(wxT("IsDeviceInitialized         : %d\n"), (int)IsDeviceInitialized());
    s += wxT("\n");
    s += wxT("---------------------------------------------\n");
    s += wxT("Video Dialogs\n\n");
    s += wxString::Format(wxT("HasVideoSourceDialog        : %d\n"), (int)HasVideoSourceDialog());
    s += wxString::Format(wxT("HasVideoFormatDialog        : %d\n"), (int)HasVideoFormatDialog());
    s += wxString::Format(wxT("HasVideoDisplayDialog       : %d\n"), (int)HasVideoDisplayDialog());
    s += wxT("\n");
    s += wxT("---------------------------------------------\n");
    s += wxT("Video Format Properties\n\n");
    s += wxString::Format(wxT("Image     width, height     : %4d, %4d\n"), GetImageWidth(), GetImageHeight());
    s += wxString::Format(wxT("Image min width, height     : %4d, %4d\n"), GetMinImageSize().GetWidth(), GetMinImageSize().GetHeight());
    s += wxString::Format(wxT("Image max width, height     : %4d, %4d\n"), GetMaxImageSize().GetWidth(), GetMaxImageSize().GetHeight());

    int width = 0, height = 0, bpp = 0;
    FOURCC fourcc = 0;
    GetVideoFormat( &width, &height, &bpp, &fourcc );

    wxString fourccStr(FOURCCTowxString(fourcc));
    if (fourcc == 0)
        fourccStr = wxT("0x0 (uncompressed)");
    else if (fourcc == wxNullFOURCC)
        fourccStr = wxT("-1 (invalid)");

    s += wxString::Format(wxT("Video format width, height  : %4d, %4d\n"), width, height);
    s += wxString::Format(wxT("Video format bits per pixel : %d\n"), bpp);
    s += wxString::Format(wxT("Video format compression    : %s\n"), fourccStr.c_str());
    s += wxString::Format(wxT("DriverSuppliesPalettes      : %d\n"), (int)DriverSuppliesPalettes());
    s += wxString::Format(wxT("IsUsingDefaultPalette       : %d\n"), (int)IsUsingDefaultPalette());
    s += wxT("\n");
    s += wxT("---------------------------------------------\n");
    s += wxString::Format(wxT("Video Preview Properties\n\n"));
    s += wxString::Format(wxT("IsPreviewing                : %d\n"), (int)IsPreviewing());
    s += wxString::Format(wxT("IsPreviewingwxImage         : %d\n"), (int)IsPreviewingwxImage());
    s += wxString::Format(wxT("IsPreviewScaled             : %d\n"), (int)IsPreviewScaled());
    s += wxString::Format(wxT("GetPreviewRateMS            : %u\n"), GetPreviewRateMS());
    s += wxString::Format(wxT("GetActualPreviewRateMS      : %u\n"), GetActualPreviewRateMS());
    s += wxString::Format(wxT("GetFrameNumber              : %u\n"), GetFrameNumber());
    s += wxString::Format(wxT("HasOverlay                  : %d\n"), (int)HasOverlay());
    s += wxString::Format(wxT("IsOverlaying                : %d\n"), (int)IsOverlaying());
    s += wxT("\n");

    return s;
}

// ----------------------------------------------------------------------
// Video format and characteristics
// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
// Capture Preview and Overlay
// ----------------------------------------------------------------------

void wxVideoCaptureWindowBase::OnPreFrame()
{
    wxLongLong millis_now = wxGetLocalTimeMillis();

    m_framenumber++;
    m_lastframetimemillis = millis_now;

    // get a nicer frame rate by averaging a few frames
    unsigned int frame_ave = (m_previewmsperframe > 500) ? 1 : 4;

    if (m_framenumber % frame_ave == 0)
    {
        m_actualpreviewmsperframe = (unsigned int)((millis_now.GetLo() - m_actualpreviewtimemillis.GetLo())/frame_ave);
        m_actualpreviewtimemillis = millis_now;
    }
}

void wxVideoCaptureWindowBase::OnPostFrame()
{
    wxVideoCaptureEvent event( wxEVT_VIDEO_FRAME, this, GetId() );
    event.SetFrameNumber( m_framenumber );
    event.SetFrameRateMS( m_actualpreviewmsperframe );
    GetEventHandler()->ProcessEvent(event);
}

void wxVideoCaptureWindowBase::DoPaint(wxPaintDC& dc)
{
    PrepareDC( dc );

    wxSize clientSize(GetClientSize());

    wxRect backRectRight;
    wxRect backRectBottom;

    // Calculate the rects to the right and bottom that we should cleanup
    if (m_wximage.Ok())// && !m_getting_wximage && m_preview_wximage)
    {
        if (!m_previewscaled)
        {
            int w = m_wximage.GetWidth();
            int h = m_wximage.GetHeight();
            int xx = 0, yy = 0;

            CalcScrolledPosition(w, h, &xx, &yy);

            backRectRight  = wxRect(xx,  0, clientSize.GetWidth()-xx, clientSize.GetHeight());
            backRectBottom = wxRect( 0, yy, xx+1,                     clientSize.GetHeight()-yy);

            //wxPrintf("r %4d %4d %4d %4d \n", backRectRight.x, backRectRight.y, backRectRight.width, backRectRight.height );
            //wxPrintf("b %4d %4d %4d %4d \n", backRectBottom.x, backRectBottom.y, backRectBottom.width, backRectBottom.height );
        }
    }
    else
    {
        backRectRight = wxRect(0, 0, clientSize.GetWidth(), clientSize.GetHeight());
    }

    if (!backRectRight.IsEmpty() || !backRectBottom.IsEmpty())
    {
        wxColour backColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
        wxPen    backPen(backColour, 1, wxPENSTYLE_SOLID);
        wxBrush  backBrush(backColour, wxBRUSHSTYLE_SOLID);

        dc.SetPen(backPen);
        dc.SetBrush(backBrush);

        if (!backRectRight.IsEmpty())  dc.DrawRectangle(backRectRight);
        if (!backRectBottom.IsEmpty()) dc.DrawRectangle(backRectBottom);
    }

    if (m_wximage.Ok())// && !m_getting_wximage && m_preview_wximage)
    {
        if (m_previewscaled)
            dc.DrawBitmap( wxBitmap(m_wximage.Scale(clientSize.x, clientSize.y)), 0, 0 );
        else
            dc.DrawBitmap(wxBitmap(m_wximage), 0, 0);
    }
}

// ----------------------------------------------------------------------
// Capture single frames, take snapshots of streaming video
// ----------------------------------------------------------------------

wxImage wxVideoCaptureWindowBase::GetwxImage(bool full_copy) const
{
    if (!full_copy)
        return m_wximage;

    // Make a full copy of the image
    if (m_wximage.Ok() && !m_getting_wximage)
    {
        int width  = m_wximage.GetWidth();
        int height = m_wximage.GetHeight();
        wxImage outimage(width, height);

        unsigned char *m_wximgptr = m_wximage.GetData();
        unsigned char *outimgptr  = outimage.GetData();

        memcpy(outimgptr, m_wximgptr, width*height*3);
        return outimage;
    }

    return wxImage();
}

// ----------------------------------------------------------------------
// Capture (append) single video frames to an AVI file
// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
// Capture streaming video to an AVI file
// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
// Capture file settings, filename to capture video to
// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
// Audio Setup
// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
// Utility Functions
// ----------------------------------------------------------------------

bool wxVideoCaptureWindowBase::SendErrorEvent(int error_num, const wxString& errorMessage)
{
    m_last_error_num    = error_num;
    m_last_errorMessage = errorMessage;

    wxVideoCaptureEvent event( wxEVT_VIDEO_ERROR, this, GetId() );
    event.SetErrorMessage(m_last_errorMessage);
    return GetEventHandler()->ProcessEvent(event);
}

// find the size of a file in KB
long int wxVideoCaptureWindowBase::GetFileSizeInKB( const wxString &filename ) const
{
    if (!wxFileExists(filename)) return -1;
    wxFile file(filename, wxFile::read);
    if (!file.IsOpened()) return -1; // file doesn't exist ?

    wxFileOffset filesize = file.Length();
    file.Close();
    return filesize/1024L;
}

//-------------------------------------------------------------------------
// wxVideoCaptureFormat manipulation
//-------------------------------------------------------------------------

int wxVideoCaptureWindowBase::GetVideoCaptureFormatCount() const
{
    CreateVideoCaptureFormatArray();
    return sm_wxVideoCaptureFormats.GetCount();
}

const wxArrayVideoCaptureFormat &wxVideoCaptureWindowBase::GetVideoCaptureFormatArray() const
{
    CreateVideoCaptureFormatArray();
    return sm_wxVideoCaptureFormats;
}

const wxVideoCaptureFormat& wxVideoCaptureWindowBase::GetVideoCaptureFormat(int index) const
{
    CreateVideoCaptureFormatArray();
    wxCHECK_MSG((index >= 0) && (index < int(sm_wxVideoCaptureFormats.GetCount())), wxNullVideoCaptureFormat,
                 wxT("invalid index in GetVideoCaptureFormat"));
    return sm_wxVideoCaptureFormats[index];
}

void wxVideoCaptureWindowBase::RegisterVideoCaptureFormat(const wxVideoCaptureFormat& videoCaptureFormat)
{
    CreateVideoCaptureFormatArray();
    sm_wxVideoCaptureFormats.Add(videoCaptureFormat);
}

int wxVideoCaptureWindowBase::FindVideoCaptureFormatFOURCC(FOURCC fourcc) const
{
    CreateVideoCaptureFormatArray();
    int i, count = sm_wxVideoCaptureFormats.GetCount();
    for (i = 0; i < count; ++i)
    {
        if (sm_wxVideoCaptureFormats[i].m_fourcc == fourcc)
            return i;
    }
    return wxNOT_FOUND;
}

int wxVideoCaptureWindowBase::FindVideoCaptureFormatV4L2pixelformat(FOURCC v4l2_pixelformat) const
{
    CreateVideoCaptureFormatArray();
    int i, count = sm_wxVideoCaptureFormats.GetCount();
    for (i = 0; i < count; ++i)
    {
        if (sm_wxVideoCaptureFormats[i].m_v4l2_pixelformat == v4l2_pixelformat)
            return i;
    }
    return wxNOT_FOUND;
}

#ifndef WXVIDCAP_LINUX_V4L

#define v4l2_fourcc CHARS_TO_FOURCC

// These FOURCCs are from Linux's <linux/videodev2.h>
// They're copied here since one list is as good as another and we might as
//   well start with V4L2's list and add the MSW ones to it.
// See here for other lists:
//  http://www.opennet.ru/docs/formats/avi_overview.html
//  http://www.fourcc.org/codecs.php
//  http://www.free-codecs.com/guides/FourCC.htm
//  http://msdn.microsoft.com/en-us/library/windows/desktop/bb970509%28v=vs.85%29.aspx

/*      Pixel format         FOURCC                          depth  Description  */

/* RGB formats */
#define V4L2_PIX_FMT_RGB332  v4l2_fourcc('R', 'G', 'B', '1') /*  8  RGB-3-3-2     */
#define V4L2_PIX_FMT_RGB444  v4l2_fourcc('R', '4', '4', '4') /* 16  xxxxrrrr ggggbbbb */
#define V4L2_PIX_FMT_RGB555  v4l2_fourcc('R', 'G', 'B', 'O') /* 16  RGB-5-5-5     */
#define V4L2_PIX_FMT_RGB565  v4l2_fourcc('R', 'G', 'B', 'P') /* 16  RGB-5-6-5     */
#define V4L2_PIX_FMT_RGB555X v4l2_fourcc('R', 'G', 'B', 'Q') /* 16  RGB-5-5-5 BE  */
#define V4L2_PIX_FMT_RGB565X v4l2_fourcc('R', 'G', 'B', 'R') /* 16  RGB-5-6-5 BE  */
#define V4L2_PIX_FMT_BGR24   v4l2_fourcc('B', 'G', 'R', '3') /* 24  BGR-8-8-8     */
#define V4L2_PIX_FMT_RGB24   v4l2_fourcc('R', 'G', 'B', '3') /* 24  RGB-8-8-8     */
#define V4L2_PIX_FMT_BGR32   v4l2_fourcc('B', 'G', 'R', '4') /* 32  BGR-8-8-8-8   */
#define V4L2_PIX_FMT_RGB32   v4l2_fourcc('R', 'G', 'B', '4') /* 32  RGB-8-8-8-8   */

/* Grey formats */
#define V4L2_PIX_FMT_GREY    v4l2_fourcc('G', 'R', 'E', 'Y') /*  8  Greyscale     */
#define V4L2_PIX_FMT_Y10     v4l2_fourcc('Y', '1', '0', ' ') /* 10  Greyscale     */
#define V4L2_PIX_FMT_Y16     v4l2_fourcc('Y', '1', '6', ' ') /* 16  Greyscale     */

/* Palette formats */
#define V4L2_PIX_FMT_PAL8    v4l2_fourcc('P', 'A', 'L', '8') /*  8  8-bit palette */

/* Luminance+Chrominance formats */
#define V4L2_PIX_FMT_YVU410  v4l2_fourcc('Y', 'V', 'U', '9') /*  9  YVU 4:1:0     */
#define V4L2_PIX_FMT_YVU420  v4l2_fourcc('Y', 'V', '1', '2') /* 12  YVU 4:2:0     */
#define V4L2_PIX_FMT_YUYV    v4l2_fourcc('Y', 'U', 'Y', 'V') /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_YYUV    v4l2_fourcc('Y', 'Y', 'U', 'V') /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_YVYU    v4l2_fourcc('Y', 'V', 'Y', 'U') /* 16 YVU 4:2:2 */
#define V4L2_PIX_FMT_UYVY    v4l2_fourcc('U', 'Y', 'V', 'Y') /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_VYUY    v4l2_fourcc('V', 'Y', 'U', 'Y') /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_YUV422P v4l2_fourcc('4', '2', '2', 'P') /* 16  YVU422 planar */
#define V4L2_PIX_FMT_YUV411P v4l2_fourcc('4', '1', '1', 'P') /* 16  YVU411 planar */
#define V4L2_PIX_FMT_Y41P    v4l2_fourcc('Y', '4', '1', 'P') /* 12  YUV 4:1:1     */
#define V4L2_PIX_FMT_YUV444  v4l2_fourcc('Y', '4', '4', '4') /* 16  xxxxyyyy uuuuvvvv */
#define V4L2_PIX_FMT_YUV555  v4l2_fourcc('Y', 'U', 'V', 'O') /* 16  YUV-5-5-5     */
#define V4L2_PIX_FMT_YUV565  v4l2_fourcc('Y', 'U', 'V', 'P') /* 16  YUV-5-6-5     */
#define V4L2_PIX_FMT_YUV32   v4l2_fourcc('Y', 'U', 'V', '4') /* 32  YUV-8-8-8-8   */
#define V4L2_PIX_FMT_YUV410  v4l2_fourcc('Y', 'U', 'V', '9') /*  9  YUV 4:1:0     */
#define V4L2_PIX_FMT_YUV420  v4l2_fourcc('Y', 'U', '1', '2') /* 12  YUV 4:2:0     */
#define V4L2_PIX_FMT_HI240   v4l2_fourcc('H', 'I', '2', '4') /*  8  8-bit color   */
#define V4L2_PIX_FMT_HM12    v4l2_fourcc('H', 'M', '1', '2') /*  8  YUV 4:2:0 16x16 macroblocks */

/* two planes -- one Y, one Cr + Cb interleaved  */
#define V4L2_PIX_FMT_NV12    v4l2_fourcc('N', 'V', '1', '2') /* 12  Y/CbCr 4:2:0  */
#define V4L2_PIX_FMT_NV21    v4l2_fourcc('N', 'V', '2', '1') /* 12  Y/CrCb 4:2:0  */
#define V4L2_PIX_FMT_NV16    v4l2_fourcc('N', 'V', '1', '6') /* 16  Y/CbCr 4:2:2  */
#define V4L2_PIX_FMT_NV61    v4l2_fourcc('N', 'V', '6', '1') /* 16  Y/CrCb 4:2:2  */

/* Bayer formats - see http://www.siliconimaging.com/RGB%20Bayer.htm */
#define V4L2_PIX_FMT_SBGGR8  v4l2_fourcc('B', 'A', '8', '1') /*  8  BGBG.. GRGR.. */
#define V4L2_PIX_FMT_SGBRG8  v4l2_fourcc('G', 'B', 'R', 'G') /*  8  GBGB.. RGRG.. */
#define V4L2_PIX_FMT_SGRBG8  v4l2_fourcc('G', 'R', 'B', 'G') /*  8  GRGR.. BGBG.. */
#define V4L2_PIX_FMT_SRGGB8  v4l2_fourcc('R', 'G', 'G', 'B') /*  8  RGRG.. GBGB.. */
#define V4L2_PIX_FMT_SBGGR10 v4l2_fourcc('B', 'G', '1', '0') /* 10  BGBG.. GRGR.. */
#define V4L2_PIX_FMT_SGBRG10 v4l2_fourcc('G', 'B', '1', '0') /* 10  GBGB.. RGRG.. */
#define V4L2_PIX_FMT_SGRBG10 v4l2_fourcc('B', 'A', '1', '0') /* 10  GRGR.. BGBG.. */
#define V4L2_PIX_FMT_SRGGB10 v4l2_fourcc('R', 'G', '1', '0') /* 10  RGRG.. GBGB.. */
    /* 10bit raw bayer DPCM compressed to 8 bits */
#define V4L2_PIX_FMT_SGRBG10DPCM8 v4l2_fourcc('B', 'D', '1', '0')
    /*
     * 10bit raw bayer, expanded to 16 bits
     * xxxxrrrrrrrrrrxxxxgggggggggg xxxxggggggggggxxxxbbbbbbbbbb...
     */
#define V4L2_PIX_FMT_SBGGR16 v4l2_fourcc('B', 'Y', 'R', '2') /* 16  BGBG.. GRGR.. */

/* compressed formats */
#define V4L2_PIX_FMT_MJPEG    v4l2_fourcc('M', 'J', 'P', 'G') /* Motion-JPEG   */
#define V4L2_PIX_FMT_JPEG     v4l2_fourcc('J', 'P', 'E', 'G') /* JFIF JPEG     */
#define V4L2_PIX_FMT_DV       v4l2_fourcc('d', 'v', 's', 'd') /* 1394          */
#define V4L2_PIX_FMT_MPEG     v4l2_fourcc('M', 'P', 'E', 'G') /* MPEG-1/2/4    */

/*  Vendor-specific formats   */
#define V4L2_PIX_FMT_CPIA1    v4l2_fourcc('C', 'P', 'I', 'A') /* cpia1 YUV */
#define V4L2_PIX_FMT_WNVA     v4l2_fourcc('W', 'N', 'V', 'A') /* Winnov hw compress */
#define V4L2_PIX_FMT_SN9C10X  v4l2_fourcc('S', '9', '1', '0') /* SN9C10x compression */
#define V4L2_PIX_FMT_SN9C20X_I420 v4l2_fourcc('S', '9', '2', '0') /* SN9C20x YUV 4:2:0 */
#define V4L2_PIX_FMT_PWC1     v4l2_fourcc('P', 'W', 'C', '1') /* pwc older webcam */
#define V4L2_PIX_FMT_PWC2     v4l2_fourcc('P', 'W', 'C', '2') /* pwc newer webcam */
#define V4L2_PIX_FMT_ET61X251 v4l2_fourcc('E', '6', '2', '5') /* ET61X251 compression */
#define V4L2_PIX_FMT_SPCA501  v4l2_fourcc('S', '5', '0', '1') /* YUYV per line */
#define V4L2_PIX_FMT_SPCA505  v4l2_fourcc('S', '5', '0', '5') /* YYUV per line */
#define V4L2_PIX_FMT_SPCA508  v4l2_fourcc('S', '5', '0', '8') /* YUVY per line */
#define V4L2_PIX_FMT_SPCA561  v4l2_fourcc('S', '5', '6', '1') /* compressed GBRG bayer */
#define V4L2_PIX_FMT_PAC207   v4l2_fourcc('P', '2', '0', '7') /* compressed BGGR bayer */
#define V4L2_PIX_FMT_MR97310A v4l2_fourcc('M', '3', '1', '0') /* compressed BGGR bayer */
#define V4L2_PIX_FMT_SN9C2028 v4l2_fourcc('S', 'O', 'N', 'X') /* compressed GBRG bayer */
#define V4L2_PIX_FMT_SQ905C   v4l2_fourcc('9', '0', '5', 'C') /* compressed RGGB bayer */
#define V4L2_PIX_FMT_PJPG     v4l2_fourcc('P', 'J', 'P', 'G') /* Pixart 73xx JPEG */
#define V4L2_PIX_FMT_OV511    v4l2_fourcc('O', '5', '1', '1') /* ov511 JPEG */
#define V4L2_PIX_FMT_OV518    v4l2_fourcc('O', '5', '1', '8') /* ov518 JPEG */
#define V4L2_PIX_FMT_STV0680  v4l2_fourcc('S', '6', '8', '0') /* stv0680 bayer */

#endif // WXVIDCAP_LINUX_V4L

#define ADD_VCF(a,b,c,d) sm_wxVideoCaptureFormats.Add(wxVideoCaptureFormat(a,b,c,d));

void wxVideoCaptureWindowBase::CreateVideoCaptureFormatArray() const
{
    if (sm_wxVideoCaptureFormats.GetCount() > 0u) return;

//.name  .palette .fourcc    .depth .flags .hshift .vshift
//"8 bpp, gray", VIDEO_PALETTE_GREY, V4L2_PIX_FMT_GREY, 8, FORMAT_FLAGS_PACKED,
//"8 bpp, dithered color", VIDEO_PALETTE_HI240, V4L2_PIX_FMT_HI240, 8, FORMAT_FLAGS_PACKED | FORMAT_FLAGS_DITHER,
//"15 bpp RGB, le", VIDEO_PALETTE_RGB555, V4L2_PIX_FMT_RGB555, 16, FORMAT_FLAGS_PACKED,
//"15 bpp RGB, be", -1, V4L2_PIX_FMT_RGB555X, 0x03, /* byteswap */ 16, FORMAT_FLAGS_PACKED,
//"16 bpp RGB, le", VIDEO_PALETTE_RGB565, V4L2_PIX_FMT_RGB565, 16, FORMAT_FLAGS_PACKED,
//"16 bpp RGB, be", -1, V4L2_PIX_FMT_RGB565X, BT848_COLOR_FMT_RGB16, 0x03, /* byteswap */ 16, FORMAT_FLAGS_PACKED,
//"24 bpp RGB, le", VIDEO_PALETTE_RGB24, V4L2_PIX_FMT_BGR24, BT848_COLOR_FMT_RGB24, 24, FORMAT_FLAGS_PACKED,
//"32 bpp RGB, le", VIDEO_PALETTE_RGB32, V4L2_PIX_FMT_BGR32, BT848_COLOR_FMT_RGB32, 32, FORMAT_FLAGS_PACKED,
//"32 bpp RGB, be", -1, V4L2_PIX_FMT_RGB32, BT848_COLOR_FMT_RGB32, 0x0f, /* byte+word swap */ 32, FORMAT_FLAGS_PACKED,
//"4:2:2, packed, YUYV", VIDEO_PALETTE_YUV422, V4L2_PIX_FMT_YUYV, BT848_COLOR_FMT_YUY2, 16, FORMAT_FLAGS_PACKED,
//"4:2:2, packed, YUYV", VIDEO_PALETTE_YUYV, V4L2_PIX_FMT_YUYV, BT848_COLOR_FMT_YUY2, 16, FORMAT_FLAGS_PACKED,
//"4:2:2, packed, UYVY", VIDEO_PALETTE_UYVY, V4L2_PIX_FMT_UYVY, BT848_COLOR_FMT_YUY2, 0x03, /* byteswap */ 16, FORMAT_FLAGS_PACKED,
//"4:2:2, planar, Y-Cb-Cr", VIDEO_PALETTE_YUV422P, V4L2_PIX_FMT_YUV422P, BT848_COLOR_FMT_YCrCb422, 16, FORMAT_FLAGS_PLANAR, 1, 0,
//"4:2:0, planar, Y-Cb-Cr", VIDEO_PALETTE_YUV420P, V4L2_PIX_FMT_YUV420, BT848_COLOR_FMT_YCrCb422, 12, FORMAT_FLAGS_PLANAR, 1, 1,
//"4:2:0, planar, Y-Cr-Cb", -1, V4L2_PIX_FMT_YVU420, BT848_COLOR_FMT_YCrCb422, 12, FORMAT_FLAGS_PLANAR | FORMAT_FLAGS_CrCb, 1, 1,
//"4:1:1, planar, Y-Cb-Cr", VIDEO_PALETTE_YUV411P, V4L2_PIX_FMT_YUV411P, BT848_COLOR_FMT_YCrCb411, 12, FORMAT_FLAGS_PLANAR, 2, 0,
//"4:1:0, planar, Y-Cb-Cr", VIDEO_PALETTE_YUV410P, V4L2_PIX_FMT_YUV410, BT848_COLOR_FMT_YCrCb411, 9, FORMAT_FLAGS_PLANAR, 2, 2,
//"4:1:0, planar, Y-Cr-Cb", -1, V4L2_PIX_FMT_YVU410, BT848_COLOR_FMT_YCrCb411, 9, FORMAT_FLAGS_PLANAR | FORMAT_FLAGS_CrCb, 2, 2,
//"raw scanlines", VIDEO_PALETTE_RAW, -1, BT848_COLOR_FMT_RAW, 8, FORMAT_FLAGS_RAW,

    // The formats added below are the ones that are thought to be used.
    // Other ones should be added as needed, but the wxVideoCaptureWindowCustomVideoFormatDialog
    // will actually call RegisterVideoCaptureFormat() to add any unknown ones
    // so, hopefully, we don't have to declare every possible format ever.

    // RGB formats
    ADD_VCF(wxT("DIB Uncompressed"),                v4l2_fourcc('D', 'I', 'B', ' '), wxNullFOURCC,          24) // MSW
    ADD_VCF(wxT("RGB Uncompressed"),                v4l2_fourcc('R', 'G', 'B', ' '), wxNullFOURCC,          24) // MSW
    ADD_VCF(wxT("RAW Uncompressed"),                v4l2_fourcc('R', 'A', 'W', ' '), wxNullFOURCC,          24) // MSW
    ADD_VCF(wxT("BI_RGB Uncompressed"),             0,                               wxNullFOURCC,          24) // MSW
    ADD_VCF(wxT("Y8  greyscale"),                   v4l2_fourcc('Y', '8', ' ', ' '), V4L2_PIX_FMT_GREY,     8)  // MSW

    ADD_VCF(wxT("8   RGB-3-3-2"),                   v4l2_fourcc('R', 'G', 'B', '1'), V4L2_PIX_FMT_RGB332,   8)  // V4L2
    ADD_VCF(wxT("16  xxxxrrrr ggggbbbb"),           v4l2_fourcc('R', '4', '4', '4'), V4L2_PIX_FMT_RGB444,   16) // V4L2
    ADD_VCF(wxT("16  RGB-5-5-5"),                   v4l2_fourcc('R', 'G', 'B', 'O'), V4L2_PIX_FMT_RGB555,   16) // V4L2
    ADD_VCF(wxT("16  RGB-5-6-5"),                   v4l2_fourcc('R', 'G', 'B', 'P'), V4L2_PIX_FMT_RGB565,   16) // V4L2
    ADD_VCF(wxT("16  RGB-5-5-5 BE"),                v4l2_fourcc('R', 'G', 'B', 'Q'), V4L2_PIX_FMT_RGB555X,  16) // V4L2
    ADD_VCF(wxT("16  RGB-5-6-5 BE"),                v4l2_fourcc('R', 'G', 'B', 'R'), V4L2_PIX_FMT_RGB565X,  16) // V4L2
    ADD_VCF(wxT("24  BGR-8-8-8"),                   v4l2_fourcc('B', 'G', 'R', '3'), V4L2_PIX_FMT_BGR24,    24) // V4L2
    ADD_VCF(wxT("24  RGB-8-8-8"),                   v4l2_fourcc('R', 'G', 'B', '3'), V4L2_PIX_FMT_RGB24,    24) // V4L2
    ADD_VCF(wxT("32  BGR-8-8-8-8"),                 v4l2_fourcc('B', 'G', 'R', '4'), V4L2_PIX_FMT_BGR32,    32) // V4L2
    ADD_VCF(wxT("32  RGB-8-8-8-8"),                 v4l2_fourcc('R', 'G', 'B', '4'), V4L2_PIX_FMT_RGB32,    32) // V4L2
    // Grey formats
    ADD_VCF(wxT("8   Greyscale"),                   v4l2_fourcc('G', 'R', 'E', 'Y'), V4L2_PIX_FMT_GREY,     8)  // V4L2
    ADD_VCF(wxT("10  Greyscale"),                   v4l2_fourcc('Y', '1', '0', ' '), V4L2_PIX_FMT_Y10,      10) // V4L2
    ADD_VCF(wxT("16  Greyscale"),                   v4l2_fourcc('Y', '1', '6', ' '), V4L2_PIX_FMT_Y16,      16) // V4L2
    // Palette formats
    ADD_VCF(wxT("8   8-bit palette"),               v4l2_fourcc('P', 'A', 'L', '8'), V4L2_PIX_FMT_PAL8,     8) // V4L2
    // Luminance+Chrominance formats
    ADD_VCF(wxT("9   YVU 4:1:0 (Indeo Raw) = YUV9"),v4l2_fourcc('Y', 'V', 'U', '9'), V4L2_PIX_FMT_YVU410,   9)  // V4L2
    ADD_VCF(wxT("12  YVU 4:2:0 (MPEG Raw)"),        v4l2_fourcc('Y', 'V', '1', '2'), V4L2_PIX_FMT_YVU420,   12) // V4L2
    ADD_VCF(wxT("16  YUV 4:2:2"),                   v4l2_fourcc('Y', 'U', 'Y', 'V'), V4L2_PIX_FMT_YUYV,     16) // V4L2
    ADD_VCF(wxT("16  YUV 4:2:2"),                   v4l2_fourcc('Y', 'Y', 'U', 'V'), V4L2_PIX_FMT_YYUV,     16) // V4L2
    ADD_VCF(wxT("16  YVU 4:2:2"),                   v4l2_fourcc('Y', 'V', 'Y', 'U'), V4L2_PIX_FMT_YVYU,     16) // V4L2
    ADD_VCF(wxT("16  YUV 4:2:2"),                   v4l2_fourcc('U', 'Y', 'V', 'Y'), V4L2_PIX_FMT_UYVY,     16) // V4L2
    ADD_VCF(wxT("16  YUV 4:2:2"),                   v4l2_fourcc('V', 'Y', 'U', 'Y'), V4L2_PIX_FMT_VYUY,     16) // V4L2
    ADD_VCF(wxT("16  YVU422 planar"),               v4l2_fourcc('4', '2', '2', 'P'), V4L2_PIX_FMT_YUV422P,  16) // V4L2
    ADD_VCF(wxT("16  YVU411 planar"),               v4l2_fourcc('4', '1', '1', 'P'), V4L2_PIX_FMT_YUV411P,  16) // V4L2
    ADD_VCF(wxT("12  YUV 4:1:1"),                   v4l2_fourcc('Y', '4', '1', 'P'), V4L2_PIX_FMT_Y41P,     12) // V4L2
    ADD_VCF(wxT("16  xxxxyyyy uuuuvvvv"),           v4l2_fourcc('Y', '4', '4', '4'), V4L2_PIX_FMT_YUV444,   16) // V4L2
    ADD_VCF(wxT("16  YUV-5-5-5"),                   v4l2_fourcc('Y', 'U', 'V', 'O'), V4L2_PIX_FMT_YUV555,   16) // V4L2
    ADD_VCF(wxT("16  YUV-5-6-5"),                   v4l2_fourcc('Y', 'U', 'V', 'P'), V4L2_PIX_FMT_YUV565,   16) // V4L2
    ADD_VCF(wxT("32  YUV-8-8-8-8"),                 v4l2_fourcc('Y', 'U', 'V', '4'), V4L2_PIX_FMT_YUV32,    32) // V4L2
    ADD_VCF(wxT("9   YUV 4:1:0 (Indeo Raw) = YVU9"),v4l2_fourcc('Y', 'U', 'V', '9'), V4L2_PIX_FMT_YUV410,   9)  // V4L2
    ADD_VCF(wxT("12  YUV 4:2:0"),                   v4l2_fourcc('Y', 'U', '1', '2'), V4L2_PIX_FMT_YUV420,   12) // V4L2
    ADD_VCF(wxT("8   8-bit color"),                 v4l2_fourcc('H', 'I', '2', '4'), V4L2_PIX_FMT_HI240,    8)  // V4L2
    ADD_VCF(wxT("8   YUV 4:2:0 16x16 macroblocks"), v4l2_fourcc('H', 'M', '1', '2'), V4L2_PIX_FMT_HM12,     8)  // V4L2

    ADD_VCF(wxT("12  YUV 4:2:0 planar = YUV12"),    v4l2_fourcc('I', '4', '2', '0'), V4L2_PIX_FMT_YUV420, 12) // MSW
    ADD_VCF(wxT("12  YUV 4:2:0 planar = YVU12"),    v4l2_fourcc('I', 'Y', 'U', 'V'), V4L2_PIX_FMT_YUV420, 12) // MSW
    ADD_VCF(wxT("12  YUV 4:1:1 planar = BTYUV"),    v4l2_fourcc('Y', '4', '1', 'P'), V4L2_PIX_FMT_Y41P,   12) // MSW
    ADD_VCF(wxT("16  Inverted YUV 4:2:2"),          v4l2_fourcc('C', 'Y', 'U', 'V'), wxNullFOURCC,        16) // MSW
    ADD_VCF(wxT("16  YUV 4:2:2 interleaved"),       v4l2_fourcc('Y', 'U', 'Y', '2'), wxNullFOURCC,        16) // MSW

    /* two planes -- one Y, one Cr + Cb interleaved  */
    //#define V4L2_PIX_FMT_NV12    v4l2_fourcc('N', 'V', '1', '2') /* 12  Y/CbCr 4:2:0  */
    //#define V4L2_PIX_FMT_NV21    v4l2_fourcc('N', 'V', '2', '1') /* 12  Y/CrCb 4:2:0  */
    //#define V4L2_PIX_FMT_NV16    v4l2_fourcc('N', 'V', '1', '6') /* 16  Y/CbCr 4:2:2  */
    //#define V4L2_PIX_FMT_NV61    v4l2_fourcc('N', 'V', '6', '1') /* 16  Y/CrCb 4:2:2  */

    /* Bayer formats - see http://www.siliconimaging.com/RGB%20Bayer.htm */
    //#define V4L2_PIX_FMT_SBGGR8  v4l2_fourcc('B', 'A', '8', '1') /*  8  BGBG.. GRGR.. */
    //#define V4L2_PIX_FMT_SGBRG8  v4l2_fourcc('G', 'B', 'R', 'G') /*  8  GBGB.. RGRG.. */
    //#define V4L2_PIX_FMT_SGRBG8  v4l2_fourcc('G', 'R', 'B', 'G') /*  8  GRGR.. BGBG.. */
    //#define V4L2_PIX_FMT_SRGGB8  v4l2_fourcc('R', 'G', 'G', 'B') /*  8  RGRG.. GBGB.. */
    //#define V4L2_PIX_FMT_SBGGR10 v4l2_fourcc('B', 'G', '1', '0') /* 10  BGBG.. GRGR.. */
    //#define V4L2_PIX_FMT_SGBRG10 v4l2_fourcc('G', 'B', '1', '0') /* 10  GBGB.. RGRG.. */
    //#define V4L2_PIX_FMT_SGRBG10 v4l2_fourcc('B', 'A', '1', '0') /* 10  GRGR.. BGBG.. */
    //#define V4L2_PIX_FMT_SRGGB10 v4l2_fourcc('R', 'G', '1', '0') /* 10  RGRG.. GBGB.. */
    /* 10bit raw bayer DPCM compressed to 8 bits */
    //#define V4L2_PIX_FMT_SGRBG10DPCM8 v4l2_fourcc('B', 'D', '1', '0')
    /*
     * 10bit raw bayer, expanded to 16 bits
     * xxxxrrrrrrrrrrxxxxgggggggggg xxxxggggggggggxxxxbbbbbbbbbb...
     */
    //#define V4L2_PIX_FMT_SBGGR16 v4l2_fourcc('B', 'Y', 'R', '2') /* 16  BGBG.. GRGR.. */

    // compressed formats
    ADD_VCF(wxT("Motion-JPEG"),                     v4l2_fourcc('M', 'J', 'P', 'G'), V4L2_PIX_FMT_MJPEG,    0) // V4L2
    ADD_VCF(wxT("JFIF JPEG"),                       v4l2_fourcc('J', 'P', 'E', 'G'), V4L2_PIX_FMT_JPEG,     0) // V4L2
    ADD_VCF(wxT("1394"),                            v4l2_fourcc('d', 'v', 's', 'd'), V4L2_PIX_FMT_DV,       0) // V4L2
    ADD_VCF(wxT("MPEG-1/2/4"),                      v4l2_fourcc('M', 'P', 'E', 'G'), V4L2_PIX_FMT_MPEG,     0) // V4L2

    //  Vendor-specific formats
    ADD_VCF(wxT("cpia1 YUV"),                       v4l2_fourcc('C', 'P', 'I', 'A'), V4L2_PIX_FMT_CPIA1,    8) // V4L2
    ADD_VCF(wxT("Winnov hw compress"),              v4l2_fourcc('W', 'N', 'V', 'A'), V4L2_PIX_FMT_WNVA,     8) // V4L2
    ADD_VCF(wxT("SN9C10x compression"),             v4l2_fourcc('S', '9', '1', '0'), V4L2_PIX_FMT_SN9C10X,  8) // V4L2
    ADD_VCF(wxT("SN9C20x YUV 4:2:0"),               v4l2_fourcc('S', '9', '2', '0'), V4L2_PIX_FMT_SN9C20X_I420, 8) // V4L2
    ADD_VCF(wxT("pwc older webcam"),                v4l2_fourcc('P', 'W', 'C', '1'), V4L2_PIX_FMT_PWC1,     8) // V4L2
    ADD_VCF(wxT("pwc newer webcam"),                v4l2_fourcc('P', 'W', 'C', '2'), V4L2_PIX_FMT_PWC2,     8) // V4L2
    ADD_VCF(wxT("ET61X251 compression"),            v4l2_fourcc('E', '6', '2', '5'), V4L2_PIX_FMT_ET61X251, 8) // V4L2
    ADD_VCF(wxT("YUYV per line"),                   v4l2_fourcc('S', '5', '0', '1'), V4L2_PIX_FMT_SPCA501,  8) // V4L2
    ADD_VCF(wxT("YYUV per line"),                   v4l2_fourcc('S', '5', '0', '5'), V4L2_PIX_FMT_SPCA505,  8) // V4L2
    ADD_VCF(wxT("YUVY per line"),                   v4l2_fourcc('S', '5', '0', '8'), V4L2_PIX_FMT_SPCA508,  8) // V4L2
    ADD_VCF(wxT("compressed GBRG bayer"),           v4l2_fourcc('S', '5', '6', '1'), V4L2_PIX_FMT_SPCA561,  8) // V4L2
    ADD_VCF(wxT("compressed BGGR bayer"),           v4l2_fourcc('P', '2', '0', '7'), V4L2_PIX_FMT_PAC207,   8) // V4L2
    ADD_VCF(wxT("compressed BGGR bayer"),           v4l2_fourcc('M', '3', '1', '0'), V4L2_PIX_FMT_MR97310A, 8) // V4L2
    ADD_VCF(wxT("compressed GBRG bayer"),           v4l2_fourcc('S', 'O', 'N', 'X'), V4L2_PIX_FMT_SN9C2028, 8) // V4L2
    ADD_VCF(wxT("compressed RGGB bayer"),           v4l2_fourcc('9', '0', '5', 'C'), V4L2_PIX_FMT_SQ905C,   8) // V4L2
    ADD_VCF(wxT("Pixart 73xx JPEG"),                v4l2_fourcc('P', 'J', 'P', 'G'), V4L2_PIX_FMT_PJPG,     8) // V4L2
    ADD_VCF(wxT("ov511 JPEG/"),                     v4l2_fourcc('O', '5', '1', '1'), V4L2_PIX_FMT_OV511,    8) // V4L2
    ADD_VCF(wxT("ov518 JPEG"),                      v4l2_fourcc('O', '5', '1', '8'), V4L2_PIX_FMT_OV518,    8) // V4L2
    ADD_VCF(wxT("stv0680 bayer"),                   v4l2_fourcc('S', '6', '8', '0'), V4L2_PIX_FMT_STV0680,  8) // V4L2
    ADD_VCF(wxT("stv0680 bayer"),                   v4l2_fourcc('S', '6', '8', '0'), V4L2_PIX_FMT_STV0680,  8) // V4L2
    ADD_VCF(wxT("stv0680 bayer"),                   v4l2_fourcc('S', '6', '8', '0'), V4L2_PIX_FMT_STV0680,  8) // V4L2
    ADD_VCF(wxT("stv0680 bayer"),                   v4l2_fourcc('S', '6', '8', '0'), V4L2_PIX_FMT_STV0680,  8) // V4L2

    //_vcf_(wxT("dmb1   Matrox MJPEG"),                   STRING_TO_FOURCC("dmb1"),   16, -1)
    //_vcf_(wxT("JBYR   se401/Kensington"),               STRING_TO_FOURCC("JBYR"),   20, -1)
    //_vcf_(wxT("ABYR   se401/Kensington"),               STRING_TO_FOURCC("ABYR"),    8, -1)
    //_vcf_(wxT("VCR1   ATI VCR 1.0"),                    STRING_TO_FOURCC("VCR1"),   16, -1)
    //_vcf_(wxT("VCR2   ATI VCR 2.0"),                    STRING_TO_FOURCC("VCR2"),   12, -1)
}
