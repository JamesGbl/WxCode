//
// Name     : mmMonthCtrl
// Purpose  : A panel displaying the days of the months as numbers.
//            The user can click on a day to raise an event.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//

#include "mmMonthCtrl.h"

#ifdef __MMDEBUG__
extern wxTextCtrl* gDebug; // For global debug output
#endif

static wxColour mmPALE_RED     (240,200,200);
static wxColour mmPALE_GREY    (230,230,230);
static wxColour mmBRIGHT_ORANGE(255,180,120);
static wxColour mmBRIGHT_BLUE  (150,170,230);
//static wxColour mmBRIGHT_BLUE  (100,200,255);
//static wxColour mmBRIGHT_BLUE  (10,110,160);
//static wxColour mmBRIGHT_YELLOW(255,255,120);
static wxBrush  mmBRIGHT_BLUE_BRUSH(mmBRIGHT_BLUE,wxSOLID);

#define mmID_MONTH_NAVIGATOR 4451

BEGIN_EVENT_TABLE(mmMonthCtrl,wxWindow)
  EVT_PAINT       (mmMonthCtrl::OnPaint)
  EVT_LEAVE_WINDOW(mmMonthCtrl::OnEnterLeave)
  EVT_MOTION      (mmMonthCtrl::OnMotion)
  EVT_LEFT_DOWN   (mmMonthCtrl::OnLeftDown)
  EVT_NAVIGATOR_PREV2(mmID_MONTH_NAVIGATOR, mmMonthCtrl::OnNavigator)
  EVT_NAVIGATOR_PREV (mmID_MONTH_NAVIGATOR, mmMonthCtrl::OnNavigator)
  EVT_NAVIGATOR_NEXT (mmID_MONTH_NAVIGATOR, mmMonthCtrl::OnNavigator)
  EVT_NAVIGATOR_NEXT2(mmID_MONTH_NAVIGATOR, mmMonthCtrl::OnNavigator)
END_EVENT_TABLE()

/////////////////////////////////////////////////////////////////////////////

mmMonthEvent::mmMonthEvent(mmMonthCtrl* month, wxEventType type)
: wxCommandEvent(type, month->GetId())
{
#ifdef __MMDEBUG__
  *gDebug<<"id:"<<month->GetId()<<"\n";
#endif
  mDate = month->GetDate();
} // mmMonthEvent constructor

/////////////////////////////////////////////////////////////////////////////

mmMonthCtrl::mmMonthCtrl(wxWindow* parent,
		         const wxWindowID  id,
		         const wxDateTime& date,
                         const wxPoint&    pos,
		         const wxSize&     size,
		         const long int    style)
: wxWindow(parent, id, pos, size, style)
{
  mTodayPen = NULL;
  mSelectPen = NULL;
  mMonthBackgroundPen = NULL;
  mMonthBackgroundBrush = NULL;

  SetTodayColour(wxRED);
  SetSelectColour(&mmBRIGHT_ORANGE);
  SetMonthBackgroundColour(wxWHITE);

  mDaysOfWeek   = 7;
  mWeeksOfMonth = 6;

  // Month name / year
  mmMultiButton* datelabel = new mmMultiButton(this,-1,"",wxNullBitmap,
                                               wxDefaultPosition,wxDefaultSize,
					       wxNO_BORDER);
  bool prev = style & mmPREV_BTN;
  bool next = style & mmNEXT_BTN;
  mMonthNavigator = new mmNavigator(this, mmID_MONTH_NAVIGATOR, datelabel,
		                    prev,next,prev,next,
		                    wxDefaultPosition, wxDefaultSize,
				    wxNO_BORDER);
  wxLayoutConstraints* navcon = new wxLayoutConstraints;
  navcon->top.SameAs(this, wxTop);
  navcon->height.Absolute(20);
  navcon->left.SameAs(this,wxLeft);
  navcon->right.SameAs(this,wxRight);
  mMonthNavigator->SetConstraints(navcon);

  SetAutoLayout(TRUE);
  Layout();

  mLastX = 10000;
  mLastY = 10000;
  mStyle = style;
  SetDate(date);
} // mmMonthCtrl constructor

mmMonthCtrl::~mmMonthCtrl()
{
  if (mMonthNavigator)
  { mMonthNavigator->Destroy(); mMonthNavigator = NULL; }
} // Destructor
 
void mmMonthCtrl::SetTodayColour(wxColour* col)
{
  mTodayColour = col;
  mTodayPen = wxThePenList->FindOrCreatePen(*col,1,wxSOLID);
} // SetTodayColour

void mmMonthCtrl::SetSelectColour(wxColour* col)
{
  mSelectColour = col;
  mSelectPen = wxThePenList->FindOrCreatePen(*col,1,wxSOLID);
} // SetSelectColour

void mmMonthCtrl::SetMonthBackgroundColour(wxColour* col)
{
  mMonthBackgroundColour = col;
  mMonthBackgroundPen = wxThePenList->FindOrCreatePen(*col,1,wxSOLID);
  mMonthBackgroundBrush = wxTheBrushList->FindOrCreateBrush(*col,wxSOLID);
  //mMonthBackgroundBrush = new wxBrush(*col,wxSOLID);
} // SetMonthBackgroundColour

void mmMonthCtrl::SetDate(const wxDateTime& date)
// Update all
{
  mDate = date;
  wxClientDC dc(this);
  CalculateSize(dc);
  UpdateDays(dc);
} // Update

void mmMonthCtrl::OnEnterLeave(wxMouseEvent& event)
// Remove the select rectangle when leaving
{
  if (mCellW != 0 && mCellH != 0)
  {
    int  w,h;
    GetClientSize(&w,&h);
    if (mLastX >= 0           && mLastX < w &&
        mLastY >= mNavH+mHdrH && mLastY < h)
    {
      wxDateTime lastdate = CoordToDate(mLastX,mLastY);
      if ((int)lastdate.GetJDN() != (int)mDate.GetJDN())
      {
        wxClientDC dc(this);
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.SetPen(*mMonthBackgroundPen);
        dc.DrawRectangle(int(mLastX/mCellW)*mCellW+1,
		         int((mLastY-mNavH-mHdrH)/mCellH)*mCellH+mNavH+mHdrH+1,
		         mCellW-2,mCellH-2);
        dc.SetPen(*wxBLACK_PEN);
      }
    }
  }
} // OnEnterLeave

void mmMonthCtrl::OnMotion(wxMouseEvent& event)
// Draw/remove the 'select'-rectangle
{
  if (mCellW != 0 && mCellH != 0 && mStyle & mmSHOW_MOVE)
  {
    if (event.GetX() >= 0           && event.GetX() < mDaysOfWeek  *mCellW &&
        event.GetY() >= mNavH+mHdrH && event.GetY() < mWeeksOfMonth*mCellH+mNavH+mHdrH)
    {
      wxDateTime evtdate  = CoordToDate(event.GetX(),event.GetY());
      wxDateTime lastdate = CoordToDate(mLastX,mLastY);
      if (lastdate != evtdate)
      {
        wxClientDC dc(this);
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
	// Remove previous select rectangle
        int last_x = int(mLastX/mCellW)*mCellW;
        int last_y = int((mLastY-mNavH-mHdrH)/mCellH)*mCellH+mNavH+mHdrH;
        dc.SetPen(*mMonthBackgroundPen);
        dc.DrawRectangle(last_x+1,last_y+1,mCellW-2,mCellH-2);
        if (evtdate.GetMonth() == mDate.GetMonth())
        {
	  // Draw new rectangle
          int x = int(event.GetX()/mCellW)*mCellW;
          int y = int((event.GetY()-mNavH-mHdrH)/mCellH)*mCellH+mNavH+mHdrH;
          dc.SetPen(*wxBLACK_PEN);
          dc.DrawRectangle(x+1,y+1,mCellW-2,mCellH-2);
          UpdateDay(dc, lastdate, x,y);
        }
      }
      mLastX = event.GetX(); mLastY = event.GetY();
    }
  }
} // OnMotion

void mmMonthCtrl::OnLeftDown(wxMouseEvent& event)
{
  if (event.GetX() > 0           && event.GetX() < mDaysOfWeek  *mCellW &&
      event.GetY() > mHdrH+mNavH && event.GetY() < mWeeksOfMonth*mCellH+mHdrH+mNavH)
  {
    int x = int(event.GetX()/mCellW)*mCellW;
    int y = int((event.GetY()-mHdrH-mNavH)/mCellH)*mCellH+mHdrH+mNavH;
    wxDateTime tmpdate = CoordToDate(x,y);
    if (tmpdate.GetMonth() == mDate.GetMonth())
    {
      SetDate(tmpdate);
      // Tell parent that a day was clicked and month panel was updated
      GenerateEvent(mmEVT_MONTH_DAY_CHANGED);
    }
  }
} // OnLeftDown

void mmMonthCtrl::OnNavigator(mmNavigatorEvent& event)
{
  switch (event.GetEventType())
  {
    case mmEVT_NAVIGATOR_PREV2: mDate -= wxDateSpan(0,12); break;
    case mmEVT_NAVIGATOR_PREV:  mDate -= wxDateSpan(0,1);  break;
    case mmEVT_NAVIGATOR_NEXT:  mDate += wxDateSpan(0,1);  break;
    case mmEVT_NAVIGATOR_NEXT2: mDate += wxDateSpan(0,12); break;
  }
  SetDate(mDate);
  // Tell parent that a navigator button was clicked and month panel was updated
  GenerateEvent(event.GetEventType());
} // OnNavigator

void mmMonthCtrl::OnPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);
  CalculateSize(dc);

  // Clear entire area (sans navigator)
  int  w,h;
  GetClientSize(&w,&h);
  dc.SetPen  (*mMonthBackgroundPen);
  dc.SetBrush(*mMonthBackgroundBrush);
  dc.DrawRectangle(0, mNavH, w, h-mNavH);

  UpdateDays(dc);
} // OnPaint

void mmMonthCtrl::UpdateDays(wxDC& dc)
// Update all dates
{
  // Init DC
  wxFont font = GetFont();
  dc.SetFont(font);
  if (mStyle & mmRESCALE_FONTS)
  {
    font.SetPointSize(int(mCellH/2));
    dc.SetFont(font);
    mMonthNavigator->GetWindow()->SetFont(font);
  }

  CalculateSize(dc);
  dc.SetTextForeground(*wxBLACK);
  dc.SetPen(*wxBLACK_PEN);

  // Update navigator date label if date changed
  if (mMonthNavigator)
  {
    wxLayoutConstraints* navcon = new wxLayoutConstraints;
    navcon->top.SameAs(this, wxTop);
    navcon->height.Absolute(mNavH);
    navcon->left.SameAs(this,wxLeft);
    navcon->right.SameAs(this,wxRight);
    mMonthNavigator->SetConstraints(navcon);
    SetAutoLayout(TRUE);
    Layout();
    mmMultiButton* btn = (mmMultiButton*)mMonthNavigator->GetWindow();
    if (btn)
    {
      wxString month_label;
      month_label.Printf("%s %d ", (const char*)mDate.GetMonthName(mDate.GetMonth()), mDate.GetYear());
      int w,h;
      btn->GetSize(&w,&h);
      long dw,dh;
      dc.GetTextExtent(month_label,&dw,&dh);
#ifdef __MMDEBUG__
      //*gDebug<<"w:"<<w<<",dw:"<<dw<<"\n";
      //*gDebug<<"h:"<<h<<",dh:"<<dh<<"\n";
#endif
      if (dw > w)
        month_label.Printf("%s %d ", (const char*)(mDate.GetMonthName(mDate.GetMonth()).Left(3)), mDate.GetYear());
      //btn->SetSize(w,2*dh);
      //Layout();
      if (btn->GetLabel() != month_label)
        btn->SetLabel(month_label);
    }
  }

  // Day names
  dc.SetBrush     (mmBRIGHT_BLUE_BRUSH);
  dc.DrawRectangle(0,mNavH,mNavW,mHdrH);
  wxDateTime tmpdate = wxDateTime(1,mDate.GetMonth(),mDate.GetYear());
  tmpdate.SetToPrevWeekDay(wxDateTime::Mon);
  tmpdate.Set(tmpdate.GetDay(),tmpdate.GetMonth(),tmpdate.GetYear(),0,0,0,0);
  for (int daycount=0; daycount<mDaysOfWeek; daycount++)
  {
    wxString day_name = wxString(tmpdate.GetWeekDayName(tmpdate.GetWeekDay()));
    day_name = day_name.Left((unsigned int)2);
    long dw,dh;
    dc.GetTextExtent(day_name,&dw,&dh);
    dc.DrawText(day_name, daycount*mCellW+int(mCellW/2-dw/2),mNavH+1);
    tmpdate += wxDateSpan::Days(1);
  } // for

  // Draw dates
  tmpdate = wxDateTime(1,mDate.GetMonth(),mDate.GetYear());
  tmpdate.SetToPrevWeekDay(wxDateTime::Mon);
  tmpdate.Set(tmpdate.GetDay(),tmpdate.GetMonth(),tmpdate.GetYear(),0,0,0,0);
  for (int weekcount=0; weekcount<mWeeksOfMonth; weekcount++) // Weeks
  {
    for (int daycount=0; daycount<mDaysOfWeek; daycount++) // Days
    {
      UpdateDay(dc, tmpdate, daycount,weekcount);
      tmpdate += wxDateSpan::Days(1);
    }
  }
} // UpdateDays

void mmMonthCtrl::UpdateDay(wxDC& dc, const wxDateTime& date,
		            int daycount, int weekcount)
{
  if (date.GetMonth() == mDate.GetMonth())
  { // Date is in current month ('enabled')
    // Draw inner border, erase background
    if ((int)date.GetJDN() == (int)wxDateTime::Today().GetJDN() && (mStyle & mmSHOW_TODAY))
      dc.SetPen(*mTodayPen); // Today's date
    else
    if ((int)date.GetJDN() == (int)mDate.GetJDN() && (mStyle & mmSHOW_SELECT))
      dc.SetPen(*mSelectPen); // Selected (clicked)
    else
      dc.SetPen(*mMonthBackgroundPen); // Default (no inner border)
    dc.SetBrush(*mMonthBackgroundBrush); // Erase background
    dc.DrawRectangle(daycount*mCellW,weekcount*mCellH+mHdrH+mNavH,
                     mCellW,mCellH);
    // Draw outer border
    if (((int)date.GetJDN() == (int)mDate.GetJDN() && (mStyle & mmSHOW_SELECT)))
      dc.SetPen(*mSelectPen);
    else
      dc.SetPen(*mMonthBackgroundPen);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawRectangle(daycount*mCellW+1, weekcount*mCellH+mHdrH+mNavH+1,
                     mCellW-2,mCellH-2);
    // Set text colour
    if (date.IsWorkDay())
      dc.SetTextForeground(*wxBLACK);
    else
      dc.SetTextForeground(*wxRED);
  }
  else
  { // Date is not in current month ('disabled')
    // Erase background
    dc.SetPen  (*mMonthBackgroundPen);
    dc.SetBrush(*mMonthBackgroundBrush);
    dc.DrawRectangle(daycount*mCellW+1, weekcount*mCellH+mHdrH+mNavH+1,
	             mCellW-2,mCellH-2);
    if (!(mStyle & mmSHOW_BORDER_DAYS))
      return;
    // Set text colour
    if (date.IsWorkDay())
      dc.SetTextForeground(mmPALE_GREY);
    else
      dc.SetTextForeground(mmPALE_RED);
  }
  dc.SetBrush(*wxTRANSPARENT_BRUSH);
  // Draw text for this date
  wxString str;
  str.Printf(" %2d ", date.GetDay());
  long dw,dh;
  dc.GetTextExtent(str,&dw,&dh);
  dc.DrawText(str, daycount*mCellW+int(mCellW/2-dw/2),
	           weekcount*mCellH+mHdrH+mNavH+1);
} // UpdateDay

void mmMonthCtrl::CalculateSize(wxDC& dc)
{
  int  w,h;
  GetClientSize(&w,&h);
  /*
  if (mMonthNavigator)
    mMonthNavigator->GetSize(&mNavW,&mNavH);
  */
  long dw,dh,ddesc;
  dc.GetTextExtent("X_",&dw,&dh,&ddesc);
  mNavH = dh + ddesc;
  mNavW = w;
  mHdrH = mNavH;
  mCellH = int((h-mNavH-mHdrH)/mWeeksOfMonth)-1; // Cell height
  mCellW = int(w/mDaysOfWeek); // Cell width
#ifdef __MMDEBUG__
  //*gDebug<<"cli w,h:"<<w<<","<<h<<"\n";
  //*gDebug<<"nav w,h:"<<mNavW<<","<<mNavH<<"\n";
#endif
} // CalculateSize

wxDateTime mmMonthCtrl::CoordToDate(int x, int y)
{
  wxDateTime tmpdate = mDate;
  tmpdate -= wxDateSpan::Days(mDate.GetDay());
  tmpdate -= wxDateSpan::Days(tmpdate.GetWeekDay()-1);
  tmpdate += wxDateSpan::Days(((int((y-mHdrH-mNavH)/mCellH)%mWeeksOfMonth)*mDaysOfWeek+int(x/mCellW)%mDaysOfWeek));
  tmpdate.Set(tmpdate.GetDay(),tmpdate.GetMonth(),tmpdate.GetYear(),0,0,0,0);
  return tmpdate;
} // CoordToDate

void mmMonthCtrl::GenerateEvent(wxEventType type)
{
  mmMonthEvent ev(this, type);
  if (GetGrandParent())
    GetGrandParent()->ProcessEvent(ev); // NB! Nasty!
  //GetEventHandler()->ProcessEvent(ev);
} // GenerateEvent

