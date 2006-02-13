/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is distributed under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* Author: Thomas Denk
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include "uservarmanager.h"
    #include "configmanager.h"
    #include "messagemanager.h"
    #include "manager.h"
    #include "cbexception.h"
    #include "globals.h"

    #include <wx/dialog.h>
    #include <wx/intl.h>
    #include <wx/xrc/xmlres.h>
    #include <wx/textctrl.h>
#endif

#include <wx/choice.h>

class UsrGlblMgrEditDialog : public wxDialog
{
    wxString curr;
public:
    UsrGlblMgrEditDialog(wxWindow* parent, const wxString& base);
private:
    void OnOKClick(wxCommandEvent& event);
    void OnCancelClick(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnFS(wxCommandEvent& event);
    void OnCB(wxCommandEvent& event);
    void Load(const wxString& s);
    void Save();
    void List();
    void Add(const wxString& base);
    DECLARE_EVENT_TABLE()
};


void UserVariableManager::Configure()
{
    UsrGlblMgrEditDialog d(Manager::Get()->GetAppWindow(), wxEmptyString);
    PlaceWindow(&d);
    d.ShowModal();
}


wxString UserVariableManager::Replace(const wxString& variable)
{
    ConfigManager * cfg = Manager::Get()->GetConfigManager(_T("global_uservars"));

    wxString package = variable.AfterLast(wxT('#')).BeforeFirst(wxT('.')).MakeLower();
    wxString member = variable.AfterFirst(wxT('.')).MakeLower();

    wxString base = cfg->Read(package + _T("/base"));

    if(base.IsEmpty())
    {
        cbMessageBox(_("At least one global variable is used but not yet defined.\n"
                       "Please define it now..."), _("Warning"), wxICON_WARNING);
        UsrGlblMgrEditDialog d(Manager::Get()->GetAppWindow(), package);
        PlaceWindow(&d);
        d.ShowModal();
    }

    if(member.IsEmpty() || member.IsSameAs(_T("base")) || member.IsSameAs(_T("dir")))
        return base;

    if(member.IsSameAs(_T("include")) || member.IsSameAs(_T("lib")) || member.IsSameAs(_T("obj")))
    {
        wxString ret = cfg->Read(package + _T("/") + member);
        if(ret.IsEmpty()
          )
            ret = base + _T("/") + member;
        return ret;
    }
    else if(member.IsSameAs(_T("cflags")) || member.IsSameAs(_T("lflags")))
    {
        return cfg->Read(package + _T("/") + member);
    }

    Manager::Get()->GetMessageManager()->DebugLog(_T("Warning: bad member ") + member + _T(" of user variable ") + package);

    return wxEmptyString;
}



//FIXME: Does XRCID already include _T(), or do we need to add that here?
BEGIN_EVENT_TABLE(UsrGlblMgrEditDialog, wxDialog)
EVT_BUTTON(XRCID("wxID_OK"), UsrGlblMgrEditDialog::OnOKClick)
EVT_BUTTON(XRCID("wxID_CANCEL"), UsrGlblMgrEditDialog::OnCancelClick)
EVT_BUTTON(XRCID("delete"), UsrGlblMgrEditDialog::OnDelete)
EVT_BUTTON(XRCID("fs_value"), UsrGlblMgrEditDialog::OnFS)
EVT_BUTTON(XRCID("fs_include"), UsrGlblMgrEditDialog::OnFS)
EVT_BUTTON(XRCID("fs_lib"), UsrGlblMgrEditDialog::OnFS)
EVT_BUTTON(XRCID("fs_obj"), UsrGlblMgrEditDialog::OnFS)
EVT_BUTTON(XRCID("fs_cflags"), UsrGlblMgrEditDialog::OnFS)
EVT_BUTTON(XRCID("fs_lflags"), UsrGlblMgrEditDialog::OnFS)
EVT_CHOICE(XRCID("variable"), UsrGlblMgrEditDialog::OnCB)
END_EVENT_TABLE()

UsrGlblMgrEditDialog::UsrGlblMgrEditDialog(wxWindow* parent, const wxString& base)
{
    wxXmlResource::Get()->LoadDialog(this, parent, _T("dlgUserGlobalVar"));

    List();
    if(!base.IsEmpty())
    {
        Add(base);
        Load(base);
    }
    else
    {
        XRCCTRL(*this, "variable", wxChoice)->SetSelection(0);
        Load(XRCCTRL(*this, "variable", wxChoice)->GetStringSelection());
    }
}

void UsrGlblMgrEditDialog::OnOKClick(wxCommandEvent& event)
{
    Save();
    EndModal(wxID_OK);
}


void UsrGlblMgrEditDialog::Load(const wxString& base)
{
    curr = base;

    XRCCTRL(*this, "variable", wxChoice)->SetStringSelection(base);

    ConfigManager * cfg = Manager::Get()->GetConfigManager(_T("global_uservars"));
    wxString s;

    s = cfg->Read(base + _T("/base"));
    XRCCTRL(*this, "value", wxTextCtrl)->SetValue(s);
    s = cfg->Read(base + _T("/include"));
    XRCCTRL(*this, "include", wxTextCtrl)->SetValue(s);
    s = cfg->Read(base + _T("/lib"));
    XRCCTRL(*this, "lib", wxTextCtrl)->SetValue(s);
    s = cfg->Read(base + _T("/obj"));
    XRCCTRL(*this, "obj", wxTextCtrl)->SetValue(s);
    s = cfg->Read(base + _T("/cflags"));
    XRCCTRL(*this, "cflags", wxTextCtrl)->SetValue(s);
    s = cfg->Read(base + _T("/lflags"));
    XRCCTRL(*this, "lflags", wxTextCtrl)->SetValue(s);
}

void UsrGlblMgrEditDialog::List()
{
    wxChoice * c = XRCCTRL(*this, "variable", wxChoice);
    c->Clear();

    ConfigManager * cfg = Manager::Get()->GetConfigManager(_T("global_uservars"));

    wxArrayString paths = cfg->EnumerateSubPaths(_T("/"));
    c->Append(paths);
    if(curr.IsEmpty())
        c->SetSelection(0);
    else
        c->SetStringSelection(curr);
}

void UsrGlblMgrEditDialog::Add(const wxString& base)
{
    XRCCTRL(*this, "variable", wxChoice)->Append(base);
}

void UsrGlblMgrEditDialog::Save()
{
    if(curr.IsEmpty())
        return;

    ConfigManager * cfg = Manager::Get()->GetConfigManager(_T("global_uservars"));

    cfg->Write(curr + _T("/base"),    XRCCTRL(*this, "value", wxTextCtrl)->GetValue());
    cfg->Write(curr + _T("/include"), XRCCTRL(*this, "include", wxTextCtrl)->GetValue());
    cfg->Write(curr + _T("/lib"),     XRCCTRL(*this, "lib", wxTextCtrl)->GetValue());
    cfg->Write(curr + _T("/obj"),     XRCCTRL(*this, "obj", wxTextCtrl)->GetValue());
    cfg->Write(curr + _T("/cflags"),  XRCCTRL(*this, "cflags", wxTextCtrl)->GetValue());
    cfg->Write(curr + _T("/lflags"),  XRCCTRL(*this, "lflags", wxTextCtrl)->GetValue());
}

void UsrGlblMgrEditDialog::OnCB(wxCommandEvent& event)
{
    Save();
    Load(event.GetString());
}

void UsrGlblMgrEditDialog::OnFS(wxCommandEvent& event)
{
    wxTextCtrl* c;
    int id = event.GetId();

    if(id == XRCID("fs_value"))
        c = XRCCTRL(*this, "value", wxTextCtrl);
    else if(id == XRCID("fs_include"))
        c = XRCCTRL(*this, "include", wxTextCtrl);
    else if(id == XRCID("fs_lib"))
        c = XRCCTRL(*this, "lib", wxTextCtrl);
    else if(id == XRCID("fs_obj"))
        c = XRCCTRL(*this, "obj", wxTextCtrl);
    else if(id == XRCID("fs_cflags"))
        c = XRCCTRL(*this, "cflags", wxTextCtrl);
    else if(id == XRCID("fs_lflags"))
        c = XRCCTRL(*this, "lflags", wxTextCtrl);
	else
		{
			c = 0;
			cbThrow(_T("Encountered invalid button ID"));
		}

    wxString path = ChooseDirectory(0,
                                    _("Choose a location"),
                                    !c->GetValue().IsEmpty() ? c->GetValue() : XRCCTRL(*this, "value", wxTextCtrl)->GetValue());
    if (!path.IsEmpty())
        c->SetValue(path);
}

void UsrGlblMgrEditDialog::OnDelete(wxCommandEvent& event)
{
    wxString g(XRCCTRL(*this, "variable", wxChoice)->GetStringSelection());

    wxMessageDialog dlg(Manager::Get()->GetAppWindow(), wxString::Format(_("Delete the global variable %s?"), g.c_str()), _("Delete"), wxYES_NO);
    PlaceWindow(&dlg);
    if(dlg.ShowModal() == wxID_YES)
        Manager::Get()->GetConfigManager(_T("global_uservars"))->DeleteSubPath(g);
    curr = wxEmptyString;
    List();
}

void UsrGlblMgrEditDialog::OnCancelClick(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}
