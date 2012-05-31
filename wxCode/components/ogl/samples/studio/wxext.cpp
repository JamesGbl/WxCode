// wxext.cpp
// Copyright (c) 2006-2011 by Troels K. All rights reserved.
// License: wxWindows Library Licence, Version 3.1 - see LICENSE.txt

#include "precomp.h"
#include <wx/stdpaths.h>

#include "wxext.h"

bool wxGetExeFolder(wxFileName* filename)
{
   filename->Assign(wxStandardPaths::Get().GetExecutablePath());
   filename->SetFullName(wxEmptyString);
   return filename->IsOk();
}

bool wxGetDevFolder(wxFileName* filename)
{
   wxGetExeFolder(filename);
   filename->RemoveLastDir();
   filename->AppendDir(wxTheApp->GetAppName().Lower());
   return filename->DirExists();
}

bool wxInitXRC()
{
   const wxString fullname = wxTheApp->GetAppName().Lower() + wxT(".xrc");
   wxFileName filename;

   wxGetExeFolder(&filename);
   filename.SetFullName(fullname);
   if (!filename.FileExists())
   {
      wxFileName dev;

      ::wxGetDevFolder(&dev);
      dev.AppendDir(wxT("src"));
      dev.AppendDir(wxT("res"));
      dev.SetFullName(fullname);
      if (dev.FileExists())
      {
         filename = dev;
      }
   }
   wxXmlResource::Get()->InitAllHandlers();
   return wxXmlResource::Get()->Load(filename.GetFullPath());
}
