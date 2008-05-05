 
 wxActiveRecordGenerator  ReadMe
 -------------
 
 Website: http://wxcode.sourceforge.net/components/activerecord
 Author: Matías Szeftel
 Version: 1.0 
 Description:
	wxActiveRecordGenerator (aka wxARG) is an application that will let you conect to a database (currently supported: SQLite, PostgreSQL and MySQL), select a table and it's relations with others (currently: has-many relation) and will generate a code which you can use to access directly to the data without the need of the SQL language (althought you can still use it if you want/need to).

 Instructions:
 -------------
	Play around with the program. All you need to use the generated classes is to include them in your project. Include also the wxActiveRecord.h and wxActiveRecord.cpp files and have a working DatabaseLayer backends (currently supported: SQLite, PostgreSQL and MySQL). You can choose what backend to use comenting out the defines at the begging of the wxActiveRecord.h file.

 About the license
 -----------------

 The wxActiveRecordGenerator application and its source code are released under the GPL license. The generated code and the wxActiveRecord.h and wxActiveRecord.cpp files are released under the wxWidgets license. 
 

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
 
 1.0 - first version of wxActiveRecordGenerator at wxCode
 
 
 [optionally: Known bugs, Author's info, ... ]

