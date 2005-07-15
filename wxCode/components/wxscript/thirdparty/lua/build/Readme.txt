
 LUA build system for Win32 README
 ---------------------------------

 In this folder are stored all the files required to compile LUA with win32 compilers.
 Bakefile (http://bakefile.sourceforge.net) is used as makefile generator with the
 help of some batch files.

 Supported makefiles:
 - makefile.vc     for Microsoft NMAKE
 - makefile.bcc    for Borland MAKE
 - makefile.wat    for OpenWatcom MAKE
 - makefile.gcc    for MinGW MINGW32-MAKE



 HOWTO compile LUA using makefiles
 ---------------------------------
 
 Choose your preferred compiler among those actually supported (see above) and then run
 
                              mycompilermake -fmakefile.mycompilerext [options]

 where [options] can be:

        BUILD=debug/release       If BUILD==debug then warnings are set to max and debug info
                                  are included in the libraries, making them bigger.
                                  If BUILD==release then warnings are disabled and optimizations
                                  are set to maximum; no debug info is embedded in the libraries.



 HOWTO regenerate makefiles for LUA
 ----------------------------------
 
 Be sure to have installed Bakefile (http://bakefile.sourceforge.net).
 Just run the RegenAll.bat batch file.
 NOTE: if you want to remove all the makefiles, you can use the "bakefile_gen -c" command.




 If you have problems with the compilation of LUA under windows (using one of the supported compiler)
 please write to:

     Francesco Montorsi <frm@users.sourceforge.net>

