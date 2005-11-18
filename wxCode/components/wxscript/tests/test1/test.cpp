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
// If you want to disable one or more of these dependencies you can use
// the USE_XXXXX options when compiling the sample using the makefiles
// of the BUILD folder.



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
#ifdef __VISUALC__
#include <conio.h>			// miscellaneous includes
#endif
#include <wx/script.h>			// our interpreters...

#include <wx/script.h>		// our interpreter...
#include <wx/sclua.h>		// our interpreter...
#include <wx/sccint.h>		// our interpreter...
#include <wx/scunderc.h>		// our interpreter...
#include <wx/scpython.h>		// our interpreter...
#include <wx/scperl.h>		// our interpreter...



// without this pragma, the stupid compiler precompiles #defines below so that
// changing them doesn't "take place" later!
#ifdef __VISUALC__
    #pragma hdrstop
#endif

#define CUSTOM_SCRIPT			wxT("math.script")


// first of all, decide if we can use the system...
#if defined(__VISUALC__)
	#define mcDETECT_MEMORY_LEAKS
#endif


#ifdef mcDETECT_MEMORY_LEAKS

	// "crtdbg.h" is included only with MSVC++ and Borland, I think...
	// "stackwalker.h" instead, contains a set of stack walker functions
	// created by Jochen Kalmbach (thanks !!!) which allow to read the
	// intercept unhandled exceptions and memory-leaks. 
	// To be used, the file must be part of the project; this is why
	// it's contained (with its CPP counterpart) in the folder of this
	// test program. Anyway,  you can find it also online at:
	//     http://www.codeproject.com/tools/leakfinder.asp
	#include <crtdbg.h>

	// define some useful macros
	#define new                 new(_NORMAL_BLOCK, THIS_FILE, __LINE__)
	#define mcDUMP_ON_EXIT		{ _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF); }

	
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;


	// this little class is used to access Stackwalker functions
	// without changing a line of code...
	class mcLeakDetector {

	public:
		mcLeakDetector() { mcDUMP_ON_EXIT; }
		~mcLeakDetector() {}
	};

	// ...infact, instancing a STATIC mcLeakDetector class, we
	// can start memory-leak detection at the very beginning of
	// the program (when the main() or winmain() has not been
	// called yet, that is, when the framework is creating the
	// static variables of the program) and end it at the very
	// end of the program (when, after the main() or winmain(),
	// the framework removes the static variables).
	static mcLeakDetector detector;

#endif



// ----------------------------------------------------------------------------
// defines
// ----------------------------------------------------------------------------

// these will be used to perform various types of tests:
#ifdef wxSCRIPT_USE_LUA
	#define TEST_LUA
#endif
#ifdef wxSCRIPT_USE_PYTHON
	#define TEST_PYTHON
#endif
#ifdef wxSCRIPT_USE_CINT
	#define TEST_CINT
#endif
#ifdef wxSCRIPT_USE_UNDERC
	#define TEST_UNDERC
#endif
#ifdef wxSCRIPT_USE_PERL
	#define TEST_PERL
#endif



// ----------------------------------------------------------------------------
// entry point
// ----------------------------------------------------------------------------

void MainTestSet()
{
	wxString basepath = wxGetCwd();
	 
	if (basepath.Right(5).IsSameAs(wxT("test1"), FALSE))
		basepath = basepath.Left(basepath.Len()-6);
	if (basepath.Right(5).IsSameAs(wxT("build"), FALSE))
		basepath = basepath.Left(basepath.Len()-6) + wxFileName::GetPathSeparator() + wxT("tests");
		
	basepath += wxFileName::GetPathSeparator();
	basepath += wxT("testscripts");
	basepath += wxFileName::GetPathSeparator();
	
	wxPrintf(wxT(">Base path is: '%s'\n"), basepath.c_str());

	// load the script
	wxString filename(basepath + CUSTOM_SCRIPT);
	wxScriptFile *pf = wxScriptInterpreter::Load(filename, wxRECOGNIZE_FROM_COMMENT);
	if (pf == NULL) {
		wxPrintf(wxT(">Failed to load '%s'.\n"), filename.c_str());
		return;
	}

	// now, try to calculate all the functions loaded at some fixed values....
	wxScriptFunctionArray arr;
	wxScriptInterpreter::GetTotalFunctionList(arr);
	wxScriptVar result;
	wxScriptVar args[2];

	for (int i=0; i<arr.GetCount(); i++) {

		wxScriptFunction *func = arr.Get(i);
		for (int j=0; j<10; j++) {
			args[0].Set(wxSTG_INT, (long)(j));

			// lets call the i-th function
			if (!func->Exec(result, args)) {
				wxPrintf(wxT(">Failed to run the '%s' function.\n"), 
						func->GetName().c_str());
				wxPrintf(wxT("Error description: %s\n"), 
					wxScriptInterpreter::GetLastErr().c_str());
				return;
			}

			wxPrintf(wxT(">Calling %s(%d); result is: %s\n"), 
				func->GetName().c_str(), j, result.GetContentString().c_str());
		}

		wxPrintf(wxT("\n"));
	}

	// do not leave memory leaks
	delete pf;
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
	wxPrintf(wxT("\n\n"));
	wxPrintf(wxT(" wxScript test program\n"));
	wxPrintf(wxT(" -------------------------\n\n"));
	wxPrintf(wxT(" I will now run a customizable script ('%s')\n"), CUSTOM_SCRIPT);
	wxPrintf(wxT(" and then I will compute the functions it contains\n"));
	wxPrintf(wxT(" for some fixed values (1,2,3...10).\n\n"));
  
	// init
	wxScriptInterpreter::Init();		// use default interpreters
	wxPrintf(wxT(">I'm initializing the script interpreters...\n"));
	if (!wxScriptInterpreter::areAllReady()) {
		wxPrintf(wxT("Initialization failed."));
		wxScriptInterpreter::Cleanup();
		return 0;
	}

#if !defined(TEST_LUA) && !defined(TEST_PYTHON) && !defined(TEST_CINT) && !defined(TEST_UNDERC) && !defined(TEST_PERL)
	wxPrintf(wxT("No script interpreter to test ?\n"));
	wxScriptInterpreter::Cleanup();
	return 0;
#endif

	wxPrintf(wxT(">For our tests, we are using:\n"));
#ifdef TEST_LUA
	wxPrintf(wxLua::Get()->GetVersionInfo() + wxT("\n"));
#endif
#ifdef TEST_PYTHON
	wxPrintf(wxPython::Get()->GetVersionInfo() + wxT("\n"));
#endif
#ifdef TEST_CINT
	wxPrintf(wxCINT::Get()->GetVersionInfo() + wxT("\n"));
#endif
#ifdef TEST_UNDERC
	wxPrintf(wxUnderC::Get()->GetVersionInfo() + wxT("\n"));
#endif
#ifdef TEST_PERL
	wxPrintf(wxPerl::Get()->GetVersionInfo() + wxT("\n"));
#endif


	// run some tests
	MainTestSet();

	wxPrintf(wxT(">Closing the script interpreter...\n"));
	wxScriptInterpreter::Cleanup();

	// wait for user keypress
	wxPrintf(wxT("Press a key to continue...\n"));
#ifdef __VISUALC__
	while (!_kbhit());
#else
	getchar();
#endif

    // return a success value
    return 0;
}



