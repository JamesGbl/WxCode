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


// To be correctly linked this test program needs many libraries:
//
// ucc12.lib           for the UnderC interpreter
// libcint.lib         for the CINT interpreter
// tolua.lib 
//  + lua.lib 
//  + lualib.lib       for the LUA interpreter & TOLUA utility
//
// and wxWidgets libraries
//
//
// If you want to disable one or more of these dependencies you can compile
// the wxscript.lib library defining one of the following symbols:
//
//   wxSCRIPT_NO_LUA
//   wxSCRIPT_NO_CINT
//   wxSCRIPT_NO_UNDERC
//   wxSCRIPT_NO_BASIC
//
// If you disable one or more of the interpreters, then you will need
// to rebuilt this test program without one or more of the following
// defines:

#define TEST_LUA
//#define TEST_CINT		// currently disabled because it does not compile on my RH9
//#define TEST_UNDERC	// some implementation problems...
#define TEST_BASIC


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
#include <wx/script.h>		// our interpreters...


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
	wxScriptInterpreter::Init(TRUE, TRUE, TRUE);
	wxPrintf(">I'm initializing the script interpreter...\n");
	if (!wxScriptInterpreter::areAllReady()) {
		wxPrintf("Initialization failed.");
		wxScriptInterpreter::Cleanup();
		return;
	}

#ifdef TEST_CINT
	// load a CINT script file
	wxPrintf(">Loading the 'script1.cxx'...\n");
	wxScriptFile *file1 = wxScriptInterpreter::Load(basepath + "script1.cxx");
	if (!file1) {
		wxPrintf("Load failed.");
		wxScriptInterpreter::Cleanup();
		return;
	}
#endif

#ifdef TEST_UNDERC
	// load an UnderC script file
	wxPrintf(">Loading the 'script2.uc'...\n");
	wxScriptFile *file2 = wxScriptInterpreter::Load(basepath + "script2.uc");
	if (!file2) {
		wxPrintf("Load failed.");
		wxScriptInterpreter::Cleanup();
		return;
	}
#endif

#ifdef TEST_LUA
	// load a Lua script file
	wxPrintf(">Loading the 'script3.lua'...\n");
	wxScriptFile *file3 = wxScriptInterpreter::Load(basepath + "script3.lua");
	if (!file3) {
		wxPrintf("\nLoad failed: %s", wxScriptInterpreter::GetLastErr().c_str());
		wxScriptInterpreter::Cleanup();
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

	// leave some space
	wxPrintf(">Test completed.\n\n\n");

	// do not leave memory leaks
	wxScriptInterpreter::Cleanup();
}

int main(int, char **)
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



