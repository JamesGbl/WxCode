 
 wxJSON ReadMe
 -------------
 
 Website: http://luccat.users.sf.net/wxjson/
 Author: Luciano Cattani
 Version: 1.1.0
 Description:
 JSON (JavaScript Object Notation) is a lightweight data-interchange format.
 It is easy for humans to read and write. It is easy for machines to parse
 and generate. It is based on a subset of the JavaScript Programming Language,
 Standard ECMA-262 3rd Edition - December 1999. JSON is a text format that is
 completely language independent but uses conventions that are familiar to
 programmers of the C-family of languages, including C, C++, C#, Java,
 JavaScript, Perl, Python, and many others. These properties make JSON an
 ideal data-interchange language.


 INSTALLATION
 ------------
 For installation instructions see the INSTALL text file.


 DOCUMENTATION
 -------------
 The library has a wide documentation that you find in the 'docs/html/' subfolder
 if you installed from the tarball.
 If you got the library by directly accessing the SVN repository, you have
 to build the docs by yourself using the 'doxygen' documentation generator
 (http://doxygen.sf.net). Type the following commands:

 $ cd docs
 $ doxygen



 CHANGELOG
 ---------
 
 0.1.0 first version of wxJSON at wxCode

 0.2.0 this version adds full support for C/C++ comment lines
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
       some string constants were not enclosed in the _T() macro.
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
       return a status code.
       fixed a bug in the wxJSONReader::AddWarning() function which did not
       take into account the individual extension's ON/OFF flag.
       Improved wxJSONWriter's and wxJSONReader's documentation related to
       input from / output to strings and streams

 0.5.0 this release adds support for 64-bits integer on platforms that support
       them.

 0.5.1 fixed a bug in the wxJSONWriter::WriteChar() function which did not
       compile on Borland BCC 5.5.
       fixed a bug in the wxJSONReader::ReadValue() function which stores wrong
       values if 32-bits integer support is disabled. This bug also affects
       all other 0.x versions of the wxJSON parser.

 0.5.2 fixed a bug in the wxJSONWriter::DoWrite() function: I forgot the 'case'
       wxJSONTYPE_(U)INT64 types in the switch statement so 64-bits integers
       where not printed in the JSON text output.
       introduced the 'unsigned int' recognition by the parser by prepending
       a plus sign (+) to the unsigned int value.

 1.0.0 this release breaks compatibility with 0.x versions: please note that
       there is not a 'compatibility' version of this library. New features:
       (r 777) changed wxJSONTYPE_EMPTY to wxJSONTYPE_INVALID.
       (r 785) added support for '(unsigned) long int' and '(unsigned) short int' data type.
               changed return value of the wxJSONValue::Is(U)Int Is(U)Int64 functions.
               removed the 'unsigned int' recognition by the parser due to incompatibility
               with other JSON parsers.
       (r 787) added the following wxJSONWriter's flags:
               - wxJSONWRITER_NO_LINEFEED: suppress LF
               - wxJSONWRITER_ESCAPE_SOLIDUS: the solidus char is not escaped by default
               - wxJSONWRITER_MULTILINE_STRING: LFs and TABs are not escaped
               - wxJSONWRITER_RECOGNIZE_UNSIGNED: force the reader to recognize unsigned
               - wxJSONWRITER_TAB_INDENT: use TABs instead of spaces for indentation

 1.0.1 a bug-fix release: fixed bug #2720660 (wxJSONwriter writes the wrong character
		for escaped characters)


 1.0.2 a bug-fix release: fixed bug #2795247 (wxJSONwriter does not escape control characters
       (r 960) fixed the bug - see the Bug Tracker on sf.net
 	
 	
 1.1.0 this release is a rewriting of the wxJSONReader and wxJSONWriter class which now 
       process JSON text input / output only in UTF-8 format. If I/O is from / to
       wxString objects, the reader and the writer convert the string object to a
       temporary UTF-8 memory-input/output-stream for processing
 	   
 1.1.1 fixed bug # 2898794 (build errors on gcc) and bug #2909326 (error reading UTF-8
       strings that contain unrepresentable chars in ANSI mode).


 1.2.0 (r 1147) fixed bug ID 2910776 (error writing double values)
       (r 1146) fixed bug ID 2898794 (error and warnings in building wxJSON)
       (r 1150) implemented the use of STL if wxWidgets is compiled using --enable-stl
       (r 1152) implement the precision format string when writing doubles
       (r 1153) document the issue when writing doubles
       (r 1154) document the STL version of containers and iterators (see Test #14)
       (r 1157) modified bakefile output: only build the wxJSON library
       (r 1158) code cleanup: removed commented out code, unused symbols and macros..
       (r 1159) use spaces for indentation and fix line-ending confusion

 1.2.1 (r 1343) fixed bug #3005593 in the wxJSONReader::SkipComment() function: when
                C-style comments are read the funct returned the wrong char

 1.3.0 (r 1164) implemented the wxJSONValue new data type: memory buffers
                implemented the writing of the new data type
                implemented the reading of the new data type


BUGS
----
Please, send bugs to luciano@mauriziovettorato.it or, better, open a bug report
on SF.net for the 'wxCode' project ( http://wxcode.sourceforge.net/links.php )


LICENSE
-------
The wxJSON library is distributed under the terms of the wxWidgets license
(http://www.wxwidgets.org/about/newlicen.htm) either the current version 3.1 or,
at your option, any later version.



