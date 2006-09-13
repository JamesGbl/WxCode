/////////////////////////////////////////////////////////////////////////////
// Name:        dockingframe.h
// Purpose:     a docking frame that could be dock on left, right, top, etc..
// Author:      Bart Rolet
// Created:     2006/08/01
// RCS-ID:      $Id: dockingframe.h,v 1.2 2006-09-13 17:45:04 bart59 Exp $
// Copyright:   (c) 2006 SESS Canada Inc.
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef DOCKINGFRAME_H_
#define DOCKINGFRAME_H_

#include "dockingframedef.h"
#include "appbar.h"

#include <wx/wx.h>



/**
 * @brief Dockable frame.
 * a frame that can be docked on a side of the desktop (and reduce the desktop size).
 */
class WXDLLIMPEXP_DOCKINGFRAME wxDockingFrame : public wxFrame
{
 
public:
    /**
     * @enum DockingPosition
     * @brief Docking positions.
     * @see Dock()
     */
     enum DockingPosition   
       {
          DOCK_LEFT, ///< Dock left
          DOCK_RIGHT, ///<Dock right     
          DOCK_TOP, ///<Dock Top
          DOCK_BOTTOM, ///<Dock Bottom
          DOCK_NONE, ///< undock
       }; 
public :
    
    /**
     * Default constructor
     * @param parent    the parent window
     * @param id        id of the window
     * @param title     title of the window, displayed in the top bar
     * @param nOrientation orientation of the application (wxVERTICAL or wxHORIZONTAL), it is
     *                      mainly used to determine how to "compress" the frame when docked
     */
    wxDockingFrame::wxDockingFrame(wxWindow * parent, wxWindowID id, const wxString& title = wxT("test"),
        int nOrientation = wxVERTICAL,
        const wxPoint& pos = wxDefaultPosition, 
        const wxSize& size = wxDefaultSize, 
        long style = wxDEFAULT_FRAME_STYLE, 
        const wxString& name = wxT("dockingframe"));
    
    /** Dock the Frame
     * @param position could be DOCK_LEFT, DOCK_RIGHT, DOCK_BOTTOM, DOCK_TOP
     */
    virtual void Dock(DockingPosition position);

    /**
     * Undock the frame
     * the frame will come back to its initial position
     */
    virtual void Undock(); 
 
    virtual ~wxDockingFrame();
    
    /**Get the Docking position
     * @return the docking position, DOCK_NONE if not docked.
     */
    virtual DockingPosition GetDockingPosition();
    
    /** is the frame docked ?
     * @return true if docked
     */
    virtual BOOL IsDocked();

    /** When the frame has been resized
     * this will update the docking area if docked
     * @todo    should prevent from resizing the docked side 
     * (ie: if docked right, only the left side should be available for resize)
    */
    void OnSize( wxSizeEvent& event);
private:
    CAppBar* m_pAppBar;
    int m_nOrientation; // orientation of the frame
    DockingPosition m_pDockingPosition; //the current docking position
    void OnDock(int nOrientation);
    wxPoint m_oldPos;
    wxSize  m_oldSize;
    
    DECLARE_EVENT_TABLE()
};

#endif /*DOCKINGFRAME_H_*/
