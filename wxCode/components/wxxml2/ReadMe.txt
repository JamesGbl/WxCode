
 wxXml2 component info
 ---------------------

 Maintainer: Francesco Montorsi (frm)
 Component:  wxxml2
 Subdir:     wxxml2
 Parts:      DS
 More infos: 
 wxWidgets:  2.4.x,2.5.x
 
 Description:
 A simple set of classes (wxXml2Namespace, wxXml2NodeProperty, wxXml2Node and
 wxXml2Document) that interface wxWidgets with libxml2.
 With this component you can add load/save/edit XML support to your wxWidgets application
 using all the power of a well-known library like libxml2.



 wxXml2 wrappers sample
 ---------------------
 
 This is a little application (a modified version of the minimal sample taken from
 the wxWidgets library) that uses the wxXml classes showing their features.

 The wrappers have been tested with various versions of libxml2, up to version 2.6.9;
 use the one you prefer starting from version 2.5.0...
 


 Installation
 ------------

 The wxXml2 wrappers are packed in a single source file (src/xml2.cpp) and
 a single header file (include/wx/xml2.h), so I think the best way for a
 programmer to use them is to build xml2.cpp together with the sources of
 its app; thus, you won't find makefiles/IDE files for lib output.
 The makefiles/IDE files are just used for the sample.

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


