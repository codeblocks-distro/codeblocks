/***************************************************************
 * Name:      wxsmith.h
 * Purpose:   Code::Blocks plugin
 * Author:    BYO<byo@o2.pl>
 * Created:   04/10/05 01:05:08
 * Copyright: (c) BYO
 * License:   GPL
 **************************************************************/

#ifndef WXSMITH_H
#define WXSMITH_H

#if defined(__GNUG__) && !defined(__APPLE__)
	#pragma interface "wxsmith.h"
#endif
// For compilers that support precompilation, includes <wx/wx.h>
#include <wx/wxprec.h>

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/splitter.h>
#include <wx/scrolwin.h>

#include <cbplugin.h>
#include <settings.h>
#include <sdk_events.h>

#include <map>

#include "wxsProject.h"

class wxsProject;

class wxSmith : public cbPlugin
{
	public:
		wxSmith();
		~wxSmith();
		int Configure();
		void BuildMenu(wxMenuBar* menuBar);
		void BuildModuleMenu(const ModuleType type, wxMenu* menu, const wxString& arg);
		void BuildToolBar(wxToolBar* toolBar);
		void OnAttach(); // fires when the plugin is attached to the application
		void OnRelease(bool appShutDown); // fires when the plugin is released from the application

        /* Function used while selecting resource in resource browser */
        void OnSelectResource(wxsResourceTreeData* Data);
        
        /* Getting current resourcec tree */
        wxTreeCtrl* GetResourceTree() { return ResourceBrowser; }
        
	protected:
	
	private:
        wxSplitterWindow* LeftSplitter;
        wxTreeCtrl* ResourceBrowser;
        wxScrolledWindow* PropertiesPanel;
        wxScrolledWindow* EventsPanel;
        wxPanel* Palette;

        /* Here's bridge between current C::B project and wxSmith projects */
        
        typedef std::map<cbProject*,wxsProject*> ProjectMapT;
        typedef ProjectMapT::iterator ProjectMapI;
        
        ProjectMapT ProjectMap;
        
        wxsProject* GetSmithProject(cbProject* Proj);
        cbProject* GetCBProject(wxsProject* Proj);
        
        /* Event processing functions */
        
        void OnProjectClose(CodeBlocksEvent& event);
        void OnProjectOpen(CodeBlocksEvent& event);
        void OnProjectActivated(CodeBlocksEvent& event);
        
        /* Selecting given objects */
        void OnSelectWidget(wxsResourceTreeData* Data);
        void OnSelectDialog(wxsResourceTreeData* Data);
                
		DECLARE_EVENT_TABLE()
};

#ifdef __cplusplus
extern "C" {
#endif
	PLUGIN_EXPORT cbPlugin* GetPlugin();
#ifdef __cplusplus
};
#endif

#endif // WXSMITH_H

