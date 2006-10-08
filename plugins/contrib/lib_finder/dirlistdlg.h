#ifndef DIRLISTDLG_H
#define DIRLISTDLG_H

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include <wx/arrstr.h>

// NOTE : once the new wxSmith is up and running these includes should be
// moved to the cpp, forward declarations is what we need here

//(*Headers(DirListDlg)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
//*)

class wxCommandEvent;

class DirListDlg: public wxDialog
{
	public:

		DirListDlg(wxWindow* parent,wxWindowID id = -1);
		virtual ~DirListDlg();

		//(*Identifiers(DirListDlg)
		enum Identifiers
		{
		    ID_BUTTON1 = 0x1000,
		    ID_BUTTON2,
		    ID_BUTTON3,
		    ID_BUTTON4,
		    ID_TEXTCTRL1
		};
		//*)

		wxArrayString Dirs;

	protected:

		//(*Handlers(DirListDlg)
		void OnButton1Click(wxCommandEvent& event);
		void OnButton2Click(wxCommandEvent& event);
		void OnButton3Click(wxCommandEvent& event);
		void OnButton4Click(wxCommandEvent& event);
		//*)

		//(*Declarations(DirListDlg)
		wxFlexGridSizer* FlexGridSizer1;
		wxTextCtrl* DirList;
		wxBoxSizer* BoxSizer1;
		wxBoxSizer* BoxSizer2;
		wxButton* Button3;
		wxButton* Button4;
		//*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
