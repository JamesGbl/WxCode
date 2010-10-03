///////////////////////////////////////////////////////////////////////////////
// Name:        cmdline.cpp
// Purpose:     Simple wxSTEditor app
// Author:      John Labenski, Troels K
// Modified by:
// Created:     10/03/2010
// RCS-ID:      
// Copyright:   (c) John Labenski
// Licence:     wxWidgets licence

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#include "wx/cmdline.h"
#include "wx/fileconf.h"
#include "wx/stedit/stedit.h"

#include "app.h"

// ----------------------------------------------------------------------------
// wxCmdLineParser functions
// ----------------------------------------------------------------------------

#if (wxVERSION_NUMBER >= 2900)
   #define WXT(a) a
#else
   #define WXT wxT
#endif

static const wxCmdLineEntryDesc cmdLineDesc[] =
{
    { wxCMD_LINE_SWITCH, WXT("h"), WXT("help"),   _("help on command line switches"),
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL|wxCMD_LINE_OPTION_HELP },

    { wxCMD_LINE_SWITCH, WXT("1"), WXT("single"), _("single file mode"),
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

    { wxCMD_LINE_SWITCH, WXT("r"), WXT("recurse"), _("open the given filespecs recursively, quote values \"*.txt\""),
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

    { wxCMD_LINE_OPTION, WXT("c"), WXT("config"), _("use config file"),
        wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL|wxCMD_LINE_NEEDS_SEPARATOR },

    { wxCMD_LINE_PARAM,  WXT(""),  WXT(""),       _("input filenames(s)"),
        wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL|wxCMD_LINE_PARAM_MULTIPLE },

    { wxCMD_LINE_NONE }
};

bool wxStEditApp::ParseCmdLine(wxArrayString* fileNames, wxSTEditorOptions* steOptions, bool* recurse) // TODO: remove and use OnInitCmdLine + OnCmdLineParsed instead
{
    wxCmdLineParser parser(cmdLineDesc, argc, argv);

/*
    // test code for looking at the args passed in
    for (int k = 0; k < argc; k++)
    {
        wxArrayString a = parser.ConvertStringToArgs(argv[k]);
        for (int n=0; n < a.GetCount(); n++)
        {
            wxPrintf(wxT("Arg %d #%d '%s'\n"), k, n, a[n].wx_str()); fflush(stdout);
        }
    }
*/

    switch ( parser.Parse() )
    {
        case -1 :
        {
            // help should be given by the wxCmdLineParser, exit program
            return false;
        }
        case 0:
        {
            // use single page, else use a notebook of editors
            if (parser.Found(wxT("1")))
            {
                steOptions->SetFrameOption(STF_CREATE_NOTEBOOK, false);
                steOptions->SetFrameOption(STF_CREATE_SINGLEPAGE, true);
                steOptions->GetMenuManager()->CreateForSinglePage();
            }
            else
            {
                steOptions->SetFrameOption(STF_CREATE_SIDEBAR, true);
                steOptions->GetMenuManager()->CreateForNotebook();
            }

            // use specified config file to load saved prefs, styles, langs
            wxString configFile;
            if (parser.Found(wxT("c"), &configFile))
            {
                wxFileName fN(configFile);

                if ( !fN.FileExists() )
                {
                    //wxLogMessage(wxT("Config file '")+configFile+wxT("' does not exist."));
                    if (configFile.IsEmpty() || !fN.IsOk() || wxIsWild(configFile))
                    {
                        int ret = wxMessageBox(wxString::Format(_("Config file '%s' has an invalid name.\nContinue without using a config file?"), configFile.wx_str()),
                                               _("Invalid config file name"),
                                               wxICON_QUESTION|wxYES_NO);
                        if (ret == wxNO)
                            return false;

                        configFile = wxEmptyString;
                    }
                    else // file doesn't exist, ask if they want to create a new one
                    {
                        int ret = wxMessageBox(wxString::Format(_("Config file '%s' does not exist.\nWould you like to create a new one?"), configFile.wx_str()),
                                               _("Invalid config file"),
                                               wxICON_QUESTION|wxYES_NO|wxCANCEL);
                        if (ret == wxCANCEL)
                            return false;
                        else if (ret == wxNO)
                            configFile = wxEmptyString;
                    }
                }

                // use the specified config file, if it's still set
                if ( configFile.Length() )
                {
                    wxFileConfig *config = new wxFileConfig(STE_APPDISPLAYNAME, wxT("wxWidgets"),
                                                            configFile, wxEmptyString,
                                                            wxCONFIG_USE_RELATIVE_PATH);
                    wxConfigBase::Set((wxConfigBase*)config);
                }
                else // don't use any config file at all, disable saving
                {
                    steOptions->GetMenuManager()->SetMenuItemType(STE_MENU_PREFS_MENU, STE_MENU_PREFS_SAVE, false);
                }
            }
            else
            {
                // Always use a wxFileConfig since I don't care for registry entries.
                wxFileConfig *config = new wxFileConfig(STE_APPDISPLAYNAME, wxT("wxWidgets"));
                wxConfigBase::Set((wxConfigBase*)config);
            }

            // they want to open the files recursively
            if (parser.Found(wxT("r")))
                *recurse = true;

            // gather up all the filenames to load
            size_t n, count = parser.GetParamCount();
            for (n = 0; n < count; n++)
                fileNames->Add(parser.GetParam(n));

            break;
        }
        default:
        {
            wxLogMessage(wxT("Unknown command line option, aborting."));
            return false;
        }
    }
    return true;
}

void wxStEditApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    wxApp::OnInitCmdLine(parser);
}

bool wxStEditApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    return wxApp::OnCmdLineParsed(parser);
}

