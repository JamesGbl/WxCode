/**
 * @file sessionmanager.cpp
 * 	
 * This file is part of XVIRC.
 * 
 * @author foldink (foldink@gmail.com)
 * @date 07-February-2010
 * @version 0.0.1
 */

#include "sessionmanager.h"
#include "session.h"

#include <wx/log.h>

/** declare the global instance of this manager */
IRCSessionManager* IRCSessionManager::m_instance = 0;

void IRCSessionManager::on_connect ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnConnect ( event, origin, params, count );
}

void IRCSessionManager::on_nick ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
    IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnNick ( event, origin, params, count );
}

void IRCSessionManager::on_quit ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
    IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnQuit ( event, origin, params, count );
}

void IRCSessionManager::on_join ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
    IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnJoin ( event, origin, params, count );
}

void IRCSessionManager::on_part ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
    IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnPart ( event, origin, params, count );
}

void IRCSessionManager::on_mode ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnMode ( event, origin, params, count );
}

void IRCSessionManager::on_umode ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnUserMode ( event, origin, params, count );
}

void IRCSessionManager::on_topic ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnTopic ( event, origin, params, count );
}

void IRCSessionManager::on_kick ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnKick ( event, origin, params, count );
}

void IRCSessionManager::on_channel ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnChanMsg ( event, origin, params, count );
}

void IRCSessionManager::on_privmsg ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnPrivMsg ( event, origin, params, count );
}

void IRCSessionManager::on_notice ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnNotice ( event, origin, params, count );
}

void IRCSessionManager::on_channel_notice ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnChanNotice ( event, origin, params, count );
}

void IRCSessionManager::on_invite ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnInvite ( event, origin, params, count );
}

void IRCSessionManager::on_ctcp_action ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnCTCPAction ( event, origin, params, count );
}

void IRCSessionManager::on_ctcp_rep ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnCTCPRep ( event, origin, params, count );
}

void IRCSessionManager::on_ctcp_req ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnCTCPReq ( event, origin, params, count );
}

void IRCSessionManager::on_unknown (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnUnknown ( event, origin, params, count );
}

void IRCSessionManager::on_numeric (irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count)
{
    IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnNumeric ( event, origin, params, count );
}

void IRCSessionManager::on_dcc_chat_req (irc_session_t * session, const char * nick, const char * addr, irc_dcc_t dccid)
{
    IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnDCCChatReq ( nick, addr, dccid );
}

void IRCSessionManager::on_dcc_send_req (irc_session_t * session, const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid)
{
    IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnDCCSendReq( nick, addr, filename, size, dccid );
}

void IRCSessionManager::on_dcc_recv (irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length)
{
    IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnDCCRecv(id, status, ctx, data, length);
}

void IRCSessionManager::on_dcc_file_recv (irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length)
{
    IRCSession* IRCNetwork = IRCSessionManager::Get()->FindSession( session );
	if( IRCNetwork != NULL )
		IRCNetwork->OnDCCFileRecv(id, status, ctx, data, length);
}

IRCSessionManager::IRCSessionManager():
	wxEvtHandler()
	
{
	memset (&m_callbacks, 0, sizeof(m_callbacks));

	// declare callbacks for irc events
	m_callbacks.event_dcc_chat_req = IRCSessionManager::on_dcc_chat_req;
	m_callbacks.event_dcc_send_req = IRCSessionManager::on_dcc_send_req;
	m_callbacks.event_ctcp_action = IRCSessionManager::on_ctcp_action;
	m_callbacks.event_ctcp_rep = IRCSessionManager::on_ctcp_rep;
	m_callbacks.event_ctcp_req = IRCSessionManager::on_ctcp_req;
	m_callbacks.event_connect = IRCSessionManager::on_connect;
	m_callbacks.event_channel = IRCSessionManager::on_channel;
	m_callbacks.event_numeric = IRCSessionManager::on_numeric;
	m_callbacks.event_privmsg = IRCSessionManager::on_privmsg;
	m_callbacks.event_unknown = IRCSessionManager::on_unknown;
	m_callbacks.event_invite = IRCSessionManager::on_invite;
	m_callbacks.event_notice = IRCSessionManager::on_notice;
	m_callbacks.event_channel_notice = IRCSessionManager::on_channel_notice;
	m_callbacks.event_topic = IRCSessionManager::on_topic;
	m_callbacks.event_umode = IRCSessionManager::on_umode;
	m_callbacks.event_join = IRCSessionManager::on_join;
	m_callbacks.event_kick = IRCSessionManager::on_kick;
	m_callbacks.event_mode = IRCSessionManager::on_mode;
	m_callbacks.event_nick = IRCSessionManager::on_nick;
	m_callbacks.event_part = IRCSessionManager::on_part;
	m_callbacks.event_quit = IRCSessionManager::on_quit;
}

IRCSessionManager::~IRCSessionManager()
{
	for( unsigned int i = 0; i < m_sessions.GetCount() ; ++i )
		delete m_sessions[i];
		
	m_sessions.Clear();
}

IRCSession* IRCSessionManager::FindSession( irc_session_t* session )
{
	for( unsigned int i = 0; i < m_sessions.GetCount() ; ++i )
	{
		IRCSession* curr = m_sessions[i];
		if( curr->Handle == session )
			return curr;
	}
	
	return NULL;
}

IRCSession* IRCSessionManager::Create()
{
	IRCSession* new_session = new IRCSession( m_callbacks );
	if ( new_session->Create() != wxTHREAD_NO_ERROR )
	{
		wxLogError("Can't create the thread!");
		delete new_session;
		new_session = NULL;
	}
	else
	{
		m_sessions.Add( new_session );
	}
	return new_session;
}

void IRCSessionManager::IRCSessionDestroy( irc_session_t* session )
{
	if ( session != NULL )
		irc_destroy_session( session );

	IRCSession* found = IRCSessionManager::FindSession( session );
	if ( found )
		m_sessions.Remove( found );
}
