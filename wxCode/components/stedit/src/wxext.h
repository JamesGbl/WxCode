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

#ifdef WXCODEUSINGDLL
   #define WXDLLIMPEXP_CODE WXIMPORT
#else
   #define WXDLLIMPEXP_CODE
#endif

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

/////////////////////////////////////////////////////////////////////////////
// wxPreviewFrameEx

#ifdef _WX_PRNTBASEH__
class WXDLLIMPEXP_CODE wxPreviewFrameEx : public wxPreviewFrame
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

#endif // __WXEXT_H__
