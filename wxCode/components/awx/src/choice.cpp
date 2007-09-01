#include "wx/awx-0.3/choice.h"

BEGIN_EVENT_TABLE(awxChoice, wxChoice)
    EVT_CHAR(awxChoice::OnChar)
    EVT_KEY_DOWN(awxChoice::OnChar)
    EVT_LEFT_DOWN(awxChoice::OnLeftDown)
    EVT_LEFT_DCLICK(awxChoice::OnLeftDown)
END_EVENT_TABLE()

awxChoice::awxChoice(wxWindow *parent, wxWindowID id, const wxPoint& pos, 
				 const wxSize& size, int n, const wxString choices[])
    :wxChoice(parent, id, pos, size, n, choices)
{
    for(int i=0; i<n; i++)
        m_states.Add(1);
}

#if wxCHECK_VERSION(2, 6, 0)
awxChoice::awxChoice(wxWindow *parent, wxWindowID id, const wxPoint& pos, 
				 const wxSize& size, const wxArrayString& choices)
    :wxChoice(parent, id, pos, size, choices)
{
    for(int i=0; i<choices.GetCount(); i++)
        m_states.Add(1);
}
#endif

void awxChoice::OnSelectMenuItem(wxCommandEvent& event)
{
    if(GetSelection() == event.GetId())
        return;
    SetSelection(event.GetId());
    
    wxCommandEvent evt(wxEVT_COMMAND_CHOICE_SELECTED, GetId());
    evt.SetInt( GetSelection() );
    evt.SetString( GetStringSelection() );
    evt.SetExtraLong( 1 ); // 1 stands for selection; 0 for deselection
    
    GetParent()->AddPendingEvent(evt);
}

void awxChoice::Clear()
{
    m_states.Clear();
    wxChoice::Clear();
}

void awxChoice::SetSelection( int n )
{
    if(m_states[n] == 0)
        return;
    wxChoice::SetSelection(n);
}

void awxChoice::Delete(int n)
{
    m_states.RemoveAt(n);
    wxChoice::Delete(n);
}

int awxChoice::DoAppend(const wxString& item)
{
    m_states.Add(1);
    return wxChoice::DoAppend(item);
}

#if wxCHECK_VERSION(2, 6, 0)
int awxChoice::DoInsert(const wxString& item, int pos)
{
    m_states.Insert(1, pos);
    return wxChoice::DoInsert(item, pos);
}
#endif

void awxChoice::OnLeftDown(wxMouseEvent& event)
{
    int i; 	// for old compilers, which makes i in a for not
			// local to the for
    wxMenu pop_menu;
    int n = GetCount();
    for(i=0; i<n; i++)
    {
//         pop_menu.AppendCheckItem(i, GetString(i));
	   pop_menu.Append(i, GetString(i));
//         if(GetSelection() == i)
//             pop_menu.Check(i, true);
        if(m_states[i] == 0)
            pop_menu.Enable(i, false);
    }
    Connect(0, n-1, wxEVT_COMMAND_MENU_SELECTED, 
		  (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)
		  &awxChoice::OnSelectMenuItem);
    PopupMenu(&pop_menu, 0, GetClientSize().GetHeight());
    Disconnect(0, n-1, wxEVT_COMMAND_MENU_SELECTED, 
			(wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)
			&awxChoice::OnSelectMenuItem);
}

void awxChoice::OnChar(wxKeyEvent& event)
{
    // don't let any keys to wxChoice for now.
    // it could select a "unselectable" item
}

void awxChoice::EnableItem(int n, const bool enable)
{
    wxASSERT(n < GetCount());
    m_states[n] = enable;
    if(GetSelection() == n && !enable)
        SetSelection(wxNOT_FOUND);
}
