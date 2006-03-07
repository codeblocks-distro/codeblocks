#ifndef EXAMINEMEMORYDLG_H
#define EXAMINEMEMORYDLG_H

#include <wx/panel.h>

class DebuggerGDB;
class wxTextCtrl;

class ExamineMemoryDlg : public wxPanel
{
	public:
		ExamineMemoryDlg(wxWindow* parent, DebuggerGDB* debugger);
		virtual ~ExamineMemoryDlg();

        // used for Freeze()/Thaw() calls
        void Begin();
        void End();

        void Clear();
        wxString GetBaseAddress();
        int GetBytes();
        void AddError(const wxString& err);
		void AddHexByte(const wxString& addr, const wxString& hexbyte);
	protected:
        void OnGo(wxCommandEvent& event);
        DebuggerGDB* m_pDbg;
        wxTextCtrl* m_pText;
        size_t m_ByteCounter;
        wxChar m_LineText[67]; // 16*3 "7F " + 3 "   " + 16 "."
	private:
        DECLARE_EVENT_TABLE()
};

#endif // EXAMINEMEMORYDLG_H
