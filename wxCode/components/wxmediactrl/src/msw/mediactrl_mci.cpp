/////////////////////////////////////////////////////////////////////////////
// Name:        src/msw/mediactrl_mci.cpp
// Purpose:     MCI (Media Command Interface) Media Backend for Windows
// Author:      Ryan Norton <wxprojects@comcast.net>
// Modified by:
// Created:     01/29/05
// RCS-ID:      $Id: mediactrl_mci.cpp,v 1.1 2006-02-15 19:21:01 ryannpcs Exp $
// Copyright:   (c) Ryan Norton
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

//===========================================================================
//  DECLARATIONS
//===========================================================================

//---------------------------------------------------------------------------
// Pre-compiled header stuff
//---------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

//---------------------------------------------------------------------------
// MediaCtrl include
//---------------------------------------------------------------------------
#include "wx/mediactrl.h"

//---------------------------------------------------------------------------
// Compilation guard (not on CE)
//---------------------------------------------------------------------------
#if wxUSE_MEDIACTRL && !defined(__WXWINCE__)

//---------------------------------------------------------------------------
// WX Includes
//---------------------------------------------------------------------------
#include "wx/log.h"         //wxLogDebug
#include "wx/math.h"        //log10 & pow
#include "wx/msw/private.h" //user info and wndproc setting/getting
#include "wx/dcclient.h"
#include "wx/timer.h"
#include "wx/dynlib.h"
#include "wx/stopwatch.h"

//---------------------------------------------------------------------------
// Externals (somewhere in src/msw/app.cpp and src/msw/window.cpp)
//---------------------------------------------------------------------------
extern "C" WXDLLIMPEXP_BASE HINSTANCE wxGetInstance(void);
#ifdef __WXWINCE__
extern WXDLLIMPEXP_CORE       wxChar *wxCanvasClassName;
#else
extern WXDLLIMPEXP_CORE const wxChar *wxCanvasClassName;
#endif

//---------------------------------------------------------------------------
//
//  wxMCIMediaBackend
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  MCI Includes
//---------------------------------------------------------------------------
#include <mmsystem.h>

class WXDLLIMPEXP_MEDIA wxMCIMediaBackend : public wxMediaBackendCommonBase
{
public:

    wxMCIMediaBackend();
    ~wxMCIMediaBackend();

    virtual bool CreateControl(wxControl* ctrl, wxWindow* parent,
                                     wxWindowID id,
                                     const wxPoint& pos,
                                     const wxSize& size,
                                     long style,
                                     const wxValidator& validator,
                                     const wxString& name);

    virtual bool Play();
    virtual bool Pause();
    virtual bool Stop();

    virtual bool Load(const wxURI& location,
                      const wxURI& proxy)
    { return wxMediaBackend::Load(location, proxy); }

    virtual bool Load(const wxString& fileName);
    virtual bool Load(const wxURI& location);

    virtual wxMediaState GetState();

    virtual bool SetPosition(wxLongLong where);
    virtual wxLongLong GetPosition();
    virtual wxLongLong GetDuration();

    virtual void Move(int x, int y, int w, int h);
    wxSize GetVideoSize() const;

    virtual double GetPlaybackRate();
    virtual bool SetPlaybackRate(double dRate);

    virtual double GetVolume();
    virtual bool SetVolume(double);

    static LRESULT CALLBACK NotifyWndProc(HWND hWnd, UINT nMsg,
                                             WPARAM wParam, LPARAM lParam);

    LRESULT CALLBACK OnNotifyWndProc(HWND hWnd, UINT nMsg,
                                     WPARAM wParam, LPARAM lParam);

    MCIDEVICEID m_hDev;     //Our MCI Device ID/Handler
    HWND m_hNotifyWnd;      //Window to use for MCI events
    bool m_bVideo;          //Whether or not we have video

    DECLARE_DYNAMIC_CLASS(wxMCIMediaBackend)
};

//===========================================================================
//  IMPLEMENTATION
//===========================================================================

//---------------------------------------------------------------------------
//
// wxMCIMediaBackend
//
//---------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxMCIMediaBackend, wxMediaBackend)

//---------------------------------------------------------------------------
// Usual debugging macros for MCI returns
//---------------------------------------------------------------------------

#ifdef __WXDEBUG__
#define wxMCIVERIFY(arg) \
{ \
    DWORD nRet; \
    if ( (nRet = (arg)) != 0) \
    { \
        TCHAR sz[5000]; \
        mciGetErrorString(nRet, sz, 5000); \
        wxFAIL_MSG(wxString::Format(_T("MCI Error:%s"), sz)); \
    } \
}
#else
#define wxMCIVERIFY(arg) (arg);
#endif

//---------------------------------------------------------------------------
// Simulation for <digitalv.h>
//
// Mingw and possibly other compilers don't have the digitalv.h header
// that is needed to have some essential features of mci work with
// windows - so we provide the declarations for the types we use here
//---------------------------------------------------------------------------

typedef struct {
    DWORD_PTR   dwCallback;
#ifdef MCI_USE_OFFEXT
    POINT   ptOffset;
    POINT   ptExtent;
#else
    RECT    rc;
#endif
} MCI_DGV_RECT_PARMS;

typedef struct {
    DWORD_PTR   dwCallback;
    HWND    hWnd;
#ifndef _WIN32
    WORD    wReserved1;
#endif
    UINT    nCmdShow;
#ifndef _WIN32
    WORD    wReserved2;
#endif
    wxChar*   lpstrText;
} MCI_DGV_WINDOW_PARMS;

typedef struct {
    DWORD_PTR dwCallback;
    DWORD     dwTimeFormat;
    DWORD     dwAudio;
    DWORD     dwFileFormat;
    DWORD     dwSpeed;
} MCI_DGV_SET_PARMS;

typedef struct {
    DWORD_PTR   dwCallback;
    DWORD   dwItem;
    DWORD   dwValue;
    DWORD   dwOver;
    wxChar*   lpstrAlgorithm;
    wxChar*   lpstrQuality;
} MCI_DGV_SETAUDIO_PARMS;

//---------------------------------------------------------------------------
// wxMCIMediaBackend Constructor
//
// Here we don't need to do much except say we don't have any video :)
//---------------------------------------------------------------------------
wxMCIMediaBackend::wxMCIMediaBackend() : m_hNotifyWnd(NULL), m_bVideo(false)
{
}

//---------------------------------------------------------------------------
// wxMCIMediaBackend Destructor
//
// We close the mci device - note that there may not be an mci device here,
// or it may fail - but we don't really care, since we're destructing
//---------------------------------------------------------------------------
wxMCIMediaBackend::~wxMCIMediaBackend()
{
    if(m_hNotifyWnd)
    {
        mciSendCommand(m_hDev, MCI_CLOSE, 0, 0);
        DestroyWindow(m_hNotifyWnd);
        m_hNotifyWnd = NULL;
    }
}

//---------------------------------------------------------------------------
// wxMCIMediaBackend::Create
//
// Here we just tell wxMediaCtrl that mci does exist (which it does, on all
// msw systems, at least in some form dating back to win16 days)
//---------------------------------------------------------------------------
bool wxMCIMediaBackend::CreateControl(wxControl* ctrl, wxWindow* parent,
                                     wxWindowID id,
                                     const wxPoint& pos,
                                     const wxSize& size,
                                     long style,
                                     const wxValidator& validator,
                                     const wxString& name)
{
    //
    // Create window
    // By default wxWindow(s) is created with a border -
    // so we need to get rid of those, and create with
    // wxCLIP_CHILDREN, so that if the driver/backend
    // is a child window, it refereshes properly
    //
    if ( !ctrl->wxControl::Create(parent, id, pos, size,
                            (style & ~wxBORDER_MASK) | wxBORDER_NONE | wxCLIP_CHILDREN,
                            validator, name) )
        return false;

    m_ctrl = wxStaticCast(ctrl, wxMediaCtrl);
    return true;
}

//---------------------------------------------------------------------------
// wxMCIMediaBackend::Load (file version)
//
// Here we have MCI load a file and device, set the time format to our
// default (milliseconds), and set the video (if any) to play in the control
//---------------------------------------------------------------------------
bool wxMCIMediaBackend::Load(const wxString& fileName)
{
    //
    //if the user already called load close the previous MCI device
    //
    if(m_hNotifyWnd)
    {
        mciSendCommand(m_hDev, MCI_CLOSE, 0, 0);
        DestroyWindow(m_hNotifyWnd);
        m_hNotifyWnd = NULL;
    }

    //
    //Opens a file and has MCI select a device.  Normally you'd put
    //MCI_OPEN_TYPE in addition to MCI_OPEN_ELEMENT - however if you
    //omit this it tells MCI to select the device instead.  This is
    //good because we have no reliable way of "enumerating" the devices
    //in MCI
    //
    MCI_OPEN_PARMS openParms;
    openParms.lpstrElementName = (wxChar*) fileName.c_str();

    if ( mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT,
                        (DWORD)(LPVOID)&openParms) != 0)
        return false;

    m_hDev = openParms.wDeviceID;

    //
    //Now set the time format for the device to milliseconds
    //
    MCI_SET_PARMS setParms;
    setParms.dwCallback = 0;
    setParms.dwTimeFormat = MCI_FORMAT_MILLISECONDS;

    if (mciSendCommand(m_hDev, MCI_SET, MCI_SET_TIME_FORMAT,
                         (DWORD)(LPVOID)&setParms) != 0)
    {
        wxLogSysError(wxT("Could not set initial time format")
                      wxT("in MCI for media!"));
        return false;
    }

    //
    //Now tell the MCI device to display the video in our wxMediaCtrl
    //
    MCI_DGV_WINDOW_PARMS windowParms;
    windowParms.hWnd = (HWND)m_ctrl->GetHandle();

    m_bVideo = (mciSendCommand(m_hDev, MCI_WINDOW,
                               0x00010000L, //MCI_DGV_WINDOW_HWND
                               (DWORD)(LPVOID)&windowParms) == 0);

    //
    // Create a hidden window and register to handle
    // MCI events
    // Note that wxCanvasClassName is already registered
    // and used by all wxWindows and normal wxControls
    //
    m_hNotifyWnd = ::CreateWindow
                    (
                        wxCanvasClassName,
                        NULL,
                        0, 0, 0, 0,
                        0,
                        (HWND) NULL,
                        (HMENU)NULL,
                        wxGetInstance(),
                        (LPVOID) NULL
                    );

    if(!m_hNotifyWnd)
    {
        wxLogSysError( wxT("Could not create hidden needed for ")
                       wxT("registering for MCI events!")  );

        return false;
    }

    wxSetWindowProc(m_hNotifyWnd, wxMCIMediaBackend::NotifyWndProc);
    wxSetWindowUserData(m_hNotifyWnd, this);

    NotifyMovieLoaded();

    return true;
}

//---------------------------------------------------------------------------
// wxMCIMediaBackend::Load (URL version)
//
// MCI doesn't support URLs directly...
//---------------------------------------------------------------------------
bool wxMCIMediaBackend::Load(const wxURI& WXUNUSED(location))
{
    return false;
}

//---------------------------------------------------------------------------
// wxMCIMediaBackend::Play
//
// Plays/Resumes the MCI device... a couple notes:
// 1) Certain drivers will crash and burn if we don't pass them an
//    MCI_PLAY_PARMS, despite the documentation that says otherwise...
// 2) There is a MCI_RESUME command, but MCI_PLAY does the same thing
//    and will resume from a stopped state also, so there's no need to
//    call both, for example
//---------------------------------------------------------------------------
bool wxMCIMediaBackend::Play()
{
    MCI_PLAY_PARMS playParms;
    playParms.dwCallback = (DWORD)m_hNotifyWnd;

    bool bOK = ( mciSendCommand(m_hDev, MCI_PLAY, MCI_NOTIFY,
                            (DWORD)(LPVOID)&playParms) == 0 );

    if(bOK)
        {
        m_ctrl->Show(m_bVideo);
        QueuePlayEvent();
        }

    return bOK;
}

//---------------------------------------------------------------------------
// wxMCIMediaBackend::Pause
//
// Pauses the MCI device - nothing special
//---------------------------------------------------------------------------
bool wxMCIMediaBackend::Pause()
{
    if (mciSendCommand(m_hDev, MCI_PAUSE, MCI_WAIT, 0) == 0)
    {
        QueuePauseEvent();
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
// wxMCIMediaBackend::Stop
//
// Stops the MCI device & seeks to the beginning as wxMediaCtrl docs outline
//---------------------------------------------------------------------------
bool wxMCIMediaBackend::Stop()
{
    if (mciSendCommand(m_hDev, MCI_STOP, MCI_WAIT, 0) == 0 &&
        mciSendCommand(m_hDev, MCI_SEEK, MCI_SEEK_TO_START, 0) == 0)
    {
        QueueStopEvent(); 
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
// wxMCIMediaBackend::GetState
//
// Here we get the state and convert it to a wxMediaState -
// since we use direct comparisons with MCI_MODE_PLAY and
// MCI_MODE_PAUSE, we don't care if the MCI_STATUS call
// fails or not
//---------------------------------------------------------------------------
wxMediaState wxMCIMediaBackend::GetState()
{
    MCI_STATUS_PARMS statusParms;
    statusParms.dwItem = MCI_STATUS_MODE;

    
    if( mciSendCommand(m_hDev, MCI_STATUS, MCI_STATUS_ITEM,
                         (DWORD)(LPVOID)&statusParms) == 0 )
    {
        if(statusParms.dwReturn == MCI_MODE_PAUSE)
            return wxMEDIASTATE_PAUSED;
        else if(statusParms.dwReturn == MCI_MODE_PLAY)
            return wxMEDIASTATE_PLAYING;
        else
            return wxMEDIASTATE_STOPPED;
    }

    wxLogSysError(wxT("Could not obtain state of media from")
                  wxT("MCI!"));
    return wxMEDIASTATE_STOPPED;
}

//---------------------------------------------------------------------------
// wxMCIMediaBackend::SetPosition
//
// Here we set the position of the device in the stream.
// Note that MCI actually stops the device after you seek it if the
// device is playing/paused, so we need to play the file after
// MCI seeks like normal APIs would
//---------------------------------------------------------------------------
bool wxMCIMediaBackend::SetPosition(wxLongLong where)
{
    MCI_SEEK_PARMS seekParms;
    seekParms.dwCallback = 0;
#if wxUSE_LONGLONG_NATIVE && !wxUSE_LONGLONG_WX
    seekParms.dwTo = (DWORD)where.GetValue();
#else /* wxUSE_LONGLONG_WX */
    /* no way to return it in one piece */
    wxASSERT( where.GetHi()==0 );
    seekParms.dwTo = (DWORD)where.GetLo();
#endif /* wxUSE_LONGLONG_* */

    //device was playing?
    bool bReplay = GetState() == wxMEDIASTATE_PLAYING;

    if( mciSendCommand(m_hDev, MCI_SEEK, MCI_TO,
                       (DWORD)(LPVOID)&seekParms) != 0)
        return false;

    //If the device was playing, resume it
    if (bReplay)
        return Play();
    else
    return true;
}

//---------------------------------------------------------------------------
// wxMCIMediaBackend::GetPosition
//
// Gets the position of the device in the stream using the current
// time format... nothing special here...
//---------------------------------------------------------------------------
wxLongLong wxMCIMediaBackend::GetPosition()
{
    MCI_STATUS_PARMS statusParms;
    statusParms.dwItem = MCI_STATUS_POSITION;

    if (mciSendCommand(m_hDev, MCI_STATUS, MCI_STATUS_ITEM,
                       (DWORD)(LPSTR)&statusParms) != 0)
        return 0;

    return statusParms.dwReturn;
}

//---------------------------------------------------------------------------
// wxMCIMediaBackend::GetVolume
//
// Gets the volume of the current media via the MCI_DGV_STATUS_VOLUME
// message.  Value ranges from 0 (minimum) to 1000 (maximum volume).
//---------------------------------------------------------------------------
double wxMCIMediaBackend::GetVolume()
{
    MCI_STATUS_PARMS statusParms;
    statusParms.dwCallback = 0;
    statusParms.dwItem = 0x4019; //MCI_DGV_STATUS_VOLUME

    if (mciSendCommand(m_hDev, MCI_STATUS, MCI_STATUS_ITEM,
                       (DWORD)(LPSTR)&statusParms) != 0)
        return 0;

    return ((double)statusParms.dwReturn) / 1000.0;
}

//---------------------------------------------------------------------------
// wxMCIMediaBackend::SetVolume
//
// Sets the volume of the current media via the MCI_DGV_SETAUDIO_VOLUME
// message.  Value ranges from 0 (minimum) to 1000 (maximum volume).
//---------------------------------------------------------------------------
bool wxMCIMediaBackend::SetVolume(double dVolume)
{
    MCI_DGV_SETAUDIO_PARMS audioParms;
    audioParms.dwCallback = 0;
    audioParms.dwItem = 0x4002; //MCI_DGV_SETAUDIO_VOLUME
    audioParms.dwValue = (DWORD) (dVolume * 1000.0);
    audioParms.dwOver = 0;
    audioParms.lpstrAlgorithm = NULL;
    audioParms.lpstrQuality = NULL;

    if (mciSendCommand(m_hDev, 0x0873, //MCI_SETAUDIO
                        //MCI_DGV_SETAUDIO+(_ITEM | _VALUE)
                        0x00800000L | 0x01000000L,
                       (DWORD)(LPSTR)&audioParms) != 0)
        return false;
    return true;
}

//---------------------------------------------------------------------------
// wxMCIMediaBackend::GetDuration
//
// Gets the duration of the stream... nothing special
//---------------------------------------------------------------------------
wxLongLong wxMCIMediaBackend::GetDuration()
{
    MCI_STATUS_PARMS statusParms;
    statusParms.dwItem = MCI_STATUS_LENGTH;

    if (mciSendCommand(m_hDev, MCI_STATUS, MCI_STATUS_ITEM,
                        (DWORD)(LPSTR)&statusParms) != 0)
        return 0;

    return statusParms.dwReturn;
}

//---------------------------------------------------------------------------
// wxMCIMediaBackend::Move
//
// Moves the window to a location
//---------------------------------------------------------------------------
void wxMCIMediaBackend::Move(int WXUNUSED(x), int WXUNUSED(y),
                                       int w,           int h)
{
    if (m_hNotifyWnd && m_bVideo)
    {
        MCI_DGV_RECT_PARMS putParms; //ifdefed MCI_DGV_PUT_PARMS
        memset(&putParms, 0, sizeof(MCI_DGV_RECT_PARMS));
        putParms.rc.bottom = h;
        putParms.rc.right = w;

        //wxStackWalker will crash and burn here on assert
        //and mci doesn't like 0 and 0 for some reason (out of range)
        //so just don't do it in that case
        if(w || h)
        {
            wxMCIVERIFY( mciSendCommand(m_hDev, MCI_PUT,
                                   0x00040000L, //MCI_DGV_PUT_DESTINATION
                                   (DWORD)(LPSTR)&putParms) );
        }
    }
}

//---------------------------------------------------------------------------
// wxMCIMediaBackend::GetVideoSize
//
// Gets the original size of the movie for sizers
//---------------------------------------------------------------------------
wxSize wxMCIMediaBackend::GetVideoSize() const
{
    if(m_bVideo)
    {
        MCI_DGV_RECT_PARMS whereParms; //ifdefed MCI_DGV_WHERE_PARMS

        wxMCIVERIFY( mciSendCommand(m_hDev, MCI_WHERE,
                       0x00020000L, //MCI_DGV_WHERE_SOURCE
                       (DWORD)(LPSTR)&whereParms) );

        return wxSize(whereParms.rc.right, whereParms.rc.bottom);
    }
    return wxSize(0,0);
}

//---------------------------------------------------------------------------
// wxMCIMediaBackend::GetPlaybackRate
//
// Obtains the playback rate of the media through MCI
//
// 1000 is a normal speed, while 2000 is double, 500 half etc.
//---------------------------------------------------------------------------
double wxMCIMediaBackend::GetPlaybackRate()
{
    MCI_STATUS_PARMS statusParms;
    statusParms.dwItem = 0x00004003L;//MCI_DGV_STATUS_SPEED;
 
    if (mciSendCommand(m_hDev, MCI_STATUS, MCI_STATUS_ITEM,
                        (DWORD)(LPSTR)&statusParms) != 0)
        return 0;

    return statusParms.dwReturn / 1000.0;
}

//---------------------------------------------------------------------------
// wxMCIMediaBackend::SetPlaybackRate
//
// Sets the playback rate of the media through MCI
//
// 1000 is a normal speed, while 2000 is double, 500 half etc.
//---------------------------------------------------------------------------
bool wxMCIMediaBackend::SetPlaybackRate(double dRate)
{
    MCI_DGV_SET_PARMS setParms;
    setParms.dwSpeed = (DWORD) (dRate * 1000.0);

    return (mciSendCommand(m_hDev, MCI_SET,
                       0x00020000L, //MCI_DGV_SET_SPEED
                       (DWORD)(LPSTR)&setParms) == 0);
}

//---------------------------------------------------------------------------
// [static] wxMCIMediaBackend::MSWWindowProc
//
// Here we process a message when MCI reaches the stopping point
// in the stream
//---------------------------------------------------------------------------
LRESULT CALLBACK wxMCIMediaBackend::NotifyWndProc(HWND hWnd, UINT nMsg,
                                                  WPARAM wParam,
                                                  LPARAM lParam)
{
    wxMCIMediaBackend* backend =
        (wxMCIMediaBackend*)wxGetWindowUserData(hWnd);

    return backend->OnNotifyWndProc(hWnd, nMsg, wParam, lParam);
}

LRESULT CALLBACK wxMCIMediaBackend::OnNotifyWndProc(HWND hWnd, UINT nMsg,
                                                  WPARAM wParam,
                                                  LPARAM lParam)
{
    if(nMsg == MM_MCINOTIFY)
    {
        wxASSERT(lParam == (LPARAM) m_hDev);
        if(wParam == MCI_NOTIFY_SUCCESSFUL && lParam == (LPARAM)m_hDev)
        {
            if ( SendStopEvent() )
            {
                wxMCIVERIFY( mciSendCommand(m_hDev, MCI_SEEK,
                                            MCI_SEEK_TO_START, 0) );

                QueueFinishEvent();
            }
        }
    }
    return DefWindowProc(hWnd, nMsg, wParam, lParam);
}


//in source file that contains stuff you don't directly use
//#include "wx/html/forcelnk.h"
//FORCE_LINK_ME(wxmediabackend_mci)

//---------------------------------------------------------------------------
//  End wxMediaCtrl Compilation Guard and this file
//---------------------------------------------------------------------------
#endif //wxUSE_MEDIACTRL
