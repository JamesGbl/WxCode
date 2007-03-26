
#ifndef __CTHREAD_h__
#define __CTHREAD_h__

#include <wx/progdlg.h>
#include <wx/thread.h>
#include "wxOpenCommanderUtils.h"

#define COPY 1

struct copyParams
{
   wxString sourcePath;
   wxString item;
   wxString newPath;
};

typedef vector<copyParams> vectorCopyParams;

class CThread : public wxThread
{
private:

   int action;
   vectorCopyParams m_copyParamsVector;
   onThreadFinishCallBackFunc onFinishCallBackFunc;
   onThreadBeginCopyFileCallBackFunc onBeginCopyFileCallBackFunc;
   onThreadEndCopyFileCallBackFunc onEndCopyFileCallBackFunc;
   void* finishParam;
   void* m_parent;
public:
    CThread() : wxThread() {}
    void* Entry();
    void copy(wxString& path, wxString& item, wxString& pathNew);
    void OnExit();
    void setParent(void* parent);
    void setFinishCallBackFunc(onThreadFinishCallBackFunc callBackFunc, void* yourParam);
    void setBeginCopyFileCallBackFunc(onThreadBeginCopyFileCallBackFunc callBackFunc);
    void setEndCopyFileCallBackFunc(onThreadEndCopyFileCallBackFunc callBackFunc);
    void setVectorCopyParams(const vectorCopyParams& copyParamsVector);

};
#endif
