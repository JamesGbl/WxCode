/////////////////////////////////////////////////////////////////////////////
// Name:        simple.cpp
// Purpose:     Demonstrates use of wxFreePhone class
// Author:      Julian Smart
// Modified by:
// Created:     10/6/98
// Copyright:   Julian Smart
// Licence:
/////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>

#include "tts/freephone.h"

#include "simple.h"

#include "resource.h"

#ifdef WIN32
#define _EXPORT 
#else
#define _EXPORT __export
#endif

HINSTANCE		g_instance;

wxFreePhone g_freephone("..\\..\\..\\dictionary\\english");

static BOOL InitInstance( HINSTANCE, int, LPSTR );

BOOL _EXPORT FAR PASCAL AboutDlgProc( HWND hwnd, unsigned msg,
				UINT wparam, LONG lparam );

BOOL _EXPORT FAR PASCAL TTSDlgProc( HWND hwnd, unsigned msg,
				UINT wparam, LONG lparam );

/*
 * WinMain - initialization, message loop
 */
int WINAPI WinMain( HINSTANCE this_inst, HINSTANCE prev_inst, LPSTR cmdline,
		    int cmdshow )
{
    SetLastError(0);

    g_instance = this_inst;
    if( !InitInstance( this_inst, cmdshow, cmdline ) ) return( FALSE );

	return 0;

} /* WinMain */

static BOOL InitInstance( HINSTANCE this_inst, int cmdshow, LPSTR cmdline )
{
	g_freephone.Init();

	DLGPROC	proc = (DLGPROC)MakeProcInstance( (DLGPROC)TTSDlgProc, g_instance );
	DialogBox( g_instance,"TTS_Dialog", NULL, proc );
	FreeProcInstance( proc );
    return( TRUE );
                        
} /* InitInstance */

/*
 * AboutDlgProc - processes messages for the about dialog.
 */
BOOL _EXPORT FAR PASCAL AboutDlgProc( HWND hwnd, unsigned msg,
				UINT wparam, LONG lparam )
{
    lparam = lparam;			/* turn off warning */

    switch( msg ) {
    case WM_INITDIALOG:
	return( TRUE );

    case WM_COMMAND:
    	if( LOWORD( wparam ) == IDOK ) {
	    EndDialog( hwnd, TRUE );
	    return( TRUE );
	}
	break;
    }
    return( FALSE );

} /* AboutDlgProc */

/*
 * TTSDlgProc - processes messages for the TTS dialog.
 */
BOOL _EXPORT FAR PASCAL TTSDlgProc( HWND hwnd, unsigned msg,
				UINT wparam, LONG lparam )
{
    lparam = lparam;			/* turn off warning */

    switch( msg ) {
    case WM_INITDIALOG:
	{
		// Centre on screen
		HDC dc = ::GetDC(NULL);
		int screenWidth = GetDeviceCaps(dc, HORZRES);
		int screenHeight = GetDeviceCaps(dc, VERTRES);
		ReleaseDC(NULL, dc);

		RECT rect;
		GetWindowRect(hwnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		int x = (screenWidth - width)/2;
		int y = (screenHeight - height)/2;

		MoveWindow(hwnd, x, y, width, height, FALSE);

		return( TRUE );
	}

    case WM_COMMAND:
	{
        switch (LOWORD( wparam ))
        {
            case IDOK:
            {
	            EndDialog( hwnd, TRUE );
				PostQuitMessage( 0 );
	            return( TRUE );
                break;
            }
            case IDC_SAVE_TO_FILE:
            {
				HWND editCtrl = GetDlgItem(hwnd, IDC_EDIT_TEXT);
				int len = GetWindowTextLength(editCtrl);
				char *windowText = new char[len+1];
				GetWindowText(editCtrl, windowText, len+1);
				
				if (strlen(windowText) != 0)
				{
					char* phonemes = NULL;
					g_freephone.TextToPhonemeFile(windowText, "test.pho");
				}
				delete[] windowText;

                MessageBox(hwnd, "Written to test.pho.", "Simple TTS", MB_OK);

	            return( TRUE );
                break;
            }
            default:
            {
                break;
            }
        }
		break;
	}
	}

    return( FALSE );

} /* TTSDlgProc */


