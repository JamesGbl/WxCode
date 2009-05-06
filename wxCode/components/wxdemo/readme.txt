wxDemoViewer
------------------------------
Author: Ján Chudý
Date: 23. 3. 2009
e-mail: chudyjansk@gmail.com
www: 


What wxDemoViewer is?
-------------------------

TODO: Popis

Requirements:
-------------

- wxDemoViewer uses wxWidgets (www.wxwidgets.org) and was tested with version 2.8.x on MS Windows
  and Linux (Ubuntu 8.10) target platforms.
- Code::Blocks (www.codeblocks.org) IDE with MinGW C++ compiler or MS Visual Studio 2005/2008
  Express are recommended for opening included workspace/project files but the library can be
  built from the command line as well.


Distribution content:
---------------------

.\build	- Premake build system for creation of various project/make files
.\doc	- wxDemoViewer doxygen documentation file (in CHM and HTML format).
.\wxDemoViewer - header and source files for application
.\wxWidgets - sample projects for the wxwidgets library.


Build instructions:
===================
The library and included sample projects can be build with build system included in
a this package. Build system is located in the ./build subdirectory.

Premake build system:
---------------------
Premake build scripts and executables are suitable for creation of various IDE project files and 
GNU makefile as well. You can simply create Code::Blocks and MS VS 2005/2008 project files by invoking of
the included batch scripts.


Installation - win32:

When building on win32, go to the ./build subdirectory and simply type:

> create_build_files.bat

There are few standard build configurations defined in the batch file and you can freely modify it or you can
inkove the premake system directly as follows:

Examples of usage of the premake scripts:

For Code::Blocks Workspace/Project files with staticaly built library and UNICODE support:
> premake\premake-win32.exe --target cb-gcc --unicode --use-wx-static

For MSVS 2005/2008 Workspace/Project files producing staticaly built library with UNICODE support and staticaly linked runtime
and no built-in wchar type:
> premake\premake-win32.exe --target vs2008 --unicode --use-wx-static

For GNU makefile with staticaly built library and UNICODE support:
> premake\premake-win32.exe --target gnu --unicode --use-wx-static


Installation - Unix:

When building on Unix, go to the ./build subdirectory and simply type:

sh ./create_build_files.sh or ./create_build_files_x64.sh on 64 bit operating system

or you can invoke the premake system as described in the Win32 installation section above.

Note that all needed executables and scripts are included in this package so you can use the premake build system immediately.
