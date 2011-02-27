// Doxygen documentation follows:
/** @mainpage TimePickerCtrl
*
* @version 0.2
*
* @authors Paul Breen (greenbreen@yahoo.com); adapted from the generic wxSpinCtrl by Vadim Zeitlin
*
* @date January 3, 2011
* <hr>
* @section intro Introduction
TimePickerCtrl is a wxWidgets control for entering and displaying time information.  It's a composite control made up of a wxTextCtrl and a wxSpinButton.  The time is displayed in 12 hour format like 9:30:00 AM.  The user can change the time by selecting the field (hours, minutes, seconds, or AM/PM) they want to change and using the wxSpinCtrl's arrow buttons.  For anyone familiar with MFC, this control is similar to a CDateTimeCtrl with the DTS_TIMEFORMAT style.
* <hr>
* @section notes Release Notes
This control is still in a beta stage of development.  There are no known bugs yet, but it is not extensively tested.  The control was developed on 32-bit i386 Ubuntu 10.04 LTS Desktop version using wxWidgets 2.8.11.  It is also known to build on Windows Vista with wxWidgets 2.8.  I would appreciate any feedback on experiences with this control on other platforms, or any suggestions for improvement, etc.  If you do email me, please include the string "timepickerctrl" in the subject line so I can filter the message to the appropriate folder.  I can't promise I'll provide fixes or implement suggestions or even respond to email messages, but I'll try to be as responsive as I can with the time I have available, and it can't hurt to ask.

Currently the time format is fixed.  Depending on public interest and my available time, I may implement customization.
* <hr>
* @section requirements Requirements
wxWidgets 2.8 or higher.  May work with lower versions of wxWidgets, but this has not been tested.  Please email me with your experiences using this control in other versions of wxWidgets.
* <hr>
*
* @section license License
TimePickerCtrl is released under the wxWindows license.  Please see http://www.opensource.org/licenses/wxwindows.php for details.
*/ // End doxygen documentation


/////////////////////////////////////////////////////////////////////////////
// Name:        timepickerctrl.h
// Purpose:     generic TimePickerCtrl class
// Author:      Paul Breen; adapted from generic wxSpinCtrl by Vadim Zeitlin
// Modified by:
// Created:     January 3, 2011
// Copyright:   (c) Paul Breen
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef TIMEPICKERCTRL_H_INCLUDED
#define TIMEPICKERCTRL_H_INCLUDED

class WXDLLEXPORT wxSpinButton;
class WXDLLEXPORT wxTextCtrl;

enum wxTimeField {TIME_FIELD_HOUR, TIME_FIELD_MINUTE, TIME_FIELD_SECOND, TIME_FIELD_AM_PM};

class TimePickerCtrl : public wxControl
{
public:
    TimePickerCtrl() { Init(); }
	TimePickerCtrl(wxWindow *parent,
				wxWindowID id = wxID_ANY,
				const wxString& value = wxEmptyString,
				const wxPoint& pos = wxDefaultPosition,
				const wxSize& size = wxDefaultSize,
				long style = wxSP_ARROW_KEYS,
				const wxDateTime time = wxDefaultDateTime,
				const wxString& name = _("TimePickerCtrl"))
    {
        Init();
        Create(parent, id, value, pos, size, style, time, name);
    }

	bool Create(wxWindow *parent,
				wxWindowID id = wxID_ANY,
				const wxString& value = wxEmptyString,
				const wxPoint& pos = wxDefaultPosition,
				const wxSize& size = wxDefaultSize,
				long style = wxSP_ARROW_KEYS,
				const wxDateTime time = wxDefaultDateTime,
				const wxString& name = _("TimePickerCtrl"));

	virtual ~TimePickerCtrl();

	//	operations
	/// This function sets the currently selected field
	void SetValue(int val);

	/// This function sets the TimePickerCtrl's wxDateTime to the given wxDateTime
	void SetValue(wxDateTime dt);

	/// This function sets the minimum and maximum values allowed in the currently selected field
	void SetRange(int min, int max);

	/// This function highlights the text in the TimePickerCtrl's associated wxTextCtrl between the given indices
	void SetSelection(long from, long to) const {m_text->SetSelection(from,to);}

    // accessors
    /// This function returns the value of the currently selected field
    int GetValue() const;

    /// This function returns the minimum value allowed in the currently selected field
    int GetMin() const;

    /// This function returns the maximum value allowed in the currently selected field
    int GetMax() const;

    /// This function returns the wxDateTime currently entered in the TimePickerCtrl
    wxDateTime GetDateTime() const {return m_time;}

    // implementation from now on

    // forward these functions to all subcontrols
    virtual bool Enable(bool enable = true);
	virtual bool Show(bool show = true);
	virtual bool Reparent(wxWindow *newParent);

    // get the subcontrols
    wxTextCtrl *GetText() const { return m_text; }
    wxSpinButton *GetSpinButton() const { return m_btn; }

	unsigned short GetHour(bool militaryQ = true) const;
	unsigned short GetMinute() const {return m_time.GetMinute();}
	unsigned short GetSecond() const {return m_time.GetSecond();}
   /// Returns true if the time contained in the control falls between noon and just before midnight
	bool pmQ() const {return m_time.GetHour()>=12;}

	bool SetHour(unsigned short h);
	bool SetMinute(unsigned short m);
	bool SetSecond(unsigned short s);

	/// Get which field is currently selected
	wxTimeField GetTimeField() const {return m_timeField;}

   /// Set which field is currently selected
	bool SetTimeField(wxTimeField f);

    /// Put the time value of the string in the text control into time and return
    /// true or return false if the text ctrl doesn't contain a valid time.
    bool GetTextValue(wxDateTime* const time) const;

protected:
    /// Override the base class virtuals involved into geometry calculations.
	wxSize DoGetBestSize() const;
	void DoMoveWindow(int x, int y, int width, int height);

    /// Common part of all constructors
    void Init();

private:
    // the subcontrols
	wxTextCtrl *m_text;
	wxSpinButton *m_btn;
	wxTimeField m_timeField;
	wxDateTime m_time;
	wxString m_format;

private:
    DECLARE_DYNAMIC_CLASS(TimePickerCtrl)
};

class TimePickerEvent : public wxEvent
{
public:
	TimePickerEvent(wxWindow* win = (wxWindow*)NULL);
	wxEvent* Clone() const {return new TimePickerEvent(*this);}

	DECLARE_DYNAMIC_CLASS(TimePickerEvent)
};

typedef void (wxEvtHandler::*TimePickerEventFunction)(TimePickerEvent&);

BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE(ptb3EVT_TIMEPICKEREVENT,1)
END_DECLARE_EVENT_TYPES()

#define EVT_TIMEPICKER(func)                              \
	DECLARE_EVENT_TABLE_ENTRY( ptb3EVT_TIMEPICKEREVENT,      \
		-1,                                    \
		-1,                                    \
		(wxObjectEventFunction)                \
		(TimePickerEventFunction) & func,              \
		(wxObject *) NULL ),

#endif // TIMEPICKERCTRL_H_INCLUDED
