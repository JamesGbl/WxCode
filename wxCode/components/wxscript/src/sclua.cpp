///////////////////////////////////////////////////////////////////////////////
// Name:        sclua.cpp
// Author:      Francesco Montorsi
// Modified by:
// Created:     2004/6/5
// RCS-ID:      
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////


// includes
#include "wx/script.h"
#ifdef wxSCRIPT_USE_LUA				// compile-time choice (see wx/script.h)

// required includes
#include <wx/tokenzr.h>
#include "wx/sclua.h"
#include <tolua.h>
#include <math.h>

// init STATICs
luaL_reg wxLua::luaLibs[] = {
        { "base",       luaopen_base },
        { "table",      luaopen_table },
        { "io",         luaopen_io },
        { "string",     luaopen_string },
        { "math",       luaopen_math },
        { "debug",      luaopen_debug },
        { "loadlib",    luaopen_loadlib },
        { NULL,         NULL }
};




// -----------------
// wxLUA
// -----------------

bool wxLua::Init()
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
	
	// fill the std array
	GetFunctionListComplete(m_arrStd);

	return m_bInit;
}

void wxLua::Cleanup()
{
	// close Lua
	lua_close(m_state);	
}

// a useful little function
bool wxLua::SetErrAndReturn(lua_State *L)
{
	// the error description should have been placed in the stack
	wxString err(lua_tostring(L, -1));
	wxScriptInterpreter::m_strLastErr = err;

	// this is a simple way to allow the caller to return immediately
	// without writing   if (result != 0) { SetErrAndReturn(L); return FALSE;}
	// but with a quick  if (result != 0) return SetErrAndReturn(L);
	return FALSE;
}

void wxLua::GetFunctionListComplete(wxScriptFunctionArray &arr) const
{
	// get the list of global variables from Lua:
	// the functions are considered global variables because
	// 
	// function myf(a)     is translated as      myf = function(a)
	// end                                       end
	//
	// that is, myf is a global variable...
	//
	lua_pushnil(m_state);			// first key must be nil
	
	char str[256];

	str[0] = 0;
	while (lua_next(m_state, LUA_GLOBALSINDEX) != 0) {
	
		int idx = -2;

		// we are interested only to functions
		if (lua_isfunction(m_state, -1)) {

			if (lua_isstring(m_state, idx))
				strcpy(str, lua_tostring(m_state, idx));
			
			// finally, create this new function entry
			arr.Append(new wxScriptFunctionLua(str));
		}

		lua_pop(m_state, 1); // removes ‘value’; keeps ‘key’ for next iteration
	}
}

void wxLua::GetFunctionList(wxScriptFunctionArray &arr) const
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
// wxSCRIPTFUNCTIONLUA
// -------------------------

bool wxScriptFunctionLua::Exec(wxScriptVar &ret, wxScriptVar *arg) const
{
	lua_State *L = wxLua::Get()->m_state;

	// first of all, push the name of the function in the Lua stack
	lua_pushstring(L, GetName());
	lua_gettable(L, LUA_GLOBALSINDEX);

	// then, push in direct order (as first the first argument) the arguments
	int n = 0;
	while (arg[n].GetType().isValid()) {

		wxScriptTypeInfo t = arg[n].GetType();

		// a bool ?
		if (t.Match(wxScriptTypeBOOL))
			tolua_pushboolean(L, arg[n].GetContentLong() != 0);

		// a long, char, or int ?
		else if (t.Match(wxScriptTypeINT) ||
				t.Match(wxScriptTypeLONG) ||
				t.Match(wxScriptTypeCHAR))
			tolua_pushnumber(L, (lua_Number)arg[n].GetContentLong());

		// a float or a double ?
		else if (t.Match(wxScriptTypeFLOAT) ||
				t.Match(wxScriptTypeDOUBLE))
			tolua_pushnumber(L, (lua_Number)arg[n].GetContentDouble());

		// a pointer ?
		else if (t.isPointer()) {

			// to chars ?
			wxScriptTypeInfo pt = t.GetPointerType();
			if (pt.Match(wxScriptTypeCHAR))
				tolua_pushstring(L, (const char *)arg[n].GetPointer());

			// to something else ?
			else
				tolua_pushusertype(L, (void*)arg[n].GetPointer(), 
							arg[n].GetType().GetPointerTypeName());
				//lua_pushlightuserdata(L, arg[n].GetPointer());
		}

		// what ???
		else 
			return FALSE;

		n++;		// push next argument
	}
				

	// exec this function (retrieving always only 1 result)
	int result = lua_pcall(L, n, 1, 0);
	if (result != 0) return wxLua::SetErrAndReturn(L);

	// get returned value from the stack...
	lua_Number res;
	switch (lua_type(L, -1)) {		// get the type of the returned value
	case LUA_TNONE:
		return FALSE;

	case LUA_TNIL:
		lua_pop(L, 1);
		ret.SetType("int");
		ret.SetContent("0");
		break;

	case LUA_TNUMBER:
		res = tolua_tonumber(L, -1, 0);
		if (floor(res) == res ||
			ceil(res) == res) {		// this is my personal check for "integerness"

			// this is an integer...
			ret.SetType("long");
			ret.SetContent((long)res);

		} else {

			// this is a floating point number...
			ret.SetType("double");
			ret.SetContent(res);
		}
		break;

	case LUA_TBOOLEAN:
		ret.SetType("bool");
		ret.SetContent(tolua_toboolean(L, -1, 0) != 0);
		break;

	case LUA_TSTRING:
		ret.SetType("char*");
		ret.SetContent(tolua_tostring(L, -1, ""));
		break;

	case LUA_TLIGHTUSERDATA:
		ret.SetType("void*");
		ret.SetContent(tolua_touserdata(L, -1, NULL));
		break;

	case LUA_TUSERDATA:
	case LUA_TFUNCTION:
		ret.SetType("void*");
		ret.SetContent(tolua_tousertype(L, -1, NULL));
		break;
	}

	// finally we have finished...
	return TRUE;
}



// --------------------
// wxSCRIPTFILELUA
// --------------------

bool wxScriptFileLua::Load(const wxString &file)
{
	// remember the script filename
	m_strFileName = file;
	lua_State *L = wxLua::Get()->m_state;

	// load it	
	int status = luaL_loadfile(L, file);
	if (status != 0) return wxLua::SetErrAndReturn(L);

	// execute the script: in this way, we'll force Lua to parse
	// all the function declarations inside the script file and
	// add them to the global table...
	int result = lua_pcall(L, 0, LUA_MULTRET, 0);
	if (result != 0) return wxLua::SetErrAndReturn(L);
	
	return TRUE;
}


#endif		// wxSCRIPT_USE_LUA

