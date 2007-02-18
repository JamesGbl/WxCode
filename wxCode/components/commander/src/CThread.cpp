
#include "CThread.h"

void* CThread::Entry()
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

void CThread::copy(wxString& path, wxString& item, wxString& pathNew)
{
 action = 1;
 copyParams copyPaths;
 copyPaths.sourcePath = path;
 copyPaths.item = item;
 copyPaths.newPath = pathNew;
 m_copyParamsVector.push_back(copyPaths);
}

void CThread::OnExit()
{
   if (onFinishCallBackFunc != NULL)
      onFinishCallBackFunc(this, finishParam, m_parent);
      
   this->Exit();
   delete(this);
}

void CThread::setFinishCallBackFunc(onThreadFinishCallBackFunc callBackFunc, void* yourParam, void* parent)
{
   onFinishCallBackFunc = callBackFunc;
   finishParam = yourParam;
   m_parent = parent;
}
