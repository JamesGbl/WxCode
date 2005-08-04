/////////////////////////////////////////////////////////////////////////////
// Name:        treelisttest.cpp
// Purpose:     wxTreeListCtrl sample
// Author:      Julian Smart
// Modified by: Otto Wyss
// Created:     04/01/98
// RCS-ID:      $Id: treelisttest.cpp,v 1.14 2005-08-04 17:12:35 wyo Exp $
// Copyright:   (c) wxCode
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#ifdef __GNUG__
    #pragma implementation "treelisttest.cpp"
    #pragma interface "treelisttest.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#ifndef WX_PRECOMP
  #include "wx/wx.h"
  #include "wx/log.h"
#endif

#include "wx/colordlg.h"

#include <wx/image.h>
#include <wx/imaglist.h>
#if wxCHECK_VERSION(2, 5, 0)
#include <wx/numdlg.h>
#endif
#include "wx/treelistctrl.h"

#include "math.h"

#ifdef __WIN32__
    // this is not supported by native control
    #define NO_VARIABLE_HEIGHT
#endif

#include "treelisttest.h"

#include "icon1.xpm"
#include "icon2.xpm"
#include "icon3.xpm"
#include "icon4.xpm"
#include "icon5.xpm"

#if defined(__WXGTK__) || defined(__WXX11__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__)
#include "mondrian.xpm"
#endif


// verify that the item is ok and insult the user if it is not
#define CHECK_ITEM( item ) if ( !item.IsOk() ) {                                 \
                             wxMessageBox(wxT("Please select some item first!"), \
                                          wxT("Tree sample error"),              \
                                          wxOK | wxICON_EXCLAMATION,             \
                                          this);                                 \
                             return;                                             \
                           }

#define MENU_LINK(name) EVT_MENU(TreeListTest_##name, MyFrame::On##name)

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_SIZE(MyFrame::OnSize)

    MENU_LINK(Quit)
    MENU_LINK(About)

    MENU_LINK(TogButtons)
    MENU_LINK(TogTwist)
    MENU_LINK(TogLines)
    MENU_LINK(TogEdit)
    MENU_LINK(TogHideRoot)
    MENU_LINK(TogRootLines)
    MENU_LINK(TogBorder)
    MENU_LINK(TogFullHighlight)
    MENU_LINK(TogVirtual)
    MENU_LINK(SetFgColour)
    MENU_LINK(SetBgColour)
    MENU_LINK(ResetStyle)

    MENU_LINK(Dump)
#ifndef NO_MULTIPLE_SELECTION
    MENU_LINK(DumpSelected)
    MENU_LINK(Select)
    MENU_LINK(Unselect)
    MENU_LINK(ToggleSel)
#endif // NO_MULTIPLE_SELECTION
    MENU_LINK(Rename)
    MENU_LINK(Count)
    MENU_LINK(CountRec)
    MENU_LINK(Sort)
    MENU_LINK(SortRev)
    MENU_LINK(SetBold)
    MENU_LINK(ClearBold)
    MENU_LINK(Delete)
    MENU_LINK(DeleteChildren)
    MENU_LINK(DeleteAll)
    MENU_LINK(Recreate)
    MENU_LINK(ToggleImages)
    MENU_LINK(ToggleButtons)
    MENU_LINK(SetImageSize)
    MENU_LINK(CollapseAndReset)
    MENU_LINK(EnsureVisible)
    MENU_LINK(AddItem)
    MENU_LINK(InsertItem)
    MENU_LINK(IncIndent)
    MENU_LINK(DecIndent)
    MENU_LINK(ToggleIcon)
    MENU_LINK(SelectRoot)
#undef MENU_LINK

END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyTreeListCtrl, wxTreeListCtrl)
    EVT_TREE_BEGIN_DRAG(TreeListTest_Ctrl, MyTreeListCtrl::OnBeginDrag)
    EVT_TREE_BEGIN_RDRAG(TreeListTest_Ctrl, MyTreeListCtrl::OnBeginRDrag)
    EVT_TREE_END_DRAG(TreeListTest_Ctrl, MyTreeListCtrl::OnEndDrag)
    EVT_TREE_BEGIN_LABEL_EDIT(TreeListTest_Ctrl, MyTreeListCtrl::OnBeginLabelEdit)
    EVT_TREE_END_LABEL_EDIT(TreeListTest_Ctrl, MyTreeListCtrl::OnEndLabelEdit)
    EVT_TREE_DELETE_ITEM(TreeListTest_Ctrl, MyTreeListCtrl::OnDeleteItem)
#if 0       // there are so many of those that logging them causes flicker
    EVT_TREE_GET_INFO(TreeListTest_Ctrl, MyTreeListCtrl::OnGetInfo)
#endif
    EVT_TREE_SET_INFO(TreeListTest_Ctrl, MyTreeListCtrl::OnSetInfo)
    EVT_TREE_ITEM_EXPANDED(TreeListTest_Ctrl, MyTreeListCtrl::OnItemExpanded)
    EVT_TREE_ITEM_EXPANDING(TreeListTest_Ctrl, MyTreeListCtrl::OnItemExpanding)
    EVT_TREE_ITEM_COLLAPSED(TreeListTest_Ctrl, MyTreeListCtrl::OnItemCollapsed)
    EVT_TREE_ITEM_COLLAPSING(TreeListTest_Ctrl, MyTreeListCtrl::OnItemCollapsing)
    //EVT_TREE_ITEM_RIGHT_CLICK(TreeListTest_Ctrl, MyTreeListCtrl::OnItemRightClick)

    EVT_CONTEXT_MENU(MyTreeListCtrl::OnContextMenu)
    EVT_TREE_SEL_CHANGED(TreeListTest_Ctrl, MyTreeListCtrl::OnSelChanged)
    EVT_TREE_SEL_CHANGING(TreeListTest_Ctrl, MyTreeListCtrl::OnSelChanging)
    EVT_TREE_KEY_DOWN(TreeListTest_Ctrl, MyTreeListCtrl::OnTreeKeyDown)
    EVT_TREE_ITEM_ACTIVATED(TreeListTest_Ctrl, MyTreeListCtrl::OnItemActivated)
    EVT_RIGHT_DCLICK(MyTreeListCtrl::OnRMouseDClick)
END_EVENT_TABLE()

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    // Create the main frame window
    MyFrame *frame = new MyFrame(wxT("wxTreeListCtrl Test"), 50, 50, 450, 600);

    // Show the frame
    frame->Show(true);
    SetTopWindow(frame);

    return true;
}


// My frame constructor
MyFrame::MyFrame(const wxString& title, int x, int y, int w, int h)
       : wxFrame((wxFrame *)NULL, wxID_ANY, title, wxPoint(x, y), wxSize(w, h)),
         m_treeListCtrl(NULL)
#if wxUSE_LOG
         , m_textCtrl(NULL)
#endif // wxUSE_LOG
{
    // This reduces flicker effects - even better would be to define
    // OnEraseBackground to do nothing. When the tree control's scrollbars are
    // show or hidden, the frame is sent a background erase event.
    SetBackgroundColour(wxColour(255, 255, 255));

    // Give it an icon
    SetIcon(wxICON(mondrian));

#if wxUSE_MENUS
    // Make a menubar
    wxMenu *file_menu = new wxMenu,
           *style_menu = new wxMenu,
           *tree_menu = new wxMenu,
           *item_menu = new wxMenu;

    file_menu->Append(TreeListTest_About, wxT("&About..."));
    file_menu->AppendSeparator();
    file_menu->Append(TreeListTest_Quit, wxT("E&xit\tAlt-X"));

    style_menu->AppendCheckItem(TreeListTest_TogButtons, wxT("Toggle &normal buttons"));
    style_menu->AppendCheckItem(TreeListTest_TogTwist, wxT("Toggle &twister buttons"));
    style_menu->AppendCheckItem(TreeListTest_ToggleButtons, wxT("Toggle image &buttons"));
    style_menu->AppendSeparator();
    style_menu->AppendCheckItem(TreeListTest_TogLines, wxT("Toggle &connecting lines"));
    style_menu->AppendCheckItem(TreeListTest_TogRootLines, wxT("Toggle &lines at root"));
    style_menu->AppendCheckItem(TreeListTest_TogHideRoot, wxT("Toggle &hidden root"));
    style_menu->AppendCheckItem(TreeListTest_TogBorder, wxT("Toggle &item border"));
    style_menu->AppendCheckItem(TreeListTest_TogFullHighlight, wxT("Toggle &full row highlight"));
    style_menu->AppendCheckItem(TreeListTest_TogVirtual, wxT("Toggle &virtual mode"));
    style_menu->AppendCheckItem(TreeListTest_TogEdit, wxT("Toggle &edit mode"));
#ifndef NO_MULTIPLE_SELECTION
    style_menu->AppendCheckItem(TreeListTest_ToggleSel, wxT("Toggle &selection mode"));
#endif // NO_MULTIPLE_SELECTION
    style_menu->AppendCheckItem(TreeListTest_ToggleImages, wxT("Toggle show ima&ges"));
    style_menu->Append(TreeListTest_SetImageSize, wxT("Set image si&ze..."));
    style_menu->AppendSeparator();
    style_menu->Append(TreeListTest_SetFgColour, wxT("Set &foreground colour..."));
    style_menu->Append(TreeListTest_SetBgColour, wxT("Set &background colour..."));
    style_menu->AppendSeparator();
    style_menu->Append(TreeListTest_ResetStyle, wxT("&Reset to default\tF10"));

    tree_menu->Append(TreeListTest_Recreate, wxT("&Recreate the tree"));
    tree_menu->Append(TreeListTest_CollapseAndReset, wxT("C&ollapse and reset"));
    tree_menu->AppendSeparator();
    tree_menu->Append(TreeListTest_AddItem, wxT("Append a &new item"));
    tree_menu->Append(TreeListTest_InsertItem, wxT("&Insert a new item"));
    tree_menu->Append(TreeListTest_Delete, wxT("&Delete this item"));
    tree_menu->Append(TreeListTest_DeleteChildren, wxT("Delete &children"));
    tree_menu->Append(TreeListTest_DeleteAll, wxT("Delete &all items"));
    tree_menu->Append(TreeListTest_SelectRoot, wxT("Select root item"));
    tree_menu->AppendSeparator();
    tree_menu->Append(TreeListTest_Count, wxT("Count children of current item"));
    tree_menu->Append(TreeListTest_CountRec, wxT("Recursively count children of current item"));
    tree_menu->AppendSeparator();
    tree_menu->Append(TreeListTest_Sort, wxT("Sort children of current item"));
    tree_menu->Append(TreeListTest_SortRev, wxT("Sort in reversed order"));
    tree_menu->AppendSeparator();
    tree_menu->Append(TreeListTest_EnsureVisible, wxT("Make the last item &visible"));
    tree_menu->AppendSeparator();
    tree_menu->Append(TreeListTest_IncIndent, wxT("Add 5 points to indentation\tAlt-I"));
    tree_menu->Append(TreeListTest_DecIndent, wxT("Reduce indentation by 5 points\tAlt-R"));

    item_menu->Append(TreeListTest_Dump, wxT("&Dump item children"));
    item_menu->Append(TreeListTest_Rename, wxT("&Rename item..."));

    item_menu->AppendSeparator();
    item_menu->Append(TreeListTest_SetBold, wxT("Make item &bold"));
    item_menu->Append(TreeListTest_ClearBold, wxT("Make item &not bold"));
    item_menu->AppendSeparator();
    item_menu->Append(TreeListTest_ToggleIcon, wxT("Toggle the item's &icon"));

#ifndef NO_MULTIPLE_SELECTION
    item_menu->AppendSeparator();
    item_menu->Append(TreeListTest_DumpSelected, wxT("Dump selected items\tAlt-D"));
    item_menu->Append(TreeListTest_Select, wxT("Select current item\tAlt-S"));
    item_menu->Append(TreeListTest_Unselect, wxT("Unselect everything\tAlt-U"));
#endif // NO_MULTIPLE_SELECTION

    wxMenuBar *menu_bar = new wxMenuBar;
    menu_bar->Append(file_menu, wxT("&File"));
    menu_bar->Append(style_menu, wxT("&Style"));
    menu_bar->Append(tree_menu, wxT("&Tree"));
    menu_bar->Append(item_menu, wxT("&Item"));
    SetMenuBar(menu_bar);
#endif // wxUSE_MENUS

#if wxUSE_LOG
    // create the controls
    m_textCtrl = new wxTextCtrl(this, wxID_ANY, wxT(""),
                                wxDefaultPosition, wxDefaultSize,
                                wxTE_MULTILINE | wxSUNKEN_BORDER);
#endif // wxUSE_LOG

    CreateTreeWithDefStyle();

    menu_bar->Check(TreeListTest_ToggleImages, true);

#if wxUSE_STATUSBAR
    // create a status bar
    CreateStatusBar(2);
#endif // wxUSE_STATUSBAR

#if wxUSE_LOG
#ifdef __WXMOTIF__
    // For some reason, we get a memcpy crash in wxLogStream::DoLogStream
    // on gcc/wxMotif, if we use wxLogTextCtl. Maybe it's just gcc?
    delete wxLog::SetActiveTarget(new wxLogStderr);
#else
    // set our text control as the log target
    wxLogTextCtrl *logWindow = new wxLogTextCtrl(m_textCtrl);
    delete wxLog::SetActiveTarget(logWindow);
#endif
#endif // wxUSE_LOG
}

MyFrame::~MyFrame()
{
#if wxUSE_LOG
    delete wxLog::SetActiveTarget(NULL);
#endif // wxUSE_LOG
}

void MyFrame::CreateTreeWithDefStyle()
{
    long style = wxTR_DEFAULT_STYLE |
#ifndef NO_VARIABLE_HEIGHT
                 wxTR_HAS_VARIABLE_ROW_HEIGHT |
#endif
                 wxTR_EDIT_LABELS;

    CreateTree(style | wxSUNKEN_BORDER);

    // as we don't know what wxTR_DEFAULT_STYLE could contain, test for
    // everything
    wxMenuBar *mbar = GetMenuBar();
    mbar->Check(TreeListTest_TogButtons, (style & wxTR_HAS_BUTTONS) != 0);
    mbar->Check(TreeListTest_TogTwist, (style & wxTR_TWIST_BUTTONS) != 0);
    mbar->Check(TreeListTest_TogLines, (style & wxTR_NO_LINES) == 0);
    mbar->Check(TreeListTest_TogRootLines, (style & wxTR_LINES_AT_ROOT) != 0);
    mbar->Check(TreeListTest_TogHideRoot, (style & wxTR_HIDE_ROOT) != 0);
    mbar->Check(TreeListTest_TogEdit, (style & wxTR_EDIT_LABELS) != 0);
    mbar->Check(TreeListTest_TogBorder, (style & wxTR_ROW_LINES) != 0);
    mbar->Check(TreeListTest_TogFullHighlight, (style & wxTR_FULL_ROW_HIGHLIGHT) != 0);
}

void MyFrame::CreateTree(long style)
{
    m_treeListCtrl = new MyTreeListCtrl(this, TreeListTest_Ctrl,
                                wxDefaultPosition, wxDefaultSize,
                                style);
    Resize();
}

void MyFrame::TogStyle(int id, long flag)
{
    long style = m_treeListCtrl->GetWindowStyle() ^ flag;

    // most TreeListCtrl styles can't be changed on the fly using the native
    // control and the tree must be recreated
#ifndef __WXMSW__
    m_treeListCtrl->SetWindowStyle(style);
#else // MSW
    delete m_treeListCtrl;
    CreateTree(style);
#endif // !MSW/MSW

    GetMenuBar()->Check(id, (style & flag) != 0);
}

void MyFrame::OnSize(wxSizeEvent& event)
{
    if ( m_treeListCtrl
#if wxUSE_LOG
                    && m_textCtrl
#endif // wxUSE_LOG
                                  )
    {
        Resize();
    }

    event.Skip();
}

void MyFrame::Resize()
{
    wxSize size = GetClientSize();
    m_treeListCtrl->SetSize(0, 0, size.x, size.y
#if !wxUSE_LOG
                                            );
#else
                                            *2/3);
    m_textCtrl->SetSize(0, 2*size.y/3, size.x, size.y/3);
#endif
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxT("Tree test sample\n")
                 wxT("(c) Julian Smart 1997, Vadim Zeitlin 1998"),
                 wxT("About tree test"),
                 wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnRename(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = m_treeListCtrl->GetSelection();

    CHECK_ITEM( item );

    // old code - now we edit in place
#if 0
    static wxString s_text;
    s_text = wxGetTextFromUser(wxT("New name: "), wxT("Tree sample question"),
            s_text, this);
    if ( !s_text.IsEmpty() )
    {
        m_treeListCtrl->SetItemText(item, s_text);
    }
#endif // 0

    // TODO demonstrate creating a custom edit control...
    (void)m_treeListCtrl->EditLabel(item);
}

void MyFrame::OnCount(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = m_treeListCtrl->GetSelection();

    CHECK_ITEM( item );

    int i = m_treeListCtrl->GetChildrenCount( item, false );

    wxLogMessage(wxT("%d children"), i);
}

void MyFrame::OnCountRec(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = m_treeListCtrl->GetSelection();

    CHECK_ITEM( item );

    int i = m_treeListCtrl->GetChildrenCount( item );

    wxLogMessage(wxT("%d children"), i);
}

void MyFrame::DoSort(bool reverse)
{
    wxTreeItemId item = m_treeListCtrl->GetSelection();

    CHECK_ITEM( item );

    m_treeListCtrl->DoSortChildren(item, reverse);
}

void MyFrame::OnDump(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId root = m_treeListCtrl->GetSelection();

    CHECK_ITEM( root );

    m_treeListCtrl->GetItemsRecursively(root);
}

#ifndef NO_MULTIPLE_SELECTION

void MyFrame::OnToggleSel(wxCommandEvent& event)
{
    TogStyle(event.GetId(), wxTR_MULTIPLE);
}

void MyFrame::OnDumpSelected(wxCommandEvent& WXUNUSED(event))
{
    wxArrayTreeItemIds array;

    size_t count = m_treeListCtrl->GetSelections(array);
    wxLogMessage(wxT("%u items selected"), count);

    for ( size_t n = 0; n < count; n++ )
    {
        wxLogMessage(wxT("\t%s"), m_treeListCtrl->GetItemText(array.Item(n)).c_str());
    }
}

void MyFrame::OnSelect(wxCommandEvent& WXUNUSED(event))
{
    m_treeListCtrl->SelectItem(m_treeListCtrl->GetSelection());
}

void MyFrame::OnSelectRoot(wxCommandEvent& WXUNUSED(event))
{
    m_treeListCtrl->SelectItem(m_treeListCtrl->GetRootItem());
}

void MyFrame::OnUnselect(wxCommandEvent& WXUNUSED(event))
{
    m_treeListCtrl->UnselectAll();
}

#endif // NO_MULTIPLE_SELECTION

void MyFrame::DoSetBold(bool bold)
{
    wxTreeItemId item = m_treeListCtrl->GetSelection();

    CHECK_ITEM( item );

    m_treeListCtrl->SetItemBold(item, bold);
}

void MyFrame::OnDelete(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = m_treeListCtrl->GetSelection();

    CHECK_ITEM( item );

    m_treeListCtrl->Delete(item);
}

void MyFrame::OnDeleteChildren(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = m_treeListCtrl->GetSelection();

    CHECK_ITEM( item );

    m_treeListCtrl->DeleteChildren(item);
}

void MyFrame::OnDeleteAll(wxCommandEvent& WXUNUSED(event))
{
    m_treeListCtrl->DeleteChildren (m_treeListCtrl->GetRootItem());
}

void MyFrame::OnRecreate(wxCommandEvent& event)
{
    OnDeleteAll(event);
    m_treeListCtrl->AddTestItemsToTree(5, 2);
}

void MyFrame::OnSetImageSize(wxCommandEvent& WXUNUSED(event))
{
    int size = wxGetNumberFromUser(wxT("Enter the size for the images to use"),
                                    wxT("Size: "),
                                    wxT("TreeListCtrl sample"),
                                    m_treeListCtrl->ImageSize());
    if ( size == -1 )
        return;

    m_treeListCtrl->CreateImageList (size);
    wxGetApp().SetShowImages(true);
}

void MyFrame::OnToggleImages(wxCommandEvent& WXUNUSED(event))
{
    if ( wxGetApp().ShowImages() )
    {
        m_treeListCtrl->CreateImageList (-1);
        wxGetApp().SetShowImages(false);
    }
    else
    {
        m_treeListCtrl->CreateImageList (16);
        wxGetApp().SetShowImages(true);
    }
}

void MyFrame::OnToggleButtons(wxCommandEvent& WXUNUSED(event))
{
    if ( wxGetApp().ShowButtons() )
    {
        m_treeListCtrl->CreateButtonsImageList (-1);
        wxGetApp().SetShowButtons(false);
    }
    else
    {
        m_treeListCtrl->CreateButtonsImageList (16);
        wxGetApp().SetShowButtons(true);
    }
}

void MyFrame::OnCollapseAndReset(wxCommandEvent& WXUNUSED(event))
{
    m_treeListCtrl->CollapseAndReset(m_treeListCtrl->GetRootItem());
}

void MyFrame::OnEnsureVisible(wxCommandEvent& WXUNUSED(event))
{
    m_treeListCtrl->DoEnsureVisible();
}

void MyFrame::OnInsertItem(wxCommandEvent& WXUNUSED(event))
{
    int image = wxGetApp().ShowImages() ? MyTreeListCtrl::TreeListCtrlIcon_File : -1;
    m_treeListCtrl->InsertItem(m_treeListCtrl->GetRootItem(), image, wxT("2nd item"));
}

void MyFrame::OnAddItem(wxCommandEvent& WXUNUSED(event))
{
    static int s_num = 0;

    wxString text;
    text.Printf(wxT("Item #%d"), ++s_num);

    m_treeListCtrl->AppendItem(m_treeListCtrl->GetRootItem(),
                           text /*,
                           MyTreeListCtrl::TreeListCtrlIcon_File */ );
}

void MyFrame::OnIncIndent(wxCommandEvent& WXUNUSED(event))
{
    unsigned int indent = m_treeListCtrl->GetIndent();
    if (indent < 100)
        m_treeListCtrl->SetIndent( indent+5 );
}

void MyFrame::OnDecIndent(wxCommandEvent& WXUNUSED(event))
{
    unsigned int indent = m_treeListCtrl->GetIndent();
    if (indent > 10)
        m_treeListCtrl->SetIndent( indent-5 );
}

void MyFrame::OnToggleIcon(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId item = m_treeListCtrl->GetSelection();

    CHECK_ITEM( item );

    m_treeListCtrl->DoToggleIcon(item);
}

void MyFrame::OnSetFgColour(wxCommandEvent& WXUNUSED(event))
{
    wxColour col = wxGetColourFromUser(this, m_treeListCtrl->GetForegroundColour());
    if ( col.Ok() )
        m_treeListCtrl->SetForegroundColour(col);
}

void MyFrame::OnSetBgColour(wxCommandEvent& WXUNUSED(event))
{
    wxColour col = wxGetColourFromUser(this, m_treeListCtrl->GetBackgroundColour());
    if ( col.Ok() )
        m_treeListCtrl->SetBackgroundColour(col);
}

// MyTreeListCtrl implementation
IMPLEMENT_DYNAMIC_CLASS(MyTreeListCtrl, wxTreeListCtrl)

MyTreeListCtrl::MyTreeListCtrl(wxWindow *parent, const wxWindowID id,
                       const wxPoint& pos, const wxSize& size,
                       long style)
          : wxTreeListCtrl(parent, id, pos, size, style)
{
    m_reverseSort = false;

    CreateImageList (16);

    // Add some items to the tree
    AddTestItemsToTree(5, 2);
}

void MyTreeListCtrl::CreateImageList (int size)
{
    if (size == -1){
        SetImageList (NULL);
        return;
    }
    if (size == 0) {
        size = wxIcon (icon1_xpm).GetWidth();
    }
    m_imageSize = size;

    // Make an image list containing small icons
    wxImageList *images = new wxImageList (size, size, true);

    // should correspond to TreeListCtrlIcon_xxx enum
    wxBusyCursor wait;
    wxIcon icons[5];
    icons[0] = wxIcon (icon1_xpm);
    icons[1] = wxIcon (icon2_xpm);
    icons[2] = wxIcon (icon3_xpm);
    icons[3] = wxIcon (icon4_xpm);
    icons[4] = wxIcon (icon5_xpm);

    for (size_t i = 0; i < WXSIZEOF(icons); i++) {
        if (size == m_imageSize) {
            images->Add (icons[i]);
        }else{
            images->Add (wxBitmap(wxBitmap(icons[i]).ConvertToImage().Rescale (size, size)));
        }
    }

    AssignImageList (images);
}

void MyTreeListCtrl::CreateButtonsImageList (int size)
{
    if (size == -1) {
        SetButtonsImageList (NULL);
        return;
    }
    if (size == 0) {
        size = wxIcon (icon3_xpm).GetWidth();
    }
    int imageSize = size;

    // Make an image list containing small icons
    wxImageList *images = new wxImageList (size, size, true);

    // should correspond to TreeListCtrlIcon_xxx enum
    wxBusyCursor wait;
    wxIcon icons[4];
    icons[0] = wxIcon (icon3_xpm);   // closed
    icons[1] = wxIcon (icon3_xpm);   // closed, selected
    icons[2] = wxIcon (icon5_xpm);   // open
    icons[3] = wxIcon (icon5_xpm);   // open, selected

    for (size_t i = 0; i < WXSIZEOF(icons); i++) {
        if (size == m_imageSize) {
            images->Add (icons[i]);
        }else{
            images->Add(wxBitmap(wxBitmap(icons[i]).ConvertToImage().Rescale (size, size)));
        }
    }

    AssignButtonsImageList (images);
}

MyTreeListCtrl::~MyTreeListCtrl()
{
}

int MyTreeListCtrl::OnCompareItems(const wxTreeItemId& item1,
                               const wxTreeItemId& item2)
{
    if ( m_reverseSort )
    {
        // just exchange 1st and 2nd items
        return wxTreeListCtrl::OnCompareItems(item2, item1);
    }
    else
    {
        return wxTreeListCtrl::OnCompareItems(item1, item2);
    }
}

void MyTreeListCtrl::AddItemsRecursively (const wxTreeItemId& idParent,
                                          size_t numChildren,
                                          size_t depth,
                                          size_t folder)
{
    if ( depth > 0 )
    {
        bool hasChildren = depth > 1;

        wxString str;
        for ( size_t n = 0; n < numChildren; n++ )
        {
            // at depth 1 elements won't have any more children
            if ( hasChildren )
                str.Printf(wxT("%s child %d"), wxT("Folder"), n + 1);
            else
                str.Printf(wxT("%s child %d.%d"), wxT("File"), folder, n + 1);

            // here we pass to AppendItem() normal and selected item images (we
            // suppose that selected image follows the normal one in the enum)
            int image, imageSel;
            if ( wxGetApp().ShowImages() )
            {
                image = depth == 1 ? TreeListCtrlIcon_File : TreeListCtrlIcon_Folder;
                imageSel = image + 1;
            }
            else
            {
                image = imageSel = -1;
            }
            wxTreeItemId id = AppendItem (idParent, str, image, imageSel,
                                          new MyTreeItemData(str, n+1));
            str.Printf (_("Item%d"), n + 1);
            SetItemText (id, 1, _("Col1, ") + str);
            SetItemText (id, 2, _("Col2, ") + str);

            // and now we also set the expanded one (only for the folders)
            if ( hasChildren && wxGetApp().ShowImages() )
            {
                SetItemImage(id, TreeListCtrlIcon_FolderOpened,
                             wxTreeItemIcon_Expanded);
            }

            // remember the last child for OnEnsureVisible()
            if ( !hasChildren && n == numChildren - 1 )
            {
                m_lastItem = id;
            }

            AddItemsRecursively(id, numChildren, depth - 1, n + 1);
        }
    }
    //else: done!
}

void MyTreeListCtrl::AddTestItemsToTree(size_t numChildren,
                                    size_t depth)
{
    int image = wxGetApp().ShowImages() ? MyTreeListCtrl::TreeListCtrlIcon_Folder : -1;
    AddColumn (_("Main"), 200);
    SetColumnEditable (0, true);
    AddColumn (_("Col1"), 80, wxALIGN_LEFT);
    SetColumnEditable (1, true);
    AddColumn (_("Col2"), 80, wxALIGN_RIGHT);
    SetColumnEditable (2, true);
    wxTreeItemId rootId = AddRoot(wxT("Root"),
                                  image, image,
                                  new MyTreeItemData(wxT("Root item"), -1));
    if ( image != -1 )
    {
        SetItemImage(rootId, TreeListCtrlIcon_FolderOpened, wxTreeItemIcon_Expanded);
    }

    AddItemsRecursively(rootId, numChildren, depth, 0);
    AddItemsRecursively(rootId, 1, 1, 0);

    // set some colours/fonts for testing
    SetItemFont(rootId, *wxITALIC_FONT);

    wxTreeItemIdValue cookie;
    wxTreeItemId id = GetFirstChild(rootId, cookie);
    SetItemTextColour(id, *wxBLUE);

    id = GetNextChild(rootId, cookie);
    id = GetNextChild(rootId, cookie);
    SetItemTextColour(id, *wxRED);
    SetItemBackgroundColour(id, *wxLIGHT_GREY);
}

void MyTreeListCtrl::GetItemsRecursively(const wxTreeItemId& idParent,
                                     wxTreeItemIdValue cookie)
{
    wxTreeItemId id;

    if ( !cookie )
        id = GetFirstChild(idParent, cookie);
    else
        id = GetNextChild(idParent, cookie);

    if ( !id.IsOk() )
        return;

    wxString text = GetItemText(id);
    wxLogMessage(text);

    if (HasChildren(id))
        GetItemsRecursively(id);

    GetItemsRecursively(idParent, cookie);
}

void MyTreeListCtrl::DoToggleIcon(const wxTreeItemId& item)
{
    int image = GetItemImage(item) == TreeListCtrlIcon_Folder ? TreeListCtrlIcon_File
                                                          : TreeListCtrlIcon_Folder;

    SetItemImage(item, image);
}


// avoid repetition
#define TREE_EVENT_HANDLER(name)                                 \
void MyTreeListCtrl::name(wxTreeEvent& event)                        \
{                                                                \
    wxLogMessage(wxT(#name));                                    \
    SetLastItem(wxTreeItemId());                                 \
    event.Skip();                                                \
}

TREE_EVENT_HANDLER(OnBeginRDrag)
TREE_EVENT_HANDLER(OnDeleteItem)
TREE_EVENT_HANDLER(OnGetInfo)
TREE_EVENT_HANDLER(OnSetInfo)
TREE_EVENT_HANDLER(OnItemExpanded)
TREE_EVENT_HANDLER(OnItemExpanding)
TREE_EVENT_HANDLER(OnItemCollapsed)
TREE_EVENT_HANDLER(OnSelChanged)
TREE_EVENT_HANDLER(OnSelChanging)

#undef TREE_EVENT_HANDLER

void LogKeyEvent(const wxChar *name, const wxKeyEvent& event)
{
    wxString key;
    long keycode = event.GetKeyCode();
    {
        switch ( keycode )
        {
            case WXK_BACK: key = wxT("BACK"); break;
            case WXK_TAB: key = wxT("TAB"); break;
            case WXK_RETURN: key = wxT("RETURN"); break;
            case WXK_ESCAPE: key = wxT("ESCAPE"); break;
            case WXK_SPACE: key = wxT("SPACE"); break;
            case WXK_DELETE: key = wxT("DELETE"); break;
            case WXK_START: key = wxT("START"); break;
            case WXK_LBUTTON: key = wxT("LBUTTON"); break;
            case WXK_RBUTTON: key = wxT("RBUTTON"); break;
            case WXK_CANCEL: key = wxT("CANCEL"); break;
            case WXK_MBUTTON: key = wxT("MBUTTON"); break;
            case WXK_CLEAR: key = wxT("CLEAR"); break;
            case WXK_SHIFT: key = wxT("SHIFT"); break;
            case WXK_ALT: key = wxT("ALT"); break;
            case WXK_CONTROL: key = wxT("CONTROL"); break;
            case WXK_MENU: key = wxT("MENU"); break;
            case WXK_PAUSE: key = wxT("PAUSE"); break;
            case WXK_CAPITAL: key = wxT("CAPITAL"); break;
            case WXK_PRIOR: key = wxT("PRIOR"); break;
            case WXK_NEXT: key = wxT("NEXT"); break;
            case WXK_END: key = wxT("END"); break;
            case WXK_HOME: key = wxT("HOME"); break;
            case WXK_LEFT: key = wxT("LEFT"); break;
            case WXK_UP: key = wxT("UP"); break;
            case WXK_RIGHT: key = wxT("RIGHT"); break;
            case WXK_DOWN: key = wxT("DOWN"); break;
            case WXK_SELECT: key = wxT("SELECT"); break;
            case WXK_PRINT: key = wxT("PRINT"); break;
            case WXK_EXECUTE: key = wxT("EXECUTE"); break;
            case WXK_SNAPSHOT: key = wxT("SNAPSHOT"); break;
            case WXK_INSERT: key = wxT("INSERT"); break;
            case WXK_HELP: key = wxT("HELP"); break;
            case WXK_NUMPAD0: key = wxT("NUMPAD0"); break;
            case WXK_NUMPAD1: key = wxT("NUMPAD1"); break;
            case WXK_NUMPAD2: key = wxT("NUMPAD2"); break;
            case WXK_NUMPAD3: key = wxT("NUMPAD3"); break;
            case WXK_NUMPAD4: key = wxT("NUMPAD4"); break;
            case WXK_NUMPAD5: key = wxT("NUMPAD5"); break;
            case WXK_NUMPAD6: key = wxT("NUMPAD6"); break;
            case WXK_NUMPAD7: key = wxT("NUMPAD7"); break;
            case WXK_NUMPAD8: key = wxT("NUMPAD8"); break;
            case WXK_NUMPAD9: key = wxT("NUMPAD9"); break;
            case WXK_MULTIPLY: key = wxT("MULTIPLY"); break;
            case WXK_ADD: key = wxT("ADD"); break;
            case WXK_SEPARATOR: key = wxT("SEPARATOR"); break;
            case WXK_SUBTRACT: key = wxT("SUBTRACT"); break;
            case WXK_DECIMAL: key = wxT("DECIMAL"); break;
            case WXK_DIVIDE: key = wxT("DIVIDE"); break;
            case WXK_F1: key = wxT("F1"); break;
            case WXK_F2: key = wxT("F2"); break;
            case WXK_F3: key = wxT("F3"); break;
            case WXK_F4: key = wxT("F4"); break;
            case WXK_F5: key = wxT("F5"); break;
            case WXK_F6: key = wxT("F6"); break;
            case WXK_F7: key = wxT("F7"); break;
            case WXK_F8: key = wxT("F8"); break;
            case WXK_F9: key = wxT("F9"); break;
            case WXK_F10: key = wxT("F10"); break;
            case WXK_F11: key = wxT("F11"); break;
            case WXK_F12: key = wxT("F12"); break;
            case WXK_F13: key = wxT("F13"); break;
            case WXK_F14: key = wxT("F14"); break;
            case WXK_F15: key = wxT("F15"); break;
            case WXK_F16: key = wxT("F16"); break;
            case WXK_F17: key = wxT("F17"); break;
            case WXK_F18: key = wxT("F18"); break;
            case WXK_F19: key = wxT("F19"); break;
            case WXK_F20: key = wxT("F20"); break;
            case WXK_F21: key = wxT("F21"); break;
            case WXK_F22: key = wxT("F22"); break;
            case WXK_F23: key = wxT("F23"); break;
            case WXK_F24: key = wxT("F24"); break;
            case WXK_NUMLOCK: key = wxT("NUMLOCK"); break;
            case WXK_SCROLL: key = wxT("SCROLL"); break;
            case WXK_PAGEUP: key = wxT("PAGEUP"); break;
            case WXK_PAGEDOWN: key = wxT("PAGEDOWN"); break;
            case WXK_NUMPAD_SPACE: key = wxT("NUMPAD_SPACE"); break;
            case WXK_NUMPAD_TAB: key = wxT("NUMPAD_TAB"); break;
            case WXK_NUMPAD_ENTER: key = wxT("NUMPAD_ENTER"); break;
            case WXK_NUMPAD_F1: key = wxT("NUMPAD_F1"); break;
            case WXK_NUMPAD_F2: key = wxT("NUMPAD_F2"); break;
            case WXK_NUMPAD_F3: key = wxT("NUMPAD_F3"); break;
            case WXK_NUMPAD_F4: key = wxT("NUMPAD_F4"); break;
            case WXK_NUMPAD_HOME: key = wxT("NUMPAD_HOME"); break;
            case WXK_NUMPAD_LEFT: key = wxT("NUMPAD_LEFT"); break;
            case WXK_NUMPAD_UP: key = wxT("NUMPAD_UP"); break;
            case WXK_NUMPAD_RIGHT: key = wxT("NUMPAD_RIGHT"); break;
            case WXK_NUMPAD_DOWN: key = wxT("NUMPAD_DOWN"); break;
            case WXK_NUMPAD_PRIOR: key = wxT("NUMPAD_PRIOR"); break;
            case WXK_NUMPAD_PAGEUP: key = wxT("NUMPAD_PAGEUP"); break;
            case WXK_NUMPAD_PAGEDOWN: key = wxT("NUMPAD_PAGEDOWN"); break;
            case WXK_NUMPAD_END: key = wxT("NUMPAD_END"); break;
            case WXK_NUMPAD_BEGIN: key = wxT("NUMPAD_BEGIN"); break;
            case WXK_NUMPAD_INSERT: key = wxT("NUMPAD_INSERT"); break;
            case WXK_NUMPAD_DELETE: key = wxT("NUMPAD_DELETE"); break;
            case WXK_NUMPAD_EQUAL: key = wxT("NUMPAD_EQUAL"); break;
            case WXK_NUMPAD_MULTIPLY: key = wxT("NUMPAD_MULTIPLY"); break;
            case WXK_NUMPAD_ADD: key = wxT("NUMPAD_ADD"); break;
            case WXK_NUMPAD_SEPARATOR: key = wxT("NUMPAD_SEPARATOR"); break;
            case WXK_NUMPAD_SUBTRACT: key = wxT("NUMPAD_SUBTRACT"); break;
            case WXK_NUMPAD_DECIMAL: key = wxT("NUMPAD_DECIMAL"); break;

            default:
            {
               if ( wxIsprint((int)keycode) )
                   key.Printf(wxT("'%c'"), (char)keycode);
               else if ( keycode > 0 && keycode < 27 )
                   key.Printf(_("Ctrl-%c"), wxT('A') + keycode - 1);
               else
                   key.Printf(wxT("unknown (%ld)"), keycode);
            }
        }
    }

    wxLogMessage( wxT("%s event: %s (flags = %c%c%c%c)"),
                  name,
                  key.c_str(),
                  event.ControlDown() ? wxT('C') : wxT('-'),
                  event.AltDown() ? wxT('A') : wxT('-'),
                  event.ShiftDown() ? wxT('S') : wxT('-'),
                  event.MetaDown() ? wxT('M') : wxT('-'));
}

void MyTreeListCtrl::OnTreeKeyDown(wxTreeEvent& event)
{
    LogKeyEvent(wxT("Tree key down "), event.GetKeyEvent());

    event.Skip();
}

void MyTreeListCtrl::OnBeginDrag(wxTreeEvent& event)
{
    // need to explicitly allow drag
    if ( event.GetItem() != GetRootItem() )
    {
        m_draggedItem = event.GetItem();

        wxLogMessage(wxT("OnBeginDrag: started dragging %s"),
                     GetItemText(m_draggedItem).c_str());

        event.Allow();
    }
    else
    {
        wxLogMessage(wxT("OnBeginDrag: this item can't be dragged."));
    }
}

void MyTreeListCtrl::OnEndDrag(wxTreeEvent& event)
{
    wxTreeItemId itemSrc = m_draggedItem,
                 itemDst = event.GetItem();
    m_draggedItem = (wxTreeItemId)0l;

    // where to copy the item?
    if ( itemDst.IsOk() && !HasChildren(itemDst) )
    {
        // copy to the parent then
        itemDst = GetItemParent(itemDst);
    }

    if ( !itemDst.IsOk() )
    {
        wxLogMessage(wxT("OnEndDrag: can't drop here."));

        return;
    }

    wxString text = GetItemText(itemSrc);
    wxLogMessage(wxT("OnEndDrag: '%s' copied to '%s'."),
                 text.c_str(), GetItemText(itemDst).c_str());

    // just do append here - we could also insert it just before/after the item
    // on which it was dropped, but this requires slightly more work... we also
    // completely ignore the client data and icon of the old item but could
    // copy them as well.
    //
    // Finally, we only copy one item here but we might copy the entire tree if
    // we were dragging a folder.
    int image = wxGetApp().ShowImages() ? TreeListCtrlIcon_File : -1;
    AppendItem(itemDst, text, image);
}

void MyTreeListCtrl::OnBeginLabelEdit(wxTreeEvent& event)
{
    wxLogMessage(wxT("OnBeginLabelEdit"));

    // for testing, prevent this item's label editing
    wxTreeItemId itemId = event.GetItem();
    if ( IsTestItem(itemId) )
    {
        wxMessageBox(wxT("You can't edit this item."));

        event.Veto();
    }
}

void MyTreeListCtrl::OnEndLabelEdit(wxTreeEvent& event)
{
    wxLogMessage(wxT("OnEndLabelEdit"));

    // don't allow anything except letters in the labels
    if ( !event.GetLabel().IsWord() )
    {
        wxMessageBox(wxT("The new label should be a single word."));

        event.Veto();
    }
}

void MyTreeListCtrl::OnItemCollapsing(wxTreeEvent& event)
{
    wxLogMessage(wxT("OnItemCollapsing"));

    // for testing, prevent the user from collapsing the first child folder
    wxTreeItemId itemId = event.GetItem();
    if ( IsTestItem(itemId) )
    {
        wxMessageBox(wxT("You can't collapse this item."));

        event.Veto();
    }
}

void MyTreeListCtrl::OnItemActivated(wxTreeEvent& event)
{
    // show some info about this item
    wxTreeItemId itemId = event.GetItem();
    MyTreeItemData *item = (MyTreeItemData *)GetItemData(itemId);

    if (item) item->ShowInfo(this);
    wxLogMessage(wxString::Format (wxT("OnItemActivated, column=%d"), event.GetInt()));
    event.Skip();
}

void MyTreeListCtrl::OnItemRightClick(wxTreeEvent& event)
{
    ShowMenu(event.GetItem(), event.GetPoint());
}

void MyTreeListCtrl::OnContextMenu(wxContextMenuEvent& event)
{
    wxPoint pt = event.GetPosition();
    wxTreeItemId item = GetSelection();
    wxLogMessage(wxT("OnContextMenu at screen coords (%i, %i)"), pt.x, pt.y);

    //check if event was generated by keyboard (MSW-specific?)
    if (pt.x==-1 && pt.y==-1) //(this is how MSW indicates it)
    {
        //attempt to guess where to show the menu
        if (item.IsOk())
        {
            //if an item was clicked, show menu to the right of it
            wxRect rect;
            GetBoundingRect(item, rect, true); //true = only the label
            pt = wxPoint(rect.GetRight(), rect.GetTop());
        }
        else
        {
            pt = wxPoint(0, 0);
        }
    }
    else
    {
        //event was generated by mouse, use supplied coords
        pt = ScreenToClient(pt);
    }

    ShowMenu(item, pt);
}

void MyTreeListCtrl::ShowMenu(wxTreeItemId id, const wxPoint& pt)
{
    wxString title;
    if ( id.IsOk() )
    {
        title << wxT("Menu for ") << GetItemText(id);
    }
    else
    {
        title = wxT("Menu for no particular item");
    }

#if wxUSE_MENUS
    wxMenu menu(title);
    menu.Append(TreeListTest_About, wxT("&About..."));
    menu.Append(TreeListTest_Dump, wxT("&Dump"));

    PopupMenu(&menu, pt);
#endif // wxUSE_MENUS
}

void MyTreeListCtrl::OnRMouseDClick(wxMouseEvent& event)
{
    wxTreeItemId id = HitTest(event.GetPosition());
    if ( !id )
        wxLogMessage(wxT("No item under mouse"));
    else
    {
        MyTreeItemData *item = (MyTreeItemData *)GetItemData(id);
        if ( item )
            wxLogMessage(wxT("Item '%s' under mouse"), item->GetDesc());
    }
}

static inline const wxChar *Bool2String(bool b)
{
    return b ? wxT("") : wxT("not ");
}

void MyTreeItemData::ShowInfo(wxTreeListCtrl *tree)
{
    wxLogMessage(wxT("Item '%s': %sselected, %sexpanded, %sbold,\n")
                 wxT("%u children (%u immediately under this item)."),
                 m_desc.c_str(),
                 Bool2String(tree->IsSelected(GetId())),
                 Bool2String(tree->IsExpanded(GetId())),
                 Bool2String(tree->IsBold(GetId())),
                 tree->GetChildrenCount(GetId()),
                 tree->GetChildrenCount(GetId(), false));
}

wxString MyTreeListCtrl::OnGetItemText( wxTreeItemData* item, long column ) const
{
    if( item != NULL )
    {
        MyTreeItemData* myitem = static_cast<MyTreeItemData*>(item);

        if( column == GetMainColumn() )
        {
            return myitem->GetDesc();
        }
        else if( myitem->GetRow() != -1 )
        {
            wxString s;
            s.Printf( wxT("V Col%ld, Item%d"), column, myitem->GetRow() );
            return s;
        }
    }
    return wxEmptyString;
}
