1. Introduction
2. Build Instructions
3. Credits/License

////////////////////////////////////////////////////////////////
1. Introduction
////////////////////////////////////////////////////////////////
wxZipStream uses the lastest unzip libarary for unzipping
and zipping zip files.  

Input Usage:
First, get the info of a file by calling WxZipInputStream::GetFileInfo()
wxZipFileInfo Info;
ZipInStream->GetFileInfo(Info);

That gets the file size, name, etc..
Then you read in the contents of the current file by calling Read(),
and then when that's done call OpenNextFile() or OpenFile() if you know
the file you want to open.

Output Usage:
Same as before, create wxZipOutputStream from another stream,
then create a file in the zip file by calling CreateFile
with (at least) the szName part of the wxZipFileInfo structure
filled out.
wxZipFileInfo info;
info.szName("File_1.txt");
ZipOutStream->MakeFile(info);
ZipOutStream->Write("MY TEXT", strlen("MY TEXT"));

That's pretty much it for now.

////////////////////////////////////////////////////////////////
2. Build Instructions
////////////////////////////////////////////////////////////////
Zipcopy is an example application which opens an existing 
zip file, reads it, and then writes a copy of it.

To build wxZipStream you need the following files:
ioapi.c
zip.c
unzip.c
zipstream.cpp

Also, if you want to build the example you need:
zipcopy.cpp

////////////////////////////////////////////////////////////////
3. Credits/License
////////////////////////////////////////////////////////////////
Author: Ryan Norton (wxprojects@comcast.net)
License: wxWindows (wxwindows.org for more info)