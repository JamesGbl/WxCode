
 wxXml2 component info
 ---------------------
 
 Component:  wxxml2
 Release:    2004-08-03
 Documents:  http://wxcode.sourceforge.net/components/wxscript/index.html
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

 The wxXml2 wrappers are packed in a single source file (src/xml2.cpp) and
 a single header file (include/wx/xml2.h), so I think the best way for a
 programmer to use them is to build xml2.cpp together with the sources of
 its app. Nonetheless, someone may find a library more portable and thus
 you can use the makefiles in the BUILD folder to create a little lib.

 The most important thing you should do if you've never used these classes
 before is to build the sample and read its source file.

 -> on Win32:
    use the MSVC++ project file to compile the sample

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


