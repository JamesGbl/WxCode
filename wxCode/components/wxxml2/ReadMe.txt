
 wxXml2 component info
 ---------------------

 Website: http://wxcode.sourceforge.net/components/wxxml2
 Version: 1.9

 Description:
 A simple set of classes, wxXml2Namespace, wxXml2Property, wxXml2Node, wxXml2DTD and
 wxXml2Document, that interface wxWidgets with libxml2.
 With this component you can add load/save/edit XML support to your wxWidgets application
 using all the power of a well-known library like libxml2.
 The full support for DTDs allows you to easily check your XML documents & thus to
 produce well-formed XML files.
 Unicode is completely supported as well.

 By now, these classes wrap only the most important libxml2 features but they are
 perfectly integrated with wxWidgets and easily extensible since they provide good
 commented code & doxygen docs.



 Install info
 ------------

 Please look at docs\html\index.html which links the installation page.
 Also be careful that if you use Microsoft Visual Studio, just opening the
 project files of the "build" folder and build them won't work !
 Don't skip the install doc ;-)



 ChangeLog
 ---------

 1.9
     - fixed a problem in the build system which appeared when compiling
       in Unicode mode under Windows (reported by Kevin Colwell).
     - updated wxXml2 to build against wxWidgets 2.9.x
     - removed "thirdparty" folder in favour of the "deps" one which contains
       precompiled binaries (compiled with MSVC on x86)

 1.8

     - revise source code for better compatibility with wxWidgets 2.8
     - updated the libxml2 and iconv libraries in the "thirdparty" folder
       respectively to versions 2.6.27 and 1.11
     - now WX_UNICODE option defaults to 0 and not 1 
     - disabled warnings when compiling libxml2 and libiconv; they give ton
       of warnings under win32 which are harmless (and in any case should be
       fixed by libxml2 and libiconv's authors, not by me ;))

 1.7
     - get rid of GCC #pragmas
     - removed tabs in favour of 4 spaces (as wx sources)
     - substituted wxXML_ with wxXML2_; this fix redefinition problems when using
       both wx/xml/xml.h header (part of wxWidgets) and xml2.h;

       NB: to upgrade your sources to wxXml2 1.7, you need to do the same
           (replace all occurrences of "wxXML_" with "wxXML2_") !

 1.6
     - fixed the install docs with a section about MSVC6 project files
     - fixed a problem with configure script about wxWidgets build flag detection
     - fixed compilation on 64 bit machines (tested under FC4)

 1.5
     - added libxml2 & libiconv to the build system for win32 users;
       this removes the ZLIB dependency from libxml2.
     - added a new parameter in wxXml2Node::Find to allow enable/disable
	   of recursive searchs
	 - updated the bakefile build system with wxWidgets bakefile presets;
	   now configure script automatically detects the wx configuration used
	 - added the wxXML2CHECK_VERSION macro
	 - fixed compilation warnings with Mingw about uninitialized base classes

 1.1
     - second release; fixed the sample and the docs

 1.0
     - first release of wxXml2 wrappers



 Authors' info
 -------------

 Francesco Montorsi          frm@users.sourceforge.net

 Thanks to the following people for their patches:
 - Scott Fant  for helping with wxXml2Node::Find
 - Cecilio     for contributions in the minimal sample about external references handling
