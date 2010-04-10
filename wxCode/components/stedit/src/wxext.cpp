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

#include "wx/stedit/stedefs.h"

#include "wxext.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(AcceleratorArray)

bool wxGetExeFolder(wxFileName* filename)
{
   filename->Assign(wxTheApp->argv[0]);
   filename->SetFullName(wxEmptyString);
   return filename->IsOk();
}

bool wxLocale_Init(wxLocale* locale, const wxString& exetitle, enum wxLanguage lang)
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

#if wxUSE_ACCEL

wxAcceleratorEntry wxGetStockAcceleratorEx(wxWindowID id)
{
    wxAcceleratorEntry ret;

    #define STOCKITEM(stockid, flags, keycode)      \
        case stockid:                               \
            ret.Set(flags, keycode, stockid);       \
            break;

    switch (id)
    {
        STOCKITEM(wxID_PRINT,                wxACCEL_CMD,'P')
        STOCKITEM(wxID_PREVIEW,              wxACCEL_CMD | wxACCEL_SHIFT,'P')
        STOCKITEM(wxID_SAVEAS,               wxACCEL_CMD | wxACCEL_SHIFT,'S')
        STOCKITEM(wxID_SELECTALL,            wxACCEL_CMD,'A')
        STOCKITEM(wxID_UNDO,                 wxACCEL_CMD,'Z')
        STOCKITEM(wxID_REDO,                 wxACCEL_CMD,'Y')
        //STOCKITEM(wxID_PREFERENCES,          wxACCEL_CMD,'T')
        STOCKITEM(wxID_ICONIZE_FRAME,        wxACCEL_ALT,WXK_FULLSCREEN)
        STOCKITEM(wxID_REFRESH,              wxACCEL_NORMAL, WXK_F5)
        STOCKITEM(wxID_PROPERTIES,           wxACCEL_ALT,WXK_RETURN)
        STOCKITEM(wxID_BACKWARD,             wxACCEL_ALT , WXK_LEFT)
        STOCKITEM(wxID_FORWARD,              wxACCEL_ALT , WXK_RIGHT)
        STOCKITEM(wxID_HELP,                 wxACCEL_NORMAL, WXK_HELP)
        STOCKITEM(wxID_REPLACE,              wxACCEL_CMD,'H')
        STOCKITEM(wxID_EXIT,                 wxACCEL_CMD, 'Q')
        STOCKITEM(wxID_CLOSE,                wxACCEL_CMD, 'W')
        STOCKITEM(wxID_CLEAR,                wxACCEL_NORMAL, WXK_DELETE)
        default:
            //if (id == wxXRCID_GOTO) ret.Set(wxACCEL_CTRL,'G',id);
            //else
            ret = wxGetStockAccelerator(id);
            break;
    };

    #undef STOCKITEM

    // always use wxAcceleratorEntry::IsOk on returned value !
    return ret;
}

void wxSetAcceleratorTable(wxWindow* wnd, const AcceleratorArray& array)
{
   const size_t count = array.GetCount();
   wxAcceleratorEntry* temp = new wxAcceleratorEntry[count];
   for (size_t i = 0; i < count; i++)
   {
      temp[i] = array.Item(i);
   }
   wxAcceleratorTable accel(count, temp);
   wnd->SetAcceleratorTable(accel);
   wxDELETEA(temp);
}

wxString wxMenuItem_GetText(const wxMenuItem* item)
{
   wxString str = item->GetItemLabel();
#ifdef __WXGTK__
   str.Replace(wxString(wxT('_')), wxString(wxT('&')));
#endif
   return str;
}

#define ACCELSTR_SEP "   "

bool wxMenuItem_SetAccelText(wxMenuItem* item, const wxString& accel, bool append)
{
   wxString str = wxMenuItem_GetText(item);
   wxString ch_sep = wxT("\t");
   const int sep = str.Find(ch_sep);

   if (wxNOT_FOUND == sep)
   {
   }
   else if (append)
   {
      ch_sep = wxT(ACCELSTR_SEP);
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

wxString wxGetAccelText(int flags, int keyCode)
{
   // wxAcceleratorEntry.ToString() produces silly text
   wxString str;
   const wxChar sep = '+'; // '-' the wx default
#if 0 && (wxVERSION_NUMBER >= 2800)
   wxAcceleratorEntry entry(flags, keyCode);
   str = entry.ToString(); // doesn't work (wxIsalnum(WXK_F2)), silly text (WXK_NUMPAD_ADD)
#else
   if (flags & wxACCEL_CTRL)
   {
      if (str.Length()) str+=sep;
      str+=_("Ctrl");
   }
   if (flags & wxACCEL_ALT)
   {
      if (str.Length()) str+=sep;
      str+=_("Alt");
   }
   if (flags & wxACCEL_SHIFT)
   {
      if (str.Length()) str+=sep;
      str+=_("Shift");
   }
   if (str.Length()) str+=sep;
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
         else str+=(wxChar)keyCode;
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
      STOCKITEM(wxID_OPEN,      _("&Open..."))    // with ellipsis
      STOCKITEM(wxID_SELECTALL, _("Select &All")) // with ampersand
      STOCKITEM(wxID_FIND,      _("&Find..."))    // with ellipsis
      STOCKITEM(wxID_REPLACE,   _("Rep&lace...")) // with ellipsis
      STOCKITEM(wxID_REVERT,    _("Re&vert..."))
      STOCKITEM(wxID_EXIT,      _("E&xit")) // more common than "Quit"
      default:
         break;
   }
#undef STOCKITEM
   if (stockLabel.Length())
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

wxString wxGetAccelText(const wxAcceleratorEntry& accel)
{
   return wxGetAccelText(accel.GetFlags(), (enum wxKeyCode)accel.GetKeyCode());
}

void wxMenu_Fixup(wxMenuBar* menu, const AcceleratorArray& array)
{
   const size_t count = array.GetCount();
   for (size_t i = 0; i < count; i++)
   {
      const wxAcceleratorEntry& entry = array.Item(i);
      wxMenuItem* item = menu->FindItem(entry.GetCommand());
      if (item)
      {
         wxMenuItem_SetAccelText(item, wxGetAccelText(entry), true);
      }
   }
}

void wxMenu_Fixup(wxMenu* menu, const AcceleratorArray& array)
{
   const size_t count = array.GetCount();
   for (size_t i = 0; i < count; i++)
   {
      const wxAcceleratorEntry& entry = array.Item(i);
      wxMenuItem* item = menu->FindItem(entry.GetCommand());
      if (item)
      {
         wxMenuItem_SetAccelText(item, wxGetAccelText(entry), true);
      }
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

#endif // wxUSE_ACCEL

