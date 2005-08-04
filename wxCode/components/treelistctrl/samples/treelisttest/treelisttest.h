/////////////////////////////////////////////////////////////////////////////
// Name:        treelisttest.h
// Purpose:     wxTreeListCtrl sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: treelisttest.h,v 1.5 2005-08-04 17:12:35 wyo Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef _TREELISTTEST_H_
#define _TREELISTTEST_H_

#include "wx/wx.h"
#include "wx/treelistctrl.h"

// Define a new application type
class MyApp : public wxApp
{
public:
    MyApp() { m_showImages = true; m_showButtons = false; }

    bool OnInit();

    void SetShowImages(bool show) { m_showImages = show; }
    bool ShowImages() const { return m_showImages; }

    void SetShowButtons(bool show) { m_showButtons = show; }
    bool ShowButtons() const { return m_showButtons; }

private:
    bool m_showImages, m_showButtons;
};

class MyTreeItemData : public wxTreeItemData
{
public:
    MyTreeItemData(const wxString& desc, int row) : m_desc(desc), m_row(row) { }

    void ShowInfo(wxTreeListCtrl *tree);
    const wxChar *GetDesc() const { return m_desc.c_str(); }
    int GetRow() const { return m_row; }

private:
    wxString m_desc;
    int m_row;
};

class MyTreeListCtrl : public wxTreeListCtrl
{
public:
    enum
    {
        TreeListCtrlIcon_File,
        TreeListCtrlIcon_FileSelected,
        TreeListCtrlIcon_Folder,
        TreeListCtrlIcon_FolderSelected,
        TreeListCtrlIcon_FolderOpened
    };

    MyTreeListCtrl() { }
    MyTreeListCtrl(wxWindow *parent, const wxWindowID id,
               const wxPoint& pos, const wxSize& size,
               long style);
    virtual ~MyTreeListCtrl();

    void OnBeginDrag(wxTreeEvent& event);
    void OnBeginRDrag(wxTreeEvent& event);
    void OnEndDrag(wxTreeEvent& event);
    void OnBeginLabelEdit(wxTreeEvent& event);
    void OnEndLabelEdit(wxTreeEvent& event);
    void OnDeleteItem(wxTreeEvent& event);
    void OnContextMenu(wxContextMenuEvent& event);
    void OnGetInfo(wxTreeEvent& event);
    void OnTreeRMouseClick(wxTreeEvent& event);
    void OnItemRightClick(wxTreeEvent& event);
    void OnSetInfo(wxTreeEvent& event);
    void OnItemExpanded(wxTreeEvent& event);
    void OnItemExpanding(wxTreeEvent& event);
    void OnItemCollapsed(wxTreeEvent& event);
    void OnItemCollapsing(wxTreeEvent& event);
    void OnSelChanged(wxTreeEvent& event);
    void OnSelChanging(wxTreeEvent& event);
    void OnTreeKeyDown(wxTreeEvent& event);
    void OnItemActivated(wxTreeEvent& event);
    void OnRMouseDClick(wxMouseEvent& event);

    void GetItemsRecursively(const wxTreeItemId& idParent,
                             wxTreeItemIdValue cookie = 0);

    void CreateImageList (int size = 0);
    void CreateButtonsImageList (int size = 0);

    void AddTestItemsToTree(size_t numChildren, size_t depth);

    void DoSortChildren(const wxTreeItemId& item, bool reverse = false)
        { m_reverseSort = reverse; wxTreeListCtrl::SortChildren(item); }
    void DoEnsureVisible() { if (m_lastItem.IsOk()) EnsureVisible(m_lastItem); }

    void DoToggleIcon(const wxTreeItemId& item);

    void ShowMenu(wxTreeItemId id, const wxPoint& pt);

    int ImageSize(void) const { return m_imageSize; }

    void SetLastItem(wxTreeItemId id) { m_lastItem = id; }

protected:
    virtual int OnCompareItems(const wxTreeItemId& i1, const wxTreeItemId& i2);
    virtual wxString OnGetItemText( wxTreeItemData* item, long column ) const;

    // is this the test item which we use in several event handlers?
    bool IsTestItem(const wxTreeItemId& item)
    {
        // the test item is the first child folder
        return GetItemParent(item) == GetRootItem() && !GetPrevSibling(item);
    }

private:
    void AddItemsRecursively(const wxTreeItemId& idParent,
                             size_t nChildren,
                             size_t depth,
                             size_t folder);

    int          m_imageSize;               // current size of images
    bool         m_reverseSort;             // flag for OnCompareItems
    wxTreeItemId m_lastItem,                // for OnEnsureVisible()
                 m_draggedItem;             // item being dragged right now

    // NB: due to an ugly wxMSW hack you _must_ use DECLARE_DYNAMIC_CLASS()
    //     if you want your overloaded OnCompareItems() to be called.
    //     OTOH, if you don't want it you may omit the next line - this will
    //     make default (alphabetical) sorting much faster under wxMSW.
    DECLARE_DYNAMIC_CLASS(MyTreeListCtrl)
    DECLARE_EVENT_TABLE()
};

// Define a new frame type
class MyFrame: public wxFrame
{
public:
    // ctor and dtor
    MyFrame(const wxString& title, int x, int y, int w, int h);
    virtual ~MyFrame();

    // menu callbacks
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnTogButtons(wxCommandEvent& event)
        { TogStyle(event.GetId(), wxTR_HAS_BUTTONS); }
    void OnTogTwist(wxCommandEvent& event)
        { TogStyle(event.GetId(), wxTR_TWIST_BUTTONS); }
    void OnTogLines(wxCommandEvent& event)
        { TogStyle(event.GetId(), wxTR_NO_LINES); }
    void OnTogEdit(wxCommandEvent& event)
        { TogStyle(event.GetId(), wxTR_EDIT_LABELS); }
    void OnTogHideRoot(wxCommandEvent& event)
        { TogStyle(event.GetId(), wxTR_HIDE_ROOT); }
    void OnTogRootLines(wxCommandEvent& event)
        { TogStyle(event.GetId(), wxTR_LINES_AT_ROOT); }
    void OnTogBorder(wxCommandEvent& event)
        { TogStyle(event.GetId(), wxTR_ROW_LINES); }
    void OnTogFullHighlight(wxCommandEvent& event)
        { TogStyle(event.GetId(), wxTR_FULL_ROW_HIGHLIGHT); }
    void OnTogVirtual(wxCommandEvent& event)
        { TogStyle(event.GetId(), wxTR_VIRTUAL); }

    void OnResetStyle(wxCommandEvent& WXUNUSED(event))
        { CreateTreeWithDefStyle(); }

    void OnSetFgColour(wxCommandEvent& event);
    void OnSetBgColour(wxCommandEvent& event);

    void OnDump(wxCommandEvent& event);
#ifndef NO_MULTIPLE_SELECTION
    void OnDumpSelected(wxCommandEvent& event);
    void OnSelect(wxCommandEvent& event);
    void OnUnselect(wxCommandEvent& event);
    void OnToggleSel(wxCommandEvent& event);
#endif // NO_MULTIPLE_SELECTION
    void OnSelectRoot(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnDeleteChildren(wxCommandEvent& event);
    void OnDeleteAll(wxCommandEvent& event);

    void OnRecreate(wxCommandEvent& event);
    void OnToggleButtons(wxCommandEvent& event);
    void OnToggleImages(wxCommandEvent& event);
    void OnSetImageSize(wxCommandEvent& event);
    void OnCollapseAndReset(wxCommandEvent& event);

    void OnSetBold(wxCommandEvent& WXUNUSED(event)) { DoSetBold(true); }
    void OnClearBold(wxCommandEvent& WXUNUSED(event)) { DoSetBold(false); }

    void OnEnsureVisible(wxCommandEvent& event);

    void OnCount(wxCommandEvent& event);
    void OnCountRec(wxCommandEvent& event);

    void OnRename(wxCommandEvent& event);
    void OnSort(wxCommandEvent& WXUNUSED(event)) { DoSort(); }
    void OnSortRev(wxCommandEvent& WXUNUSED(event)) { DoSort(true); }

    void OnAddItem(wxCommandEvent& event);
    void OnInsertItem(wxCommandEvent& event);

    void OnIncIndent(wxCommandEvent& event);
    void OnDecIndent(wxCommandEvent& event);

    void OnIncSpacing(wxCommandEvent& event);
    void OnDecSpacing(wxCommandEvent& event);

    void OnToggleIcon(wxCommandEvent& event);

    void OnIdle(wxIdleEvent& event);
    void OnSize(wxSizeEvent& event);

private:
    void TogStyle(int id, long flag);

    void DoSort(bool reverse = false);

    void Resize();

    void CreateTreeWithDefStyle();
    void CreateTree(long style);

    MyTreeListCtrl *m_treeListCtrl;
#if wxUSE_LOG
    wxTextCtrl *m_textCtrl;
#endif // wxUSE_LOG

    void DoSetBold(bool bold = true);

    DECLARE_EVENT_TABLE()
};

// menu and control ids
enum
{
    TreeListTest_Quit,
    TreeListTest_About,
    TreeListTest_TogButtons,
    TreeListTest_TogTwist,
    TreeListTest_TogLines,
    TreeListTest_TogEdit,
    TreeListTest_TogHideRoot,
    TreeListTest_TogRootLines,
    TreeListTest_TogBorder,
    TreeListTest_TogFullHighlight,
    TreeListTest_TogVirtual,
    TreeListTest_SetFgColour,
    TreeListTest_SetBgColour,
    TreeListTest_ResetStyle,
    TreeListTest_Dump,
    TreeListTest_DumpSelected,
    TreeListTest_Count,
    TreeListTest_CountRec,
    TreeListTest_Sort,
    TreeListTest_SortRev,
    TreeListTest_SetBold,
    TreeListTest_ClearBold,
    TreeListTest_Rename,
    TreeListTest_Delete,
    TreeListTest_DeleteChildren,
    TreeListTest_DeleteAll,
    TreeListTest_Recreate,
    TreeListTest_ToggleImages,
    TreeListTest_ToggleButtons,
    TreeListTest_SetImageSize,
    TreeListTest_ToggleSel,
    TreeListTest_CollapseAndReset,
    TreeListTest_EnsureVisible,
    TreeListTest_AddItem,
    TreeListTest_InsertItem,
    TreeListTest_IncIndent,
    TreeListTest_DecIndent,
    TreeListTest_IncSpacing,
    TreeListTest_DecSpacing,
    TreeListTest_ToggleIcon,
    TreeListTest_Select,
    TreeListTest_Unselect,
    TreeListTest_SelectRoot,
    TreeListTest_Ctrl = 1000
};

#endif // _TREELISTTEST_H_
