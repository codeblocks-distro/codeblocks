#include "processingdlg.h"

#include <wx/dir.h>
#include <stdio.h>
#include <wx/tokenzr.h>
#include "libraryconfigmanager.h"
#include "resultmap.h"

BEGIN_EVENT_TABLE(ProcessingDlg,wxDialog)
	//(*EventTable(ProcessingDlg)
	EVT_BUTTON(ID_BUTTON1,ProcessingDlg::OnButton1Click)
	//*)
END_EVENT_TABLE()

ProcessingDlg::ProcessingDlg(wxWindow* parent,wxWindowID id):
    StopFlag(false)
{
	//(*Initialize(ProcessingDlg)
	Create(parent,id,_T(""),wxDefaultPosition,wxDefaultSize,wxCAPTION);
	FlexGridSizer1 = new wxFlexGridSizer(0,1,0,0);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL,this,_("Processing"));
	Status = new wxStaticText(this,ID_STATICTEXT1,_("Waiting"),wxDefaultPosition,wxDefaultSize,wxST_NO_AUTORESIZE);
	Gauge1 = new wxGauge(this,ID_GAUGE1,100,wxDefaultPosition,wxSize(288,12),0);
	if ( 0 ) Gauge1->SetValue(0);
	if ( 0 ) Gauge1->SetValue(0);
	if ( 0 ) Gauge1->SetValue(0);
	StaticBoxSizer1->Add(Status,0,wxALIGN_CENTER|wxEXPAND,0);
	StaticBoxSizer1->Add(Gauge1,1,wxALIGN_CENTER,5);
	StopBtn = new wxButton(this,ID_BUTTON1,_("Stop"),wxDefaultPosition,wxDefaultSize,0);
	if (false) StopBtn->SetDefault();
	FlexGridSizer1->Add(StaticBoxSizer1,1,wxALL|wxALIGN_CENTER,5);
	FlexGridSizer1->Add(StopBtn,1,wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_CENTER,5);
	this->SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	//*)
}

ProcessingDlg::~ProcessingDlg()
{
}

void ProcessingDlg::OnButton1Click(wxCommandEvent& event)
{
    StopBtn->Disable();
    StopFlag = true;
}

bool ProcessingDlg::ReadDirs(const wxArrayString& Dirs)
{
    wxLogNull LogNull;
    Gauge1->SetRange(Dirs.Count());
    for ( size_t i = 0; i<Dirs.Count(); i++ )
    {
        if ( StopFlag ) return false;
        Gauge1->SetValue(i);
        
        wxString DirName = Dirs[i];
        if ( DirName.empty() ) continue;
        
        // Cutting off last character if it is path separator
        wxChar LastChar = DirName[DirName.Len()-1];
        if ( wxFileName::GetPathSeparators().Find(LastChar) != -1 )
        {
            DirName.RemoveLast();
        }
        
        // Reading dir content
        ReadDir(DirName);
    }
    return !StopFlag;
}

void ProcessingDlg::ReadDir(const wxString& DirName)
{
    wxDir Dir(DirName);

    if ( !Dir.IsOpened() ) return;

    Status->SetLabel(_T("Reading dir: ") + DirName);
    ::wxYield();
    if ( StopFlag ) return;

    wxString Name;

    if ( Dir.GetFirst(&Name,wxEmptyString,wxDIR_FILES|wxDIR_HIDDEN) )
    {
        do
        {
            Map[Name].Add(DirName + wxFileName::GetPathSeparator() + Name);
        }
        while ( Dir.GetNext(&Name) );
    }

    if ( Dir.GetFirst(&Name,wxEmptyString,wxDIR_DIRS|wxDIR_HIDDEN) )
    {
        do
        {
            Map[Name].Add(DirName + wxFileName::GetPathSeparator() + Name);
            ReadDir(DirName + wxFileName::GetPathSeparator() + Name);
        }
        while ( Dir.GetNext(&Name) );
    }
}

bool ProcessingDlg::ProcessLibs()
{
    Gauge1->SetRange(LCM()->GetLibraryCount());

    for ( int i=0; i<LCM()->GetLibraryCount(); ++i )
    {
        if ( StopFlag ) return false;
        ProcessLibrary(LCM()->GetLibrary(i));
    }

    return !StopFlag;
}

void ProcessingDlg::ProcessLibrary(const LibraryConfig* Config)
{
    Status->SetLabel(
        wxString::Format(
            _("Searching variable \"%s\""),
            Config->GlobalVar.c_str()));
    CheckNextFileName(_T(""),wxStringStringMap(),Config,0);
}

void ProcessingDlg::CheckNextFileName(
    const wxString& OldBasePath,
    const wxStringStringMap& OldVars,
    const LibraryConfig* Config,
    int WhichFile)
{
    if ( (int)Config->FileNames.Count() <= WhichFile )
    {
        FoundLibrary(OldBasePath,OldVars,Config);
        return;
    }

    wxArrayString Pattern;
    SplitPath(Config->FileNames[WhichFile],Pattern);

    const wxArrayString& PathArray = Map[Pattern[Pattern.Count()-1]];
    if ( PathArray.empty() ) return;

    for ( size_t i=0; i<PathArray.Count(); i++ )
    {
        wxArrayString Path;
        wxStringStringMap Vars = OldVars;
        SplitPath(PathArray[i],Path);

        int path_index = (int)Path.Count() - 1;
        int pattern_index = (int)Pattern.Count() - 1;

        while ( ( path_index >= 0 ) && ( pattern_index >= 0 ) )
        {
            wxString& PatternPart = Pattern[pattern_index];
            if ( IsVariable(PatternPart) )
            {
                wxString VarName = PatternPart.Mid(3,PatternPart.Len()-4);
                if ( Vars[VarName].empty() )
                {
                    Vars[VarName] = Path[path_index];
                }
                else
                {
                    if ( Vars[VarName] != Path[path_index] ) break;
                }
            }
            else
            {
                if ( PatternPart != Path[path_index] ) break;
            }
            path_index--;
            pattern_index--;
        }

        if ( pattern_index >= 0 ) continue;

        wxString BasePath;
        for ( int j=0; j<=path_index; j++ )
        {
            BasePath += Path[j] + wxFileName::GetPathSeparator();
        }

        if ( WhichFile > 0 )
        {
            if ( BasePath != OldBasePath ) continue;
        }

        // We have here base for out path and set of variables,
        // now have to check the rest of file names

        CheckNextFileName(BasePath,Vars,Config,WhichFile+1);
    }
}

void ProcessingDlg::SplitPath(const wxString& FileName,wxArrayString& Split)
{
    wxStringTokenizer Tknz(FileName,_T("\\/"));
    while (Tknz.HasMoreTokens()) Split.Add(Tknz.GetNextToken());
}

bool ProcessingDlg::IsVariable(const wxString& NamePart)
{
    if ( NamePart.Len() < 5 ) return false;
    if ( NamePart[0] != _T('*') ) return false;
    if ( NamePart[1] != _T('$') ) return false;
    if ( NamePart[2] != _T('(') ) return false;
    if ( NamePart[NamePart.Len()-1] != _T(')') ) return false;
    return true;
}

void ProcessingDlg::FoundLibrary(const wxString& OldBasePath,const wxStringStringMap& OldVars,const LibraryConfig* Config)
{
    wxStringStringMap Vars = OldVars;
    wxString BasePath = OldBasePath;

    if ( BasePath.empty() ) return;
    BasePath.RemoveLast();
    Vars[_T("BASE_DIR")] = BasePath;
    LibraryResult* Result = new LibraryResult();

    Result->GlobalVar = Config->GlobalVar;
    Result->LibraryName = FixVars(Config->LibraryName,Vars);
    Result->BasePath = FixPath(BasePath);

    if ( !Config->IncludePaths.empty() )
    {
        Result->IncludePath = FixPath(FixVars(Config->IncludePaths[0],Vars));
    }

    if ( !Config->LibPaths.empty() )
    {
        Result->LibPath = FixPath(FixVars(Config->LibPaths[0],Vars));
    }

    if ( !Config->ObjPaths.empty() )
    {
        Result->ObjPath = FixPath(FixVars(Config->ObjPaths[0],Vars));
    }

    wxString CFlags = FixVars(Config->CFlags,Vars);

    for ( size_t i = 1; i<Config->IncludePaths.Count(); ++i )
    {
        CFlags += _T(" -I");
        CFlags += FixPath(FixVars(Config->IncludePaths[i],Vars));
    }

    Result->CFlags = CFlags;

    wxString LFlags = FixVars(Config->LFlags,Vars);

    for ( size_t i = 1; i<Config->LibPaths.Count(); ++i )
    {
        CFlags += _T(" -L");
        CFlags += FixPath(FixVars(Config->LibPaths[i],Vars));
    }
    for ( size_t i = 1; i<Config->ObjPaths.Count(); ++i )
    {
        CFlags += _T(" -L");
        CFlags += FixPath(FixVars(Config->ObjPaths[i],Vars));
    }

    Result->LFlags = FixVars(LFlags,Vars);

    ResultArray& Array = RM()->GetGlobalVar(Config->GlobalVar);
    Array.Add(Result);
}

wxString ProcessingDlg::FixVars(wxString Original,const wxStringStringMap& Vars)
{
    for ( wxStringStringMap::const_iterator it = Vars.begin();
          it != Vars.end();
          ++it )
    {
        wxString SearchString = _T("$(") + it->first + _T(")");
        wxString ReplaceWith = it->second;
        Original.Replace(SearchString,ReplaceWith);
    }

    return Original;
}

wxString ProcessingDlg::FixPath(wxString Original)
{
    return wxFileName(Original).GetFullPath();
}
