
#ifndef __CTIMER_h__
#define __CTIMER_h__

#include <wx/timer.h>
#include <wx/progdlg.h>
#include "multiLang.h"
#include "wxOpenCommanderUtils.h"
#include "CThread.h"

class CTimer : wxTimer
{
public:
   struct dirParams
   {
      wxString path;
      wxString item;
   };
   vector<dirParams> m_dirParamsVector;
   wxProgressDialog* m_dlgProgress;
   size_t totalSize;
   long numFiles;
   multiLang lang;

  CTimer(multiLang languageMap);
  ~CTimer();
  virtual void Notify();
  void addDir(wxString& path, wxString& item);
};

#endif
