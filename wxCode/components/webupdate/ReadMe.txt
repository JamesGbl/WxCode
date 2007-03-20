
 WebUpdate component info
 ------------------------

 Website: http://wxcode.sourceforge.net/components/webupdate
 Version: 1.2

 Description:
 A complete, open source, cross-platform update solution for any type of application.
 Provides a easily-integrable, small application which connects to a web server and
 shows the updated packages to the user (supports multiple packages) and let him download
 and install them.

 Reliable downloads are granted using MD5 checksums. Local updates (e.g. updates to be taken
 from a CD-ROM or any other device) are supported, too.
 The program is highly configurable using XML and XRC files.
 For a complete list of the features of WebUpdate, look at the WebUpdate docs.


 License
 -------

 wxWidgets library license applies to WebUpdate code if you use it as
 a library (for e.g. your custom webupdater); that is you can use both
 in other open source apps and in other commercial products.
 If you use WebUpdate binaries directly, then the MIT license applies:

   Copyright (c) 2006 Francesco Montorsi

   Permission is hereby granted, free of charge, to any person 
   obtaining aing a copy of this software and associated documentation 
   files (the "Software"), to deal in the Software without restriction, 
   including without limitation the rights to use, copy, modify, merge, 
   publish, distribute, sublicense, and/or sell copies of the Software, 
   and to permit persons to whom the Software is furnished to do so, subject 
   to the following conditions:

   The above copyright notice and this permission notice shall be 
   included in all copies or substantial portions of the Software.

 (this is because wxWidgets license is for libraries and not for final
  applications). In any case, both if you WebUpdater in free or commercial
  products, some credits to the author would be very much appreciated
  even if not strictly required.


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



 Change Log
 ----------

 1.2 - completely revised the build system;
       revised the download code to be more thread-safe and more reliable;
       revised a bit the GUI (e.g. now it uses wxHyperlinkCtrl in the about dialog);
       separed library sources from the webupdater app sources (it's cleaner this way);
       

 1.1 - added i18n support (thanks to Helmut Giritzer for german translation and for
                           help to learn i18n stuff)

 1.0 - first release




 Authors' info
 -------------

 Francesco Montorsi          frm@users.sourceforge.net

 Special thanks to Ulrich Telle for the great help given in design-stage and feature-proposal.
 Thanks to Angelo Mandato for its useful wxHttpEngine component.
 Thanks to Helmut Giritzer for German translation and for helping me to add i18n support to WebUpdate.
 Thanks also to all wxWidgets developers who helped me in the wx-users mailing list.
