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
#include <wx/dialog.h>
#include <wx/combobox.h>
//*)

class Parser;
class Token;

class CCDebugInfo: public wxDialog
{
	public:

		CCDebugInfo(wxWindow* parent, Parser* parser, Token* token);
		virtual ~CCDebugInfo();

		Parser* m_pParser;
		Token* m_pToken;

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
		static const long ID_STATICTEXT17;
		static const long ID_STATICTEXT18;
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT9;
		static const long ID_STATICTEXT10;
		static const long ID_STATICTEXT11;
		static const long ID_STATICTEXT12;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXT6;
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT8;
		static const long ID_STATICTEXT13;
		static const long ID_STATICTEXT14;
		static const long ID_STATICTEXT15;
		static const long ID_STATICTEXT16;
		static const long ID_STATICTEXT32;
		static const long ID_STATICTEXT33;
		static const long ID_STATICTEXT19;
		static const long ID_STATICTEXT20;
		static const long ID_STATICTEXT22;
		static const long ID_STATICTEXT24;
		static const long ID_BUTTON4;
		static const long ID_STATICTEXT30;
		static const long ID_COMBOBOX3;
		static const long ID_BUTTON5;
		static const long ID_STATICTEXT21;
		static const long ID_COMBOBOX2;
		static const long ID_BUTTON3;
		static const long ID_STATICTEXT23;
		static const long ID_COMBOBOX1;
		static const long ID_BUTTON2;
		static const long ID_STATICTEXT25;
		static const long ID_STATICTEXT26;
		static const long ID_STATICTEXT27;
		static const long ID_STATICTEXT28;
		static const long ID_STATICTEXT34;
		static const long ID_STATICTEXT35;
		static const long ID_PANEL1;
		static const long ID_LISTBOX1;
		static const long ID_PANEL2;
		static const long ID_LISTBOX2;
		static const long ID_PANEL3;
		static const long ID_NOTEBOOK1;
		static const long ID_STATICTEXT31;
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
		//*)

		//(*Declarations(CCDebugInfo)
		wxBoxSizer* BoxSizer4;
		wxButton* btnGoChildren;
		wxStaticText* StaticText22;
		wxStaticText* StaticText9;
		wxBoxSizer* BoxSizer6;
		wxBoxSizer* BoxSizer5;
		wxStaticText* StaticText29;
		wxNotebook* Notebook1;
		wxBoxSizer* BoxSizer7;
		wxStaticText* txtIsTemp;
		wxBoxSizer* BoxSizer8;
		wxStaticText* StaticText13;
		wxStaticText* StaticText30;
		wxButton* btnGoAsc;
		wxStaticText* txtType;
		wxComboBox* cmbAncestors;
		wxStaticText* StaticText19;
		wxStaticText* StaticText32;
		wxStaticText* StaticText11;
		wxButton* btnGoParent;
		wxPanel* Panel1;
		wxStaticText* txtImplFile;
		wxStaticText* txtScope;
		wxStaticText* StaticText1;
		wxStaticText* StaticText27;
		wxStaticText* StaticText3;
		wxStaticText* txtID;
		wxStaticText* StaticText21;
		wxStaticLine* StaticLine2;
		wxStaticText* StaticText23;
		wxListBox* lstDirs;
		wxStaticText* txtIsOp;
		wxStaticText* lblInfo;
		wxStaticText* StaticText34;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxStaticText* txtActualType;
		wxButton* btnGoDesc;
		wxStaticText* txtDeclFile;
		wxStaticText* txtNamespace;
		wxStaticText* txtName;
		wxStaticLine* StaticLine1;
		wxStaticText* StaticText15;
		wxStaticText* txtArgs;
		wxBoxSizer* BoxSizer9;
		wxStaticText* txtParent;
		wxPanel* Panel2;
		wxComboBox* cmbChildren;
		wxFlexGridSizer* FlexGridSizer1;
		wxStaticText* StaticText25;
		wxComboBox* cmbDescendants;
		wxBoxSizer* BoxSizer3;
		wxStaticText* txtUserData;
		wxStaticText* StaticText17;
		wxStaticText* txtKind;
		wxTextCtrl* txtFilter;
		wxListBox* lstFiles;
		wxButton* btnFind;
		wxStaticText* txtIsLocal;
		wxStdDialogButtonSizer* StdDialogButtonSizer1;
		//*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
