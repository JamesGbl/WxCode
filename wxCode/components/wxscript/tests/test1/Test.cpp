///////////////////////////////////////////////////////////////////////
// Test - a little wxScript classes usage sample                     //
// Copyright (C) 2003 by Francesco Montorsi                          //
//                                                                   //
// This program is free software; you can redistribute it and/or     //
// modify it under the terms of the GNU General Public License       //
// as published by the Free Software Foundation; either version 2    //
// of the License, or (at your option) any later version.            //
//                                                                   //
// This program is distributed in the hope that it will be useful,   //
// but WITHOUT ANY WARRANTY; without even the implied warranty of    //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the      //
// GNU General Public License for more details.                      //
//                                                                   //
// You should have received a copy of the GNU General Public         //
// License along with this program; if not, write to the Free        //
// Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,   //
// MA 02111-1307, USA.                                               //
//                                                                   //
// For any comment, suggestion or feature request, please contact    //
// frm@users.sourceforge.net                                         //
//                                                                   //
///////////////////////////////////////////////////////////////////////


// To be correctly compiled this test program needs many libraries:
//
// ucc12.lib           for the UnderC interpreter
// libcint.lib         for the CINT interpreter
// tolua.lib 
//  + lua.lib 
//  + lualib.lib    for the LUA interpreter & TOLUA utility
//
// and wxWidgets libraries
//
//
// If you want to disable one or more of these dependencies you can define
// the wxSCRIPT_NO_UNDERC or wxSCRIPT_NO_CINT or wxSCRIPT_NO_LUA at compile
// time using the -D switch of your compiler.


// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include <wx/defs.h>		// wxWidgets includes
#include <wx/string.h>
#include <wx/file.h>
#include <wx/app.h>
#include <wx/log.h>
#include <wx/cmdline.h>
#include <wx/filename.h>

#include <stdio.h>			// miscellaneous includes
#include <tolua.h>

#include <wx/script.h>		// our interpreter...
#include "Test.h"


#ifdef wxSCRIPT_USE_LUA

	// for advanced testing
	//TOLUA_API int tolua_toexport_open(lua_State *);
#endif


// without this pragma, the stupid compiler precompiles #defines below so that
// changing them doesn't "take place" later!
#ifdef __VISUALC__
    #pragma hdrstop
#endif



// ----------------------------------------------------------------------------
// entry point
// ----------------------------------------------------------------------------

void CallFnc1(const wxScriptFunctionArray &arr)
{
	// try to exec func1 with both CINT & UnderC
	// func1 should be defined as:
	//
	//          int xxxx_func1(char *str, int n)
	//
	wxScriptFunction *fcint = arr.Get("cint_func1");
	wxScriptFunction *fuc = arr.Get("uc_func1");
	wxScriptFunction *flua = arr.Get("lua_func1");

	wxScriptVar result;
	wxScriptVar args[3];

	args[0] = wxScriptVar("char*", "try");
	args[1] = wxScriptVar("int", "3");
	args[2] = wxScriptVar();		// close the list with an empty variable

	if (fcint) {

		// check this is the function we think
		if (fcint->Match(wxScriptTypeINT, 2, wxScriptTypePCHAR, wxScriptTypeINT)) {
			
			// ok, it's our function...
			fcint->Exec(result, args);
			wxPrintf(">%s('try', 3) returned %s\n", fcint->GetName().c_str(),
				result.GetContentString().c_str());
		}
	}

	if (fuc) {

		// check this is the function we think
		if (fuc->Match(wxScriptTypeINT, 2, wxScriptTypePCHAR, wxScriptTypeINT)) {
			
			// ok, it's our function...
			fuc->Exec(result, args);
			wxPrintf(">%s('try', 3) returned %s\n", fuc->GetName().c_str(), 
				result.GetContentString().c_str());
		}
	}

	if (flua) {

		// we cannot check if this is our function...
		if (!flua->Exec(result, args)) {
			
			wxPrintf("Execution failed: %s", wxScriptInterpreter::GetLastErr().c_str());
			
		} else {
			
			wxPrintf(">%s('try', 3) returned %s\n", flua->GetName().c_str(), 
				result.GetContentString().c_str());
		}
	}
}

void CallFnc2(const wxScriptFunctionArray &arr)
{
	// try to exec func1 with both CINT & UnderC
	// func1 should be defined as:
	//
	//          bool xxxx_func2(bool input)
	//
	wxScriptFunction *fcint = arr.Get("cint_func2");
	wxScriptFunction *fuc = arr.Get("uc_func2");
	wxScriptFunction *flua = arr.Get("lua_func2");

	wxScriptVar result;
	wxScriptVar args[2];

	args[0] = wxScriptVar("bool", "true");
	args[1] = wxScriptVar();

	if (fcint) {

		// check this is the function we think
		if (fcint->Match(wxScriptTypeBOOL, 1, wxScriptTypeBOOL)) {
			
			// ok, it's our function...
			fcint->Exec(result, args);
			wxPrintf(">%s('true') returned %s\n", fcint->GetName().c_str(),
				result.GetContentString().c_str());
		}
	}

	if (fuc) {

		// check this is the function we think
		if (fcint->Match(wxScriptTypeBOOL, 1, wxScriptTypeBOOL)) {
			
			// ok, it's our function...
			fuc->Exec(result, args);
			wxPrintf(">%s('true') returned %s\n", fuc->GetName().c_str(),
				result.GetContentString().c_str());
		}
	}

	if (flua) {

		// we cannot check if this is our function...
		if (!flua->Exec(result, args)) {
			
			wxPrintf("Execution failed: %s", wxScriptInterpreter::GetLastErr().c_str());
			
		} else {
			
			wxPrintf(">%s('true') returned %s\n", flua->GetName().c_str(),
				result.GetContentString().c_str());
		}		
	}
}

void CallAdvanced(const wxScriptFunctionArray &arr)
{
	wxScriptFunction *flua = arr.Get("lua_advanced");

	// this is the test object we will pass to our LUA function:
	// it should be able to modify it and return it...
	myClass test;
	test.set(FALSE);

	wxScriptVar result;
	wxScriptVar args[2];

	args[0] = wxScriptVar("myClass*", (int *)(&test));
	args[1] = wxScriptVar();

	if (flua) {

		// we cannot check if this is our function...
		if (!flua->Exec(result, args)) {
			
			wxPrintf("Execution failed: %s", wxScriptInterpreter::GetLastErr().c_str());
			
		} else {

			// if everything was right, result is a pointer to myClass * !!!
			if (!result.GetType().isPointer()) {
				wxPrintf("Invalid return value...");
				return;
			}

			myClass *returned = (myClass *)result.GetPointer();
			bool check = (returned == &test);
			
			wxPrintf(">%s(myClass(false)) returned myClass(%d)\n", 
				flua->GetName().c_str(),
				returned->get());
		}		
	}

#ifdef wxSCRIPT_USE_UNDERC
#if 0
	// normally, to call a script function (whose you know name, return type
	// and argument list types) you should just write something like:
	const char *fncname = "uc_func3";
	wxScriptFunction *myfnc = arr.Get(fncname);
	wxScriptVar ret;

	// exec that function
	bool bexec = (myfnc ? myfnc->Exec(ret, NULL) : FALSE);
	
	// get our real object pointer which can then be normally used in
	// the C++ code of this program...
	mcMathSystem *ms = (mcMathSystem *)ret.GetPointer();
	
	if (bexec && ms) {

		wxPrintf(">Called %s; the returned address is %s and the number of lines\n",
			" contained in the returned mcMathSystem is %d.\n",
			myfnc->GetName().c_str(), ret.GetContentString(), ms->GetLineCount());

	} else {

		wxPrintf(">Error calling the %s function !!!", fncname);
	}
#endif
#endif
}

void MainTestSet()
{
	wxString basepath = wxGetCwd();
	 
	if (basepath.Right(5).IsSameAs("test1", FALSE))
		basepath = basepath.Left(basepath.Len()-6);
		
	basepath +=	wxFileName::GetPathSeparator();
	basepath += "testscripts";
	basepath += wxFileName::GetPathSeparator();
	
	wxPrintf(">Base path is: '%s'\n", basepath.c_str());

	// init
	wxScriptInterpreter::Init(TRUE, FALSE, TRUE);
	wxPrintf(">I'm initializing the script interpreter...\n");
	if (!wxScriptInterpreter::areAllReady()) {
		wxPrintf("Initialization failed.");
		return;
	}

#ifdef wxSCRIPT_USE_CINT
	// load a CINT script file
	wxPrintf(">Loading the 'script1.cxx'...\n");
	wxScriptFile *file1 = wxScriptInterpreter::Load(basepath + "script1.cxx");
	if (!file1) {
		wxPrintf("Load failed.");
		return;
	}
#endif

#ifdef wxSCRIPT_USE_UNDERC
	// load an UnderC script file
	wxPrintf(">Loading the 'script2.uc'...\n");
	wxScriptFile *file2 = wxScriptInterpreter::Load(basepath + "script2.uc");
	if (!file2) {
		wxPrintf("Load failed.");
		return;
	}
#endif


#ifdef wxSCRIPT_USE_LUA
#if 0
	// before loading a Lua script file, open the ToExport package
	// created through the TOLUA program
	tolua_toexport_open(wxLua::Get()->m_state);
#endif
	// load a Lua script file
	wxPrintf(">Loading the 'script3.lua'...\n");
	wxScriptFile *file3 = wxScriptInterpreter::Load(basepath + "script3.lua");
	if (!file3) {
		wxPrintf("\nLoad failed: %s", wxScriptInterpreter::GetLastErr().c_str());
		return;
	}
#endif

	// get function list
	wxScriptFunctionArray arr;
	wxScriptInterpreter::GetTotalFunctionList(arr);
	wxPrintf(">Loaded %d functions:\n", arr.GetCount());
	for (int i=0; i < arr.GetCount(); i++)
		wxPrintf("%s\n", arr.GetName(i).c_str());

	// do some function calls with a lot of tests to be sure everything is okay
	CallFnc1(arr);
	CallFnc2(arr);
	//CallAdvanced(arr);

	// leave some space
	wxPrintf(">Test completed.\n\n\n");

	// do not leave memory leaks
	wxScriptInterpreter::Cleanup();
}

int main(int argc, char **argv)
{
#ifdef __WXMSW__
	// first of all, do some checks
    wxApp::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "Test");
    wxInitializer initializer;
    if (!initializer) {
        fprintf(stderr, "Failed to initialize the wxWindows library, aborting.");
        return -1;
    }
#endif
	// now, type some help info...
	wxPrintf("\n\n");
	wxPrintf(" wxScript test program\n");
	wxPrintf(" -------------------------\n\n");
	wxPrintf(" This is a little test program which runs some didactive tests \n");
	wxPrintf(" about the wxScript and wxScript-related classes.\n\n");
    
	// run some tests
	MainTestSet();

    // return a success value
    return 0;
}



