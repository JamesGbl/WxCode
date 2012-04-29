/////////////////////////////////////////////////////////////////////////////
// Name:        vcap_v4l.h - wxVideoCaptureWindow using Linux V4L2 API
// Author:      John Labenski
// Created:     7/06/2001
// Modified:    01/14/03
// Copyright:   John Labenski
// License:     wxWidgets V2.0
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_VCAP_V4L_H__
#define __WX_VCAP_V4L_H__

#include <wx/defs.h>

#if !defined(__WXMSW__)

#include <linux/videodev2.h> // V4L2
#include <vector>

//---------------------------------------------------------------------------
/** @class wxVideoCaptureWindowV4L
    @brief A wxVideoCaptureWindow for Linux using V4L2 to view
    streaming video or take snapshots.
*/ // -----------------------------------------------------------------------
class WXDLLIMPEXP_VIDCAP wxVideoCaptureWindowV4L : public wxVideoCaptureWindowBase
{
public:
    wxVideoCaptureWindowV4L() : wxVideoCaptureWindowBase() { Init(); }
    wxVideoCaptureWindowV4L( wxWindow *parent, wxWindowID id = wxID_ANY,
                             const wxPoint &pos = wxDefaultPosition,
                             const wxSize &size = wxDefaultSize,
                             long style = wxSIMPLE_BORDER,
                             const wxString &name = wxT("wxVideoCaptureWindow"));

    bool Create( wxWindow *parent, wxWindowID id = wxID_ANY,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 long style = wxSIMPLE_BORDER,
                 const wxString &name = wxT("wxVideoCaptureWindow"));

    virtual ~wxVideoCaptureWindowV4L();

    // ----------------------------------------------------------------------
    /// @name Device descriptions & versions, get and enumerate
    // ----------------------------------------------------------------------
    /// @{

    virtual bool EnumerateDevices();               ///< Enumerates /dev/video*

    ///< Enumerate a single device "/dev/video0", returns index for device or wxNOT_FOUND on error.
    int EnumerateDevice(const wxString& fileName);

    /// @}
    // ----------------------------------------------------------------------
    /// @name Connect or Disconnect to device
    // ----------------------------------------------------------------------
    /// @{

    /// Connect to a device by filename, returns success.
    /// If it connected you can call GetDeviceIndex() to get the device index.
    bool DeviceConnect(const wxString& fileName);

    virtual bool DeviceConnect(int index);
    virtual bool DeviceDisconnect();

    /// @}
    // ----------------------------------------------------------------------
    /// @name Display dialogs to set/get video characteristics
    // ----------------------------------------------------------------------
    /// @{

    virtual bool HasVideoSourceDialog()  { return false; }
    virtual void ShowVideoSourceDialog() {}

    virtual bool HasVideoFormatDialog()  { return false; }
    virtual void ShowVideoFormatDialog() {}
    virtual void ShowVideoCustomFormatDialog();

    virtual bool HasVideoDisplayDialog()  { return false; }
    virtual void ShowVideoDisplayDialog() {}

    virtual wxString GetPropertiesString();

    /// @}
    // ----------------------------------------------------------------------
    /// @name Video format and characteristics
    // ----------------------------------------------------------------------
    /// @{

    virtual bool GetVideoFormat( int *width, int *height,
                                 int *bpp, FOURCC *fourcc ) const;
    virtual bool SetVideoFormat( int width, int height,
                                 int bpp, FOURCC fourcc );

    /// @}
    // ----------------------------------------------------------------------
    /// @name Capture Preview and Overlay
    // ----------------------------------------------------------------------
    /// @{

    virtual bool Preview(bool onoff, bool wxpreview = false);
    virtual bool PreviewScaled(bool fit_window);
    virtual bool SetPreviewRateMS( unsigned int msperframe = 66 );
    virtual bool Overlay(bool WXUNUSED(on)) { return false; }

    void OnPreviewTimer(wxTimerEvent& event);
    void OnFrameRequest(wxVideoCaptureEvent& event);

    /// @}
    // ----------------------------------------------------------------------
    /// @name Capture single frames, take snapshots of streaming video
    // ----------------------------------------------------------------------
    /// @{

    virtual bool SnapshotToWindow();
    virtual bool SnapshotToClipboard();
    virtual bool SnapshotToBMP( const wxString &filename );
    virtual bool SnapshotTowxImage( wxImage &image );
    virtual bool SnapshotTowxImage();

    /// @}
    // ----------------------------------------------------------------------
    /// @name Capture (append) single video frames to an AVI file
    // ----------------------------------------------------------------------
    /// @{

    // NOT IMPLEMENTED

    /// @}
    // ----------------------------------------------------------------------
    /// @name Capture streaming video to an AVI file
    // ----------------------------------------------------------------------
    /// @{

    // NOT IMPLEMENTED

    /// @}
    // ----------------------------------------------------------------------
    /// @name Capture file settings, filename to capture video to
    // ----------------------------------------------------------------------
    /// @{

    // NOT IMPLEMENTED

    /// @}
    // ----------------------------------------------------------------------
    /// @name Audio Setup
    // ----------------------------------------------------------------------
    /// @{

    // NOT IMPLEMENTED

    /// @}
    // ----------------------------------------------------------------------
    /// @name Utility Functions
    // ----------------------------------------------------------------------
    /// @{

    // nothing to do

    /// @}
protected:

    // ----------------------------------------------------------------------
    /// @name Platform dependent video conversion
    // ----------------------------------------------------------------------
    /// @{

    /// Safely get a wxImage from the already opened and initialized video device.
    bool GetVideoFrame(wxImage& wximg, bool request_another);

    /// @}
    // ----------------------------------------------------------------------
    // Implementation
    // ----------------------------------------------------------------------

    void OnCloseWindow(wxCloseEvent &event);

    // ----------------------------------------------------------------------
    /// @name Size & Position functions
    // ----------------------------------------------------------------------
    /// @{

    /// Override wxWindow::DoSetSize(...).
    virtual void DoSetSize(int x, int y, int width, int height,
                           int sizeFlags = wxSIZE_AUTO);
    /// Adjust the scrollbars, use to generally refresh too.
    void DoSizeWindow();

    /// Move when EVT_SCROLLWIN occurs when Overlaying.
    void OnScrollWin( wxScrollWinEvent& event );

    /// Called by wxWindow's EVT_MOVE, make Overlay window follow.
    void OnMove( wxMoveEvent& event );

    /// Draw the frames when using wxImages preview from EVT_PAINT.
    void OnPaint( wxPaintEvent& event );

    /// @}
    // ----------------------------------------------------------------------
    // Member Variables
    // ----------------------------------------------------------------------

    // V4L specific variables

    wxArrayString m_deviceFilenames; ///< All device files from EnumerateDevices() (/dev/video0).

    int m_fd_device; ///< The video device m_fd_device = open("/dev/video",O_RDWR).

    /// Bit masks of how the device has been initialized.
    enum V4L2_Device_Init_Type
    {
        wxV4L2_DEVICE_INIT_NONE    = 0x0000, ///< Device has not been initialized, check m_fd_device != 1 to see if it's opened.
        wxV4L2_DEVICE_INIT_DEVICE  = 0x0001, ///< init_device() has been called.
        wxV4L2_DEVICE_INIT_IO      = 0x0002, ///< init_io() has been called.
        wxV4L2_DEVICE_INIT_CAPTURE = 0x0004  ///< init_capture() has been called.
    };

    int m_v4l2_device_init; ///< Bit set of enum V4L2_Device_Init_Type to remember what we've initialized.

    /// open() a device by filename and return the file descriptor or -1 on error.
    /// If the errMsg string is provided, it will be filled on error, else
    /// error messages will be shown in a wxMessageBox.
    static int open_device(const wxString &filename, wxString* errMsg = NULL);
    /// Close the device by file descriptor, returns success.
    static bool close_device(int fd_device);

    /// Initialize the already opened m_fd_device, returns success.
    bool init_device();
    /// Uninitialize the device from the init_device() call, returns success.
    bool uninit_device();

    /// What method is being used for init_io().
    enum V4L2_IO_Methods_Type
    {
        wxV4L2_IO_METHOD_NONE,   ///< No io method chosen.
        wxV4L2_IO_METHOD_READ,   ///< read() into a user created buffer.
        wxV4L2_IO_METHOD_MMAP,   ///< Use a mmap buffer from the video device.
        wxV4L2_IO_METHOD_USERPTR ///< Use a user created buffer.
    };

    V4L2_IO_Methods_Type m_v4l2_io_method; ///< The selected device io method.

    /// Initialize device io using one of the V4L2_IO_Methods_Type.
    bool init_io();
    /// Uninitialize the device io from the init_io() call.
    bool uninit_io();

    /// Initialize device capture as required for the V4L2_IO_Methods_Type io.
    bool init_capture();
    /// Uninitialize device capture from the init_capture() call.
    bool uninit_capture();

    /// Initialize the device for read()ing frames.
    bool init_read(unsigned int num_bytes);
    /// Uninitialize the device from the init_read() call.
    bool uninit_read();

    /// Initialize the device for mmap()ed memory for reading frames.
    bool init_mmap();
    /// Uninitialize and mumap() the memory from the init_mmap() call.
    bool uninit_mmap();

    /// Initialize the device using userdata memory for reading frames.
    bool init_userp(unsigned int num_bytes);
    /// Uninitialize and free the memory from the init_userp() call.
    bool uninit_userp();

    /// Wait the given number of seconds for a frame to arrive, returns true
    /// if there is a frame available.
    bool wait_for_frame(int secs);
    /// Read a single frame into the buffer, as appropriate for the V4L2_IO_Methods_Type.
    bool read_frame();

    /// A buffer for frame data, used init_read(), init_mmap(), init_userp().
    typedef struct mem_buffer
    {
        void*  data;   ///< The mmaped data or malloced data.
        size_t length; ///< The length of the data in bytes.
    } mem_buffer;

    std::vector<mem_buffer> m_mem_buffers; ///< read(), mmap(), userp buffer storage.
    int m_mem_buffer_index;                ///< Which index into the m_mem_buffers is valid.

    /// A safe ioctl function, retries if EINTR.
    static int xioctl(int fd, int request, void *arg);

    /// memset all v4l2 structs to 0.
    void Init_V4L2_structs();

    // v4l2 structs, these are updated to the current state of the device
    struct v4l2_capability        m_v4l2_capability;
    struct v4l2_fmtdesc           m_v4l2_fmtdesc;
    struct v4l2_format            m_v4l2_format;
    struct v4l2_buffer            m_v4l2_buffer;

    std::vector<v4l2_capability>  m_devices_v4l2_capabilities; ///< capabilities of devices found on the system.
    std::vector<v4l2_fmtdesc>     m_device_v4l2_fmtdescs;      ///< FOURCC formats supported by the device.
    std::vector<v4l2_frmsizeenum> m_device_v4l2_frmsizeenums;  ///< frame sizes supported by the device.
    std::vector<v4l2_frmivalenum> m_device_v4l2_frmivalenums;  ///< frame intervals supported by the device (may be empty).

    int Get_v4l2_capability();
    int Get_v4l2_fmtdesc();
    int Get_v4l2_format();

    int Get_device_v4l2_fmtdescs();
    int Get_device_v4l2_frmsizeenums();
    int Get_device_v4l2_frmivalenums();

    wxString Get_v4l2_capability_String(const v4l2_capability& v4l2_capability_) const;
    wxString Get_v4l2_fmtdesc_String   (const v4l2_fmtdesc&    v4l2_fmtdesc_) const;
    wxString Get_v4l2_format_String    (const v4l2_format&     v4l2_format_) const;
    // The v4l2_pix_format is a member of the v4l2_format
    wxString Get_v4l2_pix_format_String (const v4l2_pix_format&  v4l2_pix_format_) const;
    wxString Get_v4l2_frmsizeenum_String(const v4l2_frmsizeenum& v4l2_frmsizeenum_) const;
    wxString Get_v4l2_frmivalenum_String(const v4l2_frmivalenum& v4l2_frmivalenum_) const;

private:
    void Init();
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxVideoCaptureWindowV4L)
};

#endif // !defined(__WXMSW__)

#endif //__WX_VCAP_V4L_H__
