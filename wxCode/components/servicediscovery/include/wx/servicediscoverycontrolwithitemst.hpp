// --
// --	servicediscoverycontrolwithitemst.hpp
// --	Licenser
// --
// --
// --	revision   	$Rev$
// --	changed by	$Author$
// --	modified	$Date$
// --
// --	Created by William H. Schultz on 5/6/08.
// --	Copyright © 2008 Cedrus Corporation. All rights reserved.
// --
// --

#ifndef _WX_SERVICE_DISCOVERY_CONTROL_WITH_ITEMS_HPP_
#define _WX_SERVICE_DISCOVERY_CONTROL_WITH_ITEMS_HPP_


#include <wx/wxprec.h>
#include "wx/servicediscoverycontrolwithitemst.h"
#include "wx/servicediscoveryservice.h"

#if wxUSE_SERVICE_DISCOVERY


class wxServiceDiscoveryControlClientData : public wxClientData
{
public:
	wxServiceDiscoveryControlClientData( const wxServiceDiscoveryResult & rEvent )
	: m_pEvent ( rEvent.Clone() )
	{}
	
	const wxServiceDiscoveryResult *	GetBonjourEvent( void ) const { return m_pEvent; }
	
	virtual ~wxServiceDiscoveryControlClientData()
	{
		delete m_pEvent;
	}
	
protected:
	const wxServiceDiscoveryResult * m_pEvent;
};



#pragma mark  -- Constructors and Destructors --

template<class T>
wxServiceDiscoveryControlWithItems<T>::wxServiceDiscoveryControlWithItems( void )
{
	m_pBrowser = NULL;
	m_bListIsDirty = true;
	m_bEventsBeenConnected = false;
}



template<class T>
wxServiceDiscoveryControlWithItems<T>::~wxServiceDiscoveryControlWithItems( void )
{
	if ( m_pBrowser != NULL )
	{
		delete m_pBrowser;
		m_pBrowser = NULL;
	}
	
	this->Clear();
	m_ResultsArray.Clear();
}


	



template<class T>
void wxServiceDiscoveryControlWithItems<T>::Init( wxServiceDiscoveryService & rService,
												  wxString local_name_to_hide )
{
	m_LocalMachineNameToHide = local_name_to_hide;
	
	
	if ( ! m_bEventsBeenConnected )
	{
		Connect( this->GetId(), wxEVT_UPDATE_UI,				wxEventHandler( wxServiceDiscoveryControlWithItems::HandleUpdateVisibleList ),		NULL, this );
		
		Connect( this->GetId(), wxEVT_BONJOUR_ADD_SERVICE,		wxEventHandler( wxServiceDiscoveryControlWithItems::HandleBonjourAddService ),		NULL, this );
		Connect( this->GetId(), wxEVT_BONJOUR_REMOVE_SERVICE, 	wxEventHandler( wxServiceDiscoveryControlWithItems::HandleBonjourRemoveService ),	NULL, this );
		Connect( this->GetId(), wxEVT_BONJOUR_RESOLVE_SERVICE,	wxEventHandler( wxServiceDiscoveryControlWithItems::HandleBonjourResolveService ),	NULL, this );
		
		Connect( 			    wxEVT_IDLE, 					wxEventHandler( wxServiceDiscoveryControlWithItems::HandleIdle ),					NULL, this );
	}
	
	m_bEventsBeenConnected = true;
	
	
	if ( m_ServiceName != rService.GetServiceType() )
	{
		if ( m_pBrowser != NULL )
		{
			m_pBrowser->Stop();
			delete m_pBrowser;
			m_pBrowser = NULL;
		}
		
		m_pBrowser = new wxServiceDiscoveryBrowser( this,
													false /*don't use threads -- more reliable*/,
													rService,
													true /*autoresolve results*/ );
		
		m_ServiceName = rService.GetServiceType();
		
		m_pBrowser->Start();
	}
}



template<class T>
bool wxServiceDiscoveryControlWithItems<T>::TransferDataToWindow()
{
	bool status = T::TransferDataToWindow();
	
	status &= ( m_pBrowser != NULL ) && ( m_ServiceName != wxEmptyString );
	
	return status;
}



template<class T>
bool wxServiceDiscoveryControlWithItems<T>::TransferDataFromWindow()
{
	bool status = T::TransferDataFromWindow();
	
	
	if ( status )
	{
		m_ResultsArray.Empty();
		
		int selection = this->GetSelection();
		
		status = false;
		
		if ( (selection != wxNOT_FOUND ) && ( this->GetClientObject( selection ) != NULL ) )
		{
			m_ResultsArray.Add( *( static_cast<wxServiceDiscoveryControlClientData*>( this->GetClientObject( selection ) )->GetBonjourEvent() ) );
			status = true;
		}
	}
	
	
	return status;
}






template<class T>
const	wxServiceDiscoveryResultArray & wxServiceDiscoveryControlWithItems<T>::GetSelectedServers( void )
{
	TransferDataFromWindow();
	return m_ResultsArray;
}




#pragma mark  				-- Event Handling --


template<class T>
void wxServiceDiscoveryControlWithItems<T>::HandleBonjourAddService( wxEvent & rEvent )
{
	wxServiceDiscoveryResult * pEvent = wxStaticCast( &rEvent, wxServiceDiscoveryResult );
	
	if ( pEvent != NULL )
		BonjourAddService( *pEvent );
	else
		wxFAIL;
}


template<class T>
void wxServiceDiscoveryControlWithItems<T>::HandleBonjourRemoveService( wxEvent & rEvent )
{
	wxServiceDiscoveryResult * pEvent = wxStaticCast( &rEvent, wxServiceDiscoveryResult );
	
	if ( pEvent != NULL )
		BonjourRemoveService( *pEvent );
	else
		wxFAIL;
}



template<class T>
void wxServiceDiscoveryControlWithItems<T>::HandleBonjourResolveService( wxEvent & rEvent )
{
	wxServiceDiscoveryResult * pEvent = wxStaticCast( &rEvent, wxServiceDiscoveryResult );
	
	if ( pEvent != NULL )
		BonjourResolveService( *pEvent );
	else
		wxFAIL;
}



template<class T>
void wxServiceDiscoveryControlWithItems<T>::HandleIdle( wxEvent & rEvent )
{
	wxIdleEvent * pEvent = wxStaticCast( &rEvent, wxIdleEvent );
	
	if ( pEvent != NULL )
		DoIdle( *pEvent );
	else
		wxFAIL;
}



template<class T>
void wxServiceDiscoveryControlWithItems<T>::HandleUpdateVisibleList( wxEvent & rEvent )
{
	wxUpdateUIEvent * pEvent = wxStaticCast( &rEvent, wxUpdateUIEvent );
	
	if ( pEvent != NULL )
		DoUpdateVisibleList( *pEvent );
	else
		wxFAIL;
}


template<class T>
void wxServiceDiscoveryControlWithItems<T>::BonjourAddService( wxServiceDiscoveryResult & rEvent )
{
	wxLogDebug( wxT("Found bonjour service:  %s  on interface %u"),
			   rEvent.ServiceName().c_str(),
			   rEvent.NetworkInterface() );
	
	m_bListIsDirty = true;
		
	if ( ! rEvent.MoreComing() )
	{
		this->UpdateWindowUI();
	}	
}



template<class T>
void wxServiceDiscoveryControlWithItems<T>::BonjourRemoveService( wxServiceDiscoveryResult & rEvent )
{
	wxLogDebug( wxT("Removing bonjour service:  %s  from interface %u"),
			   rEvent.ServiceName().c_str(),
			   rEvent.NetworkInterface() );
	
	m_bListIsDirty = true;
	
	if ( ! rEvent.MoreComing() )
	{
		this->UpdateWindowUI();
	}	
}



template<class T>
void wxServiceDiscoveryControlWithItems<T>::BonjourResolveService( wxServiceDiscoveryResult & rEvent )
{
	wxLogDebug( wxT("Resolved bonjour service:  %s  on interface %u  as:  %s  with IP:  %s"),
			   rEvent.ServiceName().c_str(),
			   rEvent.NetworkInterface(),
			   rEvent.Target().c_str(),
			   rEvent.GetAddress().IPAddress().c_str() );	
	
	m_bListIsDirty = true;
	
	if ( ! rEvent.MoreComing() )
	{
		this->UpdateWindowUI();
	}
}



template<class T>
void wxServiceDiscoveryControlWithItems<T>::DoIdle( wxIdleEvent & rEvent )
{
#if !defined( __DARWIN__ ) && !defined( _WIN32 )
	if ( m_pBrowser != NULL )
	{
		rEvent.RequestMore( m_pBrowser->DoIdle() );
	}
	else
#endif
	{
		rEvent.RequestMore( false );
	}
}



template<class T>
void wxServiceDiscoveryControlWithItems<T>::DoUpdateVisibleList( wxUpdateUIEvent & WXUNUSED( rEvent ) )
{
	if ( ( m_bListIsDirty ) && ( m_pBrowser != NULL ) )
	{
		const wxServiceDiscoveryResultArray	& results_array = m_pBrowser->GetArray();
		
		// We use the following string to ensure that the machine on which we
		// are running does *not* show up in the list of available services.
		// If it's empty, everything shows up.
		wxLogDebug( wxT("Updating list of services.  Our name:  %s"),
				   m_LocalMachineNameToHide.c_str() );
		
		const wxServiceDiscoveryResult * pFoundItem = NULL;
		bool found = false;
		
		
		// First, remove items that shouldn't be in the list
		for( unsigned int i = 0; i < this->GetCount(); i++ )
		{
			wxServiceDiscoveryControlClientData * pClientData = static_cast<wxServiceDiscoveryControlClientData *>( this->GetClientObject( i ) );
			// If there is no client data, we assume this is a manually inserted item that should be removed.
			
			found = false;

			if ( pClientData != NULL )
			{
				pFoundItem = pClientData->GetBonjourEvent();
				
				if ( pFoundItem->ServiceName() != m_LocalMachineNameToHide )
				{
					for( unsigned int j = 0; j < results_array.GetCount(); j++ )
					{
						if ( *pFoundItem == results_array[j] )
						{
							if ( pFoundItem->Resolved() == results_array[j].Resolved() )
								found = true;
							break;
						}
					}
				}
			}
			
			if ( !found )
			{
				this->Delete( i-- );
			}
		}
		
		
		// Then add items that should be in the list
		for( unsigned int j = 0; j < results_array.GetCount(); j++ )
		{
			found = false;
			
			if ( ( m_LocalMachineNameToHide == wxEmptyString ) ||
				 ( results_array[j].ServiceName() != m_LocalMachineNameToHide ) )
			{
				for( unsigned int i = 0; i < this->GetCount(); i++ )
				{
					pFoundItem = static_cast<wxServiceDiscoveryControlClientData *>( this->GetClientObject( i ) )->GetBonjourEvent();
										
					if ( *pFoundItem == results_array[j] )
					{
						found = true;
						break;
					}
				}
			}
			else
			{
				// Lie and say that it's already in the listbox.  We don't want
				// our own entry in the listbox.
				found = true;
			}
			
			
			if ( !found )
			{
				if ( results_array[j].Resolved() )
				{
					const wxString text = wxString::Format( wxT("%s (as address: %s,  connected to interface: %s)"),
														   results_array[j].ServiceName().c_str(),
														   results_array[j].Target().c_str(),
														   results_array[j].NetworkInterfaceName().c_str() );
					this->Append( text, 
								 new wxServiceDiscoveryControlClientData( results_array[j] ) );
				}
				else
				{
					const wxString text = wxString::Format( wxT("%s (connected to interface: %s)"),
														   results_array[j].ServiceName().c_str(),
														   results_array[j].NetworkInterfaceName().c_str() );
					this->Append( text, 
								 new wxServiceDiscoveryControlClientData( results_array[j] ) );
				}
			}
		}
		
		
		
		m_bListIsDirty = false;
	}
}


#endif // wxUSE_SERVICE_DISCOVERY

#endif // _WX_SERVICE_DISCOVERY_CONTROL_WITH_ITEMS_HPP_
