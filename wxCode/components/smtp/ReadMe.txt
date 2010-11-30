
 wxSMTP ReadMe
 -------------

 Website: http://wxcode.sourceforge.net/components/wxsmtp
 Author: Frank Buß
 Maintainer: Brice André
 Version: 1.0
 Description:
 This component is a SMTP client implementation allowing sending emails. MIME format is supported, allowing sending plain text mails, but also HTML mails, with optional attachments and all standard fields (To, CC, BCC). The implementation is wxWidgets event-driven and allows retrieving the sending status of the mail for each recipient of the e-mail.



 Installation - win32
 --------------------

 When building on win32, you can use the makefiles in the BUILD folder using the
 WX_DIR, WX_DEBUG, WX_UNICODE and WX_SHARED to match your wxWidgets build.
 Examples:

   > nmake -fmakefile.vc WX_DIR=c:\path\to\my\wxWidgets WX_UNICODE=0/1 WX_DEBUG=0/1 WX_SHARED=0/1
   > make -fmakefile.bcc WX_DIR=c:\path\to\my\wxWidgets WX_UNICODE=0/1 WX_DEBUG=0/1 WX_SHARED=0/1



 Installation - unix
 -------------------

 When building on a Linux/GNU-based system, type

  cd build
  ./configure
  make

 to build against the default build of wxWidgets (the one listed as 'default'
 by wx-config --list). Type "./configure --help" for more info.



 ChangeLog
 ---------

 1.7 - first version of wxSMTP at wxCode

