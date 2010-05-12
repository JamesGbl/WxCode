//! \file mmMAPIMail.cpp
//
// Name     : mmMAPIMail
// Purpose  : Send and receive email with MAPI.
// Author   : Arne Morken
// Copyright: (C) 1997-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//
// WINDOWS ONLY: GetProcAddress() requires Windows2000 or higher.
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#include <wx/mmwxdef.h>
#include "wx/mmMAPIMail.h"

/*! \brief Constructor.
 */
mmMAPIMail::mmMAPIMail()
{
    LHANDLE *session;
    mMAPIDLL = LoadLibrary(wxT("MAPI32.DLL"));
    if(!mMAPIDLL)
        mError = MAPI_NO_DLL; // MAPI DLL not found
    else
    {
        // Get procedure address
        typedef WORD(CALLBACK * MAPILOGON)(ULONG, LPSTR, LPSTR, ULONG, ULONG, LHANDLE**);
        MAPILOGON mapi_logon = (MAPILOGON)GetProcAddress(mMAPIDLL, wxString(wxT("MAPILogon")).mb_str());
        // Logon MAPI
        if((*mapi_logon)(0L, NULL, NULL, MAPI_LOGON_UI, 0L, &session) != SUCCESS_SUCCESS)
            mError = MAPI_NO_LOGON; // Couldn't log on
        else
            mError = MAPI_SUCCESS; // Logged on
    }
    mSession = (LHANDLE)session;
} // mmMAPIMail constructor

/*! \brief Destructor.
 */
mmMAPIMail::~mmMAPIMail()
{
    // Get procedure address
    typedef WORD(CALLBACK * MAPILOGOFF)(LHANDLE, ULONG, ULONG, ULONG);
    MAPILOGOFF mapi_logoff = (MAPILOGOFF)GetProcAddress(mMAPIDLL, wxString(wxT("MAPILogoff")).mb_str());
    // Logoff MAPI
    if((*mapi_logoff)(mSession, 0, 0, 0) != SUCCESS_SUCCESS)
        mError = MAPI_NO_LOGOFF;
    else
        mError = MAPI_SUCCESS;
} // mmMAPIMail destructor

/*! \brief Send a message.
 *
 * \param from 		wxString	The from field text.
 * \param to 			wxString	The to field text.
 * \param subject	wxString	The subject text
 * \param text 		wxString	The message text.
 * \return bool		True on success, otherwise false.
 *
 */
bool mmMAPIMail::Send(wxString from, wxString to, wxString subject, wxString text)
{
    if(to == wxEmptyString)
    {
        mError = MAPI_NO_RECIP;
        return FALSE;
    }

    MapiMessage   message;
    MapiRecipDesc recip[1], orig[1];
    memset((void *)&message.ulReserved,  '\0', sizeof(MapiMessage));
    memset((void *)&recip[0].ulReserved, '\0', sizeof(MapiRecipDesc));
    memset((void *)&orig[0].ulReserved,  '\0', sizeof(MapiRecipDesc));

    recip[0].ulRecipClass = MAPI_TO;
    recip[0].lpszName     = new char[256];
    strcpy(recip[0].lpszName, to.mb_str());

    orig[0].ulRecipClass  = MAPI_ORIG;
    orig[0].lpszName      = new char[256];
    strcpy(orig[0].lpszName, from.mb_str());

    message.lpRecips      = &recip[0];
    message.lpOriginator  = &orig[0];
    message.nRecipCount   = 1;
    message.lpszSubject   = new char[256];
    message.lpszNoteText  = new char[text.Length()+1];
    strcpy(message.lpszSubject,  subject.mb_str());
    strcpy(message.lpszNoteText, text.mb_str());

    typedef WORD(CALLBACK * MAPISENDMAIL)(LHANDLE, ULONG, lpMapiMessage, FLAGS, ULONG);
    MAPISENDMAIL mapi_send_mail = (MAPISENDMAIL)GetProcAddress(mMAPIDLL, wxString(wxT("MAPISendMail")).mb_str());
    if((*mapi_send_mail)(mSession, 0L, &message, 0, 0L) != SUCCESS_SUCCESS)
        mError = MAPI_NO_SEND;
    else
        mError = MAPI_SUCCESS;
    delete recip[0].lpszName;
    delete orig[0].lpszName;
    delete message.lpszSubject;
    delete message.lpszNoteText;
    return (mError == MAPI_SUCCESS);
} // mmMAPIMail::Send

/*! \brief Read a message.
 *
 * \param from 	wxString*	The from field text.
 * \param text 	wxString*	The message text.
 * \return bool	True on success, otherwise false.
 *
 */
bool mmMAPIMail::Read(wxString *from, wxString *text)
{
    if(!from || !text)
    {
        mError = MAPI_NO_SPACE;
        return FALSE;
    }

    char msg_id[513];

    // Get procedure address
    typedef WORD(CALLBACK * MAPIFINDNEXT)(LHANDLE, ULONG, LPSTR, LPSTR, FLAGS, ULONG, LPSTR);
    MAPIFINDNEXT mapi_find_next = (MAPIFINDNEXT)GetProcAddress(mMAPIDLL, wxString(wxT("MAPIFindNext")).mb_str());
    // See if there are unread messages
    if((*mapi_find_next)(mSession, 0L, NULL, NULL, MAPI_LONG_MSGID | MAPI_UNREAD_ONLY, 0L, &msg_id[0]) != SUCCESS_SUCCESS)
    {
        mError = MAPI_NO_MAIL;
        return FALSE;
    }

    // Get procedure address
    typedef WORD(CALLBACK * MAPIREADMAIL)(LHANDLE, ULONG, LPSTR, ULONG, ULONG, lpMapiMessage*);
    MAPIREADMAIL mapi_read_mail = (MAPIREADMAIL)GetProcAddress(mMAPIDLL, wxString(wxT("MAPIReadMail")).mb_str());
    // Read the message
    lpMapiMessage message;
    if((*mapi_read_mail)(mSession, 0L, msg_id, 0L, 0L, &message) != SUCCESS_SUCCESS)
    {
        mError = MAPI_NO_READ;
        return FALSE;
    }
    *text = wxString(message->lpszNoteText, wxConvUTF8);
    *from = wxString(message->lpOriginator->lpszName, wxConvUTF8);
    mError = MAPI_SUCCESS;
    return TRUE;
}

