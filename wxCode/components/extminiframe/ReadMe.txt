
 wxExtMiniFrame component info
 -----------------------------

 Website: http://wxcode.sourceforge.net/components/extminiframe
 Version: 1.6
 Description:
 This component provides a wxMainFrame window which is able to handle one or more
 wxExtMiniframes. wxExtMiniFrame extends the wxWidgets' standard wxMiniFrame adding 
 the following main features:

  1) the ability to be activated/deactivated when the main frame
     activation state changes (wxMSW only): that is, when the
     mainframe is active, then also all the wxExtMiniFrame(s) are
     always drawn as active.

  2) the ability to contain any type of children, and to use
     wxSizers to layout them; this allows the creation of
     "advanced toolbars" (e.g. like MSOffice's ones)

  3) the ability to draw any type of wxMiniButton in its caption 
     area (some standard minibuttons are provided)

  4) the ability to be rolled/unrolled (maybe using the wxCollapseBox minibutton);
     useful to temporary make smaller big wxExtMiniFrame windows

  5) the ability to show a context menu, when right-clicked in the
     caption bar, to allow the user to show/hide a specific wxExtMiniFrame
     hold by the wxMainFrame

  6) the ability to save/load the layout of the wxExtMiniFrame(s)
     in the given wxConfig object

  7) the cross-platformness (wxMSW, wxX11 and wxGTK)



 wxExtMiniFrame sample
 ---------------------

 This is a little application (a modified version of the minimal sample taken from
 the wxWidgets library) that uses the wxExtMiniFrame class showing its features.

 For information about wxExtMiniFrame design, please read the sections below.




 Installation
 ------------

 When building on win32, you can use the makefiles in the BUILD folder.
 When building wxExtMiniFrame on Unix/Linux do:

  ./configure
  make
  make install

 Type "./configure --help" for more info.
 For any problem, feel free to contact frm@users.sourceforge.net


 ChangeLog
 ---------

 1.6

     - updated to the new bakefile build system,
     - revised code for compatibility with wxWidgets 2.8
     - fixed rendering of wxExtMiniFrame under wxGTK 2 (in particular like
       the ext miniframe title, the minibutton rendering, the XOR rectangle
       drawn when the miniframe is moved)


 DESIGN issues (that is, "How I wasted my time trying various approaches")
 -------------------------------------------------------------------------

 Another approach for the implementation of a window such as wxExtMiniFrame
 could be to derive a class from wxFrame and then, as I tried, to emulate the
 active state of the mainframe drawing a fake title bar over the real bar.
 This approach has two problems:
 1) under Win32 it generates some flickering, specially for big palettes
 2) under GTK it cannot be implemented because there's no way to draw the
    mainframe's titlebar as active when it is not
    (this is a more generic problem: X windowing system gives the control
     of the top level window appearance to the "Window managers"; the
     typical window managers are KDE, Gnome, Motif... the application have
     no control over the toplevel window appearance including their titlebars)

 Another first approach could also be to avoid accepting the focus on the 
 palette window and, even while the user is dragging/resizing the window,
 continuosly transfer focus from the palette to the main frame: like the
 previous solution, in this way flickering would produce a very nasty
 effect... (at least on Win32 where I tried it)





 FOCUS issues on Win32
 ---------------------

 One of the major features of wxExtMiniFrame is its ability to keep 
 always the same activation state of the wxMainFrame associated with  
 an user-intuitive interface.

 I managed to implement this feature on Win32 only for one reason:
 on GTK we miss some important information (the window which is going
 to be activated, for example) for events such as activation & nonclient 
 activation...

 This feature is much more difficult to achieve than it would seems
 because one fix rule of Win32 is that only one window at time can
 be the active window. There's no way to have two active windows
 at the same time... however, wxExtMiniFrame uses the same trick used
 by classes like CMiniFrameWnd in the MFC (that is, the classes used
 to create detachable toolbars): when the active window is the parent
 window, the wxExtMiniFrame draws itself with the color of an active
 caption even if it's not.

 The focus synchronization system must be able to handle the focus
 changes summed up in this table:

                 [0] = another app or a child of [1], which is not [2]
                 [1] = the wxMainFrame which owns [2]
                 [2] = a wxExtMiniFrame owned by [1]


  focus   |            what must be done            | the function which
  change  |                                         |       does it
 ---------+-----------------------------------------+-------------------
 [0]->[1] | [1] becomes ACTIVE;                     | wxMainFrame::
          | all wxExtMiniFrames must be ACTIVATED   |   OnActivate 
          |                                         |              
 [1]->[0] | [1] becomes INACTIVE;                   | wxMainFrame::
          | all wxExtMiniFrames must be DEACTIVATED |   OnActivate 
          |                                         |              
 [1]->[2] | [1] doesn't update its titlebar (that   | wxMainFrame::
          | is, it keeps its ACTIVE color);         |   MSWOnNcActivate
          |                                         |                  
 [2]->[1] | [1] should already be drawn as ACTIVE   | wxExtMiniFrame:: 
          | [2] must keep its ACTIVE color          |   OnActivate     
          |                                         |                  
 [2]->[0] | [2] should notify [1] that it must be   | wxExtMiniFrame:: 
          | drawn as INACTIVE like all its palettes |   MSWOnActivate  
          |                                         |                  
 [0]->[2] | [2] should notify [1] that it must be   | wxExtMiniFrame:: 
          | drawn as ACTIVE like all its palettes   |   MSWOnActivate  



 Known bugs
 ----------

 -> under wxMSW, there is some flickering when minibuttons are redrawn
 -> under wxX11, there is some problem with wxConfig... this is a wxWidgets bug
 -> under wxGTK, there is a strange line of pixels drawn with wrong colour immediately
    under the minibuttons



 Authors' info
 -------------

 Francesco Montorsi          frm@users.sourceforge.net

 Thanks to Ajitha Pai <ajithapai@users.sourceforge.net> for the help.

