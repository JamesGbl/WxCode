#ifndef AWX_CHOICE_H
#define AWX_CHOICE_H

#include <wx/wx.h>

class awxChoice: public wxChoice
{
public:
    awxChoice(wxWindow *parent, wxWindowID id, const wxPoint& pos, 
		    const wxSize& size, int n, const wxString choices[]);
#if wxCHECK_VERSION(2, 6, 0)
    awxChoice(wxWindow *parent, wxWindowID id, const wxPoint& pos, 
		    const wxSize& size, const wxArrayString& choices);
#endif
    // additions
    void EnableItem(int i, const bool enable);

	// event handling
    void OnLeftDown(wxMouseEvent& event);
    void OnChar(wxKeyEvent& event);
    
    // overwritten
    void Delete(int n);
    void Clear();
    void SetSelection( int n );
    void OnSelectMenuItem(wxCommandEvent& event);
protected:	
    virtual int DoAppend(const wxString& item);
#if wxCHECK_VERSION(2, 6, 0)
    virtual int DoInsert(const wxString& item, int pos);
#endif
private:
    wxArrayInt m_states;
DECLARE_EVENT_TABLE();
};

#endif
