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
  
  // Parse the dictionary language name from the server FTP directory name
  virtual wxString DictionaryNameFromDirectoryName(wxString& strDirectoryName);
};

#endif  // __ASPELL_DICTIONARY_DOWNLOADER__
