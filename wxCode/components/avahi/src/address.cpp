/////////////////////////////////////////////////////////////////////////////
// Name:        address.cpp
// Purpose:     Avahi to wxWidgets internet address translation
// Author:      Kia Emilien
// Created:     2006/10/20
// RCS-ID:      $Id: address.cpp,v 1.4 2005/10/21 16:42:58 frm Exp $
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
#endif

#include "wx/avahi/address.h"

wxIPaddress* wxAvahiToWxAddress(const AvahiAddress* addr){
	if(!addr)
		return NULL;

	char a[AVAHI_ADDRESS_STR_MAX];
	avahi_address_snprint(a, sizeof(a), addr);
	wxIPaddress *wxaddr = NULL;

#if wxUSE_IPV6
	if(addr->proto==AVAHI_PROTO_INET6){
		/* IPv6 */
		wxaddr = new wxIPV6address;
	}else
#endif /* wxUSE_IPV6 */
	{
		/* IPv4 */
		wxaddr = new wxIPV4address;
	}
	wxaddr->Hostname(wxString(a, *wxConvCurrent));
	return wxaddr; 
}

AvahiAddress* wxAddressToAvahi(wxIPaddress* addr){
	if(!addr)
		return NULL;
	AvahiAddress* avahiAddress = new AvahiAddress;
	avahi_address_parse(addr->Hostname().mb_str(*wxConvCurrent),
#if wxUSE_IPV6
							(addr->Type()==wxSockAddress::IPV6)?AVAHI_PROTO_INET6:AVAHI_PROTO_INET,
#else
							AVAHI_PROTO_INET,
#endif
							avahiAddress);
	return avahiAddress;
}

void wxAddressToAvahi(wxIPaddress* addr, AvahiAddress* avahiAddress){
	avahi_address_parse(addr->Hostname().mb_str(*wxConvCurrent),
#if wxUSE_IPV6
							(addr->Type()==wxSockAddress::IPV6)?AVAHI_PROTO_INET6:AVAHI_PROTO_INET,
#else
							AVAHI_PROTO_INET,
#endif
							avahiAddress);	
}


