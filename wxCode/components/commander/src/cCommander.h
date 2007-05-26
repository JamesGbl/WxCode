
#ifndef __CCOMMANDER_h__
#define __CCOMMANDER_h__


#include <wx/string.h>
#include "wxOpenCommanderUtils.h"

//#include <wx/regex.h>

using namespace std;

#include <vector>

typedef vector<wxString> vectorString;

class cCommander
{      
   public:   
      cCommander();
      int addPath(wxString path);
      wxString getPath(int numPath);
      void setPath(int numPath, wxString& path);
      void removePath(int numPath);
      int getPathsCount();
      void setActualPath(int numPath);
      void setActualPath(wxString path);
      wxString getActualPath();
      
      long getFileDirCount();
      wxString getFileDirActualPath(long itemPos, long itemCol);
      wxString getDrive(long itemPos, long itemCol);
      void refreshFileDir();
      void setDevices(bool listDevices);
      bool getListDevices();

      void addPathsRecursive(const wxArrayString& aFilesPath);
      void removeFileDir(const wxString& item);

   private:
      void addPathRecursive(const wxString& filePath);
      
      bool blnDevices;
	   vectorString aPaths;
	   vectorString dirFileMap;
      int actualPath;
};
   
#endif    
    
