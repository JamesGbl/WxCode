// wx29.h
// Copyright (c) 2009 by Troels K. All rights reserved.
// License: wxWindows Library Licence, Version 3.1 - see LICENSE.txt

#ifndef __WX29_H__
#define __WX29_H__

#if defined(_WX_ABOUTDLG_H_) && (wxVERSION_NUMBER < 2900)
inline void wxAboutBox(const wxAboutDialogInfo& info, wxWindow* WXUNUSED(parent))
{
   wxAboutBox(info);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// wxAppEx

class wxAppEx : public wxApp
{
#if (wxVERSION_NUMBER < 2900)
protected:
   wxString m_appDisplayName;
public:
   wxString GetAppDisplayName() const { return m_appDisplayName; }

    void SetAppDisplayName(const wxString& name) { m_appDisplayName = name; }
#endif
};

#endif // __WX29_H__
