//
// Name     : mmMAPIMail
// Purpose  : Send and receive email with MAPI.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#ifndef _MMHYPERTEXT_INCLUDED
#define _MMHYPERTEXT_INCLUDED

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include <windows.h>
#include <mapi.h>
#include <wx/string.h>

// Error codes
enum MAPI_ERROR {
    MAPI_NO_DLL = 0,
    MAPI_NO_LOGON,
    MAPI_NO_LOGOFF,
    MAPI_NO_RECIP,
    MAPI_NO_SEND,
    MAPI_NO_SPACE,
    MAPI_NO_MAIL,
    MAPI_NO_READ,
    MAPI_SUCCESS
}; // enum MAPI_MAIL_ERROR

class mmMAPIMail
{
    public:
        mmMAPIMail();
        ~mmMAPIMail();

        bool Send(wxString from, wxString to, wxString subject, wxString text);
        bool Read(wxString *from, wxString *text);
        MAPI_ERROR GetError() {
            return mError;
        }

    private:
        HINSTANCE  mMAPIDLL; // Address of MAPI DLL
        LHANDLE    mSession; // Session handle
        MAPI_ERROR mError;   // Error code
}; // class mmMAPIMail

#endif

