#include "AspellDictionaryDownloader.h"
#include "bzipstream/bzipstream.h"

#include <wx/protocol/ftp.h>
#include <wx/filename.h>
#include <wx/wfstream.h>

bool AspellDictionaryDownloader::RetrieveDictionaryList(wxArrayString& DictionaryArray)
{
  wxFTP ftp;
  if ( !ftp.Connect(GetServer()) )
  {
      wxLogError("Couldn't connect");
      return false;
  }

  ftp.ChDir(GetServerDirectory());
  wxArrayString FtpListing;
  if (ftp.GetFilesList(FtpListing))
  {
    int FileCount = FtpListing.Count();
    for (int i=0; i<FileCount; i++)
    {
      // Here we let the dictionary downloader interpret the directory name
      //  and (if desireable) give back more descriptive text.  Also, the
      //  downloader can return an empty string if we don't want this
      //  dictionary in the list.
      wxString DictionaryListEntry = FtpListing[i];
      if (DictionaryListEntry != "")
        DictionaryArray.Add(DictionaryListEntry);
    }
  }
  else
  {
    ::wxMessageBox("Unable to retrieve listing of available dictionaries");
    return false;
  }
  return true;
}

wxString AspellDictionaryDownloader::DownloadDictionary(wxString& strDictionary)
{
  wxFTP ftp;
  if ( !ftp.Connect(GetServer()) )
  {
      wxLogError("Couldn't connect");
      return wxEmptyString;
  }
  ftp.ChDir(GetServerDirectory());
  ftp.ChDir(strDictionary);
  wxArrayString DictionaryFileList;
  if (ftp.GetFilesList(DictionaryFileList, _("*.bz2")))
  {
    wxString strDictionaryToDownload = SelectDictionaryToDownload(DictionaryFileList);
    wxInputStream *in = ftp.GetInputStream(strDictionaryToDownload);

    wxString strTempFile = wxFileName::CreateTempFileName(_("dict"));
    wxFileOutputStream TempFileStream(strTempFile);
    wxBufferedOutputStream TempBufferedStream(TempFileStream);
    // It might be better to switch to reading into a buffer so that we can provide
    //  progress indicator feedback.  For now, just one time it though.
    in->Read(TempBufferedStream);
    TempBufferedStream.Sync();
    int nServerSideSize = in->GetSize();
    int nDownloadedFileSize = wxFile(strTempFile).Length();
    if (nServerSideSize != nDownloadedFileSize)
    {
      ::wxMessageBox("Error downloading dictionary " + strDictionary);
      return wxEmptyString;
    }
    else
      return strTempFile;
  }
  else
  {
    ::wxMessageBox("Unable to retrieve listing of available dictionary files for " + strDictionary);
    return wxEmptyString;
  }
  return wxEmptyString;
}

bool AspellDictionaryDownloader::InstallDictionary(wxString& strFileName, bool bDeleteFileAfterInstall /*= false*/)
{
  //::wxMessageBox("Installing " + strFileName + " !!!");
  // For now, just use a test file (/tmp/aspell-el-0.50-3.zip)
  // The API to wxTarInputStream will hopefully be like
  //  wxZipInputStream so coding to the zip API will be sufficient
  wxFileInputStream FileIn(strFileName);
  wxBZipInputStream BZipIn(FileIn);

  wxString strTempFile = wxFileName::CreateTempFileName(_("tar"));
  wxFileOutputStream TempFileStream(strTempFile);
  wxBufferedOutputStream TempBufferedStream(TempFileStream);
  // It might be better to switch to reading into a buffer so that we can provide
  //  progress indicator feedback.  For now, just one time it though.
  BZipIn.Read(TempBufferedStream);
  TempBufferedStream.Sync();
  
  if (bDeleteFileAfterInstall)
  {
    ::wxRemoveFile(strFileName);
  }
  
  return true;
}

wxString AspellDictionaryDownloader::SelectDictionaryToDownload(wxArrayString& FileArray)
{
  // Loop through the array and return the most recent file
  // Eventually, we might want to check the Aspell version and make sure
  //  that we're getting a compatible version
  if (FileArray.GetCount() > 0)
  {
    return FileArray[FileArray.GetCount()-1]; // Just return the last file name in the array
  }
  else
  {
    return wxEmptyString;
  }
}


