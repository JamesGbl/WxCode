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

//#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>


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
    m_map             = MAP_FAILED;
    m_map_size        = 0;
    m_preview_wximage = false;

    InitV4LStructs();
}

void wxVideoCaptureWindowV4L::InitV4LStructs()
{
    memset(&m_video_mbuf,       0, sizeof(video_mbuf));
    memset(&m_video_mmap,       0, sizeof(video_mmap));
    memset(&m_video_capability, 0, sizeof(video_capability));
    memset(&m_video_window,     0, sizeof(video_window));
    memset(&m_video_picture,    0, sizeof(video_picture));
    memset(&m_video_buffer,     0, sizeof(video_buffer));
    memset(&m_video_channel,    0, sizeof(video_channel));
    memset(&m_video_clip,       0, sizeof(video_clip));
    memset(&m_video_capture,    0, sizeof(video_capture));
    memset(&m_video_unit,       0, sizeof(video_unit));
    memset(&m_video_tuner,      0, sizeof(video_tuner));

    memset(&m_v4l2_fmtdesc,     0, sizeof(v4l2_fmtdesc));
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
        // Of course, if the user code did add it already, we have nothing
        // to do.
        if ( !wxImage::FindHandler(wxBITMAP_TYPE_JPEG) )
            wxImage::AddHandler(new wxJPEGHandler);

        // In any case, no need to do it again.
        s_added_jpeg_handler = true;
    }


    EnumerateDevices();

    // open the device
    if (GetDeviceCount() > 0)
        DeviceConnect(0);

    print_V4L_video_mbuf();
    print_V4L_video_mmap();
    print_V4L_video_capability();
    print_V4L_video_window();
    print_V4L_video_picture();
    print_V4L_video_buffer();
    print_V4L_video_channel();

    m_v4l2_fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (ioctl(m_fd_device, VIDIOC_ENUM_FMT, &m_v4l2_fmtdesc)<0) perror("VIDIOC_ENUM_FMT");
    print_v4l2_fmtdesc();


//  short targaheader[9]={ 0,2,0,0,0,0,444,555,8216 }; /* 444 and 555 are fake */

    //m_video_mmap.format=VIDEO_PALETTE_GREY; //RGB24;    // ... GREY, RGB555, RGB565, RGB24, RGB32
    //m_video_mmap.frame=0;                   // 0 or 1 (buffer 0 or buffer 1)
    //m_video_mmap.width=272; //352;                 // from 32 to 924 (sometimes limited to 768)
    //m_video_mmap.height=139; //288;                // from 32 to 576
    //m_map_size=m_video_mmap.width*m_video_mmap.height*1; // rgb24:3, rgb32:4, rgb565:2, rgb555:2

    //targaheader[6]=v.width;
    //targaheader[7]=v.height;         // update targa header for frame

    //m_fd_device=open("/dev/video",O_RDONLY);  // open video device
    //if(m_fd_device<0) perror("/dev/video");


/*
        int xmin, xmax, ymin, ymax;
        unsigned long freq;

        if(ioctl(m_fd_device, VIDIOCGFREQ, &freq)<0) perror("VIDIOCGFREQ");
        fprintf(stderr,"freq: 0x%lx\n", freq);

        if(ioctl(m_fd_device, VIDIOCGCAP, &m_video_capability)<0) perror("VIDIOCGCAP");
        xmin=m_video_capability.minwidth, xmax=m_video_capability.maxwidth;
        ymin=m_video_capability.minheight,ymax=m_video_capability.maxheight;
        fprintf(stderr, "capture sizes: (%dx%d)-->(%dx%d)\n",xmin,ymin,xmax,ymax);

        if(ioctl(m_fd_device, VIDIOCGFBUF, &m_video_buffer)<0) perror("VIDIOCGFBUF");
        fprintf(stderr,"base:%p, size:(%dx%d), depth:%d, bpl=%d\n",
            m_video_buffer.base, m_video_buffer.width, m_video_buffer.height, m_video_buffer.depth, m_video_buffer.bytesperline);

        if(ioctl(m_fd_device, VIDIOCGPICT, &m_video_picture)<0) perror("VIDIOCGPICT");
        fprintf(stderr, "brightness:%d, hue:%d, color=%d, contrast=%d\n"
           "whiteness:%d, depth:%d, palette[%d]\n",
        m_video_picture.brightness, m_video_picture.hue, m_video_picture.colour, m_video_picture.contrast,
        m_video_picture.whiteness, m_video_picture.depth, m_video_picture.palette);

        printf("#======================== Video Capability\n");
        print_video_capability();
        printf("#======================== Video Window\n");
        print_video_window();
        printf("#======================== Video Channel\n");
        print_video_channel();
        printf("#======================== Video Mbuf\n");
        print_video_mbuf();
        printf("#==========================================\n");



//  fwrite(targaheader, sizeof(targaheader),   // save header & data to stdout
    //1, stdout);
  //fwrite(map, siz, 1, stdout);

    m_video_mmap.width=m_video_capability.maxwidth; //352;                 // from 32 to 924 (sometimes limited to 768)
    m_video_mmap.height=m_video_capability.maxheight; //288;                // from 32 to 576
    m_map_size=m_video_mmap.width*m_video_mmap.height*1; // rgb24:3, rgb32:4, rgb565:2, rgb555:2

*/

    if(ioctl(m_fd_device, VIDIOCMCAPTURE, &m_video_mmap)<0)        // start capturing a frame
        perror("VIDIOCMCAPTURE");

    if(ioctl(m_fd_device, VIDIOCSYNC, &m_video_mmap.frame)<0)      // wait for end of frame
        perror("VIDIOCSYNC");

    if (m_map != MAP_FAILED)
    {
        m_wximage.Create(m_video_mmap.width, m_video_mmap.height);
        memcpy( m_wximage.GetData(), m_map, wxMin(m_map_size, 3*m_video_mmap.width*m_video_mmap.height));
    }

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
    if (m_preview_wximage && (m_map != MAP_FAILED))
    {
        event.RequestMore();

        if(ioctl(m_fd_device, VIDIOCMCAPTURE, &m_video_mmap)<0)        // start capturing a frame
            perror("VIDIOCMCAPTURE");

        if(ioctl(m_fd_device, VIDIOCSYNC, &m_video_mmap.frame)<0)      // wait for end of frame
            perror("VIDIOCSYNC");

        if ( m_v4l2_fmtdesc.pixelformat == 1195724874) // JPEG
        {
            wxMemoryInputStream is(m_map, m_map_size);
            m_wximage = wxImage(is, wxBITMAP_TYPE_JPEG);
        }
        else
        {
            m_wximage.Create(m_video_mmap.width, m_video_mmap.height);
            //Y8_to_RGB24((void*)m_map, (void*)m_wximage.GetData(), m_video_mmap.width, m_video_mmap.height);
            memcpy( m_wximage.GetData(), m_map, wxMin(m_map_size, 3*m_video_mmap.width*m_video_mmap.height));
        }

        Refresh(false);
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
    m_videoCapabilities.clear();

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
        wxString video_filename(rootDir+filename);
        int fd_device = open_device(rootDir+filename);

        //wxPrintf(wxT("Enumerating [%s] %d\n"), video_filename.c_str(), fd_device); fflush(stdout);

        if (fd_device != -1)
        {
            struct video_capability vid_capability;
            memset(&vid_capability, 0, sizeof(video_capability));

            bool ok = (0==ioctl(fd_device, VIDIOCGCAP, &vid_capability));

            //wxPrintf(wxT("Enumerating [%s] %d\n"), video_filename.c_str(), (int)a); fflush(stdout);

            if (ok)
            {
                m_videoCapabilities.push_back(vid_capability);

                wxString deviceName = wxConvUTF8.cMB2WX(vid_capability.name);
                //printf("Enumerating [%s]\n", vid_capability.name); fflush(stdout);
                m_deviceNames.Add(deviceName);
                m_deviceVersions.Add(wxT("Unknown version"));
                m_deviceFilenames.Add(rootDir+filename);
            }
            else
                perror("Error enumerating video device");

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

    // Close it before
    if (m_fd_device != -1)
    {
        munmap_mem();
        close_device(m_fd_device);
        InitV4LStructs();
    }

    m_fd_device = open_device(m_deviceFilenames[index]);

    if (m_fd_device > -1)
    {
        m_deviceIndex = index;

        Get_V4L_video_capability();
        Get_V4L_video_buffer();
        Get_V4L_video_window();
        Get_V4L_video_channel();
        Get_V4L_video_picture();
        Get_V4L_video_tuner();
        Get_V4L_video_mbuf();

        m_video_mmap.format = VIDEO_PALETTE_GREY; //RGB24;    // ... GREY, RGB555, RGB565, RGB24, RGB32
        m_video_mmap.format = VIDEO_PALETTE_RGB24; //RGB24;    // ... GREY, RGB555, RGB565, RGB24, RGB32
        m_video_mmap.frame  = 0;                  // 0 or 1 (buffer 0 or buffer 1)
        m_video_mmap.width  = 640;                // from 32 to 924 (sometimes limited to 768)
        m_video_mmap.height = 480;                // from 32 to 576

        m_map_size          = m_video_mmap.width*m_video_mmap.height*3; // rgb24:3, rgb32:4, rgb565:2, rgb555:2

        //m_map_size = m_maxImageSize.x * m_maxImageSize.y * 3;

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
    InitV4LStructs();

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
        if (width)  *width = m_video_window.width;
        if (height) *width = m_video_window.height;
        if (bpp)    *bpp   = m_video_picture.depth;
        //if (fourcc) *fourcc = v4l2_format.v4l2_pix_format.pixelformat;
        return true;
    }

    if (width)  *width = 0;
    if (height) *width = 0;
    if (bpp)    *bpp = 0;
    if (fourcc) *fourcc = wxNullFOURCC;
    return false;
}

bool wxVideoCaptureWindowV4L::SetVideoFormat( int width, int height,
                                              int bpp, FOURCC format )
{
/*
    vd->picture.palette = palette;
    vd->mmap.format = palette;
    if(ioctl(vd->fd, VIDIOCSPICT, &(vd->picture)) < 0) {
                v4lperror("v4lsetpalette:VIDIOCSPICT");
                return -1;
        }
        return 0;
*/
    return false;
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

    if (onoff && (m_map == MAP_FAILED))
    {
        m_map_size = m_video_mmap.width*m_video_mmap.height*1;
    m_map=mmap(0, m_map_size, PROT_READ, MAP_SHARED, m_fd_device, 0);   // select memory-map
      if(m_map==MAP_FAILED) perror("mmap");
      }


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
#include <errno.h>
int wxVideoCaptureWindowV4L::open_device(const wxString &filename)
{
    int fd_device = -1; // error opening

    wxCharBuffer dev_buffer(filename.ToAscii()); //wxConvUTF8.cWX2MB(filename.c_str()));
    const char* dev_name = dev_buffer.data();
    wxStructStat st;

    if (-1 == wxStat(filename.c_str(), &st))
    {
        fprintf(stderr, "Cannot identify '%s': %d, %s\n",
                dev_name, errno, strerror (errno));
        return fd_device;
    }

    if (!S_ISCHR(st.st_mode))
    {
        fprintf(stderr, "%s is no device\n", dev_name);
        return fd_device;
    }

    fd_device = open(dev_name, O_RDWR | O_NONBLOCK, 0);

    if (fd_device == -1)
    {
        fprintf (stderr, "Cannot open '%s': %d, %s\n",
                 dev_name, errno, strerror (errno));
        //printf("Error opening device : [%s]\n", filename.c_str()); fflush(stdout);
        return fd_device;
    }

    return fd_device;
}
bool wxVideoCaptureWindowV4L::close_device(int fd_device)
{
    if (fd_device == -1) return false;

    int ret = close(fd_device);
    //printf("Closed device # %d : result %d\n", fd_device, ret);
    return ret == 0;
}

bool wxVideoCaptureWindowV4L::mmap_mem()
{
    if (m_map != MAP_FAILED)
        munmap_mem();

    if (m_fd_device == -1) return false;

    memset(&m_video_mbuf, 0, sizeof(video_mbuf));

    if(ioctl(m_fd_device, VIDIOCGMBUF, &m_video_mbuf)<0)  // get buffer size
        perror("mmap_mem() - VIDIOCGMBUF");

    m_map_size = m_video_mbuf.size;

    m_map = mmap(0, m_map_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd_device, 0);

    if (m_map == MAP_FAILED) perror("mmap");

    return m_map != MAP_FAILED;
}

bool wxVideoCaptureWindowV4L::munmap_mem()
{
    if (m_map == MAP_FAILED)
        return true;

    int ret    = munmap(m_map, m_map_size);
    m_map      = MAP_FAILED;
    m_map_size = 0;

    if (ret < 0) perror("wxVidCap::munmap_mem() ERROR : ");

    return ret == 0;
}

int wxVideoCaptureWindowV4L::xioctl(int fd, int request, void *arg) const
{
    int r;

    do
        r = ioctl (fd, request, arg);
    while ((-1 == r) && (EINTR == errno));

    return r;
}

// ----------------------------------------------------------------------------
// Get v4l structs

int wxVideoCaptureWindowV4L::Get_V4L_video_capability()
{
    memset(&m_video_capability, 0, sizeof(video_capability));
    int ret = ioctl(m_fd_device, VIDIOCGCAP, &m_video_capability);

    if (ret == 0)
    {
        m_minImageSize.x = m_video_capability.minwidth;
        m_minImageSize.y = m_video_capability.minheight;
        m_maxImageSize.x = m_video_capability.maxwidth;
        m_maxImageSize.y = m_video_capability.maxheight;
    }
    else
    {
        perror("wxVidCap::V4L_Get_video_capability() VIDIOCGCAP ERROR : ");
        m_minImageSize = wxSize( 10,  10);
        m_maxImageSize = wxSize(100, 100);
    }

    return ret;
}
int wxVideoCaptureWindowV4L::Get_V4L_video_buffer()
{
    memset(&m_video_buffer, 0, sizeof(video_buffer));
    int ret = ioctl(m_fd_device, VIDIOCGFBUF, &m_video_buffer);
    if (ret != 0) perror("wxVidCap::V4L_Get_video_buffer() VIDIOCGFBUF ERROR : ");
    return ret;
}
int wxVideoCaptureWindowV4L::Get_V4L_video_window()
{
    memset(&m_video_window, 0, sizeof(video_window));
    int ret = ioctl(m_fd_device, VIDIOCGWIN, &m_video_window);
    if (ret != 0) perror("wxVidCap::V4L_Get_video_window() VIDIOCGWIN ERROR : ");

    if (ret == 0)
        m_imageSize = wxSize(m_video_window.width, m_video_window.height);
    else
        m_imageSize = wxSize(0, 0);

    return ret;
}
int wxVideoCaptureWindowV4L::Get_V4L_video_channel()
{
    memset(&m_video_channel, 0, sizeof(video_channel));
    int ret = ioctl(m_fd_device, VIDIOCGCHAN, &m_video_channel);
    if (ret != 0) perror("wxVidCap::V4L_Get_video_window() VIDIOCGCHAN ERROR : ");
    return ret;
}
int wxVideoCaptureWindowV4L::Get_V4L_video_picture()
{
    memset(&m_video_picture, 0, sizeof(video_picture));
    int ret = ioctl(m_fd_device, VIDIOCGPICT, &m_video_picture);
    if (ret != 0) perror("wxVidCap::V4L_Get_video_picture() VIDIOCGPICT ERROR : ");
    return ret;
}
int wxVideoCaptureWindowV4L::Get_V4L_video_tuner()
{
    memset(&m_video_tuner, 0, sizeof(video_tuner));
    int ret = ioctl(m_fd_device, VIDIOCGTUNER, &m_video_tuner);
    if (ret != 0) perror("wxVidCap::V4L_Get_video_tuner() VIDIOCGTUNER ERROR : ");
    return ret;
}
int wxVideoCaptureWindowV4L::Get_V4L_video_mbuf()
{
    memset(&m_video_mbuf, 0, sizeof(video_mbuf));
    int ret = ioctl(m_fd_device, VIDIOCGMBUF, &m_video_mbuf);
    if (ret != 0) perror("wxVidCap::V4L_Get_video_mbuf() VIDIOCGMBUF ERROR : ");
    return ret;
}

int wxVideoCaptureWindowV4L::Get_v4l2_capability()
{
    memset(&m_v4l2_capability, 0, sizeof(v4l2_capability));
    int ret = ioctl(m_fd_device, VIDIOC_QUERYCAP, &m_v4l2_capability);
    if (ret != 0) perror("wxVidCap::Get_v4l2_capability() VIDIOC_QUERYCAP ERROR : ");
    return ret;
}
int wxVideoCaptureWindowV4L::Get_v4l2_fmtdesc()
{
    memset(&m_v4l2_fmtdesc, 0, sizeof(v4l2_fmtdesc));
    int ret = ioctl(m_fd_device, VIDIOC_ENUM_FMT, &m_v4l2_fmtdesc);
    if (ret != 0) perror("wxVidCap::Get_v4l2_fmtdesc() VIDIOC_ENUM_FMT ERROR : ");
    return ret;
}
int wxVideoCaptureWindowV4L::Get_v4l2_format()
{
    memset(&m_v4l2_format, 0, sizeof(v4l2_format));
    int ret = ioctl(m_fd_device, VIDIOC_G_FMT, &m_video_mbuf);
    if (ret != 0) perror("wxVidCap::Get_v4l2_format() VIDIOC_G_FMT ERROR : ");
    return ret;
}


// Set v4l structs

int wxVideoCaptureWindowV4L::Set_V4L_video_buffer()
{
    return ioctl(m_fd_device, VIDIOCSFBUF, &m_video_buffer);
}
int wxVideoCaptureWindowV4L::Set_V4L_video_window()
{
    return ioctl(m_fd_device, VIDIOCSWIN, &m_video_window);
}
int wxVideoCaptureWindowV4L::Set_V4L_videoSource(int chan)
{
    return ioctl(m_fd_device, VIDIOCSCHAN, chan);
}
int wxVideoCaptureWindowV4L::Set_V4L_video_picture()
{
    return ioctl(m_fd_device, VIDIOCSPICT, &m_video_picture);
}
int wxVideoCaptureWindowV4L::Set_V4L_video_tuner()
{
    return ioctl(m_fd_device, VIDIOCSTUNER, &m_video_tuner);
}

// print methods for ioctl structs
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <map>

static std::map<int, std::string> VIDEO_PALETTE_map;

void Init_VIDEO_PALETTE_map()
{
    if (VIDEO_PALETTE_map.size() > 0) return;

    VIDEO_PALETTE_map[VIDEO_PALETTE_GREY]       = "VIDEO_PALETTE_GREY";
    VIDEO_PALETTE_map[VIDEO_PALETTE_HI240]      = "VIDEO_PALETTE_HI240";
    VIDEO_PALETTE_map[VIDEO_PALETTE_RGB565]     = "VIDEO_PALETTE_RGB565";
    VIDEO_PALETTE_map[VIDEO_PALETTE_RGB24]      = "VIDEO_PALETTE_RGB24";
    VIDEO_PALETTE_map[VIDEO_PALETTE_RGB32]      = "VIDEO_PALETTE_RGB32";
    VIDEO_PALETTE_map[VIDEO_PALETTE_RGB555]     = "VIDEO_PALETTE_RGB555";
    VIDEO_PALETTE_map[VIDEO_PALETTE_YUV422]     = "VIDEO_PALETTE_YUV422";
    VIDEO_PALETTE_map[VIDEO_PALETTE_YUYV]       = "VIDEO_PALETTE_YUYV";
    VIDEO_PALETTE_map[VIDEO_PALETTE_UYVY]       = "VIDEO_PALETTE_UYVY";
    VIDEO_PALETTE_map[VIDEO_PALETTE_YUV420]     = "VIDEO_PALETTE_YUV420";
    VIDEO_PALETTE_map[VIDEO_PALETTE_YUV411]     = "VIDEO_PALETTE_YUV411";
    VIDEO_PALETTE_map[VIDEO_PALETTE_RAW]        = "VIDEO_PALETTE_RAW"; // bt 848
    VIDEO_PALETTE_map[VIDEO_PALETTE_YUV422P]    = "VIDEO_PALETTE_YUV422P";
    VIDEO_PALETTE_map[VIDEO_PALETTE_YUV411P]    = "VIDEO_PALETTE_YUV411P";
    VIDEO_PALETTE_map[VIDEO_PALETTE_YUV420P]    = "VIDEO_PALETTE_YUV420P";
    VIDEO_PALETTE_map[VIDEO_PALETTE_YUV410P]    = "VIDEO_PALETTE_YUV410P";
    VIDEO_PALETTE_map[VIDEO_PALETTE_PLANAR]     = "VIDEO_PALETTE_PLANAR";
    VIDEO_PALETTE_map[VIDEO_PALETTE_COMPONENT]  = "VIDEO_PALETTE_COMPONENT";
}

void wxVideoCaptureWindowV4L::print_V4L_video_mbuf()
{
    std::stringstream ss;

    ss << "Video MBuf: video_mbuf" << std::endl;

    ss << "  size      : " << m_video_mbuf.size       << std::endl;
    ss << "  frames    : " << m_video_mbuf.frames     << std::endl;
    ss << "  offset[0] : " << m_video_mbuf.offsets[0] << std::endl; // array is of size VIDEO_MAX_FRAME
    ss << "  offset[1] : " << m_video_mbuf.offsets[1] << std::endl;
    ss << "  offset[2] : " << m_video_mbuf.offsets[2] << std::endl;
    ss << "  offset[3] : " << m_video_mbuf.offsets[3] << std::endl;
    ss << "  offset[4] : " << m_video_mbuf.offsets[4] << std::endl;

    std::cout << ss.str();
}

void wxVideoCaptureWindowV4L::print_V4L_video_mmap()
{
    Init_VIDEO_PALETTE_map();

    std::stringstream ss;

    ss << "Video MMap: video_mmap" << std::endl;

    ss << "  frame         : " << m_video_mmap.frame  << std::endl;
    ss << "  width, height : " << m_video_mmap.width  << ", " << m_video_mmap.height << std::endl;
    ss << "  format        : " << m_video_mmap.format << " = " << VIDEO_PALETTE_map[m_video_mmap.format] << std::endl;

    std::cout << ss.str();
}

void wxVideoCaptureWindowV4L::print_V4L_video_capability()
{
    std::stringstream ss;

    ss << "Video Capability: video_capability" << std::endl;

    ss << "  name:      " << m_video_capability.name      << std::endl;
    ss << "  channels:  " << m_video_capability.channels  << std::endl;
    ss << "  audios:    " << m_video_capability.channels  << std::endl;
    ss << "  maxwidth:  " << m_video_capability.maxwidth  << std::endl;
    ss << "  maxheight: " << m_video_capability.maxheight << std::endl;
    ss << "  minwidth:  " << m_video_capability.minwidth  << std::endl;
    ss << "  minheight: " << m_video_capability.minheight << std::endl;

    ss << "   [ can capture to memory:                    | " << ((m_video_capability.type & VID_TYPE_CAPTURE)    ? "X" : " ") << " ]" << std::endl;
    ss << "   [ has a tuner of some form:                 | " << ((m_video_capability.type & VID_TYPE_TUNER)      ? "X" : " ") << " ]" << std::endl;
    ss << "   [ has teletext capability:                  | " << ((m_video_capability.type & VID_TYPE_TELETEXT)   ? "X" : " ") << " ]" << std::endl;
    ss << "   [ can overlay images onto frame buffer:     | " << ((m_video_capability.type & VID_TYPE_OVERLAY)    ? "X" : " ") << " ]" << std::endl;
    ss << "   [ overlay is chromakeyed:                   | " << ((m_video_capability.type & VID_TYPE_CHROMAKEY)  ? "X" : " ") << " ]" << std::endl;
    ss << "   [ overlay clipping is supported:            | " << ((m_video_capability.type & VID_TYPE_CLIPPING)   ? "X" : " ") << " ]" << std::endl;
    ss << "   [ overlay overwrites frame buffer memory:   | " << ((m_video_capability.type & VID_TYPE_FRAMERAM)   ? "X" : " ") << " ]" << std::endl;
    ss << "   [ hardware supports image scaling:          | " << ((m_video_capability.type & VID_TYPE_SCALES)     ? "X" : " ") << " ]" << std::endl;
    ss << "   [ image capture is greyscale only:          | " << ((m_video_capability.type & VID_TYPE_MONOCHROME) ? "X" : " ") << " ]" << std::endl;
    ss << "   [ capture can be of only part of the image: | " << ((m_video_capability.type & VID_TYPE_SUBCAPTURE) ? "X" : " ") << " ]" << std::endl;
    ss << "   [ Can decode MPEG streams:                  | " << ((m_video_capability.type & VID_TYPE_MPEG_DECODER)  ? "X" : " ") << " ]" << std::endl;
    ss << "   [ Can encode MPEG streams:                  | " << ((m_video_capability.type & VID_TYPE_MPEG_ENCODER)  ? "X" : " ") << " ]" << std::endl;
    ss << "   [ Can decode MJPEG streams:                 | " << ((m_video_capability.type & VID_TYPE_MJPEG_DECODER) ? "X" : " ") << " ]" << std::endl;
    ss << "   [ Can encode MJPEG streams:                 | " << ((m_video_capability.type & VID_TYPE_MJPEG_ENCODER) ? "X" : " ") << " ]" << std::endl;

    ss << std::endl;

    std::cout << ss.str();
}

void wxVideoCaptureWindowV4L::print_V4L_video_window()
{
    std::stringstream ss;

    ss << "Video Window: video_window" << std::endl;

    ss << "  position x,y  : " << m_video_window.x     << ", " << m_video_window.y << std::endl;
    ss << "  width, height : " << m_video_window.width << ", " << m_video_window.height << std::endl;
    ss << "  chromakey     : " << m_video_window.chromakey << std::endl;
    ss << "  flags         : ";

        __u32 mask = (0x1 << (sizeof(mask)*8 - 1));
        for (unsigned int b = 0; b < sizeof(mask)*8; b++, mask >>= 1)
            ss << (m_video_window.flags & mask ? "1" : "0");

    // struct video_clip - is set only
    // int clipcount - is set only
    ss << std::endl;

    std::cout << ss.str();
}

void wxVideoCaptureWindowV4L::print_V4L_video_picture()
{
    Init_VIDEO_PALETTE_map();

    std::stringstream ss;

    ss << "Video Picture: video_picture" << std::endl;
    ss << "  brightness : " << m_video_picture.brightness << std::endl;
    ss << "  hue        : " << m_video_picture.hue        << std::endl;
    ss << "  colour     : " << m_video_picture.colour     << std::endl;
    ss << "  contrast   : " << m_video_picture.contrast   << std::endl;
    ss << "  whiteness  : " << m_video_picture.whiteness  << std::endl;
    ss << "  depth      : " << m_video_picture.depth      << std::endl;
    ss << "  palette    : " << m_video_picture.palette << " = " << VIDEO_PALETTE_map[m_video_picture.palette] << std::endl;

    std::cout << ss.str();
}

void wxVideoCaptureWindowV4L::print_V4L_video_buffer()
{
    std::stringstream ss;

    ss << "Video Buffer : video_buffer" << std::endl;
    ss << "  base          : " << m_video_buffer.base  << std::endl;
    ss << "  width, height : " << m_video_buffer.width << ", " << m_video_buffer.height << std::endl;
    ss << "  depth         : " << m_video_buffer.depth << std::endl;
    ss << "  bytesperline  : " << m_video_buffer.bytesperline << std::endl;

    std::cout << ss.str();
}

void wxVideoCaptureWindowV4L::print_V4L_video_channel()
{
    std::stringstream ss;

    ss << "Video Channel: video_channel" << std::endl;

    ss << "  channel : " << m_video_channel.channel << std::endl;
    ss << "  name    : " << m_video_channel.name    << std::endl;
    ss << "  tuners  : " << m_video_channel.tuners  << std::endl;
    ss << "  flags   : " << std::endl;
    ss << "    [ channel has tuners:  | " << ((m_video_channel.flags & VIDEO_VC_TUNER) ? "X" : " ") << " ]" << std::endl;
    ss << "    [ channel has audio:   | " << ((m_video_channel.flags & VIDEO_VC_AUDIO) ? "X" : " ") << " ]" << std::endl;
// XXX not in header
//      ss << " [ channel has norm setting:\t\t| "
//           << ((chan.flags & VIDEO_VC_NORM)? "X" : " ") << " ]" << std::endl;
    ss << "  type    : " << std::endl;
    ss << "    [ input is a tv input: | " << ((m_video_channel.flags & VIDEO_TYPE_TV)     ? "X" : " ") << " ]" << std::endl;
    ss << "    [ input is a camera:   | " << ((m_video_channel.flags & VIDEO_TYPE_CAMERA) ? "X" : " ") << " ]" << std::endl;
    ss << "  norm    : " << m_video_channel.norm << std::endl;

    std::cout << ss.str();
}

void wxVideoCaptureWindowV4L::print_v4l2_capability()
{
    std::stringstream ss;

    ss << "V4L2 Format Description: v4l2_capability" << std::endl;

    ss << "  driver       : " << m_v4l2_capability.driver       << std::endl;
    ss << "  card         : " << m_v4l2_capability.card         << std::endl;
    ss << "  bus_info     : " << m_v4l2_capability.bus_info     << std::endl;
    ss << "  version      : " << m_v4l2_capability.version      << std::endl;
    ss << "  capabilities : " << m_v4l2_capability.capabilities << std::endl;
    //ss << "  reserved  : " << m_v4l2_capability.reserved  << std::endl;

    std::cout << ss.str();
}

void wxVideoCaptureWindowV4L::print_v4l2_fmtdesc()
{
    std::stringstream ss;

    ss << "V4L2 Format Description: v4l2_fmtdesc" << std::endl;

    ss << "  index       : " << m_v4l2_fmtdesc.index        << std::endl;
    ss << "  type        : " << m_v4l2_fmtdesc.type         << std::endl;
    ss << "  flags       : " << m_v4l2_fmtdesc.flags        << std::endl;
    ss << "  description : " << m_v4l2_fmtdesc.description  << std::endl;
    ss << "  pixelformat : " << m_v4l2_fmtdesc.pixelformat  << std::endl;
    //ss << "  reserved  : " << m_v4l2_fmtdesc.reserved  << std::endl;

    std::cout << ss.str();
}

void wxVideoCaptureWindowV4L::print_v4l2_format()
{
    std::stringstream ss;

    ss << "V4L2 Format Description: v4l2_format" << std::endl;

    ss << "  type        : " << m_v4l2_format.type          << std::endl;

    if (m_v4l2_format.type == V4L2_BUF_TYPE_VIDEO_CAPTURE)
        print_v4l2_pix_format(m_v4l2_format.fmt.pix);

    std::cout << ss.str();
}
void wxVideoCaptureWindowV4L::print_v4l2_pix_format(const v4l2_pix_format& v)
{
    std::stringstream ss;

    ss << "V4L2 Format Description: v4l2_pix_format" << std::endl;

    ss << "  width,height : " << v.width << ", " << v.height << std::endl;
    ss << "  pixelformat  : " << v.pixelformat  << std::endl;
    ss << "  field        : " << v.field        << std::endl;
    ss << "  bytesperline : " << v.bytesperline << std::endl;
    ss << "  sizeimage    : " << v.sizeimage    << std::endl;
    ss << "  colorspace   : " << v.colorspace   << std::endl;
    ss << "  priv         : " << v.priv         << std::endl;

    std::cout << ss.str();
}


#endif // !defined(__WXMSW__)
