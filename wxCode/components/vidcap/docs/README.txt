===============================================================================
wxVidCap - readme
===============================================================================

wxVidCap was written by John Labenski, copyright 2001
         jrl1[AT]lehigh[DOT]edu

---------------------------------------------------------------------------
wxVidCap is a capture class/program for MSW's Video For Windows (VFW) 1.1 api
and kinda' for video 4 linux (v4l) using the wxWidgets cross-platform library. 
It is distributed under the wxWidgets v2 license, free for any purpose. 

Please feel free to ask me any questions you may have regarding it. 
I would also greatly appreciate any bugfixes or enhancements you may make.

It should work on any MS Windows operating system (95,98,NT,2k,XP) that 
supports the Video For Windows 1.1 API. VFW is an old (dating back to Win 3.1) 
method of capturing video. Microsoft has depricated it and apparently some newer
video devices only support DirectX. If your device doesn't show up in the list
of connected devices please verify that your card/camera has drivers for VFW.
Other programs to try are vidcap32pb1exe.zip and Netmeeting since they use VFW.

Devices that work should be any USB camera, ATI all in Wonder cards, BT8x8 cards.    

-------------------------------------------------------------------------------
note: wxvidcap.png shows a "Caution" message.
      This is ONLY meant for that particular dialog since some drivers
      (Kensington video cams) do not gracefully fail when you set invalid 
      parameters, but crash the whole system, sigh...
      VFW does not have a method to let you get the supported parameters.
      The wxVideoCaptureWindow class is rather stable, as far as I know.

-------------------------------------------------------------------------------
Developers - The "documentation" is in the header files. There is a Makefile for
    Linux and a .dsp for Visual C++. The Makefile creates a library in your
    wxWidgets/lib dir and the MSVS project file creates a lib in a 
    wxVidCap/src/vc_lib dir. The dsp file uses the environment variable 
    $(WXWIN) to find the directory of your wxWidgets library.

Directory structure (for releases newer than 12/21/04)
    wxVidCap/
        - include/wx/vidcap/ - public headers vcapwin.h
            - vfw/ - private header for video for windows
            - v4l/ - private header for video 4 linux
        - src/ - source files
        - samples/vidcap/ - the wxVidCap sample program

-------------------------------------------------------------------------------
FAQ :

  1) My camera no longer works in MSW?! VFW is a little unstable, if you disconnect
     a camera while playing/recording or another program crashes you can probably
     bet that VFW is dead. There is nothing you can do except reboot.
  2) Why can't I preview/grab frames in xyz format using wxImages? For some bizarre reason 
     VFW can decode the frames and preview it, but when you ask VFW for an appropriate
     decompressor to 24BPP you get nothing. Unfortunately, there are so many 
     proprietary undocumented video formats its probably not worth bothering to 
     reverse engineer that nonsense. Try to use another format that your camera 
     supports.

-------------------------------------------------------------------------------
Changelog :

wxVidCap_122104.tar.gz
    This release cleans up the code by fixing unused variable warnings, 
    separating the headers, source files, and sample program into their own 
    directories. The MSVS project files create a library and the sample dsp/dsw 
    project files show how to link to it. Added project files for wxWidgets
    2.5.x versions in addition to cleaning up the previous 2.4.x versions.
    
    The window names are now wxVideoCaptureWindowVFW/V4L in the headers and 
    typedefed into wxVideoCaptureWindow (similiar to wxWindowMSW/GTK). 
    This will make it fairly easy to add a DirectShow version and perhaps even
    use it and the VFW together at the same time (different devices of course).
    
    Added WXDLLIMPEXP_VIDCAP for use in a DLL, not tested, but it should work.
    
    As before the VFW version works great, even in XP. The linux V4L is still in
    rough shape and doesn't work.

BETA - for MSW and (just barely) GTK 
wxVidCap_042503_BETA.zip - source code
wxVidCap_042503_BETA_exe.zip - executable
    Everything is called wxVideoCaptureWindow now, if you used the old version
    just find and replace. I thought the old name (wxVideoWindow) was far too much 
    like the multimedia contrib that uses wxVideoWindows.

    There's a base class now.  Basicly this works the same as most
    wxWindows classes, you have a base class w/ virtual functions, each port
    subclasses it and fills in the blanks. 

    Parts that are not implemented on both platforms are not in the base class,
    but there are some handy defines. See wxvidcap.cpp to see how I use them to 
    turn off stuff not supported in Linux. See vidcapwin.h for the base class.

    MSW - everything is fine and cleaner and what not, this is probably your best choice.

    Linux - no saving video, frames, audio, you can barely preview it in one mode and
            its probably writing all over itself in the process.
            It sounds sad, but all that really needs to be done is writing/finding
            video conversion functions, the remains of wxLivid will be my source.
            Saving video, I guess it'll have to dynamicly link to one of the GPL'd
            video saving libraries, this class is under the more liberal wxWindows
            license.

STABLE -  for wxWindows MSW ver >= 2.4
wxVidCap_042503.zip - source code
   Thanks to Stefan Heinsen this is now Unicode aware. Sorry no exe since I don't have 
   time to rebuild everything in Unicode, is win 98 unicode capable? dunno.

OLDish - for wxWindows MSW ver >= 2.4
wxVidCap_011403_exe.zip - executable only
wxVidCap_011403.zip - source code
    Mostly the same as 070601, patched to use the new event types
    of wxWindows 2.4, by Janos Vegh (thanks), and some other small tweaks like 
    passing const wxString& and what not. It also does not immediately try to preview
    the first device as soon as the the wxVideoWindow is created since some drivers 
    (ATI All In Wonder) install a few "extra" devices that don't even work and so you 
    have to click through error messages at startup and try to find the correct one.

OLD - for wxWindows MSW ver < 2.3
wxvidcap070601a_exe.zip is the executable (a capture/preview program)
wxvidcap070601a.zip is the source (makefiles for VC++ 6)

TEST PROGRAM
vidcap32pb1xxx.zip is a slightly modified (not by me) version of MSW's vidcap.exe 
    generic capture program. Please use this to confirm any bugs/features if possible.
    It has been changed to capture to multiple files to exceed the 2Gb filesize limit.