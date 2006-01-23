#ifndef CODECOMPLETION_H
#define CODECOMPLETION_H

#include <settings.h> // SDK
#include <cbplugin.h>
#include <cbproject.h>
#include <simpletextlog.h>
#include <sdk_events.h>
#include "nativeparser.h"

class CodeCompletion : public cbCodeCompletionPlugin
{
    public:
        CodeCompletion();
        ~CodeCompletion();

        virtual void OnAttach();
        virtual void OnRelease(bool appShutDown);
        cbConfigurationPanel* GetConfigurationPanel(wxWindow* parent);
        virtual int Configure();
        virtual void BuildMenu(wxMenuBar* menuBar); // offer for menu space by host
		virtual void BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data = 0);
		virtual bool BuildToolBar(wxToolBar* toolBar);
		virtual wxArrayString GetCallTips();
		virtual int CodeComplete();
		virtual void ShowCallTip();

        virtual void CodeCompleteIncludes();
	private:
    	void OnUpdateUI(wxUpdateUIEvent& event);
		void OnCodeComplete(wxCommandEvent& event);
		void OnShowCallTip(wxCommandEvent& event);
		void OnGotoFunction(wxCommandEvent& event);
        void OnClassMethod(wxCommandEvent& event);
		void OnGotoDeclaration(wxCommandEvent& event);
		void OnOpenIncludeFile(wxCommandEvent& event);
		void OnAppDoneStartup(CodeBlocksEvent& event);
		void OnStartParsingProjects(wxTimerEvent& event);
		void OnProjectOpened(CodeBlocksEvent& event);
		void OnProjectActivated(CodeBlocksEvent& event);
		void OnProjectClosed(CodeBlocksEvent& event);
		void OnProjectFileAdded(CodeBlocksEvent& event);
		void OnProjectFileRemoved(CodeBlocksEvent& event);
		void OnUserListSelection(CodeBlocksEvent& event);
		void OnReparseActiveEditor(CodeBlocksEvent& event);
		void OnEditorActivated(CodeBlocksEvent& event);
		void DoCodeComplete();
		void DoInsertCodeCompleteToken(wxString tokName);
        int DoClassMethodDeclImpl();
        int m_PageIndex;
        bool m_InitDone;

        wxString m_LastIncludeFileFrom;
        wxString m_LastIncludeFile;
        wxString m_LastKeyword;

    	wxMenu* m_EditMenu;
    	wxMenu* m_SearchMenu;
		NativeParser m_NativeParsers;
		ProjectBuildTarget* m_Target;
		wxTimer m_timer;
        DECLARE_EVENT_TABLE()
};

CB_DECLARE_PLUGIN();

#endif // CODECOMPLETION_H
