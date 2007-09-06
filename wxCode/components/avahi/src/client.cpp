/////////////////////////////////////////////////////////////////////////////
// Name:        client.cpp
// Purpose:     Avahi client.
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
#endif

#include "wx/avahi/client.h"
#include <iostream>
#include <typeinfo>

#include "avahi-common/alternative.h"
#include "avahi-common/malloc.h"

//
// wxAvahiClient
//

void wxAvahiClient_AvahiClientCallback(AvahiClient *s, AvahiClientState state, void *userdata);

IMPLEMENT_DYNAMIC_CLASS(wxAvahiClient, wxEvtHandler)

BEGIN_EVENT_TABLE(wxAvahiClient, wxEvtHandler)
END_EVENT_TABLE()


wxAvahiClient::wxAvahiClient():wxEvtHandler(),
m_client(NULL){
}    

wxAvahiClient::~wxAvahiClient()
{
    if(m_client!=NULL)
        avahi_client_free(m_client);
}

void wxAvahiClient::Create(wxAvahiPool* pool, AvahiClientFlags flags, int* error)
{
    m_client = avahi_client_new(pool->GetPool(), flags,
            wxAvahiClient_AvahiClientCallback, (void*)this, error);
}


wxString wxAvahiClient::GetVersion(){
    return wxString(avahi_client_get_version_string(m_client), wxConvUTF8);
}

wxString wxAvahiClient::GetHostName(){
    return wxString(avahi_client_get_host_name(m_client), wxConvUTF8);
}

wxString wxAvahiClient::GetDomainName(){
    return wxString(avahi_client_get_domain_name(m_client), wxConvUTF8);
}

wxString wxAvahiClient::GetFullQualifiedDomainName(){
    return wxString(avahi_client_get_host_name_fqdn(m_client), wxConvUTF8);
}

AvahiClientState wxAvahiClient::GetClientState(){
    return avahi_client_get_state(m_client);
}

int wxAvahiClient::GetError(){
    return avahi_client_errno(m_client);
}

int wxAvahiClient::GetLocalServiceCookie(){
    return avahi_client_get_local_service_cookie(m_client);
}

bool wxAvahiClient::HasNSSSupport(){
    return avahi_nss_support()==1;
}
    
void wxAvahiClient_AvahiClientCallback(AvahiClient *s, AvahiClientState state, void *userdata){
    wxEvtHandler* client = (wxEvtHandler*) userdata;
    if(client!=NULL){
        wxAvahiClientEvent event((wxAvahiClient*)client, state);
        event.SetEventObject(client);
        client->ProcessEvent(event);
    }
}

wxString wxAvahiClient::GetAlternativeHostName(const wxString& hostName){
	char* temp = avahi_alternative_host_name((const char*)hostName.mb_str(wxConvUTF8));
	wxString str(temp, wxConvUTF8);
	avahi_free(temp);
	return str;
}


//
// wxAvahiClientEvent
//
DEFINE_EVENT_TYPE( wxEVT_AVAHI_CLIENT_REGISTERING )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_CLIENT_RUNNING )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_CLIENT_COLLISION )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_CLIENT_FAILURE )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_CLIENT_CONNECTING )

wxAvahiClientEvent::wxAvahiClientEvent(const wxAvahiClientEvent& event):
wxEvent(event.GetEventType()),
m_avahiClient(event.m_avahiClient),
m_state(event.m_state)
{
}

wxAvahiClientEvent::wxAvahiClientEvent(wxAvahiClient* client, AvahiClientState state):
wxEvent(),
m_avahiClient(client),
m_state(state)
{
    switch(state){
        case AVAHI_CLIENT_S_REGISTERING:
            SetEventType(wxEVT_AVAHI_CLIENT_REGISTERING);
            break;
        case AVAHI_CLIENT_S_RUNNING:
            SetEventType(wxEVT_AVAHI_CLIENT_RUNNING);
            break;
        case AVAHI_CLIENT_S_COLLISION:
            SetEventType(wxEVT_AVAHI_CLIENT_COLLISION);
            break;
        case AVAHI_CLIENT_CONNECTING:
            SetEventType(wxEVT_AVAHI_CLIENT_CONNECTING);
            break;
        default:
            SetEventType(wxEVT_AVAHI_CLIENT_FAILURE);
            break;
    }
}

wxAvahiClientEvent::~wxAvahiClientEvent(){
}

wxAvahiClient* wxAvahiClientEvent::GetClient()const{
    return m_avahiClient;
}

AvahiClientState wxAvahiClientEvent::GetState()const{
    return m_state;
}

wxEvent* wxAvahiClientEvent::Clone() const{
    return new wxAvahiClientEvent(*this);
}



//
// wxAvahiEntryGroup
//

void wxAvahiClientEntryGroup_Callback(AvahiEntryGroup *g, AvahiEntryGroupState state, void* userdata);

IMPLEMENT_DYNAMIC_CLASS(wxAvahiEntryGroup, wxEvtHandler)

BEGIN_EVENT_TABLE(wxAvahiEntryGroup, wxEvtHandler)
END_EVENT_TABLE()

wxAvahiEntryGroup::wxAvahiEntryGroup():
wxEvtHandler(),
m_entryGroup(NULL),
m_client(NULL){
}

wxAvahiEntryGroup::wxAvahiEntryGroup(wxAvahiClient* client):
wxEvtHandler(),
m_entryGroup(NULL),
m_client(NULL){
	Create(client);
}

wxAvahiEntryGroup::~wxAvahiEntryGroup(){
	if(m_entryGroup!=NULL)
		avahi_entry_group_free(m_entryGroup);
}

void wxAvahiEntryGroup::Create(wxAvahiClient* client){
	m_entryGroup = avahi_entry_group_new(client->GetClient(), 
		wxAvahiClientEntryGroup_Callback, (void*)this);
}

int wxAvahiEntryGroup::Commit(){
	return avahi_entry_group_commit(m_entryGroup);
}

int wxAvahiEntryGroup::Reset(){
	return avahi_entry_group_reset(m_entryGroup);
}

int wxAvahiEntryGroup::GetState(){
	return avahi_entry_group_get_state(m_entryGroup);
}

int wxAvahiEntryGroup::IsEmpty(){
	return avahi_entry_group_is_empty(m_entryGroup);
}

wxAvahiClient* wxAvahiEntryGroup::GetClient(){
	if(m_client->GetClient() == avahi_entry_group_get_client(m_entryGroup))
		return m_client;
	else
		return NULL;
}

int wxAvahiEntryGroup::AddService(AvahiIfIndex interface, AvahiProtocol protocol, AvahiPublishFlags flags, const wxString& name, const wxString& type, const wxString& domain, const wxString& host, short port, wxAvahiStringList* strlst){
	return avahi_entry_group_add_service_strlst(m_entryGroup, 
				interface, protocol, flags,
				name.mb_str(wxConvUTF8), type.mb_str(wxConvUTF8),
				(domain.IsEmpty()?NULL:(const char*)domain.mb_str(wxConvUTF8)), 
				(host.IsEmpty()?NULL:(const char*)host.mb_str(wxConvUTF8)),
    			port, (AvahiStringList*)((strlst!=NULL)?strlst->GetStringList():NULL));
}

int wxAvahiEntryGroup::AddServiceSubtype(AvahiIfIndex interface, AvahiProtocol protocol, AvahiPublishFlags flags, const wxString& name, const wxString& type, const wxString& domain, const wxString& subtype){
	return avahi_entry_group_add_service_subtype(m_entryGroup, 
				interface, protocol, flags,
				(const char*)name.mb_str(wxConvUTF8), (const char*)type.mb_str(wxConvUTF8),
				(domain.IsEmpty()?NULL:(const char*)domain.mb_str(wxConvUTF8)), 
				(const char*)subtype.mb_str(wxConvUTF8));
}

int wxAvahiEntryGroup::UpdateServiceTxt(AvahiIfIndex interface, AvahiProtocol protocol, AvahiPublishFlags flags, const wxString& name, const wxString& type, const wxString& domain, wxAvahiStringList* strlst){
	return avahi_entry_group_update_service_txt_strlst(m_entryGroup, 
				interface, protocol, flags,
				(const char*)name.mb_str(wxConvUTF8), (const char*)type.mb_str(wxConvUTF8),
				(domain.IsEmpty()?NULL:(const char*)domain.mb_str(wxConvUTF8)), 
				(AvahiStringList*)((strlst!=NULL)?strlst->GetStringList():NULL));
}

int wxAvahiEntryGroup::AddAddress(AvahiIfIndex interface, AvahiProtocol protocol, AvahiPublishFlags flags, const wxString& name, const wxIPaddress& addr)
{
	AvahiAddress a;
	wxAddressToAvahi(&addr, &a);
	return avahi_entry_group_add_address(m_entryGroup, 
				interface, protocol, flags,
				(const char*)name.mb_str(wxConvUTF8),
				&a);
}

void wxAvahiClientEntryGroup_Callback(AvahiEntryGroup *g, AvahiEntryGroupState state, void* userdata){
    wxAvahiEntryGroup* group = (wxAvahiEntryGroup*) userdata;
    if(group!=NULL){
        wxAvahiEntryGroupEvent event(group, state);
        event.SetEventObject(group);
        group->ProcessEvent(event);
    }
}

wxString wxAvahiEntryGroup::GetAlternativeServiceName(const wxString& serviceName){
	char* temp = avahi_alternative_service_name((const char*)serviceName.mb_str(wxConvUTF8));
	wxString str(temp, wxConvUTF8);
	avahi_free(temp);
	return str;
}

//
// wxAvahiEntryGroupEvent
//

DEFINE_EVENT_TYPE( wxEVT_AVAHI_ENTRY_GROUP_UNCOMMITED )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_ENTRY_GROUP_REGISTERING )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_ENTRY_GROUP_ESTABLISHED )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_ENTRY_GROUP_COLLISION )
DEFINE_EVENT_TYPE( wxEVT_AVAHI_ENTRY_GROUP_FAILURE )

wxAvahiEntryGroupEvent::wxAvahiEntryGroupEvent(const wxAvahiEntryGroupEvent& event):
wxEvent(event.GetEventType()),
m_pEntryGroup(event.m_pEntryGroup),
m_state(event.m_state)
{
}

wxAvahiEntryGroupEvent::wxAvahiEntryGroupEvent(wxAvahiEntryGroup* group, AvahiEntryGroupState state):
wxEvent(),
m_pEntryGroup(group),
m_state(state)
{
    switch(state){
        case AVAHI_ENTRY_GROUP_UNCOMMITED:
            SetEventType(wxEVT_AVAHI_ENTRY_GROUP_UNCOMMITED);
            break;
        case AVAHI_ENTRY_GROUP_REGISTERING:
            SetEventType(wxEVT_AVAHI_ENTRY_GROUP_REGISTERING);
            break;
        case AVAHI_ENTRY_GROUP_ESTABLISHED:
            SetEventType(wxEVT_AVAHI_ENTRY_GROUP_ESTABLISHED);
            break;
        case AVAHI_ENTRY_GROUP_COLLISION:
            SetEventType(wxEVT_AVAHI_ENTRY_GROUP_COLLISION);
            break;
        default:
            SetEventType(wxEVT_AVAHI_ENTRY_GROUP_FAILURE);
            break;
    }
}

wxAvahiEntryGroupEvent::~wxAvahiEntryGroupEvent(){
}

wxEvent* wxAvahiEntryGroupEvent::Clone() const{
	return new wxAvahiEntryGroupEvent(*this);
}

wxAvahiEntryGroup* wxAvahiEntryGroupEvent::GetEntryGroup()const{
	return m_pEntryGroup;
}

AvahiEntryGroupState wxAvahiEntryGroupEvent::GetState()const{
	return m_state;
}


