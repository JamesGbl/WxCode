#include "AspellDictionaryDownloader.h"

bool AspellDictionaryDownloader::InstallDictionary(wxString& strFileName)
{
  ::wxMessageBox("Installing " + strFileName + " !!!");
  // For now, just use a test file (/tmp/aspell-el-0.50-3.zip)
  // The API to wxTarInputStream will hopefully be like
  //  wxZipInputStream so coding to the zip API will be sufficient
  
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
    return _("");
  }
}


