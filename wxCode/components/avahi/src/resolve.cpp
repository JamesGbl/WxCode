/////////////////////////////////////////////////////////////////////////////
// Name:        resolve.cpp
// Purpose:     Avahi resolving features.
// Author:      Kia Emilien
// Created:     2006/10/20
// RCS-ID:      $Id: resolve.cpp,v 1.4 2005/10/21 16:42:58 frm Exp $
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

#include "wx/avahi/resolve.h"

//
// wxAvahiServiceResolver
//
void wxAvahiClientServiceResolver_Callback(AvahiServiceResolver *r, AvahiIfIndex interface, AvahiProtocol protocol, AvahiResolverEvent event, const char *name, const char *type, const char *domain, const char *host_name, const AvahiAddress *a, uint16_t port, AvahiStringList *txt, AvahiLookupResultFlags flags, void *userdata);

IMPLEMENT_DYNAMIC_CLASS(wxAvahiServiceResolver, wxEvtHandler)

BEGIN_EVENT_TABLE(wxAvahiServiceResolver, wxEvtHandler)
END_EVENT_TABLE()

wxAvahiServiceResolver::wxAvahiServiceResolver():wxEvtHandler(),
m_resolver(NULL),
m_client(NULL)
{
}

wxAvahiServiceResolver::~wxAvahiServiceResolver(){
    if(m_resolver!=NULL)
        avahi_service_resolver_free(m_resolver);
}

wxAvahiServiceResolver::wxAvahiServiceResolver(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &name, const wxString &type, const wxString &domain, AvahiProtocol aprotocol, AvahiLookupFlags flags):wxEvtHandler(),
m_resolver(NULL),
m_client(NULL)
{
    Create(client, interface, protocol, name, type, domain, aprotocol, flags);
}

void wxAvahiServiceResolver::Create(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &name, const wxString &type, const wxString &domain, AvahiProtocol aprotocol, AvahiLookupFlags flags){
    m_client = client;
    m_resolver = avahi_service_resolver_new(client->GetClient(), interface, protocol,
                        (const char*)name.mb_str(wxConvUTF8), (const char*)type.mb_str(wxConvUTF8),
                        (const char*)domain.mb_str(wxConvUTF8), aprotocol, flags, wxAvahiClientServiceResolver_Callback, (void*)this);
}

wxAvahiClient* wxAvahiServiceResolver::GetClient(){
    if(m_client->GetClient() == avahi_service_resolver_get_client(m_resolver))
        return m_client;
    else
        return NULL;
}

void wxAvahiClientServiceResolver_Callback(AvahiServiceResolver *r, AvahiIfIndex interface, AvahiProtocol protocol, AvahiResolverEvent event, const char *name, const char *type, const char *domain, const char *host_name, const AvahiAddress *a, uint16_t port, AvahiStringList *txt, AvahiLookupResultFlags flags, void *userdata){
    wxAvahiServiceResolver* resolver = (wxAvahiServiceResolver*)userdata;
    if(resolver!=NULL){
        wxAvahiServiceResolverEvent evt(resolver, interface, protocol, event, wxString(name, wxConvUTF8), wxString(type, wxConvUTF8), wxString(domain, wxConvUTF8), wxString(host_name, wxConvUTF8), wxAvahiAddressToString(a), port, txt, flags);
        evt.SetEventObject(resolver);
        resolver->ProcessEvent(evt);
    }
}

//
// wxAvahiServiceResolverEvent
//

DEFINE_EVENT_TYPE( wxEVT_AVAHI_SERVICE_RESOLVER_FOUND )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_SERVICE_RESOLVER_FAILURE )

wxAvahiServiceResolverEvent::wxAvahiServiceResolverEvent(const wxAvahiServiceResolverEvent& event):
wxEvent(event.GetEventType()),
m_resolver(event.m_resolver),
m_interface(event.m_interface),
m_protocol(event.m_protocol),
m_event(event.m_event),
m_name(event.m_name),
m_type(event.m_type),
m_domain(event.m_domain),
m_hostName(event.m_hostName),
m_a(event.m_a),
m_port(event.m_port),
m_strlst(event.m_strlst),
m_flags(event.m_flags)
{
}

wxAvahiServiceResolverEvent::wxAvahiServiceResolverEvent(wxAvahiServiceResolver* resolver, AvahiIfIndex interface, AvahiProtocol protocol, AvahiResolverEvent event, const wxString &name, const wxString &type, const wxString &domain, const wxString &host_name, const wxString& a, uint16_t port, AvahiStringList *txt, AvahiLookupResultFlags flags):
wxEvent(),
m_resolver(resolver),
m_interface(interface),
m_protocol(protocol),
m_event(event),
m_name(name),
m_type(type),
m_domain(domain),
m_hostName(host_name),
m_a(a),
m_port(port),
m_strlst(txt),
m_flags(flags)
{
    switch(event){
        case AVAHI_RESOLVER_FOUND:
            SetEventType(wxEVT_AVAHI_SERVICE_RESOLVER_FOUND);
            break;
        default:
            SetEventType(wxEVT_AVAHI_SERVICE_RESOLVER_FAILURE);
            break;
    }
}

wxAvahiServiceResolverEvent::~wxAvahiServiceResolverEvent(){
}

wxEvent* wxAvahiServiceResolverEvent::Clone() const{
    return new wxAvahiServiceResolverEvent(*this);
}

//
// wxAvahiHostNameResolver
//
void wxAvahiClientHostNameResolver_Callback(AvahiHostNameResolver *r, AvahiIfIndex interface, AvahiProtocol protocol, AvahiResolverEvent event, const char *name, const AvahiAddress *a, AvahiLookupResultFlags flags, void *userdata);

IMPLEMENT_DYNAMIC_CLASS(wxAvahiHostNameResolver, wxEvtHandler)

BEGIN_EVENT_TABLE(wxAvahiHostNameResolver, wxEvtHandler)
END_EVENT_TABLE()

wxAvahiHostNameResolver::wxAvahiHostNameResolver():wxEvtHandler(),
m_resolver(NULL),
m_client(NULL)
{
}

wxAvahiHostNameResolver::~wxAvahiHostNameResolver(){
    if(m_resolver!=NULL)
        avahi_host_name_resolver_free(m_resolver);
}

wxAvahiHostNameResolver::wxAvahiHostNameResolver(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &name, AvahiProtocol aprotocol, AvahiLookupFlags flags):wxEvtHandler(),
m_resolver(NULL),
m_client(NULL)
{
    Create(client, interface, protocol, name, aprotocol, flags);
}

void wxAvahiHostNameResolver::Create(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &name, AvahiProtocol aprotocol, AvahiLookupFlags flags){
    m_client = client;
    m_resolver = avahi_host_name_resolver_new(client->GetClient(), interface, protocol,
                        (const char*)name.mb_str(wxConvUTF8), aprotocol, flags, wxAvahiClientHostNameResolver_Callback, (void*)this);
}

wxAvahiClient* wxAvahiHostNameResolver::GetClient(){
    if(m_client->GetClient() == avahi_host_name_resolver_get_client(m_resolver))
        return m_client;
    else
        return NULL;
}

void wxAvahiClientHostNameResolver_Callback(AvahiHostNameResolver *r, AvahiIfIndex interface, AvahiProtocol protocol, AvahiResolverEvent event, const char *name, const AvahiAddress *a, AvahiLookupResultFlags flags, void *userdata){
    wxAvahiHostNameResolver* resolver = (wxAvahiHostNameResolver*)userdata;
    if(resolver!=NULL){
        wxAvahiHostNameResolverEvent evt(resolver, interface, protocol, event, wxString(name, wxConvUTF8), wxAvahiAddressToString(a), flags);
        evt.SetEventObject(resolver);
        resolver->ProcessEvent(evt);
    }
}

//
// wxAvahiHostNameResolverEvent
//

DEFINE_EVENT_TYPE( wxEVT_AVAHI_HOST_NAME_RESOLVER_FOUND )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_HOST_NAME_RESOLVER_FAILURE )

wxAvahiHostNameResolverEvent::wxAvahiHostNameResolverEvent(const wxAvahiHostNameResolverEvent& event):
wxEvent(event.GetEventType()),
m_resolver(event.m_resolver),
m_interface(event.m_interface),
m_protocol(event.m_protocol),
m_event(event.m_event),
m_name(event.m_name),
m_a(event.m_a),
m_flags(event.m_flags)
{
}

wxAvahiHostNameResolverEvent::wxAvahiHostNameResolverEvent(wxAvahiHostNameResolver* resolver, AvahiIfIndex interface, AvahiProtocol protocol, AvahiResolverEvent event, const wxString &name, const wxString& a, AvahiLookupResultFlags flags):
wxEvent(),
m_resolver(resolver),
m_interface(interface),
m_protocol(protocol),
m_event(event),
m_name(name),
m_a(a),
m_flags(flags)
{
    switch(event){
        case AVAHI_RESOLVER_FOUND:
            SetEventType(wxEVT_AVAHI_HOST_NAME_RESOLVER_FOUND);
            break;
        default:
            SetEventType(wxEVT_AVAHI_HOST_NAME_RESOLVER_FAILURE);
            break;
    }
}

wxAvahiHostNameResolverEvent::~wxAvahiHostNameResolverEvent(){
}

wxEvent* wxAvahiHostNameResolverEvent::Clone() const{
    return new wxAvahiHostNameResolverEvent(*this);
}


//
// wxAvahiAddressResolver
//
void wxAvahiClientAddressResolver_Callback(AvahiAddressResolver *r, AvahiIfIndex interface, AvahiProtocol protocol, AvahiResolverEvent event, const AvahiAddress *a, const char *name, AvahiLookupResultFlags flags, void *userdata);

IMPLEMENT_DYNAMIC_CLASS(wxAvahiAddressResolver, wxEvtHandler)

BEGIN_EVENT_TABLE(wxAvahiAddressResolver, wxEvtHandler)
END_EVENT_TABLE()

wxAvahiAddressResolver::wxAvahiAddressResolver():wxEvtHandler(),
m_resolver(NULL),
m_client(NULL)
{
}

wxAvahiAddressResolver::~wxAvahiAddressResolver(){
    if(m_resolver!=NULL)
        avahi_address_resolver_free(m_resolver);
}

wxAvahiAddressResolver::wxAvahiAddressResolver(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, wxIPaddress& a, AvahiLookupFlags flags):wxEvtHandler(),
m_resolver(NULL),
m_client(NULL)
{
    Create(client, interface, protocol, a, flags);
}

void wxAvahiAddressResolver::Create(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, wxIPaddress& a, AvahiLookupFlags flags){
    m_client = client;
    AvahiAddress addr;
    wxAddressToAvahi(&a, &addr);
    m_resolver = avahi_address_resolver_new(client->GetClient(), interface, protocol, &addr, flags, wxAvahiClientAddressResolver_Callback, (void*)this);
}

wxAvahiClient* wxAvahiAddressResolver::GetClient(){
    if(m_client->GetClient() == avahi_address_resolver_get_client(m_resolver))
        return m_client;
    else
        return NULL;
}

void wxAvahiClientAddressResolver_Callback(AvahiAddressResolver *r, AvahiIfIndex interface, AvahiProtocol protocol, AvahiResolverEvent event, const AvahiAddress *a, const char *name, AvahiLookupResultFlags flags, void *userdata){
    wxAvahiAddressResolver* resolver = (wxAvahiAddressResolver*)userdata;
    if(resolver!=NULL){
        wxAvahiAddressResolverEvent evt(resolver, interface, protocol, event, wxAvahiAddressToString(a), wxString(name, wxConvUTF8), flags);
        evt.SetEventObject(resolver);
        resolver->ProcessEvent(evt);
    }
}

//
// wxAvahiAddressResolverEvent
//

DEFINE_EVENT_TYPE( wxEVT_AVAHI_ADDRESS_RESOLVER_FOUND )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_ADDRESS_RESOLVER_FAILURE )

wxAvahiAddressResolverEvent::wxAvahiAddressResolverEvent(const wxAvahiAddressResolverEvent& event):
wxEvent(event.GetEventType()),
m_resolver(event.m_resolver),
m_interface(event.m_interface),
m_protocol(event.m_protocol),
m_event(event.m_event),
m_a(event.m_a),
m_name(event.m_name),
m_flags(event.m_flags)
{
}

wxAvahiAddressResolverEvent::wxAvahiAddressResolverEvent(wxAvahiAddressResolver* resolver, AvahiIfIndex interface, AvahiProtocol protocol, AvahiResolverEvent event, const wxString &a, const wxString &name, AvahiLookupResultFlags flags):
wxEvent(),
m_resolver(resolver),
m_interface(interface),
m_protocol(protocol),
m_event(event),
m_name(name),
m_a(a),
m_flags(flags)
{
    switch(event){
        case AVAHI_RESOLVER_FOUND:
            SetEventType(wxEVT_AVAHI_ADDRESS_RESOLVER_FOUND);
            break;
        default:
            SetEventType(wxEVT_AVAHI_ADDRESS_RESOLVER_FAILURE);
            break;
    }
}

wxAvahiAddressResolverEvent::~wxAvahiAddressResolverEvent(){
}

wxEvent* wxAvahiAddressResolverEvent::Clone() const{
    return new wxAvahiAddressResolverEvent(*this);
}

