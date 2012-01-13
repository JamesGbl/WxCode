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

#include "wx/stedit/stedefs.h" // WXDLLIMPEXP_STEDIT

WX_DECLARE_OBJARRAY_WITH_DECL(wxFileName, FileNameArray, class WXDLLIMPEXP_STEDIT);

#ifdef _WX_INTL_H_
WX_DEFINE_ARRAY_INT(enum wxLanguage, LanguageArray);
class WXDLLIMPEXP_STEDIT wxLocaleHelper
{
public:

    static bool Init(wxLocale*, const wxString& exetitle, enum wxLanguage lang = wxLANGUAGE_DEFAULT);

    static bool GetSupportedLanguages(LanguageArray*);

    static bool SingleChoice(const LanguageArray&, enum wxLanguage*);

    static bool Find(const wxString&, enum wxLanguage*);
};
#endif

WXDLLIMPEXP_STEDIT void wxFrame_SetInitialPosition(wxFrame*,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize, int margin_pct = 5);
WXDLLIMPEXP_STEDIT void wxFrame_ClonePosition(wxFrame* wnd, wxWindow* other = NULL);

WXDLLIMPEXP_STEDIT void wxCommandLineUsage(wxWindow* parent);

#if wxUSE_ACCEL
class WXDLLIMPEXP_FWD_CORE wxMenuBar;
WX_DECLARE_OBJARRAY_WITH_DECL(wxAcceleratorEntry, AcceleratorArray, class WXDLLIMPEXP_STEDIT);
class WXDLLIMPEXP_STEDIT wxAcceleratorHelper
{
public:

    static wxAcceleratorEntry GetStockAccelerator(wxWindowID);

    static void SetAcceleratorTable(wxWindow*, const AcceleratorArray&);

    static void SetAccelText(wxMenuBar*, const AcceleratorArray&);

    static void SetAccelText(wxMenu*, const AcceleratorArray&);
};
WXDLLIMPEXP_STEDIT wxString wxToolBarTool_MakeShortHelp(const wxString&, const AcceleratorArray&, int id);
#endif

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

    static bool Set(wxDataObject* def, wxDataObject* primary = NULL);
};

#ifdef __WXMSW__
// Strange that wxMBConv classes work with char and wchar_t only, not with wxChar;
// this surely makes for unnecessary extra conversions
class WXDLLIMPEXP_STEDIT wxMBConvOEM : public wxMBConv
{
public:
    wxMBConvOEM();
    virtual ~wxMBConvOEM();

    virtual size_t ToWChar  (wchar_t*    dst, size_t dstLen,
                             const char* src, size_t srcLen = wxNO_LEN) const;
    virtual size_t FromWChar(char*          dst, size_t dstLen,
                             const wchar_t* src, size_t srcLen = wxNO_LEN) const;

    virtual wxMBConv* Clone() const { return new wxMBConvOEM(); }
};
#endif

class WXDLLIMPEXP_STEDIT wxTextEncoding
{
public:
    enum Type
    {
        UTF8,
        Unicode_LE,
        ISO8859_1,
    #ifdef __WXMSW__
        OEM,
    #endif
        EnumCount,
        None = wxNOT_FOUND
    };

#if (defined(__WXTRUNK_H__) || (wxVERSION_NUMBER >= 2903) ) && defined(_WX_CONVAUTO_H_) // wxBOM enum is in wx/convauto.h
    // char -> wxString method. Specify encoding.
    static bool CharToString(wxString*, const wxCharBuffer& buf, size_t buf_len = wxNO_LEN, Type encoding = None, wxBOM bom = wxBOM_None);

    // char -> wxString method. Utilizing wxConvAuto::DetectBOM.
    static bool CharToStringDetectBOM(wxString*, const wxCharBuffer& buf, size_t buf_len = wxNO_LEN, wxBOM* file_bom = NULL);
#endif

    // char -> wxString method. Specify wxMBConv conversion class
    static bool CharToString(wxString*, const char*, const wxMBConv&, size_t len);

    // wxString -> char method. Specify wxMBConv conversion class
    static wxCharBuffer StringToChar(const wxString&, const wxMBConv&);

    // wxString -> char method. Specify Type conversion; creates wxMBConv instance and calls StringToChar(wxMBConv) above
    static wxCharBuffer StringToChar(const wxString&, Type, size_t* size_ptr);

    // enum -> string representation (eg Type::UTF8 -> "utf-8")
    static wxString TypeToString(Type);

    // String representation -> enum (eg "utf-8" -> Type::UTF8)
    static Type TypeFromString(const wxString&);

    // Search the provided string for encoding specification (eg "utf-8")
    static bool TypeFromString(Type*, const char* str, const char* identifier, const char* strpbrk_ctrl);

#ifdef _WX_XML_H_
    inline static Type TypeFromString(const wxXmlDocument& xml)
    {
        return TypeFromString(xml.GetFileEncoding());
    }
#endif

    static const char* GetBOMChars(Type, size_t* count);

    static bool SaveFile(const wxString&, wxOutputStream&, Type encoding = None, bool file_bom = false);
};

#endif // __WXEXT_H__
