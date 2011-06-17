/***************************************************************
 * Name:      ThreadSearchLoggerList
 * Purpose:   ThreadSearchLoggerList implements the
 *            ThreadSearchLoggerBase interface with a wxListCtrl.
 * Author:    Jerome ANTOINE
 * Created:   2007-07-28
 * Copyright: Jerome ANTOINE
 * License:   GPL
 **************************************************************/

#include <sdk.h> // Code::Blocks SDK
#ifndef CB_PRECOMP
    // Required extra includes
#endif

#include <wx/listctrl.h>
#include <wx/dynarray.h>
#include <wx/gdicmn.h>

#include "ThreadSearch.h"
#include "ThreadSearchView.h"
#include "ThreadSearchEvent.h"
#include "ThreadSearchLoggerList.h"
#include "ThreadSearchControlIds.h"


ThreadSearchLoggerList::ThreadSearchLoggerList(ThreadSearchView& threadSearchView,
                                               ThreadSearch& threadSearchPlugin,
                                               InsertIndexManager::eFileSorting fileSorting,
                                               wxPanel* pParent,
                                               long id)
                       : ThreadSearchLoggerBase(threadSearchView, threadSearchPlugin, fileSorting),
                       m_IndexOffset(0),
                       m_SortColumn(-1),
                       m_Ascending(true)
{
    m_pListLog = new wxListCtrl(pParent, id, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL|wxSUNKEN_BORDER);
    m_pListLog->SetMinSize(wxSize(100,100));

    SetListColumns();

    // Events are managed dynamically to be able to stop/start management when required.
    ConnectEvents(pParent);
}


ThreadSearchLoggerList::~ThreadSearchLoggerList()
{
    wxWindow* pParent = m_pListLog->GetParent();
    if ( pParent != NULL )
    {
        // Events are managed dynamically to be able to stop/start management when required.
        DisconnectEvents(pParent);
    }
    m_pListLog->Destroy();
}


void ThreadSearchLoggerList::Update()
{
    ThreadSearchLoggerBase::Update();

    // Adds/Removes listview header and adds/removes vertical rules
    m_pListLog->SetSingleStyle(wxLC_NO_HEADER, !m_ThreadSearchPlugin.GetDisplayLogHeaders());
    m_pListLog->SetSingleStyle(wxLC_VRULES,     m_ThreadSearchPlugin.GetDrawLogLines());
    SetListColumns();
}


void ThreadSearchLoggerList::SetListColumns()
{
    m_pListLog->ClearAll();

    m_pListLog->InsertColumn(0, _("Directory"), wxLIST_FORMAT_LEFT, 100);
    m_pListLog->InsertColumn(1, _("File"),      wxLIST_FORMAT_LEFT, 100);
    m_pListLog->InsertColumn(2, _("Line"),      wxLIST_FORMAT_RIGHT, 50);
    m_pListLog->InsertColumn(3, _("Text"),      wxLIST_FORMAT_LEFT, 500);
}


void ThreadSearchLoggerList::OnLoggerListClick(wxListEvent& event)
{
    // Manages list log left (single) click
    if ( IsLineResultLine() )
    {
        // Gets file path and line from list control
        wxString filepath(wxEmptyString);
        long line;
        if ( GetFileLineFromListEvent(event, filepath, line) == false )
        {
            cbMessageBox(_("Failed to retrieve file path and line number"),_("Error"), wxICON_ERROR);
            return;
        }
        m_ThreadSearchView.OnLoggerClick(filepath, line);
    }
    event.Skip();
}


void ThreadSearchLoggerList::OnLoggerListDoubleClick(wxListEvent& event)
{
    // Manages list log left double click
    if ( IsLineResultLine() )
    {
        // Gets file path and line from list control
        wxString filepath(wxEmptyString);
        long line;
        if ( GetFileLineFromListEvent(event, filepath, line) == false )
        {
            cbMessageBox(_("Failed to retrieve file path and line number"), _("Error"), wxICON_ERROR);
            return;
        }
        m_ThreadSearchView.OnLoggerDoubleClick(filepath, line);
    }
    event.Skip();
}


bool ThreadSearchLoggerList::GetFileLineFromListEvent(wxListEvent& event, wxString& filepath, long &line)
{
    bool success = false;
    wxListItem listItem;

    do {
        // Finds selected item index
        long index = m_pListLog->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if ( index == -1 ) break;

        // First, gets file dir
        wxString filedir;
        listItem.m_itemId = index;
        listItem.m_col    = 0;
        listItem.m_mask   = wxLIST_MASK_TEXT;

        if ( m_pListLog->GetItem(listItem) == false ) break;

        filedir = listItem.GetText();

        // Then build complete filename from dir and file
        listItem.m_col = 1;
        if ( m_pListLog->GetItem(listItem) == false ) break;

        wxFileName sFile(filedir, listItem.GetText());
        filepath = sFile.GetFullPath();

        // Then gets line number
        listItem.m_col = 2;
        if ( m_pListLog->GetItem(listItem) == false ) break;

        if ( listItem.GetText().ToLong(&line) == true )
        {
            success = true;
        }
    } while ( 0 );

    return success;
}


bool ThreadSearchLoggerList::IsLineResultLine(long index /* -1 */)
{
    bool isResultLine = false;
    wxListItem listItem;

    do {
        if ( index == -1 )
            // Finds selected item index
            index = m_pListLog->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

        if ( index == -1 ) break;

        // First, gets file dir
        wxString filedir;
        listItem.m_itemId = index;
        listItem.m_col    = 0;
        listItem.m_mask   = wxLIST_MASK_TEXT;

        if ( m_pListLog->GetItem(listItem) == false ) break;

        filedir = listItem.GetText();
        isResultLine = !filedir.StartsWith(_("=>"));
    } while ( 0 );

    return isResultLine;
}


void ThreadSearchLoggerList::OnThreadSearchEvent(const ThreadSearchEvent& event)
{
    // A search event has been sent by the worker thread.
    // List log upddate
    const wxArrayString& words  = event.GetLineTextArray();
    const wxFileName&    filename(event.GetString());
    bool                 setFocus(false);

    wxASSERT((words.GetCount() % 2) == 0);

    // Use of Freeze Thaw to enhance speed and limit blink effect
    m_pListLog->Freeze();
    long index = m_IndexManager.GetInsertionIndex(filename.GetFullPath(), words.GetCount()/2);
    index += m_IndexOffset;
    for (size_t i = 0; i < words.GetCount(); i += 2)
    {
        m_pListLog->InsertItem(index, filename.GetPath());     // Directory
        m_pListLog->SetItem(index, 1, filename.GetFullName()); // File name
        m_pListLog->SetItem(index, 2, words[i]);               // Line index starting from 1
        m_pListLog->SetItem(index, 3, words[i+1]);             // File line matching search expression
        m_pListLog->SetItemData(index, 0);

        // We update preview log for first list item
        if ( m_pListLog->GetItemCount() == 1 )
        {
            // Gets line index
            long line = 0;
            if ( words[i].ToLong(&line) == false )
            {
                cbMessageBox(_("Failed to convert line number from %s") + words[i], _("Error"), wxICON_ERROR);
            }
            else
            {
                m_ThreadSearchView.UpdatePreview(filename.GetFullPath(), line);

                // It is useful to give focus to list to navigate in results
                // just after running a search
                setFocus = true;
            }
        }
        index++;
    }
    m_pListLog->Thaw();

    if ( setFocus == true )
    {
        // On Linux, executing SetFocus just after UpdatePreview(0)
        // does not work. Probbly because of Thaw...
        m_pListLog->SetFocus();
    }
}


wxWindow* ThreadSearchLoggerList::GetWindow()
{
    return m_pListLog;
}


void ThreadSearchLoggerList::SetFocus()
{
    m_pListLog->SetFocus();
}


void ThreadSearchLoggerList::ConnectEvents(wxEvtHandler* pEvtHandler)
{
    // Dynamic event connections.
    int id = m_pListLog->GetId();
    pEvtHandler->Connect(id, wxEVT_COMMAND_LIST_ITEM_SELECTED,
                        (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)
                        &ThreadSearchLoggerList::OnLoggerListClick, NULL, static_cast<wxEvtHandler*>(this));

    pEvtHandler->Connect(id, wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                        (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)
                        &ThreadSearchLoggerList::OnLoggerListDoubleClick, NULL, static_cast<wxEvtHandler*>(this));

    pEvtHandler->Connect(id, wxEVT_COMMAND_LIST_COL_CLICK,
                         (wxObjectEventFunction) (wxEventFunction) (wxListEventFunction)
                         &ThreadSearchLoggerList::OnColumnClick, NULL, static_cast<wxEvtHandler*>(this));

#if wxUSE_MENUS
    pEvtHandler->Connect(id, wxEVT_CONTEXT_MENU,
            (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)
            &ThreadSearchLoggerList::OnLoggerListContextualMenu, NULL, this);

    pEvtHandler->Connect(idMenuCtxDeleteItem, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(ThreadSearchLoggerList::OnDeleteListItem), NULL, this);

    pEvtHandler->Connect(idMenuCtxDeleteAllItems, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(ThreadSearchLoggerList::OnDeleteAllListItems), NULL, this);
#endif // wxUSE_MENUS
}


void ThreadSearchLoggerList::DisconnectEvents(wxEvtHandler* pEvtHandler)
{
    // Dynamic event Disconnections.
    int id = m_pListLog->GetId();
    pEvtHandler->Disconnect(id, wxEVT_COMMAND_LIST_ITEM_SELECTED,
            (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)
            &ThreadSearchLoggerList::OnLoggerListClick, NULL, static_cast<wxEvtHandler*>(this));

    pEvtHandler->Disconnect(id, wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
            (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)
            &ThreadSearchLoggerList::OnLoggerListDoubleClick, NULL, static_cast<wxEvtHandler*>(this));

    pEvtHandler->Disconnect(id, wxEVT_COMMAND_LIST_COL_CLICK,
                            (wxObjectEventFunction) (wxEventFunction) (wxListEventFunction)
                            &ThreadSearchLoggerList::OnColumnClick, NULL, static_cast<wxEvtHandler*>(this));

#if wxUSE_MENUS
    pEvtHandler->Disconnect(id, wxEVT_CONTEXT_MENU,
            (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)
            &ThreadSearchLoggerList::OnLoggerListContextualMenu, NULL, this);

    pEvtHandler->Disconnect(idMenuCtxDeleteItem, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(ThreadSearchLoggerList::OnDeleteListItem), NULL, this);

    pEvtHandler->Disconnect(idMenuCtxDeleteAllItems, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(ThreadSearchLoggerList::OnDeleteAllListItems), NULL, this);
#endif // wxUSE_MENUS
}


void ThreadSearchLoggerList::OnLoggerListContextualMenu(wxContextMenuEvent& event)
{
    wxPoint point = event.GetPosition();

    // If from keyboard
    if ( (point.x == -1) && (point.y == -1) )
    {
        wxSize size = m_pListLog->GetSize();
        point.x = size.x / 2;
        point.y = size.y / 2;
    }
    else
    {
        point = m_pListLog->ScreenToClient(point);
        long tmp;
        int flags;
        if ( m_pListLog->HitTest(point, flags, &tmp) == wxNOT_FOUND )
        {
            return;
        }
    }
    ShowMenu(point);
}


void ThreadSearchLoggerList::OnDeleteListItem(wxCommandEvent& event)
{
    // Finds selected item index
    long index = m_pListLog->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if ( index != -1 )
    {
        long start = index;
        long end = index;
        if ( IsLineResultLine(index) )
        {
            if ( (index > 0) && !IsLineResultLine(index - 1) &&
                 ((index == m_pListLog->GetItemCount() - 1) || !IsLineResultLine(index + 1)) )
            {
                start--;
            }
        }
        else
        {
            index++;
            while ( (index < m_pListLog->GetItemCount()) && IsLineResultLine(index) )
            {
                end = index;
                index++;
            }
        }

        for (int i = end; i >= start; i--)
        {
            DeleteListItem(i);
        }
    }
}


void ThreadSearchLoggerList::OnDeleteAllListItems(wxCommandEvent& event)
{
    DeleteListItems();
}


void ThreadSearchLoggerList::DeleteListItem(long index)
{
    m_pListLog->DeleteItem(index);
}


void ThreadSearchLoggerList::DeleteListItems()
{
    Clear();
}


void ThreadSearchLoggerList::Clear()
{
    m_pListLog->DeleteAllItems();
    m_IndexManager.Reset();
    m_IndexOffset = 0;
}


void ThreadSearchLoggerList::OnSearchBegin(const ThreadSearchFindData& findData)
{
    if ( m_ThreadSearchPlugin.GetDeletePreviousResults() )
    {
        Clear();
        m_IndexOffset = 0;
    }
    else
    {
        m_IndexManager.Reset();
        long index = m_pListLog->GetItemCount();
        m_pListLog->InsertItem(index, wxString::Format(_("=> %s"), findData.GetFindText().c_str()));
        m_pListLog->SetItem(index, 1, _("========="));
        m_pListLog->SetItem(index, 2, _("==="));
        m_pListLog->SetItem(index, 3, _("============"));
        m_pListLog->SetItemData(index, 1);
        m_IndexOffset = m_pListLog->GetItemCount();
    }

    m_SortColumn = -1;
    m_Ascending = true;
}

int Compare(long a, long b)
{
    return (a < b ? -1 : (a > b ? 1 : 0));
}

struct ItemLine
{
    long line;
    long searchIndex;
    bool searchHeader;
};

int wxCALLBACK SortLineAscending(long item1, long item2, long data)
{
    ItemLine const &i1 = *reinterpret_cast<ItemLine const *>(item1);
    ItemLine const &i2 = *reinterpret_cast<ItemLine const *>(item2);

    int c = Compare(i1.searchIndex, i2.searchIndex);
    if (c)
        return c;
    if (i1.searchHeader != i2.searchHeader)
        return i1.searchHeader ? -1 : 1;

    return Compare(i1.line, i2.line);
}

int wxCALLBACK SortLineDescending(long item1, long item2, long data)
{
    ItemLine const &i1 = *reinterpret_cast<ItemLine const *>(item1);
    ItemLine const &i2 = *reinterpret_cast<ItemLine const *>(item2);

    int c = Compare(i1.searchIndex, i2.searchIndex);
    if (c)
        return c;
    if (i1.searchHeader != i2.searchHeader)
        return i1.searchHeader ? -1 : 1;

    return Compare(i2.line, i1.line);
}


struct Item
{
    wxString directory, filename;
    long line;
    long searchIndex;
    bool searchHeader;

    int CompareDirectory(Item const &item) const
    {
        int c = directory.CompareTo(item.directory);
        if (c)
            return c;
        c = filename.CompareTo(item.filename);

        if (c)
            return c;
        return Compare(line, item.line);
    }

    int CompareFile(Item const &item) const
    {
        int c = filename.CompareTo(item.filename);
        if (c)
            return c;
        return Compare(line, item.line);
    }
};

int wxCALLBACK SortDirectoryAscending(long item1, long item2, long data)
{
    Item const &i1 = *reinterpret_cast<Item const *>(item1);
    Item const &i2 = *reinterpret_cast<Item const *>(item2);

    int c = Compare(i1.searchIndex, i2.searchIndex);
    if (c)
        return c;
    if (i1.searchHeader != i2.searchHeader)
        return i1.searchHeader ? -1 : 1;

    return i1.CompareDirectory(i2);
}

int wxCALLBACK SortDirectoryDescending(long item1, long item2, long data)
{
    Item const &i1 = *reinterpret_cast<Item const *>(item1);
    Item const &i2 = *reinterpret_cast<Item const *>(item2);

    int c = Compare(i1.searchIndex, i2.searchIndex);
    if (c)
        return c;
    if (i1.searchHeader != i2.searchHeader)
        return i1.searchHeader ? -1 : 1;

    return i2.CompareDirectory(i1);
}

int wxCALLBACK SortFilenameAscending(long item1, long item2, long data)
{
    Item const &i1 = *reinterpret_cast<Item const *>(item1);
    Item const &i2 = *reinterpret_cast<Item const *>(item2);

    int c = Compare(i1.searchIndex, i2.searchIndex);
    if (c)
        return c;
    if (i1.searchHeader != i2.searchHeader)
        return i1.searchHeader ? -1 : 1;

    return i1.CompareFile(i2);
}

int wxCALLBACK SortFilenameDescending(long item1, long item2, long data)
{
    Item const &i1 = *reinterpret_cast<Item const *>(item1);
    Item const &i2 = *reinterpret_cast<Item const *>(item2);

    int c = Compare(i1.searchIndex, i2.searchIndex);
    if (c)
        return c;
    if (i1.searchHeader != i2.searchHeader)
        return i1.searchHeader ? -1 : 1;

    return i2.CompareFile(i1);
}


struct ItemText
{
    wxString text;
    long searchIndex;
    bool searchHeader;
};

int wxCALLBACK SortTextAscending(long item1, long item2, long data)
{
    ItemText const &i1 = *reinterpret_cast<ItemText const *>(item1);
    ItemText const &i2 = *reinterpret_cast<ItemText const *>(item2);

    int c = Compare(i1.searchIndex, i2.searchIndex);
    if (c)
        return c;
    if (i1.searchHeader != i2.searchHeader)
        return i1.searchHeader ? -1 : 1;

    return i1.text.CompareTo(i2.text);
}

int wxCALLBACK SortTextDescending(long item1, long item2, long data)
{
    ItemText const &i1 = *reinterpret_cast<ItemText const *>(item1);
    ItemText const &i2 = *reinterpret_cast<ItemText const *>(item2);

    int c = Compare(i1.searchIndex, i2.searchIndex);
    if (c)
        return c;
    if (i1.searchHeader != i2.searchHeader)
        return i1.searchHeader ? -1 : 1;

    return i2.text.CompareTo(i1.text);
}

void ThreadSearchLoggerList::OnColumnClick(wxListEvent& event)
{
    int column = event.GetColumn();
    int count = m_pListLog->GetItemCount();
    if (column < 0 || count == 0)
        return;

    if (column != m_SortColumn)
    {
        m_SortColumn = column;
        m_Ascending = true;
    }
    else
        m_Ascending = !m_Ascending;

    switch (column)
    {
        case 2:
            {
                long searchIndex = -1;
                ItemLine *items = new ItemLine[count];

                for (int ii = 0; ii < count; ++ii)
                {
                    wxListItem item;
                    item.SetId(ii);
                    item.SetMask(wxLIST_MASK_TEXT);
                    item.SetColumn(2);
                    m_pListLog->GetItem(item);

                    wxString const &str_line = item.GetText();

                    long line;
                    if (str_line.ToLong(&line))
                        items[ii].line = line;
                    else
                        items[ii].line = -1;

                    if (m_pListLog->GetItemData(ii) == 1)
                    {
                        searchIndex++;
                        items[ii].searchHeader = true;
                    }
                    else
                        items[ii].searchHeader = false;
                    items[ii].searchIndex = searchIndex;

                    m_pListLog->SetItemPtrData(ii, reinterpret_cast<wxUIntPtr>(items + ii));
                }
                m_pListLog->SortItems(m_Ascending ? SortLineAscending : SortLineDescending, 0);

                for (int ii = 0; ii < count; ++ii)
                {
                    m_pListLog->SetItemData(ii, items[ii].searchHeader ? 1 : 0);
                }

                delete [] items;
            }

            break;
        case 0:
        case 1:
            {
                long searchIndex = -1;
                Item *items = new Item[count];

                for (int ii = 0; ii < count; ++ii)
                {
                    wxListItem item;
                    item.SetId(ii);
                    item.SetMask(wxLIST_MASK_TEXT);
                    item.SetColumn(0);
                    m_pListLog->GetItem(item);

                    items[ii].directory = item.GetText();

                    item.SetColumn(1);
                    m_pListLog->GetItem(item);
                    items[ii].filename = item.GetText();

                    item.SetColumn(2);
                    m_pListLog->GetItem(item);

                    items[ii].line = -1;
                    item.GetText().ToLong(&items[ii].line);

                    if (m_pListLog->GetItemData(ii) == 1)
                    {
                        searchIndex++;
                        items[ii].searchHeader = true;
                    }
                    else
                        items[ii].searchHeader = false;
                    items[ii].searchIndex = searchIndex;

                    m_pListLog->SetItemPtrData(ii, reinterpret_cast<wxUIntPtr>(items + ii));
                }

                if (column == 0)
                    m_pListLog->SortItems(m_Ascending ? SortDirectoryAscending : SortDirectoryDescending, 0);
                else
                    m_pListLog->SortItems(m_Ascending ? SortFilenameAscending : SortFilenameDescending, 0);

                for (int ii = 0; ii < count; ++ii)
                {
                    m_pListLog->SetItemData(ii, items[ii].searchHeader ? 1 : 0);
                }

                delete [] items;
            }
            break;
        case 3:
            {
                long searchIndex = -1;
                ItemText *items = new ItemText[count];

                for (int ii = 0; ii < count; ++ii)
                {
                    wxListItem item;
                    item.SetId(ii);
                    item.SetMask(wxLIST_MASK_TEXT);
                    item.SetColumn(3);
                    m_pListLog->GetItem(item);

                    items[ii].text = item.GetText();
                    if (m_pListLog->GetItemData(ii) == 1)
                    {
                        searchIndex++;
                        items[ii].searchHeader = true;
                    }
                    else
                        items[ii].searchHeader = false;
                    items[ii].searchIndex = searchIndex;

                    m_pListLog->SetItemPtrData(ii, reinterpret_cast<wxUIntPtr>(items + ii));
                }

                m_pListLog->SortItems(m_Ascending ? SortTextAscending : SortTextDescending, 0);

                for (int ii = 0; ii < count; ++ii)
                {
                    m_pListLog->SetItemData(ii, items[ii].searchHeader ? 1 : 0);
                }

                delete [] items;
            }
            break;
    }
}
