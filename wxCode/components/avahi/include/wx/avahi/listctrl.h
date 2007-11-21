/////////////////////////////////////////////////////////////////////////////
// Name:        wx/avahi/listctrl.h
// Purpose:     Avahi wxListCtrl-based widgets.
// Author:      Kia Emilien
// Created:     2006/10/20
// RCS-ID:      $Id $
// Copyright:   (c) 2006-2007 Kia Emilien
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef LISTCTRL_H_
#define LISTCTRL_H_

#include "wx/avahi/avahidef.h"

#include <wx/wx.h>
#include <wx/event.h>
#include <wx/regex.h>

#include "wx/avahi/tools.h"
#include "wx/avahi/client.h"
#include "wx/avahi/browse.h"
#include "wx/avahi/typedb.h"

WX_DECLARE_STRING_HASH_MAP(int,  wxAvahiServiceTypeImageListItemMap);



WX_DECLARE_STRING_HASH_MAP(long, wxAvahiServiceTypeListItemMap);


/**
 * List control allowing to enumerate service types available on the network.
 * The list can filter result.
 * The list show the name of the service type (translated) or
 *  the service type if the name is not available in the database.
 * The list retrieve automatically the icon of the service type from wxArtProvider.
 */ 
class WXDLLIMPEXP_AVAHI wxAvahiServiceTypeListCtrl : public wxListCtrl, protected wxAvahiStringFilter
{
	DECLARE_DYNAMIC_CLASS(wxAvahiServiceTypeListCtrl)
	DECLARE_EVENT_TABLE()
public:
	/** Default constructor, use Create after it.*/
	wxAvahiServiceTypeListCtrl();
	/**
	 * Long constructor.
	 * \param parent Parent window address.
	 * \param id Window ID.
	 * \param client Avahi client address.
	 * \param filter Filter to apply to enumeration result, depend on filtertype.
	 * \param filtertype Type of filtering, @see FilterType.
	 * \param style Style of the list, @see  wxListCtrl.
	 * \param pos Default position of the widget.
	 * \param size Default size of the widget.
	 * \param validator Validator to use with this control.
	 * \param interface Avahi interface to use to browse.
	 * \param protocol Avahi protocol to use to browse.
	 * \param domain Avahi domain where to browse.
	 * \param flags Avahi browse flags.
	 * \param name Name of the widget.
	 */
	wxAvahiServiceTypeListCtrl(wxWindow* parent, wxWindowID id,
			wxAvahiClient* client, const wxString& filter=wxT(""), int filtertype = 0,
			const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
			long style = wxLC_ICON, const wxValidator& validator = wxDefaultValidator,
			AvahiIfIndex interface = AVAHI_IF_UNSPEC, AvahiProtocol protocol = AVAHI_PROTO_UNSPEC,
			const wxString &domain = wxT(""), AvahiLookupFlags flags = (AvahiLookupFlags)0,
			const wxString& name = wxListCtrlNameStr);
	virtual ~wxAvahiServiceTypeListCtrl();
	/**
	 * Create function, use it after default constructor.
	 * \param parent Parent window address.
	 * \param id Window ID.
	 * \param client Avahi client address.
	 * \param filter Filter to apply to enumeration result, depend on filtertype.
	 * \param filtertype Type of filtering, @see FilterType.
	 * \param style Style of the list, @see  wxListCtrl.
	 * \param pos Default position of the widget.
	 * \param size Default size of the widget.
	 * \param validator Validator to use with this control.
	 * \param interface Avahi interface to use to browse.
	 * \param protocol Avahi protocol to use to browse.
	 * \param domain Avahi domain where to browse.
	 * \param flags Avahi browse flags.
	 * \param name Name of the widget.
	 */
	virtual bool Create(wxWindow* parent, wxWindowID id,
			wxAvahiClient* client, const wxString& filter=wxT(""), int filtertype = 0,
			const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
			long style = wxLC_ICON, const wxValidator& validator = wxDefaultValidator,
			AvahiIfIndex interface = AVAHI_IF_UNSPEC, AvahiProtocol protocol = AVAHI_PROTO_UNSPEC,
			const wxString &domain = wxT(""), AvahiLookupFlags flags = (AvahiLookupFlags)0,
			const wxString& name = wxListCtrlNameStr);
	
	/** Change the language code of the shown service name. 
	 * en, de, fr ...*/
	virtual void SetLanguage(const wxString& lang);
	/** Retrieve the current language code. */
	wxString GetLanguage()const{return m_language;}
	
	/**
	 * Change the style of the list.
	 * Use it to switch list to report/small icon/icon ...
	 */
	virtual void SetListMode(int mode);
	
protected:
	/**
	 * Service type browser assigned to the list control.*/ 
	class Browser : public wxAvahiServiceTypeBrowser
	{
	protected:
		wxAvahiServiceTypeListCtrl* m_ctrl;
	public:
		Browser(wxAvahiServiceTypeListCtrl* ctrl, wxAvahiClient* client,
			AvahiIfIndex interface = AVAHI_IF_UNSPEC, AvahiProtocol protocol = AVAHI_PROTO_UNSPEC,
			const wxString &domain = wxT(""), AvahiLookupFlags flags = (AvahiLookupFlags)0):
				wxAvahiServiceTypeBrowser(client, interface, protocol, domain, flags),
				m_ctrl(ctrl)
			{
			}
		virtual ~Browser(){}
		virtual bool ProcessEvent(wxEvent& event)
		{
			if(!wxAvahiServiceTypeBrowser::ProcessEvent(event))
				return m_ctrl->ProcessEvent(event);
			else
				return true;
		}
	};

	/** Create the browser attached to the control.
	 * Overide it to customize behavior.*/
	virtual Browser* CreateBrowser(wxAvahiClient* client, AvahiIfIndex interface = AVAHI_IF_UNSPEC, AvahiProtocol protocol = AVAHI_PROTO_UNSPEC,
			const wxString &domain = wxT(""), AvahiLookupFlags flags = (AvahiLookupFlags)0);

	/** Force the update of translated service names.*/
	virtual void UpdateLanguage();
	
	/** Create columns for report mode. 
	 * Overide it to customize report header. */ 
	virtual void CreateColumns();
	
	/** Register an image for a specific service type. */
	virtual int RegisterImage(const wxString& type, const wxIcon& icon, int which);
	/** Search the index of a registered image for a service type. */
	int GetImageIndex(const wxString& type)const;
	
private:
	/** Current language code. */ 
	wxString m_language;
	/** Assigned browser object.*/
	Browser* m_browser;
	
	wxImageList *m_ilSmall, *m_ilNormal;
	
	wxAvahiServiceTypeListItemMap m_map;
	wxAvahiServiceTypeImageListItemMap m_ilmap;
	
	void OnNewServiceType(wxAvahiServiceTypeBrowserEvent& event);
	void OnRemServiceType(wxAvahiServiceTypeBrowserEvent& event);
};


struct wxAvahiServiceListCtrlContent
{
	wxString name;
	wxString type;
	wxString domain;	
};

WX_DECLARE_HASH_MAP( int, wxAvahiServiceListCtrlContent,
                     wxIntegerHash, wxIntegerEqual, wxAvahiServiceContentMap);
WX_DECLARE_STRING_HASH_MAP(wxAvahiServiceBrowser*,  wxAvahiServiceBrowserMap);


/**
 * List control allowing to enumerate service providers available on the network.
 * The list can filter result.
 * The list retrieve automatically the icon of the service type from wxArtProvider.
 */ 
class WXDLLIMPEXP_AVAHI wxAvahiServiceListCtrl : public wxListCtrl, protected wxAvahiStringFilter
{
	DECLARE_DYNAMIC_CLASS(wxAvahiServiceListCtrl)
	DECLARE_EVENT_TABLE()
public:
	wxAvahiServiceListCtrl();
	virtual ~wxAvahiServiceListCtrl();
	/**
	 * Long constructor.
	 * \param parent Parent window address.
	 * \param id Window ID.
	 * \param client Avahi client address.
	 * \param filter Filter to apply to enumeration result, depend on filtertype.
	 * \param filtertype Type of filtering, @see FilterType.
	 * \param style Style of the list, @see  wxListCtrl.
	 * \param pos Default position of the widget.
	 * \param size Default size of the widget.
	 * \param validator Validator to use with this control.
	 * \param interface Avahi interface to use to browse.
	 * \param protocol Avahi protocol to use to browse.
	 * \param domain Avahi domain where to browse.
	 * \param flags Avahi browse flags.
	 * \param name Name of the widget.
	 */	
	wxAvahiServiceListCtrl(	wxWindow* parent, wxWindowID id,
				wxAvahiClient* client, const wxString& filter=wxT(""), int filtertype = 0,
				const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
				long style = wxLC_ICON, const wxValidator& validator = wxDefaultValidator,
				const wxString& name = wxListCtrlNameStr);

	/**
	 * Create function, use it after default constructor.
	 * \param parent Parent window address.
	 * \param id Window ID.
	 * \param client Avahi client address.
	 * \param filter Filter to apply to enumeration result, depend on filtertype.
	 * \param filtertype Type of filtering, @see FilterType.
	 * \param style Style of the list, @see  wxListCtrl.
	 * \param pos Default position of the widget.
	 * \param size Default size of the widget.
	 * \param validator Validator to use with this control.
	 * \param interface Avahi interface to use to browse.
	 * \param protocol Avahi protocol to use to browse.
	 * \param domain Avahi domain where to browse.
	 * \param flags Avahi browse flags.
	 * \param name Name of the widget.
	 */
	virtual bool Create(wxWindow* parent, wxWindowID id,
		wxAvahiClient* client, const wxString& filter=wxT(""), int filtertype = 0,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
		long style = wxLC_ICON, const wxValidator& validator = wxDefaultValidator,
		const wxString& name = wxListCtrlNameStr);

	/** Change the language code of the shown service name. 
	 * en, de, fr ...*/
	virtual void SetLanguage(const wxString& lang);
	/** Retrieve the current language code. */
	wxString GetLanguage()const{return m_language;}

	/** Add a service type to look-at.
	 * \param type Service type to look-at. 
	 * \param interface Avahi interface to use to browse.
	 * \param protocol Avahi protocol to use to browse.
	 * \param domain Avahi domain where to browse.
	 * \param flags Avahi browse flags.
	 */
	void AddServiceType(const wxString &type, 
			AvahiIfIndex interface = AVAHI_IF_UNSPEC, AvahiProtocol protocol = AVAHI_PROTO_UNSPEC,
			const wxString &domain = wxT(""), AvahiLookupFlags flags = (AvahiLookupFlags)0);

protected:
	/**
	 * Service type browser assigned to the list control.*/ 
	class Browser : public wxAvahiServiceBrowser
	{
	protected:
		wxAvahiServiceListCtrl* m_ctrl;
	public:
		Browser(wxAvahiServiceListCtrl* ctrl, wxAvahiClient* client, const wxString &type,
			AvahiIfIndex interface = AVAHI_IF_UNSPEC, AvahiProtocol protocol = AVAHI_PROTO_UNSPEC,
			const wxString &domain = wxT(""), AvahiLookupFlags flags = (AvahiLookupFlags)0):
				wxAvahiServiceBrowser(client, interface, protocol, type, domain, flags),
				m_ctrl(ctrl)
			{
			}
		virtual ~Browser(){}
		virtual bool ProcessEvent(wxEvent& event)
		{
			if(!wxAvahiServiceBrowser::ProcessEvent(event))
				return m_ctrl->ProcessEvent(event);
			else
				return true;
		}
	};

	/** Create the browser attached to the control.
	 * Overide it to customize behavior.*/
	virtual Browser* CreateBrowser(wxAvahiClient* client, const wxString &type,
			AvahiIfIndex interface = AVAHI_IF_UNSPEC, AvahiProtocol protocol = AVAHI_PROTO_UNSPEC,
			const wxString &domain = wxT(""), AvahiLookupFlags flags = (AvahiLookupFlags)0);

	/** Force the update of translated service names.*/
	virtual void UpdateLanguage();
	
	/** Create columns for report mode. 
	 * Overide it to customize report header. */ 
	virtual void CreateColumns();
	
	/** Register an image for a specific service type. */
	virtual int RegisterImage(const wxString& type, const wxIcon& icon, int which);
	/** Search the index of a registered image for a service type. */
	int GetImageIndex(const wxString& type)const;
	
private:
	/** Avahi client attached to this control.*/
	wxAvahiClient* m_client;
	/** Current language code. */ 
	wxString m_language;
	/** Map of browsers. */
	wxAvahiServiceBrowserMap m_browsers;
	/** Map of content .*/
	wxAvahiServiceContentMap m_map;

	wxAvahiServiceTypeImageListItemMap m_ilmap;
	
	void OnNewService(wxAvahiServiceBrowserEvent& event);
	void OnRemService(wxAvahiServiceBrowserEvent& event);
	
};

#endif /*LISTCTRL_H_*/
