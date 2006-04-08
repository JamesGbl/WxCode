/////////////////////////////////////////////////////////////////////////////
// Name:        MyPicsApp.cpp
// Purpose:     
// Author:      Ernesto Rangel Dallet
// Modified by: 
// Created:     12/04/05 19:21:11
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "MyPicsApp.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "MyPicsApp.h"
#include "DlgLang.h"
#include "MyFrame.h"

IMPLEMENT_APP( MyPicsApp )

IMPLEMENT_CLASS( MyPicsApp, wxApp )


BEGIN_EVENT_TABLE( MyPicsApp, wxApp )

END_EVENT_TABLE()


// initialize m_Resources, here we pass one of the "common" temp
// places defined in the class(here the systems TEMP folder for the user),we could have 
// also specified an EXSISTING custom directory (eg. C:\mydir), and a unique temp 
// sub-directory would have been automatically created & deleted inside it. 
// if you plan on ALLWAYS passing a custom extract location to EVERY
// ExtractXXX call, then you could specify BEXD_NO_CREATE to keep the
// class from creating a temp directory (of course then you would have to
// delete each extracted resource file yourself)
MyPicsApp::MyPicsApp():m_Resources(wxMSWResources::BEXD_TEMP_DIR)
{

}

bool MyPicsApp::OnInit()
{    
    wxImage::AddHandler( new wxJPEGHandler );
    wxImage::AddHandler( new wxICOHandler );

    // extract the applications resources to files, so we can use them.
    // we could have just put all
    // the resources in the .zip file, but then how would i have 
    // demostrated the other ExtractXXX functions? :)

    // extract all resources of same type (in this case imagess 
    // pecified as type "image" in the .rc file),in one call, 
    // u can specify any type you want, but if you
    // use a number, make shure its an unsigned int OVER 255.
    // Also note that the exCount param passed here is optional.
    int exCount=0;
    bool resOk = m_Resources.ExtractResources(wxT("image"), &exCount);
    if(resOk)
    {
        wxLogDebug(wxT("%d files extracted to %s using ExtractResources()"),
        exCount, m_Resources.GetResourceDir());
    }

    // extract a single resource file "wxmswres.h" of type "txtfile"
    // note that the resource name sould be the same in the .rc file
    // as the desired extracted file name INCLUDING EXTENSION, same
    // goes for ExtractResources()
    resOk = resOk && m_Resources.ExtractResource(wxT("wxmswres.h"),
        wxT("txtfile"));

    if(resOk)
    {
        wxLogDebug(wxT("resource file wxmswres.h extracted to %s using ExtractResource()"),
            m_Resources.GetResourceDir());
    }

    // extract resources contained in a zip file, in this case, the .mo 
    // catalog files, compressed to reduce .exe size
    exCount=0;
    resOk = resOk && m_Resources.ExtractZipResources(wxT("langcats"),
        wxT("zipfile"), &exCount);
    if(resOk)
    {
        wxLogDebug(wxT("%d files extracted to %s using ExtractZipResources()"),
            exCount, m_Resources.GetResourceDir());
    }

    // if the ExtractXXX functions returned true, the resources were
    // extracted successfully, but still you can check if some
    // resource is actually there using this function
    if(m_Resources.RcExtracted(wxT("wx_es.mo")))
        wxLogDebug(wxT("guess what??, wx_ex.mo was extracted successfully"));

    if(!resOk)
    {
        // oops! something went wrong, we better quit here
        wxMessageBox(_("Failed to extract the app´s resources.\nTerminating app..."),
            _("Fatal Error"), wxOK | wxCENTRE | wxICON_ERROR);
        return false;
    }

    // ask user for application language
    DlgLang dlg(NULL);
    wxString langName = dlg.GetSelLanguage();

    // now set the locale & load the app & standar wxWidgets catalogs
    // (previously extracted), but only if selected language was spanish, 
    // since wxWidgets & strings in source code are in english
    
    // set lookup path to our resource dir first!! , or wxLocale
    // will NOT find the catalogs & fail!
    m_Locale.AddCatalogLookupPathPrefix(m_Resources.GetResourceDir());

    bool iInitOk = false; bool cInitOk = false;
    int langId = langName==_("Spanish") ? 
        wxLANGUAGE_SPANISH_MODERN : wxLANGUAGE_ENGLISH_US;

    iInitOk= m_Locale.Init(langId, wxLOCALE_CONV_ENCODING);
    if(!iInitOk)
        wxLogDebug(wxT("Failed to initialize locale!"));

    if(iInitOk && langId == wxLANGUAGE_SPANISH_MODERN)
    {
        cInitOk = m_Locale.AddCatalog(wxT("wx_es"));
        cInitOk = cInitOk && m_Locale.AddCatalog(wxT("mypics_es"));
    }
    if(!cInitOk)
        wxLogDebug(wxT("Failed to load one or more catalogs!"));

    // create the app´s main window (go look at MyFrame´s creation code)
    MyFrame* pFrame = new MyFrame(NULL);
    pFrame->Maximize();
    pFrame->Show();
    SetTopWindow(pFrame);
    return true;
}

int MyPicsApp::OnExit()
{    
    return wxApp::OnExit();
}

