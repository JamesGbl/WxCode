/////////////////////////////////////////////////////////////////////////////
// Name:        art.cpp
// Purpose:     Avahi art provider.
// Author:      Kia Emilien
// Created:     2006/10/20
// RCS-ID:      $Id: client.cpp,v 1.4 2005/10/21 16:42:58 frm Exp $
// Copyright:   (c) 2006-2007 Kia Emilien
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#ifndef WX_PRECOMP
	#include <wx/module.h>
#endif

#if USE_GUI == 1

#include "wx/avahi/art.h"

//
// wxAvahiArtProviderModule
//

class wxAvahiArtProviderModule: public wxModule
{
public:
	static wxAvahiArtProvider* s_artprov;
	
	wxAvahiArtProviderModule() { }
    virtual bool OnInit()
    {
    	s_artprov = new wxAvahiArtProvider;
    	wxArtProvider::Push(s_artprov);
    	return true;
    }
    virtual void OnExit() {}
private:
    DECLARE_DYNAMIC_CLASS(wxAvahiArtProviderModule)
};
wxAvahiArtProvider* wxAvahiArtProviderModule::s_artprov = NULL;

IMPLEMENT_DYNAMIC_CLASS(wxAvahiArtProviderModule, wxModule)


//
// wxAvahiArtProvider
//

wxAvahiArtProvider& wxAvahiArtProvider::GetArtProvider()
{
	return *wxAvahiArtProviderModule::s_artprov;
}

wxAvahiArtProvider::wxAvahiArtProvider():
	wxArtProvider()
{
	Initialize();
}
	
void wxAvahiArtProvider::Initialize()
{
	RegisterServiceStockName(wxT("_workstation._tcp"), 	wxT("user-desktop"));
	RegisterServiceStockName(wxT("_ssh._tcp"), 			wxT("utilities-terminal"));
	RegisterServiceStockName(wxT("_ftp._tcp"), 			wxT("folder-remote"));
	RegisterServiceStockName(wxT("_sftp-ssh._tcp"), 	wxT("folder-remote"));
	RegisterServiceStockName(wxT("_http._tcp"), 		wxT("web-browser"));
	RegisterServiceStockName(wxT("_https._tcp"), 		wxT("web-browser"));
}

wxBitmap wxAvahiArtProvider::CreateBitmap(const wxArtID& id, 
                        const wxArtClient& client, const wxSize& size)
{
	wxBitmap bmp = wxNullBitmap;
	wxAvahiArtProviderIconMap::iterator it1 = m_iconmap.find(id);
	if(it1!=m_iconmap.end())
		bmp.CopyFromIcon(it1->second);
	else
	{
		wxAvahiArtProviderStockMap::iterator it2 = m_stockmap.find(id);
		if(it2!=m_stockmap.end())
			bmp = wxArtProvider::GetBitmap(it2->second, client, size);
	}
	return bmp; // wxT("gtk-network")
}

void wxAvahiArtProvider::RegisterServiceTypeIcon(const wxString& service, const wxIcon& icon)
{
	m_iconmap[wxT("zeroconf-servicetype-")+service] = icon;
}

void wxAvahiArtProvider::RegisterServiceStockName(const wxString& service, const wxString& stock)
{
	m_stockmap[wxT("zeroconf-servicetype-")+service] = stock;
}


#endif /* #if USE_GUI == 1 */
