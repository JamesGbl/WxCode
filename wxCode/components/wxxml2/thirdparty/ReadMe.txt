
 Thirdparty libraries readme
 ===========================

 This folder contains only the header & source files of the
 libraries required by wxXml2.
 For more info on the licenses, authors, change logs, etc you
 should download these libraries at

   http://xmlsoft.org                        for libxml2
   http://www.gnu.org/software/libiconv/     for libiconv



 Licenses
 ========

 In short the licenses are:

   MIT license for libxml2
   LGPL license for libiconv



 About the build system
 ======================

 The "build" folders placed here and in the other libraries's folders
 contains the makefiles generated using bakefile for various win32
 compilers. These makefiles have been created by Francesco Montorsi
 and they only build the libraries ignoring all the tests, utilities,
 bindings and what else comes with those libraries.
 This is because:
   1) it would be a big work to create a comprehensive set of bakefiles
      which emulate perfectly the original (and often very disordered!)
      build system.

   2) library mantainers seem to be reluctant to add bakefile-based
      build systems to their libraries removing their old build system.

   3) the bakefiles are tested & almost-surely work only on win32 because
      typically unix-based systems already include these libraries by
      default (libxml2 is used by Gnome; thus everywhere you have Gnome
      you'll probably have also libxml2 and libiconv!).
 
 On win32 obviously those libraries are not installed and if you rely on
 precompiled-binaries of libxml2 then you'll probably need to distribute
 an additional zlib.dll file which is a problem for small application:
 zlib is already included by wxWidgets and thus you get a duplication of
 the zlib compiled bytecode to distribute which makes the package bigger.

 If you don't rely on precompiled binaries because you like to compile
 the libraries you use by yourself (like me ;-)), then you'll find 
 *very* comfortable to launch a single command to build both libxml2
 and iconv with the same options, don't you ?




 How to update libxml2 and libiconv
 ==================================
 
 Just decompress the libxml2 and libiconv packages in this thirdparty\
 folder overwriting old files.
 Then go in wxXml2\build and run bakefile_gen; if there are no
 incompatible changes in libxml2 and libiconv this should be enough !
 