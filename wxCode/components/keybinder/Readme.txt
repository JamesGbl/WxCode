
 Keybinder component info
 ------------------------

 Website: http://wxcode.sourceforge.net/components/keybinder
 Version: 2.0
 Description:
 A generic system to bind one or more keyshortcut to the commands you want
 (menu commands or whatever you like).
 A pre-built panel and a complete system to view/remove/add/edit command
 shortcuts is included. Also key profiles are supported and a complete
 load/save system based on wxConfig is present.
 Integrates with your applications with few modifications and allows the
 users to customize all the commands he wants, bypassing the limitation of a
 single keybind to each command.



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

 None of course ;)




 Authors' info
 -------------

 Francesco Montorsi          frm@users.sourceforge.net

