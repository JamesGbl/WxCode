
 wxExtMiniFrame component info
 -----------------------------

 Website: http://wxcode.sourceforge.net/components/paletteframe
 Version: 2.0 
 Description:
 A base class for creation of MS-like palette windows which can be used as 
 advanced (they expose a simple wxPanel which can contain any control you want) toolbars.
 The appearance is like wxMiniFrame but a lot of new features have been added.
 Unicode is full supported as well.



 wxPaletteFrame sample
 ---------------------
 
 This is a little application (a modified version of the minimal sample taken from
 the wxWidgets library) that uses the wxPaletteFrame class showing its features.

 For information about wxPaletteFrame design, please read the comments placed at
 the beginning of "wx/palettefrm.h".

 

 Installation
 ------------

 When building on win32, you can use the makefiles in the BUILD folder.
 In particular, when building wxscript on an autoconf-based system (like Linux/GNU-based systems),
 you have to create the makefile doing:

  cd build
  aclocal
  autoconf
  ./configure [here you should use the same flags you used to configure wxWin]
  make
 
 Type "./configure --help" for more info.
 The autoconf-based systems also support a "make install" target which builds the library and then
 copies the headers of the component to /usr/local/include and the lib to /usr/local/lib.
 For any problem, feel free to contact frm@users.sourceforge.net



 Known bugs
 ----------

 -> under wxMSW, there is some flickering when minibuttons are redrawn
 -> under wxX11, there is some problem with wxConfig... this is a wxWidgets bug
 -> under wxGTK, when compiling wxGTK with the "--enable-unicode --disable-debug" options,
    the minibuttons are not displayed at all; this is going to be fixed when also wxGTK
    wxMiniFrame's same bug will be fixed.


 Authors' info
 -------------

 Francesco Montorsi          frm@users.sourceforge.net
 Ajitha Pai                  ajithapai@users.sourceforge.net

