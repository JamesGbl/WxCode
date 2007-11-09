
#include "cCommander.h"

cCommander::cCommander()
{
   actualPath = -1;
}

int cCommander::addPath(wxString path)
{   
   aPaths.push_back(path);
   aFilters.push_back("");
   actualPath = getPathsCount() - 1;
   return actualPath;
}

wxString cCommander::getPath(int numPath)
{ 
   return aPaths[numPath];
}

void cCommander::setPath(int numPath, wxString& path)
{
   aPaths[numPath] = path;
}

void cCommander::removePath(int numPath)
{
   aPaths.erase(aPaths.begin()+numPath);
   aFilters.erase(aFilters.begin()+numPath);
   if (actualPath >= aPaths.size()) setActualPath(aPaths.size() - 1);
}

int cCommander::getPathsCount()
{
   return aPaths.size();
}

void cCommander::setActualPath(int numPath, bool blnRefresh)
{
   blnDevices = false;
   actualPath = numPath;
   if (blnRefresh) refreshFileDir();
}

void cCommander::setActualPath(wxString path, bool blnRefresh)
{
   blnDevices = false;
   aPaths[actualPath] = path;
   if (blnRefresh) refreshFileDir();
}

void cCommander::setActualFilter(wxString filter)
{
   aFilters[actualPath] = filter;
   refreshFileDir();
}

void cCommander::setDevices(bool listDevices)
{
   blnDevices = listDevices;
}

bool cCommander::getListDevices()
{
   return blnDevices;
}

wxString cCommander::getActualPath()
{
   if (actualPath < 0 || actualPath > getPathsCount() - 1) actualPath = getPathsCount() - 1;
   if (actualPath < 0) return "";
   return aPaths[actualPath];
}

wxString cCommander::getActualFilter()
{
   return aFilters[actualPath];
}

long cCommander::getFileDirCount()
{
   return dirFileMap.size();
}

wxString cCommander::getFileDirActualPath(long itemPos, long itemCol)
{
   if (blnDevices) return getDrive(itemPos, itemCol);
   
 
   switch (itemCol)
   {
      case 0:
         return dirFileMap[itemPos];
      break;
      case 1:
      {
         wxString wxExt = getFileExtension(dirFileMap[itemPos]);
         return wxExt.Upper();
         break;
      }
      case 2:
      {
          if (wxDir::Exists(dirFileMap[itemPos]))
             return "<DIR>";
          else
             return formatFileSize(getFileSize(dirFileMap[itemPos]));
      }
      break;
      case 3:
         if (itemPos == 0)
            return "";
         else
            return getModificationTime(dirFileMap[itemPos]);
      break;
   }   
}

wxString cCommander::getDrive(long itemPos, long itemCol)
{
   #ifdef __WXMSW__
     //if (itemCol == 2) return "";
     driversMap drives = wxGetDrives();
     driversMap::iterator iter;
     long i=0;
     for (iter = drives.begin(); iter != drives.end(); iter++)
     {
        if (i == itemPos && iter != drives.end())
        {
           if (itemCol == 0)
             return iter->first;
           
           if (itemCol == 1)
           {
              switch (iter->second)
              {
    	           case DRIVE_REMOVABLE:
                    return REMOVABLE;
         	     case DRIVE_CDROM:
                    return CDROM;
                 default:
                    return HARDDISK;
              }
           }
           else return "";
        }
        i++;
     }
   #endif
}

void cCommander::refreshFileDir()
{
    if (blnDevices) return;

    wxDir dir;
    wxString filename;
    wxString pathFileName;
    
    wxString directory = getActualPath();

    dirFileMap.clear();
    if (!dir.Exists(directory)) return;
   
    dir.Open(directory);
    if (!dir.IsOpened()) return;
    
    bool cont = dir.GetFirst(&filename);
    dirFileMap.push_back("..");

    vector<wxString> aFiles;

    while (cont)
    {
       pathFileName = directory + "\\" + filename;
       if (dir.Exists(pathFileName))
          dirFileMap.push_back(pathFileName);
       else
       {
         wxString strFilter = aFilters[actualPath];
         if (!strFilter.IsEmpty())
         {
            strFilter.Replace("*", "");
            wxRegEx filter(strFilter.Upper());
            if ( filter.Matches(filename.Upper()) )
            {
               aFiles.push_back(pathFileName);
            }
         } 
         else aFiles.push_back(pathFileName);
       }
       cont = dir.GetNext(&filename);
    }
    for (size_t i = 0 ; i < aFiles.size(); i++)
    {
       dirFileMap.push_back(aFiles[i]);
    }
}

void cCommander::addPathsRecursive(const wxArrayString& aFilesPath)
{
   dirFileMap.clear();
   for (size_t i = 0 ; i < aFilesPath.Count(); i++)
   {
      addPathRecursive(aFilesPath[i]);
   }
   setDevices(false);
}

void cCommander::addPathRecursive(const wxString& filePath)
{
   if (wxDir::Exists(filePath))
   {
      wxDir dir;
      wxString filename;
      wxString wxStrFileName;

      dir.Open(filePath);
      if (!dir.IsOpened()) return;
      bool cont = dir.GetFirst(&filename);
      while (cont)
      {
         wxStrFileName = filePath + "\\" + filename;
         //if (wxDir::Exists(wxStrFileName))
         //   dirFileMap.push_back(wxStrFileName);
         addPathRecursive(wxStrFileName);
         cont = dir.GetNext(&filename);
      }
   }
   else
   {
      if (wxFile::Exists(filePath))
         dirFileMap.push_back(filePath);
   }
}

void cCommander::removeFileDir(const wxString& item)
{
   vectorString::iterator iter;
   for (iter = dirFileMap.begin(); *iter != item && iter != dirFileMap.end(); iter++);
   if (iter != dirFileMap.end()) 
      dirFileMap.erase(iter);
}

