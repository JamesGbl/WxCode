/////////////////////////////////////////////////////////////////////////////
// Name:        vcapwin.h - wxVideoCaptureWindowBase
// Author:      John Labenski
// Created:     7/06/2001
// Modified:    01/14/03
// Copyright:   John Labenski
// License:     wxWidgets V2.0
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_VCAPWIN_H__
#define __WX_VCAPWIN_H__

#include <wx/defs.h>
#include <wx/event.h>
#include <wx/scrolwin.h>
#include <wx/image.h>
#include <wx/timer.h>

//-----------------------------------------------------------------------------
/// @name wxVidCap version
//-----------------------------------------------------------------------------
/// @{

#define wxVIDCAP_MAJOR_VERSION      1
#define wxVIDCAP_MINOR_VERSION      0
#define wxVIDCAP_RELEASE_VERSION    0
#define wxVIDCAP_SUBRELEASE_VERSION 0
#define wxVIDCAP_VERSION_STRING     wxT("wxVidCap 1.0.0")

/// For non-Unix systems (i.e. when building without a configure script),
/// users of this component can use the following macro to check if the
/// current version is at least major.minor.release
#define wxCHECK_VIDCAP_VERSION(major,minor,release) \
    (wxVIDCAP_MAJOR_VERSION > (major) || \
    (wxVIDCAP_MAJOR_VERSION == (major) && wxVIDCAP_MINOR_VERSION > (minor)) || \
    (wxVIDCAP_MAJOR_VERSION == (major) && wxVIDCAP_MINOR_VERSION == (minor) && wxVIDCAP_RELEASE_VERSION >= (release)))

/// @}
//-----------------------------------------------------------------------------
/// @name DLL Import and Export macros (see the contrib libs like wxPlot)
//-----------------------------------------------------------------------------
/// @{

#ifdef WXMAKINGDLL_VIDCAP
    #define WXDLLIMPEXP_VIDCAP WXEXPORT
    #define WXDLLIMPEXP_DATA_VIDCAP(type) WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_VIDCAP WXIMPORT
    #define WXDLLIMPEXP_DATA_VIDCAP(type) WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_VIDCAP
    #define WXDLLIMPEXP_DATA_VIDCAP(type) type
#endif

/// Forward declare all wxVidcap classes with this macro
#if defined(HAVE_VISIBILITY) || (defined(__WINDOWS__) && defined(__GNUC__))
    #define WXDLLIMPEXP_FWD_VIDCAP
#else
    #define WXDLLIMPEXP_FWD_VIDCAP WXDLLIMPEXP_VIDCAP
#endif

/// @}
//----------------------------------------------------------------------------
// Forward declaration of classes used by the wxVideoCaptureWindow system
//----------------------------------------------------------------------------

class WXDLLIMPEXP_FWD_VIDCAP wxVideoCaptureWindowBase;
class WXDLLIMPEXP_FWD_VIDCAP wxVideoCaptureEvent;
class WXDLLIMPEXP_FWD_VIDCAP wxVideoCaptureWindowCaptureSingleFramesDialog;
class WXDLLIMPEXP_FWD_VIDCAP wxVideoCaptureWindowAudioFormatDialog;
class WXDLLIMPEXP_FWD_VIDCAP wxVideoCaptureWindowCapturePreferencesDialog;
class WXDLLIMPEXP_FWD_VIDCAP wxVideoCaptureWindowCustomVideoFormatDialog;

//----------------------------------------------------------------------------
/// @name wxVideoCaptureWindow #defines and globals
//----------------------------------------------------------------------------
/// @{

/// wxWindowId's for the wxVidCap dialogs, show only one at a time.
enum wxVidCap_WindowIds
{
    ID_wxVIDCAP_CAPSNGFRAMESDLG = wxID_HIGHEST + 1000,
    ID_wxVIDCAP_CAPPREFDLG,
    ID_wxVIDCAP_VIDEOFORMATDLG,
    ID_wxVIDCAP_AUDIOFORMATDLG,

    ID_wxVIDCAP_PREVIEW_WXIMAGE_TIMER
};


#define wxVC_HASBIT(value, bit)      (((value) & (bit)) != 0)
#define wxVC_SETBIT(value, bit, set) ((set) ? (value)|(bit) : (value)&(~(bit)))

#if defined(__WXMSW__)
    #define WXVIDCAP_MSW_VFW        ///< Using Video for Windows (VFW) on MSW.
    #define WXVIDCAP_AVI_SUPPORT    ///< AVI writing is supported.
    #define WXVIDCAP_AUDIO_SUPPORT  ///< Audio recording is supported.
#elif defined(__LINUX__)
    #define WXVIDCAP_LINUX_V4L      ///< Using Video 4 Linux (V4L).
#endif

#if defined(WXVIDCAP_LINUX_V4L)
    typedef wxUint32 FOURCC;        ///< Video fourcc types are 4 bytes.
    #define BI_RGB 0                ///< Windows Bitmap (bmp) simple RGB format.
#endif


/// Array of some predefined "standard" video widths, max 1024.
/// wxVideoCaptureWindow::SetVideoFormat( width...
/// ={80,128,160,176,180,192,240,320,352,360,384,400,480,640,704,720,768,800,1024};
#define wxVIDCAP_VIDEO_WIDTHS_COUNT 19
WXDLLIMPEXP_DATA_VIDCAP(extern const unsigned int) wxVIDCAP_VIDEO_WIDTHS[wxVIDCAP_VIDEO_WIDTHS_COUNT];

/// Array of some predefined "standard" video heights, max 768.
/// wxVideoCaptureWindow::SetVideoFormat( width, height...
/// ={60, 96, 120, 144, 180, 240, 288, 300, 360, 480, 576, 600, 768 };
#define wxVIDCAP_VIDEO_HEIGHTS_COUNT 13
WXDLLIMPEXP_DATA_VIDCAP(extern const unsigned int) wxVIDCAP_VIDEO_HEIGHTS[wxVIDCAP_VIDEO_HEIGHTS_COUNT];

/// Make a FOURCC (DWORD-32bit uint) from a string, STRING_TO_FOURCC("UYVV").
/// MUST have four characters, even for "  Y8" for example.
/// This is NOT the preferred way to do it, see below for better methods.
#define STRING_TO_FOURCC(f) (((f)[0])|((f)[1]<<8)|((f)[2]<<16)|((f)[3]<<24))
/// Make a FOURCC from 4 chars, Chars_To_FOURCC('U','Y','V','V')
#define CHARS_TO_FOURCC(a,b,c,d) (((wxUint32)(a))|((wxUint32)(b)<<8)|((wxUint32)(c)<<16)|((wxUint32)(d)<<24))

/// Return a FOURCC given a wxString.
WXDLLIMPEXP_VIDCAP FOURCC wxStringToFOURCC(const wxString &string);
/// Return a FOURCC given a regular string of chars.
WXDLLIMPEXP_VIDCAP FOURCC StringToFOURCC( const char *s );

/// Return a wxString given a FOURCC.
WXDLLIMPEXP_VIDCAP wxString FOURCCTowxString(FOURCC fourcc);
/// Return a string of chars given a FOURCC, uses a static buffer do this is not threadsafe.
WXDLLIMPEXP_VIDCAP const char* FOURCCToString(FOURCC fourcc);

/// Define an invalid FOURCC.
#define wxNullFOURCC FOURCC(-1)

/// @}
//---------------------------------------------------------------------------
/// @name Audio defines and constants
//---------------------------------------------------------------------------
/// @{

/// wxVideoCaptureWindow::[Get/Set]AudioFormat( channels, ...
enum wxVIDCAP_AUDIO_CHANNELS_Type
{
    wxVIDCAP_AUDIO_MONO   = 1,
    wxVIDCAP_AUDIO_STEREO = 2
};

/// wxVideoCaptureWindow::[Get/Set]AudioFormat( channels, bitspersample, ...
enum wxVIDCAP_AUDIO_BITS_Type
{
    wxVIDCAP_AUDIO_8BITS  = 8,
    wxVIDCAP_AUDIO_16BITS = 16
};

/// wxVideoCaptureWindow::[Get/Set]AudioFormat( channels, bitspersample, samplespersecond.
enum wxVIDCAP_AUDIO_SAMPLES_Type
{
    wxVIDCAP_AUDIO_8000HZ  = 8000,
    wxVIDCAP_AUDIO_11025HZ = 11025,
    wxVIDCAP_AUDIO_16000HZ = 16000,
    wxVIDCAP_AUDIO_22050HZ = 22050,
    wxVIDCAP_AUDIO_24000HZ = 24000,
    wxVIDCAP_AUDIO_32000HZ = 32000,
    wxVIDCAP_AUDIO_44100HZ = 44100,
    wxVIDCAP_AUDIO_48000HZ = 48000
};

/// Array of some "standard" audio rates, in same order as enum wxVIDCAP_AUDIO_SAMPLES_Type.
#define wxVIDCAP_AUDIO_SAMPLESPERSEC_COUNT 8
WXDLLIMPEXP_DATA_VIDCAP(extern const unsigned int) wxVIDCAP_AUDIO_SAMPLESPERSEC[wxVIDCAP_AUDIO_SAMPLESPERSEC_COUNT];
    // = { 8000, 11025, 16000, 22050, 24000, 32000, 44100, 48000 };

/// @}
//---------------------------------------------------------------------------
/** @class wxVideoCaptureFormat
    @brief Storage for a video format.
*/ // -----------------------------------------------------------------------
class WXDLLIMPEXP_VIDCAP wxVideoCaptureFormat
{
public :
    wxVideoCaptureFormat() : m_fourcc(wxNullFOURCC), m_v4l2_pixelformat(-1), m_bpp(0) {}

    wxVideoCaptureFormat(const wxString &description, FOURCC fourcc,
                         int v4l2_pixelformat, int bpp);

    wxString m_description;      ///< Common name of the format.
    FOURCC   m_fourcc;           ///< MSW VFW MMIO data type for BITMAPINFOHEADER->biCompression.
    FOURCC   m_v4l2_pixelformat; ///< The type v4l2 defines for it.
    int      m_bpp;              ///< Bits per pixel for BITMAPINFOHEADER->biBitCount.
};

/// An invalid wxVideoCaptureFormat using wxNullFOURCC.
WXDLLIMPEXP_DATA_VIDCAP(extern const wxVideoCaptureFormat) wxNullVideoCaptureFormat;

#include <wx/dynarray.h>
WX_DECLARE_OBJARRAY_WITH_DECL(wxVideoCaptureFormat, wxArrayVideoCaptureFormat, class WXDLLIMPEXP_VIDCAP);

/*

TODO : Maybe reimplement this... or maybe not...

// constants to specify video formats from wxVIDCAP_VIDEO_FORMATS[one below]
// used in wxVideoCaptureWindow::[Get/Set]VideoFormat( width, height, format..
enum wxVIDCAP_VIDEO_FORMAT_Type
{
    wxVIDCAP_VIDEO_FORMAT_UNKNOWN = -1, // some weird format, there are MANY
    wxVIDCAP_VIDEO_FORMAT_BI_RGB_8 = 0, // 8-bit RGB
    wxVIDCAP_VIDEO_FORMAT_BI_RGB_16,    // 16-bit RBG
    wxVIDCAP_VIDEO_FORMAT_BI_RGB_24,    // 24-bit RGB
    wxVIDCAP_VIDEO_FORMAT_BI_RGB_32,    // 32-bit RGB
    wxVIDCAP_VIDEO_FORMAT_RGB555,       // 555 16 bit RGB
    wxVIDCAP_VIDEO_FORMAT_RGB565,       // 565 16 bit RGB
    wxVIDCAP_VIDEO_FORMAT_Y8,           // Y8 Single Y plane, greyscale
    wxVIDCAP_VIDEO_FORMAT_GREY,         // v4l GREY fake fourcc I think
    wxVIDCAP_VIDEO_FORMAT_CYUV,         // CYUV Inverted YUV 4:2:2
    wxVIDCAP_VIDEO_FORMAT_UYVY,         // UYVY YUV 4:2:2 interleaved
    wxVIDCAP_VIDEO_FORMAT_YUYV,         // YUYV YUV 4:2:2 interleaved
    wxVIDCAP_VIDEO_FORMAT_YUY2,         // YUY2 YUV 4:2:2 interleaved
    wxVIDCAP_VIDEO_FORMAT_YV12,         // YV12 YUV 4:2:0 planar
    wxVIDCAP_VIDEO_FORMAT_I420,         // I420 YUV 4:2:0 planar
    wxVIDCAP_VIDEO_FORMAT_IYUV,         // IYUV YUV 4:2:0 planar
    wxVIDCAP_VIDEO_FORMAT_Y41P,         // Y41P YUV 4:1:1 planar
    wxVIDCAP_VIDEO_FORMAT_YVU9,         // YVU9 YUV planar (Indeo Raw) = YUV9
    wxVIDCAP_VIDEO_FORMAT_YUV9,         // YUV9 YUV planar (Indeo Raw) = YVU9
    wxVIDCAP_VIDEO_FORMAT_MJPG,         // MJPG Motion JPEG
    wxVIDCAP_VIDEO_FORMAT_dmb1,         // dmb1 Matrox MJPEG
    wxVIDCAP_VIDEO_FORMAT_JBYR,         // JBYR 20 bit se401 Kensington/Phillips
    wxVIDCAP_VIDEO_FORMAT_ABYR,         // ABYR 8 bit se401 Kensington/Phillips
    wxVIDCAP_VIDEO_FORMAT_VCR1,         // VCR1 ATI VCR 1.0
    wxVIDCAP_VIDEO_FORMAT_VCR2,         // VCR2 ATI VCR 2.0
    wxVIDCAP_VIDEO_FORMAT_VYUY,         // VYUY ATI Packed YUV Data
    wxVIDCAP_VIDEO_FORMAT_Y411          // Supposedly packed YUV 4:1:1 ???
};
*/

//---------------------------------------------------------------------------
/** @class wxVideoCaptureEvent
    @brief Events for the wxVideoCaptureWindow.
    This is derived from a wxEvent, so you must subclass your wxVideoCaptureWindow
    or use wxWindow::Bind/Connect() to get any of these events, this should be faster.
    If you want the parent to get events change to wxCommandEvent.
*/ // -----------------------------------------------------------------------

class WXDLLIMPEXP_VIDCAP wxVideoCaptureEvent : public wxEvent
{
public:
    wxVideoCaptureEvent(const wxVideoCaptureEvent& event);
    wxVideoCaptureEvent( wxEventType commandType = wxEVT_NULL,
                         wxVideoCaptureWindowBase *vidCapWin = NULL,
                         int win_id = wxID_ANY );

    virtual ~wxVideoCaptureEvent() {}

    virtual wxEvent *Clone() const { return new wxVideoCaptureEvent(*this); }

    /// wxEVT_VIDEO_STATUS uses this.
    /// VFW capSetCallbackOnStatus() callback sends this event anytime the status changes.
    wxString GetStatusMessage() const { return m_statusMessage; }
    void SetStatusMessage( const wxString &statusMessage ) { m_statusMessage = statusMessage; }

    /// wxEVT_VIDEO_ERROR uses this.
    /// VFW capSetCallbackOnError() callback sends this event anytime nonfatal errors are generated.
    wxString GetErrorMessage() const { return m_errorMessage; }
    void SetErrorMessage( const wxString &errorMessage ) { m_errorMessage = errorMessage; }

    /// wxEVT_VIDEO_FRAME uses this.
    /// VFW capSetCallbackOnFrame() callback sends this event when previewing.
    unsigned int GetFrameNumber() const { return m_framenumber; }
    void SetFrameNumber( unsigned int num ) { m_framenumber = num; }

    /// wxEVT_VIDEO_FRAME uses this, get the current frame rate.
    unsigned int GetFramerateMS() const { return m_frameratems; }
    void SetFrameRateMS( unsigned int num ) { m_frameratems = num; }

    wxString     m_statusMessage;
    wxString     m_errorMessage;
    unsigned int m_framenumber;
    unsigned int m_frameratems;

private:
    DECLARE_DYNAMIC_CLASS(wxVideoCaptureEvent)
};

typedef void (wxEvtHandler::*wxVideoCaptureEventFunction)(wxVideoCaptureEvent&);

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_VIDCAP, wxEVT_VIDEO,        wxEVT_USER_FIRST + 3001)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_VIDCAP, wxEVT_VIDEO_STATUS, wxEVT_USER_FIRST + 3002)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_VIDCAP, wxEVT_VIDEO_FRAME,  wxEVT_USER_FIRST + 3003)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_VIDCAP, wxEVT_VIDEO_STREAM, wxEVT_USER_FIRST + 3004)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_VIDCAP, wxEVT_VIDEO_ERROR,  wxEVT_USER_FIRST + 3005)

    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_VIDCAP, wxEVT_VIDEO_FRAME_REQ, wxEVT_USER_FIRST + 3006)
END_DECLARE_EVENT_TYPES()

/// Receive a wxVideoCaptureEvent of type wxEVT_VIDEO_STATUS whenever the status of the capture device changes.
/// In MSW for capSetCallbackOnStatus(m_hWndC, VFW_VideoStatusCallbackProc);
#define EVT_VIDEO_STATUS(id, func) DECLARE_EVENT_TABLE_ENTRY( wxEVT_VIDEO_STATUS, id, wxID_ANY, (wxObjectEventFunction)(wxEventFunction)(wxVideoCaptureEventFunction)&func, (wxObject *)NULL),
/// Receive a wxVideoCaptureEvent of type wxEVT_VIDEO_FRAME whenever a preview frame is available from the device.
/// In MSW for capSetCallbackOnFrame(m_hWndC, VFW_VideoFrameCallbackProc);
#define EVT_VIDEO_FRAME(id, func)  DECLARE_EVENT_TABLE_ENTRY( wxEVT_VIDEO_FRAME, id, wxID_ANY, (wxObjectEventFunction)(wxEventFunction)(wxVideoCaptureEventFunction)&func, (wxObject *)NULL),
/// Receive a wxVideoCaptureEvent of type wxEVT_VIDEO_STREAM whenver a frame is about to be written to an AVI file.
/// In MSW for capSetCallbackOnStream(m_hWndC, VFW_VideoStreamCallbackProc);
#define EVT_VIDEO_STREAM(id, func) DECLARE_EVENT_TABLE_ENTRY( wxEVT_VIDEO_STREAM, id, wxID_ANY, (wxObjectEventFunction)(wxEventFunction)(wxVideoCaptureEventFunction)&func, (wxObject *)NULL),
/// Receive a wxVideoCaptureEvent of type wxEVT_VIDEO_ERROR whenver a nonfatal error occurs.
/// In MSW for capSetCallbackOnError(m_hWndC, VFW_VideoErrorCallbackProc);
#define EVT_VIDEO_ERROR(id, func)  DECLARE_EVENT_TABLE_ENTRY( wxEVT_VIDEO_ERROR, id, wxID_ANY, (wxObjectEventFunction)(wxEventFunction)(wxVideoCaptureEventFunction)&func, (wxObject *)NULL),
/// Receive a wxVideoCaptureEvent of type wxEVT_VIDEO_FRAME_REQ, this is used internally to avoid blocking the wxevent loop.
#define EVT_VIDEO_FRAME_REQ(id, func)  DECLARE_EVENT_TABLE_ENTRY( wxEVT_VIDEO_FRAME_REQ, id, wxID_ANY, (wxObjectEventFunction)(wxEventFunction)(wxVideoCaptureEventFunction)&func, (wxObject *)NULL),
/// Receive a wxVideoCaptureEvent for all wxEVT_VIDEO_XXX types whenever anything happens.
#define EVT_VIDEO(id, func) \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_VIDEO_STATUS, id, wxID_ANY, (wxObjectEventFunction)(wxEventFunction)(wxVideoCaptureEventFunction)&func, (wxObject*)NULL), \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_VIDEO_FRAME,  id, wxID_ANY, (wxObjectEventFunction)(wxEventFunction)(wxVideoCaptureEventFunction)&func, (wxObject*)NULL), \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_VIDEO_STREAM, id, wxID_ANY, (wxObjectEventFunction)(wxEventFunction)(wxVideoCaptureEventFunction)&func, (wxObject*)NULL), \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_VIDEO_ERROR,  id, wxID_ANY, (wxObjectEventFunction)(wxEventFunction)(wxVideoCaptureEventFunction)&func, (wxObject*)NULL),

//---------------------------------------------------------------------------
/** @class wxVideoCaptureWindowBase
    @brief A wxScrolledWindow base class for viewing/recording streaming video or snapshots.
    The derived classes wxVideoCaptureWindowVFW and wxVideoCaptureWindowV4L
    are implementations for MSW's Video for Windows and Linux's Video 4 Linux 2.
*/ // -----------------------------------------------------------------------

class WXDLLIMPEXP_VIDCAP wxVideoCaptureWindowBase : public wxScrolledWindow
{
public:
    wxVideoCaptureWindowBase() : wxScrolledWindow() { Reset(true); }
    wxVideoCaptureWindowBase( wxWindow *parent, wxWindowID id = wxID_ANY,
                              const wxPoint &pos = wxDefaultPosition,
                              const wxSize &size = wxDefaultSize,
                              long style = wxSIMPLE_BORDER,
                              const wxString &name = wxT("wxVideoCaptureWindow"));

    virtual ~wxVideoCaptureWindowBase();

    bool Create( wxWindow *parent, wxWindowID id = wxID_ANY,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 long style = wxSIMPLE_BORDER,
                 const wxString &name = wxT("wxVideoCaptureWindow"));

    // ----------------------------------------------------------------------
    /// @name Device descriptions & versions, get and enumerate
    // ----------------------------------------------------------------------
    /// @{

    /// Enumerate all available device names & versions.
    /// This is called at creation and would only be needed to be called again
    /// to update the list if a device was added or removed.
    virtual bool EnumerateDevices() = 0;

    /// Returns how many devices were detected from EnumerateDevices().
    /// VFW supports only [0..9].
    virtual int GetDeviceCount() const { return m_deviceNames.GetCount(); }
    /// Get a string description of a device from 0 to GetDeviceCount()-1.
    /// Returns an empty string for invalid device numbers, @see GetDeviceIndex().
    virtual wxString GetDeviceName(int index) const;
    /// Get a string version of a device from 0 to GetDeviceCount()-1.
    /// Returns an empty string for invalid device numbers, @see GetDeviceIndex().
    virtual wxString GetDeviceVersion(int index) const;

    /// @}
    // ----------------------------------------------------------------------
    /// @name Connect or Disconnect to device
    // ----------------------------------------------------------------------
    /// @{

    /// Returns true if we are we connected to a device.
    virtual bool IsDeviceConnected() const { return m_deviceIndex > -1; }

    /// Returns true if the device is successfully initialized/connected/ready to capture.
    /// DON'T TRUST w/ VFW as it always seems to return true, so that's why we have IsDeviceConnected().
    /// FIXME : maybe there's some use to this? probably should just delete it.
    virtual bool IsDeviceInitialized() const { return IsDeviceConnected(); }

    /// Index of the currently connected capture device.
    /// Returns -1 if not connected, so be sure to check IsDeviceConnected().
    /// Note: VFW - the devices [0..9] are listed in system.ini
    virtual int GetDeviceIndex() const { return m_deviceIndex; }

    /// Connect to one of the available devices from 0 to GetDeviceCount()-1, returns success.
    virtual bool DeviceConnect(int index) = 0;
    /// Disconnect from the device, returns success.
    virtual bool DeviceDisconnect() = 0;

    /// @}
    // ----------------------------------------------------------------------
    /// @name Display dialogs to set/get video characteristics
    // ----------------------------------------------------------------------
    /// @{

    /// Dialog to adjust video contrast/intensity... and change source.
    ///   VFW - driver supplies the dialog, also select source.
    ///   V4L - custom dialog to change the source (not implemented yet).
    virtual bool HasVideoSourceDialog() = 0;
    /// Show a dialog to choose the video source.
    virtual void ShowVideoSourceDialog() = 0;

    /// Dialog to display the format (size).
    ///  VFW - driver supplies the dialog.
    ///  V4L - use the VideoCustomFormatDialog.
    virtual bool HasVideoFormatDialog() = 0;
    /// Show a dialog to choose the video format.
    virtual void ShowVideoFormatDialog() = 0;
    /// Dialog for setting the frame size, bpp, and compression.
    ///   VFW - WARNING: this can crash your system if you choose an unsupported format
    ///                  I'm pretty sure this is correct, but the driver itself doesn't
    ///                  check if it actually supports it and takes the system down.
    ///   V4L - no problems.
    virtual void ShowVideoCustomFormatDialog() = 0;

    /// Dialog for "redisplay of video from cap frame buffer"? Huh? saturation, brightness...
    ///   VFW - driver supplies dialog, for overlay only?
    ///   V4L - not sure what to make of this (not implemented).
    virtual bool HasVideoDisplayDialog() = 0;
    /// Show a dialog for setting the video display
    virtual void ShowVideoDisplayDialog() = 0;

    /// Dialog to display all know device characteristics.
    /// Each system displays different things, basicly a wxFrame with a wxTextCtrl
    ///  to show the GetPropertiesString().
    virtual void ShowPropertiesDialog();

    /// A formatted string of all know properties of the video system.
    /// This is the string that's printed in the ShowPropertiesDialog().
    virtual wxString GetPropertiesString();

    /// @}
    // ----------------------------------------------------------------------
    /// @name Video format and characteristics
    // ----------------------------------------------------------------------
    /// @{

    /// Get the width of the capture image or 0 if not connected.
    int GetImageWidth() const  { return m_imageSize.x; }
    /// Get the height of the capture image or 0 if not connected.
    int GetImageHeight() const { return m_imageSize.y; }
    /// Get the size of the capture image of (0,0) if not connected.
    wxSize GetImageSize() const { return m_imageSize; }

    /// Get the Max/Min allowed video sizes.
    ///   VFW max size is 1024X768, min ???, no way to find these parameters.
    ///   V4L supports a method to get the Max/Min size.
    wxSize GetMinImageSize() const { return m_minImageSize; }
    wxSize GetMaxImageSize() const { return m_maxImageSize; }

    /// Get the video format characteristics, returns success.
    /// Format is a 4 character string, "UYVY" or whatever comes out.
    virtual bool GetVideoFormat( int *width, int *height,
                                 int *bpp, FOURCC *fourcc ) const = 0;

    /// Attempt to set the video format the device puts out, returns success.
    /// Use -1 for width/height/bpp/format to keep current value.
    /// FOURCC is the 4 chararacter code e.g. "UYUV".
    /// WARNING! - Video For Windows - (Video 4 Linux - works fine)
    ///   SetVideoFormat is not for the faint of heart or people w/o saved data!
    ///   At worst it'll crash your system, some drivers don't gracefully fail.
    ///   Known culprits - Kensington,  no problem - Pinnacle (bt878).
    virtual bool SetVideoFormat( int width, int height,
                                 int bpp, FOURCC fourcc ) = 0;

    /// Returns true if the default driver palette being used.
    ///   VFW - supported, not sure what you would do with this.
    ///   V4L - unsupported.
    virtual bool IsUsingDefaultPalette() { return true; }

    /// Returns true if driver can create palettes.
    ///   VFW - supported, not sure what you'd do with it though.
    ///   V4L - unsupported.
    virtual bool DriverSuppliesPalettes() { return false; }

    /// @}
    // ----------------------------------------------------------------------
    /// @name Capture Preview and Overlay
    // ----------------------------------------------------------------------
    /// @{

    /// Turn software previewing on/off, returns success.
    /// preview_on = false turns all previewing off.
    ///   VFW - wxpreview = false uses the VFW preview capabilities.
    ///         wxpreview = true, uses the callback to decompress frames to a wxImage.
    ///   V4L - wxpreview is ignored as it always uses wxImages.
    virtual bool Preview(bool preview_on, bool wxpreview = false) = 0;

    /// Returns true if displaying using software preview method.
    virtual bool IsPreviewing() const { return m_previewing || m_preview_wximage; }

    /// Returns true if previewing using wxImages & OnPaint().
    virtual bool IsPreviewingwxImage() const { return m_preview_wximage; }

    /// Scale the preview image to window's size (no scrollbars), returns success.
    /// If overlaying then just center the window.
    virtual bool PreviewScaled(bool fit_window) { m_previewscaled = fit_window; return true; }

    /// Returns true if the video is scaled to the capture window size if previewing, or centered if overlaying.
    virtual bool IsPreviewScaled() const { return m_previewscaled; }

    /// Set the number of milliseconds per frames to *try* to capture at, returns success.
    /// You will get frames at this rate or slower, depending on your device/machine speed.
    /// The default is 66ms or 15fps.
    virtual bool SetPreviewRateMS( unsigned int msperframe = 66 ) { m_previewmsperframe = msperframe; return true; }

    /// Get the currently set preview rate, @see GetActualPreviewRateMS().
    virtual unsigned int GetPreviewRateMS() const { return m_previewmsperframe; }
    /// Get the actual measured preview rate when previewing.
    virtual unsigned int GetActualPreviewRateMS() const { return m_actualpreviewmsperframe; }

    /// Get the number of frames viewed since the device was connected.
    virtual unsigned int GetFrameNumber() const { return m_framenumber; }

    /// This function is called for every frame, immediately after it was captured.
    /// You can override it, but be sure to call the base class function to
    /// have frame numer incremented and actual frame rate calculated.
    virtual void OnPreFrame();

    /// Override this function to do image processing on the preview frames
    /// when previewing with wxImages.
    /// The MSW sequence is: The OnPreviewTimer() grabs a frame which calls the
    ///   MSW callback which calls CallbackOnFrame() which fills the m_wximage
    ///   and then calls this function and if it returns true it calls Refresh() to show the frame.
    virtual bool OnProcessFrame(wxImage& WXUNUSED(wximg)) { return true; }

    /// This function is called for every frame after OnProcessFrame().
    /// You can override it, but be sure to call the base class function to
    /// send a wxEVT_VIDEO_FRAME.
    virtual void OnPostFrame();

    /// Draw the wxImage for custom software previewing.
    /// You can override this function if desired.
    virtual void DoPaint(wxPaintDC& dc);

    /// Returns true if the device supports hardware video overlay.
    virtual bool HasOverlay() const { return m_has_overlay; }
    /// Preview the image using video card hardware overlay, ie. pci framegrabbers, returns success.
    /// Automatically turns off software preview if necessary.
    virtual bool Overlay(bool on) = 0;
    /// Returns true if displaying using hardware video overlay method.
    virtual bool IsOverlaying() const { return m_overlaying; }

    /// @}
    // ----------------------------------------------------------------------
    /// @name Capture single frames, take snapshots of streaming video
    // ----------------------------------------------------------------------
    /// @{

    /// Single stop action snapshot to window, stops previewing, returns success.
    virtual bool SnapshotToWindow() = 0;
    /// Capture a single frame to the clipboard, returns success.
    virtual bool SnapshotToClipboard() = 0;
    /// Single snapshot of video, save it as a DIB (.BMP) file, returns success.
    virtual bool SnapshotToBMP( const wxString &filename ) = 0;
    /// Take a single snapshot and fill the input image, returns success.
    virtual bool SnapshotTowxImage( wxImage &image) = 0;
    /// take a single snapshot and fill the member m_wximage @see GetwxImage().
    virtual bool SnapshotTowxImage() = 0;
    /// Get either a ref of the internal wxImage or a full copy.
    /// When getting the ref, make sure that you use it before the next frame is captured.
    /// Can use in conjunction with SnapshotTowxImage() or previewing w/ wxImages methods.
    virtual wxImage GetwxImage(bool full_copy) const;

    /// @}
    // ----------------------------------------------------------------------
    /// @name Capture (append) single video frames to an AVI file (VFW only)
    // ----------------------------------------------------------------------
    /// @{

    // NOTE : None of this is implemeted for V4L

    /// @}
    // ----------------------------------------------------------------------
    /// @name Capture streaming video to an AVI file (VFW only)
    // ----------------------------------------------------------------------
    /// @{

    // NOTE : None of this is implemeted for V4L

    /// @}
    // ----------------------------------------------------------------------
    /// @name Capture file settings, filename to capture video to (VFW only)
    // ----------------------------------------------------------------------
    /// @{

    // NOTE : None of this is implemeted for V4L

    /// @}
    // ----------------------------------------------------------------------
    /// @name Audio Setup (VFW only)
    // ----------------------------------------------------------------------
    /// @{

    // NOTE : None of this is implemeted for V4L

    /// @}
    // ----------------------------------------------------------------------
    /// @name Utility Functions
    // ----------------------------------------------------------------------
    /// @{

    /// Get the error number, either VFW error number or errno.
    int GetLastErrorNumber() const       { return m_last_error_num; }
    /// Get the last error message.
    wxString GetLastErrorMessage() const { return m_last_errorMessage; }

    /// Set the last error members and send a wxEVT_VIDEO_ERROR.
    /// Returns the result of wxEvtHandler::ProcessEvent().
    virtual bool SendErrorEvent(int error_num, const wxString& errString);

    /// find the size of a file in KB.
    long int GetFileSizeInKB( const wxString &filename ) const;

    /// @}
    //-------------------------------------------------------------------------
    /// @name wxVideoCaptureFormat manipulation, a predefined list of FOURCCs and descriptions.
    //-------------------------------------------------------------------------
    /// @{

    /// Get the number of predefined video capture formats.
    int GetVideoCaptureFormatCount() const;
    /// Get an array of the video capture formats.
    const wxArrayVideoCaptureFormat &GetVideoCaptureFormatArray() const;
    /// Get a video capture format at the given index, returns wxNullVideoCaptureFormat on error.
    const wxVideoCaptureFormat& GetVideoCaptureFormat(int index) const;
    /// Get the index of the video capture format by the VFW FOURCC, return wxNOT_FOUND when not found.
    int FindVideoCaptureFormatFOURCC(FOURCC fourcc) const;
    /// Get the index of the video capture format by the V4L2 FOURCC, return wxNOT_FOUND when not found.
    int FindVideoCaptureFormatV4L2pixelformat(FOURCC v4l2_pixelformat) const;
    /// Add a new video capture format to the array.
    void RegisterVideoCaptureFormat(const wxVideoCaptureFormat& videoCaptureFormat);

    /// @}

protected :

    /// Resets the member vars to a disconnected state.
    /// If !full then user settings are left alone call when disconnecting.
    void Reset(bool full = false);

    /// don't need to call, used internally.
    void CreateVideoCaptureFormatArray() const;

    // -----------------------------------------------------------------------
    // member vars

    wxArrayString m_deviceNames;    ///< device names from EnumerateDevices
    wxArrayString m_deviceVersions; ///< device versions from EnumerateDevices

    int           m_deviceIndex;    ///< current index of the device in use or -1

    bool          m_previewing;              ///< Currently previewing.
    bool          m_preview_wximage;         ///< m_previewing is true, but displaying using OnPaint w/ m_wximage.
    bool          m_previewscaled;           ///< Ccale the preview window fullsize.
    unsigned int  m_previewmsperframe;       ///< # milliseconds between preview frames.
    unsigned int  m_actualpreviewmsperframe; ///< Measured ms between preview frames.

    bool          m_has_overlay;             ///< Can use hardware overlay for display.
    bool          m_overlaying;              ///< Currently overlaying.

    bool          m_getting_wximage;         ///< Simple atomic block when filling the m_wximage.
                                             ///< We could use a mutex, but instead of
                                             ///< blocking we usually want to skip the frame
                                             ///< and just get the next one.

    unsigned int  m_framenumber;             ///< # of frames, since preview start.
    wxLongLong    m_lastframetimemillis;     ///< Time of the last frame.
    wxLongLong    m_actualpreviewtimemillis; ///< Milliseconds between frames, 1/(frame rate).

    wxTimer       m_previewTimer;            ///< For preview rate adjustment or wxImage capture.

    wxImage       m_wximage;                 ///< wximage to hold the streaming video.

    wxSize        m_imageSize;               ///< Size of the video.
    wxSize        m_maxImageSize;            ///< Min available capture size.
    wxSize        m_minImageSize;            ///< Max available capture size.

    int           m_last_error_num;          ///< The last error number or 0.
    wxString      m_last_errorMessage;       ///< The last error message.

    static wxArrayVideoCaptureFormat sm_wxVideoCaptureFormats; ///< The list of video formats.

private:
    DECLARE_ABSTRACT_CLASS(wxVideoCaptureWindowBase);
};

// --------------------------------------------------------------------------
// Include the platform specific implementation
// --------------------------------------------------------------------------

#if defined(WXVIDCAP_MSW_VFW)
    #include "wx/vidcap/vcap_vfw.h"
    typedef wxVideoCaptureWindowVFW wxVideoCaptureWindow;
#elif defined(WXVIDCAP_LINUX_V4L)
    #include "wx/vidcap/vcap_v4l.h"
    typedef wxVideoCaptureWindowV4L wxVideoCaptureWindow;
#endif

#endif //__WX_VCAPWIN_H__
