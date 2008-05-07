///
///	@file servicediscoverycontrolwithitemst.h
///	@ingroup Licenser
///
///	@class Licenser::servicediscoverycontrolwithitemst servicediscoverycontrolwithitemst.h servicediscoverycontrolwithitemst.cpp
///
///	$Rev$
///	$Author$
///	$Date$
///
///	Created by William H. Schultz on 5/6/08.
///	@copyright © 2008 Cedrus Corporation. All rights reserved.
///
///


#ifndef _WX_SERVICE_DISCOVERY_CONTROL_WITH_ITEMS_H_
#define _WX_SERVICE_DISCOVERY_CONTROL_WITH_ITEMS_H_

#include "wx/servicediscovery.h"

#if wxUSE_SERVICE_DISCOVERY

#include "wx/servicediscoveryresult.h"
#include "wx/servicediscoverybrowser.h"

#pragma mark  -- class servicediscoverycontrolwithitemst --

template<class T>
class wxServiceDiscoveryControlWithItems : public T
{
public:

	#pragma mark  				-- Constructors and Destructors --

								wxServiceDiscoveryControlWithItems( void );
	virtual						~wxServiceDiscoveryControlWithItems( void );
	
	
	// If you do not call Init, your control will have no idea of what to browse for.
			void				Init( wxServiceDiscoveryService & rService,
									  wxString local_name_to_hide = wxEmptyString );
	
	
	/// you must subclass, and you must call Init from this function.  It is your responsibility to ensure this function gets called,
	/// either by validating recursively or manually calling the function.
	virtual bool 				TransferDataToWindow() = 0;
	virtual bool 				TransferDataFromWindow();

	#pragma mark  				-- Event Handling --
	
	virtual	void				BonjourAddService		( wxServiceDiscoveryResult & rEvent );
	virtual	void				BonjourRemoveService	( wxServiceDiscoveryResult & rEvent );
	virtual	void				BonjourResolveService	( wxServiceDiscoveryResult & rEvent );
	
	virtual	void				DoIdle					( wxIdleEvent & rEvent );
			
	virtual	void				DoUpdateVisibleList		( wxUpdateUIEvent & rEvent );
	
			const	wxServiceDiscoveryResultArray &
								GetSelectedServers( void );
	
protected:
	
			void				HandleBonjourAddService		( wxEvent & rEvent );
			void				HandleBonjourRemoveService	( wxEvent & rEvent );
			void				HandleBonjourResolveService	( wxEvent & rEvent );
	
			void				HandleIdle					( wxEvent & rEvent );
	
			void				HandleUpdateVisibleList		( wxEvent & rEvent );
	
	
	#pragma mark 				-
	#pragma mark  				-- Member Variables --

	wxServiceDiscoveryBrowser *		m_pBrowser;
	wxServiceDiscoveryResultArray	m_ResultsArray;
	bool							m_bListIsDirty;
	
	wxString						m_ServiceName;
	wxString						m_LocalMachineNameToHide;
	
	bool							m_bEventsBeenConnected;

	DECLARE_NO_COPY_CLASS( wxServiceDiscoveryControlWithItems )
};



#endif // wxUSE_SERVICE_DISCOVERY

#endif // _WX_SERVICE_DISCOVERY_CONTROL_WITH_ITEMS_H_
