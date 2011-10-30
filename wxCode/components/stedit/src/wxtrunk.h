///////////////////////////////////////////////////////////////////////////////
// File:        wxtrunk.h
// Purpose:     wx28/wx29 compatibility
// Author:      Troels K
// Created:     2011-10-28
// RCS-ID:
// Copyright:   (c) John Labenski, Troels K
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#if (wxVERSION_NUMBER < 2812)
   #define wxT_2 wxT
#endif

#if (wxVERSION_NUMBER < 2900)
WXDLLIMPEXP_STEDIT wxArrayString wxSplit(const wxString& str, const wxChar sep, wxChar escape = wxT('\\'));
#endif

#ifdef _WX_CONVAUTO_H_
#if (wxVERSION_NUMBER >= 2903)
inline wxBOM wxConvAuto_DetectBOM(const char *src, size_t srcLen)
{
    return wxConvAuto::DetectBOM(src, srcLen);
}
#else
enum wxBOM
{
    wxBOM_Unknown = -1,
    wxBOM_None,
    wxBOM_UTF32BE,
    wxBOM_UTF32LE,
    wxBOM_UTF16BE,
    wxBOM_UTF16LE,
    wxBOM_UTF8
};
WXDLLIMPEXP_STEDIT wxBOM wxConvAuto_DetectBOM(const char *src, size_t srcLen);
#endif
#endif

#if defined(_WX_ABOUTDLG_H_) && (wxVERSION_NUMBER < 2900)
inline void wxAboutBox(const wxAboutDialogInfo& info, wxWindow* WXUNUSED(parent))
{
   wxAboutBox(info);
}
#endif

inline size_t wxWX2MBbuf_length(const wxWX2MBbuf& buf)
{
#if (wxVERSION_NUMBER >= 2900)
    return buf.length(); // wxWX2MBbuf.length() not available in wx28
#else
    return strlen(buf);
#endif
}
