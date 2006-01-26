#ifndef COMPILERSETTINGSDLG_H
#define COMPILERSETTINGSDLG_H

#include <wx/dialog.h>
#include <pluginmanager.h>

class wxListbookEvent;

class CompilerSettingsDlg : public wxDialog
{
	public:
		CompilerSettingsDlg(wxWindow* parent);
		virtual ~CompilerSettingsDlg();
		virtual void EndModal(int retCode);
	protected:
        void OnPageChanging(wxListbookEvent& event);
        void OnPageChanged(wxListbookEvent& event);
	private:
        void AddPluginPanels();
        void UpdateListbookImages();
        ConfigurationPanelsArray m_PluginPanels;
        DECLARE_EVENT_TABLE()
};

#endif // COMPILERSETTINGSDLG_H
