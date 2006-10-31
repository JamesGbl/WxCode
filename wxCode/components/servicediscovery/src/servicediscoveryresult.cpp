// --
// --	servicediscoveryresult.cpp
// --
// --	revision   	$Rev$
// --	changed by	$Author$
// --	modified	$Date$
// --
// --	Created by Hank Schultz on 10/23/06.
// --	Copyright © 2006 Cedrus Corporation. wxWidgets licence.
// --
// --

#include "wx/wxprec.h"
#include "wx/arrimpl.cpp"

#include "wx/servicediscoveryresult.h"

#ifndef _WIN32
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <net/if.h>
#else
	#include <Iprtrmib.h>
	#include <Iphlpapi.h>

	#pragma comment (lib, "Iphlpapi.lib")
#endif


DEFINE_EVENT_TYPE( wxEVT_BONJOUR_ADD_SERVICE );
DEFINE_EVENT_TYPE( wxEVT_BONJOUR_REMOVE_SERVICE );
DEFINE_EVENT_TYPE( wxEVT_BONJOUR_REGISTER_SERVICE );
DEFINE_EVENT_TYPE( wxEVT_BONJOUR_RESOLVE_SERVICE );



WX_DEFINE_USER_EXPORTED_OBJARRAY( wxServiceDiscoveryResultArray );

	
#pragma mark  -- Constructors and Destructors --

wxServiceDiscoveryResult::wxServiceDiscoveryResult( void )
	: wxEvent(),
	m_bMoreComing ( false ),
	m_NetworkInterface ( 0 ),
	m_bResolved ( false )
{
}



wxServiceDiscoveryResult::wxServiceDiscoveryResult( const wxServiceDiscoveryResult & rhs )
: wxEvent			( rhs ),
m_bMoreComing		( rhs.m_bMoreComing ),
m_ServiceName		( rhs.m_ServiceName ),
m_RegType			( rhs.m_RegType ),
m_Domain			( rhs.m_Domain ),
m_NetworkInterface	( rhs.m_NetworkInterface ),
m_InterfaceName		( rhs.m_InterfaceName ),
m_bResolved			( rhs.m_bResolved ),
m_FullDNSName		( rhs.m_FullDNSName ),
m_Port				( rhs.m_Port ),
m_TargetMachine		( rhs.m_TargetMachine ),
m_TxtRecord			( rhs.m_TxtRecord )
{
}



wxServiceDiscoveryResult * wxServiceDiscoveryResult::Clone() const
{
	return new wxServiceDiscoveryResult( *this );
}



bool wxServiceDiscoveryResult::operator==( const wxServiceDiscoveryResult & rhs ) const
{
	bool output = this == &rhs;
	
	if ( !output )
	{
		output = ( ( m_ServiceName == rhs.m_ServiceName ) &&
				   ( m_RegType == rhs.m_RegType ) &&
				   ( m_Domain == rhs.m_Domain ) &&
				   ( m_NetworkInterface == rhs.m_NetworkInterface ) );
	}
	
	return output;
}



wxServiceDiscoveryResult::~wxServiceDiscoveryResult( void )
{
}



void wxServiceDiscoveryResult::SetNetworkInterface	( wxUint32 iIface )
{
	m_NetworkInterface = iIface;
	
#ifndef _WIN32
	static char buffer[IF_NAMESIZE];
	
	if ( if_indextoname( m_NetworkInterface, buffer ) != NULL )
	{
		m_InterfaceName = wxString::FromAscii( buffer );
	}
#else
	MIB_IFROW iface;
	iface.dwIndex = m_NetworkInterface;
	
	if ( GetIfEntry( &iface ) == NO_ERROR )
	{
		m_InterfaceName = wxString::FromAscii( (char *)iface.bDescr );
	}
#endif	
	else
	{
		m_InterfaceName = _("unknown");
	}
}


