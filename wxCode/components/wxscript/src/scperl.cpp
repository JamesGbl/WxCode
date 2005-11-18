///////////////////////////////////////////////////////////////////////////////
// Name:        scperl.cpp
// Author:      Jerry Fath
// Created:     2005/13/11
// RCS-ID:      
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////



// includes
#include "wx/script.h"
#ifdef wxSCRIPT_USE_PERL				// compile-time choice (see wx/script.h)

// required includes
#include "wx/scperl.h"

#include <EXTERN.h>
#include <perl.h>
//One of the perl includes defines 'bool' as char which causes problems
//This fixes it
#undef bool

//For wxLogDebug
#include <wx/log.h>

static 	PerlInterpreter *my_perl;


//////////////////////////////////////////////////////
// From a Perl produced perlxsi.c file
//
EXTERN_C void xs_init (pTHX);

EXTERN_C void boot_DynaLoader (pTHX_ CV* cv);

EXTERN_C void
xs_init(pTHX)
{
	char *file = __FILE__;
	dXSUB_SYS;

	/* DynaLoader is a special case */
	newXS("DynaLoader::boot_DynaLoader", boot_DynaLoader, file);
}
///////////////////////////////////////////////////

// -----------------
// wxPERL
// -----------------

bool wxPerl::Init()
{
    int argc=0;
    char **argv=NULL;
    char **env=NULL;

    m_bInit = false;

    PERL_SYS_INIT3(&argc,&argv,&env);
    my_perl = perl_alloc();
    if (my_perl) {
        PL_perl_destruct_level = 0;
        perl_construct(my_perl);
        m_bInit = true;
        //add our extension to the list of the available for loading extensions:
	    wxScriptFile::m_strFileExt[wxPERL_SCRIPTFILE] = wxT("pl");
    }

	return m_bInit;
}

void wxPerl::Cleanup()
{
    PL_perl_destruct_level = 0;
    perl_destruct(my_perl);
    perl_free(my_perl);
    PERL_SYS_TERM();
}

void wxPerl::GetFunctionList(wxScriptFunctionArray &arr) const
{
    HV* mainStash;
    HE* mainEntry;
    CV* mainCV;
    STRLEN len;
    char *keyname;
    I32 heCount;
    wxString subName;

    mainStash = PL_defstash;
    heCount = hv_iterinit(mainStash);
    while ((mainEntry = hv_iternext(mainStash)) != NULL) {
        keyname = HePV(mainEntry, len);
        if (keyname != NULL) {
            if ((mainCV = get_cv(keyname, 0)) != NULL) {
                //It's a subroutine
                //We're going to take advantage of the fact that user names shouldn't be all
                //upper case to filter out some system vars
                //Better way to do this?
                subName = keyname;
                subName.MakeUpper();
                if (subName.Cmp((const char *)keyname) != 0) {
                    //It's a user sub
			        // Create the new function entry
                    subName = keyname;
			        arr.Append(new wxScriptFunctionPerl(subName));
                }
            }
        }
    }
}


wxString wxPerl::GetVersionInfo() const
{
    wxString Perl_Ver;
    
    SV* versionSV = get_sv("]", 0);
    if (versionSV) {
        STRLEN len;
        Perl_Ver = SvPV(versionSV, len);
    }
    else {
        //The above case won't work until after parse and run
        Perl_Ver = "5.008000";  //We do things that require Perl >= 5.8
    }
    wxLogDebug(wxT("wxPerlGetVersionInfo: version = %s"), Perl_Ver);
    return Perl_Ver;
}


// ---------------------
// wxSCRIPTFUNCTIONPERL
// ---------------------

bool wxScriptFunctionPerl::Exec(wxScriptVar &ret, wxScriptVar *arg) const
{
    int count;
    int argVal;
    void *argValPtr;
    double argValDbl;
    int nRetVal;

    {
        dSP ;
        ENTER ;
        SAVETMPS;

        //Push values from passed arg array
        PUSHMARK(SP) ;

        int n = 0;
	    while (arg[n].GetType().isValid()) {
		    wxScriptTypeInfo t = arg[n].GetType();

		    // a bool ?
            if (t.Match(wxScriptTypeBOOL)) {
                argVal = arg[n].GetContentLong();
			    XPUSHs(sv_2mortal(newSViv(argVal)));
            }
		    // a long, char, or int ?
		    else if (t.Match(wxScriptTypeINT) ||
				    t.Match(wxScriptTypeLONG) ||
                    t.Match(wxScriptTypeCHAR)) {
                argVal = arg[n].GetContentLong();
			    XPUSHs(sv_2mortal(newSViv(argVal)));
            }
		    // a float or a double ?
		    else if (t.Match(wxScriptTypeFLOAT) ||
                t.Match(wxScriptTypeDOUBLE)) {
                argValDbl = arg[n].GetContentDouble();
			    XPUSHs(sv_2mortal(newSVnv(argValDbl)));
            }
		    // a pointer ?
		    else if (t.isPointer()) {
			    // to chars ?
			    wxScriptTypeInfo pt = t.GetPointerType();
                if (pt.Match(wxScriptTypeCHAR)) {
                    argValPtr = arg[n].GetPointer();
                    XPUSHs(sv_2mortal(newSVpv((const char *)argValPtr, 0)));
                }
			    // to something else ?
                else {
                    argValPtr = arg[n].GetPointer();
                    XPUSHs(sv_2mortal(newSVpv((const char *)argValPtr, 0)));
                }
		    }
		    // We've got a type we don't understand
		    else 
			    return FALSE;

		    n++;		// next argument
	    }

        PUTBACK ;

        wxString funcName = GetName();
        count = call_pv(funcName, G_SCALAR | G_EVAL);

        //need to refresh SP after call_
        SPAGAIN ;

        /* Check the eval first */
        if (SvTRUE(ERRSV))
        {
            STRLEN n_a;
            wxLogDebug(wxT("wxScriptFunctionPerlExec: eval error: %s"), SvPV(ERRSV,n_a));
            POPs ;
        }
        else
        {
            if (count != 1) {
                wxLogDebug(wxT("wxScriptFunctionPerlExec: eval didn't return a value"));
            }
            else {
                //Determine the return type and set the wxScriptVar ret accordingly 
                SV *retSVp;
                svtype retType;
                retSVp = POPs;
                retType = (svtype)SvTYPE(retSVp);
                if ((retType == SVt_IV) || (retType == SVt_PVIV)) {
       			    ret.SetType(wxT("long"));
			        ret.SetContent((long)SvIV(retSVp));
                }
                else if ((retType == SVt_NV) || (retType == SVt_PVNV)) {
       			    ret.SetType(wxT("double"));
			        ret.SetContent((double)SvNV(retSVp));
                }
                else if (retType == SVt_PV) {
                    STRLEN len;
       			    ret.SetType(wxT("char*"));
			        ret.SetContent((char *)SvPV(retSVp, len));
                }
                else {
                    //This is bad - not an expected type
                }

            }
        }

        PUTBACK ;

        FREETMPS ;
        LEAVE ;
    }

    return TRUE;
}




// --------------
// wxSCRIPTFILEPERL
// --------------

bool wxScriptFilePerl::Load(const wxString &file)
{
    int exitstatus = 0;

    // remember the script filename
	m_strFileName = file;

	// load it
    char *embedding[2];
    embedding[0] = "";
    embedding[1] = (char *)m_strFileName.c_str();
    exitstatus = perl_parse(my_perl, xs_init, 2, embedding, (char **)NULL);
    PL_exit_flags |= PERL_EXIT_DESTRUCT_END;
    if (!exitstatus) {
        exitstatus = perl_run(my_perl);
    }
	
    return (exitstatus == 0);
}

#endif		// wxSCRIPT_USE_PERL


