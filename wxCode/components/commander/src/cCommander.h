
#ifndef __CCOMMANDER_h__
#define __CCOMMANDER_h__


#include <wx/string.h>
#include "wxCommanderUtils.h"

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
      int getIcon(long itemPos, long itemCol);
      int getIconDevice(long itemPos, long itemCol);
      void refreshFileDir();
      void setDevices(bool listDevices);

   private:
      bool blnDevices;
	   vectorString aPaths;
	   vectorString dirFileMap;
      int actualPath;
};
   
#endif    
    
