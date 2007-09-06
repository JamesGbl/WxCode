/////////////////////////////////////////////////////////////////////////////
// Name:        browse.cpp
// Purpose:     Avahi browsing features
// Author:      Kia Emilien
// Created:     2006/10/20
// RCS-ID:      $Id: browse.cpp,v 1.4 2005/10/21 16:42:58 frm Exp $
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

#include "wx/avahi/browse.h"

//
// wxAvahiDomainBrowser
//
void wxAvahiDomainBrowser_Callback(AvahiDomainBrowser *b, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *domain, AvahiLookupResultFlags flags, void *userdata);

IMPLEMENT_DYNAMIC_CLASS(wxAvahiDomainBrowser, wxEvtHandler)

BEGIN_EVENT_TABLE(wxAvahiDomainBrowser, wxEvtHandler)
END_EVENT_TABLE()

wxAvahiDomainBrowser::wxAvahiDomainBrowser():wxEvtHandler(),
m_browser(NULL),
m_client(NULL)
{
}

wxAvahiDomainBrowser::wxAvahiDomainBrowser(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString& domain, AvahiDomainBrowserType btype, AvahiLookupFlags flags):wxEvtHandler(),
m_browser(NULL),
m_client(NULL)
{
    Create(client, interface, protocol, domain, btype, flags);
}

wxAvahiDomainBrowser::~wxAvahiDomainBrowser(){
    if(m_browser!=NULL)
        avahi_domain_browser_free(m_browser);
}

void wxAvahiDomainBrowser::Create(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString& domain, AvahiDomainBrowserType btype, AvahiLookupFlags flags){
    m_browser = avahi_domain_browser_new(client->GetClient(),
            interface, protocol, (const char*)domain.mb_str(wxConvUTF8), btype, flags, wxAvahiDomainBrowser_Callback, this); 
}

wxAvahiClient* wxAvahiDomainBrowser::GetClient(){
    if(m_client->GetClient() == avahi_domain_browser_get_client(m_browser))
        return m_client;
    else
        return NULL;

}

void wxAvahiDomainBrowser_Callback(AvahiDomainBrowser *b, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *domain, AvahiLookupResultFlags flags, void *userdata){
    wxAvahiDomainBrowser* browser =(wxAvahiDomainBrowser*)userdata;
    if(browser!=NULL){
        wxAvahiDomainBrowserEvent evt(browser, interface, protocol, event, wxString(domain, wxConvUTF8), flags);
        evt.SetEventObject(browser);
        browser->ProcessEvent(evt);
    } 
}


//
// wxAvahiDomainBrowserEvent
//

DEFINE_EVENT_TYPE( wxEVT_AVAHI_DOMAIN_BROWSER_NEW )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_DOMAIN_BROWSER_REMOVE )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_DOMAIN_BROWSER_CACHE_EXHAUSTED )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_DOMAIN_BROWSER_ALL_FOR_NOW )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_DOMAIN_BROWSER_FAILURE )

wxAvahiDomainBrowserEvent::wxAvahiDomainBrowserEvent(const wxAvahiDomainBrowserEvent& event):
wxEvent(event.GetEventType()),
m_browser(event.m_browser),
m_interface(event.m_interface),
m_protocol(event.m_protocol),
m_event(event.m_event),
m_domain(event.m_domain),
m_flags(event.m_flags)
{
}

wxAvahiDomainBrowserEvent::wxAvahiDomainBrowserEvent(wxAvahiDomainBrowser* browser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const wxString &domain, AvahiLookupResultFlags flags):
wxEvent(),
m_browser(browser),
m_interface(interface),
m_protocol(protocol),
m_event(event),
m_domain(domain),
m_flags(flags)
{
    switch(event){
        case AVAHI_BROWSER_NEW:
            SetEventType(wxEVT_AVAHI_DOMAIN_BROWSER_NEW);
            break;
        case AVAHI_BROWSER_REMOVE:
            SetEventType(wxEVT_AVAHI_DOMAIN_BROWSER_REMOVE);
            break;
        case AVAHI_BROWSER_CACHE_EXHAUSTED:
            SetEventType(wxEVT_AVAHI_DOMAIN_BROWSER_CACHE_EXHAUSTED);
            break;
        case AVAHI_BROWSER_ALL_FOR_NOW:
            SetEventType(wxEVT_AVAHI_DOMAIN_BROWSER_ALL_FOR_NOW);
            break;
        default:
            SetEventType(wxEVT_AVAHI_DOMAIN_BROWSER_FAILURE);
            break;
    }
}

wxAvahiDomainBrowserEvent::~wxAvahiDomainBrowserEvent(){
}

wxEvent* wxAvahiDomainBrowserEvent::Clone() const{
    return new wxAvahiDomainBrowserEvent(*this);
}


//
// wxAvahiServiceTypeBrowser
//
void wxAvahiClientServiceTypeBrowser_Callback(AvahiServiceTypeBrowser *b, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *type, const char *domain, AvahiLookupResultFlags flags, void *userdata);

IMPLEMENT_DYNAMIC_CLASS(wxAvahiServiceTypeBrowser, wxEvtHandler)

BEGIN_EVENT_TABLE(wxAvahiServiceTypeBrowser, wxEvtHandler)
END_EVENT_TABLE()

wxAvahiServiceTypeBrowser::wxAvahiServiceTypeBrowser():wxEvtHandler(),
m_browser(NULL),
m_client(NULL)
{
}

wxAvahiServiceTypeBrowser::~wxAvahiServiceTypeBrowser(){
    if(m_browser!=NULL)
        avahi_service_type_browser_free(m_browser);
}

wxAvahiServiceTypeBrowser::wxAvahiServiceTypeBrowser(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &domain, AvahiLookupFlags flags):wxEvtHandler(),
m_browser(NULL),
m_client(NULL)
{
    Create(client, interface, protocol, domain, flags);
}

void wxAvahiServiceTypeBrowser::Create(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &domain, AvahiLookupFlags flags){
    m_client = client;
    m_browser = avahi_service_type_browser_new(client->GetClient(), interface, protocol, (const char*)domain.mb_str(wxConvUTF8), flags, wxAvahiClientServiceTypeBrowser_Callback, (void*)this);
}

wxAvahiClient* wxAvahiServiceTypeBrowser::GetClient(){
    if(m_client->GetClient() == avahi_service_type_browser_get_client(m_browser))
        return m_client;
    else
        return NULL;
}

void wxAvahiClientServiceTypeBrowser_Callback(AvahiServiceTypeBrowser *b, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *type, const char *domain, AvahiLookupResultFlags flags, void *userdata){
    wxAvahiServiceTypeBrowser* browser =(wxAvahiServiceTypeBrowser*)userdata;
    if(browser!=NULL){
        wxAvahiServiceTypeBrowserEvent evt(browser, interface, protocol, event, wxString(type, wxConvUTF8), wxString(domain, wxConvUTF8), flags);
        evt.SetEventObject(browser);
        browser->ProcessEvent(evt);
    } 
}


//
// wxAvahiServiceTypeBrowserEvent
//

DEFINE_EVENT_TYPE( wxEVT_AVAHI_SERVICE_TYPE_BROWSER_NEW )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_SERVICE_TYPE_BROWSER_REMOVE )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_SERVICE_TYPE_BROWSER_CACHE_EXHAUSTED )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_SERVICE_TYPE_BROWSER_ALL_FOR_NOW )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_SERVICE_TYPE_BROWSER_FAILURE )

wxAvahiServiceTypeBrowserEvent::wxAvahiServiceTypeBrowserEvent(const wxAvahiServiceTypeBrowserEvent& event):
wxEvent(event.GetEventType()),
m_browser(event.m_browser),
m_interface(event.m_interface),
m_protocol(event.m_protocol),
m_event(event.m_event),
m_type(event.m_type),
m_domain(event.m_domain),
m_flags(event.m_flags)
{
}

wxAvahiServiceTypeBrowserEvent::wxAvahiServiceTypeBrowserEvent(wxAvahiServiceTypeBrowser* browser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const wxString &type, const wxString &domain, AvahiLookupResultFlags flags):
wxEvent(),
m_browser(browser),
m_interface(interface),
m_protocol(protocol),
m_event(event),
m_type(type),
m_domain(domain),
m_flags(flags)
{
    switch(event){
        case AVAHI_BROWSER_NEW:
            SetEventType(wxEVT_AVAHI_SERVICE_TYPE_BROWSER_NEW);
            break;
        case AVAHI_BROWSER_REMOVE:
            SetEventType(wxEVT_AVAHI_SERVICE_TYPE_BROWSER_REMOVE);
            break;
        case AVAHI_BROWSER_CACHE_EXHAUSTED:
            SetEventType(wxEVT_AVAHI_SERVICE_TYPE_BROWSER_CACHE_EXHAUSTED);
            break;
        case AVAHI_BROWSER_ALL_FOR_NOW:
            SetEventType(wxEVT_AVAHI_SERVICE_TYPE_BROWSER_ALL_FOR_NOW);
            break;
        default:
            SetEventType(wxEVT_AVAHI_SERVICE_TYPE_BROWSER_FAILURE);
            break;
    }
}

wxAvahiServiceTypeBrowserEvent::~wxAvahiServiceTypeBrowserEvent(){
}

wxEvent* wxAvahiServiceTypeBrowserEvent::Clone() const{
    return new wxAvahiServiceTypeBrowserEvent(*this);
}

//
// wxAvahiServiceBrowser
//
void wxAvahiClientServiceBrowser_Callback(AvahiServiceBrowser *b, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *name, const char *type, const char *domain, AvahiLookupResultFlags flags, void *userdata);

IMPLEMENT_DYNAMIC_CLASS(wxAvahiServiceBrowser, wxEvtHandler)

BEGIN_EVENT_TABLE(wxAvahiServiceBrowser, wxEvtHandler)
END_EVENT_TABLE()


wxAvahiServiceBrowser::wxAvahiServiceBrowser():wxEvtHandler(),
m_browser(NULL),
m_client(NULL)
{
}

wxAvahiServiceBrowser::~wxAvahiServiceBrowser(){
    if(m_browser!=NULL)
        avahi_service_browser_free(m_browser);
}

wxAvahiServiceBrowser::wxAvahiServiceBrowser(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &type, const wxString &domain, AvahiLookupFlags flags):wxEvtHandler(),
m_browser(NULL),
m_client(NULL)
{
    Create(client, interface, protocol, type, domain, flags);
}

void wxAvahiServiceBrowser::Create(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &type, const wxString &domain, AvahiLookupFlags flags){
    m_client = client;
    m_browser = avahi_service_browser_new(client->GetClient(), interface, protocol, 
                            (const char*)type.mb_str(wxConvUTF8), (const char*)domain.mb_str(wxConvUTF8),
                            flags, wxAvahiClientServiceBrowser_Callback, (void*)this);
}

wxAvahiClient* wxAvahiServiceBrowser::GetClient(){
    if(m_client->GetClient() == avahi_service_browser_get_client(m_browser))
        return m_client;
    else
        return NULL;
}

void wxAvahiClientServiceBrowser_Callback(AvahiServiceBrowser *b, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *name, const char *type, const char *domain, AvahiLookupResultFlags flags, void *userdata){
    wxEvtHandler* handler = (wxEvtHandler*)  userdata;
    if(handler!=NULL){
        wxAvahiServiceBrowserEvent evt((wxAvahiServiceBrowser*)userdata, interface, protocol, event, wxString(name, wxConvUTF8), wxString(type, wxConvUTF8), wxString(domain, wxConvUTF8), flags);
        evt.SetEventObject(handler);
        handler->ProcessEvent(evt);
    }
}

//
// wxAvahiServiceBrowserEvent
//

DEFINE_EVENT_TYPE( wxEVT_AVAHI_SERVICE_BROWSER_NEW )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_SERVICE_BROWSER_REMOVE )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_SERVICE_BROWSER_CACHE_EXHAUSTED )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_SERVICE_BROWSER_ALL_FOR_NOW )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_SERVICE_BROWSER_FAILURE )

wxAvahiServiceBrowserEvent::wxAvahiServiceBrowserEvent(const wxAvahiServiceBrowserEvent& event):
wxEvent(event.GetEventType()),
m_browser(event.m_browser),
m_interface(event.m_interface),
m_protocol(event.m_protocol),
m_event(event.m_event),
m_name(event.m_name),
m_type(event.m_type),
m_domain(event.m_domain),
m_flags(event.m_flags)
{
}

wxAvahiServiceBrowserEvent::wxAvahiServiceBrowserEvent(wxAvahiServiceBrowser* browser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const wxString &name, const wxString &type, const wxString &domain, AvahiLookupResultFlags flags):
wxEvent(),
m_browser(browser),
m_interface(interface),
m_protocol(protocol),
m_event(event),
m_name(name),
m_type(type),
m_domain(domain),
m_flags(flags)
{
    switch(event){
        case AVAHI_BROWSER_NEW:
            SetEventType(wxEVT_AVAHI_SERVICE_BROWSER_NEW);
            break;
        case AVAHI_BROWSER_REMOVE:
            SetEventType(wxEVT_AVAHI_SERVICE_BROWSER_REMOVE);
            break;
        case AVAHI_BROWSER_CACHE_EXHAUSTED:
            SetEventType(wxEVT_AVAHI_SERVICE_BROWSER_CACHE_EXHAUSTED);
            break;
        case AVAHI_BROWSER_ALL_FOR_NOW:
            SetEventType(wxEVT_AVAHI_SERVICE_BROWSER_ALL_FOR_NOW);
            break;
        default:
            SetEventType(wxEVT_AVAHI_SERVICE_BROWSER_FAILURE);
            break;
    }
}

wxAvahiServiceBrowserEvent::~wxAvahiServiceBrowserEvent(){
}

wxEvent* wxAvahiServiceBrowserEvent::Clone() const{
    return new wxAvahiServiceBrowserEvent(*this);
}

//
// wxAvahiRecordBrowser
//
void wxAvahiClientRecordBrowser_Callback(AvahiRecordBrowser *b, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *name, uint16_t clazz, uint16_t type, const void *rdata, size_t size, AvahiLookupResultFlags flags, void *userdata);

IMPLEMENT_DYNAMIC_CLASS(wxAvahiRecordBrowser, wxEvtHandler)

BEGIN_EVENT_TABLE(wxAvahiRecordBrowser, wxEvtHandler)
END_EVENT_TABLE()

wxAvahiRecordBrowser::wxAvahiRecordBrowser():wxEvtHandler(),
m_browser(NULL),
m_client(NULL)
{
}

wxAvahiRecordBrowser::~wxAvahiRecordBrowser(){
    if(m_browser!=NULL)
        avahi_record_browser_free(m_browser);
}

wxAvahiRecordBrowser::wxAvahiRecordBrowser(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &name, uint16_t clazz, uint16_t type, AvahiLookupFlags flags):wxEvtHandler(),
m_browser(NULL),
m_client(NULL)
{
    Create(client, interface, protocol, name, clazz, type, flags);
}

void wxAvahiRecordBrowser::Create(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &name, uint16_t clazz, uint16_t type, AvahiLookupFlags flags){
    m_client = client;
    m_browser = avahi_record_browser_new(client->GetClient(), interface, protocol,
                        (const char*)name.mb_str(wxConvUTF8), clazz, type, flags, wxAvahiClientRecordBrowser_Callback, (void*)this);
}

wxAvahiClient* wxAvahiRecordBrowser::GetClient(){
    if(m_client->GetClient() == avahi_record_browser_get_client(m_browser))
        return m_client;
    else
        return NULL;
}

void wxAvahiClientRecordBrowser_Callback(AvahiRecordBrowser *b, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *name, uint16_t clazz, uint16_t type, const void *rdata, size_t size, AvahiLookupResultFlags flags, void *userdata){
    wxAvahiRecordBrowser* browser = (wxAvahiRecordBrowser*)userdata;
    if(browser!=NULL){
        wxAvahiRecordBrowserEvent evt(browser, interface, protocol, event, wxString(name, wxConvUTF8), clazz, type, rdata, size, flags);
        evt.SetEventObject(browser);
        browser->ProcessEvent(evt);
    }
}


//
// wxAvahiRecordBrowserEvent
//

DEFINE_EVENT_TYPE( wxEVT_AVAHI_RECORD_BROWSER_NEW )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_RECORD_BROWSER_REMOVE )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_RECORD_BROWSER_CACHE_EXHAUSTED )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_RECORD_BROWSER_ALL_FOR_NOW )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_RECORD_BROWSER_FAILURE )

wxAvahiRecordBrowserEvent::wxAvahiRecordBrowserEvent(const wxAvahiRecordBrowserEvent& event):
wxEvent(event.GetEventType()),
m_browser(event.m_browser),
m_interface(event.m_interface),
m_protocol(event.m_protocol),
m_event(event.m_event),
m_name(event.m_name),
m_clazz(event.m_clazz),
m_type(event.m_type),
m_rdata(event.m_rdata),
m_flags(event.m_flags)
{
}

wxAvahiRecordBrowserEvent::wxAvahiRecordBrowserEvent(wxAvahiRecordBrowser* browser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const wxString &name, uint16_t clazz, uint16_t type, const void *rdata, size_t size, AvahiLookupResultFlags flags):
wxEvent(),
m_browser(browser),
m_interface(interface),
m_protocol(protocol),
m_event(event),
m_name(name),
m_clazz(clazz),
m_type(type),
m_rdata(rdata),
m_flags(flags)
{
    switch(event){
        case AVAHI_BROWSER_NEW:
            SetEventType(wxEVT_AVAHI_RECORD_BROWSER_NEW);
            break;
        case AVAHI_BROWSER_REMOVE:
            SetEventType(wxEVT_AVAHI_RECORD_BROWSER_REMOVE);
            break;
        case AVAHI_BROWSER_CACHE_EXHAUSTED:
            SetEventType(wxEVT_AVAHI_RECORD_BROWSER_CACHE_EXHAUSTED);
            break;
        case AVAHI_BROWSER_ALL_FOR_NOW:
            SetEventType(wxEVT_AVAHI_RECORD_BROWSER_ALL_FOR_NOW);
            break;
        default:
            SetEventType(wxEVT_AVAHI_RECORD_BROWSER_FAILURE);
            break;
    }
}

wxAvahiRecordBrowserEvent::~wxAvahiRecordBrowserEvent(){
}

wxEvent* wxAvahiRecordBrowserEvent::Clone() const{
    return new wxAvahiRecordBrowserEvent(*this);
}
