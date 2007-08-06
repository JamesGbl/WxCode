//
// Copyright (C) 2007 Arne Steinarson <arst at users dot sourceforge dot net>
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any
// damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute
// it freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must
//    not claim that you wrote the original software. If you use this
//    software in a product, an acknowledgment in the product
//    documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must
//    not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source
//    distribution.
//

#ifndef DIR_H
#define DIR_H

// Predefined:
//   exec     - executable path
//   user     - user config directory
//   user-app - user data directory (usually user/app_name)

// Not predefined (must be set from wxApp::OnInit)
//   app      - Where application resources are (same dir
//              as exec or parent (or higher) directory)         
wxString wxGetNamedDir( const wxChar *name );
bool wxSetNamedDir( const wxChar *name, const wxChar *path );

// From an array of names, return the 1st match
wxString wxGetFileInOneOf( const wxChar *file, const char** arr, int cnt=-1 );

#if wxUSE_UNICODE==1
// Provide local code page (ANSI) versions
wxCharBuffer wxGetNamedDirAC( const char *name );
#define wxGetNamedDir(name) ((const char*)wxGetNamedDirAC(name))

bool wxSetNamedDir( const char *name, const char *path );
#endif

// Will locate sub_dir in base or parent directory of base
wxString wxFindDir( const wxChar *base, const wxChar *sub_dir );
wxString wxFindFile( const wxChar *base, const wxChar *file );

#endif // DIR_H

