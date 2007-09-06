/////////////////////////////////////////////////////////////////////////////
// Name:        wx/avahi/client.h
// Purpose:     Avahi client.
// Author:      Kia Emilien
// Created:     2006/10/20
// RCS-ID:      $Id: client.h,v 1.3 2005/10/21 16:42:58 frm Exp $
// Copyright:   (c) 2006-2007 Kia Emilien
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef CLIENT_H_
#define CLIENT_H_

#include "wx/avahi/avahidef.h"

#include <avahi-client/client.h>
#include <avahi-client/publish.h>
#include <avahi-client/lookup.h>

#include <wx/wx.h>
#include <wx/event.h>

#include <wx/avahi/pool.h>
#include <wx/avahi/strlst.h>
#include <wx/avahi/utils.h>

class wxAvahiClient;
class wxAvahiEntryGroup;
class wxAvahiDomainBrowser;

class wxAvahiClientEvent;
class wxAvahiEntryGroupEvent;
class wxAvahiDomainBrowserEvent;

/**
 * Avahi client.
 * wxAvahiClient is the interface to dialog with avahi system.
 * \note Can receive wxAvahiClientEvent event.
 */
class WXDLLIMPEXP_AVAHI wxAvahiClient : public wxEvtHandler
{
private:
    AvahiClient* m_client;
protected:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxAvahiClient)
public:
    wxAvahiClient();
    virtual ~wxAvahiClient();
    
    virtual void Create(wxAvahiPool* pool, AvahiClientFlags flags, int* error);
    
    AvahiClient* GetClient()const{return m_client;}

    wxString GetVersion();
    wxString GetHostName();
    wxString GetDomainName();
    wxString GetFullQualifiedDomainName();
    AvahiClientState GetClientState();
    int GetError();
    int GetLocalServiceCookie();
    static bool HasNSSSupport();
    static wxString GetAlternativeHostName(const wxString& hostName);
};

/**
 * Event sent to wxAvahiClient when the status of avahi client changes.
 */
class WXDLLIMPEXP_AVAHI wxAvahiClientEvent: public wxEvent
{
protected:
    wxAvahiClient* m_avahiClient;
    AvahiClientState m_state;
public:
    wxAvahiClientEvent(const wxAvahiClientEvent& event);
    wxAvahiClientEvent(wxAvahiClient* client, AvahiClientState state);
    virtual ~wxAvahiClientEvent();
    
    virtual wxEvent* Clone() const;
    
    wxAvahiClient* GetClient()const;
    AvahiClientState GetState()const;
};

DECLARE_EVENT_TYPE(wxEVT_AVAHI_CLIENT_REGISTERING, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_CLIENT_RUNNING, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_CLIENT_COLLISION, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_CLIENT_FAILURE, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_CLIENT_CONNECTING, -1)

typedef void (wxEvtHandler::*wxAvahiClientEventFunction)(wxAvahiClientEvent&);

#define EVT_AVAHI_CLIENT(fn, type) \
    DECLARE_EVENT_TABLE_ENTRY( type, -1, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( wxAvahiClientEventFunction, & fn ), (wxObject *) NULL ),

#define EVT_AVAHI_REGISTERING(fn)    EVT_AVAHI_CLIENT(fn, wxEVT_AVAHI_REGISTERING) 
#define EVT_AVAHI_RUNNING(fn)        EVT_AVAHI_CLIENT(fn, wxEVT_AVAHI_RUNNING) 
#define EVT_AVAHI_COLLISION(fn)      EVT_AVAHI_CLIENT(fn, wxEVT_AVAHI_COLLISION) 
#define EVT_AVAHI_FAILURE(fn)        EVT_AVAHI_CLIENT(fn, wxEVT_AVAHI_FAILURE) 
#define EVT_AVAHI_CONNECTING(fn)     EVT_AVAHI_CLIENT(fn, wxEVT_AVAHI_CONNECTING) 

#define EVT_AVAHI_ALL(fn) \
    EVT_AVAHI_REGISTERING(fn) \
    EVT_AVAHI_RUNNING(fn) \
    EVT_AVAHI_COLLISION(fn) \
    EVT_AVAHI_FAILURE(fn) \
    EVT_AVAHI_CONNECTING(fn) 
	

/**
 * Avahi entry group (publish).
 * An entry group is the set of informations published over the network to make autoconfiguration. 
 */
class WXDLLIMPEXP_AVAHI wxAvahiEntryGroup : public wxEvtHandler{
private:
    AvahiEntryGroup* m_entryGroup;
    wxAvahiClient*   m_client;
protected:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxAvahiEntryGroup)
public:
    wxAvahiEntryGroup();
    wxAvahiEntryGroup(wxAvahiClient* client);
    virtual ~wxAvahiEntryGroup();
    void Create(wxAvahiClient* client);
    
    AvahiEntryGroup* GetEntryGroup()const{return m_entryGroup;}

    int Commit();
    int Reset();
    int GetState();
    int IsEmpty();
    wxAvahiClient* GetClient();
    int AddService(AvahiIfIndex interface, AvahiProtocol protocol, AvahiPublishFlags flags, const wxString& name, const wxString& type, const wxString& domain, const wxString& host, short port, wxAvahiStringList* strlst=NULL);
    int AddServiceSubtype(AvahiIfIndex interface, AvahiProtocol protocol, AvahiPublishFlags flags, const wxString& name, const wxString& type, const wxString& domain, const wxString& subtype);
    int AddAddress(AvahiIfIndex interface, AvahiProtocol protocol, AvahiPublishFlags flags, const wxString& name, const wxIPaddress& addr);
    int UpdateServiceTxt(AvahiIfIndex interface, AvahiProtocol protocol, AvahiPublishFlags flags, const wxString& name, const wxString& type, const wxString& domain, wxAvahiStringList* strlst=NULL);
	/** \todo add wrapping for avahi_entry_group_add_record but not documented in avahi doxygen. */
    
    static wxString GetAlternativeServiceName(const wxString& serviceName);
};


/**
 * Entry group event
 * Event sent to wxAvahiEntryGroup when its status changes.
 */
class WXDLLIMPEXP_AVAHI wxAvahiEntryGroupEvent: public wxEvent{
private:
	wxAvahiEntryGroup* m_pEntryGroup;
	AvahiEntryGroupState m_state;
public:
    wxAvahiEntryGroupEvent(const wxAvahiEntryGroupEvent& event);
    wxAvahiEntryGroupEvent(wxAvahiEntryGroup* group, AvahiEntryGroupState state);
    virtual ~wxAvahiEntryGroupEvent();
    
    virtual wxEvent* Clone() const;
    
    wxAvahiEntryGroup* GetEntryGroup()const;
    AvahiEntryGroupState GetState()const;
};

DECLARE_EVENT_TYPE(wxEVT_AVAHI_ENTRY_GROUP_UNCOMMITED, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_ENTRY_GROUP_REGISTERING, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_ENTRY_GROUP_ESTABLISHED, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_ENTRY_GROUP_COLLISION, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_ENTRY_GROUP_FAILURE, -1)

typedef void (wxEvtHandler::*wxAvahiClientEntryGroupEventFunction)(wxAvahiEntryGroupEvent&);

#define EVT_AVAHI_ENTRY_GROUP(fn, type) \
    DECLARE_EVENT_TABLE_ENTRY( type, -1, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( wxAvahiClientEntryGroupEventFunction, & fn ), (wxObject *) NULL ),

#define EVT_AVAHI_ENTRY_GROUP_UNCOMMITED(fn)     EVT_AVAHI_ENTRY_GROUP(fn, wxEVT_AVAHI_ENTRY_GROUP_UNCOMMITED) 
#define EVT_AVAHI_ENTRY_GROUP_REGISTERING(fn)    EVT_AVAHI_ENTRY_GROUP(fn, wxEVT_AVAHI_ENTRY_GROUP_REGISTERING) 
#define EVT_AVAHI_ENTRY_GROUP_ESTABLISHED(fn)    EVT_AVAHI_ENTRY_GROUP(fn, wxEVT_AVAHI_ENTRY_GROUP_ESTABLISHED) 
#define EVT_AVAHI_ENTRY_GROUP_COLLISION(fn)      EVT_AVAHI_ENTRY_GROUP(fn, wxEVT_AVAHI_ENTRY_GROUP_COLLISION) 
#define EVT_AVAHI_ENTRY_GROUP_FAILURE(fn)        EVT_AVAHI_ENTRY_GROUP(fn, wxEVT_AVAHI_ENTRY_GROUP_FAILURE) 

#define EVT_AVAHI_ENTRY_GROUP_ALL(fn) \
    EVT_AVAHI_ENTRY_GROUP_UNCOMMITED(fn) \
    EVT_AVAHI_ENTRY_GROUP_REGISTERING(fn) \
    EVT_AVAHI_ENTRY_GROUP_ESTABLISHED(fn) \
    EVT_AVAHI_ENTRY_GROUP_COLLISION(fn) \
    EVT_AVAHI_ENTRY_GROUP_FAILURE(fn) 


#endif /*CLIENT_H_*/
