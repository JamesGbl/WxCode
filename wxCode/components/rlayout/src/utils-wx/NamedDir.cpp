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


#include <wx/wx.h>
#include <wx/stdpaths.h>

#include "wx/wxMacros.h"
#include "wx/wxStrConv.h"

#include "utils/ExpArr.hpp"


struct wxNamedDir {
    wxNamedDir( const wxChar *name, const wxChar *path ) : m_name(name), m_path(path) { }
    wxString m_name;
    wxString m_path;
    bool operator == (const wxChar *name){ return m_name==name; }
};

ExpArrP<wxNamedDir*> g_nd_dirs;


wxString wxGetNamedDir( const wxChar *name ){
    // If not initialized, do so now
    if( !g_nd_dirs.Size() ){
        wxStandardPaths sp;
        wxString path;
        
        path = sp.GetUserDataDir();
        if( !path.EndsWith(wxString(wxString(wxPSEP))) ) path += wxPSEP;
        g_nd_dirs.Push( new wxNamedDir(_T("user-app"), path) );
        
        path = sp.GetUserConfigDir();
        if( !path.EndsWith(wxString(wxPSEP)) ) path += wxPSEP;
        g_nd_dirs.Push( new wxNamedDir(_T("user"), path) );
        
        path = sp.GetExecutablePath();
        int p = path.Find(wxPSEP,true);
        if( p!=wxNOT_FOUND ){
            path = path.Left(p+1);
            g_nd_dirs.Push( new wxNamedDir(_T("exec"), path) );
        }
        else wxLogMessage(wxString::Format(_T("NamedDir.cpp: Path without separator - %s"),path));
    }
    
    // Lookup
    wxNamedDir *pnd = g_nd_dirs.Find( name );
    return pnd ? pnd->m_path : wxString();
}

bool wxSetNamedDir( const wxChar *name, const wxChar *_path ){
    wxString path = _path;
    if( !path.Length() ) return false;
    if( !path.EndsWith(wxString(wxPSEP)) ) path += wxPSEP;
    
    // Lookup and set
    wxNamedDir *pnd = g_nd_dirs.Find( name );
    if( pnd )
        pnd->m_path = path;
    else{
        pnd = new wxNamedDir( name, path );
        g_nd_dirs.Push( pnd );
    }
    
    return true;
}

#if wxUSE_UNICODE==1
// Input and output is in local code page
wxCharBuffer wxGetNamedDirAC( const char *name ){
    wxString path = wxGetNamedDir( AC2WX(name) );
    return wxCharBuffer( WX2AC(path) );
}

bool wxSetNamedDir( const char *name, const char *dir ){
    return wxSetNamedDir( AC2WX(name), AC2WX(dir) );
}
#endif

// These are slightly different on different builds (MSW/GTK)
bool wxFileExistsWrapper( const wxChar *file ){
    return wxFileExists( file );
}
bool wxDirExistsWrapper( const wxChar *file ){
    return wxDirExists( file );
}
typedef bool (*wxExistFunc)(const wxChar *);


//typedef bool (*wxExistFunc)(const wxString &);

// Look for directory 'dir' relative 'base'
wxString wxFindFileDir( const wxChar *base, const wxChar *file_dir, bool is_dir ){
    if( !base || !file_dir ) return wxEmptyString;
    
    wxExistFunc wxExists = is_dir ? wxDirExistsWrapper : wxFileExistsWrapper;
    
    wxString path = base;
    if( !path.EndsWith(wxString(wxPSEP)) ) path += wxPSEP;
    
    // Look upwards
    for( int ix=0; ix<4; ix++ ){
        if( wxExists(path+file_dir) ){
            while( ix-->0 ){
                path = path.Left( path.Length()-1 );
                path = path.Left( path.Find(wxPSEP,true) );
                path = path.Left( path.Find(wxPSEP,true)+1 );
            }
            return path;
        }
        path = path + _T("..") + wxPSEP;
    }
    
    return wxEmptyString;
}

wxString wxFindFile( const wxChar *base, const wxChar *file ){
    return wxFindFileDir( base, file, false );
}

wxString wxFindDir( const wxChar *base, const wxChar *dir ){
    return wxFindFileDir( base, dir, true );
}

