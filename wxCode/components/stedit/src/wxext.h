///////////////////////////////////////////////////////////////////////////////
// File:        wxext.h
// Purpose:     wxWidgets extensions
// Author:      Troels K
// Created:     2009-11-11
// RCS-ID:
// Copyright:   (c) John Labenski, Troels K
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXEXT_H__
#define __WXEXT_H__

#define WXK_HELP       WXK_F1
#define WXK_FULLSCREEN WXK_F11

#include <wx/stedit/stedefs.h>

#if (wxVERSION_NUMBER < 2812)
   #define wxT_2 wxT
#endif

class WXDLLIMPEXP_FWD_CORE wxMenuBar;

WX_DEFINE_ARRAY_INT(enum wxLanguage, LanguageArray);
WX_DECLARE_OBJARRAY_WITH_DECL(wxAcceleratorEntry, AcceleratorArray, class WXDLLIMPEXP_STEDIT);
WX_DECLARE_OBJARRAY_WITH_DECL(wxFileName, FileNameArray, class WXDLLIMPEXP_STEDIT);

WXDLLIMPEXP_STEDIT bool wxLocale_Init(wxLocale*, const wxString& exetitle, enum wxLanguage lang = wxLANGUAGE_DEFAULT);
WXDLLIMPEXP_STEDIT bool wxLocale_GetSupportedLanguages(LanguageArray*);
WXDLLIMPEXP_STEDIT bool wxLocale_SingleChoice(const LanguageArray&, enum wxLanguage*);
WXDLLIMPEXP_STEDIT bool wxLocale_Find(const wxString&, enum wxLanguage*);

WXDLLIMPEXP_STEDIT void wxFrame_SetInitialPosition(wxFrame*,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize, int margin_pct = 5);
WXDLLIMPEXP_STEDIT void wxFrame_ClonePosition(wxFrame* wnd, wxWindow* other = NULL);

WXDLLIMPEXP_STEDIT void wxCommandLineUsage(wxWindow* parent);

WXDLLIMPEXP_STEDIT wxAcceleratorEntry wxGetStockAcceleratorEx(wxWindowID);

WXDLLIMPEXP_STEDIT void wxSetAcceleratorTable(wxWindow*, const AcceleratorArray&);
WXDLLIMPEXP_STEDIT void wxMenu_SetAccelText(wxMenuBar*, const AcceleratorArray&);
WXDLLIMPEXP_STEDIT void wxMenu_SetAccelText(wxMenu*   , const AcceleratorArray&);
WXDLLIMPEXP_STEDIT wxString wxToolBarTool_MakeShortHelp(const wxString&, const AcceleratorArray& accel, int id);

#if (wxVERSION_NUMBER >= 2900)
#define wxMessageBoxCaption wxTheApp->GetAppDisplayName()
#else
#define wxMessageBoxCaption wxTheApp->GetAppName()
#endif

#ifdef _WX_STOCKITEM_H_
#if (wxVERSION_NUMBER < 2901)
#define wxSTOCK_WITHOUT_ELLIPSIS 4
#define wxSTOCK_FOR_BUTTON (wxSTOCK_WITH_MNEMONIC | wxSTOCK_WITHOUT_ELLIPSIS)
#endif
#define wxSTOCK_PLAINTEXT wxSTOCK_WITHOUT_ELLIPSIS
WXDLLIMPEXP_STEDIT wxString wxGetStockLabelEx(wxWindowID, long flags = wxSTOCK_WITH_MNEMONIC);
#endif

/////////////////////////////////////////////////////////////////////////////
// wxPreviewFrameEx

#ifdef _WX_PRNTBASEH__
class WXDLLIMPEXP_STEDIT wxPreviewFrameEx : public wxPreviewFrame
{
   typedef wxPreviewFrame base;
public:
   wxPreviewFrameEx(wxPrintPreviewBase* preview,
                   wxWindow *parent,
                   const wxString& title,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = wxDEFAULT_FRAME_STYLE,
                   const wxString& name = wxT("frame"));
   virtual bool Destroy();
protected:
#if (wxVERSION_NUMBER < 2900)
   void OnKeyDown(wxKeyEvent&);
#endif
   DECLARE_EVENT_TABLE()
};
#endif

#if defined(_WX_ABOUTDLG_H_) && (wxVERSION_NUMBER < 2900)
inline void wxAboutBox(const wxAboutDialogInfo& info, wxWindow* WXUNUSED(parent))
{
   wxAboutBox(info);
}
#endif

class WXDLLIMPEXP_STEDIT wxClipboardHelper
{
public:
    enum Type
    {
        Default = 1, // use the normal clipboard
        Primary = 2, // use the primary clipboard
        Both    = 3  // use both clipboards (only valid for set functions)
    };

    // Is text available in the single specified clipboard in any usable text format.
    // Formats tested are wxDF_TEXT and if avilable wxDF_UNICODETEXT and wxDF_HTML.
    static bool IsTextAvailable(Type clip_type = Default);

    // Returns true if there is data in the single specified clipboard with the given formats.
    // This function takes an array since the clipboard has to be opened to test formats.
    static bool IsFormatAvailable(const enum wxDataFormatId* array, size_t array_count, Type clip_type = Default);

    // Get the current text in the single specified clipboard into the buf.
    // Returns true if the clipboard was opened and the buf is not empty.
    static bool GetText(wxString* buf, Type clip_type = Default);
    
    // Set the text to the specified clipboard(s).
    static bool SetText(const wxString& str, Type clip_type = Default);
    
    // Set the HTML text to the clipboard. In MSW the clipboard will contain
    // a valid HTML data object and a text object, on other systems the
    // clipboard only contains a text object.
    static bool SetHtmlText(const wxString& htmldata);
};

#endif // __WXEXT_H__
