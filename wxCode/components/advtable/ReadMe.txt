 
 wxAdvTable ReadMe
 -------------
 
 Website: http://wxcode.sourceforge.net/components/advtable
 Author: Moskvichev Andrey V.
 Version: 1.0 
 Description:
 wxAdvTable is table component (like wxGrid) designed with simplicity and flexibility in mind.
 It has many advanced features like: 
  - composite rows/columns
  - built-in sorting and filtering support
  - flexible rendering and editing support
  - dynamic row/columns add/removal
  - and visual features like: highlight modes, cell attributes, etc
  - MCV (Model-Controller-View) design
 

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
 
 1.0 - first version of wxAdvTable at wxCode
 
 Known issues:
 - lack of Perl and Python bindings.
 - some selection bugs.
 - paint system must be cleaned up and optimized.
 - lack of filtering.
 - many sorting bugs.
 - tested only on wxMSW-2.8.9 (Windows XP(tm) SP2) and wxGTK-2.8.8 (Gentoo Linux x86_64), 
   if someone can test on other wxWidgets ports, please let me know.
