// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/textctrl.h"
#endif //WX_PRECOMP

#include "wx/spinbutt.h"
#include "timepickerctrl.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// the margin between the text control and the spin
static const wxCoord MARGIN = 2;

// ----------------------------------------------------------------------------
// TimePickerCtrlText: text control used by spin control
// ----------------------------------------------------------------------------

class TimePickerCtrlText : public wxTextCtrl
{
public:
    TimePickerCtrlText(TimePickerCtrl *timepicker, const wxString& value)
        : wxTextCtrl(timepicker->GetParent(), wxID_ANY, value)
    {
        m_timepicker = timepicker;
        m_keyPressedQ = false;

        // remove the default minsize, the spinctrl will have one instead
        SetSizeHints(wxDefaultCoord,wxDefaultCoord);
    }

protected:
    void OnTextChange(wxCommandEvent& event)
    {
      wxWindow* p = m_timepicker->GetParent();
		TimePickerEvent tpEvent(p);
		p->AddPendingEvent(tpEvent);

        event.Skip();
    }

    void OnMouse(wxMouseEvent& event)
    {
    	if(event.GetEventType()==wxEVT_LEFT_DOWN)
    	{
			wxString s = GetValue();
			long int pos1 = s.Find(':');
			long int pos2 = s.Find(':',true);
			long int pos3 = s.Find(' ',true);
			int w,h;
			GetTextExtent(s.SubString(0,pos1),&w,&h);
			int hr_min_boundary = w;
			GetTextExtent(s.SubString(0,pos2),&w,&h);
			int min_sec_boundary = w;
			GetTextExtent(s.SubString(0,pos3),&w,&h);
			int sec_xm_boundary = w;
			int x, y;
    		m_keyPressedQ = false;
    		SetFocus();
    		event.GetPosition(&x,&y);
    		if(x>hr_min_boundary&&x<min_sec_boundary)
				m_timepicker->SetTimeField(TIME_FIELD_MINUTE);
			else if(x>=min_sec_boundary&&x<sec_xm_boundary)
				m_timepicker->SetTimeField(TIME_FIELD_SECOND);
			else if(x>=sec_xm_boundary)
				m_timepicker->SetTimeField(TIME_FIELD_AM_PM);
			else
				m_timepicker->SetTimeField(TIME_FIELD_HOUR);
		}
    	//event.Skip();
    }

    bool ProcessEvent(wxEvent &event)
    {
        // Hand button down events to TimePickerCtrl. Doesn't work.
        if (event.GetEventType() == wxEVT_LEFT_DOWN && m_timepicker->ProcessEvent( event ))
            return true;

        return wxTextCtrl::ProcessEvent( event );
    }

    void OnChar(wxKeyEvent& event)
    {
    	int keycode = event.GetKeyCode();
    	if(keycode>=(int)'0'&&keycode<=(int)'9')
    	{
    		int val = atoi((char*)(&keycode));
    		if(!m_keyPressedQ)
    		{
    			if(m_timepicker->pmQ()&&m_timepicker->GetTimeField()==TIME_FIELD_HOUR)
					val += 12;
    			m_timepicker->SetValue(val);
				m_keyPressedQ = true;
    		}
			else // m_keyPressedQ
			{
				int oldHour;
				switch(m_timepicker->GetTimeField())
				{
				case TIME_FIELD_HOUR:
					oldHour = (m_timepicker->pmQ())?m_timepicker->GetHour()-12:m_timepicker->GetHour();
					val += 10*oldHour;
					break;
				case TIME_FIELD_MINUTE:
					val += 10*(m_timepicker->GetMinute()%10);
					break;
				case TIME_FIELD_SECOND:
					val += 10*(m_timepicker->GetSecond()%10);
					break;
				case TIME_FIELD_AM_PM:
					val = (m_timepicker->pmQ())?2:1;
					break;
				}
				m_timepicker->SetValue(val);
				m_keyPressedQ = false;
			}
    	}
    	else if( ( keycode==(int)'a' || keycode==(int)'A' )
				&& m_timepicker->GetTimeField()==TIME_FIELD_AM_PM)
    	{
    		m_timepicker->SetValue(1);
    	}
    	else if( ( keycode==(int)'p' || keycode==(int)'P' )
				&& m_timepicker->GetTimeField()==TIME_FIELD_AM_PM)
    	{
    		m_timepicker->SetValue(2);
    	}
    	else if(keycode==WXK_LEFT)
    	{
    		m_keyPressedQ = false;
    		switch(m_timepicker->GetTimeField())
    		{
			case TIME_FIELD_HOUR:
				m_timepicker->SetSelection(0,2);
				break;
			case TIME_FIELD_MINUTE:
				m_timepicker->SetTimeField(TIME_FIELD_HOUR);
				break;
			case TIME_FIELD_SECOND:
				m_timepicker->SetTimeField(TIME_FIELD_MINUTE);
				break;
			case TIME_FIELD_AM_PM:
				m_timepicker->SetTimeField(TIME_FIELD_SECOND);
				break;
    		}
    	}
    	else if(keycode==WXK_RIGHT)
    	{
    		m_keyPressedQ = false;
    		switch(m_timepicker->GetTimeField())
    		{
			case TIME_FIELD_HOUR:
				m_timepicker->SetTimeField(TIME_FIELD_MINUTE);
				break;
			case TIME_FIELD_MINUTE:
				m_timepicker->SetTimeField(TIME_FIELD_SECOND);
				break;
			case TIME_FIELD_SECOND:
				m_timepicker->SetTimeField(TIME_FIELD_AM_PM);
				break;
			case TIME_FIELD_AM_PM:
				m_timepicker->SetSelection(9,-1);
				break;
    		}
    	}
    	else if(keycode==WXK_UP)
    	{
    		m_keyPressedQ = false;
    		switch(m_timepicker->GetTimeField())
    		{
			case TIME_FIELD_HOUR:
				m_timepicker->SetValue(m_timepicker->GetHour()+1);
				break;
			case TIME_FIELD_MINUTE:
				m_timepicker->SetValue(m_timepicker->GetMinute()+1);
				break;
			case TIME_FIELD_SECOND:
				m_timepicker->SetValue(m_timepicker->GetSecond()+1);
				break;
			case TIME_FIELD_AM_PM:
				m_timepicker->SetValue(2);
				break;
    		}
    	}
    	else if(keycode==WXK_DOWN)
    	{
    		m_keyPressedQ = false;
    		switch(m_timepicker->GetTimeField())
    		{
			case TIME_FIELD_HOUR:
				m_timepicker->SetValue(m_timepicker->GetHour()-1);
				break;
			case TIME_FIELD_MINUTE:
				m_timepicker->SetValue(m_timepicker->GetMinute()-1);
				break;
			case TIME_FIELD_SECOND:
				m_timepicker->SetValue(m_timepicker->GetSecond()-1);
				break;
			case TIME_FIELD_AM_PM:
				m_timepicker->SetValue(1);
				break;
    		}
    	}
    }

private:
    TimePickerCtrl *m_timepicker;
    bool m_keyPressedQ;

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(TimePickerCtrlText, wxTextCtrl)
    EVT_TEXT(wxID_ANY, TimePickerCtrlText::OnTextChange)
    EVT_MOUSE_EVENTS(TimePickerCtrlText::OnMouse)
    EVT_CHAR(TimePickerCtrlText::OnChar)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// TimePickerCtrlButton: spin button used by time picker control
// ----------------------------------------------------------------------------

class TimePickerCtrlButton : public wxSpinButton
{
public:
    TimePickerCtrlButton(TimePickerCtrl *timepicker, int style)
        : wxSpinButton(timepicker->GetParent())
    {
        m_timepicker = timepicker;

        SetWindowStyle(style | wxSP_VERTICAL);

        // remove the default minsize, the spinctrl will have one instead
        SetSizeHints(wxDefaultCoord,wxDefaultCoord);
    }

protected:
    void OnSpinButton(wxSpinEvent& eventSpin)
    {
    	m_timepicker->GetText()->SetFocus();
    	m_timepicker->SetValue(eventSpin.GetPosition());
        wxCommandEvent event(wxEVT_COMMAND_SPINCTRL_UPDATED, m_timepicker->GetId());
        event.SetEventObject(m_timepicker);
        event.SetInt(eventSpin.GetPosition());

        m_timepicker->GetEventHandler()->ProcessEvent(event);

        eventSpin.Skip();
    }

private:
    TimePickerCtrl *m_timepicker;

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(TimePickerCtrlButton, wxSpinButton)
    EVT_SPIN(wxID_ANY, TimePickerCtrlButton::OnSpinButton)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(TimePickerCtrl, wxControl)

void TimePickerCtrl::Init()
{
    m_text = NULL;
    m_btn = NULL;
    m_timeField = TIME_FIELD_HOUR;
    m_format = _("hh:mm:ss tt");
}

bool TimePickerCtrl::Create(	wxWindow *parent,
								wxWindowID id,
								const wxString& value,
								const wxPoint& pos,
								const wxSize& size,
								long style,
								const wxDateTime time,
								const wxString& name)
{
    if ( !wxControl::Create(parent, id, wxDefaultPosition, wxDefaultSize, style,
                            wxDefaultValidator, name) )
    {
        return false;
    }

	m_text = new TimePickerCtrlText(this,value);
	m_btn = new TimePickerCtrlButton(this,style);

	if(!value.empty())
	{
		m_text->ChangeValue(value);
		if(time!=wxInvalidDateTime)
			m_time = time;
		else
			if(!m_time.ParseTime(value))
				m_time.Set((unsigned short)12);
	}
	else
	{
		if(time!=wxInvalidDateTime)
			m_time = time;
		else
			m_time.Set((unsigned short)12);
	}
	switch(m_timeField)
	{
	case TIME_FIELD_HOUR:
		m_btn->SetRange(0, 23);
		m_btn->SetValue(m_time.GetHour());
		break;
	case TIME_FIELD_MINUTE:
		m_btn->SetRange(0,59);
		m_btn->SetValue(m_time.GetMinute());
		break;
	case TIME_FIELD_SECOND:
		m_btn->SetRange(0,59);
		m_btn->SetValue(m_time.GetSecond());
		break;
	case TIME_FIELD_AM_PM:
		m_btn->SetRange(1,2);
		m_btn->SetValue((m_time.GetHour()<12?1:2));
	}
    SetInitialSize(size);
    Move(pos);
    if(!value.empty())
		m_text->ChangeValue(value);
    else
		m_text->ChangeValue(wxString::Format(_("%2d:%02d:%02d %s"),GetHour(false),GetMinute(),GetSecond(), (pmQ()?_("PM"):_("AM"))));

	// have to disable this window to avoid interfering it with message
    // processing to the text and the button... but pretend it is enabled to
    // make IsEnabled() return true
    wxControl::Enable(false); // don't use non virtual Disable() here!
    m_isEnabled = true;

    // we don't even need to show this window itself - and not doing it avoids
    // that it overwrites the text control
    wxControl::Show(false);
    m_isShown = true;
    return true;
}

TimePickerCtrl::~TimePickerCtrl()
{
	delete m_text;
	m_text = NULL;
	delete m_btn;
	m_btn = NULL;
}

void TimePickerCtrl::DoMoveWindow(int x, int y, int width, int height)
{
    wxControl::DoMoveWindow(x, y, width, height);

    // position the subcontrols inside the client area
    wxSize sizeBtn = m_btn->GetSize();

    wxCoord wText = width - sizeBtn.x;
    m_text->SetSize(x, y, wText, height);
    m_btn->SetSize(x + wText + MARGIN, y, wxDefaultCoord, height);
}

void TimePickerCtrl::SetValue(int val)
{
    wxCHECK_RET( m_btn, _T("invalid call to TimePickerCtrl::SetValue") );
    switch(m_timeField)
    {
	case TIME_FIELD_HOUR:
		if(val>=0&&val<=23)
		{
			m_btn->SetValue(val);
			m_time.SetHour(val);
		}
		break;
	case TIME_FIELD_MINUTE:
		if(val>=0&&val<=59)
		{
			m_btn->SetValue(val);
			m_time.SetMinute(val);
		}
			break;
	case TIME_FIELD_SECOND:
		if(val>=0&&val<=59)
		{
			m_btn->SetValue(val);
			m_time.SetSecond(val);
		}
		break;
	case TIME_FIELD_AM_PM:
		if(val==1)
		{
			m_btn->SetValue(val);
			if(pmQ())
			m_time.SetHour(m_time.GetHour()-12);
		}
		else if(val==2)
		{
			m_btn->SetValue(val);
			if(!pmQ())
			m_time.SetHour(m_time.GetHour()+12);
		}
		break;
    }
	m_text->SetValue(wxString::Format(_("%2d:%02d:%02d %s"),GetHour(false),GetMinute(),GetSecond(),(pmQ()?_("PM"):_("AM"))));
	switch(m_timeField)
	{
	case TIME_FIELD_HOUR:
		SetSelection(0,2);
		break;
	case TIME_FIELD_MINUTE:
		SetSelection(3,5);
		break;
	case TIME_FIELD_SECOND:
		SetSelection(6,8);
		break;
	case TIME_FIELD_AM_PM:
		SetSelection(9,-1);
		break;
	}
}

void TimePickerCtrl::SetValue(wxDateTime dt)
{
	m_time = dt;
	int val;
    switch(m_timeField)
    {
	case TIME_FIELD_HOUR:
		m_btn->SetValue(m_time.GetHour());
		break;
	case TIME_FIELD_MINUTE:
		m_btn->SetValue(m_time.GetMinute());
		break;
	case TIME_FIELD_SECOND:
		m_btn->SetValue(m_time.GetSecond());
		break;
	case TIME_FIELD_AM_PM:
		if(pmQ())
			m_btn->SetValue(1);
		else
			m_btn->SetValue(0);
		break;
    }
	m_text->SetValue(wxString::Format(_("%2d:%02d:%02d %s"),GetHour(false),GetMinute(),GetSecond(),(pmQ()?_("PM"):_("AM"))));
	switch(m_timeField)
	{
	case TIME_FIELD_HOUR:
		SetSelection(0,2);
		break;
	case TIME_FIELD_MINUTE:
		SetSelection(3,5);
		break;
	case TIME_FIELD_SECOND:
		SetSelection(6,8);
		break;
	case TIME_FIELD_AM_PM:
		SetSelection(9,-1);
		break;
	}
}

void TimePickerCtrl::SetRange(int min, int max)
{
    wxCHECK_RET( m_btn, _T("invalid call to wxSpinCtrlg::SetRange") );

    m_btn->SetRange(min, max);
}

wxSize TimePickerCtrl::DoGetBestSize() const
{
    wxSize sizeBtn = m_btn->GetBestSize(),
           sizeText = m_text->GetBestSize();

    return wxSize(sizeBtn.x + sizeText.x + MARGIN, sizeText.y);
}

// ----------------------------------------------------------------------------
// value and range access
// ----------------------------------------------------------------------------

bool TimePickerCtrl::GetTextValue(wxDateTime* const time) const
{
	if(m_time.IsValid())
	{
		*time = m_time;
		return true;
	}
    return false;
}

int TimePickerCtrl::GetValue() const
{
    return m_btn ? m_btn->GetValue() : 0;
}

int TimePickerCtrl::GetMin() const
{
    return m_btn ? m_btn->GetMin() : 0;
}

int TimePickerCtrl::GetMax() const
{
    return m_btn ? m_btn->GetMax() : 0;
}

// ----------------------------------------------------------------------------
// operations forwarded to the subcontrols
// ----------------------------------------------------------------------------

bool TimePickerCtrl::Enable(bool enable)
{
    if ( !wxControl::Enable(enable) )
        return false;

    m_btn->Enable(enable);
    m_text->Enable(enable);

    return true;
}

bool TimePickerCtrl::Show(bool show)
{
    if ( !wxControl::Show(show) )
        return false;

    // under GTK Show() is called the first time before we are fully
    // constructed
    if ( m_btn )
    {
        m_btn->Show(show);
        m_text->Show(show);
    }

    return true;
}

bool TimePickerCtrl::Reparent(wxWindow *newParent)
{
    if ( m_btn )
    {
        m_btn->Reparent(newParent);
        m_text->Reparent(newParent);
    }

    return true;
}

bool TimePickerCtrl::SetHour(unsigned short h)
{
	if(h<24)
	{
		m_time.SetHour(h);
		return true;
	}
	else
		return false;
}

bool TimePickerCtrl::SetMinute(unsigned short m)
{
	if(m<60)
	{
		m_time.SetMinute(m);
		return true;
	}
	else
		return false;
}

bool TimePickerCtrl::SetSecond(unsigned short s)
{
	if(s<60)
	{
		m_time.SetSecond(s);
		return true;
	}
	else
		return false;
}

bool TimePickerCtrl::SetTimeField(wxTimeField f)
{
	switch(f)
	{
	case TIME_FIELD_HOUR:
		m_btn->SetRange(0,23);
		m_btn->SetValue(GetHour());
		m_timeField = f;
		SetSelection(0,2);
		return true;
	case TIME_FIELD_MINUTE:
		m_btn->SetRange(0,59);
		m_btn->SetValue(GetMinute());
		m_timeField = f;
		SetSelection(3,5);
		return true;
	case TIME_FIELD_SECOND:
		m_btn->SetRange(0,59);
		m_btn->SetValue(GetSecond());
		m_timeField = f;
		SetSelection(6,8);
		return true;
	case TIME_FIELD_AM_PM:
		m_btn->SetRange(1,2);
		m_btn->SetValue((pmQ())?2:1);
		m_timeField = f;
		SetSelection(9,-1);
		return true;
	default:
		return false;
	}
}

unsigned short TimePickerCtrl::GetHour(bool militaryQ) const
{
	unsigned short h = m_time.GetHour();
	if(militaryQ)
		return m_time.GetHour();
	h = h%12;
	if(h)
		return h;
	else
		return 12;
}

IMPLEMENT_DYNAMIC_CLASS(TimePickerEvent, wxEvent)
DEFINE_EVENT_TYPE(ptb3EVT_TIMEPICKEREVENT)

TimePickerEvent::TimePickerEvent(wxWindow* win)
{
	SetEventType(ptb3EVT_TIMEPICKEREVENT);
	SetEventObject(win);
}
