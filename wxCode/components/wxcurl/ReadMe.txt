
 wxCurl component info
 ---------------------

 Website: http://wxcode.sourceforge.net/components/wxcurl
 Version: 0.3

 Description:
 wxCURL is a simplified and integrated interface between LibCURL and wxWidgets.
 wxCURL provides several sub-classes for simplified interfaces to HTTP, WebDAV,
 FTP and Telnet based resources.


 Installation
 ------------

 When building on win32, you can use the makefiles in the BUILD folder.
 You can use the WX_DIR, WX_DEBUG, WX_VERSION, WX_SHARED, WX_MONOLITHIC, etc 
 options to match an existent wxWidgets build.

 When building on a Linux/GNU-based system, type

  ./configure
  make

 Type "./configure --help" for more info.



 ChangeLog
 ---------

 0.3
     - first release of wxCurl since it was moved to wxCode
     - new bakefile-based build system which supports all possible wxWidgets build modes
       and allows an easy-build of libCURL also on Windows keeping a copy in thirdparty\libcurl
     - removed #pragma interface for GCC which are not used anymore
     - now EVT_CURL_* macros also take an ID which allows the same event handler to process
       events coming from different wxCurlBase sessions
     - now wxCurlBase do not use anymore a boolean parameter to register if progress/begin/end
       events should be sent but rather the wxCURL_SEND_PROGRESS/BEGIN/END_EVENTS symbols which
       allow users to write more readable code
     - doxyfied the documentation of wxCURL

 Authors' info
 -------------

 Casey O'Donnell  ckodonnell@users.sourceforge.net

 Contributions by:
   Marcio Valerio        mvalerio@users.sourceforge.net
   Ryan Wilcox           ryanwilcox@users.sourceforge.net
   Francesco Montorsi    frm@users.sourceforge.net

