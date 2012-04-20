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

#include <linux/videodev2.h>

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

    void EnumerateDevices();

    // ----------------------------------------------------------------------
    // Connect or Disconnect to device
    // ----------------------------------------------------------------------

    bool DeviceConnect(int index);
    bool DeviceDisconnect();

    // ----------------------------------------------------------------------
    // Display dialogs to set/get video characteristics
    // ----------------------------------------------------------------------

    void VideoCustomFormatDialog();
    void PropertiesDialog();
    wxString GetPropertiesString();

    // ----------------------------------------------------------------------
    // Video characteristics and manipulation
    // ----------------------------------------------------------------------

    bool GetVideoFormat( int *width, int *height, int *bpp, FOURCC *fourcc );
    bool SetVideoFormat( int width, int height, int bpp, FOURCC fourcc );

    // ----------------------------------------------------------------------
    // Capture Preview and Overlay
    // ----------------------------------------------------------------------

    void OnPreviewwxImageTimer(wxTimerEvent& event); // get frames

    bool Preview(bool onoff, bool wxpreview = false);
    bool Overlay(bool WXUNUSED(on)) { return false; }

    bool SetPreviewRateMS( unsigned int msperframe = 66 )
	{ return DoSetPreviewRateMS( msperframe ); }

    // ----------------------------------------------------------------------
    // Capture single frames, take snapshots of streaming video
    // ----------------------------------------------------------------------

    bool SnapshotToWindow();
    bool SnapshotToClipboard();
    bool SnapshotToBMP( const wxString &filename );
    bool SnapshotTowxImage( wxImage &image);
    bool SnapshotTowxImage();

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

    // move m_hWndC when EVT_SCROLLWIN occurs when Overlaying
    void OnScrollWin( wxScrollWinEvent &event );

    // called by wxWindow's EVT_MOVE, make Overlay window follow
    void OnMove( wxMoveEvent &event );

    // draw the frames when using wxImages preview from EVT_PAINT
    void OnDraw( wxPaintEvent &event );

    // ----------------------------------------------------------------------
    // Platform dependent video conversion
    // ----------------------------------------------------------------------

    // FIXME add conversion here!

    // ----------------------------------------------------------------------
    // Member Variables
    // ----------------------------------------------------------------------

    // Generic variables
    wxArrayString                 m_deviceFilenames; // all device files from EnumerateDevices

    unsigned char *m_bmpdata;       // big 'ole temp storage for DIB

    bool m_grab_wximage;            // grab a single frame into m_wximage
    bool m_getting_wximage;         // true when filling the m_wximage

    wxTimer m_preview_wximage_timer; // for preview rate adjustment

    wxString m_capturefilename;

    wxString m_statustext;          // MSW status messages
    wxString m_errortext;           // MSW error messages

    // V4L specific variables

    // safe - open and close a device (can call w/o checks)
    int m_fd_device;      // the device m_fd_device = open("/dev/video",O_RDWR)

    /// open() a device by filename and return the file descriptor or -1 on error.
    /// If the errMsg string is provided, it will be filled on error, else
    /// error messages will be shown in a wxMessageBox.
    int open_device(const wxString &filename, wxString* errMsg = NULL) const;
    /// Close the device by file descriptor, returns success.
    bool close_device(int fd_device) const;

    // safe - map and unmap the shared video memory (can call w/o checks)
    bool mmap_mem();        // mmap the memory to m_map
    bool munmap_mem();      // mumap the memory from m_map

    typedef struct mmap_buffer
    {
        void * start;
        size_t length;
    } mmap_buffer;

    std::vector<mmap_buffer>  m_mmap_buffers;

    // safe - ioctl function
    int xioctl(int fd, int request, void *arg) const;

    void Init_V4L2_structs();

    // v4l2 structs
    struct v4l2_capability  m_v4l2_capability;
    struct v4l2_fmtdesc     m_v4l2_fmtdesc;
    struct v4l2_format      m_v4l2_format;

    std::vector<v4l2_capability>  m_v4l2_capability_vector;
    std::vector<v4l2_fmtdesc>     m_v4l2_fmtdesc_vector;

    int Get_v4l2_capability();
    int Get_v4l2_fmtdesc();
    int Get_v4l2_fmtdesc_vector();
    int Get_v4l2_format();

    void Print_v4l2_capability(const v4l2_capability& v4l2_capability_) const;
    void Print_v4l2_fmtdesc(const v4l2_fmtdesc& v4l2_fmtdesc_) const;
    void Print_v4l2_format(const v4l2_format& v4l2_format_) const;
    void Print_v4l2_pix_format(const v4l2_pix_format& v4l2_pix_format_) const;

private:
    void Init();
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxVideoCaptureWindowV4L)
};

#endif // !defined(__WXMSW__)

#endif //__WX_VCAP_V4L_H__
