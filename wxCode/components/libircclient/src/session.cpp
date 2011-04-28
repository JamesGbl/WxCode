//===========================================================================
//
//  wxLibIRCClient - A wxWidgets libircclient wrapper library
//  Copyright (C) 2009 Andrea Zanellato (xvirc dot client at gmail dot com)
//
//  This program is FREE software. You can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your opinion) any later version.
//
//  This program is distributed in the HOPE that it will be USEFUL,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//  See the GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program. If not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
//===========================================================================
/**
 * @file session.cpp
 * @author Andrea Zanellato (xvirc.client@gmail.com)
 * @date 
 * @version 0.0.1
 */
#include "session.h"
#include "sessionmanager.h"
#include "sessionevents.h"

#include <wx/log.h>

wxLIRCCSession::wxLIRCCSession( irc_callbacks_t Callbacks )
:
wxThread()
{
	m_eState = DISCONNECTED;
	this->handle = irc_create_session( &Callbacks );
	wxLogDebug("<wxLIRCCSession::wxLIRCCSession> Started\n");
}

void wxLIRCCSession::OnExit()
{
	m_eState = DISCONNECTED;
	this->onDisconnect();
    wxLogDebug("<wxLIRCCSession::OnExit> Disconnected.\n");
}

void *wxLIRCCSession::Entry()
{
    irc_run ( this->handle );
    return NULL;
}

const wxString wxLIRCCSession::GetServer()
{
	wxString server( m_server, wxConvUTF8 );
	return server;
}

const wxString wxLIRCCSession::GetServerPassword()
{
	wxString passwd( m_svrpwd, wxConvUTF8 );
	return passwd;
}

const wxString wxLIRCCSession::GetNickName()
{
	wxString nicknm( m_nick, wxConvUTF8 );
	return nicknm;
}

const wxString wxLIRCCSession::GetUserName()
{
	wxString usernm( m_user, wxConvUTF8 );
	return usernm;
}

const wxString wxLIRCCSession::GetRealName()
{
	wxString realnm( m_name, wxConvUTF8 );
	return realnm;
}

int wxLIRCCSession::Connect( const wxString& server, unsigned short port,
							const wxString& server_password, const wxString& nickname,
                            const wxString& username, const wxString& realname )
{
    m_port 		= port;
    m_nick 		= nickname.mb_str();
    m_server 	= server.mb_str();

	username.IsEmpty() 			? m_user 	= 0 : m_user 	= username.mb_str();
	realname.IsEmpty() 			? m_name 	= 0 : m_user 	= realname.mb_str();
	server_password.IsEmpty() 	? m_svrpwd 	= 0 : m_svrpwd 	= server_password.mb_str();

	m_eState 	= CONNECTING;
	wxLogDebug("<wxLIRCCSession::Connect> Connecting");

	int retcode = irc_connect( this->handle, m_server, port, m_svrpwd,
								m_nick, m_user, m_name );

	if ( retcode == 0 )
	{
		m_eState = CONNECTED;
		wxLogDebug("<wxLIRCCSession::Connect> Connected");
	}
	else
	{
		m_eState = DISCONNECTED;
		wxLogDebug("<wxLIRCCSession::Connect> Disconnected (retcode = %i)", retcode );
	}

	return retcode;
}

int wxLIRCCSession::ConnectIPV6( const wxString& server, unsigned short port,
								const wxString& server_password, const wxString& nickname,
								const wxString& username, const wxString& realname )
{
    m_port 		= port;
    m_nick 		= nickname.mb_str();
    m_server 	= server.mb_str();

	username.IsEmpty() 			? m_user 	= 0 : m_user 	= username.mb_str();
	realname.IsEmpty() 			? m_name 	= 0 : m_user 	= realname.mb_str();
	server_password.IsEmpty() 	? m_svrpwd 	= 0 : m_svrpwd 	= server_password.mb_str();

	m_eState = CONNECTING;
	wxLogDebug("<wxLIRCCSession::ConnectIPV6> Connecting");

	int retcode = irc_connect( this->handle, m_server, port, m_svrpwd,
								m_nick, m_user, m_name );

	if ( retcode == 0 )
	{
		m_eState = CONNECTED;
		wxLogDebug("<wxLIRCCSession::ConnectIPV6> Connected");
	}
	else
	{
		m_eState = DISCONNECTED;
		wxLogDebug("<wxLIRCCSession::ConnectIPV6> Disconnected (retcode = %i)", retcode );
	}

	return retcode;
}

void wxLIRCCSession::onConnect( const char * event, const char * origin, const char ** params, unsigned int count)
{
// No extra params supplied; params is 0.
    wxQueueEvent( wxLIRCCSessionManager::Get(), new wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_CONNECT ) );
}

void wxLIRCCSession::onDisconnect()
{
    wxQueueEvent( wxLIRCCSessionManager::Get(), new wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_DISCONNECT ) );
}

void wxLIRCCSession::onNick ( const char * event, const char * origin, const char ** params, unsigned int count )
{
	if ( count < 1 ) return;

    wxArrayString data;
    wxString sNick( origin, wxConvUTF8 );
    wxString sNewNick( params[0], wxConvUTF8 );

    data.Add(sNick);       // The person, who changes the nick. Note that it can be you!
    data.Add(sNewNick);    // Mandatory, contains the new nick.

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_NICK, wxLIRCC_ID_CHAN_NICK, this );
    sessionevent.SetParams( data );
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onQuit( const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sNick( origin, wxConvUTF8 );
    wxString sReason( params[0], wxConvUTF8 );

    data.Add( sNick) ;   // The person, who is disconnected.
    data.Add( sReason ); // Optional, contains the reason message (user-specified).

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_QUIT, wxLIRCC_ID_CHAN_QUIT, this );
    sessionevent.SetParams( data );
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onJoin( const char * event, const char * origin, const char ** params, unsigned int count )
{
	if ( count < 1 ) return;

    wxArrayString data;
    wxString sNick( origin, wxConvUTF8 );
    wxString sChan( params[0], wxConvUTF8 );

    data.Add( sNick ); // Who joins the channel. By comparing it with your own nickname, you can check whether your JOIN command succeed.
    data.Add( sChan ); // Mandatory, contains the channel name.

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_JOIN, wxLIRCC_ID_CHAN_JOIN, this );
    sessionevent.SetParams( data );
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onPart( const char * event, const char * origin, const char ** params, unsigned int count )
{
	if ( count < 1 ) return;

    wxArrayString data;
    wxString sNick( origin, wxConvUTF8 );
    wxString sChan( params[0], wxConvUTF8 );
    wxString sReason( params[1], wxConvUTF8 );

    data.Add( sNick );   // Who parts the channel. By comparing it with your own nickname, you can check whether your PART command succeed.
    data.Add( sChan );   // Mandatory, contains the channel name.
    data.Add( sReason ); // Optional, contains the reason message (user-defined).

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_PART, wxLIRCC_ID_CHAN_PART, this );
    sessionevent.SetParams( data );
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onMode( const char * event, const char * origin, const char ** params, unsigned int count )
{
	if ( count < 2 ) return;

    wxArrayString data;
    wxString sNick( origin, wxConvUTF8 );
    wxString sChan( params[0], wxConvUTF8 );
    wxString sChanMode( params[1], wxConvUTF8 );
    wxString sModeArg( params[2], wxConvUTF8 );

    data.Add( sNick );       // Who changed the channel mode.
    data.Add( sChan );       // Mandatory, contains the channel name.
    data.Add( sChanMode );   // Mandatory, contains the changed channel mode, like '+t', '-s' and so on.
    data.Add( sModeArg );    // Optional, contains the mode argument
                           // (e.g. a key for +k mode, or user who got the channel operator status for +o mode)
    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_MODE, wxLIRCC_ID_CHAN_MODE, this );
    sessionevent.SetParams( data );
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onUserMode( const char * event, const char * origin, const char ** params, unsigned int count )
{
	if ( count < 1 ) return;

    wxArrayString data;
    wxString sNick( origin, wxConvUTF8 );
    wxString sMode( params[0], wxConvUTF8 );

    data.Add( sNick ); // Who changed the user mode.
    data.Add( sMode ); // Mandatory, contains the user changed mode, like '+t', '-i' and so on.

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_U_MODE, wxLIRCC_ID_USER_MODE, this );
    sessionevent.SetParams( data );
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onTopic( const char * event, const char * origin, const char ** params, unsigned int count )
{
	if ( count < 1 ) return;

    wxArrayString data;
    wxString sNick( origin, wxConvUTF8 );
    wxString sChan( params[0], wxConvUTF8 );
    wxString sTopic( params[1], wxConvUTF8 );

    data.Add( sNick );   // Who changes the channel topic.
    data.Add( sChan );   // Mandatory, contains the channel name.
    data.Add( sTopic );  // Optional, contains the new topic.

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_TOPIC, wxLIRCC_ID_CHAN_TOPIC, this );
    sessionevent.SetParams( data );
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onKick( const char * event, const char * origin, const char ** params, unsigned int count )
{
	if ( count < 1 ) return;

    wxArrayString data;
    wxString sNick( origin, wxConvUTF8 );
    wxString sChan( params[0], wxConvUTF8 );
    wxString sKicked( params[1], wxConvUTF8 );
    wxString sReason( params[2], wxConvUTF8 );

    data.Add( sNick );  	// Who kicked the poor.
    data.Add( sChan ); 		// Mandatory, contains the channel name.
    data.Add( sKicked ); 	// Optional, contains the nick of kicked person.
    data.Add( sReason ); 	// Optional, contains the kick text.

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_KICK, wxLIRCC_ID_CHAN_KICK, this );
    sessionevent.SetParams( data );
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onChanMsg( const char * event, const char * origin, const char ** params, unsigned int count )
{
    if ( count < 1 ) return;

    wxArrayString data;
    wxString sNick( origin, wxConvUTF8 );
    wxString sChan( params[0], wxConvUTF8 );
    wxString sMsg( params[1], wxConvUTF8 );

    data.Add( sNick );   // Who generates the message.
    data.Add( sChan );   // Mandatory, contains the channel name.
    data.Add( sMsg );    // Optional, contains the message text.

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_CHANMSG, wxLIRCC_ID_CHAN_MSG, this );
    sessionevent.SetParams( data );
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onPrivMsg( const char * event, const char * origin, const char ** params, unsigned int count )
{
    if ( count < 1 ) return;

    wxArrayString data;
    wxString sNick( origin, wxConvUTF8 );
    wxString sMe( params[0], wxConvUTF8 );
    wxString sMsg( params[1], wxConvUTF8 );

    data.Add( sNick );   // Who generates the message.
    data.Add( sMe );     // Mandatory, contains your nick.
    data.Add( sMsg );    // Optional, contains the message text

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_PRIVMSG, wxLIRCC_ID_USER_MSG, this );
    sessionevent.SetParams( data );
	wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onNotice( const char * event, const char * origin, const char ** params, unsigned int count )
{
    if ( count < 1 ) return;

    wxArrayString data;
    wxString sNick( origin, wxConvUTF8 );
    wxString sTarget( params[0], wxConvUTF8 );
    wxString sMsg( params[1], wxConvUTF8 );

    data.Add( sNick );   // Who generates the message.
    data.Add( sTarget ); // Mandatory, contains target name.
    data.Add( sMsg );    // Optional, contains the message text.

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_NOTICE, wxLIRCC_ID_USER_NOTICE, this );
    sessionevent.SetParams( data );
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onChanNotice( const char * event, const char * origin, const char ** params, unsigned int count )
{
    if ( count < 1 ) return;

    wxArrayString data;
    wxString sNick( origin, wxConvUTF8 );
    wxString sTarget( params[0], wxConvUTF8 );
    wxString sMsg( params[1], wxConvUTF8 );

    data.Add( sNick );   // Who generates the message.
    data.Add( sTarget ); // Mandatory, contains the channel name.
    data.Add( sMsg );    // Optional, contains the message text.

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_CHAN_NOTICE, wxLIRCC_ID_CHAN_NOTICE, this );
    sessionevent.SetParams( data );
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onInvite( const char * event, const char * origin, const char ** params, unsigned int count )
{
    if ( count != 2 ) return;

    wxArrayString data;
    wxString sWho( origin, wxConvUTF8 );
    wxString sMe( params[0], wxConvUTF8 );
    wxString sChan( params[1], wxConvUTF8 );

    data.Add( sWho );   // Who INVITEs you.
    data.Add( sMe ); 	// Mandatory, contains your nick.
    data.Add( sChan ); 	// Mandatory, contains the channel name you're invited into.

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_INVITE, wxLIRCC_ID_INVITE, this );
    sessionevent.SetParams( data );
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onCTCPRep( const char * event, const char * origin, const char ** params, unsigned int count )
{
    if ( count != 1 ) return;

    wxArrayString data;
    wxString sWho( origin, wxConvUTF8 );
    wxString sCTCP( params[0], wxConvUTF8 );

    data.Add( sWho );   // Who generates the message.
    data.Add( sCTCP ); 	// Mandatory, the complete CTCP message, including its arguments.

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_CTCPREP, wxLIRCC_ID_CTCP_REPLY, this );
    sessionevent.SetParams( data );
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );

//  Mirc generates PING, FINGER, VERSION, TIME and ACTION messages,
//  check the source code of libirc_event_ctcp_internal function to 
//  see how to write your own CTCP request handler.
}

void wxLIRCCSession::onCTCPReq( const char * event, const char * origin, const char ** params, unsigned int count )
{
    if ( count != 1 ) return;

    wxArrayString data;
    wxString sWho( origin, wxConvUTF8 );
    wxString sCTCP( params[0], wxConvUTF8 );

    data.Add( sWho );   // Who generates the message.
    data.Add( sCTCP ); 	// Mandatory, the CTCP message itself with its arguments.

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_CTCPREQ, wxLIRCC_ID_CTCP_REQUEST, this );
    sessionevent.SetParams( data );
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onCTCPAction( const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sNick( origin, wxConvUTF8 );
    wxString sTarget( params[0], wxConvUTF8 );
    wxString sMsg( params[1], wxConvUTF8 );

    data.Add( sNick );   // Who generates the message.
    data.Add( sTarget ); // Mandatory?, contains the target name.
    data.Add( sMsg );    // Mandatory, the ACTION message.

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_CTCPACT, wxLIRCC_ID_CTCP_ACTION, this );
    sessionevent.SetParams( data );
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onUnknown ( const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sOrigin( origin, wxConvUTF8 );
    wxString param;

    for ( unsigned int i = 0; i < count; ++i)
    {
        wxString param( params[i], wxConvUTF8 );
        data.Add( param );
    }

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_UNKNOWN, wxLIRCC_ID_CTCP_UNKNOWN, this );
    sessionevent.SetId( event );
    sessionevent.SetCount( count );
    sessionevent.SetOrigin( sOrigin );
    sessionevent.SetParams( data );
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onNumeric ( unsigned int event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sOrigin( origin, wxConvUTF8 );
    wxString param;

    for ( unsigned int i = 0; i < count; ++i)
    {
        wxString param( params[i], wxConvUTF8 );
        data.Add( param );
    }

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_NUMERIC, wxLIRCC_ID_NUMERIC, this );
    sessionevent.SetId( event );
    sessionevent.SetCount( count );
    sessionevent.SetOrigin( sOrigin );
    sessionevent.SetParams( data );
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onDCCChatReq ( const char * nick, const char * addr, irc_dcc_t dccid )
{
    wxString sNick( nick, wxConvUTF8 );
    wxString sAddr( addr, wxConvUTF8 );

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_DCCCHAT, wxLIRCC_ID_DCC_CHAT_REQ, this );
    sessionevent.SetOrigin( sNick ); 	// The person who requested DCC CHAT with you.
    sessionevent.SetString( sAddr ); 	// The person's IP address in decimal-dot notation.
    sessionevent.SetId( dccid ); 		// An ID associated with this request. Use it in calls to irc_dcc_accept() or irc_dcc_decline().
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );

// The "DCC Chat" event is triggered when someone requests a DCC CHAT from you.
// See the params in ::irc_event_dcc_chat_t specification.
}

void wxLIRCCSession::onDCCSendReq ( const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid )
{
    wxArrayString data;
    wxString sNick( nick, wxConvUTF8 );
    wxString sAddr( addr, wxConvUTF8 );
    wxString sFilename( filename, wxConvUTF8 );
	data.Add( sAddr );
	data.Add( sFilename );

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_DCCSEND, wxLIRCC_ID_DCC_SEND_REQ, this );
    sessionevent.SetOrigin( sNick ); 	// Who wants to send a file to you using DCCSEND.
    sessionevent.SetParams( data ); 	// The person's IP address in decimal-dot notation...
										// ...and the sent filename.
	sessionevent.SetExtraLong( size ); 	// The filename size.
    sessionevent.SetId( dccid ); 		// An ID associated with this request. Use it in calls to irc_dcc_accept() or irc_dcc_decline().
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );

// The "DCC Send" event is triggered wants to send a file to you using DCC SEND.
// As with chat, in respond you should call either irc_dcc_accept() to accept this request and receive the file,
// or irc_dcc_decline() to decline this request.
}

void wxLIRCCSession::onDCCRecv( irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length )
{

}

void wxLIRCCSession::onDCCFileRecv( irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length )
{

}
