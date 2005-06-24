/////////////////////////////////////////////////////////////////////////////
// Name:        webupdate.cpp
// Purpose:     wxUpdateCheck
// Author:      Francesco Montorsi
// Created:     2005/06/23
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
	#pragma implementation "webupdate.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#ifndef WX_PRECOMP
#endif

#include "wx/url.h"
#include "wx/webupdate.h"



// ---------------------
// wxWEBUPDATEDOWNLOAD
// ---------------------

wxWebUpdatePlatform wxWebUpdateDownload::GetThisPlatform()
{
       switch ( wxGetOsVersion() )
        {
            case wxMOTIF_X:
                return wxWUP_MOTIF;
                
            case wxMAC:
            case wxMAC_DARWIN:
                return wxWUP_MAC;
                
            case wxGTK:
            case wxGTK_WIN32:
            case wxGTK_OS2:
            case wxGTK_BEOS:
                return wxWUP_GTK;
                
            case wxWINDOWS:
            case wxPENWINDOWS:
            case wxWINDOWS_NT:
            case wxWIN32S:
            case wxWIN95:
            case wxWIN386:
                return wxWUP_MSW;
                
            case wxMGL_UNIX:
            case wxMGL_X:
            case wxMGL_WIN32:
            case wxMGL_OS2:
                return wxWUP_MGL;
                
            case wxWINDOWS_OS2:
            case wxOS2_PM:
                return wxWUP_OS2;
                
            default:
                return wxWUP_INVALID;    
        }
}



// ---------------
// wxUPDATECHECK
// ---------------
/*
CUpdateCheck::wxUpdateCheck()
{

}

wxUpdateCheck::~wxUpdateCheck()
{
}

BOOL wxUpdateCheck::GetFileVersion(DWORD &dwMS, DWORD &dwLS)
{
	char szModuleFileName[MAX_PATH];

    LPBYTE  lpVersionData; 

	if (GetModuleFileName(AfxGetInstanceHandle(), szModuleFileName, sizeof(szModuleFileName)) == 0) return FALSE;

    DWORD dwHandle;     
    DWORD dwDataSize = ::GetFileVersionInfoSize(szModuleFileName, &dwHandle); 
    if ( dwDataSize == 0 ) 
        return FALSE;

    lpVersionData = new BYTE[dwDataSize]; 
    if (!::GetFileVersionInfo(szModuleFileName, dwHandle, dwDataSize, (void**)lpVersionData) )
    {
		delete [] lpVersionData;
        return FALSE;
    }

    ASSERT(lpVersionData != NULL);

    UINT nQuerySize;
	VS_FIXEDFILEINFO* pVsffi;
    if ( ::VerQueryValue((void **)lpVersionData, _T("\\"),
                         (void**)&pVsffi, &nQuerySize) )
    {
		dwMS = pVsffi->dwFileVersionMS;
		dwLS = pVsffi->dwFileVersionLS;
		delete [] lpVersionData;
        return TRUE;
    }

	delete [] lpVersionData;
    return FALSE;

}

void wxUpdateCheck::Check(UINT uiURL)
{
	CString strURL(MAKEINTRESOURCE(uiURL));
	Check(strURL);
}
*/

wxUpdateCheckFlag wxUpdateCheck::Check(const wxString &version,
                                       const wxString &packagename)
{
    // first of all, we need to open a connection to the given url
    wxURL url(strURL);
    if (url.GetError() != wxURL_NOERR) {

        return UCF_FAILED;
    }
    
	wxInputStream *xml = url.GetInputStream();
	if (!xml) {
		
        return UCF_FAILED;
	}

    // now it's time to parse the XML file we expect to be in 'xml' input stream
    wxXmlDocument doc(xml);
    wxXmlNode *root = doc.GetRoot();
    if (!ParseXMLUpdateScript(root)) {

        return UCF_FAILED;
    }

    // do the real version check
    if (m_arrPackages
}

bool wxUpdateCheck::Load(const wxString &strURL,     )
{
    // refer to "webupdate.dtd" for a definition of the XML webupdate info script
}

/*
HINSTANCE wxUpdateCheck::GotoURL(LPCTSTR url, int showcmd)
{
    TCHAR key[MAX_PATH + MAX_PATH];

    // First try ShellExecute()
    HINSTANCE result = ShellExecute(NULL, _T("open"), url, NULL,NULL, showcmd);

    // If it failed, get the .htm regkey and lookup the program
    if ((UINT)result <= HINSTANCE_ERROR) 
	{

        if (GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) 
		{
            lstrcat(key, _T("\\shell\\open\\command"));

            if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) 
			{
                TCHAR *pos;
                pos = _tcsstr(key, _T("\"%1\""));
                if (pos == NULL) {                     // No quotes found
                    pos = _tcsstr(key, _T("%1"));      // Check for %1, without quotes 
                    if (pos == NULL)                   // No parameter at all...
                        pos = key+lstrlen(key)-1;
                    else
                        *pos = '\0';                   // Remove the parameter
                }
                else
                    *pos = '\0';                       // Remove the parameter

                lstrcat(pos, _T(" "));
                lstrcat(pos, url);

                result = (HINSTANCE) WinExec(key,showcmd);
            }
        }
    }

    return result;
}

LONG wxUpdateCheck::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
    HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

    if (retval == ERROR_SUCCESS) 
	{
        long datasize = MAX_PATH;
        TCHAR data[MAX_PATH];
        RegQueryValue(hkey, NULL, data, &datasize);
        lstrcpy(retdata,data);
        RegCloseKey(hkey);
    }

    return retval;
}


void wxUpdateCheck::MsgUpdateAvailable(DWORD dwMSlocal, DWORD dwLSlocal, DWORD dwMSWeb, DWORD dwLSWeb, const CString& strURL)
{
	CString strMessage;
	strMessage.Format(IDS_UPDATE_AVAILABLE, HIWORD(dwMSlocal), LOWORD(dwMSlocal), HIWORD(dwMSWeb), LOWORD(dwMSWeb));

	if (AfxMessageBox(strMessage, MB_YESNO|MB_ICONINFORMATION) == IDYES)
		GotoURL(strURL, SW_SHOW);
}

void wxUpdateCheck::MsgUpdateNotAvailable(DWORD dwMSlocal, DWORD dwLSlocal)
{
	AfxMessageBox(IDS_UPDATE_NO, MB_OK|MB_ICONINFORMATION);
}

void wxUpdateCheck::MsgUpdateNoCheck(DWORD dwMSlocal, DWORD dwLSlocal)
{
	AfxMessageBox(IDS_UPDATE_NOCHECK, MB_OK|MB_ICONINFORMATION);
}
*/
