//////////////////////////////////////////////////////////////////////////////
// File:        app.cpp
// Purpose:     CheckDFB application
// Maintainer:  Otto Wyss
// Created:     2003-10-10
// RCS-ID:      $Id: app.cpp,v 1.1 2004-10-01 18:41:03 wyo Exp $
// Copyright:   (c) wyoDesktop
// Licence:     wxWidgets licence
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------

// For compilers that support precompilation, includes <wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all 'standard' wxWidgets headers)
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

//! standard header
#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

//! directFB headers
#include <directfb.h>

//! wxWidgets headers
#include <wx/cmdline.h>  // command line support
#include <wx/config.h>   // configuration support
#include <wx/file.h>     // file content support
#include <wx/string.h>   // strings support

//----------------------------------------------------------------------------
// resources
//----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources)
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #include "app.xpm"
#endif


//============================================================================
// declarations
//============================================================================

const wxString APP_NAME = _T("CheckDFB");
const wxString APP_DESCR = _(
    "This application is just a sample application to show if the TestDFB \n"
    "is correctly set up and working. It gives the first impression of what \n"
    "can be expected of wyoDesktop."
);

const wxString APP_MAINT = _T("Otto Wyss");
const wxString APP_VENDOR = _T("wyoDesktop");
const wxString APP_COPYRIGTH = _T("(C) 2004 wyoDesktop");
const wxString APP_LICENCE = _T("wxWidgets");

const wxString APP_VERSION = _T("0.1.beta");
const wxString APP_BUILD = _T(__DATE__);

const wxString APP_WEBSITE = _T("http://wyoDesktop.sourceforge.net");
const wxString APP_MAIL = _T("http://wyoDesktop.sourceforge.net/feedback.php");

const wxString APP_INFOS = _(
    "If you like this app and want to help just subscribe to the users mailing \n"
    "and ask what you can do."
);

const wxString FRAMEBUFFER = _T("Framebuffer");
const wxString FB_DEVICE = _T("Device");
const wxString FB_WIDTH = _T("Width");
const wxString FB_HEIGTHT = _T("Height");
const wxString KEYBOARD = _T("Keyboard");
const wxString KB_DEVICE = _T("Device");
const wxString MOUSE = _T("Mouse");
const wxString MS_DEVICE = _T("Device");


//----------------------------------------------------------------------------
//! global application name
wxString g_appname;

//----------------------------------------------------------------------------
//! application
class App: public wxAppConsole {

public:
    App();

    // standard overrides
    void OnInitCmdLine (wxCmdLineParser& parser);
    bool OnCmdLineParsed (wxCmdLineParser& parser);
    bool OnInit();
    int  OnRun();

private:

};

//----------------------------------------------------------------------------
//! Check
class Check: public wxObject {

public:
    //! constructor/destructor
    Check () {};
    ~Check () {};

    // functions
    bool FbDevice (); // framebuffer
    bool KbDevice (); // keyboard
    bool MsDevice (); // mouse

    bool TestFB ();  // framebuffer
    bool TestDFB (); // DirectFB
    bool TestGDK (); // GDK library

private:
    // devices
    wxString m_fbName;
    int m_fbDev;
    wxString m_kbName;
    wxString m_msName;

    // framebuffer
    struct fb_fix_screeninfo m_fix_info;
    struct fb_var_screeninfo m_var_info;

};


//============================================================================
// implementation
//============================================================================

IMPLEMENT_APP_CONSOLE (App)

//----------------------------------------------------------------------------
// App
//----------------------------------------------------------------------------

App::App () {
}

bool App::OnInit () {

    //_CrtSetBreakAlloc (<memory_number>);

    // set application and vendor name
    SetAppName (APP_NAME);
    SetVendorName (APP_VENDOR);
    g_appname.Append (APP_NAME);

    // print welcome
    printf (_T("%s (%s)\n\n"), g_appname.c_str(), APP_VERSION.c_str());
    printf (_T("%s\n\n"), APP_DESCR.c_str());

    return wxAppConsole::OnInit();
}

void App::OnInitCmdLine (wxCmdLineParser& parser) {
    wxAppConsole::OnInitCmdLine(parser);

    static const wxCmdLineEntryDesc cmdLineDesc[] = {
        { wxCMD_LINE_SWITCH, _T("v"), _T("verbois"),
            _T("Verbois: Outputs detailed messages"),
            wxCMD_LINE_VAL_NONE, 0 },
        { wxCMD_LINE_NONE , 0, 0, 0, wxCMD_LINE_VAL_NONE, 0 }
    };

    parser.SetDesc(cmdLineDesc);
}

// Handle command line options
//
bool App::OnCmdLineParsed (wxCmdLineParser& parser) {
    return wxAppConsole::OnCmdLineParsed(parser);
}

// Run
//
int App::OnRun() {
    printf (_T("Test started!\n"));

    // check devices
    Check ck;
    if (!ck.KbDevice ()) return 1;
    if (!ck.MsDevice ()) return 1;
    if (!ck.FbDevice ()) return 1;

    // check TestFB
    if (!ck.TestFB ()) return 1;

    // check TestDFB
    if (!ck.TestDFB ()) return 1;

    // check TestGDK
    if (!ck.TestGDK ()) return 1;

    printf (_T("Test ended successfully!\n"));
    return 0;
}

// functions
bool Check::FbDevice() {

    // get framebuffer device name
    wxConfig* cfg = new wxConfig ();
    m_fbName = cfg->Read (FRAMEBUFFER + _T("/") + FB_DEVICE, _T("/dev/fb0"));
    delete cfg;

    // open for reading and writing
    m_fbDev = 0;
    m_fbDev = open (m_fbName.c_str(), O_RDWR);
    if (m_fbDev == -1) {
        perror (NULL);
        printf (_T("*** Error: Cannot open device %s\n"), m_fbName.c_str());
        printf (_T("If the name is wrong, change the config file (see \"Readme.txt\")\n"));
        return false;
    }
    printf (_T("The device %s was opened successfully.\n"), m_fbName.c_str());

    // get fixed screen information
    if (ioctl (m_fbDev, FBIOGET_FSCREENINFO, &m_fix_info) == -1) {
        perror (NULL);
        printf (_T("*** Error: Cannot read fix screen information\n"));
        return false;
    }
    printf (_T("ID=%s, type=%d, visual=%d\n"),
            m_fix_info.id, m_fix_info.type, m_fix_info.visual);

    // get variable screen information
    if (ioctl (m_fbDev, FBIOGET_VSCREENINFO, &m_var_info) == -1) {
        perror (NULL);
        printf (_T("*** Error: Cannot reading variable information.\n"));
        return false;
    }
    printf (_T("Resolution=%dx%d, bits per pixels=%dbpp\n"),
            m_var_info.xres, m_var_info.yres, m_var_info.bits_per_pixel);

    printf (_T("\n"));
    return true;
}

bool Check::KbDevice() {

    // get keyboard device name
    wxConfig* cfg = new wxConfig ();
    m_kbName = cfg->Read (KEYBOARD + _T("/") + KB_DEVICE, _T("/dev/tty0"));
    delete cfg;

    // open for reading
    int kbdev = 0;
    kbdev = open (m_kbName.c_str(), O_RDONLY);
    if (kbdev == -1) {
        perror (NULL);
        printf (_T("*** Error: Cannot open device %s\n"), m_kbName.c_str());
        printf (_T("If the name is wrong, change the config file (see \"Readme.txt\")\n"));
        return false;
    }
    printf (_T("The device %s was opened successfully.\n"), m_kbName.c_str());

    // enter text
    printf (_T("\n"
               "Now enter any text, it should be echoed on the screen. If you don't \n"
               "see anything, the test will stop after pausing for at least 5 sec. \n"
               "To end the test press the RETURN key twice. \n"));
    sleep (2);
    while () {
    }

    printf (_T("\n"));
    return true;
}

bool Check::MsDevice() {

    // get mouse device name
    wxConfig* cfg = new wxConfig ();
    m_msName = cfg->Read (MOUSE + _T("/") + MS_DEVICE, _T("/dev/psaux"));
    delete cfg;

    // open for reading
    int msdev = 0;
    msdev = open (m_msName, O_RDONLY);
    if (msdev == -1) {
        perror (NULL);
        printf (_T("*** Error: Cannot open device %s\n"), m_msName.c_str());
        printf (_T("If the name is wrong, change the config file (see \"Readme.txt\")\n"));
        return false;
    }
    printf (_T("The device %s was opened successfully.\n"), m_msName.c_str());

    // move mouse, press buttons
    printf (_T("\n"
               "Now move the mouse around or press any button. Each 2pixels movment in x-\n"
               "direction will be echoed by an 'x' char (negative direction by an 'X'), \n"
               "y-direction will be echoed by an 'y' char (negative by an 'Y'). Pressing \n"
               "the left mouse button will be echoed by an 'L', middle by an 'M', rigth \n"
               "by an 'R'. If you don't see anything, the test will stop after pausing \n"
               "for at least 5 sec. To end the test press the RETURN key twice. \n"));
    sleep (2);

    printf (_T("\n"));
    return true;
}

bool Check::TestFB() {

    // get output area size
    wxConfig* cfg = new wxConfig ();
    int width = wxMin (cfg->Read (FRAMEBUFFER + _T("/") + FB_WIDTH, 256), m_var_info.xres);
    int height = wxMin (cfg->Read (FRAMEBUFFER + _T("/") + FB_HEIGTHT, 256), m_var_info.yres);
    delete cfg;

    // map the srceen to memory
    int screensize = m_var_info.xres*m_var_info.yres*m_var_info.bits_per_pixel/8;
    char *fb = NULL;
    fb = (char *)mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, m_fbDev, 0);
    if (fb == MAP_FAILED) {
        perror (NULL);
        printf (_T("*** Error: failed to map screen to memory.\n"));
        return false;
    }
    printf (_T("The scree was mapped to memory successfully.\n"));

    // Figure out where in memory to put the pixel
    int xpos = (m_var_info.xres - width) / 2;
    int ypos = (m_var_info.yres - height) / 2;
    int x, y;
    int pos;
    int red, green, blue;
    for (x = 0; x < width; ++x ) {
        for (y = 0; y < height; ++y ) {
            pos = (m_var_info.xoffset + xpos + x) * (m_var_info.bits_per_pixel/8) +
                  (m_var_info.yoffset + ypos + y) * m_fix_info.line_length;
            if (m_var_info.bits_per_pixel == 32 ) {
                red = 256 - x * 256 / width;
                green = 256 - y * 256 / height;
                blue = x * 128 / width + y * 128 / height;
                *(fb + pos + 0) = blue;
                *(fb + pos + 1) = green;
                *(fb + pos + 2) = red;
                *(fb + pos + 3) = 0;
            }else if (m_var_info.bits_per_pixel == 24 ) {
                red = 256 - x * 256 / width;
                green = 256 - y * 256 / height;
                blue = x * 128 / width + y * 128 / height;
                *(fb + pos + 0) = blue;
                *(fb + pos + 1) = green;
                *(fb + pos + 2) = red;
            }else if (m_var_info.bits_per_pixel == 16 ) {
                red = 32 - x * 32 / width;
                green = 32 - y * 32 / height;
                blue = x * 16 / width + y * 16 / height;
                *((unsigned short int*)(fb + pos)) = red<<11 | green<<6 | blue;
            }else{ //assume 8bpp, no idea if this works ???
                red = 4 - x * 4 / width;
                green = 4 - y * 4 / height;
                blue = x * 2 / width + y * 2 / height;
                *(fb + pos) = red<<5 | green<<3 | blue;
            }

        }
    }

    // wait a little bit to see something
    printf (_T("Wait for 5 seconds\n"));
    sleep (5);

    // unmap and close
    munmap (fb, screensize);
    close (m_fbDev);
    printf (_T("The FbDevice is closed\n"));

    printf (_T("\n"));
    return true;
}

bool Check::TestDFB() {

    // get output area size
    wxConfig* cfg = new wxConfig ();
    int width = wxMin (cfg->Read (FRAMEBUFFER + _T("/") + FB_WIDTH, 256), m_var_info.xres);
    int height = wxMin (cfg->Read (FRAMEBUFFER + _T("/") + FB_HEIGTHT, 256), m_var_info.yres);
    delete cfg;

    DFBResult err;
#define DFBCHECK(x...) \
    err = x; \
    if (err != DFB_OK) { \
        fprintf( stderr, "%s <%d>:\n\t", __FILE__, __LINE__ ); \
        DirectFBErrorFatal( #x, err ); \
    }

    IDirectFB *dfb = NULL;
    IDirectFBSurface *primary = NULL;
    DFBSurfaceDescription dsc;

    // initialize
    DFBCHECK (DirectFBInit (NULL, NULL));
    DFBCHECK (DirectFBCreate (&dfb));
    dfb->SetCooperativeLevel (dfb, DFSCL_NORMAL);
    dsc.flags = DSDESC_CAPS;
    dsc.caps  = (DFBSurfaceCapabilities)(DSCAPS_PRIMARY | DSCAPS_FLIPPING);
    DFBCHECK (dfb->CreateSurface( dfb, &dsc, &primary ));
    printf (_T("DirectFB was initializes successfully.\n"));

    // blank (colour black) the area
    DFBCHECK (primary->FillRectangle (primary, 0, 0, width, height));

    // Figure out where in memory to put the pixel
    int x, y;
    int red, green, blue;
    for (x = 0; x < width; ++x ) {
        for (y = 0; y < height; ++y ) {
            if (m_var_info.bits_per_pixel == 32 ) {
                red = 256 - x * 256 / width;
                green = 256 - y * 256 / height;
                blue = x * 128 / width + y * 128 / height;
            }else if (m_var_info.bits_per_pixel == 24 ) {
                red = 256 - x * 256 / width;
                green = 256 - y * 256 / height;
                blue = x * 128 / width + y * 128 / height;
            }else if (m_var_info.bits_per_pixel == 16 ) {
                red = 256 - x * 32 / width;
                green = 256 - y * 32 / height;
                blue = x * 16 / width + y * 16 / height;
            }else{ //assume 8bpp, no idea if this works ???
                red = 256 - x * 4 / width;
                green = 256 - y * 4 / height;
                blue = x * 4 / width + y * 4 / height;
            }
            DFBCHECK (primary->SetColor (primary, red, green, blue, 0));
            DFBCHECK (primary->FillRectangle (primary, x, y, 1, 1));

        }
    }
    DFBCHECK (primary->Flip (primary, NULL, (DFBSurfaceFlipFlags)0));

    // wait a little bit to see something
    printf (_T("Wait for 5 seconds\n"));
    sleep (5);

    // release
    primary->Release( primary );
    dfb->Release( dfb );

    printf (_T("\n"));
    return true;
}

bool Check::TestGDK() {

    printf (_T("\n"));
    return true;
}
