#ifndef __ASPELL_DICTIONARY_DOWNLOADER__
#define __ASPELL_DICTIONARY_DOWNLOADER__

#include "EngineDictionaryDownloader.h"

class AspellDictionaryDownloader : public EngineDictionaryDownloader
{
public:
  
  // Populate DictionaryArray with a list of the available dictionaries
  virtual bool RetrieveDictionaryList(wxArrayString& DictionaryArray);

  // Download the given dictionary and return the local file name
  virtual wxString DownloadDictionary(wxString& strDictionary);
  
  // Given the filename (downloaded under the hard drive), install the dictionary
  virtual bool InstallDictionary(wxString& strFileName, bool bDeleteFileAfterInstall = false);

  // Return the FTP server to connect to for the dictionaries
  virtual wxString GetServer() { return _("ftp.gnu.org"); }
  
  // Return the FTP server path were to find the dictionaries
  virtual wxString GetServerDirectory() { return _("/gnu/aspell/dict"); }

  // Determine which file to download from an array of filenames
  wxString SelectDictionaryToDownload(wxArrayString& FileArray);
};

#endif  // __ASPELL_DICTIONARY_DOWNLOADER__
