/////////////////////////////////////////////////////////////////////////////
// Name:        wx/avahi/art.h
// Purpose:     Avahi art provider.
// Author:      Kia Emilien
// Created:     2006/10/20
// RCS-ID:      $Id $
// Copyright:   (c) 2006-2007 Kia Emilien
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef ART_H_
#define ART_H_
#include "wx/avahi/avahidef.h"

#include <wx/wx.h>
#include <wx/artprov.h>
#include <wx/hashmap.h>

WX_DECLARE_STRING_HASH_MAP(wxIcon, wxAvahiArtProviderIconMap);
WX_DECLARE_STRING_HASH_MAP(wxString, wxAvahiArtProviderStockMap);

/**
 * Special art provider for zeroconf services.
 * A service type art id begin with "zeroconf-servicetype-" : 
 *   * zeroconf-servicetype-_ssh._tcp for SSH service*/
class wxAvahiArtProvider : public wxArtProvider
{
protected:
	wxBitmap CreateBitmap(const wxArtID& id, 
                        const wxArtClient& client,
                        const wxSize& size);

	virtual void Initialize();
	wxAvahiArtProviderIconMap  m_iconmap;
	wxAvahiArtProviderStockMap m_stockmap;
  
public:
	wxAvahiArtProvider();
	virtual ~wxAvahiArtProvider(){}

	/** Add an icon for a service type. 
	 * \param service Service type (without the "zeroconf-servicetype-").
	 * \param icon Icon to register.
	 **/
	void RegisterServiceTypeIcon(const wxString& service, const wxIcon& icon);
	/** Add a stock icon name to a service type (without the "zeroconf-servicetype-").
	 * The stock icone name is a name corresponding to a name given by another art provider.
	 * \param service Service name.
	 * \param stock Stock icon name.
	 */
	void RegisterServiceStockName(const wxString& service, const wxString& stock);

	/** Get the global avahi art provider.*/
	static wxAvahiArtProvider& GetArtProvider();

	static void RegisterStandardServiceTypeIcon(const wxString& service, const wxIcon& icon)
	{GetArtProvider().RegisterServiceTypeIcon(service, icon);}
	static void RegisterStandardServiceStockName(const wxString& service, const wxString& stock)
	{GetArtProvider().RegisterServiceStockName(service, stock);}
};


#endif /*ART_H_*/
