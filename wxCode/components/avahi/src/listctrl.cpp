/////////////////////////////////////////////////////////////////////////////
// Name:        listctrl.cpp
// Purpose:     Avahi wxListCtrl-based widgets.
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
	#include <wx/imaglist.h>
	#include <wx/artprov.h>
	#include <wx/settings.h>
	#include <wx/tokenzr.h>
	#include <wx/regex.h>
#endif

// Compile wxAvahi*ListCtrl only if wanted
#if USE_GUI == 1

#include "wx/avahi/listctrl.h"

//
// Static tools
//

static wxIcon wxResizeIcon(const wxIcon& icon, wxSize sz, int quality = wxIMAGE_QUALITY_HIGH)
{
	if(icon.GetWidth()==sz.x && icon.GetHeight()==sz.y)
		return icon;
	else
	{
		wxImage img = icon.ConvertToImage();
		img.Rescale(sz.x, sz.y, quality);
		wxIcon res;
		wxBitmap bmp(img);
		res.CopyFromBitmap(bmp);
		return res;
	}
}

static inline int wxGetIconSizeX(int which, wxWindow* window=NULL)
{
	if(which==wxIMAGE_LIST_NORMAL)
		return wxSystemSettings::GetMetric(wxSYS_ICON_X, window)!=-1 ?
				wxSystemSettings::GetMetric(wxSYS_ICON_X, window) : 32;
	else
		return wxSystemSettings::GetMetric(wxSYS_SMALLICON_X , window)!=-1 ?
				wxSystemSettings::GetMetric(wxSYS_SMALLICON_X , window) : 16;
}

static inline int wxGetIconSizeY(int which, wxWindow* window=NULL)
{
	if(which==wxIMAGE_LIST_NORMAL)
		return wxSystemSettings::GetMetric(wxSYS_ICON_Y, window)!=-1 ?
				wxSystemSettings::GetMetric(wxSYS_ICON_Y, window) : 32;
	else
		return wxSystemSettings::GetMetric(wxSYS_SMALLICON_Y, window)!=-1 ?
				wxSystemSettings::GetMetric(wxSYS_SMALLICON_Y, window) : 16;
}

static inline wxSize wxGetIconSize(int which, wxWindow* window=NULL)
{
	return wxSize(wxGetIconSizeX(which, window), wxGetIconSizeY(which, window));
}

//
// wxAvahiServiceTypeListCtrl
//

IMPLEMENT_DYNAMIC_CLASS(wxAvahiServiceTypeListCtrl, wxListCtrl)

BEGIN_EVENT_TABLE(wxAvahiServiceTypeListCtrl, wxListCtrl)
	EVT_AVAHI_SERVICE_TYPE_BROWSER_NEW(wxAvahiServiceTypeListCtrl::OnNewServiceType)
	EVT_AVAHI_SERVICE_TYPE_BROWSER_REMOVE(wxAvahiServiceTypeListCtrl::OnRemServiceType)
END_EVENT_TABLE()

wxAvahiServiceTypeListCtrl::wxAvahiServiceTypeListCtrl():
	wxListCtrl(),
	wxAvahiStringFilter(),
	m_language(wxT("")),
	m_browser(NULL),
	m_ilSmall(NULL),
	m_ilNormal(NULL)
{
}

wxAvahiServiceTypeListCtrl::wxAvahiServiceTypeListCtrl(wxWindow* parent, wxWindowID id,
		wxAvahiClient* client, const wxString& filter, int filtertype,
		const wxPoint& pos, const wxSize& size,
		long style, const wxValidator& validator,
		AvahiIfIndex interface, AvahiProtocol protocol,
		const wxString &domain, AvahiLookupFlags flags,
		const wxString& name):
wxListCtrl(parent, id, pos, size, style, validator, name),
wxAvahiStringFilter(filter, filtertype),
m_language(wxT("")),
m_browser(CreateBrowser(client, interface, protocol, domain, flags))
{
	m_ilSmall  = new wxImageList(wxGetIconSizeX(wxIMAGE_LIST_SMALL, this),
			wxGetIconSizeY(wxIMAGE_LIST_SMALL, this));
	m_ilNormal = new wxImageList(wxGetIconSizeX(wxIMAGE_LIST_NORMAL, this),
			wxGetIconSizeY(wxIMAGE_LIST_NORMAL, this));
	AssignImageList(m_ilSmall, wxIMAGE_LIST_SMALL);
	AssignImageList(m_ilNormal, wxIMAGE_LIST_NORMAL);
	
	SetListMode(style);
}

wxAvahiServiceTypeListCtrl::~wxAvahiServiceTypeListCtrl()
{
	if(m_browser)
		delete m_browser;
}

bool wxAvahiServiceTypeListCtrl::Create(wxWindow* parent, wxWindowID id,
		wxAvahiClient* client, const wxString& filter, int filtertype,
		const wxPoint& pos, const wxSize& size,
		long style, const wxValidator& validator,
		AvahiIfIndex interface, AvahiProtocol protocol,
		const wxString &domain, AvahiLookupFlags flags,
		const wxString& name)
{
	SetFilter(filter, filtertype);
	
	if(!wxListCtrl::Create(parent, id, pos, size, style, validator, name))
		return false;

	m_ilSmall  = new wxImageList(wxGetIconSizeX(wxIMAGE_LIST_SMALL, this),
			wxGetIconSizeY(wxIMAGE_LIST_SMALL, this));
	m_ilNormal = new wxImageList(wxGetIconSizeX(wxIMAGE_LIST_NORMAL, this),
			wxGetIconSizeY(wxIMAGE_LIST_NORMAL, this));
	
	AssignImageList(m_ilSmall, wxIMAGE_LIST_SMALL);
	AssignImageList(m_ilNormal, wxIMAGE_LIST_NORMAL);
	
	SetListMode(style);
	
	m_browser = CreateBrowser(client, interface, protocol, domain, flags);
	return true;
}

void wxAvahiServiceTypeListCtrl::SetListMode(int mode)
{
	SetWindowStyleFlag(mode);

	if(mode&wxLC_REPORT)
		CreateColumns();
}


void wxAvahiServiceTypeListCtrl::CreateColumns()
{
	InsertColumn(0, wxT("Service name"));
	InsertColumn(1, wxT("Service type"));
}


wxAvahiServiceTypeListCtrl::Browser* wxAvahiServiceTypeListCtrl::CreateBrowser(
		wxAvahiClient* client, AvahiIfIndex interface,
		AvahiProtocol protocol,	const wxString &domain, AvahiLookupFlags flags)
{
	return new Browser(this, client, interface, protocol, domain, flags);
}

void wxAvahiServiceTypeListCtrl::SetLanguage(const wxString& lang)
{
	m_language = lang;	
	UpdateLanguage();
}

void wxAvahiServiceTypeListCtrl::UpdateLanguage()
{
    long item = -1;
    for ( ;; )
    {
        item = GetNextItem(item);
        if ( item == -1 )
            break;

        wxAvahiServiceTypeListItemMap::iterator it;
        for( it = m_map.begin(); it != m_map.end(); ++it )
        {
        	if(it->second==item)
        	{
        		SetItemText(item, wxAvahiServiceTypeDatabase::GetDatabase().GetServiceName(it->first, m_language));
        		break;
        	}
        }
    }
}

int wxAvahiServiceTypeListCtrl::RegisterImage(const wxString& type, const wxIcon& icon, int which)
{
	if(icon.IsOk() && icon.GetWidth()>0 && icon.GetHeight()>0)
	{
		int imgid =  GetImageIndex(type);
		wxImageList *il1, *il2;
		if(imgid==wxNOT_FOUND)
		{
			imgid = m_ilNormal->Add(wxResizeIcon(icon, wxGetIconSize(wxIMAGE_LIST_NORMAL, this)));
			m_ilSmall->Add(wxResizeIcon(icon, wxGetIconSize(wxIMAGE_LIST_SMALL, this)));
			m_ilmap[type] = imgid;
		}
		else if(which==wxIMAGE_LIST_NORMAL)
		{
			m_ilNormal->Replace(imgid, wxResizeIcon(icon, wxGetIconSize(wxIMAGE_LIST_NORMAL, this)));
		}
		else
		{
			m_ilSmall->Replace(imgid, wxResizeIcon(icon, wxGetIconSize(wxIMAGE_LIST_SMALL, this)));
		}
		return imgid;
	}	
	else
		return -1;
}

int wxAvahiServiceTypeListCtrl::GetImageIndex(const wxString& type)const
{
	wxAvahiServiceTypeImageListItemMap::const_iterator it = m_ilmap.find(type);
	if(it!=m_ilmap.end())
		return it->second;
	else
		return -1;
}

void wxAvahiServiceTypeListCtrl::OnNewServiceType(wxAvahiServiceTypeBrowserEvent& event)
{
	wxString type = event.GetType();
	
	if(!Test(type))
		return;
	
	wxString name = wxAvahiServiceTypeDatabase::GetDatabase().GetServiceName(type, m_language);
	int ilidx = GetImageIndex(type);
	if(ilidx == wxNOT_FOUND)
	{
		 wxIcon icon1 = wxArtProvider::GetIcon(wxT("zeroconf-servicetype-")+type, wxART_OTHER,
				 wxGetIconSize(wxIMAGE_LIST_NORMAL, this));
		 wxIcon icon2 = wxArtProvider::GetIcon(wxT("zeroconf-servicetype-")+type, wxART_OTHER,
				 wxGetIconSize(wxIMAGE_LIST_SMALL, this));
		 if(icon1.IsOk())
			 ilidx = RegisterImage(type, icon1, wxIMAGE_LIST_NORMAL);
		 if(icon2.IsOk())
			 ilidx = RegisterImage(type, icon2, wxIMAGE_LIST_SMALL);
	}
	if(name.IsEmpty())
		name = type;
	wxMutexGuiEnter();
	int id = InsertItem(GetItemCount(), name, ilidx);
	if(HasFlag(wxLC_REPORT))
		SetItem(id, 1, type);
	m_map[type] = id; 
	wxMutexGuiLeave();
}

void wxAvahiServiceTypeListCtrl::OnRemServiceType(wxAvahiServiceTypeBrowserEvent& event)
{
	wxMutexGuiEnter();
	DeleteItem(m_map[event.GetType()]);
	wxMutexGuiLeave();
	m_map.erase(event.GetType());
}



//
// wxAvahiServiceListCtrl
//

IMPLEMENT_DYNAMIC_CLASS(wxAvahiServiceListCtrl, wxListCtrl)

BEGIN_EVENT_TABLE(wxAvahiServiceListCtrl, wxListCtrl)
	EVT_AVAHI_SERVICE_BROWSER_NEW(wxAvahiServiceListCtrl::OnNewService)
	EVT_AVAHI_SERVICE_BROWSER_REMOVE(wxAvahiServiceListCtrl::OnRemService)
END_EVENT_TABLE()

wxAvahiServiceListCtrl::wxAvahiServiceListCtrl():
	wxListCtrl(),
	wxAvahiStringFilter(),
	m_language(wxT(""))
{
}

wxAvahiServiceListCtrl::wxAvahiServiceListCtrl(wxWindow* parent, wxWindowID id,
		wxAvahiClient* client, const wxString& filter, int filtertype,
		const wxPoint& pos, const wxSize& size,
		long style, const wxValidator& validator,
		const wxString& name):
wxListCtrl(parent, id, pos, size, style, validator, name),
wxAvahiStringFilter(filter, filtertype),
m_client(client),
m_language(wxT(""))
{
	AssignImageList(new wxImageList(wxGetIconSizeX(wxIMAGE_LIST_NORMAL, this),
			wxGetIconSizeY(wxIMAGE_LIST_NORMAL, this)), wxIMAGE_LIST_NORMAL);

	AssignImageList(new wxImageList(wxGetIconSizeX(wxIMAGE_LIST_SMALL, this),
			wxGetIconSizeY(wxIMAGE_LIST_SMALL, this)), wxIMAGE_LIST_SMALL);
	
	if(style&wxLC_REPORT)
		CreateColumns();
}

wxAvahiServiceListCtrl::~wxAvahiServiceListCtrl()
{
}

bool wxAvahiServiceListCtrl::Create(wxWindow* parent, wxWindowID id,
		wxAvahiClient* client, const wxString& filter, int filtertype,
		const wxPoint& pos, const wxSize& size,
		long style, const wxValidator& validator,
		const wxString& name)
{
	m_client = client;
	SetFilter(filter, filtertype);
	
	if(!wxListCtrl::Create(parent, id, pos, size, style, validator, name))
		return false;

	AssignImageList(new wxImageList(wxGetIconSizeX(wxIMAGE_LIST_NORMAL, this),
			wxGetIconSizeY(wxIMAGE_LIST_NORMAL, this)), wxIMAGE_LIST_NORMAL);

	AssignImageList(new wxImageList(wxGetIconSizeX(wxIMAGE_LIST_SMALL, this),
			wxGetIconSizeY(wxIMAGE_LIST_SMALL, this)), wxIMAGE_LIST_SMALL);

	if(style&wxLC_REPORT)
		CreateColumns();
	
	return true;
}


void wxAvahiServiceListCtrl::CreateColumns()
{
	InsertColumn(0, wxT("Host"));
	InsertColumn(1, wxT("Service name"));
	InsertColumn(2, wxT("Service type"));
}

void wxAvahiServiceListCtrl::AddServiceType(const wxString &type, 
		AvahiIfIndex interface, AvahiProtocol protocol,
		const wxString &domain, AvahiLookupFlags flags)
{
	wxAvahiServiceBrowserMap::iterator iter = m_browsers.find(type);
	if(iter==m_browsers.end())
	{
		m_browsers[type] = CreateBrowser(m_client, type, interface, protocol, domain, flags);
	}
}

wxAvahiServiceListCtrl::Browser* wxAvahiServiceListCtrl::CreateBrowser(wxAvahiClient* client, const wxString &type,
		AvahiIfIndex interface, AvahiProtocol protocol,
		const wxString &domain, AvahiLookupFlags flags)
{
	return new Browser(this, client, type, interface, protocol, domain, flags);
}

void wxAvahiServiceListCtrl::SetLanguage(const wxString& lang)
{
	m_language = lang;	
	UpdateLanguage();
}

void wxAvahiServiceListCtrl::UpdateLanguage()
{
	if(HasFlag(wxLC_REPORT))
	{
		long item = wxID_ANY;
	    for ( ;; )
	    {
	        item = GetNextItem(item);
	        if(item == wxNOT_FOUND)
	            break;
	        
	        SetItem(item, 1, wxAvahiServiceTypeDatabase::GetDatabase().GetServiceName(m_map[item].type, m_language));
	    }
	}
}

int wxAvahiServiceListCtrl::RegisterImage(const wxString& type, const wxIcon& icon, int which)
{
	if(icon.IsOk() && icon.GetWidth()>0 && icon.GetHeight()>0)
	{
		int imgid =  GetImageIndex(type);
		wxImageList *il1, *il2;
		if(imgid==wxNOT_FOUND)
		{
			il1 = GetImageList(wxIMAGE_LIST_NORMAL);
			il2 = GetImageList(wxIMAGE_LIST_SMALL);
			
			imgid = il1->Add(wxResizeIcon(icon, wxGetIconSize(wxIMAGE_LIST_NORMAL, this)));
			il2->Add(wxResizeIcon(icon, wxGetIconSize(wxIMAGE_LIST_SMALL, this)));
			m_ilmap[type] = imgid;
		}
		else if(which==wxIMAGE_LIST_NORMAL)
		{
			il1 = GetImageList(wxIMAGE_LIST_NORMAL);
			il1->Replace(imgid, wxResizeIcon(icon, wxGetIconSize(wxIMAGE_LIST_NORMAL, this)));
		}
		else
		{
			il2 = GetImageList(wxIMAGE_LIST_SMALL);
			il2->Replace(imgid, wxResizeIcon(icon, wxGetIconSize(wxIMAGE_LIST_SMALL, this)));
		}
		return imgid;
	}	
	else
		return -1;
}

int wxAvahiServiceListCtrl::GetImageIndex(const wxString& type)const
{
	wxAvahiServiceTypeImageListItemMap::const_iterator it = m_ilmap.find(type);
	if(it!=m_ilmap.end())
		return it->second;
	else
		return -1;
}

void wxAvahiServiceListCtrl::OnNewService(wxAvahiServiceBrowserEvent& event)
{
	wxAvahiServiceListCtrlContent content;
	content.type = event.GetType();
	content.name = event.GetName();
	content.domain = event.GetDomain();
	
	if(!Test(content.name))
		return;
	
	wxString sname = wxAvahiServiceTypeDatabase::GetDatabase().GetServiceName(content.type, m_language);
	int ilidx = GetImageIndex(content.type);
	if(ilidx == wxNOT_FOUND)
	{
		 wxIcon icon1 = wxArtProvider::GetIcon(wxT("zeroconf-servicetype-")+content.type, wxART_OTHER,
				 wxGetIconSize(wxIMAGE_LIST_NORMAL, this));
		 wxIcon icon2 = wxArtProvider::GetIcon(wxT("zeroconf-servicetype-")+content.type, wxART_OTHER,
				 wxGetIconSize(wxIMAGE_LIST_SMALL, this));
		 if(icon1.IsOk())
			 ilidx = RegisterImage(content.type, icon1, wxIMAGE_LIST_NORMAL);
		 if(icon2.IsOk())
			 ilidx = RegisterImage(content.type, icon2, wxIMAGE_LIST_SMALL);
	}
	if(sname.IsEmpty())
		sname = content.type;
	wxMutexGuiEnter();
	int id = InsertItem(GetItemCount(), content.name, ilidx);
	if(HasFlag(wxLC_REPORT))
	{
		SetItem(id, 1, sname);
		SetItem(id, 2, content.type);
	}
	m_map[id] = content; 
	wxMutexGuiLeave();
}

void wxAvahiServiceListCtrl::OnRemService(wxAvahiServiceBrowserEvent& event)
{
	int id = wxID_ANY;
	wxAvahiServiceContentMap::iterator it;
    for( it = m_map.begin(); it != m_map.end(); ++it )
    {
    	if(it->second.name == event.GetName() &&
    			it->second.type == event.GetType() &&
    			it->second.domain == event.GetDomain())
    	{
    		id = it->first;
    		break;
    	}
    }
	if(id!=wxID_ANY)
	{
		wxMutexGuiEnter();
		DeleteItem(id);
		wxMutexGuiLeave();
		m_map.erase(id);
	}
}

#endif /* USE_GUI == 1 */

