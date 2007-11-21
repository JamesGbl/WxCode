/////////////////////////////////////////////////////////////////////////////
// Name:        wx/avahi/address.h
// Purpose:     Avahi to wxWidgets internet address translation
// Author:      Kia Emilien
// Created:     2006/10/20
// RCS-ID:      $Id: address.h,v 1.3 2005/10/21 16:42:58 frm Exp $
// Copyright:   (c) 2006-2007 Kia Emilien
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef ADDRESS_H_
#define ADDRESS_H_

#include "wx/avahi/avahidef.h"

#include <avahi-common/address.h>
#include <avahi-client/publish.h>
#include <avahi-client/lookup.h>

#include <wx/wx.h>
#include <wx/socket.h>

/**
 * Test to know if wxAvahi is present.
 * Do nothing.
 * Can be used in configure.ac to test if wxAvahi is linkable.
 * \return wxAvahi numeric version.
 */
WXDLLIMPEXP_AVAHI int wxAvahi();

/**
 * Test to know if wxAvahiGui is present.
 * Do nothing.
 * Can be used in configure.ac to test if wxAvahiGui is linkable.
 * \return wxAvahi numeric version.
 */ 
WXDLLIMPEXP_AVAHI int wxAvahiGui();


/** 
 * Convert an Avahi address to a readable string.
 * Usable with wxIPAdresses.
 */
WXDLLIMPEXP_AVAHI wxString wxAvahiAddressToString(const AvahiAddress* avahiAddress);

/** 
 * Set an AvahiAddress from wxIPaddress
 */
WXDLLIMPEXP_AVAHI void wxAddressToAvahi(const wxIPaddress* addr, AvahiAddress* avahiAddress);


/**
 * Normalize a domain name into its canonical form.
 */
WXDLLIMPEXP_AVAHI wxString wxAvahiNormalizeName(const wxString& str);

/**
 * Test if two domain name are equal.
 */
WXDLLIMPEXP_AVAHI bool wxAvahiDomainEqual(const wxString& domain1, const wxString& domain2);

/**
 * Retrieve a hash integer for a domain.
 * Usefull to index domains.
 */
WXDLLIMPEXP_AVAHI unsigned wxAvahiDomainHash(const wxString& domain);

/**
 * Test if it is a valid generic service type.
 */
WXDLLIMPEXP_AVAHI bool wxAvahiValidServiceTypeGeneric(const wxString& service);

/**
 * Test if it is a valid strict service type.
 */
WXDLLIMPEXP_AVAHI bool wxAvahiValidServiceTypeStrict(const wxString& service);

/**
 * Test if it is a valid service subtype.
 */
WXDLLIMPEXP_AVAHI bool wxAvahiValidServiceSubtype(const wxString& subtype);

/**
 * Test if it is a valid domain name.
 */
WXDLLIMPEXP_AVAHI bool wxAvahiValidDomainName(const wxString& domain);

/**
 * Test if it is a valid service name.
 */
WXDLLIMPEXP_AVAHI bool wxAvahiValidServiceName(const wxString& service);

/**
 * Test if it is a valid host name.
 */
WXDLLIMPEXP_AVAHI bool wxAvahiValidHostName(const wxString& host);

/**
 * Test if it is a valid FQDN.
 */
WXDLLIMPEXP_AVAHI bool wxAvahiValidFQDN(const wxString& fqdn);

/**
 * Construct a service name by joining a name, a type and a domain.
 */
WXDLLIMPEXP_AVAHI wxString wxAvahiServiceNameJoin(const wxString& name, const wxString& type, const wxString& domain);

/**
 * Split a service name in a name, a type and a domain.
 */
WXDLLIMPEXP_AVAHI void wxAvahiServiceNameJoin(const wxString& service, wxString& name, wxString& type, wxString& domain);

/**
 * Retrieve a type from a subtype
 */
WXDLLIMPEXP_AVAHI wxString wxAvahiGetTypeFromSubtype(const wxString& subtype);

#endif /*ADDRESS_H_*/
