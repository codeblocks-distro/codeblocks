#include <sdk.h>
#include <wx/xrc/xmlres.h>
#include <configmanager.h>
#include <wx/intl.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/checklst.h>
#include <wx/radiobox.h>
#include <wx/spinctrl.h>
#include <wx/colordlg.h>
#include <wx/msgdlg.h>
#include "wxAUI/manager.h"

#include "environmentsettingsdlg.h"
#ifdef __WXMSW__
    #include "associations.h"
#endif

BEGIN_EVENT_TABLE(EnvironmentSettingsDlg, wxDialog)
    EVT_UPDATE_UI(-1, EnvironmentSettingsDlg::OnUpdateUI)
    EVT_BUTTON(XRCID("btnSetAssocs"), EnvironmentSettingsDlg::OnSetAssocs)
    EVT_BUTTON(XRCID("btnFNBorder"), EnvironmentSettingsDlg::OnChooseColor)
    EVT_BUTTON(XRCID("btnFNFrom"), EnvironmentSettingsDlg::OnChooseColor)
    EVT_BUTTON(XRCID("btnFNTo"), EnvironmentSettingsDlg::OnChooseColor)
    EVT_BUTTON(XRCID("btnAuiBgColor"), EnvironmentSettingsDlg::OnChooseColor)
    EVT_BUTTON(XRCID("btnAuiSashColor"), EnvironmentSettingsDlg::OnChooseColor)
    EVT_BUTTON(XRCID("btnAuiCaptionColor"), EnvironmentSettingsDlg::OnChooseColor)
    EVT_BUTTON(XRCID("btnAuiCaptionTextColor"), EnvironmentSettingsDlg::OnChooseColor)
    EVT_BUTTON(XRCID("btnAuiBorderColor"), EnvironmentSettingsDlg::OnChooseColor)
    EVT_BUTTON(XRCID("btnAuiGripperColor"), EnvironmentSettingsDlg::OnChooseColor)
END_EVENT_TABLE()

EnvironmentSettingsDlg::EnvironmentSettingsDlg(wxWindow* parent, wxDockArt* art)
    : m_pArt(art)
{
    wxXmlResource::Get()->LoadDialog(this, parent, _T("dlgEnvironmentSettings"));

    ConfigManager *cfg = Manager::Get()->GetConfigManager(_T("app"));
    ConfigManager *pcfg = Manager::Get()->GetConfigManager(_T("project_manager"));
    ConfigManager *mcfg = Manager::Get()->GetConfigManager(_T("message_manager"));
    ConfigManager *acfg = Manager::Get()->GetConfigManager(_T("an_dlg"));

    // tab "General"
    XRCCTRL(*this, "chkShowSplash", wxCheckBox)->SetValue(cfg->ReadBool(_T("/environment/show_splash"), true));
    XRCCTRL(*this, "chkDDE", wxCheckBox)->SetValue(cfg->ReadBool(_T("/environment/use_dde"), true));
    XRCCTRL(*this, "chkSingleInstance", wxCheckBox)->SetValue(cfg->ReadBool(_T("/environment/single_instance"), true));
    XRCCTRL(*this, "chkAssociations", wxCheckBox)->SetValue(cfg->ReadBool(_T("/environment/check_associations"), true));
    XRCCTRL(*this, "chkModifiedFiles", wxCheckBox)->SetValue(cfg->ReadBool(_T("/environment/check_modified_files"), true));
    XRCCTRL(*this, "chkDebugLog", wxCheckBox)->SetValue(mcfg->ReadBool(_T("/has_debug_log"), false));
    XRCCTRL(*this, "rbAppStart", wxRadioBox)->SetSelection(cfg->ReadBool(_T("/environment/blank_workspace"), true) ? 1 : 0);

    // tab "View"
    XRCCTRL(*this, "rbProjectOpen", wxRadioBox)->SetSelection(pcfg->ReadInt(_T("/open_files"), 1));
    XRCCTRL(*this, "rbToolbarSize", wxRadioBox)->SetSelection(cfg->ReadBool(_T("/environment/toolbar_size"), true) ? 1 : 0);
    XRCCTRL(*this, "chkAutoHideMessages", wxCheckBox)->SetValue(mcfg->ReadBool(_T("/auto_hide"), false));
    XRCCTRL(*this, "chkShowStartPage", wxCheckBox)->SetValue(cfg->ReadBool(_T("/environment/start_here_page"), true));

    // tab "Appearence"
    XRCCTRL(*this, "cmbEditorTabs", wxComboBox)->SetSelection(cfg->ReadInt(_T("/environment/editor_tabs_style"), 0));
    XRCCTRL(*this, "btnFNBorder", wxButton)->SetBackgroundColour(cfg->ReadColour(_T("/environment/editor_gradient_border"), wxColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW))));
    XRCCTRL(*this, "btnFNFrom", wxButton)->SetBackgroundColour(cfg->ReadColour(_T("/environment/editor_gradient_from"), wxColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE))));
    XRCCTRL(*this, "btnFNTo", wxButton)->SetBackgroundColour(cfg->ReadColour(_T("/environment/editor_gradient_to"), *wxWHITE));
    XRCCTRL(*this, "spnAuiBorder", wxSpinCtrl)->SetValue(cfg->ReadInt(_T("/environment/aui/border_size"), m_pArt->GetMetric(wxAUI_ART_PANE_BORDER_SIZE)));
    XRCCTRL(*this, "spnAuiSash", wxSpinCtrl)->SetValue(cfg->ReadInt(_T("/environment/aui/sash_size"), m_pArt->GetMetric(wxAUI_ART_SASH_SIZE)));
    XRCCTRL(*this, "spnAuiCaption", wxSpinCtrl)->SetValue(cfg->ReadInt(_T("/environment/aui/caption_size"), m_pArt->GetMetric(wxAUI_ART_CAPTION_SIZE)));
    XRCCTRL(*this, "btnAuiBgColor", wxButton)->SetBackgroundColour(cfg->ReadColour(_T("/environment/aui/bg_color"), m_pArt->GetColor(wxAUI_ART_BACKGROUND_COLOUR)));
    XRCCTRL(*this, "btnAuiSashColor", wxButton)->SetBackgroundColour(cfg->ReadColour(_T("/environment/aui/sash_color"), m_pArt->GetColor(wxAUI_ART_SASH_COLOUR)));
    XRCCTRL(*this, "btnAuiCaptionColor", wxButton)->SetBackgroundColour(cfg->ReadColour(_T("/environment/aui/caption_color"), m_pArt->GetColor(wxAUI_ART_CAPTION_COLOUR)));
    XRCCTRL(*this, "btnAuiCaptionTextColor", wxButton)->SetBackgroundColour(cfg->ReadColour(_T("/environment/aui/caption_text_color"), m_pArt->GetColor(wxAUI_ART_CAPTION_TEXT_COLOUR)));
    XRCCTRL(*this, "btnAuiBorderColor", wxButton)->SetBackgroundColour(cfg->ReadColour(_T("/environment/aui/border_color"), m_pArt->GetColor(wxAUI_ART_BORDER_COLOUR)));
    XRCCTRL(*this, "btnAuiGripperColor", wxButton)->SetBackgroundColour(cfg->ReadColour(_T("/environment/aui/gripper_color"), m_pArt->GetColor(wxAUI_ART_GRIPPER_COLOUR)));

    // tab "Dialogs"
    wxCheckListBox* lb = XRCCTRL(*this, "chkDialogs", wxCheckListBox);
    lb->Clear();
    wxArrayString dialogs = acfg->EnumerateKeys(_T("/"));
    for (unsigned int i = 0; i < dialogs.GetCount(); ++i)
    {
        wxString caption = acfg->Read(dialogs[i]);
        if (!caption.IsEmpty())
            lb->Append(caption);
    }

    // tab "Batch builds"
#ifdef __WXMSW__
    XRCCTRL(*this, "txtBatchBuildsCmdLine", wxTextCtrl)->SetValue(cfg->Read(_T("/batch_build_args"), DEFAULT_BATCH_BUILD_ARGS));
#endif

    // tab "Network"
    XRCCTRL(*this, "txtProxy", wxTextCtrl)->SetValue(cfg->Read(_T("/network_proxy")));

    // tab "Tweaks"
    XRCCTRL(*this, "chkSafebutSlow", wxCheckBox)->SetValue(mcfg->ReadBool(_T("/safe_but_slow"), false));

    // disable some windows-only settings, in other platforms
#ifndef __WXMSW__
    XRCCTRL(*this, "chkDDE", wxCheckBox)->Enable(false);
    XRCCTRL(*this, "chkAssociations", wxCheckBox)->Enable(false);
    XRCCTRL(*this, "btnSetAssocs", wxButton)->Enable(false);
    XRCCTRL(*this, "txtBatchBuildsCmdLine", wxTextCtrl)->Enable(false);
#endif
}

EnvironmentSettingsDlg::~EnvironmentSettingsDlg()
{
    //dtor
}

void EnvironmentSettingsDlg::OnSetAssocs(wxCommandEvent& event)
{
#ifdef __WXMSW__
    Associations::Set();
#endif
    wxMessageBox(_("Code::Blocks associated with C/C++ files."), _("Information"), wxICON_INFORMATION);
}

void EnvironmentSettingsDlg::OnChooseColor(wxCommandEvent& event)
{
	wxColourData data;
	wxWindow* sender = FindWindowById(event.GetId());
    data.SetColour(sender->GetBackgroundColour());

	wxColourDialog dlg(this, &data);
    if (dlg.ShowModal() == wxID_OK)
    {
    	wxColour color = dlg.GetColourData().GetColour();
	    sender->SetBackgroundColour(color);
    }
}

void EnvironmentSettingsDlg::OnUpdateUI(wxUpdateUIEvent& event)
{
    bool en = XRCCTRL(*this, "cmbEditorTabs", wxComboBox)->GetSelection() == 1;
    XRCCTRL(*this, "btnFNBorder", wxButton)->Enable(en);
    XRCCTRL(*this, "btnFNFrom", wxButton)->Enable(en);
    XRCCTRL(*this, "btnFNTo", wxButton)->Enable(en);
}

void EnvironmentSettingsDlg::EndModal(int retCode)
{
    if (retCode == wxID_OK)
    {
        ConfigManager *cfg = Manager::Get()->GetConfigManager(_T("app"));
        ConfigManager *pcfg = Manager::Get()->GetConfigManager(_T("project_manager"));
        ConfigManager *mcfg = Manager::Get()->GetConfigManager(_T("message_manager"));
        ConfigManager *acfg = Manager::Get()->GetConfigManager(_T("an_dlg"));

        // tab "General"
        cfg->Write(_T("/environment/show_splash"),           (bool) XRCCTRL(*this, "chkShowSplash", wxCheckBox)->GetValue());
        cfg->Write(_T("/environment/use_dde"),               (bool) XRCCTRL(*this, "chkDDE", wxCheckBox)->GetValue());
        cfg->Write(_T("/environment/single_instance"),       (bool) XRCCTRL(*this, "chkSingleInstance", wxCheckBox)->GetValue());
        cfg->Write(_T("/environment/check_associations"),    (bool) XRCCTRL(*this, "chkAssociations", wxCheckBox)->GetValue());
        cfg->Write(_T("/environment/check_modified_files"),  (bool) XRCCTRL(*this, "chkModifiedFiles", wxCheckBox)->GetValue());
        mcfg->Write(_T("/has_debug_log"),                    (bool) XRCCTRL(*this, "chkDebugLog", wxCheckBox)->GetValue());

        // tab "View"
        cfg->Write(_T("/environment/blank_workspace"),       (bool) XRCCTRL(*this, "rbAppStart", wxRadioBox)->GetSelection() ? true : false);
        pcfg->Write(_T("/open_files"),                       (int)  XRCCTRL(*this, "rbProjectOpen", wxRadioBox)->GetSelection());
        cfg->Write(_T("/environment/toolbar_size"),          (bool) XRCCTRL(*this, "rbToolbarSize", wxRadioBox)->GetSelection() == 1);
        mcfg->Write(_T("/auto_hide"),                        (bool) XRCCTRL(*this, "chkAutoHideMessages", wxCheckBox)->GetValue());
        cfg->Write(_T("/environment/start_here_page"),       (bool) XRCCTRL(*this, "chkShowStartPage", wxCheckBox)->GetValue());

        // tab "Appearence"
        cfg->Write(_T("/environment/editor_tabs_style"),        (int)XRCCTRL(*this, "cmbEditorTabs", wxComboBox)->GetSelection());
        cfg->Write(_T("/environment/editor_gradient_border"),   XRCCTRL(*this, "btnFNBorder", wxButton)->GetBackgroundColour());
        cfg->Write(_T("/environment/editor_gradient_from"),     XRCCTRL(*this, "btnFNFrom", wxButton)->GetBackgroundColour());
        cfg->Write(_T("/environment/editor_gradient_to"),       XRCCTRL(*this, "btnFNTo", wxButton)->GetBackgroundColour());
        cfg->Write(_T("/environment/aui/border_size"),          (int)XRCCTRL(*this, "spnAuiBorder", wxSpinCtrl)->GetValue());
        cfg->Write(_T("/environment/aui/sash_size"),            (int)XRCCTRL(*this, "spnAuiSash", wxSpinCtrl)->GetValue());
        cfg->Write(_T("/environment/aui/caption_size"),         (int)XRCCTRL(*this, "spnAuiCaption", wxSpinCtrl)->GetValue());
        cfg->Write(_T("/environment/aui/bg_color"),             XRCCTRL(*this, "btnAuiBgColor", wxButton)->GetBackgroundColour());
        cfg->Write(_T("/environment/aui/sash_color"),           XRCCTRL(*this, "btnAuiSashColor", wxButton)->GetBackgroundColour());
        cfg->Write(_T("/environment/aui/caption_color"),        XRCCTRL(*this, "btnAuiCaptionColor", wxButton)->GetBackgroundColour());
        cfg->Write(_T("/environment/aui/caption_text_color"),   XRCCTRL(*this, "btnAuiCaptionTextColor", wxButton)->GetBackgroundColour());
        cfg->Write(_T("/environment/aui/border_color"),         XRCCTRL(*this, "btnAuiBorderColor", wxButton)->GetBackgroundColour());
        cfg->Write(_T("/environment/aui/gripper_color"),        XRCCTRL(*this, "btnAuiGripperColor", wxButton)->GetBackgroundColour());
        m_pArt->SetMetric(wxAUI_ART_PANE_BORDER_SIZE,   XRCCTRL(*this, "spnAuiBorder", wxSpinCtrl)->GetValue());
        m_pArt->SetMetric(wxAUI_ART_SASH_SIZE,          XRCCTRL(*this, "spnAuiSash", wxSpinCtrl)->GetValue());
        m_pArt->SetMetric(wxAUI_ART_CAPTION_SIZE,       XRCCTRL(*this, "spnAuiCaption", wxSpinCtrl)->GetValue());
        m_pArt->SetColor(wxAUI_ART_BACKGROUND_COLOUR,   XRCCTRL(*this, "btnAuiBgColor", wxButton)->GetBackgroundColour());
        m_pArt->SetColor(wxAUI_ART_SASH_COLOUR,         XRCCTRL(*this, "btnAuiSashColor", wxButton)->GetBackgroundColour());
        m_pArt->SetColor(wxAUI_ART_CAPTION_COLOUR,      XRCCTRL(*this, "btnAuiCaptionColor", wxButton)->GetBackgroundColour());
        m_pArt->SetColor(wxAUI_ART_CAPTION_TEXT_COLOUR, XRCCTRL(*this, "btnAuiCaptionTextColor", wxButton)->GetBackgroundColour());
        m_pArt->SetColor(wxAUI_ART_BORDER_COLOUR,       XRCCTRL(*this, "btnAuiBorderColor", wxButton)->GetBackgroundColour());
        m_pArt->SetColor(wxAUI_ART_GRIPPER_COLOUR,      XRCCTRL(*this, "btnAuiGripperColor", wxButton)->GetBackgroundColour());

        // tab "Dialogs"
        wxCheckListBox* lb = XRCCTRL(*this, "chkDialogs", wxCheckListBox);
        for (int i = 0; i < lb->GetCount(); ++i)
        {
            if (lb->IsChecked(i))
                acfg->UnSet(lb->GetString(i));
        }

        // tab "Batch builds"
#ifdef __WXMSW__
        wxString bbargs = XRCCTRL(*this, "txtBatchBuildsCmdLine", wxTextCtrl)->GetValue();
        if (bbargs != cfg->Read(_T("/batch_build_args"), DEFAULT_BATCH_BUILD_ARGS))
        {
            cfg->Write(_T("/batch_build_args"), bbargs);
            Associations::SetBatchBuildOnly();
        }
#endif

        // tab "Network"
        cfg->Write(_T("/network_proxy"),    XRCCTRL(*this, "txtProxy", wxTextCtrl)->GetValue());

        // tab "Tweaks"
        mcfg->Write(_T("/safe_but_slow"),   (bool) XRCCTRL(*this, "chkSafebutSlow", wxCheckBox)->GetValue());
    }

    wxDialog::EndModal(retCode);
}
