#include "wxswindowres.h"

#include "../wxswidgetfactory.h"
#include "../wxswindoweditor.h"
#include "../wxscodegen.h"
#include <manager.h>
#include <wx/xrc/xmlres.h>
#include <editormanager.h>

const wxChar* EmptySource =
_T("\
#include \"$(Include)\"\n\
\n\
BEGIN_EVENT_TABLE($(ClassName),$(BaseClassName))\n\
//(*EventTable($(ClassName))\n\
//*)\n\
END_EVENT_TABLE()\n\
\n\
$(ClassName)::$(ClassName)(wxWidnow* parent,wxWindowID id):\
    $(BaseClassCtor)\n\
{\n\
    //(*Initialize($(ClassName))\n\
    //*)\n\
}\n\
\n\
$(ClassName)::~$(ClassName)()\n\
{\n\
}\n\
\n\
");

const wxChar* EmptyHeader =
_T("\
#ifndef $(Guard)\n\
#define $(Guard)\n\
\n\
//(*Headers($(ClassName))\n\
//*)\n\
\n\
class $(ClassName): public $(BaseClassName)\n\
{\n\
    public:\n\
\n\
        $(ClassName)(wxWidnow* parent,wxWindowID id = -1);\n\
        virtual ~$(ClassName);\n\
\n\
        //(*Identifiers($(ClassName))\n\
        //*)\n\
\n\
    protected:\n\
\n\
        //(*Handlers($(ClassName))\n\
        //*)\n\
\n\
        //(*Declarations($(ClassName))\n\
        //*)\n\
\n\
    private:\n\
\n\
        DECLARE_EVENT_TABLE()\n\
};\n\
\n\
#endif\n\
");


wxsWindowRes::wxsWindowRes(
    wxsProject* Project,
    const wxString& Class, 
    const wxString& Xrc, 
    const wxString& Src,
    const wxString& Head,
    WindowResType _Type):
        wxsResource(Project),
        ClassName(Class),
        XrcFile(Xrc),
        SrcFile(Src),
        HFile(Head),
        Type(_Type)
{
    RootWidget = wxsWidgetFactory::Get()->Generate(GetWidgetClass(true));
}

wxsWindowRes::~wxsWindowRes()
{
    EditClose();
    wxsWidgetFactory::Get()->Kill(RootWidget);
    switch ( Type )
    {
    	case Dialog:
            GetProject()->DeleteDialog(dynamic_cast<wxsDialogRes*>(this));
            break;
            
        case Frame:
            GetProject()->DeleteFrame(dynamic_cast<wxsFrameRes*>(this));
            break;
            
        case Panel:
            GetProject()->DeletePanel(dynamic_cast<wxsPanelRes*>(this));
            break;
            
        default:;
    }
}

wxsEditor* wxsWindowRes::CreateEditor()
{
    wxsWindowEditor* Edit = new wxsWindowEditor(Manager::Get()->GetEditorManager()->GetNotebook(),XrcFile,this);
    Edit->BuildPreview(RootWidget);
    return Edit;
}

void wxsWindowRes::Save()
{
    TiXmlDocument* Doc = GenerateXml();
    
    if ( Doc )
    {
        wxString FullFileName = GetProject()->GetInternalFileName(XrcFile);
        Doc->SaveFile(FullFileName.mb_str());
        delete Doc;
    }
}

TiXmlDocument* wxsWindowRes::GenerateXml()
{
    TiXmlDocument* NewDoc = new TiXmlDocument;
    TiXmlElement* Resource = NewDoc->InsertEndChild(TiXmlElement("resource"))->ToElement();
    TiXmlElement* XmlDialog = Resource->InsertEndChild(TiXmlElement("object"))->ToElement();
    XmlDialog->SetAttribute("class",wxString(GetWidgetClass()).mb_str());
    XmlDialog->SetAttribute("name",ClassName.mb_str());
    if ( !RootWidget->XmlSave(XmlDialog) )
    {
        delete NewDoc;
        return NULL;
    }
    return NewDoc;
}

void wxsWindowRes::ShowPreview()
{
    Save();
    
    wxXmlResource Res(GetProject()->GetInternalFileName(XrcFile));
    Res.InitAllHandlers();
    
    switch ( Type )
    {
    	case Dialog:
    	{
            wxDialog Dlg;
            
            if ( Res.LoadDialog(&Dlg,NULL,ClassName) )
            {
                Dlg.ShowModal();
            }
            break;
    	}
            
        case Frame:
        {
            wxFrame* Frm = new wxFrame;
            if ( Res.LoadFrame(Frm,NULL,ClassName) )
            {
            	Frm->Show();
            }
            break;
        }
            
        case Panel:
        {
        	wxDialog Dlg(NULL,-1,wxString::Format(_("Frame preview: %s"),ClassName.c_str()));
        	wxPanel* Panel = Res.LoadPanel(&Dlg,ClassName);
        	if ( Panel )
        	{
        		Dlg.Fit();
        		Dlg.ShowModal();
        	}
        	break;
        }
        
        default:;
    }
}

const wxString& wxsWindowRes::GetResourceName()
{
    return GetClassName();
}

bool wxsWindowRes::GenerateEmptySources()
{
    // Generating file variables
    
    wxString FName = wxFileName(HFile).GetFullName();
    FName.MakeUpper();
    wxString Guard(_T("__"));
    
    for ( int i=0; i<(int)FName.Length(); i++ )
    {
        char ch = FName.GetChar(i);
        if ( ( ch < 'A' || ch > 'Z' ) && ( ch < '0' || ch > '9' ) ) Guard.Append('_');
        else Guard.Append(ch);
    }
    
    wxFileName IncludeFN(GetProject()->GetProjectFileName(HFile));
    IncludeFN.MakeRelativeTo(
        wxFileName(GetProject()->GetProjectFileName(SrcFile)).GetPath() );
    wxString Include = IncludeFN.GetFullPath();
    

    FILE* Fl = fopen(GetProject()->GetProjectFileName(HFile).mb_str(),"wt");
    if ( !Fl ) return false;
    wxString Content = EmptyHeader;
    Content.Replace(_T("$(Guard)"),Guard,true);
    Content.Replace(_T("$(ClassName)"),ClassName,true);
    Content.Replace(_T("$(BaseClassName)"),GetWidgetClass(),true);
    switch ( Type )
    {
    	case Dialog:
            Content.Replace(_T("$(BaseClassCtor)"),_T("wxDialog(parent,id,wxT(\"\"))"),true);
            break;
            
        case Frame:
            Content.Replace(_T("$(BaseClassCtor)"),_T("wxFrame(parent,id,wxT(\"\"))"),true);
            break;
            
        case Panel:
            Content.Replace(_T("$(BaseClassCtor)"),_T("wxPanel(parent,id)"),true);
            break;
            
        default:;
    }
    fprintf(Fl,"%s",(const char*)Content.mb_str());
    fclose(Fl);
    
    Fl = fopen(GetProject()->GetProjectFileName(SrcFile).mb_str(),"wt");
    if ( !Fl ) return false;
    Content = EmptySource;
    Content.Replace(_T("$(Include)"),Include,true);
    Content.Replace(_T("$(ClassName)"),ClassName,true);
    Content.Replace(_T("$(BaseClassName)"),GetWidgetClass(),true);
    fprintf(Fl,"%s",(const char*)Content.mb_str());
    fclose(Fl);
    return true;
}

void wxsWindowRes::NotifyChange()
{
	assert ( GetProject() != NULL );
	
	#if 1
	
	int TabSize = 4;
	int GlobalTabSize = 2 * TabSize;
	
	// Generating producing code
	wxsCodeGen Gen(RootWidget,TabSize,TabSize);
	
	// Creating code header

	wxString CodeHeader = wxString::Format(_T("//(*Initialize(%s)"),GetClassName().c_str());
	wxString Code = CodeHeader + _T("\n");
	
	// Creating local and global declarations
	
	wxString GlobalCode;
	bool WasDeclaration = false;
	AddDeclarationsReq(RootWidget,Code,GlobalCode,TabSize,GlobalTabSize,WasDeclaration);
	if ( WasDeclaration )
	{
		Code.Append('\n');
	}
		
	// Creating window-generating code
	
	Code.Append(Gen.GetCode());
	Code.Append(' ',TabSize);
	
// TODO (SpOoN#1#): Apply title and centered properties for frame and dialog
	

	wxsCoder::Get()->AddCode(GetProject()->GetProjectFileName(SrcFile),CodeHeader,Code);
	
	// Creating global declarations
	
	CodeHeader = wxString::Format(_T("//(*Declarations(%s)"),GetClassName().c_str());
	Code = CodeHeader + _T("\n") + GlobalCode;
	Code.Append(' ',GlobalTabSize);
	wxsCoder::Get()->AddCode(GetProject()->GetProjectFileName(HFile),CodeHeader,Code);
	
	#endif
}

void wxsWindowRes::AddDeclarationsReq(wxsWidget* Widget,wxString& LocalCode,wxString& GlobalCode,int LocalTabSize,int GlobalTabSize,bool& WasLocal)
{
	static wxsCodeParams EmptyParams;
	
	if ( !Widget ) return;
	int Count = Widget->GetChildCount();
	for ( int i=0; i<Count; i++ )
	{
		wxsWidget* Child = Widget->GetChild(i);
		bool Member = Child->GetBaseParams().IsMember;
		wxString& Code = Member ? GlobalCode : LocalCode;
		
		Code.Append(' ',Member ? GlobalTabSize : LocalTabSize);
		Code.Append(Child->GetDeclarationCode(EmptyParams));
		Code.Append('\n');
		
		WasLocal |= !Member;
		AddDeclarationsReq(Child,LocalCode,GlobalCode,LocalTabSize,GlobalTabSize,WasLocal);
	}
}

inline const wxChar* wxsWindowRes::GetWidgetClass(bool UseRes)
{
	switch ( Type )
	{
		case Dialog: return _T("wxDialog");
		case Frame:  return _T("wxFrame");
		case Panel:  return UseRes ? _T("wxPanelr") : _T("wxPanel");
	}
	
	return _T("");
}
