// wxext.cpp
// Copyright (c) 2006-2009 by Troels K. All rights reserved.
// License: wxWindows Library Licence, Version 3.1 - see LICENSE.txt

#include "precomp.h"
#include "wxext.h"

bool wxGetExeFolder(wxFileName* filename)
{
   filename->Assign(wxTheApp->argv[0]);
   filename->SetFullName(wxEmptyString);
   return filename->IsOk();
}

bool wxGetDevFolder(wxFileName* filename)
{
   wxGetExeFolder(filename);
   filename->RemoveLastDir();
   filename->AppendDir(wxTheApp->GetAppName().MakeLower());
   return filename->DirExists();
}

bool wxInitXRC()
{
   wxXmlResource::Get()->InitAllHandlers();

   wxString fullname = wxTheApp->GetAppName().Lower() + wxT(".xrc");

   wxFileName filename;
   wxGetExeFolder(&filename);
   filename.SetFullName(fullname);
   if (!filename.FileExists())
   {
      ::wxGetDevFolder(&filename);
      filename.AppendDir(wxT("src"));
      filename.AppendDir(wxT("res"));
      filename.SetFullName(fullname);
   }
   return wxXmlResource::Get()->Load(filename.GetFullPath());
}
