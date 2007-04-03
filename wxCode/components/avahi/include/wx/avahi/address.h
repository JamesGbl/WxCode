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
 * Alloc wxIPaddress from AvahiAddress
 * Must delete it after use.
 */
WXDLLIMPEXP_AVAHI wxIPaddress* wxAvahiToWxAddress(const AvahiAddress* addr);

/**
 * Alloc AvahiAddress from wxIPaddress
 * Must delete it after use.
 */
WXDLLIMPEXP_AVAHI AvahiAddress* wxAddressToAvahi(wxIPaddress* addr);

/** 
 * Set an AvahiAddress from wxIPaddress
 */
WXDLLIMPEXP_AVAHI void wxAddressToAvahi(wxIPaddress* addr, AvahiAddress* avahiAddress);

#endif /*ADDRESS_H_*/
