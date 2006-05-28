///////////////////////////////////////////////////////////////////////////////
// Name:        dragbutt.cpp
// Purpose:     implements wxDragCtrl as a composite control
// Author:      A.J. Lavin.
// Modified by:
// Created:     27.07.04
// RCS-ID:      $Id: dragbutt.h,v 1.1 2006-05-28 12:56:12 victor_gil Exp $
// Copyright:   (c) 2004 A.J. Lavin
// License:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DRAGBUTTON_H_
#define _WX_DRAGBUTTON_H_

#include <stdio.h>

#include <wx/defs.h>
#include <wx/control.h>
#include <wx/event.h>
#include <wx/panel.h>
#include <wx/wx.h>



extern const char* wxDRAGBUTTON_NAME;



#define wxDB_VERTICAL     1
#define wxDB_HORIZONTAL   2
#define wxDB_HIDE_CURSOR   4
#define wxDB_FREEZE_CURSOR 8
#define wxDB_SHOW_RANGE_POPUP 16
#define wxDB_CHANGE_CURSOR    64



#if defined( __WXMSW__)
#define  wxDB_DEFAULT_FLAGS ( wxDB_HORIZONTAL | wxDB_FREEZE_CURSOR)
#else
#define  wxDB_DEFAULT_FLAGS ( wxDB_HORIZONTAL | wxDB_HIDE_CURSOR)
#endif


class WXDLLEXPORT wxDragButtonBase : public wxStaticText
{
 public:

  enum
  {
    DefaultPrecision = 0,
    DefaultMaxSigFigs = 15
  };

  wxDragButtonBase();
  wxDragButtonBase( wxWindow *parent,
		wxWindowID id = -1,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDB_DEFAULT_FLAGS,
		const wxString& name = wxDRAGBUTTON_NAME);
  
  double GetValue() const { return m_value; }
  double GetMin() const { return m_min; }
  double GetMax() const { return m_max; }
  int GetPrecision() const { return m_precision ; }

  bool AcceptsFocus() const{return false;}  
  void SetValue(double val);

  // Set the range of values through which the button can drag.
  // maxVal must be greater than minVal.
  void SetRange(double minVal, double maxVal);

  // Set the number digits precision. Least significant digit will be
  // 10^(-decimal)
  void SetPrecision( int precision = DefaultPrecision);

  // Set the maximum number of significant figures.
  void SetMaxSigFigs( int n = DefaultMaxSigFigs);
  
  // Select logarithmic or linear scale.
  void SetLogarithmic( bool logarithmic = true);

  // Set mouse acceleration. 1 <-> linear, > 1 accelerated.
  // The acceleration argument must be at least 1.
  void SetAcceleration( double acceleration);

  bool IsVertical() const { return (m_windowStyle & wxDB_VERTICAL) != 0; }
  bool IsLogarithmic() const { return m_logarithmic; }

  void OnEraseBackground( wxEraseEvent& event);
  void OnSize( wxSizeEvent& event);
  void OnPaint( wxPaintEvent& event);

  void OnLeftDown( wxMouseEvent& event);
  void OnLeftUp( wxMouseEvent& event);
  void OnMotion( wxMouseEvent& event);
  void OnDClick( wxMouseEvent& event);
  void OnKeyDown( wxKeyEvent& event);
  void OnEnterWin (wxMouseEvent &event);
  void OnLeaveWin (wxMouseEvent &event);
  
  static wxString ToString( double value, int precision);
  static wxString ToStringE( double value, int precision);

  // Returns the magnitude of the argument in base 10.
  static int GetMagnitude( double x);

 protected:

  virtual wxSize DoGetBestSize() const;
  virtual void DoDrawDragger( wxPaintDC& dc);

  virtual void DoDrawFrozenCursor( wxDC& dc);

  virtual void HideCursor();
  virtual void ShowCursor();

  virtual wxBitmap getNormalBitmap();
  virtual wxBitmap getMousedownBitmap();
  virtual wxBitmap getDisabledBitmap();

 private:

  DECLARE_EVENT_TABLE()
  DECLARE_DYNAMIC_CLASS( wxDragButtonBase)


  void Init();

  bool Hit( int x, int y) const;
  void RefreshAncestors();

  int GetTotalDrag( const wxPoint& mousePosition) const;
  int GetLastDrag( const wxPoint& mousePosition) const;

  // Restart the current dragging motion from the center of screen.
  // Must call after changing m_scale.
  void RecenterDrag();

  wxCursor last_cursor;
  
  wxBitmap m_normal;
  wxBitmap m_down;
  wxBitmap m_disbit;

  wxPoint m_downClick;
  wxPoint m_dragStart;
  wxPoint m_dragLast;
  
  int style;
  
  double m_value0;

  double m_min;
  double m_max;
  double m_value;
  double m_acceleration;

  int m_minPrecision;
  int m_maxSigFigs;

  // Smallest change in cursor positin that affects the m_value.
  int m_quantum;

  // Change to m_value per quantum change in cursor position.
  double m_scale;

  // Precision of m_value.  If mode is linear then precision is the
  // number of digits after the decimal point, if greater than 0. 0
  // precision indicates an integer. Precision < 0 indicates the
  // number is a multiple of 10^(-m_precision).  If the mode is
  // logarithmic, then precision euqls the number of significant
  // figures.
  int m_precision;

  unsigned int m_logSteps;

  bool m_logarithmic;
  bool m_dragging;


  double ticks_off_zero;
};


#if defined( __WXMSW__)
#include "dragbutt-msw.h"
#elif defined (__WXGTK__)
#include "dragbutt-gtk.h"
#else
// Platform not yet implemented
#error
#endif


class  wxDragButtonEvent : public wxCommandEvent
{
 public:
  
  wxDragButtonEvent( wxEventType commandType = wxEVT_NULL, int id = 0)
    : wxCommandEvent( commandType, id)
    {
    }
  wxDragButtonEvent( const wxDragButtonEvent& event)
    : wxCommandEvent( event)
    {
    }

  double GetValue() const { return m_value; }
  int GetPrecision() const { return m_precision; }

  void SetValue( double value) { m_value = value; }
  void SetPrecision( int precision) { m_precision = precision; }
	
  DECLARE_DYNAMIC_CLASS( wxDragButtonEvent)
 private:

  

  double m_value;

  // Measures precision of value in terms of number of decimal places.
  // Examples:
  // 1 indicates precise to 0.1 (i.e. 10^-1)
  // -1 indicates precise to 10 (i.e. 10^1)
  int m_precision;
};


DECLARE_EVENT_TYPE(wxEVT_DRAGBUTTON_DRAG, 133001)
DECLARE_EVENT_TYPE(wxEVT_DRAGBUTTON_PRECISION, 133002)
DECLARE_EVENT_TYPE(wxEVT_DRAGBUTTON_DOWN, 133003)
DECLARE_EVENT_TYPE(wxEVT_DRAGBUTTON_UP, 133004)
DECLARE_EVENT_TYPE(wxEVT_DRAGBUTTON_DCLICK, 133005)

typedef void (wxEvtHandler::*wxDragButtonEventFunction)(wxDragButtonEvent&);

#define EVT_DRAGBUTTON_DRAG(id, func) \
DECLARE_EVENT_TABLE_ENTRY( wxEVT_DRAGBUTTON_DRAG, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxDragButtonEventFunction) & func, (wxObject *)NULL ),

#define EVT_DRAGBUTTON_PRECISION(id, func) \
DECLARE_EVENT_TABLE_ENTRY( wxEVT_DRAGBUTTON_PRECISION, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxDragButtonEventFunction) & func, (wxObject *)NULL ),

#define EVT_DRAGBUTTON_DOWN(id, func) \
DECLARE_EVENT_TABLE_ENTRY( wxEVT_DRAGBUTTON_DOWN, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxDragButtonEventFunction) & func, (wxObject *)NULL ),

#define EVT_DRAGBUTTON_UP(id, func) \
DECLARE_EVENT_TABLE_ENTRY( wxEVT_DRAGBUTTON_UP, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxDragButtonEventFunction) & func, (wxObject *)NULL ),

#define EVT_DRAGBUTTON_DCLICK(id, func) \
DECLARE_EVENT_TABLE_ENTRY( wxEVT_DRAGBUTTON_DCLICK, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxDragButtonEventFunction) & func, (wxObject *)NULL ),

#endif
