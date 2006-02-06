#include <sdk.h>
#include "disassemblydlg.h"
#include "debuggergdb.h"
#include <wx/intl.h>
#include <wx/xrc/xmlres.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/listctrl.h>
#include <wx/wfstream.h>
#include <globals.h>
BEGIN_EVENT_TABLE(DisassemblyDlg, wxPanel)
    EVT_BUTTON(XRCID("btnSave"), DisassemblyDlg::OnSave)
//    EVT_BUTTON(XRCID("btnRefresh"), DisassemblyDlg::OnRefresh)
END_EVENT_TABLE()

DisassemblyDlg::DisassemblyDlg(wxWindow* parent, DebuggerGDB* debugger)
    : m_pDbg(debugger),
    m_LastActiveAddr(0)
{
	//ctor
	wxXmlResource::Get()->LoadPanel(this, parent, _T("dlgDisassembly"));
//	SetWindowStyle(GetWindowStyle() | wxFRAME_FLOAT_ON_PARENT);
	wxFont font(8, wxMODERN, wxNORMAL, wxNORMAL);
    XRCCTRL(*this, "lcCode", wxListCtrl)->SetFont(font);

    StackFrame sf;
    Clear(sf);
}

DisassemblyDlg::~DisassemblyDlg()
{
	//dtor
}

void DisassemblyDlg::Clear(const StackFrame& frame)
{
    XRCCTRL(*this, "lblFunction", wxStaticText)->SetLabel(frame.valid ? frame.function : _T("??"));
    wxString addr = _T("??");
    if (frame.valid)
        addr.Printf(_T("%p"), (void*)frame.address);
    XRCCTRL(*this, "lblAddress", wxStaticText)->SetLabel(addr);

    wxListCtrl* lc = XRCCTRL(*this, "lcCode", wxListCtrl);
    lc->ClearAll();
	lc->Freeze();
	lc->DeleteAllItems();
    lc->InsertColumn(0, _("Address"), wxLIST_FORMAT_LEFT);
    lc->InsertColumn(1, _("Instruction"), wxLIST_FORMAT_LEFT);
	lc->Thaw();
}

void DisassemblyDlg::AddAssemblerLine(unsigned long int addr, const wxString& line)
{
    wxListCtrl* lc = XRCCTRL(*this, "lcCode", wxListCtrl);
	lc->Freeze();
	wxString fmt;
	fmt.Printf(_T("0x%x"), (size_t)addr);
	lc->InsertItem(lc->GetItemCount(), fmt);
	int n = lc->GetItemCount() - 1;
    lc->SetItem(n, 1, line);
    lc->SetItemData(n, addr);
	lc->Thaw();

	for (int i = 0; i < 2; ++i)
	{
        lc->SetColumnWidth(i, wxLIST_AUTOSIZE);
	}

	SetActiveAddress(m_LastActiveAddr);
}

void DisassemblyDlg::SetActiveAddress(unsigned long int addr)
{
//    if (addr == m_LastActiveAddr)
//        return;
    m_LastActiveAddr = addr;
    wxListCtrl* lc = XRCCTRL(*this, "lcCode", wxListCtrl);
    for (int i = 0; i < lc->GetItemCount(); ++i)
    {
        if (lc->GetItemData(i) >= addr)
        {
            lc->SetItemState(i, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED);
            lc->EnsureVisible(i);
            break;
        }
    }
}

void DisassemblyDlg::OnSave(wxCommandEvent& event)
{
    wxFileDialog dlg(this,
                        _("Save as text file"),
                        wxEmptyString,
                        wxEmptyString,
                        ALL_FILES_FILTER,
                        wxSAVE | wxOVERWRITE_PROMPT);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() != wxID_OK)
        return;

    wxFFileOutputStream output(dlg.GetPath());
    wxTextOutputStream text(output);

    wxListCtrl* lc = XRCCTRL(*this, "lcCode", wxListCtrl);
    for (int i = 0; i < lc->GetItemCount(); ++i)
    {
        wxListItem info;
        info.m_itemId = i;
        info.m_col = 1;
        info.m_mask = wxLIST_MASK_TEXT;
        wxString instr = lc->GetItem(info) && !info.m_text.IsEmpty() ? info.m_text : _T("??");

        text << lc->GetItemText(i) << _T(": ") << instr << _T('\n');
    }

    if (XRCCTRL(*this, "txtCode", wxTextCtrl)->SaveFile(dlg.GetPath()))
        wxMessageBox(_("File saved"), _("Result"), wxICON_INFORMATION);
    else
        wxMessageBox(_("File could not be saved..."), _("Result"), wxICON_ERROR);
}

void DisassemblyDlg::OnRefresh(wxCommandEvent& event)
{
    m_pDbg->Disassemble();
}
