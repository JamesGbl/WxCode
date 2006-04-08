/////////////////////////////////////////////////////////////////////////////
// Name:        wxMSWResources.h

// Author:      Ernesto Rangel Dallet - midnight_software@yahoo.com.mx

// Created:     11/29/05 02:34:12

// Licence:     wxWidgets License

// Purpose:     header file for the wxMSWResources class. This class is  
//              designed to allow users of the MSW port of wxWidgets to
//              take advantage of the windows resource system, allowing
//              the creation of apps that do not require any aditional 
//              files to be distributed, like .mo catalogs, bitmaps etc.
//              You could of course manage your images using xpm, and 
//              compile them in your code, but xpm (as of this writting)
//              can only handle images with about 256 colors, and it cannot
//              handle other resources / files like .mo catalogs, wav files
//              etc.
//              The basic approach is, that any aditional files needed by
//              the app be compiled as part of the app큦 resources, and
//              this class will extract all files into a temp folder when
//              the app starts, so they can be used by the app, then the 
//              class will automatically delete the files & temp folder
//              when the application closes. Note that you are in no way
//              limited to adding only images or .mo catalogs, you can add 
//              *ANY* kind of binary/text file to your .rc file, and the 
//              class will extracted for you. You can even add the files
//              as a .zip file to keep the .exe큦 size low! and the class
//              will extract them, un-compress them and write the proper
//              files to disk!. It also provides "raw" access
//              to the resources data (via GetResData()) in case some 
//              resources need to be used in a diferent way.

// Notes:       Keep in mind that the destructor of this class deletes
//              the temp directory it created, and the extracted resources
//              inside it, so dont destroy the class instance unless you are
//              100% shure you wont be needing these resource files!. The
//              recommended aproach is that you add a member variable of this
//              class to your wxApp derived class.
//              IMPORTANT: while debuging you app, if it crashes (or you stop
//              execution), the destructor will NOT be called, so make shure
//              you remember to delete the tmp folder manually.
//              Also keep in mind that any resources extracted to a "custom"
//              directory (specified as last parameter to the ExtractXXX functions)
//              will *NOT* be deleted automatically by the class, you need to
//              delete these files yourself.

// Usage:
//              The code is well commented, so you shoudnt have much trouble
//              understanding how it works, I also sugest you check out the 
//              provided sample app(s).

/////////////////////////////////////////////////////////////////////////////

#ifndef _WXMSWRES_H_
#define _WXMSWRES_H_


#ifdef __WXMSW__ //// needless to say, this class only works under msw

class wxMSWResources
{
    DECLARE_NO_COPY_CLASS(wxMSWResources)

public:
    // constants for common base extract directory paths
    static const wxChar* BEXD_TEMP_DIR; // use system큦 TEMP dir
    static const wxChar* BEXD_HOME_DIR; // use the user큦 home dir
    static const wxChar* BEXD_APP_DIR;  // use the applications running dir
    static const wxChar* BEXD_NO_CREATE;// Dont create any default dir

    ////////// Construction / Destruction //////////

    // constructor, takes a constant to one of the common extract
    // base directories, or an absolute path to user defined 
    // directory wich must allready exsist
    wxMSWResources(const wxChar* BaseDir = BEXD_TEMP_DIR);

    // destructor, deletes the default extraction dir and files inside it
    ~wxMSWResources();


    ////////// Resource extraction //////////

    // extract resource ResName of type ResType to ExDir (default if NULL)
    bool ExtractResource(const wxChar* ResName, const wxChar* ResType,
        const wxChar* ExDir = NULL);

    // extract all resources of ResType to ExDir (default dir if NULL).
    // if not NULL, *Extracted will have number of extracted resources
    bool ExtractResources(const wxChar* ResType, int* Extracted = NULL, 
        const wxChar* ExDir = NULL);

    // extract all ziped resources from ResName to ExDir (default dir if NULL)
    bool ExtractZipResources(const wxChar* ResName, const wxChar* ResType, 
        int* pExtracted = NULL, const wxChar* ExDir = NULL);

    // get the "raw" resource data, its size, and a GDI handle to (it?)
    void* GetResData(const wxChar* ResName, const wxChar* ResType, 
        HGLOBAL& rHandle, DWORD& rSize);


    ////////// Resource Access //////////

    // get the full path & name of the extracted resource file
    wxString GetResourcePath(const wxChar* ResName)const;

    // short version of the GetResourcePath() in case u h8 typing :)
    inline wxString GetRcP(const wxChar* ResName)const
    { return GetResourcePath(ResName); }

    // check if the resource ResName is available in the resource dir
    bool RcExtracted(const wxChar* ResName)const
    { return (m_ResourceDir.IsEmpty() ? false : 
        wxFileExists(m_ResourceDir + wxFILE_SEP_PATH + ResName));
    }
    
    // get the temp directory where resource files are being stored
    wxString GetResourceDir()
    {
        return m_BaseExDir==BEXD_NO_CREATE ? 
        wxEmptyString : GetAbsoluteExPath(NULL);
    }

    // private helpers
private:
    // delete the temp directory and resource files
    void CleanUp();
    
    // build the path to extraction dir.
    wxString GetExDir();

    // get the absolute path to the dir to extract resources
    wxString GetAbsoluteExPath(const wxChar* ExDir);

    // write a reswource file from data
    bool WriteResourceFile(const wxChar* ResFile, void* pResData, DWORD DtaSize);

    // ennumerate names of resources of ResType available in the module
    bool GetAvailableResNames(const wxChar* ResType, wxArrayString& Names);

    // W32 API defined resource enum function
    static BOOL CALLBACK ResEnumProc(HMODULE hModule, LPCTSTR lpszType, 
        LPTSTR lpszName, LONG_PTR lParam);


    // private data members
private:
    wxString m_BaseExDir;
    wxString m_ResourceDir;
};

#endif // __WXMSW__

#endif // !_WXMSWRES_H_