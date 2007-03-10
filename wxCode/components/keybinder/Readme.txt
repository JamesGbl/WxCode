
 Keybinder component info
 ------------------------

 Website: http://wxcode.sourceforge.net/components/keybinder
 Version: 2.1
 Description:

 A generic system to bind one or more keyshortcut to the commands you want
 (menu commands or whatever you like).

 A pre-built panel and a complete system to view/remove/add/edit command
 shortcuts is included. Also key profiles are supported and a complete
 load/save system based on wxConfig is present.

 Integrates with your applications with few modifications and allows the
 users to customize all the commands they want, bypassing the limitation of a
 single key-shortcut to each command.



 Installation
 ------------

 When building on win32, you can use the makefiles in the BUILD folder.
 When building on Unix you should just do:

  ./configure [here you should use the same flags you used to configure wxWin]
  make
  make install

 Type "./configure --help" for more info.
 For any problem, feel free to contact frm@users.sourceforge.net



 ChangeLog
 ---------

 2.1

     - updated to the new bakefile build system,
     - revised code for compatibility with wxWidgets 2.8
     - now wxKeyConfigPanel does not use any "build mode" but rather stores
       the wxKEYBINDER_* flags into the wxWindow style
     - now debug messages are logged only if wxKEYBINDER_DEBUG_MSG is defined to 1
     - now wxKeyConfigPanel provides a regular Create() function like all other
       wxWidgets GUI controls/windows.



 Known bugs
 ----------

 None




 Authors' info
 -------------

 Francesco Montorsi          frm@users.sourceforge.net

