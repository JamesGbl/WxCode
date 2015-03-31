wxReportDocument 1.2.2
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
15/03/31	v1.2.3
- Fixed printed page backgroud color when page border is in use.

13/12/05	v1.2.2
- Fixed calculation of paragraph's width when there are multiple newlines in the text.
- Fixed positioning of page items when non-zero margins are defined (i.e. page margin is taken into account for
  all page items except those placed inside the header and the footer).
- wxInitAllImageHandlers() call removed from the library - a user must do it himself in his code.

13/11/19	v1.2.1
- Fixed drawing of the border around a text item defined via main text item's style (wxReportTextItem::SetStyle()).

13/09/15	v1.2.0
- Content of table cells can be wrapped

13/07/01	v1.1.0
- Various improvements and bug fixes

13/02/25	v1.0.0
- Initial release
