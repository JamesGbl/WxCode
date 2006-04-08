/////////////////////////////////////////////////////////////////////////////
// Name:        wxMSWResources.cpp

// Author:      Ernesto Rangel Dallet - midnight_software@yahoo.com.mx

// Created:     11/29/05 02:34:12

// Licence:     wxWidgets License

// Purpose:     implementation file for the wxMSWResources class.   

/////////////////////////////////////////////////////////////////////////////

#ifdef __WXMSW__ //// needless to say, this class only works under msw
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/wxmswres.h"
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/mstream.h>
#include <wx/zipstrm.h>

// smart ptr needed for zip data reading
#include <wx/ptr_scpd.h>
wxDEFINE_SCOPED_PTR_TYPE(wxZipEntry);

// constants for common base extract directory paths
const wxChar* wxMSWResources::BEXD_TEMP_DIR = wxT("<tm");
const wxChar* wxMSWResources::BEXD_HOME_DIR = wxT("<uh");
const wxChar* wxMSWResources::BEXD_APP_DIR =  wxT("<ap");
const wxChar* wxMSWResources::BEXD_NO_CREATE =wxT("<nc");

// constructor, takes a constant to one of the common extract
// base directories, or an absolute path to user defined 
// directory wich must allready exsist
wxMSWResources::wxMSWResources(const wxChar* BaseDir)
{
    wxASSERT(BaseDir); // must NOT be NULL!
    m_BaseExDir=BaseDir;
    wxASSERT(!m_BaseExDir.IsEmpty()); // must not be empty!
}

// destructor, deletes the default extraction dir and files inside it
wxMSWResources::~wxMSWResources()
{
    // delete our default extraction directory and extracted files in it
    CleanUp();
}

// create the extraction dir if it doesnt exsists, unless
// it is user defined in wich case we dont create it
wxString wxMSWResources::GetAbsoluteExPath(const wxChar* UsrDir)
{
    wxString thePath;
    if(UsrDir!=NULL)
    {
        // user has specified an absolute path for this particular resource.
        thePath=UsrDir;
        if(thePath.IsEmpty())
            return thePath; // empty???

        // we dont create a custom directory, so make shure its there
        if(wxDirExists(thePath))
        {
            if(thePath.Last()!=wxFILE_SEP_PATH)
                thePath+=wxFILE_SEP_PATH;

            return thePath;
        }
        else
        {
            // it doesnt exsists, fail
            wxLogDebug(wxT("wxMSWResources: ERROR - specified extraction directory:%s\nDoes NOT exsist"),
                thePath.c_str());
            return wxEmptyString;
        }
    }
    else
    {
        if(m_BaseExDir==BEXD_NO_CREATE)
        {
            // we were asked NOT to create a temp directory in constructor,
            // yet user is asking to extract the resource to it! (duh)
            wxLogDebug(wxT("wxMSWResources: ERROR - extract dir doesnt exsist. BEXD_NO_CREATE flag was used"));
            return wxEmptyString;
        }

        // use the default path, passed to constructor, could be
        // a const meaning a usual dir like TEMP, etc, or a user defined,
        // check if we allready have it, if not, get it now      
        if(!m_ResourceDir.IsEmpty())
        {
            // allready have the path, meaning we allready created the tmp dir
            // just make shure its still there
            if(wxDirExists(m_ResourceDir))
                return m_ResourceDir + wxFILE_SEP_PATH;
            else
                return wxEmptyString; // WTF? someone must have deleted it!
        }
        else
        {
            // dont have it (must be first time this is called) so get the
            // path and create the directory
            thePath=GetExDir();
            if(thePath.IsEmpty())
                return thePath; // failed to get the path!
            wxLogDebug(wxT("wxMSWResources: Creating temp directory %s"), 
                thePath.c_str());
            if(wxMkdir(thePath))
            {
                // everyting is sweet.
                m_ResourceDir = thePath;
                return m_ResourceDir + wxFILE_SEP_PATH;
            }
            else
            {
                // failed to create the dir!
                wxLogDebug(wxT("wxMSWResources: ERROR - failed to create default extraction directory:\n%s"),
                    thePath.c_str());
                return wxEmptyString;
            }
        }
    }
}

// get the "raw" resource data, its size, and a GDI handle to it
void* wxMSWResources::GetResData(const wxChar* ResName, const wxChar* ResType, 
                                 HGLOBAL& rHandle, DWORD& rSize)
{
    // find the resource in our module (note this is mostly W32 code)
    rHandle=NULL;
    HINSTANCE hMyInst = wxGetInstance();

    HRSRC hres = FindResource(hMyInst, ResName, ResType);
    if(hres==NULL)
    {
        wxLogDebug(wxT("wxMSWResources: ERROR - resource %s of type %s not found in module"), 
            ResName, ResType);
        return NULL; // resource not found!
    }
    // get its size
    rSize=SizeofResource(hMyInst, hres);

    rHandle = LoadResource(hMyInst, hres);
    if(rHandle==NULL)
    {
        wxLogDebug(wxT("wxMSWResources: ERROR - Failed to load resource %s of type %s from module"), 
            ResName, ResType);
        return NULL; // resource not found!
    }

    // now get the motha!
    void* pData = LockResource(rHandle);
    if(pData==NULL)
    {
        wxLogDebug(wxT("wxMSWResources: ERROR - Failed to lock resource %s of type %s from module"), 
            ResName, ResType);
    }
    return pData;
}

// write a reswource file from data
bool wxMSWResources::WriteResourceFile(const wxChar* ResFile, void* pResData, DWORD DtaSize)
{
    // create the file (dont overwrite if allready there!)
    wxFile theFile;
    if(!theFile.Create(ResFile, false))
    {
        wxLogDebug(wxT("wxMSWResources: ERROR - Failed to create resource file %s"),
            ResFile);
        return false;
    }

    // write the data to it
    size_t written = theFile.Write(pResData, (size_t)DtaSize);
    theFile.Close();
    return written==(size_t)DtaSize;
}

// extract resource ResName of type ResType to ExDir (default if NULL)
bool wxMSWResources::ExtractResource(const wxChar* ResName, const wxChar* ResType, 
                                     const wxChar* ExDir)
{
    // build the full output file name
    wxString fileName=GetAbsoluteExPath(ExDir);
    if(fileName.IsEmpty())
        return false; // no dir to extract to!
    fileName+=ResName;

    // extract the resource
    HGLOBAL hResHandle = NULL;
    DWORD resSize=0L;
    void* pResource = GetResData(ResName, ResType, hResHandle, resSize);
    if(pResource==NULL)
        return false; // oops!

    // now write it to a file
    bool ok = WriteResourceFile(fileName, pResource, resSize);

    // release the GDI resources used by resource
    DeleteObject((HGDIOBJ)hResHandle);

    return ok;
}

// extract all resources of ResType to ExDir (default dir if NULL)
bool wxMSWResources::ExtractResources(const wxChar* ResType, int* Extracted, const wxChar* ExDir)
{
    if(Extracted)
        *Extracted=0;

    // get a list of available resources of this type
    wxArrayString names;
    if(!GetAvailableResNames(ResType, names))
    {
        wxLogDebug(wxT("wxMSWResources: ERROR - Failed to ennumerate available resources"));
        return false;
    }

    // and extract them
    int count = 0;
    bool ok = true;
    wxString resName;
    while(count<(int)names.size() && ok)
    {
        resName = names[count].MakeLower(); // enumresources returns names in UPPER
        ok = ExtractResource(resName, ResType, ExDir);
        if(ok)
            count++;
    }
    if(Extracted)
        *Extracted = count;
    return ok;
}

// extract all ziped resources from ResName to ExDir (default dir if NULL)
bool wxMSWResources::ExtractZipResources(const wxChar* ResName, 
                                         const wxChar* ResType, 
                                         int* pExtracted,
                                         const wxChar* ExDir)
{
    if(pExtracted)
        *pExtracted=0;

    // get the output dir path
    wxString exPath=GetAbsoluteExPath(ExDir);
    if(exPath.IsEmpty())
        return false; // no dir to extract to!

    // get the raw data from the (hopefully) zipped resource in the module
    DWORD dtaSize=0L;
    HGLOBAL hRes=NULL;
    void* pData=NULL;
    pData=GetResData(ResName, ResType, hRes, dtaSize);
    if(!pData || dtaSize==0L)
        return false;

    // now create a memory stream from it to pass to a zip stream
    // and write a file from each (unzipped)entry in it
    wxZipEntryPtr zipEntry;
    wxMemoryInputStream rawStream(pData, dtaSize); 
    wxZipInputStream zipStream(rawStream);
    bool ok=zipStream.IsOk();
    wxFileOffset entrySize=0;
    // get the entries (code snipped borrowed from wxWidgets docs) :)
    wxString resName;
    while(ok, zipEntry.reset(zipStream.GetNextEntry()), zipEntry.get()!=NULL)
    {
        // we only extract from the root of the zip file (no dirs)
        if(!zipEntry->IsDir())
        {
            resName=zipEntry->GetName();
            entrySize=zipEntry->GetSize();
            if(!resName.IsEmpty() && entrySize>0)
            {
                void* buffer = new wxByte[entrySize];
                ok=(zipStream.Read(buffer, entrySize).LastRead()==entrySize);
                if(ok)
                {
                    ok=WriteResourceFile(exPath + resName, buffer, entrySize);
                    (*pExtracted)++;
                }
                delete[]buffer;
            }
        }
    }
    return ok;
}

// get the full path & name of the extracted resource file
wxString wxMSWResources::GetResourcePath(const wxChar* ResName)const
{
    if(m_ResourceDir.IsEmpty())
        return wxEmptyString; // no default extract dir == no resources in it!
    wxString theFile= m_ResourceDir + wxFILE_SEP_PATH + ResName;
    if(wxFileExists(theFile))
        return theFile;
    else
        return wxEmptyString; // file is not there!

}

void wxMSWResources::CleanUp()
{
    // delete our temp directory and all files inside it(if created)
    if(!m_ResourceDir.IsEmpty() && wxDirExists(m_ResourceDir))
    {
        wxLogDebug(wxT("wxMSWResources: Deleting temp resource dir & files in:%s"),
            m_ResourceDir.c_str());
        wxArrayString names;
        wxDir::GetAllFiles(m_ResourceDir, &names, wxEmptyString, wxDIR_FILES);
        for(size_t i=0; i<names.GetCount(); i++)
        {
            if(!wxRemoveFile(names[i]))
                wxLogDebug(wxT("wxMSWResources: WARNING . Failed to delete file:%s"),
                names[i].c_str());
        }
        if(!wxRmdir(m_ResourceDir))
        {
            wxLogDebug(
                wxT("wxMSWResources: WARNING - Failed to delete the temp directory:\n%s"),
                m_ResourceDir.c_str());
        }
    }
    m_ResourceDir = wxT("");
}

// ennumerate resources of ResType available in the module
bool wxMSWResources::GetAvailableResNames(const wxChar* ResType, wxArrayString & Names)
{
    // enum resources of type ResType on the app´s module (.exe)
    BOOL enumOk = EnumResourceNames(wxGetInstance(), ResType, 
        ResEnumProc, (WPARAM)&Names);
    if(enumOk==0)
        return false;

    return true;
}

BOOL wxMSWResources::ResEnumProc(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam)
{
    // were being passed a wxArrayString (pointer) to store the name
    wxASSERT(lParam);
    wxArrayString* pNames = (wxArrayString*)(WPARAM)lParam;
    wxString resName(lpszName);
    pNames->Add(resName);
    return 1;
}

wxString wxMSWResources::GetExDir()
{
    // we should have a default base dir!, a defined constant to
    // one of the usual dirs or a user specified dir
    wxASSERT(!m_BaseExDir.IsEmpty());
    
    wxString baseDir;
    if(m_BaseExDir==BEXD_TEMP_DIR)
    {
        // use the windows temp directory. so get its path...
        wxChar buffer[1024] = wxT("\0");
        if(GetTempPath(1024, buffer)!=0)
            baseDir = buffer;
    }
    else if(m_BaseExDir==BEXD_HOME_DIR)
    {
        // use the current user´s home dir. 
        // lets use wxGetHomeDir() (easier this way that using W32 methods)
        baseDir = wxGetHomeDir();
    }
    else if(m_BaseExDir==BEXD_APP_DIR)
    {
        // use the app´s dir. (lets hope the app is not running from a CD!)
        wxChar buffer[1024] = wxT("\0");
        if(GetModuleFileName(NULL, buffer, 1024)!=0)
            baseDir = wxPathOnly(buffer);
    }
    else
    {
        // user specified a custom directory in the constructor, use it
        // we do NOT create it if it doesnt exsists
        baseDir=m_BaseExDir;
    }

    // we should have a base directory name by now, if not,
    // something went wrong, so return empty string
    if(baseDir.IsEmpty())
        return wxEmptyString;

    // the base directory should actually exsist, if it doesnt, fail.
    if(!wxDirExists(baseDir))
        return wxEmptyString;

    // make shure it ends with "\"
    if(baseDir.Last()!=wxFILE_SEP_PATH)
        baseDir+=wxFILE_SEP_PATH;

    // now generate a unique name for the actual extract directory
    // to create inside the base directory in format myapp_rc##.tmp
    wxString dirName=wxTheApp->GetAppName() + wxT("_rc.tmp");
    wxString fullDir = baseDir + dirName;
    int num=0;
    while(wxDirExists(fullDir))
    {
        // a dir with same name allready exsists, try again with diferent name
        dirName=wxString::Format(wxT("%s_rc%d.tmp"), 
            wxTheApp->GetAppName().c_str(), num);
        fullDir = baseDir + dirName;
        num++;
    }

    return fullDir;
}

#endif // __WXMSW__
