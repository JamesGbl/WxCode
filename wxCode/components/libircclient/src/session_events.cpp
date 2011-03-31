/**
 * @file session_events.cpp
 * 	
 * This file is part of XVIRC.
 * 
 * @author foldink (foldink@gmail.com)
 * @date 07-February-2010
 * @version 0.0.1
 */
#include "session_events.h"

IMPLEMENT_DYNAMIC_CLASS(IRCSessionEvent,wxCommandEvent);

const wxEventType wxEVT_THREAD_IRC_CONNECT = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_NICK = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_QUIT = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_JOIN = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_PART = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_MODE = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_U_MODE = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_TOPIC = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_KICK = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_CHANMSG = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_PRIVMSG = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_NOTICE = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_CHAN_NOTICE = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_INVITE = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_CTCPREP = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_CTCPREQ = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_CTCPACT = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_UNKNOWN = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_NUMERIC = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_DCCCHAT = wxNewEventType();
const wxEventType wxEVT_THREAD_IRC_DCCSEND = wxNewEventType();