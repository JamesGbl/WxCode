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
	PyObject *CreatePyObjFromScriptVar(const wxScriptVar &toconvert) const;

	//! Converts the given PyObject into a wxScriptVar.
	wxScriptVar CreateScriptVarFromPyObj(PyObject *toconvert) const;

public:

	//! Creates the object; no info about return value and
	//! arguments are required since wxPython does not store them.
	wxScriptFunctionPython(const wxString &name = wxEmptyString,
						PyObject *dictionary = NULL, PyObject *func = NULL);

	virtual wxScriptFunction *Clone() const {
		wxScriptFunction *newf = new wxScriptFunctionPython();
		newf->DeepCopy(this);
		return newf;
	}

	virtual void DeepCopy(const wxScriptFunction *tocopy) {
		wxScriptFunctionPython *pf = (wxScriptFunctionPython *)tocopy;
		m_pDict = pf->m_pDict;
		m_pFunc = pf->m_pFunc;
		wxScriptFunction::DeepCopy(tocopy);
	}

	void Set(const wxString &name, PyObject *dictionary, PyObject *function);
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

	//! The Python global objects.
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


