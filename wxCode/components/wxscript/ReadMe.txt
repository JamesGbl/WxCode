
 wxScript component info
 -----------------------

 Website: http://wxcode.sourceforge.net/components/wxscript
 Version: 2.0
 
 Description:
 A set of abstract classes to add a script-interpreter support to your wxWidgets
 applications/libraries.
 The implementation of these interfaces for the Lua, Python, UnderC and CINT interpreters
 (these two are C/C++ intepreters) are provided.
 The advantage of a single basic interface makes extremely easy for the programmer
 not only to add scripting support to its program but also to give the user
 multi-scripting languages support, learning how to use a single set of powerful classes:
 wxPython, wxLua, wxUnderC and wxCINT.
 These classes are easily extensible and they support Unicode and shared builds as well.



  What is wxScript ?
 --------------------
  
 wxScript & related classes are a wxWidgets-based interface to some widely used
 script interpreter: bynow, the supported interpreters are CINT, UnderC and Lua
 but the classes can be used with any other interpreter with few efforts.
 wxScript allows a programmer to:

  1) allow the user of his program to extend the program itself using a simple (Lua)
     or a powerful (C++ through CINT and UnderC) language.

  2) write parts of his program as scripts, with the advantage that the
     time spent compiling the sources can be greately reduced with only a little
     runtime cost and with the advantage to make his program deep-customizable by 
     the end user.



  How does it work ?
 --------------------

 wxScript just inits the required interpreters and then uses their native APIs to
 get access to the functions loaded from the scripts.
 The interpreters' APIs are not too difficult to learn; the problem is that they
 are all different and sometimes, wxScript has to perform some 'hacks' to emulate
 functions not immediately available through the interpreter's API.
 Thus, wxScript hides all interpreter-specific code and problems, providing to the
 programmer a simple & clean way to access scripted functions.



  Why wxScript supports different interpreters instead of 
  focusing on the powerful features of a specific one ?
 ---------------------------------------------------------

 There are various answer to this question:
 1) Lua is good for extending applications with lightweight & easy-to-read scripts
    since this language has been specifically designed with this purpose.
    UnderC and CINT instead are good for programmers (since they're C++ intepreters)
    that want to extend their application in a OOP way or that just want to use
    a familiar language.
 2) The user of your program is not forced to learn the scripting language you choose
    since with wxScript you can load scripts in different languages getting the same
    results.
 3) When I started to look around for a script intepreter which suited my requirements,
    I came across various open-source ones and since I had to read good part of all their
    docs to understand if it was what I was searching, it was not a big problem
    support them all... :-)



  How can I compile wxScript and its samples ?
 ----------------------------------------------

 If you want to use wxScript system in your program, you can compile it together with all the
 CPP files placed in the main folder of this package, or you can compile wxscript as a small
 library, using the makefiles of the BUILD folder, and then link your program against it.

 In particular, when building wxscript on an autoconf-based system (like Linux/GNU-based systems),
 you have to create the makefile doing:

  cd build
  aclocal
  autoconf
  ./configure [here you should use the same flags you used to configure wxWin]
  make
 
 Type "./configure --help" for more info.
 The autoconf-based systems also support a "make install" target which builds the library and then
 copies the headers of the component to /usr/local/include and the lib to /usr/local/lib.
 For any problem, feel free to contact frm@users.sourceforge.net



 Known bugs
 ----------

 --> when using PYTHON, there is a memory leak; I'm sure this happens with MSVC6 in DEBUG mode
     but it could happen also with other compilers and even in release mode
     (I think this is a python lib bug not a wxscript one...);
     if you don't want to use python and you don't want to have that leak use USE_PYTHON=0 option



 Authors' info
 -------------

 Francesco Montorsi          frm@users.sourceforge.net
