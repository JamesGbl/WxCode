#ifndef __ENGINE_DICTIONARY_DOWNLOADER__
#define __ENGINE_DICTIONARY_DOWNLOADER__

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


// Abstract base class comparable to wxSpellCheckEngineInterface with derived classes
//  handling the details needed for downloading and installing the dictionaries

class EngineDictionaryDownloader
{
public:  
  
  // Return the FTP server to connect to for the dictionaries
  virtual wxString GetServer() = 0;
  
  // Return the FTP server path were to find the dictionaries
  virtual wxString GetServerDirectory() = 0;
  
  // Given the filename (downloaded under the hard drive), install the dictionary
  virtual bool InstallDictionary(wxString& strFileName) = 0;
  
  // Parse the dictionary language name from the server FTP directory name
  virtual wxString DictionaryNameFromDirectoryName(wxString& strDirectoryName) { return strDirectoryName; }
  
  // Parse the dictionary language name from the server FTP directory name
  virtual wxString DirectoryNameFromDictionaryName(wxString& strDictionaryName) { return strDictionaryName; }
  
  // Determine which file to download from an array of filenames
  virtual wxString SelectDictionaryToDownload(wxArrayString& FileArray) = 0;
  
  // Return the wildcard for the dictionaries on the FTP server
  virtual wxString GetDictionaryFileMask() { return _(""); }
};

#endif  // __ENGINE_DICTIONARY_DOWNLOADER__
