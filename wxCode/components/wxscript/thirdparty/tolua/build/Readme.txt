
 LUA build system for Win32 README
 ---------------------------------

 In this folder are stored all the files required to compile LUA with win32 compilers.
 Bakefile (http://bakefile.sourceforge.net) is used as makefile generator with the
 help of some batch files.

 


 HOWTO compile LUA using makefiles
 ---------------------------------
 
 1) choose your preferred compiler among those actually supported (see Bakefiles.bkgen) and then run
 
                                   mycompilermake -fmakefile.mycompilerext



 HOWTO regenerate makefiles for LUA
 ----------------------------------
 
 Be sure to have installed Bakefile (http://bakefile.sourceforge.net).
 Just run the RegenAll.bat batch file.
 NOTE: if you want to remove all the makefiles, you can use the "bakefile_gen -c" command.




 If you have problems with the compilation of LUA under windows (using one of the supported compiler)
 please write to:

     Francesco Montorsi <frm@users.sourceforge.net>

