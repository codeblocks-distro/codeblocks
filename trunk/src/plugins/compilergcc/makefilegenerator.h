#ifndef MAKEFILEGENERATOR_H
#define MAKEFILEGENERATOR_H

#include <settings.h>
#include <cbproject.h>
#include "compilergcc.h"
#include "customvars.h"
#include "compiler.h"

WX_DEFINE_ARRAY(ProjectBuildTarget*, TargetsArray);
WX_DEFINE_ARRAY(ProjectFile*, FilesArray); // keep our own copy, to sort it by file weight (priority)

/*
 * No description
 */
class MakefileGenerator
{
	public:
		// class constructor
		MakefileGenerator(CompilerGCC* compiler, cbProject* project, const wxString& makefile, int logIndex);
		// class destructor
		~MakefileGenerator();
        bool CreateMakefile();
    private:
        void DoAppendCompilerOptions(wxString& cmd, ProjectBuildTarget* target = 0L, bool useGlobalOptions = false);
        void DoAppendLinkerOptions(wxString& cmd, ProjectBuildTarget* target = 0L, bool useGlobalOptions = false);
        void DoAppendIncludeDirs(wxString& cmd, ProjectBuildTarget* target = 0L, const wxString& prefix = "-I", bool useGlobalOptions = false);
        void DoAppendLibDirs(wxString& cmd, ProjectBuildTarget* target = 0L, const wxString& prefix = "-L", bool useGlobalOptions = false);
        void DoAddMakefileVars(wxString& buffer);
#ifdef __WXMSW__
        void DoAddMakefileResources(wxString& buffer);
#endif // __WXMSW__
        void DoAddMakefileObjs(wxString& buffer);
        void DoAddMakefileIncludes(wxString& buffer);
        void DoAddMakefileLibs(wxString& buffer);
        void DoAddMakefileOptions(wxString& buffer);
        void DoAddMakefileCFlags(wxString& buffer);
        void DoAddMakefileLDFlags(wxString& buffer);
        void DoAddMakefileTargets(wxString& buffer);
		void DoAddPhonyTargets(wxString& buffer);
        void DoAddMakefileTarget_All(wxString& buffer);
        void DoAddMakefileTargets_BeforeAfter(wxString& buffer);
		void DoAddMakefileCommands(const wxString& prefix, const wxArrayString& commands, wxString& buffer);
        void DoAddMakefileTarget_Clean(wxString& buffer);
        void DoAddMakefileTarget_Dist(wxString& buffer);
        void DoAddMakefileTarget_Depend(wxString& buffer);
        void DoAddMakefileTarget_Link(wxString& buffer);
        void DoAddMakefileTarget_Objs(wxString& buffer);
        void DoGetMakefileIncludes(wxString& buffer, ProjectBuildTarget* target);
        void DoGetMakefileLibs(wxString& buffer, ProjectBuildTarget* target);
        void DoGetMakefileCFlags(wxString& buffer, ProjectBuildTarget* target);
        void DoGetMakefileLDFlags(wxString& buffer, ProjectBuildTarget* target);

        wxString ReplaceCompilerMacros(CommandType et,
                                    const wxString& compilerVar,
                                    ProjectBuildTarget* target,
                                    const wxString& file,
                                    const wxString& object,
                                    const wxString& deps);
		void DoPrepareFiles();
		void DoPrepareValidTargets();
		bool IsTargetValid(ProjectBuildTarget* target);
        void ReplaceMacros(ProjectFile* pf, wxString& text);

		CompilerGCC* m_Compiler;
		Compiler* m_CompilerSet;
		const CompilerPrograms& m_Programs;
		const CompilerSwitches& m_Switches;
        cbProject* m_Project;
        wxString m_Makefile;
		TargetsArray m_LinkableTargets;
        FilesArray m_Files;
        int m_LogIndex;
		CustomVars m_Vars;
		
		wxString m_Quiet; // used for compiler simple log
};

#endif // MAKEFILEGENERATOR_H
