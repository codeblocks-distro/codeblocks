#include "../wxsheaders.h"
#include "wxsintproperty.h"

#include <wx/textctrl.h>
#include <wx/msgdlg.h>

wxsIntProperty::wxsIntProperty(int& Int):
    Value(Int),
    PGId(0)
{
	//ctor
}

void wxsIntProperty::AddToPropGrid(wxPropertyGrid* Grid,const wxString& Name)
{
    PGId = Grid->Append(Name,wxPG_LABEL,Value);
}

bool wxsIntProperty::PropGridChanged(wxPropertyGrid* Grid,wxPGId Id)
{
    if ( Id == PGId )
    {
        Value = Grid->GetPropertyValue(Id).GetLong();
        int Cor = Value;
        Value = CorrectValue(Value);
        if ( Value != Cor )
        {
            Grid->SetPropertyValue(Id,Value);
        }
        return ValueChanged(true);
    }
    return true;
}

void wxsIntProperty::UpdatePropGrid(wxPropertyGrid* Grid)
{
    Grid->SetPropertyValue(PGId,Value);
}
