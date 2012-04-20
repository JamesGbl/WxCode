//TODO:
//

/////////////////////////////////////////////////////////////////////////////
// Name:        vcap_v4l.cpp - wxVideoCaptureWindow using Linux V4L2 API
// Author:      John Labenski
// Created:     07/06/01
// Modified:    01/14/03
// Copyright:   John Labenski
// License:     wxWidgets V2.0
/////////////////////////////////////////////////////////////////////////////

/*
VIDEO_PALETTE_GREY
VIDEO_PALETTE_HI240
VIDEO_PALETTE_RGB565
VIDEO_PALETTE_RGB24
VIDEO_PALETTE_RGB32
VIDEO_PALETTE_RGB555
VIDEO_PALETTE_YUV422
VIDEO_PALETTE_YUYV
VIDEO_PALETTE_RAW
VIDEO_PALETTE_YUV422P
VIDEO_PALETTE_YUV411P
VIDEO_PALETTE_YUV420P
VIDEO_PALETTE_YUV410P
*/

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "precomp.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#if !defined(__WXMSW__)

#include <wx/clipbrd.h>
#include <wx/file.h>
#include <wx/dir.h>
#include "wx/mstream.h"

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

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <map>


//----------------------------------------------------------------------------
// wxVideoCaptureWindow #defines and globals
//----------------------------------------------------------------------------

// id's for the dialogs, only one at a time please
enum
{
    IDD_wxVIDCAP_CAPSNGFRAMESDLG = 10,
    IDD_wxVIDCAP_CAPPREFDLG,
    IDD_wxVIDCAP_VIDEOFORMATDLG,
    IDD_wxVIDCAP_AUDIOFORMATDLG,

    IDD_wxVIDCAP_PREVIEW_WXIMAGE_TIMER = 511
};

//------------------------------------------------------------------------------
// wxVideoCaptureWindow
//------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(wxVideoCaptureWindowV4L, wxVideoCaptureWindowBase)

BEGIN_EVENT_TABLE(wxVideoCaptureWindowV4L, wxVideoCaptureWindowBase)
    EVT_MOVE(                                     wxVideoCaptureWindowV4L::OnMove)
    EVT_PAINT(                                    wxVideoCaptureWindowV4L::OnDraw)
    EVT_SCROLLWIN(                                wxVideoCaptureWindowV4L::OnScrollWin)
    EVT_TIMER(IDD_wxVIDCAP_PREVIEW_WXIMAGE_TIMER, wxVideoCaptureWindowV4L::OnPreviewwxImageTimer)
    EVT_IDLE(                                     wxVideoCaptureWindowV4L::OnIdle)
    EVT_CLOSE(                                    wxVideoCaptureWindowV4L::OnCloseWindow)
END_EVENT_TABLE()

void wxVideoCaptureWindowV4L::Init()
{
    m_fd_device       = -1;
    m_preview_wximage = false;

    Init_V4L2_structs();
}

void wxVideoCaptureWindowV4L::Init_V4L2_structs()
{
    memset(&m_v4l2_capability,  0, sizeof(v4l2_capability));
    memset(&m_v4l2_fmtdesc,     0, sizeof(v4l2_fmtdesc));
    memset(&m_v4l2_format,      0, sizeof(v4l2_format));
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
    if (GetDeviceCount() > 0)
        DeviceConnect(0);

    Print_v4l2_capability(m_v4l2_capability);
    Print_v4l2_fmtdesc(m_v4l2_fmtdesc);
    Print_v4l2_format(m_v4l2_format);

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

void wxVideoCaptureWindowV4L::OnIdle( wxIdleEvent &event )
{
    if (m_preview_wximage && (m_mmap_buffers.size() > 0))
    {
        event.RequestMore();

        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(v4l2_buffer));

        buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;

        if (-1 == xioctl (m_fd_device, VIDIOC_DQBUF, &buf))
        {
            switch (errno)
            {
                case EAGAIN:
                    // Resource unavailable, try again
                    return;
                case EIO:
                    // Could ignore EIO, see spec.
                    // fall through
                default:
                    perror ("VIDIOC_DQBUF");
                    return;
            }
        }

        //assert ();
        wxCHECK_RET(buf.index < m_mmap_buffers.size(), wxT("Invalid buffer index."));

        //process_image (buffers[buf.index].start);

        if ( m_v4l2_fmtdesc.pixelformat == 1195724874) // FOURCC = "JPEG"
        {
            wxMemoryInputStream is(m_mmap_buffers[buf.index].start, m_mmap_buffers[buf.index].length);
            m_wximage = wxImage(is, wxBITMAP_TYPE_JPEG);
        }
        else if ((m_imageSize.GetWidth() > 0) && (m_imageSize.GetHeight() > 0))
        {
            // Just show whatever garbage is in memory
            if (!m_wximage.IsOk() ||
                (m_wximage.GetWidth()  != m_imageSize.GetWidth()) ||
                (m_wximage.GetHeight() != m_imageSize.GetHeight()))
            {
                m_wximage.Create(m_imageSize.GetWidth(), m_imageSize.GetHeight());
            }
            //Y8_to_RGB24((void*)m_map, (void*)m_wximage.GetData(), m_video_mmap.width, m_video_mmap.height);
            memcpy( m_wximage.GetData(), m_mmap_buffers[buf.index].start, wxMin(m_mmap_buffers[buf.index].length, 3*m_imageSize.GetWidth()*m_imageSize.GetHeight()));
        }

        Refresh(false);

        if (-1 == xioctl (m_fd_device, VIDIOC_QBUF, &buf))
                perror("VIDIOC_QBUF");
    }

    event.Skip();
}

void wxVideoCaptureWindowV4L::DoSetSize(int x, int y, int width, int height,
                                        int sizeFlags)
{
    wxVideoCaptureWindowBase::DoSetSize(x, y, width, height, sizeFlags);
    DoSizeWindow();
}

// do the window resizing, call whenever video format changes
void wxVideoCaptureWindowV4L::DoSizeWindow()
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
}

void wxVideoCaptureWindowV4L::OnMove( wxMoveEvent &event )
{
    event.Skip();
}

void wxVideoCaptureWindowV4L::OnScrollWin( wxScrollWinEvent &event )
{
    event.Skip();
}

void wxVideoCaptureWindowV4L::OnDraw( wxPaintEvent &event )
{
    wxPaintDC dc(this);
    PrepareDC( dc );

    if (m_wximage.Ok())// && !m_getting_wximage && m_preview_wximage)
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

// get all the device versions and names, used internally
void wxVideoCaptureWindowV4L::EnumerateDevices()
{
    m_deviceNames.Clear();
    m_deviceVersions.Clear();
    m_deviceFilenames.Clear();
    m_v4l2_capability_vector.clear();

    //wxString rootDir(wxT("/dev/v4l/by-id"));
    wxString rootDir(wxT("/dev/"));
    wxString filename;

    wxDir dir(rootDir);

    if (!dir.IsOpened())
    {
        wxMessageBox(wxT("Unable to open video devices in : ") + rootDir,
                     wxT("Error opening video devices"),
                     wxOK, this);
        return;
    }
    bool has_files = dir.GetFirst(&filename, wxT("video*"), wxDIR_FILES);

    while (has_files)
    {
        wxString errMsg; // use this as a sink for the error messages
        wxString video_filename(rootDir+filename);
        int fd_device = open_device(rootDir+filename, &errMsg);

        //wxPrintf(wxT("Enumerating [%s] %d\n"), video_filename.c_str(), fd_device); fflush(stdout);

        if (fd_device != -1)
        {
            //wxPrintf(wxT("Enumerating [%s] %d\n"), video_filename.c_str(), (int)a); fflush(stdout);

            struct v4l2_capability v4l2_capability_;
            memset(&v4l2_capability_, 0, sizeof(v4l2_capability));
            bool ok = (0==ioctl(fd_device, VIDIOC_QUERYCAP, &v4l2_capability_));

            if (ok && (v4l2_capability_.capabilities & V4L2_CAP_VIDEO_CAPTURE))
            {
                m_v4l2_capability_vector.push_back(v4l2_capability_);
                memcpy(&m_v4l2_capability_vector[m_v4l2_capability_vector.size()-1], &v4l2_capability_, sizeof(v4l2_capability));

                wxString deviceName = wxConvUTF8.cMB2WX((const char*)v4l2_capability_.card);
                //printf("Enumerating [%s]\n", v4l2_capability_.card); fflush(stdout);
                m_deviceNames.Add(deviceName);
                m_deviceVersions.Add(wxT("Unknown version"));
                m_deviceFilenames.Add(rootDir+filename);
            }
            //else
            //    perror("Error enumerating video device");

            close_device(fd_device);
        }

        has_files = dir.GetNext(&filename);
    }
}

// ----------------------------------------------------------------------
// Connect and Disconnect to device
// ----------------------------------------------------------------------

bool wxVideoCaptureWindowV4L::DeviceConnect(int index)
{
    wxCHECK_MSG((index >= 0) && (index < GetDeviceCount()), false, wxT("invalid device index"));

    m_imageSize = wxSize(0, 0);

    // Close it before
    if (m_fd_device != -1)
    {
        munmap_mem();
        close_device(m_fd_device);
        Init_V4L2_structs();
    }

    m_fd_device = open_device(m_deviceFilenames[index]);

    if (m_fd_device > -1)
    {
        m_deviceIndex = index;

        Get_v4l2_capability();
        Get_v4l2_fmtdesc();
        Get_v4l2_fmtdesc_vector();
        Get_v4l2_format();

        if (m_v4l2_format.type == V4L2_BUF_TYPE_VIDEO_CAPTURE)
            m_imageSize = wxSize(m_v4l2_format.fmt.pix.width, m_v4l2_format.fmt.pix.height);

/*
        // ------------------------------------------------------------------
        // Setup cropping
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
*/

        // ------------------------------------------------------------------

        mmap_mem();
    }

    return true;
}

bool wxVideoCaptureWindowV4L::DeviceDisconnect()
{
    m_preview_wximage = false;
    m_deviceIndex = -1;

    munmap_mem();
    close_device(m_fd_device);
    Init_V4L2_structs();

    return true;
}

// ----------------------------------------------------------------------
// Display device's dialogs to set video characteristics
// ----------------------------------------------------------------------

void wxVideoCaptureWindowV4L::VideoCustomFormatDialog()
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

void wxVideoCaptureWindowV4L::PropertiesDialog()
{
    wxDialog *dialog = new wxDialog(this, -1, wxT("wxVideoCaptureWindow Properties"),
                                    wxDefaultPosition, wxDefaultSize,
                                    wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);

    wxBoxSizer *dialogsizer = new wxBoxSizer( wxVERTICAL );
    wxTextCtrl *textctrl = new wxTextCtrl( dialog, -1, wxT(""),
                                           wxDefaultPosition, wxSize(300,400),
                                           wxTE_MULTILINE|wxTE_READONLY);

    dialogsizer->Add( textctrl, 1, wxEXPAND);

    dialog->SetAutoLayout(true);
    dialog->SetSizer(dialogsizer);
    dialogsizer->Fit(dialog);
    dialogsizer->SetSizeHints(dialog);

    wxString wxstr;
    int i;
    textctrl->AppendText(wxT("Detected Devices:\n"));
    textctrl->AppendText(wxString::Format(wxT("GetDeviceCount() : %d\n\n"), GetDeviceCount()));
    for (i=0; i<GetDeviceCount(); i++)
    {
        textctrl->AppendText(GetDeviceName(i)+wxT("\n"));
        textctrl->AppendText(GetDeviceVersion(i)+wxT("\n\n"));
    }

    textctrl->AppendText(wxT("Currently connected device:\n"));
    textctrl->AppendText(wxString::Format(wxT("GetDeviceIndex(): %d\n\n"), GetDeviceIndex()));
    textctrl->AppendText(GetDeviceName()+wxT("\n"));
    textctrl->AppendText(GetDeviceVersion()+wxT("\n\n"));

    textctrl->AppendText(wxString::Format(wxT("Video device properties:\n\n")));

    textctrl->AppendText(wxString::Format(wxT("IsDeviceConnected() : %d\n"), IsDeviceConnected()));
    textctrl->AppendText(wxString::Format(wxT("IsDeviceInitialized() : %d\n\n"), IsDeviceInitialized()));

    textctrl->AppendText(wxString::Format(wxT("HasVideoSourceDialog() : %d\n"), HasVideoSourceDialog()));
    textctrl->AppendText(wxString::Format(wxT("HasVideoFormatDialog() : %d\n"), HasVideoFormatDialog()));
    textctrl->AppendText(wxString::Format(wxT("HasVideoDisplayDialog() : %d\n\n"), HasVideoDisplayDialog()));

    textctrl->AppendText(wxString::Format(wxT("Video properties:\n\n")));

    textctrl->AppendText(wxString::Format(wxT("GetImageWidth() x GetImageHeight() : %d x %d\n\n"), GetImageWidth(), GetImageHeight()));

    int width, height, bpp;
    FOURCC fourcc;
    GetVideoFormat( &width, &height, &bpp, &fourcc );
    wxString fourccStr(FOURCCTowxString(fourcc));

    textctrl->AppendText(wxString::Format(wxT("GetVideoFormat() -> width x height : %d x %d\n"), width, height));
    textctrl->AppendText(wxString::Format(wxT("GetVideoFormat() -> bits per pixel : %d \n"), bpp));
    textctrl->AppendText(wxString::Format(wxT("GetVideoFormat() -> compression : %s \n\n"), fourccStr.c_str()));

    textctrl->AppendText(wxString::Format(wxT("Video preview properties:\n\n")));

    textctrl->AppendText(wxString::Format(wxT("IsPreviewing() : %d\n"), IsPreviewing()));
    textctrl->AppendText(wxString::Format(wxT("IsPreviewScaled() : %d\n"), IsPreviewScaled()));
    textctrl->AppendText(wxString::Format(wxT("GetPreviewRateMS() : %d\n"), GetPreviewRateMS()));

    textctrl->AppendText(wxString::Format(wxT("HasOverlay() : %d\n"), HasOverlay()));
    textctrl->AppendText(wxString::Format(wxT("IsOverlaying() : %d\n\n"), IsOverlaying()));

    textctrl->AppendText(wxString::Format(wxT("Palette properties:\n\n")));

    textctrl->AppendText(wxString::Format(wxT("DriverSuppliesPalettes() : %d\n"), DriverSuppliesPalettes()));
    textctrl->AppendText(wxString::Format(wxT("IsUsingDefaultPalette() : %d\n"), IsUsingDefaultPalette()));

    dialog->ShowModal();
}

wxString wxVideoCaptureWindowV4L::GetPropertiesString()
{
    return wxString();
}

// ----------------------------------------------------------------------
// Video characteristics and manipulation
// ----------------------------------------------------------------------

bool wxVideoCaptureWindowV4L::GetVideoFormat(int *width, int *height,
                                             int *bpp, FOURCC *fourcc)
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
    v4l2_format v4l2_format_;

    memset(&v4l2_format_, 0, sizeof(v4l2_format));

    v4l2_format_.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    int ret = ioctl(m_fd_device, VIDIOC_G_FMT, &v4l2_format_);

    v4l2_format_.fmt.pix.width  = width;
    v4l2_format_.fmt.pix.height = height;
    v4l2_format_.fmt.pix.pixelformat = format;

    v4l2_format_.fmt.pix.colorspace = V4L2_COLORSPACE_SRGB;

    ret = ioctl(m_fd_device, VIDIOC_TRY_FMT, &v4l2_format_);
    if (ret != 0) perror("wxVidCap::SetVideoFormat() VIDIOC_TRY_FMT ERROR : ");

    if (ret == 0)
    {
        ret = ioctl(m_fd_device, VIDIOC_S_FMT, &v4l2_format_);
        if (ret != 0) perror("wxVidCap::SetVideoFormat() VIDIOC_S_FMT ERROR : ");
    }

    Get_v4l2_format();
    Print_v4l2_format(m_v4l2_format);

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

//    if (onoff && (m_map == MAP_FAILED))
//    {
//        m_map_size = m_video_mmap.width*m_video_mmap.height*1;
//    m_map=mmap(0, m_map_size, PROT_READ, MAP_SHARED, m_fd_device, 0);   // select memory-map
//      if(m_map==MAP_FAILED) perror("mmap");
//      }


    return false;
}

void wxVideoCaptureWindowV4L::OnPreviewwxImageTimer(wxTimerEvent& event)
{
    event.Skip();
}

// ----------------------------------------------------------------------
// Capture single frames, take snapshots of streaming video
// ----------------------------------------------------------------------

bool wxVideoCaptureWindowV4L::SnapshotToWindow()
{
    return false;
}

bool wxVideoCaptureWindowV4L::SnapshotToClipboard()
{
    return false;
}

bool wxVideoCaptureWindowV4L::SnapshotToBMP( const wxString &filename )
{
    return 0;
}

bool wxVideoCaptureWindowV4L::SnapshotTowxImage( wxImage &image )
{
    return false;
}

bool wxVideoCaptureWindowV4L::SnapshotTowxImage()
{
    return false;
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

// ----------------------------------------------------------------------------
// Open/Close the /dev/video device

int wxVideoCaptureWindowV4L::open_device(const wxString &filename, wxString* errMsg_) const
{
    wxWindow* parent = (wxWindow*)this; // unconst for use in wxMessageBox

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
                         wxOK | wxCENTRE | wxICON_ERROR, parent);
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
                         wxOK | wxCENTRE | wxICON_ERROR, parent);
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
                         wxOK | wxCENTRE | wxICON_ERROR, parent);
        }

        //fprintf (stderr, "Cannot open '%s': %d, %s\n",
        //         dev_name, errno, strerror (errno));
        return fd_device;
    }

    return fd_device;
}
bool wxVideoCaptureWindowV4L::close_device(int fd_device) const
{
    if (fd_device == -1) return false;

    int ret = close(fd_device);
    //printf("Closed device # %d : result %d\n", fd_device, ret);
    return ret == 0;
}

bool wxVideoCaptureWindowV4L::mmap_mem()
{
    munmap_mem();

    struct v4l2_requestbuffers requestbuffers;
    memset(&requestbuffers, 0, sizeof(v4l2_requestbuffers));

    requestbuffers.count  = 4;
    requestbuffers.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    requestbuffers.memory = V4L2_MEMORY_MMAP;

    if (-1 == xioctl (m_fd_device, VIDIOC_REQBUFS, &requestbuffers))
    {
        if (EINVAL == errno)
        {
            perror("VIDIOC_REQBUFS no mem mapping");
            //fprintf (stderr, "%s does not support "
            //                "memory mapping\n", dev_name);
        }
        else
        {
            perror("VIDIOC_REQBUFS");
        }

        return false;
    }

    if (requestbuffers.count < 2)
    {
    //    fprintf (stderr, "Insufficient buffer memory on %s\n", dev_name);
    }

    for (size_t n_buffers = 0; n_buffers < requestbuffers.count; ++n_buffers)
    {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(v4l2_buffer));

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = n_buffers;

        if (-1 == xioctl(m_fd_device, VIDIOC_QUERYBUF, &buf))
        {
            perror("VIDIOC_QUERYBUF");
            break;
        }

        m_mmap_buffers.push_back(mmap_buffer());

        m_mmap_buffers[n_buffers].length = buf.length;
        m_mmap_buffers[n_buffers].start =
                        mmap (NULL,                     // start anywhere
                              buf.length,
                              PROT_READ | PROT_WRITE,   // required
                              MAP_SHARED,               // recommended
                              m_fd_device, buf.m.offset);

        if (MAP_FAILED == m_mmap_buffers[n_buffers].start)
        {
            perror("mmap");
            break;
        }
    }


    for (size_t i = 0; i < requestbuffers.count; ++i)
    {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(v4l2_buffer));

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = i;

        if (-1 == xioctl (m_fd_device, VIDIOC_QBUF, &buf))
            perror ("VIDIOC_QBUF");
    }

    enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (-1 == xioctl (m_fd_device, VIDIOC_STREAMON, &buf_type))
        perror ("VIDIOC_STREAMON");

    return m_mmap_buffers.size() > 0;
}

bool wxVideoCaptureWindowV4L::munmap_mem()
{
    size_t n, count = m_mmap_buffers.size();
    bool ok = true;

    for (n = 0; n < count; ++n)
    {
        if (-1 == munmap (m_mmap_buffers[n].start, m_mmap_buffers[n].length))
        {
            ok = false;
            perror ("wxVidCap::munmap_mem() ERROR : ");
        }
    }

    m_mmap_buffers.clear();

    return ok;
}

int wxVideoCaptureWindowV4L::xioctl(int fd, int request, void *arg) const
{
    int r = 0;

    do
    {
        r = ioctl (fd, request, arg);
    }
    while ((-1 == r) && (EINTR == errno)); // interrupted, try again

    return r;
}

// ----------------------------------------------------------------------------
// Get v4l structs

int wxVideoCaptureWindowV4L::Get_v4l2_capability()
{
    memset(&m_v4l2_capability, 0, sizeof(v4l2_capability));
    int ret = xioctl(m_fd_device, VIDIOC_QUERYCAP, &m_v4l2_capability);
    if (ret != 0) perror("wxVidCap::Get_v4l2_capability() VIDIOC_QUERYCAP ERROR : ");

    // if (EINVAL == errno) fprintf (stderr, "%s is no V4L2 device\n", dev_name);

    return ret;
}

int wxVideoCaptureWindowV4L::Get_v4l2_fmtdesc()
{
    memset(&m_v4l2_fmtdesc, 0, sizeof(v4l2_fmtdesc));

    m_v4l2_fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    int ret = ioctl(m_fd_device, VIDIOC_ENUM_FMT, &m_v4l2_fmtdesc);
    if (ret != 0) perror("wxVidCap::Get_v4l2_fmtdesc() VIDIOC_ENUM_FMT ERROR : ");
    return ret;
}

int wxVideoCaptureWindowV4L::Get_v4l2_fmtdesc_vector()
{
    m_v4l2_fmtdesc_vector.clear();

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
            m_v4l2_fmtdesc_vector.push_back(v4l2_fmtdesc_);
            memcpy(&m_v4l2_fmtdesc_vector[index], &v4l2_fmtdesc_, sizeof(v4l2_fmtdesc));
            Print_v4l2_fmtdesc(m_v4l2_fmtdesc_vector[index]);
        }

        index++;
    }

    return 0;
}

int wxVideoCaptureWindowV4L::Get_v4l2_format()
{
    memset(&m_v4l2_format, 0, sizeof(v4l2_format));

    m_v4l2_format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    int ret = ioctl(m_fd_device, VIDIOC_G_FMT, &m_v4l2_format);
    if (ret != 0) perror("wxVidCap::Get_v4l2_format() VIDIOC_G_FMT ERROR : ");
    return ret;
}

// --------------------------------------------------------------------------
// print methods for ioctl structs
// --------------------------------------------------------------------------

void wxVideoCaptureWindowV4L::Print_v4l2_capability(const v4l2_capability& v4l2_capability_) const
{
    std::stringstream ss;

    ss << "v4l2_capability  : " << std::endl;

    ss << "  driver         : " << v4l2_capability_.driver       << std::endl;
    ss << "  card           : " << v4l2_capability_.card         << std::endl;
    ss << "  bus_info       : " << v4l2_capability_.bus_info     << std::endl;
    ss << "  version        : " << v4l2_capability_.version      << std::endl;
    ss << "  capabilities   : " << v4l2_capability_.capabilities << std::endl;

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

    std::cout << ss.str();
}

void wxVideoCaptureWindowV4L::Print_v4l2_fmtdesc(const v4l2_fmtdesc& v4l2_fmtdesc_) const
{
    std::stringstream ss;

    ss << "v4l2_fmtdesc     :" << std::endl;

    ss << "  index          : " << v4l2_fmtdesc_.index        << std::endl;
    ss << "  type           : " << v4l2_fmtdesc_.type         << std::endl;
    ss << "  flags          : " << v4l2_fmtdesc_.flags        << std::endl;
    ss << "  description    : " << v4l2_fmtdesc_.description  << std::endl;
    ss << "  pixelformat    : " << v4l2_fmtdesc_.pixelformat  << " FOURCC = " << FOURCCTowxString(v4l2_fmtdesc_.pixelformat) << std::endl;
    //ss << "  reserved     : " << v4l2_fmtdesc_.reserved  << std::endl;

    std::cout << ss.str();
}

void wxVideoCaptureWindowV4L::Print_v4l2_format(const v4l2_format& v4l2_format_) const
{
    std::stringstream ss;

    ss << "v4l2_format      :" << std::endl;

    ss << "  type           : " << v4l2_format_.type          << std::endl;

    if (v4l2_format_.type == V4L2_BUF_TYPE_VIDEO_CAPTURE)
        Print_v4l2_pix_format(v4l2_format_.fmt.pix);

    std::cout << ss.str();
}

void wxVideoCaptureWindowV4L::Print_v4l2_pix_format(const v4l2_pix_format& v4l2_pix_format_) const
{
    std::stringstream ss;

    ss << "v4l2_pix_format  :" << std::endl;

    ss << "  width,height   : " << v4l2_pix_format_.width << ", " << v4l2_pix_format_.height << std::endl;
    ss << "  pixelformat    : " << v4l2_pix_format_.pixelformat  << " FOURCC = " << FOURCCTowxString(v4l2_pix_format_.pixelformat) << std::endl;
    ss << "  field          : " << v4l2_pix_format_.field        << std::endl;
    ss << "  bytesperline   : " << v4l2_pix_format_.bytesperline << std::endl;
    ss << "  sizeimage      : " << v4l2_pix_format_.sizeimage    << std::endl;
    ss << "  colorspace     : " << v4l2_pix_format_.colorspace   << std::endl;
    ss << "  priv           : " << v4l2_pix_format_.priv         << std::endl;

    std::cout << ss.str();
}

#endif // !defined(__WXMSW__)
