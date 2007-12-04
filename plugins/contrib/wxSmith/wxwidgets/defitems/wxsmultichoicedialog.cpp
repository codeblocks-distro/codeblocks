/*
* This file is part of wxSmith plugin for Code::Blocks Studio
* Copyright (C) 2007  Bartlomiej Swiecki
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
*
* $Revision: 4504 $
* $Id: wxsMultiChoiceDialog.cpp 4504 2007-10-02 21:52:30Z byo $
* $HeadURL: svn+ssh://byo@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsMultiChoiceDialog.cpp $
*/

#include "wxsmultichoicedialog.h"
#include "../wxsitemresdata.h"
#include <wx/choicdlg.h>

namespace
{
    wxsRegisterItem<wxsMultiChoiceDialog> Reg(_T("MultiChoiceDialog"),wxsTTool,_T("Dialogs"),50,false);

    WXS_ST_BEGIN(wxsMultiChoiceDialogStyles,_T("wxCHOICEDLG_STYLE"))
        WXS_ST_CATEGORY("wxMultiChoiceDialog")
        WXS_ST(wxCHOICEDLG_STYLE)
        WXS_ST(wxOK)
        WXS_ST(wxCANCEL)
        WXS_ST(wxCENTRE)
        WXS_ST_DEFAULTS()
    WXS_ST_END()
}

wxsMultiChoiceDialog::wxsMultiChoiceDialog(wxsItemResData* Data):
    wxsTool(Data,&Reg.Info,0,wxsMultiChoiceDialogStyles)
{
    m_Message = _("Select items");
}

void wxsMultiChoiceDialog::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("<wx/choicdlg.h>"),GetInfo().ClassName,hfInPCH);

            wxString ChoicesName;
            if ( m_Content.GetCount() > 0 )
            {
                ChoicesName = GetCoderContext()->GetUniqueName(_T("__wxMultiChoiceDialogChoices"));
                Codef(_T("wxString %s[%d] = \n{\n"),ChoicesName.c_str(),(int)m_Content.GetCount());
                for ( size_t i = 0; i < m_Content.GetCount(); ++i )
                {
                    Codef(_T("\t%t%s\n"),m_Content[i].c_str(),((i!=m_Content.GetCount()-1)?_T(","):_T("")));
                }
                Codef(_T("};\n"));
            }

            Codef(_T("%C(%W, %t, %t, %d, %s, %T, %P);\n"),
                m_Message.c_str(),
                m_Caption.c_str(),
                (int)m_Content.GetCount(),
                (m_Content.IsEmpty()?_T("0"):ChoicesName.c_str()));

            BuildSetupWindowCode();
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsMultiChoiceDialog::OnBuildCreatingCode"),GetLanguage());
        }
    }
}

void wxsMultiChoiceDialog::OnEnumToolProperties(long Flags)
{
    WXS_SHORT_STRING(wxsMultiChoiceDialog,m_Message,_("Message"),_T("message"),_T(""),false);
    WXS_SHORT_STRING(wxsMultiChoiceDialog,m_Caption,_("Caption"),_T("caption"),_T(""),false);
    WXS_ARRAYSTRING (wxsMultiChoiceDialog,m_Content,_("Items"),  _T("content"),_T("item"));
}
