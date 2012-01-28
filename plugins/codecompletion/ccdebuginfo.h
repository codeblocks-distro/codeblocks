/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef CCDEBUGINFO_H
#define CCDEBUGINFO_H

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

//(*Headers(CCDebugInfo)
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/listbox.h>
#include <wx/statline.h>
#include <wx/panel.h>
#include <wx/button.h>
#include "scrollingdialog.h"
#include <wx/combobox.h>
//*)

class ParserBase;
class Token;

// When instance this class, *MUST* entered a critical section, and should call ShowModal() rather than the other
// e.g. wxCriticalSectionLocker locker(s_TokensTreeCritical);
class CCDebugInfo: public wxScrollingDialog
{
public:
    CCDebugInfo(wxWindow* parent, ParserBase* parser, Token* token);
    virtual ~CCDebugInfo();

    void FillFiles();
    void FillDirs();
    void DisplayTokenInfo();
    void FillChildren();
    void FillAncestors();
    void FillDescendants();

    //(*Identifiers(CCDebugInfo)
    static const long ID_STATICTEXT29;
    static const long ID_TEXTCTRL1;
    static const long ID_BUTTON1;
    static const long ID_STATICLINE1;
    static const long ID_STATICTEXT18;
    static const long ID_STATICTEXT2;
    static const long ID_STATICTEXT10;
    static const long ID_STATICTEXT12;
    static const long ID_STATICTEXT4;
    static const long ID_STATICTEXT6;
    static const long ID_STATICTEXT8;
    static const long ID_STATICTEXT37;
    static const long ID_STATICTEXT41;
    static const long ID_STATICTEXT14;
    static const long ID_STATICTEXT16;
    static const long ID_STATICTEXT33;
    static const long ID_STATICTEXT39;
    static const long ID_STATICTEXT20;
    static const long ID_STATICTEXT24;
    static const long ID_BUTTON4;
    static const long ID_COMBOBOX3;
    static const long ID_BUTTON5;
    static const long ID_COMBOBOX2;
    static const long ID_BUTTON3;
    static const long ID_COMBOBOX1;
    static const long ID_BUTTON2;
    static const long ID_STATICTEXT26;
    static const long ID_STATICTEXT28;
    static const long ID_STATICTEXT35;
    static const long ID_PANEL1;
    static const long ID_LISTBOX1;
    static const long ID_PANEL2;
    static const long ID_LISTBOX2;
    static const long ID_PANEL3;
    static const long ID_NOTEBOOK1;
    static const long ID_STATICTEXT31;
    static const long ID_BUTTON6;
    static const long ID_STATICLINE2;
    //*)

protected:
    //(*Handlers(CCDebugInfo)
    void OnInit(wxInitDialogEvent& event);
    void OnFindClick(wxCommandEvent& event);
    void OnGoAscClick(wxCommandEvent& event);
    void OnGoDescClick(wxCommandEvent& event);
    void OnGoParentClick(wxCommandEvent& event);
    void OnGoChildrenClick(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    //*)

    //(*Declarations(CCDebugInfo)
    wxBoxSizer* BoxSizer4;
    wxButton* btnGoChildren;
    wxBoxSizer* BoxSizer6;
    wxStaticText* txtTemplateArg;
    wxStaticText* txtArgsStripped;
    wxStaticText* txtFullType;
    wxStaticText* txtIsConst;
    wxBoxSizer* BoxSizer5;
    wxStaticText* StaticText29;
    wxNotebook* Notebook1;
    wxBoxSizer* BoxSizer7;
    wxStaticText* txtIsTemp;
    wxBoxSizer* BoxSizer8;
    wxButton* btnSave;
    wxButton* btnGoAsc;
    wxComboBox* cmbAncestors;
    wxButton* btnGoParent;
    wxPanel* Panel1;
    wxStaticText* txtImplFile;
    wxStaticText* txtScope;
    wxStaticText* txtID;
    wxStaticLine* StaticLine2;
    wxListBox* lstDirs;
    wxStaticText* txtIsOp;
    wxStaticText* lblInfo;
    wxButton* btnGoDesc;
    wxStaticText* txtDeclFile;
    wxStaticText* txtNamespace;
    wxStaticText* txtName;
    wxStaticLine* StaticLine1;
    wxStaticText* txtArgs;
    wxBoxSizer* BoxSizer9;
    wxStaticText* txtParent;
    wxPanel* Panel2;
    wxComboBox* cmbChildren;
    wxFlexGridSizer* FlexGridSizer1;
    wxComboBox* cmbDescendants;
    wxBoxSizer* BoxSizer3;
    wxStaticText* txtUserData;
    wxStaticText* txtBaseType;
    wxStaticText* txtKind;
    wxTextCtrl* txtFilter;
    wxListBox* lstFiles;
    wxButton* btnFind;
    wxStaticText* txtIsLocal;
    wxStdDialogButtonSizer* StdDialogButtonSizer1;
    //*)

private:
    ParserBase* m_Parser;
    Token*      m_Token;

    DECLARE_EVENT_TABLE()
};

#endif
