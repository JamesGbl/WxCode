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
 * @file session.h
 * @author foldink (foldink@gmail.com)
 * @author Andrea Zanellato (xvirc.client@gmail.com)
 * @date 
 * @version 0.0.1
 */
#ifndef __IRCSession__
#define __IRCSession__

#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include <wx/arrstr.h>
#include <wx/string.h>
#include <wx/thread.h>

#include "libircclient.h"

#include "defs.h"

typedef struct {
// 	unsigned int 	event_numeric;
	wxString 		event;
	wxString 		origin;
	wxArrayString* 	params;

} IRCEventData;

class WXDLLIMPEXP_LIBIRCLIENT IRCSession : public wxThread
{
public:
	IRCSession( irc_callbacks_t Callbacks );

	/** thread execution starts here */
	virtual void *Entry();

	/** called when the thread exits - whether it terminates normally or is
	    stopped with Delete() (but not when it is Kill()ed!) */
	virtual void OnExit();

	/** IRCSession Handle */
	irc_session_t* Handle;

	/** Parent Object */
	//wxPanel* m_panel;

	/** IRCSession Properties */
	unsigned int Port() {
		return m_port;
	}
	const char * Server() {
		return m_server;
	}
	const char * ServerPassword() {
		return m_svrpwd;
	}
	const wxString NickName() {
		wxString nick( m_nick, wxConvUTF8 );
		return nick;
	}
	const char * UserName() {
		return m_user;
	}
	const char * RealName() {
		return m_name;
	}

	/** IRCSession Methods */
	int IRCConnect( const char * server,
	                unsigned short port,
	                const char * server_password,
	                const char * nickname,
	                const char * username,
	                const char * realname );

	int IRCConnect6(const char * server,
	                unsigned short port,
	                const char * server_password,
	                const char * nickname,
	                const char * username,
	                const char * realname );

	void Disconnect() {
		irc_disconnect( Handle );
	} //    this->Delete(0); }
	int IsConnected() {
		return irc_is_connected( Handle );
	}
	int SendRaw ( const char * message ) {
		return irc_send_raw ( Handle, message );
	}
	int Quit ( const char * reason ) {
		return irc_cmd_quit ( Handle, reason );
	}
	int Join ( const char * channel, const char * key ) {
		return irc_cmd_join ( Handle, channel, key );
	}
	int Part ( const char * channel ) {
		return irc_cmd_part ( Handle, channel );
	}
	int SendInvite ( const char * nick, const char * channel ) {
		return irc_cmd_invite ( Handle, nick, channel);
	}
	int GetNames ( const char * channel ) {
		return irc_cmd_names ( Handle, channel );
	}
	int GetList ( const char * channel ) {
		return irc_cmd_list ( Handle, channel );
	}
	int SetTopic ( const char * channel, const char * topic ) {
		return irc_cmd_topic ( Handle, channel, topic );
	}
	int SetChannelMode ( const char * channel, const char * mode ) {
		return irc_cmd_channel_mode ( Handle, channel, mode );
	}
	int SetUserMode ( const char * mode ) {
		return irc_cmd_user_mode ( Handle, mode );
	}
	int SetNick ( const char * newnick ) {
		return irc_cmd_nick ( Handle, newnick );
	}
	int Whois ( const char * nick ) {
		return irc_cmd_whois ( Handle, nick );
	}
	int SendMsg ( const char * target, const char * message ) {
		return irc_cmd_msg  ( Handle, target, message );
	}
	int Action ( const char * target, const char * message ) {
		return irc_cmd_me ( Handle, target, message );
	}
	int SendNotice ( const char * target, const char * message ) {
		return irc_cmd_notice ( Handle, target, message );
	}
	int Kick ( const char * nick, const char * channel, const char * reason ) {
		return irc_cmd_kick ( Handle, nick, channel, reason);
	}
	int CTCPRequest ( const char * nick, const char * request ) {
		return irc_cmd_ctcp_request ( Handle, nick, request);
	}
	int CTCPReply ( const char * nick, const char * reply ) {
		return irc_cmd_ctcp_reply ( Handle, nick, reply);
	}
	void GetNickFromTarget ( const char * target, char *nick, size_t size ) {
		irc_target_get_nick ( target, nick, size );
	}
	void GetHostFromTarget ( const char * target, char *nick, size_t size ) {
		irc_target_get_host ( target, nick, size );
	}
	int DCCChat ( void * ctx, const char * nick, irc_dcc_callback_t callback, irc_dcc_t * dccid ) {
		return irc_dcc_chat ( Handle, ctx, nick, callback, dccid );
	}
	int DCCMsg	( irc_dcc_t dccid, const char * message ) {
		return irc_dcc_msg ( Handle, dccid, message );
	}
	int	DCCAccept ( irc_dcc_t dccid, void * ctx, irc_dcc_callback_t callback ) {
		return irc_dcc_accept ( Handle, dccid, ctx, callback );
	}
	int DCCDecline ( irc_dcc_t dccid ) {
		return irc_dcc_decline ( Handle, dccid );
	}
	int DCCSend ( void * ctx, const char * nick, const char * filename, irc_dcc_callback_t callback, irc_dcc_t * dccid ) {
		return irc_dcc_sendfile ( Handle, ctx, nick, filename, callback, dccid );
	}
	int DCCDestroy ( irc_dcc_t dccid ) {
		return irc_dcc_destroy ( Handle, dccid );
	}
	void GetVersion ( unsigned int * high, unsigned int * low ) {
		irc_get_version ( high, low );
	}
	void SetContext ( void * ctx ) {
		irc_set_ctx ( Handle, ctx );
	}
//        void GetContext () { *irc_get_ctx ( Handle ); }
	int GetErrorNumber () {
		return irc_errno ( Handle );
	}
	const char * GetErrorString ( int err_number ) {
		return irc_strerror ( err_number );
	}
	void SetLibOption ( unsigned int option ) {
		irc_option_set ( Handle, option );
	}
	void ResetLibOption ( unsigned int option ) {
		irc_option_reset ( Handle, option );
	}
	char * StripColorCodes ( const char * message ) {
		return irc_color_strip_from_mirc ( message );
	}
	char * ConvertColorsFromMirc ( const char * message ) {
		return irc_color_convert_from_mirc ( message );
	}
	char * ConvertColorsToMirc ( const char * message ) {
		return irc_color_convert_to_mirc ( message );
	}

	/** IRCSession Events */
//	wxThreadEvent *evt; // TODO : Protected or Private?

	void OnConnect ( const char * event, const char * origin, const char ** params, unsigned int count );
	void OnNick ( const char * event, const char * origin, const char ** params, unsigned int count );
	void OnQuit ( const char * event, const char * origin, const char ** params, unsigned int count );
	void OnJoin ( const char * event, const char * origin, const char ** params, unsigned int count );
	void OnPart ( const char * event, const char * origin, const char ** params, unsigned int count );
	void OnMode ( const char * event, const char * origin, const char ** params, unsigned int count );
	void OnUserMode ( const char * event, const char * origin, const char ** params, unsigned int count );
	void OnTopic ( const char * event, const char * origin, const char ** params, unsigned int count );
	void OnKick ( const char * event, const char * origin, const char ** params, unsigned int count );
	void OnChanMsg ( const char * event, const char * origin, const char ** params, unsigned int count );
	void OnPrivMsg ( const char * event, const char * origin, const char ** params, unsigned int count );
	void OnNotice ( const char * event, const char * origin, const char ** params, unsigned int count );
	void OnChanNotice ( const char * event, const char * origin, const char ** params, unsigned int count );
	void OnInvite ( const char * event, const char * origin, const char ** params, unsigned int count );
	void OnCTCPReq ( const char * event, const char * origin, const char ** params, unsigned int count );
	void OnCTCPRep ( const char * event, const char * origin, const char ** params, unsigned int count );
	void OnCTCPAction ( const char * event, const char * origin, const char ** params, unsigned int count );
	void OnUnknown ( const char * event, const char * origin, const char ** params, unsigned int count );

	void OnNumeric ( unsigned int event, const char * origin, const char ** params, unsigned int count );

	void OnDCCChatReq ( const char * nick, const char * addr, irc_dcc_t dccid );
	void OnDCCSendReq ( const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid );

	void OnDCCRecv ( irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length );
	void OnDCCFileRecv ( irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length );

private:
	unsigned int m_port;
	const char * m_server;
	const char * m_svrpwd;
	const char * m_nick;
	const char * m_user;
	const char * m_name;
};

#endif // __IRCSession__
