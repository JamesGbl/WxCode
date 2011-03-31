#include "session.h"
#include "sessionmanager.h"
#include "session_events.h"

#include <wx/log.h>

IRCSession::IRCSession( irc_callbacks_t Callbacks )
:
wxThread()
{
	Handle = irc_create_session( &Callbacks );
	wxLogDebug("<IRCSession::IRCSession> Started\n");
}

void IRCSession::OnExit()
{
    wxLogDebug("<IRCSession::OnExit> Disconnected.\n");
}

void *IRCSession::Entry()
{
/*
	if ( !Handle )
	{
        wxLogDebug("<*IRCSession::Entry> Session creation error.\n");
		return NULL;
	}
*/
    irc_run ( Handle );
    return NULL;
}

int IRCSession::IRCConnect( const char * server,
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

    return irc_connect( this->Handle,
                        server,
                        port,
                        server_password,
                        nickname,
                        username,
                        realname );
}

int IRCSession::IRCConnect6(const char * server,
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

    return irc_connect6( this->Handle,
                        server,
                        port,
                        server_password,
                        nickname,
                        username,
                        realname );
}

void IRCSession::OnConnect( const char * event, const char * origin, const char ** params, unsigned int count)
{
/** No extra params supplied; params is 0. */
//    IRCSessionEvent m_event = IRCSessionEvent( wxEVT_THREAD_IRC_CONNECT , -1 , this );
    wxQueueEvent( IRCSessionManager::Get(), new IRCSessionEvent( wxEVT_THREAD_IRC_CONNECT ) );
}

void IRCSession::OnNick ( const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sNewNick(params[0], wxConvUTF8);

    data.Add(sNick);       /** The person, who changes the nick. Note that it can be you! */
    data.Add(sNewNick);    /** Mandatory, contains the new nick. */

    IRCSessionEvent m_event = IRCSessionEvent( wxEVT_THREAD_IRC_NICK , -1 , this );
    m_event.SetParams(data);
    wxQueueEvent( IRCSessionManager::Get(), m_event.Clone() );
}

void IRCSession::OnQuit( const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sReason(params[0], wxConvUTF8);

    data.Add(sNick);   /** The person, who is disconnected. */
    data.Add(sReason); /** Optional, contains the reason message (user-specified). */

    IRCSessionEvent m_event = IRCSessionEvent( wxEVT_THREAD_IRC_QUIT , -1 , this );
    m_event.SetParams(data);
    wxQueueEvent( IRCSessionManager::Get(), m_event.Clone() );
}

void IRCSession::OnJoin( const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sChan(params[0], wxConvUTF8);

    data.Add(sNick); /** Who joins the channel. By comparing it with your own nickname, you can check whether your JOIN command succeed. */
    data.Add(sChan); /** Mandatory, contains the channel name. */

    IRCSessionEvent m_event = IRCSessionEvent( wxEVT_THREAD_IRC_JOIN , -1 , this );
    m_event.SetParams(data);
    wxQueueEvent( IRCSessionManager::Get(), m_event.Clone() );
}

void IRCSession::OnPart( const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sChan(params[0], wxConvUTF8);
    wxString sReason(params[1], wxConvUTF8);

    data.Add(sNick);   /** Who parts the channel. By comparing it with your own nickname, you can check whether your PART command succeed. */
    data.Add(sChan);   /** Mandatory, contains the channel name. */
    data.Add(sReason); /** Optional, contains the reason message (user-defined). */

    IRCSessionEvent m_event = IRCSessionEvent( wxEVT_THREAD_IRC_PART , -1 , this );
    m_event.SetParams(data);
    wxQueueEvent( IRCSessionManager::Get(), m_event.Clone() );
}

void IRCSession::OnMode( const char * event, const char * origin, const char ** params, unsigned int count )
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
    IRCSessionEvent m_event = IRCSessionEvent( wxEVT_THREAD_IRC_MODE , -1 , this );
    m_event.SetParams(data);
    wxQueueEvent( IRCSessionManager::Get(), m_event.Clone() );
}

void IRCSession::OnUserMode( const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sMode(params[0], wxConvUTF8);

    data.Add(sNick); /** Who changed the user mode. */
    data.Add(sMode); /** Mandatory, contains the user changed mode, like '+t', '-i' and so on. */

    IRCSessionEvent m_event = IRCSessionEvent( wxEVT_THREAD_IRC_U_MODE, -1 , this );
    m_event.SetParams(data);
    wxQueueEvent( IRCSessionManager::Get(), m_event.Clone() );
}

void IRCSession::OnTopic( const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sChan(params[0], wxConvUTF8);
    wxString sTopic(params[1], wxConvUTF8);

    data.Add(sNick);   /** Who changes the channel topic. */
    data.Add(sChan);   /** Mandatory, contains the channel name. */
    data.Add(sTopic);  /** Optional, contains the new topic. */

    IRCSessionEvent m_event = IRCSessionEvent( wxEVT_THREAD_IRC_TOPIC, -1 , this );
    m_event.SetParams(data);
    wxQueueEvent( IRCSessionManager::Get(), m_event.Clone() );
}

void IRCSession::OnKick( const char * event, const char * origin, const char ** params, unsigned int count )
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

    IRCSessionEvent m_event = IRCSessionEvent( wxEVT_THREAD_IRC_KICK, -1 , this );
    m_event.SetParams(data);
    wxQueueEvent( IRCSessionManager::Get(), m_event.Clone() );
}

void IRCSession::OnChanMsg( const char * event, const char * origin, const char ** params, unsigned int count )
{
    if ( count != 2 ) return;

    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sChan(params[0], wxConvUTF8);
    wxString sMsg(params[1], wxConvUTF8);

    data.Add(sNick);   /** Who generates the message. */
    data.Add(sChan);   /** Mandatory, contains the channel name. */
    data.Add(sMsg);    /** Optional, contains the message text. */

    IRCSessionEvent m_event = IRCSessionEvent( wxEVT_THREAD_IRC_CHANMSG, -1 , this );
    m_event.SetParams(data);
    wxQueueEvent( IRCSessionManager::Get(), m_event.Clone() );
}

void IRCSession::OnPrivMsg( const char * event, const char * origin, const char ** params, unsigned int count )
{
    if ( count != 2 ) return;

    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sMe(params[0], wxConvUTF8);
    wxString sMsg(params[1], wxConvUTF8);

    data.Add(sNick);   /** Who generates the message. */
    data.Add(sMe);     /** Mandatory, contains your nick. */
    data.Add(sMsg);    /** Optional, contains the message text. */

    IRCSessionEvent m_event = IRCSessionEvent( wxEVT_THREAD_IRC_PRIVMSG, -1 , this );
    m_event.SetParams(data);
//    wxQueueEvent( IRCSessionManager::Get(), m_event.Clone() );
	wxQueueEvent( IRCSessionManager::Get(), m_event.Clone() );
}

void IRCSession::OnNotice( const char * event, const char * origin, const char ** params, unsigned int count )
{
    if ( count != 2 ) return;

    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sTarget(params[0], wxConvUTF8);
    wxString sMsg(params[1], wxConvUTF8);

    data.Add(sNick);   /** Who generates the message. */
    data.Add(sTarget); /** Mandatory, contains target name (nick or channel). */
    data.Add(sMsg);    /** Optional, contains the message text. */

    IRCSessionEvent m_event = IRCSessionEvent( wxEVT_THREAD_IRC_NOTICE, -1 , this );
    m_event.SetParams(data);
    wxQueueEvent( IRCSessionManager::Get(), m_event.Clone() );
}

void IRCSession::OnChanNotice( const char * event, const char * origin, const char ** params, unsigned int count )
{
    if ( count != 2 ) return;

    wxArrayString data;
    wxString sNick(origin, wxConvUTF8);
    wxString sTarget(params[0], wxConvUTF8);
    wxString sMsg(params[1], wxConvUTF8);

    data.Add(sNick);   /** Who generates the message. */
    data.Add(sTarget); /** Mandatory, contains target name (nick or channel). */
    data.Add(sMsg);    /** Optional, contains the message text. */

    IRCSessionEvent m_event = IRCSessionEvent( wxEVT_THREAD_IRC_CHAN_NOTICE, -1 , this );
    m_event.SetParams(data);
    wxQueueEvent( IRCSessionManager::Get(), m_event.Clone() );
}

void IRCSession::OnInvite( const char * event, const char * origin, const char ** params, unsigned int count )
{

}

void IRCSession::OnCTCPRep( const char * event, const char * origin, const char ** params, unsigned int count )
{

}

void IRCSession::OnCTCPReq( const char * event, const char * origin, const char ** params, unsigned int count )
{

}

void IRCSession::OnCTCPAction( const char * event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
//    wxString sEvent(event, wxConvUTF8);
    wxString sNick(origin, wxConvUTF8);
    wxString sChan(params[0], wxConvUTF8);
    wxString sMsg(params[1], wxConvUTF8);

    data.Add(sNick);   /** Who generates the message. */
    data.Add(sChan);   /** Mandatory?, contains the channel name. */
    data.Add(sMsg);    /** Mandatory, the ACTION message. */

    IRCSessionEvent m_event = IRCSessionEvent( wxEVT_THREAD_IRC_CTCPACT, -1 , this );
    m_event.SetParams(data);
    wxQueueEvent( IRCSessionManager::Get(), m_event.Clone() );
}

void IRCSession::OnUnknown ( const char * event, const char * origin, const char ** params, unsigned int count )
{

}

void IRCSession::OnNumeric ( unsigned int event, const char * origin, const char ** params, unsigned int count )
{
    wxArrayString data;
    wxString sOrigin(origin, wxConvUTF8);
    wxString param;

    for ( unsigned int i = 0; i < count; ++i)
    {
        wxString param(params[i], wxConvUTF8);
        data.Add(param);
    }

    IRCSessionEvent m_event = IRCSessionEvent( wxEVT_THREAD_IRC_CHANMSG, -1 , this );
    m_event.SetId(event);
    m_event.SetCount(count);
    m_event.SetOrigin(sOrigin);
    m_event.SetParams(data);
    wxQueueEvent( IRCSessionManager::Get(), m_event.Clone() );
}

void IRCSession::OnDCCChatReq ( const char * nick, const char * addr, irc_dcc_t dccid )
{

}

void IRCSession::OnDCCSendReq ( const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid )
{

}

void IRCSession::OnDCCRecv( irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length )
{

}

void IRCSession::OnDCCFileRecv( irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length )
{

}
