#include "wx/dockingframe.h"


BEGIN_EVENT_TABLE( wxDockingFrame, wxFrame )
     EVT_SIZE(wxDockingFrame::OnSize)
END_EVENT_TABLE()

wxDockingFrame::wxDockingFrame(wxWindow * parent, wxWindowID id, const wxString& title,
    int nOrientation,
    const wxPoint& pos, 
    const wxSize& size, 
    long style, 
    const wxString& name)
    :wxFrame(parent, id,title, pos, size,style,name)
{
    m_nOrientation =   nOrientation;
    m_pDockingPosition = DOCK_NONE; //not docked at startup
    m_pAppBar = new CAppBar(this);
    
}

wxDockingFrame::~wxDockingFrame(){
    wxLogMessage(wxT("delete AppBar"));
    
    if(m_pAppBar->IsDocked())
        Undock();
        
    delete m_pAppBar;
}

void wxDockingFrame::Dock(DockingPosition position)
{
    //save the docking Position
    m_pDockingPosition = position;
    
    switch (position)
    {
        case DOCK_LEFT:
            OnDock(ABE_LEFT);
            break;
        case DOCK_RIGHT:
            OnDock(ABE_RIGHT);
            break;
        case DOCK_BOTTOM:
            OnDock(ABE_BOTTOM);
            break;
        case DOCK_TOP:
            OnDock(ABE_TOP);
            break;   
    }
}

void wxDockingFrame::OnDock(int nOrientation) 
{
    wxLogMessage(wxT("OnDock()"));
    this->Hide();
    
    if(!m_pAppBar->IsDocked())
    {
        m_oldPos = this->GetPosition();
        m_oldSize = this->GetSize();  
        //if not yet docked, change the apparence of the title bar
        //store the position and size to restore it on undock
        this->SetWindowStyle( wxRESIZE_BORDER |
            wxFRAME_NO_TASKBAR | wxFRAME_TOOL_WINDOW |wxSTAY_ON_TOP  );    
    }
    //Get the DesktopSize (before adding the AppBar)
    wxSize displayS = wxGetClientDisplayRect().GetSize();
     
    m_pAppBar->AppBar_SetSide(nOrientation);
    
    if(m_nOrientation == wxVERTICAL)
        this->SetSizeHints(100,displayS.GetHeight(),displayS.GetWidth(), displayS.GetHeight());
    else
       this->SetSizeHints(displayS.GetWidth(),50,displayS.GetWidth(), displayS.GetHeight());
    this->Show();

}

void wxDockingFrame::Undock()
{
    wxLogMessage(wxT("Undock()"));
    
    m_pDockingPosition = DOCK_NONE;
    
    if(m_pAppBar->IsDocked())
    {
         this->Show(false);
         
        if (m_pAppBar->AppBar_UnRegister())
        {
            //FCTRACE(5,"AppBar UnRegister OK");   
        }
            
        this->SetWindowStyle(
                       wxMINIMIZE_BOX | wxMAXIMIZE_BOX 
                       | wxTHICK_FRAME | wxSYSTEM_MENU
                       |wxCAPTION |wxCLOSE_BOX /*| wxFRAME_NO_TASKBAR*/);
            
        //this->OnDock(false);   
                 
        wxSize displayS = wxGetClientDisplayRect().GetSize();
        this->SetSizeHints(100,200,displayS.GetWidth(), displayS.GetHeight());
         
        this->SetSize(m_oldSize);             
        this->Move(m_oldPos);  
        
        //wxCommandEvent callBackEvent(wxsEVT_CALLBACK_MENU, -1); //a call back event to update the menus
    
        //ProcessEvent(callBackEvent);
        
        this->Show();
        
    }     
}

wxDockingFrame::DockingPosition wxDockingFrame::GetDockingPosition()
{
    return m_pDockingPosition;
    
}
    
BOOL wxDockingFrame::IsDocked()
{
    return m_pAppBar->IsDocked();
}

void wxDockingFrame::OnSize( wxSizeEvent& event)
{
    if(m_pAppBar->IsDocked())
    {
    RECT rcWindow;
    POPTIONS pOpt = m_pAppBar->GetAppbarData();
        
    // Let the system know the appbar size has changed
    m_pAppBar->AppBar_Size();
    
    GetWindowRect((HWND)(GetHandle()), &rcWindow);
        
        
    if (pOpt->uSide == ABE_TOP || pOpt->uSide == ABE_BOTTOM)
              pOpt->cyHeight = rcWindow.bottom - rcWindow.top;
    else
              pOpt->cxWidth = rcWindow.right - rcWindow.left;
    }
              
    event.Skip();
}