
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
 wxXml2Document) that interface wxWidgets and libxml2.
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

 -> on Win32 with wxMSW:
    use the MSVC++ project file to compile the sample (you must put the "wxlibxml" folder
    in the [wxWidgets main folder]\samples to allow MSVC++ to correctly load the project).

 -> on Linux with wxGTK:
    Use the command
    
          make -fmakefile.unx

    to compile the sample.




 Known bugs
 ----------

 None




 Authors' info
 -------------

 Francesco Montorsi          frm@users.sourceforge.net


