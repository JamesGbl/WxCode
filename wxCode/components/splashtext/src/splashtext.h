/////////////////////////////////////////////////////////////////////////////
// Name:        splashtext.h
// Purpose:     SplashText: adds a version number to a bitmap
// Author:      Julian Smart
// Modified by:
// Created:     21/8/2000
// RCS-ID:      $Id: splashtext.h,v 1.1.1.1 2003-12-28 21:05:25 wyo Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MAKESPLASH_H_
#define _MAKESPLASH_H_

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#ifdef __GNUG__
    #pragma interface "splashtext.h"
#endif

class MyFrame;
class wxBitmapViewerWindow;

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    MyApp();
    virtual bool OnInit();

    bool MakeSplash();
	void SetDefaults();
	bool ProcessCommandLine(MyFrame* frame);

    void Log(const wxString& text);

protected:
    wxString    m_inputFilename;
    wxString    m_outputFilename;
    wxString    m_text;
    wxString    m_fontFaceName;
    long        m_fontPointSize;
    long        m_fontStyle;
    long        m_fontWeight;
    wxPoint     m_textPosition;
    bool        m_centre;
    bool        m_rightJustify;
    bool        m_batchMode;
    bool        m_verbose;
	bool		m_antialias;
	long		m_scaleFactor;
	wxColour	m_textForegroundColour;
	wxColour	m_textBackgroundColour;
public:
	wxCmdLineParser	m_parser;
	wxString	m_cmdLine;
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnShowUsage(wxCommandEvent& event);
    void OnShowReadMe(wxCommandEvent& event);
    void OnSetArgs(wxCommandEvent& event);

    void Log(const wxString& text);

    wxTextCtrl*				m_logCtrl;
    wxSplitterWindow*		m_splitterWindow;
    wxBitmapViewerWindow*	m_viewerWindow;

private:
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};

/*
 * wxBitmapViewerWindow
 * Displays a bitmap, setting the scrollbars appropriately
 */

class wxBitmapViewerWindow: public wxScrolledWindow
{
public:
    wxBitmapViewerWindow(wxWindow* parent, wxWindowID id = -1,
      const wxPoint& pos = wxDefaultPosition,
      const wxSize& sz = wxDefaultSize,
      long style = 0);

//// Overrides

//// Events
	void OnPaint(wxPaintEvent& event);

/// Operations
	void SetBitmap(const wxBitmap& bitmap);
	const wxBitmap& GetBitmap() const { return m_bitmap; }

    
//// Data members
protected:
	wxBitmap	m_bitmap;

    DECLARE_EVENT_TABLE()
	DECLARE_CLASS(wxBitmapViewerWindow)
};


// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    SplashText_Quit = 100,
    SplashText_SetArgs,
    SplashText_ShowUsage,
    SplashText_ShowReadme,
    SplashText_LogCtrl,
    SplashText_ViewerWindow,
    SplashText_SplitterWindow,
    SplashText_About
};

// Do crude antialiasing on the given image
wxImage wxAntiAlias(const wxImage& image);
wxImage wxAntiAlias2(const wxImage& image);

// Convert a colour to a 6-digit hex string
wxString wxColourToHexString(const wxColour& col);

// Convert 6-digit hex string to a colour
wxColour wxHexStringToColour(const wxString& hex);

// Determine the image type for a given filename
int wxDetermineImageType(const wxString& filename);

#endif
    // _MAKESPLASH_H_
