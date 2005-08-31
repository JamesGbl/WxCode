
 WebUpdate component info
 ------------------------
 
 Website: http://wxcode.sourceforge.net/components/webupdate
 Version: 1.0
 
 Description:
 A complete, open source, cross-platform update solution for any type of application.
 Provides a easily-integrable, small application which connects to a web server and
 shows the updated packages to the user (support multiple packages) and let him download
 and install them.
 The program is highly configurable using XML and XRC files.
 For a complete list of the features of WebUpdate, look at the WebUpdate docs.



 Installation
 ------------

 When building on win32, you can use the makefiles in the BUILD folder.
 The USE_HTTPENGINE=0/1 option can be used to compile with/without the
 wxHttpEngine component.

 When building on a Linux/GNU-based system, type

  cd build
  ./configure
  make
 
 Type "./configure --help" for more info.
 The --with-httpengine/--without-httpengine option can be used to compile
 with/without the wxHttpEngine component.

 For any problem, feel free to contact frm@users.sourceforge.net



 Authors' info
 -------------

 Francesco Montorsi          frm@users.sourceforge.net

 Special thanks to Ulrich Telle for the great help given in design-stage and feature-proposal.
 Thanks to Angelo Mandato for its useful wxHttpEngine component.
 Thanks also to all wxWidgets developers who helped me in the wx-users mailing list.
