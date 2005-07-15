#include "wxsdefwidget.h"

wxsDefWidget::wxsDefWidget(wxsWidgetManager* Man,BasePropertiesType pType):
    wxsWidget(Man,pType)
{
    // evInit();
}

wxsDefWidget::~wxsDefWidget()
{
	// evDestroy();
}

bool wxsDefWidget::MyXmlLoad()
{
    Return = true;
    evXmlL();
    return Return;
}

bool wxsDefWidget::MyXmlSave()
{
    Return = true;
    evXmlS();
    return Return;
}

void wxsDefWidget::CreateObjectProperties()
{
    evProps();
    wxsWidget::CreateObjectProperties();
}

const char* wxsDefWidget::GetProducingCode(wxsCodeParams& Params)
{
    CodeResult = GetGeneratingCodeStr();
    
    CodeReplace(wxT("ThisWidget"),BaseParams.VarName);
    CodeReplace(wxT("parent"),Params.ParentName);
    CodeReplace(wxT("id"),BaseParams.IdName);
    CodeReplace(wxT("pos"),GetCodeDefines().Pos);
    CodeReplace(wxT("size"),GetCodeDefines().Size);
    CodeReplace(wxT("style"),GetCodeDefines().Style);
    CodeReplace(wxT("font"),GetCodeDefines().Font);
    CodeReplace(wxT("fcolour"),GetCodeDefines().FColour);
    CodeReplace(wxT("bcolour"),GetCodeDefines().BColour);
    
    evCode();
    return CodeResult.c_str();
}

void wxsDefWidget::evInit()
{
    evUse = Init;
    BuildExtVars();
}

void wxsDefWidget::evXmlL()
{
    evUse = XmlL;
    BuildExtVars();
}

void wxsDefWidget::evXmlS()
{
    evUse = XmlS;
    BuildExtVars();
}

void wxsDefWidget::evCode()
{
    evUse = Code;
    BuildExtVars();
}

void wxsDefWidget::evProps()
{
    evUse = Props;
    BuildExtVars();
}

void wxsDefWidget::evDestroy()
{
    evUse = Destroy;
    BuildExtVars();
}

void wxsDefWidget::CodeReplace(const wxString& Old,const wxString& New)
{
    CodeResult.Replace(Old,New,true);
}

const char* wxsDefWidget::GetDeclarationCode(wxsCodeParams& Params)
{
    static wxString Tmp;
    Tmp = wxT(GetWidgetNameStr());
    Tmp.Append(wxT("* "));
    Tmp += BaseParams.VarName;
    Tmp.Append(';');
    return Tmp.c_str();
}


void wxsDefWidget::evBool(bool& Val,char* Name,char* XrcName,char* PropName,bool DefValue)
{
    switch ( evUse )
    {
        case Init:
        {
            Val = DefValue;
            break;
        }
        
        case XmlL:
        {
            Val = XmlGetInteger(XrcName,DefValue?1:0) != 0;
            break;
        }
        
        case XmlS:
        {
            if ( Val != DefValue )
            {
                XmlSetInteger(XrcName,Val?1:0);
            }
            break;
        }

        case Destroy:
        {
            break;
        }
                
        case Code:
        {
            CodeReplace(Name,wxString::Format("%s",Val?"true":"false"));
            break;
        }
        
        case Props:
        {
            PropertiesObject.AddProperty(PropName,Val);
            break;
        }
    }
}

void wxsDefWidget::evInt(int& Val,char* Name,char* XrcName,char* PropName,int DefValue)
{
    switch ( evUse )
    {
        case Init:
        {
            Val = DefValue;
            break;
        }
        
        case XmlL:
        {
            Val = XmlGetInteger(XrcName,DefValue);
            break;
        }
        
        case XmlS:
        {
            if ( Val != DefValue )
            {
                XmlSetInteger(XrcName,Val);
            }
            break;
        }

        case Destroy:
        {
            break;
        }
                
        case Code:
        {
            CodeReplace(Name,wxString::Format("%d",Val));
            break;
        }
        
        case Props:
        {
            PropertiesObject.AddProperty(PropName,Val);
            break;
        }
    }
}

void wxsDefWidget::ev2Int(int& Val1,int& Val2,char* Name,char* XrcName,char* PropName,int DefValue1,int DefValue2)
{
    switch ( evUse )
    {
        case Init:
        {
            Val1 = DefValue1;
            Val2 = DefValue2;
            break;
        }
        
        case XmlL:
        {
            XmlGetIntPair(XrcName,Val1,Val2,DefValue1,DefValue2);
            break;
        }
        
        case XmlS:
        {
            if ( Val1!=DefValue1 || Val2!=DefValue2 )
            {
                XmlSetIntPair(XrcName,Val1,Val2);
            }
            break;
        }

        case Destroy:
        {
            break;
        }
                
        case Code:
        {
            CodeReplace(Name,wxString::Format("wxPoint(%d,%d)",Val1,Val2));
            break;
        }
        
        case Props:
        {
            PropertiesObject.Add2IProperty(PropName,Val1,Val2);
            break;
        }
    }
}

void wxsDefWidget::evStr(wxString& Val,char* Name,char* XrcName,char* PropName,wxString DefValue)
{
    switch ( evUse )
    {
        case Init:
        {
            Val = DefValue;
            break;
        }
        
        case XmlL:
        {
            const char* Value = XmlGetVariable(XrcName);
            if ( Value ) Val = Value;
            else Val = DefValue;
            break;
        }
        
        case XmlS:
        {
            if ( Val != DefValue )
            {
                XmlSetVariable(XrcName,Val);
            }
            break;
        }
        
        case Destroy:
        {
            break;
        }
        
        case Code:
        {
            CodeReplace(Name,wxString::Format("wxT(%s)",GetCString(Val).c_str()));
            break;
        }
        
        case Props:
        {
            PropertiesObject.AddProperty(PropName,Val);
        }
    }
}

void wxsDefWidget::evStrArray(wxArrayString& Val,char* Name,char* XrcParentName,char* XrcChildName,char* PropName, int& DefValue)
{
    switch ( evUse )
    {
        case Init:
        {
            Val.Clear();
            break;
        }
        
        case XmlL:
        {
			if( !XmlGetStringArray(XrcParentName,XrcChildName,Val) )
			{
				Val.Clear();
			}             
            break;
        }
        
        case XmlS:
        {
			if( !XmlSetStringArray(XrcParentName,XrcChildName,Val) )
			{
				Val.Clear();
			}           
            break;
        }

        case Destroy:
        {
			Val.Clear();
            break;
        }

        case Code:
        {
            // Replacing wxsDWAddStrings function calls
            
            wxString CodeToSearch = wxString::Format(wxT("wxsDWAddStrings(%s,%s);"),Name,GetBaseParams().VarName.c_str());
            wxString ReplaceWith;
            for ( size_t i = 0; i<Val.GetCount(); i++ )
            {
            	ReplaceWith.Append(GetBaseParams().VarName);
            	ReplaceWith.Append(wxT("->Append(wxT("));
            	ReplaceWith.Append(GetCString(Val[i].c_str()));
            	ReplaceWith.Append(wxT("));\n"));
            }
            CodeReplace(CodeToSearch,ReplaceWith);
            
            // Replacing wxsDWSelectString function calls
            
            CodeToSearch.Printf(wxT("wxsDWSelectString(%s,%d,%s)"),Name,DefValue,GetBaseParams().VarName.c_str());
            ReplaceWith.Printf(wxT("%s->SetSelection(%d)"),GetBaseParams().VarName.c_str(),DefValue);
            CodeReplace(CodeToSearch,ReplaceWith);
            
            break;
        }
        
        case Props:
        {
           PropertiesObject.AddProperty(PropName,Val,DefValue,-1);
        }
    }
}

void wxsStopMouseEvents::SkipEvent(wxMouseEvent& event)
{}

wxsStopMouseEvents wxsStopMouseEvents::Object;

BEGIN_EVENT_TABLE(wxsStopMouseEvents,wxEvtHandler)
    EVT_LEFT_DOWN(wxsStopMouseEvents::SkipEvent)
    EVT_LEFT_DCLICK(wxsStopMouseEvents::SkipEvent)
END_EVENT_TABLE()
