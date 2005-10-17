/***************************************************************
 * Name:      exporter.h
 * Purpose:   Code::Blocks plugin
 * Author:    Ceniza<ceniza@gda.utp.edu.co>
 * Copyright: (c) Ceniza
 * License:   GPL
 **************************************************************/

#ifndef EXPORTER_H
#define EXPORTER_H

#if defined(__GNUG__) && !defined(__APPLE__)
	#pragma interface "exporter.h"
#endif
// For compilers that support precompilation, includes <wx/wx.h>
#include <wx/wxprec.h>

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <cbplugin.h> // the base class we 're inheriting
#include <settings.h> // needed to use the Code::Blocks SDK

class Exporter : public cbPlugin
{
	public:
		Exporter();
		~Exporter();
    void BuildMenu(wxMenuBar *menuBar);
    void RemoveMenu(wxMenuBar *menuBar);
		int Configure() { return 0; }
		void OnAttach(); // fires when the plugin is attached to the application
		void OnRelease(bool appShutDown); // fires when the plugin is released from the application
    void OnExport(wxCommandEvent &event);
    void OnUpdateUI(wxUpdateUIEvent &event);
  private:
    void BuildModuleMenu(const ModuleType type, wxMenu *menu, const wxString &arg) {}
    bool BuildToolBar(wxToolBar *toolBar) { return false; }
    void RemoveToolBar(wxToolBar *toolBar) {}

    DECLARE_EVENT_TABLE();
};

// Declare the plugin's hooks
CB_DECLARE_PLUGIN();

#endif // EXPORTER_H
