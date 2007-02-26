
#ifndef __CTHREAD_h__
#define __CTHREAD_h__

#include <wx/progdlg.h>
#include <wx/thread.h>
#include "wxCommanderUtils.h"

#define COPY 1

typedef void (*onThreadFinishCallBackFunc)(wxThread*, void*, void*);

class CThread : public wxThread
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
   void* m_parent;
public:
    CThread() : wxThread() { }
    void* Entry();
    void copy(wxString& path, wxString& item, wxString& pathNew);
    void OnExit();
    void setFinishCallBackFunc(onThreadFinishCallBackFunc callBackFunc, void* yourParam, void* parent);
};
#endif
