  wxScript component info
 -------------------------

 Maintainer: Francesco Montorsi (frm)
 Component:  wxscript
 Subdir:     wxscript
 Parts:      DS
 wxWidgets:  2.4.x,2.5.x

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

  [TOWRITE]
  


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

 
