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

#ifndef WXUTILS_H_INCLUDED
#define WXUTILS_H_INCLUDED

#include <wx/wxprec.h>
#include "wx/strconv.h"
#include "utils/types.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "wx/wxMacros.h"
#include "wx/wxStrConv.h"	

	
// Translate flags for wxWindow to readable string
const wxChar *wxFlag2Str( int flag, bool brief=true );

// Return string for give key code
wxString KeyCode2Str( int qual, int key_code );

// Save and load a wxString from UTF8 encoded file
bool wxSaveString( const wxString& str, const wxChar *path );
bool wxLoadString( wxString& str, const wxChar *path );


// Return either x or y component based on index
// Allows accessing wxPoint and wxSize as a 2-element array.
int GetCoord(const wxPoint &pt, int ix);
int GetCoord(const wxSize &sz, int ix);


// Manipulate wxLongLong bits
bool wxLLGetBit( const wxLongLong &ll, int bit );
void wxLLSetBit( wxLongLong &ll, int bit );
void wxLLClearBit( wxLongLong &ll, int bit );
//int wxLLNextBit( const wxLongLong &ll, bool set=true, int from_pos=0 );

#if wxUSE_LONGLONG_NATIVE==0
    wxLongLong operator | ( const wxLongLong &o1, const wxLongLong &o2 );
    wxLongLong operator & ( const wxLongLong &o1, const wxLongLong &o2 );
    wxLongLong operator ^ ( const wxLongLong &o1, const wxLongLong &o2 );
#endif
// Operator ~ doesn't seem to be defined in native long long type (MinGW)
wxLongLong operator ~ ( const wxLongLong &o1 );

/*
// As wxMessageBox, except will centre on local parent (not top level)
int wxLocalMsgBox(	 const wxString& message,
                     const wxString& caption = wxMessageBoxCaptionStr,
                     long style = wxOK | wxCENTRE,
                     wxWindow *parent = NULL,
                     int x = wxDefaultCoord, int y = wxDefaultCoord);

// This creates a top-level wxDialog at the center of a child window to be
// used as parent for a locally centered dialog.
class wxDummyParent : public wxDialog {
public:
	wxDummyParent( wxWindow *parent, bool do_show=true );
};
*/

/*
// This provides the class for locally centred dialogs
class wxLocalMsgDlg : public wxMessageDialog {
public:
	wxLocalMsgDlg(	wxWindow *parent, const wxString& message,
                    const wxString& caption = wxMessageBoxCaptionStr,
                    long style = wxOK|wxCENTRE,
                    const wxPoint& pos = wxDefaultPosition);	
	void Ctor( wxWindow *parent );
};
*/


// A fast critical section / mutex class
// It will work under Gcc using AtomicAdd or under
// Windows using InterlockedIncrement. For other platforms
// a replacement for this atomic increment must be found.
class wxCritSect {
public:
	// If using the crit sect statically or globally, 
	// set do_construct to false, then it can be used
	// correctly without init (it works well from the
	// zero state)
	wxCritSect( bool do_construct=true );

	void Enter( );
	void Leave( );
	bool TryEnter( );

protected:
	int m_ec;	// Entry count
	int m_lc;	// Leave count
	int m_thid;	// Thread ID of owner
	int m_tcnt; // Thread count
};


class wxCritSectLocker {
public:
	wxCritSectLocker( wxCritSect &cs ) : m_cs(cs) { m_cs.Enter(); }
	~wxCritSectLocker( ) { m_cs.Leave(); }

	wxCritSect &m_cs;
};

// Returns true if all events were processed and all queues are empty
bool wxProcessEventsUntilEmpty( );


// These functions inserts and removes an event handler
// for the wxEvtHandler class. 'handler' is put as the
// next handler for _this
void wxInsertEventHandlerBefore(wxEvtHandler *_this, wxEvtHandler *handler);
void wxRemoveEventHandlerBefore(wxEvtHandler *_this, wxEvtHandler *handler);
void wxInsertEventHandlerAfter(wxEvtHandler *_this, wxEvtHandler *handler);
void wxRemoveEventHandlerAfter(wxEvtHandler *_this, wxEvtHandler *handler);

wxString DcChopText( wxDC *dc, const wxChar *str, int w, int *ph=NULL );

// The bg color is used as mask
wxBitmap wxAuiBitmapFromBits(   const unsigned char bits[], int w, int h,
                                const wxColour& color_bg, const wxColour &color_fg,
                                bool use_mask=false );
int BitmapTypeFromExt( const wxString name );

wxWindow *FindChildWindowAt( wxWindow *wnd, const wxPoint &pt, bool full_rec=false );


// It is very difficult to find the default dialog/toolbar background colour
// from wxSystemSettings. Instead sample it directly from a window.
bool GetFrameColour( wxColour &bgc );
bool SetFrameColourFrom( wxFrame *wnd );
bool SetFrameColourFrom( wxToolBar *ptb );

bool GetCaptionColour( wxColour &cc );
bool SetCaptionColourFrom( wxWindow *pfrm, wxScreenDC *pdc=NULL );


#ifdef UTIL_OVERLOAD_MALLOC
	#define malloc(size) wxDebugAlloc( size, NULL, -1, false, false )
	#define free(pbuf) wxDebugFree( pbuf )
#endif

// For redirecting to global new/delete operators (memory.h)
void *wxMalloc( size_t size );
void wxFree( void *pv );


/*
// This is intended to be used from a main executable, to link all its 
// allocations down to wxDebugAlloc. 
// For use on Win32 when wxW is in a DLL, and to get the same heap and
// allocation point for all DLLs and the EXE.
#ifdef UTIL_OVERLOAD_NEW
	#define UTIL_INLINE inline
	UTIL_INLINE void * operator new (size_t size, wxChar * fileName, int lineNum);
	UTIL_INLINE void * operator new (size_t size);
	UTIL_INLINE void * operator new[] (size_t size);
	UTIL_INLINE void * operator new[] (size_t size, wxChar * fileName, int lineNum);
	UTIL_INLINE void operator delete (void * buf);
	UTIL_INLINE void operator delete[] (void * buf);
#endif
*/


#endif // WXUTILS_H_INCLUDED


