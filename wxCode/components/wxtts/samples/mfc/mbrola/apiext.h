/*
 * FPMs-TCTS SOFTWARE LIBRARY
 *
 * File :    apiext.cpp
 * Purpose : Windows API Extended Functions
 * Author  : Alain Ruelle
 * Email   : ruelle@tcts.fpms.ac.be
 *
 * Copyright (c) 1997 Faculte Polytechnique de Mons (TCTS lab)
 * All rights reserved.
 * PERMISSION IS HEREBY DENIED TO USE, COPY, MODIFY, OR DISTRIBUTE THIS
 * SOFTWARE OR ITS DOCUMENTATION FOR ANY PURPOSE WITHOUT WRITTEN
 * AGREEMENT OR ROYALTY FEES.
 */

#ifndef __APIEXT_H__
#define __APIEXT_H__

#include <afx.h>

// Windows NT only
#define SHELLFOLDER_COMMON_DESKTOP		0
#define SHELLFOLDER_COMMON_PROGRAMS		1
#define SHELLFOLDER_COMMON_STARTMENU	2
#define SHELLFOLDER_COMMON_STARTUP		3
// Windows 95 & NT
#define SHELLFOLDER_DEFAULT_DESKTOP		16
#define SHELLFOLDER_DEFAULT_PROGRAMS	17
#define SHELLFOLDER_DEFAULT_RECENT		20
#define SHELLFOLDER_DEFAULT_SENDTO		21

#define SHELLFOLDER_USER_DESKTOP		32
#define SHELLFOLDER_USER_PROGRAMS		33
#define SHELLFOLDER_USER_STARTMENU		34
#define SHELLFOLDER_USER_STARTUP		35
#define SHELLFOLDER_USER_FAVORITES		36
#define SHELLFOLDER_USER_RECENT			37
#define SHELLFOLDER_USER_SENDTO			38
#define SHELLFOLDER_USER_TEMPLATES		39

typedef BYTE SHELLFOLDER;

#define HKEY_SHELLFOLDER	"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"

typedef struct _REGISTERCOMMAND {
	LPTSTR	lpszName;
	LPTSTR	lpszCommand;
	BOOL	useDDE;
	LPTSTR	lpszDDEMessage;
	LPTSTR	lpszDDEApplication;
	LPTSTR	lpszDDETopic;
} REGISTERCOMMAND;

typedef REGISTERCOMMAND* LPREGISTERCOMMAND;

#define UNREG_EXTENSION	FALSE
#define	UNREG_COMMAND	TRUE

// Get a single value in a key of the Registry
long RegGetValue(HKEY hKey,LPCTSTR lpSubKey,LPCTSTR lpValueName,LPTSTR lpValueBuf,LPDWORD ldwTotsize,DWORD dwValueType);

// Set a single value in a key of the Registry
long RegSetValueEx2(HKEY hKey,LPCTSTR lpSubKey,LPCTSTR lpValueName,DWORD dwType,CONST BYTE* lpData,DWORD cbData);

// Delete a key and it's descendant (Windows NT)
long RegDeleteKeyEx(HKEY hKey, LPCTSTR lpSubKey);

// Return the index of a subkey
long RegGetKeyIndex(HKEY hKey,LPCTSTR lpszSubKey, LPCTSTR lpszIdxKey, LPDWORD dwIdx);

// Get the Common Shell Folders directory
long GetShellFoldersDirectory(SHELLFOLDER sfType,LPTSTR lpPathBuf,DWORD dwSize);


long RegisterExtension(LPCTSTR lpszExt,
					   LPCTSTR lpszCmdName,
					   LPCTSTR lpszDescr,
					   LPCTSTR lpszIcon,
					   LPREGISTERCOMMAND lpCmdDescr,
					   DWORD dwCommand);

long UnRegisterExtension(LPCTSTR lpszExt,BOOL bCommand);

LPTSTR GetTempFile(LPCTSTR ext=NULL, LPCTSTR path=NULL);

void PumpMessage();

//void Pause(long nms);

#endif
