/////////////////////////////////////////////////////////////////////////////
// Name:        wxpython.h
// Purpose:     wxScriptFunctionPython, wxScriptFilePython, wxPython
// Author:      Francesco Montorsi
// Modified by: 
// Created:     2004/6/5
// RCS-ID:      
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#if defined(wxSCRIPT_USE_PYTHON) && !defined(__SCPYTHON_H__)
#define __SCPYTHON_H__


// includes
#include "wx/script.h"		// base classes

// Python includes
#include <Python.h>




// String conversions (all python functions uses always chars)
// ---------------------------------------------------------

#if wxUSE_UNICODE
	#define WX2PY(x)		(wxString(x).mb_str(wxConvUTF8).data())
#else
	#define WX2PY(x)		(wxString(x).mb_str(wxConvUTF8))
#endif

#define PY2WX(x)			(wxString(x, wxConvUTF8))




//! A Python interpreted function.
class wxScriptFunctionPython : public wxScriptFunction
{
public:

	//! Creates the object; no info about return value and
	//! arguments are required since wxPython does not store them.
	wxScriptFunctionPython(const wxString &name = wxEmptyString) { 
		Set(name, wxT(""), NULL, 0);
	}


	virtual wxScriptFunction *Clone() const {
		wxScriptFunction *newf = new wxScriptFunctionPython();
		newf->DeepCopy(this);
		return newf;
	}

	virtual bool Exec(wxScriptVar &ret, wxScriptVar *arg) const;
};




//! A wxPython file script.
class wxScriptFilePython : public wxScriptFile
{
public:
	wxScriptFilePython(const wxString &toload = wxEmptyString) { 
		m_tScriptFile = wxPYTHON_SCRIPTFILE;
		if (!toload.IsEmpty())
			Load(toload);
	}

	virtual bool Load(const wxString &filename);
};




//! The wxPython interpreter.
class wxPython : public wxScriptInterpreter
{
public:

	PyObject *m_pModule, *m_pDict, *m_pLocals, *m_pGlobals;

protected:

	//! The array of the standard functions of python.
	wxScriptFunctionArray m_arrStd;

	//! Returns the array containing ALL currently interpreted functions.
	//! This functions is different from #GetFunctionList() because
	//! it doesn't exclude functions loaded by default by UnderC.
	virtual void GetFunctionListComplete(wxScriptFunctionArray &) const;

public:
	wxPython() {}//: m_bInit(FALSE) {}
	virtual ~wxPython() { Py_Finalize(); }

	//! Returns the global instance of this class.
	static wxPython *Get() { return m_pPython; }

	//! Inits Python interpreter.
	virtual bool Init();

	//! Returns TRUE if Python is ready.
	virtual bool isReady() const;

	//! Returns the list of the functions currently recognized by the interpreter.
	virtual void GetFunctionList(wxScriptFunctionArray &) const;
};



#endif		// wxSCRIPT_USE_PYTHON


