1. Introduction
2. Build Instructions
3. TODO
4. Credits/License

////////////////////////////////////////////////////////////////
1. Introduction
////////////////////////////////////////////////////////////////
wxRarInputStream unpacks rar files using a modified 
version of the unrar source so that it will
work on any platform.

Usage:
Convoluted API right now.

//Open the file, wxLogSysError will be called if error
wxRarInputStream ris(szFile);

//Loop through files, extracting each one
while(ris.OpenNextFile())
{
	//Extract this file to szDirPath/szFileName
	ris.ExtractFile(szDirPath, szFileName);
}

Low-Level Docs:
Read unrardll.txt on how to use the unrar functions

////////////////////////////////////////////////////////////////
2. Build Instructions
////////////////////////////////////////////////////////////////
Everything is automated using bakefiles.

For linux/OSX use build/unix/makefile.gcc.
Windows build/msw.

////////////////////////////////////////////////////////////////
3. TODO
////////////////////////////////////////////////////////////////
- Better API
- Extract to memory (Using Read())
- Open arbitrary locations

////////////////////////////////////////////////////////////////
4. Credits/License
////////////////////////////////////////////////////////////////
Author: Ryan Norton (wxprojects@comcast.net)
License: wxWindows (wxwindows.org for more info)
