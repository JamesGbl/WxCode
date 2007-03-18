
#ifndef __MULTILANG_h__
#define __MULTILANG_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif

#include <wx/intl.h>
#include <wx/dir.h>
#include <wx/textfile.h>
#include <map>
#include <vector>

using namespace std;

typedef map <wxString, wxString, less<wxString> > stringMap;

class multiLang
{
   public:
      multiLang();
      multiLang(const multiLang& copyLang);
      ~multiLang();
      
      void setActualLang(wxString language);
      void setLangsDir(wxString langDir);      
      wxString getActualLang();
      wxArrayString getListLangs();
      wxString getSystemLanguage();
      bool updateLangMap(wxString langDir, wxString language);
      bool updateLangMap();
      
      wxString operator[](wxString index);
      multiLang& operator=(const multiLang& copyLang);
   private:
      stringMap langMap;
      wxArrayString langList;
      wxString m_langsDir;
      wxString m_systemLangName;
      wxString m_actualLang;
};

#endif
