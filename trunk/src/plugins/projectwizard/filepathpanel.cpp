#include <sdk.h>
#ifndef CB_PRECOMP
    #include <cbproject.h>
    #include <projectbuildtarget.h>
    #include <projectmanager.h>
    #include <manager.h>
#endif
#include <wx/filefn.h>
#include "filepathpanel.h"

BEGIN_EVENT_TABLE(FilePathPanel,wxPanel)
	//(*EventTable(FilePathPanel)
	EVT_TEXT(ID_TEXTCTRL1,FilePathPanel::OntxtFilenameText)
	EVT_BUTTON(ID_BUTTON1,FilePathPanel::OnbtnBrowseClick)
	EVT_CHECKBOX(ID_CHECKBOX1,FilePathPanel::OnchkAddToProjectChange)
	//*)
END_EVENT_TABLE()

FilePathPanel::FilePathPanel(wxWindow* parent,wxWindowID id)
    : txtFilename(0),
    btnBrowse(0),
    lblGuard(0),
    txtGuard(0),
    chkAddToProject(0),
    cmbTargets(0)
{
	//(*Initialize(FilePathPanel)
	wxBoxSizer* BoxSizer1;
	wxStaticText* StaticText1;
	wxStaticText* StaticText2;
	wxStaticText* StaticText4;

	Create(parent,id,wxDefaultPosition,wxDefaultSize,wxTAB_TRAVERSAL);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticText1 = new wxStaticText(this,ID_STATICTEXT1,_("Please enter the file\'s name and whether to add it\nto the active project."),wxDefaultPosition,wxDefaultSize,0);
	StaticText2 = new wxStaticText(this,ID_STATICTEXT2,_("Filename:"),wxDefaultPosition,wxDefaultSize,0);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	txtFilename = new wxTextCtrl(this,ID_TEXTCTRL1,_("Text"),wxDefaultPosition,wxDefaultSize,0);
	if ( 0 ) txtFilename->SetMaxLength(0);
	btnBrowse = new wxButton(this,ID_BUTTON1,_("..."),wxDefaultPosition,wxSize(22,22),0);
	if (false) btnBrowse->SetDefault();
	BoxSizer2->Add(txtFilename,1,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	BoxSizer2->Add(btnBrowse,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	lblGuard = new wxStaticText(this,ID_STATICTEXT3,_("Header guard word:"),wxDefaultPosition,wxDefaultSize,0);
	txtGuard = new wxTextCtrl(this,ID_TEXTCTRL2,_("Text"),wxDefaultPosition,wxDefaultSize,0);
	if ( 0 ) txtGuard->SetMaxLength(0);
	chkAddToProject = new wxCheckBox(this,ID_CHECKBOX1,_("Add file to active project"),wxDefaultPosition,wxDefaultSize,0);
	chkAddToProject->SetValue(false);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	StaticText4 = new wxStaticText(this,ID_STATICTEXT4,_("In build target:"),wxDefaultPosition,wxDefaultSize,0);
	cmbTargets = new wxChoice(this,ID_CHOICE1,wxDefaultPosition,wxDefaultSize,0,NULL,0);
	BoxSizer3->Add(16,16,0);
	BoxSizer3->Add(StaticText4,0,wxALL|wxALIGN_CENTER,0);
	BoxSizer3->Add(cmbTargets,1,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	BoxSizer1->Add(StaticText1,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,8);
	BoxSizer1->Add(StaticText2,0,wxLEFT|wxRIGHT|wxTOP|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,8);
	BoxSizer1->Add(BoxSizer2,0,wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,8);
	BoxSizer1->Add(lblGuard,0,wxLEFT|wxRIGHT|wxTOP|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,8);
	BoxSizer1->Add(txtGuard,0,wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,8);
	BoxSizer1->Add(-1,-1,1);
	BoxSizer1->Add(chkAddToProject,0,wxLEFT|wxRIGHT|wxTOP|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,8);
	BoxSizer1->Add(BoxSizer3,0,wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,8);
	this->SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	//*)

	txtFilename->SetValue(wxEmptyString);
	txtGuard->SetValue(wxEmptyString);

    // fill targets list
    cbProject* prj = Manager::Get()->GetProjectManager()->GetActiveProject();
    if (prj)
    {
        cmbTargets->Clear();
        for (int i = 0; i < prj->GetBuildTargetsCount(); ++i)
        {
            ProjectBuildTarget* bt = prj->GetBuildTarget(i);
            if (bt)
                cmbTargets->Append(bt->GetTitle());
        }
        cmbTargets->SetSelection(prj->GetActiveBuildTarget());
        chkAddToProject->SetValue(true);
        cmbTargets->Enable(true);
    }
    else
    {
        chkAddToProject->SetValue(false);
        cmbTargets->Enable(false);
    }
}

FilePathPanel::~FilePathPanel()
{
}

void FilePathPanel::SetAddToProject(bool add)
{
    cbProject* prj = Manager::Get()->GetProjectManager()->GetActiveProject();
    add = add && prj;
    chkAddToProject->SetValue(add);
    cmbTargets->Enable(add);
}

void FilePathPanel::OntxtFilenameText(wxCommandEvent& event)
{
    if (!txtFilename || txtFilename->GetValue().IsEmpty())
        return;
    wxString name = wxFileNameFromPath(txtFilename->GetValue());
    while (name.Replace(_T(" "), _T("_")))
        ;
    while (name.Replace(_T("\t"), _T("_")))
        ;
    while (name.Replace(_T("."), _T("_")))
        ;
    name.MakeUpper();
    name << _T("_INCLUDED");
    txtGuard->SetValue(name);
}

void FilePathPanel::OnbtnBrowseClick(wxCommandEvent& event)
{
    cbProject* prj = Manager::Get()->GetProjectManager()->GetActiveProject();
    wxFileDialog* dlg = new wxFileDialog(this,
                            _("Select filename"),
                            prj ? prj->GetBasePath() : _T(""),
                            txtFilename->GetValue(),
                            m_ExtFilter,
                            wxSAVE | wxOVERWRITE_PROMPT);
    PlaceWindow(dlg);
    if (dlg->ShowModal() == wxID_OK)
		txtFilename->SetValue(dlg->GetPath());
    dlg->Destroy();
}

void FilePathPanel::OnchkAddToProjectChange(wxCommandEvent& event)
{
    SetAddToProject(event.IsChecked());
}
