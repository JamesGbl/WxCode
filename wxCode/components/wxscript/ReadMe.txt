  wxScript component info
 -------------------------

 Component:  wxscript
 Release:    2004-09-07
 Documents:  http://wxcode.sourceforge.net/components/wxscript/index.html
 wxWidgets:  2.4.x, 2.5.x
 Maintainer: Francesco Montorsi (frm)
 Subdir:     wxscript
 
 Description:
 A set of abstract classes to add a script-interpreter to your wxWidgets
 applications/libraries.

 The implementation of these interfaces for the Lua, UnderC and CINT interpreters are given.
 The advantage of a single base interface allows the programmer who wants to use this
 component to learn how to use a single set of powerful classes: wxLua, wxUnderC and wxCINT
 all exposes the same functions adding to the program Lua & C++ script-interpreting
 capabilities.


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
  focusing on the powerful feature of a specific one ?
 ---------------------------------------------------------

  There are various answer to this question:
  1) Lua is good for extending applications with lightweight & easy-to-read scripts
     since this language has been specifically designed with this purpose.
     UnderC and CINT instead are good for programmers (since they're C++ intepreters)
     that want to extend their application in a OOP way or that just want to use
     a familiar language.
  2) The user of your program is not forced to learn the scripting language you choice
     since with wxScript you can load scripts in different languages getting the same
     results.
  3) When I started to look around for a script intepreter which suited my requirements,
     I came across various open-source ones and since I had to read good part of all their
     docs to understand if it was what I was searching, it was not a big problem
     support them all... :-)



  How can I compile wxScript and its samples ?
 ----------------------------------------------

  If you want to use wxScript system in your program, you must compile it with all the
  CPP files placed in the main folder of this package (that is, the folder containing
  this ReadMe file). To compile, the class needs to be able to find its header files
  in a "wx" folder placed in a standard include path: wxScript files uses the
  following convention
   
      #include <wx/script.h>     

  thus, you have to move the header files in your wxWidgets' include folder.

  Note: all the CPP and H files of this package, begins with the "sc" prefix to 
       avoid conflicts with the CINT, UnderC and Lua libraries.

  
  If you want to compile the samples, you can use MSVC++ or you can use the makefiles
  placed in the main package directory.

 
