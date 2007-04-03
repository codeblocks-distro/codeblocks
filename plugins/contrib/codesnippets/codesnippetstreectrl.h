/*
	This file is part of Code Snippets, a plugin for Code::Blocks
	Copyright (C) 2006 Arto Jonsson
	Copyright (C) 2007 Pecan Heber

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
// RCS-ID: $Id: codesnippetstreectrl.h 30 2007-04-01 20:17:29Z Pecan $

#ifndef CODESNIPPETSTREECTRL_H
#define CODESNIPPETSTREECTRL_H

class TiXmlNode;
class TiXmlElement;

#include <wx/treectrl.h>
#include "snippetitemdata.h"
#include <tinyxml/tinyxml.h>
#include "snippetproperty.h"

#if defined(__WXGTK__)
    #include <X11/Xlibint.h>
    #include <X11/keysymdef.h>
    #include <X11/keysym.h>
    #include <X11/extensions/XTest.h>
    #undef Absolute //wx Layout.h and STC conflicts
#endif


// ----------------------------------------------------------------------------
class CodeSnippetsTreeCtrl : public wxTreeCtrl
// ----------------------------------------------------------------------------
{
	public:
		CodeSnippetsTreeCtrl() { }
		CodeSnippetsTreeCtrl(wxWindow *parent, const wxWindowID id, const wxPoint& pos, const wxSize& size, long style);
		~CodeSnippetsTreeCtrl();

		int         OnCompareItems(const wxTreeItemId& item1, const wxTreeItemId& item2);
		void        SaveItemsToFile(const wxString& fileName);
		bool        LoadItemsFromFile(const wxString& fileName, bool bAppendItems);
		void        LoadItemsFromXmlNode(const TiXmlElement* node, const wxTreeItemId& parentID);
		void        SaveItemsToXmlNode(TiXmlNode* node, const wxTreeItemId& parentID);
		void        AddCodeSnippet(const wxTreeItemId& parent, wxString title, wxString codeSnippet, bool editNow);
		wxTreeItemId AddCategory(const wxTreeItemId& parent, wxString title, bool editNow);
        bool        RemoveItem(wxTreeItemId itemId);

        bool        SetFileChanged(bool truefalse) { return m_fileChanged = truefalse; }
        bool        GetFileChanged() { return m_fileChanged; }
        void        SaveFileModificationTime(wxDateTime savedTime = time_t(0));
        wxDateTime  GetSavedFileModificationTime(){ return m_LastXmlModifiedTime;}
        wxTreeItemId ConvertSnippetToCategory(wxTreeItemId itemId);
        void        OnItemSelected(wxTreeEvent& event);
        void        EditSnippetAsFileLink();
        void        SaveSnippetAsFileLink();
        void        EditSnippetAsText();
        CodeSnippetsTreeCtrl* GetSnippetsTreeCtrl(){return m_pSnippetsTreeCtrl;}

        wxString GetSnippet()
            {
                wxString itemData = wxEmptyString;
                wxTreeItemId itemID = GetSelection();
                if (not itemID.IsOk()) return itemData;
                SnippetItemData* pItem = (SnippetItemData*)(GetItemData(itemID));
                itemData = pItem->GetSnippet();
                return itemData;
            }
        wxString GetSnippet( wxTreeItemId itemId )
            {
                wxString itemData = wxEmptyString;
                if (not itemId.IsOk()) return itemData;
                SnippetItemData* pItem = (SnippetItemData*)(GetItemData(itemId));
                itemData = pItem->GetSnippet();
                return itemData;
            }
        wxString GetSnippetLabel(wxTreeItemId treeItemId=(void*)0)
            {
                wxTreeItemId itemId = treeItemId;
                if (not itemId.IsOk()) itemId = GetSelection();
                if (not itemId.IsOk()) return wxEmptyString;
                return GetItemText(itemId);
            }

        void SetSnippet( wxString text )
            {
                wxTreeItemId itemID = GetSelection();
                if (not itemID.IsOk()) return;
                SnippetItemData* pItem = (SnippetItemData*)(GetItemData(itemID));
                pItem->SetSnippet( text);
                SetFileChanged(true);
                return;
            }

        bool IsSnippet(wxTreeItemId treeItemId = (void*)0)
            {
                wxTreeItemId itemId = treeItemId;
                if (not itemId.IsOk()) itemId = GetSelection();
                if (not itemId.IsOk()) return false;
                SnippetItemData* pItem = (SnippetItemData*)(GetItemData(itemId));
                return pItem->IsSnippet();
            }
        bool IsFileSnippet (wxTreeItemId treeItemId = (void*)0 )
            {
                wxTreeItemId itemId = treeItemId;
                if (not itemId.IsOk()) itemId = GetSelection();
                if (not itemId.IsOk()) return false;
                if (not IsSnippet(itemId) ) return false;
                if ( not ::wxFileExists( GetSnippet(itemId))) return false;
                return true;
            }
        void            SetSnippetImage(wxTreeItemId itemId);
        wxTreeItemId    GetAssociatedItemID(){return m_MnuAssociatedItemID;}
        void            SetAssociatedItemID(wxTreeItemId id){m_MnuAssociatedItemID = id;}
        bool            EditSnippetProperties(wxTreeItemId& itemId);
        int             ExecuteDialog(wxDialog* pdlg, wxSemaphore& waitSem);


	private:

	    bool                    m_fileChanged;
   		wxDateTime              m_LastXmlModifiedTime;
		wxTreeCtrl*             m_pEvtTreeCtrlBeginDrag;
        wxTreeItemId            m_TreeItemId;
        wxPoint                 m_TreeMousePosn;
        wxString                m_TreeText;
        bool                    m_MouseCtrlKeyDown;
   		wxTreeItemId            m_MnuAssociatedItemID;
   		bool                    m_bMouseLeftWindow;
        wxDialog*               m_pTopDialog;
   		CodeSnippetsTreeCtrl*   m_pSnippetsTreeCtrl;

       #if defined(__WXMSW__)
        void MSW_MouseMove(int x, int y );
       #endif
       #if defined(__WXGTK__) && defined(HAVEX11DEV)
        // XDisplay pointer
        Display* m_pXDisplay;
       #endif


        TiXmlDocument*  CopyTreeNodeToXmlDoc(wxTreeItemId TreeItemId = (void*)0 );
        void            CopySnippetsToXmlDoc(TiXmlNode* Node, const wxTreeItemId& itemID);
        void            EditSnippet(SnippetItemData* pSnippetItemData, wxString fileName=wxEmptyString);


        void OnBeginTreeItemDrag(wxTreeEvent& event);
        void OnEndTreeItemDrag(wxTreeEvent& event);
   		void OnLeaveWindow(wxMouseEvent& event);
        void OnMouseEvent(wxMouseEvent& event);
        void OnShutdown(wxCloseEvent& event);




		// Must use this so overridden OnCompareItems() works on MSW,
		// see wxWidgets Samples -> TreeCtrl sample
		//
		// Yes, again, ugly way to solve wxWidgets' weirdness
		DECLARE_DYNAMIC_CLASS(CodeSnippetsTreeCtrl)
        DECLARE_EVENT_TABLE()
};



#endif // CODESNIPPETSTREECTRL_H
