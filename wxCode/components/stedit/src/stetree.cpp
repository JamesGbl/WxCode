///////////////////////////////////////////////////////////////////////////////
// Name:        stetree.cpp
// Purpose:     wxSTEditorTreeCtrl
// Author:      John Labenski
// Modified by:
// Created:     03/05/2012
// RCS-ID:
// Copyright:   (c) John Labenski
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include "wx/stedit/stedit.h"
#include "wx/stedit/stetree.h"

#include "wxext.h"

//-----------------------------------------------------------------------------
// wxSTETreeItemData - wxTreeItemData for the wxTreeCtrl file list
//-----------------------------------------------------------------------------

wxSTETreeItemData::wxSTETreeItemData(const wxSTETreeItemData& steTreeData)
                  :m_id(steTreeData.m_id),
                   m_page_num(steTreeData.m_page_num),
                   m_notePage(steTreeData.m_notePage),
                   m_modified(steTreeData.m_modified),
                   m_root(steTreeData.m_root),
                   m_fileName(steTreeData.m_fileName),
                   m_treePath(steTreeData.m_treePath)
{
}

wxSTETreeItemData::~wxSTETreeItemData()
{
    wxSTEditor* editor = NULL;

    if (m_notePage != NULL)
    {
        editor = wxDynamicCast(m_notePage, wxSTEditor);

        if (!editor && wxDynamicCast(m_notePage, wxSTEditorSplitter))
            editor = wxDynamicCast(m_notePage, wxSTEditorSplitter)->GetEditor();
    }

    if (editor)
        editor->SetTreeItemId(wxTreeItemId());
}

//-----------------------------------------------------------------------------
// wxSTEditorTreeCtrl - wxTreeCtrl for the wxSTEditorNotebook class
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxSTEditorTreeCtrl, wxTreeCtrl)

BEGIN_EVENT_TABLE(wxSTEditorTreeCtrl, wxTreeCtrl)
    EVT_RIGHT_UP            (wxSTEditorTreeCtrl::OnRightUp)
    EVT_MENU                (wxID_ANY, wxSTEditorTreeCtrl::OnMenu)
    EVT_TREE_ITEM_ACTIVATED (wxID_ANY, wxSTEditorTreeCtrl::OnTreeCtrl)
END_EVENT_TABLE()

bool wxSTEditorTreeCtrl::Create(wxWindow *parent, wxWindowID id,
                                const wxPoint& pos, const wxSize& size,
                                long style, const wxValidator& validator,
                                const wxString& name)
{
    if (!wxTreeCtrl::Create(parent, id, pos, size, style, validator, name))
        return false;

    m_popupMenu = new wxMenu;
    m_popupMenu->Append(ID_STT_FILE_OPEN,  wxGetStockLabelEx(wxID_OPEN), _("Open file"));
    m_popupMenu->Append(ID_STT_FILE_CLOSE, _("Close"), _("Close selected page"));
    m_popupMenu->Append(ID_STT_FILE_PROPERTIES, wxGetStockLabelEx(wxID_PROPERTIES), _("Show document properties dialog"));
    m_popupMenu->AppendSeparator();
    m_popupMenu->Append(ID_STT_EXPAND_ALL,   _("Expand all paths"),   _("Expand all paths"));
    m_popupMenu->Append(ID_STT_COLLAPSE_ALL, _("Collapse all paths"), _("Collapse all paths"));
    m_popupMenu->AppendSeparator();
    m_popupMenu->AppendRadioItem(ID_STT_SHOW_FILENAME_ONLY,      _("Show only the filename"), _("Show only the filename"));
    m_popupMenu->AppendRadioItem(ID_STT_SHOW_FILEPATH_ONLY,      _("Show only the full file path"), _("Show only the full file path"));
    m_popupMenu->AppendRadioItem(ID_STT_SHOW_PATH_THEN_FILENAME, _("Show files grouped by paths"), _("Show files grouped by paths"));
    m_popupMenu->AppendRadioItem(ID_STT_SHOW_ALL_PATHS,          _("Show all paths"), _("Show all paths"));

    m_popupMenu->Check(ID_STT_SHOW_PATH_THEN_FILENAME, true);

    return true;
}

void wxSTEditorTreeCtrl::Init()
{
    m_display_type = SHOW_PATH_THEN_FILENAME;
    m_popupMenu    = NULL;
    m_steNotebook  = NULL;
}

wxSTEditorTreeCtrl::~wxSTEditorTreeCtrl()
{
    delete m_popupMenu;
    SetSTENotebook(NULL);
}

void wxSTEditorTreeCtrl::OnRightUp( wxMouseEvent& event )
{
    int flags = 0;
    wxTreeItemId id = HitTest(event.GetPosition(), flags);

    wxSTETreeItemData* data = NULL;
    if (id)
        data = (wxSTETreeItemData*)GetItemData(id);

    bool on_file = (data != NULL);

    m_popupMenu->Enable(ID_STT_FILE_OPEN,  on_file);
    m_popupMenu->Enable(ID_STT_FILE_CLOSE, on_file);

    // Are we selecting an editor or some other wxWindow
    wxSTEditor* editor = NULL;
    if (data != NULL)
    {
        editor = wxDynamicCast(data->m_notePage, wxSTEditor);

        if (!editor && wxDynamicCast(data->m_notePage, wxSTEditorSplitter))
            editor = wxDynamicCast(data->m_notePage, wxSTEditorSplitter)->GetEditor();
    }
    m_popupMenu->Enable(ID_STT_FILE_PROPERTIES, (editor != NULL));

    PopupMenu(m_popupMenu, event.GetPosition());
}

void wxSTEditorTreeCtrl::OnMenu( wxCommandEvent& event )
{
    HandleMenuEvent(event);
}

bool wxSTEditorTreeCtrl::HandleMenuEvent(wxCommandEvent &event)
{
    int win_id  = event.GetId();
    wxTreeItemId id = GetSelection();

    wxSTETreeItemData* data = NULL;
    if (id)
        data = (wxSTETreeItemData*)GetItemData(id);

    switch (win_id)
    {
        case ID_STT_FILE_OPEN :
        {
            if (id)
            {
                wxTreeEvent treeEvent(wxEVT_NULL, this, id);
                OnTreeCtrl(treeEvent);
            }
            return true;
        }
        case ID_STT_FILE_CLOSE :
        {
            if (id && m_steNotebook && data)
            {
                m_steNotebook->ClosePage(data->m_page_num, true);
            }
            return true;
        }
        case ID_STT_FILE_PROPERTIES :
        {
            if (id && m_steNotebook && data && data->m_notePage)
            {
                wxSTEditor* editor = wxDynamicCast(data->m_notePage, wxSTEditor);

                if (!editor && wxDynamicCast(data->m_notePage, wxSTEditorSplitter))
                    editor = wxDynamicCast(data->m_notePage, wxSTEditorSplitter)->GetEditor();

                if (editor)
                    editor->ShowPropertiesDialog();
            }
            return true;
        }
        case ID_STT_EXPAND_ALL :
        {
            ExpandAll();
            return true;
        }
        case ID_STT_COLLAPSE_ALL :
        {
            // Can't CollapseAll() if root node is hidden.
            wxTreeItemIdValue cookie;
            wxTreeItemId rootId = GetRootItem();
            wxTreeItemId childId = GetFirstChild(rootId, cookie);
            for (; childId; childId = GetNextChild(rootId, cookie))
            {
                CollapseAllChildren(childId);
            }
            return true;
        }
        case ID_STT_SHOW_FILENAME_ONLY :
        case ID_STT_SHOW_FILEPATH_ONLY :
        case ID_STT_SHOW_PATH_THEN_FILENAME :
        case ID_STT_SHOW_ALL_PATHS :
        {
            SetDisplayType((Display_Type)(win_id-ID_STT_SHOW_FILENAME_ONLY));
            return true;
        }
    }

    return false;
}

void wxSTEditorTreeCtrl::OnTreeCtrl(wxTreeEvent &event)
{
    if (m_steNotebook != NULL)
    {
        wxTreeItemId id = event.GetItem();
        wxSTETreeItemData* data = (wxSTETreeItemData*)GetItemData(id);
        if (data && (data->m_page_num >= 0))
            m_steNotebook->SetSelection(data->m_page_num);
        else
            event.Skip();
    }
}

void wxSTEditorTreeCtrl::OnNotebookPageChanged(wxNotebookEvent &event)
{
    event.Skip();
    UpdateFromNotebook();
}

void wxSTEditorTreeCtrl::OnSTEState(wxSTEditorEvent &event)
{
    event.Skip();

    if ( event.HasStateChange(STE_FILENAME | STE_MODIFIED | STE_EDITABLE) )
        UpdateFromNotebook();
}

void wxSTEditorTreeCtrl::OnWindowDestroy( wxWindowDestroyEvent& event )
{
    event.Skip();

    wxLongToLongHashMap::iterator it = m_windowToSTETreeItemDataMap.find((long)event.GetEventObject());

    if (it != m_windowToSTETreeItemDataMap.end())
    {
        wxSTETreeItemData* oldData = (wxSTETreeItemData*)it->second;
        DeleteItem(oldData->m_id, true, -1, GetRootItem());
        m_windowToSTETreeItemDataMap.erase(it->first);
    }
}

void wxSTEditorTreeCtrl::SetSTENotebook(wxSTEditorNotebook* notebook)
{
    if (m_steNotebook != NULL)
    {
        m_steNotebook->Disconnect(wxID_ANY, wxEVT_STN_PAGE_CHANGED,
                                  wxNotebookEventHandler(wxSTEditorTreeCtrl::OnNotebookPageChanged),
                                  NULL, this);
        m_steNotebook->Disconnect(wxID_ANY, wxEVT_STE_STATE_CHANGED,
                                  wxSTEditorEventHandler(wxSTEditorTreeCtrl::OnSTEState),
                                  NULL, this);
    }

    m_steNotebook = notebook;

    DeleteAllItems();
    m_windowToSTETreeItemDataMap.clear();

    if (m_steNotebook != NULL)
    {
        UpdateFromNotebook();

        m_steNotebook->Connect(wxID_ANY, wxEVT_STN_PAGE_CHANGED,
                               wxNotebookEventHandler(wxSTEditorTreeCtrl::OnNotebookPageChanged),
                               NULL, this);
        m_steNotebook->Connect(wxID_ANY, wxEVT_STE_STATE_CHANGED,
                               wxSTEditorEventHandler(wxSTEditorTreeCtrl::OnSTEState),
                               NULL, this);
    }
}

void wxSTEditorTreeCtrl::SetDisplayType(Display_Type display_type)
{
    m_display_type = display_type;
    SetSTENotebook(m_steNotebook);
}

/*
// can't use wxArray::Index since MSVC can't convert from wxTreeItemId to wxTreeItemIdBase
static int Find_wxArrayTreeItemId(const wxArrayTreeItemIds& arrayIds, const wxTreeItemId& id)
{
    size_t n, id_count = arrayIds.GetCount();
    for (n = 0; n < id_count; n++)
    {
        if (arrayIds[n] == id)
            return n;
    }
    return wxNOT_FOUND;
}
*/

void wxSTEditorTreeCtrl::UpdateFromNotebook()
{
    wxSTEditorNotebook *noteBook = GetSTEditorNotebook();
    if (!noteBook)
        return;

    int n;
    int page_count = noteBook->GetPageCount();
    int note_sel   = noteBook->GetSelection();

    wxTreeItemId id, selId;

    // Check for and add a root item to the treectrl, it's hidden
    wxTreeItemId rootId = GetRootItem();
    if (!rootId)
        rootId = AddRoot(_("Root"), -1, -1, NULL);

    // Check for and add a "Opened files" item to the treectrl
    wxArrayString openedfilesPath; openedfilesPath.Add(_("Opened files"));
    wxTreeItemId openedId = FindOrInsertItem(openedfilesPath, STE_TREECTRL_FIND_OR_INSERT);

    wxLongToLongHashMap windowToSTETreeItemDataMap = m_windowToSTETreeItemDataMap;

    Freeze();

    for (n = 0; n < page_count; n++)
    {
        id = wxTreeItemId(); // start fresh

        wxSTEditor* editor = noteBook->GetEditor(n);
        wxWindow* notePage = noteBook->GetPage(n);

        // If this editor was already added to the tree, check if it's still correct
        if (editor && editor->GetTreeItemId())
        {
            // get and check the old tree item id, the filename/path could have changed
            id = editor->GetTreeItemId();
            wxSTETreeItemData* oldData = NULL;
            if (id)
                oldData = (wxSTETreeItemData*)GetItemData(id);

            if (oldData && (oldData->m_notePage == notePage) &&
                (oldData->m_fileName != editor->GetFileName()))
            {
                // Erase refs to this page, we will recreate it
                m_windowToSTETreeItemDataMap.erase((long)notePage);
                windowToSTETreeItemDataMap.erase((long)notePage);
                DeleteItem(id, true, -1, openedId);

                // null it and add it correctly later
                editor->SetTreeItemId(wxTreeItemId());
            }
            else
            {
                if (oldData)
                    oldData->m_page_num = n; // resync

                windowToSTETreeItemDataMap.erase((long)notePage);
            }
        }

        if (!id)
        {
            // Create new data to add to the wxTreeItem
            wxSTETreeItemData* steTreeData = new wxSTETreeItemData(n, notePage);

            notePage->Connect(wxID_ANY, wxEVT_DESTROY,
                              wxWindowDestroyEventHandler(wxSTEditorTreeCtrl::OnWindowDestroy),
                              NULL, this);

            if (editor)
            {
                steTreeData->m_root = _("Opened files");
                steTreeData->m_fileName = editor->GetFileName();
                steTreeData->m_modified = editor->IsModified();
                wxFileName fn(steTreeData->m_fileName);
                fn.Normalize();

                switch (m_display_type)
                {
                    case SHOW_FILENAME_ONLY :
                    {
                        steTreeData->m_treePath.Add(steTreeData->m_root);
                        steTreeData->m_treePath.Add(fn.GetFullName());
                        break;
                    }
                    case SHOW_FILEPATH_ONLY :
                    {
                        steTreeData->m_treePath.Add(steTreeData->m_root);
                        steTreeData->m_treePath.Add(fn.GetFullPath());
                        break;
                    }
                    case SHOW_PATH_THEN_FILENAME :
                    {
                        steTreeData->m_treePath.Add(steTreeData->m_root);
                        steTreeData->m_treePath.Add(fn.GetPath());
                        steTreeData->m_treePath.Add(fn.GetFullName());
                        break;
                    }
                    case SHOW_ALL_PATHS :
                    {
                        steTreeData->m_treePath.Add(steTreeData->m_root);

                        //steTreeData.m_treePath.Add(fn.GetPath());
                        wxArrayString dirs = fn.GetDirs();
                        for (size_t i = 0; i < dirs.GetCount(); ++i)
                            steTreeData->m_treePath.Add(dirs[i]);

                        steTreeData->m_treePath.Add(fn.GetFullName());
                        break;
                    }
                }
            }
            else
            {
                steTreeData->m_root = _("Others");
                steTreeData->m_fileName = noteBook->GetPageText(n);

                steTreeData->m_treePath.Add(steTreeData->m_root);
                steTreeData->m_treePath.Add(steTreeData->m_fileName.GetFullPath());
            }

            // Always insert a new editor since if we already did, it'd have a treeitem id.
            // For other windows, who knows, you can only have one tree node per notebook page name
            if (editor)
            {
                id = FindOrInsertItem(steTreeData->m_treePath, STE_TREECTRL_INSERT);
                editor->SetTreeItemId(id);
            }
            else
                id = FindOrInsertItem(steTreeData->m_treePath, STE_TREECTRL_FIND_OR_INSERT);

            // must set new data before deleting old in MSW since it checks old before setting new
            wxTreeItemData* oldData = GetItemData(id);
            steTreeData->m_id = id;
            SetItemData(id, steTreeData);
            if (oldData) delete oldData;

            m_windowToSTETreeItemDataMap[(long)notePage] = (long)steTreeData;

            SetItemTextColour(id, steTreeData->m_modified ? *wxRED : *wxBLACK);
        }

        // we should have valid id at this point

        if (n == note_sel)
            selId = id;
        else if (IsBold(id))
            SetItemBold(id, false);
    }

    wxLongToLongHashMap::iterator it;
    for( it = windowToSTETreeItemDataMap.begin(); it != windowToSTETreeItemDataMap.end(); ++it )
    {
        wxSTETreeItemData* oldData = (wxSTETreeItemData*)it->second;
        DeleteItem(oldData->m_id, true, -1, openedId);
        m_windowToSTETreeItemDataMap.erase(it->first);
    }

    SortAllChildren(GetRootItem());
    Thaw();

    if (selId)
    {
        SetItemBold(selId);
        SelectItem(selId);
    }
}

void wxSTEditorTreeCtrl::SortAllChildren(const wxTreeItemId& item_)
{
    wxCHECK_RET(item_, wxT("Invalid wxTreeCtrl item"));

    wxTreeItemIdValue cookie;
    wxTreeItemId childId = GetFirstChild(item_, cookie);
    for (; childId; childId = GetNextChild(item_, cookie))
    {
        SortChildren(childId);
        SortAllChildren(childId);
    }
}

int wxSTEditorTreeCtrl::OnCompareItems(const wxTreeItemId& item1, const wxTreeItemId& item2)
{
    // When only showing the filenames we sort by the full path
    if (m_display_type == SHOW_FILENAME_ONLY)
    {
        wxSTETreeItemData* data1 = (wxSTETreeItemData*)GetItemData(item1);
        wxSTETreeItemData* data2 = (wxSTETreeItemData*)GetItemData(item2);

        if (data1 && data2)
        {
            int ret = data1->m_root.Cmp(data2->m_root);
            if (ret == 0)
                ret = data1->m_fileName.GetFullPath().Cmp(data2->m_fileName.GetFullPath());
            return ret;
        }
    }

    // by default sort by the name shown in the tree
    return wxTreeCtrl::OnCompareItems(item1, item2);
}

wxArrayString wxSTEditorTreeCtrl::GetItemPath(const wxTreeItemId& id_)
{
    wxArrayString pathArray;

    wxTreeItemId rootId = GetRootItem();
    if (!rootId)
        return pathArray;

    for (wxTreeItemId id = id_; id && (id != rootId); id = GetItemParent(id))
    {
        pathArray.Insert(GetItemText(id), 0);
    }

    return pathArray;
}

bool wxSTEditorTreeCtrl::DeleteItem(const wxArrayString& treePath, bool delete_empty)
{
    wxTreeItemId id = FindOrInsertItem(treePath, STE_TREECTRL_FIND);

    return DeleteItem(id, delete_empty, -1) > 0;
}

int wxSTEditorTreeCtrl::DeleteItem(const wxTreeItemId& id_, bool delete_empty, int levels, const wxTreeItemId& topId)
{
    int n = 0;
    wxTreeItemId id = id_;
    wxTreeItemId parentId;
    wxTreeItemId rootId = GetRootItem();

    if (!id)
        return 0;
    else if (!delete_empty)
    {
        Delete(id);
        n++;
    }
    else
    {
        // back up the tree and delete all parents that have no other children
        wxTreeItemId parentId_last;
        wxTreeItemId parentId = GetItemParent(id);
        wxTreeItemId rootId = GetRootItem();
        Delete(id);
        n++;

        while( parentId && (parentId != rootId) && (parentId != topId) &&
               ((n <= levels) || (levels == -1)))
        {
            unsigned int child_count = GetChildrenCount(parentId, false);

            if (child_count <= 1)
            {
                // verify that if a single child that it's not a file
                if (child_count == 1)
                {
                    wxTreeItemIdValue cookie;
                    wxTreeItemId childId = GetFirstChild(parentId, cookie);
                    wxSTETreeItemData* itemData = (wxSTETreeItemData*)GetItemData(childId);
                    if (itemData && (itemData->m_page_num != -1))
                        break;
                }

                // no other children in this node, try next parent
                parentId_last = parentId;
                parentId = GetItemParent(parentId);
                n++;
            }
            else
                break;
        }

        if (parentId_last)
            Delete(parentId_last);
    }

    return n;
}

wxTreeItemId wxSTEditorTreeCtrl::FindOrInsertItem(const wxArrayString& treePath, int find_type)
{
    wxCHECK_MSG(treePath.GetCount() > 0, wxTreeItemId(), wxT("Nothing to insert"));

    int n = 0, count = treePath.GetCount();

    // check for and add "Root" if not only_find
    wxTreeItemId parentId = GetRootItem();
    if (!parentId)
    {
        if (find_type == STE_TREECTRL_FIND)
            return wxTreeItemId();

        parentId = AddRoot(_("Root"), -1, -1, NULL);
    }

    wxTreeItemIdValue rootCookie;
    wxTreeItemId id = GetFirstChild(parentId, rootCookie);

    // check for and add first path if not only_find
    if (!id)
    {
        if (find_type == STE_TREECTRL_FIND)
            return wxTreeItemId();

        parentId = id = AppendItem(parentId, treePath[n], -1, -1, NULL);
        n++;
    }

    // Iterate though the path list
    while (id && (n < count))
    {
        if (GetItemText(id) == treePath[n])
        {
            if (n == count - 1)       // found the existing item w/ full path
            {
                if (find_type == STE_TREECTRL_INSERT)
                    return AppendItem(parentId, treePath[n], -1, -1, NULL);
                else
                    return id;
            }

            parentId = id;
            id = GetFirstChild(id, rootCookie); // next path part
            n++;
        }
        else
        {
            id = GetNextSibling(id);         // find this path part
        }

        if (!id)
        {
            if (find_type == STE_TREECTRL_FIND)
                return wxTreeItemId();

            id = parentId;                              // use last good parent
            for (; n < count; n++)                      // append rest of path
            {
                id = AppendItem(id, treePath[n], -1, -1, NULL);

                if (n == count - 1)
                    return id;
            }
        }

    }

    return wxTreeItemId();
}

size_t wxSTEditorTreeCtrl::GetAllChildrenItemIds(const wxTreeItemId& start_id, wxArrayTreeItemIds& arrayIds, int get_type)
{
    // MSW crashes on GetNextSibling on the root item
    if (start_id == GetRootItem())
    {
        wxTreeItemIdValue cookie;
        wxTreeItemId id = GetFirstChild(start_id, cookie);
        return DoGetAllChildrenItemIds(id, arrayIds, get_type);
    }

    return DoGetAllChildrenItemIds(start_id, arrayIds, get_type);
}

size_t wxSTEditorTreeCtrl::DoGetAllChildrenItemIds(const wxTreeItemId& start_id, wxArrayTreeItemIds& arrayIds, int get_type)
{
    size_t count = 0;

    for (wxTreeItemId id = start_id;
         id;
         id = GetNextSibling(id))
    {
        if (get_type == STE_TREECTRL_GET_ALL)
        {
            arrayIds.Add(id);
            count++;
        }
        else
        {
            wxTreeItemData* data = GetItemData(id);
            if ((data && ((get_type & STE_TREECTRL_GET_DATA) != 0)) ||
                (!data && ((get_type & STE_TREECTRL_GET_NODATA) != 0)))
            {
                arrayIds.Add(id);
                count++;
            }
        }

        wxTreeItemIdValue childCookie;
        wxTreeItemId childId = GetFirstChild(id, childCookie);
        if (childId)
            count += DoGetAllChildrenItemIds(childId, arrayIds, get_type);
    }
    return count;
}
