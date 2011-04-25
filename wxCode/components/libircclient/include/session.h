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
#ifndef __LIRCCSESSION_H__
#define __LIRCCSESSION_H__

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

class WXDLLIMPEXP_LIBIRCLIENT wxLIRCCSession : public wxThread
{
friend class wxLIRCCSessionManager;

public:
	wxLIRCCSession( irc_callbacks_t Callbacks );

	/** wxLIRCCSession Properties */
	unsigned int Port() {
		return m_port;
	}
	const wxString Server() {
		wxString server( m_server, wxConvUTF8 );
		return server;
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

	/** wxLIRCCSession Methods */
	int Connect( const char * server,
	                unsigned short port,
	                const char * server_password,
	                const char * nickname,
	                const char * username,
	                const char * realname );

	int ConnectIPV6(const char * server,
	                unsigned short port,
	                const char * server_password,
	                const char * nickname,
	                const char * username,
	                const char * realname );

	void Disconnect() {
		irc_disconnect( handle );
	} //    this->Delete(0); }
	int IsConnected() {
		return irc_is_connected( handle );
	}
	int SendRaw ( const char * message ) {
		return irc_send_raw ( handle, message );
	}
	int Quit ( const char * reason ) {
		return irc_cmd_quit ( handle, reason );
	}
	int Join ( const char * channel, const char * key ) {
		return irc_cmd_join ( handle, channel, key );
	}
	int Part ( const char * channel ) {
		return irc_cmd_part ( handle, channel );
	}
	int SendInvite ( const char * nick, const char * channel ) {
		return irc_cmd_invite ( handle, nick, channel);
	}
	int GetNames ( const char * channel ) {
		return irc_cmd_names ( handle, channel );
	}
	int GetList ( const char * channel ) {
		return irc_cmd_list ( handle, channel );
	}
	int SetTopic ( const char * channel, const char * topic ) {
		return irc_cmd_topic ( handle, channel, topic );
	}
	int SetChannelMode ( const char * channel, const char * mode ) {
		return irc_cmd_channel_mode ( handle, channel, mode );
	}
	int SetUserMode ( const char * mode ) {
		return irc_cmd_user_mode ( handle, mode );
	}
	int SetNick ( const char * newnick ) {
		return irc_cmd_nick ( handle, newnick );
	}
	int Whois ( const char * nick ) {
		return irc_cmd_whois ( handle, nick );
	}
	int SendMsg ( const char * target, const char * message ) {
		return irc_cmd_msg  ( handle, target, message );
	}
	int Action ( const char * target, const char * message ) {
		return irc_cmd_me ( handle, target, message );
	}
	int SendNotice ( const char * target, const char * message ) {
		return irc_cmd_notice ( handle, target, message );
	}
	int Kick ( const char * nick, const char * channel, const char * reason ) {
		return irc_cmd_kick ( handle, nick, channel, reason);
	}
	int CTCPRequest ( const char * nick, const char * request ) {
		return irc_cmd_ctcp_request ( handle, nick, request);
	}
	int CTCPReply ( const char * nick, const char * reply ) {
		return irc_cmd_ctcp_reply ( handle, nick, reply);
	}
	void GetNickFromTarget ( const char * target, char *nick, size_t size ) {
		irc_target_get_nick ( target, nick, size );
	}
	void GetHostFromTarget ( const char * target, char *nick, size_t size ) {
		irc_target_get_host ( target, nick, size );
	}
	int DCCChat ( void * ctx, const char * nick, irc_dcc_callback_t callback, irc_dcc_t * dccid ) {
		return irc_dcc_chat ( handle, ctx, nick, callback, dccid );
	}
	int DCCMsg	( irc_dcc_t dccid, const char * message ) {
		return irc_dcc_msg ( handle, dccid, message );
	}
	int	DCCAccept ( irc_dcc_t dccid, void * ctx, irc_dcc_callback_t callback ) {
		return irc_dcc_accept ( handle, dccid, ctx, callback );
	}
	int DCCDecline ( irc_dcc_t dccid ) {
		return irc_dcc_decline ( handle, dccid );
	}
	int DCCSend ( void * ctx, const char * nick, const char * filename, irc_dcc_callback_t callback, irc_dcc_t * dccid ) {
		return irc_dcc_sendfile ( handle, ctx, nick, filename, callback, dccid );
	}
	int DCCDestroy ( irc_dcc_t dccid ) {
		return irc_dcc_destroy ( handle, dccid );
	}
	void GetVersion ( unsigned int * high, unsigned int * low ) {
		irc_get_version ( high, low );
	}
	void SetContext ( void * ctx ) {
		irc_set_ctx ( handle, ctx );
	}
//        void GetContext () { *irc_get_ctx ( handle ); }
	int GetErrorNumber () {
		return irc_errno ( handle );
	}
	const char * GetErrorString ( int err_number ) {
		return irc_strerror ( err_number );
	}
	void SetLibOption ( unsigned int option ) {
		irc_option_set ( handle, option );
	}
	void ResetLibOption ( unsigned int option ) {
		irc_option_reset ( handle, option );
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

private:
	/** thread execution starts here */
	virtual void *Entry();

	/** called when the thread exits - whether it terminates normally or is
	    stopped with Delete() (but not when it is Kill()ed!) */
	virtual void OnExit();

	/** wxLIRCCSession handle */
	irc_session_t* handle;

	/** wxLIRCCSession Events */
	void onConnect ( const char * event, const char * origin, const char ** params, unsigned int count );
	void onNick ( const char * event, const char * origin, const char ** params, unsigned int count );
	void onQuit ( const char * event, const char * origin, const char ** params, unsigned int count );
	void onJoin ( const char * event, const char * origin, const char ** params, unsigned int count );
	void onPart ( const char * event, const char * origin, const char ** params, unsigned int count );
	void onMode ( const char * event, const char * origin, const char ** params, unsigned int count );
	void onUserMode ( const char * event, const char * origin, const char ** params, unsigned int count );
	void onTopic ( const char * event, const char * origin, const char ** params, unsigned int count );
	void onKick ( const char * event, const char * origin, const char ** params, unsigned int count );
	void onChanMsg ( const char * event, const char * origin, const char ** params, unsigned int count );
	void onPrivMsg ( const char * event, const char * origin, const char ** params, unsigned int count );
	void onNotice ( const char * event, const char * origin, const char ** params, unsigned int count );
	void onChanNotice ( const char * event, const char * origin, const char ** params, unsigned int count );
	void onInvite ( const char * event, const char * origin, const char ** params, unsigned int count );
	void onCTCPReq ( const char * event, const char * origin, const char ** params, unsigned int count );
	void onCTCPRep ( const char * event, const char * origin, const char ** params, unsigned int count );
	void onCTCPAction ( const char * event, const char * origin, const char ** params, unsigned int count );
	void onUnknown ( const char * event, const char * origin, const char ** params, unsigned int count );

	void onNumeric ( unsigned int event, const char * origin, const char ** params, unsigned int count );

	void onDCCChatReq ( const char * nick, const char * addr, irc_dcc_t dccid );
	void onDCCSendReq ( const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid );

	void onDCCRecv ( irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length );
	void onDCCFileRecv ( irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length );

	unsigned int m_port;
	const char * m_server;
	const char * m_svrpwd;
	const char * m_nick;
	const char * m_user;
	const char * m_name;
};

#endif //__LIRCCSESSION_H__
