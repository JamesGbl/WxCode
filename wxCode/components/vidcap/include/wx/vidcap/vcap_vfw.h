/////////////////////////////////////////////////////////////////////////////
// Name:        vcap_vfw.h - wxVideoCaptureWindow using MSW VFW 1.1 API
// Author:      John Labenski
// Created:     7/06/2001
// Modified:    01/14/03
// Copyright:   John Labenski
// License:     wxWidgets V2.0
/////////////////////////////////////////////////////////////////////////////
//
// Usage notes:
// Link against vfw.lib
// Read the header and the cpp file to figure out what something does
// Test "problems" against MSW vidcap.exe to see if the same problem occurs

#ifndef __WX_VCAP_VFW_H__
#define __WX_VCAP_VFW_H__

#include <wx/defs.h>

#if defined(__WXMSW__)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#include <windows.h>    // MSW headers
#include <windowsx.h>   // for GlobalAllocPtr to set audio parameters
#include <vfw.h>        // Video For Windows 1.1

//---------------------------------------------------------------------------
/** @class wxVideoCaptureWindowVFW
    @brief A wxVideoCaptureWindow for MSW's VideoForWindows to view/record
           streaming video or take snapshots.
*/ // -----------------------------------------------------------------------

class WXDLLIMPEXP_VIDCAP wxVideoCaptureWindowVFW: public wxVideoCaptureWindowBase
{
public:
    wxVideoCaptureWindowVFW() : wxVideoCaptureWindowBase() { Init(); }
    wxVideoCaptureWindowVFW( wxWindow *parent, wxWindowID id = wxID_ANY,
                             const wxPoint &pos = wxDefaultPosition,
                             const wxSize &size = wxDefaultSize,
                             long style = wxSIMPLE_BORDER,
                             const wxString &name = wxT("wxVideoCaptureWindow") );

    bool Create( wxWindow *parent, wxWindowID id = wxID_ANY,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 long style = wxSIMPLE_BORDER,
                 const wxString &name = wxT("wxVideoCaptureWindow") );

    virtual ~wxVideoCaptureWindowVFW();

    // ----------------------------------------------------------------------
    /// @name Device descriptions & versions, get and enumerate
    // ----------------------------------------------------------------------
    /// @{

    virtual bool EnumerateDevices();

    /// @}
    // ----------------------------------------------------------------------
    /// @name Connect or Disconnect to device
    // ----------------------------------------------------------------------
    /// @{

    virtual bool IsDeviceInitialized();
    virtual bool DeviceConnect(int index);
    virtual bool DeviceDisconnect();

    /// @}
    // ----------------------------------------------------------------------
    /// @name Display dialogs to set/get video characteristics
    // ----------------------------------------------------------------------
    /// @{

    virtual bool HasVideoSourceDialog();
    virtual void ShowVideoSourceDialog();

    virtual bool HasVideoFormatDialog();
    virtual void ShowVideoFormatDialog();
    virtual void ShowVideoCustomFormatDialog();

    virtual bool HasVideoDisplayDialog();
    virtual void ShowVideoDisplayDialog();

    /// Dialog to show the available compression codecs used for capture.
    ///   VFW - MSW system dialog.
    void ShowVideoCompressionDialog();

    /// Dialog to setup most of the capture preferences
    ///   shows only whats available on each system.
    void ShowCapturePreferencesDialog();

    /// Dialog to set the audio channels, bits/sample, samples/second.
    ///   VFW - works only if HasAudioHardware() is true, i.e. you have a sound card.
    void ShowAudioFormatDialog();

    virtual wxString GetPropertiesString();

    // ----------------------------------------------------------------------
    /// @name Video format and characteristics
    // ----------------------------------------------------------------------
    /// @{

    virtual bool GetVideoFormat( int *width, int *height,
                                 int *bpp, FOURCC *fourcc ) const;

    //***********************************************************************
    /// WARNING! - Video For Windows
    /// SetVideoFormat is not for the faint of heart or people w/o saved data!
    /// At worst it'll crash your system, some drivers don't gracefully fail.
    /// There doesn't seem to be a way to get supported values for a device.
    /// This function lets you set them to anything, use the ShowVideoFormatDialog()
    /// which the driver supplies to be on the safe side.
    virtual bool SetVideoFormat( int width, int height,
                                 int bpp, FOURCC fourcc );

    virtual bool IsUsingDefaultPalette();
    virtual bool DriverSuppliesPalettes();

    /// @}
    // ----------------------------------------------------------------------
    /// @name Capture Preview and Overlay
    // ----------------------------------------------------------------------
    /// @{

    virtual bool Preview(bool on, bool wxpreview = false);
    virtual bool PreviewScaled(bool on);
    virtual bool SetPreviewRateMS( unsigned int msperframe = 66 );
    virtual bool Overlay(bool on);

    void OnPreviewTimer(wxTimerEvent& event);

    /// @}
    // ----------------------------------------------------------------------
    /// @name Capture single frames, take snapshots of streaming video
    // ----------------------------------------------------------------------
    /// @{

    virtual bool SnapshotToWindow();
    virtual bool SnapshotToClipboard();
    virtual bool SnapshotToBMP( const wxString &filename );
    virtual bool SnapshotTowxImage( wxImage &image);
    virtual bool SnapshotTowxImage();

    /// @}
    // ----------------------------------------------------------------------
    /// @name Capture (append) single video frames to an AVI file
    // ----------------------------------------------------------------------
    /// @{

    // Overview: These frames will be saved to the SetCaptureFilename() file
    // USE: CaptureSingleFramesToFileOpen then CaptureSingleFrames...
    //   (see SetAviMaxIndexEntries() ) then CaptureSingleFramesToFileClose

    //   SaveCapturedFileAs() can be used to extract video to new file
    //   this is useful only if the the file is larger than the # of frames
    //   eg. you've called SetCaptureFilesizeMB to create a larger than necessary file

    /// Get how many frames we've taken, reset to -1 when closed.
    int GetCapturedFramesToFileCount() { return m_capsingleframecount; }

    /// Open an AVI file to capture single frames into it, returns success.
    bool CaptureSingleFramesToFileOpen();
    /// Capture single frames into the AVI file, returns success.
    bool CaptureSingleFramesToFile();
    /// Close the AVI file when done, returns success.
    bool CaptureSingleFramesToFileClose();

    /// A simple dialog to capture single frames to an AVI file.
    /// It sets up the file and just click ok to save another frame.
    void CaptureSingleFramesToFileDialog();

    /// @}
    // ----------------------------------------------------------------------
    /// @name Capture streaming video to an AVI file
    // ----------------------------------------------------------------------
    /// @{

    /// Record video segment, returns success.
    /// Starting/stopping controlled by setting up capture parameters.
    bool CaptureVideoToFile();

    /// Stop capturing video & audio to AVI file, close file and end.
    /// These two must use capSetCallbackOnYield, I think? UNTESTED
    bool CaptureVideoToFileStop();
    /// Abort capturing video to AVI file and "discard" data.
    /// notes: Capture operation must yield to use this macro
    ///        image data is retained, audio is not. UNTESTED
    bool CaptureVideoToFileAbort();

    /// Returns true if a capture is in progress.
    bool IsCapturingNow();
    /// Get the time elapsed since the start of current/recent capture.
    unsigned long int GetCaptureTimeElapsedMS();
    /// Get the number of frames processed during current/recent capture,
    /// includes dropped frames.
    unsigned long int GetCapturedVideoFramesCount();
    /// Get the number of frames dropped during current/recent capture.
    /// When a frame is dropped, the previous frame is put into the AVI file
    /// to not affect audio syncronization.
    unsigned long int GetCapturedVideoFramesDropped();

    /// Get the set number of microsecond/frame to record with.
    unsigned long int GetMicroSecPerFrameRequested();
    /// Set microsecond/frame to record with, default = 66667us = 15fps
    /// not necessarily what you'll get if fps set too high
    void SetMicroSecPerFrameRequested( unsigned long int framespersec);

    /// Get keycode to terminate streaming video.
    unsigned int GetAbortKey();
    /// Set keycode to terminate streaming video, default = VK_ESCAPE.
    /// NOTE use RegisterHotKey() for system wide key, NOT IMPLEMENTED, NOT TESTED.
    void SetAbortKey( unsigned int key );
    /// Get if the video capture will be aborted on left mouse button.
    bool GetAbortLeftMouse();
    /// Set if the video capture will be aborted on left mouse button, default = true.
    void SetAbortLeftMouse( bool leftmouse );
    /// Get if the video capture will be aborted on right mouse button.
    bool GetAbortRightMouse();
    /// Set if the video capture will be aborted on right mouse button, default = true.
    void SetAbortRightMouse( bool rightmouse );

    /// Get if a time limit was put on the capture, @see GetCaptureTimeLimit().
    bool GetTimeLimitedCapture();
    /// Set if a time limit should be put on the capture, @see SetCaptureTimeLimit().
    void SetTimeLimitedCapture( bool usetimelimit );
    /// Get the number of seconds to capture video for, @see GetTimeLimitedCapture().
    unsigned int GetCaptureTimeLimit();
    /// Set the number of seconds to capture video for, @see SetTimeLimitedCapture().
    void SetCaptureTimeLimit( unsigned int timelimit );

    /// Get if a popup "OK" dialog is shown to initiate capture.
    bool GetDialogInitiatedCapture();
    /// Set if a popup "OK" dialog is shown to initiate capture, default = false.
    void SetDialogInitiatedCapture( bool usedialog );

    /// Get the max allowed % of frames dropped.
    unsigned int GetMaxAllowedFramesDropped();
    /// Set the max allowed % of frames dropped, default=10 [0..100], popup error dialog if exceeded.
    /// Note: Dropped frames are replaced by the last grabbed frame in AVI file.
    void SetMaxAllowedFramesDropped(unsigned int maxdrop );

    /// Get the actual number of video buffers allocated in the memory heap.
    unsigned int GetNumVideoBuffersAllocated();
    /// Get the max number of video buffers to allocate, if 0 then autocalc, dunno?
    unsigned int GetNumVideoBuffers();
    /// Set the max number of video buffers to allocate.
    /// The default values are probably fine, but who knows?
    void SetNumVideoBuffers( unsigned int vidbufs );

    /// Get if a background should be used thread to capture.
    bool GetUseThreadToCapture();
    /// Set if a background should be used thread to capture, controls NOT disabled, default=false.
    /// See VFW_CallbackOnCaptureYield() also VFW's capSetCallbackOnYield()... NOT TESTED.
    void SetUseThreadToCapture( bool usethread );

    /// Get if capture should use double resolution.
    bool GetStepCaptureAt2x();
    /// Set if capture should use double resolution, (height&width*2).
    /// VFW : "enable if hardware doesn't support decimation & using RGB" huh?
    /// ??? does this work? and for what device?
    void SetStepCaptureAt2x( bool cap2x);

    /// Get the number of times a frame is sampled for average frame, typically 5.
    unsigned int GetStepCaptureAverageFrames();
    /// Set the number of times a frame is sampled for average frame, typically 5.
    /// ??? does this work? and for what device?
    void SetStepCaptureAverageFrames( unsigned int aveframes );

    /// Get the max number of index entries in AVI file [1800..324,000].
    unsigned long int GetAviMaxIndexEntries();
    /// Set the max number of index entries in AVI file [1800..324,000].
    /// Each frame or audio buffer uses 1 index, sets max num of frames.
    /// default = 34,952 (32K frames + proportional num of audio buffers).
    void SetAviMaxIndexEntries( unsigned long int maxindex);

    /// Get the logical block size in an AVI file in bytes.
    unsigned int GetChunkGranularity();
    /// Set the logical block size in an AVI file in bytes.
    ///  - 0 = current sector size, recommended default=2K
    ///  - if larger than the data structure, the rest is filled w/ RIFF "JUNK"
    ///  - 64,128,256,512,1K,2K,4K,8K,16K,32K are valid... I think?
    void SetChunkGranularity( unsigned int chunkbytes );

    // NOTE capFileSetInfoChunk can embed "data" in an AVI file, why bother?

    /// @}
    // ----------------------------------------------------------------------
    /// @name Capture file settings, filename to capture video to
    // ----------------------------------------------------------------------
    /// @{

    // default file for VFW is c:\capture.avi
    // Overview: SetCaptureFilename() sets the file to capture AVI video to.
    // SetCaptureFilesizeMB() preallocates space for the video, make > needed
    // SaveCapturedFileAs() then copies only the actual video to a new file
    // or just set the filesize < than the needed, but take performance hit

    // Set the name of a capture file, used with SetCaptureFilesizeMB
    //     MSW wants you to set the name, preallocate space, capture and then
    //     use SaveCapturedFileAs() to extract the video to a new file.
    //     The idea is a permanent capture file exists for best performance

    /// Get if a valid capture file exists.
    bool CaptureFileExists();
    /// Get the name of the AVI file to capture video to.
    wxString GetCaptureFilename();
    /// Set the name of the AVI file to capture video to, returns success.
    /// Doesn't create, open, or allocate.
    bool SetCaptureFilename(const wxString &capfilename);
    /// Get the capture filename from MSW AVIcap window, returns success (internal use only).
    bool VFW_GetCaptureFilename();
    /// Dialog to ask the user what capture filename to use, returns success.
    bool SetCaptureFilenameDialog();

    /// Preallocate space for the capture file, returns success.
    /// This is to get a fixed "contiguous" file to capture to.
    /// However the user must run a defrag program to guarantee it's contiguous.
    /// If the captured data is smaller, then the rest of the file is just junk.
    /// If set to zero or smaller than the actual capture size or if the file
    /// doesn't exist there be a performance loss...
    bool SetCaptureFilesizeMB(unsigned int filesizeMB=0 );
    /// Show a dialog to preallocate space for video capture, returns success.
    bool ShowCaptureFileSizeDialog();

    /// Extract the actual data from the SetCaptureFilename() and save it to a new file.
    /// This is used to create a file that is correctly sized to the video data size.
    /// If not enough space on disk for this file then it's deleted.
    bool SaveCapturedFileAs( const wxString &filename );

    /// Get the amount of free disk space in KB.
    /// I guess that newer versions of wxWidgets will have these? 2.3?
    long int GetFreeDiskSpaceInKB( const wxString &filepath );

    /// @}
    // ----------------------------------------------------------------------
    /// @name Audio Setup
    // ----------------------------------------------------------------------
    /// @{

    /// Get if the device has waveform-audio, usually means a sound card.
    bool HasAudioHardware();

    /// Set the audio recording properties, see audio enumerations.
    /// - channels = 1, 2 (mono, stereo)
    /// - bitspersample = 8, 16
    /// - samplespersecond = 8000,11025,16000,22050,24000,32000,44100,48000 (freq)
    /// note:
    ///   - CD = stereo, 16bit, 44100
    ///   - Radio = mono, 8bit, 22050
    ///   - telephone = mono, 8bit, 11025
    bool SetAudioFormat( int channels = 1,
                         int bitspersample = 8,
                         int samplespersecond = 11025 );

    /// Get the audio format, @see SetAudioFormat(), returns success.
    bool GetAudioFormat( int *channels ,
                         int *bitspersample ,
                         int *samplespersecond );

    /// Get if audio should be captured with the video.
    bool GetCaptureAudio();
    /// Set to capture audio with the video, default = true if device has audio capability.
    void SetCaptureAudio( bool capaudio );

    /// Get the max number of audio buffers.
    unsigned int GetNumAudioBuffers();
    /// Set the max number of audio buffers, (max = 10), if 0 then autocalc, default = 4.
    void SetNumAudioBuffers( unsigned int audiobufs );

    /// Get the number of audio buffers allocated.
    unsigned int GetNumAudioBuffersAllocated();
    /// Get the audio buffer size.
    unsigned long int GetAudioBufferSize();
    /// Set the audio buffer size, default = 0 = max of either 1/2 sec or 10Kb.
    void SetAudioBufferSize( unsigned long int audiobufsize );

    /// Get if the audio stream controls clock for AVI file.
    bool GetAudioStreamMaster();
    /// Set if the audio stream controls clock for AVI file.
    /// Video duration is forced to match audio duration if false they can differ in length.
    void SetAudioStreamMaster( bool audiomaster);

    /// Get the number of waveform-audio samples processed during current/recent capture.
    unsigned long int GetCapturedWaveSamplesCount();

    /// @}
    // ----------------------------------------------------------------------
    /// @name Utility Functions
    // ----------------------------------------------------------------------
    /// @{

    // nothing here

    /// @}
    // ----------------------------------------------------------------------
    /// @name MSW VFW Callbacks call these wxVideoCaptureWindow functions.
    ///       SetCallbackOnXXX() turns them on/off.
    // ----------------------------------------------------------------------
    /// @{

    /// Set if VFW should call our error callback.
    bool VFW_SetCallbackOnError(bool on);
    /// Called when a nonfatal error occurs.
    /// reports: no disk space, read-only file, can't access hardware,
    ///          too many frames dropped, and others?
    virtual bool VFW_CallbackOnError(const wxString &errortext, int errorid);

    /// Set if VFW should call our status callback.
    bool VFW_SetCallbackOnStatus(bool on);
    /// Called when the device's status changes (usually).
    virtual bool VFW_CallbackOnStatus(const wxString &statustext, int statusid);

    /// Set if VFW should call our frame callback.
    bool VFW_SetCallbackFrame(bool on);
    /// Called when preview frames are available for software previewing.
    /// Previewing or not, when rate is > 0 and streaming capture is not in progress.
    virtual bool VFW_CallbackOnFrame(LPVIDEOHDR lpVHdr);

    /// Set if VFW should call our yield callback.
    bool VFW_SetCallbackOnCaptureYield(bool on);
    /// Called during capture (between frames) to update the gui.
    virtual bool VFW_CallbackOnCaptureYield();

    /// Set if VFW should call our capture control callback.
    bool VFW_SetCallbackOnCaptureControl(bool on);
    /// Called after preroll (capture ready) and between capture frames.
    virtual bool VFW_CallbackOnCaptureControl(int nState);

    /// Set if VFW should call our stream callback.
    bool VFW_SetCallbackOnVideoStream(bool on);
    /// Called before video frames are written to an AVI file.
    virtual bool VFW_CallbackOnVideoStream(LPVIDEOHDR lpVHdr);

    /// Set if VFW should call our wave stream callback.
    bool VFW_SetCallbackOnWaveStream(bool on);
    /// Called during capture before audio buffers written, can be modified here.
    virtual bool VFW_CallbackOnWaveStream(LPWAVEHDR lpWHdr);

    /// @}
protected:

    // ----------------------------------------------------------------------
    /// @name Platform dependent video conversion
    // ----------------------------------------------------------------------
    /// @{

    /// Decompress a video frame into the m_wximage member variable, returns success.
    /// Device Dependent Bitmap -> 24bpp Device Independent Bitmap -> m_wximage.
    bool VFW_DDBtoDIB(LPVIDEOHDR lpVHdr);

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

    /// Move m_hWndC when EVT_SCROLLWIN occurs when Overlaying.
    void OnScrollWin( wxScrollWinEvent &event );

    /// Scroll the m_hWndC to this point, necessary for Overlay only.
    bool VFW_ScrollHWND(int x, int y);

    /// x,y offset of the upperleft corner of MSW capture window.
    wxPoint VFW_GetHWNDViewStart();

    /// Called by wxWindow's EVT_MOVE, make Overlay window follow.
    void OnMove( wxMoveEvent &event );

    /// Draw the frames when using wxImages preview from EVT_PAINT.
    void OnPaint( wxPaintEvent &event );

    /// @}
    // ----------------------------------------------------------------------
    // Member Variables
    // ----------------------------------------------------------------------

    // Generic variables

    unsigned char *m_bmpdata;       ///< big 'ole temp storage for DIB.

    bool m_grab_wximage;            ///< Grab a single frame into m_wximage.

    wxString m_capturefilename;

    long int m_capsingleframecount; ///< Number of frames taken, -1 when closed, >= 0 if open.

    wxString m_statusMessage;       ///< MSW status messages.

    // MSW specific variables
    HWND m_hWndC;                   ///< The VFW vidcap HWND, it does everything.

    HIC m_hic_compressor;           ///< Handle to device (de)compressor.

    BITMAPINFO *m_lpBmpInfo24bpp;   ///< Bitmap header for 24bpp bitmaps.
    BITMAPINFO *m_lpBmpInfo;        ///< Storage for video bitmap header.
    BITMAPINFO *m_lpBmpInfoLast;    ///< Storage for video bitmap header.

    // ------------------------------------------------------------------------
    CAPDRIVERCAPS m_CAPDRIVERCAPS;  ///< Capture Driver Capabilities.
    bool VFW_GetCAPDRIVERCAPS();    ///< Used internally to fill m_CAPDRIVERCAPS.

    // Parts of CAPDRIVERCAPS that are not used
    // HANDLE hVideo[In,Out,ExitIn,ExtOut] are unused in Win32, always NULL.

    // ------------------------------------------------------------------------
    CAPSTATUS m_CAPSTATUS;           ///< Current status of the capture driver.
    bool VFW_GetCAPSTATUS();         ///< Used internally to fill m_CAPSTATUS.

    // Parts of CAPSTATUS that are not used
    // HPALETTE hPalCurrent - Current palette in use
    // DWORD dwReturn - Error code after any operation, 32 bit unsigned int, StatusCallback does this

    // ------------------------------------------------------------------------
    CAPTUREPARMS m_CAPTUREPARMS;    ///< Capture Parameters.
    bool VFW_GetCAPTUREPARMS();     ///< Used internally to get m_CAPTUREPARMS.
    bool VFW_SetCAPTUREPARMS();     ///< used internally to set m_CAPTUREPARMS.

    // Parts of CAPTUREPARMS that are unused
    // BOOL fUsingDOSMemory - unused in win32
    // BOOL fMCIControl - controlling a MCI compatible device, VCR, laserdisc
    // BOOL fStepMCIDevice - MCI device step capture if true, false is streaming
    // DWORD dwMCIStartTime - starting position in ms, for MCI device capture
    // DWORD dwMCIStopTime - stopping position in ms for MCI device capture
    // BOOL fDisableWriteCache - not used in Win32

private:
    void Init();
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxVideoCaptureWindowVFW)
};

#include <wx/msw/winundef.h>

#endif // defined(__WXMSW__)

#endif //__WX_VCAP_VFW_H__
