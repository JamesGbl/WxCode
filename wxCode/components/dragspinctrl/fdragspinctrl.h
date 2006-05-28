/*
Class: wxFSpinCtrl
File: wxfspinctrl.h
Author Víctor Alejandro Gil Sepúlveda
From: Spain
Finishing Date: 14-2-02
Definition: See Docs
*/

#ifndef _DSPINCONTROL_FLOAT_1 
#define _DSPINCONTROL_FLOAT_1

#include <wx/string.h>
#include <wx/window.h>
#include <wx/object.h>
#include <wx/event.h>
#include <wx/wx.h>
#include <wx/cursor.h>
#include "dragbutt.h"
#include <fstream>
#include <wx/image.h>

#include <iostream>
using namespace std;



#include "wxsciunitparser.h"



#include "stextctrl.h"


#define STARTID 10000000

// Events
class wxFDSCEvent : public wxCommandEvent {
public:
    wxFDSCEvent(wxEventType e= wxEVT_NULL,int id=0):wxCommandEvent(e,id){}   
    wxFDSCEvent(const wxFDSCEvent &event): wxCommandEvent(event){ }
    virtual wxEvent *Clone() const {return (new wxFDSCEvent(*this));}
    void SetDouble(double x){d=x;}
    float GetDouble(void){return d;}
    DECLARE_DYNAMIC_CLASS(wxFDSCEvent)
private:
    double d;
};

// Event of text changed
DECLARE_EVENT_TYPE(wxEVT_FDSC_TEXT_CHANGED, 13000)
// Parsing error event
DECLARE_EVENT_TYPE(wxEVT_FDSC_ERROR, 14000)

typedef void (wxEvtHandler::*wxFDSCEventFunction)(wxFDSCEvent&);

#define FDSC_TEXT_CHANGED_EVENT(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_FDSC_TEXT_CHANGED, id, wxID_ANY, \
	(wxObjectEventFunction)(wxEventFunction)(wxFDSCEventFunction)&fn, \
	(wxObject *) NULL \
    ),
    
#define FDSC_PARSE_ERROR_EVENT(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_FDSC_ERROR, id, wxID_ANY, \
	(wxObjectEventFunction)(wxEventFunction)(wxFDSCEventFunction)&fn, \
	(wxObject *) NULL \
    ),




// The Control Class
class wxFDragSpinCtrl :  public  wxPanel
{


public:
    
                wxFDragSpinCtrl          (wxWindow *parent, wxWindowID id = -1,
                                         const wxPoint& pos = wxDefaultPosition,
                                         const wxSize& size = wxDefaultSize,
                                         long style = 0,
                                         const double initial = 0, double min = 0, double max= 1, int precision = 1,
                                         int alignment = 0,
                                         const wxString& unit=_T(""),
                                         wxSciUnitParser* scup = NULL, 
                                         const wxString& name = _T("wxDragSpinCtrl"));
    
                ~wxFDragSpinCtrl         (void);
    
     void       SetUnit                 (const wxString& u);
     void       SetLabel                (const wxString& v);
     void       SetValue                (double val);
     void       SetSCUP                 (wxSciUnitParser* scup);
     void       SetDCPrecision          (int precision);
     void       SetDCAcceleration       (double a);
     void       SetDCLogarithmic        (bool b);
     void       SetDCRange              (double a,double b);
     
     wxString           GetUnit                 (void) const;
     float              GetIncrement            (void) const;
     wxString           GetLabel                (void) const;
     double             GetValue                (void) ;
     wxSciUnitParser*   GetSCUP                 (void) const;
     int                GetDCPrecision          (void) const;
     bool               GetDCLogarithmic        (void) const;
     double             GetMax                  (void) const;
     double             GetMin                  (void) const;
  
     bool       	DBpushed                (void); 
     bool       	Enable			(bool enable = true) ;
     void       	Disable			(void);
     
 
 protected:

  virtual wxSize DoGetBestSize() const;


 private:
        void            SendTextChangedEvent    (void);
        void            SendParseError          (int);
        void            OnEnter                 (wxCommandEvent& );
        void            OnDrag                  (wxDragButtonEvent& );
        void            OnLooseFocus            (wxFocusEvent& );
        void            OnLooseFocusWidget      (wxFocusEvent& );
        void            OnDown                  (wxDragButtonEvent& );
        void            OnUp                    (wxDragButtonEvent& );
        void            OnGetFocus              (wxFocusEvent&);
        void            OnGetFocus2             (wxFocusEvent&);
        void            OnKDown                 (wxKeyEvent&);
        void            OnKUp                   (wxKeyEvent&);  
        void            OnDClick                (wxDragButtonEvent&);
        void            OnPrecision             (wxDragButtonEvent&);
        void            OnClick                 (wxMouseEvent&);
           
           

        wxString            value;
        wxDragButton*       db;
        wxTextCtrl*         tc;
	    wxTextCtrl*         tc2;
        long                tc_id,id_drag,tc2_id;
        wxString            unit;
        wxBoxSizer          *all,*box;
        wxSciUnitParser*    parser;
        double              min, max;
        bool                shift,pushed,isenabled;
       
        
       
       
};


#endif


