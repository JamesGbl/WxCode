
 wxXml2 component info
 ---------------------
 
 Component:  wxxml2
 Release:    2005-1-4
 Documents:  http://wxcode.sourceforge.net/components/wxxml2/index.html
 wxWidgets:  2.4.x, 2.5.x
 Maintainer: Francesco Montorsi (frm)
 Subdir:     wxxml2
 
 Description:
 A simple set of classes, wxXml2Namespace, wxXml2Property, wxXml2Node, wxXml2DTD and
 wxXml2Document, that interfaces wxWidgets with libxml2.
 With this component you can add load/save/edit XML support to your wxWidgets application
 using all the power of a well-known library like libxml2.
 The full support for DTDs allows you to easily check your XML documents & thus to
 produce well-formed XML files.
 Unicode is completely supported as well.

 By now, these classes wrap only the most important libxml2 features but they are
 perfectly integrated with wxWidgets and easily extensible since they provide good
 commented code & doxygen docs.


 wxXml2 wrappers sample
 ----------------------
 
 This is a little application (a modified version of the minimal sample taken from
 the wxWidgets library) that uses the wxXml classes showing their main features.

 The wrappers have been tested with various versions of libxml2, up to version 2.6.16;
 use the one you prefer starting from version 2.5.0...
 


 Installation
 ------------

 The wxXml2 wrappers are packed in a few source files and few header files,
 so you could think the best way to use them is to build the source files
 together with the sources of your app: in this case you should just add
 the source & header files to your project and compile them with the
 standard wxWidgets flags.

 Anyway, others may find a library more portable.
 To build a library, you can use the makefiles in the BUILD folder.
 They support all common flags (BUILD=release/debug, UNICODE=1/0 and so on...).

 Obviously these wrappers require the presence of the LIBXML2 library
 and, only on win32, of the ICONV library. These two libraries are cross-platform
 and can be compiled with various compilers. If you want to download the
 precompiled binaries for win32, I suggest you to download the LIBS package
 of the MathStudio project (http://sourceforge.net/projects/mathstudio) which
 contains these two libraries compiled with MSVC, BORLAND and MINGW compilers.

 The most important thing you should do, specially if you've never used these classes
 before, is to build the sample and read its source file.
 The doxygen docs are a useful and detailed reference manual to use after you learnt
 how wxxml2 wrappers must be used.


 -> on Linux:
    Use the command
    
          make

    to compile the sample.




 Known bugs
 ----------

 None




 Authors' info
 -------------

 Francesco Montorsi          frm@users.sourceforge.net


