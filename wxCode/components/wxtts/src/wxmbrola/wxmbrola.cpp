/////////////////////////////////////////////////////////////////////////////
// Name:        wxmbrola.cpp
// Purpose:     wxMbrola class, for driving Mbrola
// Author:      Julian Smart
// Modified by:
// Created:     10/6/98
// Copyright:   Julian Smart
// Licence:
/////////////////////////////////////////////////////////////////////////////

#include "tts/wxmbrola.h"

#if !defined(__WXMBROLA_MFC__)
#ifdef _WINDOWS
#include <windows.h>
#endif
#endif

#include "tts/mbrplay.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define _MAX_BUFFSIZE		1024
#define _FREQ_DEFAULT		16000
#define _FREQ_DEFAULT_MAX	32000
#define _FREQ_DEFAULT_MIN	8000

static int wxMbrolaCallback(UINT msg, WPARAM wParam, LPARAM lParam);

wxMbrola* wxMbrola::sm_objectStackFirst = NULL;

wxMbrola::wxMbrola()
{
	m_errors = 0;
	m_defaultDatabase = "";
	m_defaultPath = "";
	m_pitchMin = (float)0.2;
	m_pitchMax = 8.0;		// min & max pitch ratio
	m_durationMin = (float)0.2;
	m_durationMax = 8.0;		// min & max time ratio
	m_voiceMin = _FREQ_DEFAULT_MIN;
	m_voiceMax = _FREQ_DEFAULT_MAX;		// min & max voice frequency

#if !defined(__WXMBROLA_MFC__) && !defined(__WXMBROLA_WXWIN__)
    m_defaultDatabase = NULL;
    m_defaultPath = NULL;
#endif

	PushObject(this);
}

wxMbrola::~wxMbrola()
{
	PopObject(this);

    WXMBROLA_STRING_DELETE(m_defaultDatabase);
    WXMBROLA_STRING_DELETE(m_defaultPath);
}

// Initialize the class
Bool wxMbrola::Init(const CStringRef iniFile)
{
	m_errors = 0;

	FILE *fhdl=NULL;
	char *buff=(char *) malloc((_MAX_PATH+1)*sizeof(char));

#if !defined(__WXMBROLA_MFC__) && !defined(__WXMBROLA_WXWIN__)
    if (iniFile && (strcmp(iniFile, "") != 0))
#else
    if (iniFile != "")
#endif
        fhdl=fopen((char*) (const char*) iniFile, "rt");

	// Search the ini file in the directory where the progam started
	if (fhdl==NULL)
	{
		GetCurrentDirectory(_MAX_PATH,buff);
		strcat(buff,"\\mbroli.ini");
		fhdl=fopen(buff,"rt");
	}

	// if not found, search in the windows directory
	if (fhdl==NULL)
	{
		
		GetWindowsDirectory(buff,_MAX_PATH);
		strcat(buff,"\\mbroli.ini");
		fhdl=fopen(buff,"rt");
	}

	// if we found a .ini, then read it
	if (fhdl!=NULL)
	{
		char clab[_MAX_PATH+1];
		DWORD dwRead;

		fclose(fhdl);
		GetPrivateProfileString("GENERAL","Path","",clab,_MAX_PATH,buff);
		WXMBROLA_STRING_ASSIGN(m_defaultPath, clab);

		GetPrivateProfileString("PITCH","Min","0.2",clab,19,buff);
		m_pitchMin=(float)atof(clab);

		GetPrivateProfileString("PITCH","Max","8",clab,19,buff);
		m_pitchMax=(float)atof(clab);

		GetPrivateProfileString("TIME","Min","0.2",clab,19,buff);
		m_durationMin=(float)atof(clab);

		GetPrivateProfileString("TIME","Max","8",clab,19,buff);
		m_durationMax=(float)atof(clab);

		m_voiceMin=GetPrivateProfileInt("VOICE","Min",_FREQ_DEFAULT_MIN,buff);

		m_voiceMax=GetPrivateProfileInt("VOICE","Max",_FREQ_DEFAULT_MAX,buff);

		dwRead=GetPrivateProfileString("DATABASE","Default","",clab,_MAX_PATH,buff);
		if (dwRead>0)
		{
		    WXMBROLA_STRING_ASSIGN(m_defaultDatabase, clab);
            int base;

			if ((base=MBR_RegIdxGetDatabaseIndex(clab))<0)
				m_errors=2;
		}
		else
		{
            free(buff);
			return FALSE;
		}
	}
	else
	{
		int base;
        // Is this right? Maybe we should not return FALSE just
        // because there was no default database. How fatal is it?
		if ((base=MBR_RegIdxGetDefaultDatabase())<0)
		{
            free(buff);
			return FALSE;
		}
		else
		{
#if !defined(__WXMBROLA_MFC__) && !defined(__WXMBROLA_WXWIN__)
			Bool ans = GetDatabaseId(base, buff);
            WXMBROLA_STRING_ASSIGN(m_defaultDatabase, buff);
#else
			Bool ans = GetDatabaseId(base, m_defaultDatabase);
#endif
			if (!ans)
            {
                free(buff);
				return FALSE;
            }
		}
	}
	free(buff);
	if (m_errors>0)
	{
/*
		MessageBeep(MB_ICONHAND);
		switch(m_errors)
		{
		case 1:
			MessageBox("Error :\n   Unable to find the mbroli.ini file\n",AfxGetApp()->m_pszAppName,MB_ICONSTOP);
			break;
		case 2:
			MessageBox("Error :\n   No Default Database specified in the mbroli.ini file\n",AfxGetApp()->m_pszAppName,MB_ICONSTOP);
			break;
		case 3:
			MessageBox("Error :\n   Can't find the specified Default Database in the Registry",AfxGetApp()->m_pszAppName,MB_ICONSTOP);
			break;
		case 4:
			MessageBox("Error :\n   An Error occurred while loading databases\n",AfxGetApp()->m_pszAppName,MB_ICONSTOP);
			break;
		}
*/
		return FALSE;
	}
	return TRUE;
}

// Get the string ID given the database index
Bool wxMbrola::GetDatabaseId(int n, CStringRef dbId) const
{
	char buf[100];
	if (MBR_RegIdxGetDatabaseId(n, buf, 100) < 0)
	{
		return FALSE;
	}
	else
	{
#if !defined(__WXMBROLA_MFC__) && !defined(__WXMBROLA_WXWIN__)
        strcpy(dbId, buf);
#else
		dbId = buf;
#endif
		return TRUE;
	}
}

// Set the current database
Bool wxMbrola::SetCurrentDatabase(const CStringRef dbId)
{
	int ret;
	if((ret=MBR_SetDatabase(dbId)) < 0)
		return FALSE;
	else
		return TRUE;
}

// Gets the current database
Bool wxMbrola::GetCurrentDatabase(CStringRef dbId) const
{
	char buf[100];
	if (MBR_GetDatabase(buf, 100) < 0)
	{
		return FALSE;
	}
	else
	{
#if !defined(__WXMBROLA_MFC__) && !defined(__WXMBROLA_WXWIN__)
        strcpy(dbId, buf);
#else
		dbId = buf;
#endif
		return TRUE;
	}
}

CString wxMbrola::GetDatabaseAllInfo() const
{
#if defined(__WXMBROLA_MFC__)
	CString info;
	int len=MBR_GetDatabaseAllInfo(NULL,0)+1;
	MBR_GetDatabaseAllInfo(info.GetBuffer(len),len);
	info.ReleaseBuffer();
	return info;
#elif defined(__WXMBROLA_WXWIN__)
	wxString info;
	int len=MBR_GetDatabaseAllInfo(NULL,0)+1;
    char* buf = new char[len];
	MBR_GetDatabaseAllInfo(buf,len);
    info = buf;
	return info;
#else
	int len=MBR_GetDatabaseAllInfo(NULL,0)+1;
    char* buf = new char[len];
	MBR_GetDatabaseAllInfo(buf,len);
	return buf;
#endif
}

// Get the Mbrola version information
CString wxMbrola::GetVersion()
{
#if defined(__WXMBROLA_MFC__)
	CString ver;
	MBR_GetVersion(ver.GetBuffer(60),60);
	return ver;
#elif defined(__WXMBROLA_WXWIN__)
	wxString ver;
    char buf[60];
	MBR_GetVersion(buf,60);
    ver = buf;
	return ver;
#else
    static char ver[60];
	MBR_GetVersion(ver,60);
	return ver;
#endif
}

// Set pitch
Bool wxMbrola::SetPitch(float pitch)
{
	return (MBR_SetPitchRatio(pitch) == 0);
}

// Get pitch
float wxMbrola::GetPitch() const
{
	return MBR_GetPitchRatio() ;
}

// Set duration
Bool wxMbrola::SetDuration(float duration)
{
	return (MBR_SetDurationRatio(duration) == 0);
}

// Get duration
float wxMbrola::GetDuration() const
{
	return MBR_GetDurationRatio();
}

// Set voice freq
Bool wxMbrola::SetFreq(long freq)
{
	return (MBR_SetVoiceFreq(freq) == 0);
}

// Get voice freq
long wxMbrola::GetFreq() const
{
	return MBR_GetVoiceFreq() ;
}

// Get default voice frequency for this database
long wxMbrola::GetDefaultFreq() const
{
	return MBR_GetDefaultFreq();
}

// Is it playing?
Bool wxMbrola::IsPlaying() const
{
	return (MBR_IsPlaying() != 0);
}

// Play.
int wxMbrola::Play(const CStringRef text, Bool sync)
{
	long flags = 0;
	if (sync)
		flags |= MBR_WAIT;
	flags |= MBR_CALLBACK|MBR_MSGALL;
	return (int) MBR_Play((const char*) text, flags, NULL, (DWORD) & wxMbrolaCallback);
}

// Play from a .pho file
int wxMbrola::PlayFromFile(const CStringRef filename, Bool sync)
{
	long flags = 0;
	if (sync)
		flags |= MBR_WAIT;
	flags |= MBR_CALLBACK|MBR_MSGALL|MBR_BYFILE;
	int ret = MBR_Play((const char*) filename, flags, NULL, (DWORD) & wxMbrolaCallback) ;
	return ret;
}

// Play to a WAV file
int wxMbrola::PlayToFile(const CStringRef text, const CStringRef filename, Bool sync)
{
	long flags = 0;
	if (sync)
		flags |= MBR_WAIT;
	flags |= MBR_CALLBACK|MBR_MSGALL|MBROUT_WAVE;
	return (int) MBR_Play((const char*) text, flags, (const char*) filename, (DWORD) & wxMbrolaCallback) ;
}

// Stop
Bool wxMbrola::Stop()
{
	return (MBR_Stop() == 0) ;
}

// Wait for end
Bool wxMbrola::WaitForEnd()
{
	return (MBR_WaitForEnd() == 0) ;
}

// Get the last error
CString wxMbrola::GetLastError() const
{
#if defined(__WXMBROLA_MFC__) || defined(__WXMBROLA_WXWIN__)
	char msg[400];
	msg[0] = 0;
	MBR_LastError(msg,400);
	return CString(msg);
#else
	static char msg[400];
	msg[0] = 0;
	MBR_LastError(msg,400);
	return msg;
#endif
}

int wxMbrola::GetDatabaseCount() const
{
    return (int) MBR_RegGetDatabaseCount();
}

void wxMbrola::SetDefaultDatabase(const CStringRef dbId)
{
    WXMBROLA_STRING_ASSIGN(m_defaultDatabase, dbId);
}

void wxMbrola::SetDefaultPath(const CStringRef path)
{
    WXMBROLA_STRING_ASSIGN(m_defaultPath, path);
}

// Maintain a stack of objects so we know which is the
// currently active one, for sending events to.
void wxMbrola::PushObject(wxMbrola* obj)
{
    obj->m_next = sm_objectStackFirst;
	sm_objectStackFirst = obj;
}

void wxMbrola::PopObject(wxMbrola* obj)
{
	// Not a real pop - remove from anywhere on the list.
    if (sm_objectStackFirst == obj)
    {
        sm_objectStackFirst = sm_objectStackFirst->m_next;
    }
    else
    {
        wxMbrola* o = sm_objectStackFirst;
        while (o)
        {
            if (o->m_next == obj)
            {
                o->m_next = obj->m_next;
                return;
            }
            o = o->m_next;
        }
    }
}

wxMbrola* wxMbrola::GetCurrentObject()
{
    return sm_objectStackFirst;
}

char *wxMbrola::copystring (const char *s)
{
  if (s == NULL) s = "";
  size_t len = strlen (s) + 1;

  char *news = new char[len];
  memcpy (news, s, len);	// Should be the fastest

  return news;
}

// Overridables
void wxMbrola::OnStart()
{
}

void wxMbrola::OnStop()
{
}

void wxMbrola::OnRead()
{
}

void wxMbrola::OnWrite()
{
}

void wxMbrola::OnWait()
{
}

static int wxMbrolaCallback(UINT msg, WPARAM wParam, LPARAM lParam)
{
	wxMbrola* mbrola = wxMbrola::GetCurrentObject();
	if (!mbrola)
		return 0;

	switch (msg)
	{
	case WM_MBR_INIT:
		{
			mbrola->OnStart();
			break;
		}
	case WM_MBR_READ:
		{
			mbrola->OnRead();
			break;
		}
	case WM_MBR_WAIT:
		{
			mbrola->OnWait();
			break;
		}
	case WM_MBR_WRITE:
		{
			mbrola->OnWrite();
			break;
		}
	case WM_MBR_END:
		{
			mbrola->OnStop();
			break;
		}
	default:
		{
			break;
		}
	}
	return 0;
}

