/*
  demowxplot By Germán Carrera

  code taken from some examples, from wxwindows samples, plplot webpage examples
  and old wxplot - plplot

  is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Library Public License as published
  by the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

*/
class MyChild;
// Define a new application
class MyApp : public wxApp
{
public:
    bool OnInit();

    wxMenu *file_menu;
};

class MyCanvas : public wxScrolledWindow
{
public:
    MyCanvas(wxWindow *parent, const wxPoint& pos, const wxSize& size, const int type );
    virtual void OnDraw(wxDC& dc);

    bool IsDirty() const { return m_dirty; }

    void OnEvent(wxMouseEvent& event);

    int type;
    void Plot1(wxDC& dc,int w_,int h_);
    void Plot2(wxDC& dc, int k, int w_,int h_);
    void Plot3(wxDC& dc, int w_,int h_);
    void plfbox(PLFLT x0, PLFLT y0, wxPlot *wxplot);
  
    void cmap1_init(int gray);
  
    void ReDraw(wxSizeEvent& event);
  
private:
    bool m_dirty;

    DECLARE_EVENT_TABLE()
};

// Define a new frame
class MyFrame : public wxMDIParentFrame
{
public:

    MyFrame(wxWindow *parent, const wxWindowID id, const wxString& title,
            const wxPoint& pos, const wxSize& size, const long style, wxMenu *file_menu_);

    void OnSize(wxSizeEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnNewWindow1(wxCommandEvent& event);
    void OnNewWindow2(wxCommandEvent& event);
    void OnNewWindow3(wxCommandEvent& event);
    void OnNewWindow4(wxCommandEvent& event);
    void OnNewWindow5(wxCommandEvent& event);
    void OnNewWindow6(wxCommandEvent& event);

    void OnPrint(wxCommandEvent& WXUNUSED(event));
    void OnPrintSetup(wxCommandEvent& WXUNUSED(event));
    void OnPrintPreview(wxCommandEvent& WXUNUSED(event));
    void OnPageSetup(wxCommandEvent& event);

    void OnQuit(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

    void OnMenuOpen(wxMenuEvent& event);
  
    wxMenu *file_menu;
    MyChild *mchild[6];
  
    DECLARE_EVENT_TABLE()
};

class MyChild: public wxMDIChildFrame
{
public:
    MyCanvas *canvas;

    MyChild(wxMDIParentFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size, const long style, int type);
    ~MyChild();


    void OnActivate(wxActivateEvent& event);

    void OnRefresh(wxCommandEvent& event);
    void OnUpdateRefresh(wxUpdateUIEvent& event);

    void OnChangePosition(wxCommandEvent& event);
    void OnChangeSize(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnMove(wxMoveEvent& event);
    void OnClose(wxCloseEvent& event);

    int type;

    DECLARE_EVENT_TABLE()
};

class MyPrintout: public wxPrintout
{
 public:
   
  MyPrintout(char *title, MyCanvas *canvas_);
  bool OnPrintPage(int page);
  bool HasPage(int page);
  bool OnBeginDocument(int startPage, int endPage);
  void GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo);
  
  wxDC *dc;
  MyCanvas *canvas;

};


// menu items ids
enum
{
    MDI_QUIT = 100,
    MDI_NEW_WINDOW1,
    MDI_NEW_WINDOW2,
    MDI_NEW_WINDOW3,
    MDI_NEW_WINDOW4,
    MDI_NEW_WINDOW5,
    MDI_NEW_WINDOW6,
    MDI_REFRESH,
    MDI_CHANGE_TITLE,
    MDI_CHANGE_POSITION,
    MDI_CHANGE_SIZE,
    MDI_CHILD_QUIT,
    ID_PRINT,
    ID_PRINT_SETUP, 
    ID_PAGE_SETUP, 
    ID_PREVIEW,
    MDI_ABOUT
  
};
