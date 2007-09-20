
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
   case ADD_PATH_RECURSIVE:
   {
      m_cCommanderAct->addPathsRecursive(m_aFilesPath); 
      totalSizeRecursive = 0;
      for (size_t i = 0 ; i < m_aFilesPath.Count(); i++)
      {
         totalSizeRecursive = totalSizeRecursive + getDirSize(m_aFilesPath[i]);
      }
   }
   break;
 }
 return NULL;
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

void CThread::addPathsRecursive(cCommander* cCommanderAct, wxArrayString aFilesPath)
{
   action = ADD_PATH_RECURSIVE;
   m_cCommanderAct = cCommanderAct;
   m_aFilesPath = aFilesPath;
}

void CThread::OnExit()
{
   if (onFinishCallBackFunc != NULL)
      onFinishCallBackFunc((void*)this, finishParam, m_parent);
   
   this->Wait();   
   this->Exit();
   this->Delete();
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
