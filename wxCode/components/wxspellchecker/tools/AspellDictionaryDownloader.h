#ifndef __ASPELL_DICTIONARY_DOWNLOADER__
#define __ASPELL_DICTIONARY_DOWNLOADER__

#include "EngineDictionaryDownloader.h"

class AspellDictionaryDownloader : public EngineDictionaryDownloader
{
public:
  // Return the FTP server to connect to for the dictionaries
  virtual wxString GetServer() { return _("ftp.gnu.org"); }
  
  // Return the FTP server path were to find the dictionaries
  virtual wxString GetServerDirectory() { return _("/gnu/aspell/dict"); }
  
  // Given the filename (downloaded under the hard drive), install the dictionary
  virtual bool InstallDictionary(wxString& strFileName);
  
  // Determine which file to download from an array of filenames
  virtual wxString SelectDictionaryToDownload(wxArrayString& FileArray);
  
  // Return the wildcard for the dictionaries on the FTP server
  virtual wxString GetDictionaryFileMask() { return _("*.bz2"); }
};

#endif  // __ASPELL_DICTIONARY_DOWNLOADER__
