#include <sdk.h>
#include <wx/intl.h>
#include <wx/xrc/xmlres.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/spinctrl.h>
#include <wx/utils.h>
#include <configmanager.h>
#include "addtododlg.h"


AddTodoDlg::AddTodoDlg(wxWindow* parent, wxArrayString& types)
    : m_Types(types)
{
	wxXmlResource::Get()->LoadDialog(this, parent, _T("dlgAddToDo"));
	LoadUsers();

    // load types
    wxComboBox* cmb = XRCCTRL(*this, "cmbType", wxComboBox);
    cmb->Clear();
    for (unsigned int i = 0; i < m_Types.GetCount(); ++i)
    {
        cmb->Append(m_Types[i]);
    }
    if (m_Types.Index(_T("TODO")) == wxNOT_FOUND)
        cmb->Append(_T("TODO"));
    if (m_Types.Index(_T("FIXME")) == wxNOT_FOUND)
        cmb->Append(_T("FIXME"));
    if (m_Types.Index(_T("NOTE")) == wxNOT_FOUND)
        cmb->Append(_T("NOTE"));

    wxString sels = Manager::Get()->GetConfigManager(_T("todo_list"))->Read(_T("last_used_type"));
    if (!sels.IsEmpty())
    {
        int sel = cmb->FindString(sels);
        if (sel != -1)
            cmb->SetSelection(sel);
    }
    else
        cmb->SetSelection(0);
}

AddTodoDlg::~AddTodoDlg()
{
	//dtor
}

void AddTodoDlg::LoadUsers()
{
	wxComboBox* cmb = XRCCTRL(*this, "cmbUser", wxComboBox);

	wxArrayString users;
	Manager::Get()->GetConfigManager(_T("todo_list"))->Read(_T("users"), &users);

	cmb->Clear();
    cmb->Append(users);

	if (cmb->GetCount() == 0)
		cmb->Append(wxGetUserId());
	cmb->SetSelection(0);
}

void AddTodoDlg::SaveUsers()
{
	wxComboBox* cmb = XRCCTRL(*this, "cmbUser", wxComboBox);
	wxArrayString users;

	for (int i = 0; i < cmb->GetCount(); ++i)
	{
		users.Add(cmb->GetString(i));
	}
	Manager::Get()->GetConfigManager(_T("todo_list"))->Write(_T("users"), users);
}

wxString AddTodoDlg::GetText()
{
    return XRCCTRL(*this, "txtText", wxTextCtrl)->GetValue();
}

wxString AddTodoDlg::GetUser()
{
    return XRCCTRL(*this, "cmbUser", wxComboBox)->GetValue();
}

int AddTodoDlg::GetPriority()
{
    int prio = XRCCTRL(*this, "spnPriority", wxSpinCtrl)->GetValue();
    if (prio < 1)
        prio = 1;
    else if (prio > 9)
        prio = 9;
    return prio;
}

ToDoPosition AddTodoDlg::GetPosition()
{
    return (ToDoPosition)(XRCCTRL(*this, "cmbPosition", wxComboBox)->GetSelection());
}

wxString AddTodoDlg::GetType()
{
    return XRCCTRL(*this, "cmbType", wxComboBox)->GetValue();
}

ToDoCommentType AddTodoDlg::GetCommentType()
{
    return (ToDoCommentType)(XRCCTRL(*this, "cmbStyle", wxComboBox)->GetSelection());
}

void AddTodoDlg::EndModal(int retVal)
{
	if (retVal == wxID_OK)
	{
		SaveUsers();

        // "save" types
        wxComboBox* cmb = XRCCTRL(*this, "cmbType", wxComboBox);
        m_Types.Clear();
        if (cmb->FindString(cmb->GetValue()) == wxNOT_FOUND)
            m_Types.Add(cmb->GetValue());
        for (int i = 0; i < cmb->GetCount(); ++i)
        {
            m_Types.Add(cmb->GetString(i));
        }

        Manager::Get()->GetConfigManager(_T("todo_list"))->Write(_T("last_used_type"), cmb->GetValue());
	}

	wxDialog::EndModal(retVal);
}
