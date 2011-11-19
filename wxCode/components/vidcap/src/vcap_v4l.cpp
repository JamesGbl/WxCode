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
    EVT_MOVE(wxVideoCaptureWindowV4L::OnMove)
    EVT_PAINT(wxVideoCaptureWindowV4L::OnDraw)
    EVT_SCROLLWIN(wxVideoCaptureWindowV4L::OnScrollWin)
    EVT_TIMER(IDD_wxVIDCAP_PREVIEW_WXIMAGE_TIMER, wxVideoCaptureWindowV4L::OnPreviewwxImageTimer)
    EVT_IDLE(wxVideoCaptureWindowV4L::OnIdle)
    EVT_CLOSE(wxVideoCaptureWindowV4L::OnCloseWindow)
END_EVENT_TABLE()

wxVideoCaptureWindowV4L::wxVideoCaptureWindowV4L( wxWindow *parent, wxWindowID id,
                                                  const wxPoint &pos, const wxSize &size,
                                                  long style, const wxString &name )
                     :wxVideoCaptureWindowBase()
{
    m_fd_device = -1;
    m_map = MAP_FAILED;
    m_map_size = 0;
    m_preview_wximage = false;

    if (!Create( parent, id, pos, size, style, name ))
        wxMessageBox(wxT("Can't create capture window"), wxT("wxVideoCaptureWindow Error"));
}

bool wxVideoCaptureWindowV4L::Create( wxWindow *parent, wxWindowID id,
                                      const wxPoint &pos, const wxSize &size,
                                      long style, const wxString &name )
{
    if (!wxVideoCaptureWindowBase::Create(parent, id, pos, size, style, name))
        return false;

    EnumerateDevices();

    // open the device
    DeviceConnect(0);

//  short targaheader[9]={ 0,2,0,0,0,0,444,555,8216 }; /* 444 and 555 are fake */

    m_video_mmap.format=VIDEO_PALETTE_GREY; //RGB24;    // ... GREY, RGB555, RGB565, RGB24, RGB32
    m_video_mmap.frame=0;                   // 0 or 1 (buffer 0 or buffer 1)
    m_video_mmap.width=272; //352;                 // from 32 to 924 (sometimes limited to 768)
    m_video_mmap.height=139; //288;                // from 32 to 576
    m_map_size=m_video_mmap.width*m_video_mmap.height*1; // rgb24:3, rgb32:4, rgb565:2, rgb555:2

    //targaheader[6]=v.width;
    //targaheader[7]=v.height;         // update targa header for frame

    //m_fd_device=open("/dev/video",O_RDONLY);  // open video device
    //if(m_fd_device<0) perror("/dev/video");

    mmap_mem();

    {
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
    }

    if(ioctl(m_fd_device, VIDIOCMCAPTURE, &m_video_mmap)<0)        // start capturing a frame
        perror("VIDIOCMCAPTURE");

    if(ioctl(m_fd_device, VIDIOCSYNC, &m_video_mmap.frame)<0)      // wait for end of frame
        perror("VIDIOCSYNC");

//  fwrite(targaheader, sizeof(targaheader),   // save header & data to stdout
    //1, stdout);
  //fwrite(map, siz, 1, stdout);

    if (m_map != MAP_FAILED)
    {
        m_wximage.Create(m_video_mmap.width, m_video_mmap.height);
        memcpy( m_wximage.GetData(), m_map, m_map_size);
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

// yes this has the same stuff as ~wxVideoCaptureWindow, erm... which is better?
// I think that makes the closing more responsive when previewing w/ wxImages
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

        m_wximage.Create(m_video_mmap.width, m_video_mmap.height);
        //Y8_to_RGB24((void*)m_map, (void*)m_wximage.GetData(), m_video_mmap.width, m_video_mmap.height);
        memcpy( m_wximage.GetData(), m_map, m_map_size);
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

    wxString rootDir(wxT("/dev/v4l/"));
    wxString filename;

    wxDir dir(rootDir);
    if (!dir.IsOpened())
    {
        wxMessageBox(wxT("Unable to open video devices in /dev/v4l/"),
                     wxT("Error opening video devices"),
                     wxOK, this);
        return;
    }
    bool has_files = dir.GetFirst(&filename, wxT("video*"), wxDIR_FILES);

    while (has_files)
    {
        if (open_device(rootDir+filename))
        {
            V4L_Get_video_capability();
            wxString deviceName = wxConvUTF8.cMB2WX(m_video_capability.name);
            //printf("Enumerating [%s]\n", m_video_capability.name); fflush(stdout);
            m_deviceNames.Add(deviceName);
            m_deviceVersions.Add(wxT("Unknown version"));
            m_deviceFilenames.Add(rootDir+filename);
            close_device();
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

    //if (m_fd_device > -1)

    if (open_device(m_deviceFilenames[index]))
    {
        m_deviceIndex = index;
        V4L_Get_video_capability();
    }

    return true;
}

bool wxVideoCaptureWindowV4L::DeviceDisconnect()
{
    m_preview_wximage = false;
    m_deviceIndex = -1;

    munmap_mem();
    close_device();

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
bool wxVideoCaptureWindowV4L::open_device(const wxString &filename)
{
    close_device();

    wxCharBuffer dev_buffer(filename.ToAscii()); //wxConvUTF8.cWX2MB(filename.c_str()));
    const char* dev_name = dev_buffer.data();
    wxStructStat st;

    if (-1 == wxStat(filename.c_str(), &st))
    {
        fprintf(stderr, "Cannot identify '%s': %d, %s\n",
                dev_name, errno, strerror (errno));
        return false;
    }

    if (!S_ISCHR(st.st_mode))
    {
        fprintf(stderr, "%s is no device\n", dev_name);
        return false;
    }

    m_fd_device = open(dev_name, O_RDWR | O_NONBLOCK, 0);
    if (m_fd_device == -1)
    {
        fprintf (stderr, "Cannot open '%s': %d, %s\n",
                 dev_name, errno, strerror (errno));
        //printf("Error opening device : [%s]\n", filename.c_str()); fflush(stdout);
        return false;
    }

    return true;
}
bool wxVideoCaptureWindowV4L::close_device()
{
    if (m_fd_device == -1) return false;
    munmap_mem();
    int ret = close(m_fd_device);
    m_fd_device = -1;
    printf("*** Close %d\n", ret);
    return ret == 0;
}

bool wxVideoCaptureWindowV4L::mmap_mem()
{
    if (m_map != MAP_FAILED) munmap_mem();
    if (m_map_size < 1) return false;
    if (m_fd_device == -1) return false;

    m_map=mmap(0, m_map_size, PROT_READ, MAP_SHARED, m_fd_device, 0);   // select memory-map
    if(m_map==MAP_FAILED) perror("mmap");
    return m_map != MAP_FAILED;
}

bool wxVideoCaptureWindowV4L::munmap_mem()
{
    if (m_map != MAP_FAILED)
    {
        int ret = munmap(m_map, m_map_size);
        m_map = MAP_FAILED;
        m_map_size = 0;
        return ret == 0;
    }
    return false;
}

int wxVideoCaptureWindowV4L::xioctl(int fd, int request, void *arg) const
{
    int r;

    do r = ioctl (fd, request, arg);
    while (-1 == r && EINTR == errno);

    return r;
}

// ----------------------------------------------------------------------------
// Get v4l structs

int wxVideoCaptureWindowV4L::V4L_Get_video_capability()
{
    bool ok = 0==ioctl(m_fd_device, VIDIOCGCAP, &m_video_capability);
    if (ok)
    {
        m_minImageSize.x = m_video_capability.minwidth;
        m_minImageSize.y = m_video_capability.minheight;
        m_maxImageSize.x = m_video_capability.maxwidth;
        m_maxImageSize.y = m_video_capability.maxheight;
    }
    else
    {


    }
    return ok;
}
int wxVideoCaptureWindowV4L::V4L_Get_video_buffer()
{
    return ioctl(m_fd_device, VIDIOCGFBUF, &m_video_buffer);
}
int wxVideoCaptureWindowV4L::V4L_Get_video_window()
{
    bool ok = 0==ioctl(m_fd_device, VIDIOCGWIN, &m_video_window);
    if (ok)
    {
        m_imageSize.x = m_video_window.width;
        m_imageSize.y = m_video_window.height;
    }
    else
    {
        m_imageSize = wxSize(0, 0);
    }
    return ok;
}
int wxVideoCaptureWindowV4L::V4L_Get_video_channel()
{
    return ioctl(m_fd_device, VIDIOCGCHAN, &m_video_channel);
}
int wxVideoCaptureWindowV4L::V4L_Get_video_picture()
{
    return ioctl(m_fd_device, VIDIOCGPICT, &m_video_picture);
}
int wxVideoCaptureWindowV4L::V4L_Get_video_tuner()
{
    return ioctl(m_fd_device, VIDIOCGTUNER, &m_video_tuner);
}
int wxVideoCaptureWindowV4L::V4L_Get_video_mbuf()
{
    return ioctl(m_fd_device, VIDIOCGMBUF, &m_video_mbuf);
}

// Set v4l structs

int wxVideoCaptureWindowV4L::V4L_Set_video_buffer()
{
    return ioctl(m_fd_device, VIDIOCSFBUF, &m_video_buffer);
}
int wxVideoCaptureWindowV4L::V4L_Set_video_window()
{
    return ioctl(m_fd_device, VIDIOCSWIN, &m_video_window);
}
int wxVideoCaptureWindowV4L::V4L_Set_videoSource(int chan)
{
    return ioctl(m_fd_device, VIDIOCSCHAN, chan);
}
int wxVideoCaptureWindowV4L::V4L_Set_video_picture()
{
    return ioctl(m_fd_device, VIDIOCSPICT, &m_video_picture);
}
int wxVideoCaptureWindowV4L::V4L_Set_video_tuner()
{
    return ioctl(m_fd_device, VIDIOCSTUNER, &m_video_tuner);
}

// print methods for ioctl structs
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

void wxVideoCaptureWindowV4L::print_video_capability()
{
    cerr << "Video Capability: m_video_capability" << endl;

    cout << "name: " << m_video_capability.name << endl;

    cerr << " [ can capture to memory:\t\t\t| "
         << ((m_video_capability.type & VID_TYPE_CAPTURE)?    "X" : " ") << " ]" << endl;
    cerr << " [ has a tuner of some form:\t\t\t| "
         << ((m_video_capability.type & VID_TYPE_TUNER)?      "X" : " ") << " ]" << endl;
    cerr << " [ has teletext capability:\t\t\t| "
         << ((m_video_capability.type & VID_TYPE_TELETEXT)?   "X" : " ") << " ]" << endl;
    cerr << " [ can overlay images onto frame buffer:\t| "
         << ((m_video_capability.type & VID_TYPE_OVERLAY)?    "X" : " ") << " ]" << endl;
    cerr << " [ overlay is chromakeyed:\t\t\t| "
         << ((m_video_capability.type & VID_TYPE_CHROMAKEY)?  "X" : " ") << " ]" << endl;
    cerr << " [ overlay clipping is supported:\t\t| "
         << ((m_video_capability.type & VID_TYPE_CLIPPING)?   "X" : " ") << " ]" << endl;
    cerr << " [ overlay overwrites frame buffer memory:\t| "
         << ((m_video_capability.type & VID_TYPE_FRAMERAM)?   "X" : " ") << " ]" << endl;
    cerr << " [ hardware supports image scaling:\t\t| "
         << ((m_video_capability.type & VID_TYPE_SCALES)?     "X" : " ") << " ]" << endl;
    cerr << " [ image capture is greyscale only:\t\t| "
         << ((m_video_capability.type & VID_TYPE_MONOCHROME)? "X" : " ") << " ]" << endl;
    cerr << " [ capture can be of only part of the image:\t| "
         << ((m_video_capability.type & VID_TYPE_SUBCAPTURE)? "X" : " ") << " ]" << endl;

    cerr << "channels: "  << m_video_capability.channels  << endl;
    cerr << "audios: "    << m_video_capability.channels  << endl;
    cerr << "maxwidth: "  << m_video_capability.maxwidth  << endl;
    cerr << "maxheight: " << m_video_capability.maxheight << endl;
    cerr << "minwidth: "  << m_video_capability.minwidth  << endl;
    cerr << "minheight: " << m_video_capability.minheight << endl;
}

void wxVideoCaptureWindowV4L::print_video_window()
{
    cerr << "Video Window: m_video_window" << endl;

    cerr << "x co-ord: "  << m_video_window.x         << endl;
    cerr << "y co-ord: "  << m_video_window.y         << endl;
    cerr << "width: "     << m_video_window.width     << endl;
    cerr << "height: "    << m_video_window.height    << endl;
    cerr << "chromakey: " << m_video_window.chromakey << endl;
    __u32 mask = (0x1 << (sizeof(mask)*8 - 1));
    cerr << "flags: ";
            for (unsigned int b = 0; b < sizeof(mask)*8; b++, mask >>= 1)
                    cerr << (m_video_window.flags & mask ? "1" : "0");
    cerr << endl;
}

void wxVideoCaptureWindowV4L::print_video_channel()
{
    cerr << "Video Channel: m_video_channel" << endl;

    cerr << "number: " << m_video_channel.channel << endl;
    cerr << "name: "   << m_video_channel.name    << endl;
    cerr << "tuners: " << m_video_channel.tuners  << endl;
    cerr << "flags: "  << endl;
    cerr << " [ channel has tuners:\t\t\t| "
         << ((m_video_channel.flags & VIDEO_VC_TUNER)? "X" : " ") << " ]" << endl;
    cerr << " [ channel has audio:\t\t\t| "
         << ((m_video_channel.flags & VIDEO_VC_AUDIO)? "X" : " ") << " ]" << endl;
// XXX not in header
//      cerr << " [ channel has norm setting:\t\t| "
//           << ((chan.flags & VIDEO_VC_NORM)? "X" : " ") << " ]" << endl;
    cerr << "type: "   << endl;
    cerr << " [ input is a tv input:\t\t\t| "
         << ((m_video_channel.flags & VIDEO_TYPE_TV)? "X" : " ") << " ]" << endl;
    cerr << " [ input is a camera:\t\t\t| "
         << ((m_video_channel.flags & VIDEO_TYPE_CAMERA)? "X" : " ") << " ]" << endl;
    cerr << "norm: "   << m_video_channel.norm << endl;
}

void wxVideoCaptureWindowV4L::print_video_mbuf()
{
    cerr << "Video MBuf: m_video_mbuf" << endl;

    cerr << "size: "   << m_video_mbuf.size       << endl;
    cerr << "frames: " << m_video_mbuf.frames     << endl;
    cerr << "offset: " << m_video_mbuf.offsets[1] << endl;
}

#endif // !defined(__WXMSW__)
