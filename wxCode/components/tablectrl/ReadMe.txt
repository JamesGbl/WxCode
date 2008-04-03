 wxTableCtrl
 -----------

 Website: http://wxcode.sourceforge.net/components/tablectrl
 Author: Jan Knepper
 Version: 0.0.1
 Description: 
 wxTableCtrl is a wxWidgets control for viewing (and editing) tableized data. Tableized data
 could be a database table, a list or any other data that exists in the form of records (a 
 collection of data elements) that repeats.
 It is not required for wxTableCtrl to have a index (size_t, int, long) into the table containing
 the data. wxTableCtrl uses an interface class with members GetFst/GetNxt/GetLst/GetPrv to obtain
 a record from the table.


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

 0.0.1  - first version of wxTableCtrl at wxCode


 [optionally: Known bugs, Author's info, ... ]


 Author's info
 -------------
 Jan Knepper, jan@smartsoft.us
 
 
 
 History
 -------
 wxTableCtrl originally has been developed for zApp to browse, edit, maintain 
 Btrieve (www.btrieve.com/www.pervasive.com) tables. When Rogue Wave dropped 
 zApp from their product list it was ported to MFC where it has been used in 
 at least 10 commercial products that are still on the market. Currently it 
 is begin ported to wxWidgets. The effort started several years ago, but is 
 being completed in segments when usage requirements increase.
 