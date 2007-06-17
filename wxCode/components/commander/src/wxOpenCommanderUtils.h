
#ifndef __wxOpenCommanderUTILS_h__
#define __wxOpenCommanderUTILS_h__

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
#include <wx/filename.h>

#include <wx/utils.h>
#include <wx/mimetype.h>

#include <sys\types.h>
#include <sys\stat.h>
#include <io.h>
#include <unistd.h>
#include <time.h>

#ifdef USE_EXECUTE
    #define EXEC(cmd) wxExecute((cmd), wxEXEC_ASYNC)
#else
    #define EXEC(cmd) system(cmd)
#endif

#include <wx/listctrl.h>
#include <wx/dnd.h>

#include <wx/process.h>

#include <vector>
#include <map>

#ifdef __WXMSW__
   #include <windows.h>
#endif

using namespace std;

#define COPY_FILE_OK 0
#define ABORT_COPY_FILE 1
#define ABORT_COPY_PROCESS 2


typedef map <wxString, int, less<wxString> > driversMap;

typedef int (*onThreadBeginCopyFileCallBackFunc)(void*, const wxString&, const wxString&);
typedef void (*onThreadEndCopyFileCallBackFunc)(void*, bool, const wxString&, const wxString&);
typedef void (*onThreadFinishCallBackFunc)(void*, void*, void*);

void Exec(wxString& path, wxString& file);
wxString LongLongTowxString(long long const& size);
long long getFileSize(const wxString& fileName);
long long getDirSize(wxString& directoryFile);
long long getDirSize(wxString& directoryFile, long& numFiles);
wxString formatFileSize(long long st_size);
wxString getModificationTime(wxString& fileName);
bool deleteDirFile(wxString& path, wxString& item);
bool deleteDirectory(wxString& path, wxString& item);
bool renameDirFile(wxString& path, wxString& oldName, wxString& newName);
bool copyDirFile(wxString& path, wxString& item, wxString& pathNew, void* parent ,onThreadBeginCopyFileCallBackFunc onBeginCopyFile, onThreadEndCopyFileCallBackFunc onEndCopyFile);
bool copyDirectory(wxString& path, wxString& item, wxString& pathNew, void* parent, onThreadBeginCopyFileCallBackFunc onBeginCopyFile, onThreadEndCopyFileCallBackFunc onEndCopyFile);
wxString keyCodeToString(int keyCode);
driversMap wxGetDrives();
wxString getLastDir(wxString directory);
wxString getFileExtension(const wxString& strFileName);

int get_file_attrib(char *filename);
#endif
