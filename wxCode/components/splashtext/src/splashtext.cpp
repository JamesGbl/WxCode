/////////////////////////////////////////////////////////////////////////////
// Name:        splashtext.cpp
// Purpose:     SplashText: adds text (such as a version number) to a bitmap
// Author:      Julian Smart
// Modified by:
// Created:     21/8/2000
// RCS-ID:      $Id: splashtext.cpp,v 1.1.1.1 2003-12-28 21:05:25 wyo Exp $
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
    { wxCMD_LINE_SWITCH, "h", "help",           "show usage" },
    { wxCMD_LINE_SWITCH, "v", "verbose",        "be verbose" },
    { wxCMD_LINE_SWITCH, "q", "quiet",          "be quiet" },
    { wxCMD_LINE_SWITCH, "b", "batch",          "batch mode (no GUI)" },
    { wxCMD_LINE_SWITCH, "bo", "bold",          "use a bold font" },
    { wxCMD_LINE_SWITCH, "it", "italic",        "use an italic font" },
    { wxCMD_LINE_SWITCH, "r", "right-justify",  "right justify the text" },
    { wxCMD_LINE_SWITCH, "l", "left-justify",   "left justify the text (the default)" },
    { wxCMD_LINE_SWITCH, "c", "centre",         "centre the text" },
    { wxCMD_LINE_SWITCH, "a", "antialias",     "antialias (smooth) the text" },

    { wxCMD_LINE_OPTION, "o", "output",         "output file (finished splash screen)" },
    { wxCMD_LINE_OPTION, "i", "input",          "input file (template splash screen)" },
    { wxCMD_LINE_OPTION, "t", "text",           "text to add to splash screen" },
    { wxCMD_LINE_OPTION, "f", "face",           "facename to use" },
    { wxCMD_LINE_OPTION, "p", "pointsize",      "point size to use", wxCMD_LINE_VAL_NUMBER },
    { wxCMD_LINE_OPTION, "x", "x-position",     "x position of text", wxCMD_LINE_VAL_NUMBER },
    { wxCMD_LINE_OPTION, "y", "y-position",     "y position of text", wxCMD_LINE_VAL_NUMBER },
    { wxCMD_LINE_OPTION, "fcol", "foreground-colour",     "text foreground colour in 6-digit hex" },
    { wxCMD_LINE_OPTION, "bcol", "background-colour",     "text background colour in 6-digit hex" },
    { wxCMD_LINE_OPTION, "m", "magnify",     "magnification factor (amount text is magnified, then scaled down)", wxCMD_LINE_VAL_NUMBER },

    { wxCMD_LINE_NONE }
};

MyApp::MyApp()
{
	SetDefaults();
}

void MyApp::SetDefaults()
{
    m_inputFilename = "";
    m_outputFilename = "";
    m_text = "";
    m_fontFaceName = "Arial";
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

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

    // create the main application window
    MyFrame *frame = new MyFrame("SplashText",
                                 wxPoint(50, 50), wxSize(500, 500));

	// Reconstruct the command line for future editing by user
	int i;
	for (i = 1; i < argc; i++)
	{
		wxString str(argv[i]);
		if (str.Find(' ') != -1)
			str = wxString(wxT('"')) + str + wxString(wxT('"'));
		m_cmdLine += str;
		if (i < (argc - 1))
			m_cmdLine += " ";
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
        Log("Sorry, syntax error.");
		success = FALSE;
    }
    else
    {
        if (m_parser.Found("h"))
        {
            m_parser.Usage();
			success = FALSE;
        }
        else
        {
			m_batchMode = m_parser.Found("b");

			// command line OK. Find option values.
			if (!m_parser.Found("i", & m_inputFilename))
			{
				Log("You must supply an input filename.");
				m_batchMode = FALSE;
				success = FALSE;
			}
			if (!m_parser.Found("o", & m_outputFilename))
			{
				Log("You must supply an output filename.");
				m_batchMode = FALSE;
				success = FALSE;
			}
			if (!m_parser.Found("t", & m_text))
			{
				Log("You must supply some text to add.");
				success = FALSE;
				m_batchMode = FALSE;
			}
			m_parser.Found("f", & m_fontFaceName);
			m_parser.Found("p", & m_fontPointSize);
			long x = 0, y = 0;
			m_parser.Found("x", & x);
			m_parser.Found("y", & y);
			m_textPosition = wxPoint(x, y);

			wxString colourStr;
			if (m_parser.Found("fcol", & colourStr))
			{
				m_textForegroundColour = wxHexStringToColour(colourStr);
			}
			if (m_parser.Found("bcol", & colourStr))
			{
				m_textBackgroundColour = wxHexStringToColour(colourStr);
			}
			m_parser.Found("m", & m_scaleFactor);
			
			// Find switches
			m_verbose = m_parser.Found("v");
			m_centre = m_parser.Found("c");
			m_rightJustify = m_parser.Found("r");
			m_antialias = m_parser.Found("a");
			if (m_parser.Found("bo"))
				m_fontWeight = wxBOLD;
			if (m_parser.Found("it"))
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
		msg.Printf("Sorry, could not load template image %s", (const char*) m_inputFilename);
		wxLogMessage(msg);
		return FALSE;
	}

	wxString msg;
	msg.Printf("Creating new file %s from template file %s and text %s",
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

    wxImage image(bitmap);
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
    bitmap = image.ConvertToBitmap();

	// Now draw the image into the template image

	wxBitmap templateBitmap = templateImage.ConvertToBitmap();
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
	memDCTemplate.DrawBitmap(bitmap, x, y, TRUE);
	memDCTemplate.SelectObject(wxNullBitmap);

	wxImage completeImage(templateBitmap);

	int saveImageType = wxDetermineImageType(m_outputFilename);
	if (saveImageType == -1)
	{
		wxLogMessage("Sorry, unknown output image file type.");
		return FALSE;
	}

	((MyFrame*) GetTopWindow())->m_viewerWindow->SetBitmap(templateBitmap);

	// TODO: get the depth from the original image, and set for this image.
	// May have to do explicit image reduction for this to work.

	if (!completeImage.SaveFile(m_outputFilename, saveImageType))
	{
		wxString msg;
		msg.Printf("Sorry, could not save image to %s", (const char*) m_outputFilename);
		wxLogMessage(msg);
		return FALSE;
	}
#if 0
    if (wxTheClipboard->Open())
    {
        wxTheClipboard->Clear();
        wxTheClipboard->SetData(new wxBitmapDataObject(bitmap));
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
    wxMenu *menuFile = new wxMenu("", wxMENU_TEAROFF);

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(SplashText_ShowUsage, "Show &Usage\tCtrl-U", "Show usage");
    helpMenu->Append(SplashText_ShowReadme, "Show &ReadMe\tCtrl-M", "Show ReadMe file");
    helpMenu->AppendSeparator();
    helpMenu->Append(SplashText_About, "&About...\tCtrl-A", "Show about dialog");

    menuFile->Append(SplashText_SetArgs, "&Run Command...\tCtrl-R", "Run command after prompting");
    menuFile->Append(SplashText_Quit, "E&xit\tAlt-X", "Quit this program");

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&File");
    menuBar->Append(helpMenu, "&Help");

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
    SetStatusText("Welcome to MakeSplash");
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

    wxMessageBox(msg, "About SplashText", wxOK | wxICON_INFORMATION, this);
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
	wxString args = wxGetTextFromUser(_("Please enter command line arguments"), _("MakeSplash"),
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
	if (wxFileExists("splashtext.txt"))
		m_logCtrl->LoadFile("splashtext.txt");
	else
	{
		wxMessageBox("Sorry, could not find splashtext.txt", "SplashText", wxOK|wxICON_INFORMATION);
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

       anti.SetRGB( x, y, (unsigned char) red, (unsigned char) green, (unsigned char) blue );
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
    if (ext == "jpg" || ext == "jpeg")
        return wxBITMAP_TYPE_JPEG;
    else if (ext == "gif")
        return wxBITMAP_TYPE_GIF;
    else if (ext == "bmp")
        return wxBITMAP_TYPE_BMP;
    else if (ext == "png")
        return wxBITMAP_TYPE_PNG;
    else if (ext == "pcx")
        return wxBITMAP_TYPE_PCX;
    else if (ext == "tif" || ext == "tiff")
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
