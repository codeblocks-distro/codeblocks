/*
	This file is part of Code Snippets, a plugin for Code::Blocks
	Copyright (C) 2006 Arto Jonsson

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

#include "sdk.h"
#ifndef CB_PRECOMB
	#include <wx/event.h>
	#include <wx/frame.h> // Manager::Get()->GetAppWindow()
	#include <wx/intl.h>
	#include <wx/menu.h>
	#include <wx/menuitem.h>
	#include <wx/string.h>
	#include "licenses.h"
	#include "manager.h"
	#include "sdk_events.h"
#endif

#include "codesnippets.h"
#include "codesnippetswindow.h"

// Implement the plugin's hooks
CB_IMPLEMENT_PLUGIN(CodeSnippets, "CodeSnippets");

int idViewSnippets = wxNewId();

// Events handling
BEGIN_EVENT_TABLE(CodeSnippets, cbPlugin)
	EVT_UPDATE_UI(idViewSnippets, CodeSnippets::OnUpdateUI)
	EVT_MENU(idViewSnippets, CodeSnippets::OnViewSnippets)
END_EVENT_TABLE()

CodeSnippets::CodeSnippets()
{
	m_PluginInfo.name = _T("CodeSnippets");
	m_PluginInfo.title = _("Code snippets");
	m_PluginInfo.version = _T("0.2");
	m_PluginInfo.description =
					_T("Code Snippets allows you to create and save ")
					_T("small pieces of code (snippets) for later use.\n")
					_T("\n")
					_T("For more information, see readme.html.");
	m_PluginInfo.author = _T("Arto Jonsson");
	m_PluginInfo.authorEmail = _T("arto.jonsson@gmail.com");
	m_PluginInfo.authorWebsite = _T("http://ajonsson.kapsi.fi/");
	m_PluginInfo.thanksTo =
					_T("Technical help\n\n")
					_T("\twxWidgets Discussion Forum\n")
					_T("\t(http://wxforum.shadonet.com/)\n\n")
					_T("Special thanks\n\n")
					_T("\tCode::Blocks developers\n")
					_T("\twxWidgets developers\n")
					_T("\tTinyXML developers\n");
	m_PluginInfo.license = LICENSE_GPL;
}

CodeSnippets::~CodeSnippets()
{
	// dtor
}

void CodeSnippets::OnAttach()
{
	m_SnippetsWindow = new CodeSnippetsWindow();

	CodeBlocksDockEvent evt(cbEVT_ADD_DOCK_WINDOW);
	evt.name = _T("CodeSnippetsPane");
	evt.title = _("Code snippets");
	evt.pWindow = m_SnippetsWindow;
	evt.dockSide = CodeBlocksDockEvent::dsFloating;
	evt.desiredSize.Set(400, 150);
	evt.floatingSize.Set(400, 150);
	evt.minimumSize.Set(1, 1);
	Manager::Get()->GetAppWindow()->ProcessEvent(evt);
}

void CodeSnippets::OnRelease(bool appShutDown)
{
	CodeBlocksDockEvent evt(cbEVT_REMOVE_DOCK_WINDOW);
	evt.pWindow = m_SnippetsWindow;
	Manager::Get()->GetAppWindow()->ProcessEvent(evt);
	m_SnippetsWindow->Destroy();
}

int CodeSnippets::Configure()
{
	//create and display the configuration dialog for your plugin
	/*cbConfigurationDialog dlg(Manager::Get()->GetAppWindow(), wxID_ANY, _("Your dialog title"));
	cbConfigurationPanel* panel = GetConfigurationPanel(&dlg);
	if (panel)
	{
		dlg.AttachConfigurationPanel(panel);
		PlaceWindow(&dlg);
		return dlg.ShowModal() == wxID_OK ? 0 : -1;
	}*/
	return 0;
}

void CodeSnippets::BuildMenu(wxMenuBar* menuBar)
{
	int idx = menuBar->FindMenu(_("View"));
	if (idx != wxNOT_FOUND)
	{
		wxMenu* view = menuBar->GetMenu(idx);
		wxMenuItemList& items = view->GetMenuItems();
		// Find the first separator and insert before it
		for (size_t i = 0; i < items.GetCount(); ++i)
		{
			if (items[i]->IsSeparator())
			{
				view->InsertCheckItem(i, idViewSnippets, _("Code snippets"), _("Toggle displaying the code snippets."));
				return;
			}
		}
		// Not found, just append
		view->AppendCheckItem(idViewSnippets, _("Code snippets"), _("Toggle displaying the code snippets."));
	}
}

void CodeSnippets::OnViewSnippets(wxCommandEvent& event)
{
	CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
	evt.pWindow = m_SnippetsWindow;
	Manager::Get()->GetAppWindow()->ProcessEvent(evt);
}

void CodeSnippets::OnUpdateUI(wxUpdateUIEvent& /*event*/)
{
	// Check if the Code Snippets window is visible, if it's not, uncheck the menu item
	Manager::Get()->GetAppWindow()->GetMenuBar()->Check(idViewSnippets, IsWindowReallyShown(m_SnippetsWindow));
}
