///////////////////////////////////////////////////////////////////////////////
// Name:        scpython.cpp
// Author:      Francesco Montorsi
// Modified by:
// Created:     2005/1/12
// RCS-ID:      
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////




// includes
#include "wx/script.h"
#ifdef wxSCRIPT_USE_PYTHON				// compile-time choice (see wx/script.h)

// required includes
#include <wx/tokenzr.h>
#include <wx/file.h>
#include "wx/scpython.h"
#include <math.h>

// Since I don't know any way to check the python version, I'll use this
// simple trick to know if we have BOOL support
#ifdef Py_BOOLOBJECT_H
	#define wxPYTHON_HAS_BOOL
#endif


// -----------------
// wxPYTHON
// -----------------

bool wxPython::Init()
{
	// inizialize the python interpreter
	Py_Initialize();
	
	// add our extension to the list of the available for loading extensions:
	wxScriptFile::m_strFileExt[wxPYTHON_SCRIPTFILE] = wxT("PY");

	// initialize our "execution frame"
	m_pModule = PyImport_AddModule("__main__");

	// PyImport_AddModule returns a borrowed reference: we'll now transform
	// it to a full reference...
	Py_INCREF(m_pModule);

	// we'll do the same for our dictionary...
	m_pGlobals = PyModule_GetDict(m_pModule);
	Py_INCREF(m_pGlobals);

	// our locals dict will be a reference to the global one...
	m_pLocals = m_pGlobals;
	Py_INCREF(m_pLocals);

	// everything was okay ?
	return isReady();
}

void wxPython::Cleanup()
{
	// free all our references
	Py_DECREF(m_pModule);
	Py_DECREF(m_pGlobals);
	Py_DECREF(m_pLocals);

	// and everything else which is python-related
	Py_Finalize();
}

bool wxPython::isReady() const
{
	// returns !=0 is everything is okay
	return Py_IsInitialized() != 0;
}

void wxPython::GetFunctionList(wxScriptFunctionArray &arr) const
{
	// get the list of the callable objects stored into the
	// GLOBAL dictionary...
	try {

		// we'll scan the items of the globals dictionary
		// (which are given to us into a PyList)
		PyObject *list = PyDict_Values(m_pGlobals);
		if (!list || PyList_Check(list) == FALSE)
			return;
				
		for (int i=0,max=PyList_Size(list); i<max; i++) {
			
			// get the i-th item: this returns a borrowed reference...
			PyObject *elem = PyList_GetItem(list, i);
			if (elem && PyCallable_Check(elem) != 0 &&
				PyObject_HasAttrString(elem, "func_name")) {
				
				// this is a function; add it to the list
				PyObject *str = PyObject_GetAttrString(elem, "func_name");
				wxString name(PYSTRING2WX(str));

				// this is a function defined in the GLOBALS dictionary
				// and identified by the PyObject "elem"
				arr.Append(new wxScriptFunctionPython(name, m_pGlobals, elem));				
				Py_DECREF(str);
			}
		}

		Py_DECREF(list);
		
	} catch (...) {
		
		// if there was an exception, then something went wrong
		wxPython::Get()->OnException();
		return;
	}
}

void wxPython::OnException()
{
	wxASSERT(PyErr_Occurred() != NULL);
	wxString err;

#define pyEXC_HANDLE(x)						\
	else if (PyErr_ExceptionMatches(PyExc_##x))		\
		err = wxString(wxT(#x)) + wxT("\n");

#if 1

	// a generic exception description
	if (PyErr_ExceptionMatches(PyExc_AssertionError))
		err = wxT("Assertion error\n");
	pyEXC_HANDLE(AttributeError)
	pyEXC_HANDLE(EOFError)
	pyEXC_HANDLE(FloatingPointError)
	pyEXC_HANDLE(IOError)
	pyEXC_HANDLE(ImportError)
	pyEXC_HANDLE(IndexError)
	pyEXC_HANDLE(KeyError)
	pyEXC_HANDLE(KeyboardInterrupt)
	pyEXC_HANDLE(MemoryError)
	pyEXC_HANDLE(NameError)
	pyEXC_HANDLE(NotImplementedError)
	pyEXC_HANDLE(OSError)
	pyEXC_HANDLE(OverflowError)
	pyEXC_HANDLE(RuntimeError)
	pyEXC_HANDLE(SyntaxError)
	pyEXC_HANDLE(SystemError)
	pyEXC_HANDLE(SystemExit)
	pyEXC_HANDLE(TypeError)
	pyEXC_HANDLE(ValueError)
	pyEXC_HANDLE(ZeroDivisionError)
	else
		err = wxT("Unknown error\n");
#else

	// for more detailed output (fix on stderr)
	PyErr_Print();

#endif

	wxScriptInterpreter::m_strLastErr = err;
	PyErr_Clear();
}




// -------------------------
// wxSCRIPTFUNCTIONPYTHON
// -------------------------

wxScriptFunctionPython::wxScriptFunctionPython(const wxString &name,
											   PyObject *dict, PyObject *func)
{
	m_pDict = NULL;
	m_pFunc = NULL;

	// set our name, dict & func pointers
	Set(name, dict, func);
}

wxScriptFunctionPython::~wxScriptFunctionPython()
{ 
	// free python objects
	ReleaseOldObj(); 
} 

void wxScriptFunctionPython::ReleaseOldObj()
{
	// we don't need these python objects anymore...
	if (m_pDict) Py_DECREF(m_pDict);
	if (m_pFunc) Py_DECREF(m_pFunc);

	m_pDict = NULL;
	m_pFunc = NULL;
}

void wxScriptFunctionPython::Set(const wxString &name, PyObject *dict, PyObject *func)
{
	ReleaseOldObj();

	// set name
	m_strName = name;

	// and our python vars
	m_pDict = dict;
	m_pFunc = func;

	// do we need to proceed ?
	if (!dict || !func)
		return;		// don't proceed

	// we now own a reference to these python objects
	Py_INCREF(m_pDict);
	Py_INCREF(m_pFunc);

	// get the number of arguments taken by this function
	wxASSERT(PyObject_HasAttrString(func, "func_code"));
	PyObject *code = PyObject_GetAttrString(func, "func_code");
	wxASSERT(PyObject_HasAttrString(code, "co_argcount"));
	PyObject *argc = PyObject_GetAttrString(code, "co_argcount");
	wxASSERT(PyInt_Check(argc));

	// convert the PyInt object to a number & store it
	m_nArgCount = (int)PyInt_AsLong(argc);
	Py_DECREF(code);
	Py_DECREF(argc);
}

bool wxScriptFunctionPython::Exec(wxScriptVar &ret, wxScriptVar *arg) const
{
	if (!m_pFunc) return FALSE;		// this is an invalid function

	// create the ruple with the arguments to give to the function
	PyObject *t = PyTuple_New(m_nArgCount);
	for (int i=0; i < m_nArgCount; i++) {

		// PyTuple_SetItem function will steal a reference from
		// CreatePyObjFromScriptVar so refcount should be okay doing nothing
		if (PyTuple_SetItem(t, i, CreatePyObjFromScriptVar(arg[i])) != 0) {

			wxScriptInterpreter::m_strLastErr = 
				wxT("Could not create the argument tuple");
			return FALSE;
		}
	}

	// do the function call
	try {
	
		PyObject *res = PyObject_CallObject(m_pFunc, t);
		Py_DECREF(t);		// we won't use the tuple anymore...

		if (res == NULL) {		// something wrong ?

			if (PyErr_Occurred() != NULL)
				wxPython::Get()->OnException();
			else
				wxScriptInterpreter::m_strLastErr = wxT("Unknown call error\n");
			return FALSE;
		}
	
		// convert python result object to a wxScriptVar
		ret = CreateScriptVarFromPyObj(res);
		Py_DECREF(res);		// we won't use this anymore

	} catch (...) {

		// something wrong...
		wxPython::Get()->OnException();
		return FALSE;
	}	

	// finally we have finished...
	return TRUE;
}

PyObject *wxScriptFunctionPython::CreatePyObjFromScriptVar(const wxScriptVar &toconvert) const
{
	wxScriptTypeInfo t(toconvert.GetType());
	PyObject *res = NULL;
		
	if (t.Match(*wxScriptTypeINT) ||
		t.Match(*wxScriptTypeLONG) ||
		t.Match(*wxScriptTypeCHAR)) {

		res = PyInt_FromLong(toconvert.GetContentLong());

	} else if (t.Match(*wxScriptTypeFLOAT) || 
				t.Match(*wxScriptTypeDOUBLE)) {

		res = PyFloat_FromDouble(toconvert.GetContentDouble());

	} else if (t.Match(*wxScriptTypeBOOL)) {

#ifdef wxPYTHON_HAS_BOOL
		res = PyBool_FromLong(toconvert.GetContentLong());
#else
		res = PyInt_FromLong(toconvert.GetContentLong());
#endif

	} else if (t.isPointer()) {

		wxScriptTypeInfo pt = t.GetPointerType();
		if (pt.Match(wxScriptTypeCHAR))
			res = PyString_FromString(WX2PY(toconvert.GetContentString()));
		else
			res = PyBuffer_FromMemory(toconvert.GetPointer(), 0);

	}

	return res;
}

wxScriptVar wxScriptFunctionPython::CreateScriptVarFromPyObj(PyObject *toconvert) const
{
	wxScriptVar ret;

	// the order is important (a PyBool is a subtype of a PyInt) !!
#ifdef wxPYTHON_HAS_BOOL
	if (PyBool_Check(toconvert)) {

		// compare this boolean object to the Py_True object
		int res;
		if (PyObject_Cmp(toconvert, Py_True, &res) == -1) {

			ret.GetType().SetGenericType(wxSTG_UNDEFINED);
			return ret;
		}

		// and then set the bool scriptvar
		if (res == -1)
			ret.Set(wxSTG_BOOL, (bool)FALSE);
		else
			ret.Set(wxSTG_BOOL, (bool)TRUE);

	} else 
#endif
	if (PyInt_Check(toconvert)) {

		ret.Set(wxSTG_INT, (long)PyInt_AsLong(toconvert));

	} else if (PyLong_Check(toconvert)) {

		ret.Set(wxSTG_LONG, (long)PyLong_AsLong(toconvert));

	} else if (PyFloat_Check(toconvert)) {

		ret.Set(wxSTG_DOUBLE, (double)PyFloat_AsDouble(toconvert));

	} else if (PyObject_CheckReadBuffer(toconvert)) {

		const void *buffer;
		int len;
		PyObject_AsReadBuffer(toconvert, &buffer, &len);
		ret.Set(wxT("void*"), wxString::Format(wxT("%X"), buffer));

	} else if (PyString_Check(toconvert)) {

		ret.Set(wxT("char*"), PY2WX(PyString_AsString(toconvert)));
	}

	return ret;
}

void wxScriptFunctionPython::DeepCopy(const wxScriptFunction *tocopy)
{
	wxScriptFunctionPython *pf = (wxScriptFunctionPython *)tocopy;

	// release old objects
	ReleaseOldObj();

	// inc the refcount of new objects
	m_pDict = pf->m_pDict;
	m_pFunc = pf->m_pFunc;
	Py_INCREF(m_pDict);
	Py_INCREF(m_pFunc);

	wxScriptFunction::DeepCopy(tocopy);
}




// --------------------
// wxSCRIPTFILEPYTHON
// --------------------

bool wxScriptFilePython::Load(const wxString &filename)
{
	try {

		// open the file
		FILE *f = fopen(WX2PY(filename),"r");
		if (!f) {

			wxScriptInterpreter::m_strLastErr = 
				wxT("Cannot open ") + filename + wxT("\n");
			return FALSE;
		}

		// run the file; this will parse all the functions
		// it contains and add it to the global dictionary
		PyRun_SimpleFile(f, (char *)WX2PY(filename));

		// everything is okay...
		fclose(f);
		return TRUE;

	} catch (...) {

		wxPython::Get()->OnException();

		// if there was an exception, then something gone wrong
		return FALSE;
	}
	
	return TRUE;
}

#endif		// wxSCRIPT_USE_PYTHON
