
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
   
   #ifdef __WXMSW__
      
      if (ext.Upper() == "EXE" || ext.Upper() == "BAT")
         command = path + "\\" + file; 
            
      STARTUPINFO sinfo;
      PROCESS_INFORMATION pinfo;
      memset(&sinfo,0,sizeof(STARTUPINFO));
      memset(&pinfo,0,sizeof(PROCESS_INFORMATION));
      sinfo.cb = sizeof(STARTUPINFO);

      CreateProcess(0, (char*)command.mb_str(), 0, 0, 0, 0, 0, (char*)path.mb_str(), &sinfo, &pinfo);

   #else
	   wxExecute(command, wxEXEC_ASYNC);
   #endif   
}

long long getDirSize(wxString& directoryFile)
{
   long numFiles;
   return getDirSize(directoryFile, numFiles);
}

long long getDirSize(wxString& directoryFile, long& numFiles)
{
   wxDir dir;
   wxString filename;
   long long totalSize = 0;
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

wxString LongLongTowxString(long long const& size)
{
   char buffer[sizeof(long long)];
   sprintf(buffer,"%lld",size);
   return buffer;
}

long long getFileSize(const wxString& fileName)
{
   struct _stati64 fileStat;
   int err = _stati64( fileName, &fileStat );

   if (err != 0) return 0;
   return fileStat.st_size;
}

wxString formatFileSize(__int64 st_size)
{
   wxString size;
   if (st_size >= 1000000000)
   {
     size = LongLongTowxString(st_size/10000000);
     size = size.Left(size.Len()-2) + "," + size.Right(2);
     size+=" Gb";
     return size;
   }
   if (st_size >= 1000000)
   {
     size = LongLongTowxString(st_size/10000);
     size = size.Left(size.Len()-2) + "," + size.Right(2);
     size+=" Mb";
     return size;
   }
   if (st_size >= 1000)
   {
     size = LongLongTowxString(st_size/1000);
     size+=" Kb";
     return size;
   }
   size = LongLongTowxString(st_size);
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
   if ((tmDateFile->tm_hour) == 23)
     hour = "00";
   else  
     hour << (tmDateFile->tm_hour+2);
   min << tmDateFile->tm_min;
   if (tmDateFile->tm_mday < 10) day = "0" + day;
   if (tmDateFile->tm_mon+1 < 10) month = "0" + month;
   if (tmDateFile->tm_hour+2 < 10) hour = "0" + hour;
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

bool copyDirFile(wxString& path, wxString& item, wxString& pathNew, void* parent, onThreadBeginCopyFileCallBackFunc onBeginCopyFile = NULL, onThreadEndCopyFileCallBackFunc onEndCopyFile = NULL)
{
   wxString filePath = path + "\\" + item;

   bool blnIsDirectory = false;
   {
     wxDir dir;
     blnIsDirectory = dir.Exists(filePath);
   }
   
   if (blnIsDirectory)
      return copyDirectory(path, item, pathNew, parent, onBeginCopyFile, onEndCopyFile);

   wxFile file;
   if (file.Exists(filePath))
   {
      bool blnCopy = false;
      int intCopy = COPY_FILE_OK;
      
      wxString itemNew = (path == pathNew ? "Copy of " + item : item);
      if (onBeginCopyFile) intCopy = onBeginCopyFile(parent, path + "\\" + item, pathNew + "\\" + itemNew);
      if (intCopy == COPY_FILE_OK) blnCopy = wxCopyFile(path + "\\" + item, pathNew + "\\" + itemNew, true);
      if (onEndCopyFile) onEndCopyFile(parent, blnCopy, path + "\\" + item, pathNew + "\\" + itemNew);
      return blnCopy;
   }

   return false;
}

class wxDirTraverserSimple : public wxDirTraverser
{
   public:
      wxDirTraverserSimple(wxString& path, wxString& pathNew, wxString& item, void* parent, onThreadBeginCopyFileCallBackFunc onBeginCopyFile = NULL, onThreadEndCopyFileCallBackFunc onEndCopyFile = NULL) : path(path), pathNew(pathNew), item(item)
      {  
         m_onBeginCopyFile = onBeginCopyFile;
         m_onEndCopyFile = onEndCopyFile;
         m_parent = parent;
      }
         
      virtual wxDirTraverseResult OnFile(const wxString& filename)
      {
         if (path+"\\"+item == pathNew.Left(path.Len()))
            return wxDIR_CONTINUE;
         wxString newFile = filename;
         newFile.Replace(path, pathNew);
         if (newFile == filename.Left(newFile.Len()))
            return wxDIR_CONTINUE;
  
         bool blnCopy = false;
         int intCopy = COPY_FILE_OK;

         //wxString itemNew (path == pathNew ) "Copy of " + item : item);
         if (m_onBeginCopyFile) intCopy = m_onBeginCopyFile(m_parent, filename, newFile);
         if (intCopy == COPY_FILE_OK) blnCopy = wxCopyFile(filename, newFile, true);
         if (m_onBeginCopyFile) m_onEndCopyFile(m_parent, blnCopy, filename, newFile);         
         return (intCopy != ABORT_COPY_PROCESS ? wxDIR_CONTINUE : wxDIR_STOP);
      }

      virtual wxDirTraverseResult OnDir(const wxString& dirname)
      {
         wxString oldPath = path + "\\" + item;
         if (oldPath == pathNew.Left(oldPath.Len()))
            return wxDIR_CONTINUE;
         wxString newDir = dirname;
         newDir.Replace(path, pathNew);
         if (!wxDir::Exists(newDir))
            wxMkdir(newDir);
         return wxDIR_CONTINUE;
      }
      
   private:
      wxString& path;
      wxString& pathNew;
      wxString& item;
      void* m_parent;
      onThreadBeginCopyFileCallBackFunc m_onBeginCopyFile; 
      onThreadEndCopyFileCallBackFunc m_onEndCopyFile;
};

bool copyDirectory(wxString& path, wxString& item, wxString& pathNew, void* parent, onThreadBeginCopyFileCallBackFunc onBeginCopyFile = NULL, onThreadEndCopyFileCallBackFunc onEndCopyFile = NULL)
{
    wxString newItem = (path == pathNew ? "Copy of " + item : item);
    
   if (!wxDir::Exists(pathNew + "\\" + newItem))
      wxMkdir(pathNew + "\\" + newItem);      
        
   wxDir dir(path + "\\" + newItem);
  
   wxDirTraverserSimple traverser(path, pathNew, item, parent, onBeginCopyFile, onEndCopyFile);
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
   char drive[4] = "?:";
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
