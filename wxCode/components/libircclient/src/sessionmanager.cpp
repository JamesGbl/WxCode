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
 * @file sessionmanager.cpp
 * @author foldink (foldink@gmail.com)
 * @date 07-February-2010
 * @version 0.0.1
 */

#include "sessionmanager.h"
#include "session.h"

#include <wx/log.h>

/** declare the global instance of this manager */
wxLIRCCSessionManager* wxLIRCCSessionManager::m_instance = 0;

void wxLIRCCSessionManager::on_connect ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onConnect ( event, origin, params, count );
}

void wxLIRCCSessionManager::on_nick ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onNick ( event, origin, params, count );
}

void wxLIRCCSessionManager::on_quit ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onQuit ( event, origin, params, count );
}

void wxLIRCCSessionManager::on_join ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onJoin ( event, origin, params, count );
}

void wxLIRCCSessionManager::on_part ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onPart ( event, origin, params, count );
}

void wxLIRCCSessionManager::on_mode ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onMode ( event, origin, params, count );
}

void wxLIRCCSessionManager::on_umode ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onUserMode ( event, origin, params, count );
}

void wxLIRCCSessionManager::on_topic ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onTopic ( event, origin, params, count );
}

void wxLIRCCSessionManager::on_kick ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onKick ( event, origin, params, count );
}

void wxLIRCCSessionManager::on_channel ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onChanMsg ( event, origin, params, count );
}

void wxLIRCCSessionManager::on_privmsg ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onPrivMsg ( event, origin, params, count );
}

void wxLIRCCSessionManager::on_notice ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onNotice ( event, origin, params, count );
}

void wxLIRCCSessionManager::on_channel_notice ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onChanNotice ( event, origin, params, count );
}

void wxLIRCCSessionManager::on_invite ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onInvite ( event, origin, params, count );
}

void wxLIRCCSessionManager::on_ctcp_action ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onCTCPAction ( event, origin, params, count );
}

void wxLIRCCSessionManager::on_ctcp_rep ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onCTCPRep ( event, origin, params, count );
}

void wxLIRCCSessionManager::on_ctcp_req ( irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count )
{
	wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onCTCPReq ( event, origin, params, count );
}

void wxLIRCCSessionManager::on_unknown (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onUnknown ( event, origin, params, count );
}

void wxLIRCCSessionManager::on_numeric (irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count)
{
    wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onNumeric ( event, origin, params, count );
}

void wxLIRCCSessionManager::on_dcc_chat_req (irc_session_t * session, const char * nick, const char * addr, irc_dcc_t dccid)
{
    wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onDCCChatReq ( nick, addr, dccid );
}

void wxLIRCCSessionManager::on_dcc_send_req (irc_session_t * session, const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid)
{
    wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onDCCSendReq( nick, addr, filename, size, dccid );
}

void wxLIRCCSessionManager::on_dcc_recv (irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length)
{
    wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onDCCRecv(id, status, ctx, data, length);
}

void wxLIRCCSessionManager::on_dcc_file_recv (irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length)
{
    wxLIRCCSession* pSession = wxLIRCCSessionManager::Get()->FindSession( session );
	if( pSession != NULL )
		pSession->onDCCFileRecv(id, status, ctx, data, length);
}

wxLIRCCSessionManager::wxLIRCCSessionManager(): wxEvtHandler()
{
	memset (&m_callbacks, 0, sizeof(m_callbacks));

	// declare callbacks for irc events
	m_callbacks.event_dcc_chat_req = wxLIRCCSessionManager::on_dcc_chat_req;
	m_callbacks.event_dcc_send_req = wxLIRCCSessionManager::on_dcc_send_req;
	m_callbacks.event_ctcp_action = wxLIRCCSessionManager::on_ctcp_action;
	m_callbacks.event_ctcp_rep = wxLIRCCSessionManager::on_ctcp_rep;
	m_callbacks.event_ctcp_req = wxLIRCCSessionManager::on_ctcp_req;
	m_callbacks.event_connect = wxLIRCCSessionManager::on_connect;
	m_callbacks.event_channel = wxLIRCCSessionManager::on_channel;
	m_callbacks.event_numeric = wxLIRCCSessionManager::on_numeric;
	m_callbacks.event_privmsg = wxLIRCCSessionManager::on_privmsg;
	m_callbacks.event_unknown = wxLIRCCSessionManager::on_unknown;
	m_callbacks.event_invite = wxLIRCCSessionManager::on_invite;
	m_callbacks.event_notice = wxLIRCCSessionManager::on_notice;
	m_callbacks.event_channel_notice = wxLIRCCSessionManager::on_channel_notice;
	m_callbacks.event_topic = wxLIRCCSessionManager::on_topic;
	m_callbacks.event_umode = wxLIRCCSessionManager::on_umode;
	m_callbacks.event_join = wxLIRCCSessionManager::on_join;
	m_callbacks.event_kick = wxLIRCCSessionManager::on_kick;
	m_callbacks.event_mode = wxLIRCCSessionManager::on_mode;
	m_callbacks.event_nick = wxLIRCCSessionManager::on_nick;
	m_callbacks.event_part = wxLIRCCSessionManager::on_part;
	m_callbacks.event_quit = wxLIRCCSessionManager::on_quit;
}

wxLIRCCSessionManager::~wxLIRCCSessionManager()
{
	for( unsigned int i = 0; i < m_sessions.GetCount() ; ++i )
		delete m_sessions[i];
		
	m_sessions.Clear();
}

wxLIRCCSession* wxLIRCCSessionManager::FindSession( irc_session_t* session )
{
	for( unsigned int i = 0; i < m_sessions.GetCount() ; ++i )
	{
		wxLIRCCSession* curr = m_sessions[i];
		if( curr->handle == session )
			return curr;
	}
	
	return NULL;
}

wxLIRCCSession* wxLIRCCSessionManager::Create()
{
	wxLIRCCSession* new_session = new wxLIRCCSession( m_callbacks );
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

void wxLIRCCSessionManager::DestroySession( irc_session_t* session )
{
	if ( session )
	{
		wxLIRCCSession* found = wxLIRCCSessionManager::FindSession( session );
		if ( found )
		{
			m_sessions.Remove( found );
			irc_destroy_session( session );
		}
	}
}
