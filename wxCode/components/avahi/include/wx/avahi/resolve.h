/////////////////////////////////////////////////////////////////////////////
// Name:        wx/avahi/resolve.h
// Purpose:     Avahi resolving features.
// Author:      Kia Emilien
// Created:     2006/10/20
// RCS-ID:      $Id: resolve.h,v 1.3 2005/10/21 16:42:58 frm Exp $
// Copyright:   (c) 2006-2007 Kia Emilien
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef RESOLVE_H_
#define RESOLVE_H_

#include "wx/avahi/avahidef.h"

#include <avahi-client/client.h>
#include <avahi-client/publish.h>
#include <avahi-client/lookup.h>

#include <wx/wx.h>
#include <wx/event.h>

#include <wx/avahi/pool.h>
#include <wx/avahi/strlst.h>
#include <wx/avahi/utils.h>
#include <wx/avahi/client.h>

class wxAvahiServiceResolver;
class wxAvahiHostNameResolver;
class wxAvahiAddressResolver;

class wxAvahiServiceResolverEvent;
class wxAvahiHostNameResolverEvent;
class wxAvahiAddressResolverEvent;


/**
 * Service resolver
 */
class WXDLLIMPEXP_AVAHI wxAvahiServiceResolver : public wxEvtHandler{
private:
    AvahiServiceResolver* m_resolver;
    wxAvahiClient*   m_client;
protected:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxAvahiServiceResolver)
public:
    wxAvahiServiceResolver();
    wxAvahiServiceResolver(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &name, const wxString &type, const wxString &domain, AvahiProtocol aprotocol, AvahiLookupFlags flags);    
    virtual ~wxAvahiServiceResolver();
    void Create(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &name, const wxString &type, const wxString &domain, AvahiProtocol protocol, AvahiLookupFlags flags);
    
    AvahiServiceResolver* GetServiceResolver()const{return m_resolver;}
    wxAvahiClient* GetClient();
};


/**
 * Service resolver event
 * Event sent by wxAvahiServiceResolver each time a service is resolved.
 */
class WXDLLIMPEXP_AVAHI wxAvahiServiceResolverEvent: public wxEvent{
private:
    wxAvahiServiceResolver* m_resolver;
    AvahiIfIndex m_interface;
    AvahiProtocol m_protocol;
    AvahiResolverEvent m_event;
    wxString m_name;
    wxString m_type;
    wxString m_domain;
    wxString m_hostName;
    wxString m_a;
    uint16_t m_port;
    wxAvahiStringList m_strlst;
    AvahiLookupResultFlags m_flags;
public:
    wxAvahiServiceResolverEvent(const wxAvahiServiceResolverEvent& event);
    wxAvahiServiceResolverEvent(wxAvahiServiceResolver* resolver, AvahiIfIndex interface, AvahiProtocol protocol, AvahiResolverEvent event, const wxString &name, const wxString &type, const wxString &domain, const wxString &host_name, const wxString& a, uint16_t port, AvahiStringList *txt, AvahiLookupResultFlags flags);
    virtual ~wxAvahiServiceResolverEvent();
    
    virtual wxEvent* Clone() const;
    
    wxAvahiServiceResolver* GetServiceResolver()const{return m_resolver;}
    AvahiIfIndex        GetInterface()const{return m_interface;}
    AvahiProtocol       GetProtocol()const{return m_protocol;}
    AvahiResolverEvent  GetEvent()const{return m_event;}
    wxString            GetName()const{return m_name;}
    wxString            GetType()const{return m_type;}
    wxString            GetDomain()const{return m_domain;}
    wxString            GetHostName()const{return m_hostName;}
    wxString            GetAddress()const{return m_a;}
    uint16_t            GetPort()const{return m_port;}
    wxAvahiStringList   GetStringList()const{return m_strlst;}
    AvahiLookupResultFlags GetFlag()const{return m_flags;}
};

DECLARE_EVENT_TYPE(wxEVT_AVAHI_SERVICE_RESOLVER_FOUND, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_SERVICE_RESOLVER_FAILURE, -1)

typedef void (wxEvtHandler::*wxAvahiClientServiceResolverEventFunction)(wxAvahiServiceResolverEvent&);

#define EVT_AVAHI_SERVICE_RESOLVER(fn, type) \
    DECLARE_EVENT_TABLE_ENTRY( type, -1, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( wxAvahiClientServiceResolverEventFunction, & fn ), (wxObject *) NULL ),

#define EVT_AVAHI_SERVICE_RESOLVER_FOUND(fn)     EVT_AVAHI_SERVICE_RESOLVER(fn, wxEVT_AVAHI_SERVICE_RESOLVER_FOUND) 
#define EVT_AVAHI_SERVICE_RESOLVER_FAILURE(fn)   EVT_AVAHI_SERVICE_RESOLVER(fn, wxEVT_AVAHI_SERVICE_RESOLVER_FAILURE) 

#define EVT_AVAHI_SERVICE_RESOLVER_ALL(fn) \
    EVT_AVAHI_SERVICE_RESOLVER_FOUND(fn) \
    EVT_AVAHI_SERVICE_RESOLVER_FAILURE(fn)


/**
 * Host name resolver
 */
class WXDLLIMPEXP_AVAHI wxAvahiHostNameResolver : public wxEvtHandler{
private:
    AvahiHostNameResolver* m_resolver;
    wxAvahiClient*   m_client;
protected:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxAvahiHostNameResolver)
public:
    wxAvahiHostNameResolver();
    wxAvahiHostNameResolver(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &name, AvahiProtocol aprotocol, AvahiLookupFlags flags);    
    virtual ~wxAvahiHostNameResolver();
    void Create(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &name, AvahiProtocol aprotocol, AvahiLookupFlags flags);
    
    AvahiHostNameResolver* GetServiceResolver()const{return m_resolver;}
    wxAvahiClient* GetClient();
};

/**
 * Host name resolver event
 * Event sent by wxAvahiHostNameResolver each time an host name is resolved.
 */
class WXDLLIMPEXP_AVAHI wxAvahiHostNameResolverEvent: public wxEvent{
private:
    wxAvahiHostNameResolver* m_resolver;
    AvahiIfIndex m_interface;
    AvahiProtocol m_protocol;
    AvahiResolverEvent m_event;
    wxString m_name;
    wxString m_a;
    AvahiLookupResultFlags m_flags;
public:
    wxAvahiHostNameResolverEvent(const wxAvahiHostNameResolverEvent& event);
    wxAvahiHostNameResolverEvent(wxAvahiHostNameResolver* resolver, AvahiIfIndex interface, AvahiProtocol protocol, AvahiResolverEvent event, const wxString &name, const wxString& a, AvahiLookupResultFlags flags);
    virtual ~wxAvahiHostNameResolverEvent();
    
    virtual wxEvent* Clone() const;
    
    wxAvahiHostNameResolver* GetResolver()const{return m_resolver;}
    AvahiIfIndex        GetInterface()const{return m_interface;}
    AvahiProtocol       GetProtocol()const{return m_protocol;}
    AvahiResolverEvent  GetEvent()const{return m_event;}
    wxString            GetName()const{return m_name;}
    wxString            GetAddress()const{return m_a;}
    AvahiLookupResultFlags GetFlag()const{return m_flags;}
};

DECLARE_EVENT_TYPE(wxEVT_AVAHI_HOST_NAME_RESOLVER_FOUND, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_HOST_NAME_RESOLVER_FAILURE, -1)

typedef void (wxEvtHandler::*wxAvahiClientHostNameResolverEventFunction)(wxAvahiHostNameResolverEvent&);

#define EVT_AVAHI_HOST_NAME_RESOLVER(fn, type) \
    DECLARE_EVENT_TABLE_ENTRY( type, -1, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( wxAvahiClientHostNameResolverEventFunction, & fn ), (wxObject *) NULL ),

#define EVT_AVAHI_HOST_NAME_RESOLVER_FOUND(fn)     EVT_AVAHI_HOST_NAME_RESOLVER(fn, wxEVT_AVAHI_HOST_NAME_RESOLVER_FOUND) 
#define EVT_AVAHI_HOST_NAME_RESOLVER_FAILURE(fn)   EVT_AVAHI_HOST_NAME_RESOLVER(fn, wxEVT_AVAHI_HOST_NAME_RESOLVER_FAILURE)

#define EVT_AVAHI_HOST_NAME_RESOLVER_ALL(fn) \
    EVT_AVAHI_HOST_NAME_RESOLVER_FOUND(fn) \
    EVT_AVAHI_HOST_NAME_RESOLVER_FAILURE(fn)


/**
 * Address resolver
 */
class WXDLLIMPEXP_AVAHI wxAvahiAddressResolver : public wxEvtHandler{
private:
    AvahiAddressResolver* m_resolver;
    wxAvahiClient*   m_client;
protected:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxAvahiAddressResolver)
public:
    wxAvahiAddressResolver();
    wxAvahiAddressResolver(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, wxIPaddress &a, AvahiLookupFlags flags);    
    virtual ~wxAvahiAddressResolver();
    virtual void Create(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, wxIPaddress &a, AvahiLookupFlags flags);
    
    AvahiAddressResolver* GetServiceResolver()const{return m_resolver;}
    wxAvahiClient* GetClient();
};

/**
 * Address resolver event
 * Event sent by wxAvahiAddressResolver each time an address is resolved.
 */
class WXDLLIMPEXP_AVAHI wxAvahiAddressResolverEvent: public wxEvent{
private:
    wxAvahiAddressResolver* m_resolver;
    AvahiIfIndex m_interface;
    AvahiProtocol m_protocol;
    AvahiResolverEvent m_event;
    wxString m_a;
    wxString m_name;
    AvahiLookupResultFlags m_flags;
public:
    wxAvahiAddressResolverEvent(const wxAvahiAddressResolverEvent& event);
    wxAvahiAddressResolverEvent(wxAvahiAddressResolver* resolver, AvahiIfIndex interface, AvahiProtocol protocol, AvahiResolverEvent event, const wxString &a, const wxString &name, AvahiLookupResultFlags flags);
    virtual ~wxAvahiAddressResolverEvent();
    
    virtual wxEvent* Clone() const;
    
    wxAvahiAddressResolver* GetResolver()const{return m_resolver;}
    AvahiIfIndex        GetInterface()const{return m_interface;}
    AvahiProtocol       GetProtocol()const{return m_protocol;}
    AvahiResolverEvent  GetEvent()const{return m_event;}
    wxString            GetName()const{return m_name;}
    wxString            GetAddress()const{return m_a;}
    AvahiLookupResultFlags GetFlag()const{return m_flags;}
};

DECLARE_EVENT_TYPE(wxEVT_AVAHI_ADDRESS_RESOLVER_FOUND, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_ADDRESS_RESOLVER_FAILURE, -1)

typedef void (wxEvtHandler::*wxAvahiClientAddressResolverEventFunction)(wxAvahiAddressResolverEvent&);

#define EVT_AVAHI_ADDRESS_RESOLVER(fn, type) \
    DECLARE_EVENT_TABLE_ENTRY( type, -1, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( wxAvahiClientAddressResolverEventFunction, & fn ), (wxObject *) NULL ),

#define EVT_AVAHI_ADDRESS_RESOLVER_FOUND(fn)     EVT_AVAHI_ADDRESS_RESOLVER(fn, wxEVT_AVAHI_ADDRESS_RESOLVER_FOUND) 
#define EVT_AVAHI_ADDRESS_RESOLVER_FAILURE(fn)   EVT_AVAHI_ADDRESS_RESOLVER(fn, wxEVT_AVAHI_ADDRESS_RESOLVER_FAILURE)

#define EVT_AVAHI_ADDRESS_RESOLVER_ALL(fn) \
    EVT_AVAHI_ADDRESS_RESOLVER_FOUND(fn) \
    EVT_AVAHI_ADDRESS_RESOLVER_FAILURE(fn)


#endif /*RESOLVE_H_*/
