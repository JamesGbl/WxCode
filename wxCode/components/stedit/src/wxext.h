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


WX_DECLARE_OBJARRAY(wxAcceleratorEntry, AcceleratorArray);

WXDLLIMPEXP_STEDIT bool wxLocale_Init(wxLocale*, const wxString& exetitle, enum wxLanguage lang = wxLANGUAGE_DEFAULT);
WXDLLIMPEXP_STEDIT wxAcceleratorEntry wxGetStockAcceleratorEx(wxWindowID);

WXDLLIMPEXP_STEDIT void wxSetAcceleratorTable(wxWindow*, const AcceleratorArray&);
class WXDLLIMPEXP_FWD_CORE wxMenuBar;
WXDLLIMPEXP_STEDIT void wxMenu_Fixup(wxMenuBar*, const AcceleratorArray&);
WXDLLIMPEXP_STEDIT void wxMenu_Fixup(wxMenu   *, const AcceleratorArray&);

#ifdef _WX_STOCKITEM_H_
#if (wxVERSION_NUMBER < 2901)
#define wxSTOCK_WITHOUT_ELLIPSIS 4
#define wxSTOCK_FOR_BUTTON (wxSTOCK_WITH_MNEMONIC | wxSTOCK_WITHOUT_ELLIPSIS)
#endif
#define wxSTOCK_PLAINTEXT wxSTOCK_WITHOUT_ELLIPSIS
WXDLLIMPEXP_STEDIT wxString wxGetStockLabelEx(wxWindowID, long flags = wxSTOCK_WITH_MNEMONIC);
#endif

#endif // __WXEXT_H__
