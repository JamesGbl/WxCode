/////////////////////////////////////////////////////////////////////////////
// Name:        sccint.h
// Purpose:     wxScriptFunctionCINT, wxScriptFileCINT, wxCINT
// Author:      Francesco Montorsi
// Modified by: 
// Created:     2004/29/4
// RCS-ID:      
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#if defined(wxSCRIPT_USE_CINT) && !defined(__SCCINT_H__)
#define __SCCINT_H__


// includes
#include <G__ci.h>			// CINT required
#include <Api.h>
#include "wx/script.h"		// base classes


//! An interpreted function.
class wxScriptFunctionCINT : public wxScriptFunction
{
public:

	//! Creates & set the properties of the object.
	wxScriptFunctionCINT(const wxString &name = wxEmptyString, 
						 const wxString &ret = wxEmptyString,
						 wxScriptTypeInfo *arr = NULL, int n = 0) { Set(name, ret, arr, n);	}
	virtual ~wxScriptFunctionCINT() {}

	virtual bool Exec(wxScriptVar &ret, wxScriptVar *arg) const;
	
	wxSCRIPTFNC_IMPLEMENT_CLONE(wxScriptFunctionCINT)
};


//! A CINT file script.
class wxScriptFileCINT : public wxScriptFile
{
public:
	wxScriptFileCINT() { m_tScriptFile = wxCINT_SCRIPTFILE; }
	virtual ~wxScriptFileCINT() {}

	virtual bool Load(const wxString &filename);
};


//! The CINT interpreter.
class wxCINT : public wxScriptInterpreter
{
protected:

	//! TRUE if the script interpreter is ready to work.
	bool m_bInit;

public:
	wxCINT() : m_bInit(FALSE) {}
	virtual ~wxCINT() { G__scratch_all(); }

	//! Returns the global instance of this class.
	static wxCINT *Get() { return m_pCINT; }

	//! Inits CINT.
	virtual bool Init();

	//! Returns TRUE if CINT is ready.
	virtual bool isReady() const { return m_bInit; }

	//! Returns the list of the functions currently recognized by the interpreter.
	virtual void GetFunctionList(wxScriptFunctionArray &) const;
};


#endif		// wxSCRIPT_USE_CINT


