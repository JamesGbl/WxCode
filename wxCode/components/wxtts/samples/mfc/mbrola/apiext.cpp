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
 *
 * 09/10/97 : Created. Put here input output function depending on the
 *            little endian or big endian format
 *            Also audio_file header writing and file format extraction
 */

#include "stdafx.h"
#include "ApiExt.h"

// Get a single value in a key of the Registry
long RegGetValue(HKEY hKey,LPCTSTR lpSubKey,LPCTSTR lpValueName,LPTSTR lpValueBuf,LPDWORD ldwTotsize,DWORD dwValueType)
{
	long ret=ERROR_SUCCESS;
	HKEY nextKey;

	if (lpSubKey)
		ret=RegOpenKey(hKey,lpSubKey,&nextKey);
	else
		nextKey=hKey;
	if (ret==ERROR_SUCCESS)
	{
		if (lpValueName)
		{
			VALENT myValue;
			myValue.ve_valuename=new char[strlen(lpValueName)+1];
			strcpy(myValue.ve_valuename,lpValueName);
			myValue.ve_type=dwValueType;
			myValue.ve_valuelen=0;
			myValue.ve_valueptr=(DWORD) lpValueBuf;
			ret=RegQueryMultipleValues(nextKey,&myValue,1,lpValueBuf,ldwTotsize);
			if ((ret==ERROR_MORE_DATA) || (ret==ERROR_NO_MORE_ITEMS))
				ret=ERROR_SUCCESS;
			delete myValue.ve_valuename;
		}
		else
		{
			long mylong=*ldwTotsize;
			ret=RegQueryValue(nextKey,NULL,lpValueBuf,&mylong);
			*ldwTotsize=mylong;
		}
	}
	RegCloseKey(nextKey);
	return ret;
}

long RegSetValueEx2(HKEY hKey,LPCTSTR lpSubKey,LPCTSTR lpValueName,DWORD dwType,CONST BYTE* lpData,DWORD cbData)
{
	long ret=ERROR_SUCCESS;
	HKEY nextKey;

	if (lpSubKey)
		ret=RegOpenKey(hKey,lpSubKey,&nextKey);
	else
		nextKey=hKey;
	if (ret==ERROR_SUCCESS)
	{
		RegSetValueEx(nextKey,lpValueName,0,dwType,lpData,cbData);
	}
	if (lpSubKey)
		RegCloseKey(nextKey);
	return ret;
}

// Delete a key and all of it's subkeys (Windows NT)
long RegDeleteKeyEx(HKEY hKey, LPCTSTR lpSubKey)
{
	long ret=ERROR_SUCCESS;
	HKEY subKey;

	if ((ret=RegOpenKey(hKey,lpSubKey,&subKey))==ERROR_SUCCESS)
	{
		char key[_MAX_PATH+1];

		ret=RegEnumKey(subKey,0,key,_MAX_PATH);
		while ((ret==ERROR_MORE_DATA) || (ret==ERROR_SUCCESS))
		{
			RegDeleteKeyEx(subKey,key);
			RegDeleteKey(subKey,key);
			ret=RegEnumKey(subKey,0,key,_MAX_PATH);
		}
		if (ret==ERROR_NO_MORE_ITEMS)
			ret=ERROR_SUCCESS;
		RegCloseKey(subKey);
		RegDeleteKey(hKey,lpSubKey);
	}
	return ret;
}

// Return the index of a subkey
long RegGetKeyIndex(HKEY hKey,LPCTSTR lpszSubKey, LPCTSTR lpszIdxKey, LPDWORD dwIdx)
{
	HKEY newKey;
	long ret=ERROR_SUCCESS;
		
	if (!lpszSubKey)
		newKey=hKey;
	else
		ret=RegOpenKey(hKey,lpszSubKey,&newKey);

	if (lpszIdxKey && dwIdx)
	{
		char buff[_MAX_PATH+1];
		char *idxKey=strdup(lpszIdxKey);
		bool found=false;
		
		*dwIdx=0;
		strupr(idxKey);
		ret=RegEnumKey(newKey,*dwIdx,buff,_MAX_PATH);
		while (((ret==ERROR_MORE_DATA) || (ret==ERROR_SUCCESS)) && (!found))
		{
			found=(strcmp(idxKey,strupr(buff))==0);
			(*dwIdx)++;
			ret=RegEnumKey(newKey,*dwIdx,buff,_MAX_PATH);
		}
		if (!found)
		{
			*dwIdx=0;
			ret=ERROR_FILE_NOT_FOUND;
		}
		else
		{
			(*dwIdx)--;
			ret=ERROR_SUCCESS;
		}
		free(idxKey);
	}
	else
		ret=ERROR_FILE_NOT_FOUND;
	if (lpszSubKey)
		RegCloseKey(newKey);
	
	return ret;
}

// Get the Common Shell Folders directory
long GetShellFoldersDirectory(SHELLFOLDER sfType,LPTSTR lpPathBuf,DWORD dwSize)
{
	long ret=ERROR_SUCCESS;
	OSVERSIONINFO myInfo;
	HKEY startKey;
	
	myInfo.dwOSVersionInfoSize=sizeof(myInfo);
	GetVersionEx(&myInfo);
	if ((myInfo.dwPlatformId==VER_PLATFORM_WIN32_NT) && (sfType>=SHELLFOLDER_COMMON_DESKTOP) && (sfType<=SHELLFOLDER_COMMON_STARTUP))
		sfType+=32;

	if ((sfType>=SHELLFOLDER_COMMON_DESKTOP) && (sfType<=SHELLFOLDER_COMMON_STARTUP))
		startKey=HKEY_LOCAL_MACHINE;
	else if ((sfType>=SHELLFOLDER_DEFAULT_DESKTOP) && (sfType<=SHELLFOLDER_DEFAULT_SENDTO))
		ret=RegOpenKey(HKEY_USERS,".DEFAULT",&startKey);
	else
		startKey=HKEY_CURRENT_USER;

	if (ret==ERROR_SUCCESS)
	{
		char keyName[20];

		switch(sfType)
		{
		case SHELLFOLDER_COMMON_DESKTOP	:
			strcpy(keyName,"Common Desktop");
			break;
		case SHELLFOLDER_COMMON_PROGRAMS :
			strcpy(keyName,"Common Programs");
			break;
		case SHELLFOLDER_COMMON_STARTMENU :
			strcpy(keyName,"Common Start Menu");
			break;
		case SHELLFOLDER_COMMON_STARTUP	:
			strcpy(keyName,"Common Startup");
			break;
		case SHELLFOLDER_DEFAULT_DESKTOP :
		case SHELLFOLDER_USER_DESKTOP :
			strcpy(keyName,"Desktop");
			break;
		case SHELLFOLDER_DEFAULT_PROGRAMS :
		case SHELLFOLDER_USER_PROGRAMS :
			strcpy(keyName,"Programs");
			break;
		case SHELLFOLDER_DEFAULT_RECENT	:
		case SHELLFOLDER_USER_RECENT :
			strcpy(keyName,"Recent");
			break;
		case SHELLFOLDER_DEFAULT_SENDTO	:
		case SHELLFOLDER_USER_SENDTO :
			strcpy(keyName,"SendTo");
			break;
		case SHELLFOLDER_USER_STARTUP :
			strcpy(keyName,"Startup");
			break;
		case SHELLFOLDER_USER_FAVORITES :
			strcpy(keyName,"Favorites");
			break;
		case SHELLFOLDER_USER_TEMPLATES :
			strcpy(keyName,"Templates");
			break;
		case SHELLFOLDER_USER_STARTMENU	: 
		default:
			strcpy(keyName,"Start Menu");
			break;
		}
		ret=RegGetValue(startKey,HKEY_SHELLFOLDER,keyName,lpPathBuf,&dwSize,REG_SZ);
	}
	if (ret!=ERROR_SUCCESS)
		*lpPathBuf=0;

	RegCloseKey(startKey);
	return ret;
}

// Register a program as a player for a given extension
long RegisterExtension(LPCTSTR lpszExt,
					   LPCTSTR lpszCmdName,
					   LPCTSTR lpszDescr,
					   LPCTSTR lpszIcon,
					   LPREGISTERCOMMAND lpCmdDescr, DWORD dwCommand)
{
	long ret=ERROR_SUCCESS;

	if (lpszExt && lpCmdDescr)
	{
		char *ext=new char[strlen(lpszExt)+2];
		char *cmd;

		HKEY newKey;
	
		if (!lpszCmdName)
		{
			cmd=new char[strlen(lpszExt)+5];
			strcpy(cmd,lpszExt);
			strcat(cmd,"File");
		}
		else
		{
			cmd=new char[strlen(lpszCmdName)+1];
			strcpy(cmd,lpszCmdName);
		}

		strcpy(ext,".");
		strcat(ext,lpszExt);

		if ((ret=RegCreateKey(HKEY_CLASSES_ROOT,ext,&newKey))==ERROR_SUCCESS)
		{
			if ((ret=RegSetValue(HKEY_CLASSES_ROOT,ext,REG_SZ,cmd,strlen(cmd)+1))==ERROR_SUCCESS)
			{
				RegCloseKey(newKey);
				if ((ret=RegCreateKey(HKEY_CLASSES_ROOT,cmd,&newKey))==ERROR_SUCCESS)
				{
					if (lpszDescr)
					{
						ret=RegSetValue(HKEY_CLASSES_ROOT,cmd,REG_SZ,lpszDescr,strlen(lpszDescr)+1);
					}
					if ((ret==ERROR_SUCCESS) && lpszIcon)
					{
						ret=RegSetValue(newKey,"DefaultIcon",REG_SZ,lpszIcon,strlen(lpszIcon)+1);
					}
					if (ret==ERROR_SUCCESS)
					{
						HKEY shellKey;
						if ((ret=RegCreateKey(newKey,"Shell",&shellKey))==ERROR_SUCCESS)
						{
							for(DWORD i=0; (i<dwCommand) && (ret==ERROR_SUCCESS) ; i++)
							{
								HKEY tempKey;
								if ((ret=RegCreateKey(shellKey,lpCmdDescr[i].lpszName,&tempKey))==ERROR_SUCCESS)
								{
									ret=RegSetValue(tempKey,"command",REG_SZ,lpCmdDescr[i].lpszCommand,strlen(lpCmdDescr[i].lpszCommand)+1);
								}
								if ((lpCmdDescr[i].useDDE) && (ret==ERROR_SUCCESS))
								{
									HKEY ddeKey;
									if ((ret=RegCreateKey(tempKey,"ddeexec",&ddeKey))==ERROR_SUCCESS)
									{
										if ((ret=RegSetValue(tempKey,"ddeexec",REG_SZ,lpCmdDescr[i].lpszDDEMessage,strlen(lpCmdDescr[i].lpszDDEMessage)+1))==ERROR_SUCCESS)
										{
											if ((ret=RegSetValue(ddeKey,"Application",REG_SZ,lpCmdDescr[i].lpszDDEApplication,strlen(lpCmdDescr[i].lpszDDEApplication)+1))==ERROR_SUCCESS)
											{
												ret=RegSetValue(ddeKey,"Topic",REG_SZ,lpCmdDescr[i].lpszDDETopic,strlen(lpCmdDescr[i].lpszDDETopic)+1);
											}
										}
									}
									RegCloseKey(ddeKey);
								}
								RegCloseKey(tempKey);
							}
						}
						RegCloseKey(shellKey);
					}
				}
			}
		}
		RegCloseKey(newKey);
		delete ext;
		delete cmd;
	}
	return ret;
}

// Unregister a program extension
long UnRegisterExtension(LPCTSTR lpszExt,BOOL bCommand)
{
	long ret=ERROR_SUCCESS;

	if (lpszExt)
	{
		HKEY extKey;
		char *ext=new char[strlen(lpszExt)+2];
		strcpy(ext,".");
		strcat(ext,lpszExt);

		if ((ret=RegOpenKey(HKEY_CLASSES_ROOT,ext,&extKey))==ERROR_SUCCESS)
		{
			char cmdName[_MAX_PATH+1];
			DWORD len=_MAX_PATH+1;
			if ((ret=RegGetValue(extKey,NULL,NULL,cmdName,&len,REG_SZ))==ERROR_SUCCESS)
			{
				if ((ret=RegDeleteKeyEx(HKEY_CLASSES_ROOT,ext))==ERROR_SUCCESS)
				{
					if (bCommand==UNREG_COMMAND)
					{
						ret=RegDeleteKeyEx(HKEY_CLASSES_ROOT,cmdName);
					}
				}
			}
		}
		delete ext;
	}
	return ret;
}

// Get a temp file name.  We can pass it an extension for the file.
// If no extension is passed, the default ".tmp" is added
char* GetTempFile(const char* ext, const char* path)
{
	static long tmold;
	char *tmppath=new char[_MAX_PATH+13];
	char *tmpfile=new char[32];
	long tm;
	int l;

	if (!path)
		GetTempPath(_MAX_PATH,tmppath);
	else
		strcpy(tmppath,path);
	l=strlen(tmppath);
	time(&tm);
	if (tm==tmold)
		tm=tm+1;
	
	ltoa(tm,tmpfile,10);
	memcpy(tmppath+l,tmpfile+strlen(tmpfile)-8,8);
	*(tmppath+l+8)='\0';
	if (ext==NULL)
	{
		strcat(tmppath,".tmp");
	}
	else
	{
		strcat(tmppath,".");
		strcat(tmppath,ext);
	}
	delete tmpfile;
	tmold=tm;

	return tmppath;
}

// PumpMessage function
// Distribute windows messages, while there is messages
void PumpMessage()
{
	MSG message;
		
	while (PeekMessage(&message,NULL,0,0,PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}