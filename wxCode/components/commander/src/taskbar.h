
#include <wx/taskbar.h>
#include "multiLang.h"

class wxOpenCommanderTaskBar: public wxTaskBarIcon
{
public:
#if defined(__WXCOCOA__)
    wxOpenCommanderTaskBar(wxFrame* window, wxTaskBarIconType iconType = DEFAULT_TYPE, multiLang& langMap) : lang(langMap) : wxTaskBarIcon(iconType)
    {
       isVisible = true;
       dialog = window;
    }
#else
    wxOpenCommanderTaskBar(wxFrame* window, multiLang& langMap) : lang(langMap)
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
    multiLang& lang;
    bool isVisible;

DECLARE_EVENT_TABLE()
};
