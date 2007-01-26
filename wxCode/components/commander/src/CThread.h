
#ifndef __CTHREAD_h__
#define __CTHREAD_h__

#include <wx/progdlg.h>
#include <wx/thread.h>
#include "wxCommanderUtils.h"

typedef void (*onThreadFinishCallBackFunc)(wxThread*, void*);

class wxSimpleThread : public wxThread
{
private:
   struct copyParams
   {
      wxString sourcePath;
      wxString item;
      wxString newPath;
   };
   int action;
   vector<copyParams> m_copyParamsVector;
   onThreadFinishCallBackFunc onFinishCallBackFunc;
   void* finishParam;
public:
    wxSimpleThread() : wxThread()
    {
    }
    void* Entry()
    {
       switch (action)
       {
         case 1:
         {
            for (size_t i = 0 ; i < m_copyParamsVector.size(); i++)
            {
               copyDirFile(m_copyParamsVector[i].sourcePath, m_copyParamsVector[i].item, m_copyParamsVector[i].newPath);
            }
         }
         break;
      }
      return NULL;
    }
    void copy(wxString& path, wxString& item, wxString& pathNew)
    {
       action = 1;
       copyParams copyPaths;
       copyPaths.sourcePath = path;
       copyPaths.item = item;
       copyPaths.newPath = pathNew;
       m_copyParamsVector.push_back(copyPaths);
    }
    void OnExit()
    {
       if (onFinishCallBackFunc != NULL)
          onFinishCallBackFunc(this, finishParam);

       this->Exit();
       delete(this);
    }
    void setFinishCallBackFunc(onThreadFinishCallBackFunc callBackFunc, void* yourParam)
    {
       onFinishCallBackFunc = callBackFunc;
       finishParam = yourParam;
    }
};
#endif
