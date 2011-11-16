///////////////////////////////////////////////////////////////////////////////
// File:        wxext.cpp
// Purpose:     wxWidgets extensions
// Author:      Troels K
// Created:     2009-11-11
// RCS-ID:
// Copyright:   (c) John Labenski, Troels K
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include <wx/dir.h>
#include <wx/stdpaths.h>
#include <wx/cmdline.h>
#include <wx/clipbrd.h>
#include <wx/convauto.h>

#include <wx/stedit/stedefs.h>

#include "wxtrunk.h"
#include "wxext.h"

#ifndef WXPRECOMP
    #include <wx/settings.h>
    #include <wx/app.h>
    #include <wx/choicdlg.h>
#endif // WXPRECOMP

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(AcceleratorArray);
WX_DEFINE_OBJARRAY(FileNameArray);

bool wxGetExeFolder(wxFileName* filename)
{
    wxFileName temp;

    temp.Assign(wxStandardPaths::Get().GetExecutablePath());
    temp.SetFullName(wxEmptyString);
    
    bool ok = temp.IsOk();

    if (ok && filename)
    {
        *filename = temp;
    }
    return ok;
}

/*static*/
bool wxLocaleHelper::Init(wxLocale* locale, const wxString& exetitle, enum wxLanguage lang)
{
   wxFileName filename;

   wxGetExeFolder(&filename);
   filename.AppendDir(wxT("locale"));
   wxLocale::AddCatalogLookupPathPrefix(filename.GetFullPath());

   bool ok = locale->Init(lang);
   if (ok)
   {
      locale->AddCatalog(exetitle, (enum wxLanguage)locale->GetLanguage(), wxEmptyString);
   }
   return ok;
}

/*static*/
bool wxLocaleHelper::Find(const wxString& str, enum wxLanguage* lang)
{
   const size_t len = str.Length();
   
   for (int i = wxLANGUAGE_UNKNOWN + 1; i < wxLANGUAGE_USER_DEFINED; i++)
   {
      const wxLanguageInfo* info = wxLocale::GetLanguageInfo(i);
      
      if (   info
          && (   (0 == str.CmpNoCase(info->CanonicalName))
              || (0 == str.CmpNoCase(info->CanonicalName.Left(len)))
             )
          )
      {
         *lang = (enum wxLanguage)i;
         return true;
      }
   }
   return false;
}

/*static*/
bool wxLocaleHelper::GetSupportedLanguages(LanguageArray* array)
{
   wxFileName filename;
   wxDir dir;

   wxGetExeFolder(&filename);
   filename.AppendDir(wxT("locale"));

   bool ok = dir.Open(filename.GetFullPath());

   if (ok)
   {
      const enum wxLanguage default_lang = wxLANGUAGE_ENGLISH;
      wxString str;
      
      array->Add(default_lang);
      for (bool cont = dir.GetFirst(&str, wxEmptyString, wxDIR_DIRS);
           cont;
           cont = dir.GetNext(&str))
      {
         enum wxLanguage lang;

         if (   wxLocaleHelper::Find(str, &lang)
             && (lang != default_lang))
         {
            array->Add(lang);
         }
      }
   }
   return ok;
}

/*static*/
bool wxLocaleHelper::SingleChoice(const LanguageArray& array, enum wxLanguage* lang)
{
   wxArrayString as;

   for (size_t i = 0; i < array.GetCount(); i++)
   {
      enum wxLanguage temp = (enum wxLanguage)array.Item(i);
      as.Add(wxLocale::GetLanguageName(temp));
   }
   int index = wxGetSingleChoiceIndex(wxT("Language"), wxMessageBoxCaption, as);
   bool ok = (index != wxNOT_FOUND);
   
   if (ok)
   {
      *lang = (enum wxLanguage)array.Item(index);
   }
   return ok;
}

#if wxUSE_ACCEL

/*static*/
wxAcceleratorEntry wxAcceleratorHelper::GetStockAccelerator(wxWindowID id)
{
    wxAcceleratorEntry ret;

    #define STOCKITEM(stockid, flags, keycode)      \
        case stockid:                               \
            ret.Set(flags, keycode, stockid);       \
            break;

    switch (id)
    {
    #if (wxVERSION_NUMBER < 2903)
        STOCKITEM(wxID_PRINT,                wxACCEL_CTRL,'P')
        STOCKITEM(wxID_UNDO,                 wxACCEL_CTRL,'Z')
    #endif
        STOCKITEM(wxID_PREVIEW,              wxACCEL_CTRL | wxACCEL_SHIFT,'P')
        STOCKITEM(wxID_SAVEAS,               wxACCEL_CTRL | wxACCEL_SHIFT,'S')
        STOCKITEM(wxID_SELECTALL,            wxACCEL_CTRL,'A')
        STOCKITEM(wxID_REDO,                 wxACCEL_CTRL,'Y')
        //STOCKITEM(wxID_PREFERENCES,          wxACCEL_CTRL,'T')
        STOCKITEM(wxID_ICONIZE_FRAME,        wxACCEL_ALT,WXK_FULLSCREEN)
        STOCKITEM(wxID_REFRESH,              wxACCEL_NORMAL, WXK_F5)
        STOCKITEM(wxID_PROPERTIES,           wxACCEL_ALT,WXK_RETURN)
        STOCKITEM(wxID_BACKWARD,             wxACCEL_ALT , WXK_LEFT)
        STOCKITEM(wxID_FORWARD,              wxACCEL_ALT , WXK_RIGHT)
        STOCKITEM(wxID_HELP,                 wxACCEL_NORMAL, WXK_HELP)
        STOCKITEM(wxID_REPLACE,              wxACCEL_CTRL,'H')
        STOCKITEM(wxID_EXIT,                 wxACCEL_CTRL, 'Q')
        STOCKITEM(wxID_CLOSE,                wxACCEL_CTRL, 'W')
        STOCKITEM(wxID_CLEAR,                wxACCEL_NORMAL, WXK_DELETE)
        STOCKITEM(wxID_ABOUT,                wxACCEL_SHIFT, WXK_HELP)
        default:
            ret = wxGetStockAccelerator(id);
            break;
    }

    #undef STOCKITEM

#if (wxVERSION_NUMBER >= 2902)
    wxASSERT(ret.IsOk());
#else
    // trac.wxwidgets.org/ticket/12444
    // trac.wxwidgets.org/ticket/12445
#endif
    return ret;
}

/*static*/
void wxAcceleratorHelper::SetAcceleratorTable(wxWindow* wnd, const AcceleratorArray& array)
{
   const size_t count = array.GetCount();
   wxAcceleratorEntry* temp = new wxAcceleratorEntry[count];

   for (size_t i = 0; i < count; i++)
   {
      temp[i] = array.Item(i);
   }
   wxAcceleratorTable accel((int)count, temp);
   wnd->SetAcceleratorTable(accel);
   delete [] temp;
}

#endif // wxUSE_ACCEL

static wxString wxMenuItem_GetText(const wxMenuItem* item)
{
   wxString str = item->GetItemLabel();

#ifdef __WXGTK__
   str.Replace(wxString(wxT('_')), wxString(wxT('&')));
#endif
   return str;
}

#ifdef __WXMSW__
    #define ACCELSTR_SEP "   "
#endif

static bool wxMenuItem_SetAccelText(wxMenuItem* item, const wxString& accel, bool append = true)
{
   wxString str = wxMenuItem_GetText(item);
   wxString ch_sep = wxT("\t");
   const int sep = str.Find(ch_sep);

   if (wxNOT_FOUND == sep)
   {
   }
   else if (append)
   {
#ifdef __WXMSW__
    ch_sep = wxT(ACCELSTR_SEP);
#else
    // Having multiple accelerators per menu item in GTK yields these warnings in the console window,
    // "Unknown accel modifier: 'w   ctrl'...No accel key found, accel string ignored."
    return false;
#endif
   }
   else
   {
      str.Truncate(sep);
   }
   item->SetItemLabel(wxString::Format(wxT("%s%s%s"),
      str.wx_str(),
      ch_sep.wx_str(),
      accel.wx_str()));
   return true;
}

static wxString wxGetAccelText(int flags, int keyCode)
{
   // wxAcceleratorEntry.ToString() produces silly text
   wxString str;
#if (wxVERSION_NUMBER >= 2901)
   const wxChar sep = '+'; // the new wx default
#else
   const wxChar sep = '-'; // the old annoying wx default
#endif
#if 0 && (wxVERSION_NUMBER >= 2800)
   wxAcceleratorEntry entry(flags, keyCode);
   str = entry.ToString(); // doesn't work (wxIsalnum(WXK_F2)), silly text (WXK_NUMPAD_ADD)
#else

   if (flags & wxACCEL_CTRL)
   {
      if (!str.IsEmpty()) str+=sep;
      str+=_("Ctrl");
   }
   if (flags & wxACCEL_ALT)
   {
      if (!str.IsEmpty()) str+=sep;
      str+=_("Alt");
   }
   if (flags & wxACCEL_SHIFT)
   {
      if (!str.IsEmpty()) str+=sep;
      str+=_("Shift");
   }
   if (!str.IsEmpty()) str+=sep;
   switch (keyCode)
   {
      case WXK_INSERT         : str+=_("Insert" ); break;
      case WXK_PAGEUP         : str+=_("PgUp"   ); break;
      case WXK_PAGEDOWN       : str+=_("PgDn"   ); break;
      case WXK_HOME           : str+=_("Home"   ); break;
      case WXK_END            : str+=_("End"    ); break;
      case WXK_RETURN         : str+=_("Return" ); break;
      case WXK_DELETE         : str+=_("Del" ); break;
      case WXK_NUMPAD_ADD     : str+=_("Num+"   ); break;
      case WXK_NUMPAD_SUBTRACT: str+=_("Num-"   ); break;
      default:
         if ( (keyCode >= WXK_F1) && (keyCode <= WXK_F24) )
         {
            str+=wxString::Format(wxT("F%d"), keyCode - WXK_F1 + 1);
         }
         else
         {
            wxASSERT(keyCode >= ' ');
            str+=(wxChar)keyCode;
         }
         break;
   }
#endif
   return str;
}

wxString wxGetStockLabelEx(wxWindowID id, long flags)
{
#define STOCKITEM(stockid, label) \
     case stockid:                 \
         stockLabel = label;       \
         break;

   wxString stockLabel;

   switch (id)
   {
      STOCKITEM(wxID_ABOUT,     _("&About..."))   // + ellipsis
      STOCKITEM(wxID_OPEN,      _("&Open..."))    // + ellipsis
      STOCKITEM(wxID_SELECTALL, _("Select &All")) // + ampersand
      STOCKITEM(wxID_FIND,      _("&Find..."))    // + ellipsis
      STOCKITEM(wxID_REPLACE,   _("Rep&lace...")) // + ellipsis
      STOCKITEM(wxID_REVERT,    _("Re&vert..."))
      STOCKITEM(wxID_PROPERTIES, _("Proper&ties...")) // + ellipsis
      default:
         break;
   }
#undef STOCKITEM
   if (!stockLabel.IsEmpty())
   {
       if ( !(flags & wxSTOCK_WITH_MNEMONIC) )
       {
           stockLabel = wxStripMenuCodes(stockLabel);
       }
#if (wxVERSION_NUMBER >= 2901)
       if (flags & wxSTOCK_WITHOUT_ELLIPSIS)
       {
           wxString baseLabel;
           if ( stockLabel.EndsWith(wxT("..."), &baseLabel) )
               stockLabel = baseLabel;
       }
#else
       // handled below
#endif
   }
   else
   {
      stockLabel = wxGetStockLabel(id, flags);
   }
#if (wxVERSION_NUMBER < 2901)
   if (flags & wxSTOCK_WITHOUT_ELLIPSIS)
   {
      wxString baseLabel;
      if ( stockLabel.EndsWith(wxT("..."), &baseLabel) )
         stockLabel = baseLabel;
   }
#endif
   return stockLabel;
}

static wxString wxGetAccelText(const wxAcceleratorEntry& accel)
{
   return wxGetAccelText(accel.GetFlags(), (enum wxKeyCode)accel.GetKeyCode());
}

/*static*/
void wxAcceleratorHelper::SetAccelText(wxMenuBar* menubar, const AcceleratorArray& accel)
{
   size_t count = menubar->GetMenuCount();

   for (size_t j = 0; j < count; j++)
   {
      wxMenu* menu = menubar->GetMenu(j);

      wxAcceleratorHelper::SetAccelText(menu, accel);
   }
}

wxString wxToolBarTool_MakeShortHelp(const wxString& rstr, const AcceleratorArray& accel, int id)
{
   wxString str = rstr;

   if (accel.GetCount() && !str.IsEmpty())
   {
      wxString strAccel;

      for (size_t i = 0; i < accel.GetCount(); i++)
      {
         const wxAcceleratorEntry& element = accel.Item(i);

         if (element.GetCommand() == id)
         {
            if (!strAccel.IsEmpty()) strAccel+=wxT(ACCELSTR_SEP);
            strAccel+=wxGetAccelText(element);
         }
      }
      if (!strAccel.IsEmpty())
      {
         str+=wxString::Format(wxT(" (%s)"), strAccel.wx_str());
      }
   }
   return str;
}

static bool wxMenuItem_SetAccelText(wxMenuItem* item, const wxAcceleratorEntry& entry)
{
   return wxMenuItem_SetAccelText(item, wxGetAccelText(entry));
}

static void wxMenu_SetAccelText(wxMenu* menu, const wxAcceleratorEntry& accel)
{
   wxMenuItemList& list = menu->GetMenuItems();

   for (wxMenuItemList::iterator it = list.begin();
        it != list.end();
        it++)
   {
      wxMenuItem* item = *it;

      if (item->IsSubMenu())
      {
         wxMenu_SetAccelText(item->GetSubMenu(), accel);
      }
      else if (item->GetId() == accel.GetCommand())
      {
         wxMenuItem_SetAccelText(item, accel);
      }
   }
}

/*static*/
void wxAcceleratorHelper::SetAccelText(wxMenu* menu, const AcceleratorArray& array)
{
   for (size_t i = 0; i < array.GetCount(); i++)
   {
      const wxAcceleratorEntry& accel = array.Item(i);

      wxMenu_SetAccelText(menu, accel);
   }
}

/////////////////////////////////////////////////////////////////////////////
// wxPreviewFrameEx

wxPreviewFrameEx::wxPreviewFrameEx(wxPrintPreviewBase* preview,
                wxWindow *parent,
                const wxString& title,
                const wxPoint& pos,
                const wxSize& size,
                long style,
                const wxString& name)
   : wxPreviewFrame(preview, parent, title, pos, size, style, name)
{
}

BEGIN_EVENT_TABLE(wxPreviewFrameEx, wxPreviewFrame)
#if (wxVERSION_NUMBER < 2900)
    EVT_CHAR_HOOK(wxPreviewFrameEx::OnKeyDown)
#endif
END_EVENT_TABLE()

bool wxPreviewFrameEx::Destroy()
{
   bool ok = base::Destroy();
   if (ok && GetParent())
   {
      GetParent()->Raise();
   }
   return ok;
}

#if (wxVERSION_NUMBER < 2900)
// trac.wxwidgets.org/ticket/8570
void wxPreviewFrameEx::OnKeyDown(wxKeyEvent& event)
{
   switch (event.GetKeyCode())
   {
      case WXK_ESCAPE:
         Close();
         break;
      default:
         event.Skip();
         break;
   }
}
#endif

void wxCommandLineUsage(wxWindow* parent)
{
    wxCmdLineParser parser;
    wxTheApp->OnInitCmdLine(parser);
#if (wxVERSION_NUMBER >= 2900)
    // GetUsageString() is public
    wxMessageBox(parser.GetUsageString(), wxTheApp->GetAppDisplayName(), wxOK | wxICON_INFORMATION, parent);
#elif defined(__WXMSW__)
    // GetUsageString() is private, sigh
    parser.Usage();
    wxUnusedVar(parent);
#else
   // Usage() goes to console, sigh
    wxUnusedVar(parent);
#endif
}

void wxFrame_SetInitialPosition(wxFrame* wnd, const wxPoint& pos, const wxSize& size, int margin_pct)
{
   if (size == wxDefaultSize)
   {
      wxRect rect = wxGetClientDisplayRect();
      wxSize size(
         (rect.width  * (100 - margin_pct*2))/100,
         (rect.height * (100 - margin_pct*2))/100);

      wnd->SetSize(size);
   }
   if (pos == wxDefaultPosition)
   {
      wnd->Center();
   }
}

void wxFrame_ClonePosition(wxFrame* wnd, wxWindow* otherwindow /*= NULL*/)
{
   otherwindow = otherwindow ? wxGetTopLevelParent(otherwindow) : wxTheApp->GetTopWindow();
   wxFrame* topframe = wxStaticCast(otherwindow, wxFrame);

   if (topframe->IsMaximized())
   {
      wnd->Maximize();
   }
   else if (topframe->IsFullScreen())
   {
      wnd->Maximize();
   }
   else
   {
      wxRect rect = topframe->GetScreenRect();

      wnd->SetSize(rect);
   }
}

#define HASBIT(value, bit)      (((value) & (bit)) != 0)

/*static*/
bool wxClipboardHelper::IsTextAvailable(Type clip_type)
{
    wxCHECK_MSG(clip_type != Both, false, wxT("Getting values from both clipboards is not supported"));

    bool ok = false;
#if wxUSE_CLIPBOARD
    const enum wxDataFormatId text[] =
    {
        wxDF_TEXT
      //,wxDF_OEMTEXT,     // This is wxDF_TEXT in MSW, not supported in GTK/OSX
#   if wxUSE_UNICODE
        ,wxDF_UNICODETEXT  // asserts in ANSI build
#   endif // wxUSE_UNICODE
#   ifdef __WXMSW__
        ,wxDF_HTML         // Only supported in MSW
#   endif // __WXMSW__
    };

    ok = IsFormatAvailable(text, WXSIZEOF(text), clip_type);
#endif // wxUSE_CLIPBOARD
    return ok;
}

/*static*/
bool wxClipboardHelper::IsFormatAvailable(const enum wxDataFormatId* array,
                                                     size_t array_count,
                                                     Type clip_type)
{
    wxCHECK_MSG(clip_type != Both, false, wxT("Getting values from both clipboards is not supported"));

    bool ok = false;
#if wxUSE_CLIPBOARD
    wxClipboard* clipboard = wxTheClipboard;
    bool was_open = clipboard->IsOpened();
    ok = was_open || clipboard->Open();

    if (ok)
    {
        size_t i;

        clipboard->UsePrimarySelection(HASBIT(clip_type, Primary));
        for (i = 0; i < array_count; i++)
        {
        #ifdef __WXMSW__
            // wxClipboard::IsSupported(wxDF_HTML) returns false always; handle it here instead
            if (array[i] == wxDF_HTML)
            {
                static int CF_HTML = ::RegisterClipboardFormat(_T("HTML Format"));

                if (::IsClipboardFormatAvailable(CF_HTML))
                {
                    break;
                }
            }
            else
        #endif
            if (clipboard->IsSupported(wxDataFormat(array[i])))
            {
                break;
            }
        }
        ok = (i != array_count);

        if (!was_open)
            clipboard->Close();
    }
#endif // wxUSE_CLIPBOARD
    return ok;
}

/*static*/
bool wxClipboardHelper::GetText(wxString* str, Type clip_type)
{
    wxCHECK_MSG(clip_type != Both, false, wxT("Getting values from both clipboards is not supported"));

    if (!str) return false;
    bool ok = false;

#if wxUSE_DATAOBJ && wxUSE_CLIPBOARD
    wxClipboard* clipboard = wxTheClipboard;
    bool was_open = clipboard->IsOpened();
    ok = was_open || clipboard->Open();

    if (ok)
    {
        wxTextDataObject temp;

        clipboard->UsePrimarySelection(HASBIT(clip_type, Primary));
        ok = clipboard->GetData(temp);

        if (ok)
            *str = temp.GetText();

        if (!was_open)
            clipboard->Close();
    }
#endif // wxUSE_DATAOBJ && wxUSE_CLIPBOARD
    return ok && !str->empty();
}

/*static*/
bool wxClipboardHelper::Set(wxDataObject* def, wxDataObject* primary)
{
#if wxUSE_DATAOBJ && wxUSE_CLIPBOARD
    wxClipboard* clipboard = wxTheClipboard;
    bool was_open = clipboard->IsOpened();
    bool ok = was_open || clipboard->Open();

    if (ok)
    {
        if (def)
        {
            clipboard->UsePrimarySelection(false);
            ok = clipboard->SetData(def);
            if (ok)
            {
                def = NULL;
            }
        }
    #ifndef __WXMSW__
        if (primary)
        {
            clipboard->UsePrimarySelection(true);
            ok = clipboard->SetData(primary);
            clipboard->UsePrimarySelection(false);
            if (ok)
            {
                primary = NULL;
            }
        }
    #endif // __WXMSW__
        if (!was_open)
        {
            clipboard->Close();
        }
        //clipboard->Flush(); // else emu and wxc is freezing
    }
    delete def;
    delete primary;
    return ok;
#else
    return false;
#endif
}

/*static*/
bool wxClipboardHelper::SetText(const wxString& str, Type clip_type)
{
#if wxUSE_DATAOBJ && wxUSE_CLIPBOARD
    return Set(HASBIT(clip_type, Default) ? new wxTextDataObject(str) : NULL,
               HASBIT(clip_type, Primary) ? new wxTextDataObject(str) : NULL);
#else
    return false;
#endif
}

/*static*/
bool wxClipboardHelper::SetHtmlText(const wxString& htmldata)
{
    bool ok;
#ifdef __WXMSW__
    ok = wxOpenClipboard();
    if (ok)
    {
        EmptyClipboard();
        const wxCharBuffer buf(htmldata.mb_str());
        ok = wxSetClipboardData(wxDF_HTML, buf.data()); // save as html
        wxSetClipboardData(wxDF_TEXT, buf.data());      // save also as plain text
        wxCloseClipboard();
    }
#else
    ok = SetText(htmldata);
#endif
    return ok;
}

#if (wxVERSION_NUMBER < 2903)
static const char BOM_UTF32BE[] = { '\x00', '\x00', '\xFE', '\xFF' };
static const char BOM_UTF32LE[] = { '\xFF', '\xFE', '\x00', '\x00' };
static const char BOM_UTF16BE[] = { '\xFE', '\xFF'                 };
static const char BOM_UTF16LE[] = { '\xFF', '\xFE'                 };
static const char BOM_UTF8[]    = { '\xEF', '\xBB', '\xBF'         };

const char* wxConvAuto_GetBOMChars(wxBOM bom, size_t* count)
{
    wxCHECK_MSG( count , NULL, wxS("count pointer must be provided") );

    switch ( bom )
    {
        case wxBOM_UTF32BE: *count = WXSIZEOF(BOM_UTF32BE); return BOM_UTF32BE;
        case wxBOM_UTF32LE: *count = WXSIZEOF(BOM_UTF32LE); return BOM_UTF32LE;
        case wxBOM_UTF16BE: *count = WXSIZEOF(BOM_UTF16BE); return BOM_UTF16BE;
        case wxBOM_UTF16LE: *count = WXSIZEOF(BOM_UTF16LE); return BOM_UTF16LE;
        case wxBOM_UTF8   : *count = WXSIZEOF(BOM_UTF8   ); return BOM_UTF8;
        case wxBOM_Unknown:
        case wxBOM_None:
            wxFAIL_MSG( wxS("Invalid BOM type") );
            return NULL;
    }

    wxFAIL_MSG( wxS("Unknown BOM type") );
    return NULL;
}

#define BOM_EQUAL(src,array) ( 0 == memcmp((src), array, sizeof(array) ) )

wxBOM wxConvAuto_DetectBOM(const char *src, size_t srcLen)
{
    // examine the buffer for BOM presence
    //
    // quoting from http://www.unicode.org/faq/utf_bom.html#BOM:
    //
    //  Bytes           Encoding Form
    //
    //  00 00 FE FF     UTF-32, big-endian
    //  FF FE 00 00     UTF-32, little-endian
    //  FE FF           UTF-16, big-endian
    //  FF FE           UTF-16, little-endian
    //  EF BB BF        UTF-8
    //
    // as some BOMs are prefixes of other ones we may need to read more bytes
    // to disambiguate them

    switch ( srcLen )
    {
        case 0:
            return wxBOM_Unknown;

        case 1:
            if ( src[0] == '\x00' || src[0] == '\xFF' ||
                 src[0] == '\xFE' || src[0] == '\xEF')
            {
                // this could be a BOM but we don't know yet
                return wxBOM_Unknown;
            }
            break;

        case 2:
        case 3:
            if ( src[0] == '\xEF' && src[1] == '\xBB' )
            {
                if ( srcLen == 3 )
                    return src[2] == '\xBF' ? wxBOM_UTF8 : wxBOM_None;

                return wxBOM_Unknown;
            }

            if ( BOM_EQUAL(src, BOM_UTF16BE) )
                return wxBOM_UTF16BE;

            if ( BOM_EQUAL(src, BOM_UTF16LE) )
            {
                // if the next byte is 0, it could be an UTF-32LE BOM but if it
                // isn't we can be sure it's UTF-16LE
                if ( srcLen == 3 && src[2] != '\x00' )
                    return wxBOM_UTF16LE;

                return wxBOM_Unknown;
            }

            if ( src[0] == '\x00' && src[1] == '\x00' )
            {
                // this could only be UTF-32BE, check that the data we have so
                // far allows for it
                if ( srcLen == 3 && src[2] != '\xFE' )
                    return wxBOM_None;

                return wxBOM_Unknown;
            }
            break;

        default:
            // we have at least 4 characters so we may finally decide whether
            // we have a BOM or not
            if ( BOM_EQUAL(src, BOM_UTF8) )
                return wxBOM_UTF8;

            if ( BOM_EQUAL(src, BOM_UTF32BE) )
                return wxBOM_UTF32BE;

            if ( BOM_EQUAL(src, BOM_UTF32LE) )
                return wxBOM_UTF32LE;

            if ( BOM_EQUAL(src, BOM_UTF16BE) )
                return wxBOM_UTF16BE;

            if ( BOM_EQUAL(src, BOM_UTF16LE) )
                return wxBOM_UTF16LE;
    }

    return wxBOM_None;
}
#endif

#if (wxVERSION_NUMBER < 2900)
wxArrayString wxSplit(const wxString& str, const wxChar sep, wxChar escape)
{
   wxUnusedVar(escape);
   wxStringTokenizerMode mode = wxTOKEN_RET_EMPTY;
   wxArrayString temp = wxStringTokenize(str, wxString(sep), mode);

   return temp;
}
#endif

// annoying function only here because the wxString ctor conv argument is WXUNUSED in some build types
/*static*/
wxString wxTextEncoding::CharToString(const char* src, const wxMBConv& conv, size_t len)
{
    wxString str;

    if (len)
    {
        size_t wlen;
        wxWCharBuffer buf(conv.cMB2WC(src, len, &wlen));

        str = wxString(buf.data(), wxConvLibc /*WXUNUSED*/, wlen);
    }
    return str;
}

// annoying function only here because the wxString ctor conv argument is WXUNUSED in some build types
/*static*/
wxCharBuffer wxTextEncoding::StringToChar(const wxString& src, const wxMBConv& conv)
{
    wxWCharBuffer wbuf(src.wc_str(wxConvLibc /*WXUNUSED*/));
    wxCharBuffer buf(conv.cWC2MB(wbuf));

    return buf;
}

/*static*/
wxCharBuffer wxTextEncoding::StringToChar(const wxString& s, Type encoding, size_t* size_ptr)
{
    wxCharBuffer buf;
    size_t size;

    switch (encoding)
    {
        case None:
            buf = s.mb_str(*wxConvCurrent);
            size = wxBuffer_length(buf);
            break;
        case Unicode_LE:
        {
            const wxWCharBuffer temp = s.wc_str(*wxConvCurrent);
            size = wxBuffer_length(temp) * sizeof(wchar_t);

            buf.extend(size);
            memcpy(buf.data(), temp.data(), size);
            break;
        }
        case UTF8:
            buf = StringToChar(s, wxConvUTF8);
            size = wxBuffer_length(buf);
            break;
        case ISO8859_1:
            buf = StringToChar(s, wxConvISO8859_1);
            size = wxBuffer_length(buf);
            break;
    #ifdef __WXMSW__
        case OEM:
            buf = StringToChar(s, wxMBConvOEM());
            size = wxBuffer_length(buf);
            break;
    #endif
        default:
            size = 0;
            break;
    }
    if (size_ptr)
    {
        *size_ptr = size;
    }
    return buf;
}

/*static */
wxString wxTextEncoding::CharToString(const wxCharBuffer& buf, size_t buf_len, wxBOM* file_bom_ptr)
{
    wxConvAuto conv_auto;
    wxBOM file_bom;
    wxString str;

    if (buf_len == wxNO_LEN) buf_len = wxBuffer_length(buf);

#if (wxVERSION_NUMBER >= 2903) && wxUSE_UNICODE
    str = wxString(buf.data(), conv_auto, buf_len); // ctor conv arg WXUNUSED() in ansi builds in wx28 (in wx293 ansi builds ??)
    file_bom = conv_auto.GetBOM();
#else
    // The method wxAutoConv.GetBOM() is not in wx 2.8, so roll our own
    file_bom = wxConvAuto_DetectBOM(buf.data(), buf_len);
    size_t bom_charcount = 0;
    
    switch (file_bom)
    {
        case wxBOM_Unknown:
        case wxBOM_None:
            // wxConvAuto.GetBOMChars() barks if passed these
            break;
        default:
            wxConvAuto_GetBOMChars(file_bom, &bom_charcount);
            break;
    }

    switch (file_bom)
    {
        case wxBOM_UTF32BE:
        case wxBOM_UTF32LE:
        case wxBOM_UTF16BE:
            // not supported
            break;
        case wxBOM_UTF16LE:
            str = wxString((wchar_t*)(buf.data() + bom_charcount), *wxConvCurrent, (buf_len - bom_charcount) / sizeof(wchar_t));
            break;
        case wxBOM_UTF8:
            str = CharToString(buf.data() + bom_charcount, wxConvUTF8, buf_len - bom_charcount);
            break;
        default:
            str = wxString(buf.data(), wxConvLibc, buf_len);
            break;
    }
#endif // 2.9
    if (file_bom_ptr) *file_bom_ptr = file_bom;
    return str;
}

#ifdef __WXMSW__

wxMBConvOEM::wxMBConvOEM() : wxMBConv()
{
}

wxMBConvOEM::~wxMBConvOEM()
{
}

size_t wxMBConvOEM::ToWChar(wchar_t*    dst, size_t dstLen,
                            const char* src, size_t srcLen) const
{
    if (srcLen == wxNO_LEN) srcLen = strlen(src);
    wxCharBuffer buf(srcLen);
    
    OemToCharBuffA(src, buf.data(), srcLen);
    return dst ? mbstowcs(dst, buf.data(), dstLen) : wxBuffer_length(buf);
}

size_t wxMBConvOEM::FromWChar(char*          dst, size_t dstLen,
                              const wchar_t* src, size_t srcLen) const
{
    size_t len;
    if (srcLen == wxNO_LEN) srcLen = wcslen(src);
    wxCharBuffer temp(srcLen);
    
    wcstombs(temp.data(), src, srcLen);
    if (dst)
    {
        CharToOemBuffA(temp.data(), dst, dstLen);
        len = strlen(dst);
    }
    else
    {
        len = wxBuffer_length(temp);
    }
    return len;
}
#endif

/*static*/
wxTextEncoding::Type wxTextEncoding::TypeFromString(const wxString& rstr)
{
    const struct _MAP
    {
        const wxChar* name;
        Type encoding;
    } map[] =
    {
        { wxT("utf-8"     ), UTF8      },
        { wxT("iso-8859-1"), ISO8859_1 }
    };
    wxString str = rstr;

    str.MakeLower();
    for (size_t i = 0; i < WXSIZEOF(map); i++)
    {
        if (str == map[i].name)
        {
            return map[i].encoding;
        }
    }

    return None;
}

/*static*/
bool wxTextEncoding::TypeFromString(const char* str, const char* identifier, const char* ctrl, Type* encoding)
{
    const char* p = strstr(str, identifier);
    
    if (p)
    {
        const char* begin = p + strlen(identifier);
        const char* end   = strpbrk(begin, ctrl);

        if (begin && end)
        {
            *encoding = TypeFromString(wxString::From8BitData(begin, end - begin));
            return true;
        }
    }
    return false;
}

/*static*/
wxString wxTextEncoding::LoadFile(const wxCharBuffer& charBuf, size_t buf_len, Type encoding)
{
    wxString str;

    switch (encoding)
    {
        case Unicode_LE:
            str = CharToString(charBuf.data(), wxConvAuto(), buf_len);
            break;
        case UTF8:
            str = CharToString(charBuf.data(), wxConvUTF8, buf_len);
            break;
        case ISO8859_1:
            str = CharToString(charBuf.data(), wxConvISO8859_1, buf_len);
            break;
    #ifdef __WXMSW__
        case OEM:
            str = CharToString(charBuf.data(), wxMBConvOEM(), buf_len);
            break;
    #endif
        case None:
        default:
            str = wxConvertMB2WX(charBuf.data());
            break;
    }
    return str;
}

/*static*/
bool wxTextEncoding::SaveFile(const wxString& s, wxOutputStream& stream, Type encoding, bool file_bom)
{
    bool ok = true;
    const char* bom_chars;
    size_t size;

    // write bom
    if (ok && file_bom) switch (encoding)
    {
        case Unicode_LE:
            bom_chars = wxConvAuto_GetBOMChars(wxBOM_UTF16LE, &size);
            ok = bom_chars && (size == stream.Write(bom_chars, size * sizeof(char)).LastWrite());
            break;
        case UTF8:
            bom_chars = wxConvAuto_GetBOMChars(wxBOM_UTF8, &size);
            ok = bom_chars && (size == stream.Write(bom_chars, size * sizeof(char)).LastWrite());
            break;
        case None:
    #ifdef __WXMSW__
        case OEM:
    #endif
            break;
        default:
            ok = false;
            break;
    }

    // write text
    if (ok)
    {
        const wxCharBuffer buf = StringToChar(s, encoding, &size);
        
        ok = (buf.data() != NULL);
        if (ok)
        {
            ok = (size == stream.Write(buf.data(), size).LastWrite());
        }
    }
    return ok;
}
