
#include "cCommander.h"

cCommander::cCommander()
{
   actualPath = 0;
}

int cCommander::addPath(wxString path)
{   
   aPaths.push_back(path);
   setActualPath(aPaths.size()-1);
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
   if (actualPath >= aPaths.size()) setActualPath(aPaths.size() - 1);
}

int cCommander::getPathsCount()
{
   return aPaths.size();
}

void cCommander::setActualPath(int numPath)
{
   blnDevices = false;
   actualPath = numPath;
   refreshFileDir();
}

void cCommander::setActualPath(wxString path)
{
   blnDevices = false;
   aPaths[actualPath] = path;
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
   return aPaths[actualPath];
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
         return getLastDir(dirFileMap[itemPos]);
      break;
      case 1:
      {
          if (wxDir::Exists(getActualPath() + "//" + dirFileMap[itemPos]))
             return "<DIR>";
          else
             return formatFileSize(getFileSize(getActualPath() + "//" + dirFileMap[itemPos]));
      }
      break;
      case 2:
         wxString strFile = getActualPath() + "//" + dirFileMap[itemPos];
         return getModificationTime(strFile);
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
                    return "Removable";
         	     case DRIVE_CDROM:
                    return "CD/DVD";
                 default:
                    return "Hard Disk";
              }
           }
           else return "";
        }
        i++;
     }
   #endif
}

int cCommander::getIcon(long itemPos, long itemCol)
{
   if (blnDevices) return getIconDevice(itemPos, itemCol);   
   if (itemPos == 0) return 1;
   if (wxDir::Exists(getActualPath() + "//" + dirFileMap[itemPos]))
      return 1;
   else
      return 0;
}

int cCommander::getIconDevice(long itemPos, long itemCol)
{
    #ifdef __WXMSW__
     driversMap drives = wxGetDrives();
     driversMap::iterator iter;
     long i=0;
     for (iter = drives.begin(); iter != drives.end(); iter++)
     {
        if (i == itemPos)
        {
           switch (iter->second)
           {
              case DRIVE_REMOVABLE:
              {
                 if (iter->first == "A:\\" || iter->first == "B:\\")
                    return 5;
                 else
                    return 3;
              }
     	        case DRIVE_CDROM:
                 return 4;
              default:
                 return 3;
           }
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
    wxString wxStrFileName;
    
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
       wxStrFileName = directory + "\\" + filename;
       if (dir.Exists(wxStrFileName))
          dirFileMap.push_back(filename);
       else
          aFiles.push_back(filename);
       cont = dir.GetNext(&filename);
    }
    for (size_t i = 0 ; i < aFiles.size(); i++)
    {
       dirFileMap.push_back(aFiles[i]);
    }
}

