 
 XSTC ReadMe
 -------------
 
 Website: http://wxcode.sourceforge.net/components/xstc
 Author: Nuklear Zelph
 Version: 1.0 
 Description:
  The goal of XSTC is to create a small wrapper class for wxStyledTextCtrl/wxScintilla that will automate
  some of the most common component useages without interfering with the scintilla bridge class. (wxSTC)
  It will automate brace matching, lexer setting based on file extention, folding and lexing. there is not
  much currently, but configuration files can be used in the future to overwrite the default behavior for
  this functionality. so a .txt could be set to the LEX_CPP lexer and coloring of course. a simple save as
  implimentation is built in, it does not check for existance, just a valid filename.
 
 

 Installation - win32
 --------------------
 
 When building on win32, you can use the makefiles in the BUILD folder using the
 WX_DIR, WX_DEBUG, WX_UNICODE and WX_SHARED to match your wxWidgets build.
 Examples:
 
   > nmake -fmakefile.vc WX_DIR=c:\path\to\my\wxWidgets WX_UNICODE=0/1 WX_DEBUG=0/1 WX_SHARED=0/1
   > make -fmakefile.bcc WX_DIR=c:\path\to\my\wxWidgets WX_UNICODE=0/1 WX_DEBUG=0/1 WX_SHARED=0/1
 
 you might get the dev generated makefile to work, it probably will take tweaking, i never try to do it 
 that way. currently only a dev-cpp project file is provided. no ms compiler project files will ever be unless
 someone gives me one. i do plan on supporting bakefile in the future. i just don't have time to figure it out yet.

     
 Installation - unix
 -------------------

 When building on a Linux/GNU-based system, type

  cd build
  ./configure
  make

 to build against the default build of wxWidgets (the one listed as 'default' 
 by wx-config --list). Type "./configure --help" for more info.

 for now you'd need to write your own makefile of create a project file unless you are using code::blocks.


 ChangeLog
 ---------
 
 1.0 - first version of XSTC at wxCode
       this has a load, save and close function for the editor. 
	   lex_cpp and darkstyle lexer color setup functions.
	   load keywords and a property string parser.
	   some default behavior is set in the constructor, also the event tables are
	      dynamically linked in the constructor so that the events are set to the users
		  defined window instead of creating a child set to a static id. 
	   it uses a copy constructor so it can be called like it where wxSTC/wxSci
	   a few macros built so that when used XSTC can be compiled with either library and
	      there is no difference in functionality and no compiler errors.
	   for the configuration behavior files a struct was created. the constructor sets the
	      keyworkds, but if a filename is written in then that will be searched for. this for keywords.
	   bracematching, folding, implimented including some ext based lexer property settiiings
	   
	for further information get the source and run doxygen
 
 
 [optionally: Known bugs, Author's info, ... ]
 
 aloha note:
 ----------
 
 do keep in mind that this is still in alpha. it is not going to work very well. i am going to add some other
 samples in a while. consider the current sample a devel test app. it should be a piece of cake to add to your
 project, either just add the to the project ot if you want make it into a library and use it that way. i suggest
 you make it a library that includes wxSTC/wxSci and you'll need thier header file too of course.
 
 something like this in the project
 
 XSTC
  SRC
   xstc.cpp
  HEAD
   stc.h
   xstc.h
   
  or for a library
  
  stc.h
  xstc.h
  
  -lxstc
  
  the second header is needed since xstc is only a amall automater and you will stinn need to call the bridge.

