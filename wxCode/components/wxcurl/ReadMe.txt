
 wxCurl component info
 ---------------------

 Website: http://wxcode.sourceforge.net/components/wxcurl
 Version: 1.0

 Description:
 wxCURL is a simplified and integrated interface between LibCURL and wxWidgets.

 As libCURL authors state, "libcurl is free, thread-safe, IPv6 compatible, 
 feature rich, well supported, fast, thoroughly documented and is already used 
 by many known, big and successful companies and numerous applications."

 wxCURL provides several sub-classes for simplified interfaces to HTTP, WebDAV,
 FTP and Telnet based resources, threads specialized for non-blocking downloads/uploads,
 stock download and upload dialogs.


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

 1.0
     - first release of wxCurl since it was moved to wxCode
     - new bakefile-based build system which supports all possible wxWidgets build modes
       and allows an easy-build of libCURL also on Windows keeping a copy in thirdparty\libcurl
     - removed #pragma interface for GCC which are not used anymore
     - revised the code to make it compile against wxWidgets 2.9
     - now EVT_CURL_* macros also take an ID which allows the same event handler to process
       events coming from different wxCurlBase sessions
     - now wxCurlBase do not use anymore a boolean parameter to register if progress/begin/end
       events should be sent but rather the wxCURL_SEND_PROGRESS/BEGIN/END_EVENTS symbols which
       allow users to write more readable code
     - doxyfied the documentation of wxCURL
     - splitted wxCurlProgressEvent into wxCurlDownloadEvent and wxCurlUploadEvent classes
       and added more getters GUI-oriented (e.g. GetHumanReadableSpeed, GetEstimatedTime, etc)
     - added a wxCurlDownloadThread and a wxCurlUploadThread to ease non-blocking downloads/uploads
     - added a wxCurlSizeQueryThread to query file sizes before downloading them
     - added a wxCurlDownloadDialog and wxCurlUploadDialog for easier downloading/uploading
     - added a wxCurlConnectionSettings[Panel|Dialog] to present stock connection options to the user


 Authors' info
 -------------

 Casey O'Donnell  ckodonnell@users.sourceforge.net

 Contributions by:
   Marcio Valerio        mvalerio@users.sourceforge.net
   Ryan Wilcox           ryanwilcox@users.sourceforge.net
   Francesco Montorsi    frm@users.sourceforge.net      (wxCurl*Thread, wxCurl*Dialog)

