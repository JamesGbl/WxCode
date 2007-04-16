/////////////////////////////////////////////////////////////////////////////
// Name:        awx/button.h
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: button.h,v 1.3 2004/08/30 10:20:19 jb Exp $
// Copyright:   (c) 2003,2004 Joachim Buermann
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_MULTI_BUTTON__
#define __WX_MULTI_BUTTON__

#include <wx/dcmemory.h>
#include <wx/wx.h>

class wxBitmap;

/*!
  \class awxButton
  a graphical button class like wxBitmap but with 4 possible states.
  The states are:
  \li \c up, means button isn't pressed (normal state)
  \li \c over, (mouse over the button, but not pressed)
  \li \c down, button  is pressed (mouse left click)
  \li \c disabled, button is inactive (disabled) and cannot pressed

*/
class awxButton:public wxWindow {

    DECLARE_DYNAMIC_CLASS (awxButton)

	   public:
    /*!
	 The awxButton constructor
	 \param parent A pointer to it's parent class, mostly awxToolbar.
	 \param id button identifier. If -1, will automatically create an identifier.
	 \param pos button position. wxDefaultPosition is (-1, -1) which indicates that 
	 wxWidgets should generate a default position for the button. If
	 using the awxButton class directly, supply an actual position.
	 \param size the button size. wxDefaultSize is (-1, -1) which
	 indicates that awxButton use a size of 32x32.
	 \param upXPM pointer to the xpm data including the up image
	 \param overXPM pointer to the xpm data including the over image. If NULL,
	 a standard high border is used.
	 \param downXPM pointer to the xpm data including the down image. If NULL,
	 a standard sunken border is used.
	 \param disXPM pointer to the xpm data including the disable image. If NULL,
	 the upXPM image is used.
    */
    awxButton(wxWindow * parent, wxWindowID id,
		    const wxPoint & pos,
		    const wxSize & size,
		    char **upXPM, char **overXPM, char **downXPM, char **disXPM);
    /*!
	 The default ctor makes nothing.
    */
    awxButton() {};

    /*!
	 The awxButton destructor. Because awxButton uses some allocated memory
	 from heap, the destructor must free it. It must also be virtual, so
	 derivated classes (and their ctors) can call it.
    */
    virtual ~ awxButton();

    /*!
	 Disables the button so it cannot be pressed any longer. The
	 button use the internal disable image to drawing this state.	 
    */
    virtual void Disable();

    /*!
	 Enables the button so it can be pressed by the user. The
	 button use the internal up image to drawing this state.	 
    */
    virtual void Enable();
    
    /*!
	 For wxWidget compatiblity. Enable or disable the button.
	 \param enable true enables the button, false disables it.
    */
    virtual bool Enable(bool enable) {
	   if(enable) Enable();
	   else Disable();
	   return true;
    };

    /*!
	 Returns the internal 'press' state of the button.
	 \return true, if teh button is pressed, false otherwise
    */
    virtual bool IsPressed() {return m_state == State_ButtonDown;};

    /*!
	 Returns the active state of the button.
	 \return true, if teh button is enabled, false otherwise
    */
    bool IsEnabled() {return m_enabled;};
    /*!
	 Paint the content of the button. In detail:
	 At the first time (m_painted is false), DrawOnBitmap() was
	 called (to initiate the internal bitmap), and the m_painted flag
	 was set to true.
	 Apart from that only a simple draw of the internal bitmap is
	 done.
    */
    virtual void OnPaint(wxPaintEvent& WXUNUSED(event));
    /*!
	 On MS only call Redraw() and skip the event.
	 On linux gtk the background image isn't repaint by the toolbar
	 at this time, so defer the redraw for later.
    */
    virtual void OnEraseBackground(wxEraseEvent &event);
    /*!
	 All mouse events will be handled by this (if the button is
	 enabled).
	 If the left mouse button changed to up, a wxEVT_COMMAND_MENU_SELECTED
	 event with the button id was fired (in detail, the event was
	 forwarded to the parent of the button.
    */
    virtual void OnMouseEvent(wxMouseEvent& WXUNUSED(event));
    /*!
	 Only call Redraw() and skip the event.
    */
    virtual void OnSizeEvent(wxSizeEvent& event);
    /*!
	 If the button isn't pressed, this call set the button in the
	 pressed state (also drawing the associated image and/or border.
    */
    virtual bool Press();
    /*!
	 If the button is pressed, this call set the button in the
	 unpressed (release) state (also drawing the associated image
	 and/or border.
    */
    virtual bool Release();
    /*!
	 Set the label for the button. This can be done also dynamically
	 within a running application.
	 \param text the new label for the button
    */
    virtual void SetText(const wxChar* text);
protected:
    /*!
	 The internal button states
    */
    enum ButtonState {
	   /// button is active but not pressed (up)
	   State_ButtonUp,
	   /// mouse pointer is over the button (not pressed)
	   State_ButtonOver,
	   /// left click on the button
	   State_ButtonDown,
	   /// button is disabled
	   State_ButtonDis,
	   /// internal beginning state different from the other ones
	   State_ButtonNew
    };

    /*!
	 The kind of border used by the button
    */
    enum BorderType {
	   /// sunken border, seem like a pressed button
	   Border_Sunken,
	   /// flat (no) border
	   Border_Flat,
	   /// high border, seems like a active (not pressed) button
	   Border_High
    };

    /// width of the button
    int m_width;
    /// height of the button
    int m_height;
    /*! 
	 if the buttons width is greater as the image, m_dx covered the
	 offset for the image from the origin (left top corner) to draw
	 the image in the center (horizontally).
    */
    int m_dx;
    /*! 
	 if the buttons height is greater as the image, m_dy covered the
	 offset for the image from the origin (left top corner) to draw
	 the image in the center (vertically).
    */
    int m_dy;

    /// the internal enabled state
    bool m_enabled;

    /// the actual button state (up, over, down and disabled)
    ButtonState m_state;
    /// to check against the last state, this state variable is needed
    ButtonState m_laststate;
    /// the internal bitmap for wxMemoryDC (buffered drawing) 
    wxBitmap* m_bitmap;
    /// the button text
    wxString m_text;
    /// the font of thebutton text
    wxFont *m_font;
    /// the four images (for each state)
    wxIcon *m_icons[4];
    /*!
	 Internal flag. Is set to true, if the first OnPaint() was occured.
	 As long as the flag is false, a Redraw() call does nothing.
    */
    bool m_painted;
    /// true, if the theme handling was enabled.
    bool m_theme;
    /*!
	 Internal function to draw the border on the given memory dc. 
	 \param dc reference to the internal wxMemoryDC context.
	 \param border kind of border, sunken, flat or high
    */
    void DrawBorder(wxDC& dc, BorderType border);
    /*!
	 Draw the content of the button (dependend of it's state, icons)
	 into the memory bitmap (m_bitmap).
	 If theme handling is enabled and a connected awxToolbar exists,
	 the button queries the background image from the awxToolbar and
	 copy it into it's memory bitmap.
	 Without theme handling, the background will be filled with the
	 system wide background colour for toolbars.
	 Following the icon and borders will be drawed, finished by
	 drawing the button label (if exists).
	 The DrawOnBitmap member function is only be called, if the
	 internal state has been changed. Otherwise only the content of
	 the bitmap will be paste into the wxPaintDC context (for example,
	 if the application was minimized, maximized or covered by another
	 program.
    */
    virtual void DrawOnBitmap();
    /*!
	 The Redraw function is used outside of a paint event (like
	 changing the internal state). It calls the DrawOnBitmap member
	 and copy the bitmap content in a wxClientDC context.
    */
    void Redraw();
public:
    /*!
	 Set the theme handling on (enabled) or of (disabled)
	 param enable true activates the theme drawing, false disabled it
    */
    void EnableTheme(bool enable) {
	   m_theme = enable;
    };
    DECLARE_EVENT_TABLE()
	   };

/*!
  \class awxCheckButton
  This Button has two States. Pressed and not pressed. In contrary to
  awxButton awxCheckButton keeps its pressed or not pressed condition
  until it changes back to its original condition by clicking the
  button again.
*/
class awxCheckButton : public awxButton
{
protected:
    bool m_snapin;
public:
    awxCheckButton(wxWindow * parent, wxWindowID id,
			    const wxPoint & pos,
			    const wxSize & size,
			    char **upXPM,char **overXPM,char **downXPM,char **disXPM) :
	   awxButton(parent,id,pos,size,
			   upXPM,overXPM,downXPM,disXPM) {
	   m_snapin = false;
    };
    virtual void OnMouseEvent(wxMouseEvent& event);
    virtual bool Press();
    virtual bool Release();
    DECLARE_EVENT_TABLE()
	   };

class awxRadioButtonBox;

/*!
  \class awxRadioButton
  Multiple awxRadioButton are used to select a certain state with
  mutual exclusion. The behaviour is like that of wxRadioButton.
*/
class awxRadioButton : public awxCheckButton
{
protected:
    awxRadioButtonBox* m_box;
public:
    awxRadioButton(wxWindow * parent, wxWindowID id,
			    const wxPoint & pos,
			    const wxSize & size,
			    char **upXPM,char **overXPM,char **downXPM,char **disXPM,
			    awxRadioButtonBox* selectBox = NULL);
    virtual void OnMouseEvent(wxMouseEvent& event);
    DECLARE_EVENT_TABLE()
	   };

/*!
  \class awxSeparator
*/
class awxSeparator : public awxButton
{
private:
    void DrawOnBitmap();
public:
    awxSeparator(wxWindow * parent, wxWindowID id = -1,
			  const wxPoint & pos = wxDefaultPosition,
			  const wxSize & size = wxSize(8,32)) :
	   awxButton(parent,id,pos,size,
			   NULL,NULL,NULL,NULL) {};
    void OnEraseBackground(wxEraseEvent& WXUNUSED(event)) {
	   DrawOnBitmap();
    };
public:
    DECLARE_EVENT_TABLE()
	   };

/*!
  \class awxRadioButtonBox
*/
class awxRadioButtonBox : public wxWindow
{
protected:
    WX_DEFINE_ARRAY(awxRadioButton*,awxButtonArray);
    awxButtonArray m_array;
public:
    awxRadioButtonBox(wxWindow* parent,wxWindowID id) :
	   wxWindow(parent,id)
	   {Show(false);};
    void Add(awxRadioButton* button) {
	   m_array.Add(button);
    };
    void ReleaseAll() {
	   for(unsigned int i=0;i<m_array.GetCount();i++) {
		  m_array[i]->Release();
	   }
    };
    void SetValue(wxWindowID id) {
	   awxRadioButton* button = NULL;
	   for(unsigned int i=0;i<m_array.GetCount();i++) {
		  if(m_array[i]->GetId() == id) {
			 button = m_array[i];
			 break;
		  }
	   }
	   if(button) {
		  ReleaseAll();
		  button->Press();
	   }
    };
};

#endif
