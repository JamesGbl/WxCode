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

	m_pModule = PyImport_AddModule("__main__");
	m_pDict = PyModule_GetDict(m_pModule);
	m_pGlobals = m_pDict;
	m_pLocals = m_pDict;
	
	// fill the std array
	GetFunctionListComplete(m_arrStd);

	return isReady();
}

bool wxPython::isReady() const
{
	// returns !=0 is everything is okay
	return Py_IsInitialized() != 0;
}

/*
bool wxPython::Init()
{
	// init Lua
	m_state = lua_open();
	m_bInit = (m_state != NULL);

	// now load standard Lua libraries
	const luaL_reg *lib;
	
	for (lib = luaLibs; lib->func != NULL; lib++) {
		lib->func(m_state);           // Open the library								
		
		// Flush the stack, by setting the top to 0, in order to
		// ignore any result given by the library load function.		
		lua_settop(m_state, 0);
	}

	// add our extension to the list of the available for loading extensions:
	wxScriptFile::m_strFileExt[wxPython_SCRIPTFILE] = wxT("LUA");
	
	// fill the std array
	GetFunctionListComplete(m_arrStd);

	return m_bInit;
}

void wxPython::Cleanup()
{
	// close Lua
	lua_close(m_state);	
}

// a useful little function
bool wxPython::SetErrAndReturn(lua_State *L)
{
	// the error description should have been placed in the stack
	wxString err(LUA2WX(lua_tostring(L, -1)));
	wxScriptInterpreter::m_strLastErr = err;

	// this is a simple way to allow the caller to return immediately
	// without writing   if (result != 0) { SetErrAndReturn(L); return FALSE;}
	// but with a quick  if (result != 0) return SetErrAndReturn(L);
	return FALSE;
}
*/
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
	return;

	try {

	PyObject *dict = PyObject_Dir(NULL);
	if (!dict || PyDict_Check(dict) == FALSE)
		return;
	//PyErr_Occurred() 


	PyObject *list = PyDict_Items(dict);
	for (int i=0,max=PyList_Size(list); i<max; i++) {

		PyObject *elem = PyList_GetItem(list, i);
		if (PyObject_TypeCheck(elem, &PyCFunction_Type) != 0) {

			// this is a function; add it
			PyObject *str = PyObject_GetAttrString(elem, "func_name");
			wxString name(PY2WX(PyString_AsString(str)));
			arr.Append(new wxScriptFunctionPython(name));
		}
	}
	
	} catch (...) {

		// if there was an exception, then something gone wrong
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

bool wxScriptFunctionPython::Exec(wxScriptVar &ret, wxScriptVar *arg) const
{
	

	// finally we have finished...
	return TRUE;
}




// --------------------
// wxSCRIPTFILEPYTHON
// --------------------

bool wxScriptFilePython::Load(const wxString &filename)
{
//	return TRUE;

	try {
	
		// open the file
		wxFile file(filename, wxFile::read);
		int len = file.Length();

		// create a buffer & fill it with file contents
		char *data = new char[len+1];
		if (file.Read(data, len) != len) {
			delete [] data;
			return FALSE;
		}

		data[len] = '\0';
		wxString str(PY2WX(data));
		delete [] data;

		// remember the script filename
		//PyRun_File(, WX2PY(file), Py_file_input, 
		//		wxPython::Get()->m_pGlobals, wxPython::Get()->m_pLocals);
		PyObject *ret = PyRun_String(WX2PY(str), 0,
				wxPython::Get()->m_pGlobals, wxPython::Get()->m_pLocals);

		PyObject_Del(ret);

	} catch (...) {

		// if there was an exception, then something gone wrong
		return FALSE;
	}
	
	return TRUE;
}

#endif		// wxSCRIPT_USE_PYTHON
