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
#ifdef HAVE_WCHAR_H
#undef HAVE_WCHAR_H		// Python.h will redefine this when we are in Unicode mode
#endif

#include <Python.h>



// String conversions (all python functions uses always chars)
// ---------------------------------------------------------

#if wxUSE_UNICODE
	#define WX2PY(x)		(wxString(x).mb_str(wxConvUTF8).data())
#else
	#define WX2PY(x)		(wxString(x).mb_str(wxConvUTF8))
#endif

#define PY2WX(x)			(wxString(x, wxConvUTF8))


#define PYSTRING2WX(x)		(PY2WX(PyString_AsString(x)))
#define WX2PYSTRING(x)		(PyString_FromString(WX2PY(x))




//! A Python interpreted function.
class wxScriptFunctionPython : public wxScriptFunction
{
protected:

	//! The python dictionary where this function is contained.
	PyObject *m_pDict;

	//! The Python function.
	PyObject *m_pFunc;

protected:

	//! Converts the given wxScriptVar into a PyObject.
	//! Returns a "borrowed reference" (see python docs).
	PyObject *CreatePyObjFromScriptVar(const wxScriptVar &toconvert) const;

	//! Converts the given PyObject into a wxScriptVar.
	wxScriptVar CreateScriptVarFromPyObj(PyObject *toconvert) const;

	//! Decreases the refcount of the python objects currently holded.
	void ReleaseOldObj();

public:

	//! Creates the object; no info about return value and
	//! arguments are required since wxPython does not store them.
	wxScriptFunctionPython(const wxString &name = wxEmptyString,
						PyObject *dictionary = NULL, PyObject *func = NULL);
	virtual ~wxScriptFunctionPython();


	virtual wxScriptFunction *Clone() const {
		wxScriptFunction *newf = new wxScriptFunctionPython();
		newf->DeepCopy(this);
		return newf;
	}

	virtual void DeepCopy(const wxScriptFunction *tocopy);
	virtual void Set(const wxString &name, PyObject *dictionary, PyObject *function);
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

	//! The current python module.
	PyObject *m_pModule;

	//! The current dictionaries being used.
	PyObject *m_pLocals, *m_pGlobals;

	//! Recognizes the given type of python exception and sets
	//! the wxScriptInterpreter::m_strLastErr variable accordingly.
	void OnException();

public:
	wxPython() {}//: m_bInit(FALSE) {}
	virtual ~wxPython() { Cleanup(); }

	//! Returns the global instance of this class.
	static wxPython *Get() { return m_pPython; }

	//! Inits Python interpreter.
	virtual bool Init();

	//! Undoes what #Init() does.
	virtual void Cleanup();

	//! Returns TRUE if Python is ready.
	virtual bool isReady() const;

	//! Returns the list of the functions currently recognized by the interpreter.
	virtual void GetFunctionList(wxScriptFunctionArray &) const;

	//! Uses the Py_GetVersion function to return a version string.
	virtual wxString GetVersionInfo() const
		{ return wxT("Python ") + PY2WX(Py_GetVersion()); }
};



#endif		// wxSCRIPT_USE_PYTHON


