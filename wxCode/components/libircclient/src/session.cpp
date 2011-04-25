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
	handle = irc_create_session( &Callbacks );
	wxLogDebug("<wxLIRCCSession::wxLIRCCSession> Started\n");
}

void wxLIRCCSession::OnExit()
{
    wxLogDebug("<wxLIRCCSession::OnExit> Disconnected.\n");
}

void *wxLIRCCSession::Entry()
{
/*
	if ( !handle )
	{
        wxLogDebug("<*wxLIRCCSession::Entry> Session creation error.\n");
		return NULL;
	}
*/
    irc_run ( handle );
    return NULL;
}

int wxLIRCCSession::Connect( const char * server,
                            unsigned short port,
                            const char * server_password,
                            const char * nickname,
                            const char * username,
                            const char * realname )
{
    m_server = server;
    m_svrpwd = server_password;
    m_port = port;
    m_nick = nickname;
    m_user = username;
    m_name = realname;

    return irc_connect( this->handle,
                        server,
                        port,
                        server_password,
                        nickname,
                        username,
                        realname );
}

int wxLIRCCSession::ConnectIPV6(const char * server,
                            unsigned short port,
                            const char * server_password,
                            const char * nickname,
                            const char * username,
                            const char * realname )
{
    m_server = server;
    m_svrpwd = server_password;
    m_port = port;
    m_nick = nickname;
    m_user = username;
    m_name = realname;

    return irc_connect6( this->handle,
                        server,
                        port,
                        server_password,
                        nickname,
                        username,
                        realname );
}

void wxLIRCCSession::onConnect( const char * event, const char * origin, const char ** params, unsigned int count)
{
/** No extra params supplied; params is 0. */
//    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_CONNECT , -1 , this );
    wxQueueEvent( wxLIRCCSessionManager::Get(), new wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_CONNECT ) );
}

void wxLIRCCSession::onNick ( const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sNewNick(params[0], wxConvUTF8);

    data.Add(sNick);       /** The person, who changes the nick. Note that it can be you! */
    data.Add(sNewNick);    /** Mandatory, contains the new nick. */

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_NICK , -1 , this );
    sessionevent.SetParams(data);
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onQuit( const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sReason(params[0], wxConvUTF8);

    data.Add(sNick);   /** The person, who is disconnected. */
    data.Add(sReason); /** Optional, contains the reason message (user-specified). */

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_QUIT , -1 , this );
    sessionevent.SetParams(data);
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onJoin( const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sChan(params[0], wxConvUTF8);

    data.Add(sNick); /** Who joins the channel. By comparing it with your own nickname, you can check whether your JOIN command succeed. */
    data.Add(sChan); /** Mandatory, contains the channel name. */

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_JOIN , -1 , this );
    sessionevent.SetParams(data);
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onPart( const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sChan(params[0], wxConvUTF8);
    wxString sReason(params[1], wxConvUTF8);

    data.Add(sNick);   /** Who parts the channel. By comparing it with your own nickname, you can check whether your PART command succeed. */
    data.Add(sChan);   /** Mandatory, contains the channel name. */
    data.Add(sReason); /** Optional, contains the reason message (user-defined). */

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_PART , -1 , this );
    sessionevent.SetParams(data);
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onMode( const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sChan(params[0], wxConvUTF8);
    wxString sChanMode(params[1], wxConvUTF8);
    wxString sModeArg(params[2], wxConvUTF8);

    data.Add(sNick);       /** Who changed the channel mode. */
    data.Add(sChan);       /** Mandatory, contains the channel name. */
    data.Add(sChanMode);   /** Mandatory, contains the changed channel mode, like '+t', '-s' and so on. */
    data.Add(sModeArg);    /** Optional, contains the mode argument
                                (for example, a key for +k mode, or user who got the channel operator status for +o mode) */
    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_MODE , -1 , this );
    sessionevent.SetParams(data);
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onUserMode( const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sMode(params[0], wxConvUTF8);

    data.Add(sNick); /** Who changed the user mode. */
    data.Add(sMode); /** Mandatory, contains the user changed mode, like '+t', '-i' and so on. */

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_U_MODE, -1 , this );
    sessionevent.SetParams(data);
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onTopic( const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sChan(params[0], wxConvUTF8);
    wxString sTopic(params[1], wxConvUTF8);

    data.Add(sNick);   /** Who changes the channel topic. */
    data.Add(sChan);   /** Mandatory, contains the channel name. */
    data.Add(sTopic);  /** Optional, contains the new topic. */

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_TOPIC, -1 , this );
    sessionevent.SetParams(data);
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onKick( const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sChan(params[0], wxConvUTF8);
    wxString sKicked(params[1], wxConvUTF8);
    wxString sReason(params[2], wxConvUTF8);

    data.Add(sNick);   /** Who kicked the poor. */
    data.Add(sChan);   /** Mandatory, contains the channel name. */
    data.Add(sKicked); /** Optional, contains the nick of kicked person. */
    data.Add(sReason); /** Optional, contains the kick text. */

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_KICK, -1 , this );
    sessionevent.SetParams(data);
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onChanMsg( const char * event, const char * origin, const char ** params, unsigned int count )
{
    if ( count != 2 ) return;

    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sChan(params[0], wxConvUTF8);
    wxString sMsg(params[1], wxConvUTF8);

    data.Add(sNick);   /** Who generates the message. */
    data.Add(sChan);   /** Mandatory, contains the channel name. */
    data.Add(sMsg);    /** Optional, contains the message text. */

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_CHANMSG, -1 , this );
    sessionevent.SetParams(data);
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onPrivMsg( const char * event, const char * origin, const char ** params, unsigned int count )
{
    if ( count != 2 ) return;

    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sMe(params[0], wxConvUTF8);
    wxString sMsg(params[1], wxConvUTF8);

    data.Add(sNick);   /** Who generates the message. */
    data.Add(sMe);     /** Mandatory, contains your nick. */
    data.Add(sMsg);    /** Optional, contains the message text. */

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_PRIVMSG, -1 , this );
    sessionevent.SetParams(data);
//    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
	wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onNotice( const char * event, const char * origin, const char ** params, unsigned int count )
{
    if ( count != 2 ) return;

    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sTarget(params[0], wxConvUTF8);
    wxString sMsg(params[1], wxConvUTF8);

    data.Add(sNick);   /** Who generates the message. */
    data.Add(sTarget); /** Mandatory, contains target name (nick or channel). */
    data.Add(sMsg);    /** Optional, contains the message text. */

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_NOTICE, -1 , this );
    sessionevent.SetParams(data);
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onChanNotice( const char * event, const char * origin, const char ** params, unsigned int count )
{
    if ( count != 2 ) return;

    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sTarget(params[0], wxConvUTF8);
    wxString sMsg(params[1], wxConvUTF8);

    data.Add(sNick);   /** Who generates the message. */
    data.Add(sTarget); /** Mandatory, contains target name (nick or channel). */
    data.Add(sMsg);    /** Optional, contains the message text. */

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_CHAN_NOTICE, -1 , this );
    sessionevent.SetParams(data);
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onInvite( const char * event, const char * origin, const char ** params, unsigned int count )
{

}

void wxLIRCCSession::onCTCPRep( const char * event, const char * origin, const char ** params, unsigned int count )
{

}

void wxLIRCCSession::onCTCPReq( const char * event, const char * origin, const char ** params, unsigned int count )
{

}

void wxLIRCCSession::onCTCPAction( const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
//    wxString sEvent(event, wxConvUTF8);
    wxString sNick(origin, wxConvUTF8);
    wxString sChan(params[0], wxConvUTF8);
    wxString sMsg(params[1], wxConvUTF8);

    data.Add(sNick);   /** Who generates the message. */
    data.Add(sChan);   /** Mandatory?, contains the channel name. */
    data.Add(sMsg);    /** Mandatory, the ACTION message. */

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_CTCPACT, -1 , this );
    sessionevent.SetParams(data);
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onUnknown ( const char * event, const char * origin, const char ** params, unsigned int count )
{

}

void wxLIRCCSession::onNumeric ( unsigned int event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sOrigin(origin, wxConvUTF8);
    wxString param;

    for ( unsigned int i = 0; i < count; ++i)
    {
        wxString param(params[i], wxConvUTF8);
        data.Add(param);
    }

    wxLIRCCSessionEvent sessionevent = wxLIRCCSessionEvent( wxLIRCC_EVT_THREAD_CHANMSG, -1 , this );
    sessionevent.SetId(event);
    sessionevent.SetCount(count);
    sessionevent.SetOrigin(sOrigin);
    sessionevent.SetParams(data);
    wxQueueEvent( wxLIRCCSessionManager::Get(), sessionevent.Clone() );
}

void wxLIRCCSession::onDCCChatReq ( const char * nick, const char * addr, irc_dcc_t dccid )
{

}

void wxLIRCCSession::onDCCSendReq ( const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid )
{

}

void wxLIRCCSession::onDCCRecv( irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length )
{

}

void wxLIRCCSession::onDCCFileRecv( irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length )
{

}
