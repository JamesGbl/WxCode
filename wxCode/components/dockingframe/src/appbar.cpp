#include "wx/appbar.h"
//#include "windows.h"

#include <windowsx.h>
/////////////////////////////APP BAR



CAppBar::CAppBar(wxFrame* mainFrame){
   
    m_pMainFrame = mainFrame;
    pHandler = (HWND) (m_pMainFrame->GetHandle());
    
    g_fAppRegistered = FALSE;      // TRUE if the appbar is registered
    // Current rect of the appbar
    g_dtSlideHide = 400;
    g_dtSlideShow = 200;
    
    m_pOptions =new OPTIONS;
   
    m_pOptions->fAutoHide = FALSE;
    m_pOptions->fOnTop = TRUE;
    m_pOptions->uSide = ABE_RIGHT;
    m_pOptions->cxWidth = (m_pMainFrame->GetSize()).GetWidth();
    m_pOptions->cyHeight = (m_pMainFrame->GetSize()).GetHeight();
        
    
}
CAppBar::~CAppBar()
{
    AppBar_UnRegister(); 
    delete m_pOptions;    
}

//
//  FUNCTION: SlideWindow(HWND, LPRECT) 
//
//  PURPOSE:    Slides the AppBar off the edge of the screen when the AppBar
//                has the AutoHide state set.
//
//  PARAMETERS:
//        hwnd    - handle of the window to scroll off the screen
//     prc     - rectangle you wish the appbar to occupy
//
//  COMMENTS:
//

void CAppBar::SlideWindow( LPRECT prc)
{
 
    RECT rcOld;
    RECT rcNew;
    int x, y, dx, dy, dt, t, t0;
    BOOL fShow;
    HANDLE hThreadMe;
    int priority;

    rcNew = *prc;

    if ((g_dtSlideShow > 0) && (g_dtSlideHide > 0)) 
    {
        GetWindowRect(pHandler, &rcOld);

        fShow = (rcNew.bottom - rcNew.top) > (rcOld.bottom - rcOld.top) ||
                (rcNew.right - rcNew.left) > (rcOld.right - rcOld.left);

        dx = (rcNew.right - rcOld.right) + (rcNew.left - rcOld.left);
        dy = (rcNew.bottom - rcOld.bottom) + (rcNew.top - rcOld.top);

       if (fShow)
        {
         rcOld = rcNew;
            OffsetRect(&rcOld, -dx, -dy);
         SetWindowPos(pHandler, NULL, rcOld.left, rcOld.top,
                   rcOld.right - rcOld.left, rcOld.bottom - rcOld.top,
                   SWP_NOZORDER | SWP_NOACTIVATE | SWP_DRAWFRAME);

         dt = g_dtSlideShow;
       }
     else
      {
         dt = g_dtSlideHide;
       }

       hThreadMe = GetCurrentThread();
       priority = GetThreadPriority(hThreadMe);
      SetThreadPriority(hThreadMe, THREAD_PRIORITY_HIGHEST);

      t0 = GetTickCount();
      while ((t = GetTickCount()) < t0 + dt)
        {
         x = rcOld.left + dx * (t - t0) / dt;
          y = rcOld.top + dy * (t - t0) / dt;

         SetWindowPos(pHandler, NULL, x, y, 0, 0,
                       SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
         if (fShow)
                UpdateWindow(pHandler);
           else
              UpdateWindow(GetDesktopWindow());
     }

       SetThreadPriority(hThreadMe, priority);
   }

    SetWindowPos(pHandler, NULL, rcNew.left, rcNew.top,
                rcNew.right - rcNew.left, rcNew.bottom - rcNew.top,
               SWP_NOZORDER | SWP_NOACTIVATE | SWP_DRAWFRAME);

}



//
//  FUNCTION:   AppBar_Size(HWND) 
//
//  PURPOSE:    Handles updating the appbar's size and position.
//
//  PARAMETERS:
//      hwnd    - handle of the appbar
//
//  COMMENTS:
//

void CAppBar::AppBar_Size()
{
    
    if (g_fAppRegistered)
    {
        //TODO: WHY GetWindowRect Return the old position ?????
        GetWindowRect(pHandler, &m_rc);
        AppBar_QuerySetPos(m_pOptions->uSide, &m_rc, &abd, TRUE);
    }
}


//
//  FUNCTION:   AppBar_QueryPos 
//
//  PURPOSE:    Asks the system if the AppBar can occupy the rectangle specified
//                in lprc.  The system will change the lprc rectangle to make
//             it a valid rectangle on the desktop.
//
//  PARAMETERS:
//      hwnd - Handle to the AppBar window.
//      lprc - Rectange that the AppBar is requesting to occupy.
//

void CAppBar::AppBar_QueryPos(LPRECT lprc)
{
   
    int iWidth = 0;
    int iHeight = 0;

    // Fill out the APPBARDATA struct and save the edge we're moving to
    // in the appbar OPTIONS struct.
    abd.rc = *lprc;
    abd.uEdge = m_pOptions->uSide;

    // Calculate the part we want to occupy.  We only figure out the top
    // and bottom coordinates if we're on the top or bottom of the screen.
    // Likewise for the left and right.  We will always try to occupy the
   // full height or width of the screen edge.
    if ((ABE_LEFT == abd.uEdge) || (ABE_RIGHT == abd.uEdge))
    {
        iWidth = abd.rc.right - abd.rc.left;
        abd.rc.top = 0;
        abd.rc.bottom = GetSystemMetrics(SM_CYSCREEN);
    }
    else
    {
        iHeight = abd.rc.bottom - abd.rc.top;
        abd.rc.left = 0;
        abd.rc.right = GetSystemMetrics(SM_CXSCREEN);
    }

    // Ask the system for the screen space
   SHAppBarMessage(ABM_QUERYPOS, &abd);

    // The system will return an approved position along the edge we're asking
    // for.  However, if we can't get the exact position requested, the system
    // only updates the edge that's incorrect.  For example, if we want to 
   // attach to the bottom of the screen and the taskbar is already there, 
    // we'll pass in a rect like 0, 964, 1280, 1024 and the system will return
    // 0, 964, 1280, 996.  Since the appbar has to be above the taskbar, the 
    // bottom of the rect was adjusted to 996.  We need to adjust the opposite
    // edge of the rectangle to preserve the height we want.

    switch (abd.uEdge)
    {
     case ABE_LEFT:
            abd.rc.right = abd.rc.left + iWidth;
          break;

      case ABE_RIGHT:
           abd.rc.left = abd.rc.right - iWidth;
          break;

      case ABE_TOP:
         abd.rc.bottom = abd.rc.top + iHeight;
         break;

      case ABE_BOTTOM:
          abd.rc.top = abd.rc.bottom - iHeight;
         break;
    }


    *lprc = abd.rc; 
}


//
//  FUNCTION:   AppBar_QuerySetPos(UINT, LPRECT, PAPPBARDATA, BOOL) 
//
//  PURPOSE:    Asks the system if the appbar can move itself to a particular
//              side of the screen and then does move the appbar.
//
//  PARAMETERS:
//      uEdge   - Side of the screen to move to.  Can be ABE_TOP, ABE_BOTTOM,
//                ABE_LEFT, or ABE_RIGHT.
//      lprc    - Screen rect the appbar wishes to occupy.  This will be 
//                modified and will return the area the system will let the
//                appbar occupy.
//      pabd    - Pointer to the APPBARDATA struct used in all appbar system
//                calls.
//        fMove   - TRUE if the function should move the appbar, FALSE if the
//               caller will move the AppBar.
//

void CAppBar::AppBar_QuerySetPos(UINT uEdge, LPRECT lprc, PAPPBARDATA pabd, BOOL fMove)
{
    int iHeight = 0;
    int iWidth = 0;

    // Fill out the APPBARDATA struct and save the edge we're moving to
    // in the appbar OPTIONS struct.
    pabd->rc = *lprc;
    pabd->uEdge = uEdge;
    m_pOptions->uSide = uEdge;

    AppBar_QueryPos(&(pabd->rc));

     SHAppBarMessage(ABM_SETPOS, pabd);
    // Tell the system we're moving to this new approved position.
    if (fMove)
    {       
        MoveWindow(pabd->hWnd, pabd->rc.left, pabd->rc.top, 
                 pabd->rc.right - pabd->rc.left,
                   pabd->rc.bottom - pabd->rc.top, TRUE);       
    }

    // Save the appbar rect.  We use this later when we autohide.  If we're
    // currently hidden, then don't mess with this.
    if (!m_pOptions->fAutoHide)
        g_rcAppBar = pabd->rc;
}


//
//  FUNCTION:   AppBar_PosChanged(PAPPBARDATA)
//
//  PURPOSE:    The system has changed our position for some reason.  We need
//                to recalculate the position on the screen we want to occupy
//             by determining how wide or thick we are and the update the
//              screen position.
//              
//
//  PARAMETERS:
//      pabd    - Pointer to the APPBARDATA structure used in all AppBar calls
//                to the system.
//

void CAppBar::AppBar_PosChanged(PAPPBARDATA pabd)
{
    RECT rc;
    RECT rcWindow;
    int iHeight;
    int iWidth;
   
    // Start by getting the size of the screen.
    rc.top = 0;
    rc.left = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);

  // Update the g_rcAppbar so when we slide (if hidden) we slide to the 
    // right place.
   if (m_pOptions->fAutoHide)
    {
        g_rcAppBar = rc;
        switch (m_pOptions->uSide)
        {
             case ABE_TOP:
                 g_rcAppBar.bottom = g_rcAppBar.top + g_cyHeight;
                  break;
    
              case ABE_BOTTOM:
                  g_rcAppBar.top = g_rcAppBar.bottom - g_cyHeight;
                  break;
    
              case ABE_LEFT:
                    g_rcAppBar.right = g_rcAppBar.left + g_cxWidth;
                   break;
    
              case ABE_RIGHT:
                   g_rcAppBar.left = g_rcAppBar.right - g_cxWidth;
                   break;
        }           
  }        

    // Now get the current window rectangle and find the height and width
    GetWindowRect(pabd->hWnd, &rcWindow);
    iHeight = rcWindow.bottom - rcWindow.top;
    iWidth = rcWindow.right - rcWindow.left;

    // Depending on which side we're on, try to preserve the thickness of
    // the window    
    switch (m_pOptions->uSide) 
    {
      case ABE_TOP:
         rc.bottom = rc.top + iHeight;
         break;

      case ABE_BOTTOM:
          rc.top = rc.bottom - iHeight;
         break;

      case ABE_LEFT:
            rc.right = rc.left + iWidth;
          break;

      case ABE_RIGHT:
           rc.left = rc.right - iWidth;
          break;
    }        

    // Move the appbar.
    AppBar_QuerySetPos(m_pOptions->uSide, &rc, pabd, TRUE);

}


//
//  FUNCTION:   AppBar_Callback(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:    Handles notification messages sent from the system to our
//              appbar.
//
//  PARAMETERS:
//      hwnd    - Handle of the appbar window receiving the notification.
//      uMsg    - The appbar notification message that we registered.
//      wParam  - Contains the specific notification code.
//      lParam  - Extra information dependant on the notification code.
//

void CAppBar::AppBar_Callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //APPBARDATA abd;
    static HWND hwndZOrder = NULL;
    
    abd.cbSize = sizeof(abd);
    abd.hWnd = pHandler;
    
    switch (wParam) 
    {
        // Notifies the appbar that the taskbar's autohide or always-on-top 
        // state has changed.  The appbar can use this to conform to the settings
        // of the system taskbar.
        case ABN_STATECHANGE:
         //DebugMsg("AppBarCallback: ABN_STATECHANGE\r\n");
          break;

        // Notifies the appbar when a full screen application is opening or 
        // closing.  When a full screen app is opening, the appbar must drop
        // to the bottom of the Z-Order.  When the app is closing, we should 
        // restore our Z-order position.
     case ABN_FULLSCREENAPP:
           //DebugMsg("AppBarCallback: ABN_FULLSCREENAPP\r\n");
            if (lParam) 
          {
                // A full screen app is opening.  Move us to the bottom of the 
                // Z-Order.  

               // First get the window that we're underneath so we can correctly
             // restore our position
               hwndZOrder = GetWindow(pHandler, GW_HWNDPREV);
                
              // Now move ourselves to the bottom of the Z-Order
                SetWindowPos(pHandler, HWND_BOTTOM, 0, 0, 0, 0, 
                           SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);            
           } 
            else 
         {
                // The app is closing.  Restore the Z-order             
                SetWindowPos(pHandler, m_pOptions->fOnTop ? HWND_TOPMOST : hwndZOrder,
                          0, 0, 0, 0, 
                          SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
               
              hwndZOrder = NULL;
            }
         break;
        
        // Notifies the appbar when an event has occured that may effect the 
        // appbar's size and position.  These events include changes in the 
        // taskbar's size, position, and visiblity as well as adding, removing,
        // or resizing another appbar on the same side of the screen.
     case ABN_POSCHANGED:
            //DebugMsg("AppBarCallback: ABN_POSCHANGED\r\n");

            // Update our position in response to the system change
          AppBar_PosChanged(&abd);
          break;
    }
}


//
//  FUNCTION:   AppBar_Register(HWND)
//
//  PURPOSE:    Registers the appbar with the system.
//
//  PARAMETERS:
//      hwnd    - handle of the appbar to register.
//
//  RETURN VALUE:
//      Returns TRUE if successful, FALSE otherwise.
//
//  COMMENTS:
//      Sets the system wide g_fAppRegistered variable.
//

BOOL CAppBar::AppBar_Register()
{
   
    abd.cbSize = sizeof(APPBARDATA);
    abd.hWnd = pHandler;
    abd.uCallbackMessage = APPBAR_CALLBACK;

    m_pOptions->cxWidth = (m_pMainFrame->GetSize()).GetWidth();
    m_pOptions->cyHeight = (m_pMainFrame->GetSize()).GetHeight();

    g_fAppRegistered = SHAppBarMessage(ABM_NEW, &abd);
    

    
    return g_fAppRegistered;
}


//
//  FUNCTION:   AppBar_UnRegister(HWND)
//
//  PARAMETERS:
//      hwnd    - handle of the appbar to register.
//
//  RETURN VALUE:
//      Returns TRUE if successful, FALSE otherwise.
//
//  COMMENTS:
//      Sets the system wide g_fAppRegistered variable.
//

BOOL CAppBar::AppBar_UnRegister()
{
  
    abd.cbSize = sizeof(APPBARDATA);
    abd.hWnd = pHandler;

    g_fAppRegistered = !SHAppBarMessage(ABM_REMOVE, &abd);
        
    return !g_fAppRegistered;
}        


//
//  FUNCTION:   AppBar_SetAutoHide(HWND, BOOL)
//
//  PURPOSE:    Causes the appbar window to either auto hide or stop auto
//              hiding.
//
//  PARAMETERS:
//      hwnd    - Handle of the appbar window to change the auto hide state.
//      fHide   - TRUE if we want the window to autohide, FALSE to stop.
//
//  RETURN VALUE:
//      Returns TRUE if successful, FALSE otherwise.
//

BOOL CAppBar::AppBar_SetAutoHide(BOOL fHide)
{
    if (fHide)
        return AppBar_AutoHide();
    else
      return AppBar_NoAutoHide();
}


//
//  FUNCTION:   AppBar_AutoHide(HWND)
//
//  PURPOSE:    Does the work of changing the appbar to autohide.  We check
//              to see if we can autohide, and if so unregister and tell
//              the system we are autohiding.
//
//  PARAMETERS:
//      hwnd    - Window handle of the appbar.
//
//  RETURN VALUE:
//      TRUE if successful, FALSE otherwise.
//

BOOL CAppBar::AppBar_AutoHide()
{
    HWND hwndAutoHide;
    BOOL fSuccess;  
    RECT rc;

    abd.cbSize = sizeof(APPBARDATA);
    abd.hWnd = pHandler;
    abd.uEdge = m_pOptions->uSide;
  
    // First figure out if someone already has this side for 
    // autohiding
    hwndAutoHide = (HWND) SHAppBarMessage(ABM_GETAUTOHIDEBAR, &abd);
    if (hwndAutoHide != NULL)
    {
         //DebugMsg(TEXT("ERROR: Another appbar is already hiding ")
          //        TEXT("on this edge.  Cannot set to autohide.\r\n"));
         return (FALSE);
    }

    // We can autohide on this edge.  Set the autohide style.
    abd.lParam = TRUE;          

    fSuccess = (BOOL) SHAppBarMessage(ABM_SETAUTOHIDEBAR, &abd);
    if (!fSuccess)
    {
         //DebugMsg(TEXT("ERROR: Error trying to set autohidebar.\r\n"));
        //ErrorHandler();
        return (FALSE);
      }
    else
    {
        // Since we're allowed to autohide, we need to adjust our screen 
        // rectangle to the autohidden position.  This will allow the system
        // to resize the desktop.
        m_pOptions->fAutoHide = TRUE;
        g_cxWidth = m_pOptions->cxWidth;
        g_cyHeight = m_pOptions->cyHeight;

        rc = g_rcAppBar;
        switch (m_pOptions->uSide)
        {
         case ABE_TOP:
             rc.bottom = rc.top + 2; 
              break;
            case ABE_BOTTOM:
              rc.top = rc.bottom - 2;
               break;
            case ABE_LEFT:
                rc.right = rc.left + 2;
               break;
            case ABE_RIGHT:
               rc.left = rc.right - 2;
               break;
        }

       AppBar_QuerySetPos(m_pOptions->uSide, &rc, &abd, TRUE);
    }

   return (TRUE);
}


//
//  FUNCTION:   AppBar_NoAutoHide(HWND)
//
//  PURPOSE:    Does the work of changing the appbar to no-autohide.  We 
//              check to make sure we are actually auto hiding, and if so
//              re-register the appbar to get our desktop scren space back.
//
//  PARAMETERS:
//      hwnd    - Window handle of the appbar.
//
//  RETURN VALUE:
//      TRUE if successful, FALSE otherwise.
//

BOOL CAppBar::AppBar_NoAutoHide()
{
    HWND hwndAutoHide;
    BOOL fSuccess;  

    abd.cbSize = sizeof(APPBARDATA);
    abd.hWnd = pHandler;
    abd.uEdge = m_pOptions->uSide;
  
    // First let's check to see if we're the appbar attached to the
    // side of the screen
    abd.uEdge = m_pOptions->uSide;
    hwndAutoHide = (HWND) SHAppBarMessage(ABM_GETAUTOHIDEBAR, &abd);
    if (hwndAutoHide != pHandler)
     {
       //  DebugMsg(TEXT("ERROR: We're not hidden currently\r\n"));
          return (FALSE);
       }
    
       // We can autohide or stop autohide on this edge.  Set the autohide style.
        abd.lParam = FALSE;         

    fSuccess = (BOOL) SHAppBarMessage(ABM_SETAUTOHIDEBAR, &abd);
    if (!fSuccess)
    {
    // DebugMsg(TEXT("ERROR: Error trying to set autohidebar.\r\n"));
      //  ErrorHandler();
        return (FALSE);
    }
    else
    {
        // Need to change the appbar to get the screen desktop space
        // back.  Also need to reattach the appbar to that edge of the
        // screen.
        m_pOptions->fAutoHide = FALSE;        
      
        m_pOptions->cxWidth = g_cxWidth;
        m_pOptions->cyHeight = g_cyHeight;

        AppBar_SetSide(m_pOptions->uSide);   
    }

   return (TRUE);
}


//
//  FUNCTION:   AppBar_SetSide(HWND, UINT)
//
//  PURPOSE:    Sets the side the AppBar is currently attached to.
//
//  PARAMETERS:
//      hwnd    - Window handle of the appbar.
//      uSide   - Side of the screen to attach to.  Can be ABE_TOP, ABE_BOTTOM,
//                ABE_LEFT, or ABE_RIGHT.
//
//  RETURN VALUE:
//      TRUE if successful, FALSE otherwise.
//

BOOL CAppBar::AppBar_SetSide(UINT uSide)
{
    
    if(!g_fAppRegistered)
        AppBar_Register();

    BOOL       fAutoHide = FALSE;
    // Calculate the size of the screen so we can occupy the full width or
    // height of the screen on the edge we request.
    m_rc.top = 0;
    m_rc.left = 0;
    m_rc.right = GetSystemMetrics(SM_CXSCREEN);   
    m_rc.bottom = GetSystemMetrics(SM_CYSCREEN);
    // Fill out the APPBARDATA struct with the basic information
    abd.cbSize = sizeof(APPBARDATA);
    abd.hWnd = pHandler;

    // If the appbar is autohidden, turn that off so we can move the bar
    if (m_pOptions->fAutoHide)
    {
        fAutoHide = m_pOptions->fAutoHide;

        // Turn off the redrawing of the desktop while we move things around.
        // If you put any breakpoints in this area you will lock up the desktop
        // Since turning off the desktop repaints turns it off for all the apps
        // in the system
        SetWindowRedraw(GetDesktopWindow(), FALSE);
        AppBar_SetAutoHide(FALSE);
    }

    // Adjust the rectangle to set our height or width depending on the
    // side we want.
    switch (uSide)
    {
     case ABE_TOP:
         m_rc.bottom = m_rc.top + m_pOptions->cyHeight;
          break;
        case ABE_BOTTOM:
          m_rc.top = m_rc.bottom - m_pOptions->cyHeight;
          break;
        case ABE_LEFT:
            m_rc.right = m_rc.left + m_pOptions->cxWidth;
           break;
        case ABE_RIGHT:
           m_rc.left = m_rc.right - m_pOptions->cxWidth;
           break;
    }
 
    // Move the appbar to the new screen space.
    AppBar_QuerySetPos(uSide, &m_rc, &abd, TRUE);

    // If the appbar was hidden, rehide it now
    if (fAutoHide)
    {
        AppBar_SetAutoHide(TRUE);

        SetWindowRedraw(GetDesktopWindow(), TRUE);
        RedrawWindow(GetDesktopWindow(), NULL, NULL, 
                  RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);         
    }

    return (TRUE);
}   
                              

//
//  FUNCTION:   AppBar_SetAlwaysOnTop(HWND, BOOL) 
//
//  PURPOSE:    Set's the Always-On-Top state for the appbar.
//
//  PARAMETERS:
//      hwnd    - Window handle of the AppBar to set the state for.
//      fOnTop  - TRUE to set the AppBar to Always-On-Top, FALSE otherwise
//

void CAppBar::AppBar_SetAlwaysOnTop(BOOL fOnTop)
{
    
    // Update the window position to HWND_TOPMOST if we're to be always
    // on top, or HWND_NOTOPMOST if we're not.
    SetWindowPos(pHandler, (fOnTop) ? HWND_TOPMOST : HWND_NOTOPMOST,
               0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

    // Store the setting in the appbar OPTIONS struct.
    m_pOptions->fOnTop = fOnTop;
}


//
//  FUNCTION:   AppBar_Hide(HWND)
//
//  PURPOSE:    Causes the appbar to hide itself.
//
//  PARAMETERS:
//      hwnd    - Handle of the AppBar to hide.
//

void CAppBar::AppBar_Hide()
{
    RECT rc;

    // Don't want to hide if AutoHide not set
    if (!m_pOptions->fAutoHide)
    {
        return;
    }

    // Calculate a hidden rectangle to use
    rc = g_rcAppBar;
    switch (m_pOptions->uSide)
    {
     case ABE_TOP:
         rc.bottom = rc.top + 2; 
          break;
        case ABE_BOTTOM:
          rc.top = rc.bottom - 2;
           break;
        case ABE_LEFT:
            rc.right = rc.left + 2;
           break;
        case ABE_RIGHT:
           rc.left = rc.right - 2;
           break;
    }

   m_pOptions->fHiding = TRUE;   
    //SlideWindow(hwnd, &rc);
}


//
//  FUNCTION:   AppBar_UnHide(HWND) 
//
//  PURPOSE:    Causes a hidden appbar to unhide itself.
//
//  PARAMETERS:
//      hwnd    - Handle of the appbar to unhide.
//

void CAppBar::AppBar_UnHide()
{
   
    SlideWindow(&g_rcAppBar);
    m_pOptions->fHiding = FALSE;

    AppBar_SetAutoHideTimer();
}


//
//  FUNCTION:   AppBar_SetAutoHideTimer(HWND) 
//
//  PURPOSE:    Starts the auto hide timer.
//
//  PARAMETERS:
//      hwnd    - Handle of the appbar to set the timer for.
//
//  COMMENTS:
//      This is called to cause a delay between when the user's mouse leaves
//      the appbar area and the appbar is slid off the screen.
//

void CAppBar::AppBar_SetAutoHideTimer()
{
    
    if (m_pOptions->fAutoHide)
      SetTimer(pHandler, IDT_AUTOHIDE, 500, NULL);
}


//
//  FUNCTION:   AppBar_SetAutoUnhideTimer(HWND)
//
//  PURPOSE:    Starts the auto-UNhide timer.
//
//  PARAMETERS: 
//      hwnd    - Handle of the appbar to set the timer for.    
//
//  COMMENTS:
//      This is called to cause a delay between the time where the mouse enters
//      the appbar window and the time where the appbar displays itself.
//

void CAppBar::AppBar_SetAutoUnhideTimer()
{
   
 
    if (m_pOptions->fAutoHide && m_pOptions->fHiding)
        SetTimer(pHandler, IDT_AUTOUNHIDE, 50, NULL);
}


//
//  FUNCTION:   GetAppbarData(HWND)
//
//  PURPOSE:    Retrieves a pointer to the OPTIONS structure stored in the
//              window's extra bytes.
//
//  PARAMETERS:
//      hwnd    - Handle of the window to retrieve the pointer from.
//
//  RETURN VALUE:
//      Returns a pointer to an OPTIONS struct
//


POPTIONS CAppBar::GetAppbarData()      
{
    return m_pOptions;
}

BOOL CAppBar::IsDocked()
{
     return g_fAppRegistered;
} 
 




