
#include <wx/taskbar.h>

class wxCommanderTaskBar: public wxTaskBarIcon
{
public:
#if defined(__WXCOCOA__)
    wxCommanderTaskBar(wxFrame* window, wxTaskBarIconType iconType = DEFAULT_TYPE)   :   wxTaskBarIcon(iconType)
    {
       isVisible = true;
       dialog = window;
    }
#else
    wxCommanderTaskBar(wxFrame* window)
    {
       isVisible = true;
       dialog = window;
    }
#endif

    void OnLeftButtonDClick(wxTaskBarIconEvent&);
    void OnMenuRestore(wxCommandEvent&);
    void OnMenuHide(wxCommandEvent&);
    void OnMenuExit(wxCommandEvent&);
    virtual wxMenu *CreatePopupMenu();

  private:
    wxFrame* dialog;
    bool isVisible;

DECLARE_EVENT_TABLE()
};
