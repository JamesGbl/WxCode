// --
// --	servicediscoveryresult.h
// --
// --	@class wxServiceDiscoveryResult
// --
// --	revision   	$Rev$
// --	changed by	$Author$
// --	modified	$Date$
// --
// --	Created by Hank Schultz on 10/23/06.
// --	@copyright © 2006 Cedrus Corporation. wxWidgets licence.
// --
// --

#ifndef _WX_SERVICE_DISCOVERY_RESULT_H_
#define _WX_SERVICE_DISCOVERY_RESULT_H_

#include "wx/servicediscovery.h"

#if wxUSE_SERVICE_DISCOVERY

#include "wx/socket.h"


#pragma mark  -- class wxServiceDiscoveryResult --

class WXDLLIMPEXP_SERVICEDISCOVERY wxServiceDiscoveryResult : public wxEvent
{
public:

	#pragma mark  				-- Constructors and Destructors --

								wxServiceDiscoveryResult( void );
								wxServiceDiscoveryResult( const wxServiceDiscoveryResult & rhs );

	virtual wxServiceDiscoveryResult * Clone() const;

	virtual						~wxServiceDiscoveryResult( void );
	
	
	virtual bool				operator==( const wxServiceDiscoveryResult & rhs ) const;
	
	
	#pragma mark 				-
	#pragma mark  				-- Bonjour Browse Data --

	virtual	bool				MoreComing			( void ) const			{ return m_bMoreComing; }
	virtual void				SetMoreComing		( bool iMore )			{ m_bMoreComing = iMore; }
	
	virtual	const wxString &	ServiceName			( void ) const			{ return m_ServiceName; }
	virtual void				SetServiceName		( wxString iName )		{ m_ServiceName = iName; }
	
	virtual	const wxString &	RegType				( void ) const 			{ return m_RegType; }
	virtual void				SetRegType			( wxString iType ) 		{ m_RegType = iType; }

	virtual	const wxString &	Domain				( void ) const 			{ return m_Domain; }
	virtual void				SetDomain			( wxString iDomain )	{ m_Domain = iDomain; }

	virtual wxUint32			NetworkInterface	( void ) const			{ return m_NetworkInterface; }
	virtual const wxString &	NetworkInterfaceName( void ) const			{ return m_InterfaceName; }
	virtual void				SetNetworkInterface	( wxUint32 iIface );

	
	
	#pragma mark 				-
	#pragma mark  				-- Bonjour Resolve Data --
	
	virtual	bool				Resolved			( void ) const			{ return m_bResolved; }
	virtual void				SetResolved			( bool iResolved )		{ m_bResolved = iResolved; }

	virtual	const wxString &	FullName			( void ) const			{ return m_FullDNSName; }
	virtual void				SetFullName			( wxString iName )		{ m_FullDNSName = iName; }
	
	virtual wxUint16			Port				( void ) const			{ return m_Port; }
	virtual void				SetPort				( wxUint16 iPort );
	
	virtual const wxString &	Target				( void ) const			{ return m_TargetMachine; }
	virtual void				SetTarget			( wxString iTarget );
	
	virtual	const wxString &	TextRecord			( void ) const			{ return m_TxtRecord; }
	virtual void				SetTextRecord		( wxString iTxtRecord )	{ m_TxtRecord = iTxtRecord; }
	
	virtual const wxIPV4address	GetAddress			( void ) const			{ return m_Address; }
	
	
protected:
	#pragma mark 				-
	#pragma mark  				-- Member Variables --

		
			bool				m_bMoreComing;
			wxString			m_ServiceName;
			wxString			m_RegType;
			wxString			m_Domain;
			wxUint32			m_NetworkInterface;
			wxString			m_InterfaceName;
			
			wxIPV4address		m_Address;
			
			bool				m_bResolved;
			
			wxString			m_FullDNSName;
			wxUint16			m_Port;
			wxString			m_TargetMachine;
			wxString			m_TxtRecord;
	
	DECLARE_NO_ASSIGN_CLASS( wxServiceDiscoveryResult )
};


WX_DECLARE_USER_EXPORTED_OBJARRAY( wxServiceDiscoveryResult, wxServiceDiscoveryResultArray, WXDLLIMPEXP_SERVICEDISCOVERY );


BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EXPORTED_EVENT_TYPE( WXDLLIMPEXP_SERVICEDISCOVERY, wxEVT_BONJOUR_ADD_SERVICE,		wxEVT_USER_FIRST+1 )
	DECLARE_EXPORTED_EVENT_TYPE( WXDLLIMPEXP_SERVICEDISCOVERY, wxEVT_BONJOUR_REMOVE_SERVICE,	wxEVT_USER_FIRST+2 )
	DECLARE_EXPORTED_EVENT_TYPE( WXDLLIMPEXP_SERVICEDISCOVERY, wxEVT_BONJOUR_REGISTER_SERVICE,	wxEVT_USER_FIRST+3 )
	DECLARE_EXPORTED_EVENT_TYPE( WXDLLIMPEXP_SERVICEDISCOVERY, wxEVT_BONJOUR_RESOLVE_SERVICE,	wxEVT_USER_FIRST+4 )
END_DECLARE_EVENT_TYPES();


typedef void (wxEvtHandler::*wxBONJOUR_EVENT_FUNCTION)(wxServiceDiscoveryResult&);


#define wxBONJOUR_EVENT_HANDLER(func) \
(wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxBONJOUR_EVENT_FUNCTION, &func)



#define EVT_BONJOUR_ADD_SERVICE(func)		wx__DECLARE_EVT0(wxEVT_BONJOUR_ADD_SERVICE,			wxBONJOUR_EVENT_HANDLER(func))
#define EVT_BONJOUR_REMOVE_SERVICE(func)	wx__DECLARE_EVT0(wxEVT_BONJOUR_REMOVE_SERVICE,		wxBONJOUR_EVENT_HANDLER(func))
#define EVT_BONJOUR_REGISTER_SERVICE(func)	wx__DECLARE_EVT0(wxEVT_BONJOUR_REGISTER_SERVICE,	wxBONJOUR_EVENT_HANDLER(func))
#define EVT_BONJOUR_RESOLVE_SERVICE(func)	wx__DECLARE_EVT0(wxEVT_BONJOUR_RESOLVE_SERVICE,		wxBONJOUR_EVENT_HANDLER(func))


#endif // wxUSE_SERVICE_DISCOVERY

#endif // _WX_SERVICE_DISCOVERY_RESULT_H_
