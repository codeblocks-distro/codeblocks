/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact e-mail: Yiannis An. Mandravellos <mandrav@codeblocks.org>
* Program URL   : http://www.codeblocks.org
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include <sdk.h>
#include "classbrowser.h" // class's header file
#include "nativeparser.h"
#include <wx/intl.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/menu.h>
#include <wx/button.h>
#include <wx/xrc/xmlres.h>
#include <manager.h>
#include <configmanager.h>
#include <pluginmanager.h>
#include <editormanager.h>
#include <projectmanager.h>
#include <cbeditor.h>
#include <globals.h>

class myTextCtrl : public wxTextCtrl
{
    public:
        myTextCtrl(ClassBrowser* cb,
                    wxWindow* parent,
                    wxWindowID id,
                    const wxString& value = wxEmptyString,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = wxTE_PROCESS_ENTER,
                    const wxValidator& validator = wxDefaultValidator,
                    const wxString& name = wxTextCtrlNameStr)
            : wxTextCtrl(parent, id, value, pos, size, style, validator, name),
            m_CB(cb)
            {}
        virtual ~myTextCtrl() {}
    protected:
        ClassBrowser* m_CB;

        // Intercept key presses to handle Enter. */
        void OnKey(wxKeyEvent& event)
        {
            if (event.GetKeyCode() == WXK_RETURN)
            {
                wxCommandEvent e;
                m_CB->OnSearch(e);
            }
            else
                event.Skip();
        }
        DECLARE_EVENT_TABLE()
};
BEGIN_EVENT_TABLE(myTextCtrl, wxTextCtrl)
	EVT_KEY_DOWN(myTextCtrl::OnKey)
END_EVENT_TABLE()

int idMenuJumpToDeclaration = wxNewId();
int idMenuJumpToImplementation = wxNewId();
int idMenuRefreshTree = wxNewId();
int idCBViewInheritance = wxNewId();
int idCBViewModeFlat = wxNewId();
int idCBViewModeStructured = wxNewId();
int idMenuForceReparse = wxNewId();

BEGIN_EVENT_TABLE(ClassBrowser, wxPanel)
	EVT_TREE_ITEM_ACTIVATED(XRCID("treeAll"), ClassBrowser::OnTreeItemDoubleClick)
    EVT_TREE_ITEM_RIGHT_CLICK(XRCID("treeAll"), ClassBrowser::OnTreeItemRightClick)

    EVT_MENU(idMenuJumpToDeclaration, ClassBrowser::OnJumpTo)
    EVT_MENU(idMenuJumpToImplementation, ClassBrowser::OnJumpTo)
    EVT_MENU(idMenuRefreshTree, ClassBrowser::OnRefreshTree)
    EVT_MENU(idMenuForceReparse, ClassBrowser::OnForceReparse)
    EVT_MENU(idCBViewInheritance, ClassBrowser::OnCBViewMode)
    EVT_MENU(idCBViewModeFlat, ClassBrowser::OnCBViewMode)
    EVT_MENU(idCBViewModeStructured, ClassBrowser::OnCBViewMode)
    EVT_CHOICE(XRCID("cmbView"), ClassBrowser::OnViewScope)
    EVT_BUTTON(XRCID("btnSearch"), ClassBrowser::OnSearch)
END_EVENT_TABLE()

// class constructor
ClassBrowser::ClassBrowser(wxWindow* parent, NativeParser* np)
    : m_NativeParser(np),
    m_TreeForPopupMenu(0),
	m_pParser(0L)
{
	wxXmlResource::Get()->LoadPanel(this, parent, _T("pnlCB"));
    m_Search = new myTextCtrl(this, parent, XRCID("txtSearch"));
    wxXmlResource::Get()->AttachUnknownControl(_T("txtSearch"), m_Search);

	m_Tree = XRCCTRL(*this, "treeAll", wxTreeCtrl);

    bool all = Manager::Get()->GetConfigManager(_T("code_completion"))->ReadBool(_T("/show_all_symbols"), false);
    XRCCTRL(*this, "cmbView", wxChoice)->SetSelection(all ? 1 : 0);
}

// class destructor
ClassBrowser::~ClassBrowser()
{
    UnlinkParser();
}

void ClassBrowser::SetParser(Parser* parser)
{
	if (parser != m_pParser)
	{
		UnlinkParser();
		if(parser)
		{
            parser->AbortBuildingTree();
            parser->m_pClassBrowser = this;
            m_pParser = parser;
            Update();
		}
	}
}

void ClassBrowser::UnlinkParser()
{
    if(m_pParser)
    {
        if(m_pParser->m_pClassBrowser == this)
        {
            m_pParser->AbortBuildingTree();
            m_pParser->m_pClassBrowser = NULL;
        }
        m_pParser = NULL;
    }
}

void ClassBrowser::Update()
{
	if (m_pParser)
	{
		wxArrayString treeState;
		wxTreeItemId root = m_Tree->GetRootItem();
		if (root.IsOk())
            ::SaveTreeState(m_Tree, root, treeState);
		m_pParser->BuildTree(*m_Tree);
		root = m_Tree->GetRootItem();
		if (root.IsOk())
		{
            ::RestoreTreeState(m_Tree, root, treeState);
            if (!m_Tree->IsExpanded(root))
                m_Tree->Expand(root);
		}
	}
	else
		m_Tree->DeleteAllItems();
}

void ClassBrowser::ShowMenu(wxTreeCtrl* tree, wxTreeItemId id, const wxPoint& pt)
{
// NOTE: local variables are tricky! If you build two local menus
// and attach menu B to menu A, on function exit both menu A and menu B
// will be destroyed. But when destroying menu A, menu B will be destroyed
// again. Its already-freed memory will be accessed, generating a segfault.

// A safer approach is to make all menus heap-based, and delete the topmost
// on exit.

    m_TreeForPopupMenu = tree;
    if ( !id.IsOk() )
        return;

#if wxUSE_MENUS
	wxString caption;
    wxMenu *menu=new wxMenu(wxEmptyString);

	ClassTreeData* ctd = (ClassTreeData*)tree->GetItemData(id);
    if (ctd)
    {
        switch (ctd->GetToken()->m_TokenKind)
        {
            case tkConstructor:
            case tkDestructor:
            case tkFunction:
                if (ctd->GetToken()->m_ImplLine != 0 && !ctd->GetToken()->GetImplFilename().IsEmpty())
                    menu->Append(idMenuJumpToImplementation, _("Jump to &implementation"));
                // intentionally fall through
            default:
                menu->Append(idMenuJumpToDeclaration, _("Jump to &declaration"));
        }
    }

    if (menu->GetMenuItemCount() != 0)
        menu->AppendSeparator();

    wxMenu *sub = new wxMenu(_T(""));
    sub->AppendCheckItem(idCBViewInheritance, _("Show inherited members"));
    sub->AppendSeparator();
    sub->AppendRadioItem(idCBViewModeFlat, _("Flat"));
    sub->AppendRadioItem(idCBViewModeStructured, _("Structured"));

    menu->Append(wxNewId(), _("&View options"), sub);
    menu->Append(idMenuRefreshTree, _("&Refresh tree"));

    if (id == m_Tree->GetRootItem())
    {
        menu->AppendSeparator();
        menu->Append(idMenuForceReparse, _("Re-parse now"));
    }

    menu->Check(idCBViewInheritance, m_pParser ? m_pParser->ClassBrowserOptions().showInheritance : false);
    sub->Check(idCBViewModeFlat, m_pParser ? m_pParser->ClassBrowserOptions().viewFlat : false);
    sub->Check(idCBViewModeStructured, m_pParser ? !m_pParser->ClassBrowserOptions().viewFlat : false);

    PopupMenu(menu, tree->GetParent()->ScreenToClient(tree->ClientToScreen(pt)));
    delete menu; // Prevents memory leak
#endif // wxUSE_MENUS
}

// events

void ClassBrowser::OnTreeItemRightClick(wxTreeEvent& event)
{
    wxTreeCtrl* tree = m_Tree;
	tree->SelectItem(event.GetItem());
    ShowMenu(tree, event.GetItem(), event.GetPoint());// + tree->GetPosition());
}

void ClassBrowser::OnJumpTo(wxCommandEvent& event)
{
    wxTreeCtrl* tree = m_TreeForPopupMenu;
	wxTreeItemId id = tree->GetSelection();
	ClassTreeData* ctd = (ClassTreeData*)tree->GetItemData(id);
    if (ctd)
    {
        cbProject* prj = Manager::Get()->GetProjectManager()->GetActiveProject();
        if (prj)
        {
            wxString base = prj->GetBasePath();
            wxFileName fname;
            if (event.GetId() == idMenuJumpToImplementation)
                fname.Assign(ctd->GetToken()->GetImplFilename());
            else
                fname.Assign(ctd->GetToken()->GetFilename());
            NormalizePath(fname,base);
        	cbEditor* ed = Manager::Get()->GetEditorManager()->Open(fname.GetFullPath());
			if (ed)
			{
                int line;
                if (event.GetId() == idMenuJumpToImplementation)
                    line = ctd->GetToken()->m_ImplLine - 1;
                else
                    line = ctd->GetToken()->m_Line - 1;
				ed->GotoLine(line);
			}
        }
    }
}

void ClassBrowser::OnTreeItemDoubleClick(wxTreeEvent& event)
{
    wxTreeCtrl* tree = (wxTreeCtrl*)event.GetEventObject();
	wxTreeItemId id = event.GetItem();
	ClassTreeData* ctd = (ClassTreeData*)tree->GetItemData(id);
    if (ctd)
    {
        cbProject* prj = Manager::Get()->GetProjectManager()->GetActiveProject();
        if (prj)
        {
			bool toImp = false;
			switch (ctd->GetToken()->m_TokenKind)
			{
			case tkConstructor:
            case tkDestructor:
            case tkFunction:
                if (ctd->GetToken()->m_ImplLine != 0 && !ctd->GetToken()->GetImplFilename().IsEmpty())
                    toImp = true;
				break;
			default:
				break;
			}

            wxString base = prj->GetBasePath();
            wxFileName fname;
            if (toImp)
                fname.Assign(ctd->GetToken()->GetImplFilename());
            else
                fname.Assign(ctd->GetToken()->GetFilename());

            NormalizePath(fname, base);
        	cbEditor* ed = Manager::Get()->GetEditorManager()->Open(fname.GetFullPath());
			if (ed)
			{
				int line;
                if (toImp)
                    line = ctd->GetToken()->m_ImplLine - 1;
                else
                    line = ctd->GetToken()->m_Line - 1;
				ed->GotoLine(line);

				wxFocusEvent ev(wxEVT_SET_FOCUS);
				ev.SetWindow(this);
				ed->GetControl()->AddPendingEvent(ev);
			}
        }
    }
}

void ClassBrowser::OnRefreshTree(wxCommandEvent& event)
{
	Update();
}

void ClassBrowser::OnForceReparse(wxCommandEvent& event)
{
    if (m_NativeParser)
        m_NativeParser->ForceReparseActiveProject();
}

void ClassBrowser::OnCBViewMode(wxCommandEvent& event)
{
	if (!m_pParser)
		return;

	if (event.GetId() == idCBViewInheritance)
		m_pParser->ClassBrowserOptions().showInheritance = event.IsChecked();
	else if (event.GetId() == idCBViewModeFlat)
		m_pParser->ClassBrowserOptions().viewFlat = event.IsChecked();
	else if (event.GetId() == idCBViewModeStructured)
		m_pParser->ClassBrowserOptions().viewFlat = !event.IsChecked();
	else
		return;

	m_pParser->WriteOptions();
	Update();
}

void ClassBrowser::OnViewScope(wxCommandEvent& event)
{
	if (m_pParser)
	{
		m_pParser->ClassBrowserOptions().showAllSymbols = event.GetSelection() == 1;
		m_pParser->WriteOptions();
        Update();
	}
	else
	{
	    // we have no parser; just write the setting in the configuration
        Manager::Get()->GetConfigManager(_T("code_completion"))->Write(_T("/show_all_symbols"), event.GetSelection() == 1);
	}
}

bool ClassBrowser::RecursiveSearch(const wxString& search, wxTreeCtrl* tree, const wxTreeItemId& parent, wxTreeItemId& result)
{
    wxTreeItemIdValue cookie;
	wxTreeItemId child = tree->GetFirstChild(parent, cookie);
	while (child.IsOk())
	{
        ClassTreeData* ctd = static_cast<ClassTreeData*>(tree->GetItemData(child));
        if (ctd && ctd->GetToken())
        {
            Token* token = ctd->GetToken();
            if (token->m_Name.Lower().StartsWith(search))
            {
                result = child;
                return true;
            }
        }
        if (tree->ItemHasChildren(child))
        {
            if (RecursiveSearch(search, tree, child, result))
                return true;
        }
        child = tree->GetNextChild(parent, cookie);
	}
	return false;
}

void ClassBrowser::OnSearch(wxCommandEvent& event)
{
    if (m_Search->GetValue().IsEmpty())
        return;
    wxTreeItemId result;
    if (RecursiveSearch(m_Search->GetValue().Lower(), m_Tree, m_Tree->GetRootItem(), result))
    {
        m_Tree->SelectItem(result);
        m_Tree->EnsureVisible(result);
    }
}
