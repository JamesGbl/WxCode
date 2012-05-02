/////////////////////////////////////////////////////////////////////////////
// Name:        vcap_v4l.cpp - wxVideoCaptureWindow using Linux V4L2 API
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

#if !defined(__WXMSW__)

#include <wx/clipbrd.h>
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/settings.h>

#include "wx/vidcap/vcapwin.h"
#include "vcapdlgs.h"
#include "vcapconv.h"

#include <fcntl.h>  // for open(char*, O_RDWR)
#include <unistd.h> // for close(int)
#include <errno.h>

//#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
#include <malloc.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <map>

//----------------------------------------------------------------------------
// wxVideoCaptureWindow #defines and globals
//----------------------------------------------------------------------------

#if wxCHECK_VERSION(2,9,0)
    #define STDSTR_TO_WXSTR(s) wxString(s)
    #define CHAR_TO_WXSTR(s)   wxString(s)
#else
    #define STDSTR_TO_WXSTR(s) wxString(s.c_str(), wxConvUTF8)
    #define CHAR_TO_WXSTR(s)   wxString(s,         wxConvUTF8)
#endif

//------------------------------------------------------------------------------
// wxVideoCaptureWindow
//------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(wxVideoCaptureWindowV4L, wxVideoCaptureWindowBase)

BEGIN_EVENT_TABLE(wxVideoCaptureWindowV4L, wxVideoCaptureWindowBase)
    EVT_MOVE(                                     wxVideoCaptureWindowV4L::OnMove)
    EVT_PAINT(                                    wxVideoCaptureWindowV4L::OnPaint)
    EVT_SCROLLWIN(                                wxVideoCaptureWindowV4L::OnScrollWin)
    EVT_TIMER( ID_wxVIDCAP_PREVIEW_WXIMAGE_TIMER, wxVideoCaptureWindowV4L::OnPreviewTimer)
    EVT_VIDEO_FRAME_REQ( wxID_ANY,                wxVideoCaptureWindowV4L::OnFrameRequest)
    EVT_CLOSE(                                    wxVideoCaptureWindowV4L::OnCloseWindow)
END_EVENT_TABLE()

void wxVideoCaptureWindowV4L::Init()
{
    m_fd_device         = -1;
    m_preview_wximage   = false;

    m_v4l2_device_init  = wxV4L2_DEVICE_INIT_NONE;
    m_v4l2_io_method    = wxV4L2_IO_METHOD_NONE;

    m_mem_buffer_index  = -1;

    Init_V4L2_structs();
}

void wxVideoCaptureWindowV4L::Init_V4L2_structs()
{
    memset(&m_v4l2_capability, 0, sizeof(v4l2_capability));
    memset(&m_v4l2_fmtdesc,    0, sizeof(v4l2_fmtdesc));
    memset(&m_v4l2_format,     0, sizeof(v4l2_format));
    memset(&m_v4l2_buffer,     0, sizeof(v4l2_buffer));

    // don't clear the m_devices_v4l2_capabilities since it enumerates system devices
    m_device_v4l2_fmtdescs.clear();
    m_device_v4l2_frmsizeenums.clear();
    m_device_v4l2_frmivalenums.clear();
}

wxVideoCaptureWindowV4L::wxVideoCaptureWindowV4L( wxWindow *parent, wxWindowID id,
                                                  const wxPoint &pos, const wxSize &size,
                                                  long style, const wxString &name )
                        :wxVideoCaptureWindowBase()
{
    Init();

    if (!Create( parent, id, pos, size, style, name ))
        wxMessageBox(wxT("Can't create capture window"), wxT("wxVideoCaptureWindow Error"));
}

bool wxVideoCaptureWindowV4L::Create( wxWindow *parent, wxWindowID id,
                                      const wxPoint &pos, const wxSize &size,
                                      long style, const wxString &name )
{
    if (!wxVideoCaptureWindowBase::Create(parent, id, pos, size, style, name))
        return false;

    static bool s_added_jpeg_handler = false;

    if ( !s_added_jpeg_handler )
    {
        // Of course, if the user code did add it already, we have nothing to do.
        if ( !wxImage::FindHandler(wxBITMAP_TYPE_JPEG) )
            wxImage::AddHandler(new wxJPEGHandler);

        // In any case, no need to do it again.
        s_added_jpeg_handler = true;
    }

    EnumerateDevices();

    // open the first device, if there is one
    //if (GetDeviceCount() > 0) DeviceConnect(0);

    // set up some generic useful parameters
    SetPreviewRateMS(100);              // 10 fps

    return true;
}

// yes this has the same stuff as OnCloseWindow, erm... which is better?
wxVideoCaptureWindowV4L::~wxVideoCaptureWindowV4L()
{
    DeviceDisconnect();
}

void wxVideoCaptureWindowV4L::OnCloseWindow(wxCloseEvent &event)
{
    event.Skip();
}

void wxVideoCaptureWindowV4L::DoSetSize(int x, int y, int width, int height,
                                        int sizeFlags)
{
    wxVideoCaptureWindowBase::DoSetSize(x, y, width, height, sizeFlags);
    DoSizeWindow();
}

void wxVideoCaptureWindowV4L::DoSizeWindow()
{
    if (IsPreviewScaled())
    {
        // the video will fill the extent of the window
        SetScrollbars(1, 1, 2, 2, 0, 0, true); // make them go away
    }
    else
    {
        // use scrollbars when video is larger than window size
        SetScrollbars(1, 1, GetImageWidth(), GetImageHeight(), 0, 0, true);
    }

    int virtual_width, virtual_height;
    GetVirtualSize( &virtual_width, &virtual_height );

    // clear up backgound if necessary
    if ((GetImageWidth() < virtual_width) || (GetImageHeight() < virtual_height))
    {
        Refresh(false);
    }
}

void wxVideoCaptureWindowV4L::OnMove( wxMoveEvent &event )
{
    event.Skip();
}

void wxVideoCaptureWindowV4L::OnScrollWin( wxScrollWinEvent &event )
{
    event.Skip();
}

void wxVideoCaptureWindowV4L::OnPaint( wxPaintEvent &event )
{
    wxPaintDC dc(this);
    DoPaint(dc);
}

// ----------------------------------------------------------------------
// Device Descriptions, get and enumerate
// ----------------------------------------------------------------------

bool wxVideoCaptureWindowV4L::EnumerateDevices()
{
    m_deviceNames.Clear();
    m_deviceVersions.Clear();
    m_deviceFilenames.Clear();
    m_devices_v4l2_capabilities.clear();

    wxFileName fileName(wxT("/dev/video*"));

    wxString fname;
    wxString rootDir(fileName.GetPath());
    wxDir dir(rootDir);

    if (!dir.IsOpened())
    {
        SendErrorEvent(-1, wxT("Unable to open video devices in : '") + rootDir + wxT("'"));
        return false;
    }

    bool has_files = dir.GetFirst(&fname, fileName.GetFullName(), wxDIR_FILES);

    while (has_files)
    {
        wxString deviceFilename(rootDir+wxFILE_SEP_PATH+fname);
        EnumerateDevice(deviceFilename);
        has_files = dir.GetNext(&fname);
    }

    return true; // no errors, even if we didn't find anything.
}

int wxVideoCaptureWindowV4L::EnumerateDevice(const wxString& deviceFilename)
{
    int device_idx = wxNOT_FOUND;

#if wxCHECK_VERSION(2,9,4)
    if (!wxFileName::Exists(deviceFilename)) // use FileExists() in 2.8 only works for files, not devices
        return device_idx;
#endif //wxCHECK_VERSION(2,9,4)

    wxString errMsg; // use this as a sink for the error messages
    int fd_device = open_device(deviceFilename, &errMsg);

    if (fd_device < 0) return device_idx;

    struct v4l2_capability v4l2_capability_;
    memset(&v4l2_capability_, 0, sizeof(v4l2_capability));
    bool ok = (0==ioctl(fd_device, VIDIOC_QUERYCAP, &v4l2_capability_));


    if (ok && (v4l2_capability_.capabilities & V4L2_CAP_VIDEO_CAPTURE))
    {
        m_devices_v4l2_capabilities.push_back(v4l2_capability_);
        memcpy(&m_devices_v4l2_capabilities[m_devices_v4l2_capabilities.size()-1], &v4l2_capability_, sizeof(v4l2_capability));

        wxString deviceName = wxConvUTF8.cMB2WX((const char*)v4l2_capability_.card);

        // See if we've already enumerated this device and replace its values.
        device_idx = m_deviceFilenames.Index(deviceFilename);

        if (device_idx != wxNOT_FOUND)
        {
            m_deviceNames[device_idx]     = deviceName + wxT(" (") + deviceFilename + wxT(")");
            m_deviceVersions[device_idx]  = wxString::Format(wxT("%u"), v4l2_capability_.version);
            m_deviceFilenames[device_idx] = deviceFilename;
        }
        else
        {
            m_deviceNames.Add(deviceName + wxT(" (") + deviceFilename + wxT(")"));
            m_deviceVersions.Add(wxString::Format(wxT("%u"), v4l2_capability_.version));
            m_deviceFilenames.Add(deviceFilename);
            device_idx = m_deviceNames.GetCount() - 1;
        }
    }
    //else perror("Error enumerating video device");

    close_device(fd_device);

    return device_idx;
}

// ----------------------------------------------------------------------
// Connect and Disconnect to device
// ----------------------------------------------------------------------

bool wxVideoCaptureWindowV4L::DeviceConnect(const wxString& fileName)
{
    // Close whatever was open and (re)open the new one
    DeviceDisconnect();

    // Add this device to our list if it's valid.
    int device_idx = EnumerateDevice(fileName);
    // if we got an invalid index, there was a problem with it
    if (device_idx == wxNOT_FOUND) return false;

    return DeviceConnect(device_idx);
}

bool wxVideoCaptureWindowV4L::DeviceConnect(int index)
{
    wxCHECK_MSG((index >= 0) && (index < GetDeviceCount()), false, wxT("invalid device index"));

    // Close whatever was open and (re)open the new one
    DeviceDisconnect();

    m_fd_device = open_device(m_deviceFilenames[index]);

    if (m_fd_device > -1)
    {
        m_deviceIndex = index;

        bool ok = init_device();
        DoSizeWindow();
        return ok;
    }

    return false;
}

bool wxVideoCaptureWindowV4L::DeviceDisconnect()
{
    m_preview_wximage = false;
    m_imageSize       = wxSize(0, 0);
    m_wximage.Destroy();

    if (wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_CAPTURE)) uninit_capture();
    if (wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_IO))      uninit_io();
    if (wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_DEVICE))  uninit_device();

    if (m_fd_device != -1) close_device(m_fd_device);

    m_deviceIndex = -1;
    m_fd_device   = -1;
    Init_V4L2_structs();

    return true;
}

// ----------------------------------------------------------------------
// Display device's dialogs to set video characteristics
// ----------------------------------------------------------------------

void wxVideoCaptureWindowV4L::ShowVideoCustomFormatDialog()
{
    if (IsDeviceConnected())
    {
        if (!FindWindow(ID_wxVIDCAP_VIDEOFORMATDLG))
        {
            wxVideoCaptureWindowCustomVideoFormatDialog* dlg = new wxVideoCaptureWindowCustomVideoFormatDialog(this, ID_wxVIDCAP_VIDEOFORMATDLG);
            dlg->Show();
        }
    }
}

wxString wxVideoCaptureWindowV4L::GetPropertiesString()
{
    int n, count;
    wxString s = wxVideoCaptureWindowBase::GetPropertiesString();

    s += wxT("=============================================\n");
    s += wxT("Video 4 Linux 2 (V4L2) device structs\n");
    s += wxT("=============================================\n");

    s += wxT("\n");
    s += wxT("---------------------------------------------\n");
    s += Get_v4l2_capability_String(m_v4l2_capability);

    s += wxT("\n");
    s += wxT("---------------------------------------------\n");
    s += Get_v4l2_format_String(m_v4l2_format);

    s += wxT("\n");
    s += wxT("---------------------------------------------\n");
    s += Get_v4l2_fmtdesc_String(m_v4l2_fmtdesc);

    s += wxT("\n");
    s += wxT("---------------------------------------------\n");
    count = (int)m_device_v4l2_fmtdescs.size();
    s += wxString::Format(wxT("Supported Video Formats      : %d\n"), count);
    for (n = 0; n < count; ++n)
    {
        s += Get_v4l2_fmtdesc_String(m_device_v4l2_fmtdescs[n]);
    }

    s += wxT("\n");
    s += wxT("---------------------------------------------\n");
    count = (int)m_device_v4l2_frmsizeenums.size();
    s += wxString::Format(wxT("Supported Video Sizes        : %d\n"), count);
    for (n = 0; n < count; ++n)
    {
        s += Get_v4l2_frmsizeenum_String(m_device_v4l2_frmsizeenums[n]);
    }

    s += wxT("\n");
    s += wxT("---------------------------------------------\n");
    count = (int)m_device_v4l2_frmivalenums.size();
    s += wxString::Format(wxT("Supported Video Intervals    : %d\n"), count);
    for (n = 0; n < count; ++n)
    {
        s += Get_v4l2_frmivalenum_String(m_device_v4l2_frmivalenums[n]);
    }

    s += wxT("\n");
    count = (int)m_devices_v4l2_capabilities.size();
    s += wxT("=============================================\n");
    s += wxString::Format(wxT("Enumerated devices found     : %d\n"), count);
    s += wxT("=============================================\n");
    for (n = 0; n < count; ++n)
    {
        s += wxString::Format(wxT("\n%2d : %s\n"), n, m_deviceFilenames[n].c_str());
        s += Get_v4l2_capability_String(m_devices_v4l2_capabilities[n]);
        s += wxT("\n");
    }

    return s;
}

// ----------------------------------------------------------------------
// Video format and characteristics
// ----------------------------------------------------------------------

bool wxVideoCaptureWindowV4L::GetVideoFormat(int *width, int *height,
                                             int *bpp, FOURCC *fourcc) const
{
    if (IsDeviceConnected())
    {
        if (width)  *width  = m_v4l2_format.fmt.pix.width;
        if (height) *height = m_v4l2_format.fmt.pix.height;
        if (bpp)    *bpp    = 0; //m_video_picture.depth;
        if (fourcc) *fourcc = m_v4l2_format.fmt.pix.pixelformat;
        return true;
    }

    if (width)  *width  = 0;
    if (height) *height = 0;
    if (bpp)    *bpp    = 0;
    if (fourcc) *fourcc = wxNullFOURCC;
    return false;
}

bool wxVideoCaptureWindowV4L::SetVideoFormat( int width, int height,
                                              int bpp, FOURCC format )
{
    wxCHECK_MSG(IsDeviceConnected(), false, wxT("Error setting video format - no device connected."));

    v4l2_format v4l2_format_;
    memset(&v4l2_format_, 0, sizeof(v4l2_format));
    v4l2_format_.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    int ret = ioctl(m_fd_device, VIDIOC_G_FMT, &v4l2_format_);

    v4l2_format_.fmt.pix.width       = width;
    v4l2_format_.fmt.pix.height      = height;
    v4l2_format_.fmt.pix.pixelformat = format;
    //v4l2_format_.fmt.pix.colorspace  = m_v4l2_format.fmt.pix.colorspace; //V4L2_COLORSPACE_SRGB;

    v4l2_format_.fmt.pix.bytesperline = 0;
    v4l2_format_.fmt.pix.sizeimage    = 0;

    bool is_previewing = IsPreviewing();
    if (is_previewing)
        Preview(false);

    ret = ioctl(m_fd_device, VIDIOC_TRY_FMT, &v4l2_format_);
    if (ret != 0)
    {
        SendErrorEvent(errno, wxT("wxVidCap::SetVideoFormat() VIDIOC_TRY_FMT ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
    }
    else
    {
        ret = xioctl(m_fd_device, VIDIOC_S_FMT, &v4l2_format_);
        if (ret != 0)
            SendErrorEvent(errno, wxT("wxVidCap::SetVideoFormat() VIDIOC_S_FMT ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));

        // don't exit, reread the format structs below
    }

    Get_v4l2_fmtdesc(); // update the video format
    Get_v4l2_format();

    if (is_previewing)
        Preview(true);

    DoSizeWindow();     // clean it up, readjust the scollbars

    return (ret == 0);
}

// ----------------------------------------------------------------------
// Capture Preview and Overlay
// ----------------------------------------------------------------------

bool wxVideoCaptureWindowV4L::Preview(bool onoff, bool wxpreview)
{
    if (!IsDeviceConnected())
    {
        m_preview_wximage = false;
        return false;
    }

    m_preview_wximage = onoff;

    if (!m_preview_wximage)
    {
        m_previewTimer.Stop();

        if (wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_CAPTURE)) uninit_capture();
        if (wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_IO))      uninit_io();
    }
    else
    {
        if (!wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_IO))      init_io();
        if (!wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_CAPTURE)) init_capture();

        m_previewTimer.Start(1, true);
    }

    return true;
}

bool wxVideoCaptureWindowV4L::PreviewScaled(bool fit_window)
{
    if (fit_window)
    {
        // Remove the scrollbars so the client size doesn't include the scrollbars
        SetScrollbars(1, 1, 2, 2, 0, 0, false);
    }

    wxVideoCaptureWindowBase::PreviewScaled(fit_window);

    DoSizeWindow();     // clean it up, readjust the scollbars

    return true;
}

bool wxVideoCaptureWindowV4L::SetPreviewRateMS( unsigned int msperframe )
{
    wxVideoCaptureWindowBase::SetPreviewRateMS(msperframe);

    if (m_preview_wximage)
    {
        m_previewTimer.Stop();
        m_previewTimer.Start(1, true);
    }

    return true;
}

void wxVideoCaptureWindowV4L::OnPreviewTimer(wxTimerEvent& event)
{
    if (!m_preview_wximage || !IsDeviceConnected()) return;

    if (!m_getting_wximage && GetVideoFrame(m_wximage, true))
    {
        OnPreFrame();

        if (OnProcessFrame(m_wximage)) // Call the stub in case something is done here
            Refresh(false);   // draw image

        OnPostFrame();
    }

    // Use an event and queue it so this function returns before OnFrameRequest()
    // is called. This allows the wxevent handler to keep going so menus
    // work event at high frame rates.
    wxVideoCaptureEvent frameReqEvent(wxEVT_VIDEO_FRAME_REQ, this, GetId());
    GetEventHandler()->AddPendingEvent(frameReqEvent);
}

void wxVideoCaptureWindowV4L::OnFrameRequest(wxVideoCaptureEvent& )
{
    // Use a one-shot to keep the frame rate constant and to not allow
    // an overflow of timer requests that we can't get frames for
    wxLongLong millis_now = wxGetLocalTimeMillis();
    int remaining_ms = int(m_previewmsperframe) - int(wxLongLong(millis_now - m_lastframetimemillis).GetLo());
    remaining_ms = wxMax(remaining_ms, 1); // alays let this function return
    remaining_ms = wxMin(remaining_ms, (int)m_previewmsperframe);
    m_previewTimer.Start(remaining_ms, true);
}

// ----------------------------------------------------------------------
// Capture single frames, take snapshots of streaming video
// ----------------------------------------------------------------------

bool wxVideoCaptureWindowV4L::SnapshotToWindow()
{
    if (!IsDeviceConnected()) return false;

    Preview(false);
    bool ok = SnapshotTowxImage( m_wximage );
    Refresh(false);

    return ok;
}

bool wxVideoCaptureWindowV4L::SnapshotToClipboard()
{
    wxImage wximg;
    bool grabbedOK = SnapshotTowxImage(wximg);

    if (wximg.Ok() && grabbedOK)
    {
        if (wxTheClipboard->Open())
        {
            // This data objects are held by the clipboard,
            // so do not delete them in the app.
            wxTheClipboard->SetData( new wxBitmapDataObject(wxBitmap(wximg)) );
            wxTheClipboard->Close();
            return true;
        }
    }

    return false;
}

bool wxVideoCaptureWindowV4L::SnapshotToBMP( const wxString &filename )
{
    wxImage wximg;
    bool grabbedOK = SnapshotTowxImage(wximg);

    if (wximg.Ok() && grabbedOK)
    {
        return wximg.SaveFile(filename, wxBITMAP_TYPE_BMP);
    }

    return false;
}

bool wxVideoCaptureWindowV4L::SnapshotTowxImage( wxImage &image )
{
    if (!IsDeviceConnected()) return false;

    if (IsPreviewing())
    {
        GetVideoFrame(image, true);
        return true;
    }
    else
    {
        bool ok = true;
        if (!wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_IO))      ok &= init_io();
        if (!wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_CAPTURE)) ok &= init_capture();

        if (ok)
        {
            wait_for_frame(1);
            ok &= GetVideoFrame(image, false);
        }

        if (wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_IO))      uninit_io();
        if (wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_CAPTURE)) uninit_capture();
        return ok;
    }

    return false;
}

bool wxVideoCaptureWindowV4L::SnapshotTowxImage()
{
    return SnapshotTowxImage(m_wximage);
}

// ----------------------------------------------------------------------
// Capture (append) single video frames to an AVI file
// ----------------------------------------------------------------------

// NOT IMPLEMENTED

// ----------------------------------------------------------------------
// Capture streaming video to a file
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

// ----------------------------------------------------------------------
// Platform dependent video conversion
// ----------------------------------------------------------------------

bool wxVideoCaptureWindowV4L::GetVideoFrame(wxImage& wximg, bool request_another)
{
    wxCHECK_MSG(IsDeviceConnected(), false, wxT("wxVidCap device not open for reading frames"));
    wxCHECK_MSG(wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_CAPTURE), false, wxT("wxVidCap device not initialized for reading frames"));

    if (!read_frame()) return false;

    if ((m_mem_buffer_index < 0) || (m_mem_buffer_index >= (int)m_mem_buffers.size()))
        return false;

    void*  image_buf        = m_mem_buffers[m_mem_buffer_index].data;
    size_t image_buf_length = m_mem_buffers[m_mem_buffer_index].length;

    if (!image_buf) return false;

    // For our internal image use a simple atomic block for filling the wxImage
    if (wximg.GetRefData() == m_wximage.GetRefData())
        m_getting_wximage = true;

    if ( m_v4l2_fmtdesc.pixelformat == 1195724874) // FOURCC = "JPEG"
    {
        wxVidCap_JPEG_to_wxImage(image_buf, image_buf_length, wximg);
    }
    else if ((m_imageSize.GetWidth() > 0) && (m_imageSize.GetHeight() > 0))
    {
        // Just show whatever garbage is in memory
        if (!wximg.IsOk() ||
            (wximg.GetWidth()  != m_imageSize.GetWidth()) ||
            (wximg.GetHeight() != m_imageSize.GetHeight()))
        {
            wximg.Create(m_imageSize.GetWidth(), m_imageSize.GetHeight());
        }

        const int data_size = wxMin((int)image_buf_length, 3*m_imageSize.GetWidth()*m_imageSize.GetHeight());
        memcpy(wximg.GetData(), image_buf, data_size);
    }

    m_getting_wximage = false;

    if (request_another && (m_v4l2_io_method != wxV4L2_IO_METHOD_READ))
    {
        // m_v4l2_buffer should be setup correctly from the last call to VIDIOC_DQBUF
        if (-1 == xioctl (m_fd_device, VIDIOC_QBUF, &m_v4l2_buffer))
            SendErrorEvent(errno, wxT("wxVidCap::GetVideoFrame() VIDIOC_QBUF ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
    }

    return true;
}

// ----------------------------------------------------------------------------
// Open/Close the /dev/video device

// static
int wxVideoCaptureWindowV4L::open_device(const wxString &filename, wxString* errMsg_)
{
    wxString errMsg;
    int fd_device = -1; // error opening

    wxCharBuffer dev_buffer(filename.ToAscii()); //wxConvUTF8.cWX2MB(filename.c_str()));
    const char* dev_name = dev_buffer.data();
    wxStructStat st;

    if (-1 == wxStat(filename, &st))
    {
        errMsg = wxString::Format(wxT("Cannot identify '%s': %d, %s"),
                                  filename.wx_str(), errno, strerror(errno));
        if (errMsg_)
            *errMsg_ = errMsg;
        else
        {
            wxMessageBox(errMsg, wxT("Error opening video device"),
                         wxOK | wxCENTRE | wxICON_ERROR);
        }

        //fprintf(stderr, "Cannot identify '%s': %d, %s\n",
        //        dev_name, errno, strerror (errno));
        return fd_device;
    }

    if (!S_ISCHR(st.st_mode))
    {
        errMsg = wxString::Format(wxT("Invalid device '%s'"),
                                  filename.wx_str() );
        if (errMsg_)
            *errMsg_ = errMsg;
        else
        {
            wxMessageBox(errMsg, wxT("Error opening video device"),
                         wxOK | wxCENTRE | wxICON_ERROR);
        }

        //fprintf(stderr, "%s is no device\n", dev_name);
        return fd_device;
    }

    fd_device = open(dev_name, O_RDWR | O_NONBLOCK, 0);

    if (fd_device == -1)
    {
        errMsg = wxString::Format(wxT("Cannot open '%s': %d, %s"),
                                  filename.wx_str(), errno, strerror(errno));
        if (errMsg_)
            *errMsg_ = errMsg;
        else
        {
            wxMessageBox(errMsg, wxT("Error opening video device"),
                         wxOK | wxCENTRE | wxICON_ERROR);
        }

        //fprintf (stderr, "Cannot open '%s': %d, %s\n",
        //         dev_name, errno, strerror (errno));
        return fd_device;
    }

    return fd_device;
}

// static
bool wxVideoCaptureWindowV4L::close_device(int fd_device)
{
    wxCHECK_MSG((fd_device != -1), false, wxT("wxVidCap device already closed."));

    int ret = close(fd_device);

    return ret == 0;
}

bool wxVideoCaptureWindowV4L::init_device()
{
    wxCHECK_MSG(IsDeviceConnected(), false, wxT("wxVidCap device not open for initialization"));
    wxCHECK_MSG(m_v4l2_device_init == wxV4L2_DEVICE_INIT_NONE, false, wxT("wxVidCap device already initialized"));

    // ------------------------------------------------------------------
    // Verify that the connected device can capture video

    int v4l2_cap_ret = Get_v4l2_capability();

    if (v4l2_cap_ret != 0)
        return false;

    if ((m_v4l2_capability.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0)
    {
        SendErrorEvent(errno, wxT("wxVidCap::init_device() VIDIOC_QUERYCAP not a video capture device ERROR"));
        return false;
    }

    m_v4l2_device_init |= wxV4L2_DEVICE_INIT_DEVICE;

    // ------------------------------------------------------------------
    // Read the other v4l2 structs if we know we've got a capture device

    Get_v4l2_fmtdesc();
    Get_v4l2_format();
    Get_device_v4l2_fmtdescs();
    Get_device_v4l2_frmsizeenums();
    Get_device_v4l2_frmivalenums();

    // ------------------------------------------------------------------
    // Setup cropping by clearing it

    struct v4l2_cropcap cropcap;
    struct v4l2_crop    crop;

    memset(&cropcap, 0, sizeof(v4l2_cropcap));
    memset(&crop,    0, sizeof(v4l2_crop));

    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (0 == xioctl(m_fd_device, VIDIOC_CROPCAP, &cropcap))
    {
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c    = cropcap.defrect; // reset to default

        if (-1 == xioctl(m_fd_device, VIDIOC_S_CROP, &crop))
        {
            switch (errno)
            {
                case EINVAL:
                    // Cropping not supported.
                    break;
                default:
                    // Errors ignored.
                    break;
            }
        }
    }
    else
    {
        // Errors ignored.
    }

    return true;
}

bool wxVideoCaptureWindowV4L::uninit_device()
{
    wxCHECK_MSG(IsDeviceConnected(), false, wxT("wxVidCap device closed before uninitializing"));
    wxCHECK_MSG(wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_DEVICE), false, wxT("wxVidCap attempt to uninit non-initialized device"));

    if (wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_IO)) uninit_io();

    m_v4l2_device_init &= ~wxV4L2_DEVICE_INIT_DEVICE;

    return true;
}

bool wxVideoCaptureWindowV4L::init_io()
{
    wxCHECK_MSG(IsDeviceConnected(), false, wxT("wxVidCap device not open for io initialization"));
    wxCHECK_MSG(!wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_IO), false, wxT("wxVidCap device already initialized for io"));

    m_v4l2_io_method = wxV4L2_IO_METHOD_NONE;

    if ((m_v4l2_capability.capabilities & V4L2_CAP_STREAMING) != 0)
    {
        //fprintf (stderr, "%s does not support streaming i/o\n", dev_name);
        // V4L2_IO_METHOD_MMAP or V4L2_IO_METHOD_USERPTR, MMAP seems more robust
        m_v4l2_io_method = wxV4L2_IO_METHOD_MMAP;
    }
    else if ((m_v4l2_capability.capabilities & V4L2_CAP_READWRITE) != 0)
    {
        //fprintf (stderr, "%s does not support read i/o\n", dev_name);
        m_v4l2_io_method = wxV4L2_IO_METHOD_READ;
    }

    if (m_v4l2_io_method == wxV4L2_IO_METHOD_NONE)
    {
        SendErrorEvent(errno, wxT("wxVidCap::init_io() unable to use mmap() or read() to get frames ERROR"));
        return false;
    }

    m_v4l2_device_init |= wxV4L2_DEVICE_INIT_IO;

    //m_v4l2_io_method = wxV4L2_IO_METHOD_READ;
    //m_v4l2_io_method = wxV4L2_IO_METHOD_MMAP;
    //m_v4l2_io_method = wxV4L2_IO_METHOD_USERPTR;

    unsigned int num_bytes = 2*m_v4l2_format.fmt.pix.width; // Buggy driver paranoia.
    num_bytes = wxMax(num_bytes, m_v4l2_format.fmt.pix.bytesperline);
    num_bytes = num_bytes * m_v4l2_format.fmt.pix.height;

    switch (m_v4l2_io_method)
    {
        case wxV4L2_IO_METHOD_READ    : return init_read(num_bytes);
        case wxV4L2_IO_METHOD_MMAP    : return init_mmap();
        case wxV4L2_IO_METHOD_USERPTR : return init_userp(num_bytes);
        default : break;
    }

    return false;
}

bool wxVideoCaptureWindowV4L::uninit_io()
{
    wxCHECK_MSG(IsDeviceConnected(), false, wxT("wxVidCap device closed before uninitializing io"));
    wxCHECK_MSG(wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_IO), false, wxT("wxVidCap attempt to uninit non-initialized device io"));

    if (wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_CAPTURE)) uninit_capture();

    m_v4l2_device_init &= ~wxV4L2_DEVICE_INIT_IO;

    switch (m_v4l2_io_method)
    {
        case wxV4L2_IO_METHOD_READ    : return uninit_read();
        case wxV4L2_IO_METHOD_MMAP    : return uninit_mmap();
        case wxV4L2_IO_METHOD_USERPTR : return uninit_userp();
        default : break;
    }

    return false;
}

bool wxVideoCaptureWindowV4L::init_capture()
{
    wxCHECK_MSG(IsDeviceConnected(), false, wxT("wxVidCap device not open for capture"));
    wxCHECK_MSG(!wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_CAPTURE), false, wxT("wxVidCap device already initialized for capture"));

    switch (m_v4l2_io_method)
    {
        case wxV4L2_IO_METHOD_READ:
            // Nothing to do.
            m_v4l2_device_init |= wxV4L2_DEVICE_INIT_CAPTURE;
            return true;

        case wxV4L2_IO_METHOD_MMAP:
        {
            size_t n, count = m_mem_buffers.size();

            for (n = 0; n < count; ++n)
            {
                memset(&m_v4l2_buffer, 0, sizeof(v4l2_buffer));

                m_v4l2_buffer.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                m_v4l2_buffer.memory      = V4L2_MEMORY_MMAP;
                m_v4l2_buffer.index       = n;

                if (-1 == xioctl (m_fd_device, VIDIOC_QBUF, &m_v4l2_buffer))
                    SendErrorEvent(errno, wxT("wxVidCap::init_capture() mmap VIDIOC_QBUF ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
            }

            m_v4l2_device_init |= wxV4L2_DEVICE_INIT_CAPTURE;

            enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

            if (-1 == xioctl (m_fd_device, VIDIOC_STREAMON, &buf_type))
            {
                SendErrorEvent(errno, wxT("wxVidCap::init_capture() mmap VIDIOC_STREAMON ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
                return false;
            }

            return true;
        }
        case wxV4L2_IO_METHOD_USERPTR:
        {
            size_t n, count = m_mem_buffers.size();

            for (n = 0; n < count; ++n)
            {
                memset(&m_v4l2_buffer, 0, sizeof(v4l2_buffer));

                m_v4l2_buffer.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                m_v4l2_buffer.memory      = V4L2_MEMORY_USERPTR;
                m_v4l2_buffer.index       = n;
                m_v4l2_buffer.m.userptr   = (unsigned long) m_mem_buffers[n].data;
                m_v4l2_buffer.length      = m_mem_buffers[n].length;

                if (-1 == xioctl (m_fd_device, VIDIOC_QBUF, &m_v4l2_buffer))
                    SendErrorEvent(errno, wxT("wxVidCap::init_capture() userp VIDIOC_QBUF ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
            }

            m_v4l2_device_init |= wxV4L2_DEVICE_INIT_CAPTURE;

            enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

            if (-1 == xioctl (m_fd_device, VIDIOC_STREAMON, &buf_type))
            {
                SendErrorEvent(errno, wxT("wxVidCap::init_capture() userp VIDIOC_STREAMON ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
                return false;
            }

            return true;
        }
        default : break;
    }

    return false;
}

bool wxVideoCaptureWindowV4L::uninit_capture()
{
    wxCHECK_MSG(IsDeviceConnected(), false, wxT("wxVidCap device closed before uninitializing capture"));
    wxCHECK_MSG(wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_CAPTURE), false,
                wxT("wxVidCap attempt to uninit non-initialized device capture"));

    m_v4l2_device_init &= ~wxV4L2_DEVICE_INIT_CAPTURE;

    switch (m_v4l2_io_method)
    {
        case wxV4L2_IO_METHOD_READ :
        {
            // Nothing to do
            break;
        }
        case wxV4L2_IO_METHOD_MMAP :
        case wxV4L2_IO_METHOD_USERPTR :
        {
            size_t n, count = m_mem_buffers.size();

            // dequeue all buffers
            for (n = 0; n < count; ++n)
            {
                struct v4l2_buffer buf;
                memset(&buf, 0, sizeof(v4l2_buffer));

                buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory      = V4L2_MEMORY_MMAP;
                buf.index       = n;

                // not an error if it's not queued
                xioctl (m_fd_device, VIDIOC_DQBUF, &buf);
                //if (-1 == xioctl (m_fd_device, VIDIOC_DQBUF, &buf))
                //    perror ("VIDIOC_DQBUF");
            }

            enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

            if (-1 == xioctl (m_fd_device, VIDIOC_STREAMOFF, &buf_type))
                SendErrorEvent(errno, wxT("wxVidCap::uninit_capture() VIDIOC_STREAMOFF ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));

            break;
        }
        default : break;
    }

    return true;
}

bool wxVideoCaptureWindowV4L::init_read(unsigned int num_bytes)
{
    wxCHECK_MSG(m_mem_buffers.size() == 0, false, wxT("Non-empty buffers in init_read"));

    if (!num_bytes) return false;

    m_mem_buffers.push_back(mem_buffer());

    m_mem_buffers[0].data   = malloc(num_bytes);
    m_mem_buffers[0].length = num_bytes;

    if (!m_mem_buffers[0].data)
    {
        fprintf (stderr, "Out of memory\n");
        SendErrorEvent(errno, wxT("wxVidCap::init_read() out of memory ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
        // probably crash...
        return false;
    }

    return true;
}

bool wxVideoCaptureWindowV4L::uninit_read()
{
    wxCHECK_MSG(m_mem_buffers.size() == 1, false, wxT("Buffer count not one in uninit_read"));

    free(m_mem_buffers[0].data);
    m_mem_buffers.clear();

    return true;
}

bool wxVideoCaptureWindowV4L::init_mmap()
{
    wxCHECK_MSG(IsDeviceConnected(), false, wxT("wxVidCap device closed before initializing mmap"));
    wxCHECK_MSG(m_mem_buffers.size() == 0, false, wxT("Non-empty buffers in init_mmap"));

    struct v4l2_requestbuffers requestbuffers;
    memset(&requestbuffers, 0, sizeof(v4l2_requestbuffers));

    requestbuffers.count  = 4;
    requestbuffers.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    requestbuffers.memory = V4L2_MEMORY_MMAP;

    if (-1 == xioctl (m_fd_device, VIDIOC_REQBUFS, &requestbuffers))
    {
        if (EINVAL == errno)
            SendErrorEvent(errno, wxT("wxVidCap::init_mmap() VIDIOC_REQBUFS device does not support mmap ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
        else
            SendErrorEvent(errno, wxT("wxVidCap::init_mmap() VIDIOC_REQBUFS ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));

        return false;
    }

    if (requestbuffers.count < 2)
    {
        fprintf (stderr, "Insufficient buffer memory on %s\n", "device");
        SendErrorEvent(errno, wxT("wxVidCap::init_mmap() insufficient buffer memory ERROR"));
        // probably crash...
    }

    size_t n = 0;

    for (n = 0; n < requestbuffers.count; ++n)
    {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(v4l2_buffer));

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = n;

        if (-1 == xioctl(m_fd_device, VIDIOC_QUERYBUF, &buf))
        {
            SendErrorEvent(errno, wxT("wxVidCap::init_mmap() VIDIOC_QUERYBUF ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
            break;
        }

        m_mem_buffers.push_back(mem_buffer());

        m_mem_buffers[n].length = buf.length;
        m_mem_buffers[n].data  =
                        mmap (NULL,                     // start anywhere
                              buf.length,
                              PROT_READ | PROT_WRITE,   // required
                              MAP_SHARED,               // recommended
                              m_fd_device, buf.m.offset);

        if (MAP_FAILED == m_mem_buffers[n].data)
        {
            SendErrorEvent(errno, wxT("wxVidCap::init_mmap() mmap() MAP_FAILED ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
            break;
        }
    }

    return m_mem_buffers.size() > 0;
}

bool wxVideoCaptureWindowV4L::uninit_mmap()
{
    wxCHECK_MSG(IsDeviceConnected(), false, wxT("wxVidCap device closed before uninitializing mmap"));
    wxCHECK_MSG(m_mem_buffers.size() > 0, false, wxT("Empty buffers in uninit_mmap"));

    bool ok = true;
    size_t n, count = m_mem_buffers.size();

    for (n = 0; n < count; ++n)
    {
        if (-1 == munmap (m_mem_buffers[n].data, m_mem_buffers[n].length))
        {
            ok = false;
            SendErrorEvent(errno, wxT("wxVidCap::uninit_mmap() munmap() ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
        }
    }

    m_mem_buffers.clear();

    // Free all buffers when count == 0
    struct v4l2_requestbuffers requestbuffers;
    memset(&requestbuffers, 0, sizeof(v4l2_requestbuffers));

    requestbuffers.count  = 0;
    requestbuffers.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    requestbuffers.memory = V4L2_MEMORY_MMAP;

    if (-1 == xioctl (m_fd_device, VIDIOC_REQBUFS, &requestbuffers))
    {
        if (EINVAL == errno)
        {
            SendErrorEvent(errno, wxT("wxVidCap::uninit_mmap() VIDIOC_REQBUFS device does not support mmap ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
        }
        else
            SendErrorEvent(errno, wxT("wxVidCap::uninit_mmap() VIDIOC_REQBUFS ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));

        return false;
    }

    return ok;
}

bool wxVideoCaptureWindowV4L::init_userp(unsigned int num_bytes0)
{
    wxCHECK_MSG(IsDeviceConnected(), false, wxT("wxVidCap device closed before initializing userp buffer"));
    wxCHECK_MSG(m_mem_buffers.size() == 0, false, wxT("Non-empty buffers in init_userp"));

    struct v4l2_requestbuffers v4l2_requestbuffers_;
    memset(&v4l2_requestbuffers_, 0, sizeof(v4l2_requestbuffers));

    v4l2_requestbuffers_.count  = 4;
    v4l2_requestbuffers_.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_requestbuffers_.memory = V4L2_MEMORY_USERPTR;

    if (-1 == xioctl (m_fd_device, VIDIOC_REQBUFS, &v4l2_requestbuffers_))
    {
        if (EINVAL == errno)
        {
            SendErrorEvent(errno, wxT("wxVidCap::init_userp() VIDIOC_REQBUFS device does not support user pointer io ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
            return false;
        }
        else
        {
            SendErrorEvent(errno, wxT("wxVidCap::init_userp() VIDIOC_REQBUFS ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
            return false;
        }
    }

    unsigned int page_size = getpagesize ();
    unsigned int num_bytes = (num_bytes0 + page_size - 1) & ~(page_size - 1);

    for (size_t n = 0; n < 4; ++n)
    {
        m_mem_buffers.push_back(mem_buffer());

        m_mem_buffers[n].length = num_bytes;
        m_mem_buffers[n].data   = memalign (/* boundary */ page_size, num_bytes);

        if (!m_mem_buffers[n].data)
        {
            fprintf (stderr, "Out of memory\n");
            SendErrorEvent(errno, wxT("wxVidCap::init_userp() out of memory ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
            return false;
        }
    }

    return true;
}

bool wxVideoCaptureWindowV4L::uninit_userp()
{
    wxCHECK_MSG(IsDeviceConnected(), false, wxT("wxVidCap device closed before uninitializing userp buffer"));
    wxCHECK_MSG(m_mem_buffers.size() > 0, false, wxT("Empty buffers in uninit_userp"));

    for (size_t n = 0; n < m_mem_buffers.size(); ++n)
        free (m_mem_buffers[n].data);

    m_mem_buffers.clear();

    // Free all buffers when count == 0
    struct v4l2_requestbuffers requestbuffers;
    memset(&requestbuffers, 0, sizeof(v4l2_requestbuffers));

    requestbuffers.count  = 0;
    requestbuffers.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    requestbuffers.memory = V4L2_MEMORY_USERPTR;

    if (-1 == xioctl (m_fd_device, VIDIOC_REQBUFS, &requestbuffers))
    {
        if (EINVAL == errno)
        {
            SendErrorEvent(errno, wxT("wxVidCap::uninit_userp() VIDIOC_REQBUFS device does not support user pointer io ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
        }
        else
            SendErrorEvent(errno, wxT("wxVidCap::uninit_userp() VIDIOC_REQBUFS ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));

        return false;
    }

    return true;
}

bool wxVideoCaptureWindowV4L::wait_for_frame(int secs)
{
    if (!IsDeviceConnected()) return false;

    // try a few times
    for (size_t n = 0; n < 10; ++n)
    {
        fd_set fds;
        struct timeval tv;

        FD_ZERO (&fds);
        FD_SET (m_fd_device, &fds);

        // Timeout.
        tv.tv_sec = 2;
        tv.tv_usec = 0;

        int r = select (m_fd_device + 1, &fds, NULL, NULL, &tv);

        if (-1 == r)
        {
            if (EINTR == errno)
                continue;

            SendErrorEvent(errno, wxT("wxVidCap::wait_for_frame() select ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
        }

        if (0 == r)
        {
            // this might not really be an error, but more of a warning.
            fprintf (stderr, "select timeout\n");
            SendErrorEvent(errno, wxT("wxVidCap::wait_for_frame() select timeout ERROR"));
            return false;
        }

        return true;
    }

    return false;
}

bool wxVideoCaptureWindowV4L::read_frame()
{
    wxCHECK_MSG(IsDeviceConnected(), false, wxT("wxVidCap device closed before reading frame"));
    wxCHECK_MSG(wxVC_HASBIT(m_v4l2_device_init, wxV4L2_DEVICE_INIT_CAPTURE), false, wxT("wxVidCap attempt to read frame of non-initialized device"));
    wxCHECK_MSG(m_mem_buffers.size() > 0, false, wxT("wxVidCap empty buffers for read_frame"));

    m_mem_buffer_index = -1; // set to unknown in case of failure

    switch (m_v4l2_io_method)
    {
        case wxV4L2_IO_METHOD_READ :
        {
            if (-1 == read(m_fd_device, m_mem_buffers[0].data, m_mem_buffers[0].length))
            {
                switch (errno)
                {
                    case EAGAIN :
                        return false;

                    case EIO:
                        // Could ignore EIO, see spec.
                        // fall through
                    default:
                        SendErrorEvent(errno, wxT("wxVidCap::read_frame() read ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
                        return false;
                }
            }

            m_mem_buffer_index = 0; // only one buffer

            return true;
        }
        case wxV4L2_IO_METHOD_MMAP :
        {
            // Setup the buffer and VIDIOC_DQBUF, we need to save m_v4l2_buffer
            // for the next time we call VIDIOC_QBUF
            memset(&m_v4l2_buffer, 0, sizeof(v4l2_buffer));

            m_v4l2_buffer.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            m_v4l2_buffer.memory = V4L2_MEMORY_MMAP;

            if (-1 == xioctl (m_fd_device, VIDIOC_DQBUF, &m_v4l2_buffer))
            {
                switch (errno)
                {
                    case EAGAIN:
                        //perror ("VIDIOC_DQBUF 1");
                        // Resource unavailable, try again
                        return false;
                    case EIO:
                        // Could ignore EIO, see spec.
                        // fall through
                    default:
                        SendErrorEvent(errno, wxT("wxVidCap::read_frame() VIDIOC_DQBUF mmap ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
                        return false;
                }
            }

            m_mem_buffer_index = m_v4l2_buffer.index;

            wxCHECK_MSG(m_mem_buffer_index < (int)m_mem_buffers.size(), false, wxT("Error in mmap buffer index"));

            return true;
        }
        case wxV4L2_IO_METHOD_USERPTR :
        {
            // Setup the buffer and VIDIOC_DQBUF, we need to save m_v4l2_buffer
            // for the next time we call VIDIOC_QBUF
            memset(&m_v4l2_buffer, 0, sizeof(v4l2_buffer));

            m_v4l2_buffer.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            m_v4l2_buffer.memory = V4L2_MEMORY_USERPTR;

            if (-1 == xioctl (m_fd_device, VIDIOC_DQBUF, &m_v4l2_buffer))
            {
                switch (errno)
                {
                    case EAGAIN:
                        return 0;
                    case EIO:
                        // Could ignore EIO, see spec.
                        // fall through
                    default:
                        SendErrorEvent(errno, wxT("wxVidCap::read_frame() VIDIOC_DQBUF userp ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
                        return false;
                }
            }

            // find what buffer was filled
            size_t n, count = m_mem_buffers.size();
            for (n = 0; n < count; ++n)
            {
                if ((m_v4l2_buffer.m.userptr == (unsigned long) m_mem_buffers[n].data) &&
                    (m_v4l2_buffer.length == m_mem_buffers[n].length))
                {
                    m_mem_buffer_index = n;
                    break;
                }
            }

            wxCHECK_MSG(n < count, false, wxT("Error finding user buffer"));

            return true;
        }
        default : break;
    }

    return false;
}

// static
int wxVideoCaptureWindowV4L::xioctl(int fd, int request, void *arg)
{
    int r = 0;

    do
    {
        r = ioctl (fd, request, arg);
    }
    while ((-1 == r) && (EINTR == errno)); // if interrupted, try again

    return r;
}

// ----------------------------------------------------------------------------
// Get v4l structs

int wxVideoCaptureWindowV4L::Get_v4l2_capability()
{
    memset(&m_v4l2_capability, 0, sizeof(v4l2_capability));
    int ret = xioctl(m_fd_device, VIDIOC_QUERYCAP, &m_v4l2_capability);
    if (ret != 0)
    {
        if (errno == EINVAL)
            SendErrorEvent(errno, wxT("wxVidCap::Get_v4l2_capability() VIDIOC_QUERYCAP not a video device ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
        else
            SendErrorEvent(errno, wxT("wxVidCap::Get_v4l2_capability() VIDIOC_QUERYCAP ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
    }

    return ret;
}

int wxVideoCaptureWindowV4L::Get_v4l2_fmtdesc()
{
    memset(&m_v4l2_fmtdesc, 0, sizeof(v4l2_fmtdesc));

    m_v4l2_fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    int ret = ioctl(m_fd_device, VIDIOC_ENUM_FMT, &m_v4l2_fmtdesc);
    if (ret != 0)
        SendErrorEvent(errno, wxT("wxVidCap::Get_v4l2_fmtdesc() VIDIOC_ENUM_FMT ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));

    return ret;
}

int wxVideoCaptureWindowV4L::Get_v4l2_format()
{
    memset(&m_v4l2_format, 0, sizeof(v4l2_format));

    m_v4l2_format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    int ret = ioctl(m_fd_device, VIDIOC_G_FMT, &m_v4l2_format);
    if (ret != 0)
        SendErrorEvent(errno, wxT("wxVidCap::Get_v4l2_format() VIDIOC_G_FMT ERROR : ") + CHAR_TO_WXSTR(strerror(errno)));
    else
        m_imageSize = wxSize(m_v4l2_format.fmt.pix.width, m_v4l2_format.fmt.pix.height);

    return ret;
}

int wxVideoCaptureWindowV4L::Get_device_v4l2_fmtdescs()
{
    m_device_v4l2_fmtdescs.clear();

    int index = 0;
    int ret = 0;
    while (ret == 0)
    {
        v4l2_fmtdesc v4l2_fmtdesc_;
        memset(&v4l2_fmtdesc_, 0, sizeof(v4l2_fmtdesc));

        v4l2_fmtdesc_.index = index;
        v4l2_fmtdesc_.type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        ret = ioctl(m_fd_device, VIDIOC_ENUM_FMT, &v4l2_fmtdesc_);

        if (ret == 0)
        {
            m_device_v4l2_fmtdescs.push_back(v4l2_fmtdesc_);
            memcpy(&m_device_v4l2_fmtdescs[index], &v4l2_fmtdesc_, sizeof(v4l2_fmtdesc));
        }
        // else not an error when we reach the end of the list

        index++;
    }

    return 0;
}

int wxVideoCaptureWindowV4L::Get_device_v4l2_frmsizeenums()
{
    m_device_v4l2_frmsizeenums.clear();

    int index = 0;
    int ret = 0;
    while (ret == 0)
    {
        v4l2_frmsizeenum v4l2_frmsizeenum_;
        memset(&v4l2_frmsizeenum_, 0, sizeof(v4l2_frmsizeenum));

        v4l2_frmsizeenum_.index        = index;
        v4l2_frmsizeenum_.pixel_format = m_v4l2_fmtdesc.pixelformat;

        ret = ioctl(m_fd_device, VIDIOC_ENUM_FRAMESIZES, &v4l2_frmsizeenum_);

        if (ret == 0)
        {
            m_device_v4l2_frmsizeenums.push_back(v4l2_frmsizeenum_);
            memcpy(&m_device_v4l2_frmsizeenums[index], &v4l2_frmsizeenum_, sizeof(v4l2_frmsizeenum));
        }
        // else not an error when we reach the end of the list

        index++;
    }

    return 0;
}

int wxVideoCaptureWindowV4L::Get_device_v4l2_frmivalenums()
{
    m_device_v4l2_frmivalenums.clear();

    int index = 0;
    int ret = 0;
    while (ret == 0)
    {
        v4l2_frmivalenum v4l2_frmivalenum_;
        memset(&v4l2_frmivalenum_, 0, sizeof(v4l2_frmivalenum));

        v4l2_frmivalenum_.index        = index;
        v4l2_frmivalenum_.pixel_format = m_v4l2_fmtdesc.pixelformat;
        v4l2_frmivalenum_.width        = m_v4l2_format.fmt.pix.width;
        v4l2_frmivalenum_.height       = m_v4l2_format.fmt.pix.height;

        ret = ioctl(m_fd_device, VIDIOC_ENUM_FRAMEINTERVALS, &v4l2_frmivalenum_);

        if (ret == 0)
        {
            m_device_v4l2_frmivalenums.push_back(v4l2_frmivalenum_);
            memcpy(&m_device_v4l2_frmivalenums[index], &v4l2_frmivalenum_, sizeof(v4l2_frmivalenum));
        }
        // else not an error when we reach the end of the list

        index++;
    }

    return 0;
}

// --------------------------------------------------------------------------
// print methods for ioctl structs
// --------------------------------------------------------------------------

wxString wxVideoCaptureWindowV4L::Get_v4l2_capability_String(const v4l2_capability& v4l2_capability_) const
{
    std::stringstream ss;

    ss << "v4l2_capability" << std::endl;

    ss << "  driver                     : " << v4l2_capability_.driver       << std::endl;
    ss << "  card                       : " << v4l2_capability_.card         << std::endl;
    ss << "  bus_info                   : " << v4l2_capability_.bus_info     << std::endl;
    ss << "  version                    : " << v4l2_capability_.version      << std::endl;
    ss << "  capabilities               : " << v4l2_capability_.capabilities << std::endl;

    ss << "    [ Is a video capture device:      | " << ((v4l2_capability_.capabilities & V4L2_CAP_VIDEO_CAPTURE)       ? "X" : " ") << " ]" << std::endl;
    ss << "    [ Is a video output device:       | " << ((v4l2_capability_.capabilities & V4L2_CAP_VIDEO_OUTPUT)        ? "X" : " ") << " ]" << std::endl;
    ss << "    [ Can do video overlay:           | " << ((v4l2_capability_.capabilities & V4L2_CAP_VIDEO_OVERLAY)       ? "X" : " ") << " ]" << std::endl;
    ss << "    [ Is a raw VBI capture device:    | " << ((v4l2_capability_.capabilities & V4L2_CAP_VBI_CAPTURE)         ? "X" : " ") << " ]" << std::endl;
    ss << "    [ Is a raw VBI output device:     | " << ((v4l2_capability_.capabilities & V4L2_CAP_VBI_OUTPUT)          ? "X" : " ") << " ]" << std::endl;
    ss << "    [ Is a sliced VBI capture device: | " << ((v4l2_capability_.capabilities & V4L2_CAP_SLICED_VBI_CAPTURE)  ? "X" : " ") << " ]" << std::endl;
    ss << "    [ Is a sliced VBI output device:  | " << ((v4l2_capability_.capabilities & V4L2_CAP_SLICED_VBI_OUTPUT)   ? "X" : " ") << " ]" << std::endl;
    ss << "    [ RDS data capture:               | " << ((v4l2_capability_.capabilities & V4L2_CAP_RDS_CAPTURE)         ? "X" : " ") << " ]" << std::endl;
    ss << "    [ Can do video output overlay:    | " << ((v4l2_capability_.capabilities & V4L2_CAP_VIDEO_OUTPUT_OVERLAY)? "X" : " ") << " ]" << std::endl;
    ss << "    [ Can do hardware frequency seek: | " << ((v4l2_capability_.capabilities & V4L2_CAP_HW_FREQ_SEEK)        ? "X" : " ") << " ]" << std::endl;
    ss << "    [ Is an RDS encoder:              | " << ((v4l2_capability_.capabilities & V4L2_CAP_RDS_OUTPUT)          ? "X" : " ") << " ]" << std::endl;
    ss << "    [ has a tuner:                    | " << ((v4l2_capability_.capabilities & V4L2_CAP_TUNER)               ? "X" : " ") << " ]" << std::endl;
    ss << "    [ has audio support:              | " << ((v4l2_capability_.capabilities & V4L2_CAP_AUDIO)               ? "X" : " ") << " ]" << std::endl;
    ss << "    [ is a radio device:              | " << ((v4l2_capability_.capabilities & V4L2_CAP_RADIO)               ? "X" : " ") << " ]" << std::endl;
    ss << "    [ has a modulator:                | " << ((v4l2_capability_.capabilities & V4L2_CAP_MODULATOR)           ? "X" : " ") << " ]" << std::endl;
    ss << "    [ read/write systemcalls:         | " << ((v4l2_capability_.capabilities & V4L2_CAP_READWRITE)           ? "X" : " ") << " ]" << std::endl;
    ss << "    [ async I/O:                      | " << ((v4l2_capability_.capabilities & V4L2_CAP_ASYNCIO)             ? "X" : " ") << " ]" << std::endl;
    ss << "    [ streaming I/O ioctls:           | " << ((v4l2_capability_.capabilities & V4L2_CAP_STREAMING)           ? "X" : " ") << " ]" << std::endl;

    //ss << "  reserved  : " << v4l2_capability_.reserved  << std::endl;

    return STDSTR_TO_WXSTR(ss.str());
}

wxString wxVideoCaptureWindowV4L::Get_v4l2_fmtdesc_String(const v4l2_fmtdesc& v4l2_fmtdesc_) const
{
    std::stringstream ss;

    ss << "v4l2_fmtdesc" << std::endl;

    ss << "  index                      : " << v4l2_fmtdesc_.index        << std::endl;
    ss << "  type                       : " << v4l2_fmtdesc_.type         << std::endl;
    ss << "  flags                      : " << v4l2_fmtdesc_.flags        << std::endl;
    ss << "  description                : " << v4l2_fmtdesc_.description  << std::endl;
    ss << "  pixelformat                : " << v4l2_fmtdesc_.pixelformat  << " FOURCC = " << FOURCCTowxString(v4l2_fmtdesc_.pixelformat) << std::endl;
    //ss << "  reserved                 : " << v4l2_fmtdesc_.reserved  << std::endl;

    return STDSTR_TO_WXSTR(ss.str());
}

wxString wxVideoCaptureWindowV4L::Get_v4l2_format_String(const v4l2_format& v4l2_format_) const
{
    std::stringstream ss;

    ss << "v4l2_format" << std::endl;

    ss << "  type                       : " << v4l2_format_.type          << std::endl;

    if (v4l2_format_.type == V4L2_BUF_TYPE_VIDEO_CAPTURE)
        ss << Get_v4l2_pix_format_String(v4l2_format_.fmt.pix);

    return STDSTR_TO_WXSTR(ss.str());
}

wxString wxVideoCaptureWindowV4L::Get_v4l2_pix_format_String(const v4l2_pix_format& v4l2_pix_format_) const
{
    std::stringstream ss;

    ss << "v4l2_pix_format" << std::endl;

    ss << "  width,height               : " << v4l2_pix_format_.width << ", " << v4l2_pix_format_.height << std::endl;
    ss << "  pixelformat                : " << v4l2_pix_format_.pixelformat  << " FOURCC = " << FOURCCTowxString(v4l2_pix_format_.pixelformat) << std::endl;
    ss << "  field                      : " << v4l2_pix_format_.field        << std::endl;
    ss << "  bytesperline               : " << v4l2_pix_format_.bytesperline << std::endl;
    ss << "  sizeimage                  : " << v4l2_pix_format_.sizeimage    << std::endl;
    ss << "  colorspace                 : " << v4l2_pix_format_.colorspace   << std::endl;
    ss << "  priv                       : " << v4l2_pix_format_.priv         << std::endl;

    return STDSTR_TO_WXSTR(ss.str());
}

wxString wxVideoCaptureWindowV4L::Get_v4l2_frmsizeenum_String(const v4l2_frmsizeenum& v4l2_frmsizeenum_) const
{
    std::stringstream ss;

    ss << "v4l2_frmsizeenum" << std::endl;

    ss << "  index                      : " << v4l2_frmsizeenum_.index << std::endl;
    ss << "  pixel_format               : " << v4l2_frmsizeenum_.pixel_format  << " FOURCC = " << FOURCCTowxString(v4l2_frmsizeenum_.pixel_format) << std::endl;
    ss << "  type                       : " << v4l2_frmsizeenum_.type          << std::endl;

    if (v4l2_frmsizeenum_.type == V4L2_FRMSIZE_TYPE_DISCRETE)
    {
    ss << "  v4l2_frmsize_discrete        " << std::endl;
    ss << "    width,height             : " << v4l2_frmsizeenum_.discrete.width << ", " << v4l2_frmsizeenum_.discrete.height << std::endl;
    }
    else //if (v4l2_frmsizeenum_.type == V4L2_FRMSIZE_TYPE_STEPWISE) ? V4L2_FRMSIZE_TYPE_CONTINUOUS
    {
    ss << "  v4l2_frmsize_stepwise        " << std::endl;
    ss << "    min_width,min_height     : " << v4l2_frmsizeenum_.stepwise.min_width  << ", " << v4l2_frmsizeenum_.stepwise.min_height  << std::endl;
    ss << "    max_width,max_height     : " << v4l2_frmsizeenum_.stepwise.max_width  << ", " << v4l2_frmsizeenum_.stepwise.max_height  << std::endl;
    ss << "    step_width,step_height   : " << v4l2_frmsizeenum_.stepwise.step_width << ", " << v4l2_frmsizeenum_.stepwise.step_height << std::endl;
    }

    return STDSTR_TO_WXSTR(ss.str());
}

wxString wxVideoCaptureWindowV4L::Get_v4l2_frmivalenum_String(const v4l2_frmivalenum& v4l2_frmivalenum_) const
{
    std::stringstream ss;

    ss << "v4l2_frmivalenum" << std::endl;

    ss << "  index                      : " << v4l2_frmivalenum_.index << std::endl;
    ss << "  pixel_format               : " << v4l2_frmivalenum_.pixel_format  << " FOURCC = " << FOURCCTowxString(v4l2_frmivalenum_.pixel_format) << std::endl;
    ss << "  width,height               : " << v4l2_frmivalenum_.width << ", " << v4l2_frmivalenum_.height << std::endl;
    ss << "  type                       : " << v4l2_frmivalenum_.type          << std::endl;

    if (v4l2_frmivalenum_.type == V4L2_FRMIVAL_TYPE_DISCRETE)
    {
    ss << "  v4l2_fract discrete" << std::endl;
    ss << "  interval a/b (sec)         : " << v4l2_frmivalenum_.discrete.numerator  << ", " << v4l2_frmivalenum_.discrete.denominator << std::endl;
    }
    else //if (v4l2_frmivalenum_.type == V4L2_FRMIVAL_TYPE_STEPWISE) ? V4L2_FRMIVAL_TYPE_CONTINUOUS
    {
    ss << "  v4l2_frmival_stepwise" << std::endl;
    ss << "    min  interval a/b (sec)  : " << v4l2_frmivalenum_.stepwise.min.numerator  << ", " << v4l2_frmivalenum_.stepwise.min.denominator << std::endl;
    ss << "    max  interval a/b (sec)  : " << v4l2_frmivalenum_.stepwise.max.numerator  << ", " << v4l2_frmivalenum_.stepwise.max.denominator << std::endl;
    ss << "    step interval a/b (sec)  : " << v4l2_frmivalenum_.stepwise.step.numerator << ", " << v4l2_frmivalenum_.stepwise.step.denominator << std::endl;
    }

    return STDSTR_TO_WXSTR(ss.str());
}

#endif // !defined(__WXMSW__)
