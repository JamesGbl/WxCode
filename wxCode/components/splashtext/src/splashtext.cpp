/////////////////////////////////////////////////////////////////////////////
// Name:        splashtext.cpp
// Purpose:     SplashText: adds text (such as a version number) to a bitmap
// Author:      Julian Smart
// Modified by:
// Created:     21/8/2000
// RCS-ID:      $Id: splashtext.cpp,v 1.2 2005-08-04 12:31:20 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#ifdef __GNUG__
    #pragma implementation "splashtext.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/clipbrd.h"
#include "wx/image.h"
#include "wx/cmdline.h"
#include "wx/splitter.h"
#include "wx/scrolwin.h"

#include "splashtext.h"

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------
// the application icon
#if defined(__WXGTK__) || defined(__WXMOTIF__)
    #include "splash.xpm"
#endif

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(SplashText_Quit,  MyFrame::OnQuit)
    EVT_MENU(SplashText_About, MyFrame::OnAbout)
    EVT_MENU(SplashText_ShowUsage, MyFrame::OnShowUsage)
    EVT_MENU(SplashText_SetArgs, MyFrame::OnSetArgs)
    EVT_MENU(SplashText_ShowReadme, MyFrame::OnShowReadMe)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

static const wxCmdLineEntryDesc cmdLineDesc[] =
{
    { wxCMD_LINE_SWITCH, wxT("h"), wxT("help"),           wxT("show usage") },
    { wxCMD_LINE_SWITCH, wxT("v"), wxT("verbose"),        wxT("be verbose") },
    { wxCMD_LINE_SWITCH, wxT("q"), wxT("quiet"),          wxT("be quiet") },
    { wxCMD_LINE_SWITCH, wxT("b"), wxT("batch"),          wxT("batch mode (no GUI)") },
    { wxCMD_LINE_SWITCH, wxT("bo"), wxT("bold"),          wxT("use a bold font") },
    { wxCMD_LINE_SWITCH, wxT("it"), wxT("italic"),        wxT("use an italic font") },
    { wxCMD_LINE_SWITCH, wxT("r"), wxT("right-justify"),  wxT("right justify the text") },
    { wxCMD_LINE_SWITCH, wxT("l"), wxT("left-justify"),   wxT("left justify the text (the default)") },
    { wxCMD_LINE_SWITCH, wxT("c"), wxT("centre"),         wxT("centre the text") },
    { wxCMD_LINE_SWITCH, wxT("a"), wxT("antialias"),     wxT("antialias (smooth) the text") },

    { wxCMD_LINE_OPTION, wxT("o"), wxT("output"),         wxT("output file (finished splash screen)") },
    { wxCMD_LINE_OPTION, wxT("i"), wxT("input"),          wxT("input file (template splash screen)") },
    { wxCMD_LINE_OPTION, wxT("t"), wxT("text"),           wxT("text to add to splash screen") },
    { wxCMD_LINE_OPTION, wxT("f"), wxT("face"),           wxT("facename to use") },
    { wxCMD_LINE_OPTION, wxT("p"), wxT("pointsize"),      wxT("point size to use"), wxCMD_LINE_VAL_NUMBER },
    { wxCMD_LINE_OPTION, wxT("x"), wxT("x-position"),     wxT("x position of text"), wxCMD_LINE_VAL_NUMBER },
    { wxCMD_LINE_OPTION, wxT("y"), wxT("y-position"),     wxT("y position of text"), wxCMD_LINE_VAL_NUMBER },
    { wxCMD_LINE_OPTION, wxT("fcol"), wxT("foreground-colour"),     wxT("text foreground colour in 6-digit hex") },
    { wxCMD_LINE_OPTION, wxT("bcol"), wxT("background-colour"),     wxT("text background colour in 6-digit hex") },
    { wxCMD_LINE_OPTION, wxT("m"), wxT("magnify"),     wxT("magnification factor (amount text is magnified, then scaled down)"), wxCMD_LINE_VAL_NUMBER },

    { wxCMD_LINE_NONE }
};

MyApp::MyApp()
{
}

void MyApp::SetDefaults()
{
    m_inputFilename = wxT("");
    m_outputFilename = wxT("");
    m_text = wxT("");
    m_fontFaceName = wxT("Arial");
    m_fontPointSize = 14;
    m_fontStyle = wxNORMAL;
    m_fontWeight = wxNORMAL;
    m_textPosition = wxPoint(0, 0);
    m_centre = FALSE;
    m_rightJustify = FALSE;
    m_batchMode = FALSE;
    m_verbose = FALSE;
	m_antialias = FALSE;
	m_scaleFactor = 4;
	m_textForegroundColour = * wxBLACK;
	m_textBackgroundColour = * wxWHITE;
}

// wxT('Main program') equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
	SetDefaults();
	wxInitAllImageHandlers();

    // create the main application window
    MyFrame *frame = new MyFrame(wxT("SplashText"),
                                 wxPoint(50, 50), wxSize(500, 500));

	// Reconstruct the command line for future editing by user
	int i;
	for (i = 1; i < argc; i++)
	{
		wxString str(argv[i]);
		if (str.Find(wxT(' ')) != -1)
			str = wxString(wxT('"')) + str + wxString(wxT('"'));
		m_cmdLine += str;
		if (i < (argc - 1))
			m_cmdLine += wxT(" ");
	}

    m_parser.SetCmdLine(argc, argv);
	m_parser.SetDesc(cmdLineDesc);

    Log(wxT("Welcome to MakeSplash, by Julian Smart, 2000."));

	bool success = ProcessCommandLine(frame);

	if (!success || !m_batchMode)
	{
		frame->Show(TRUE);
		return TRUE;
	}
	else
	{
		frame->Destroy();
		return FALSE;
	}
}

bool MyApp::ProcessCommandLine(MyFrame* frame)
{
	bool success = TRUE;

    wxLogTextCtrl textLog(frame->m_logCtrl);
	wxLog* oldTarget = wxLog::SetActiveTarget(& textLog);
	Log(m_cmdLine);
    int retValue = m_parser.Parse();
    if (retValue == -1)
    {
        // Help message
        m_parser.Usage();
		success = FALSE;
    }
    else if (retValue > 0)
    {
        // Syntax error
        Log(wxT("Sorry, syntax error."));
		success = FALSE;
    }
    else
    {
        if (m_parser.Found(wxT("h")))
        {
            m_parser.Usage();
			success = FALSE;
        }
        else
        {
			m_batchMode = m_parser.Found(wxT("b"));

			// command line OK. Find option values.
			if (!m_parser.Found(wxT("i"), & m_inputFilename))
			{
				Log(wxT("You must supply an input filename."));
				m_batchMode = FALSE;
				success = FALSE;
			}
			if (!m_parser.Found(wxT("o"), & m_outputFilename))
			{
				Log(wxT("You must supply an output filename."));
				m_batchMode = FALSE;
				success = FALSE;
			}
			if (!m_parser.Found(wxT("t"), & m_text))
			{
				Log(wxT("You must supply some text to add."));
				success = FALSE;
				m_batchMode = FALSE;
			}
			m_parser.Found(wxT("f"), & m_fontFaceName);
			m_parser.Found(wxT("p"), & m_fontPointSize);
			long x = 0, y = 0;
			m_parser.Found(wxT("x"), & x);
			m_parser.Found(wxT("y"), & y);
			m_textPosition = wxPoint(x, y);

			wxString colourStr;
			if (m_parser.Found(wxT("fcol"), & colourStr))
			{
				m_textForegroundColour = wxHexStringToColour(colourStr);
			}
			if (m_parser.Found(wxT("bcol"), & colourStr))
			{
				m_textBackgroundColour = wxHexStringToColour(colourStr);
			}
			m_parser.Found(wxT("m"), & m_scaleFactor);
			
			// Find switches
			m_verbose = m_parser.Found(wxT("v"));
			m_centre = m_parser.Found(wxT("c"));
			m_rightJustify = m_parser.Found(wxT("r"));
			m_antialias = m_parser.Found(wxT("a"));
			if (m_parser.Found(wxT("bo")))
				m_fontWeight = wxBOLD;
			if (m_parser.Found(wxT("it")))
				m_fontStyle = wxITALIC;
        }
    }
	if (success)
	{
		success = MakeSplash();
	}

	wxLog::GetActiveTarget()->Flush();
	wxLog::SetActiveTarget(oldTarget);

	return success;
}

#define USE_SCALING 1

bool MyApp::MakeSplash()
{
	// First, load the original (template) splash image.
	int imageType = wxDetermineImageType(m_inputFilename);
	wxImage templateImage;
	if (imageType == -1 || !wxFileExists(m_inputFilename) ||
		!templateImage.LoadFile(m_inputFilename, imageType))
	{
		wxString msg;
		msg.Printf(wxT("Sorry, could not load template image %s"), (const wxChar*) m_inputFilename);
		wxLogMessage(msg);
		return FALSE;
	}

	wxString msg;
	msg.Printf(wxT("Creating new file %s from template file %s and text %s"),
		(const wxChar*) m_outputFilename, (const wxChar*) m_inputFilename, (const wxChar*) m_text);
	Log(msg);

    wxScreenDC screenDC;
	bool useScaling = m_antialias && (m_scaleFactor > 1);
	int pointSize;

#if !USE_SCALING
    useScaling = FALSE;
#endif

	if (useScaling)
		pointSize = m_scaleFactor*m_fontPointSize;
	else
		pointSize = m_fontPointSize;

	wxString faceName = m_fontFaceName;

    wxFont font(pointSize, wxDEFAULT, m_fontStyle, m_fontWeight, FALSE, faceName);
    screenDC.SetFont(font);

    int w, h, scaledW, scaledH;
    screenDC.GetTextExtent(m_text, & w, & h);

	if (useScaling)
	{
		wxFont font2(m_fontPointSize, wxDEFAULT, m_fontStyle, m_fontWeight, FALSE, faceName);
		screenDC.SetFont(font2);
		screenDC.GetTextExtent(m_text, & scaledW, & scaledH);
	}

    screenDC.SetFont(wxNullFont);

	wxBrush backgroundBrush(m_textBackgroundColour, wxSOLID);

    wxBitmap bitmap(w, h);
    wxMemoryDC memDC;
    memDC.SelectObject(bitmap);
    memDC.SetFont(font);
    memDC.SetBackgroundMode(wxTRANSPARENT);
    memDC.SetTextForeground(m_textForegroundColour);
    memDC.SetBackground(backgroundBrush);
    memDC.SetBrush(backgroundBrush);
    memDC.Clear();
    memDC.DrawText(m_text, 0, 0);
    memDC.SelectObject(wxNullBitmap);

    wxImage image = bitmap.ConvertToImage();
	if (m_antialias)
	{
		wxImage anti = wxAntiAlias(image);
		image = anti;
	}

	if (useScaling)
	{
		// Now create an image and rescale it down to the original point size
		image.Rescale(w/m_scaleFactor, h/m_scaleFactor);
	}

	image.SetMaskColour(m_textBackgroundColour.Red(), m_textBackgroundColour.Green(), m_textBackgroundColour.Blue());
    
	wxBitmap bitmap2(image);

	// Now draw the image into the template image

	wxBitmap templateBitmap(templateImage);
	wxMemoryDC memDCTemplate;
	memDCTemplate.SelectObject(templateBitmap);
	
	int x, y;
	if (m_centre)
	{
		y = m_textPosition.y; // Currently, always start at this position
		x = m_textPosition.x - (image.GetWidth() / 2);
	}
	else if (m_rightJustify)
	{
		y = m_textPosition.y; // Currently, always start at this position
		x = m_textPosition.x - (image.GetWidth());
	}
	else
	{
		y = m_textPosition.y; // Currently, always start at this position
		x = m_textPosition.x;
	}
	memDCTemplate.DrawBitmap(bitmap2, x, y, TRUE);
	memDCTemplate.SelectObject(wxNullBitmap);

	wxImage completeImage = templateBitmap.ConvertToImage();

	int saveImageType = wxDetermineImageType(m_outputFilename);
	if (saveImageType == -1)
	{
		wxLogMessage(wxT("Sorry, unknown output image file type."));
		return FALSE;
	}

	((MyFrame*) GetTopWindow())->m_viewerWindow->SetBitmap(templateBitmap);

	// TODO: get the depth from the original image, and set for this image.
	// May have to do explicit image reduction for this to work.

	if (!completeImage.SaveFile(m_outputFilename, saveImageType))
	{
		wxString msg;
		msg.Printf(wxT("Sorry, could not save image to %s"), (const wxChar*) m_outputFilename);
		wxLogMessage(msg);
		return FALSE;
	}
#if 0
    if (wxTheClipboard->Open())
    {
        wxTheClipboard->Clear();
        wxTheClipboard->SetData(new wxBitmapDataObject(bitmap2));
        wxTheClipboard->Close();
    }
#endif
    return TRUE;
}

void MyApp::Log(const wxString& text)
{
    if (GetTopWindow())
    {
        ((MyFrame*) GetTopWindow())->Log(text);
    }
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
    m_logCtrl = NULL;
	m_splitterWindow = NULL;
	m_viewerWindow = NULL;

#ifdef __WXMAC__
    // we need this in order to allow the about menu relocation, since ABOUT is
    // not the default id of the about menu
    wxApp::s_macAboutMenuItemId = SplashText_About;
#endif

    // set the frame icon
    SetIcon(wxICON(splash));

    // create a menu bar
    wxMenu *menuFile = new wxMenu(wxT(""), wxMENU_TEAROFF);

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(SplashText_ShowUsage, wxT("Show &Usage\tCtrl-U"), wxT("Show usage"));
    helpMenu->Append(SplashText_ShowReadme, wxT("Show &ReadMe\tCtrl-M"), wxT("Show ReadMe file"));
    helpMenu->AppendSeparator();
    helpMenu->Append(SplashText_About, wxT("&About...\tCtrl-A"), wxT("Show about dialog"));

    menuFile->Append(SplashText_SetArgs, wxT("&Run Command...\tCtrl-R"), wxT("Run command after prompting"));
    menuFile->Append(SplashText_Quit, wxT("E&xit\tAlt-X"), wxT("Quit this program"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, wxT("&File"));
    menuBar->Append(helpMenu, wxT("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

	m_splitterWindow = new wxSplitterWindow(this, SplashText_SplitterWindow, wxDefaultPosition,
		wxDefaultSize, wxSP_3D);

    m_logCtrl = new wxTextCtrl(m_splitterWindow, SplashText_LogCtrl, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxNO_BORDER);
    m_viewerWindow = new wxBitmapViewerWindow(m_splitterWindow, SplashText_ViewerWindow, wxDefaultPosition, wxDefaultSize, 0);

	m_splitterWindow->SplitHorizontally(m_logCtrl, m_viewerWindow);
	m_splitterWindow->SetSashPosition(200);

#if 0
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText(wxT("Welcome to MakeSplash"));
#endif // wxUSE_STATUSBAR
}


// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf(_T("SplashText adds text to a bitmap."));

    wxMessageBox(msg, wxT("About SplashText"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::Log(const wxString& text)
{
    if (m_logCtrl)
    {
        m_logCtrl->AppendText(text);
        m_logCtrl->AppendText(wxT("\n"));
        m_logCtrl->SetInsertionPointEnd();
    }
}

void MyFrame::OnShowUsage(wxCommandEvent& event)
{
    wxLogTextCtrl textLog(m_logCtrl);
	wxLog* oldTarget = wxLog::SetActiveTarget(& textLog);
	wxLog::GetActiveTarget()->Flush();

	wxGetApp().m_parser.Usage();

	wxLog::SetActiveTarget(oldTarget);
}

void MyFrame::OnSetArgs(wxCommandEvent& event)
{
	wxString args = wxGetTextFromUser(_T("Please enter command line arguments"), _T("MakeSplash"),
		wxGetApp().m_cmdLine);
	if (!args.IsEmpty())
	{
		wxGetApp().SetDefaults();
		wxGetApp().m_cmdLine = args;
		wxGetApp().m_parser.SetCmdLine(wxGetApp().m_cmdLine);
		wxGetApp().ProcessCommandLine(this);
	}
}

void MyFrame::OnShowReadMe(wxCommandEvent& event)
{
	if (wxFileExists(wxT("splashtext.txt")))
		m_logCtrl->LoadFile(wxT("splashtext.txt"));
	else
	{
		wxMessageBox(wxT("Sorry, could not find splashtext.txt"), wxT("SplashText"), wxOK|wxICON_INFORMATION);
	}
}

wxImage wxAntiAlias(const wxImage& image)
{
    wxImage anti(image.GetWidth(), image.GetHeight());
	
	/* This is quite slow, but safe. Use wxImage::GetData() for speed instead. */
	
	for (int y = 0; y < image.GetHeight(); y++)
	{
		for (int x = 0; x < image.GetWidth(); x++)
		{
			if (y == 0 || x == 0 ||
				x == (image.GetWidth() - 1) ||
				y == (image.GetHeight() - 1))
			{
				anti.SetRGB(x, y, image.GetRed(x, y), image.GetGreen(x, y), image.GetBlue(x, y));
			}
			else
			{
				int red = (int) image.GetRed( x, y ) +
					(int) image.GetRed( x-1, y ) +
					(int) image.GetRed( x, y+1 ) +
					(int) image.GetRed( x+1, y+1 );
				red = red/4;
				
				int green = (int) image.GetGreen( x, y ) +
					(int) image.GetGreen( x-1, y ) +
					(int) image.GetGreen( x, y+1 ) +
					(int) image.GetGreen( x+1, y+1 );
				green = green/4;
				
				int blue = (int) image.GetBlue( x, y ) +
					(int) image.GetBlue( x-1, y ) +
					(int) image.GetBlue( x, y+1 ) +
					(int) image.GetBlue( x+1, y+1 );
				blue = blue/4;
				anti.SetRGB( x, y, red, green, blue );
			}
		}
	}
	return anti;
}

// Suggestion by Carlos Moreno
wxImage wxAntiAlias2(const wxImage& image)
{
    wxImage anti(image.GetWidth(), image.GetHeight());

  /* This is quite slow, but safe. Use wxImage::GetData() for speed instead. */

  for (int y = 1; y < image.GetHeight() - 1; y++)
    for (int x = 1; x < image.GetWidth() - 1; x++)
    {
       long red =
            ((int) image.GetRed( x-1, y-1 )) * 1 +
            ((int) image.GetRed( x, y-1 )) * 4 +
            ((int) image.GetRed( x+1, y-1 )) * 1 +
            ((int) image.GetRed( x+1, y )) * 4 +
            ((int) image.GetRed( x+1, y+1 )) * 1 +
            ((int) image.GetRed( x, y+1 )) * 4 +
            ((int) image.GetRed( x-1, y+1 )) * 1 +
            ((int) image.GetRed( x-1, y )) * 4 +
            ((int) image.GetRed( x, y )) * 20 ;

       red = red/40;

       long green =
            ((int) image.GetGreen( x-1, y-1 )) * 1 +
            ((int) image.GetGreen( x, y-1 )) * 4 +
            ((int) image.GetGreen( x+1, y-1 )) * 1 +
            ((int) image.GetGreen( x+1, y )) * 4 +
            ((int) image.GetGreen( x+1, y+1 )) * 1 +
            ((int) image.GetGreen( x, y+1 )) * 4 +
            ((int) image.GetGreen( x-1, y+1 )) * 1 +
            ((int) image.GetGreen( x-1, y )) * 4 +
            ((int) image.GetGreen( x, y )) * 20 ;

       green = green/40;

       long blue =
            ((int) image.GetBlue( x-1, y-1 )) * 1 +
            ((int) image.GetBlue( x, y-1 )) * 4 +
            ((int) image.GetBlue( x+1, y-1 )) * 1 +
            ((int) image.GetBlue( x+1, y )) * 4 +
            ((int) image.GetBlue( x+1, y+1 )) * 1 +
            ((int) image.GetBlue( x, y+1 )) * 4 +
            ((int) image.GetBlue( x-1, y+1 )) * 1 +
            ((int) image.GetBlue( x-1, y )) * 4 +
            ((int) image.GetBlue( x, y )) * 20 ;

       blue = blue/40;

       anti.SetRGB( x, y, (wxChar) red, (wxChar) green, (wxChar) blue );
    }
    return anti;
}

// Convert a colour to a 6-digit hex string
wxString wxColourToHexString(const wxColour& col)
{
    wxString hex;

    hex += wxDecToHex(col.Red());
    hex += wxDecToHex(col.Green());
    hex += wxDecToHex(col.Blue());

    return hex;
}

// Convert 6-digit hex string to a colour
wxColour wxHexStringToColour(const wxString& hex)
{
    unsigned int r = 0;
    unsigned int g = 0;
    unsigned int b = 0;
    r = wxHexToDec(hex.Mid(0, 2));
    g = wxHexToDec(hex.Mid(2, 2));
    b = wxHexToDec(hex.Mid(4, 2));

    return wxColour(r, g, b);
}

// Returns the image type, or -1, determined from the extension.
int wxDetermineImageType(const wxString& filename)
{
    wxString path, name, ext;

    wxSplitPath(filename, & path, & name, & ext);

    ext.MakeLower();
    if (ext == wxT("jpg") || ext == wxT("jpeg"))
        return wxBITMAP_TYPE_JPEG;
    else if (ext == wxT("gif"))
        return wxBITMAP_TYPE_GIF;
    else if (ext == wxT("bmp"))
        return wxBITMAP_TYPE_BMP;
    else if (ext == wxT("png"))
        return wxBITMAP_TYPE_PNG;
    else if (ext == wxT("pcx"))
        return wxBITMAP_TYPE_PCX;
    else if (ext == wxT("tif") || ext == wxT("tiff"))
        return wxBITMAP_TYPE_TIF;
    else
        return -1;
}

/*
 * wxBitmapViewerWindow
 * Displays a bitmap, setting the scrollbars appropriately
 */
IMPLEMENT_CLASS(wxBitmapViewerWindow, wxScrolledWindow)

BEGIN_EVENT_TABLE(wxBitmapViewerWindow, wxScrolledWindow)
	EVT_PAINT(wxBitmapViewerWindow::OnPaint)
END_EVENT_TABLE()

wxBitmapViewerWindow::wxBitmapViewerWindow(wxWindow* parent, wxWindowID id,
      const wxPoint& pos,
      const wxSize& sz,
      long style):
	wxScrolledWindow(parent, id, pos, sz, style)
{
}

void wxBitmapViewerWindow::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	PrepareDC(dc);

	if (!m_bitmap.Ok())
		return;

	wxSize sz = GetClientSize();

	int x = wxMax(0, (sz.x - m_bitmap.GetWidth())/2);
	int y = wxMax(0, (sz.y - m_bitmap.GetHeight())/2);

	dc.DrawBitmap(m_bitmap, x, y);
}

void wxBitmapViewerWindow::SetBitmap(const wxBitmap& bitmap)
{
	m_bitmap = bitmap;
	if (m_bitmap.Ok())
	{
		SetScrollbars(1, 1, m_bitmap.GetWidth(), m_bitmap.GetHeight());
	}
}
