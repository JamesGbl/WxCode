
#ifndef __WXCOMMANDERUTILS_h__
#define __WXCOMMANDERUTILS_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif

#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/file.h>

#include <wx/utils.h>
#include <wx/mimetype.h>

#include <sys\types.h>
#include <sys\stat.h>
#include <io.h>
#include <unistd.h>
#include <time.h>

#ifdef USE_EXECUTE
    #define EXEC(cmd) wxExecute((cmd), wxEXEC_SYNC)
#else
    #define EXEC(cmd) system(cmd)
#endif

#include <wx/listctrl.h>
#include <wx/dnd.h>

#include <vector>
#include <map>

#ifdef __WXMSW__
   #include <windows.h>
#endif

using namespace std;

typedef map <wxString, int, less<wxString> > driversMap;

void Exec(wxString& path, wxString& file);
size_t getFileSize(const wxString& fileName);
size_t getDirSize(wxString& directoryFile);
size_t getDirSize(wxString& directoryFile, long& numFiles);
wxString formatFileSize(size_t st_size);
wxString getModificationTime(wxString& fileName);
bool deleteDirFile(wxString& path, wxString& item);
bool deleteDirectory(wxString& path, wxString& item);
bool renameDirFile(wxString& path, wxString& oldName, wxString& newName);
bool copyDirFile(wxString& path, wxString& item, wxString& pathNew);
bool copyDirectory(wxString& path, wxString& item, wxString& pathNew);
wxString keyCodeToString(int keyCode);
driversMap wsGetDrives();
wxString getLastDir(wxString directory);

int get_file_attrib(char *filename);
#endif
