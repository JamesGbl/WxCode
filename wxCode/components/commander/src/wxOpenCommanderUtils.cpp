
#include "wxOpenCommanderUtils.h"

void Exec(wxString& path, wxString& file)
{
   wxString ext = getFileExtension(path  + "\\" + file);
   wxFileType* mimeFile = wxTheMimeTypesManager->GetFileTypeFromExtension(ext);
   if (!mimeFile) return;
   wxString command = mimeFile->GetOpenCommand(path + "\\" + file);
   delete(mimeFile);
   if (!command) return;
   if (command.Right(1)=="*")
      command = command.Left(command.Length()-1);
   if (ext.Upper() == ".BAT")
   {
      #ifdef __WXMSW__
        command = "Exec.bat " + path + " " + command;
      #else
        command = path + " " + command;  
      #endif 
   }   
   wxExecute(command, wxEXEC_ASYNC);
}

size_t getDirSize(wxString& directoryFile)
{
   long numFiles;
   return getDirSize(directoryFile, numFiles);
}

size_t getDirSize(wxString& directoryFile, long& numFiles)
{
   wxDir dir;
   wxString filename;
   size_t totalSize = 0;
   if (!dir.Exists(directoryFile))
   {
      numFiles++;
      return getFileSize(directoryFile);
   }
   else
   {
      dir.Open(directoryFile);
      if (dir.IsOpened())
      {
         wxString wxStrFileName;
         bool cont = dir.GetFirst(&filename);
         while (cont)
         {
            wxStrFileName = directoryFile + "\\" + filename;
            totalSize = totalSize + getDirSize(wxStrFileName, numFiles);
            cont = dir.GetNext(&filename);
         }
      }
   }
   return totalSize;
}

size_t getFileSize(const wxString& fileName)
{
   struct stat fileStat;
   int err = stat( fileName, &fileStat );
   if (0 != err) return 0;
   return fileStat.st_size;
}

wxString formatFileSize(size_t st_size)
{
   wxString size;
   if (st_size >= 1000000000)
   {
     size << (st_size/10000000);
     size = size.Left(size.Len()-2) + "," + size.Right(2);
     size+=" Gb";
     return size;
   }
   if (st_size >= 1000000)
   {
     size << (st_size/10000);
     size = size.Left(size.Len()-2) + "," + size.Right(2);
     size+=" Mb";
     return size;
   }
   if (st_size >= 1000)
   {
     size << (st_size/1000);
     size+=" Kb";
     return size;
   }
   size << (st_size);
   size+=" Bytes";

   return size;
}

wxString getModificationTime(wxString& fileName)
{  
   struct tm* tmDateFile;
   struct stat attrib;
   stat(fileName, &attrib);

   tmDateFile = gmtime(&(attrib.st_mtime));
   wxString min, hour, day, month, year;
   day << tmDateFile->tm_mday;
   month << (tmDateFile->tm_mon+1);
   year << (tmDateFile->tm_year+1900);
   hour << (tmDateFile->tm_hour+1);
   min << tmDateFile->tm_min;
   if (tmDateFile->tm_mday < 10) day = "0" + day;
   if (tmDateFile->tm_mon+1 < 10) month = "0" + month;
   if (tmDateFile->tm_hour+1 < 10) hour = "0" + hour;
   if (tmDateFile->tm_min < 10) min = "0" + min;
   return day + "/" + month + "/" + year + " " + hour + ":" + min;
}

bool deleteDirFile(wxString& path, wxString& item)
{
   wxFile file;
   wxDir dir;
   wxString filePath = path + "\\" + item;

   if (dir.Exists(filePath))
      return deleteDirectory(path, item);

   if (file.Exists(filePath))
   {
	   SetFileAttributes(filePath.c_str(), FILE_ATTRIBUTE_NORMAL); // Make it not read only, so we can delete it
      return wxRemoveFile(filePath);
   }

   return false;
}

bool deleteDirectory(wxString& path, wxString& item)
{
   {
      wxDir dir;
      wxString filePath = path + "\\" + item;
      wxString file;
      dir.Open(filePath);
      if (!dir.IsOpened()) return false;
      bool cont = false;
      if (dir.HasFiles() || dir.HasSubDirs())
         cont = dir.GetFirst(&file);
      while (cont)
      {
         wxString strFile = filePath + "\\" + file;
         
         if (dir.Exists(strFile))
            deleteDirectory(filePath, file);

         SetFileAttributes(strFile.c_str(), FILE_ATTRIBUTE_NORMAL); // Make it not read only, so we can delete it
         wxRemoveFile(strFile);

         cont = dir.GetNext(&file);
      }
   }
   return wxRmdir(path + "\\" + item);
}

bool copyDirFile(wxString& path, wxString& item, wxString& pathNew)
{
   wxString filePath = path + "\\" + item;

   bool blnIsDirectory = false;
   {
     wxDir dir;
     blnIsDirectory = dir.Exists(filePath);
   }

   if (blnIsDirectory)
      return copyDirectory(path, item, pathNew);

   wxFile file;
   if (file.Exists(filePath))
      return wxCopyFile(path + "\\" + item, pathNew + "\\" + item, true);

   return false;
}

class wxDirTraverserSimple : public wxDirTraverser
{
   public:
      wxDirTraverserSimple(wxString& path, wxString& pathNew, wxString& item) : path(path), pathNew(pathNew), item(item) { }
      virtual wxDirTraverseResult OnFile(const wxString& filename)
      {
         if (path+"\\"+item == pathNew.Left(path.Len()))
            return wxDIR_CONTINUE;
         wxString newFile = filename;
         newFile.Replace(path, pathNew);
         if (newFile == filename.Left(newFile.Len()))
            return wxDIR_CONTINUE;

         wxCopyFile(filename, newFile, true);
         return wxDIR_CONTINUE;
      }

      virtual wxDirTraverseResult OnDir(const wxString& dirname)
      {
         wxString oldPath = path + "\\" + item;
         if (oldPath == pathNew.Left(oldPath.Len()))
            return wxDIR_CONTINUE;
         wxString newDir = dirname;
         newDir.Replace(path, pathNew);
         wxMkdir(newDir);
         return wxDIR_CONTINUE;
      }
   private:
      wxString& path;
      wxString& pathNew;
      wxString& item;
};

bool copyDirectory(wxString& path, wxString& item, wxString& pathNew)
{
    wxDirTraverserSimple traverser(path, pathNew, item);
    wxMkdir(pathNew + "\\" + item);
    wxDir dir(path + "\\" + item);
    dir.Traverse(traverser);
    return true;
}

bool renameDirFile(wxString& path, wxString& oldName, wxString& newName)
{
   wxFile file;
   wxDir dir;
   wxString oldFilePath = path + "\\" + oldName;
   wxString newFilePath = path + "\\" + newName;

   return (rename(oldFilePath, newFilePath)==0);
}

wxString keyCodeToString(int keyCode)
{
   wxString key;
   switch ( keyCode )
   {
      case WXK_F1: key = _T("F1"); break;
      case WXK_F2: key = _T("F2"); break;
      case WXK_F3: key = _T("F3"); break;
      case WXK_F4: key = _T("F4"); break;
      case WXK_F5: key = _T("F5"); break;
      case WXK_F6: key = _T("F6"); break;
      case WXK_F7: key = _T("F7"); break;
      case WXK_F8: key = _T("F8"); break;
      case WXK_F9: key = _T("F9"); break;
      case WXK_F10: key = _T("F10"); break;
      case WXK_F11: key = _T("F11"); break;
      case WXK_F12: key = _T("F12"); break;
      case WXK_F13: key = _T("F13"); break;
      case WXK_F14: key = _T("F14"); break;
      case WXK_F15: key = _T("F15"); break;
      case WXK_F16: key = _T("F16"); break;
      case WXK_F17: key = _T("F17"); break;
      case WXK_F18: key = _T("F18"); break;
      case WXK_F19: key = _T("F19"); break;
      case WXK_F20: key = _T("F20"); break;
      case WXK_F21: key = _T("F21"); break;
      case WXK_F22: key = _T("F22"); break;
      case WXK_F23: key = _T("F23"); break;
      case WXK_F24: key = _T("F24"); break;
      default:
      {
         if ( keyCode > 0 && keyCode < 27 )
             key.Printf(_("Ctrl + %c"), _T('A') + keyCode - 1);
      }
  }
  return key;
}

driversMap wxGetDrives()
{
   driversMap drives;
 #ifdef __WXMSW__    
   char drive[4] = "?:\\";
   for(int c = 'A'; c <= 'Z'; c++)
   {
      *drive = c;
      int n = GetDriveType(drive);
      if(n == DRIVE_NO_ROOT_DIR || *drive == 'B') continue;
      drives[drive] = n;
   }
 #endif
   return drives;
}

wxString getLastDir(wxString directory)
{
   if (directory.Right(1) == "\\")
      directory = directory.Left(directory.Length()-1);
   return directory.AfterLast(wxT('\\'));
}

wxString getFileExtension(const wxString& strFileName)
{
   wxFileName fileName(strFileName);
   return fileName.GetExt();
}
