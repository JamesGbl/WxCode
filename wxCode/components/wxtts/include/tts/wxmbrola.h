/////////////////////////////////////////////////////////////////////////////
// Name:        wxmbrola.h
// Purpose:     wxMbrola class, for driving Mbrola
// Author:      Julian Smart
// Modified by:
// Created:     10/6/98
// Copyright:   Julian Smart
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXMBROLA_H_
#define _WXMBROLA_H_

#ifdef _AFXDLL
#  define __WXMBROLA_MFC__
#else
// Set your framework (here or in the makefile). If all are undefined, we're
// using native Windows or Unix.
// Porting notes: the only thing for which you really need to include windows.h
// in the header is the regfn function. Presumably this will be different on Unix anyway
// and would be #ifdefed out.

// #define __WXMBROLA_MFC__
// #define __WXMBROLA_WXWIN__
#endif

#if defined(__WXMSW__) || defined(__WXGTK__) || defined(__WXMOTIF__)
#ifndef __WXMBROLA_WXWIN__
#define __WXMBROLA_WXWIN__
#endif
#endif

#ifdef __WXMBROLA_MFC__
#include <afx.h>
#elif defined(__WXMBROLA_WXWIN__)
#include <wx/wx.h>
#else
#include <windows.h>
#endif

#ifdef __WXMBROLA_MFC__
#   ifndef CStringRef
#       define CStringRef CString&
#   endif
#   ifndef Bool
#       define Bool bool
#   endif
#elif defined(__WXMBROLA_WXWIN__)
#   ifndef CString
#       define CString wxString
#       define CStringRef wxString&
#   endif
#   ifndef Bool
#       define Bool bool
#   endif
#else
#   ifndef CString
#       define CString char*
#       define CStringRef char*
#   endif
#   ifndef Bool
#       define Bool bool
#   endif
#endif

#if !defined(__WXMBROLA_MFC__) && !defined(__WXMBROLA_WXWIN__)
#define WXMBROLA_STRING_ASSIGN(a, b) delete[] a; a = copystring(b)
#define WXMBROLA_STRING_DELETE(a) delete[] a
#else
#define WXMBROLA_STRING_ASSIGN(a, b) a = b
#define WXMBROLA_STRING_DELETE(a)
#endif

#define wxMBRERR_NOMBROLADLL			-11
#define wxMBRERR_NOREGISTRY			    -10
#define wxMBRERR_NORESOURCE			    -9
#define wxMBRERR_NOTHREAD				-8
#define wxMBRERR_DATABASENOTVALID		-7
#define wxMBRERR_CANTOPENDEVICEOUT	    -6
#define wxMBRERR_BADCOMMAND			    -5
#define wxMBRERR_CANTOPENFILE			-4
#define wxMBRERR_WRITEERROR			    -3
#define wxMBRERR_MBROLAERROR			-2
#define wxMBRERR_CANCELLEDBYUSER		-1
#define wxMBRERR_NOERROR				0

/////////////////////////////////////////////////////////////////////////////
// wxMbrola dialog

class wxMbrola
{
// Construction
public:
	wxMbrola();
	virtual ~wxMbrola();

  public:
	// Initialize the class, with optional iniFile
	Bool Init(const CStringRef iniFile = "");


// Accessors
	inline CString GetDefaultDatabase() const { return m_defaultDatabase; }
	void SetDefaultDatabase(const CStringRef dbId);

	inline CString GetDefaultPath() const { return m_defaultPath; }
	void SetDefaultPath(const CStringRef path);

	inline void SetMinPitch(float val) { m_pitchMin = val ;}
	inline float GetMinPitch() const { return m_pitchMin; }
	inline void SetMaxPitch(float val) { m_pitchMax = val ;}
	inline float GetMaxPitch() const { return m_pitchMax; }

	inline void SetMinDuration(float val) { m_durationMin = val ;}
	inline float GetMinDuration() const { return m_durationMin; }
	inline void SetMaxDuration(float val) { m_durationMax = val ;}
	inline float GetMaxDuration() const { return m_durationMax; }

	inline void SetMinFreq(long val) { m_voiceMin = val ;}
	inline long GetMinFreq() const { return m_voiceMin; }
	inline void SetMaxFreq(long val) { m_voiceMax = val ;}
	inline long GetMaxFreq() const { return m_voiceMax; }

// Mbrola DLL functions

	int GetDatabaseCount() const ;

	// Get the string ID given the database index
	Bool GetDatabaseId(int n, CStringRef id) const;

	// Set the current database
	Bool SetCurrentDatabase(const CStringRef dbId);

	// Gets the current database name
	Bool GetCurrentDatabase(CStringRef dbId) const;

	// Get all information about the current database
	CString GetDatabaseAllInfo() const;

	// Get the Mbrola version information
	static CString GetVersion() ;

	// Set pitch
	Bool SetPitch(float pitch);

	// Get pitch
	float GetPitch() const;

	// Set duration
	Bool SetDuration(float duration);

	// Get duration
	float GetDuration() const;

	// Set voice freq
	Bool SetFreq(long freq);

	// Get voice freq
	long GetFreq() const;

	// Get default voice frequency for this database
	long GetDefaultFreq() const;

	// Is it playing?
	Bool IsPlaying() const ;

	// Play. Returns one of these values:
    /*
    wxMBRERR_NOMBROLADLL
    wxMBRERR_NOREGISTRY
    wxMBRERR_NORESOURCE
    wxMBRERR_NOTHREAD
    wxMBRERR_DATABASENOTVALID
    wxMBRERR_CANTOPENDEVICEOUT
    wxMBRERR_BADCOMMAND
    wxMBRERR_CANTOPENFILE
    wxMBRERR_WRITEERROR
    wxMBRERR_MBROLAERROR
    wxMBRERR_CANCELLEDBYUSER
    wxMBRERR_NOERROR
    */

	int Play(const CStringRef text, Bool sync);

	// Play to a WAV file
	int PlayToFile(const CStringRef text, const CStringRef filename, Bool sync);

	// Play from a file
	int PlayFromFile(const CStringRef filename, Bool sync);

	// Stop
	Bool Stop();

	// Wait for end
	Bool WaitForEnd();

	// Get the last error
	CString GetLastError() const;

// Overridables
	virtual void OnStart();
	virtual void OnStop();
	virtual void OnRead();
	virtual void OnWrite();
	virtual void OnWait();

// Implementation

	// Maintain a stack of objects so we know which is the
	// currently active one, for sending events to.
	static void PushObject(wxMbrola* obj);
	static void PopObject(wxMbrola* obj);
	static wxMbrola* GetCurrentObject();
    static char *copystring (const char *s);

// Data members
protected:
	int			m_isSpeakRunning;
	short		m_errors;
	CString		m_defaultDatabase;
	CString		m_defaultPath;
	float		m_pitchMin,m_pitchMax;		// min & max pitch ratio (for the spinner)
	float		m_durationMin,m_durationMax;		// min & max time (duration) ratio (for the spinner)
	long		m_voiceMin,m_voiceMax;		// min & max voice frequency (for the spinner)

	static wxMbrola* sm_objectStackFirst;   // Stack of wxMbrola objects, so we can
									        // know which is the currently active one
    wxMbrola*   m_next;                     // Next in stack
};

#endif

