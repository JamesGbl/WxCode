//TODO:
//
// What is the MSW capture window's ID? it's 125 now...
// EVT_USER + 101? set to a real value...
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
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "wx/vidcap/vcapwin.h"
#include <wx/file.h>

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
    for (unsigned int i=0; i<4; i++) s4[i] = s[i];
    s4[4] = 0;
    return s4;
}

FOURCC StringToFOURCC( const char *s )
{
    char s4[5] = "    "; // when the FOURCC is not 4 chars like "Y8"

    unsigned int len = strlen(s);
    if (len < 1) return FOURCC(0); // for BI_RGB = 0

    for (unsigned int i=0; i<len; i++) s4[i] = s[i];
    return FOURCC((s4[0])|(s4[1]<<8)|(s4[2]<<16)|(s4[3]<<24));
}

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(wxArrayVideoCaptureFormat);

static wxArrayVideoCaptureFormat s_wxVideoCaptureFormats;


/*

// array of some predefined "standard" video formats and their descriptions
const wxvidcap_video_format_typedef wxVIDCAP_VIDEO_FORMATS[wxVIDCAP_VIDEO_FORMATS_COUNT]=
{
    { BI_RGB,            8, "8-bit RGB" },
    { BI_RGB,                   16, "16-bit RGB" },
    { BI_RGB,                   24, "24-bit RGB" },
    { BI_RGB,                   32, "32-bit ARGB" },
    { BI_RGB,   16, "555 16 bit RGB" },
    { STRING_TO_FOURCC("R565"), 16, "565 16 bit RGB" },
    { STRING_TO_FOURCC("Y8  "),  8, "Y8     Single Y plane, greyscale" },
    { STRING_TO_FOURCC("CYUV"), 16, "CYUV   Inverted YUV 4:2:2" },
    { STRING_TO_FOURCC("UYVY"), 16, "UYVY   YUV 4:2:2 interleaved" },
    { STRING_TO_FOURCC("YUYV"), 16, "YUYV   YUV 4:2:2 interleaved" },
    { STRING_TO_FOURCC("YUY2"), 16, "YUY2   YUV 4:2:2 interleaved" },
    { STRING_TO_FOURCC("YV12"), 12, "YV12   YUV 4:2:0 planar (MPEG Raw) = YVU12" },
    { STRING_TO_FOURCC("I420"), 12, "I420   YUV 4:2:0 planar = YUV12" },
    { STRING_TO_FOURCC("IYUV"), 12, "IYUV   YUV 4:2:0 planar" },
    { STRING_TO_FOURCC("Y41P"), 12, "Y41P   YUV 4:1:1 planar = BTYUV" },
    { STRING_TO_FOURCC("YVU9"),  9, "YVU9   YUV planar (Indeo Raw) = YUV9" },
    { STRING_TO_FOURCC("YUV9"),  9, "YUV9   YUV planar (Indeo Raw) = YVU9" },
    { STRING_TO_FOURCC("MJPG"), 16, "MJPG   Motion JPEG" },
    { STRING_TO_FOURCC("dmb1"), 16, "dmb1   Matrox MJPEG" },
    { STRING_TO_FOURCC("JBYR"), 20, "JBYR   se401/Kensington" },
    { STRING_TO_FOURCC("ABYR"),  8, "ABYR   se401/Kensington" },
    { STRING_TO_FOURCC("VCR1"), 16, "VCR1   ATI VCR 1.0" },
    { STRING_TO_FOURCC("VCR2"), 12, "VCR2   ATI VCR 2.0" },
    { STRING_TO_FOURCC("VYUY"), 16, "VYUY   ATI Packed YUV Data" },
    { STRING_TO_FOURCC("Y411"), 16, "Y411   YUV 4:1:1 perhaps" }
};
*/
// array of "standard" audio rates
const long int wxVIDCAP_AUDIO_SAMPLESPERSEC[wxVIDCAP_AUDIO_SAMPLESPERSEC_COUNT]=
    { 8000, 11025, 16000, 22050, 24000, 32000, 44100, 48000 };

//------------------------------------------------------------------------------
// wxVideoCaptureEvent, events for the wxVideoCaptureWindow
//------------------------------------------------------------------------------
DEFINE_EVENT_TYPE(wxEVT_VIDEO_STATUS)
DEFINE_EVENT_TYPE(wxEVT_VIDEO_FRAME)
DEFINE_EVENT_TYPE(wxEVT_VIDEO_STREAM)
DEFINE_EVENT_TYPE(wxEVT_VIDEO_ERROR)

IMPLEMENT_DYNAMIC_CLASS(wxVideoCaptureEvent, wxEvent)

wxVideoCaptureEvent::wxVideoCaptureEvent(wxEventType commandType,
                                         wxVideoCaptureWindowBase *vidCapWin,
                                         int Id)
                    :wxEvent(Id, commandType)
{
    SetEventObject( vidCapWin );
}

//----------------------------------------------------------------------------
// wxVideoCaptureWindowBase : window for viewing/recording streaming video or snapshots
//----------------------------------------------------------------------------
IMPLEMENT_ABSTRACT_CLASS(wxVideoCaptureWindowBase, wxScrolledWindow);

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
    return wxScrolledWindow::Create(parent, id, pos, size, style, name);
}

wxVideoCaptureWindowBase::~wxVideoCaptureWindowBase()
{
}

void wxVideoCaptureWindowBase::Reset(bool full)
{
    //m_clientSize = GetClientSize();

    if (full) m_deviceNames.Clear();
    if (full) m_deviceVersions.Clear();

    m_deviceIndex = -1;

    m_previewing = false;
    m_preview_wximage = false;
    if (full) m_previewscaled = false;
    if (full) m_previewmsperframe = 66;             // 15fps
    m_actualpreviewmsperframe = 0;

    m_has_overlay = false;
    m_overlaying = false;

    if (full) m_framenumber = 0;

    m_imageSize = wxSize(0, 0);
    m_minImageSize = wxSize(32, 32);    // just a guess, VFW doesn't support this so...
    m_maxImageSize = wxSize(1024, 768); // just a guess, VFW doesn't support this so...
}

// get the device description for a particular device
wxString wxVideoCaptureWindowBase::GetDeviceName(int index) const
{
    if (((unsigned int)index < m_deviceNames.GetCount()) && (index >= 0))
        return wxString(m_deviceNames.Item(index));

    return wxEmptyString;
}

// get the device version for a particular device
wxString wxVideoCaptureWindowBase::GetDeviceVersion(int index) const
{
    if ((index>=0) && (index < (int)m_deviceVersions.GetCount()))
        return wxString(m_deviceVersions.Item(index));

    return wxEmptyString;
}

// ----------------------------------------------------------------------
// Video characteristics and manipulation
// ----------------------------------------------------------------------

// find the size of a file in KB
long int wxVideoCaptureWindowBase::GetFileSizeInKB( const wxString &filename )
{
    if (!wxFileExists(filename)) return -1;
    wxFile file(filename, wxFile::read);
    if (!file.IsOpened()) return -1; // file doesn't exist ?

    long int filesize = file.Length();
    file.Close();
    return filesize/1024L;
}

// ----------------------------------------------------------------------
// Capture single frames, take snapshots of streaming video
// ----------------------------------------------------------------------

wxImage wxVideoCaptureWindowBase::GetwxImage()
{
    return m_wximage;
/*
    if (m_wximage.Ok() && !m_getting_wximage)
    {
        int width = m_wximage.GetWidth();
        int height = m_wximage.GetHeight();
        wxImage outimage( width, height);

        unsigned char *m_wximgptr = m_wximage.GetData();
        unsigned char *outimgptr = outimage.GetData();

        memcpy(outimgptr, m_wximgptr, width*height*3);
        return outimage;
    }
    return wxImage();
*/
}

//-------------------------------------------------------------------------
// wxVideoCaptureFormat manipulation
int wxVideoCaptureWindowBase::GetVideoCaptureFormatCount()
{
    CreateVideoCaptureFormatArray();
    return s_wxVideoCaptureFormats.GetCount();
}
wxArrayVideoCaptureFormat &wxVideoCaptureWindowBase::GetVideoCaptureFormatArray()
{
    CreateVideoCaptureFormatArray();
    return s_wxVideoCaptureFormats;
}
wxVideoCaptureFormat wxVideoCaptureWindowBase::GetVideoCaptureFormat(int index)
{
    CreateVideoCaptureFormatArray();
    wxCHECK_MSG(index >= 0 && index < int(s_wxVideoCaptureFormats.GetCount()), wxVideoCaptureFormat(),
                 wxT("invalid index in GetVideoCaptureFormat"));
    return s_wxVideoCaptureFormats[index];
}
void wxVideoCaptureWindowBase::RegisterVideoCaptureFormat(wxVideoCaptureFormat *new_VideoFormat)
{
    CreateVideoCaptureFormatArray();
    wxCHECK_RET(new_VideoFormat, wxT("invalid format"));
    s_wxVideoCaptureFormats.Add(new_VideoFormat);
}

int wxVideoCaptureWindowBase::FindVideoCaptureFormatFOURCC(FOURCC fourcc)
{
    CreateVideoCaptureFormatArray();
    int i, count = s_wxVideoCaptureFormats.GetCount();
    for (i=0; i<count; i++)
    {
        if (s_wxVideoCaptureFormats[i].m_fourcc == fourcc)
            return i;
    }
    return -1;
}

int wxVideoCaptureWindowBase::FindVideoCaptureFormatV4Lpalette(int v4lpalette)
{
    CreateVideoCaptureFormatArray();
    int i, count = s_wxVideoCaptureFormats.GetCount();
    for (i=0; i<count; i++)
    {
        if (s_wxVideoCaptureFormats[i].m_v4l1_palette == v4lpalette)
            return i;
    }
    return -1;
}

// define the video 4 linux 1 palette mapping onto the fourcc
// see /usr/src/linux-2.4.19-16mdk/drivers/media/video/v4l1-compat.c
// for palette_to_pixelformat(int palette) WHY CAN'T THESE BE PUBLIC! &^#$&*
// these are set to -1 on other platforms since they don't matter
#ifndef WXVIDCAP_LINUX_V4L
#define VIDEO_PALETTE_GREY     -1 // 1       /* Linear greyscale */
#define VIDEO_PALETTE_HI240    -1 // 2       /* High 240 cube (BT848) */
#define VIDEO_PALETTE_RGB565   -1 // 3       /* 565 16 bit RGB */
#define VIDEO_PALETTE_RGB24    -1 // 4       /* 24bit RGB */
#define VIDEO_PALETTE_RGB32    -1 // 5       /* 32bit RGB */
#define VIDEO_PALETTE_RGB555   -1 // 6       /* 555 15bit RGB */
#define VIDEO_PALETTE_YUV422   -1 // 7       /* YUV422 capture */
#define VIDEO_PALETTE_YUYV     -1 // 8
#define VIDEO_PALETTE_UYVY     -1 // 9       /* The great thing about standards is ... */
#define VIDEO_PALETTE_YUV420   -1 // 10
#define VIDEO_PALETTE_YUV411   -1 // 11      /* YUV411 capture */
#define VIDEO_PALETTE_RAW      -1 // 12      /* RAW capture (BT848) */
#define VIDEO_PALETTE_YUV422P  -1 // 13      /* YUV 4:2:2 Planar */
#define VIDEO_PALETTE_YUV411P  -1 // 14      /* YUV 4:1:1 Planar */
#define VIDEO_PALETTE_YUV420P  -1 // 15      /* YUV 4:2:0 Planar */
#define VIDEO_PALETTE_YUV410P  -1 // 16      /* YUV 4:1:0 Planar */
//#define VIDEO_PALETTE_PLANAR    13      /* start of planar entries */
//#define VIDEO_PALETTE_COMPONENT 7       /* start of component entries */
#endif // WXVIDCAP_LINUX_V4L

#define _vcf_(a,b,c,d)  s_wxVideoCaptureFormats.Add(new wxVideoCaptureFormat(a,b,c,d));

void wxVideoCaptureWindowBase::CreateVideoCaptureFormatArray()
{
    if (int(s_wxVideoCaptureFormats.GetCount()) > 0) return;

#ifdef WXVIDCAP_LINUX_V4L_thisdoesntexistsoitsnotrun
    // These are for v4l, sooo much duplicated work... sigh...
    // see videodev.h videodev2.h bttv-driver.c v4l1-compat.c

    _vcf_(wxT("GREY = Y8 Single Y plane, greyscale"), STRING_TO_FOURCC("GREY"),  8, VIDEO_PALETTE_GREY)
    _vcf_(wxT("HI24   8 bpp, dithered color"), STRING_TO_FOURCC("HI24"),     8, VIDEO_PALETTE_HI240)

    _vcf_(wxT("RGBP 16  RGB-5-6-5"), STRING_TO_FOURCC("RGBP"), , VIDEO_PALETTE_RGB565)
    _vcf_(wxT("BGR3 24  BGR-8-8-8"), STRING_TO_FOURCC("BGR3"), , VIDEO_PALETTE_RGB24)
    _vcf_(wxT("BGR4 32  BGR-8-8-8-8"), STRING_TO_FOURCC("BGR4"), , VIDEO_PALETTE_RGB32)
    _vcf_(wxT("RGB0 16  RGB-5-5-5"), STRING_TO_FOURCC("RGB0"), , VIDEO_PALETTE_RGB555)
    _vcf_(wxT("YUYV 16  YUV 4:2:2"), STRING_TO_FOURCC("YUYV"), , VIDEO_PALETTE_YUV422) // these 2 are same
    _vcf_(wxT("YUYV 16  YUV 4:2:2"), STRING_TO_FOURCC("YUYV"), , VIDEO_PALETTE_YUYV)
    _vcf_(wxT("UYVY 16  YUV 4:2:2"), STRING_TO_FOURCC("UYVY"), , VIDEO_PALETTE_UYVY)
    _vcf_(wxT("YU12 12  YUV 4:2:0"), STRING_TO_FOURCC("YU12"), , VIDEO_PALETTE_YUV420) // not in v4l2
    _vcf_(wxT(""), STRING_TO_FOURCC(""), , VIDEO_PALETTE_YUV411) ???
    _vcf_(wxT(""), STRING_TO_FOURCC(""), , VIDEO_PALETTE_RAW)
    _vcf_(wxT(""), STRING_TO_FOURCC(""), , VIDEO_PALETTE_YUV422P)
    _vcf_(wxT(""), STRING_TO_FOURCC(""), , VIDEO_PALETTE_YUV411P)
    _vcf_(wxT(""), STRING_TO_FOURCC(""), , VIDEO_PALETTE_YUV420P)
    _vcf_(wxT(""), STRING_TO_FOURCC(""), , VIDEO_PALETTE_YUV410P)

    _vcf_(wxT(""), STRING_TO_FOURCC(""), , )

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

#endif // WXVIDCAP_LINUX_V4L

    _vcf_(wxT("8-bit RGB"),                             BI_RGB,  8, -1)
    _vcf_(wxT("16-bit RGB"),                            BI_RGB, 16, -1)
    _vcf_(wxT("24-bit RGB"),                            BI_RGB, 24, -1)
    _vcf_(wxT("32-bit ARGB"),                           BI_RGB, 32, -1)
    _vcf_(wxT("Y8     Single Y plane, greyscale"),      STRING_TO_FOURCC("Y8  "),    8, -1)
    _vcf_(wxT("CYUV   Inverted YUV 4:2:2"),             STRING_TO_FOURCC("CYUV"),   16, -1)
    _vcf_(wxT("UYVY   YUV 4:2:2 interleaved"),          STRING_TO_FOURCC("UYVY"),   16, -1)
    _vcf_(wxT("YUYV   YUV 4:2:2 interleaved"),          STRING_TO_FOURCC("YUYV"),   16, -1)
    _vcf_(wxT("YUY2   YUV 4:2:2 interleaved"),          STRING_TO_FOURCC("YUY2"),   16, -1)
    _vcf_(wxT("YV12   YUV 4:2:0 planar (MPEG Raw) = YVU12"), STRING_TO_FOURCC("YV12"),  12, -1)
    _vcf_(wxT("I420   YUV 4:2:0 planar = YUV12"),       STRING_TO_FOURCC("I420"),   12, -1)
    _vcf_(wxT("IYUV   YUV 4:2:0 planar"),               STRING_TO_FOURCC("IYUV"),   12, -1)
    _vcf_(wxT("Y41P   YUV 4:1:1 planar = BTYUV"),       STRING_TO_FOURCC("Y41P"),   12, -1)
    _vcf_(wxT("YVU9   YUV planar (Indeo Raw) = YUV9"),  STRING_TO_FOURCC("YVU9"),    9, -1)
    _vcf_(wxT("YUV9   YUV planar (Indeo Raw) = YVU9"),  STRING_TO_FOURCC("YUV9"),    9, -1)
    _vcf_(wxT("MJPG   Motion JPEG"),                    STRING_TO_FOURCC("MJPG"),   16, -1)
    _vcf_(wxT("dmb1   Matrox MJPEG"),                   STRING_TO_FOURCC("dmb1"),   16, -1)
    _vcf_(wxT("JBYR   se401/Kensington"),               STRING_TO_FOURCC("JBYR"),   20, -1)
    _vcf_(wxT("ABYR   se401/Kensington"),               STRING_TO_FOURCC("ABYR"),    8, -1)
    _vcf_(wxT("VCR1   ATI VCR 1.0"),                    STRING_TO_FOURCC("VCR1"),   16, -1)
    _vcf_(wxT("VCR2   ATI VCR 2.0"),                    STRING_TO_FOURCC("VCR2"),   12, -1)
    _vcf_(wxT("VYUY   ATI Packed YUV Data"),            STRING_TO_FOURCC("VYUY"),   16, -1)
}
