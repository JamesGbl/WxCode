/////////////////////////////////////////////////////////////////////////////
// Name:        vcap_v4l.h - wxVideoCaptureWindow using Linux V4L2 API
// Author:      John Labenski
// Created:     7/06/2001
// Modified:    01/14/03
// Copyright:   John Labenski
// License:     wxWidgets V2.0
/////////////////////////////////////////////////////////////////////////////
//
// Usage notes:

#ifndef __WX_VCAP_V4L_H__
#define __WX_VCAP_V4L_H__

#include <wx/defs.h>
#include <wx/timer.h>

#if !defined(__WXMSW__)

class wxTimerEvent;
class wxScrollWinEvent;
class wxSizeEvent;
class wxMoveEvent;
class wxPaintEvent;

#include <linux/videodev2.h> // V4L2

#include <vector>

//----------------------------------------------------------------------------
// wxVideoCaptureWindow : window for viewing/recording streaming video or snapshots
//----------------------------------------------------------------------------
class WXDLLIMPEXP_VIDCAP wxVideoCaptureWindowV4L: public wxVideoCaptureWindowBase
{
public:
    wxVideoCaptureWindowV4L() : wxVideoCaptureWindowBase() { Init(); }
    wxVideoCaptureWindowV4L( wxWindow *parent, wxWindowID id = -1,
                             const wxPoint &pos = wxDefaultPosition,
                             const wxSize &size = wxDefaultSize,
                             long style = wxSIMPLE_BORDER,
                             const wxString &name = wxT("wxVideoCaptureWindow"));

    bool Create( wxWindow *parent, wxWindowID id = -1,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 long style = wxSIMPLE_BORDER,
                 const wxString &name = wxT("wxVideoCaptureWindow"));

    virtual ~wxVideoCaptureWindowV4L();

    // ----------------------------------------------------------------------
    // Device descriptions & versions, get and enumerate
    // ----------------------------------------------------------------------

    virtual void EnumerateDevices();

    // ----------------------------------------------------------------------
    // Connect or Disconnect to device
    // ----------------------------------------------------------------------

    virtual bool DeviceConnect(int index);
    virtual bool DeviceDisconnect();

    // ----------------------------------------------------------------------
    // Display dialogs to set/get video characteristics
    // ----------------------------------------------------------------------

    virtual bool HasVideoSourceDialog()  { return false; }
    virtual void ShowVideoSourceDialog() {}

    virtual bool HasVideoFormatDialog()  { return false; }
    virtual void ShowVideoFormatDialog() {}
    virtual void ShowVideoCustomFormatDialog();

    virtual bool HasVideoDisplayDialog()  { return false; }
    virtual void ShowVideoDisplayDialog() {}

    virtual wxString GetPropertiesString();

    // ----------------------------------------------------------------------
    // Video format and characteristics
    // ----------------------------------------------------------------------

    virtual bool GetVideoFormat( int *width, int *height,
                                 int *bpp, FOURCC *fourcc ) const;
    virtual bool SetVideoFormat( int width, int height,
                                 int bpp, FOURCC fourcc );

    // ----------------------------------------------------------------------
    // Capture Preview and Overlay
    // ----------------------------------------------------------------------

    virtual bool Preview(bool onoff, bool wxpreview = false);
    virtual bool PreviewScaled(bool fit_window);
    virtual bool SetPreviewRateMS( unsigned int msperframe = 66 );
    virtual bool Overlay(bool WXUNUSED(on)) { return false; }

    void OnPreviewwxImageTimer(wxTimerEvent& event); // get frames

    // ----------------------------------------------------------------------
    // Capture single frames, take snapshots of streaming video
    // ----------------------------------------------------------------------

    virtual bool SnapshotToWindow();
    virtual bool SnapshotToClipboard();
    virtual bool SnapshotToBMP( const wxString &filename );
    virtual bool SnapshotTowxImage( wxImage &image);
    virtual bool SnapshotTowxImage();

    // ----------------------------------------------------------------------
    // Capture (append) single video frames to an AVI file
    // ----------------------------------------------------------------------

    // NOT IMPLEMENTED

    // ----------------------------------------------------------------------
    // Capture streaming video to an AVI file
    // ----------------------------------------------------------------------

    // NOT IMPLEMENTED

    // ----------------------------------------------------------------------
    // Capture file settings, filename to capture video to
    // ----------------------------------------------------------------------

    // NOT IMPLEMENTED

    // ----------------------------------------------------------------------
    // Audio Setup
    // ----------------------------------------------------------------------

    // NOT IMPLEMENTED

protected:
    // ----------------------------------------------------------------------
    // Implementation
    // ----------------------------------------------------------------------

    void OnCloseWindow(wxCloseEvent &event);

    void OnIdle( wxIdleEvent &event );

    // ----------------------------------------------------------------------
    // Size & Position functions
    // ----------------------------------------------------------------------

    virtual void DoSetSize(int x, int y, int width, int height,
                           int sizeFlags = wxSIZE_AUTO);
    // adjust the scrollbars, use to generally refresh too
    void DoSizeWindow();

    // move when EVT_SCROLLWIN occurs when Overlaying
    void OnScrollWin( wxScrollWinEvent &event );

    // called by wxWindow's EVT_MOVE, make Overlay window follow
    void OnMove( wxMoveEvent &event );

    // draw the frames when using wxImages preview from EVT_PAINT
    void OnPaint( wxPaintEvent &event );

    // ----------------------------------------------------------------------
    // Platform dependent video conversion
    // ----------------------------------------------------------------------

    /// Safely get a wxImage from the mmapped video device.
    bool GetMMapVideoFrame(wxImage& wximg, bool request_another);

    // ----------------------------------------------------------------------
    // Member Variables
    // ----------------------------------------------------------------------

    wxTimer m_previewTimer;    ///< For preview rate adjustment.

    // V4L specific variables

    wxArrayString m_deviceFilenames; ///< All device files from EnumerateDevices() (/dev/video0).

    int m_fd_device;      ///< The video device m_fd_device = open("/dev/video",O_RDWR).

    enum V4L2_Device_Init_Type
    {
        wxV4L2_DEVICE_INIT_NONE    = 0x0000,
        wxV4L2_DEVICE_INIT_DEVICE  = 0x0001,
        wxV4L2_DEVICE_INIT_IO      = 0x0002,
        wxV4L2_DEVICE_INIT_CAPTURE = 0x0004
    };

    int m_v4l2_device_init;

    /// open() a device by filename and return the file descriptor or -1 on error.
    /// If the errMsg string is provided, it will be filled on error, else
    /// error messages will be shown in a wxMessageBox.
    int open_device(const wxString &filename, wxString* errMsg = NULL) const;
    /// Close the device by file descriptor, returns success.
    bool close_device(int fd_device) const;

    /// Initialize the already opened m_fd_device, returns success.
    bool init_device();
    bool uninit_device();

    enum V4L2_IO_Methods_Type
    {
        wxV4L2_IO_METHOD_NONE,
        wxV4L2_IO_METHOD_READ,   // read() into a user created buffer.
        wxV4L2_IO_METHOD_MMAP,   // Use a mmap buffer from the video device.
        wxV4L2_IO_METHOD_USERPTR // Use a user created buffer.
    };

    V4L2_IO_Methods_Type m_v4l2_io_method;

    /// Init capture io using one of the V4L2_IO_Methods_Type.
    bool init_io();
    /// Uninit the capture io setup by init_io().
    bool uninit_io();

    /// Init capture as required for the V4L2_IO_Methods_Type io.
    bool init_capture();
    /// Uninit capture used by the V4L2_IO_Methods_Type io.
    bool uninit_capture();

    /// Wait the given number of seconds for a frame to arrive, returns true
    /// if there is a frame available.
    bool wait_for_frame(int secs);
    /// Read a single frame into the buffer, see V4L2_IO_Methods_Type.
    bool read_frame();

    /// mmap() the memory to mmap_buffer and initialize capture.
    bool init_mmap();
    /// mumap() the memory from mmap_buffer and uninitialize capture.
    bool uninit_mmap();

    /// A buffer for mmaped data.
    typedef struct mmap_buffer
    {
        void*  start;
        size_t length;
    } mmap_buffer;

    std::vector<mmap_buffer> m_mmap_buffers; ///< mmap buffer storage.
    int m_mmap_buffer_index;

    /// A safe ioctl function.
    int xioctl(int fd, int request, void *arg) const;

    /// memset all v4l2 structs to 0.
    void Init_V4L2_structs();

    // v4l2 structs
    struct v4l2_capability        m_v4l2_capability;
    struct v4l2_fmtdesc           m_v4l2_fmtdesc;
    struct v4l2_format            m_v4l2_format;

    std::vector<v4l2_capability>  m_v4l2_capability_vector;
    std::vector<v4l2_fmtdesc>     m_v4l2_fmtdesc_vector;
    std::vector<v4l2_frmsizeenum> m_v4l2_frmsizeenum_vector;
    std::vector<v4l2_frmivalenum> m_v4l2_frmivalenum_vector;

    int Get_v4l2_capability();
    int Get_v4l2_fmtdesc();
    int Get_v4l2_format();

    int Get_v4l2_fmtdesc_vector();
    int Get_v4l2_frmsizeenum_vector();
    int Get_v4l2_frmivalenum_vector();

    wxString Get_v4l2_capability_String(const v4l2_capability& v4l2_capability_) const;
    wxString Get_v4l2_fmtdesc_String(const v4l2_fmtdesc& v4l2_fmtdesc_) const;
    wxString Get_v4l2_format_String(const v4l2_format& v4l2_format_) const;
    // The v4l2_pix_format is a member of the v4l2_format
    wxString Get_v4l2_pix_format_String(const v4l2_pix_format& v4l2_pix_format_) const;
    wxString Get_v4l2_frmsizeenum_String(const v4l2_frmsizeenum& v4l2_frmsizeenum_) const;
    wxString Get_v4l2_frmivalenum_String(const v4l2_frmivalenum& v4l2_frmivalenum_) const;

private:
    void Init();
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxVideoCaptureWindowV4L)
};

#endif // !defined(__WXMSW__)

#endif //__WX_VCAP_V4L_H__
