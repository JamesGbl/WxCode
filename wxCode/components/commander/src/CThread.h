
#ifndef __CTHREAD_h__
#define __CTHREAD_h__

#include <wx/progdlg.h>
#include <wx/thread.h>
//#include "wxOpenCommanderUtils.h"
#include "cCommander.h"

#define COPY 1
#define ADD_PATH_RECURSIVE 2

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
   cCommander* m_cCommanderAct;
   wxArrayString m_aFilesPath;
public:
   long long totalSizeRecursive;
public:
    CThread() : wxThread() {}
    void* Entry();
    void OnExit();
    void setParent(void* parent);
    void setFinishCallBackFunc(onThreadFinishCallBackFunc callBackFunc, void* yourParam);
    void setBeginCopyFileCallBackFunc(onThreadBeginCopyFileCallBackFunc callBackFunc);
    void setEndCopyFileCallBackFunc(onThreadEndCopyFileCallBackFunc callBackFunc);
    void setVectorCopyParams(const vectorCopyParams& copyParamsVector);
    void addPathsRecursive(cCommander* cCommanderAct, wxArrayString aFilesPath);

};
#endif
