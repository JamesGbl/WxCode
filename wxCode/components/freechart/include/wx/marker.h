/////////////////////////////////////////////////////////////////////////////
// Name:	marker.h
// Purpose:
// Author:	Moskvichev Andrey V.
// Created:	2008/11/07
// RCS-ID:	$Id: wxAdvTable.h,v 1.3 2008/11/07 16:42:58 moskvichev Exp $
// Copyright:	(c) 2008 Moskvichev Andrey V.
// Licence:	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef MARKER_H_
#define MARKER_H_

#include <wx/wxfreechartdefs.h>
#include <wx/drawobject.h>

class WXDLLEXPORT Marker : public DrawObject
{
public:
	Marker();
	virtual ~Marker();

	virtual void Draw(wxDC &dc, wxRect rcData) = 0;
};

class WXDLLEXPORT PointMarker : public Marker
{
public:
	PointMarker();
	virtual ~PointMarker();

	virtual void Draw(wxDC &dc, wxRect rcData);

	void SetText(const wxString &text)
	{
		m_text = text;
		FireNeedRedraw();
	}

private:
	wxString m_text;
	wxFont m_textFont;
	wxColour m_textColour;
};


class WXDLLEXPORT LineMarker
{
public:
	LineMarker(wxPen linePen);
	virtual ~LineMarker();

	virtual void Draw(wxDC &dc, wxRect rcData);
};

class WXDLLEXPORT RangeMarker
{
public:
	RangeMarker(wxBrush brush, wxPen pen);
	virtual ~RangeMarker();

	virtual void Draw(wxDC &dc, wxRect rcData);
};

#endif /*MARKER_H_*/