 
 wxDockingFrame ReadMe
 -------------
 
 Website: http://wxcode.sourceforge.net/components/dockingframe
 Author: Barthemely Rolet
 Version: 1.0 
 Description:
 A frame dockable on the windows desktop, reducing the size of the desktop,
 it works a dock bar od "app bar".
 
 

 Installation - win32
 --------------------
the simplier is to use the VC++ projects
Currently only the  static lib is compiling.
You need to setup the WXWIN directory.

the sample is using the unicode static lib.

to use it, just include the .h and link with the static lib.
See the sample minimal for and exemple.

 
 When building on win32, you can use the makefiles in the BUILD folder using the
 WX_DIR, WX_DEBUG, WX_UNICODE and WX_SHARED to match your wxWidgets build.

 Examples:
 
   > nmake -fmakefile.vc WX_DIR=c:\path\to\my\wxWidgets WX_UNICODE=0/1 WX_DEBUG=0/1 WX_SHARED=0/1
   > make -fmakefile.bcc WX_DIR=c:\path\to\my\wxWidgets WX_UNICODE=0/1 WX_DEBUG=0/1 WX_SHARED=0/1


     
 Installation - unix
 -------------------

////////////////// WARNING //////////////////////////

-> Not working on Linux for now.

 When building on a Linux/GNU-based system, type

  cd build
  ./configure
  make

 to build against the default build of wxWidgets (the one listed as 'default' 
 by wx-config --list). Type "./configure --help" for more info.



 ChangeLog
 ---------
 
 1.0 - first version of wxDockingFrame at wxCode
 1.1 - Resize of the frame when docked, now resize the desktop area.
 
 [optionally: Known bugs, Author's info, ... ]

