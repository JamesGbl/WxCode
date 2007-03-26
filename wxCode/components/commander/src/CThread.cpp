
#include "CThread.h"

void* CThread::Entry()
{
 switch (action)
 {
   case COPY:
   {
      for (size_t i = 0 ; i < m_copyParamsVector.size(); i++)
      {
         copyDirFile(m_copyParamsVector[i].sourcePath, m_copyParamsVector[i].item, m_copyParamsVector[i].newPath, m_parent, onBeginCopyFileCallBackFunc, onEndCopyFileCallBackFunc);
      }
   }
   break;
 }
 return NULL;
}

void CThread::copy(wxString& path, wxString& item, wxString& pathNew)
{
  action = COPY;
  copyParams copyPaths;
  copyPaths.sourcePath = path;
  copyPaths.item = item;
  copyPaths.newPath = pathNew;
  m_copyParamsVector.push_back(copyPaths);
}

void CThread::setParent(void* parent)
{
   m_parent = parent;
}

void CThread::setVectorCopyParams(const vectorCopyParams& copyParamsVector)
{
   action = COPY;
   m_copyParamsVector = copyParamsVector;
}

void CThread::OnExit()
{
   if (onFinishCallBackFunc != NULL)
      onFinishCallBackFunc(this, finishParam, m_parent);
      
   this->Exit();
   delete(this);
}

void CThread::setFinishCallBackFunc(onThreadFinishCallBackFunc callBackFunc, void* yourParam = NULL)
{
   onFinishCallBackFunc = callBackFunc;
   finishParam = yourParam;
}

void CThread::setBeginCopyFileCallBackFunc(onThreadBeginCopyFileCallBackFunc callBackFunc)
{
   onBeginCopyFileCallBackFunc = callBackFunc;
}

void CThread::setEndCopyFileCallBackFunc(onThreadEndCopyFileCallBackFunc callBackFunc)
{
   onEndCopyFileCallBackFunc = callBackFunc;
}
