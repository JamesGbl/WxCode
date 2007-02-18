
#include "CTimer.h"

CTimer::CTimer(multiLang languageMap) : wxTimer()
{
  lang = languageMap;
  m_dlgProgress = NULL;
  Start(1000);
}

CTimer::~CTimer()
{
  if (m_dlgProgress)
  {
     m_dlgProgress->Destroy();
     m_dlgProgress = (wxProgressDialog*)NULL;
  }
}

void CTimer::Notify()
{
  size_t actualSize = 0;
  long actualNumFiles = 0;
  for (size_t i = 0 ; i < m_dirParamsVector.size(); i++)
  {
     wxString strActualSize = m_dirParamsVector[i].path + "\\" + m_dirParamsVector[i].item;
     actualSize += getDirSize(strActualSize, actualNumFiles);
  }
  if (m_dlgProgress == NULL)
  {
     m_dlgProgress = new wxProgressDialog
                    (
                     lang["Coping files"],
                     _T(""),
                     100,
                     NULL,
                     wxPD_ELAPSED_TIME |
                     wxPD_ESTIMATED_TIME |
                     wxPD_REMAINING_TIME
                    );
  }
  int percent = 0;
  if (totalSize>0)
     percent = ((actualSize/1000) * 100)/(totalSize/1000);

  wxString actualMessage;
  actualMessage = lang["Coping file"] + " ";
  actualMessage << actualNumFiles;
  actualMessage+= " " + lang["of"] + " ";
  actualMessage <<  numFiles;
  actualMessage+= " - " + formatFileSize(actualSize) + " " + lang["of"] + " " + formatFileSize(totalSize);
  m_dlgProgress->Update(percent, actualMessage);
  if (percent >= 100)
  {
    actualMessage = lang["Finalizing the copy. Wait a moment..."];
    m_dlgProgress->Update(100, actualMessage);
    this->Stop();
  }
}

void CTimer::addDir(wxString& path, wxString& item)
{
   dirParams dirPaths;
   dirPaths.path = path;
   dirPaths.item = item;
   m_dirParamsVector.push_back(dirPaths);
}
