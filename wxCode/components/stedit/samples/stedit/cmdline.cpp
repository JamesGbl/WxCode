///////////////////////////////////////////////////////////////////////////////
// Name:        cmdline.cpp
// Purpose:     Simple wxSTEditor app
// Author:      John Labenski, Troels K
// Modified by:
// Created:     10/03/2010
// RCS-ID:
// Copyright:   (c) John Labenski
// Licence:     wxWidgets licence

// For compilers that support precompilation, includes <wx/wx.h".
#include <wx/wxprec.h>

#include <wx/cmdline.h>
#include <wx/fileconf.h>
#include <wx/msgdlg.h>
#include <wx/stedit/stedit.h>
#include "../../src/wxext.h"

#include "app.h"

// ----------------------------------------------------------------------------
// wxCmdLineParser functions
// ----------------------------------------------------------------------------

CommandLine::CommandLine()
{
    m_lang = wxLANGUAGE_DEFAULT;
    m_recurse = false;
}

enum parm
{
   enum_parm_single,
   enum_parm_recurse,
   enum_parm_config,
   enum_parm_filenames,
   enum_parm_lang,
   enum_parm_langdialog,
   enum_parm_enumcount
};

static const wxCmdLineEntryDesc cmdLineDesc[] =
{
    { wxCMD_LINE_SWITCH, wxT_2("1"), wxT_2("single"), _("single file mode"),
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

    { wxCMD_LINE_SWITCH, wxT_2("r"), wxT_2("recurse"), _("open the given filespecs recursively, quote values \"*.txt\""),
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

    { wxCMD_LINE_OPTION, wxT_2("c"), wxT_2("config"), _("use config file"),
        wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL|wxCMD_LINE_NEEDS_SEPARATOR },

    { wxCMD_LINE_PARAM,  wxT_2(""),  wxT_2(""),       _("input filename(s)"),
        wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL|wxCMD_LINE_PARAM_MULTIPLE },

    { wxCMD_LINE_OPTION, wxT_2("l"), wxT_2("lang"), _("Specify language"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
    { wxCMD_LINE_SWITCH, NULL, wxT_2("langdialog"), _("Language dialog"), wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

    { wxCMD_LINE_NONE, NULL, NULL, NULL, wxCMD_LINE_VAL_NONE, 0 },
};
#define C_ASSERT_(n,e) typedef char __C_ASSERT__##n[(e)?1:-1]
C_ASSERT_(1,WXSIZEOF(cmdLineDesc) == (enum_parm_enumcount + 1));

void wxStEditApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.SetDesc(cmdLineDesc);
    wxApp::OnInitCmdLine(parser);
}

bool wxStEditApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    // use single page, else use a notebook of editors
    if (parser.Found(cmdLineDesc[enum_parm_single].longName))
    {
        m_steOptions.SetFrameOption(STF_CREATE_NOTEBOOK, false);
        m_steOptions.SetFrameOption(STF_CREATE_SINGLEPAGE, true);
        m_steOptions.GetMenuManager()->CreateForSinglePage();
    }
    else
    {
        m_steOptions.SetFrameOption(STF_CREATE_SIDEBAR, true);
        m_steOptions.GetMenuManager()->CreateForNotebook();
    }

    // use specified config file to load saved prefs, styles, langs
    wxString configFile;
    if (parser.Found(cmdLineDesc[enum_parm_config].longName, &configFile))
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
            m_steOptions.GetMenuManager()->SetMenuItemType(STE_MENU_PREFS_MENU, STE_MENU_PREFS_SAVE, false);
        }
    }
    else
    {
        // Always use a wxFileConfig since I don't care for registry entries.
        wxFileConfig *config = new wxFileConfig(STE_APPDISPLAYNAME, wxT("wxWidgets"));
        wxConfigBase::Set((wxConfigBase*)config);
    }

    // they want to open the files recursively
    if (parser.Found(cmdLineDesc[enum_parm_recurse].longName))
        m_cmdline.m_recurse = true;

    // gather up all the filenames to load
    size_t n, count = parser.GetParamCount();
    for (n = 0; n < count; n++)
        m_cmdline.m_fileNames.Add(parser.GetParam(n));

    wxString str;
    if (parser.Found(cmdLineDesc[enum_parm_lang].longName, &str))
    {
        if (str.Length()) wxLocale_Find(str, &m_cmdline.m_lang);
    }

    if (parser.Found(cmdLineDesc[enum_parm_langdialog].longName))
    {
        LanguageArray array;
        wxLocale_GetSupportedLanguages(&array);
        wxLocale_SingleChoice(array, &m_cmdline.m_lang);
    }

    return wxApp::OnCmdLineParsed(parser);
}
