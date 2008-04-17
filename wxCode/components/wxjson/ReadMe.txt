 
 wxJSON ReadMe
 -------------
 
 Website: http://lnx.mauriziovettorato.it/public/json/home.html
 Author: Luciano Cattani
 Version: 0.4.1
 Description:
 JSON (JavaScript Object Notation) is a lightweight data-interchange format.
 It is easy for humans to read and write. It is easy for machines to parse
 and generate. It is based on a subset of the JavaScript Programming Language,
 Standard ECMA-262 3rd Edition - December 1999. JSON is a text format that is
 completely language independent but uses conventions that are familiar to
 programmers of the C-family of languages, including C, C++, C#, Java,
 JavaScript, Perl, Python, and many others. These properties make JSON an
 ideal data-interchange language.


 Installation
 ------------
 For installation instructions see the INSTALL text file.



 ChangeLog
 ---------
 
 0.1 - first version of wxJSON at wxCode

 0.2 - this version adds full support for C/C++ comment lines
       reading and writing

 0.2.1 this is a bug fix release (thanks to Jere, who reported the bugs):

       - wxJSONValue::sm_progr: the initialization of this static data
         member fails to compile if the library is compiled as an
         imported DLL on win32

       - ::wxLogTrace(char*) function does not accept char* as arguments
         in wxWidgets 2.8.7: all string constants parameters to that function
         were enclosed in the _T() macro

 0.2.2 bug fix release: this release fixed the bug in the wxJSONValue::
       IsSameAs() function (see docs for more details)

 0.2.3 bug fix release: the library did not compile in UNICODE mode because
       same string constants were not enclosed in the _T() macro.
       In this release the compile problems were fixed and the library and
       the test application do compile but they do not work properly in the
       JSON parser and writer when compiled in UNICODE mode.

 0.3.0 this release adds support for writing and reading JSON text from
       a stream in UTF-8 format in both ANSI and Unicode builds of wxWidgets.

 0.3.1 fixed a bug in the wxJSONReader::ReadString() function which did not
       handle correctly the unicode escape sequences.

 0.4.0 this version uses copy-on-write (also known as 'reference counting')
       in the copy constructor and assignment operator 

 0.4.1 fixed bugs in the wxJSONWriter::Write() function which now does not
       return a status code. Improved wxJSONWriter's and wxJSONReader's
       documentation related to input from / output to strings and streams


BUGS
----
Please, send bugs to luciano AT mauriziovettorato_DOT_it or open a bug report
on SF.net for the 'wxCode' project ( wxcode.sf.net)

