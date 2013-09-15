wxReportDocument 1.0.0
----------------------
License:	wxWidgets License
Author:		Václav Kvasnička (c) 2012
			Michal Bližňák (c) 2013


Build instructions
------------------

Premake3 utility is used for automated generation of Makefile and project files for CodeLite IDE
(Code::Blocks and VS are supported as well). Switch to the "build" subfolder and run
"create_build_files.sh" script for Linux/Mac OS X or "create_build_files.bat" for MS Windows.
For MS Windows, correct version of used wxWidgets library must be specified inside the script (
wxWidgets version will be detected automatically on Linux and Mac OS X). After that, GNU Makefile and
solution/project files will be generated into the "build" subfolder for further usage.


Changes
-------
13/09/15	v1.2.0
- Content of table cells can be wrapped

13/07/01	v1.1.0
- Various improvements and bug fixes

13/02/25	v1.0.0
- Initial release
