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
 * @file sessionevents.cpp
 * @author foldink (foldink@gmail.com)
 * @date 07-February-2010
 * @version 0.0.1
 */
#include "sessionevents.h"

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