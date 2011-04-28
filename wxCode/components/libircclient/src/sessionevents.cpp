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

IMPLEMENT_DYNAMIC_CLASS(wxLIRCCSessionEvent,wxCommandEvent);

const wxEventType wxLIRCC_EVT_THREAD_CONNECT = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_DISCONNECT = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_NICK = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_QUIT = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_JOIN = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_PART = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_MODE = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_U_MODE = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_TOPIC = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_KICK = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_CHANMSG = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_PRIVMSG = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_NOTICE = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_CHAN_NOTICE = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_INVITE = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_CTCPREP = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_CTCPREQ = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_CTCPACT = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_UNKNOWN = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_NUMERIC = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_DCCCHAT = wxNewEventType();
const wxEventType wxLIRCC_EVT_THREAD_DCCSEND = wxNewEventType();
