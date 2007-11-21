/////////////////////////////////////////////////////////////////////////////
// Name:        wx/avahi/browse.h
// Purpose:     Avahi browsing features
// Author:      Kia Emilien
// Created:     2006/10/20
// RCS-ID:      $Id: browsing.h,v 1.3 2005/10/21 16:42:58 frm Exp $
// Copyright:   (c) 2006-2007 Kia Emilien
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef BROWSE_H_
#define BROWSE_H_

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

class wxAvahiDomainBrowser;
class wxAvahiServiceTypeBrowser;
class wxAvahiServiceBrowser;
class wxAvahiRecordBrowser;

class wxAvahiDomainBrowserEvent;
class wxAvahiServiceTypeBrowserEvent;
class wxAvahiServiceBrowserEvent;
class wxAvahiRecordBrowserEvent;

/**
 * Domain browser.
 */
class WXDLLIMPEXP_AVAHI wxAvahiDomainBrowser : public wxEvtHandler{
private:
    AvahiDomainBrowser* m_browser;
    wxAvahiClient*   m_client;
protected:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxAvahiDomainBrowser)
public:
    wxAvahiDomainBrowser();
    wxAvahiDomainBrowser(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString& domain, AvahiDomainBrowserType btype, AvahiLookupFlags flags);
    virtual ~wxAvahiDomainBrowser();
    void Create(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString& domain, AvahiDomainBrowserType btype, AvahiLookupFlags flags);

    AvahiDomainBrowser* GetDomainBrowser()const{return m_browser;}
    wxAvahiClient* GetClient();
};

/**
 * Domain browser event
 * Event sent by wxAvahiDomainBrowser each time a new domain is available.
 */

class WXDLLIMPEXP_AVAHI wxAvahiDomainBrowserEvent: public wxEvent{
private:
    wxAvahiDomainBrowser* m_browser;
    AvahiIfIndex m_interface;
    AvahiProtocol m_protocol;
    AvahiBrowserEvent m_event;
    wxString m_domain;
    AvahiLookupResultFlags m_flags;
public:
    wxAvahiDomainBrowserEvent(const wxAvahiDomainBrowserEvent& event);
    wxAvahiDomainBrowserEvent(wxAvahiDomainBrowser* browser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const wxString& domain, AvahiLookupResultFlags flags);
    virtual ~wxAvahiDomainBrowserEvent();
    
    virtual wxEvent* Clone() const;
    
    wxAvahiDomainBrowser* GetDomainBrowser()const{return m_browser;}
    AvahiIfIndex    GetInterface()const{return m_interface;}
    AvahiProtocol   GetProtocol()const{return m_protocol;}
    AvahiBrowserEvent GetEvent()const{return m_event;}
    wxString        GetDomain()const{return m_domain;}
    AvahiLookupResultFlags GetFlag()const{return m_flags;}
};

DECLARE_EVENT_TYPE(wxEVT_AVAHI_DOMAIN_BROWSER_NEW, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_DOMAIN_BROWSER_REMOVE, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_DOMAIN_BROWSER_CACHE_EXHAUSTED, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_DOMAIN_BROWSER_ALL_FOR_NOW, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_DOMAIN_BROWSER_FAILURE, -1)

typedef void (wxEvtHandler::*wxAvahiClientDomainBrowserEventFunction)(wxAvahiDomainBrowserEvent&);

#define EVT_AVAHI_DOMAIN_BROWSER(fn, type) \
    DECLARE_EVENT_TABLE_ENTRY( type, -1, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( wxAvahiClientDomainBrowserEventFunction, & fn ), (wxObject *) NULL ),

#define EVT_AVAHI_DOMAIN_BROWSER_NEW(fn)             EVT_AVAHI_DOMAIN_BROWSER(fn, wxEVT_AVAHI_DOMAIN_BROWSER_NEW) 
#define EVT_AVAHI_DOMAIN_BROWSER_REMOVE(fn)          EVT_AVAHI_DOMAIN_BROWSER(fn, wxEVT_AVAHI_DOMAIN_BROWSER_REMOVE) 
#define EVT_AVAHI_DOMAIN_BROWSER_CACHE_EXHAUSTED(fn) EVT_AVAHI_DOMAIN_BROWSER(fn, wxEVT_AVAHI_DOMAIN_BROWSER_CACHE_EXHAUSTED) 
#define EVT_AVAHI_DOMAIN_BROWSER_ALL_FOR_NOW(fn)     EVT_AVAHI_DOMAIN_BROWSER(fn, wxEVT_AVAHI_DOMAIN_BROWSER_ALL_FOR_NOW) 
#define EVT_AVAHI_DOMAIN_BROWSER_FAILURE(fn)         EVT_AVAHI_DOMAIN_BROWSER(fn, wxEVT_AVAHI_DOMAIN_BROWSER_FAILURE) 

#define EVT_AVAHI_DOMAIN_BROWSER_ALL(fn) \
    EVT_AVAHI_DOMAIN_BROWSER_NEW(fn) \
    EVT_AVAHI_DOMAIN_BROWSER_REMOVE(fn) \
    EVT_AVAHI_DOMAIN_BROWSER_CACHE_EXHAUSTED(fn) \
    EVT_AVAHI_DOMAIN_BROWSER_ALL_FOR_NOW(fn) \
    EVT_AVAHI_DOMAIN_BROWSER_FAILURE(fn)


/**
 * Service type browser
 */
class WXDLLIMPEXP_AVAHI wxAvahiServiceTypeBrowser : public wxEvtHandler{
private:
    AvahiServiceTypeBrowser* m_browser;
    wxAvahiClient*   m_client;
protected:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxAvahiDomainBrowser)
public:
    wxAvahiServiceTypeBrowser();
    wxAvahiServiceTypeBrowser(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &domain, AvahiLookupFlags flags);
    virtual ~wxAvahiServiceTypeBrowser();
    void Create(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &domain, AvahiLookupFlags flags);
    
    AvahiServiceTypeBrowser* GetServiceTypeBrowser()const{return m_browser;}
    wxAvahiClient* GetClient();
};

/**
 * Service type browser event
 * Event sent by wxAvahiServiceTypeBrowser each time a new service type is available.
 */
class WXDLLIMPEXP_AVAHI wxAvahiServiceTypeBrowserEvent: public wxEvent{
private:
    wxAvahiServiceTypeBrowser* m_browser;
    AvahiIfIndex m_interface;
    AvahiProtocol m_protocol;
    AvahiBrowserEvent m_event;
    wxString m_type;
    wxString m_domain;
    AvahiLookupResultFlags m_flags;
public:
    wxAvahiServiceTypeBrowserEvent(const wxAvahiServiceTypeBrowserEvent& event);
    wxAvahiServiceTypeBrowserEvent(wxAvahiServiceTypeBrowser* browser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const wxString& type, const wxString &domain, AvahiLookupResultFlags flags);
    virtual ~wxAvahiServiceTypeBrowserEvent();
    
    virtual wxEvent* Clone() const;
    
    wxAvahiServiceTypeBrowser* GetServiceTypeBrowser()const{return m_browser;}
    AvahiIfIndex    GetInterface()const{return m_interface;}
    AvahiProtocol   GetProtocol()const{return m_protocol;}
    AvahiBrowserEvent GetEvent()const{return m_event;}
    wxString        GetType()const{return m_type;}
    wxString        GetDomain()const{return m_domain;}
    AvahiLookupResultFlags GetFlag()const{return m_flags;}
};

DECLARE_EVENT_TYPE(wxEVT_AVAHI_SERVICE_TYPE_BROWSER_NEW, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_SERVICE_TYPE_BROWSER_REMOVE, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_SERVICE_TYPE_BROWSER_CACHE_EXHAUSTED, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_SERVICE_TYPE_BROWSER_ALL_FOR_NOW, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_SERVICE_TYPE_BROWSER_FAILURE, -1)

typedef void (wxEvtHandler::*wxAvahiClientServiceTypeBrowserEventFunction)(wxAvahiServiceTypeBrowserEvent&);

#define EVT_AVAHI_SERVICE_TYPE_BROWSER(fn, type) \
    DECLARE_EVENT_TABLE_ENTRY( type, -1, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( wxAvahiClientServiceTypeBrowserEventFunction, & fn ), (wxObject *) NULL ),

#define EVT_AVAHI_SERVICE_TYPE_BROWSER_NEW(fn)               EVT_AVAHI_SERVICE_TYPE_BROWSER(fn, wxEVT_AVAHI_SERVICE_TYPE_BROWSER_NEW) 
#define EVT_AVAHI_SERVICE_TYPE_BROWSER_REMOVE(fn)            EVT_AVAHI_SERVICE_TYPE_BROWSER(fn, wxEVT_AVAHI_SERVICE_TYPE_BROWSER_REMOVE) 
#define EVT_AVAHI_SERVICE_TYPE_BROWSER_CACHE_EXHAUSTED(fn)   EVT_AVAHI_SERVICE_TYPE_BROWSER(fn, wxEVT_AVAHI_SERVICE_TYPE_BROWSER_CACHE_EXHAUSTED) 
#define EVT_AVAHI_SERVICE_TYPE_BROWSER_ALL_FOR_NOW(fn)       EVT_AVAHI_SERVICE_TYPE_BROWSER(fn, wxEVT_AVAHI_SERVICE_TYPE_BROWSER_ALL_FOR_NOW) 
#define EVT_AVAHI_SERVICE_TYPE_BROWSER_FAILURE(fn)           EVT_AVAHI_SERVICE_TYPE_BROWSER(fn, wxEVT_AVAHI_SERVICE_TYPE_BROWSER_FAILURE) 

#define EVT_AVAHI_SERVICE_TYPE_BROWSER_ALL(fn) \
    EVT_AVAHI_SERVICE_TYPE_BROWSER_NEW(fn) \
    EVT_AVAHI_SERVICE_TYPE_BROWSER_REMOVE(fn) \
    EVT_AVAHI_SERVICE_TYPE_BROWSER_CACHE_EXHAUSTED(fn) \
    EVT_AVAHI_SERVICE_TYPE_BROWSER_ALL_FOR_NOW(fn) \
    EVT_AVAHI_SERVICE_TYPE_BROWSER_FAILURE(fn)


/**
 * Service browser
 */
class WXDLLIMPEXP_AVAHI wxAvahiServiceBrowser : public wxEvtHandler{
private:
    AvahiServiceBrowser* m_browser;
    wxAvahiClient*   m_client;
protected:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxAvahiServiceBrowser)
public:
    wxAvahiServiceBrowser();
    wxAvahiServiceBrowser(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &type, const wxString &domain, AvahiLookupFlags flags);    
    virtual ~wxAvahiServiceBrowser();
    virtual void Create(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &type, const wxString &domain, AvahiLookupFlags flags);
    
    AvahiServiceBrowser* GetServiceBrowser()const{return m_browser;}
    wxAvahiClient* GetClient();
};

/**
 * Service browser event
 * Event sent by wxAvahiServiceBrowser each time a new service is available.
 */
class WXDLLIMPEXP_AVAHI wxAvahiServiceBrowserEvent: public wxEvent{
private:
    wxAvahiServiceBrowser* m_browser;
    AvahiIfIndex m_interface;
    AvahiProtocol m_protocol;
    AvahiBrowserEvent m_event;
    wxString m_name;
    wxString m_type;
    wxString m_domain;
    AvahiLookupResultFlags m_flags;
public:
    wxAvahiServiceBrowserEvent(const wxAvahiServiceBrowserEvent& event);
    wxAvahiServiceBrowserEvent(wxAvahiServiceBrowser* browser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const wxString &name, const wxString &type, const wxString& domain, AvahiLookupResultFlags flags);
    virtual ~wxAvahiServiceBrowserEvent();
    
    virtual wxEvent* Clone() const;
    
    wxAvahiServiceBrowser* GetServiceBrowser()const{return m_browser;}
    AvahiIfIndex    GetInterface()const{return m_interface;}
    AvahiProtocol   GetProtocol()const{return m_protocol;}
    AvahiBrowserEvent GetEvent()const{return m_event;}
    wxString        GetName()const{return m_name;}
    wxString        GetType()const{return m_type;}
    wxString        GetDomain()const{return m_domain;}
    AvahiLookupResultFlags GetFlag()const{return m_flags;}
};

DECLARE_EVENT_TYPE(wxEVT_AVAHI_SERVICE_BROWSER_NEW, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_SERVICE_BROWSER_REMOVE, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_SERVICE_BROWSER_CACHE_EXHAUSTED, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_SERVICE_BROWSER_ALL_FOR_NOW, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_SERVICE_BROWSER_FAILURE, -1)

typedef void (wxEvtHandler::*wxAvahiClientServiceBrowserEventFunction)(wxAvahiServiceBrowserEvent&);

#define EVT_AVAHI_SERVICE_BROWSER(fn, type) \
    DECLARE_EVENT_TABLE_ENTRY( type, -1, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( wxAvahiClientServiceBrowserEventFunction, & fn ), (wxObject *) NULL ),

#define EVT_AVAHI_SERVICE_BROWSER_NEW(fn)               EVT_AVAHI_SERVICE_BROWSER(fn, wxEVT_AVAHI_SERVICE_BROWSER_NEW) 
#define EVT_AVAHI_SERVICE_BROWSER_REMOVE(fn)            EVT_AVAHI_SERVICE_BROWSER(fn, wxEVT_AVAHI_SERVICE_BROWSER_REMOVE) 
#define EVT_AVAHI_SERVICE_BROWSER_CACHE_EXHAUSTED(fn)   EVT_AVAHI_SERVICE_BROWSER(fn, wxEVT_AVAHI_SERVICE_BROWSER_CACHE_EXHAUSTED) 
#define EVT_AVAHI_SERVICE_BROWSER_ALL_FOR_NOW(fn)       EVT_AVAHI_SERVICE_BROWSER(fn, wxEVT_AVAHI_SERVICE_BROWSER_ALL_FOR_NOW) 
#define EVT_AVAHI_SERVICE_BROWSER_FAILURE(fn)           EVT_AVAHI_SERVICE_BROWSER(fn, wxEVT_AVAHI_SERVICE_BROWSER_FAILURE) 

#define EVT_AVAHI_SERVICE_BROWSER_ALL(fn) \
    EVT_AVAHI_SERVICE_BROWSER_NEW(fn) \
    EVT_AVAHI_SERVICE_BROWSER_REMOVE(fn) \
    EVT_AVAHI_SERVICE_BROWSER_CACHE_EXHAUSTED(fn) \
    EVT_AVAHI_SERVICE_BROWSER_ALL_FOR_NOW(fn) \
    EVT_AVAHI_SERVICE_BROWSER_FAILURE(fn)



/**
 * Record browser
 */
class WXDLLIMPEXP_AVAHI wxAvahiRecordBrowser : public wxEvtHandler{
private:
    AvahiRecordBrowser* m_browser;
    wxAvahiClient*   m_client;
protected:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxAvahiRecordBrowser)
public:
    wxAvahiRecordBrowser();
    wxAvahiRecordBrowser(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &name, uint16_t clazz, uint16_t type, AvahiLookupFlags flags);    
    virtual ~wxAvahiRecordBrowser();
    void Create(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &name, uint16_t clazz, uint16_t type, AvahiLookupFlags flags);
    
    AvahiRecordBrowser* GetBrowser()const{return m_browser;}
    wxAvahiClient* GetClient();
};

/**
 * Record browser event
 * Event sent by wxAvahiRecordBrowser each time a new record is available.
 */
class WXDLLIMPEXP_AVAHI wxAvahiRecordBrowserEvent: public wxEvent{
private:
    wxAvahiRecordBrowser* m_browser;
    AvahiIfIndex m_interface;
    AvahiProtocol m_protocol;
    AvahiBrowserEvent m_event;
    wxString m_name;
    uint16_t m_clazz;
    uint16_t m_type;
    const void *m_rdata;
    AvahiLookupResultFlags m_flags;
public:
    wxAvahiRecordBrowserEvent(const wxAvahiRecordBrowserEvent& event);
    wxAvahiRecordBrowserEvent(wxAvahiRecordBrowser* browser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const wxString &name, uint16_t clazz, uint16_t type, const void *rdata, size_t size, AvahiLookupResultFlags flags);
    virtual ~wxAvahiRecordBrowserEvent();
    
    virtual wxEvent* Clone() const;
    
    wxAvahiRecordBrowser* GetServiceBrowser()const{return m_browser;}
    AvahiIfIndex    GetInterface()const{return m_interface;}
    AvahiProtocol   GetProtocol()const{return m_protocol;}
    AvahiBrowserEvent GetEvent()const{return m_event;}
    wxString        GetName()const{return m_name;}
    uint16_t        GetClazz()const{return m_clazz;}
    uint16_t        GetType()const{return m_type;}
    const void *    GetData()const{return m_rdata;}
    AvahiLookupResultFlags GetFlag()const{return m_flags;}
};

DECLARE_EVENT_TYPE(wxEVT_AVAHI_RECORD_BROWSER_NEW, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_RECORD_BROWSER_REMOVE, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_RECORD_BROWSER_CACHE_EXHAUSTED, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_RECORD_BROWSER_ALL_FOR_NOW, -1)
DECLARE_EVENT_TYPE(wxEVT_AVAHI_RECORD_BROWSER_FAILURE, -1)

typedef void (wxEvtHandler::*wxAvahiClientRecordBrowserEventFunction)(wxAvahiRecordBrowserEvent&);

#define EVT_AVAHI_RECORD_BROWSER(fn, type) \
    DECLARE_EVENT_TABLE_ENTRY( type, -1, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( wxAvahiClientRecordBrowserEventFunction, & fn ), (wxObject *) NULL ),

#define EVT_AVAHI_RECORD_BROWSER_NEW(fn)               EVT_AVAHI_RECORD_BROWSER(fn, wxEVT_AVAHI_RECORD_BROWSER_NEW) 
#define EVT_AVAHI_RECORD_BROWSER_REMOVE(fn)            EVT_AVAHI_RECORD_BROWSER(fn, wxEVT_AVAHI_RECORD_BROWSER_REMOVE) 
#define EVT_AVAHI_RECORD_BROWSER_CACHE_EXHAUSTED(fn)   EVT_AVAHI_RECORD_BROWSER(fn, wxEVT_AVAHI_RECORD_BROWSER_CACHE_EXHAUSTED) 
#define EVT_AVAHI_RECORD_BROWSER_ALL_FOR_NOW(fn)       EVT_AVAHI_RECORD_BROWSER(fn, wxEVT_AVAHI_RECORD_BROWSER_ALL_FOR_NOW) 
#define EVT_AVAHI_RECORD_BROWSER_FAILURE(fn)           EVT_AVAHI_RECORD_BROWSER(fn, wxEVT_AVAHI_RECORD_BROWSER_FAILURE) 

#define EVT_AVAHI_RECORD_BROWSER_ALL(fn) \
    EVT_AVAHI_RECORD_BROWSER_NEW(fn) \
    EVT_AVAHI_RECORD_BROWSER_REMOVE(fn) \
    EVT_AVAHI_RECORD_BROWSER_CACHE_EXHAUSTED(fn) \
    EVT_AVAHI_RECORD_BROWSER_ALL_FOR_NOW(fn) \
    EVT_AVAHI_RECORD_BROWSER_FAILURE(fn)


#endif /*BROWSE_H_*/
