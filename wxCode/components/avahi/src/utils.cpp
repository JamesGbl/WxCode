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

#include "wx/avahi/utils.h"
#include <avahi-common/domain.h>


//
// wxAvahi tests
//

int wxAvahi()
{
	return wxAVAHI_MAJOR << 16 + wxAVAHI_MINOR << 8 + wxAVAHI_RELEASE;
}

#if USE_GUI == 1
int wxAvahiGui()
{
	return wxAvahi();
}
#endif /* USE_GUI == 1 */



//
// Address
//

void wxAddressToAvahi(const wxIPaddress* addr, AvahiAddress* avahiAddress){
	avahi_address_parse(addr->Hostname().mb_str(wxConvUTF8),
#if wxUSE_IPV6
							(addr->Type()==wxSockAddress::IPV6)?AVAHI_PROTO_INET6:AVAHI_PROTO_INET,
#else
							AVAHI_PROTO_INET,
#endif
							avahiAddress);	
}

wxString wxAvahiAddressToString(const AvahiAddress* avahiAddress){
	if(!avahiAddress)
		return wxT("");
	char buffer[AVAHI_ADDRESS_STR_MAX];
	avahi_address_snprint(buffer, AVAHI_ADDRESS_STR_MAX, avahiAddress);
	return wxString(buffer, wxConvUTF8);
}


wxString wxAvahiNormalizeName(const wxString& str)
{
	char buffer[AVAHI_DOMAIN_NAME_MAX];
	avahi_normalize_name((const char*)str.mb_str(wxConvUTF8), buffer, AVAHI_DOMAIN_NAME_MAX);
	return wxString(buffer, wxConvUTF8);
}

bool wxAvahiDomainEqual(const wxString& domain1, const wxString& domain2)
{
	return avahi_domain_equal((const char*)domain1.mb_str(wxConvUTF8), (const char*)domain2.mb_str(wxConvUTF8))!=0;
}

unsigned wxAvahiDomainHash(const wxString& domain)
{
	return avahi_domain_hash ((const char*)domain.mb_str(wxConvUTF8));
}

bool wxAvahiValidServiceTypeGeneric(const wxString& service)
{
	return avahi_is_valid_service_type_generic((const char*)service.mb_str(wxConvUTF8))==1;
}

bool wxAvahiValidServiceTypeStrict(const wxString& service)
{
	return avahi_is_valid_service_type_strict((const char*)service.mb_str(wxConvUTF8))==1;
}

bool wxAvahiValidServiceSubtype(const wxString& subtype)
{
	return avahi_is_valid_service_subtype((const char*)subtype.mb_str(wxConvUTF8))==1;
}

bool wxAvahiValidDomainName(const wxString& domain)
{
	return avahi_is_valid_domain_name((const char*)domain.mb_str(wxConvUTF8))==1;
}

bool wxAvahiValidServiceName(const wxString& service)
{
	return avahi_is_valid_service_name((const char*)service.mb_str(wxConvUTF8))==1;
}

bool wxAvahiValidHostName(const wxString& host)
{
	return avahi_is_valid_host_name((const char*)host.mb_str(wxConvUTF8))==1;
}

bool wxAvahiValidFQDN(const wxString& fqdn)
{
	return avahi_is_valid_fqdn((const char*)fqdn.mb_str(wxConvUTF8))==1;
}

wxString wxAvahiServiceNameJoin(const wxString& name, const wxString& type, const wxString& domain)
{
	char buffer[AVAHI_DOMAIN_NAME_MAX];
	avahi_service_name_join(buffer, AVAHI_DOMAIN_NAME_MAX, (const char*)name.mb_str(wxConvUTF8), (const char*)type.mb_str(wxConvUTF8), (const char*)domain.mb_str(wxConvUTF8));
	return wxString(buffer, wxConvUTF8);
}

void wxAvahiServiceNameJoin(const wxString& service, wxString& name, wxString& type, wxString& domain)
{
	char buf1[AVAHI_DOMAIN_NAME_MAX];
	char buf2[AVAHI_DOMAIN_NAME_MAX];
	char buf3[AVAHI_DOMAIN_NAME_MAX];
	avahi_service_name_split((const char*)service.mb_str(wxConvUTF8), buf1, AVAHI_DOMAIN_NAME_MAX,
																		buf2, AVAHI_DOMAIN_NAME_MAX, 
																		buf3, AVAHI_DOMAIN_NAME_MAX);
	name = wxString(buf1, wxConvUTF8);
	type = wxString(buf2, wxConvUTF8);
	domain = wxString(buf3, wxConvUTF8);
}

wxString wxAvahiGetTypeFromSubtype(const wxString& subtype)
{
	return wxString(avahi_get_type_from_subtype((const char*)subtype.mb_str(wxConvUTF8)) , wxConvUTF8);
}
