#include "configurationpanel.h"
#include <wx/intl.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/sizer.h>

cbConfigurationDialog::cbConfigurationDialog(wxWindow* parent, int id, const wxString& title)
    : wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize),
    m_pPanel(0)
{
}

void cbConfigurationDialog::AttachConfigurationPanel(cbConfigurationPanel* panel)
{
    assert(panel);

    m_pPanel = panel;
    m_pPanel->Reparent(this);

    wxBoxSizer* bs = new wxBoxSizer(wxVERTICAL);
    bs->Add(m_pPanel, 1, wxGROW | wxRIGHT | wxTOP | wxBOTTOM, 8);

    wxStaticLine* line = new wxStaticLine(this);
    bs->Add(line, 0, wxGROW | wxLEFT | wxRIGHT, 8);

    m_pOK = new wxButton(this, wxID_OK, _("&OK"));
    m_pOK->SetDefault();
    m_pCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"));
    wxStdDialogButtonSizer* but = new wxStdDialogButtonSizer;
    but->AddButton(m_pOK);
    but->AddButton(m_pCancel);
    but->Realize();
    bs->Add(but, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 8);

    SetSizer(bs);

    bs->SetSizeHints(this);
    CenterOnParent();
}

cbConfigurationDialog::~cbConfigurationDialog()
{
}

void cbConfigurationDialog::EndModal(int retCode)
{
    if (retCode == wxID_OK)
        m_pPanel->OnApply();
    else
        m_pPanel->OnCancel();
    wxDialog::EndModal(retCode);
}
