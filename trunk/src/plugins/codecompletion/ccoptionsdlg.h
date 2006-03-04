#ifndef CCOPTIONSDLG_H
#define CCOPTIONSDLG_H

#include <cbplugin.h>
#include <settings.h>
#include "nativeparser.h"
#include "parser/parser.h"

#ifdef __WXMSW__
    #define USE_CUST_CTRL true
#else
    #define USE_CUST_CTRL false
#endif

class CCOptionsDlg : public cbConfigurationPanel
{
	public:
		CCOptionsDlg(wxWindow* parent, NativeParser* np);
		virtual ~CCOptionsDlg();

        virtual wxString GetTitle(){ return _("Code-completion and class-browser"); }
        virtual wxString GetBitmapBaseName(){ return _T("generic-plugin"); }
        virtual void OnApply();
        virtual void OnCancel(){}
	protected:
		void OnOK(wxCommandEvent& event);
		void OnChooseColor(wxCommandEvent& event);
		void OnInheritanceToggle(wxCommandEvent& event);
		void OnSliderScroll(wxScrollEvent& event);
		void OnUpdateUI(wxUpdateUIEvent& event);
	private:
		void UpdateSliderLabel();
		Parser m_Parser;
		NativeParser* m_pNativeParsers;
		DECLARE_EVENT_TABLE()
};

#endif // CCOPTIONSDLG_H
