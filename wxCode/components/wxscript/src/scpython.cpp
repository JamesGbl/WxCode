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
	m_pDict = PyModule_GetDict(m_pModule);
	m_pGlobals = m_pDict;
	m_pLocals = m_pDict;
	
	// fill the std array
	GetFunctionListComplete(m_arrStd);

	return isReady();
}

void wxPython::Cleanup()
{
	//Py_DECREF(m_pGlobals);
	//Py_DECREF(m_pDict);
	//Py_DECREF(m_pModule);

	Py_Finalize();
}

bool wxPython::isReady() const
{
	// returns !=0 is everything is okay
	return Py_IsInitialized() != 0;
}

void wxPython::GetFunctionListComplete(wxScriptFunctionArray &arr) const
{
	// get the list of global variables from Lua:
	// the functions are considered global variables because
	// 
	// function myf(a)     is translated as      myf = function(a)
	// end                                       end
	//
	// that is, myf is a global variable...
	//

	try {

		// we'll scan the items of the globals dictionary
		// (which are given to us into a PyList)
		PyObject *list = PyDict_Values(m_pGlobals);
		if (!list || PyList_Check(list) == FALSE)
			return;
				
		for (int i=0,max=PyList_Size(list); i<max; i++) {
			
			// get the i-th item
			PyObject *elem = PyList_GetItem(list, i);
			if (elem && PyCallable_Check(elem) != 0) {
				
				// this is a function; add it to the list
				PyObject *str = PyObject_GetAttrString(elem, "func_name");
				wxString name(PY2WX(PyString_AsString(str)));
				arr.Append(new wxScriptFunctionPython(name, m_pGlobals, elem));
			}
		}
		
	} catch (...) {
		
		// if there was an exception, then something went wrong
		return;
	}
}

void wxPython::GetFunctionList(wxScriptFunctionArray &arr) const
{
	arr.Clear();
	GetFunctionListComplete(arr);
	
	// be sure that this is not a standard function
	for (int i=0; i < arr.GetCount(); i++) {

		// compare this function with all the std functions
		for (int j=0; j < m_arrStd.GetCount(); j++) {
			if (m_arrStd.GetName(j) == arr.GetName(i)) {

				// this function is a standard one; remove it because
				// we want to keep only user-loaded functions
				arr.Remove(i);
				i--;		// recheck this entry
				break;
			}
		}
	}
}



// -------------------------
// wxSCRIPTFUNCTIONPYTHON
// -------------------------

wxScriptFunctionPython::wxScriptFunctionPython(const wxString &name,
											   PyObject *dict, PyObject *func)
{
//	wxASSERT_MSG(func != NULL && dict != NULL, wxT("Invalid Python objects"));

	// set our name, dict & func pointers
	Set(name, dict, func);
}

void wxScriptFunctionPython::Set(const wxString &name, PyObject *dict, PyObject *func)
{
	// set name
	m_strName = name;

	// do we need to proceed ?
	if (!dict || !func)
		return;		// don't proceed

	// and our python vars
	m_pDict = dict;
	m_pFunc = func;

	// get the number of arguments taken by this function
	wxASSERT(PyObject_HasAttrString(func, "func_code"));
	PyObject *code = PyObject_GetAttrString(func, "func_code");
	wxASSERT(PyObject_HasAttrString(code, "co_argcount"));
	PyObject *argc = PyObject_GetAttrString(code, "co_argcount");
	wxASSERT(PyInt_Check(argc));

	// convert the PyInt object to a number & store it
	m_nArgCount = (int)PyInt_AsLong(argc);
}

bool wxScriptFunctionPython::Exec(wxScriptVar &ret, wxScriptVar *arg) const
{
	if (!m_pFunc) return FALSE;		// this is an invalid function

	// create the ruple with the arguments to give to the function
	PyObject *t = PyTuple_New(m_nArgCount);
	for (int i=0; i < m_nArgCount; i++) {
		if (PyTuple_SetItem(t, i, CreatePyObjFromScriptVar(arg[i])) != 0) {

			wxScriptInterpreter::m_strLastErr = 
				wxT("Could not create the argument tuple");
			return FALSE;
		}
	}

	// do the function call
	try {
	
		PyObject *res = PyObject_CallObject(m_pFunc, t);
		if (PyErr_Occurred() != NULL) {

			PyErr_Print();
			return FALSE;
		}

		// convert python result object to a wxScriptVar
		ret = CreateScriptVarFromPyObj(res);

	} catch (...) {

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

		res = PyBool_FromLong(toconvert.GetContentLong());

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

	if (PyBool_Check(toconvert)) {

		int res;
		if (PyObject_Cmp(toconvert, Py_True, &res) == -1) {

			ret.GetType().SetGenericType(wxSTG_UNDEFINED);
			return ret;
		}

		if (res == -1)
			ret.Set(wxSTG_BOOL, (bool)FALSE);
		else
			ret.Set(wxSTG_BOOL, (bool)TRUE);

	} else if (PyInt_Check(toconvert)) {

		ret.Set(wxSTG_INT, (long)PyInt_AsLong(toconvert));

	} else if (PyString_Check(toconvert)) {

		ret.Set(wxT("char*"), PY2WX(PyString_AsString(toconvert)));
	}

	return ret;
}




// --------------------
// wxSCRIPTFILEPYTHON
// --------------------

bool wxScriptFilePython::Load(const wxString &filename)
{
	try {
		FILE *f = fopen(WX2PY(filename),"r");
		if (!f)
			return FALSE;
		PyRun_SimpleFile(f, WX2PY(filename));
			//return TRUE;
		//return FALSE;
		fclose(f);
		return TRUE;

	
		// open the file
		wxFile file(filename, wxFile::read);
		int len = file.Length();

		// create a buffer & fill it with file contents
		char *data = new char[len+1];
		if ((int)file.Read(data, len) != len) {

			// read error
			delete [] data;
			return FALSE;
		}

		data[len] = '\0';
		wxString str(PY2WX(data));
		delete [] data;

		// remember the script filename
		PyObject *ret = PyRun_StringFlags(WX2PY(str), Py_file_input,
			wxPython::Get()->m_pGlobals, wxPython::Get()->m_pLocals, 0);
		/*PyObject *ret = PyRun_StringFlags("def nothing():\n\treturn 'ciao'\n", 
				Py_file_input, wxPython::Get()->m_pGlobals, 
				wxPython::Get()->m_pLocals, 0);*/
		if (ret) Py_DECREF(ret);
		if (PyErr_Occurred() != NULL) {

			PyErr_Print();
			return FALSE;
		}

		return TRUE;		

	} catch (...) {

		// if there was an exception, then something gone wrong
		return FALSE;
	}
	
	return TRUE;
}

#endif		// wxSCRIPT_USE_PYTHON
