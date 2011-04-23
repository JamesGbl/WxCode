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
 * @file defs.h
 * @author Andrea Zanellato (xvirc.client@gmail.com)
 * @date 
 * @version 0.0.1
 */
#ifndef __WXLIBIRCLIENTDEFS_H__
#define __WXLIBIRCLIENTDEFS_H__

#ifdef USING_SOURCE_LIBIRCLIENT
	#define WXDLLIMPEXP_LIBIRCLIENT
    #define WXDLLIMPEXP_DATA_LIBIRCLIENT(type) type
#elif defined( LIB_USINGDLL )
    #define WXDLLIMPEXP_LIBIRCLIENT
    #define WXDLLIMPEXP_DATA_LIBIRCLIENT(type)
#elif defined( WXMAKINGDLL_LIBIRCLIENT )
    #define WXDLLIMPEXP_LIBIRCLIENT WXEXPORT
    #define WXDLLIMPEXP_DATA_LIBIRCLIENT(type) WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_LIBIRCLIENT WXIMPORT
    #define WXDLLIMPEXP_DATA_LIBIRCLIENT(type) WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_LIBIRCLIENT
    #define WXDLLIMPEXP_DATA_LIBIRCLIENT(type) type
#endif

#endif //__WXLIBIRCLIENTDEFS_H__
